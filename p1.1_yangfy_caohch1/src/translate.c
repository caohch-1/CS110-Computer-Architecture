/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tables.h"
#include "translate_utils.h"
#include "translate.h"


/* Writes instructions during the assembler's first pass to OUTPUT. The case
   for general instructions has already been completed, but you need to write
   code to translate the li, bge and mv pseudoinstructions. Your pseudoinstruction
   expansions should not have any side effects.

   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS.

   Error checking for regular instructions are done in pass two. However, for
   pseudoinstructions, you must make sure that ARGS contains the correct number
   of arguments. You do NOT need to check whether the registers / label are
   valid, since that will be checked in part two.

   Also for li:
    - make sure that the number is representable by 32 bits. (Hint: the number
        can be both signed or unsigned).
    - if the immediate can fit in the imm field of an addiu instruction, then
        expand li into a single addi instruction. Otherwise, expand it into
        a lui-addi pair.

   And for bge and move:
    - your expansion should use the fewest number of instructions possible.

   venus has slightly different translation rules for li, and it allows numbers
   larger than the largest 32 bit number to be loaded with li. You should follow
   the above rules if venus behaves differently.

   Use fprintf() to write. If writing multiple instructions, make sure that
   each instruction is on a different line.

   Returns the number of instructions written (so 0 if there were any errors).
 */
