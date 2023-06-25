/* this file handling the creation of the output files from all the data collected during first & second pass.*/
#include "output_management.h"

/*gets a number and puts it's hexadecimal representation into a given buffer.
 * puts exactly 6 chars, padding zeros to the left of it if there is less than 6 chars
 * buffer must have min length of 7 (include's '\0')*/
void to_base_16(long num, char *buff){
    char holder[24] = {0}; /*large enough to hold the biggest number in hexa*/
    char *temp = holder;

    sprintf(holder, "%6.6x", (int) num);
    if (strlen(holder) > 6) {
        temp += strlen(holder) - 6;
    }
    strcpy(buff, temp);
}

/*gets a data image, instruction image, symbol table, ic & dc, and file names of the object, entry and external
 * it creates object file and entry & external if necessary only
 * in object : firstly the ic & dc as an "headline" then a list of addresses and their content in base 16
 * in entry : list of labels declared as entry and their value in the symbol table
 * in external : list of labels declared as extern and their value in the symbol table*/

void output_files_creation(MemWord data_img[], MemWord instruct_img[], labelnode sym_table, int ic, int dc,
                            char *obj_name, char *external_name, char *entry_name) {
    FILE *object, *entry, *external;
    labelnode current;
    int entry_flag, external_flag, i; /*flags to check if need to create those files or not*/

    entry_flag = external_flag = 0;

    /*first the object file creation : .obj*/
    if (!(object = fopen(obj_name, "w"))) {
        printf("Failed to open the file");
        return;
    }

    fprintf(object, "%d %d\n", ic, dc); /*headline first*/

    for (i = 0; i < (ic); i++){
        int address = i + STARTING_MEMORY;
        char word_content[6 + 1] = {0};
        to_base_16(instruct_img[i], word_content);
        fprintf(object, "%07d %s\n", address, word_content);
    }
    for (i = 0; i < dc; i++){
        int address = (ic) + STARTING_MEMORY + i;
        char word_content[7] = {0};
        to_base_16(data_img[i], word_content);
        fprintf(object, "%07d %s\n", address, word_content);
    }
    fclose(object);

    /*second the external file creation : .ext*/
    if (!(external = fopen(external_name, "w"))){
        printf("Failed to open the file");
        return;
    }

    current = sym_table;
    while (current){
        if (current->type == EXTERNAL_TYPE){
            external_flag = 1;
            fprintf(external, "%s %07d\n", current->name, current->val);
        }
        current = current->next;
    }
    fclose(external);
    if (!external_flag) {remove(external_name);} /*delete the file if there are no externals*/

    /*third the entry file creation : .ent*/
    if (!(entry = fopen(entry_name, "w"))){
        printf("Failed to open the file");
        return;
    }

    current = sym_table;
    while (current){
        if (current->type == ENTRY_TYPE){
            entry_flag = 1;
            fprintf(entry, "%s %07d\n", current->name, current->val);
        }
        current = current->next;
    }
    fclose(entry);
    if (!entry_flag) {remove(entry_name);}

}
