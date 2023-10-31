//
// Created by 12167 on 2023/9/27.
//

#include "utility.h"

char* extract_substring(const char* str, int start_index, int end_index) {
    if (str == NULL || start_index > end_index || start_index < 0 || end_index >= strlen(str)) {
        return NULL; // 输入无效
    }

    int substring_length = end_index - start_index + 1;
    char* substring = (char*)malloc((substring_length + 1) * sizeof(char));
    if (substring == NULL) {
        return NULL; // 内存分配失败
    }

    strncpy(substring, str + start_index, substring_length);
    substring[substring_length] = '\0';

    return substring;
}

void split_string(const char* str, char delimiter, char** part1, char** part2) {
    if (str == NULL || part1 == NULL || part2 == NULL) {
        return; // 输入无效
    }

    char* delimiter_ptr = strchr(str, delimiter);
    if (delimiter_ptr == NULL) {
        *part1 = strdup(str); // 未找到分隔符，直接复制整个字符串作为part1
        *part2 = NULL; // part2为空
    } else {
        int delimiter_index = delimiter_ptr - str;
        int str_length = strlen(str);

        *part1 = (char*)malloc((delimiter_index + 1) * sizeof(char));
        if (*part1 != NULL) {
            strncpy(*part1, str, delimiter_index);
            (*part1)[delimiter_index] = '\0';
        }

        int part2_length = str_length - delimiter_index;
        *part2 = (char*)malloc((part2_length + 1) * sizeof(char));
        if (*part2 != NULL) {
            strncpy(*part2, str + delimiter_index + 1, part2_length);
            (*part2)[part2_length] = '\0';
        }
    }
}

int compareStringLiterals(const char* str1, const char* str2) {
    if (str1 == str2) {
        printf("String literals are equal.\n");
        return 0;
    } else {
        printf("String literals are not equal.\n");
        return -1;
    }
}

