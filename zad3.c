//#include _CTR_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_SIZE 256

typedef struct _person* position;

typedef struct _person
{
    char fname[NAME_SIZE];
    char lname[NAME_SIZE];
    int birth_year;
    position next;
}_person;

void print_list(position current);
int add(char* fname,char* lname,int birth_year,position person);
int add_end(char* fname,char* lname,int birth_year,position person);
position find_person(char* lname,position person);
position find_before(char* lname,position person);
int delete_person(char* lname,position person);
int append_after(char* fname,char* lname,int birth_year,position person,char* search);
int append_before(char* fname,char* lname,int birth_year,position person,char* search);
int sort_people(position person);
int write_file(position person);
int read_file(position person);
int delete_all(position person);

int main()
{
    char fname[NAME_SIZE];
    char lname[NAME_SIZE];
    int birth_year;
    int n_beg, n_end;

    puts("How many people would you like to add at the beginning?\n");
    scanf("%d", &n_beg);
    fflush(stdin);

    _person head = {.fname = " ", .lname = " ", .birth_year = 0};
    head.next = NULL;

    for(int i = 0; i < n_beg; i++)
    {
        printf("Enter first and last name of %d.person: \n", i+1);
        scanf("%s %s", fname, lname);
        printf("Enter birth year: \n");
        scanf("%d", &birth_year);

        add(fname, lname, birth_year,&head);
    }

    print_list(head.next);

    puts("How many people would you like to add at the end?\n");
    scanf("%d", &n_end);
    fflush(stdin);

    for(int i = 0; i < n_end; i++)
    {
        printf("Enter first and last name of %d.person: \n", i+1);
        scanf("%s %s", fname, lname);
        printf("Enter birth year: \n");
        scanf("%d", &birth_year);

        add_end(fname, lname, birth_year,&head);
    }
    fflush(stdin);

    print_list(head.next);

    puts("Enter the last name you want to search: ");
    scanf("%[^\n]", lname);
    fflush(stdin);

    if(find_person(lname,&head) != NULL) printf("The person has been found!\n");
    else printf("The person with the last name: %s has not been found!\n", lname);

    puts("Enter the last name of a person that you would like to delete: \n");
    scanf("%[^\n]", lname);
    fflush(stdin);

    delete_person(lname,&head);

    print_list(head.next);

    char search[NAME_SIZE];
    puts("Enter the last name of a person that you want to add before it: \n");
    scanf("%[^\n]", search);
    fflush(stdin);
    printf("Enter first and last name of the new person: \n");
    scanf("%s %s", fname, lname);
    printf("Enter birth year: \n");
    scanf("%d", &birth_year);
    fflush(stdin);

    append_before(fname,lname,birth_year,&head,search);

    print_list(&head);
    
    puts("Enter the last name of a person that you want to add after it: \n");
    scanf("%[^\n]", search);
    fflush(stdin);
    printf("Enter first and last name of the new person: \n");
    scanf("%s %s", fname, lname);
    printf("Enter birth year: \n");
    scanf("%d", &birth_year);

    append_after(fname,lname,birth_year,&head,search);

    print_list(head.next);

    sort_people(&head);

    puts("Writing sorted list of people to the file: people.txt ...\n");
    write_file(head.next);

    puts("Reading from a file...\n");
    _person head_file;
    position current = &head_file;
    head_file.next = NULL;
    read_file(&head_file);
    puts("Contents of the file: \n");
    while(current != NULL)
    {
        printf("%s %s - %d\n", current->fname, current->lname, current->birth_year);
        current = current->next;
    }
    free(current);

    delete_all(head.next);
    delete_all(head_file.next);

    return 0;
}

void print_list(position current)
{
    while(current != NULL)
    {
        printf("%s %s - %d\n", current->fname, current->lname, current->birth_year);
        current = current->next;
    }
}

