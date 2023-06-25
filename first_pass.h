

#ifndef FINALPROJECT_FIRST_PASS_H
#define FINALPROJECT_FIRST_PASS_H



#include <stdio.h>
#include "assembly_tables.h"
#include "memory_word.h"
#include "label_table.h"

/* runs a first pass on a given file, data immage, instruction image & symbol table */
int firstPass(FILE *asfile, MemWord data_img[], MemWord instruct_img[], labelnode *symbolstable, int *store_ic, int *store_dc);


/* gets a register string, in the format "*r1" or "r1" (1 could be 1,2,..,8) and returns the number of that register (as an int) */
#define getRegNum(regstr) ((regstr[0] != '*')?(regstr[1] - '0'):(regstr[2]-'0'))
#endif
