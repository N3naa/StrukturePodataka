#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

struct TreeNode;
typedef struct TreeNode* Tree;
typedef struct TreeNdoe* Position;

struct TreeNode
{
    int value;
    Tree left;
    Tree right;
};

Tree CreateTreeNode(int value);
Tree Insert(Tree root, int value);
int Replace(Tree root);
void PrintInOrder(Tree root);
void FreeTree(Tree root);
void WriteFile(FILE* fp, Tree root);

//This can also be solved using a queue

int main()
{
    int array[] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7};
    
    Tree root = NULL;

    for(int i = 0; i < 10; i++)
    {
        root = Insert(root,array[i]);
    }

    Replace(root);

    printf("InOrder of the predetermined numbers: \n");
    PrintInOrder(root);

    FreeTree(root);

    Tree rand_root = NULL;

    srand(time(NULL));

    for(int i = 0; i < 10; i++)
    {
        int value = rand()%(90 - 10 + 1) + 10;
        rand_root = Insert(rand_root,value);
    }

    FILE* fp = fopen("inorder.txt", "w");

    if(fp == NULL)
    {
        perror("Could not open file!");
        return EXIT_FAILURE;
    }

    WriteFile(fp,rand_root);
    fclose(fp);

    Replace(root);

    fp = fopen("inorder.txt", "a");

    if(fp == NULL)
    {
        perror("Could not open file!");
        return EXIT_FAILURE;
    }

    WriteFile(fp,rand_root);
    fclose(fp);

    return 0;
}

Tree CreateTreeNode(int value)
{
    Tree new_tree_node = (Tree )malloc(sizeof(struct TreeNode));

    if(new_tree_node != NULL)
    {
        new_tree_node->value = value;
        new_tree_node->left = NULL;
        new_tree_node->right = NULL;
    }

    return new_tree_node;
}

Tree Insert(Tree root, int value)
{
    if(root == NULL)
    {
        root = CreateTreeNode(value);
    }

    if(value > root->value)
    {
        root->left = Insert(root->left, value);
    }

    if(value < root->value)
    {
        root->right = Insert(root->right, value);
    }

    return root;
}

int Replace(Tree root)
{
    if(root == NULL)
    {
        return 0;
    }

    int prev_value = root->value;

    root->value = Replace(root->left) + Replace(root->right);

    return prev_value + root->value;
}

void PrintInOrder(Tree root)
{
    if(root != NULL)
    {
        PrintInOrder(root->left);
        printf("%d\n", root->value);
        PrintInOrder(root->right);
    }

}

void FreeTree(Tree root)
{
    if(root != NULL)
    {
        FreeTree(root->left);
        FreeTree(root->right);
        free(root);
    }

    //Dangling pointer
    root = NULL;
}

void WriteFile(FILE* fp, Tree root)
{
    if(root == NULL)
    {
        fprintf(fp, "\n");
        return;
    }

    WriteFile(fp, root->left);
    fprintf(fp,"%d \n", root->value);
    WriteFile(fp, root->right);

    fputs(" ", fp);
}