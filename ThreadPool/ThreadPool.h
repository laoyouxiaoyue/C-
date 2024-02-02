//
// Created by 99694 on 2024/1/30.
//

#ifndef CLION_THREADPOOL_H
#define CLION_THREADPOOL_H
#include<mutex>
#include<atomic>
#include<thread>
#include<vector>
#include<condition_variable>
#include "TaskQueue.h"
template<class T>
class ThreadPool{
private:
    std::mutex mutexPool; //线程池锁
    std::condition_variable notEmpty;
    int maxNum;
    int minNum;
    std::atomic<int>busyNum;
    std::atomic<int>liveNum;
    std::atomic<int>deleteNum;
    TaskQueue<T> *taskQueue;
    std::vector<std::thread>worker;
    std::thread *manager_therad;
    const static int NUMBER = 2;//
    bool isShutdown; //工作状态 1工作 0销毁
public:
    ThreadPool(int minNum,int maxNum);
    ~ThreadPool();
    static void * work(void *arg);  //工作线程
    static void * manager(void *arg);  //管理线程
    void thread_quit();
    int getBusyNum();//获取工作线程数
    int getAliveNum();//获取存货线程数
    void addTask(const Task<T> &task);
};
#endif //CLION_THREADPOOL_H
