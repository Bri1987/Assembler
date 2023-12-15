//
// Created by 12167 on 2023/10/1.
//

#ifndef AS_FINAL_ELF_INIT_H
#define AS_FINAL_ELF_INIT_H

#include "lib/util.h"
#include "elf_std.h"
#include "elf.h"
#include "elf_add.h"

// Section indexes
enum {
    SEC_NULL = 0,
    SEC_TEXT,
   // SEC_REL_TEXT,
    SEC_DATA,
    SEC_BSS,
    SEC_ARM_ATTRS,
    SEC_SYMTAB,
    SEC_STRTAB,           //字符串表
    SEC_SHSTRTAB,         //段表字符串表
    SEC_GSYMTAB, // This is beyond NUM_HEADERS. See append_symbols for the reason
};

bool elf_init(elf_context* elf);

#endif //AS_FINAL_ELF_INIT_H
