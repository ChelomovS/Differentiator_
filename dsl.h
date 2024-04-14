#ifndef DSL_H
#define DSL_H

#include "differentiator.h"

// Дифференцирование левого узла
#define dL diff(ptr_node->left)
// Дифференцирование правого узла
#define dR diff(ptr_node->right)

// Копирование левого узла
#define cL copy_node(ptr_node->left)
// Копирование левого узла
#define cR copy_node(ptr_node->right)

// Создание узла с const
#define _NUM(value)    create_num_node(value,    nullptr, nullptr)
// Создание узла с переменной
#define _VAR(variable) create_var_node(variable, nullptr, nullptr)

// Создание узлов операций
#define _ADD(left, right) create_op_node(ADD, left, right, 2)
#define _SUB(left, right) create_op_node(SUB, left, right, 2)
#define _MUL(left, right) create_op_node(MUL, left, right, 2)
#define _DIV(left, right) create_op_node(DIV, left, right, 2)

// Сложные функции
#define _SIN(left)          create_op_node(SIN, left, nullptr, 1)
#define _COS(left)          create_op_node(COS, left, nullptr, 1) 
#define _TAN(left)          create_op_node(TAN, left, nullptr, 1)
#define _LN (left)          create_op_node(LN,  left, nullptr, 1)
#define _POW(left, right)   create_op_node(POW, left, right,   2)

#endif // DSL_H
