#include <stdio.h>
#include <math.h>
#include <string.h>
#include "debug.h"
#include "differentiator.h"
#include "simplification.h"
#include "tree_dump.h"
#include "dsl.h"

const char* str = nullptr;

void differentiator_ctr(Differentiator* differentiator, size_t file_size)
{
    ASSERT(differentiator != nullptr);

    differentiator->buffer = (char*)calloc(file_size, sizeof(char));
    ASSERT(differentiator->buffer != nullptr);
    
    differentiator->ptr_node = (Node*)calloc(1, sizeof(Node));
    ASSERT(differentiator->ptr_node != nullptr);
}

void differentiator_dtr(Differentiator* differentiator)
{
    ASSERT(differentiator != nullptr);

    free(differentiator->buffer);
    differentiator->buffer = nullptr;

    tree_dtor(differentiator->ptr_node);
    differentiator->ptr_node = nullptr;
}

void tree_dtor(Node* ptr_node)
{
    ASSERT(ptr_node != nullptr);

    if (ptr_node->left != nullptr)
        tree_dtor(ptr_node->left);

    if (ptr_node->right != nullptr)
        tree_dtor(ptr_node->right);

    free(ptr_node);
}

// creating different nodes
Node* create_op_node(operations operation, Node* left, Node* right, size_t arg_number)
{
    ASSERT(left   != nullptr);

    Node* op_node = (Node*)calloc(1, sizeof(Node));
    if (op_node == nullptr)
        return nullptr;
        
    op_node->type        = type_operation;
    op_node->value       = 0;
    op_node->operation   = operation;
    op_node->left        = left;
    op_node->right       = right;
    op_node->arg_number  = arg_number;

    return op_node;
}

Node* create_num_node(double value, Node* left, Node* right)
{
    Node* num_node = (Node*)calloc(1, sizeof(Node));
    if (num_node == nullptr)
        return nullptr;

    num_node->type       = type_num;
    num_node->value      = value;
    num_node->operation  = NOT_OPERATION;
    num_node->left       = left;
    num_node->right      = right;
    num_node->arg_number = 0;

    return num_node;
}

Node* create_var_node(char* variable, Node* left, Node* right)
{
    ASSERT(variable != nullptr);

    Node* var_node = (Node*)calloc(1, sizeof(Node));
    if (var_node == nullptr)
        return nullptr;

    var_node->type       = type_var;
    var_node->value      = 0;
    strcpy(var_node->var, variable);
    var_node->operation  = NOT_OPERATION;
    var_node->left       = left;
    var_node->right      = right;
    var_node->arg_number = 0;

    return var_node;
}

Node* copy_node(Node* ptr_node)
{
    ASSERT(ptr_node != nullptr);

    Node* copied_node = (Node*)calloc(1, sizeof(Node));
    if (copied_node == nullptr)
        return nullptr;

    // копирование данных из старого узла
    copied_node->type       = ptr_node->type;
    copied_node->operation  = ptr_node->operation;
    copied_node->value      = ptr_node->value;
    strcpy(copied_node->var, ptr_node->var);
    copied_node->arg_number = ptr_node->arg_number;
    // рекурсивный вызов в левое поддерево
    if (ptr_node->left != nullptr)
        copied_node->left = copy_node(ptr_node->left);
    
    // рекурсивный вызов в правое поддерево
    if (ptr_node->right != nullptr)
        copied_node->right = copy_node(ptr_node->right);

    return copied_node;
}

void delete_tree(Node* node)
{
    if (node->left != nullptr)
    {
        delete_tree(node->left);
    }
    if (node->right != nullptr)
    {
        delete_tree(node->right);
    }
    free(node);
}


Node* diff(Node* ptr_node) // обработка ошибок при вызове diff
{
    ASSERT(ptr_node != nullptr);

    Node* new_node = (Node*)calloc(1, sizeof(Node));
    if (new_node == nullptr)
        return nullptr;

    switch(ptr_node->type)
    {
        case type_uninit: return nullptr;
        case type_num: return _NUM(0);
        case type_var: return _NUM(1);
        case type_operation:
        {
            switch (ptr_node->operation)
            {
                case ADD:
                {
                    new_node = _ADD(dL, dR);
                    return new_node;
                }

                case SUB:
                {
                    new_node = _SUB(dL, dR);
                    return new_node;
                }

                case MUL:
                {
                    new_node = _ADD(_MUL(dL, cR), _MUL(cL, dR));
                    return new_node;
                }

                case DIV: 
                {
                    new_node = _DIV(_SUB(_MUL(dL, cR), _MUL(cL, dR)), _MUL(cR, cR));
                    return new_node;
                }

                case SIN:
                {
                    new_node = _MUL(dL, _COS(cL));
                    return new_node;
                }

                case COS:
                {
                    new_node = _MUL(_NUM(-1), _MUL(dL, _SIN(cL)));
                    return new_node;
                }

                case LN:
                {
                    new_node = _MUL(dL, _DIV(_NUM(1), cL));
                    return new_node;
                }

                case TAN:
                {
                    ASSERT(0);
                    return nullptr;
                }
                
                case POW:
                {
                    ASSERT(0);
                    return nullptr;
                }

                case NOT_OPERATION:
                {
                    fprintf(stderr, "shit happened\n");
                    ASSERT(0 && ":(");
                }

                default:
                {
                    fprintf(stderr, "bye bye \n");
                    ASSERT(0 && ":(");
                }
            } 
        }
    }
}

