#pragma once
#include <stdarg.h>

void _assemblerException(char *filename, int lineno, char *format_message, ...);

#define assemblerException(...) _assemblerException(__FILE__, __LINE__, __VA_ARGS__) 



