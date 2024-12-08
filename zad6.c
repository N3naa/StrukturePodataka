#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#define FILE_LINE 1024
#define NAME_SIZE 256

struct Articale;
typedef struct Articale* Position_Articale;

struct Articale
{
    char articale_name[NAME_SIZE];
    int amount;
    float price;
    Position_Articale next;
};

struct Receipt;
typedef struct Receipt* Position_Receipt;

struct Receipt
{
    char file_receipt[NAME_SIZE];
    char date[FILE_LINE];
    struct Articale Head;
    Position_Receipt next;
};

int ReadFile(Position_Receipt head_receipt);
int InsertSortedReceipt(Position_Receipt head_receipt, Position_Receipt new_receipt);
int InsertSortedArticale(Position_Articale head_articale, Position_Articale new_articale);
int InsertAfterReceipt(Position_Receipt current, Position_Receipt new_receipt);
int InsertAfterArticale(Position_Articale current, Position_Articale new_articale);
int CompareDates(char* first_date, char* second_date);
Position_Receipt CreateReceipt(char* file_name, char* date);
Position_Articale CreateArticale(char* articale_name, int amount, float price);
int CreateArticaleList(Position_Receipt new_receipt);
int MergeReceipts(Position_Receipt current, Position_Receipt new_receipt);
bool IsPresent(Position_Articale head_articale, Position_Articale articale);
int SearchInArticaleList(Position_Articale head_articale, char* item);
float FindPrice(Position_Articale head_articale, char* item);
int SearchItem(Position_Receipt head_receipt, char* item,char* first_date, char* second_date);
int DeleteReceipt(Position_Receipt head_receipt);
int DeleteArticaleList(Position_Articale head_articale);
bool InTimePeriod(char* receipt_date, char* first_date, char* second_date);

int main()
{
    struct Receipt Head_Receipt = { .file_receipt = " ", .date = " ", .next = NULL, .Head.articale_name = " ", .Head.amount = 0, .Head.price = 0.0, .Head.next = NULL};

    ReadFile(&Head_Receipt);

    puts("*****************************************************************************************************\n");
    puts("This program searches the receipts for an articale in a given time period (both provided by the user)\n and outputs the amount of said item sold, along with the total price.\nNote: by setting month and/or day as zero, the program will default to only using the relevent \n data provided!\n");
    puts("*****************************************************************************************************\n\n");

    char item[NAME_SIZE];
    char first_date[FILE_LINE];
    char second_date[FILE_LINE];

    puts("Enter an articale: ");
    scanf("%[^\n]", item);
    fflush(stdin);
    puts("Enter time period (format: YYYY-MM-D): ");
    scanf("%[^\n]", first_date);
    fflush(stdin);
    scanf("%[^\n]", second_date);

    SearchItem(&Head_Receipt,item,first_date,second_date);

    DeleteReceipt(&Head_Receipt);

    return 0;
}

int ReadFile(Position_Receipt head_receipt)
{
    FILE* fp = fopen("racuni.txt", "r");

    if(fp == NULL)
    {
        perror("Error opening file!");
        return EXIT_FAILURE;
    }

    char buffer[FILE_LINE];
    char file_name[NAME_SIZE];

    while(!feof(fp))
    {
        fgets(file_name,NAME_SIZE,fp);
        file_name[strcspn(file_name,"\n")] = 0;
        FILE* fp_receipt = fopen(file_name,"r");

        if(fp_receipt == NULL)
        {
            perror("Error opening receipt file!");
            fclose(fp);
            return EXIT_FAILURE;
        }

        fgets(buffer,FILE_LINE,fp_receipt);

        Position_Receipt new_receipt = CreateReceipt(file_name,buffer);

        if(new_receipt != NULL) InsertSortedReceipt(head_receipt,new_receipt);
        fclose(fp_receipt);
    }

    return EXIT_SUCCESS;
}

Position_Receipt CreateReceipt(char* file_name, char* date)
{
    Position_Receipt q = (Position_Receipt )malloc(sizeof(struct Receipt));

    if(q == NULL)
    {
        perror("Could not allocate new receipt!");
        return NULL;
    }

    strcpy(q->file_receipt,file_name);
    strcpy(q->date,date);

    return q;
}

