
#ifndef FINALPROJECT_LABEL_TABLE_H
#define FINALPROJECT_LABEL_TABLE_H

#include <string.h>
#include <stdlib.h>

enum boolean {false,true};

#define CODE_LOC 1
#define DATA_LOC 2
#define NONE_LOC 3

#define EXTERNAL_TYPE 1
#define ENTRY_TYPE 2
#define NONE_TYPE 3

#define MAX_LABEL 32

struct node{
    char *name;
    int val;
    int location;
    int type;
    struct node *next;
};

typedef struct node *labelnode;

void addlabelnocheck(labelnode *headptr, char *name, int val, int location, int type);

void addlable(labelnode *headptr, char *name, int val, int location, int type);

int tablecontainslabel(labelnode head , char *lablename);

labelnode getlabel(labelnode head , char *lablename);

void freetable(labelnode head);

#endif
