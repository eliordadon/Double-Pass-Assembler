
#include <string.h>
#include "assembly_tables.h"

char *registers[REG_SIZE] = {R_0, R_1, R_2, R_3, R_4, R_5, R_6, R_7};

char *instructs[INSTRUCTS_SIZE] = {DATA_I, STRING_I, ENTRY_I, EXTERN_I};

char *operations_0[OPERATION_0_SIZE] = {RTS_O, STOP_O};
char *operations_1[OPERATION_1_SIZE] = {CLR_O, NOT_O, INC_O, DEC_O, JMP_O, BNE_O, JSR_O, RED_O, PRN_O};
char *operations_2[OPERATION_2_SIZE] = {MOV_O, CMP_O, ADD_O, SUB_O, LEA_O};

/*gets an array of strings, length of the strings arr, and a string to search for in the strings arr.
 * return index + 1 if found, FALSE otherwise.*/
int is_in (char *strings_arr[], char *str_to_search, int arr_len){
    int index = 0;

    while (index < arr_len){
        if (!strcmp(strings_arr[index],str_to_search)) {return index+1;}
        index++;
    }

    return FALSE;
}

/*gets a string and check whether it's a reg / instruct / operations with 0, 1, and 2 arguments.
 * return the type of the string, FALSE otherwise.*/
int is_who(char *str){
    if (is_in(registers,str,REG_SIZE)) {return REGISTER;}

    if (is_in(instructs,str,INSTRUCTS_SIZE)) {return INSTRUCT;}

    if (is_in(operations_0,str,OPERATION_0_SIZE)) {return OPER_0_ARG;}

    if (is_in(operations_1,str,OPERATION_1_SIZE)) {return OPER_1_ARG;}

    if (is_in(operations_2,str,OPERATION_2_SIZE)) {return OPER_2_ARG;}

    return FALSE;
}

/*gets a string and check whether it's a saved assembly word, return TRUE if it is, FALSE otherwise*/
int is_saved_word(char *str){
    return is_who(str) == FALSE ? FALSE : TRUE;
}

/*gets a string that represent an operation and return It's opcode.
 * assuming that operation is valid*/
int getOpcode(char *operation){
    if ((!strcmp(operation, JMP_O) || !strcmp(operation, BNE_O) || !strcmp(operation, JSR_O))) {return JMP_BNE_JSR_OPC;}

    if ((!strcmp(operation, ADD_O) || !strcmp(operation, SUB_O))) {return ADD_SUB_OPC;}

    return (!strcmp(operation, MOV_O)) ? MOV_OPC : (!strcmp(operation, CMP_O)) ? CMP_OPC : (!strcmp(operation, LEA_O)) ?
            LEA_OPC : (!strcmp(operation, RED_O)) ? RED_OPC : (!strcmp(operation, PRN_O)) ?
            PRN_OPC : (!strcmp(operation, RTS_O)) ? RTS_OPC : (!strcmp(operation, STOP_O)) ? STOP_OPC : CLR_NOT_INC_DEC_OPC;
}

/*gets a string that represent an operation and return It's funct.
 * assuming that operation is valid*/
int getFunct(char *operation){
    if (!strcmp(operation, ADD_O) || !strcmp(operation, CLR_O) || !strcmp(operation, JMP_O)) {return ADD_CLR_JMP_FUNCT;}

    if (!strcmp(operation, SUB_O) || !strcmp(operation, NOT_O) || !strcmp(operation, BNE_O)) {return SUB_NOT_BNE_FUNCT;}

    if (!strcmp(operation, INC_O) || !strcmp(operation, JSR_O)) {return INC_JSR_FUNCT;}

    if (!strcmp(operation, DEC_O)) {return DEC_FUNCT;}

    return NONE_FUNCT;
}

/*gets a string represent an argument of an operation and returns his addressing method */
int getAddressingM (char *argument){
    if (argument[0] == '#') {return ADDRESS_IMMEDIATE;}

    if (argument[0] == '&') {return ADDRESS_M_RELATIVE;}

    if (is_who(argument) == REGISTER) {return ADDRESS_M_REG_DIRECT;}

    return ADDRESS_M_DIRECT;
}


