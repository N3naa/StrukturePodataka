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
    Position next;
};

int Push(Position p,double value);
int Pop(Position p);
int Operation(Position p,char operator);
int ReadFile(Position p);
int Add(Position p);
int Multipy(Position p);
int Subtract(Position p);
int Divide(Position p);

int main()
{
    struct StackElement Stack = { .value = 0, .next = NULL};

    ReadFile(&Stack);

    printf("Result: %.2lf\n", Stack.next->value);

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

    q->next = p->next;
    p->next = q;

    return EXIT_SUCCESS;
}

int Pop(Position p)
{
    Position temp;

    if(p->next == NULL)
    {
        perror("Popping from empty stack!");
        return EXIT_FAILURE;
    }

    temp = p->next;
    p->next = p->next->next;
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
    printf("Postfix expersion: %s\n", buffer);
    fclose(fp);

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
            Operation(p,operator);
            p_buffer += numBytes;
        }

    }

    return EXIT_SUCCESS;
}

int Operation(Position p,char operator)
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

    return EXIT_SUCCESS;
}

int Add(Position p)
{
    double value = p->next->value + p->next->next->value;

    Pop(p);
    Pop(p);

    Push(p,value);

    return EXIT_SUCCESS;
}

int Multipy(Position p)
{
    double value = p->next->value * p->next->next->value;

    Pop(p);
    Pop(p);

    Push(p,value);

    return EXIT_SUCCESS;
}

int Subtract(Position p)
{
    double value = p->next->next->value - p->next->value;

    Pop(p);
    Pop(p);

    Push(p,value);

    return EXIT_SUCCESS;
}

int Divide(Position p)
{
    if(p->next->next->value == 0)
    {
        puts("Dividing by zero!");
        return EXIT_FAILURE;
    }

    double value = p->next->next->value / p->next->value;

    Pop(p);
    Pop(p);

    Push(p,value);

    return EXIT_SUCCESS;
}