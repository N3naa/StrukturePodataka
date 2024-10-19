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
int add_beginning(char* fname,char* lname,int birth_year,position person);
int add_end(char* fname,char* lname,int birth_year,position person);
position find_person(char* lname,position person);
position find_before(char* lname,position person);
int delete_person(char* lname,position person);

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

        add_beginning(fname, lname, birth_year,&head);
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
    position p = find_person(lname,&head);
    if(p != NULL) printf("The person %s %s has been found!\n", p->fname, p->lname);
    else printf("The person with the last name: %s has not been found!\n", lname);

    puts("Enter the last name of a person that you would like to delete: \n");
    scanf("%[^\n]", lname);
    fflush(stdin);

    delete_person(lname,&head);

    print_list(head.next);

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

int add_beginning(char* fname,char* lname,int birth_year,position person)
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

    return person;
}

/*position find_before(char* lname,position person)
{
    while(person->next != NULL && (strcmp(person->next->lname,lname) != 0))
    {
        person = person->next;
    }

    if(person->next != NULL) return person;
    else return NULL;
}*/

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