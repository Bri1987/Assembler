//
// Created by 12167 on 2023/10/1.
//

#ifndef AS_FINAL_ELF_STD_H
#define AS_FINAL_ELF_STD_H

// Executable header constants
#define EI_NIDENT 16          // Sizeof ident buf
#define EV_CURRENT 1          // version number
#define EF_ARM_ABI 0x05000000 // EABI v5

// Symbol binding
enum {
    STB_LOCAL = 0,
    STB_GLOBAL,
    STB_WEAK,
    STB_LOPROC = 13,
    STB_HIPROC = 15,
};

// Symbol type
enum {
    STT_NOTYPE = 0,
    STT_OBJECT,
    STT_FUNC,
    STT_SECTION,
    STT_FILE,
    STT_LOPROC = 13,
    STT_HIPROC = 15,
};

// Macros for symbol st_info
#define ELF32_ST_BIND(i) ((i)>>4)
#define ELF32_ST_TYPE(i) ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

// Word width
enum {
    ELFCLASS32 = 1,  // 32 bit words
    ELFCLASS64,      // 64 bit words
};

// Endianness
enum {
    ELFDATA2LSB = 1,  // Little endian, 2's complement
    ELFDATA2MSB,      // Big endian, 2's complement
};

// ELF machine types (e_machine)
enum {
    EM_NONE = 0,   // No machine
    EM_M32,        // AT&T WE 32100
    EM_SPARC,      // Sun SPARC
    EM_386,        // Intel 80386
    EM_68K,        // Motorola 68000
    EM_88K,        // Motorola 88000
    EM_860,        // Intel 80860
    EM_MIPS,       // MIPS RS3000
    EM_ARM = 0x28  // ARM
};

// ELF file types (e_type)
enum {
    ET_NONE = 0,         // No file type
    ET_REL,              // Relocatable file
    ET_EXEC,             // Executable file
    ET_DYN,              // Shared object file
    ET_CORE,             // Core file
    ET_LOPROC = 0xff00,  // Processor-specific
    ET_HIPROC = 0xffff,  // Processor-specific
};

typedef unsigned int   Elf32_Addr;
typedef unsigned short Elf32_Half;
typedef unsigned int   Elf32_Off;
typedef int            Elf32_Sword;
typedef unsigned int   Elf32_Word;

// Executable header type. One per ELF file.
typedef struct {
    unsigned char e_ident[EI_NIDENT];             //表示ELF文件的标识信息
    Elf32_Half e_type;                            //表示 ELF 文件的类型，比如可执行文件、共享库等
    Elf32_Half e_machine;
    Elf32_Word e_version;
    Elf32_Addr e_entry;                          //表示程序的入口地址
    Elf32_Off  e_phoff;                          //表示程序头表（Program Header Table）的偏移量
    Elf32_Off  e_shoff;                          //表示节表（Section Header Table）的偏移量
    Elf32_Word e_flags;                          //表示 ELF 文件的标志信息
    Elf32_Half e_ehsize;                         //表示 ELF 文件头部的大小  sizeof(Elf32_Ehdr);
    Elf32_Half e_phentsize;                      //表示program header table中每个表项的大小，每一个表项描述一个Segment信息
    Elf32_Half e_phnum;                          //表示program header table中表项的数量
    Elf32_Half e_shentsize;                      //Section header table中每个表项的大小sizeof(Elf32_Shdr)
    Elf32_Half e_shnum;                          //num sections
    Elf32_Half e_shstrndx;                       //表示节表中字符串表的索引，第多少个表项描述的是字符串表......
} Elf32_Ehdr;

