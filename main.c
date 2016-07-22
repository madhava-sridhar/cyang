static void
yperror (const char *s, const char *tok)
{
    fprintf(stderr, "Oops! expected %s (last seen: %s)\n", s, tok);
}

static void
linkage_seen (const char *stmt)
{
    G.linkage_seen = true;
    if (G.meta_seen) {
        fprintf(stderr, "Error: linkage-statement (%s) should be "
                "placed before meta-stmts(org/contact/desc/rev)", stmt);
    }
    if (G.revision_seen) {
        fprintf(stderr, "Error: linkage-statement (%s) should be "
                "placed before revisions\n", stmt);
    }
}

static void
meta_seen (const char *stmt)
{
    G.meta_seen = true;
    if (G.revision_seen) {
        fprintf(stderr, "Error: meta-statement (%s) should be before revision(s)\n", stmt);
    }
}

static void
revision_seen ()
{
    G.revision_seen = true;
}

static void
revision_date_chk (const char *r)
{
    static char last_rev[12] = {0};
    if (last_rev[0] == '\0') {
        strcpy(last_rev, r);
    } else {
        if (strcmp(last_rev, r) < 1) {
            fprintf(stderr, "revision dates not in reverse chronological"
                    " order (%s > %s)\n", r, last_rev);
        }
    }
}

static void
ensure_single (int c, const char *m)
{
#define MAX 10
    static int a[MAX] = {0};
    assert(c < MAX);
    a[c]++;
    if (a[c] > 1) {
        fprintf(stderr, "Error: %s can occur only once\n", m);
    }
}

int
main (int argc, char *argv[])
{
    if (!yang_parse()) {
        printf("Syntax error\n");
        return 1;
    }
    printf("Parse OK\n");
    return 0;
}
