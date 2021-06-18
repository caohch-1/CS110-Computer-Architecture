/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/utils.h"
#include "src/tables.h"
#include "src/translate_utils.h"
#include "src/translate.h"

#include "assembler.h"

#define MAX_ARGS 3
#define BUF_SIZE 1024
const char *IGNORE_CHARS = " \f\n\r\t\v,()";

/*******************************
 * Helper Functions
 *******************************/

/* you should not be calling this function yourself. */
static void raise_label_error(uint32_t input_line, const char *label) {
    write_to_log("Error - invalid label at line %d: %s\n", input_line, label);
}

/* call this function if more than MAX_ARGS arguments are found while parsing
   arguments.

   INPUT_LINE is which line of the input file that the error occurred in. Note
   that the first line is line 1 and that empty lines are included in the count.

   EXTRA_ARG should contain the first extra argument encountered.
 */
static void raise_extra_argument_error(uint32_t input_line, const char *extra_arg) {
    write_to_log("Error - extra argument at line %d: %s\n", input_line, extra_arg);
}

/* You should call this function if write_pass_one() or translate_inst()
   returns -1.

   INPUT_LINE is which line of the input file that the error occurred in. Note
   that the first line is line 1 and that empty lines are included in the count.
 */
static void raise_instruction_error(uint32_t input_line, const char *name, char **args,
                                    int num_args) {

    write_to_log("Error - invalid instruction at line %d: ", input_line);
    log_inst(name, args, num_args);
}

/* Truncates the string at the first occurrence of the '#' character. */
static void skip_comments(char *str) {
    char *comment_start = strchr(str, '#');
    if (comment_start) {
        *comment_start = '\0';
    }
}

/* Reads STR and determines whether it is a label (ends in ':'), and if so,
   whether it is a valid label, and then tries to add it to the symbol table.

   INPUT_LINE is which line of the input file we are currently processing. Note
   that the first line is line 1 and that empty lines are included in this count.

   BYTE_OFFSET is the offset of the NEXT instruction (should it exist).

   Four scenarios can happen:
    1. STR is not a label (does not end in ':'). Returns 0.
    2. STR ends in ':', but is not a valid label. Returns -1.
    3a. STR ends in ':' and is a valid label. Addition to symbol table fails.
        Returns -1.
    3b. STR ends in ':' and is a valid label. Addition to symbol table succeeds.
        Returns 1.
 */
static int add_if_label(uint32_t input_line, char *str, uint32_t byte_offset,
                        SymbolTable *symtbl) {

    size_t len = strlen(str);
    if (str[len - 1] == ':') {
        /* Codes by Ta*/
        str[len - 1] = '\0';
        if (is_valid_label(str)) {
            /* valid symbol and add successfully*/
            if (add_to_table(symtbl, str, byte_offset) == 0) {
                return 1;
            } else {
                /* valid symbol and add fail*/
                return -1;
            }
        } else {
            /*Invaild label*/
            raise_label_error(input_line, str);
            return -1;
        }
    } else {
        /* Not label*/
        return 0;
    }
}

/*******************************
 * Implement the Following
 *******************************/

/* First pass of the assembler. You should implement pass_two() first.

   This function should read each line, strip all comments, scan for labels,
   and pass instructions to write_pass_one(). The symbol table should also
   been built and written to specified file. The input file may or may not
   be valid. Here are some guidelines:

    1. Only one label may be present per line. It must be the first token present.
        Once you see a label, regardless of whether it is a valid label or invalid
        label, treat the NEXT token as the beginning of an instruction.
    2. If the first token is not a label, treat it as the name of an instruction.
        DO NOT try to check it is a valid instruction in this pass.
    3. Everything after the instruction name should be treated as arguments to
        that instruction. If there are more than MAX_ARGS arguments, call
        raise_extra_argument_error() and pass in the first extra argument. Do not
        write that instruction to the output file (eg. don't call write_pass_one())
    4. Only one instruction should be present per line. You do not need to do
        anything extra to detect this - it should be handled by guideline 3.
    5. A line containing only a label is valid. The address of the label should
        be the byte offset of the next instruction, regardless of whether there
        is a next instruction or not.
    6. If an instruction contains an immediate, you should output it AS IS.
    7. Comments should always be skipped before any further process.

   Just like in pass_two(), if the function encounters an error it should NOT
   exit, but process the entire file and return -1. If no errors were encountered,
   it should return 0.
 */
