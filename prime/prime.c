// To know whether a given number is prime or not

#include <stdio.h>
#include <stdlib.h>

int isPrime(char *value) 
{
    int divisor = 0;
    int number = atoi(value);

    if (!isdigit(value[0]) || number <= 1) 
    {
        return 1;
    }
    else
    {
        for (divisor = 2; divisor * divisor <= number; divisor++) 
        {
            if (number % divisor == 0) 
            {
                return 1;
            }
        }
        return 2;
    }
    return 0;
}
