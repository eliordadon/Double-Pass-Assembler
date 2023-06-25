#include "input_management.h"

/*returns how many times a given char is appearing in a given string*/
int how_many_times(char *string, char c){
    int count = 0;
    for (; string && *string; string++){
        if ((*string) == c) {count++;}
    }
    return count;
}

/*returns TRUE if the string only containing letters and digits, FALSE otherwise*/
int only_letters_digits(char *string){
    for (; *string != '\0'; string++){
        if (!isdigit(*string) && !isalpha(*string)) { return FALSE;}
    }
    return TRUE;
}

/*gets an argument (first or second one depends on line_after_op), returns TRUE if argument is exist and put it in store_in, return ERR otherwise*/
int get_argument(char *line_after_op, char *store_in) {
    char *argument;
    char *templine; /*because we don't want strtok to make changes on the source line*/

    if (!(templine = (char *) malloc(MAX_LINE_LEN))){
        print_err("Memory Allocation Failed");
        return ERR;
    }

    if (!line_after_op){
        print_err("not enough arguments");
        return ERR;
    }

    if (line_after_op[0] == ','){
        print_err("comma before argument is not allowed");
        return ERR;
    }

    strcpy(templine, line_after_op);

    if(line_after_op[0] == '&'){
        line_after_op++;
    }



    if(templine){
	argument = remove_space(strtok(templine, ",")); /*first argument without start/end spaces*/
    }

    if (argument == NULL){
        print_err("argument of operation is missing");
        return ERR;
    }

    if (holds_space(argument)){
        print_err("argument cannot store whitespaces inside it (not starting / end spaces)");
        return ERR;
    }

    if (strlen(argument) > MAX_LABEL_LEN){
        print_err("argument length is exceeds the maximum length of a valid argument");
        return ERR;
    }

    strcpy(store_in, argument); /*the argument is valid*/

    free(templine);

    return TRUE;
}

/*gets a line and check whether it contains an operation or instruction. returns OPER if operation, INST if instruction. also puts the op/inst name in store_in.
 * return ERR if there are any errors.*/
int get_inst_or_op(char *line, char *store_in){
    char *templine;
    char first[MAX_INSTRCUT_N_OPER] = {0};
    int first_len = 0;

    for(templine = line; *templine != '\0' && !isspace(*templine); templine++) {first_len++;}

    if (first_len > MAX_INSTRCUT_N_OPER){
        print_err("instruction or operation length is exceeds the maximum length of an instruction or operation");
        return ERR;
    }

    sscanf(line, "%s", first);

    if (is_who(first) == INSTRUCT){
        strcpy(store_in, first);
        return INST;
    }
    if (is_who(first) == OPER_0_ARG || is_who(first) == OPER_1_ARG || is_who(first) == OPER_2_ARG){
        strcpy(store_in, first);
        return OPER;
    }

    /*not instruction / operation - unknown*/
    print_err("unknown instruction or operation");
    return ERR;
}

/*gets a line and if the line contains a LABEL and the LABEL is valid, it puts the LABEL in store_label_in and return a pointer to the rest of the line.
 * if the LABEL is invalid return ERR, return the line as it is if there is no LABEL*/
