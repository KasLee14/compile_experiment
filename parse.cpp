#include "parse.h"
#include "stdio.h"
#ifdef _PASER_WITH_SEMANITCS
#include "semantics.cpp"
#endif

//本文件在语法分析器模块的基础上，增加了语义处理所需的代码
scanner scan;

//函数区
void Parser(char *SrcFilePtr) {
	/*语法分析主程序:
	调用词法分析器的GetToken函数（封装在FetchToken中）返回记号，
	 然后使用核心产生式program()对记号流进行递归下降分析，
	 判断记号流的结构是否符合文法规则
	*/
	if (!scan.InitScanner(SrcFilePtr)) {
		printf("open Source File Error!\n");
		return;
	}
	FetchToken();//返回的记号存放在全局变量token中
	Program();//递归下降的核心产生式
	scan.CloseScanner();
}

//主函数：产生式(语句级）逻辑区
void Program() {
	while (token.type != NONTOKEN) {//词法分析器输出NONTOKEN表示已达记号流末尾
		Statement();//匹配一条语句
		MatchToken(SEMICO);
	}
}

void Statement() {
	switch (token.type) {
		case ORIGIN:
			OriginStatment();
			break;
		case ROT:
			RotStatement();
			break;
		case SCALE:
			ScaleStatment();
			break;
		case FOR:
			ForStatement();
			break;
		default:
			SyntaxError(2);
	}
}

void OriginStatment() {
	struct ExprNode *origin_x, * origin_y;
	MatchToken(ORIGIN);
	MatchToken(IS);
	MatchToken(L_BRACKET);
	origin_x = Expression();
	MatchToken(COMMA);
	origin_y = Expression();
	MatchToken(R_BRACKET);
	/*
		//测试函数：打印表达式的语法树
		printf("[*]SyntaxTree:\n");
		PrintSyntaxTree(origin_x, 0);
		printf("\n");
		printf("[*]SyntaxTree:\n");
		PrintSyntaxTree(origin_y, 0);
		printf("\n");
	*/
#ifdef _PASER_WITH_SEMANITCS
	Origin_x = get_expr_value(origin_x);
	Origin_y = get_expr_value(origin_y);
#endif
}

void RotStatement() {
	struct ExprNode *rotate;
	MatchToken(ROT);
	MatchToken(IS);
	rotate = Expression();

	/*
	//测试函数：打印表达式的语法树
	printf("[*]SyntaxTree:\n");
	PrintSyntaxTree(rotate, 0);
	printf("\n");
	*/
#ifdef _PASER_WITH_SEMANITCS
	Rot_ang = get_expr_value(rotate);
#endif
}

void ScaleStatment() {
	struct ExprNode *scale_x, * scale_y;
	MatchToken(SCALE);
	MatchToken(IS);
	MatchToken(L_BRACKET);
	scale_x = Expression();
	MatchToken(COMMA);
	scale_y = Expression();
	MatchToken(R_BRACKET);

	/*
		//测试函数：打印表达式的语法树
		printf("[*]SyntaxTree:\n");
		PrintSyntaxTree(scale_x, 0); printf("\n");
		printf("[*]SyntaxTree:\n");
		PrintSyntaxTree(scale_y, 0); printf("\n");
	*/
#ifdef _PASER_WITH_SEMANITCS
	Scale_x = get_expr_value(scale_x);
	Scale_y = get_expr_value(scale_y);
#endif
}

