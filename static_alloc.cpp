#pragma once
#include<iostream>
class Allocator{
private:
    struct obj{
        obj* next;
    };
public:
    void* allocate(size_t);
    void deallocate(void*, size_t);
private:
    obj* freeStore = nullptr;
    const int CHUNK = 5;
};

void* Allocator::allocate(size_t size){
    obj* p;
    if(!freeStore) {
        size_t chunk = size * CHUNK;
        p = freeStore = static_cast<obj*>(malloc(chunk));
        for(int i = 0; i < CHUNK -1 ; ++i){
            p->next = p + 1;
            p++;
        }
        p->next = nullptr;
    }
    p = freeStore;
    freeStore = freeStore->next;
    return p;
}

void Allocator::deallocate(void* p, size_t size){
    (static_cast<obj*>(p))->next = freeStore;
    freeStore = static_cast<obj*>(p);
}