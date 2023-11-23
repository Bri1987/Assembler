//
// Created by 12167 on 2023/9/27.
//
#include "scan.h"
#include "./lib/ArrayList.h"
#include "instruction.h"
#include "scan.h"
#include "utility.h"
#include "parse.h"

extern struct ArrayList *inst_list;
extern HashMap *labelMap;
extern HashMap *globVMap;
extern HashSet *globLabelSet;

HashSet *b_inst_set;
HashSet *global_dataSet;           //存与全局变量相关指令的
int pc_count;

char_opcode_pair parse_char_opcode_map[] = PARSE_OPCODE_MAP;
print_opcode_pair print_opcode_map[] = PRINT_OPCODE_MAP;
struct parse_reg_pair_st parse_reg_map[] = PARSE_REG_PAIR_MAP;
parse_cond_pair parse_cond_map[] = PARSE_COND_MAP;

bool data_reach = false;
bool deal_global = false;          //标记是不是正在处理全局变量
codegen_global_pair *cur_pair = NULL;

void parse_eols(struct scan_table_st *st) {
    while (scan_table_accept(st, TK_EOL));
}

enum Opcode parse_char_to_opcode(char *s) {
    int i;
    int map_len = sizeof(parse_char_opcode_map) / sizeof(parse_char_opcode_map[0]);

    for (i = 0; i < map_len; i++) {
        if (strncmp(s, parse_char_opcode_map[i].name, SCAN_TOKEN_LEN) == 0) {
            return parse_char_opcode_map[i].opcode;
        }
    }

    return invalid;
}

enum COND parse_char_to_cond(char *s) {
    int i;
    int map_len = sizeof(parse_cond_map) / sizeof(parse_cond_map[0]);

    for (i = 0; i < map_len; i++) {
        if (strncmp(s, parse_cond_map[i].name, SCAN_TOKEN_LEN) == 0) {
            return parse_cond_map[i].cond;
        }
    }

    return AL;
}

int parse_char_to_reg(char *s) {
    int i;
    int reg_map_len = sizeof(parse_reg_map) / sizeof(parse_reg_map[0]);

    for (i = 0; i < reg_map_len; i++) {
        if (strncmp(s, parse_reg_map[i].name, SCAN_TOKEN_LEN) == 0) {
            return parse_reg_map[i].num;
        }
    }

    //TODO 走到这里来说明是多寄存器

    return -1;
}

void parse_opcode(struct scan_token_st *tp, Instruction* instruction) {
    //TODO movw在哪里处理
    enum Opcode opcode = parse_char_to_opcode(tp->value);
    if(opcode != invalid) {
        //无cond
        instruction->opcode = opcode;
    } else {
        //last 2 characters是不是后缀，先找后缀
        char* suffix = extract_substring(tp->value, strlen(tp->value) - 2,strlen(tp->value) - 1);
        enum COND cond = parse_char_to_cond(suffix);
        if(cond != AL){
            instruction->cond = cond;
            //拿到前面的opcode
            char* c_op = extract_substring(tp->value,0,strlen(tp->value) - 3);
            instruction->opcode = parse_char_to_opcode(c_op);
        }
    }
}

//0-15位, 包含哪一个寄存器，哪一位就为1
//传入 {
signed int handle_multi_reglist(struct scan_table_st *st, int begin){
    int val = 0;
    struct scan_token_st *cur;
    int reg ;
    while ((cur = scan_table_get(st,begin))->id != TK_RPAR){
        if(cur->id != TK_COMMA) {
            if((reg = parse_char_to_reg(cur->value)) < 0) {
                //多寄存器
                char* begin_reg, *end_reg;
                split_string(cur->value,'-',&begin_reg,&end_reg);
                int b_reg = parse_char_to_reg(begin_reg);
                int e_reg = parse_char_to_reg(end_reg);

                for(int i = b_reg ; i<=e_reg ;i++){
                    val |= 1 << i;
                }
            } else {
                val |= 1 << reg;
            }
        }
        begin++;
    }

    /* Accept instruction tokens */
    scan_table_accept_any_n(st, begin + 1);
    return val;
}

