#include <stdlib.h>
#include <math.h>
#include <graphics.h>


#include "semantic.h"

struct Token token;
double Parameter = 0;
double Origin_x = 0.0, Origin_y = 0.0;
double Rot_ang = 0.0;
double Scale_x = 1, Scale_y = 1;
double Draw_x, Draw_y;


//������ʽ��ֵ
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

//������ʵ������ֵ
void CalcCoord(struct ExprNode *x_nptr, struct ExprNode *y_nptr, double &x, double &y) {
	double x_val, x_temp, y_val;

	// ������ʽ��ֵ���õ����ԭʼ����
	x_val = get_expr_value(x_ptr);
	y_val = get_expr_value(y_ptr);

	// �����任
	x_val *= Scale_x ;
	y_val *= Scale_y ;

	// ��ת�任
	x_temp = x_val * cos(rot_angle) + y_val * sin(rot_angle);
	y_val  = y_val * cos(rot_angle) - x_val * sin(rot_angle);
	x_val  = x_temp;

	// ƽ�Ʊ任
	x_val += Origin_x;
	y_val += Origin_y;

	// ���ر任��������
	x = x_val;
	y = y_val;
}

// ����һ����
void draw_pixel(unsigned long x, unsigned long y) {
	putpixel(x, y, RED);
}

// ͼ�λ���
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

