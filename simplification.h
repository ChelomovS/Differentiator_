#ifndef SIMPLIFICATION_H
#define SIMPLIFICATION_H

#include "differentiator.h"
#include "simplification.h"

Node* simplificate    (Node* node);
Node* simplificate_num(Node* node, bool* change_flag);
Node* simplificate_op (Node* node, bool* change_flag);
Node* simplificate_add(Node* node, bool* change_flag);
Node* simplificate_sub(Node* node, bool* change_flag);
Node* simplificate_mul(Node* node, bool* change_flag);
Node* simplificate_div(Node* node, bool* change_flag);
//Node* simplificate_sin(Node* node, bool* change_flag);
//Node* simplificate_cos(Node* node, bool* change_flag);
//Node* simplificate_ln (Node* node, bool* change_flag);
//Node* simplificate_pow(Node* node, bool* change_flag);

#endif // SIMPLIFICATION_H
