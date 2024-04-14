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

    FILE* filein = fopen(argv[1], "r");
    size_t file_size = GetFileSize(filein);
 
    Differentiator differentiator = {};
    differentiator_ctr(&differentiator, file_size + 1);
    fprintf(stderr, "%zu", file_size);
    fread(differentiator.buffer, sizeof(char), file_size + 1, filein);
    fclose(filein);
    differentiator.ptr_node = recursive_descent(differentiator.buffer);
    make_png_dump(differentiator.ptr_node);
    differentiator.ptr_node = differentiate(differentiator.ptr_node);
    differentiator_dtr(&differentiator);
    return 0;
}