unsigned write_pass_one(FILE *output, const char *name, char **args, int num_args) {
    if (!output || !name || !args) {
        return 0;
    }

    /* Deal with pseudo-instructions. */
    if (strcmp(name, "li") == 0) {
        /* YOUR CODE HERE */
        long int imm;
        int imm_ret = -1;
        /* 0: Check wrong input*/
        /* 0.1: Number of arguments should be 2*/
        if (num_args != 2) return 0;
        /* 0.2: Args should not be Null*/
        if (args == NULL || args[0] == NULL || args[1] == NULL) return 0;
        /* 0.3: Argument2 should be 32-bits immediate*/
        if (args[1][0] == '0' && (args[1][1] == 'x' || args[1][1] == 'X')) {
            imm_ret = translate_num(&imm, args[1], -2147483648, 4294967296);
        } else {
            imm_ret = translate_num(&imm, args[1], -2147483648, 2147483647);
        }
        /* Error process*/
        if (imm_ret == -1) return 0;

        /* Translate range should be different*/
        if (args[1][0] == '0' && (args[1][1] == 'x' || args[1][1] == 'X') && imm >= 0 && imm <= 4095) {
            imm_ret = 0;
        } else if (imm >= -2048 && imm <= 2047) {
            imm_ret = 0; /* Then out of range*/
        } else {
            imm_ret = -1;
        }

        /* 1: Translation*/
        /* 1.1: Single addi*/
        if (imm_ret == 0) {
            /* 1.1.0: Malloc for new arguments*/
            char **new_args = (char **) malloc(3 * sizeof(char *));
            /* 1.1.1: Simply copy first argument*/
            new_args[0] = (char *) malloc((strlen(args[0]) + 1) * sizeof(char));
            memcpy(new_args[0], args[0], strlen(args[0]) + 1);
            /* 1.1.2: Change imm into str and assign this str into 2nd new_args*/
            new_args[2] = args[1];
            /* 1.1.3: Second register should be x0*/
            new_args[1] = "x0";
            /* 1.1.3: Write into file*/
            write_inst_string(output, "addi", new_args, 3);
            /* 1.1.4: Free*/
            free(new_args[0]);
            free(new_args);
            return 1;
        }
        /* 1.2: lui-addi*/
        /* Translate range should be different*/
        if (args[1][0] == '0' && (args[1][1] == 'x' || args[1][1] == 'X')) {
            imm_ret = translate_num(&imm, args[1], -2147483648, 4294967296);
        } else {
            imm_ret = translate_num(&imm, args[1], -2147483648, 2147483647);
        }
        /* Start process*/

        if (imm_ret == -1) return 0;

        if (imm_ret == 0) {
            int upper, low;
            /* 1.2.0: Malloc for new arguments*/
            char **lui_args = (char **) malloc(2 * sizeof(char *));
            char **addi_args = (char **) malloc(3 * sizeof(char *));

            /* 1.2.1: Simply Set 1st argument(registers) of lui_args, same as 1st and 2nd arguments of addi_args*/
            lui_args[0] = (char *) malloc((strlen(args[0]) + 1) * sizeof(char));
            memcpy(lui_args[0], args[0], strlen(args[0]) + 1);
            /* For addi_args*/
            addi_args[0] = (char *) malloc((strlen(args[0]) + 1) * sizeof(char));
            memcpy(addi_args[0], args[0], strlen(args[0]) + 1);

            addi_args[1] = (char *) malloc((strlen(args[0]) + 1) * sizeof(char));
            memcpy(addi_args[1], args[0], strlen(args[0]) + 1);

            /* 1.2.2: Split imm into 5:3 hex and assign to 2nd argument of lui, same as 3rd argument of addi_args*/
            /* 1.2.2.1: Malloc*/
            lui_args[1] = (char *) malloc(10 * sizeof(char));
            addi_args[2] = (char *) malloc(10 * sizeof(char));
            /* 1.2.2.2: Upper needs imm first 5 bytes, so right shift 3 bytes*/
            upper = (imm & 0xfffff000) >> 12;
            /* 1.2.2.3: Low needs imm last 3 bytes, so "and" 0x00000fff*/
            low = imm & 0xfff;
            if (low >= 2048) {
                low |= 0xfffff000;
                upper ++;
            }
            /* 1.2.2.4: Assign each*/
            sprintf(lui_args[1], "%d", upper);
            sprintf(addi_args[2], "%d", low);

            /* 1.2.4: Write*/
            write_inst_string(output, "lui", lui_args, 2);
            write_inst_string(output, "addi", addi_args, 3);

            /* 1.1.4: Free*/
            free(lui_args[0]); free(lui_args[1]);
            free(addi_args[0]); free(addi_args[1]); free(addi_args[2]);
            free(lui_args);
            free(addi_args);

            /* 1.1.5: Return*/
            return 2;
        }

    }
    else if (strcmp(name, "beqz") == 0) {
        /* YOUR CODE HERE */
        char **new_args;
        /* 0: Check wrong input*/
        /* 0.1: Number of arguments should be 2*/
        if (num_args != 2) return 0;
        /* 0.2: Args should not be Null*/
        if (args == NULL || args[0] == NULL || args[1] == NULL) return 0;

        /* 1: Write*/
        new_args = (char **) malloc(3 * sizeof(char *));
        new_args[0] = args[0]; new_args[1] = "x0"; new_args[2] = args[1];
        write_inst_string(output, "beq", new_args, 3);
        /* 2: Free*/
        free(new_args);

        return 1;
    }
    else if (strcmp(name, "mv") == 0) {
        /* YOUR CODE HERE */
        char **new_args;
        /* 0: Check wrong input*/
        /* 0.1: Number of arguments should be 2*/
        if (num_args != 2) return 0;
        /* 0.2: Args should not be Null*/
        if (args == NULL || args[0] == NULL || args[1] == NULL) return 0;

        /* 1: Write*/
        new_args = (char **) malloc(3 * sizeof(char *));
        new_args[0] = args[0]; new_args[1] = args[1]; new_args[2] = "0";
        write_inst_string(output, "addi", new_args, 3);
        /* 2: Free*/
        free(new_args);

        return 1;
    }
    else if (strcmp(name, "j") == 0) {
        /* 0: Check wrong input*/
        char **new_args;
        /* 0.1: Number of arguments should be 2*/
        if (num_args != 1) return 0;
        /* 0.2: Args should not be Null*/
        if (args == NULL || args[0] == NULL) return 0;

        /* 1: Write*/
        new_args = (char **) malloc(2 * sizeof(char *));
        new_args[0] = "zero";
        new_args[1] = args[0];
        write_inst_string(output, "jal", new_args, 2);
        /* 2: Free*/
        free(new_args);
        return 1;
    }
    else if (strcmp(name, "jr") == 0) {
        /* 0: Check wrong input*/
        char **new_args;
        /* 0.1: Number of arguments should be 2*/
        if (num_args != 1) return 0;
        /* 0.2: Args should not be Null*/
        if (args == NULL || args[0] == NULL) return 0;

        /* 1: Write*/
        new_args = (char **) malloc(3 * sizeof(char *));
        new_args[0] = "zero"; new_args[1] = args[0]; new_args[2] = "0";
        write_inst_string(output, "jalr", new_args, 3);

        /* 2: Free*/
        free(new_args);
        return 1;
    }
    else if (strcmp(name, "jal") == 0) {

        /* 0: Check wrong input*/
        char **new_args;
        /* 0.1: Number of arguments should be 2*/
        if (num_args != 1) return 0;
        /* 0.2: Args should not be Null*/
        if (args == NULL || args[0] == NULL) return 0;

        /* 1: Write*/
        new_args = (char **) malloc(2 * sizeof(char *));
        new_args[0] = "ra";
        new_args[1] = args[0];
        write_inst_string(output, "jal", new_args, 2);
        /* 2: Free*/
        free(new_args);
        return 1;
    }
    else {
        /* Normal instruction*/
        write_inst_string(output, name, args, num_args);
        return 1;
    }
    return 0;
}


