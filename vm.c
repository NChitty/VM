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
    free = ic+10;
    bp = ic;
    pc = 0;
    sp = 499;

    //Print Column headers
    //Print initial Values for each Register
    while(pc < ic) {
        //Fetch Cycle
        ir[0] = pas[pc];
        ir[1] = pas[pc+1];
        ir[2] = pas[pc+2];
        //Execute Cycle
        switch(ir[0]) {
        
        }
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