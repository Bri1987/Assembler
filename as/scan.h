//
// Created by 12167 on 2023/9/26.
//

#ifndef AS_FINAL_SCAN_H
#define AS_FINAL_SCAN_H

/*
 * scan.c
 */

/*

 //TODO 感叹号的处理 ldmfd sp!,{r4-r12}

# Scanner EBNF for ARM Assembly

tokenlist   ::= (token)*
token       ::= ident | imm | symbol
directive   ::= "." ident
ident       ::= letter (letter | digit | '_')*
imm         ::= '#' ('-')+ digit (digit)*
symbol      ::= '[' | ']' | ',' | ':' | '{' | '}' | '-' | '!'
letter      ::= 'a' | ... | 'z' | 'A' | ... | 'Z'
digit       ::= '0' | ... | '9'
eol         ::= '\n'

# Ignore

comment     ::= @ (char)* eol | '/' '*' (char | eol)* '*' '/'
whitespace  ::=  (' ' | '\t') (' ' | '\t')*

*/

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

enum scan_token_enum {
    TK_IDENT,    /* (identifier) add, sub, mul, foo, r0, r1, lr, ... */
    TK_DIR,      /* (directive) .global */
    TK_IMM,      /* #1, #-2, ... */
    TK_COMMA,    /* , */
    TK_LBRACKET, /* [ */
    TK_RBRACKET, /* ] */
    TK_COLON,    /* : */
    TK_LPAR,     /* { */
    TK_RPAR,     /* } */
    TK_RANGE,    /* - */
    TK_EXLCA,    /* ! */
    TK_EOL,      /* End of line */
    TK_EOT,      /* End of text */
    TK_NONE,     /* Invalid token */
    TK_ANY,      /* Special whilecard token for parsing */
};


#define SCAN_TOKEN_STRINGS {"IDENT", "DIR", "IMM", "COMMA", \
                            "LBRACKET", "RBRACKET", "COLON", "LPAR", "RPAR", \
                            "RANGE", "EXCLA"                                \
                            "EOL", "EOT", "NONE", "ANY"};

struct scan_token_st {
    enum scan_token_enum id;
    char value[SCAN_TOKEN_LEN];
};

struct scan_table_st {
    struct scan_token_st table[SCAN_TOKEN_TABLE_LEN];
    int len;
    int next;
};

void scan_table_init(struct scan_table_st *st);
struct scan_token_st * scan_table_new_token(struct scan_table_st *st);
void scan_table_scan(struct scan_table_st *st, char *input, int len);
void scan_table_print(struct scan_table_st *st);
void scan_error(char *err);
char * scan_read_token(struct scan_token_st *tp, char *p, int len,
                       enum scan_token_enum id);
bool scan_is_letter(char c);
bool scan_is_digit(char c);
char *scan_c_comment(char *p, char *end);
bool scan_is_whitespace(char c);
char * scan_whitespace(char *p, char *end);
char * scan_ident(char *p, char *end, struct scan_token_st *tp);
char * scan_signed_integer(char *p, char *end, struct scan_token_st *tp);
char * scan_token(char *p, char *end, struct scan_token_st *tp);
void scan_token_print(struct scan_token_st *t);
bool scan_table_accept(struct scan_table_st *st, enum scan_token_enum tk_expected);
void scan_table_accept_any_n(struct scan_table_st *st, int n);

#endif //AS_FINAL_SCAN_H
