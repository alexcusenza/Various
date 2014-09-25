#ifndef _H_THREADPOOL
#define _H_THREADPOOL

#include <pthread.h>
#include <deque>
#include <iostream>
#include <vector>

using namespace std;

const int DEFAULT_POOL_SIZE = 10;
const int STARTED = 0;
const int STOPPED = 1;


class Mutex
{
public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();
    pthread_mutex_t * get_mutex_ptr();

private:
    pthread_mutex_t mt_lock;
    volatile bool is_locked;
};

class CondVar
{
public:
    CondVar();
    ~CondVar();

    void wait(pthread_mutex_t * );
    void signal();
    void broadcast();
private:
    pthread_cond_t mt_condvar;
};

//template<class TClass>
class Task
{
public:
    // Task(TCLass::* obj_fn_ptr); // pass an object method pointer
    Task(void (*fn_ptr)(void*), void* arg); // pass a free function pointer
    ~Task();
    void operator()();
    void run();
private:
    // TClass* _obj_fn_ptr;
    void (* m_fn_ptr)(void*);
    void * m_arg;
};

class ThreadPool
{
public:
    ThreadPool();
    ThreadPool(int);
    ~ThreadPool();
    int initialize_threadpool();
    int destroy_threadpool();
    void * execute_thread();
    int add_task(Task * );
private:
    int m_poolsize;
    Mutex mc_mutex;
    CondVar mc_condvar;
    std::vector<pthread_t> mt_threads; // storage for threads
    std::deque<Task*> mt_tasks;
    volatile int m_poolstate;
};

#endif /* _H_THREADPOOL */
