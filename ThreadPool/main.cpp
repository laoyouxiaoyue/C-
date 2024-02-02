#include <iostream>
#include "ThreadPool.h"
#include "TaskQueue.h"
#include "TaskQueue.cpp"
#include "ThreadPool.cpp"
#include <bits/stdc++.h>
using namespace std;
void task(void *arg)
{
    int num=*(int *)arg;
    cout << "corrent id is: " << this_thread::get_id() << " :" << num << endl;
    this_thread::sleep_for(chrono::seconds(1));
}
int main() {


    ThreadPool<int> threadPool(2,10);
    for(int i=0;i<100;i++)
    {
        int *num=new int(i+100);
        threadPool.addTask(Task<int>(task,num));
    }
    std::this_thread::sleep_for(chrono::seconds(20));
    for (int i = 0; i < 20; ++i) {
        int* num = new int(i + 200);
        threadPool.addTask(Task<int>(task, num));
    }
    std::this_thread::sleep_for(chrono::seconds(20));
    cout << "end threadPool test" << endl;
    return 0;
}
