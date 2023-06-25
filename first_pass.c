
#include "first_pass.h"
#include "input_management.h"

/* instruction counter & data counter */

int linenum = 0;
#define BITS_IN_CELL 24 /* each cell has 24 bits*/

int InstructlineDeciphering ( char *ptrlineafterinstruct, char *instruct, char *opt_label, MemWord image[], int *dc, labelnode *symbolstable);
int operationDeciphering(char *ptrafterline, char *operation, char *opt_label, MemWord image[],int *ic, labelnode *symbolstable);

int firstPass(FILE *asfile, MemWord data_img[], MemWord instruct_img[], labelnode *symbolstable, int *ic, int *dc) {
    labelnode current;
    int complete = TRUE;
    char line[MAX_LINE_LEN] = {0};
    char op_or_instruct[MAX_INSTRCUT_N_OPER] , label[MAX_LABEL_LEN];

    linenum = 0;

    while (scan_line(line, asfile) != EOF) {
        int instruct_type;
        char *ptr_after_label, *ptr_after_op_instruct;

        memset(op_or_instruct, 0, MAX_INSTRCUT_N_OPER);
        memset(label, 0, MAX_LABEL_LEN);

        linenum++;
        /* skipping white spaces and ; */
        if (*line == '\0' || line[0] == ';') { continue; }

        ptr_after_label = holds_label(line, label);

        if (ptr_after_label == (char *) ERR) {
            complete = FALSE;
            continue;
        }


        instruct_type = get_inst_or_op(ptr_after_label, op_or_instruct);

        if (instruct_type == ERR) {
            complete = FALSE;
            continue;
        }

        ptr_after_op_instruct = ptr_after_label + strlen(op_or_instruct);
        ptr_after_op_instruct = remove_space(ptr_after_op_instruct); /*--------------------MAYBE A MISTAKE WITH \n-----------------------*/

        if (instruct_type == INST) {
            if ((InstructlineDeciphering(ptr_after_op_instruct, op_or_instruct, label, data_img, dc, symbolstable)) == ERR) {
                complete = FALSE;
                continue;
            }
        } else if (instruct_type == OPER) {
            if ((operationDeciphering(ptr_after_op_instruct, op_or_instruct, label, instruct_img, ic, symbolstable)) == ERR) {
                complete = FALSE;
                continue;
            }
        }

    }
    /* increase all labels marked with data attribute (for second pass) */
    current = *symbolstable;
    while (current) {
        if (current->location == DATA_LOC) {
            current->val += *ic;
        }

        current = current->next;
    }

    
    return (!complete) ? ERR : TRUE;
}

