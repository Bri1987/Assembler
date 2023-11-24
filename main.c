#include <stdio.h>
#include "as/scan.h"
#include "as/config.h"
#include "lib/hash_set.h"
#include "instruction.h"
#include "lib/ArrayList.h"
#include "lib/hash_map.h"
#include "as/codegen.h"
#include "as/parse.h"

HashMap *labelMap;
HashMap *globVMap;
HashSet *globLabelSet;     //.global main

//putint -- i
HashMap *unknown_instMap;

struct ArrayList *inst_list;

int main(int argc, char **argv) {
    struct scan_table_st scan_table;
    struct config_st config;
    labelMap = HashMapInit();
    globVMap = HashMapInit();
    globLabelSet = HashSetInit();
    unknown_instMap = HashMapInit();
    armasm_config_init(&config);
    armasm_parse_args(argc, argv, &config);
    armasm_read_source(&config);
    //词法分析
    scan_table_init(&scan_table);
    scan_table_scan(&scan_table, config.source, config.source_len);
    scan_table_print(&scan_table);

    //生成inst
    inst_list = newArrayList();
    parse_program(&scan_table);
    print_instruction();

    //codegen
    struct codegen_table_st code_table;
    codegen_table_init(&code_table);
    codegen_programme(&code_table);
    codegen_print_hex(&code_table);

    //write elf
    codegen_elf_write(&code_table,config.objfile);

    return 0;
}
