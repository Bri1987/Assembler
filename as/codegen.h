//
// Created by 12167 on 2023/9/27.
//

#ifndef AS_FINAL_CODEGEN_H
#define AS_FINAL_CODEGEN_H
#include "utility.h"
#include "./elf/elf.h"
#include "./elf/elf_init.h"
#include "./elf/elf_std.h"
#include "./elf/elf_write.h"

#define SCAN_TOKEN_LEN 64
#define CODEGEN_TABLE_LEN 256

typedef struct codegen_global_pair {
    int offset;
    int occupy;
    char label[SCAN_TOKEN_LEN];
    //有初始化值的数组要存值
    int num[300];
}codegen_global_pair;

struct codegen_table_st {
    uint32_t table[CODEGEN_TABLE_LEN];
    int len;
    int next;
    struct codegen_global_pair labels[CODEGEN_TABLE_LEN];
    int label_count;
    struct codegen_global_pair publics[CODEGEN_TABLE_LEN];
    int public_count;
};

typedef struct codegen_map_st {
    enum COND cond;
    uint32_t bits;
}codegen_map_st;

#define CODEGEN_COND_MAP \
   { {EQ,0b0000}, {NE, 0b0001}, {GE, 0b1010}, {LT, 0b1011}, \
     {GT, 0b1100}, {LE, 0b1101}, {AL,0b1110}}

typedef struct codegen_opcode_map_st {
    enum Opcode opcode;
    uint32_t bits;
}codegen_opcode_map_st;

#define CODEGEN_OPCODE_MAP \
   { {add, 0b0100}, {sub, 0b0010}, {lsl, 0b1101}, {cmp, 0b1010},{mov,0b1101},{movt,0b00110100},{movw,0b00110000}, \
     {b, 0b1010}, {bl,0b1011},{ldr, 0b0010}  , {str,0b0010} , {stmfd,0b100100}, {ldmfd,0b100010}, \
     {push, 0b100100101101}, {pop, 0b100010111101}}       \


void codegen_table_init(struct codegen_table_st *ct);
void codegen_programme(struct codegen_table_st *ct);
void codegen_elf_write(struct codegen_table_st *ct, char *path);
void codegen_print_hex(struct codegen_table_st *ct);

#endif //AS_FINAL_CODEGEN_H