/* Writes the instruction in hexadecimal format to OUTPUT during pass #2.

   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS.

   The symbol table (SYMTBL) is given for any symbols that need to be resolved
   at this step.

   You must perform error checking on all instructions and make sure that their
   arguments are valid. If an instruction is invalid, you should not write
   anything to OUTPUT but simply return -1. venus may be a useful resource for
   this step.

   Note the use of helper functions. Consider writing your own! If the function
   definition comes afterwards, you must declare it first (see translate.h).

   Returns 0 on success and -1 on error.
 */
int translate_inst(FILE *output, const char *name, char **args, size_t num_args, uint32_t addr, uint32_t current_addr) {
    /* R-type*/
    if (strcmp(name, "add") == 0) return write_rtype(0x33, 0x00, 0x00, output, args, num_args);
    else if (strcmp(name, "or") == 0) return write_rtype(0x33, 0x06, 0x00, output, args, num_args);
    else if (strcmp(name, "sll") == 0) return write_rtype(0x33, 0x01, 0x00, output, args, num_args);
    else if (strcmp(name, "slt") == 0) return write_rtype(0x33, 0x02, 0x00, output, args, num_args);
    /* R-type*/
    else if (strcmp(name, "sltu") == 0) return write_rtype(0x33, 0x03, 0x00, output, args, num_args);
    /* I-type*/
    else if (strcmp(name, "lb") == 0) return write_itype(0x03, 0x00, output, args, num_args);
    else if (strcmp(name, "lw") == 0) return write_itype(0x03, 0x02, output, args, num_args);
    else if (strcmp(name, "lbu") == 0) return write_itype(0x03, 0x04, output, args, num_args);
    else if (strcmp(name, "addi") == 0) return write_itype(0x13, 0x00, output, args, num_args);
    /* I-type*/
    else if (strcmp(name, "ori") == 0) return write_itype(0x13, 0x06, output, args, num_args);
    else if (strcmp(name, "jalr") == 0) return write_itype(0x67, 0x00, output, args, num_args);
    /* S-type*/
    else if (strcmp(name, "sb") == 0) return write_stype(0x23, 0x00, output, args, num_args);
    else if (strcmp(name, "sw") == 0) return write_stype(0x23, 0x02, output, args, num_args);
    /* U-type*/
    else if (strcmp(name, "lui") == 0) return write_utype(0x37, output, args, num_args);
    /* SB-type*/
    else if (strcmp(name, "beq") == 0) return write_sbtype(0x63, 0x00, output, args, num_args, addr, current_addr);
    else if (strcmp(name, "bne") == 0) return write_sbtype(0x63, 0x01, output, args, num_args, addr, current_addr);
    else if (strcmp(name, "blt") == 0) return write_sbtype(0x63, 0x04, output, args, num_args, addr, current_addr);
    else if (strcmp(name, "bge") == 0) return write_sbtype(0x63, 0x05, output, args, num_args, addr, current_addr);
    /* UJ-type*/
    else if (strcmp(name, "jal") == 0) return write_ujtype(0x6f, output, args, num_args, addr, current_addr);
    /* Error*/
    else return -1;
}

/* A helper function for writing most R-type instructions. You should use
   translate_reg() to parse registers and write_inst_hex() to write to
   OUTPUT. Both are defined in translate_utils.h.

   This function is INCOMPLETE. Complete the implementation below. You will
   find bitwise operations to be the cleanest way to complete this function.
 */
