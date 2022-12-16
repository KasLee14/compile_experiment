#include"parse.h"
#include"stdio.h"

scanner scan;
//������
void Parser(char* SrcFilePtr) {
	/*�﷨����������:
	���ôʷ���������GetToken��������װ��FetchToken�У����ؼǺţ�
	 Ȼ��ʹ�ú��Ĳ���ʽprogram()�ԼǺ������еݹ��½�������
	 �жϼǺ����Ľṹ�Ƿ�����ķ�����
	*/
	if (!scan.InitScanner(SrcFilePtr)) {
		printf("open Source File Error!\n");
		return;
	}
	FetchToken();//���صļǺŴ����ȫ�ֱ���token��
	Program();//�ݹ��½��ĺ��Ĳ���ʽ
	scan.CloseScanner();
}

//������������ʽ(��伶���߼���
void Program() {
	while (token.type != NONTOKEN) {//�ʷ����������NONTOKEN��ʾ�Ѵ�Ǻ���ĩβ
		Statement();//ƥ��һ�����
		MatchToken(SEMICO);
	}
}

void Statement() {
	switch (token.type)
	{
	case ORIGIN:OriginStatment(); break;
	case ROT:RotStatement(); break;
	case SCALE:ScaleStatment(); break;
	case FOR:ForStatement(); break;
	default:SyntaxError(2);
	}
}

void OriginStatment() {
	struct ExprNode* origin_x, * origin_y;
	MatchToken(ORIGIN); MatchToken(IS); MatchToken(L_BRACKET);
	origin_x = Expression(); MatchToken(COMMA);
	origin_y = Expression(); MatchToken(R_BRACKET);

	//���Ժ�������ӡ���ʽ���﷨��
	printf("[*]SyntaxTree:\n");
	PrintSyntaxTree(origin_x, 0); printf("\n");
	printf("[*]SyntaxTree:\n");
	PrintSyntaxTree(origin_y, 0); printf("\n");
}

void RotStatement() {
	struct ExprNode* rotate;
	MatchToken(ROT); MatchToken(IS);
	rotate = Expression();

	//���Ժ�������ӡ���ʽ���﷨��
	printf("[*]SyntaxTree:\n");
	PrintSyntaxTree(rotate, 0); printf("\n");

}

void ScaleStatment() {
	struct ExprNode* scale_x, * scale_y;
	MatchToken(SCALE); MatchToken(IS); MatchToken(L_BRACKET);
	scale_x = Expression(); MatchToken(COMMA);
	scale_y = Expression(); MatchToken(R_BRACKET);

	//���Ժ�������ӡ���ʽ���﷨��
	printf("[*]SyntaxTree:\n");
	PrintSyntaxTree(scale_x, 0); printf("\n");
	printf("[*]SyntaxTree:\n");
	PrintSyntaxTree(scale_y, 0); printf("\n");
}

void ForStatement() {
	struct ExprNode* start_ptr, * end_ptr, * step_ptr, * x_ptr, * y_ptr;

	MatchToken(FOR); MatchToken(T); MatchToken(FROM); start_ptr = Expression();
	MatchToken(TO); end_ptr = Expression();
	MatchToken(STEP); step_ptr = Expression();
	MatchToken(DRAW); MatchToken(L_BRACKET); x_ptr = Expression();
	MatchToken(COMMA); y_ptr = Expression();
	MatchToken(R_BRACKET);

	//���Ժ�������ӡ���ʽ���﷨��
	printf("[*]SyntaxTree:\n");
	PrintSyntaxTree(start_ptr, 0); printf("\n");
	printf("[*]SyntaxTree:\n");
	PrintSyntaxTree(end_ptr, 0); printf("\n");
	printf("[*]SyntaxTree:\n");
	PrintSyntaxTree(step_ptr, 0); printf("\n");
	printf("[*]SyntaxTree:\n");
	PrintSyntaxTree(x_ptr, 0); printf("\n");
	printf("[*]SyntaxTree:\n");
	PrintSyntaxTree(y_ptr, 0); printf("\n");

}

//������������ʽ(���ʽ��)�߼���
struct ExprNode* Expression() {
	struct ExprNode* left, * right;
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

struct ExprNode* Term() {
	struct ExprNode* left, * right;
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

struct ExprNode* Factor() {
	struct ExprNode* factor_Node;
	enum Token_Type token_tmp;

	if (token.type == PLUS || token.type == MINUS) {
		struct ExprNode* ConstPtr = (struct ExprNode*)malloc(sizeof(struct ExprNode));
		ConstPtr->Content.CaseConst = 0;
		ConstPtr->OpCode = CONST_ID;

		token_tmp = token.type;
		MatchToken(token.type);

		factor_Node = Factor();
		factor_Node = MakeExprNode(token_tmp, ConstPtr, factor_Node);
	}
	else
		factor_Node = Component();

