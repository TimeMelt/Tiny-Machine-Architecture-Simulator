// Tiny Machine Architecture (Simulator)

#include <stdio.h>
#include <stdlib.h>
#define MAXPROGRAMSIZE 100
#define MAXMEMORYSIZE 10

struct instruction 
{
    int opCode, deviceOrAddress;
};

struct instruction programMemory[MAXPROGRAMSIZE];
struct instruction instructionRegister;

int accumulateor;
int memory[MAXMEMORYSIZE];
int programCounter = 0;
int memoryAddressRegister;
int memoryDataRegister;
int decoder = 0;

void fetchExecute(void);
void load(void);
void store(void);
void add(void);
void sub(void);
void output(void);
void input(void);
void skipz(void);
void jump(void);

int main(int argc, char *argv[])
{
    int c;

    //read input file 
    FILE *file;

    if(argc!=2)
    {
        printf("Please input the file!\n");
        exit(1);
    }

    file = fopen(argv[1], "r");

    if (file)
    {
        printf("Assembling the Program...\n");
        int programIndex = 0;

        while ((c = getc(file)) != EOF)
        {
            c-='0';
            if(c<0||c>9)
            {
                continue;
            }

            if(programIndex%2==0)
            {
                programMemory[programCounter+programIndex/2].opCode=c;
            }
            else
            {
                programMemory[programCounter+programIndex/2].deviceOrAddress=c;
            }

            programIndex++;

        }

        printf("Program Assembled.\n\n");
        printf("Run.\n\n");

        while(decoder!=7)
        {
            switch(decoder)
            {
                case 0:
                    fetchExecute();
                    break;
                case 1:
                    load();
                    break;
                case 2:
                    add();
                    break;
                case 3:
                    store();
                    break;
                case 4:
                    sub();
                    break;
                case 5:
                    input();
                    break;
                case 6:
                    output();
                    break;
                case 8:
                    jump();
                    break;
                case 9:
                    skipz();
                    break;

            }
        }

        printf("Program Complete\n");
        fclose(file);
    } 
    else
    {
        printf("File not found\n");
    }
    
    // system("PAUSE"); uncomment when running in windows env.

    return 0;

}

//Fetch an instruction from the memory address pointed to by the PC (Program counter),
//places it in the instruction register (IR) and increments the PC by one
//The OP (Operation code) in the IR tells the computer what is the instruction that must be executed.
void fetchExecute()
{
    memoryAddressRegister=programCounter;
    ++programCounter;
    memoryDataRegister = programMemory[memoryAddressRegister].deviceOrAddress;
    instructionRegister.deviceOrAddress=memoryDataRegister;
    memoryDataRegister = programMemory[memoryAddressRegister].opCode;
    instructionRegister.opCode=memoryDataRegister;
    decoder = instructionRegister.opCode;
}

//Loads the contents of instructionRegister address into AC (AC stand for Accumulator).
void load()
{
    printf("/*Loading value into AC*/\n");
    memoryAddressRegister = instructionRegister.deviceOrAddress;
    memoryDataRegister = memory[memoryAddressRegister];
    accumulateor = memoryDataRegister;
    decoder = 0;
    int i = 0;
    printf("PC = %d \t AC = %d \t MEM = [",programCounter,accumulateor);
    for(i = 0; i < MAXMEMORYSIZE - 1; i++)
    {
        printf("%d, ",memory[i]);
    }
    printf("%d]\n\n",memory[i]);
}

//Store the contents of AC into instructionResister address
void store()
{
    printf("/*Storing AC value into memory*/\n");
    memoryAddressRegister = instructionRegister.deviceOrAddress;
    memoryDataRegister = accumulateor;
    memory[memoryAddressRegister] = memoryDataRegister;
    decoder = 0;
    int i = 0;
    printf("PC = %d \t AC = %d \t MEM = [",programCounter,accumulateor);
    for(i = 0; i < MAXMEMORYSIZE - 1; i++)
    {
        printf("%d, ",memory[i]);
    }
    printf("%d]\n\n",memory[i]);
}

//The data value stored at instructionRegister address is added to the AC and the result is stored back in the AC.
void add()
{
    printf("/*Adding memory location value and AC into AC*/\n");
    memoryAddressRegister = instructionRegister.deviceOrAddress;
    memoryDataRegister = memory[memoryAddressRegister];
    accumulateor = accumulateor+memoryDataRegister;
    decoder = 0;
    int i = 0;
    printf("PC = %d \t AC = %d \t MEM = [",programCounter,accumulateor);
    for(i = 0; i < MAXMEMORYSIZE - 1; i++)
    {
        printf("%d, ",memory[i]);
    }
    printf("%d]\n\n",memory[i]);
}

//Subtracts the value located at instructionRegister address from the AC and stored the result back in the AC.
void sub()
{
    printf("/*Subtracting memory location value and AC into AC*/\n");
    memoryAddressRegister = instructionRegister.deviceOrAddress;
    memoryDataRegister = memory[memoryAddressRegister];
    accumulateor = accumulateor-memoryDataRegister;
    decoder = 0;
    int i = 0;
    printf("PC = %d \t AC = %d \t MEM = [",programCounter,accumulateor);
    for(i = 0; i < MAXMEMORYSIZE - 1; i++)
    {
        printf("%d, ",memory[i]);
    }
    printf("%d]\n\n",memory[i]);
}

//Print out the contents of the AC in the output device.
void output()
{
    printf("/*outputting AC to screen*/\n");
    printf("%d\n", accumulateor);
    int i = 0;
    printf("PC = %d \t AC = %d \t MEM = [",programCounter,accumulateor);
    for(i = 0; i < MAXMEMORYSIZE - 1; i++)
    {
        printf("%d, ",memory[i]);
    }
    printf("%d]\n\n",memory[i]);
    decoder = 0;
}

//A value from the input device is transferred into the AC.
void input()
{
    printf("Enter a number: ");
    scanf("%d",&accumulateor);
    decoder = 0;
    int i = 0;
    printf("PC = %d \t AC = %d \t MEM = [",programCounter,accumulateor);
    for(i = 0; i < MAXMEMORYSIZE - 1; i++)
    {
        printf("%d, ",memory[i]);
    }
    printf("%d]\n\n",memory[i]);
    decoder = 0;
}

//If the contents of the Accumulator = 0 the next instruction is skipped.
void skipz()
{
    if (accumulateor==0)
    {
        ++programCounter;
    }
    decoder = 0;
}

//Causes an unconditional branch to instructionRegister address.
void jump()
{
    programCounter = instructionRegister.deviceOrAddress;
    decoder = 0;
}