void ForStatement() {
	struct ExprNode *start_ptr, * end_ptr, * step_ptr, * x_ptr, * y_ptr;

	MatchToken(FOR);
	MatchToken(T);
	MatchToken(FROM);
	start_ptr = Expression();
	MatchToken(TO);
	end_ptr = Expression();
	MatchToken(STEP);
	step_ptr = Expression();
	MatchToken(DRAW);
	MatchToken(L_BRACKET);
	x_ptr = Expression();
	MatchToken(COMMA);
	y_ptr = Expression();
	MatchToken(R_BRACKET);

	/*
		//测试函数：打印表达式的语法树
		printf("[*]SyntaxTree:\n");
		PrintSyntaxTree(start_ptr, 0);
		printf("\n");
		printf("[*]SyntaxTree:\n");
		PrintSyntaxTree(end_ptr, 0);
		printf("\n");
		printf("[*]SyntaxTree:\n");
		PrintSyntaxTree(step_ptr, 0);
		printf("\n");
		printf("[*]SyntaxTree:\n");
		PrintSyntaxTree(x_ptr, 0);
		printf("\n");
		printf("[*]SyntaxTree:\n");
		PrintSyntaxTree(y_ptr, 0);
		printf("\n");
	*/
#ifdef _PASER_WITH_SEMANITCS
	draw_loop(get_expr_value(start_ptr),get_expr_value(end_ptr),get_expr_value(step_ptr),x_ptr,y_ptr);
#endif
}

//主函数：产生式(表达式级)逻辑区
struct ExprNode *Expression() {
	struct ExprNode *left, * right;
	enum Token_Type token_tmp;
	left = Term();
	while (token.type == PLUS || token.type == MINUS) {
		token_tmp = token.type;
		MatchToken(token_tmp);
		right = Term();
		left = MakeExprNode(token_tmp, left, right);
	}
	return left;
}

struct ExprNode *Term() {
	struct ExprNode *left, * right;
	enum Token_Type token_tmp;

	left = Factor();
	while (token.type == MUL || token.type == DIV) {
		token_tmp = token.type;
		MatchToken(token_tmp);
		right = Factor();
		left = MakeExprNode(token_tmp, left, right);
	}
	return left;
}

struct ExprNode *Factor() {
	struct ExprNode *factor_Node;
	enum Token_Type token_tmp;

	if (token.type == PLUS || token.type == MINUS) {
		struct ExprNode *ConstPtr = (struct ExprNode *)malloc(sizeof(struct ExprNode));
		ConstPtr->Content.CaseConst = 0;
		ConstPtr->OpCode = CONST_ID;

		token_tmp = token.type;
		MatchToken(token.type);

		factor_Node = Factor();
		factor_Node = MakeExprNode(token_tmp, ConstPtr, factor_Node);
	} else
		factor_Node = Component();

	return factor_Node;
}

struct ExprNode *Component() {
	struct ExprNode *component_Node;
	enum Token_Type token_tmp;

	component_Node = Atom();

	if (token.type == POWER) {
		struct ExprNode *component_Node_another;
		token_tmp = token.type;
		MatchToken(POWER);
		component_Node_another = Component();

		component_Node = MakeExprNode(token_tmp, component_Node, component_Node_another);
	}
	return component_Node;
}

struct ExprNode *Atom() {
	struct ExprNode *atom_Node;
	struct Token token_tmp = token;
	switch (token.type) {
		case CONST_ID:
			atom_Node = MakeExprNode(token.type, token.value);
			MatchToken(token.type);
			break;
		case T:
			atom_Node = MakeExprNode(token.type);
			MatchToken(token.type);
			break;
		case FUNC: {

			MatchToken(token.type);
			MatchToken(L_BRACKET);
			atom_Node = MakeExprNode(FUNC, token_tmp.FuncPtr, Expression());
			MatchToken(R_BRACKET);
			break;
		}
		case L_BRACKET:
			MatchToken(token.type);
			atom_Node = Expression();
			MatchToken(R_BRACKET);
			break;
		default:
			SyntaxError(3);
	}
	return atom_Node;
}

//构建语法树
struct ExprNode *MakeExprNode(enum Token_Type opcode, ...) {//此函数参数可变
	struct ExprNode *ExprPtr = (struct ExprNode *)malloc(sizeof(struct ExprNode));
	va_list ArgPtr;
	ExprPtr->OpCode = opcode;
	va_start(ArgPtr, opcode);

