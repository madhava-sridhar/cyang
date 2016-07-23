#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <libgen.h>
#include <getopt.h>
#include <string.h>

#define YYPARSE yang_parse
// #define YY_DEBUG
#define YPERROR(S)  yperror(S, yytext)

#define STREQ(A,B) (0==strcmp((A),(B)))

typedef struct _globals {
    bool linkage_seen;
    bool meta_seen;
    bool revision_seen;
    char *filename;
    int linenumber;
    FILE *fp;
    int errcnt;
} globals_t;

static globals_t G = {false, false, false, NULL, 1, NULL, 0};

#define YY_INPUT(buf, result, max)              \
  {                                             \
    int c = getc(G.fp);                         \
    if (c == '\n' || c == '\r') ++G.linenumber; \
    result = (c == EOF) ? 0 : (*(buf) = c, 1);  \
  }

static void modname_chk(const char *name);
static void yperror(const char *s, const char *tok);
static void linkage_seen(const char *s);
static void meta_seen(const char *s);
static void revision_seen(void);
static void revision_date_chk(const char *r);
static void ensure_single(int c, const char *m);
