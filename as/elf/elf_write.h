//
// Created by 12167 on 2023/10/1.
//

#ifndef AS_FINAL_ELF_WRITE_H
#define AS_FINAL_ELF_WRITE_H

#include "lib/util.h"
#include "elf_std.h"
#include "elf_add.h"
#include "elf.h"

bool elf_write_file(elf_context *elf, FILE *f);

#endif //AS_FINAL_ELF_WRITE_H
