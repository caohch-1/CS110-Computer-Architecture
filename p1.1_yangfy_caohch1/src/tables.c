/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "tables.h"

const int SYMBOLTBL_NON_UNIQUE = 0;
const int SYMBOLTBL_UNIQUE_NAME = 1;

/*******************************
 * Helper Functions
 *******************************/

/* allocation_failed*/
void allocation_failed() {
    write_to_log("Error: allocation failed\n");
    exit(1);
}

/* addr_alignment_incorrect*/
void addr_alignment_incorrect() {
    write_to_log("Error: address is not a multiple of 4.\n");
}

/* name_already_exists*/
void name_already_exists(const char *name) {
    write_to_log("Error: name '%s' already exists in table.\n", name);
}

/* write_sym*/
void write_sym(FILE *output, uint32_t addr, const char *name) {
    fprintf(output, "%u\t%s\n", addr, name);
}

/*******************************
 * Symbol Functions
 *******************************/
/*Create a new symbol*/
Symbol *create_symbol(char *name, uint32_t addr) {
    /* Get length of name as 3th argument of memcpy()*/
    size_t name_len = strlen(name) + 1;
    /* Malloc for itself*/
    Symbol *new_symbol = (Symbol *) malloc(sizeof(Symbol));
    if (new_symbol == NULL) allocation_failed();
    /* Malloc for name*/
    new_symbol->name = (char *) malloc(name_len * sizeof(char));
    if (new_symbol->name == NULL) allocation_failed();
    /* Deepcopy name's contexts*/
    memcpy(new_symbol->name, name, name_len);
    new_symbol->addr = addr;

    return new_symbol;
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Check if name is already in table*/
int find_symbol(SymbolTable *table, const char *name) {
    uint32_t i;
    if (table == NULL) return -1;
    /* Traverse table*/
    for (i = 0; i < table->len; ++i) {
        if (strcmp(table->dataArray[i]->name, name) == 0) return i;
    }
    return -1;
}

/* Creates a new SymbolTable containg 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time.
   If memory allocation fails, you should call allocation_failed().
   Mode will be either SYMBOLTBL_NON_UNIQUE or SYMBOLTBL_UNIQUE_NAME. You will need
   to store this value for use during add_to_table().
 */
SymbolTable *create_table(int mode) {
    /* YOUR CODE HERE */
    /* Malloc for table itself */
    SymbolTable *new_table = (SymbolTable *) malloc(sizeof(SymbolTable));
    if (new_table == NULL) allocation_failed();
    /* Init table's properties, here we init cap as 10*/
    new_table->mode = mode;
    new_table->len = 0;
    new_table->cap = 10;
    /* Malloc for dataArray*/
    new_table->dataArray = (Symbol **) malloc(10 * sizeof(Symbol *));
    if (new_table->dataArray == NULL) allocation_failed();

    return new_table;
}

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable *table) {
    /* YOUR CODE HERE */
    if (table == NULL) return;
    else {
        uint32_t i;
        /* Free every symbol in table*/
        for (i = 0; i < table->len; ++i) {
            free(table->dataArray[i]->name);
            free(table->dataArray[i]);
        }
        /* Free dataArray*/
        free(table->dataArray);
        /* Free table itself*/
        free(table);
    }
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE.
   1. ADDR is given as the byte offset from the first instruction.
   2. The SymbolTable must be able to resize itself as more elements are added.

   3. Note that NAME may point to a temporary array, so it is not safe to simply
   store the NAME pointer. You must store a copy of the given string.

   4. If ADDR is not word-aligned, you should call addr_alignment_incorrect()
   and return -1.

   5. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists
   in the table, you should call name_already_exists() and return -1.

   6.If memory allocation fails, you should call allocation_failed().

   Otherwise, you should store the symbol name and address and return 0.
 */
int add_to_table(SymbolTable *table, const char *name, uint32_t addr) {
    /* YOUR CODE HERE */
    /* 0: Check invalid input*/
    /* 0.1: Check NULL input*/
    if (table == NULL || name == NULL) return -1;
    /* 0.2: Check word alignment*/
    if (addr % 4 != 0) {
        addr_alignment_incorrect();
        return -1;
    }
    /* 0.3: Check whether name already exists in table if SYMTBL_UNIQUE_NAME*/
    if (table->mode == SYMBOLTBL_UNIQUE_NAME && find_symbol(table, (char *) name) != -1) {
        name_already_exists(name);
        return -1;
    }


    /* 1: Add new symbol into table*/
    if (table->len != table->cap) {
        /* 1.1: Current capacity is enough*/
        /* 1.1.1: Create new symbol*/
        Symbol *new_symbol = create_symbol((char *) name, addr);
        /* 1.1.2: Add new symbol into dataArray*/
        table->dataArray[table->len] = new_symbol;
        /* 1.1.3: Update len of table*/
        table->len = table->len + 1;

        return 0;
    } else {
        /* 1.2: Current capacity need to be extended*/
        /* 1.2.1: Create new symbol*/
        Symbol *new_symbol = create_symbol((char *) name, addr);
        /* 1.2.2: Malloc new dataArray， every time double the cap*/
        Symbol **new_dataArray = (Symbol **) malloc(2 * table->cap * sizeof(Symbol *));
        if (new_dataArray == NULL) allocation_failed();
        /* 1.2.3: Copy old dataArray into new dataArray*/
        memcpy(new_dataArray, table->dataArray, table->cap * sizeof(Symbol *));
        /* 1.2.4: Add new symbol into new dataArray*/
        new_dataArray[table->len] = new_symbol;
        /* 1.2.5: free old dataArray*/
        free(table->dataArray);
        /* 1.2.6: Update properties of table, including dataArray, cap and len*/
        table->dataArray = new_dataArray;
        table->cap = 2 * table->cap;
        table->len = table->len + 1;

        return 0;
    }
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable *table, const char *name) {
    /* YOUR CODE HERE */
    /*Get target symbol's index*/
    int res_index = find_symbol(table, name);
    if (res_index == -1) return 1;
    else return table->dataArray[res_index]->addr;
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_sym() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable *table, FILE *output) {
    /* YOUR CODE HERE */
    uint32_t i;
    if (table == NULL || output == NULL) return;
    for (i = 0; i < table->len; ++i) {
        write_sym(output, table->dataArray[i]->addr, table->dataArray[i]->name);
    }
}

/* Reads the symbol table file written by the `write_table` function, and
   restore the symbol table.
 */
SymbolTable *create_table_from_file(int mode, FILE *file) {
    /* YOUR CODE HERE */
    /* Create table*/
    SymbolTable *table = create_table(mode);
    /* Read file*/
    while (!feof(file)) {
        uint32_t addr;
        char name[40];
        fscanf(file, "%u\t%s\n", &addr, name);
        /* Add into table*/
        add_to_table(table, name, addr);
    }
    return table;
}
