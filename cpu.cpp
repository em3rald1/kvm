#include "cpu.h"

cpu::cpu() {
    this->memory.reserve(0xffff);
}

void cpu::loadExecutable(std::vector<u8> data) {
    for(int i = 0; i < data.size(); i++) {
        this->memory[i] = data[i];
    }
}

void cpu::loadLibrary(vector<u8> data, u16 offset) {
    for(int i = 0; i < data.size(); i++) {
        this->memory[i+offset] = data[i];
    }
}

u8 cpu::fetch() {
    return this->memory[ip++];
}

void cpu::printState() {
    std::cout << "ACC: " << (int)this->a << std::endl;
    std::cout << "IP: " << this->ip << std::endl;
    std::cout << "X: " << (int)this->x << std::endl;
    std::cout << "Y: " << (int)this->y << std::endl;
    //std::cout << "ACC: " << this->a << std::endl;

}

void cpu::reset() {
    this->memory.clear();
    this->memory.reserve(0xffff);
}

bool cpu::step() {
    u8 instruction = fetch();
    bool status = false;
    switch(instruction) {
        case HLT: {
            status = true; 
            break;
        }
        case LDA: {
            u8 d = fetch();
            this->a = d;
            zf = a == 0;
            break;
        }
        case LDX: {
            u8 d = fetch();
            this->x = d;
            break;
        }
        case LDY: {
            this->y = fetch();
            break;
        }
        case STA: {
            u8 type = fetch();
            switch(type) {
                case STORE_TYPES::IMM: {
                    u16 d = (fetch() << 8) | fetch();
                    this->memory[d] = this->a;
                    break;
                }
                case STORE_TYPES::OFFSET: {
                    u16 d = (fetch() << 8) | fetch();
                    this->memory[this->ip+d] = this->a;
                    break;
                }
                case STORE_TYPES::OFFX: {
                    u16 d = (fetch() << 8) | fetch();
                    this->memory[d+this->x] = this->a;
                    break;
                }
            }
            break;
        }
        case STX: {
            u8 type = fetch();
            switch(type) {
                case STORE_TYPES::IMM: {
                    u16 d = (fetch() << 8) | fetch();
                    this->memory[d] = this->x;
                    break;
                }
                case STORE_TYPES::OFFSET: {
                    u16 d = (fetch() << 8) | fetch();
                    this->memory[this->ip+d] = this->x;
                    break;
                }
                case STORE_TYPES::OFFX: {
                    u16 d = (fetch() << 8) | fetch();
                    this->memory[d+this->y] = this->x;
                    break;
                }
            }
            break;
        }
        case STY: {
            u8 type = fetch();
            switch(type) {
                case STORE_TYPES::IMM: {
                    u16 d = (fetch() << 8) | fetch();
                    this->memory[d] = this->y;
                    break;
                }
                case STORE_TYPES::OFFSET: {
                    u16 d = (fetch() << 8) | fetch();
                    this->memory[this->ip+d] = this->y;
                    break;
                }
                case STORE_TYPES::OFFX: {
                    u16 d = (fetch() << 8) | fetch();
                    this->memory[d+this->x] = this->y;
                    break;
                }
            }
            break;
        }
        case PHA: {
            push(a);
            break;
        }
        case PHX: {
            push(x);
            break;
        }
        case PHY: {
            push(y);
            break;
        }
        case PLA:{
            a = pull();
            break;
        }
        case PLX: {
            x = pull();
            break;
        }
        case PLY: {
            y = pull();
            break;
        }
        case CMP: {
            a = fetch() - fetch();
            zf = a == 0;
            break;
        }
        case BNE: {
            if(!zf) {
                ip = (fetch() << 8 ) | fetch();
            }
            break;
        }
        case BEQ: {
            if(zf) {
                ip = (fetch() << 8 ) | fetch();
            }
            break;
        }
        case BRC: {
            ip = (fetch() << 8 ) | fetch();
            break;
        }
        case JSR: {
            this->memory[0xef00] = ip >> 8;
            this->memory[0xef01] = ip;
            ip = (u16)((u16)fetch() << 8 ) | (u16)fetch();
            break;
        }
        case RTS: {
            ip = (this->memory[0xef00] << 8) | this->memory[0xef01];
            break;
        }
    }
    return status;
}

void cpu::push(u8 d) {
    this->memory[this->sp | 0x3f00] = d;
    this->sp--;
}

u8 cpu::pull() {
    this->sp++;
    return this->memory[this->sp | 0x3f00];
}

void cpu::execute(bool dbg) {
    bool x = false;
    while(!x) {
        x = this->step();
        if(dbg) this->printState();
    }
}

u8& cpu::gr(u8 i) {
    switch(i){
        case 0: return a;
        case 1: return x;
        case 2: return y;
        case 3: return sp;
    }
}