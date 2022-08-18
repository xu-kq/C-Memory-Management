#include<iostream>
#include<string>
#include "static_alloc.cpp"
using std::endl;
using std::cout;
using std::string;

class Foo{
public:
    long L;
    // string str;
    static Allocator myAlloc;
public:
    Foo(long l) : L(l) { }
    static void* operator new(size_t size) {
        return myAlloc.allocate(size);
    }
    static void operator delete(void* p, size_t size) {
        myAlloc.deallocate(p, size);
    }
};
Allocator Foo::myAlloc;

int main(){
    cout << sizeof(Foo) << endl;

    size_t const N = 100;
    Foo* p[N];
    for(int i = 0; i < N; ++i){
        p[i] = new Foo(i);
    }

    for(int i = 0; i < N; ++i) {
        cout << p[i] << endl;
    }

    for(int i = 0; i < N; ++i){
        delete p[i];
    }
}