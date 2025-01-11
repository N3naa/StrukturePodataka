#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

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
    int key;
    Tree root;
    Position next;
};

struct HashTable;
typedef struct HashTable* HashTab;

struct HashTable
{
    int size;
    Position* HashList;
};

HashTab InitializeTable(int size);
void FreeHashTable(HashTab hash_table);
int FreeList(Position p);
void FreeTree(Tree root);
void Add(int key, char* country_name, char* country_file, HashTab hash_table);
int NextPrime(int size);
bool IsPrime(int x);
int HashFunction(char* country_name, int size);
int ReadFile(HashTab hash_table);
int InsertSorted(int key, char* country_name, char* country_file, HashTab hash_table);
int CreateTree(Position p, char* country_file);
Tree CreateTreeNode(char* city_name, int population);
Tree InsertPopulationTree(Tree root, char* city_name, int population);
Tree InsertCityNameTree(Tree root, char* city_name, int population);
void PrintHashTable(HashTab hash_table);
void PrintList(Position p);
void PrintTree(Tree root);
int SearchHashTable(HashTab hash_table, char* country_name, int population);
int SearchList(Position p, char* country_name, int population);
void SearchTree(Tree root, int population);

int main()
{
    HashTab hashTable = InitializeTable(11);

    ReadFile(hashTable);

    puts("Countries and their cities: ");
    PrintHashTable(hashTable);

    char country[NAME_SIZE] = "\0";
    int population = 0;

    puts("Enter country: ");
    scanf("%s", country);
    fflush(stdin);
    puts("Enter population: ");
    scanf("%d", &population);

    printf("Cities of %s that have a population larger than %d: \n", country, population);
    SearchHashTable(hashTable,country,population);
    FreeHashTable(hashTable);

    return 0;
}

HashTab InitializeTable(int size)
{
    HashTab hash_table;

    hash_table = (HashTab )malloc(sizeof(struct HashTable));

    if(hash_table == NULL)
    {
        puts("Could not allocate hash table!");
        return NULL;
    }

    hash_table->size = NextPrime(size);

    hash_table->HashList = (Position *)malloc(sizeof(Position )*hash_table->size);

    if(hash_table->HashList == NULL)
    {
        puts("Could not alloate hash list!");
        free(hash_table);
        return NULL;
    }

    for(int i = 0; i < hash_table->size; i++)
    {
        hash_table->HashList[i] = NULL;
    }

    return hash_table;
}

void Add(int key, char* country_name, char* country_file, HashTab hash_table)
{
    Position new_node = (Position )malloc(sizeof(struct Node));

    if(new_node == NULL)
    {
        puts("Could not allocate new node!");
        exit(-1);
    }

    new_node->key = key;
    strcpy(new_node->country_name,country_name);
    new_node->next = hash_table->HashList[HashFunction(country_name,hash_table->size)];
    hash_table->HashList[HashFunction(country_name,hash_table->size)] = new_node;

    CreateTree(new_node,country_file);
}

int NextPrime(int size)
{
    if(!IsPrime(size))
    {
        for(int i = 1; i < size; i++)
        {
            size += i;

            if(IsPrime(size))
            {
                break;
            }
        }
    }

    return size;
}

bool IsPrime(int x)
{
    bool check = true;
    
    for(int i = 2; i <= x/2; i++)
    {
        if(x % i == 0)
        {
            check = false;
            break;
        }
    }

    return check;
}

int HashFunction(char* country_name, int size)
{
    return (country_name[0] + country_name[1] + country_name[2] + country_name[3] + country_name[4])%size;
}

int ReadFile(HashTab hash_table)
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
        char* country_file = strtok(NULL, " ");

        int key = HashFunction(country_name,hash_table->size);

        InsertSorted(key,country_name,country_file,hash_table);

        memset(buffer,'\0',FILE_LINE);
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

int InsertSorted(int key, char* country_name, char* country_file, HashTab hash_table)
{
    if(*(hash_table->HashList) == NULL)
    {
        Add(key,country_name,country_file,hash_table);
        return EXIT_SUCCESS;
    }

    else if(*(hash_table->HashList) != NULL && strcmp((*hash_table->HashList)->country_name,country_name) > 0)
    {
        Add(key,country_name,country_file,hash_table);
        return EXIT_SUCCESS;
    }

    else 
    {
        int i = 0;
        while(i < hash_table->size && strcmp((*hash_table->HashList)->country_name,country_name) < 0)
        {
            i++;
            hash_table->HashList += i;
        }

        Add(key,country_name,country_file,hash_table);
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

        p->root = InsertPopulationTree(p->root,city_name,population);
        //p->root = InsertCityNameTree(p->root,city_name,population);

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

void PrintHashTable(HashTab hash_table)
{
    for(int i = 0; i < hash_table->size; i++)
    {
        PrintList(hash_table->HashList[i]);
    }

}

void PrintList(Position p)
{
    while(p != NULL)
    {
        printf("%s: \n", p->country_name);
        PrintTree(p->root);
        puts(" ");
        p = p->next;
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

int SearchHashTable(HashTab hash_table, char* country_name, int population)
{
    for(int i = 0; i < hash_table->size; i++)
    {
        SearchList(hash_table->HashList[i],country_name,population);
    }

    return EXIT_SUCCESS;
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

void FreeHashTable(HashTab hash_table)
{
    for(int i = 0; i < hash_table->size; i++)
    {
        FreeList(hash_table->HashList[i]);
    }

    free(hash_table);
}

int FreeList(Position p)
{
    Position temp;

    while(p != NULL)
    {
        temp = p;
        p = p->next;
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