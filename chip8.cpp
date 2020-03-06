#include "chip8.hpp"

chip8::chip8() {
    cout << "Hello world" << endl;        
}


chip8::~chip8() {
    cout << "Bye world" << endl;
}


void chip8::initialize(const char *fileName) {
    // Initialize registers and memory
    pc = ROM_START_ADDRESS;
    I = 0;
    currentOpcode = 0;
    sp = 0;

    // Clear display
    memset(graphics, 0, ROWS * COLS * sizeof(boolean));

    // Clear stack
    memset(stack, 0, MAX_STACK_DEPTH * sizeof(byte));

    // Clear registers V0-VF
    memset(V, 0, NUM_REGISTERS * sizeof(reg));

    // Clear memory
    memset(memory, 0, MEMORY_SIZE * sizeof(byte));

    // Load fontset
    memcpy(memory + FONTSET_START_ADDRESS, fontset, FONTSET_LENGTH * sizeof(byte));

    // Set random seed
    srand(time(NULL));

    // Load ROM
    loadRom(fileName);

    // Reset timers
    delayTimer = 0;
    soundTimer = 0;
}


void chip8::emulateCycle() {
    // Fetch opcode
    currentOpcode = memory[pc] << 8 | memory[pc + 1];
    cout << "current opcode: " << hex << currentOpcode << endl;

    // Decode and execute opcode
    decodeOpcode(currentOpcode);

    // Update timers
    if (delayTimer > 0) {
        delayTimer--;
    }

    if (soundTimer > 0) {
        if (soundTimer == 1) {
            cout << "WOW!" << endl;
        }
        soundTimer--;
    }
}


