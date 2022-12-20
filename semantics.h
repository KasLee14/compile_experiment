#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "scanner.h"
//���������Ҫͨ�����﷨�������̵õ�ʵ�֡�
//����û�в����������ķ������������ͨ�������������������������﷨��������ʵ��
//ȫ�ֱ�����
extern struct Token token;

extern double Parameter; //����
extern double Origin_x, Origin_y ;  //�ᡢ��ƽ�ƾ���
extern double Rot_ang; //��ת�Ƕ�
extern double Scale_x, Scale_y ;   //�ᡢ�ݱ�������

//�������庯��

// ��ñ��ʽ��ֵ
double get_expr_value(struct ExprNode *root);
// ����������
void cal_coord(struct ExprNode *x_nptr, struct ExprNode *y_nptr, double &x, double &y);
// ����һ����
void draw_pixel(unsigned long x, unsigned long y);
// ͼ�λ���
void draw_loop(double start_val, double end_val, double step_val, struct ExprNode *x_ptr, struct ExprNode *y_ptr);