int InsertAfterReceipt(Position_Receipt current, Position_Receipt new_receipt)
{
    new_receipt->next = current->next;
    current->next = new_receipt;

    return EXIT_SUCCESS;
}

int InsertSortedReceipt(Position_Receipt head_receipt, Position_Receipt new_receipt)
{
    if(head_receipt->next == NULL)
    {
        InsertAfterReceipt(head_receipt,new_receipt);
        CreateArticaleList(new_receipt);
        return EXIT_SUCCESS;
    }

    else
    {
        Position_Receipt current = head_receipt;

        while(current->next != NULL && CompareDates(current->next->date,new_receipt->date) == 1)
        {
            current = current->next;
        }

        if(current->next == NULL || CompareDates(current->next->date,new_receipt->date) == -1)
        {
            InsertAfterReceipt(current,new_receipt);
            CreateArticaleList(new_receipt);
            return EXIT_SUCCESS;
        }

        else
        {
            CreateArticaleList(new_receipt);
            MergeReceipts(current,new_receipt);
            DeleteReceipt(new_receipt);
        }
    }

    return EXIT_SUCCESS;
}

int CreateArticaleList(Position_Receipt new_receipt)
{
    FILE* fp_receipt = fopen(new_receipt->file_receipt,"r");

    if(fp_receipt == NULL)
    {
        perror("Error opeing receipt file!");
        return EXIT_FAILURE;
    }

    char buffer[FILE_LINE];
    char articale_name[NAME_SIZE];
    int amount;
    float price;
    int check = 0;

    memset(new_receipt->Head.articale_name,'\0',NAME_SIZE);
    new_receipt->Head.amount = 0;
    new_receipt->Head.price = 0.0;

    fgets(buffer,FILE_LINE,fp_receipt);

    while(!feof(fp_receipt))
    {
        fgets(buffer,FILE_LINE,fp_receipt);
        sscanf(buffer,"%s %d %f %n", articale_name, &amount, &price, &check);

        if(check == 0)
        {
            puts("Faulty receipt!");
            break;
        }

        Position_Articale new_articale = CreateArticale(articale_name,amount,price);

        if(new_articale != NULL) InsertSortedArticale(&(new_receipt->Head),new_articale);
        check = 0;
    }

    fclose(fp_receipt);

    return EXIT_SUCCESS;
}

Position_Articale CreateArticale(char* articale_name, int amount, float price)
{
    Position_Articale new_articale = (Position_Articale )malloc(sizeof(struct Articale));

    if(new_articale == NULL)
    {
        perror("Could not allocate new articale!");
        return NULL;
    }

    strcpy(new_articale->articale_name,articale_name);
    new_articale->amount = amount;
    new_articale->price = price;

    return new_articale;
}

int InsertSortedArticale(Position_Articale head_articale, Position_Articale new_articale)
{
    if(head_articale->next == NULL)
    {
        InsertAfterArticale(head_articale,new_articale);
        return EXIT_SUCCESS;
    }

    else if(head_articale->next != NULL && strcmp(head_articale->next->articale_name,new_articale->articale_name)<0)
    {
        InsertAfterArticale(head_articale,new_articale);
        return EXIT_SUCCESS;
    }

    Position_Articale current = head_articale;
    while(current->next != NULL && strcmp(current->next->articale_name,new_articale->articale_name)>0)
    {
        current = current->next;
    }

    if(current->next != NULL && strcmp(current->next->articale_name,new_articale->articale_name) != 0)
    {
        InsertAfterArticale(current,new_articale);
    }

    else if(current->next == NULL)
    {
        InsertAfterArticale(current,new_articale);
    }

    else
    {
        current->next->amount += new_articale->amount;
        free(new_articale);
    }

    return EXIT_SUCCESS;
}

int InsertAfterArticale(Position_Articale current, Position_Articale new_articale)
{
    new_articale->next = current->next;
    current->next = new_articale;

    return EXIT_SUCCESS;
}

