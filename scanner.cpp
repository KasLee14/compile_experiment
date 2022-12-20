#include "scanner.h"

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

//初始化词法分析器，成功时返回非0，失败返回0
int scanner::InitScanner(const char *fileName) {

    file = fopen(fileName, "r");
    if(file != nullptr) return 1;
    else return 0;

}

//关闭词法分析器
void scanner::CloseScanner() {

    if(file != nullptr) fclose(file);

}

// 从输入源程序中读入一个字符,并自动返回大写
char scanner::get_char() {

    char next_char = getc(file);
    return toupper(next_char);

}

// 把预读的字符退回到输入源程序中
void scanner::back_char(char next_char){

    if(next_char != EOF) ungetc(next_char, file);

}

// 加入字符到记号缓冲区
void scanner::add_in_token_str(char next_char){

    int len = sizeof(token_buf);
    token_buf[len] = next_char;  //把字符存入缓冲区
    token_buf[len + 1] = '\0';  //把缓冲区末尾设置为终止符

}

// 清空记号缓冲区
void scanner::empty_token_str(){

    memset(token_buf, 0, token_len);

}

// 判断所给的字符串是否在符号表中
Token scanner::check_token(const char *c_str){

    Token token;
    for(int i = 0; i < 17; i++){
        if(strcmp(c_str, TokenTab[i].lexeme) == 0) return TokenTab[i];
        else{
            memset(&token, 0, sizeof(Token));
            token.type = ERRTOKEN;
            return token;
        }
    }

}

//识别并返回一个记号。遇到非法输入时 .type=ERRTOKEN、文件结束时 .type=NONTOKEN
Token scanner::GetToken(){

    Token token;
    memset(&token, 0, sizeof(Token));
    empty_token_str();
    token.lexeme = token_buf;
    char next_char;

    //持续读取到不是空格为止
    while(1){
        next_char = get_char();
        if(next_char == EOF){
            token.type = NONTOKEN;
            return token;
        }
        if(next_char == '\n') line_no++;
        if(!isspace(next_char)) break;
    }

    add_in_token_str(next_char);  //将读取到的字符放入缓存区

    if(isalpha(next_char)){  //如果读取到字母则为常量、函数、关键字等
        while(true){
            next_char = get_char();
            if(!isalnum(next_char)) break;
            else add_in_token_str(next_char);
        }
        back_char(next_char);
        token = check_token(token_buf);
        token.lexeme = token_buf;
        return token;
    }else if(isdigit(next_char)){  //如果读取到数字则为一个常量
        while(1){
            next_char = get_char();
            if(!std::isdigit(next_char)) break;
            else add_in_token_str(next_char);
        }
        if(next_char == '.'){
            add_in_token_str(next_char);
            while(1){
                next_char = get_char();
                if(!std::isdigit(next_char)) break;
                else add_in_token_str(next_char);
            }
        }
        back_char(next_char);
        token.type = CONST_ID;
        token.value = atof (token_buf);
        return token;
    }else{  //不是数字不是字符则为符号
        if(next_char == ';') token.type = SEMICO;
        else if(next_char == '(') token.type = L_BRACKET;
        else if(next_char == ')') token.type = R_BRACKET;
        else if(next_char == ',') token.type = COMMA;
        else if(next_char == '+') token.type = PLUS;
        else if(next_char == '-') token.type = MINUS;
        else if(next_char == '*'){ //*则为MUL，**则为POWER
            next_char = get_char();
            if(next_char == '*'){
                token.type = POWER;
            }
            else{
                token.type = MUL;
            }
        }
        else if(next_char == '/'){ //单斜杠是除法，双斜杠是注释，则跳过
            next_char = get_char();
            if(next_char == '/'){
                while(next_char != EOF && next_char != '\n') next_char = get_char();
                back_char(next_char);
                return GetToken();
            }
            else{
                token.type = DIV;
            }
        }
        else token.type = ERRTOKEN;
    }
    return token;
}




