//
// Created by 12167 on 2023/9/27.
//

#ifndef AS_FINAL_UTILITY_H
#define AS_FINAL_UTILITY_H

#include <stdio.h>
#include <malloc.h>
#include <string.h>
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
#include "lib/ArrayList.h"
#include "lib/hash_map.h"

char* extract_substring(const char* str, int start_index, int end_index);
void split_string(const char* str, char delimiter, char** part1, char** part2);
int compareStringLiterals(const char* str1, const char* str2);

#endif //AS_FINAL_UTILITY_H
