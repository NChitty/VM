#include <stdio.h>
#include <stdlib.h>

#define MAX_PAS_LENGTH 500

int base(int L);
void print_execution(int line, char *opname, int *IR, int PC, int BP, int SP, int DP, int *pas, int GP);

// Registers
int pas[MAX_PAS_LENGTH], bp=0, sp=0, pc=0, dp=0, gp=0, free=0, ic=0, ir[3];
int main(int argc, char *argv[]) {
    // Start of program
    FILE* text;
    if(argc == 2) {
        text=fopen(argv[1], "r");
    } else {
        println("Use ./vm <file>");
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
    free = ic+40;
    bp = ic;
    pc = 0;
    sp = MAX_PAS_LENGTH-1;

    //Print Column headers
    printf("\t\t\t\t\tPC\tBP\tSP\tDP\tdata\n");
    //Print initial Values for each Register
    printf("Initial Values:\t%d\t%d\t%d\t%d\n", pc, bp, sp, dp);

    // Halt flag
    int halt = 1;
    //Program loop
    while(pc < ic && halt) {
        //Fetch Cycle
        ir[0] = pas[pc];
        ir[1] = pas[pc+1];
        ir[2] = pas[pc+2];
        int line = pc/3;
        //Execute Cycle
        char* opname;
        switch(ir[0]) {
            case 1: // LIT 0, M
                opname = "LIT";
                if (bp == gp) {
                    dp += 1;
                    pas[dp] = ir[2];
                } else {
                    sp -= 1;
                    pas[sp] = ir[2];
                }
                break;
            case 2: // OPR 0 # (0 <= # <= 13)
                switch(ir[2]) {
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
                        if(bp == gp)
                            pas[--dp] = pas[dp] == pas[dp+1];
                        else
                            pas[++sp] = pas[sp] == pas[sp-1];
                        break;
                    case 9: // NEQ
                        opname = "NEQ";
                        if(bp == gp)
                            pas[--dp] = pas[dp] != pas[dp+1];
                        else
                            pas[++sp] = pas[sp] != pas[sp-1];
                        break;
                    case 10: // LSS
                        opname = "LSS";
                        if(bp == gp)
                            pas[--dp] = pas[dp] < pas[dp+1];
                        else
                            pas[++sp] = pas[sp] < pas[sp-1];
                        break;
                    case 11: // LEQ
                        opname = "LEQ";
                        if(bp == gp)
                            pas[--dp] = pas[dp] <= pas[dp+1];
                        else
                            pas[++sp] = pas[sp] <= pas[sp-1];
                        break;
                    case 12: // GTR
                        opname = "GTR";
                        if(bp == gp)
                            pas[--dp] = pas[dp] > pas[dp+1];
                        else
                            pas[++sp] = pas[sp] > pas[sp-1];
                        break;
                    case 13: // GEQ
                        opname = "GEQ";
                        if(bp == gp)
                            pas[--dp] = pas[dp] >= pas[dp+1];
                        else
                            pas[++sp] = pas[sp] >= pas[sp-1];
                        break;
                }
                break;
            case 3: // LOD
                opname = "LOD";
                if(bp == gp)
                    pas[++dp] = pas[gp+ir[2]];
                else
                    if (base(ir[1]) == gp)
                        pas[--sp] = pas[gp+ir[2]];
                    else
                        pas[--sp] = pas[base(ir[1]+ir[2])];
                break;
            case 4: // STO
                opname = "STO";
                if(bp == gp)
                    pas[gp+ir[2]] = pas[dp--];
                else
                    if(base(ir[1]) == gp)
                        pas[gp+ir[2]] = pas[sp++];
                    else
                        pas[base(ir[1])+ir[2]] = pas[sp++]; 
                break;
            case 5: // CAL
                opname = "CAL";
                pas[sp-1] = base(ir[1]);
                pas[sp-2] = bp;
                pas[sp-3] = pc;
                bp = sp - 1;
                pc = ir[2];
                break;
            case 6: // INC
                opname = "INC";
                if(bp == gp)
                    dp = dp + ir[2];
                else
                    sp = sp - ir[2];
                break;
            case 7: // JMP
                opname = "JMP";
                pc = ir[2];
                break;
            case 8: // JPC
                opname = "JPC";
                if(bp == gp) {
                    if(pas[dp] == 0) pc = ir[2];
                    dp -= 1;
                } else {
                    if(pas[sp] == 0) pc = ir[2];
                    sp += 1;
                }
                break;
            case 9: // SYS
                opname = "SYS";
                switch(ir[2]) { // switch on M
                    case 1:
                        if(bp == gp)
                            printf("Data: %d\n", pas[dp--]);
                        else
                            printf("Top of Stack Value: %d\n", pas[sp++]);
                        break;
                    case 2:
                        int val = 0;
                        printf("Please Enter an Integer: ");
                        scanf("%d", &val);
                        if(bp == gp)
                            pas[++dp] = val;
                        else
                            pas[--sp] = val;
                        break;
                    case 3:
                        halt = 0;
                }
                break;
        }
        
        print_execution(line, opname, ir, pc, bp, sp, dp, pas, gp);
        
        //jmp condition (do not increase pc)
        if(line*3 != pc) continue;
        
        pc += 3;
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

void print_execution(int line, char *opname, int *IR, int PC, int BP, int SP, int DP, int *pas, int GP){
    int i;
    // print out instruction and registers
    printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t", line, opname, IR[1], IR[2], PC, BP, SP, DP);
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