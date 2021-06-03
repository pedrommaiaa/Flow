// Function prototypes for all compiler files
int scan(token_T *t);

AST_T *mkastnode(int op, AST_T *left, AST_T *right, int intvalue);
AST_T *mkastleaf(int op, int intvalue);
AST_T *mkastunary(int op, AST_T *left, int intvalue);
AST_T *binexpr(int ptp);

int interpretAST(AST_T *n);
void generatecode(AST_T *n);

void freeall_registers(void);
void cgpreamble();
void cgpostamble();
int cgload(int value);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);