char *holds_label(char *line, char *store_label_in){
    char *rest_of_line, *label;

    int how_many_colon = how_many_times(line, ':');

    if (how_many_colon < 1) {return line;} /*no label is provided*/

    if (how_many_colon > 1){
        print_err("two or more labels declared, only one declaration of label is allowed in one line");
        return (char *) ERR;
    }

    label = strtok(line, ":");/*only one label is provided, valid*/

    if (isspace(label[strlen(label) - 1])) { /*e.g Hello :*/
        print_err("whitespaces between the label and : are not allowed");
        return (char *) ERR;
    }

    label = remove_space(label);

    if (label[0] == '\0'){
        print_err("there is no label before the ':' character");
        return (char *) ERR;
    }

    if (is_saved_word(label)){
        print_err("label cannot be a saved word, e.g register / instruction / operation name");
        return (char *) ERR;
    }

    if (!isalpha(label[0])){
        print_err("first letter in label is a non-alphabetic character");
        return (char *) ERR;
    }

    if (strlen(label) > MAX_LABEL_LEN - 1) { /*excluding '\0'*/
        print_err("label length is exceeds the maximum length of a valid label, which is 31 letters");
        return (char *) ERR;
    }

    if (!only_letters_digits(label)){
        print_err("non-alphabetic or non-digit or a whitespace inside are not allowed");
        return (char *) ERR;
    }

    strcpy(store_label_in, label); /*label is ok*/

    rest_of_line = remove_space(strtok(NULL, ":")); /*rest of the line, without after ':'*/

    return rest_of_line;
}

/*gets a string that contains an inner string and return the inner string if is valid one, ERR otherwise*/
char *get_string(char *string){
    char *inner_str;
    int inner_len, quotes_num, i;

    if (!string) {return (char *) ERR;}

    quotes_num = how_many_times(string, '"');

    if (quotes_num < MAX_QUOTES || string[0] != '"' || string[strlen(string) - 1] != '"'){
        print_err("Illegal string, missing quotes");
        return (char *) ERR;
    }

    if (quotes_num > MAX_QUOTES){
        print_err("Illegal string, exactly two quotes required");
        return (char *) ERR;
    }

    for (i = 0; i < strlen(string); i++){ /*valid string, replace any '\t' back to ':' (because of scan_line)*/
        if (string[i] == STRING_COLON) {string[i] = ':';}
    }

    inner_len = strlen(string) - MAX_QUOTES; /*without quotes*/

    if(!(inner_str = (char *) malloc(inner_len + 1))) { /*including '\0'*/
        print_err("Memory Allocation Failed");
        return (char *) ERR;
    }

    strncpy(inner_str, string + 1, inner_len); /*skip first quote*/
    inner_str[inner_len + 1] = '\0';

    return inner_str;
}

/*scans a line from a file and store the line in store_in as a string, removes all beginning / end spaces from the line,
 * also switch any ':' chars in a string to '\t', return TRUE if succeed , and EOF on end of file */
int scan_line(char *store_in, FILE * fd){
    char *templine, *line_ptr, *colon_ptr;

    if (!(templine = (char *) malloc(MAX_LINE_LEN))) { print_err("Memory Allocation Failed"); }

    if ((line_ptr = fgets(templine, MAX_LINE_LEN, fd)) == NULL) { return EOF; }

    line_ptr = remove_space(line_ptr);
    colon_ptr = line_ptr;

    while ((colon_ptr = strstr(colon_ptr, ":")) != NULL) { /*switching any ':' in string to '\t'*/
        int how_many_before, how_many_after, index;
        index = colon_ptr - line_ptr; /*index of ':'*/
        line_ptr[index] = '\0'; /*because we need to calculate how_many_before*/
        how_many_before = how_many_times(line_ptr, '"');
        line_ptr[index] = ':';
        how_many_after = how_many_times(colon_ptr + 1, '"');
        if (how_many_before % MAX_QUOTES != 0 && how_many_after % MAX_QUOTES != 0){
            line_ptr[index] = STRING_COLON; /*we in a string, switch ':' to '\t'*/
        }
        colon_ptr++;
    }
    strcpy(store_in, line_ptr);
    free(templine);
    return TRUE;
}

/*removes all beginning / end spaces from the string and return a pointer to the new string*/
char *remove_space (char *string){
    int index, flag;

    if (!string || !(*string) || (*string) == EOF) { return NULL;}

    for (; isspace(*string); string++); /*skipping the starting spaces*/

    for (index = strlen(string) - 1, flag = TRUE; index >= 0 && flag; --index){
        char keeper = string[index];
        (isspace(keeper)) ? (string[index] = '\0') : (flag = FALSE); /*remove all ending spaces*/
    }
    return string;
}

