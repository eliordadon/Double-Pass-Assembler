# Double-pass 24-bit Assembler
This project is an assignment in a C programming course at the Open University (20465). The purpose of this project is to build an assembler for a 15-instruction assembly language, designed for an imaginary 24-bit CPU with 21 bits of memory size.  
  
## License  
  
The project is open-source and licensed under the MIT License.  
  
## Technologies Used  
  
The project utilizes the C programming language and was developed using the CLion IDE. Memory leaks and other issues were managed and checked using Valgrind on an Ubuntu operating system.  
  
## Structure  
  
-  ```assembler```: Main entry point to the assembler program. Takes command-line arguments representing file names, performs two passes over each file, and creates the final output files.  
- ```assembly_tables```: Manages the assembly language syntax by defining register, instruction, and operation tables, and provides functionality to interpret and validate assembly code.  
- ```first_pass```: Implements the first pass of an assembler, parsing and validating assembly code line by line. Deciphers instruction and operation lines, manages symbols and labels, and fills memory images with appropriate data or instructions.  
- ```input_management```: Contains functions to parse, validate and manage inputs for an assembler program. Handles reading lines, extracting and validating labels, operations, and arguments from these lines.  
- ```label_table```: Provides functions for managing a linked list of labels used in an assembler program. Includes functionalities for adding labels, checking the existence of labels in the table, retrieving label nodes, and freeing the entire label table.  
- ```memory_word```: Contains functions for generating machine code words in an assembly-like language. Includes functionality for creating the first operation word and additional words based on different addressing methods and other instructions parameters.  
- ```output_management```: Handles the creation of the output files based on the data gathered during the first and second passes of the assembly process. Translates numbers to hexadecimal representation, and writes instructions, data, and symbol table entries to object, entry, and external files.  
- ```second_pass```: Performs the second pass of the assembly process on a given file. Deals with operations (not handled in the first pass), resolves label references, validates and handles .entry declarations, and populates instruction images based on the operation arguments and their addressing modes.   
  
## Usage  
  
To use the assembler, run: ```./assembler file1 file2 file3 ...``` The assembler will process each file specified (without the extension). Note: Double-quoting file names is not supported.  
  
## Build Instructions  
  
You can build this project in few ways:  
  
1. Using CMake: Run ```cmake --build .``` (you might have to run ```cmake --configure .``` first) - This method provides support for the CLion IDE.  
2. Using ```make``` command (makefile)  
3. Manually (Not recommended): Compile each ```*.c``` file to ```*.o``` and then compile all to a single executable using ```CC```/```GCC```/```CLang``` with ```-g a.o b.o .... -o exe```.  
  
## Other Information  
  
For your convinience there's examples, ```kelet.``` and ```myvalid.``` which already been run to check the program
