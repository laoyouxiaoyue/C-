//
// Created by 99694 on 2024/1/30.
//

#include "TaskQueue.h"
#include<mutex>
template<class T>
void TaskQueue<T>::addTask(const Task<T> &t) {
    std::lock_guard<std::mutex> lock(mtx);
    Queue.push(t);
}
template<class T>
int TaskQueue<T>::taskNumber() {
    std::lock_guard<std::mutex> lock(mtx);
    return Queue.size();
}
template<class T>
Task<T> TaskQueue<T>::takeTask() {
    std::lock_guard<std::mutex> lock(mtx);
    Task<T> task;
    if(!Queue.empty())
    {
        task=Queue.front();
        Queue.pop();
    }
    return task;
}
