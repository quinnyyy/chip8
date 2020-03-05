#ifndef chip8_hpp
#define chip8_hpp

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace::std;

// opcodes are 2 bytes
typedef unsigned short opcode;


// registers are 8 bits
typedef unsigned char reg;

// address space is 0x000 - 0xFFF. Thus we need 2 bytes to represent an address
// we will use this for index and pc registers
typedef unsigned short address;


typedef unsigned char byte;

/*
    The chip 8 has 35 opcodes which are all 2 bytes long.

    The chip 8 has 4K memory in total

    The chip 8 has 15 8-bit general purpose registers named V0, V1, ... VE
    The 16th register is used for the carry flag

    There is an Index register I and a program counter (pc) which can have
    a value from 0x000 to 0xFFF
    
    The systems memory map:
    0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
    0x050-0x0A0 - Used for the built in 4 x 5 pixel font set (0-F)
    0x200-0xFFF - Program ROM and work RAM

    The graphics system:
    The chip 8 has one instruction that draws a sprite to the screen.
    Drawing is done in XOR mode and if a pixel is turned off as a
    result of drawing, the VF register is set. This is used for collision
    detection.

    The graphics of the chip 8 are black and white and the screen is 64 x 32.

    The chip 8 does not have any interrupts or hardware registers. There are
    two timer registers that count at 60 Hz. When set above zero they will
    count down to zero.

    Stack is used to remember current location before jump is performed.
    The system has 16 levels of stack.

    The chip 8 has a HEX based keypad (0x0 - 0xF) 
*/

class chip8 {
public:
    chip8();
    ~chip8();
    void initialize();
    void emulateCycle();

    void test();

private:
    opcode currentOpcode;
    byte X;
    byte Y;
    address NNN;
    byte NN;
    byte N;

    const static address ROM_START_ADDRESS = 0x200;
    const static address FONTSET_START_ADDRESS = 0x50;
    const static int ROM_LENGTH = 3584; // i think...
    const static int FONTSET_LENGTH = 80;
    const static byte fontset[FONTSET_LENGTH];

    const static int MEMORY_SIZE = 4096;
    byte memory[MEMORY_SIZE];

    const static int NUM_REGISTERS = 16;
    reg V[NUM_REGISTERS];

    address I;
    address pc;

    const static int MAX_STACK_DEPTH = 16;
    byte stack[MAX_STACK_DEPTH];
    reg sp;

    byte key[16];

    void loadRom(const char *fileName);
    void decodeOpcode(opcode op);
    void setOpcodeFields(opcode op);

    void execute00E0(opcode op);
    void execute00EE(opcode op);
    void execute1NNN(opcode op);
    void execute2NNN(opcode op);
    void execute3XNN(opcode op);
    void execute4XNN(opcode op);
    void execute5XY0(opcode op);
    void execute6XNN(opcode op);
    void execute7XNN(opcode op);
    void execute8XY0(opcode op);
    void execute8XY1(opcode op);
    void execute8XY2(opcode op);
    void execute8XY3(opcode op);
    void execute8XY4(opcode op);
    void execute8XY5(opcode op);
    void execute8XY6(opcode op);
    void execute8XY7(opcode op);
    void execute8XYE(opcode op);
    void execute9XY0(opcode op);
    void executeANNN(opcode op);
    void executeBNNN(opcode op);
    void executeCXNN(opcode op);
    void executeDXYN(opcode op);
    void executeEX9E(opcode op);
    void executeEXA1(opcode op);
    void executeFX07(opcode op);
    void executeFX0A(opcode op);
    void executeFX15(opcode op);
    void executeFX18(opcode op);
    void executeFX1E(opcode op);
    void executeFX29(opcode op);
    void executeFX33(opcode op);
    void executeFX55(opcode op);
    void executeFX65(opcode op);
};

#endif
