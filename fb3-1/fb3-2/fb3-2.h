extern int yylineno;
void yyerror(char *s, ...); // C에선 ...이 다중 인자 받는의미

struct symbol {
	char *name;
	double value;
	struct ast *func;
	struct symlist *syms;
};

#define NHASH 9997
struct symbol symtab[NHASH];

struct symbol *lookup(char*);

struct symlist {
	struct symbol *sym;
	struct symlist *next;
};

struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
void symlistfree(struct symlist *sl);

/* node 타입
 * + - * / |   연산자
 * 0-7 ops, 04 equal, 02 less, 01 greater
 * M  단향 마이너스 ...?
 * L expression or statement list
 * W WHILE statement
 * N symbol ref
 * = assignment
 * S list of symbols
 * F built in function call
 * C user function call
 */

enum bifs {
	B_sqrt = 1,
	B_exp,
	B_log,
	B_print
};


/* AST 노드들 */
/* 모두 common한 initial nodetype을 지님*/

struct ast {
	int nodetype;
	struct ast *l;
	struct ast *r;
};

struct fncall {
	int nodetype;
	struct ast *l;
	enum bifs functype;
};

struct ufncall {
	int nodetype;
	struct ast *l;
	struct symbol *s;
};

struct flow{
	int nodetype;
	struct ast *cond;
	struct ast *tl;
	struct ast *el;
};

struct numval{
	int nodetype;
	double number;
};

struct symref {
	int nodetype;
	struct symbol *s;
};

struct symasgn{
	int nodetype;
	struct symbol *s;
	struct ast *v;
};

struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
struct ast *newfunc(int functype, struct ast *l);
struct ast *newcall(struct symbol *s, struct ast *l);
struct ast *newref(struct symbol *s);
struct ast *newasgn(struct symbol *s, struct ast *v);
struct ast *newnum(double d);
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *tr);

void dodef(struct symbol *name, struct symlist *syms, struct ast *stmts);

double eval(struct ast *);

void treefree(struct ast *);
