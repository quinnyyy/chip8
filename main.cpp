#include <iostream>
#include "chip8.hpp"
using namespace::std;

int main() {
    cout << "hello from main" << endl;
    chip8 test;
    test.initialize();
    test.test();
    //test.emulateCycle();
}
