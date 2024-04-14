#include <math.h>
#include <string.h>
#include "simplification.h"
#include "differentiator.h"
#include "debug.h"

Node* simplificate(Node* node)
{
    ASSERT(node != nullptr);
    
    bool change_flag = false;
    do
    {
        change_flag = false;

        if (node->type == type_operation)
        {
            node = simplificate_num(node, &change_flag);
        }

        if (node->type == type_operation)
        {
            node = simplificate_op(node, &change_flag);
        }
    }
    while (change_flag == true);

    return node;
}

Node* simplificate_num(Node* node, bool* change_flag) // упрощает выражения с константами
{
    ASSERT(node != nullptr);
    ASSERT(change_flag != nullptr);

    if (node->left != nullptr)
    {
        if (node->left->type == type_operation)
        {
           node->left = simplificate_num(node->left, change_flag);
        }
    }

    if (node->right != nullptr)
    {
        if (node->right->type == type_operation)
        {
            node->right = simplificate_num(node->right, change_flag);
        }
    }

    if (node->arg_number == 1)
    {
        if (node->left->type == type_num)
        {
            switch(node->operation)
            {
                case SIN:
                {
                    node->type = type_num;
                    node->operation = NOT_OPERATION;
                    node->value = sin(node->left->value);
                    node->arg_number = 0;
                    delete_tree(node->left);
                    node->left = nullptr;
                    *change_flag = true;
                    return node;
                }

                case COS:
                {
                    node->type = type_num;
                    node->operation = NOT_OPERATION;
                    node->value = cos(node->left->value);
                    node->arg_number = 0;
                    delete_tree(node->left);
                    node->left = nullptr; 
                    *change_flag = true;
                    return node;
                }

                case LN:
                {
                    node->type = type_num;
                    node->operation = NOT_OPERATION;
                    node->value = log(node->left->value);
                    node->arg_number = 0;
                    delete_tree(node->left);
                    node->left = nullptr;
                    *change_flag = true;
                    return node;
                }
                case TAN:
                {
                    node->type = type_num;
                    node->operation = NOT_OPERATION;
                    node->value = tan(node->left->value);
                    node->arg_number = 0;
                    delete_tree(node->left);
                    node->left = nullptr;
                    *change_flag = true;
                    return node;
                }
                default:
                // erorrs
                break;
            }
        }
    }
    else if (node->arg_number == 2) 
    {
        if (node->left->type == type_num && node->right->type == type_num)
        {
            switch (node->operation)
            {
                case ADD:
                {
                    node->type = type_num;
                    node->operation = NOT_OPERATION;
                    node->value = node->left->value + node->right->value;
                    node->arg_number = 0;
                    delete_tree(node->left);
                    node->left = nullptr;
                    delete_tree(node->right);
                    node->right = nullptr;
                    *change_flag = true;
                    return node;
                }

                case SUB:
                {
                    node->type = type_num;
                    node->operation = NOT_OPERATION;
                    node->value = node->left->value - node->right->value;
                    node->arg_number = 0;
                    delete_tree(node->left);
                    node->left = nullptr;
                    delete_tree(node->right);
                    node->right = nullptr;
                    *change_flag = true;
                    return node;
                }

                case MUL:
                {
                    node->type = type_num;
                    node->operation = NOT_OPERATION;
                    node->value = node->left->value * node->right->value;
                    node->arg_number = 0;
                    delete_tree(node->left);
                    node->left = nullptr;
                    delete_tree(node->right);
                    node->right = nullptr;
                    *change_flag = true;
                    return node;
                }

                case DIV:
                {
                    node->type = type_num;
                    node->operation = NOT_OPERATION;
                    node->value = node->left->value / node->right->value;
                    node->arg_number = 0;
                    delete_tree(node->left);
                    node->right = nullptr;
                    delete_tree(node->right);
                    node->left = nullptr;
                    *change_flag = true;
                    return node;
                }
                default:
                    break;               
            }
        } 
    }

    return node;
}

Node* simplificate_op(Node* node, bool* change_flag)
{
    ASSERT(node != nullptr);
    ASSERT(change_flag != nullptr);

    if (node->type == type_operation)
    {
        if (node->left->type == type_operation)
        {
            node->left = simplificate_op(node->left, change_flag);
        }
        if (node->right->type == type_operation)
        {
            node->right = simplificate_op(node->right, change_flag);
        }
    }

    if (node->type == type_operation)
    {
        switch (node->operation)
        {
            case ADD: 
                printf("симпл сложение \n");
                node = simplificate_add(node, change_flag); 
                break;
            case SUB: 
                node = simplificate_sub(node, change_flag); 
                break;
            case MUL: 
                printf("симпл умножение \n");
                node = simplificate_mul(node, change_flag); 
                break;
            case DIV: 
                node = simplificate_div(node, change_flag);
                break;
            default:
                break;
        }
    }

    return node;
}

Node* simplificate_add(Node* node, bool* change_flag)
{
    ASSERT(node != nullptr);
    ASSERT(change_flag != nullptr);
    
    if (node->left->type == type_num && node->left->value == 0)
    {
        delete_tree(node->left);
        Node* node_holder = node;    
        node = node->right;
        free(node_holder);
        *change_flag = true;
        return node;
    }
    else if (node->right->type == type_num && node->right->value == 0)
    {
        delete_tree(node->right); 
        Node* node_holder = node;     
        node = node->left;
        free(node_holder);
        *change_flag = true;
        return node;
    }

    return node;
}

Node* simplificate_sub(Node* node, bool* change_flag)
{
    ASSERT(node != nullptr);
    ASSERT(change_flag != nullptr);

    if (node->right->type == type_num && node->right->value == 0)
    {
        Node* node_holder = node;
        delete_tree(node->right);        
        node = node->left;
        free(node_holder);
        *change_flag = true;
        return node;
    }
    return node;
}

Node* simplificate_mul(Node* node, bool* change_flag)
{
    ASSERT(node != nullptr);
    ASSERT(change_flag != nullptr);

    if ((node->left->type == type_num  && node->left->value == 0) || (node->right->type == type_num && node->right->value == 0))
    {
        fprintf(stderr, "Я тут 0\n");
        node->type = type_num;
        node->operation = NOT_OPERATION;
        node->value = 0;
        node->arg_number = 0;
        delete_tree(node->left);
        node->left = nullptr;
        delete_tree(node->right);
        node->right = nullptr;
        *change_flag = true;
        return node;
    }
    else if (node->left->type == type_num && node->left->value == 1)
    {
        fprintf(stderr, "Ятут 1\n");
        Node* node_holder = node;
        delete_tree(node->left);    
        node = node->right;
        free(node_holder);
        *change_flag = true;
        return node;
    }
    else if (node->right->type == type_num && node->right->value == 1)
    {
        Node* node_holder = node;
        delete_tree(node->right);      
        node = node->left;
        free(node_holder);
        *change_flag = true;
        return node;
    }
    return node;
}

Node* simplificate_div(Node* node, bool* change_flag)
{
    ASSERT(node != nullptr);
    ASSERT(change_flag != nullptr);

    if (node->right->value == 1)
    {
        Node* holder_node = node->left;
        delete_tree(node->right);        
        node = copy_node(node->left);
        free(holder_node);
        *change_flag = true;
        return node;
    }
    else if (node->left->type == type_num && node->left->value == 0)
    {
        node->type = type_num;
        node->operation = NOT_OPERATION;
        node->value = 0;
        node->arg_number = 0;
        delete_tree(node->left);
        delete_tree(node->right);
        node->left = nullptr;
        node->right = nullptr;
        *change_flag = true;
        return node;
    }
    return node;
}