Node* differentiate(Node* ptr_node)
{
    ASSERT(ptr_node != nullptr);

    Node* diff_tree = diff(ptr_node);

    make_png_dump(diff_tree);
    Node* simplificated_tree = simplificate(diff_tree);
    make_png_dump(simplificated_tree);
    return simplificated_tree;
}

Node* recursive_descent(const char* buffer) // written
{
    ASSERT(buffer != nullptr);

    str = buffer;

    return get_G();
}

Node* get_G() // written
{
    Node* node = get_E();

    REQUIRE('$', str);

    return node;
}

Node* get_E() // written
{
    Node* node = get_T();

    while (*str == '+' || *str == '-')
    {
        char op = *str;
        str++;

        Node* right_node = get_T();

        if (op == '+')
        {
            node = create_op_node(ADD, node, right_node, 2);
        }
        else if (op == '-')
        {
            node = create_op_node(SUB, node, right_node, 2);
        }
    }

    return node;
}

Node* get_T() // written
{
    Node* node = get_O();

    while(*str == '*' || *str == '/')
    {
        char op = *str;
        str++;

        Node* right_node = get_O();

        if (op == '*')  
        {
            node = create_op_node(MUL, node, right_node, 2);
        }
        else if (op == '/')
        {
            node = create_op_node(DIV, node, right_node, 2);
        }
    }
    return node;
}

Node* get_O() // written
{
    if (strncmp(str, "sin", 3) == 0)
    {
        str += 3;

        return create_op_node(SIN, get_P(), nullptr, 1);
    }

    if (strncmp(str, "ln", 2) == 0)
    {
        str += 2;

        return create_op_node(LN, get_P(), nullptr, 1);
    }

    if (strncmp(str, "cos", 3) == 0)
    {
        str += 3;

        return create_op_node(COS, get_P(), nullptr, 1);
    }

    if (strncmp(str, "tg", 2) == 0)
    {
        str += 2;

        return create_op_node(TAN, get_P(), nullptr, 1);
    }

    return get_F();
}

Node* get_F() // written
{
    Node* node = get_P();

    while(*str == '^')
    {
        str++;

        Node* right_node = get_P();
        node = create_op_node(POW, node, right_node, 2);
    }

    return node;
}

Node* get_P() // written
{
    if (*str == '(')
    {
        str++;

        Node* node = get_E();
        REQUIRE(')', str);

        return node;
    }

    return get_N();
}

Node* get_N() // written
{

    if (*str == 'x')
    {
        str++;
        return create_var_node("x", nullptr, nullptr);
    }

    double val = 0;

    const char* old_str = str;

    while('0' <= *str && *str <= '9')
    {
        val = val * 10 + (*str - '0');
        str++;
    }

    if (old_str == str)
    {
        syntax_error();
    }
    
    return create_num_node(val, nullptr, nullptr);
}

void tree_dump(Node* ptr_node, FILE* file)
{
    ASSERT(ptr_node != nullptr);
    ASSERT(file != nullptr);

    if (ptr_node == nullptr)
    {
        return;
    }
    
    fprintf(file, "Указатель на данный узел: %p  \n", ptr_node);
    
    if (ptr_node->type == type_operation)
        switch(ptr_node->operation)
        {
        case(ADD):  
            fprintf(file, "Это сложение \n"); 
            break;
        case(SUB):
            fprintf(file, "Это вычитание \n");
            break;
        case(MUL):
            fprintf(file, "Это умножение \n");
            break;
        case(DIV):
            fprintf(file, "Это деление \n");
            break;
        case(SIN):
            fprintf(file, "Это синус \n");
            break;
        case(COS):
            fprintf(file, "Это косинус \n");
            break;
        case(LN):
            fprintf(file, "Это натуральный логарифм \n");
            break;
        case(TAN):
            fprintf(file, "Это тангенс \n");
            break;
        case(POW):
            fprintf(file, "Это степень \n");
            break;
        case(NOT_OPERATION):
            fprintf(file, "Ошибка \n");
            ASSERT(0 && ":(");
        }

    if (ptr_node->type == type_num)
        fprintf(file, "Значение константы:       %lf \n", ptr_node->value);
    if (ptr_node->type == type_var)
        fprintf(file, "Переменная:               %s  \n", ptr_node->var);

    fprintf(file, "Количество аргументов:    %zu \n",     ptr_node->arg_number);
    fprintf(file, "Левый подузел:            %p  \n",     ptr_node->left);
    fprintf(file, "Правый подузел:           %p  \n\n\n", ptr_node->right);

    if (ptr_node->left != nullptr)
        tree_dump(ptr_node->left, file);
    if (ptr_node->right != nullptr)
        tree_dump(ptr_node->right, file);
}

void syntax_error()
{
    fprintf(stderr, "SYNTAX ERROR!\n");

    exit(1);
}