int pass_one(FILE *input, FILE *inter, FILE *symtbl) {
    /* YOUR CODE HERE */
    int err;
    /* A buffer for line parsing. */
    char line[BUF_SIZE] __attribute__((unused));

    /* Variables for argument parsing. */
    char *args[4] __attribute__((unused));
    int input_line, byte_offset;
    SymbolTable *new_symbol;

    /* Variables for parsing*/
    new_symbol = create_table(SYMBOLTBL_UNIQUE_NAME);
    input_line = 1;
    byte_offset = 0;
    err = 0;


    /* For each line, there are some hints of what you should do:
        1. Skip all comments
        2. Use `strtok()` to read next token
        3. Deal with labels
        4. Parse the instruction
     */

    /* Every Iteration phase one line*/
    while (fgets(line, BUF_SIZE, input) != NULL) {
        char *temp, *name;
        int flag_for_not_label, num_args;
        flag_for_not_label = 0, num_args = 0;
        /* 1: Check if comments
         * if true add input_line then continue*/
        skip_comments(line);
        temp = strtok(line, IGNORE_CHARS);
        if (temp == NULL) { input_line++; continue;}

        /* 2: Check if label
         * if label, let temp forward by strtok, then temp will be name*/
        flag_for_not_label = add_if_label(input_line, temp, 4 * byte_offset, new_symbol);
        if (flag_for_not_label == 1) temp = strtok(NULL, IGNORE_CHARS);
        if (flag_for_not_label == -1) {input_line++; continue;}
        name = temp;


        /* 3: Phase line, if num_args >=4 then meet error*/
        while (temp != NULL && num_args < 4) {
            /* 3.1: Get current parameter until null*/
            temp = strtok(NULL, IGNORE_CHARS);
            if (temp == NULL) break;
            /* 3.2: Store parameter into args*/
            args[num_args] = temp;

            num_args++;
        }

        /* 4: Args Post-Process, flag=-1 means this line has error*/
        if ((num_args <= 3 && num_args != 0) || (num_args == 0 && name != NULL) ) {
            int new_line_num;
            char **new_args = (char **) malloc(num_args * sizeof(char *));
            memcpy(new_args, args, num_args * sizeof(char *));
            /* Now can write*/
            new_line_num = write_pass_one(inter, name, new_args, num_args);
            if (new_line_num == 0) {
                raise_instruction_error(input_line, name, new_args, num_args); err = 1;
            }
            /* Expand to 2 lines need add more byte_offset */
            byte_offset += new_line_num;
            free(new_args);
        } else if (num_args > 3) {
            /* Special for extra args*/
            raise_extra_argument_error(input_line, temp);
            err = 1;
        }
        input_line++;
    }
    /* Write*/
    write_table(new_symbol, symtbl);
    free_table(new_symbol);

    return err;
}

/* Second pass of the assembler.

   This function should read an intermediate file and the corresponding symbol table
   file, translates it into machine code. You may assume:
    1. The input file contains no comments
    2. The input file contains no labels
    3. The input file contains at maximum one instruction per line
    4. All instructions have at maximum MAX_ARGS arguments
    5. The symbol table file is well formatted
    6. The symbol table file contains all the symbol required for translation
   If an error is reached, DO NOT EXIT the function. Keep translating the rest of
   the document, and at the end, return -1. Return 0 if no errors were encountered. */