/*gets a string and check whether it contain any spaces , return TRUE if it does, FALSE otherwise.*/
int holds_space(char *string){
    for (; *string; string++){
        if (isspace(*string))
            return TRUE;
    }
    return FALSE;
}

/*gets an operation and the source/destination addressing methods and return whether the methods are valid for the operation or not.
 * return TRUE if valid, FALSE otherwise*/
int valid_addressing (char *operation, int src_addr_method, int dest_addr_method){
    int oper_opcode = getOpcode(operation);

    if(is_who(operation) == OPER_1_ARG){ /*no source argument only destination*/
        if (oper_opcode == CLR_NOT_INC_DEC_OPC){
            if (dest_addr_method == ADDRESS_M_DIRECT || dest_addr_method == ADDRESS_M_REG_DIRECT) {return TRUE;}
        }
        else if (oper_opcode == JMP_BNE_JSR_OPC){
            if (dest_addr_method == ADDRESS_M_DIRECT || dest_addr_method == ADDRESS_M_RELATIVE) {return TRUE;}
        }
        else if(oper_opcode == RED_OPC){
            if (dest_addr_method == ADDRESS_M_DIRECT || dest_addr_method == ADDRESS_M_REG_DIRECT) {return TRUE;}
        }
        else if (oper_opcode == PRN_OPC){
            if (dest_addr_method == ADDRESS_IMMEDIATE|| dest_addr_method == ADDRESS_M_DIRECT || dest_addr_method == ADDRESS_M_REG_DIRECT) {return TRUE;}
        }

        print_err("addressing method isn't valid");
        return FALSE;
    }
    else{ /*takes two arguments*/
        if ((src_addr_method != ADDRESS_IMMEDIATE) && (src_addr_method != ADDRESS_M_DIRECT) && (src_addr_method != ADDRESS_M_REG_DIRECT)){
            print_err("Invalid addressing method for source argument");
            return FALSE;
        }
        if ((!strcmp(operation, LEA_O)) && (src_addr_method != ADDRESS_M_DIRECT)) {
            print_err("Invalid addressing method for source argument");
            return FALSE;
        }

        if ((dest_addr_method != ADDRESS_IMMEDIATE) && (dest_addr_method != ADDRESS_M_DIRECT) && (dest_addr_method != ADDRESS_M_REG_DIRECT)){
            print_err("Invalid addressing method for destination argument");
            return FALSE;
        }

        if ((!strcmp(operation, MOV_O) || !strcmp(operation, ADD_O) || !strcmp(operation, SUB_O) || !strcmp(operation, LEA_O)) &&
                (dest_addr_method != ADDRESS_M_DIRECT) && (dest_addr_method != ADDRESS_M_REG_DIRECT)){
            print_err("Invalid addressing method for destination argument");
            return FALSE;
        }

        return TRUE;
    }
}

/*returns whether the string is a valid number or not, TRUE if valid, FALSE otherwise.*/
int valid_num (char *string){
    if (!string || string[0] == '\0'){
        print_err("no number provided");
        return FALSE;
    }

    if (string[0] == '-' || string[0] == '+') { string++; } /*skipping - or +*/

    if (!(*string)){ /*it's - or + without any number*/
        print_err("no number provided after '-' or '+'");
        return FALSE;
    }

    for (; *string; string++){
        if (!isdigit(*string)){
            print_err("invalid number - contain non-digit character");
            return FALSE;
        }
    }
    return TRUE;
}

/*gets an operation as a string and return how many arguments it takes, assuming operation is valid*/
int how_many_args(char *operation){
    return (is_who(operation) == OPER_0_ARG) ? 0 : (is_who(operation) == OPER_1_ARG) ? 1 : 2;
}
