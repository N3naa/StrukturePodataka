#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define MAX_LINE 1024

struct Element;
typedef struct Element* Position;

struct Element
{
    int exponent;
    int coefficient;
    Position next;
};

void Print(Position p);
Position CreateElement(int exponent, int coefficient);
int ReadFile(Position p,char* file_name);
int InsertSorted(Position head, Position new_element);
int InsertAfter(Position p,Position nE);
int DeleteAfter(Position p);
int Sum(Position p1,Position p2,Position S);
int Product(Position p1,Position p2,Position P);

int main()
{
    struct Element P1 = { .exponent = 0, .coefficient = 0, .next = NULL};
    struct Element P2 = { .exponent = 0, .coefficient = 0, .next = NULL};

    char first_file[] = "first_polynomial.txt";
    char second_file[] = "second_polynomial.txt";

    ReadFile(&P1,first_file);
    printf("First Polynomial:\t\t");
    Print(P1.next);
    ReadFile(&P2,second_file);
    printf("Second Polynomial:\t\t");
    Print(P2.next);

    struct Element S = { .exponent = 0, .coefficient = 0, .next = NULL};
    Sum(P1.next,P2.next,&S);
    printf("Sum of Polynomials:\t\t");
    Print(S.next);

    struct Element P = { .exponent = 0, .coefficient = 0, .next = NULL};
    Product(P1.next,P2.next,&P);
    printf("Product of Polynomials: \t");
    Print(P.next);

    return 0;
}

void Print(Position p)
{
    while(p != NULL)
    {
        if(p->coefficient > 0) printf("%dx^%d ", p->coefficient, p->exponent);
        else printf("\b\b%dx^%d ", p->coefficient, p->exponent);

        if(p->next != NULL) printf(" + ");
        p = p->next;
    }

    puts(" ");
}

int InsertSorted(Position head, Position new_element)
{
    Position temp = head;

    if(temp->next != NULL && temp->next->exponent < new_element->exponent)
    {
        InsertAfter(temp,new_element);
    }
    
    else
    {
        while(temp->next != NULL && temp->next->exponent > new_element->exponent)
        {
            temp = temp->next;
        }

        if(temp->next == NULL || temp->next->exponent != new_element->exponent)
        {
            InsertAfter(temp,new_element);
        }

        else if(temp->next->exponent == new_element->exponent)
        {
            int ResultCoefficient = temp->next->coefficient + new_element->coefficient;

            if(ResultCoefficient == 0)
            {
                DeleteAfter(temp);
                free(new_element);
            }

            else
            {
                temp->next->coefficient = ResultCoefficient;
                free(new_element);
            }
        }
    }

    return EXIT_SUCCESS;
}

int ReadFile(Position p,char* file_name)
{
    FILE* fp = fopen(file_name,"r");

    if(fp == NULL)
    {
        perror("Could not open the file!");
        return EXIT_FAILURE;
    }
    
    char buffer[MAX_LINE];
    char* p_buffer = buffer;
    int numBytes = 0;
    int coefficient = 0;
    int exponent = 0;

    fgets(buffer,MAX_LINE,fp);
    
    while(strlen(p_buffer) > 0)
    {
        int status = sscanf(p_buffer," %d %d %n", &coefficient, &exponent, &numBytes);
        if(status != 2)
        {
            perror("Corrupted file!!!");
            return EXIT_FAILURE;
        }

        Position new_element = CreateElement(exponent,coefficient);
        InsertSorted(p,new_element);
        p_buffer += numBytes;
    }

    return EXIT_SUCCESS;
}

Position CreateElement(int exponent, int coefficient)
{
    Position q = (Position )malloc(sizeof(struct Element));

    if(q == NULL)
    {
        perror("Could not allocate!");
        return NULL;
    }

    q->exponent = exponent;
    q->coefficient = coefficient;

    return q;
}

int InsertAfter(Position p,Position new_element)
{

    new_element->next = p->next;
    p->next = new_element;

    return EXIT_SUCCESS;
}

int DeleteAfter(Position p)
{
    Position trash;

    if(p->next == NULL)
    {
        perror("Freeing NULL pointer!");
        return EXIT_FAILURE;
    }

    trash = p->next;
    p->next = p->next->next;
    free(trash);

    return EXIT_SUCCESS;
}

int Sum(Position p1,Position p2,Position S)
{
    Position temp;
    
    while(p1 != NULL && p2 != NULL)
    {
        if(p1->exponent == p2->exponent)
        {
            temp = CreateElement(p1->exponent, p1->coefficient + p2->coefficient);
            InsertSorted(S,temp);
            p1 = p1->next;
            p2 = p2->next;
        }

        else if(p1->exponent > p2->exponent)
        {
            temp = CreateElement(p1->exponent, p1->coefficient);
            InsertSorted(S,temp);
            p1 = p1->next;
        }

        else
        {
            temp = CreateElement(p2->exponent, p2->coefficient);
            InsertSorted(S,temp);
            p2 = p2->next;
        }
    }

    if(p1 != NULL) temp = p1;
    else temp = p2;
    while(temp != NULL)
    {
        InsertSorted(S,temp);
        temp = temp->next;
    }

    return EXIT_SUCCESS;
}

int Product(Position p1,Position p2,Position P)
{
    Position temp, head = p2;

    while(p1 != NULL)
    {
        p2 = head;
        while(p2 != NULL)
        {
            temp = CreateElement(p1->exponent + p2->exponent, p1->coefficient * p2->coefficient);
            InsertSorted(P,temp);
            p2 = p2->next;
        }

        p1 = p1->next;
    }

    return EXIT_SUCCESS;
}