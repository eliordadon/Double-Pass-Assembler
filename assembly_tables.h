#ifndef PROJECT_ASSEMBLER_ASSEMBLY_TABLES_H
#define PROJECT_ASSEMBLER_ASSEMBLY_TABLES_H

#define TRUE 1
#define FALSE 0
#define ERR -1

/*opcodes*/
#define MOV_OPC 0
#define CMP_OPC 1
#define ADD_SUB_OPC 2
#define LEA_OPC 4
#define CLR_NOT_INC_DEC_OPC 5
#define JMP_BNE_JSR_OPC 9
#define RED_OPC 12
#define PRN_OPC 13
#define RTS_OPC 14
#define STOP_OPC 15

/*functs*/
#define NONE_FUNCT 0
#define ADD_CLR_JMP_FUNCT 1
#define SUB_NOT_BNE_FUNCT 2
#define INC_JSR_FUNCT 3
#define DEC_FUNCT 4

/* instructions */
#define INSTRUCTS_SIZE 4
#define DATA_I ".data"
#define STRING_I ".string"
#define ENTRY_I ".entry"
#define EXTERN_I ".extern"

/* registers */
#define REG_SIZE 8
#define R_0 "r0"
#define R_1 "r1"
#define R_2 "r2"
#define R_3 "r3"
#define R_4 "r4"
#define R_5 "r5"
#define R_6 "r6"
#define R_7 "r7"

/* operations with 0 operands */
#define OPERATION_0_SIZE 2
#define RTS_O "rts"
#define STOP_O "stop"

/* operations with 1 operands */
#define OPERATION_1_SIZE 9
#define CLR_O "clr"
#define NOT_O "not"
#define INC_O "inc"
#define DEC_O "dec"
#define JMP_O "jmp"
#define BNE_O "bne"
#define JSR_O "jsr"
#define RED_O "red"
#define PRN_O "prn"

/* operations with 2 operands */
#define OPERATION_2_SIZE 5
#define MOV_O "mov"
#define CMP_O "cmp"
#define ADD_O "add"
#define SUB_O "sub"
#define LEA_O "lea"

/* maximum lengths */
#define MAX_LINE_LEN 81 /*including '\n'*/
#define MAX_LABEL_LEN 32 /*including '\0'*/
#define MAX_INSTRCUT_N_OPER 8 /* .string is the longest*/

/* addressing methods */
enum addressing_methods {ADDRESS_IMMEDIATE, ADDRESS_M_DIRECT, ADDRESS_M_RELATIVE, ADDRESS_M_REG_DIRECT }; /*from 0 to 3*/
/* types */
enum type {REGISTER = 1, INSTRUCT, OPER_0_ARG, OPER_1_ARG, OPER_2_ARG};

/*function prototype*/
int is_in (char *strings_arr[], char *str_to_search, int arr_len);
int is_who(char *str);
int is_saved_word(char *str);
int getOpcode(char *operation);
int getFunct(char *operation);
int getAddressingM (char *argument);


#endif
