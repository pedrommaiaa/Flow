// Function prototypes for all compiler files
// scan.c
int scan(token_T *t);

// tree.c
AST_T *mkastnode(int op, AST_T *left, AST_T *right, int intvalue);
AST_T *mkastleaf(int op, int intvalue);
AST_T *mkastunary(int op, AST_T *left, int intvalue);


// gen.c
int genAST(AST_T *n);
void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int reg);

// cg.c
void freeall_registers(void);
void cgpreamble();
void cgpostamble();
int cgload(int value);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);

// expr.c
AST_T *binexpr(int ptp);

// stmt.c
void statements(void);

// misc.c
void match(int t, char *what);
void semi(void);