int CompareDates(char* first_date, char* second_date)
{
    int first_year, first_month, first_day;
    int second_year, second_month, second_day;

    sscanf(first_date,"%d-%d-%d ", &first_year, &first_month, &first_day);
    sscanf(second_date,"%d-%d-%d ", &second_year, &second_month, &second_day);

    if(first_year > second_year)
    {
        return 1;
    }

    else if(first_year < second_year)
    {
        return -1;
    }

    else
    {
        if(first_month > second_month)
        {
            return 1;
        }

        else if(first_month < second_month)
        {
            return -1;
        }

        else
        {
            if(first_day > second_day)
            {
                return 1;
            }   

            else if(first_day < second_day)
            {
                return -1;
            }
        }
    }

    return 0;
}

int MergeReceipts(Position_Receipt current, Position_Receipt new_receipt)
{
    Position_Articale temp, dummy = &(new_receipt->Head);

    while(dummy->next != NULL)
    {
        if(!IsPresent(&(current->next->Head),dummy->next))
        {
            temp = CreateArticale(dummy->next->articale_name,dummy->next->amount,dummy->next->price);
            
            InsertSortedArticale(&(current->next->Head),temp);
        }

        dummy = dummy->next;
    }

    return EXIT_SUCCESS;
}

bool IsPresent(Position_Articale head_articale, Position_Articale articale)
{
    while(head_articale->next != NULL)
    {
        if(strcmp(head_articale->next->articale_name,articale->articale_name) == 0)
        {
            return true;
        }

        head_articale = head_articale->next;
    }

    return false;
}

int SearchItem(Position_Receipt head_receipt, char* item, char* first_date, char* second_date)
{
    int amount = 0;
    float price;

    while(head_receipt->next != NULL)
    {
        if(InTimePeriod(head_receipt->next->date,first_date,second_date))
        {
            if(amount == 0) price = FindPrice(&(head_receipt->next->Head),item);
            amount += SearchInArticaleList(&(head_receipt->next->Head),item);
        }

        head_receipt = head_receipt->next;
    }

    printf("%s ,between the time period %s to %s, was sold %d times.\n", item, first_date, second_date, amount);

    printf("The total is: %.2f", amount*price);

    return EXIT_SUCCESS;
}

int SearchInArticaleList(Position_Articale head_articale, char* item)
{
    int count = 0;
    
    while(head_articale->next != NULL)
    {
        if(strcmp(head_articale->next->articale_name,item) == 0)
        {
            count = head_articale->next->amount;
            break;
        }

        head_articale = head_articale->next;
    }

    return count;
}

float FindPrice(Position_Articale head_articale, char* item)
{
    float price = 0.0;

    while(head_articale->next != NULL)
    {
        if(strcmp(head_articale->next->articale_name,item) == 0)
        {
            price = head_articale->next->price;
            break;
        }

        head_articale = head_articale->next;
    }

    return price;
}

int DeleteReceipt(Position_Receipt head_receipt)
{
    Position_Receipt temp;

    while(head_receipt->next != NULL)
    {
        DeleteArticaleList(&(head_receipt->next->Head));
        temp = head_receipt->next;
        head_receipt->next = head_receipt->next->next;
        free(temp);
    }

    return EXIT_SUCCESS;
}

int DeleteArticaleList(Position_Articale head_articale)
{
    Position_Articale temp;

    while(head_articale->next != NULL)
    {
        temp = head_articale->next;
        head_articale->next = head_articale->next->next;
        free(temp);
    }

    return EXIT_SUCCESS;
}

bool InTimePeriod(char* receipt_date, char* first_date, char* second_date)
{
    int receipt_year, receipt_month, receipt_day;
    int first_year, first_month, first_day;
    int second_year, second_month, second_day;

    sscanf(receipt_date,"%d-%d-%d ", &receipt_year, &receipt_month, &receipt_day);
    sscanf(first_date,"%d-%d-%d ", &first_year, &first_month, &first_day);
    sscanf(second_date,"%d-%d-%d ", &second_year, &second_month, &second_day);

    if(receipt_year >= first_year && receipt_year <= second_year)
    {
        if(first_month == 0 && second_month == 0)
        {
            return true;
        }

        else
        {
            if(receipt_month >= first_month && receipt_month <= second_month )
            {
                if(first_day == 0 && second_day == 0)
                {
                    return true;
                }

                else
                {
                    if(receipt_day >= first_day && receipt_day <= second_day)
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}