	switch (opcode) {
		case CONST_ID://常数节点
			ExprPtr->Content.CaseConst = (double)va_arg(ArgPtr, double);
			break;
		case T://参数节点
			ExprPtr->Content.CaseParmPtr = &Parameter;
			break;
		case FUNC://函数节点
			ExprPtr->Content.CaseFunc.MathFuncPtr = (FuncPtr)va_arg(ArgPtr, FuncPtr);
			ExprPtr->Content.CaseFunc.Child = (struct ExprNode *)va_arg(ArgPtr, struct ExprNode *);
			break;
		default://二元运算节点
			ExprPtr->Content.CaseOperator.Left = (struct ExprNode *)va_arg(ArgPtr, struct ExprNode *);
			ExprPtr->Content.CaseOperator.Right = (struct ExprNode *)va_arg(ArgPtr, struct ExprNode *);
	}
	return ExprPtr;
}

//辅助函数
void FetchToken() {
	token = scan.GetToken();
	if (token.type == ERRTOKEN)
		SyntaxError(1);
}

void MatchToken(enum Token_Type AToken) {//判断全局变量token当前类型值是否与AToken匹配
	if (token.type != AToken)
		SyntaxError(4);
	else {
		//printf("Match Token %s\n", token.lexeme);//测试语句
		FetchToken();
	}
}

void SyntaxError(int case_of) {//错误处理函数
	switch (case_of) {
		case 1://由FetchToken函数抛出：词法分析获取的记号类型为ERRTOKEN,即不构成正确的单词
			printf("[*]Error code %d: Incorrect word:%s\n", case_of, token.lexeme);
			break;
		case 2://由Statement函数抛出：不符合文法定义，句子开头的Token不正确
			printf("[*]Error code %d:Incorrect start of sentence:%s\n", case_of, token.lexeme);
			break;
		case 3://由Atom函数抛出：不符合文法定义，表达式不正确
			printf("[*]Error code %d:Incorrect Expression:%s\n", case_of, token.lexeme);
			break;
		case 4://由MatchToken函数抛出：不符合文法定义，语句错误（保留字、标点符等匹配失败）
			printf("[*]Error code %d:Incorrect Match:%s\n", case_of, token.lexeme);
			break;
	}
	system("pause");
	exit(0);//程序终止
}


//测试函数
void PrintSyntaxTree(struct ExprNode *root, int indent) {//打印表达式的语法树
	char blank[50];
	blank[indent] = '\0';
	for (int i = 0; i < indent; i++)
		blank[i] = ' ';

	switch (root->OpCode) {
		case CONST_ID:
			printf("%s%f\n", blank, root->Content.CaseConst);
			break;

		case T:
			printf("%s%s\n", blank, "T");
			break;

		case FUNC:
			printf("%s%x\n", blank, root->Content.CaseFunc.MathFuncPtr);
			if (root->Content.CaseFunc.Child != NULL)
				PrintSyntaxTree(root->Content.CaseFunc.Child, indent + 1);
			break;

		default:
			switch (root->OpCode) {
				case PLUS:
					printf("%s%s\n", blank, "+");
					if (root->Content.CaseOperator.Left == NULL)
						printf("%s %s\n", blank, "0.0000");
					else {
						PrintSyntaxTree(root->Content.CaseOperator.Left, indent + 1);
						PrintSyntaxTree(root->Content.CaseOperator.Right, indent + 1);
					}
					break;

				case MINUS:
					printf("%s%s\n", blank, "-");
					if (root->Content.CaseOperator.Left == NULL)
						printf("%s %s\n", blank, "0.0000");
					else {
						PrintSyntaxTree(root->Content.CaseOperator.Left, indent + 1);
						PrintSyntaxTree(root->Content.CaseOperator.Right, indent + 1);
					}
					break;

				case MUL:
					printf("%s%s\n", blank, "*");
					PrintSyntaxTree(root->Content.CaseOperator.Left, indent + 1);
					PrintSyntaxTree(root->Content.CaseOperator.Right, indent + 1);
					break;

				case DIV:
					printf("%s%s\n", blank, "/");
					PrintSyntaxTree(root->Content.CaseOperator.Left, indent + 1);
					PrintSyntaxTree(root->Content.CaseOperator.Right, indent + 1);
					break;

				case POWER:
					printf("%s%s\n", blank, "**");
					PrintSyntaxTree(root->Content.CaseOperator.Left, indent + 1);
					PrintSyntaxTree(root->Content.CaseOperator.Right, indent + 1);
					break;
			}
	}
}

