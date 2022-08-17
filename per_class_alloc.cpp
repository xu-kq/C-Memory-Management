#include<iostream>
using namespace std;

class Screen {
public:
    Screen(int x) : i(x) { };
    int get() { return i; }
    void* operator new(size_t);
    void  operator delete(void*, size_t);

private:
    Screen* next;
    static Screen* freeStore;
    static int chunkSize;
private:
    int i;
};
Screen* Screen::freeStore = nullptr;
int Screen::chunkSize = 24;

void* Screen::operator new(size_t size){
    Screen* p;
    if(!freeStore) {
        size_t chunk = size * chunkSize;
        // apply for a long series of memory;
        // once initializer a small chunk of size 24.
        freeStore = p = reinterpret_cast<Screen *>(new char[chunk]);
        while(p != &freeStore[chunkSize - 1]) {  // the last one indicates a nullptr
            p->next = p+1;
            p++;
        }
        p->next = nullptr;
    }
    p = freeStore;
    freeStore = freeStore->next;
    return p;
}

void Screen::operator delete(void* p, size_t t) {
    (static_cast<Screen *>(p))->next = freeStore;
    freeStore = static_cast<Screen *>(p);
}

int main(){
    using std::cout;
    using std::endl;

    // 64-bits system : 8bytes of per Screen obj.
    cout << sizeof(Screen) << endl;
    
    size_t const N = 100;
    Screen* p[N];

    for(int i = 0; i < N; ++i){
        p[i] = new Screen(i);
    }


    // with overload operator new, 16 bytes distance;
    // with global operator new, 32 byetes.
    for(int i = 0; i < 10; ++i) {
        cout << p[i] << endl;
    }

    for(int i = 0; i < N; ++i) {
        delete p[i];
    }

    return 0; 
}