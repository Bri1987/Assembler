//
// Created by 12167 on 2023/9/26.
//

#ifndef AS_FINAL_CONFIG_H
#define AS_FINAL_CONFIG_H

#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define SCAN_TOKEN_LEN 64
#define SCAN_TOKEN_TABLE_LEN 1024
#define SCAN_INPUT_LEN 2048

struct config_st {
    char sourcefile[SCAN_INPUT_LEN];
    char source[SCAN_INPUT_LEN];
    int source_len;
    char hexfile[SCAN_INPUT_LEN];
    bool hexoutput;
    char objfile[SCAN_INPUT_LEN];
    bool objoutput;
    bool debug;
};

void armasm_config_init(struct config_st *cp);
void armasm_print_usage (void);
void armasm_parse_args(int argc, char **argv, struct config_st *cp);
void armasm_read_source(struct config_st *cp);

#endif //AS_FINAL_CONFIG_H
