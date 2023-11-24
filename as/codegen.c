//
// Created by 12167 on 2023/9/28.
//
#include "codegen.h"
#define COND_BIT 28

extern HashMap *labelMap;
extern HashSet *globLabelSet;
extern struct ArrayList *inst_list;
codegen_map_st codegen_cond_map[] = CODEGEN_COND_MAP;
codegen_opcode_map_st codegen_opcode_map[] = CODEGEN_OPCODE_MAP;

//将labelMap的内容填入codegen_table_
void addLabels(struct codegen_table_st *ct){
    HashMapFirst(labelMap);
    for(Pair* p = HashMapNext(labelMap); p!=NULL; p= HashMapNext(labelMap)){
        codegen_global_pair *label = (codegen_global_pair*) malloc(sizeof (codegen_global_pair));
        strcpy(label->label,p->key);
        int *value = p->value;
        label->offset = *value;
        ct->labels[ct->label_count] = label;
        ct->label_count++;

        HashSetFirst(globLabelSet);
        for(char* p_label = HashSetNext(globLabelSet); p_label != NULL; p_label = HashSetNext(globLabelSet)){
            if(strcmp(p_label,p->key)==0){
                ct->publics[ct->public_count] = label;
                ct->public_count++;
                break;
            }
        }
    }


}

void codegen_error(char *err) {
    printf("codegen_error: %s\n", err);
    exit(-1);
}

void codegen_table_init(struct codegen_table_st *ct) {
    ct->len = 0;
    ct->next = 0;
}

void codegen_add_inst(struct codegen_table_st *ct, uint32_t inst) {
    ct->table[ct->len] = inst;
    ct->len += 1;
}

uint32_t codegen_cond_to_bit(enum COND cond) {
    int i;
    int map_len = sizeof(codegen_cond_map) / sizeof(codegen_cond_map[0]);

    for (i = 0; i < map_len; i++) {
        if (cond == codegen_cond_map[i].cond) {
            return codegen_cond_map[i].bits;
        }
    }

    codegen_error("");
}

uint32_t codegen_opcode_to_bit(enum Opcode opcode) {
    int i;
    int map_len = sizeof(codegen_opcode_map) / sizeof(codegen_opcode_map[0]);

    for (i = 0; i < map_len; i++) {
        if (opcode == codegen_opcode_map[i].opcode) {
            return codegen_opcode_map[i].bits;
        }
    }

    codegen_error("");
}

void codegen_simple_three_value(struct codegen_table_st *ct, Instruction* instruction) {
    //rd = rn + rm

    const uint32_t DP_IMM_BIT = 25;
    const uint32_t DP_OP_BIT  = 21;
    const uint32_t DP_RN_BIT  = 16;
    const uint32_t DP_RD_BIT  = 12;
    uint32_t inst = 0;

    uint32_t cond_bit = codegen_cond_to_bit(instruction->cond);
    uint32_t opcode_bit = codegen_opcode_to_bit(instruction->opcode);
    uint32_t rn = instruction->rn.reg;

    uint32_t rm;
    if(instruction->rm.isreg)
        rm = instruction->rm.reg;
    else
        rm = instruction->rm.imm;

    inst = (cond_bit           << COND_BIT)
           | (instruction->imm << DP_IMM_BIT)
           | ( opcode_bit      << DP_OP_BIT)
           | (rn               << DP_RN_BIT)
           | (instruction->rd.reg  << DP_RD_BIT)
           | rm;
    codegen_add_inst(ct, inst);
}

void codegen_cmp(struct codegen_table_st *ct, Instruction* instruction) {
    //cmp rn, rm
    const uint32_t DP_IMM_BIT = 25;
    const uint32_t DP_OP_BIT  = 21;
    const uint32_t DP_RN_BIT  = 16;
    const uint32_t DP_RD_BIT  = 12;
    uint32_t inst = 0;

    uint32_t cond_bit = codegen_cond_to_bit(instruction->cond);
    uint32_t opcode_bit = codegen_opcode_to_bit(instruction->opcode);
    uint32_t rn = instruction->rn.imm;

    uint32_t rm;
    if(instruction->rm.isreg)
        rm = instruction->rm.reg;
    else
        rm = instruction->rm.imm;

    inst = (cond_bit           << COND_BIT)
           | (instruction->imm << DP_IMM_BIT)
           | ( opcode_bit      << DP_OP_BIT)
           | (rn               << DP_RN_BIT)
           | (0  << DP_RD_BIT)
           | rm;
    codegen_add_inst(ct, inst);
}

