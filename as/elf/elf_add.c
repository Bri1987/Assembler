//
// Created by 12167 on 2023/10/1.
//
#include "elf_add.h"

// grow_and_add() provides dynamically growable buffers
//为data*提供
bool grow_and_add(elf_section *sec, void *src, size_t src_len) {
    const int SEC_BLOCK_SIZE = 128;
    if ((sec->logical_len + src_len) >= sec->memory_len) {
        sec->data = realloc(sec->data, sec->memory_len + SEC_BLOCK_SIZE);
        if (!sec->data) {
            printf("failed to realloc\n");
            return false;
        }
        sec->memory_len += SEC_BLOCK_SIZE;
    }

    memcpy(&sec->data[sec->logical_len], src, src_len);
    sec->logical_len += src_len;
    return true;
}

bool add_symbol(elf_section *sec, Elf32_Sym *sym) {
    return grow_and_add(sec, sym, sizeof(Elf32_Sym));
}

// add_int() adds an instruction to the text section
static bool add_int(elf_section *sec, unsigned int i) {
    return grow_and_add(sec, &i, sizeof(unsigned int));
}

// add_str() adds a string to the ELF string table specified by sec
static bool add_str(elf_section *sec, char *str) {
    int str_len = strlen(str);
    if (0 == str_len)
        return true; // the null section's name is ""

    str_len++; // ELF strtab includes the trailing '\0'
    return grow_and_add(sec, str, str_len);
}

// add_rel() creates an ELF relocation object for a BX instruction
static bool add_rel(elf_section *sec, unsigned int offset) {
    Elf32_Rel rel = {
            .r_offset = offset,
            .r_info = ELF32_R_INFO(STB_LOCAL, R_ARM_V4BX),
    };
    return grow_and_add(sec, &rel, sizeof(Elf32_Rel));
}

// add_text_symbol() adds a user-defined symbol (e.g. a label) to
// the symbol table. Those symbols refer (st_shndx) to the .text section
static bool add_text_symbol(elf_section *sec, unsigned int st_name, int offset, int binding) {
    Elf32_Sym sym;
    memset(&sym, 0, sizeof(sym));
    sym.st_info = ELF32_ST_INFO(binding, STT_NOTYPE);
    sym.st_name = st_name;
    sym.st_shndx = SEC_TEXT;
    sym.st_value = offset;

    return add_symbol(sec, &sym);
}

// elf_add_instr() is the public way to add a machine code instruction
// into the section body for .text
bool elf_add_instr(elf_context *elf, unsigned int instr) {
    if (!elf || !instr)
        return false;

    // Save off the previous instruction offset. If we create a
    // relocation, its offset is this value
    unsigned int rel_offset = elf->sections[SEC_TEXT].logical_len;

    // Add the instruction itself
    // Add the instruction itself
    if (!add_int(&elf->sections[SEC_TEXT], instr))
        return false;

    //TODO why
    // Add a relocation for BX instructions. I'm not sure why.
    // Greg guesses for PC-relative data, which follows a BX
//    if (instr >> 4 == ARM_BX_28) {
//        if (!add_rel(&elf->sections[SEC_REL_TEXT], rel_offset))
//            return false;
//    }
    return true;
}

// align_sections() pads each non-word-aligned section out to a word
// This works because Elf32_Ehdr starts the sections off word-aligned
static void align_sections(elf_context *elf) {
    for (int s = 0; s < NUM_SECTIONS; s++) {
        elf_section *sec = &elf->sections[s];
        int mod = sec->logical_len % sizeof(Elf32_Word);
        if (mod != 0) {
            int pad = sizeof(Elf32_Word) - mod;
            unsigned char*data = realloc(sec->data, sec->logical_len + pad);
            for (int p = 0; p < pad; p++)
                data[sec->logical_len + p] = 0;
            sec->data = data;
            sec->logical_len += pad;
        }
    }
}
// count_local_symbols() supports the symtab section header sh_info
static int count_local_symbols(elf_section *sec) {
    int count = 0;
    int num_symbols = sec->logical_len / sizeof(Elf32_Sym);
    Elf32_Sym *syms = (Elf32_Sym*) sec->data;
    for (int i = 0; i < num_symbols; i++) {
        if (STB_LOCAL == ELF32_ST_BIND(syms[i].st_info))
            count++;
    }
    return count;
}

