#ifndef PARSE_H
#define PARSE_H

//本文件在语法分析器模块的基础上，增加了语义处理所需的代码
#include"semantics.h"

// 表达式节点
struct ExprNode
{
	enum Token_Type OpCode;//记号种类
	union {
		struct {
			struct ExprNode* Left, * Right;
		}CaseOperator;//二元运算

		struct {
			struct ExprNode* Child;
			FuncPtr MathFuncPtr;
		}CaseFunc;//函数调用

		double CaseConst;//常数，绑定右值
		double* CaseParmPtr;//参数T，绑定左值
	}Content;
};


//函数区
void Parser(char* SrcFilePtr);//语法分析主程序

//主函数：产生式(语句级)逻辑区
void Program();
void Statement();
void OriginStatment();
void RotStatement();
void ScaleStatment();
void ForStatement();

//主函数：产生式(表达式级)逻辑区
struct ExprNode* Expression();
struct ExprNode* Term();
struct ExprNode* Factor();
struct ExprNode* Component();
struct ExprNode* Atom();

//构建语法树
struct ExprNode* MakeExprNode(enum Token_Type opcode, ...);

//辅助函数
void FetchToken();
void MatchToken(enum Token_Type AToken);
void SyntaxError(int case_of);

//测试函数
void PrintSyntaxTree(struct ExprNode* root, int indent);//打印表达式的语法树


#endif

