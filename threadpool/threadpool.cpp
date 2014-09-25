#include "threadpool.h"

#include <errno.h>
#include <string.h>
#include <pthread.h>

// +------------------------------------+
//  CLASS MUTEX
// +------------------------------------+

Mutex::Mutex()
{
    is_locked = false;
    pthread_mutex_init(&mt_lock, NULL);
}

Mutex::~Mutex()
{
    if (is_locked)
    {
        unlock();                // FIXME: is this correct? Can a thread unlock a mutex locked by another thread?
    }
    pthread_mutex_destroy(&mt_lock);
}

void Mutex::lock()
{
    is_locked = true;
    pthread_mutex_lock(&mt_lock);
}

void Mutex::unlock()
{
    is_locked = false;                // do it BEFORE unlocking to avoid race condition
    pthread_mutex_unlock(&mt_lock);
}

pthread_mutex_t * Mutex::get_mutex_ptr()
{
    return &mt_lock;
}

// +------------------------------------+
//  CLASS CONDVAR
// +------------------------------------+

CondVar::CondVar()
{
    pthread_cond_init(&mt_condvar, NULL);
}

CondVar::~CondVar()
{
    pthread_cond_destroy(&mt_condvar);
}

void CondVar::wait(
    pthread_mutex_t * mutex)
{
    pthread_cond_wait(&mt_condvar, mutex);
}

void CondVar::signal()
{
    pthread_cond_signal(&mt_condvar);
}

void CondVar::broadcast()
{
    pthread_cond_broadcast(&mt_condvar);
}

// +------------------------------------+
//  CLASS TASK
// +------------------------------------+
Task::Task( void (*fn_ptr)(void *),
    void * arg)
:m_fn_ptr(fn_ptr), m_arg(arg)
{

}

Task::~Task()
{
}

void Task::operator()()
{
    (*m_fn_ptr)(m_arg);
}

void Task::run()
{
    (*m_fn_ptr)(m_arg);
}

// +------------------------------------+
//  CLASS THREADPOOL
// +------------------------------------+

ThreadPool::ThreadPool()
: m_poolsize(DEFAULT_POOL_SIZE)
{
    cout << "Constructed DEFAULT POOL SIZE " << m_poolsize << endl;
}

ThreadPool::ThreadPool( int pool_size)
: m_poolsize(pool_size)
{
    cout << "Constructed ThreadPool of size " << m_poolsize << endl;
}

ThreadPool::~ThreadPool(
    void)
{
    // Release resources
    if (m_poolstate != STOPPED)
    {
        destroy_threadpool();
    }
}

// We can't pass a member function to pthread_create.
// So created the wrapper function that calls the member function
// we want to run in the thread.
//extern "C"
static void * start_thread(
    void * arg)
{
    ThreadPool * tp = (ThreadPool *) arg;
    tp->execute_thread();
    return NULL;
}

int ThreadPool::initialize_threadpool()
{
    // TODO: Consider lazy loading threads instead of creating all at once
    m_poolstate = STARTED;
    int ret = -1;
    for (int i = 0; i < m_poolsize; i++)
    {
        pthread_t tid;
        ret = pthread_create(&tid, NULL, start_thread, (void*) this);
        if (ret != 0)
        {
            cerr << "pthread_create() failed: " << ret << endl;
            return -1;
        }
        mt_threads.push_back(tid);
    }
    cout << m_poolsize << " threads created by the thread pool" << endl;

    return 0;
}

int ThreadPool::destroy_threadpool()
{
    /*Note: this is not for synchronization, its for thread communication!
     * destroy_threadpool() will only be called from the main thread, yet
     * the modified m_poolstate may not show up to other threads until its
     * modified in a lock!
     */
    mc_mutex.lock();
    m_poolstate = STOPPED;
    mc_mutex.unlock();
    cout << "Broadcasting STOP signal to all threads..." << endl;
    mc_condvar.broadcast();                // notify all threads we are shutting down

    int ret = -1;
    for (int i = 0; i < m_poolsize; i++)
    {
        void * result;
        ret = pthread_join(mt_threads[i], &result);
        cout << "pthread_join() returned " << ret << ": " << strerror(errno)
                                                                                                                                        << endl;
        mc_condvar.broadcast();                // try waking up a bunch of threads that are still waiting
    }
    cout << m_poolsize << " threads exited from the thread pool" << endl;
    return 0;
}

void * ThreadPool::execute_thread()
{
    Task * task = NULL;
    cout << pthread_self() << " Thread Starting " << endl;
    while (true)
    {
        cout << pthread_self() << " Thread Locking " << endl;
        mc_mutex.lock();

        /* We need to put pthread_cond_wait in a loop for two reasons:
         * 1. There can be spurious wakeups (due to signal/ENITR)
         * 2. When mutex is released for waiting, another thread can be waken up
         *    from a signal/broadcast and that thread can mess up the condition.
         *    So when the current thread wakes up the condition may no longer be
         *    actually true!
         */
        while ((m_poolstate != STOPPED) && (mt_tasks.empty()))
        {
            // Wait until there is a task in the queue
            // Unlock mutex while wait, then lock it back when signaled
            cout << pthread_self() << " Waiting and Unlocking " << endl;
            mc_condvar.wait(mc_mutex.get_mutex_ptr());
            cout << pthread_self() << " Signaled and Locking " << endl;
        }

        // If the thread was woken up to notify process shutdown, return from here
        if (m_poolstate == STOPPED)
        {
            cout << pthread_self() << " Unlocking and exiting: " << endl;
            mc_mutex.unlock();
            pthread_exit(NULL);
        }

        task = mt_tasks.front();
        mt_tasks.pop_front();

        cout << pthread_self() << " Thread Unlocking " << endl;
        mc_mutex.unlock();

        // execute the task
        (*task)();                // could also do task->run(arg);

    }
    return NULL;
}

int ThreadPool::add_task(
    Task * task)
{
    mc_mutex.lock();
    mt_tasks.push_back(task);
    mc_condvar.signal();                // wake up one thread that is waiting for a task to be available
    mc_mutex.unlock();

    return 0;
}
