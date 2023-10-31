//
// Created by 12167 on 2023/9/27.
//

#include "instruction.h"

struct Instruction* newInstruction(enum Opcode opcode, enum COND cond,uint8_t rd, uint8_t rn, uint8_t rm, uint8_t rs, int32_t immediate, uint8_t shift_amount, char *label, char *branch_label) {
    if (rd >= 16 || rd < 0) assemblerException("Unrecognized register: %d", rd);
    if (rn >= 16 || rn < 0) assemblerException("Unrecognized register: %d", rs);
    if (rm >= 16 || rm < 0) assemblerException("Unrecognized register: %d", rm);
    if (rs >= 16 || rs < 0) assemblerException("Unrecognized register: %d", rs);
    if (shift_amount >= 32 || shift_amount < 0) assemblerException("shift amount out of range: %d", shift_amount);

    struct Instruction* r = malloc(sizeof (struct Instruction));
    r->opcode = opcode;
    r->cond = cond;

    strncpy(r->label, label, MAX_LABEL_LENGTH);
    strncpy(r->branch_label, branch_label, MAX_LABEL_LENGTH);

    return r;
}

struct Instruction* newInstructionBlank() {
    return newInstruction(add, AL,0, 0, 0, 0, 0, 0, "", "");
}