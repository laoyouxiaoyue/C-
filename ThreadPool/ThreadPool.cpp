//
// Created by 99694 on 2024/1/30.
//

#include "ThreadPool.h"
#include<iostream>
#include<mutex>
#include<thread>
#include<atomic>
#include<vector>
using namespace std;
template<class T>
int ThreadPool<T>::getAliveNum() {
    return liveNum;
}
template<class T>
int ThreadPool<T>::getBusyNum() {
    return busyNum;
}
template<class T>
void * ThreadPool<T>::work(void *arg){
    cout << this_thread::get_id() << " worker thread starting..." << endl;
    ThreadPool *threadPool=static_cast<ThreadPool *>(arg);
    while(true)
    {
        unique_lock<mutex> poolLock(threadPool->mutexPool);
        while(!threadPool->taskQueue->taskNumber()&&!threadPool->isShutdown)
        {
            cout << this_thread::get_id() << " waiting task..." << endl;
            threadPool->notEmpty.wait(poolLock);
            if(threadPool->deleteNum>0&&threadPool->liveNum>threadPool->minNum)
            {
                threadPool->deleteNum--;
                threadPool->liveNum--;
                poolLock.unlock();
                return NULL;
            }
        }
        if(threadPool->isShutdown)
        {
            poolLock.unlock();
            return NULL;
        }
        Task task=threadPool->taskQueue->takeTask();
        threadPool->busyNum++;
        poolLock.unlock();
        task.function(task.arg);
        free(task.arg);
        threadPool->busyNum--;

    }

}
template<class T>
void * ThreadPool<T>::manager(void *arg) {
    auto *threadPool=static_cast<ThreadPool*>(arg);
    cout << this_thread::get_id() << " manager thread starting..." << endl;
    int checkInterval=3;
    while(!threadPool->isShutdown)
    {
        this_thread::sleep_for(chrono::seconds(checkInterval));
        unique_lock<mutex> poolLock(threadPool->mutexPool);
        int queueSize=threadPool->taskQueue->taskNumber();
        int noBusyNum=threadPool->liveNum-threadPool->busyNum;
        if(threadPool->busyNum<queueSize)
        {
            while(noBusyNum>0)
            {
                threadPool->notEmpty.notify_one();
                noBusyNum--;
            }
        }
        if(queueSize>threadPool->liveNum&&threadPool->liveNum<threadPool->maxNum)
        {
            for(int i=0;i<NUMBER&&threadPool->liveNum<threadPool->maxNum;i++)
            {
                threadPool->worker.push_back(thread(work,threadPool));
                threadPool->liveNum++;
            }
        }
        poolLock.unlock();
        if(threadPool->busyNum*2<threadPool->liveNum&&threadPool->liveNum>threadPool->minNum)
        {
            threadPool->deleteNum=NUMBER;
            for(int i=0;i<NUMBER;i++)
            {
                threadPool->notEmpty.notify_one();
            }
        }

        if(threadPool->worker.size()>threadPool->liveNum)
        {
            threadPool->thread_quit();
        }
    }
    return NULL;
}
template<class T>
void ThreadPool<T>::thread_quit() {

   auto x=worker.begin();
   for(;x!=worker.end();)
   {
       auto id=(*x).native_handle();
       int kill=pthread_kill(id,0);
       if(kill==ESRCH)
       {
           if((*x).joinable()) {
               (*x).join();
           }
           x=worker.erase(x);
       }
       else{
           x++;
       }
   }
}
template<class T>
void ThreadPool<T>::addTask(const Task<T> &task) {
    taskQueue->addTask(task);
}
template<class T>
ThreadPool<T>::ThreadPool(int minum, int maxum) {
    this->liveNum=minum;
    this->busyNum=0;
    this->minNum=minum;
    this->maxNum=maxum;
    this->deleteNum=0;
    taskQueue=new TaskQueue<T>;
    if(taskQueue== nullptr)
    {
        cout<<"创建任务队列失败.."<<'\n';
        return;
    }
    worker.reserve(maxNum);
    isShutdown=false;
    manager_therad=new thread(manager,this);
    if(manager_therad== nullptr)
    {
        cout<<"管理者线程创造失败\n";
        return;
    }
    for(int i=0;i<minNum;i++)
    {
        worker.push_back(thread(work,this));
    }
}
template<class T>
ThreadPool<T>::~ThreadPool()
{
    isShutdown=true;
    if(liveNum>0)
    {
        notEmpty.notify_all();
    }
    if(manager_therad!= nullptr)
    {
        if(manager_therad->joinable())
        {
            manager_therad->join();
        }
        delete manager_therad;
    }
    if(!worker.empty())
    {
        thread_quit();
    }
}