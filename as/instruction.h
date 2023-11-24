//
// Created by 12167 on 2023/9/27.
//

#ifndef AS_FINAL_INSTRUCTION_H
#define AS_FINAL_INSTRUCTION_H

#include <stdint.h>
#include <stdbool.h>
#include "AssemblerException.h"
#include "lib/hash_set.h"
#include <stdlib.h>
#include <string.h>

enum Opcode{
    //data processing
    lsl,
    add,
    sub,
    mul,
    cmp,

    mov,
    movw,       //TODO 特殊处理了，如果需要还是把后缀分开
    movt,

    // branching
    b,
    bx,
    bl,

    //mem
    str,
    ldr,
    ldmfd,
    stmfd,

    //stack
    push,
    pop,

    // indicates error
    invalid,
};

enum COND {
    EQ,
    NE,
    GE,
    LT,
    GT,
    LE,
    AL
};

#define MAX_LABEL_LENGTH 64

typedef struct parse_cond_pair_ {
    char *name;
    enum COND cond;
}parse_cond_pair;

#define PARSE_COND_MAP \
   { {"eq", EQ}, {"ne", NE}, {"ge", GE}, {"lt", LT}, \
     {"gt", GT}, {"le", LE}}

typedef struct char_opcode_pair_ {
    char *name;
    enum Opcode opcode;
}char_opcode_pair;

typedef struct print_opcode_pair_ {
    enum Opcode opcode;
    char *name;
}print_opcode_pair;

#define PARSE_OPCODE_MAP \
   { {"add", add}, {"sub", sub}, {"lsl", lsl}, {"cmp", cmp}, \
     {"b", b},{"bx",bx} ,{"bl",bl},{"mul", mul}, {"mov",mov}  ,                   \
     {"pop",pop}, {"push",push}         ,                     \
     {"ldr",ldr},{"str",str},{"ldmfd",ldmfd},{"stmfd",stmfd}, \
     {"movw",movw},{"movt",movt}}

#define PRINT_OPCODE_MAP \
   { {add,"add"}, {sub,"sub"}, {lsl,"lsl"}, {cmp,"cmp"}, \
     {b, "b"},{bx,"bx"} ,{bl,"bl"},{mul, "mul"}, {mov,"mov"}  ,                   \
     {pop,"pop"}, {push,"push"}         ,                     \
     {ldr,"ldr"},{str,"str"},{ldmfd,"ldmfd"},{stmfd,"stmfd"}, \
     {movw,"movw"},{movt,"movt"}}

typedef struct Operand {
    uint8_t reg;
    signed int imm;
    unsigned int isreg : 1;       //表明reg是0 - 15
    unsigned int immisreg : 1;    //表明是imm还是reg
    unsigned int writeback : 1;    //^与!需要writeback
}Operand;

typedef struct Instruction {
    enum Opcode opcode;   // id indicating the instruction
    enum COND cond;
    Operand rd;     //rd
    Operand rn;     //src1
    Operand rm;     //src2
    Operand shift;
    uint32_t shift_amount;  // shift amount
    unsigned int imm : 1;      //I 位

    char label[MAX_LABEL_LENGTH]; // label of the line this Instruction appears on
    char branch_label[MAX_LABEL_LENGTH];  // label used by branch or jump instructions
    int index;          //标记是第几条
}Instruction;

struct Instruction* newInstructionBlank();
void insert_into_ops_set();


#endif //AS_FINAL_INSTRUCTION_H
