#include <stdio.h>

void print_reg(int reg_num) {
    if(reg_num == 0) printf("x0");
    else if(reg_num == 1) printf("ra");
    else if(reg_num == 2) printf("sp");
    else if(reg_num == 3) printf("gp");
    else if(reg_num == 4) printf("tp");
    else if(reg_num == 5) printf("t0");
    else if(reg_num == 6) printf("t1");
    else if(reg_num == 7) printf("t2");
    else if(reg_num == 8) printf("s0");
    else if(reg_num == 9) printf("s1");
    else if(reg_num == 10) printf("a0");
    else if(reg_num == 11) printf("a1");
    else if(reg_num == 12) printf("a2");
    else if(reg_num == 13) printf("a3");
    else if(reg_num == 14) printf("a4");
    else if(reg_num == 15) printf("a5");
    else if(reg_num == 16) printf("a6");
    else if(reg_num == 17) printf("a7");
    else if(reg_num == 18) printf("s2");
    else if(reg_num == 19) printf("s3");
    else if(reg_num == 20) printf("s4");
    else if(reg_num == 21) printf("s5");
    else if(reg_num == 22) printf("s6");
    else if(reg_num == 23) printf("s7");
    else if(reg_num == 24) printf("s8");
    else if(reg_num == 25) printf("s9");
    else if(reg_num == 26) printf("s10");
    else if(reg_num == 27) printf("s11");
    else if(reg_num == 28) printf("t3");
    else if(reg_num == 29) printf("t4");
    else if(reg_num == 30) printf("t5");
    else if(reg_num == 31) printf("t6");
}

void rtype(int machine_code) {
    int funct7, rs2, rs1, funct3, rd;
    funct7 = (machine_code & 0xfe000000) >> 25;
    rs2    = (machine_code & 0x01f00000) >> 20;
    rs1    = (machine_code & 0x000f8000) >> 15;
    funct3 = (machine_code & 0x00007000) >> 12;
    rd     = (machine_code & 0x00000f80) >> 7;

    if (funct3 == 0x0) printf("add ");
    else if (funct3 == 0x6) printf("or ");
    else if (funct3 == 0x2) printf("slt ");
    else if (funct3 == 0x3) printf("sltu ");
    else if (funct3 == 0x1) printf("sll ");

    print_reg(rd);
    printf(", ");
    print_reg(rs1);
    printf(", ");
    print_reg(rs2);
    printf("\n");
}

/* For jalr, addi, ori*/
void itype1(int machine_code) {
    int imm, rs1, funct3, rd, opcode;
    imm    = (machine_code & 0xfff00000) >> 20; //Use sra in risc-v assembly instead of srl !!!

    //following code is useless in rsic-v assembly since we use sra!!!
    if (((imm & 0x800) >> 11) == 1) imm |= 0xfffff000;

    rs1    = (machine_code & 0x000f8000) >> 15;
    funct3 = (machine_code & 0x00007000) >> 12;
    rd     = (machine_code & 0x00000f80) >> 7;
    opcode = (machine_code & 0x0000007f);

    if (opcode == 0x67 && funct3 == 0x0) printf("jalr ");
    else if (opcode == 0x13 && funct3 == 0x0) printf("addi ");
    else if (opcode == 0x13 && funct3 == 0x6) printf("ori ");

    print_reg(rd);
    printf(", ");
    print_reg(rs1);
    printf(", ");
    printf("%d", imm);
    printf("\n");
}

/* For lb, lbu, lw*/
void itype2(int machine_code) {
    int imm, rs1, funct3, rd;
    imm    = (machine_code & 0xfff00000) >> 20; //Use sra in risc-v assembly instead of srl !!!

    //following code is useless in rsic-v assembly since we use sra!!!
    if (((imm & 0x800) >> 11) == 1) imm |= 0xfffff000;

    rs1    = (machine_code & 0x000f8000) >> 15;
    funct3 = (machine_code & 0x00007000) >> 12;
    rd     = (machine_code & 0x00000f80) >> 7;

    if (funct3 == 0x0) printf("lb ");
    else if (funct3 == 0x4) printf("lbu ");
    else if (funct3 == 0x2) printf("lw ");

    print_reg(rd);
    printf(", ");
    printf("%d", imm);
    printf("(");
    print_reg(rs1);
    printf(")");
    printf("\n");
}

