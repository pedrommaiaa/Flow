// Function prototypes for all compiler files

// scan.c
void reject_token(token_T *t);
int scan(token_T *t);

// tree.c
AST_T *mkastnode(int op, int type,
			  AST_T *left,
			  AST_T *mid,
			  AST_T *right, int intvalue);
AST_T *mkastleaf(int op, int type, int intvalue);
AST_T *mkastunary(int op, int type,
			    AST_T *left, int intvalue);

// gen.c
int genlabel(void);
int genAST(AST_T *n, int reg, int parentASTop);
void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int reg);
void genglobsym(int id);
int genprimsize(int type);
void genreturn(int reg, int id);

// cg.c
void freeall_registers(void);
void cgpreamble();
void cgpostamble();
void cgfuncpreamble(int id);
void cgfuncpostamble(int id);
int cgloadint(int value, int type);
int cgloadglob(int id);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);
int cgcall(int r, int id);
int cgstorglob(int r, int id);
void cgglobsym(int id);
int cgcompare_and_set(int ASTop, int r1, int r2);
int cgcompare_and_jump(int ASTop, int r1, int r2, int label);
void cglabel(int l);
void cgjump(int l);
int cgwiden(int r, int oldtype, int newtype);
int cgprimsize(int type);
void cgreturn(int reg, int id);
int cgaddress(int id);
int cgderef(int r, int type);

// expr.c
AST_T *funccall(void);
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
int addglob(char *name, int type, int stype, int endlabel);

// decl.c
void var_declaration(void);
AST_T *function_declaration(void);

// types.c
int type_compatible(int *left, int *right, int onlyright);
int pointer_to(int type);
int value_at(int type);
