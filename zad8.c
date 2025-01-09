#include <stdio.h>
#include <stdlib.h>

struct TreeNode;
typedef struct TreeNode* Tree;
typedef struct TreeNode* Position;

struct TreeNode
{
    int value;
    Tree left;
    Tree right;
};

struct Queue
{
    Tree* array;
    int front;
    int rear;
    int size;
    unsigned capacity;
};

Tree CreateTree(int value);
Tree Insert(Tree root, int value);
Tree Add(Tree root, int value);
Position FindMin(Tree root);
Tree Delete(Tree root, int value);
Position Search(Tree root, int value);
void PrintInOrder(Tree root);
void PrintPostOrder(Tree root);
void PrintPreOrder(Tree root);
struct Queue* CreateQueue(unsigned capacity);
void Enqueue(struct Queue* queue, Tree root);
Tree Dequeue(struct Queue* queue);
void PrintLevelOrder(Tree root);
int FreeTree(Tree root);

int main()
{
    Tree root = NULL;

    root = Insert(root,7);
    root = Insert(root,4);
    root = Insert(root,11);
    root = Insert(root,3);
    root = Insert(root,29);
    root= Insert(root,16);
    root = Insert(root,6);

    root = Add(root,18);

    root = Delete(root,3);

    if(root != NULL) printf("The %d element exist in Binary Tree!\n", root->value);

    puts("InOrder: ");
    PrintInOrder(root);
    puts("PostOrder: ");
    PrintPostOrder(root);
    puts("PreOrder: ");
    PrintPreOrder(root);
    puts("LevelOrder: ");
    PrintLevelOrder(root);

    FreeTree(root);

    return 0;
}

Tree CreateTree(int value)
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
        root = CreateTree(value);
    }

    else if(value < root->value)
    {
        root->left = Insert(root->left, value);
    }

    else if(value > root->value)
    {
        root->right = Insert(root->right, value);
    }

    return root;
}

Tree Add(Tree root, int value)
{   
    if(root == NULL)
    {
        return CreateTree(value);
    }

    else if(value < root->value)
    {
        root->left = Add(root->left, value);
    }

    else if(value > root->value)
    {
        root->right = Add(root->right, value);
    }

    return root;
}

Position FindMin(Tree root)
{
    if(root == NULL)
    {
        return NULL;
    }

    else if(root->left == NULL)
    {
        return root;
    }

    else return FindMin(root->left);
}

Tree Delete(Tree root, int value)
{
    if(root == NULL)
    {
        printf("The %d element does't exist!\n", value);
        return NULL;
    }

    else if(value < root->value)
    {
        root->left = Delete(root->left, value);
    }

    else if(value > root->value)
    {
        root->right = Delete(root->right, value);
    }

    else if(root->left != NULL && root->right != NULL)
    {
        Position temp = FindMin(root);
        root->value = temp->value;
        root->right = Delete(root->right, root->value);
    }

    else
    {
        Position temp = root;

        if(root->left == NULL)
        {
            root = root->right;
        }

        else
        {
            root = root->left;
        }

        free(temp);
    }

    return root;
}

Position Search(Tree root, int value)
{
    if(root == NULL)
    {
        return NULL;
    }

    else if(value == root->value)
    {
        return root;
    }

    else if(value < root->value)
    {
        return Search(root->left, value);
    }

    else
    {
        return Search(root->right, value);
    }

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

void PrintPostOrder(Tree root)
{
    if(root != NULL)
    {
        PrintPostOrder(root->left);
        PrintPostOrder(root->right);
        printf("%d\n", root->value);
    }

}

void PrintPreOrder(Tree root)
{
    if(root != NULL)
    {
        printf("%d\n", root->value);
        PrintPreOrder(root->left);
        PrintPreOrder(root->right);
    }

}

struct Queue* CreateQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue* )malloc(sizeof(struct Queue));

    if(queue != NULL)
    {
        queue->capacity = capacity;
        queue->front = 0;
        queue->size = 0;
        queue->rear = capacity - 1;
        queue->array = (Tree* )malloc(queue->capacity*sizeof(struct TreeNode));
    }

    return queue;
}

void Enqueue(struct Queue* queue, Tree root)
{
    if(queue->size == queue->capacity)
    {
        return;
    }

    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = root;
    queue->size++;
}

Tree Dequeue(struct Queue* queue)
{
    if(queue->size == 0)
    {
        return NULL;
    }

    Tree root = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return root;
}

void PrintLevelOrder(Tree root)
{
   if(root == NULL)
   {
        return;
   }

   struct Queue* queue = CreateQueue(100);
    Enqueue(queue,root);

    while(queue->size != 0)
    {
        Tree temp = Dequeue(queue);
        printf("%d\n", temp->value);

        if(temp->left != NULL)
        {
            Enqueue(queue, temp->left);
        }

        if(temp->right != NULL)
        {
            Enqueue(queue, temp->right);
        }
    }

    free(queue->array);
    free(queue);

}

int FreeTree(Tree root)
{
    if(root != NULL)
    {
        FreeTree(root->left);
        FreeTree(root->right);
        free(root);
    }

    return EXIT_SUCCESS;
}