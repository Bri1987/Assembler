//
// Created by 12167 on 2023/10/1.
//

#ifndef AS_FINAL_ELF_H
#define AS_FINAL_ELF_H

#include "elf_std.h"

#define NUM_SECTIONS 8

/* elf_section represents the body of each section */
typedef struct {
    unsigned char *data;
    unsigned int   logical_len;          //实际占用的字节数
    unsigned int   memory_len;           //在内存中占用的字节数，可能会包含一些填充字节，动态扩展可能多的
} elf_section;

/* elf_context represents all the information we collect to write the file */
typedef struct {
    Elf32_Ehdr  ehdr;                       //elf文件
    elf_section sections[NUM_SECTIONS + 1]; // +1 for globals. See append_symbols()
    Elf32_Shdr  shdrs[NUM_SECTIONS];        //每个节都有一个对应的节表头部
} elf_context;

#endif //AS_FINAL_ELF_H
