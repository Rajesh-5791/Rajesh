// To test the prime.c program

#include <stdio.h>
#include <stdlib.h>
#include "prime.c"

void testPrime()
{
    int counter, successCounter = 0;
    char testCases[20][10] = {"-5", "0", "1", "2", "3", "4", "10", "15", "25", "31", "a", "r8", "rajesh", "7raj", "ra7", "!", " ", "\t", "%", "  "};
    int testCasesSize = 20;

    for (counter = 0; counter < testCasesSize; counter++) 
    {
        if (isPrime(testCases[counter]) == 0)
        {
            printf("Failed(%s)\n", testCases[counter]);
        }
        else
        {
            successCounter++;
        }
    }
    if (counter == successCounter)
    {
        printf("All tests passed.\n");
    }
}

int main() 
{
    testPrime();
}