#include <stdlib.h>
#include <math.h>
#include <graphics.h>


#include "semantics.h"

struct Token token;
double Parameter = 0;
double Origin_x = 0.0, Origin_y = 0.0;
double Rot_ang = 0.0;
double Scale_x = 1, Scale_y = 1;
double Draw_x, Draw_y;


//计算表达式的值
double get_expr_value(struct ExprNode *root) {
	if (root == NULL)
		return 0.0;
	switch (root -> OpCode) {
		case PLUS:
			return GetExprValue(root->Content.CaseOperator.Left) + GetExprValue(root->Content.CaseOperator.Right);
			break;
		case MINUS:
			return GetExprValue(root->Content.CaseOperator.Left) - GetExprValue(root->Content.CaseOperator.Right);
			break;
		case MUL:
			return GetExprValue(root->Content.CaseOperator.Left) * GetExprValue(root->Content.CaseOperator.Right);
			break;
		case DIV:
			return GetExprValue(root->Content.CaseOperator.Left) * GetExprValue(root->Content.CaseOperator.Right);
			break;
		case POWER:
			return pow(GetExprValue(root->Content.CaseOperator.Left), GetExprValue(root->Content.CaseOperator.Right));
			break;
		case FUNC:
			return ((FuncPtr)root->Content.CaseFunc.MathFuncPtr)(get_expr_value(root->Content.CaseFunc.Child));
		case CONST_ID :
			return root->Content.CaseConst;
			break;
		case T:
			return Parameter;
		default:
			return 0.0 ;
	}
}

//计算点的实际坐标值
void CalcCoord(struct ExprNode *x_nptr, struct ExprNode *y_nptr, double &x, double &y) {
	double x_val, x_temp, y_val;

	// 计算表达式的值，得到点的原始坐标
	x_val = get_expr_value(x_ptr);
	y_val = get_expr_value(y_ptr);

	// 比例变换
	x_val *= Scale_x ;
	y_val *= Scale_y ;

	// 旋转变换
	x_temp = x_val * cos(rot_angle) + y_val * sin(rot_angle);
	y_val  = y_val * cos(rot_angle) - x_val * sin(rot_angle);
	x_val  = x_temp;

	// 平移变换
	x_val += Origin_x;
	y_val += Origin_y;

	// 返回变换后点的坐标
	x = x_val;
	y = y_val;
}

// 绘制一个点
void draw_pixel(unsigned long x, unsigned long y) {
	putpixel(x, y, RED);
}

// 图形绘制
void draw_loop(double start_val, double end_val, double step_val, struct ExprNode *x_ptr, struct ExprNode *y_ptr);
{
	initgraph(640, 480);
	for (Parameter = start_val; Parameter <= end_val; Parameter += step_val) {
		CalcCoord(x_ptr, y_ptr);
		putpixel(Draw_x, Draw_y, RED);
	}
	_getch();
	closegraph();
}

