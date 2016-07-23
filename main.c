static void
yperror (const char *s, const char *tok)
{
    fprintf(stderr, "Oops! expected %s (last seen: %s)\n", s, tok);
}

static void
modname_chk (const char *modname)
{
    char *file = strdup(G.filename);
    char *name = basename(file);
    char *p    = strrchr(name, '.');

    *p = '\0';
    if (!STREQ(name, modname)) {
        fprintf(stderr, "Error: module name '%s' not same as file name '%s'\n",
                modname, name);
        G.errcnt++;
    }
}

static void
linkage_seen (const char *stmt)
{
    G.linkage_seen = true;
    if (G.meta_seen) {
        fprintf(stderr, "Error: linkage-statement (%s) should be "
                "placed before meta-stmts(org/contact/desc/rev)", stmt);
        G.errcnt++;
    }
    if (G.revision_seen) {
        fprintf(stderr, "Error: linkage-statement (%s) should be "
                "placed before revisions\n", stmt);
        G.errcnt++;
    }
}

static void
meta_seen (const char *stmt)
{
    G.meta_seen = true;
    if (G.revision_seen) {
        fprintf(stderr, "Error: meta-statement (%s) should be "
                        "before revision(s)\n", stmt);
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
        G.errcnt++;
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
        G.errcnt++;
    }
}

static void
yyerror (char *message)
{
    fprintf(stderr, "%s:%d: %s", G.filename, G.linenumber, message);
    if (yyctx->__text[0]) fprintf(stderr, " near token '%s'", yyctx->__text);
    if (yyctx->__pos < yyctx->__limit || !feof(G.fp)) {
        yyctx->__buf[yyctx->__limit]= '\0';
        fprintf(stderr, " before text \"");
        while (yyctx->__pos < yyctx->__limit) {
            if ('\n' == yyctx->__buf[yyctx->__pos]
                    || '\r' == yyctx->__buf[yyctx->__pos]) break;
            fputc(yyctx->__buf[yyctx->__pos++], stderr);
        }
        if (yyctx->__pos == yyctx->__limit) {
            int c;
            while ((c = fgetc(G.fp)) != EOF && c != '\n' && c != '\r')
                fputc(c, stderr);
        }
        fputc('\"', stderr);
    }
    fprintf(stderr, "\n");
    exit(1);
}

/* Command-line option handling */
static struct option long_options[] =
{
    {"path"  , required_argument, 0, 'p'}, // TODO
    {"output", required_argument, 0, 'o'},
    {"help"  , no_argument      , 0, 'h'},
    {0, 0, 0, 0}
};

struct opt_help_ {
    char *arg_help;
    char *long_help;
} opt_help[] = {
    {"<path>", "Path to find module/submodules (colon delimited list)"}, /* TODO */
    {"<file>", "Write output to file"}                                 ,
    {NULL    , "Display this help and exit"}                           ,
    {NULL, NULL}
};

static
void print_usage (char *s)
{
    int i = 0;

    printf("\nUsage: %s  [OPTIONS] [FILE]+\n\n", basename(s));
    while (long_options[i].name) {
        printf(" -%c, --%-15s %-18s %s\n", long_options[i].val,
                long_options[i].name,
                opt_help[i].arg_help ? opt_help[i].arg_help : "",
                opt_help[i].long_help);
        i++;
    }
    printf("\n");
}
int
main (int argc, char *argv[])
{
    int   i, gc;
    int   option_index = 0;

    while (1) {
        gc = getopt_long(argc, argv, "ho:p:", long_options, &option_index);
        if (gc == -1) /* Detect the end of the options. */
            break;
        switch (gc) {
            /* TODO */
            case 'o':
                break;
            case 'p':
                break;
            case '?':
                /* getopt_long already printed an error message. */
            case 'h':
                print_usage(argv[0]);
                exit(0);

            default:
                abort();
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "No input files\n");
        print_usage(argv[0]);
        exit(0);
    }

    for (i = optind; i < argc; i++) {
        G.fp = fopen(argv[i], "r");
        if (!G.fp) {
            perror(argv[i]);
            exit(1);
        }
        G.filename = argv[i];
        if (!yang_parse()) {
            yyerror("Syntax error");
            fclose(G.fp);
            return 1;
        }
        fclose(G.fp);
    }

    if (G.errcnt > 0) {
        fprintf(stderr, "%d errors seen\n", G.errcnt);
    } else {
        printf("Parse OK\n");
    }
    return 0;
}
