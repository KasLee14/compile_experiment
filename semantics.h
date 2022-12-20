#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "parse.h"
#include <wingdi.h>
#include <windows.h>

//语义分析主要通过在语法分析过程得到实现。
//由于没有采用面向对象的方法，语义分析通过加入语义分析辅助函数后的语法分析函数实现



//辅助语义函数

// 获得表达式的值
double get_expr_value(struct ExprNode *root);
// 计算点的坐标
void cal_coord(struct ExprNode *x_ptr, struct ExprNode *y_ptr, double &x, double &y);
// 绘制一个点
void draw_pixel(unsigned long x, unsigned long y);
// 图形绘制
void draw_loop(double start_val, double end_val, double step_val, struct ExprNode *x_ptr, struct ExprNode *y_ptr);

#endif
