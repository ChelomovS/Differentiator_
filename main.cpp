#include <stdio.h>
#include "differentiator.h"
#include "fileLib.h"
#include "tree_dump.h"

int main(const int argc, const char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "TOO FEW FILES \n");
        return 0;
    }
    
    Differentiator differentiator = {};
    load_data(argv[1], &differentiator);
    differentiator.ptr_node = recursive_descent(differentiator.buffer);
    make_png_dump(differentiator.ptr_node);
    differentiator.ptr_node = differentiate(differentiator.ptr_node);
    differentiator_dtr(&differentiator);
    return 0;
}