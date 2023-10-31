#include <stdlib.h>
#include <stdio.h>
#include "AssemblerException.h"

void _assemblerException(char *filename, int lineno, char *format_message, ...) {
  va_list args;
  va_start(args, format_message);
  char message[256];
  vsprintf(message, format_message, args);
  fprintf(stderr, "%s:%d %s\n", filename, lineno, message);
  exit(1);
}
  