	return factor_Node;
}

struct ExprNode* Component() {
	struct ExprNode* component_Node;
	enum Token_Type token_tmp;

	component_Node = Atom();

	if (token.type == POWER) {
		struct ExprNode* component_Node_another;
		token_tmp = token.type;
		MatchToken(POWER);
		component_Node_another = Component();

		component_Node = MakeExprNode(token_tmp, component_Node, component_Node_another);
	}
	return component_Node;
}

struct ExprNode* Atom() {
	struct ExprNode* atom_Node;
    struct Token token_tmp = token;
	switch (token.type) {
	case CONST_ID:
		atom_Node = MakeExprNode(token.type, token.value); MatchToken(token.type); break;
	case T:
		atom_Node = MakeExprNode(token.type); MatchToken(token.type); break;
	case FUNC:
	{

		MatchToken(token.type); MatchToken(L_BRACKET); atom_Node = MakeExprNode(FUNC, token_tmp.FuncPtr, Expression());
		MatchToken(R_BRACKET);
        break;
	}
	case L_BRACKET:
		MatchToken(token.type); atom_Node = Expression(); MatchToken(R_BRACKET); break;
	default:SyntaxError(3);
	}
	return atom_Node;
}

//�����﷨��
struct ExprNode* MakeExprNode(enum Token_Type opcode, ...) {//�˺��������ɱ�
	struct ExprNode* ExprPtr = (struct ExprNode*)malloc(sizeof(struct ExprNode));
	va_list ArgPtr;
	ExprPtr->OpCode = opcode;
	va_start(ArgPtr, opcode);

	switch (opcode) {
	case CONST_ID://�����ڵ�
		ExprPtr->Content.CaseConst = (double)va_arg(ArgPtr, double); break;
	case T://�����ڵ�
		ExprPtr->Content.CaseParmPtr = &Parameter; break;
	case FUNC://�����ڵ�
		ExprPtr->Content.CaseFunc.MathFuncPtr = (FuncPtr)va_arg(ArgPtr, FuncPtr);
		ExprPtr->Content.CaseFunc.Child = (struct ExprNode*)va_arg(ArgPtr, struct ExprNode*); break;
	default://��Ԫ����ڵ�
		ExprPtr->Content.CaseOperator.Left = (struct ExprNode*)va_arg(ArgPtr, struct ExprNode*);
		ExprPtr->Content.CaseOperator.Right = (struct ExprNode*)va_arg(ArgPtr, struct ExprNode*);
	}
	return ExprPtr;
}

//��������
void FetchToken() {
	token = scan.GetToken();
	if (token.type == ERRTOKEN)
		SyntaxError(1);
}

void MatchToken(enum Token_Type AToken) {//�ж�ȫ�ֱ���token��ǰ����ֵ�Ƿ���ATokenƥ��
	if (token.type != AToken)
		SyntaxError(4);
	else {
		printf("Match Token %s\n", token.lexeme);//�������
		FetchToken();
	}
}

void SyntaxError(int case_of) {//��������
	switch (case_of) {
	case 1://��FetchToken�����׳����ʷ�������ȡ�ļǺ�����ΪERRTOKEN,����������ȷ�ĵ���
		printf("[*]Error code %d: Incorrect word:%s\n", case_of, token.lexeme);
		break;
	case 2://��Statement�����׳����������ķ����壬���ӿ�ͷ��Token����ȷ
		printf("[*]Error code %d:Incorrect start of sentence:%s\n", case_of, token.lexeme);
		break;
	case 3://��Atom�����׳����������ķ����壬���ʽ����ȷ
		printf("[*]Error code %d:Incorrect Expression:%s\n", case_of, token.lexeme);
		break;
	case 4://��MatchToken�����׳����������ķ����壬�����󣨱����֡�������ƥ��ʧ�ܣ�
		printf("[*]Error code %d:Incorrect Match:%s\n", case_of, token.lexeme);
		break;
	}
	system("pause");
	exit(0);//������ֹ
}


//���Ժ���
void PrintSyntaxTree(struct ExprNode* root, int indent) {//��ӡ���ʽ���﷨��
	char blank[50]; blank[indent] = '\0';
	for (int i = 0; i < indent; i++)
		blank[i] = ' ';

	switch (root->OpCode) {
	case CONST_ID:printf("%s%f\n", blank, root->Content.CaseConst); break;

	case T:printf("%s%s\n", blank, "T"); break;

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

