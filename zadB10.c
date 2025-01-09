#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define NAME_SIZE 256
#define FILE_LINE 1024

struct Node;
typedef struct Node* Position;

struct Node
{
    char city_name[NAME_SIZE];
    int population;
    Position next;
};

struct TreeNode;
typedef struct TreeNode* Tree;

struct TreeNode
{
    char country_name[NAME_SIZE];
    Position next;
    Tree left;
    Tree right;
};

Tree ReadFile(Tree root);
Tree CreateTreeNode(char* country_name);
Tree InsertSortedTree(Tree root, char* country_name, char* country_file);
int CreateList(Tree root, char* country_file);
Position CreateListNode(char* city_name, int population);
Position InsertPopulationList(Position p, char* city_name, int population);
Position InsertCityNameList(Position p, char* city_name, int population);
void PrintTree(Tree root);
void PrintList(Position p);
void SearchTree(Tree root, char* country_name, int population);
int SearchList(Position p, int population);
void FreeTree(Tree root);
int FreeList(Position p);

int main()
{
    Tree root = NULL;

    root = ReadFile(root);

    puts("Countries and their cities: ");
    PrintTree(root);

    char country[NAME_SIZE] = "\0";
    int population = 0;

    puts("Enter country: ");
    scanf("%s", country);
    fflush(stdin);
    puts("Enter population: ");
    scanf("%d", &population);

    printf("Cities of %s that have a population larger than %d: \n", country, population);
    SearchTree(root,country,population);

    FreeTree(root);

    return 0;
}

Tree ReadFile(Tree root)
{
    FILE* fp = fopen("drzave.txt","r");

    if(fp == NULL)
    {
        perror("Could not open file!");
        exit(-1);
    }

    char buffer[FILE_LINE] = "\0";

    while(!feof(fp))
    {
        fgets(buffer,FILE_LINE,fp);
        buffer[strcspn(buffer,"\n")] = 0;

        char* country_name = strtok(buffer, " ");
        char* conutry_file = strtok(NULL, " ");

        root = InsertSortedTree(root,country_name,conutry_file);

        memset(buffer,'\0',FILE_LINE);
    }

    fclose(fp);

    return root;
}

Tree CreateTreeNode(char* country_name)
{
    Tree new_tree_node = (Tree )malloc(sizeof(struct TreeNode));

    if(new_tree_node != NULL)
    {
        strcpy(new_tree_node->country_name,country_name);
        new_tree_node->next = NULL;
        new_tree_node->left = NULL;
        new_tree_node->right = NULL;
    }

    return new_tree_node;
}

Tree InsertSortedTree(Tree root, char* country_name, char* country_file)
{
    if(root == NULL)
    {
        root = CreateTreeNode(country_name);
        CreateList(root,country_file);
    }

    else if(strcmp(country_name,root->country_name) < 0)
    {
        root->left = InsertSortedTree(root->left,country_name,country_file);
    }

    else if(strcmp(country_name,root->country_name) > 0)
    {
        root->right = InsertSortedTree(root->right,country_name,country_file);
    }

    return root;
}

int CreateList(Tree root, char* country_file)
{
    FILE* fp = fopen(country_file,"r");

    if(fp == NULL)
    {
        perror("Could not open country file!");
        return EXIT_FAILURE;
    }

    char buffer[FILE_LINE] = "\0";
    char city_name[NAME_SIZE] = "\0";
    int population = 0;

    while(!feof(fp))
    {
        fgets(buffer,FILE_LINE,fp);

        int check = 0;
        sscanf(buffer,"%s %d %n", city_name, &population, &check);

        if(check == 0)
        {
            puts("Corrupt file!");
            return EXIT_FAILURE;
        }

        root->next = InsertPopulationList(root->next,city_name,population);
        //root->next = InsertCityNameList(root->next,city_name,population);

        memset(buffer,'\0', FILE_LINE);
        memset(city_name,'\0', NAME_SIZE);
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

Position CreateListNode(char* city_name, int population)
{
    Position q = (Position )malloc(sizeof(struct Node));

    if(q != NULL)
    {
        strcpy(q->city_name,city_name);
        q->population = population;
    }

    return q;
}

Position InsertPopulationList(Position p, char* city_name, int population)
{
    Position q, head = p;
    
    if(p == NULL)
    {
        p = CreateListNode(city_name,population);
        return p;
    }

    else if(p != NULL && p->population < population)
    {
        q = CreateListNode(city_name,population);

        q->next = p;

        return q;
    }

    else if(p->next == NULL)
    {
        p->next = CreateListNode(city_name,population);

        return p;
    }

    else if(p->next != NULL && p->next->population < population)
    {
        q = CreateListNode(city_name,population);
        q->next = p->next;
        p->next = q;

        return head;
    }

    else 
    {
        while(p->next != NULL && p->next->population > population)
        {
            p = p->next;
        }

        q = CreateListNode(city_name,population);
        q->next = p->next;
        p->next = q;
    }

    return head;
}

Position InsertCityNameList(Position p, char* city_name, int population)
{
    Position q, head = p;
    
    if(p == NULL)
    {
        p = CreateListNode(city_name,population);
        return p;
    }

    else if(p != NULL && strcmp(p->city_name,city_name) > 0)
    {
        q = CreateListNode(city_name,population);

        q->next = p;

        return q;
    }

    else if(p->next == NULL)
    {
        p->next = CreateListNode(city_name,population);

        return p;
    }

    else if(p->next != NULL && strcmp(p->next->city_name,city_name) > 0)
    {
        q = CreateListNode(city_name,population);
        q->next = p->next;
        p->next = q;

        return head;
    }

    else 
    {
        while(p->next != NULL && strcmp(p->next->city_name,city_name) < 0)
        {
            p = p->next;
        }

        q = CreateListNode(city_name,population);
        q->next = p->next;
        p->next = q;
    }

    return head;
}

void PrintTree(Tree root)
{
    if(root != NULL)
    {
        PrintTree(root->left);
        PrintTree(root->right);
        printf("%s: \n", root->country_name);
        PrintList(root->next);
        puts(" ");
    }
    
}

void PrintList(Position p)
{
    while(p != NULL)
    {
        printf("-%s \n", p->city_name);
        p = p->next;
    }

}

void SearchTree(Tree root, char* country_name, int population)
{
    if(root != NULL)
    {
        SearchTree(root->left,country_name,population);
        if(strcmp(root->country_name,country_name) == 0)
        {
            SearchList(root->next,population);
            return;
        }
        SearchTree(root->right,country_name,population);
    }
    
}

int SearchList(Position p, int population)
{
    while(p != NULL)
    {
        if(p->population > population)
        {
            printf("-%s \n", p->city_name);
        }

        p = p->next;
    }

    return EXIT_SUCCESS;
}

void FreeTree(Tree root)
{
    if(root != NULL)
    {
        FreeTree(root->left);
        FreeTree(root->right);
        FreeList(root->next);
        free(root);
    }

}

int FreeList(Position p)
{
    Position temp;

    while(p != NULL)
    {
        temp = p;
        p = p->next;
        free(temp);
    }

    return EXIT_SUCCESS;
}