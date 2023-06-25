#ifndef FINALPROJECT_OUTPUT_MANAGEMENT_H
#define FINALPROJECT_OUTPUT_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include "memory_word.h"
#include "label_table.h"

/*creates the output files based on the data img & instruction img, the symbol table and the ic & dc.*/
void output_files_creation(MemWord data_img[], MemWord instruct_img[], labelnode sym_table, int ic, int dc,
                           char *obj_name, char *external_name, char *entry_name);
#endif
