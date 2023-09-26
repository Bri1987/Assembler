//
// Created by 12167 on 2023/9/26.
//
#include "config.h"

void armasm_config_init(struct config_st *cp) {
    cp->sourcefile[0] = '\0';
    cp->source[0] = '\0';
    cp->source_len = 0;
    cp->hexfile[0] = '\0';
    cp->hexoutput = false;
    cp->objfile[0] = '\0';
    cp->objoutput = false;
    cp->debug = false;
}

void armasm_print_usage (void) {
    printf("usage: ./armasm [-h hexfile] [-o objname] <filename.s>\n");
    printf("  examples: ./armasm -h foo.hex foo.s\n");
    printf("  examples: ./armasm -o bar.o bar.s\n");
}

void armasm_parse_args(int argc, char **argv, struct config_st *cp) {
    int i = 1;

    if (argc == 1) {
        armasm_print_usage();
        exit(0);
    }

    while (i < (argc - 1)) {
        if (argv[i][0] == '-' && argv[i][1] == 'h' && (i + 1) < argc) {
            strncpy(cp->hexfile, argv[i + 1], SCAN_INPUT_LEN);
            cp->hexoutput = true;
            i += 2;
        } else if (argv[i][0] == '-' && argv[i][1] == 'd') {
            cp->debug = true;
            i += 1;
        } else if (argv[i][0] == '-' && argv[i][1] == 'o' && (i + 1) < argc) {
            strncpy(cp->objfile, argv[i + 1], SCAN_INPUT_LEN);
            cp->objoutput = true;
            i += 2;
        } else {
            printf("Invalid option: %s\n", argv[i]);
            exit(-1);
        }
    }

    if (i >= argc) {
        printf("Missing input source file.\n");
        armasm_print_usage();
        exit(-1);
    }

    strncpy(cp->sourcefile, argv[i], SCAN_INPUT_LEN);

    if (!(cp->hexoutput) && !(cp->objoutput)) {
        printf("At least one of -h or -o must be provided\n");
        exit(-1);
    }
}

void armasm_read_source(struct config_st *cp) {
    int fd;
    int rv;
    char buf[1];
    fd = open(cp->sourcefile, O_RDONLY);

    if (fd < 0) {
        printf("Cannot open source file");
        exit(-1);
    }

    while ((rv = read(fd, buf, 1)) > 0) {
        cp->source[cp->source_len] = buf[0];
        cp->source_len += 1;
    }

    if (rv < 0) {
        printf("Cannot read source file");
        exit(-1);
    }
}