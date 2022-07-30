
int main () {
    Type func = main();
    Another_One::do_something();
}

namespace Another_One {
    template <int SIZE>
    struct Type {
        
    };    
    
    template <template<int a> class thing>
    internal int do_soemthing() {
        
    }


}



struct MyClass {
    void do_thing();
};


void MyClass::do_thing(int thing) {
    
}
