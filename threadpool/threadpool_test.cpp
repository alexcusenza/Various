#include "threadpool.h"

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include "clTimeSpec.h"
#include <time.h>

using namespace std;

const int MAX_TASKS = 10;
const int NUMTHREADS = 3;

void hello(void* arg)
{
    class clTimeSpec cTimeHello;

    int cnt;
    int * x = (int *) arg;

    cout << "Hello #" << *x <<" Started"<< endl;
    //cTimeHello.GetTimePrev();

    for (int i=0; i<1000000; i++)
        cnt++;

    cnt = cnt * *x;
    //cout << "Hello #" << *x <<" Finished "<<  endl;

    //cTimeHello.GetTimeCurr();
    //double fResult = cTimeHello.TimeDiffCombine();

    //cout << "!!! HELLO #" << *x << " Class DiffTime: " << fResult << endl;
}

int main(int argc, char* argv[])
{
    class ThreadPool tp(NUMTHREADS);
    class clTimeSpec cTimeMain;

    std::cout.precision(10);

    int ret = tp.initialize_threadpool();
    if (ret == -1)
    {
        cerr << "Failed to initialize thread pool!" << endl;
        return 0;
    }

    for (int i = 0; i < MAX_TASKS; i++)
    {
        int *x = new int();
        *x = i+1;

        Task * t = new Task(&hello, (void *) x);
        cout << "Task Initiated " << *x <<  endl;
        tp.add_task(t);
    }

    cTimeMain.GetTimePrev();
    sleep(1);
    usleep(75*1000);
    cTimeMain.GetTimeCurr();
    double fResult = cTimeMain.TimeDiffCombine();
    cout << "!!! MAIN GetClass DiffTime1: " << fResult << endl;

    tp.destroy_threadpool();

    // TODO: delete worker objects
    cout << "Exiting app..." << endl;

    return 0;
}