void stype(int machine_code) {
    int imm, rs2, rs1, funct3;

    //Use sra in risc-v assembly instead of srl !!!
    imm = (machine_code & 0xfe000000) >> 20; // imm[11:5]
    //Use srl in risc-v assembly instead of sra !!!
    imm |= ((machine_code & 0x00000f80) >> 7); // imm[4:0]

    //following code is useless in rsic-v assembly since we use sra!!!
    if (((imm & 0x800) >> 11) == 1) imm |= 0xfffff000;

    rs2    = (machine_code & 0x01f00000) >> 20;
    rs1    = (machine_code & 0x000f8000) >> 15;
    funct3 = (machine_code & 0x00007000) >> 12;

    if (funct3 == 0x0) printf("sb ");
    else if (funct3 == 0x2) printf("sw ");

    print_reg(rs2);
    printf(", ");
    printf("%d", imm);
    printf("(");
    print_reg(rs1);
    printf(")");
    printf("\n");
}

void sbtype(int machine_code) {
    int imm, rs2, rs1, funct3;

    //Use sra in risc-v assembly instead of srl !!!
    imm  = (machine_code & 0x80000000) >> 19; // imm[12]
    //Use srl in risc-v assembly instead of sra !!!
    imm |= (machine_code & 0x00000080) << 4; // imm[11]
    imm |= ((machine_code & 0x7e000000) >> 20); // imm[10:5]
    imm |= ((machine_code & 0x00000f00) >> 7); // imm[4:1]

    //following code is useless in rsic-v assembly since we use sra!!!
    if (((imm & 0x1000) >> 12) == 1) imm |= 0xffffe000;

    rs2    = (machine_code & 0x01f00000) >> 20;
    rs1    = (machine_code & 0x000f8000) >> 15;
    funct3 = (machine_code & 0x00007000) >> 12;

    if (funct3 == 0x0) printf("beq ");
    else if (funct3 == 0x1) printf("bne ");
    else if (funct3 == 0x4) printf("blt ");
    else if (funct3 == 0x5) printf("bge ");

    print_reg(rs1);
    printf(", ");
    print_reg(rs2);
    printf(", ");
    printf("%d", imm);
    printf("\n");
}

void ujtype(int machine_code) {
    int imm, rd;

    //Use sra in risc-v assembly instead of srl !!!
    imm  = (machine_code & 0x80000000) >> 11; // imm[20]
    //Use srl in risc-v assembly instead of sra !!!
    imm |= (machine_code & 0x000ff000); // imm[19:12]
    imm |= ((machine_code & 0x00100000) >> 9); // imm[11]
    imm |= ((machine_code & 0x7fe00000) >> 20); // imm[10:1]

    //following code is useless in rsic-v assembly since we use sra!!!
    if (((imm & 0x100000) >> 20) == 1) imm |= 0xffe00000;

    rd = (machine_code & 0x00000f80) >> 7;

    printf("jal ");
    print_reg(rd);
    printf(", ");
    printf("%d", imm);
    printf("\n");
}

void utype(int machine_code) {
    int imm, rd;
    //Use srl in risc-v assembly instead of sra !!!
    imm = (machine_code & 0xfffff000) >> 12;
    rd  = (machine_code & 0x00000f80) >> 7;

    printf("lui ");
    print_reg(rd);
    printf(", ");
    printf("%d", imm);
    printf("\n");
}

int main() {
    int lines_of_machine_codes = 9;
    int machine_codes[9] = {0x000502B3, 0x00100313, 0x00028863, 0x01DE13B3, 0xFFF28293, 0xFF5FF06F, 0x00600533, 0x00008067, 0xFF1C42B7};

    int i = 0;
    for (i = 0; i < lines_of_machine_codes; i++) {
        int opcode = machine_codes[i] & 0x7f;
        if (opcode == 0x33) rtype(machine_codes[i]);
        else if (opcode == 0x67 || opcode == 0x13) itype1(machine_codes[i]);
        else if (opcode == 0x03) itype2(machine_codes[i]);
        else if (opcode == 0x23) stype(machine_codes[i]);
        else if (opcode == 0x63) sbtype(machine_codes[i]);
        else if (opcode == 0x6f) ujtype(machine_codes[i]);
        else if (opcode == 0x37) utype(machine_codes[i]);
    }

    return 0;
}