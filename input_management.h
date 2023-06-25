#ifndef FINALPROJECT_INPUT_MANAGEMENT_H
#define FINALPROJECT_INPUT_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "assembly_tables.h"

extern char *operations_0[], *operations_1[], *operations_2[], *instructs[], *registers[];

extern int linenum;

/*type*/
#define OPER 5
#define INST 6

#define MAX_QUOTES 2
#define STRING_COLON '\t'

/*macros*/
#define print_err(string) printf("Error, line %d: " string ".\n", linenum);

/*return TRUE if the string holds chars other then spaces, and FALSE if the string is holding spaces only*/
#define holds_ex_text(string) (!(string) || !(remove_space(string))) ? FALSE : TRUE

/*return FALSE if the string not containing two commas (",,"), TRUE if it does*/
#define holds_dou_commas(string) (!(strstr(string, ",,"))) ? FALSE : TRUE


/*function prototype*/

/*gets an argument (first or second one depends on line_after_op), returns TRUE if argument is exist and put it in store_in, return ERR otherwise*/
int get_argument(char *line_after_op, char *store_in);

/*gets a line and check whether it contains an operation or instruction. returns OPER if operation, INST if instruction. also puts the op/inst name in store_in.
 * return ERR if there are any errors.*/
int get_inst_or_op(char *line, char *store_in);

/*gets a line and if the line contains a LABEL and the LABEL is valid, it puts the LABEL in store_label_in and return a pointer to the rest of the line.
 * if the LABEL is invalid return ERR, return the line as it is if there is no LABEL*/
char *holds_label(char *line, char *store_label_in);

/*gets a string that contains an inner string and return the inner string if is valid one, ERR otherwise*/
char *get_string(char *string);

/*scans a line from a file and store the line in store_in as a string, removes all beginning / end spaces from the line,
 * also switch any ':' chars in a string to '\t', return TRUE if succeed , and EOF on end of file */
int scan_line(char *store_in, FILE * fd);

/*removes all beginning / end spaces from the string and return a pointer to the new string*/
char *remove_space (char *string);

/*gets a string and check whether it contain any spaces , return TRUE if it does, FALSE otherwise.*/
int holds_space(char *string);

/*gets an operation and the source/destination addressing methods and return whether the methods are valid for the operation or not.
 * return TRUE if valid, FALSE otherwise*/
int valid_addressing (char *operation, int src_add_method, int dest_add_method);

/*returns whether the string is a valid number or not, TRUE if valid, FALSE otherwise.*/
int valid_num (char *num);

/*gets an operation as a string and return how many arguments it takes, assuming operation is valid*/
int how_many_args(char *operation);


#endif