void handle_b_inst(){
    HashSetFirst(b_inst_set);
    for(Instruction* instruction = HashSetNext(b_inst_set); instruction!=NULL; instruction = HashSetNext(b_inst_set)) {
        int *pc = HashMapGet(labelMap,instruction->branch_label);
        if(pc == NULL){
            pc = returnLabel(instruction->branch_label,labelMap);
        }
        instruction->rd.imm = *pc;
    }
}

void handle_global() {
    //将地址放到rn.imm
    HashSetFirst(global_dataSet);
    for(Instruction* instruction = HashSetNext(global_dataSet); instruction!=NULL; instruction = HashSetNext(global_dataSet)) {
        //TODO 可能找不到
        codegen_global_pair *p = HashMapGet(globVMap,instruction->branch_label);
        if(p == NULL)
            p = returnGlobV(instruction->branch_label,globVMap);
        assert(p != NULL);       //TODO 先这样吧
        instruction->rn.imm = p->offset;
    }
}

bool getLabel(char* label,HashMap* map){
    HashMapFirst(map);
    for(Pair* pair = HashMapNext(map); pair!=NULL; pair = HashMapNext(map)){
        if(strcmp(label,pair->key)==0)
            return true;
    }
    return false;
}

int* returnLabel(char* label,HashMap* map){
    HashMapFirst(map);
    for(Pair* pair = HashMapNext(map); pair!=NULL; pair = HashMapNext(map)){
        if(strcmp(label,pair->key)==0)
            return pair->value;
    }
    return NULL;
}

codegen_global_pair * returnGlobV(char* label,HashMap* map){
    HashMapFirst(map);
    for(Pair* pair = HashMapNext(map); pair!=NULL; pair = HashMapNext(map)){
        if(strcmp(label,pair->key)==0)
            return pair->value;
    }
    return NULL;
}

void parse_program(struct scan_table_st *st){
    pc_count = 0;
    b_inst_set = HashSetInit();
    global_dataSet = HashSetInit();

    int count = 0;
    while (!scan_table_accept(st, TK_EOT)) {
        Instruction instruction;
        Instruction *inst = parse_instruction(st);

        if(inst != NULL){
            instruction = *inst;
            addLast(inst_list, instruction);
        }
        parse_eols(st);

        count++;
        printf("%d\n",count);
    }
    handle_b_inst();
    handle_global();
}

bool is_three_value_opcode(enum Opcode opcode) {
    if(opcode == add || opcode == sub || opcode == mul)
        return true;
    return false;
}

bool is_shift(enum Opcode opcode){
    if(opcode == lsl)
        return true;
    return false;
}

