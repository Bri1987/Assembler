//
// Created by 12167 on 2023/9/26.
//
#include "scan.h"

char *scan_token_strings[] = SCAN_TOKEN_STRINGS;

void scan_error(char *err) {
    printf("scan_error: %s\n", err);
    exit(-1);
}

void scan_table_init(struct scan_table_st *st) {
    st->len = 0;
    st->next = 0;
}

struct scan_token_st * scan_table_new_token(struct scan_table_st *st) {
    struct scan_token_st *tp;

    tp = &st->table[st->len];
    st->len += 1;

    return tp;
}

char * scan_read_token(struct scan_token_st *tp, char *p, int len,
                       enum scan_token_enum id) {
    /* Read a token starting a p for len characters.
       Update the given token with the token string and token id. */
    int i;

    tp->id = id;
    for (i = 0; i < len; i++) {
        tp->value[i] = *p;
        p += 1;
    }
    tp->value[i] = '\0';
    return p;
}

bool scan_is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool scan_is_digit(char c) {
    return c >= '0' && c <= '9';
}

char *scan_c_comment(char *p, char *end) {
    while (p < end) {
        if ((*p == '*') && (*(p + 1) == '/')) {
            p = p + 2;
            break;
        }
        p += 1;
    }
    return p;
}

bool scan_is_whitespace(char c) {
    return (c == ' ' || c == '\t' || c == '\r');
}

char * scan_whitespace(char *p, char *end) {
    while (scan_is_whitespace(*p) && (p < end)) {
        p += 1;
    }
    return p;
}

char * scan_ident(char *p, char *end, struct scan_token_st *tp) {
    int i = 0;
    while ((scan_is_letter(*p) || scan_is_digit(*p) || *p == '_'  || *p == '-') && (p < end)) {
        tp->value[i] = *p;
        i += 1;
        p += 1;
    }
    tp->value[i] = '\0';
    tp->id = TK_IDENT;
    return p;
}

char * scan_signed_integer(char *p, char *end, struct scan_token_st *tp) {
    int i = 0;

    /* Detect minus sign */
    if (*p == '-') {
        tp->value[i] = '-';
        p += 1;
        i += 1;
    }

    while (scan_is_digit(*p) && p < end) {
        tp->value[i] = *p;
        p += 1;
        i += 1;
    }
    tp->value[i] = '\0';
    tp->id = TK_IMM;
    return p;
}

char * scan_token(char *p, char *end, struct scan_token_st *tp) {
    if (p == end) {
        p = scan_read_token(tp, p, 0, TK_EOT);
    } else if ((*p == '/') && (*(p + 1) == '*')) {
        /* Ignore C-style comments */
        p = scan_c_comment(p, end);
        p = scan_token(p, end, tp);
    } else if (scan_is_whitespace(*p)) {
        p = scan_whitespace(p, end);
        p = scan_token(p, end, tp);
    } else if (scan_is_letter(*p)) {
        p = scan_ident(p, end, tp);
    } else if(scan_is_digit(*p)) {
        p = scan_signed_integer(p, end, tp);
    } else if (*p == '#') {
        if(*(p+1) != ':'){
            p = scan_signed_integer(p + 1, end, tp);
        } else {
            //TODO 不确定怎么处理
            // movw    r3,#:lower16:g
            // movt    r3,#:upper16:g
            p = p + 1;
            p = scan_token(p, end, tp);
        }
    } else if (*p == '.') {
        p = p + 1;
        p = scan_ident(p, end, tp);
        tp->id = TK_DIR;
    } else if (*p == ',') {
        p = scan_read_token(tp, p, 1, TK_COMMA);
    } else if (*p == '[') {
        p = scan_read_token(tp, p, 1, TK_LBRACKET);
    } else if (*p == ']') {
        p = scan_read_token(tp, p, 1, TK_RBRACKET);
    } else if (*p == ':') {
        p = scan_read_token(tp, p, 1, TK_COLON);
    } else if(*p == '{') {
        p = scan_read_token(tp, p, 1, TK_LPAR);
    } else if(*p == '}') {
        p = scan_read_token(tp, p, 1, TK_RPAR);
    }
//    else if(*p == '-') {
//        p = scan_read_token(tp, p, 1, TK_RANGE);
//    }
    else if(*p == '!') {
        p = scan_read_token(tp, p, 1, TK_EXLCA);
    }
    else if (*p == '\n') {
        p = scan_read_token(tp, p, 1, TK_EOL);
    }  else {
//        printf("%c",*p);
//        scan_error("Invalid character");
        //TODO 跳过处理
        p += 1;
        p = scan_token(p, end, tp);
    }

    return p;
}

void scan_table_scan(struct scan_table_st *st, char *input, int len) {
    struct scan_token_st *tp;
    char *p = input;
    char *end = p + len;

    while(true) {
        tp = scan_table_new_token(st);
        p = scan_token(p, end, tp);
        if (tp->id == TK_EOT) {
            break;
        }
    }
}

void scan_token_print(struct scan_token_st *t) {
    char *token_str;

    if (t->value[0] == '\n') {
        token_str = "\\n";
    } else {
        token_str = t->value;
    }

    printf("%s(\"%s\")\n", scan_token_strings[t->id], token_str);
}

void scan_table_print(struct scan_table_st *st) {
    int i;

    for (i = 0; i < st->len; i++) {
        scan_token_print(&st->table[i]);
    }
}

struct scan_token_st scan_token_none = {TK_NONE, ""};

struct scan_token_st * scan_table_get(struct scan_table_st *st, int i) {
    if (st->next + i > (st->len - 1)) {
        return &scan_token_none;
    } else {
        return &(st->table[st->next + i]);
    }
}

bool scan_table_accept(struct scan_table_st *st, enum scan_token_enum tk_expected) {
    struct scan_token_st *tp;

    if (tk_expected == TK_ANY) {
        st->next += 1;
        return true;
    }

    tp = scan_table_get(st, 0);

    if (tp->id == tk_expected) {
        st->next += 1;
        return true;
    }

    return false;
}

void scan_table_accept_any_n(struct scan_table_st *st, int n) {
    int i;

    for (i = 0; i < n; i++) {
        scan_table_accept(st, TK_ANY);
    }
}
