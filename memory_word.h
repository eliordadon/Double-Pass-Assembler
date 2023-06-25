
#ifndef PROJECT_ASSEMBLER_MEMORY_WORD_H
#define PROJECT_ASSEMBLER_MEMORY_WORD_H


/*typedef struct{
    unsigned int ARE: 3;
    unsigned int funct: 5;
    unsigned int dest_reg: 3;
    unsigned int dest_addressing: 2;
    unsigned int source_reg: 3;
    unsigned int source_addressing: 2;
    unsigned int opcode: 6;
}firWord;*/

typedef long MemWord;

enum field_ARE {ABSOLUTE = 4, RELOCATABLE = 2, EXTERNAL = 1 }; /* 100, 010, 001 */

#define STARTING_MEMORY 100
#define MAX_DATA 9999
#define MAX_INSTRUCT 9999


MemWord fir_word(int opcode, int sour_method, int sour_reg, int dest_method, int dest_reg, int funct);
MemWord additional_word(long val, int which_addressing, int is_extern);


#endif
