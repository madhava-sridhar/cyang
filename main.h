#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define YYPARSE yang_parse
// #define YY_DEBUG
#define YPERROR(S)  yperror(S, yytext)

typedef struct _globals {
    bool linkage_seen;
    bool meta_seen;
    bool revision_seen;
} globals_t;

static globals_t G = {false, false, false};

static void yperror(const char *s, const char *tok);
static void linkage_seen(const char *s);
static void meta_seen(const char *s);
static void revision_seen(void);
static void revision_date_chk(const char *r);
static void ensure_single(int c, const char *m);
