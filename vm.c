#include <stdio.h>
#include <stdlib.h>

#define MAX_PAS_LENGTH 500
#define JUMP 0x0F // Bottom bits = 1 
#define HALT 0xF0 // Top bits = 1

// Instruction Register Struct to clean up code
typedef struct instruction_register {
    int OP;
    int L;
    int M;
} instruction_register;

int base(int L);
void print_execution(int line, char *opname, instruction_register IR, int PC, int BP, int SP, int DP, int *pas, int GP);

// Registers
int pas[MAX_PAS_LENGTH], bp=0, sp=0, pc=0, dp=0, gp=0, FREE=0, ic=0;
instruction_register ir;
// 8-bit register to hold flags, currently only jump and hold
unsigned char flags = 0x00;
int main(int argc, char *argv[]) {
    // Start of program
    FILE* text;
    if(argc == 2) {
        text=fopen(argv[1], "r");
    } else {
        printf("Use ./vm <file>\n");
        return -1;
    }

    //Read all the instructions from the file in the pas
    while(!feof(text)) {
        int op, l, m;
        fscanf(text, "%d %d %d", &op, &l, &m);
        pas[ic] = op;
        pas[ic+1] = l;
        pas[ic+2] = m;
        ic +=3;
    }
    fclose(text);

    //Set all the registers to their appropriate positions
    gp = ic;
    dp = ic-1;
    FREE = ic+40;
    bp = ic;
    pc = 0;
    sp = MAX_PAS_LENGTH;

    //Print Column headers
    printf("\t\t\t\tPC\tBP\tSP\tDP\tdata\n");
    //Print initial Values for each Register
    printf("Initial Values:\t\t\t%d\t%d\t%d\t%d\n", pc, bp, sp, dp);

    //Program loop
    while(1) {
        //Fetch Cycle
        flags  = 0x00;
        ir.OP = pas[pc];
        ir.L = pas[pc+1];
        ir.M = pas[pc+2];
        int line = pc/3;
        pc += 3;
        //Execute Cycle
        char* opname;
        switch(ir.OP) {
            case 1: // LIT 0, M
                opname = "LIT";
                if (bp == gp) {
                    dp += 1;
                    pas[dp] = ir.M;
                } else {
                    sp -= 1;
                    pas[sp] = ir.M;
                }
                break;
            case 2: // OPR 0 # (0 <= # <= 13)
                switch(ir.M) {
                    case 0: // RTN
                        opname = "RTN";
                        sp = bp + 1;
                        bp = pas[sp-2];
                        pc = pas[sp-3];
                        break;
                    case 1: // NEG
                        opname = "NEG";
                        if(bp==gp)
                            pas[dp] *= -1;
                        else
                            pas[sp] *= -1;
                        break;
                    case 2: // ADD
                        opname = "ADD";
                        if(bp == gp) {
                            dp -= 1;
                            pas[dp] = pas[dp] + pas[dp+1];
                        } else {
                            sp += 1;
                            pas[sp] = pas[sp] + pas[sp-1];
                        }
                        break;
                    case 3: // SUB
                        opname = "SUB";
                        if(bp == gp) {
                            dp -= 1;
                            pas[dp] = pas[dp] - pas[dp+1];
                        } else {
                            sp += 1;
                            pas[sp] = pas[sp] - pas[sp-1];
                        }
                        break;
                    case 4: // MUL
                        opname = "MUL";
                        if(bp == gp) {
                            dp -= 1;
                            pas[dp] = pas[dp] * pas[dp+1];
                        } else {
                            sp += 1;
                            pas[sp] = pas[sp] * pas[sp-1];
                        }
                        break;
                    case 5: // DIV
                        opname = "DIV";
                        if(bp == gp) {
                            dp -= 1;
                            pas[dp] = pas[dp] / pas[dp+1];
                        } else {
                            sp += 1;
                            pas[sp] = pas[sp] / pas[sp-1];
                        }
                        break;
                    case 6: // ODD
                        opname = "ODD";
                        if(bp == gp)
                            pas[dp] = pas[dp] % 2;
                        else
                            pas[sp] = pas[sp] % 2;
                        break;
                    case 7: // MOD
                        opname = "MOD";
                        if(bp == gp) {
                            dp -= 1;
                            pas[dp] = pas[dp] % pas[dp+1];
                        } else {
                            sp += 1;
                            pas[sp] = pas[sp] % pas[sp-1];
                        }
                    case 8: // EQL
                        opname = "EQL";
                        if(bp == gp) {
                            dp -= 1;
                            pas[dp] = (pas[dp] == pas[dp+1]);
                        } else {
                            sp += 1;
                            pas[sp] = (pas[sp] == pas[sp-1]);
                        }
                        break;
                    case 9: // NEQ
                        opname = "NEQ";
                        if(bp == gp) {
                            dp -= 1;
                            pas[dp] = (pas[dp] != pas[dp+1]);
                        } else {
                            sp += 1;
                            pas[sp] = pas[sp] != pas[sp-1];
                        }
                        break;
                    case 10: // LSS
                        opname = "LSS";
                        if(bp == gp) {
                            dp -= 1;
                            pas[dp] = pas[dp] < pas[dp+1];
                        } else {
                            sp += 1;
                            pas[sp] = pas[sp] < pas[sp-1];
                        }
                        break;
                    case 11: // LEQ
                        opname = "LEQ";
                        if(bp == gp) {
                            dp -= 1;
                            pas[dp] = pas[dp] <= pas[dp+1];
                        } else {
                            sp += 1;
                            pas[sp] = pas[sp] <= pas[sp-1];
                        }
                        break;
                    case 12: // GTR
                        opname = "GTR";
                        if(bp == gp) {
                            dp -= 1;
                            pas[dp] = pas[dp] > pas[dp+1];
                        } else {
                            sp += 1;
                            pas[sp] = pas[sp] > pas[sp-1];
                        }
                        break;
                    case 13: // GEQ
                        opname = "GEQ";
                        if(bp == gp) {
                            dp -= 1;
                            pas[dp] = pas[dp] >= pas[dp+1];
                        } else {
                            sp += 1;
                            pas[sp] = pas[sp] >= pas[sp-1];
                        }
                        break;
                }
                break;
            case 3: // LOD
                opname = "LOD";
                if(bp == gp) {
                    dp += 1;
                    pas[dp] = pas[gp+ir.M];
                } else {
                    sp -= 1;
                    if (base(ir.L) == gp)
                        pas[sp] = pas[gp+ir.M];
                    else
                        pas[sp] = pas[base(ir.L+ir.M)];
                }
                break;
            case 4: // STO
                opname = "STO";
                if(bp == gp) {
                    pas[gp+ir.M] = pas[dp];
                    dp -= 1;
                } else {
                    if(base(ir.L) == gp)
                        pas[gp+ir.M] = pas[sp];
                    else
                        pas[base(ir.L)+ir.M] = pas[sp]; 
                    sp +=1;
                }
                break;
            case 5: // CAL
                opname = "CAL";
                pas[sp-1] = base(ir.L);
                pas[sp-2] = bp;
                pas[sp-3] = pc;
                bp = sp - 1;
                pc = ir.M;
                flags |= JUMP;
                break;
            case 6: // INC
                opname = "INC";
                if(bp == gp)
                    dp = dp + ir.M;
                else
                    sp = sp - ir.M;
                break;
            case 7: // JMP
                opname = "JMP";
                pc = ir.M;
                flags |= JUMP;
                break;
            case 8: // JPC
                opname = "JPC";
                if(bp == gp) {
                    if(pas[dp] == 0) pc = ir.M;
                    dp -= 1;
                } else {
                    if(pas[sp] == 0) pc = ir.M;
                    sp += 1;
                }
                break;
            case 9: // SYS
                opname = "SYS";
                switch(ir.M) { // switch on M
                    case 1:
                        if(bp == gp)
                            printf("Data: %d\n", pas[dp--]);
                        else
                            printf("Top of Stack Value: %d\n", pas[sp++]);
                        break;
                    case 2:
                        printf("Please Enter an Integer: ");
                        if(bp == gp) {
                            dp += 1;
                            scanf("%d", &pas[dp]);
                        } else {
                            sp -= 1;
                            scanf("%d", &pas[sp]);
                        }
                        break;
                    case 3:
                        flags |= HALT;
                }
                break;
        }
        print_execution(line, opname, ir, pc, bp, sp, dp, pas, gp);
        if((flags & JUMP) == JUMP) continue;
        if((flags & HALT) == HALT) break; 
    }
    return 0;
}

/**********************************************/
/*          Find base L levels down           */
/**********************************************/
int base(int L) {
    int arb = bp; // arb = activation record base
    while ( L > 0) {   //find base L levels down
        arb = pas[arb];
        L--;
    }
    return arb;
}

void print_execution(int line, char *opname, instruction_register IR, int PC, int BP, int SP, int DP, int *pas, int GP){
    int i;
    // print out instruction and registers
    printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t", line, opname, IR.L, IR.M, PC, BP, SP, DP);
    // print data section
    for (i = GP; i <= DP; i++)
        printf("%d ", pas[i]);
    printf("\n");
    // print stack
    printf("\tstack : ");
    for (i = MAX_PAS_LENGTH - 1; i >= SP; i--)
        printf("%d ", pas[i]);
    printf("\n");
}