#include <math.h>
#include <string.h>
#include "simplification.h"
#include "differentiator.h"
#include "debug.h"
#include "dsl.h"

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
                    SIMPLE_FUNC(node, sin);
                    *change_flag = true;
                    return node;
                }

                case COS:
                {
                    SIMPLE_FUNC(node, cos);
                    *change_flag = true;
                    return node;
                }

                case LN:
                {
                    SIMPLE_FUNC(node, log);
                    *change_flag = true;
                    return node;
                }
                case TAN:
                {
                    SIMPLE_FUNC(node, tan)
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
                    SIMPLE_ADD(node);
                    *change_flag = true;
                    return node;
                }

                case SUB:
                {
                    SIMPLE_SUB(node);
                    *change_flag = true;
                    return node;
                }

                case MUL:
                {
                    SIMPLE_MUL(node);
                    *change_flag = true;
                    return node;
                }

                case DIV:
                {
                    SIMPLE_DIV(node);
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

    if (node->type == type_operation && node->left != nullptr)
    {
        if (node->left->type == type_operation)
        {
            node->left = simplificate_op(node->left, change_flag);
        }
    }
    
    if (node->type == type_operation && node->right != nullptr)
    {
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
                node = simplificate_add(node, change_flag); 
                break;
            case SUB: 
                node = simplificate_sub(node, change_flag); 
                break;
            case MUL: 
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
        CHANGE_NODES(node, node->left, node->right);
        *change_flag = true;
        return node;
    }
    else if (node->right->type == type_num && node->right->value == 0)
    {
        CHANGE_NODES(node, node->right, node->left);
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
        CHANGE_NODES(node, node->right, node->left);
        *change_flag = true;
        return node;
    }
    return node;
}

Node* simplificate_mul(Node* node, bool* change_flag)
{
    ASSERT(node != nullptr);
    ASSERT(change_flag != nullptr);

    if ((node->left->type == type_num && node->left->value == 0) ||
                         (node->right->type == type_num && node->right->value == 0))
    {
        MAKE_ZERO_VALUE(node);
        *change_flag = true;
        return node;
    }
    else if (node->left->type == type_num && node->left->value == 1)
    {
        CHANGE_NODES(node, node->left, node->right);
        *change_flag = true;
        return node;
    }
    else if (node->right->type == type_num && node->right->value == 1)
    {
        CHANGE_NODES(node, node->right, node->left)
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
        CHANGE_NODES(node, node->right, node->left);
        *change_flag = true;
        return node;
    }
    else if (node->left->type == type_num && node->left->value == 0)
    {
        MAKE_ZERO_VALUE(node);
        *change_flag = true;
        return node;
    }
    return node;
}
