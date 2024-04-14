#ifndef TREE_DUMP_H
#define TREE_DUMP_H

#include <stdio.h>
#include "differentiator.h"

void tree_dump_dot(FILE* dump_file, Node* node, int* node_counter);
void make_png_dump(Node* node);

#endif // TREE_DUMP_H