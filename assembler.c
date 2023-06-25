/*This Project made by Elior Dadon and Mishel Magera

this file contains the main function and combines everything into one working assembler
 * the files are giving as command line arguments, no limit for the files provided*/

#include "input_management.h"
#include "first_pass.h"
#include "second_pass.h"
#include "output_management.h"

/*initializes a string that will be represent a given file name + a given extension*/
#define init_(name, file_name, extension){\
name = malloc(strlen(file_name) + strlen(extension) + 1);\
strcpy(name, file_name);\
strcat(name, extension);\
}

/*frees everything needs to be free in every iteration at main*/
#define freeiter() {\
/*free(ic);\
free(dc);*/\
free(as_name);\
free(obj_name);\
free(externals_name);\
free(entry_name);\
freetable(symbol_table);\
}

#define MAX_EXTENSION 4 /*.as + '\0'*/

int main(int argc, const char *argv[]){
    char *as_name, *obj_name, *externals_name, *entry_name;
    int file_index, iteration_check;

    as_name = obj_name = externals_name = entry_name = NULL;

    if (argc < 2){
        printf("assembly files have not provided, provide the files names as command line arguments without extension");
        exit(0);
    }
    /*each iteration of loop handling a single file*/
    for (file_index = 1; file_index < argc; file_index++){
        FILE *fd;
        int ic, dc;
        labelnode symbol_table = NULL;
        MemWord data_img[MAX_DATA] = {0}, instruct_img[MAX_INSTRUCT] = {0};

        ic = 100;
        dc = 0;

        as_name = malloc(strlen(argv[file_index] + MAX_EXTENSION + 1)); /*+1 for '\0'*/
        strcpy(as_name, argv[file_index]); /*without .as*/

        if(!(fd = fopen(strcat(as_name, ".as"), "r"))){
            printf("Cannot open file \"%s\", moving to the next file\n", as_name);
            freeiter()
            continue;
        }

        /*strings holding output files names*/
        init_(obj_name, argv[file_index], ".ob")
        init_(externals_name, argv[file_index], ".ext")
        init_(entry_name, argv[file_index], ".ent")

        putchar('\n');
        printf("SCANNING FILE \"%s\" ...\n", as_name);
        /**/
        iteration_check = firstPass(fd, data_img, instruct_img, &symbol_table, &ic, &dc);
        if (iteration_check == ERR){
            freeiter()
            continue;
        }
        /*printf("\nfinish image printing for FIRST PASS (only instruct image)\n"); ------------TESTING PRINTING ---------------*/
        /**/
        rewind(fd);
        /**/
        iteration_check = sec_pass(fd, instruct_img, &symbol_table);
        if (iteration_check == ERR){
            freeiter()
            continue;
        }
        fclose(fd); /*done with the file*/

        /**/

        printf("The file is scanned in it's entirety without errors.\nStarting the creation of the output files \"%s\", \"%s\" & \"%s\"...\n", obj_name, externals_name, entry_name);
        ic -= STARTING_MEMORY;
        output_files_creation(data_img, instruct_img, symbol_table, ic, dc, obj_name, externals_name, entry_name);
        printf("Files creation completed !\n");

        /**/
        freeiter()
    }
    putchar('\n');
    return 0;
}
