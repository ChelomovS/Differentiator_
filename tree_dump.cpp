#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "debug.h"
#include "tree_dump.h"
#include "differentiator.h"

int number_png = 1;

void tree_dump_dot(FILE* dump_file, Node* node, int* node_counter) 
{
    int counter_now = *node_counter;

	if (node != nullptr) 
    {
		if(node->type == type_operation) 
        {
			switch(node->operation) 
            {
				case ADD:
					fprintf(dump_file, "\tNode_%d [label = \"+\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]\n", *(node_counter));
					break;
				case SUB:
					fprintf(dump_file, "\tNode_%d [label = \"-\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]\n", *(node_counter));
					break;
				case MUL:
					fprintf(dump_file, "\tNode_%d [label = \"*\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]\n", *(node_counter));
					break;
				case DIV:
					fprintf(dump_file, "\tNode_%d [label = \"/\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]\n", *(node_counter));
					break;
                case SIN:
                    fprintf(dump_file, "\tNode_%d [label = \"sin\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]\n", *(node_counter));
					break;
                case COS:
                    fprintf(dump_file, "\tNode_%d [label = \"cos\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]\n", *(node_counter));
					break;
                case TAN:
                    fprintf(dump_file, "\tNode_%d [label = \"tg\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]\n", *(node_counter));
					break;
                case POW:
                    fprintf(dump_file, "\tNode_%d [label = \"^\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]\n", *(node_counter));
					break;
				default:
					break;
                
			}
		} 
        else if (node->type == type_var) 
        {
			fprintf(dump_file, "\tNode_%d [label = \"x\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]\n", *(node_counter));
		} 
        else if (node->type == type_num)
        { 
			fprintf(dump_file, "\tNode_%d [label = \"%lf\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]\n", *(node_counter), node->value);
		}
    }

    if (node->left != nullptr)
    {  
        (*node_counter)++; 
        fprintf(dump_file, "\tNode_%d -> Node_%d\n", counter_now, *(node_counter));
        tree_dump_dot(dump_file, node->left, node_counter);
    }
    if (node->right != nullptr)
    {
        (*node_counter)++; 
        fprintf(dump_file, "\tNode_%d -> Node_%d\n", counter_now, *(node_counter));
        tree_dump_dot(dump_file, node->right, node_counter);
    }
}

void make_png_dump(Node* node)
{
    FILE* graph_file = fopen("graphiz/graph.dot", "w");
    fprintf(graph_file, "digraph G{\n");
    int node_counter = 0;
    tree_dump_dot(graph_file, node, &node_counter);
    fprintf(graph_file, "}");
	fclose(graph_file);

    char call_graph[100] = " ";
    snprintf(call_graph, 50, "dot graphiz/graph.dot -Tpng -o graphiz/graph%d.png", number_png);
    number_png++;
    system(call_graph);
}