int InstructlineDeciphering ( char *ptrlineafterinstruct, char *instruct, char *opt_label, MemWord image[],
                     int *dc, labelnode *symbolstable) {

    if (!holds_ex_text(ptrlineafterinstruct)) {
        print_err("instruction needs an operand")
        return ERR;
    }

    if (strcmp(instruct, DATA_I) == 0 || strcmp(instruct, STRING_I) == 0) { /* if the instruction is .string or .data */
        if (tablecontainslabel(*symbolstable, opt_label)) {
            print_err("The label already declared");
            return ERR;
        }
        /*checks if there is a label and adds it into symbol table as "data"*/
        if (opt_label && opt_label[0] != '\0') {
            addlable(symbolstable, opt_label, *dc, DATA_LOC,NONE_TYPE);
        }
        if (strcmp(instruct, STRING_I) == 0) {
            int i = 0;
            char *string = get_string(ptrlineafterinstruct);
            /* if the parameter is invalid */
            if (string == (char *) ERR) {
                return ERR;
            }
            /*  inserts a valid string it into the image */
            while(i < strlen(string)) {
                image[i + (*dc)] = string[i];
                i++;
            }
            (*dc) += i;
            image[(*dc)++] = 0;
        } else { /* then its .data */
            long num;
            char *numstring, *linecopy;
            unsigned mask24 = 0, i;

            /* create an int with all first 23 bits turned on, and others off*/
            for (i = 0; i < BITS_IN_CELL; ++i) {
                mask24 <<= 1;
                mask24 |= 1;
            }

            linecopy = malloc(strlen(ptrlineafterinstruct) + 1);
            strcpy(linecopy, ptrlineafterinstruct); /* creating a copy */


            if ((linecopy[strlen(linecopy) - 1] == ',') || (linecopy[0] == ',')) { /*------MADE A CHANGE HERE --------*/
                print_err("invalid comma");
                return ERR;
            }

            if(holds_dou_commas(linecopy)){ /*------MADE CHANGE HERE ---------*/
                print_err("two consecutive commas (,,).")
            }

            numstring = strtok(linecopy, ",");

            while (numstring) {
                int sign;
                numstring = remove_space(numstring);
                /*if number invalid returns error*/
                if (!valid_num(numstring)) {
                    free(linecopy);
                    return ERR;
                }
                /* move the sign bit from the last one to the 23th bit*/
                num = atoi(numstring);
                sign = (num & (1 << (sizeof(int) * 8 - 8))) ? 1 : 0; /* get bit of sign*/
                num &= mask24; /* turn off all bits after the 23th*/
                num |= sign << (BITS_IN_CELL - 1); /* put the sign bit in place- at the 23th bit*/
                /*  add the number to image*/
                image[(*dc)++] = num;
                /* move to the next number */
                numstring = strtok(NULL, ",");
            }
            free(linecopy);
        }
    } else { /* if label*/
        char *checklabel;
        if (!ptrlineafterinstruct) {
            print_err("no label after instruction");
            return ERR;
        }
        checklabel = malloc(strlen(ptrlineafterinstruct + 2));
        strcpy(checklabel, ptrlineafterinstruct);
        strcat(checklabel, ":c");
        if (holds_label(checklabel, ptrlineafterinstruct) == (char *) ERR) {
            free(checklabel);
            return ERR;
        }
        if (strcmp(instruct, EXTERN_I) == 0) {
            labelnode temp;

            free(checklabel);

            if ((temp = (getlabel(*symbolstable, ptrlineafterinstruct))) && temp->type != EXTERNAL_TYPE) {
                print_err("a declared label cannot be external");
                return ERR;
            } else {
                /* insert argument as external label with value 0 into the symbol table*/
                addlable(symbolstable, ptrlineafterinstruct, 0, NONE_LOC, EXTERNAL_TYPE);
            }

        }
    }
    return TRUE;
}

