//
// Created by 99694 on 2024/1/30.
//

#ifndef CLION_TASKQUEUE_H
#define CLION_TASKQUEUE_H
#include <queue>
#include <mutex>
#include <functional>
typedef std::function<void(void *)> Callback;
template<class T>
class Task{
public:
    Callback function;
    T *arg;
    Task()
    {
        function= nullptr;
        arg= nullptr;
    }
    Task(Callback function_,void *args)
    {
        this->function=function_;
        this->arg=(T *)args;
    }
};
template<class T>
class TaskQueue{
private:
    std::queue<Task<T>>Queue;
    std::mutex mtx;
public:
    void addTask(const Task<T> &t);
    int taskNumber();
    Task<T> takeTask();
};
#endif //CLION_TASKQUEUE_H