int pass_two(FILE *inter, FILE *symtbl, FILE *output) {
    /* YOUR CODE HERE */
    SymbolTable *new_symbol;
    char line[BUF_SIZE];
    /* return value, if any error will be 1*/
    int err;
    uint32_t index_line = 1;
    err = 0;

    /* Write text segment. */
    fprintf(output, ".text\n");

    /* 1: Create symbol table here by reading the symbol table file. */
    new_symbol = create_table(SYMBOLTBL_UNIQUE_NAME);
    while (fgets(line, BUF_SIZE, symtbl) != NULL) {
        /* 1.0: Prepare variables*/
        char *name, *addr;
        long int addr_;
        addr_ = 0;
        /* 1.1 : Split into 2 part*/
        addr = strtok(line, IGNORE_CHARS);
        name = strtok(NULL, IGNORE_CHARS);
        /* 1.2: Translate addr into uint32_t*/
        translate_num(&addr_, addr, 0, 4294967296);

        add_to_table(new_symbol, name, addr_);
    }


    /* For each line, there are some hints of what you should do:
        1. Get instruction name.
        2. Parse instruction arguments; Extra arguments should be filtered out in
         `pass_one()`, so you don't need to worry about that here.
        3. Use `translate_inst()` to translate the instruction and write to the
         output file;
        4. Or if an error occurs, call `raise_instruction_error()` instead of write
         the instruction.
     */
    /* 2: Pharse instructions*/
    while (fgets(line, BUF_SIZE, inter) != NULL) {
        /* 2.0: Prepare vaiables*/
        char *temp, *name;
        int i, num_args;
        char **args, **new_args;
        uint32_t addr;

        /* 2.1: Malloc for args, maximum num of args is 3*/
        args = (char **) malloc(3 * sizeof(char *));
        i = 0;

        /* 2.2: Temp used to record arguments, but first is name*/
        temp = strtok(line, IGNORE_CHARS);
        name = temp;

        /* 2.3: Get arguments*/
        while (temp != NULL && i < 3) {
            temp = strtok(NULL, IGNORE_CHARS);
            if (temp == NULL) break;
            /*Copy argument into args*/
            args[i] = temp;
            i++;
        }
        /* 2.4: Set num_args*/
        num_args = i;
        
        /* 2.6: Copy args into new_args for parameter of translate_inst*/
        new_args = (char **) malloc(num_args * sizeof(char *));
        memcpy(new_args, args, num_args * sizeof(char *));

        /* 2.7: Translate instruction*/
        if (num_args == 1) {
            /* 2.7.1: jal*/
            addr = get_addr_for_symbol(new_symbol, new_args[0]);
        } else if (num_args == 2) {
            /* 2.7.2: beqz*/
            addr = get_addr_for_symbol(new_symbol, new_args[1]);
        } else if (num_args == 3) {
            /* 2.7.3: sbtype*/
            addr = get_addr_for_symbol(new_symbol, new_args[2]);
        }
        if (translate_inst(output, name, new_args, num_args, addr, index_line) == -1) {
            raise_instruction_error(index_line, name, new_args, num_args);
            err = 1;
        }

        /* 2.8: Free*/
        free(new_args);
        free(args);
        index_line++;

    }

    /* Write symbol segment. */
    fprintf(output, "\n.symbol\n");
    /* Write symbols here by calling `write_table()` */
    write_table(new_symbol, output);
    free_table(new_symbol);
    return err;
}

/*******************************
 * Do Not Modify Code Below
 *******************************/

static int open_files_pass_one(FILE **input, FILE **inter, FILE **symtbl,
                               const char *input_name, const char *inter_name, const char *symtbl_name) {

    /* Unable to open 1*/
    *input = fopen(input_name, "r");
    if (!*input) {
        write_to_log("Error: unable to open input file: %s\n", input_name);
        return -1;
    }
    /* Unable to open 2*/
    *inter = fopen(inter_name, "w");
    if (!*inter) {
        write_to_log("Error: unable to open intermediate file: %s\n", inter_name);
        fclose(*input); return -1;
    }
    /* Unable to open 3*/
    *symtbl = fopen(symtbl_name, "w");
    if (!*symtbl) {
        write_to_log("Error: unable to open symbol table file: %s\n", symtbl_name);
        fclose(*input); fclose(*inter); return -1;
    }
    /* Normal*/
    return 0;
}

