//
// Created by 12167 on 2023/9/26.
//

#ifndef AS_FINAL_PARSE_H
#define AS_FINAL_PARSE_H

#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include "codegen.h"
#include "util.h"


/*
 * parse.c
 */

/*
A simple grammar for ARM assembly

program     ::= statements EOT

statements  ::= statement EOL
              | statement EOL statements

statement   ::= directive label
              | label ":" (EOL)* instruction
              | instruction

instruction ::= dp register "," register "," register
              | dp register "," register "," immediate
              | mem register "," "[" register "," register "]"
              | mem register "," "[" register "]"
              | mem register "," "[" register "," immediate "]"
              | branch label
              | bx register

register    ::= "r0" | "r1" | ...| "r15" | "sp" | "lr" | "ip"

dp          ::= "add" | "cmp" | "lsl" | "lsr" | "mov"

mem         ::= "ldr" |"ldrb" | "str" | "strb"

branch      ::= "b" | "bl" | beq" | "bne" | "bgt" | "bge" | "blt" | "ble"

bx          ::= "bx"

*/

#define SCAN_TOKEN_LEN 64

struct parse_reg_pair_st {
    char *name;
    int num;
};

#define PARSE_REG_PAIR_MAP \
   { {"r0", 0}, {"r1", 1}, {"r2", 2,}, {"r3", 3}, \
     {"r4", 4}, {"r5", 5}, {"r6", 6,}, {"r7", 7}, \
     {"r8", 8}, {"r9", 9}, {"r10", 10}, {"r11", 11}, \
     {"r12", 12}, {"r13", 13}, {"r14", 14,}, {"r15", 15}, \
     {"fp", 11}, {"ip", 12}, {"sp", 13}, {"lr", 14}, {"pc", 15} }

struct Instruction* parse_instruction(struct scan_table_st *st);
void parse_program(struct scan_table_st *st);
void print_instruction();

int* returnLabel(char* label,HashMap* map);
codegen_global_pair * returnGlobV(char* label,HashMap* map);

#endif //AS_FINAL_PARSE_H
