//
// Created by 12167 on 2023/10/1.
//

#ifndef AS_FINAL_ELF_ADD_H
#define AS_FINAL_ELF_ADD_H

#include "elf.h"
#include "elf_std.h"
#include "elf_init.h"

// Section names in the order they appear in the section header table
static char *snames[NUM_SECTIONS] = {
        "", // The null section has no name
        "", // TODO .text reuses .rel.text
        ".rel.text",
        ".data",
        ".bss",
        ".ARM.attributes",
        ".symtab",
        ".strtab",
        ".shstrtab",
};

bool grow_and_add(elf_section *sec, void *src, size_t src_len);
bool add_symbol(elf_section *sec, Elf32_Sym *sym);
bool elf_add_instr(elf_context *elf, unsigned int instr);
void finalize_shdrs(elf_context *elf);
void finalize_sections(elf_context *elf);

bool elf_add_symbol(elf_context *elf, char *name, int offset, int binding);

#endif //AS_FINAL_ELF_ADD_H
