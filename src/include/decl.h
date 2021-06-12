// Function prototypes for all compiler files
// scan.c
int scan(token_T *t);

// tree.c
AST_T *mkastnode(int op, AST_T *left, AST_T *mid, AST_T *right, int intvalue);
AST_T *mkastleaf(int op, int intvalue);
AST_T *mkastunary(int op, AST_T *left, int intvalue);

// gen.c
int genAST(AST_T *n, int reg, int parentASTop);
void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int reg);
void genglobsym(char *s);

// cg.c
void freeall_registers(void);
void cgpreamble();
void cgfuncpreamble();
void cgfuncpostamble();
int cgloadint(int value);
int cgloadglob(char *identifier);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);
int cgstorglob(int r, char *identifier);
void cgglobsym(char *sym);
int cgcompare_and_set(int ASTop, int r1, int r2);
int cgcompare_and_jump(int ASTop, int r1, int r2, int label);
void cglabel(int l);
void cgjump(int l);

// expr.c
AST_T *binexpr(int ptp);

// stmt.c
AST_T *compound_statement(void);

// misc.c
void match(int t, char *what);
void semi(void);
void lbrace(void);
void rbrace(void);
void lparen(void);
void rparen(void);
void ident(void);
void fatal(char *s);
void fatals(char *s1, char *s2);
void fatald(char *s, int d);
void fatalc(char *s, int c);

// sym.c
int findglob(char *s);
int addglob(char *name);

// decl.c
void var_declaration(void);
AST_T *function_declaration(void);