struct Instruction* parse_instruction(struct scan_table_st *st){
    Instruction* instruction = NULL;
    struct scan_token_st *tp0, *tp1, *tp2, *tp3, *tp4, *tp5, *tp6, *tp7,*tp8;
    tp0 = scan_table_get(st, 0);
    tp1 = scan_table_get(st, 1);
    tp2 = scan_table_get(st, 2);
    //.text , .arch armv7ve
    //TODO 需要的特殊的, 比如.data这种还没处理
    //TODO 伪指令
    if(tp0->id == TK_DIR) {
        if(strcmp(tp0->value,"data") == 0){
            data_reach = true;
            /* Accept instruction tokens */
            scan_table_accept_any_n(st, 1);
        } else if(strcmp(tp0->value,"bss") == 0){
            //TODO bss处理

            deal_global = false;
            /* Accept instruction tokens */
            scan_table_accept_any_n(st, 1);
        } else if(strcmp(tp0->value,"align") == 0 || strcmp(tp0->value,"global") == 0) {
            HashSetAdd(globLabelSet,tp1->value);

            /* Accept instruction tokens */
            scan_table_accept_any_n(st, 2);
        }
        else if(deal_global) {
            //pc其实已经没用了，但是还是用pc_count算偏移
            if(strcmp(tp0->value,"long") == 0){
                pc_count += 4;
                if(tp2->id == TK_IMM)
                    cur_pair->num[cur_pair->occupy/4] = atoi(tp2->value);
                cur_pair->occupy += 4;
            } else if(strcmp(tp0->value,"zero") == 0) {
                int occupy = atoi(tp2->value);
                pc_count += occupy;
                cur_pair->occupy += occupy;
            }
            /* Accept instruction tokens */
            scan_table_accept_any_n(st, 2);
        }

    } else if (tp0->id == TK_IDENT && tp1->id == TK_COLON && !data_reach) {
        instruction = newInstructionBlank();
        strncpy(instruction->label, tp0->value, SCAN_TOKEN_LEN);

        int *cur_pc = malloc(4);
        *cur_pc = pc_count;
        HashMapPut(labelMap,tp0->value,cur_pc);

        //Accept the two tokens
        scan_table_accept_any_n(st, 2);
    } else if(tp0->id == TK_IDENT && tp1->id == TK_COLON && data_reach) {
        //是全局变量了
        codegen_global_pair* pair = (codegen_global_pair*) malloc(sizeof(codegen_global_pair));
        pair->offset = pc_count;
        HashMapPut(globVMap,tp0->value,pair);
        cur_pair = pair;
        deal_global = true;

        //Accept the two tokens
        scan_table_accept_any_n(st, 2);
    } else {
        instruction = newInstructionBlank();
        parse_opcode(tp0,instruction);

        tp3 = scan_table_get(st, 3);
        tp4 = scan_table_get(st, 4);
        tp5 = scan_table_get(st, 5);
        tp6 = scan_table_get(st, 6);
        tp7 = scan_table_get(st, 7);
        tp8 = scan_table_get(st, 8);

        //判断是不是跳转
        if(instruction->opcode == b) {
            //不能跳转寄存器
            strcpy(instruction->branch_label,tp1->value);

            HashSetAdd(b_inst_set,instruction);

            /* Accept instruction tokens */
            scan_table_accept_any_n(st, 2);
            pc_count += 4;
        }
        else if(instruction->opcode == bx){
            //bx rd
            Operand rd = {0, 0, 0, 0, 0};
            rd.isreg = 1;
            rd.reg = parse_char_to_reg(tp1->value);
            instruction->rd = rd;
            /* Accept instruction tokens */
            scan_table_accept_any_n(st, 2);
            pc_count += 4;
        }
        else if(instruction->opcode == bl) {
            //bl rd
            Operand rd= {0, 0, 0, 0, 0};
            int ret;
            if((ret = parse_char_to_reg(tp1->value)) < 0){
                //是imm
                rd.isreg = 0;

                if(getLabel(tp1->value,labelMap)){
                    int *b_pc = HashMapGet(labelMap,tp1->value);
                    if(b_pc == NULL)
                        b_pc = returnLabel(tp1->value,labelMap);
                    rd.imm = *b_pc;
                    rd.isreg = 0;
                } else {
                    //还没存到这里
                    strcpy(instruction->branch_label,tp1->value);
                    HashSetAdd(b_inst_set,instruction);
                }
            } else{
                rd.reg = ret;
                rd.isreg = 1;
            }
            instruction->rd = rd;
            /* Accept instruction tokens */
            scan_table_accept_any_n(st, 2);
            pc_count += 4;
        }
            //判断是不是push pop
        else if(instruction->opcode == push || instruction->opcode == pop){
            //push rn
            if(tp1->id == TK_LPAR) {
                Operand rn= {0, 0, 0, 0, 0};
                rn.imm = handle_multi_reglist(st,2);
                rn.isreg = 0;
                rn.immisreg = 1;
                instruction->rn = rn;

                //自动调整了st
            }
            pc_count += 4;
        }
        else if(instruction->opcode == cmp && tp2->id == TK_COMMA && tp4->id== TK_EOL){
            //rn
            Operand rn= {0, 0, 0, 0, 0};
            rn.reg = parse_char_to_reg(tp3->value);
            rn.isreg = 1;

            //rm
            Operand rm= {0, 0, 0, 0, 0};
            if(tp5->id == TK_IMM){
                rm.imm = atoi(tp3->value);
                rm.immisreg = 0;
            } else {
                rm.reg = parse_char_to_reg(tp3->value);
                rm.isreg = 1;
            }

            instruction->rn = rn;
            instruction->rm = rm;

            /* Accept instruction tokens */
            scan_table_accept_any_n(st, 4);
            pc_count += 4;
        }
        else if(instruction->opcode == mov && tp2->id == TK_COMMA ){
            //rd
            Operand rd= {0, 0, 0, 0, 0};
            rd.reg = parse_char_to_reg(tp3->value);
            rd.isreg = 1;

            //rn
            Operand rn= {0, 0, 0, 0, 0};
            if(tp5->id == TK_IMM){
                rn.imm = atoi(tp3->value);
                rn.immisreg = 0;
            } else {
                rn.reg = parse_char_to_reg(tp3->value);
                rn.isreg = 1;
            }

            instruction->rn = rn;
            instruction->rd = rd;

            /* Accept instruction tokens */
            scan_table_accept_any_n(st, 4);
            pc_count += 4;
        }
        else if(instruction->opcode == movt || instruction->opcode == movw) {
            //TODO 情况没处理完
            //movt    r1,#0xffff
            //movw    r8,#:lower16:cns
            //rd
            Operand rd= {0, 0, 0, 0, 0};
            rd.reg = parse_char_to_reg(tp3->value);
            rd.isreg = 1;

            if(tp3->id == TK_COLON) {
                //一定还没读到.data
                //TODO 暂时借用一下Branch_label
                strcpy(instruction->branch_label,tp6->value);
                HashSetAdd(global_dataSet,instruction);

                /* Accept instruction tokens */
                scan_table_accept_any_n(st, 7);
            } else {
                //TODO 处理一下0x
                scan_table_accept_any_n(st, 7);
            }
            pc_count += 4;
        }
        else if(instruction->opcode == str){
            //str rn,[rd]

            //rn
            Operand rn= {0, 0, 0, 0, 0};
            rn.reg = parse_char_to_reg(tp3->value);
            rn.isreg = 1;

            Operand rd= {0, 0, 0, 0, 0};
            rd.reg = parse_char_to_reg(tp4->value);
            rd.isreg = 1;
            instruction->rn = rn;
            instruction->rd = rd;

            if(tp6->id == TK_IMM) {
                instruction->shift_amount = atoi(tp6->value);
                if(tp8->id == TK_EXLCA) {
                    rd.writeback = 1;
                    /* Accept instruction tokens */
                    scan_table_accept_any_n(st, 9);
                } else {
                    /* Accept instruction tokens */
                    scan_table_accept_any_n(st, 8);
                }
            } else {
                /* Accept instruction tokens */
                scan_table_accept_any_n(st, 6);
            }
            pc_count += 4;
        }
        else if(instruction->opcode == ldr){
            //ldr rd,[rn]
            //ldr rd,[rn,#1]

            Operand rn= {0, 0, 0, 0, 0};
            rn.reg = parse_char_to_reg(tp4->value);
            rn.isreg = 1;

            Operand rd= {0, 0, 0, 0, 0};
            rd.reg = parse_char_to_reg(tp1->value);
            rd.isreg = 1;
            instruction->rn = rn;
            instruction->rd = rd;

            //看有没有shift
            if(tp6->id == TK_IMM) {
                instruction->shift_amount = atoi(tp6->value);

                /* Accept instruction tokens */
                scan_table_accept_any_n(st, 8);
            }
            else {
                /* Accept instruction tokens */
                scan_table_accept_any_n(st, 6);
            }
            pc_count += 4;
        }
        else if(instruction->opcode == stmfd || instruction->opcode == ldmfd) {
            //TODO !又单列了，得改
            //stmfd rn ,rd
            //ldmfd rn ,rd
            Operand rn= {0, 0, 0, 0, 0};
            rn.isreg = true;
            rn.reg = parse_char_to_reg(tp1->value);
            int begin = 4;
            if(tp2->id == TK_EXLCA){
                rn.writeback = 1;
                begin++;
            }

            //TODO 应该一定是multi_reg吧
            int val = handle_multi_reglist(st, begin);
            Operand rd= {0, 0, 0, 0, 0};
            rd.immisreg = 1;
            rd.imm = val;
            instruction->rn = rn;
            instruction->rd = rd;
            pc_count += 4;
        }

            //add,sub
        else if(is_three_value_opcode(instruction->opcode) && tp2->id == TK_COMMA && tp4->id == TK_COMMA){
            //rd
            Operand rd= {0, 0, 0, 0, 0};
            rd.reg = parse_char_to_reg(tp1->value);
            rd.isreg = 1;
            //rn
            Operand rn= {0, 0, 0, 0, 0};
            rn.reg = parse_char_to_reg(tp3->value);
            rn.isreg = 1;

            //rm
            Operand rm= {0, 0, 0, 0, 0};
            if(tp5->id == TK_IMM){
                rm.imm = atoi(tp5->value);
                rm.immisreg = 0;
                instruction->imm = 1;
            } else {
                rm.reg = parse_char_to_reg(tp5->value);
                rm.isreg = 1;
            }

            instruction->rd = rd;
            instruction->rn = rn;
            instruction->rm = rm;

            /* Accept instruction tokens */
            scan_table_accept_any_n(st, 6);
            pc_count += 4;
        } else if(is_shift(instruction->opcode)){
            //rd
            Operand rd= {0, 0, 0, 0, 0};
            rd.reg = parse_char_to_reg(tp1->value);
            rd.isreg = 1;
            //rn
            Operand rn= {0, 0, 0, 0, 0};
            if(tp3->id == TK_IMM){
                rn.imm = atoi(tp3->value);
                rn.immisreg = 0;
            } else {
                rn.reg = parse_char_to_reg(tp3->value);
                rn.isreg = 1;
            }

            //rm
            Operand shift= {0, 0, 0, 0, 0};
            if(tp5->id == TK_IMM){
                shift.imm = atoi(tp5->value);
                shift.immisreg = 0;
            } else {
                shift.reg = parse_char_to_reg(tp5->value);
                shift.isreg = 1;
            }

            instruction->rd = rd;
            instruction->rn = rn;
            instruction->shift = shift;

            /* Accept instruction tokens */
            scan_table_accept_any_n(st, 6);
            pc_count += 4;
        }
    }
    return instruction;
}

