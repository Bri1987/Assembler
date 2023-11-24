//
// Created by 12167 on 2023/10/1.
//
#include "elf_init.h"

static void init_ehdr(elf_context *elf) {
    memset(&elf->ehdr, 0, sizeof(Elf32_Ehdr));
    elf->ehdr.e_ident[0] = 0x7f;
    elf->ehdr.e_ident[1] = 'E';
    elf->ehdr.e_ident[2] = 'L';
    elf->ehdr.e_ident[3] = 'F';
    elf->ehdr.e_ident[4] = ELFCLASS32;
    elf->ehdr.e_ident[5] = ELFDATA2LSB;
    elf->ehdr.e_ident[6] = EV_CURRENT;
    elf->ehdr.e_type = ET_REL;
    elf->ehdr.e_machine = EM_ARM;
    elf->ehdr.e_version = EV_CURRENT;
    elf->ehdr.e_entry = 0;
    elf->ehdr.e_phoff = 0;
    elf->ehdr.e_shoff = 0; // filled in at write-time
    elf->ehdr.e_flags = EF_ARM_ABI;
    elf->ehdr.e_ehsize = sizeof(Elf32_Ehdr);
    elf->ehdr.e_phentsize = 0;
    elf->ehdr.e_phnum = 0;
    elf->ehdr.e_shentsize = sizeof(Elf32_Shdr);
    elf->ehdr.e_shnum = NUM_SECTIONS; // Number of entries in SH table
    elf->ehdr.e_shstrndx = SEC_SHSTRTAB; // index of SH entry containing names
}


//初始化一个字符串表，使其以一个 NULL 字节开头，这是 ELF 文件格式所要求的
//字符串表中的第一个字符串的偏移量必须大于 0，以便区分第一个字符串和 NULL 字节
static bool init_strtab(elf_context *elf, int sec_id) {
    elf_section *strtab = &elf->sections[sec_id];
    return grow_and_add(strtab, "", 1);         //给该section的data一个初始化内存
}

// init_arm_attributes() fills in the .ARM.attributes section
static bool init_arm_attributes(elf_context *elf) {
    // This sucks but my attempts to make a real struct out of it were
    // thwarted by the compiler's alignment rules.
    elf_arm_attributes attrs = {
            AA_VERSION,
            (unsigned char) sizeof(elf_arm_attributes),
            0, // word
            0, // word
            0, // word
            'a',
            'e',
            'a',
            'b',
            'i',
            '\0',
            AA_TAG_FILE,
            0x0b, // file section len
            0, // word
            0, // word
            0, // word
            AA_TAG_CPU,
            AA_CPU_V4T, // will this work on all RPi models?
            AA_TAG_ARM_ISA,
            true, // arm enabled
            AA_TAG_THUMB_ISA,
            true, // thumb enabled
    };

    elf_section *sec = &elf->sections[SEC_ARM_ATTRS];
    size_t size = sizeof(elf_arm_attributes);

    sec->data = malloc(size);
    if (!sec->data) {
        printf("init_arm_attributes failed to malloc\n");
        return false;
    }
    memcpy(sec->data, &attrs, size);
    sec->logical_len = sec->memory_len = size;
    return true;
}

// init_shdrs() initializes static info in the section headers
// offsets are calculated at write-time
static void init_shdrs(elf_context *elf) {
    Elf32_Shdr *shdrs = elf->shdrs;
    for (int s = 1; s < NUM_SECTIONS; s++) {
        switch (s) {
            case SEC_TEXT:
                shdrs[s].sh_type = SHT_PROGBITS;
                shdrs[s].sh_flags = SHF_ALLOC | SHF_EXECINSTR;
                shdrs[s].sh_addralign = sizeof(Elf32_Word);
                break;
//            case SEC_REL_TEXT:
//                shdrs[s].sh_type = SHT_REL;
//                shdrs[s].sh_flags = SHF_EXECINSTR;
//                shdrs[s].sh_link = SEC_SYMTAB;            //该节中的重定位项所引用的符号信息存储在符号表节中
//                shdrs[s].sh_info = SEC_TEXT;
//                shdrs[s].sh_addralign = sizeof(Elf32_Word);
//                shdrs[s].sh_entsize = sizeof(Elf32_Rel);
//                break;
            case SEC_DATA:
                shdrs[s].sh_type = SHT_PROGBITS;
                shdrs[s].sh_flags = SHF_ALLOC | SHF_WRITE;
                shdrs[s].sh_addralign = sizeof(unsigned char);
                break;
            case SEC_BSS:
                shdrs[s].sh_type = SHT_NOBITS;
                shdrs[s].sh_flags = SHF_ALLOC | SHF_WRITE;
                shdrs[s].sh_addralign = sizeof(unsigned char);
                break;
            case SEC_ARM_ATTRS:
                shdrs[s].sh_type = SHT_ARM_ATTRS;
                shdrs[s].sh_addralign = sizeof(unsigned char);
                break;
            case SEC_SYMTAB:
                shdrs[s].sh_type = SHT_SYMTAB;
                shdrs[s].sh_link = SEC_STRTAB;
                shdrs[s].sh_info = 0; // filled in at write-time
                shdrs[s].sh_addralign = sizeof(Elf32_Word);
                shdrs[s].sh_entsize = sizeof(Elf32_Sym);
                break;
            case SEC_STRTAB:
                shdrs[s].sh_type = SHT_STRTAB;
                shdrs[s].sh_addralign = sizeof(unsigned char);
                break;
            case SEC_SHSTRTAB:
                shdrs[s].sh_type = SHT_STRTAB;
                shdrs[s].sh_addralign = sizeof(unsigned char);
                break;
            default:
                printf("add_final_hdrs: unknown index %d\n", s);
        }
    }
}

// init_symbols() adds boilerplate symbols to the symbol
// table. These do not refer (st_shndx) to the .text section
static bool init_symbols(elf_context *elf) {
    Elf32_Sym sym;
    elf_section *sec = &elf->sections[SEC_SYMTAB];

    // syms[0] is STN_UNDEF, and all elements are 0
    memset(&sym, 0, sizeof(Elf32_Sym));
    if (!add_symbol(sec, &sym))
        return false;

    // syms[1] seems to be for the .text section
    memset(&sym, 0, sizeof(Elf32_Sym));
    sym.st_info = ELF32_ST_INFO(STB_LOCAL, STT_SECTION);
    sym.st_shndx = SEC_TEXT;
    if (!add_symbol(sec, &sym))
        return false;

    // syms[2] seems to be for the .data section
    memset(&sym, 0, sizeof(Elf32_Sym));
    sym.st_info = ELF32_ST_INFO(STB_LOCAL, STT_SECTION);
    sym.st_shndx = SEC_DATA;
    if (!add_symbol(sec, &sym))
        return false;

    // syms[3] seems to be for the .bss section
    memset(&sym, 0, sizeof(Elf32_Sym));
    sym.st_info = ELF32_ST_INFO(STB_LOCAL, STT_SECTION);
    sym.st_shndx = SEC_BSS;
    if (!add_symbol(sec, &sym))
        return false;

    return true;
}

// elf_init() initializes all of the static/boilerplate info so
// the writing code only has to summarize dynamic information
bool elf_init(elf_context* elf) {
    memset(elf,0,sizeof (elf_context));
    if (!init_strtab(elf, SEC_STRTAB))
        return false;
    if (!init_strtab(elf, SEC_SHSTRTAB))
        return false;
    if (!init_arm_attributes(elf))
        return false;

    init_ehdr(elf);
    init_shdrs(elf);
    if (!init_symbols(elf))
        return false;
    return true;
}