#ifndef COMPILE_EXPERIMENT_SCANNER_H
#define COMPILE_EXPERIMENT_SCANNER_H

#include <bits/stdc++.h>

enum Token_Type			  // 记号的类别
{
    ORIGIN, SCALE, ROT, IS,	  // 保留字（一字一码）
    TO, STEP, DRAW,FOR, FROM, // 保留字
    T,				  // 参数
    SEMICO, L_BRACKET, R_BRACKET, COMMA,// 分隔符
    PLUS, MINUS, MUL, DIV, POWER,		// 运算符
    FUNC,				  // 函数（调用）
    CONST_ID,			  // 常数
    NONTOKEN,			  // 空记号（源程序结束）
    ERRTOKEN			  // 出错记号（非法输入）
};

struct Token
{
    enum Token_Type  type;  // 类别，规定所有记号的类别（命名常数便于记忆/理解）
    char * lexeme;  // 属性，原始输入的字符串，亦可为数组
    double value;  // 属性，若记号是常数则存常数的值
    double (* FuncPtr)(double);  // 属性，若记号是函数则存函数地址

};

//区分记号的符号表
struct Token TokenTab[] = {
        {CONST_ID, "PI", 3.1415926, NULL},
        {CONST_ID, "E", 2.71828, NULL},
        {T, "T",	 0.0, NULL},
        {FUNC, "SIN", 0.0, sin},
        {FUNC, "COS", 0.0, cos},
        {FUNC, "TAN", 0.0, tan},
        {FUNC, "LN", 0.0, log},
        {FUNC, "EXP", 0.0, exp},
        {FUNC, "SQRT", 0.0, sqrt},
        {ORIGIN, "ORIGIN", 0.0, NULL},
        {SCALE, "SCALE", 0.0, NULL},
        {ROT, "ROT", 0.0, NULL},
        {IS, "IS", 0.0, NULL},
        {FOR, "FOR", 0.0, NULL},
        {FROM, "FROM", 0.0, NULL},
        {TO, "TO", 0.0, NULL},
        {STEP, "STEP", 0.0, NULL},
        {DRAW, "DRAW", 0.0, NULL}
};

const int token_len = 100;

class scanner{

    FILE * file;  //输入文件流
    char token_buf[token_len];  // 记号字符缓冲

public:

    unsigned int line_no;  //表示记号所在源文件的行号
    scanner(){ line_no = 1; } //行号默认为1
    int InitScanner(const char* fileName);  //初始化词法分析器，成功时返回非0，失败返回0
    struct Token GetToken();  //识别并返回一个记号。遇到非法输入时 .type=ERRTOKEN、文件结束时 .type=NONTOKEN
    void CloseScanner();  //关闭词法分析器

private:

    char get_char();				            // 从输入源程序中读入一个字符
    void back_char(char next_char);			    // 把预读的字符退回到输入源程序中
    void add_in_token_str(char next_char);		// 加入字符到记号缓冲区
    void empty_token_str();				        // 清空记号缓冲区
    Token check_token(const char *c_str);	// 判断所给的字符串是否在符号表中

};



#endif //COMPILE_EXPERIMENT_SCANNER_H