void chip8::loadRom(const char *fileName) {
    // Error check later
    FILE *f;
    byte *buffer = (byte*)malloc(ROM_LENGTH);
    memset(buffer, 0, ROM_LENGTH*sizeof(byte));
    f = fopen(fileName,"r");
    fseek(f, 0, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(buffer, 1, fsize, f);
    fclose(f);

    for (int i = 0; i < ROM_LENGTH; i++) {
        memory[i + ROM_START_ADDRESS] = buffer[i];
    }
}


void chip8::decodeOpcode(opcode op) {
    setOpcodeFields(op);
    switch(op & 0xF000) {
    case 0x0000:
        //Ignoring 0NNN. Maybe will implement later if need it...
        switch(op & 0x000F) {
        case 0x0000:
            execute00E0(op);
        break;
        case 0x000E:
            execute00EE(op);
        break;
        default:
            cout << "Unrecognized opcode: " << hex << op << endl;
        }
    break;
    case 0x1000:
        execute1NNN(op);
    break;
    case 0x2000:
        execute2NNN(op);
    break;
    case 0x3000:
        execute3XNN(op);
    break;
    case 0x4000:
        execute4XNN(op);
    break;
    case 0x5000:
        execute5XY0(op);
    break;
    case 0x6000:
        execute6XNN(op);
    break;
    case 0x7000:
        execute7XNN(op);
    break;
    case 0x8000:
        switch(op & 0x000F) {
        case 0x0000:
            execute8XY0(op);
        break;
        case 0x0001:
            execute8XY1(op);
        break;
        case 0x0002:
            execute8XY2(op);
        break;
        case 0x0003:
            execute8XY3(op);
        break;
        case 0x0004:
            execute8XY4(op);
        break;
        case 0x0005:
            execute8XY5(op);
        break;
        case 0x0006:
            execute8XY6(op);
        break;
        case 0x0007:
            execute8XY7(op);
        break;
        case 0x000E:
            execute8XYE(op);
        break;
        default:
            cout << "Unrecognized opcode: " << hex << op << endl;
        }
    break;
    case 0x9000:
        execute9XY0(op);
    break;
    case 0xA000:
        executeANNN(op);
    break;
    case 0xB000:
        executeBNNN(op);
    break;
    case 0xC000:
        executeCXNN(op);
    break;
    case 0xD000:
        executeDXYN(op);
    break;
    case 0xE000:
        switch(op & 0x00FF) {
        case 0x009E:
            executeEX9E(op);
        break;
        case 0x00A1:
            executeEXA1(op);
        default:
            cout << "Unrecognized opcode: " << hex << op << endl;
        }
    break;
    case 0xF000:
        switch(op & 0x00FF) {
        case 0x0007:
            executeFX07(op);
        break;
        case 0x000A:
            executeFX0A(op);
        break;
        case 0x0015:
            executeFX15(op);
        break;
        case 0x0018:
            executeFX18(op);
        break;
        case 0x001E:
            executeFX1E(op);
        break;
        case 0x0029:
            executeFX29(op);
        break;
        case 0x0033:
            executeFX33(op);
        break;
        case 0x0055:
            executeFX55(op);
        break;
        case 0x0065:
            executeFX65(op);
        break;
        default:
            cout << "Unrecognized opcode: " << hex << op << endl;
        }
    break;
    default:
        cout << "Unrecognized opcode: " << hex << op << endl;
    }
}


void chip8::setOpcodeFields(opcode op) {
    X = (op >> 8) & 0x000F;
    Y = (op >> 4) & 0x000F;
    NNN = op & 0x0FFF;
    NN = op & 0x00FF;
    N = op & 0x000F;
}


void chip8::execute00E0(opcode op) {
    cout << "00E0" << endl;
    /* Clears the screen */
    
    pc += 2;
}


void chip8::execute00EE(opcode op) {
    cout << "00EE" << endl;
    /* Returns from the subroutine */
    sp--;
    pc = stack[sp];
}


void chip8::execute1NNN(opcode op) {
    cout << "1NNN" << endl;
    /* Jumps to address NNN */
    pc = op & 0x0FFF;
}


void chip8::execute2NNN(opcode op) {
    cout << "2NNN" << endl;
    /* Calls subroutine at NNN */
    stack[sp] = pc;
    sp++;
    pc = op & 0x0FFF;
}


void chip8::execute3XNN(opcode op) {
    cout << "3XNN" << endl;
    /* Skips the next instruction if VX equals NN. */
    
    if (V[X] == NN) {
        pc += 4;
    } else {
        pc += 2;
    }
}


void chip8::execute4XNN(opcode op) {
    cout << "4XNN" << endl;
    /* Skips the next instruction if VX doesn't equal NN */
    
    if (V[X] != NN) {
        pc += 4;
    } else {
        pc += 2;
    }
}


void chip8::execute5XY0(opcode op) {
    cout << "5XY0" << endl;
    /* Skips the next instruction if VX equals VY */

    if (V[X] == V[Y]) {
        pc += 4;
    } else {
        pc += 2;
    }
}


void chip8::execute6XNN(opcode op) {
    cout << "6XNN" << endl;
    /* Sets VX to NN */

    V[X] = NN;
    pc += 2;
}


void chip8::execute7XNN(opcode op) {
    cout << "7XNN" << endl;
    /* Adds NN to VX (Carry flag is not changed) */
    
    V[X] += NN;
    pc += 2;
}


void chip8::execute8XY0(opcode op) {
    cout << "8XY0" << endl;
    /* Sets VX to the value of VY */
    V[X] = V[Y];
    pc += 2;
}


void chip8::execute8XY1(opcode op) {
    cout << "8XY1" << endl;
    /* Sets VX to VX | VY */
    V[X] = V[X] | V[Y];
    pc += 2;
}


void chip8::execute8XY2(opcode op) {
    cout << "8XY2" << endl;
    /* Sets VX to VX & VY */
    V[X] = V[X] & V[Y];
    pc += 2;
}


void chip8::execute8XY3(opcode op) {
    cout << "8XY3" << endl;
    /* Sets VX to VX ^ VY */
    V[X] = V[X] ^ V[Y];
    pc += 2;
}


void chip8::execute8XY4(opcode op) {
    cout << "8XY4" << endl;
    /* Adds VY to VX. VF is set to 1 when there's a carry and 0 when there isn't */
    if (0xFF - V[X] < V[Y]) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }
    V[X] += V[Y];
    pc += 2;
}


void chip8::execute8XY5(opcode op) {
    cout << "8XY5" << endl;
    /* VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't */
    if (V[Y] > V[X]) {
        V[0xF] = 0;
    } else {
        V[0xF] = 1;
    }
    V[X] -= V[Y];
    pc += 2;
}


void chip8::execute8XY6(opcode op) {
    cout << "8XY6" << endl;
    /* Stores the least significant bit of VX in VF and then shifts VX to the right by 1 */
    V[0xF] = V[X] & 0x0001;
    V[X] >>= 1;
    pc += 2;
}


