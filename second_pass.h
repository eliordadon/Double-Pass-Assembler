#ifndef FINALPROJECT_SECOND_PASS_H
#define FINALPROJECT_SECOND_PASS_H

#include <stdio.h>
#include "memory_word.h"
#include "label_table.h"

/*gets a file contains assembly commands, instruction image and symbol table and performs a second passage on the file*/
int sec_pass (FILE *fd, MemWord instruct_img[], labelnode *sym_table);

#endif
