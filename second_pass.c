/*this file is dedicated to handle the Second Pass on a given file*/

#include "second_pass.h"
#include "input_management.h"
#include "assembly_tables.h"

extern int linenum;

int oper_handle_II (char *, char *, char *, MemWord [], int, labelnode *); /* handle with ADDR_DIR operands*/

/*does a second pass on a given file, return TRUE on success, ERROR otherwise*/
int sec_pass (FILE *fd, MemWord instruct_img[], labelnode *sym_table){
    int ic = 0 , success_flag = TRUE;
    char line[MAX_LINE_LEN] = {0};

    linenum = 0;

    while (scan_line(line, fd) != EOF){
        char label[MAX_LABEL_LEN] = {0}, *after_label, inst_or_op[MAX_INSTRCUT_N_OPER];
        linenum++;

        if (*line == '\0' || line[0] == ';') { continue;} /*skip lines with only whitespaces, and comment ones*/

        after_label = holds_label(line, label);
        get_inst_or_op(after_label, inst_or_op);

        /*dealt with data / string / extern in first pass, so skip them*/
        if (!strcmp(inst_or_op, DATA_I) || !strcmp(inst_or_op, STRING_I) || !strcmp(inst_or_op, EXTERN_I)) { continue;}

        if (!strcmp(inst_or_op, ENTRY_I)){ /*in first pass we didn't check for errors, so now we will*/
            char *after_entry = remove_space(after_label + strlen(ENTRY_I));
            char *temp_label = malloc(MAX_LABEL_LEN + 2); /*+2 for ":c"*/
            char *reference = temp_label; /*for freeing*/
            char after_entry_label[MAX_LABEL_LEN] = {0}; /*for using in holds_label*/
            labelnode label_II;

            if (!after_entry || !strlen(after_entry)){
                print_err(".entry must have an operand");
                success_flag = 0;
                free(reference);
                continue;
            }

            strcpy(temp_label, after_entry);

            if (holds_space(temp_label)){
                print_err("whitespaces inside label (not before / after) are not allowed");
                success_flag = 0;
                free(reference);
                continue;
            }

            temp_label = strcat(temp_label, ":c");
            if (holds_label(temp_label, after_entry_label) == (char *) ERR){
                success_flag = 0;
                free(reference);
                continue;
            }

            /*valid label, now need to check that label is declared*/
            label_II = getlabel(*sym_table, temp_label);
            if (label_II == NULL){
                print_err("label has not been defined");
                success_flag = 0;
                free(reference);
                continue;
            }

            if (label_II->type != EXTERNAL_TYPE){ /*if not external - add entry type to label in symbol table*/
                label_II->type = ENTRY_TYPE;
            }
            else{ /*if external - add another label to symbol table with val 0 just for printing in the end*/
                addlable(sym_table, label_II->name, 0, label_II->location, ENTRY_TYPE);
            }

            free(reference);
        }
        else{ /*then it's an operation*/
            char *after_oper = after_label + strlen(inst_or_op);
            after_oper = remove_space(after_oper);

            ic = oper_handle_II(after_oper, inst_or_op, label, instruct_img, ic, sym_table);
            if (ic == ERR) {success_flag = 0;}
        }
    }

    return (!success_flag) ? ERR : TRUE;
}

/*gets a line containing arguments of an operation, operation name, optional label, instruction image, the next free index in image and a symbol table.
 * codes all arguments that addressed by direct addressing method. on error returns ERR, otherwise return the free index.
 * the function will not check for an input issues*/
int oper_handle_II (char *line_after_op, char *operation, char *optional_label, MemWord image[], int free_index, labelnode *sym_table){
    int args_number = how_many_args(operation);

    free_index++; /*no need to deal with first word*/

    if (args_number >= 1) { /*only the operations that take at least one operand*/
        char arg_I[MAX_LABEL_LEN], arg_II[MAX_LABEL_LEN];
        int addr_m_I, addr_m_II;

        addr_m_I = addr_m_II = 0;

        get_argument(line_after_op, arg_I);
        addr_m_I = getAddressingM(arg_I);

        if (args_number == 2) { /*2 operands, fetch the other one*/
            line_after_op += strlen(arg_I);
            line_after_op = remove_space(line_after_op) + 1;
            get_argument(line_after_op, arg_II);

            addr_m_II = getAddressingM(arg_II);
        }

        if ((addr_m_I == ADDRESS_M_REG_DIRECT) && (addr_m_II == ADDRESS_M_REG_DIRECT)) { /*then only first word*/
            return free_index;
        }
        else{
            if (addr_m_I == ADDRESS_M_DIRECT){ /*it's a label - fill the missing data*/
                labelnode node_I = getlabel(*sym_table, arg_I);
                int is_extern, address;
                if (node_I == NULL){
                    print_err("declaration of label is missing");
                    return ERR;
                }

                address = node_I->val;
                is_extern = (node_I->type) == EXTERNAL_TYPE;

                image[free_index] = additional_word(address, ADDRESS_M_DIRECT, is_extern);

                if (node_I->type == EXTERNAL_TYPE) { /*because in first pass we gave val 0*/
                    if(node_I->val == 0){
                        node_I->val = free_index + STARTING_MEMORY;
                    } else {
                        addlabelnocheck(sym_table, node_I->name, (free_index + STARTING_MEMORY), NONE_LOC, EXTERNAL_TYPE); /*------MADE CHANGE HERE--------*/
                    }
                }
            }
            else if(addr_m_I == ADDRESS_M_RELATIVE){
                labelnode node_I = getlabel(*sym_table, (arg_I+1));
                long distance;

                if(node_I == NULL){
                    print_err("declaration of label is missing")
                    return ERR;
                }


                distance = (node_I->val) - (free_index + STARTING_MEMORY - 1); /*Because we skip first word*/
                image[free_index] = additional_word(distance, ADDRESS_M_RELATIVE, 0);

            }

            if(addr_m_I != ADDRESS_M_REG_DIRECT){free_index++;} /*first operand already takes a word*/

            if (args_number == 2) { /*codes the second arg (if needed)*/
                if (addr_m_II == ADDRESS_M_DIRECT){
                    labelnode node_II = getlabel(*sym_table, arg_II);
                    int is_extern, address;
                    if (node_II == NULL){
                        print_err("declaration of label not exist");
                        return ERR;
                    }

                    address = node_II->val;
                    is_extern = (node_II->type) == EXTERNAL_TYPE;

                    image[free_index] = additional_word(address, ADDRESS_M_DIRECT, is_extern);


                    if (node_II->type == EXTERNAL_TYPE) { /*because in first pass we gave val 0*/
                        if(node_II->val == 0){
                            node_II->val = free_index + STARTING_MEMORY;
                        } else {
                            addlabelnocheck(sym_table, node_II->name, (free_index + STARTING_MEMORY), NONE_LOC, EXTERNAL_TYPE);
                        }
                    }
                }

                if(addr_m_II != ADDRESS_M_REG_DIRECT){free_index++;} /*sec arg takes another word*/
            }
        }
    }

    return free_index;
}