void chip8::execute8XY7(opcode op) {
    cout << "8XY7" << endl;
    /* Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't */
    if (V[X] > V[Y]) {
        V[0xF] = 0;
    } else {
        V[0xF] = 1;
    }
    V[X] = V[Y] - V[X];
    pc += 2;
}


void chip8::execute8XYE(opcode op) {
    cout << "8XYE" << endl;
    /* Stores the most significant bit of VX in VF and then shifts VX to the left by 1 */
    V[0xF] = (V[X] & 0x8000) >> 31;
    V[X] <<= 1;
    pc += 2;
}


void chip8::execute9XY0(opcode op) {
    cout << "9XY0" << endl;
    /* Skips the next instruction if VX doesn't equal VY */
    if (V[X] != V[Y]) {
        pc += 4;
    } else {
        pc += 2;
    }
}


void chip8::executeANNN(opcode op) {
    cout << "ANNN" << endl;
    /* Sets I to the address NNN */
    I = NNN;
    pc += 2;
}


void chip8::executeBNNN(opcode op) {
    cout << "BNNN" << endl;
    /* Jumps to the address NNN plus V0 */
    pc = V[0] + NNN;
}


void chip8::executeCXNN(opcode op) {
    cout << "CXNN" << endl;
    /* Sets VX to the result of a bitwise and operation on a random number and NN */
    V[X] = (rand() % 0xFF) & NN;
    pc += 2;
}


void chip8::executeDXYN(opcode op) {
    cout << "DXYN" << endl;
    /* Draws a sprite ... */
    pc += 2;
}


void chip8::executeEX9E(opcode op) {
    cout << "EX9E" << endl;
    /* Skips the next instruction if the key stored in VX is pressed */
    pc += 2;
}


void chip8::executeEXA1(opcode op) {
    cout << "EXA1" << endl;
    /* Skips the next instruction if the key stored in VX isn't pressed */
    pc += 2;
}


void chip8::executeFX07(opcode op) {
    cout << "FX07" << endl;
    /* Sets VX to the value of the delay timer */
    
    V[X] = delayTimer;
    pc += 2;
}


void chip8::executeFX0A(opcode op) {
    cout << "FX0A" << endl;
    /* A key press is awaited, and then stored in VX. (Blocking operation) */
    pc += 2;
}


void chip8::executeFX15(opcode op) {
    cout << "FX15" << endl;
    /* Sets the delay timer to VX */
    
    delayTimer = V[X];
    pc += 2;
}


void chip8::executeFX18(opcode op) {
    cout << "FX18" << endl;
    /* Sets the sound timer to VX */

    soundTimer = V[X];
    pc += 2;
}


void chip8::executeFX1E(opcode op) {
    cout << "FX1E" << endl;
    /* Adds VX to I. VF is set to 1 when there is a range overflow (I + VX > 0xFFF) */
    /* and to 0 when there isn't */
    if (I + V[X] > 0xFFF) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }
    I += V[X];
    pc += 2;
}


void chip8::executeFX29(opcode op) {
    cout << "FX29" << endl;
    /* Sets I to the location of the sprite for the character in VX. */
    pc += 2;
}


void chip8::executeFX33(opcode op) {
    cout << "FX33" << endl;
    /* Stores the BCD representation of VX, with the most significant of */
    /* three digits at the address in I, the middle digit at I plus 1, and */
    /* the least significant digit at I plus 2. */
    memory[I] = V[X] / 100;
    memory[I + 1] = (V[X] / 10) % 10;
    memory[I + 2] = V[X] % 10;
    pc += 2;
}


void chip8::executeFX55(opcode op) {
    cout << "FX55" << endl;
    /* Stores V0 to VX (including VX) in memory starting at address I. */
    /* The offset from I is increased by 1 for each value written, but */
    /* I itself is left unmodified */
    for (int i = 0; i <= X; i++) {
        memory[I + i] = V[i];
    }
    pc += 2;
}


void chip8::executeFX65(opcode op) {
    cout << "FX65" << endl;
    /* Fills V0 to VX (including VX) with values from memory starting at */
    /* address I. The offset from I is increased by 1 for each value written, */
    /* but I itself is left unmodified */
    for (int i = 0; i <= X; i++) {
        V[i] = memory[I + i];
    }
    pc += 2;
}


const byte chip8::fontset[FONTSET_LENGTH] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