char* print_opcode(enum Opcode opcode) {
    int i;
    int map_len = sizeof(print_opcode_map) / sizeof(print_opcode_map[0]);

    for (i = 0; i < map_len; i++) {
        if(opcode == print_opcode_map[i].opcode)
            return print_opcode_map[i].name;
    }

    return NULL;
}

//TODO 打印是有问题的
void print_instruction(){
    for(int i = 0; i < inst_list->size ;i++){
        struct Instruction instruction = inst_list->elements[i];
        if(instruction.rd.immisreg){
            if(instruction.rm.imm)
                printf("%s rd :%d, rn :%d, rm :%d, imm :%d\n",print_opcode(instruction.opcode),instruction.rd.imm,instruction.rn.reg,instruction.rm.imm,instruction.imm);
            else
                printf("%s rd :%d, rn :%d, rm :%d, imm :%d\n",print_opcode(instruction.opcode),instruction.rd.imm,instruction.rn.reg,instruction.rm.reg,instruction.imm);
        }
        else {
            if(instruction.rm.imm)
                printf("%s rd :%d, rn :%d, rm :%d, imm :%d\n",print_opcode(instruction.opcode),instruction.rd.reg,instruction.rn.reg,instruction.rm.imm,instruction.imm);
            else
                printf("%s rd :%d, rn :%d, rm :%d, imm :%d\n",print_opcode(instruction.opcode),instruction.rd.reg,instruction.rn.reg,instruction.rm.reg,instruction.imm);
        }
    }
}