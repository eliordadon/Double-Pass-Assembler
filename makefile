assembler: assembler.o assembly_tables.o first_pass.o label_table.o memory_word.o output_management.o second_pass.o input_management.o
	gcc -ansi -Wall -pedantic -g assembler.o assembly_tables.o first_pass.o label_table.o memory_word.o output_management.o second_pass.o input_management.o -o assembler

assembler.o: assembler.c input_management.h first_pass.h second_pass.h memory_word.h output_management.h
	gcc -ansi -Wall -pedantic -c assembler.c -o assembler.o

assembly_tables.o: assembly_tables.c assembly_tables.h
	gcc -ansi -Wall -pedantic -c assembly_tables.c -o assembly_tables.o
	
first_pass.o: first_pass.c first_pass.h input_management.h
	gcc -ansi -Wall -pedantic -c first_pass.c -o first_pass.o
	
label_table.o: label_table.c label_table.h
	gcc -ansi -Wall -pedantic -c label_table.c -o label_table.o
	
memory_word.o: memory_word.c memory_word.h
	gcc -ansi -Wall -pedantic -c memory_word.c -o memory_word.o
	
output_management.o: output_management.c output_management.h label_table.h
	gcc -ansi -Wall -pedantic -c output_management.c -o output_management.o
	
second_pass.o: second_pass.c second_pass.h label_table.h input_management.h
	gcc -ansi -Wall -pedantic -c second_pass.c -o second_pass.o
	
input_management.o: input_management.c input_management.h assembly_tables.h
	gcc -ansi -Wall -pedantic -c input_management.c -o input_management.o
