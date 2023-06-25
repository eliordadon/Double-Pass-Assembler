#include "memory_word.h"
#include "assembly_tables.h"

/*void first_word (int opcode, int sour_method, int sour_reg, int dest_method, int dest_reg, int funct, MemWord *word){

    word->first->ARE = ABSOLUTE;
    word->first->funct = funct;
    word->first->dest_reg = dest_reg;
    word->first->dest_addressing = dest_method;
    word->first->source_reg = sour_reg;
    word->first->source_addressing = sour_method;
    word->first->opcode = opcode;
}*/

/*gets as arguments a pointer to MemWord and the details needed for creating the first word, creates the first word of an operation*/
MemWord fir_word(int opcode, int sour_method, int sour_reg, int dest_method, int dest_reg, int funct){
    MemWord word;
    word = 0;
    word |= ABSOLUTE; /*all first words ABSOLUTE, bits 0-2*/
    word |= (funct << 3); /*bits 3-7*/
    word |= (dest_reg << 8); /*bits 8-10*/
    word |= (dest_method << 11); /*bits 11-12*/
    word |= (sour_reg << 13); /*bits 13-15*/
    word |= (sour_method << 16); /*bits 16-17*/
    word |= (opcode << 18); /*bits 18-23*/

    return word;

}

/*creates an additional word with space for field ARE, field ARE is set based on the addressing method*/
MemWord additional_word(long val, int which_addressing, int is_extern){ /*val can be int value, char (ascii value),
 *                                                                                  address value, or a distance value. */
    MemWord word = 0; /*-------MADE A CHANGE HERE ALL EXTERN ADDRESS'S ARE 1 (0 | EXTERNAL)---------- */
    if (is_extern) {
        word |= EXTERNAL;
        return word;
    }
    word = (val << 3); /*bits 3-23 (saving space for field ARE*/
    word |= which_addressing == ADDRESS_M_DIRECT ? RELOCATABLE : ABSOLUTE;
    return word;
}

