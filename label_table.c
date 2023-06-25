
#include "label_table.h"

void addlabelnocheck(labelnode *headptr, char *name, int val, int location, int type){
	labelnode head=*headptr;
    labelnode newnode=malloc(sizeof(struct node));
    newnode->val = val;
    newnode->location = location;
    newnode->type = type;
    (newnode->name) = malloc(MAX_LABEL);
    strcpy(newnode->name, name);
    if (head == NULL) {
        newnode->next = NULL;
        *headptr = newnode;
    }
    else {
        newnode->next = head;
        *headptr = newnode;

    }
	
}

void addlable(labelnode *headptr, char *name, int val, int location, int type){
    if (tablecontainslabel(*headptr, name))
        return;
    addlabelnocheck(headptr, name, val, location, type);
}

/*The function gets a lable table list and a lable name
 * return true if lable of that name exist in the list
 * and false otherwise*/
int tablecontainslabel(labelnode head , char *lablename){
    labelnode temp=head;
    if(temp==NULL)
        return false;
    while(temp){
        if(strcmp(temp->name,lablename)==0)
            return true;
        temp=temp->next;
    }
    return false;
}

/*The function geta a lable table list and a lable name
 * return the node of the name if it exist
 * and NULL otherwise*/
labelnode getlabel(labelnode head , char *lablename){
    labelnode temp=head;
    while(temp){
        if(strcmp(temp->name,lablename)==0)
            return temp;
        temp=temp->next;
    }
    return NULL;
}


/*geta a list and frees its elements*/
void freetable(labelnode head) {
    labelnode p;
    while (head) {
        p = head;
        head = head->next;
        free(p);
    }
}
