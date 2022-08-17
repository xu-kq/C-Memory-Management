#include<iostream>
class Airplane{
private:
    struct AirplaneRep{
        unsigned long miles;
        char type;
    };
private:
    union{ AirplaneRep rep; 
           Airplane* next;  // embedded ptr
    };
public:
    unsigned long getMiles() { return rep.miles; }
    char getType() { return rep.type; }
    void set(unsigned long m, char t) {
        rep.miles = m; rep.type = t;
    }
public:
    static void* operator new(size_t size);
    static void operator delete(void* deadObject, size_t size);
private:
    static const int BLOCK_SIZE;
    static Airplane* headOfFreeList;
};
Airplane* Airplane::headOfFreeList = nullptr;
const int Airplane::BLOCK_SIZE = 512;

void* Airplane::operator new(size_t size){
    // Error when inheritance, turn to global::op new.
    if(size != sizeof(Airplane))
        return ::operator new(size);

    Airplane* p = headOfFreeList;
    if(p)
        headOfFreeList = p->next;
    else{
        // free list empty, apply a chunk
        Airplane* newBlock = static_cast<Airplane*>
        (::operator new(BLOCK_SIZE * sizeof(Airplane)));

        // link into a list
        for(int i = 1; i < BLOCK_SIZE - 1; ++i)
            newBlock[i].next = &newBlock[i+1];
        newBlock[BLOCK_SIZE - 1].next = 0;
        p = newBlock;
        headOfFreeList = &newBlock[1];
    }
    return p;
}

// not give back the memory to system
// it is not good.
// give back is difficult, considering it in the future ver.

void Airplane::operator delete(void* deadObject, size_t size) {
    if(deadObject == nullptr)   return;
    if(size != sizeof(Airplane)) {
        ::operator delete(deadObject);
        return;
    }

    Airplane* carcass =
        static_cast<Airplane*>(deadObject);
    
    carcass->next = headOfFreeList;
    headOfFreeList = carcass;
}
int main(){
    using std::cout;
    using std::endl;
    
    cout << sizeof(Airplane) << endl;
    size_t const N = 100;
    Airplane* p[N];

    for(int i = 0; i< N; ++i){
        p[i] = new Airplane;
    }

    p[1]->set(100,'A');
    p[5]->set(2000,'B');
    p[9]->set(500000,'C');

    for(int i = 0; i < 10; ++i){
        cout << p[i] << endl;
    }

    for(int i = 0; i < N; ++i){
        delete p[i];
    }
    return 0;
}