static int open_files_pass_two(FILE **inter, FILE **symtbl, FILE **output,
                               const char *inter_name, const char *symtbl_name, const char *output_name) {
    /* Open inter*/
    *inter = fopen(inter_name, "r");
    if (!*inter) {
        write_to_log("Error: unable to open intermediate file: %s\n", inter_name);
        return -1;
    }

    /* Open symtbl*/
    *symtbl = fopen(symtbl_name, "r");
    if (!*symtbl) {
        write_to_log("Error: unable to open symbol table file: %s\n", inter_name);
        fclose(*inter); return -1;
    }

    /* Open output*/
    *output = fopen(output_name, "w");
    if (!*output) {
        write_to_log("Error: unable to open output file: %s\n", output_name);
        fclose(*inter); fclose(*symtbl); return -1;
    }

    /* return*/
    return 0;
}

/*Close file*/
static void close_files(FILE *file1, FILE *file2, FILE *file3) {
    fclose(file1);
    fclose(file2);
    fclose(file3);
}

/* Runs the two-pass assembler. Most of the actual work is done in pass_one()
   and pass_two().
 */
int assemble(const char *in, const char *out, const char *int_, const char *sym) {
    FILE *input, *inter, *symtbl, *output;
    int err = 0;

    if (in) {
        /* In part*/
        printf("Running pass one: %s -> %s, %s\n", in, int_, sym);
        if (open_files_pass_one(&input, &inter, &symtbl, in, int_, sym) != 0) exit(1);
        if (pass_one(input, inter, symtbl) != 0) err = 1;
        /* close*/
        close_files(input, inter, symtbl);
    }

    if (out) {
        /* Out part*/
        printf("Running pass two: %s, %s -> %s\n", int_, sym, out);
        if (open_files_pass_two(&inter, &symtbl, &output, int_, sym, out) != 0) exit(1);
        if (pass_two(inter, symtbl, output) != 0) err = 1;
        /* close*/
        close_files(inter, symtbl, output);
    }

    return err;
}

static void print_usage_and_exit() {
    /* Test print*/
    printf("Usage:\n");
    printf("  Runs both passes: assembler <input file> <intermediate file> <symbol table file> <output file>\n");
    printf("  Run pass #1:      assembler -p1 <input file> <intermediate file> <symbol table file>\n");
    printf("  Run pass #2:      assembler -p2 <intermediate file> <symbol table> <output file>\n");
    /* Test print 2*/
    printf("Append -log <file name> after any option to save log files to a text file.\n");
    exit(0);
}

int main(int argc, char **argv) {
    /* Prepare variables*/
    int mode = 0;
    char *input, *inter, *output, *symtbl;
    int err;

    /* Print_usage_and_exit*/
    if (argc != 5 && argc != 7) print_usage_and_exit();

    /* Set mode*/
    if (strcmp(argv[1], "-p1") == 0) mode = 1;
    else if (strcmp(argv[1], "-p2") == 0) mode = 2;


    if (mode == 1) {
        /* Mode1*/
        input = argv[2]; inter = argv[3]; symtbl = argv[4]; output = NULL;
    } else if (mode == 2) {
        /* Mode2*/
        input = NULL; inter = argv[2]; symtbl = argv[3]; output = argv[4];
    } else {
        input = argv[1];  inter = argv[2]; symtbl = argv[3]; output = argv[4];
    }

    /* For argc == 7*/
    if (argc == 7) {
        if (strcmp(argv[5], "-log") == 0) set_log_file(argv[6]);
        else print_usage_and_exit();
    }

    /* Begin assemble*/
    err = assemble(input, output, inter, symtbl);

    /* Write error*/
    if (err) write_to_log("One or more errors encountered during assembly operation.\n");
    else write_to_log("Assembly operation completed successfully!\n");

    if (is_log_file_set()) printf("Results saved to %s\n", argv[5]);

    return err;
}
