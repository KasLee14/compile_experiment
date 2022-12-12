#ifndef PARSE_H
#define PARSE_H

#include"scanner.h"

//ȫ�ֱ�����
struct Token token;
double Parameter;

//���ݽṹ��
typedef double(*FuncPtr)(double);

// ���ʽ�ڵ�
struct ExprNode
{
	enum Token_Type OpCode;//�Ǻ�����
	union {
		struct {
			struct ExprNode* Left, * Right;
		}CaseOperator;//��Ԫ����

		struct {
			struct ExprNode* Child;
			FuncPtr MathFuncPtr;
		}CaseFunc;//��������

		double CaseConst;//����������ֵ
		double* CaseParmPtr;//����T������ֵ
	}Content;
};


//������
void Parser(char* SrcFilePtr);//�﷨����������

//������������ʽ(��伶)�߼���
void Program();
void Statement();
void OriginStatment();
void RotStatement();
void ScaleStatment();
void ForStatement();

//������������ʽ(���ʽ��)�߼���
struct ExprNode* Expression();
struct ExprNode* Term();
struct ExprNode* Factor();
struct ExprNode* Component();
struct ExprNode* Atom();

//�����﷨��
struct ExprNode* MakeExprNode(enum Token_Type opcode, ...);

//��������
void FetchToken();
void MatchToken(enum Token_Type AToken);
void SyntaxError(int case_of);

//���Ժ���
void PrintSyntaxTree(struct ExprNode* root, int indent);//��ӡ���ʽ���﷨��


#endif