int write_rtype(uint8_t opcode, uint8_t funct3, uint8_t funct7, FILE *output, char **args, size_t num_args) {
    /*  Perhaps perform some error checking? */
    int rd, rs, rt;
    uint32_t instruction = 0;
    /* 0: Check wrong input*/
    /* 0.1: Number of arguments should be 2*/
    if (num_args != 3) return -1;
    /* 0.2: Args should not be Null*/
    if (args == NULL || args[0] == NULL || args[1] == NULL || args[2] == NULL) return -1;

    /* 1: Write instruction*/
    /* 1.1: Get 3 register int*/
    rd = translate_reg(args[0]);
    rs = translate_reg(args[1]);
    rt = translate_reg(args[2]);

    /* YOUR CODE HERE */
    /* 1.2: Error checking for register ids */
    if (rd == -1 || rs == -1 || rt == -1) return -1;
    /* 1.3: Generate instruction */
    instruction = instruction | (opcode);
    instruction = instruction | (rd << 7);
    instruction = instruction | (funct3 << 12);
    instruction = instruction | (rs << 15);
    /* 1.3: Generate instruction */
    instruction = instruction | (rt << 20);
    instruction = instruction | (funct7 << 25);

    /* 1.4: Write instruction*/
    write_inst_hex(output, instruction);
    return 0;
}

int write_itype(uint8_t opcode, uint8_t funct3, FILE *output, char **args, size_t num_args) {
    /* YOUR CODE HERE */
    /* for instruction with offset, like 'lb', we should first split second argument
     * same as write_stype 0.3.1-0.3.3*/
    long int imm;
    int rd, rs1, imm_ret = -1;
    uint32_t instruction = 0x0;
    /* 0: Check wrong input*/
    /* 0.1: Number of arguments should be 3 or 2*/
    if (num_args != 3) return -1;

    /* Detect opcode, if lb lw lbu, change args[1] and args[2]*/
    if (opcode == 3) {
        char* temp = args[2];
        args[2] = args[1];
        args[1] = temp;
    }

    /* 0.2: Args should not be Null*/
    if (args == NULL || args[0] == NULL || args[1] == NULL || args[2] == NULL) {
        return -1;
    }
    /* 0.3: Argument3 should be 12-bits immediate*/
    if (args[2][0] == '0' && (args[2][1] == 'x' || args[2][1] == 'X')) imm_ret = translate_num(&imm, args[2], -2048, 4095);
    else imm_ret = translate_num(&imm, args[2], -2048, 2047);

    if (imm_ret == -1) {
        return -1;
    }

    /* 1: Create instruction*/
    /* 1.1: Get 2 register int*/
    rd = translate_reg(args[0]);
    rs1 = translate_reg(args[1]);
    /* 1.2: Error checking for register ids */
    if (rd == -1 || rs1 == -1){return -1; }
    /* 1.3: Generate instruction */
    instruction = instruction | (opcode);
    instruction = instruction | (rd << 7);
    instruction = instruction | (funct3 << 12);
    instruction = instruction | (rs1 << 15);
    /* 1.3: Generate instruction */
    instruction = instruction | (imm << 20);

    write_inst_hex(output, instruction);
    return 0;
}

int write_stype(uint8_t opcode, uint8_t funct3, FILE *output, char **args, size_t num_args) {
    /* YOUR CODE HERE */
    /* Prepare variables*/
    long int imm;
    int rs1, rs2, imm_ret = -1;
    /* Prepare variables*/
    uint32_t instruction = 0x0;
    /* 0: Check wrong input*/
    /* 0.1: Number of arguments should be 2*/
    if (num_args != 3) return -1;
    /* 0.2: Args should not be Null*/
    if (args == NULL || args[0] == NULL || args[1] == NULL || args[2] == NULL) return -1;

    /* 0.3.3: Translate imm_str into long int imm and check range*/

    if (args[1][0] == '0' && (args[1][1] == 'x' || args[1][1] == 'X')) imm_ret = translate_num(&imm, args[1], -2048, 4095);
    else imm_ret = translate_num(&imm, args[1], -2048, 2047);
    if (imm_ret == -1) return -1;


    /* 1: Create instruction*/
    /* 1.1: Get 2 register int*/
    rs1 = translate_reg(args[2]);
    rs2 = translate_reg(args[0]);
    /* 1.2: Error checking for register ids */
    if (rs1 == -1 || rs2 == -1) return -1;
    /* 1.3: Generate instruction */
    instruction = instruction | (opcode);
    instruction = instruction | ((imm & 0x01f) << 7);
    instruction = instruction | (funct3 << 12);
    instruction = instruction | (rs1 << 15);
    /* 1.3: Generate instruction */
    instruction = instruction | (rs2 << 20);
    instruction = instruction | ((imm >> 5) << 25);

    write_inst_hex(output, instruction);
    return 0;
}

