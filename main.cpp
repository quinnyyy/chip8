#include <iostream>
#include "chip8.hpp"
using namespace::std;

int main() {
    chip8 c8;
    c8.initialize("brix.ch8");

    while (true) {
        c8.emulateCycle();        
    }
}
