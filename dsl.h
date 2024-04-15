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

#define SIMPLE_FUNC(ptr_node, function)\
                ptr_node->type = type_num;\
                ptr_node->operation = NOT_OPERATION;\
                ptr_node->value = function(ptr_node->left->value);\
                ptr_node->arg_number = 0;\
                delete_tree(ptr_node->left);\
                ptr_node->left = nullptr;

#define SIMPLE_ADD(ptr_node)\
                ptr_node->type = type_num;\
                ptr_node->operation = NOT_OPERATION;\
                ptr_node->value = ptr_node->left->value + ptr_node->right->value;\
                ptr_node->arg_number = 0;\
                delete_tree(ptr_node->left);\
                ptr_node->left = nullptr;\
                delete_tree(node->right);\
                ptr_node->right = nullptr;

#define SIMPLE_SUB(ptr_node)\
                ptr_node->type = type_num;\
                ptr_node->operation = NOT_OPERATION;\
                ptr_node->value = ptr_node->left->value - ptr_node->right->value;\
                ptr_node->arg_number = 0;\
                delete_tree(ptr_node->left);\
                ptr_node->left = nullptr;\
                delete_tree(ptr_node->right);\
                ptr_node->right = nullptr;

#define SIMPLE_MUL(ptr_node)\
                ptr_node->type = type_num;\
                ptr_node->operation = NOT_OPERATION;\
                ptr_node->value = ptr_node->left->value * ptr_node->right->value;\
                ptr_node->arg_number = 0;\
                delete_tree(ptr_node->left);\
                ptr_node->left = nullptr;\
                delete_tree(ptr_node->right);\
                ptr_node->right = nullptr;
                
// func 

#define SIMPLE_DIV(ptr_node)\
                ptr_node->type = type_num;\
                ptr_node->operation = NOT_OPERATION;\
                ptr_node->value = ptr_node->left->value / ptr_node->right->value;\
                ptr_node->arg_number = 0;\
                delete_tree(ptr_node->left);\
                ptr_node->left = nullptr;\
                delete_tree(ptr_node->right);\
                ptr_node->right = nullptr;

#define MAKE_ZERO_VALUE(ptr_node)\
                ptr_node->type = type_num;\
                ptr_node->operation = NOT_OPERATION;\
                ptr_node->value = 0;\
                ptr_node->arg_number = 0;\
                delete_tree(ptr_node->left);\
                delete_tree(ptr_node->right);\
                ptr_node->left = nullptr;\
                ptr_node->right = nullptr;

#define CHANGE_NODES(ptr_node, ptr_node_for_delete, new_ptr_node)\
                delete_tree(ptr_node_for_delete);\
                Node* ptr_node_holder = ptr_node;\
                ptr_node = new_ptr_node;\
                free(ptr_node_holder);

#endif // DSL_H
