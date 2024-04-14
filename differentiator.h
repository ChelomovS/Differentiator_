#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdio.h>

static const size_t MAX_SIZE_OF_VAR = 5;

enum operations 
{
    NOT_OPERATION = 0,
    ADD           = 1,
    SUB           = 2,
    MUL           = 3,
    DIV           = 4,
    SIN           = 5,
    COS           = 6,
    LN            = 7,
    TAN           = 8,
    POW           = 9
};

enum types
{
    type_uninit    = 0, // неинициализованный тип
    type_num       = 1, // число
    type_var       = 2, // переменная 
    type_operation = 3  // операция 
};

struct Node
{ 
    types type;                 // тип хранимой информации
    operations operation;       // тип операции
    double value;              // значение const 
    char var[MAX_SIZE_OF_VAR]; // имя переменной
    size_t arg_number;         // сколько аргументов, если это функция (т.е. 1 или 2) 

    Node* left;                // указатель на левый узел
    Node* right;               // указатель на правый узел
};

struct Differentiator
{
    Node* ptr_node; // указатель на текущий узел
    char* buffer;   // буффер, куда копируется данные из файла
};

#define REQUIRE(a, ptr_str)\
        if(*ptr_str == a) ptr_str++;\
        else syntax_error();\

#define CONVERT_TO_RAD(deg)\
        ((deg) * M_PI / 180)\


void differentiator_ctr(Differentiator* differentiator, size_t file_size);
void differentiator_dtr(Differentiator* differentiator);
void tree_dtor(Node* ptr_node);
Node* create_op_node(operations operation, Node* left, Node* right, size_t arg_number);
Node* create_num_node(double value, Node* left, Node* right);
Node* create_var_node(char* variable, Node* left, Node* right);
Node* copy_node(Node* ptr_node);
void  delete_tree(Node* node);
Node* diff(Node* ptr_node);
Node* differentiate(Node* ptr_node);

Node* recursive_descent(const char* buffer);
Node* get_G();
Node* get_E();
Node* get_T();
Node* get_P();
Node* get_O();
Node* get_F();
Node* get_N();
void tree_dump(Node* ptr_node, FILE* file);
void syntax_error();

#endif // DIFFERENTIATOR_H