// Section header types
enum {
    SHT_NULL = 0,
    SHT_PROGBITS,                               //包含程序定义的信息，如代码和数据
    SHT_SYMTAB,                                 //该section包含一个符号表，一般提供用于链接编辑的符号
    SHT_STRTAB,
    SHT_RELA,                                    //包含重定位表，用于链接 rel data
    SHT_HASH,                                   //包含符号哈希表，用于快速查找符号，所有参与动态链接的目标都必须包含一个符号哈希表
    SHT_DYNAMIC,                                //包含动态链接信息
    SHT_NOTE,                                   //包含注释信息
    SHT_NOBITS,                                 //不占用文件空间的节类型
    SHT_REL,                                     //包含重定位表，用于链接 rel text
    SHT_SHLIB,                                  //保留节类型，未使用
    SHT_DYNSYM,                                 //包含动态符号表，用于动态加载
    SHT_INIT_ARRAY,                             //包含初始化函数指针数组
    SHT_FINI_ARRAY,                             //包含终止函数指针数组
    SHT_GROUP,                                  //包含节组信息
    SHT_SYMTAB_SHNDX,                           //包含符号表节索引表
    SHT_NUM,                                    //节类型数量
    SHT_ARM_ATTRS = 0x70000003,                 //ARM 特定的节类型，包含属性信息
};

// Section header flags
enum {
    SHF_WRITE = 0x1,
    SHF_ALLOC = 0x2,                            //标志表示该节在进程的内存空间中可用
    SHF_EXECINSTR = 4,                          //标志表示该节包含可执行指令
    SHF_MASKPROC = 0xf0000000,                  //标志表示该节的处理器特定标志
};

// Section header
typedef struct {
    Elf32_Word sh_name;                          //section名称，是在字符串表节区的索引
    Elf32_Word sh_type;
    Elf32_Word sh_flags;                         //描述杂项属性
    Elf32_Addr sh_addr;                          //如果该section将出现在进程的内存映像中，
                                                 //则该成员给出该section的第一个字节应该驻留的地址。否则，该成员的值为0
    Elf32_Off  sh_offset;                        //该节在elf文件中偏移量
    Elf32_Word sh_size;                          //该节的大小
    Elf32_Word sh_link;                          //holds a section header table index link,表示当前节依赖于对应的节
    Elf32_Word sh_info;                          //该节的附加信息，如符号表节中存储的符号数量
    Elf32_Word sh_addralign;                     //该节的对齐方式
    Elf32_Word sh_entsize;                       //某些节区中包含固定大小的项目，如符号表节中每个符号的大小,没有则是0
} Elf32_Shdr;

// ARM Attributes constants. See the Build Attributes reference above
#define AA_VERSION 0x41
#define AA_SIZE 0x16
#define AA_VENDOR "aeabi"
#define AA_TAG_FILE 1
#define AA_FILE_BYTE_SIZE 0x0b
#define AA_TAG_CPU 6
#define AA_CPU_V4T 2
#define AA_TAG_ARM_ISA 8
#define AA_ARM_ISA 1
#define AA_TAG_THUMB_ISA 9
#define AA_THUMB_ISA 1

// ARM attributes. I tried to make this a real struct but the
// compiler's alignment rules were in the way.
typedef unsigned char elf_arm_attributes[AA_SIZE];

// Symbol in the symbol table
typedef struct {
    Elf32_Word    st_name;              //存储一个指向字符串表的索引来表示对应符号的名称
    Elf32_Addr    st_value;            //存储对应符号的取值，具体值依赖于上下文，可能是一个指针地址，立即数等
    Elf32_Word    st_size;             //符号的大小，未知则为0
    unsigned char st_info;             //指定符号的类型和绑定属性, 解析出bind type info三个属性
    unsigned char st_other;
    Elf32_Half    st_shndx;           //每个符号都有属于的节，当前成员存储的就是对应节的索引
} Elf32_Sym;

// Relocation
typedef struct {
    Elf32_Addr r_offset;             //可重定位文件: 节开始到存储单元受重定位影响的字节偏移量
                                     //可执行/共享文件: 存储单元受重定位影响的虚拟地址
    Elf32_Word r_info;               //表示重定位类型和符号表索引。其中，高 8 位表示重定位类型，低 24 位表示符号表索引
} Elf32_Rel;

// High 28 bits of BX to identify needed relocations
#define ARM_BX_28 0xe12fff1
#define ARM_BL_24 0xfffffe

// This appears to be the only R_TYPE code we need (of a long list)
#define R_ARM_CALL 0x1c

#define ELF32_R_SYM(i) ((i)>>8)
#define ELF32_R_TYPE(i) ((unsigned char)(i))
#define ELF32_R_INFO(s,t) (((s)<<8)+(unsigned char)(t))

#endif //AS_FINAL_ELF_STD_H
