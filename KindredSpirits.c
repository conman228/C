//Connor Rinaldo
//co897824
//COP 3502 Spring 2018 

#include <stdio.h>
#include <stdlib.h>
#include "KindredSpirits.h"

node *nodeCreate(int data)//Makes Nodes
{
    node *temp = malloc(sizeof(node));
    
    temp->data = data;
    temp->left = NULL;
    temp->right = NULL;

    return temp;
}

int treesize(node *root)//Gets amount of nodes
{
    if(root == NULL)
        return 0;
    else 
        return treesize(root->left) + treesize(root->right) + 1;
}

void preorder(node *root, int *a, int *array)//Pre MLR
{
    if(root == NULL)
        return;

    array[(*a)++] = root->data;
    preorder(root->left, a, array);
    preorder(root->right, a, array);
}

void postorder(node *root, int *a, int *array)//Post LRM
{
    if(root == NULL)
        return;

    if(root->left != NULL)
        postorder(root->left, a, array);
    
    if(root->right != NULL)
        postorder(root->right, a, array);

    array[(*a)++] = root->data;
}

int isReflection(node *a, node *b)//Check if it is reflected
{
    if(a == NULL && b == NULL)
        return 1;
    
    if(a == NULL & b != NULL || a != NULL && b == NULL)
        return 0;

    if(a->data == b->data)
    {
        isReflection(a->left, b->right);
        isReflection(a->right, b->left);
    }
    else
        return 0;
}

node *makeReflection(node *root)//Makes a refelction for the tree
{
    if(root == NULL)
        return NULL;

    node *temp = nodeCreate(root->data);
    temp->left = makeReflection(root->right);
    temp->right = makeReflection(root->left);

    return temp;
}

int kindredSpirits(node *a, node *b)//Checks
{
    int *first, *second, firstLen, secondLen, i, num = 0, op = 0;  
    int *Ptr = &num;
      
    if(a != NULL && b != NULL)//Gets the size of checks for NULL
    {
        firstLen = treesize(a);
        secondLen = treesize(b);
    }
    else if(a == NULL && b == NULL)
        return 1;
    else
        return 0;

    if(firstLen != secondLen)//If they dont have the same number, cant be the same
        return 0;

    first = malloc(sizeof(int) * firstLen);
    second = malloc(sizeof(int) * secondLen);

    preorder(a, Ptr, first);//Use pointer to put data in right place
    *Ptr = 0;
    postorder(b, Ptr, second);
    *Ptr = 0;

    for(i = 0; i < firstLen; i++)//A - pre, B - Post
    {
        if(first[i] != second[i])
        {
            preorder(b, Ptr, first);
            *Ptr = 0;
            postorder(a, Ptr, second);

            for(i = 0; i < firstLen; i++)//A - Post, B - Pre
            {
                if(first[i] != second[i])
                {
                    free(first);
                    free(second);
                    return 0;
                }
            }
        }
    }//Free all memory
                
    free(first);
    free(second);
    return 1;
}

double difficultyRating(void)
{
    return 3.5;
}

double hoursSpent(void)
{
    return 10.0;
}
