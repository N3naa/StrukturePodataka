#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define NAME_SIZE 256
#define FILE_LINE 1024

struct TreeNode;
typedef struct TreeNode* Tree;

struct TreeNode
{
    char city_name[NAME_SIZE];
    int population;
    Tree left;
    Tree right;
};

struct Node;
typedef struct Node* Position;

struct Node
{
    char country_name[NAME_SIZE];
    Tree root;
    Position next;
};

int ReadFile(Position p);
int CreateNodeList(Position p, char* counrty_file);
int InsertSortedList(Position p, char* country_name, char* country_file);
int CreateTree(Position p, char* country_file);
Tree CreateTreeNode(char* city_name, int population);
Tree InsertPopulationTree(Tree root, char* city_name, int population);
Tree InsertCityNameTree(Tree root, char* city_name, int population);
void PrintList(Position p);
void PrintTree(Tree root);
int SearchList(Position p, char* country_name, int population);
void SearchTree(Tree root, int population);
int FreeList(Position p);
void FreeTree(Tree root);

int main()
{
    struct Node Head = { .country_name = " ", .next = NULL};

    ReadFile(&Head);

    puts("Countries and their cities: ");
    PrintList(Head.next);

    char country[NAME_SIZE] = "\0";
    int population = 0;

    puts("Enter country: ");
    scanf("%s", country);
    fflush(stdin);
    puts("Enter population: ");
    scanf("%d", &population);

    printf("Cities of %s that have a population larger than %d: \n", country, population);
    SearchList(Head.next,country,population);

    FreeList(&Head);

    return 0;
}

int ReadFile(Position head)
{
    FILE* fp = fopen("drzave.txt","r");

    if(fp == NULL)
    {
        perror("Could not open file!");
        return EXIT_FAILURE;
    }

    char buffer[FILE_LINE] = "\0";

    while(!feof(fp))
    {
        fgets(buffer,FILE_LINE,fp);
        buffer[strcspn(buffer,"\n")] = 0;

        char* country_name = strtok(buffer, " ");
        char* conutry_file = strtok(NULL, " ");

        InsertSortedList(head,country_name,conutry_file);

        memset(buffer,'\0',FILE_LINE);
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

int CreateNodeList(Position p, char* counrty_name)
{
    Position q = (Position )malloc(sizeof(struct Node));

    if(q == NULL)
    {
        perror("Could not allocate node of a list!");
        return EXIT_FAILURE;
    }

    strcpy(q->country_name, counrty_name);
    q->root = NULL;
    q->next = p->next;
    p->next = q;

    return EXIT_SUCCESS;
}

int InsertSortedList(Position p, char* country_name, char* country_file)
{
    if(p->next == NULL)
    {
        CreateNodeList(p,country_name);
        CreateTree(p,country_file);
        return EXIT_SUCCESS;
    }

    else if(p->next != NULL && strcmp(p->next->country_name, country_name) > 0)
    {
        CreateNodeList(p,country_name);
        CreateTree(p,country_file);
        return EXIT_SUCCESS;
    }

    else
    {
        while(p->next != NULL && strcmp(p->next->country_name, country_name) < 0)
        {
            p = p->next;
        }

        CreateNodeList(p,country_name);
        CreateTree(p,country_file);
    }

    return EXIT_SUCCESS;
}

int CreateTree(Position p, char* country_file)
{
    FILE* fp = fopen(country_file, "r");

    if(fp == NULL)
    {
        perror("Could not open conutry file!");
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

        p->next->root = InsertPopulationTree(p->next->root,city_name,population);
        //p->next->root = InsertCityNameTree(p->next->root,city_name,population);

        memset(buffer,'\0', FILE_LINE);
        memset(city_name,'\0', NAME_SIZE);
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

Tree CreateTreeNode(char* city_name, int population)
{
    Tree new_tree_node = (Tree )malloc(sizeof(struct TreeNode));

    if(new_tree_node != NULL)
    {
        strcpy(new_tree_node->city_name,city_name);
        new_tree_node->population = population;
        new_tree_node->left = NULL;
        new_tree_node->right = NULL;
    }

    return new_tree_node;
}

Tree InsertPopulationTree(Tree root, char* city_name, int population)
{
    if(root == NULL)
    {
        root = CreateTreeNode(city_name,population);
    }

    else if(population < root->population)
    {
        root->left = InsertPopulationTree(root->left,city_name,population);
    }

    else if(population > root->population)
    {
        root->right = InsertPopulationTree(root->right,city_name,population);
    }

    return root;
}

Tree InsertCityNameTree(Tree root, char* city_name, int population)
{
    if(root == NULL)
    {
        root = CreateTreeNode(city_name,population);
    }

    else if(strcmp(city_name,root->city_name) < 0)
    {
        root->left = InsertCityNameTree(root->left,city_name,population);
    }

    else if(strcmp(city_name,root->city_name) > 0)
    {
        root->right = InsertCityNameTree(root->right,city_name,population);
    }

    return root;
}

void PrintList(Position p)
{
    while(p != NULL)
    {
        printf("%s: \n", p->country_name);
        PrintTree(p->root);
        p = p->next;
        puts(" ");
    }

}

void PrintTree(Tree root)
{
    if(root != NULL)
    {
        printf("- %s \n", root->city_name);
        PrintTree(root->left);
        PrintTree(root->right);
    }

}

int SearchList(Position p,char* country_name, int population)
{
    while(p != NULL)
    {
        if(strcmp(p->country_name,country_name) == 0)
        {
            SearchTree(p->root,population);
            return EXIT_SUCCESS;
        }

        p = p->next;
    }

    printf("%s is not in countries list!\n", country_name);
    return EXIT_FAILURE;
}

void SearchTree(Tree root, int population)
{
    if(root == NULL)
    {
        return ;
    }

    if(population < root->population)
    {
        printf("-%s \n", root->city_name);
    }

    SearchTree(root->right,population);

    SearchTree(root->left,population);

}

int FreeList(Position p)
{
    Position temp;

    while(p->next != NULL)
    {
        temp = p->next;
        p->next = p->next->next;
        FreeTree(temp->root);
        free(temp);
    }

    return EXIT_SUCCESS;
}

void FreeTree(Tree root)
{
    if(root != NULL)
    {
        FreeTree(root->left);
        FreeTree(root->right);
        free(root);
    }

}