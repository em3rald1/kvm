#include "cpu.h"

int main() {
    cpu cpu;
    cpu.loadExecutable(vector<u8> {
        LDX, 50,
        JSR, 0x0f, 0x00,
        HLT
    });
    cpu.loadLibrary(vector<u8> {
        PHA,
        LDA, 40,
        STA, STORE_TYPES::OFFX, 00, 50, 
        PLA,
        RTS
    }, 0x0f00);
    cpu.execute(true);
}