/* Hint: the way for branch to calculate relative address. e.g. bne
     bne rs rt label
   assume the byte_offset(addr) of label is L,
   current instruction byte_offset(addr) is A
   the relative address I for label satisfy:
     L = A + I
   so the relative addres is
     I = L - A              */
int write_sbtype(uint8_t opcode, uint8_t funct3, FILE *output, char **args, size_t num_args,
                 uint32_t addr, uint32_t current_addr) {
    /* YOUR CODE HERE */
    int rs1, rs2;
    uint32_t instruction = 0x0;
    long int relative_addr;
    /* 0: Check wrong input*/
    /* 0.1: Number of arguments should be 3*/
    if (num_args != 3) return -1;
    /* 0.2: Args should not be Null*/
    if (args == NULL || args[0] == NULL || args[1] == NULL || args[2] == NULL) return -1;
    if (addr == 1) return -1;

    /* 1: Create instruction*/
    /* 1.1: Get 2 register int*/
    rs1 = translate_reg(args[0]);
    rs2 = translate_reg(args[1]);
    /* 1.2: Error checking for register ids */
    if (rs1 == -1 || rs2 == -1) return -1;

    relative_addr = addr;
    relative_addr -= ((current_addr - 1) * 4);

    /* 1.3: Generate instruction */
    instruction = instruction | (opcode);
    instruction = instruction | ((relative_addr & 0x800) >> 4);
    instruction = instruction | ((relative_addr & 0x1e) << 7);
    instruction = instruction | (funct3 << 12);
    /* 1.3: Generate instruction */
    instruction = instruction | (rs1 << 15);
    instruction = instruction | (rs2 << 20);
    instruction = instruction | ((relative_addr & 0x7e0) << 20);
    instruction = instruction | ((relative_addr & 0x1000) << 19);

    /*Write*/
    write_inst_hex(output, instruction);
    return 0;
}

int write_utype(uint8_t opcode, FILE *output, char **args, size_t num_args) {
    /* YOUR CODE HERE */
    long int imm;
    int rd, imm_ret = -1;
    uint32_t instruction = 0x0;
    /* 0: Check wrong input*/
    /* 0.1: Number of arguments should be 2*/
    if (num_args != 2) return -1;
    /* 0.2: Args should not be Null*/
    if (args == NULL || args[0] == NULL || args[1] == NULL) return -1;
    /* 0.3: Argument3 should be 20-bits immediate*/
    imm_ret = translate_num(&imm, args[1], 0, 1048575);
    if (imm_ret== -1) return -1;

    /* 1: Create instruction*/
    /* 1.1: Get register int*/
    rd = translate_reg(args[0]);
    /* 1.2: Error checking for register ids */
    if (rd == -1) return -1;
    /* 1.3: Generate instruction */
    instruction = instruction | (opcode);
    instruction = instruction | (rd << 7);
    instruction = instruction | (imm <<12);

    /* 1.4: Write instruction*/
    write_inst_hex(output, instruction);
    return 0;
}

/* In this project there is no need to relocate labels,
   you may think about the reasons. */
int write_ujtype(uint8_t opcode, FILE *output, char **args, size_t num_args,
                 uint32_t addr, uint32_t current_addr) {
    /* YOUR CODE HERE */
    int rd;
    uint32_t instruction = 0x0;
    long int relative_addr;
    /* 0: Check wrong input*/
    /* 0.1: Number of arguments should be 1*/
    if (num_args != 2) return -1;
    /* 0.2: Args should not be Null*/
    if (args == NULL || args[0] == NULL || args[1] == NULL) return -1;
    /* 0.3: rd here should be ra?*/
    rd = translate_reg(args[0]);

    relative_addr = addr;
    relative_addr -= ((current_addr - 1) * 4);

    /* 1.3: Generate instruction */
    instruction = instruction | (opcode);
    instruction = instruction | (rd << 7);
    instruction = instruction | (relative_addr & 0xff000);
    instruction = instruction | ((relative_addr & 0x800) << 9);
    /* 1.3: Generate instruction */
    instruction = instruction | ((relative_addr & 0x7fe) << 20);
    instruction = instruction | ((relative_addr & 0x100000) << 11);

    /* Write*/
    write_inst_hex(output, instruction);
    return 0;
}