int add(char* fname,char* lname,int birth_year,position person)
{
    position new_person = (position)malloc(sizeof(_person));

    if(new_person == NULL)
    {
        printf("Could not add!\n");
        return EXIT_FAILURE;
    }

    strcpy(new_person->fname, fname);
    strcpy(new_person->lname, lname);
    new_person->birth_year = birth_year;

    new_person->next = person->next;
    person->next = new_person;

    return EXIT_SUCCESS;
}
int add_end(char* fname,char* lname,int birth_year,position person)
{
    position new_person = (position)malloc(sizeof(_person));

    if(new_person == NULL)
    {
        printf("Could not add!\n");
        return EXIT_FAILURE;
    }

    strcpy(new_person->fname, fname);
    strcpy(new_person->lname, lname);
    new_person->birth_year = birth_year;

    while(person->next != NULL)
    {
        person = person->next;
    }

    new_person->next = person->next;
    person->next = new_person;

    return EXIT_SUCCESS;
}

position find_person(char* lname,position person)
{
    while(person != NULL && (strcmp(person->lname,lname)))
    {
        person = person->next;
    }

    printf("%s %s!", person->fname, person->lname);

    return person;
}

position find_before(char* lname,position person)
{
    while(person->next != NULL && (strcmp(person->next->lname,lname) != 0))
    {
        person = person->next;
    }

    if(person->next != NULL) return person;
    else return NULL;
}

int delete_person(char* lname,position person)
{
    position temp;

    //person = find_before(lname,person);

    while(person->next != NULL && (strcmp(person->next->lname,lname)))
    {
        person = person->next;
    }

    if(person == NULL)
    {
        printf("The given last name is not in the list!\n");
        return EXIT_FAILURE;
    }

    temp = person->next;
    person->next = person->next->next;
    free(temp);

    return EXIT_SUCCESS;
}

int append_before(char* fname,char* lname,int birth_year,position person,char* search)
{
    position new_person = (position)malloc(sizeof(_person));

    if(new_person == NULL)
    {
        printf("Could not append!\n");
        return EXIT_FAILURE;
    }

    //person = find_person(search,person->next);

    while(person->next != NULL && (strcmp(person->next->lname,search)!=0))
    {
        person = person->next;
    }

    if(person->next == NULL)
    {
       printf("The given last name is not in the list!\n"); 
        return EXIT_FAILURE;
    }
    
    strcpy(new_person->fname, fname);
    strcpy(new_person->lname, lname);
    new_person->birth_year = birth_year;

    new_person->next = person->next;
    person->next = new_person;

    return EXIT_SUCCESS;
}

int append_after(char* fname,char* lname,int birth_year, position person,char* search)
{
    position new_person = (position)malloc(sizeof(_person));

    if(new_person == NULL)
    {
        printf("Could not append!\n");
        return EXIT_FAILURE;
    }

    //person = find_before(search,person);

    while(person != NULL && (strcmp(person->lname,search)!=0))
    {
        person = person->next;
    }

    if(person == NULL)
    {
       printf("The given last name is not in the list!\n"); 
        return EXIT_FAILURE;
    }

    strcpy(new_person->fname, fname);
    strcpy(new_person->lname, lname);
    new_person->birth_year = birth_year;

    new_person->next = person->next;
    person->next = new_person; 

    return EXIT_SUCCESS;
}

int sort_people(position person)
{
    position current, temp;
    
    while(person != NULL)
    {
        current = person;

        while(current->next != NULL)
        {
            if(strcmp(current->next->lname,current->lname)<0)
            {
                temp = current;
                current = current->next;
                current->next = temp;
            }
            current = current->next;
        }
        person = person->next;
    }
    return EXIT_SUCCESS;
}

int write_file(position person)
{
    FILE* fp = fopen("people.txt","w");

    if(fp == NULL)
    {
        printf("Error opening file!\n");
        return EXIT_FAILURE;
    }

    while(person != NULL)
    {
        fprintf(fp,"%s\t%s\t%d\n", person->fname, person->lname, person->birth_year);
        person = person->next;
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

int read_file(position person)
{
    FILE* fp = fopen("people.txt", "r");
    char fname[NAME_SIZE];
    char lname[NAME_SIZE];
    int birth_day;
    
    if(fp == NULL)
    {
        printf("Error opening file!\n");
        return EXIT_FAILURE;
    }

    while(!feof(fp))
    {
        fscanf(fp,"%s   %s  %d", fname, lname, &birth_day);
        add_end(fname,lname,birth_day,person);
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

int delete_all(position person)
{
    position temp;

    puts("\nDeleting the linked list...\n");
    
    while(person != NULL)
    {
        temp = person->next;
        free(person);
        person = temp;
    }

    return EXIT_SUCCESS;
}