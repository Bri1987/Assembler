#include <stdio.h>
#include "as/scan.h"
#include "as/config.h"

int main(int argc, char **argv) {
    struct scan_table_st scan_table;
    struct config_st config;
    armasm_config_init(&config);
    armasm_parse_args(argc, argv, &config);
    armasm_read_source(&config);
    scan_table_init(&scan_table);
    scan_table_scan(&scan_table, config.source, config.source_len);
    scan_table_print(&scan_table);
    return 0;
}
