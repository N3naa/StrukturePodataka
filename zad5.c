#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#define MAXLINE 1024

struct StackElement;
typedef struct StackElement* Position;

struct StackElement
{
    double value;
    Position prev;
};

int Push(Position p,double value);
int Pop(Position p);
int ReadFile(Position p);
int Add(Position p);
int Multipy(Position p);
int Subtract(Position p);
int Divide(Position p);

int main()
{
    struct StackElement Stack = { .value = 0, .prev = NULL};

    ReadFile(&Stack);

    printf("Result: %.2lf\n", Stack.prev->value);
    free(Stack.prev);

    return 0;
}

int Push(Position p,double value)
{
    Position q = (Position )malloc(sizeof(struct StackElement));

    if(q == NULL)
    {
        perror("Could not allocate!");
        return EXIT_FAILURE;
    }

    q->value = value;

    q->prev = p->prev;
    p->prev = q;

    return EXIT_SUCCESS;
}

int Pop(Position p)
{
    Position temp;

    if(p->prev == NULL)
    {
        perror("Popping from empty stack!");
        return EXIT_FAILURE;
    }

    temp = p->prev;
    p->prev = p->prev->prev;
    free(temp);

    return EXIT_SUCCESS;
}

int ReadFile(Position p)
{
    FILE* fp = fopen("postfix_notation.txt", "r");

    if(fp == NULL)
    {
        perror("Errror opening file!");
        return EXIT_FAILURE;
    }

    char buffer[MAXLINE];
    char *p_buffer = buffer;
    int numBytes;
    char operator;
    double operand;

    fgets(buffer,MAXLINE,fp);
    fclose(fp);
    printf("Postfix expression: %s\n", buffer);

    while(strlen(p_buffer) > 0)
    {
        int check_operand = sscanf(p_buffer,"%lf %n", &operand, &numBytes);

        if(check_operand != 1)
        {
            perror("Invalid postfix notation!");
            return EXIT_FAILURE;
        }

        Push(p,operand);

        p_buffer += numBytes;

        int check_operator = 0;
        while((check_operator = sscanf(p_buffer," %c %n", &operator, &numBytes) == 1) && !isalnum(operator))
        {
    
            switch (operator)
            {
                case '+':
                    Add(p);
                    break;
                case '*':
                    Multipy(p);
                    break;
                case '-':
                    Subtract(p);
                    break;
                case '/':
                    Divide(p);
                    break;
                default:
                    puts("Invalid operator!");
                    break;
            } 

            p_buffer += numBytes;
        }

    }

    return EXIT_SUCCESS;
}

int Add(Position p)
{
    double result = p->prev->value + p->prev->prev->value;

    Pop(p);
    Pop(p);

    Push(p,result);

    return EXIT_SUCCESS;
}

int Multipy(Position p)
{   
    double result = p->prev->value * p->prev->prev->value;

    Pop(p);
    Pop(p);

    Push(p,result);

    return EXIT_SUCCESS;
}

int Subtract(Position p)
{
    double result = p->prev->prev->value - p->prev->value;

    Pop(p);
    Pop(p);

    Push(p,result);

    return EXIT_SUCCESS;
}

int Divide(Position p)
{
    if(p->prev->value == 0)
    {
        perror("Dividing by zero!");
        return EXIT_FAILURE;
    }
    
    double result = p->prev->prev->value / p->prev->value;

    Pop(p);
    Pop(p);

    Push(p,result);

    return EXIT_SUCCESS;
}