/* append_symbols() takes the symbols from sections[src] and appends
   them to sections[dest]. This is odd-looking but I realized late in the
   project that STB_GLOBAL symbols must occur after STB_LOCAL symbols
   when we write .symtab to disk
*/
static void append_symbols(elf_context *elf, int dest, int src) {
    int num_src_symbols = elf->sections[src].logical_len / sizeof(Elf32_Sym);
    Elf32_Sym *sym_ptr = (Elf32_Sym*) elf->sections[src].data;
    elf_section *dest_symtab = &elf->sections[dest];
    for (int s = 0; s < num_src_symbols; s++)
        add_symbol(dest_symtab, &sym_ptr[s]);

}

// finalize_shdrs() fills in the section headers
void finalize_shdrs(elf_context *elf) {
    Elf32_Shdr *shdrs = elf->shdrs;

    // Fill in the size of the symbol table
    shdrs[SEC_SYMTAB].sh_info = count_local_symbols(&elf->sections[SEC_SYMTAB]);
//
    // Create an aggregated symbol table with locals and globals
    append_symbols(elf, SEC_SYMTAB, SEC_GSYMTAB);

    // Fill in the offset and size of each section
    unsigned int sec_body_len = sizeof(Elf32_Ehdr);
    for (int s = 1; s < NUM_SECTIONS; s++) {
        shdrs[s].sh_offset = sec_body_len;
        int slen = elf->sections[s].logical_len;
        shdrs[s].sh_size = slen;
        sec_body_len += slen;
    }
}

// finalize_sections() adds boilerplate section data which isn't
// derived from the source code's instructions or labels
//添加写入每个节都必须有的信息
void finalize_sections(elf_context *elf) {

    elf_section *shstrtab = &elf->sections[SEC_SHSTRTAB];
    for (int s = 1; s < NUM_SECTIONS; s++) {
        // The offset of each header's name (sh_name) is the logical
        // length of the string table before adding the name
        elf->shdrs[s].sh_name = shstrtab->logical_len;

        // Build the string table for section header names
        add_str(shstrtab, snames[s]);
    }
    //elf->shdrs[SEC_TEXT].sh_name = 5; //TODO  GNU asm reuses .text from .rel.text

    // "$a" is an ARM ELF convention marking the beginning of ARM instructions
    elf_section *strtab = &elf->sections[SEC_STRTAB];
    unsigned int dollar_a_offset = strtab->logical_len;
    add_str(strtab, "$a");

    // Add the symbol for $a
    elf_section *symtab = &elf->sections[SEC_SYMTAB];
    add_text_symbol(symtab, dollar_a_offset, 0, STB_LOCAL);

    // Add the symbol for the ARM attributes section
    Elf32_Sym arm_attrs;
    memset(&arm_attrs, 0, sizeof(Elf32_Sym));
    arm_attrs.st_info = ELF32_ST_INFO(STB_LOCAL, STT_SECTION);
    arm_attrs.st_shndx = SEC_ARM_ATTRS;
    add_symbol(symtab, &arm_attrs);

    align_sections(elf);
}


// elf_add_symbol() is the public way to add a human-readable name, and
// into the section bodies for .symtab and .strtab
bool elf_add_symbol(elf_context *elf, char *name, int offset, int binding) {
    if (!elf || !name || !name[0])
        return false;

    // The offset of the new string in the string table is the
    // logical length of the string table before adding it
    elf_section *strtab = &elf->sections[SEC_STRTAB];
    unsigned int strtab_off = strtab->logical_len;
    if (!add_str(strtab, name))
        return false;

    // Add an element to the symbol table whose name is the new
    // label's offset in the string table
    elf_section *symtab;
    if (STB_LOCAL == binding)
        symtab = &elf->sections[SEC_SYMTAB];
    else
        symtab = &elf->sections[SEC_GSYMTAB];
    if (!add_text_symbol(symtab, strtab_off, offset, binding))
        return false;

    return true;
}