int operationDeciphering(char *ptrafteroperand, char *operation, char *opt_label, MemWord image[],int *ic, labelnode *symbolstable) {
    int numargs, temp_ic = (*ic) - STARTING_MEMORY;
    /* firstly deal with label */
    if (opt_label && *opt_label != '\0') {
        /* then a label exists.*/
        if (tablecontainslabel(*symbolstable, opt_label)) {
            print_err("label already declared")
            return ERR;
        }
        /* insert it as code (location attribute), with loading index of ic value  */
        addlable(symbolstable, opt_label, *ic, CODE_LOC, NONE_TYPE);
    }

    /* move after the operation*/
    numargs = how_many_args(operation);/* get number of arguments the operation takes */
    if (numargs == 0) {
        if (holds_ex_text(ptrafteroperand)) {
            print_err("extra text after operation");
        }
        /* source & destination adressing methods are 0- we have no arguments at all */
        image[temp_ic] = fir_word(getOpcode(operation),0,0,0, 0, getFunct(operation));
        temp_ic++;
        (*ic)++;
    } else if (numargs >= 1) {
        int addrM1, addrM2, code;
        char arg1[MAX_LABEL_LEN], arg2[MAX_LABEL_LEN];

        addrM1 = addrM2 = 0;
        if (ptrafteroperand == NULL){
            print_err("operation takes at least one operand")
            return ERR;
        }
        if (holds_dou_commas(ptrafteroperand)) {
            print_err(" two consecutive commas (,,)")
            return ERR;
        }

        /* fetch first argument into arg1*/
        code = get_argument(ptrafteroperand, arg1);
        if (code == ERR) {
            return ERR;
        }
        /* get adressing method*/
        addrM1 = getAddressingM(arg1);

        if (numargs == 1) {
            if (holds_ex_text(ptrafteroperand + strlen(arg1))&&(ptrafteroperand[strlen(ptrafteroperand)-1] != '\n')&& ptrafteroperand[0]!='&') {
                print_err("extra text after command");
                return ERR;
            }
            /* check iif the adressing method fits with the operation */
            if (valid_addressing(operation, -1, addrM1) == FALSE) {
                return ERR;
            }
            /* then the argument is valid. */

        } else { /* then it's 2 arguments*/
            /* skip the first argument */
            ptrafteroperand += strlen(arg1);
            ptrafteroperand = remove_space(ptrafteroperand);

            if (ptrafteroperand == NULL){ /*-----MADE A CHANGE HERE ------*/
                print_err("operation takes two operands.")
                return ERR;
            }

            if (/*ptrafteroperand && */ ptrafteroperand[0] == ',') { /*-----MADE A CHANGE HERE ------*/
                ptrafteroperand++;
            }
            /* fetch the second argument into arg2 */
            code = get_argument(ptrafteroperand, arg2);
            if (code == ERR) {
                return ERR;
            }
            /* check if addressing methods are valid for operation*/
            addrM2 = getAddressingM(arg2);
            if (!valid_addressing(operation, addrM1, addrM2)) {
                return ERR;
            }
            ptrafteroperand = remove_space(ptrafteroperand);
            if (holds_ex_text(ptrafteroperand + strlen(arg2))) {
                print_err("extra text after command");
                return ERR;
            }

            /* then valid two arguments. */

        }


        /* check if we have two register arguments*/
        if ((addrM1 == ADDRESS_M_REG_DIRECT) && (addrM2 == ADDRESS_M_REG_DIRECT )) {
            int reg1 = getRegNum(arg1);
            int reg2 = getRegNum(arg2);
            /* insert the first word */
            image[temp_ic] = fir_word(getOpcode(operation),addrM1, reg1, addrM2, reg2, getFunct(operation));
            temp_ic++;
            (*ic)++;
        } else {
            if (numargs == 1) {
                /* arg1 is destination argument if numargs is 1*/
                int is_arg_1_reg = (addrM1 == ADDRESS_M_REG_DIRECT ? getRegNum(arg1) : 0);

                image[temp_ic] = fir_word(getOpcode(operation), 0, 0, addrM1, is_arg_1_reg, getFunct(operation));
                temp_ic++;
                (*ic)++;
                if(addrM1 != ADDRESS_M_REG_DIRECT) {
                    if (addrM1 == ADDRESS_IMMEDIATE) {
                        /* then insert the number it represents */
                        int num;
                        char *numst = arg1 + 1; /* skip the '#' */
                        if (!valid_num(numst)) {
                            return ERR;
                        }

                        num = atoi(numst);
                        image[temp_ic] = additional_word(num, ADDRESS_IMMEDIATE, 0);
                        temp_ic++;
                        (*ic)++;
                    } else { /* then its ADDRESS_DIRECT or ADDRESS_RELATIVE */
                        /* save space for later- ADDRESS_DIRECT & ADDRESS_RELATIVE will be handled in the second passage */
                        image[temp_ic] = 0;
                        temp_ic++;
                        (*ic)++;
                    }
                }

            }
            else if(numargs == 2){
                int is_arg_1_reg = (addrM1 == ADDRESS_M_REG_DIRECT ? getRegNum(arg1) : 0);
                int is_arg_2_reg = (addrM2 == ADDRESS_M_REG_DIRECT ? getRegNum(arg2) : 0);

                image[temp_ic] = fir_word(getOpcode(operation), addrM1, is_arg_1_reg, addrM2, is_arg_2_reg, getFunct(operation));
                temp_ic++;
                (*ic)++;

                if(addrM1 != ADDRESS_M_REG_DIRECT) {
                    if (addrM1 == ADDRESS_IMMEDIATE) {
                        int num;
                        char *numst = arg1 + 1;
                        if (!valid_num(numst)) {
                            return ERR;
                        }

                        num = atoi(numst);
                        image[temp_ic] = additional_word(num, ADDRESS_IMMEDIATE, 0);
                        temp_ic++;
                        (*ic)++;
                    } else {
                        image[temp_ic] = 0;
                        temp_ic++;
                        (*ic)++;
                    }
                }

                if(addrM2 != ADDRESS_M_REG_DIRECT) {
                    if (addrM2 == ADDRESS_IMMEDIATE) {
                        int num;
                        char *numst = arg2 + 1;
                        if (!valid_num(numst)) {
                            return ERR;
                        }

                        num = atoi(numst);
                        image[temp_ic] = additional_word(num, ADDRESS_IMMEDIATE, 0);
                        temp_ic++;
                        (*ic)++;
                    } else {
                        image[temp_ic] = (int)0;
                        temp_ic++;
                        (*ic)++;
                    }
                }
            }
        }
    }
    /*everything went through without any errors */
    return TRUE;
}
