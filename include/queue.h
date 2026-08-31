#pragma once
#include "song.h"

class Queue{
    private:
        Song* queue;
        int n;       // Sức chứa tối đa
        int front;   // Vị trí bài hát đầu hàng
        int rear;    // Vị trí bài hát cuối hàng
        int size;    // Số bài hát hiện tại
    public:
        Queue();
        Queue(int n, int front, int rear, int size);
        ~Queue();
        void enqueue(const Song& song); // Thêm bài hát vào cuối hàng chờ
        void dequeue();  // Xóa bài hát ở đầu hàng chờ
        Song* getFront() const;   
        bool empty() const;
        bool full() const;
        int getSize() const;
};