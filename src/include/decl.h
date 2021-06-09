// Function prototypes for all compiler files
// scan.c
int scan(token_T *t);

// tree.c
AST_T *mkastnode(int op, AST_T *left, AST_T *right, int intvalue);
AST_T *mkastleaf(int op, int intvalue);
AST_T *mkastunary(int op, AST_T *left, int intvalue);

// gen.c
int genAST(AST_T *n, int reg);
void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int reg);
void genglobsym(char *s);

// cg.c
void freeall_registers(void);
void cgpreamble();
void cgpostamble();
int cgloadint(int value);
int cgloadglob(char *identifier);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);
int cgstorglob(int r, char *identifier);
void cgglobsym(char *sym);
int cgequal(int r1, int r2);
int cgnotequal(int r1, int r2);
int cglessthan(int r1, int r2);
int cggreaterthan(int r1, int r2);
int cglessequal(int r1, int r2);
int cggreaterequal(int r1, int r2);


// expr.c
AST_T *binexpr(int ptp);

// stmt.c
void statements(void);

// misc.c
void match(int t, char *what);
void semi(void);
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