void codegen_mov(struct codegen_table_st *ct, Instruction* instruction) {
    //mov rd,rn
    //p486 encoding A1
    const uint32_t DP_IMM_BIT = 25;
    const uint32_t DP_OP_BIT  = 21;
    //const uint32_t DP_RN_BIT  = 16;
    const uint32_t DP_RD_BIT  = 12;
    uint32_t inst = 0;

    uint32_t cond_bit = codegen_cond_to_bit(instruction->cond);
    uint32_t opcode_bit = codegen_opcode_to_bit(instruction->opcode);
    uint32_t rd = instruction->rd.reg;

    uint32_t rn;
    if(instruction->rn.isreg)
        rn = instruction->rn.reg;
    else
        rn = instruction->rn.imm;

    inst = (cond_bit           << COND_BIT)
           | (!instruction->rn.immisreg << DP_IMM_BIT)
           | (opcode_bit      << DP_OP_BIT)
           | (rn               )
           | (rd  << DP_RD_BIT)
           ;
    codegen_add_inst(ct, inst);
}

void codegen_ldr(struct codegen_table_st *ct, Instruction* instruction) {
    //ldr rd,[rn]       手册409
    //ldr rd,[rn,#1]

    //TODO writeback没处理
    //TODO ldrt,ldrb这种后缀还没处理
    const uint32_t RT_BIT = 12;
    const uint32_t RN_BIT = 16;
    const uint32_t OP_BIT = 25;
    const uint32_t U_BIT = 23;
    const uint32_t W_BIT = 21;

    uint32_t inst = 0;
    uint32_t cond_bit = codegen_cond_to_bit(instruction->cond);
    uint32_t opcode_bit = codegen_opcode_to_bit(instruction->opcode);

    inst = (cond_bit           << COND_BIT)
           |(instruction->rd.reg << RT_BIT)
           |(instruction->rn.reg << RN_BIT)
           |(opcode_bit        << OP_BIT)
           |(1                 << 20)
           |(0                 << 22)
            |(1                 << U_BIT)
            |(instruction->shift_amount)
           ;
    codegen_add_inst(ct, inst);
}

void codegen_str(struct codegen_table_st *ct, Instruction* instruction) {
    //str rn,[rd]
    //str rn,[rd]
    //str rn,[rd]!   手册675

    //TODO strb这种后缀还没处理
    const uint32_t RT_BIT = 12;
    const uint32_t RN_BIT = 16;
    const uint32_t OP_BIT = 25;
    const uint32_t U_BIT = 23;
    const uint32_t W_BIT = 21;

    uint32_t inst = 0;
    uint32_t cond_bit = codegen_cond_to_bit(instruction->cond);
    uint32_t opcode_bit = codegen_opcode_to_bit(instruction->opcode);

    inst = (cond_bit           << COND_BIT)
           |(instruction->rn.reg << RT_BIT)
           |(instruction->rd.reg << RN_BIT)
           |(opcode_bit        << OP_BIT)
           |(0                 << 20)        //str
           |(0                 << 22)
           |(1                 << U_BIT)        //U
           |(instruction->rd.writeback   << W_BIT)       //W
           |(instruction->shift_amount)
            ;
    codegen_add_inst(ct, inst);
}

void codegen_stmfd_ldmfd(struct codegen_table_st *ct, Instruction* instruction) {
    //stmfd rn ,rd      手册670
    const uint32_t RN_BIT  = 16;
    const uint32_t W_BIT = 21;
    const uint32_t OP_BIT = 22;
    uint32_t inst = 0;

    uint32_t cond_bit = codegen_cond_to_bit(instruction->cond);
    uint32_t opcode_bit = codegen_opcode_to_bit(instruction->opcode);

    inst = (cond_bit           << COND_BIT)
           | (instruction->rn.writeback << W_BIT)
           | (opcode_bit        << OP_BIT)
           | (instruction->rn.reg  << RN_BIT)
           | (instruction->rd.imm)
           ;

    if(instruction->opcode == ldmfd){
        inst |= 1 << 20;
    }
    codegen_add_inst(ct, inst);
}

void codegen_push_pop(struct codegen_table_st *ct, Instruction* instruction) {
    //push rn      手册540
    //pop rn       手册537v
    const uint32_t OP_BIT = 16;
    uint32_t inst = 0;

    uint32_t cond_bit = codegen_cond_to_bit(instruction->cond);
    uint32_t opcode_bit = codegen_opcode_to_bit(instruction->opcode);

    inst = (cond_bit           << COND_BIT)
            |(opcode_bit       << OP_BIT)
            |(instruction->rn.imm)
            ;
    codegen_add_inst(ct, inst);
}

void codegen_bx(struct codegen_table_st *ct, Instruction* instruction) {
    //bx rd     手册350页
    const uint32_t BX_CODE_BIT = 4;
    uint32_t cond_bit = codegen_cond_to_bit(instruction->cond);
    const uint32_t bx_code = 0b000100101111111111110001;

    uint32_t inst = (cond_bit << COND_BIT)
                    | (bx_code << BX_CODE_BIT)
                    | instruction->rd.reg;
    codegen_add_inst(ct, inst);
}

