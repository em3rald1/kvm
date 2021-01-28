#pragma once
#include <iostream>
#include <vector>

using std::vector;
typedef unsigned char u8;
typedef unsigned short u16;

enum STORE_TYPES {
    IMM = 0,
    OFFSET = 1,
    OFFX = 2,
};

enum ALU_TYPES {
    IMMA = 0,
    R_IMMA = 1,
    RRA = 2,
    IMMS = 0,
    R_IMMS = 1,
    IMM_RS = 2,
    RRS = 3,
};

#define HLT 0xff
#define LDA 1
#define LDX 2
#define LDY 3
#define STA 4
#define STX 5
#define STY 6
#define PHA 7
#define PHX 8
#define PHY 9
#define PLA 10
#define PLX 11
#define PLY 12

#define CMP 13
#define BNE 14
#define BEQ 15
#define BRC 16
#define JSR 17
#define RTS 18

#define ADD 19
#define SUB 20

class cpu
{
    u16 ip = 0;
    u8 a = 0;
    u8 x = 0;
    u8 y = 0;
    u8 sp = 0xff;
    vector<u8> memory;
    bool zf = true;
    u8 libraries;
    public:
        cpu();
        void loadExecutable(vector<u8> data);
        void loadLibrary(vector<u8> lib, u16 offset);
        bool step();
        u8 fetch();
        void printState();
        void reset();
        void push(u8 d);
        u8 pull();
        void execute(bool dbg);
        u8& gr(u8 i);
};