void codegen_b(struct codegen_table_st *ct, Instruction* instruction) {
    //b imm      手册332
    const uint32_t B_CODE_BIT = 4;
    const uint32_t OP_BIT = 24;
    uint32_t inst = 0;

    uint32_t cond_bit = codegen_cond_to_bit(instruction->cond);
    uint32_t opcode_bit = codegen_opcode_to_bit(instruction->opcode);

    inst = (cond_bit           << COND_BIT)
           |(opcode_bit       << OP_BIT)
           |(instruction->rd.imm)
            ;
    codegen_add_inst(ct, inst);
}

void codegen_bl(struct codegen_table_st *ct, Instruction* instruction) {
    //bl rd, bl imm         手册346
    uint32_t inst = 0;

    uint32_t cond_bit = codegen_cond_to_bit(instruction->cond);
    if(instruction->rd.isreg == 0) {
        //bl imm
        uint32_t opcode_bit = codegen_opcode_to_bit(instruction->opcode);
        inst = (cond_bit           << COND_BIT)
               |(opcode_bit       << 24)
               |(instruction->rn.imm)
               ;
    } else {
        //bl rd
        const uint32_t bx_code = 0b000100101111111111110011;
        inst = (cond_bit << COND_BIT)
                | (bx_code << 4)
                | instruction->rd.reg;
    }
    codegen_add_inst(ct, inst);
}

void codegen_movw_movt(struct codegen_table_st *ct, Instruction* instruction) {
    const uint32_t OP_BIT = 20;
    const uint32_t IMM4_BIT = 16;
    const uint32_t Rd_BIT = 12;
    uint32_t inst = 0;

    uint32_t cond_bit = codegen_cond_to_bit(instruction->cond);
    uint32_t opcode_bit = codegen_opcode_to_bit(instruction->opcode);

    //拿到低16位或高16位
    signed int imm16;
    if(instruction->opcode == movt){
        imm16 = instruction->rn.imm & 0xFFFF;
    } else{
        imm16 = (instruction->rn.imm >> 16) & 0xFFFF;
    }

    inst = (cond_bit                  << COND_BIT)
            | (opcode_bit             << OP_BIT)
            | (instruction->rd.reg    << Rd_BIT)
            | ((imm16 >> 12)          << IMM4_BIT)              //取16位中的前4位
            | (imm16 & 0xFFF)                                   //取16位中的后12位
            ;
    codegen_add_inst(ct, inst);
}

void codegen_inst(struct codegen_table_st *ct, Instruction* inst){
    switch (inst->opcode) {
        case add:
        case sub:
            codegen_simple_three_value(ct,inst);
            break;
        case mov:
            codegen_mov(ct,inst);
            break;
        case cmp:
            codegen_cmp(ct,inst);
            break;
        case ldr:
            codegen_ldr(ct,inst);
            break;
        case str:
            codegen_str(ct,inst);
            break;
        case ldmfd:
        case stmfd:
            codegen_stmfd_ldmfd(ct,inst);
            break;
        case push:
        case pop:
            codegen_push_pop(ct,inst);
            break;
        case bx:
            codegen_bx(ct,inst);
            break;
        case b:
            codegen_b(ct,inst);
            break;
        case bl:
            codegen_bl(ct,inst);
            break;
        case movt:
        case movw:
            codegen_movw_movt(ct,inst);
            break;
        case invalid:
            break;
    }
}

void codegen_print_hex(struct codegen_table_st *ct) {
    int i;

    printf("v2.0 raw\n");
    for (i = 0; i < ct->len; i++) {
        printf("%08X\n", ct->table[i]);
    }
}

void codegen_programme(struct codegen_table_st *ct){
    for(int i = 1; i < inst_list->size ;i++){
        Instruction cur_inst = inst_list->elements[i];
        codegen_inst(ct,&cur_inst);
    }
}

bool codegen_is_public_label(struct codegen_table_st *ct, struct codegen_global_pair *pl) {
    for (int i = 0; i < ct->public_count; i++) {
        if (!strcmp(ct->publics[i]->label, pl->label)) {
            return true;
        }
    }
    return false;
}

void codegen_elf_write(struct codegen_table_st *ct, char *path){
    elf_context elf;
    struct codegen_global_pair *pl;
    int binding;
    elf_init(&elf);

    addLabels(ct);

    //加入符号表
    for(int i = 0; i < ct->label_count; i++) {
        //TODO labels还没加东西
        //TODO lables和publics还没搞清楚
        pl = ct->labels[i];
        if (codegen_is_public_label(ct, pl)) {
            binding = STB_GLOBAL;
        } else {
            binding = STB_LOCAL;
        }
        elf_add_symbol(&elf, pl->label, pl->offset, binding);
    }

    //填充.text
    for(int i = 0; i < inst_list->size ;i++){
        elf_add_instr(&elf,ct->table[i]);
    }

    FILE *f = fopen(path, "w");
    if(!f) {
        perror(path);
        return;
    }

    if (!elf_write_file(&elf, f)) {
        printf("elf_write_file failed\n");
    }

    fclose(f);
    printf("path %s\n",path);
}