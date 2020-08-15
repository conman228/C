//Connor Rinaldo
//co897824
//COP 3502 Spring 2018 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ListyString.h"

int main(int argc, char **argv)//Main calls functions
{
    processInputFile(argv[1]);
    return 0;
}

void setsUpReplaceCat(ListyString *listy, char key, char *commands, int beg, int end, int op)//Sets up the string and or key
{
    int m, strLen = 0, pos = beg;
    char *str;
  
    while(pos != end-1 && commands[pos] != '@' && commands[pos] != '+' && commands[pos] != '-' && commands[pos] != '~' && commands[pos] != '?' && commands[pos] != '!')//Gets the length of string
	{
        strLen++;
		pos++;
	}
	
    str = malloc(sizeof(char) * (strLen+1));
    pos = beg;

    for(m = 0; m < strLen; m++)
        str[m] = commands[pos++];

	str[m-1] = '\0';

    if(op == 0)//Calls one of the two
        replaceChar(listy, key, str);
    else
        listyCat(listy, str);

    free(str);
}

int processInputFile(char *filename)//gets the file name, checks, fills, and calls
{
    FILE *file = NULL;
    
    if((file = fopen(filename, "r")) == NULL)
        return 1; 
 
    char ch, com, *word, *commands, *temp = malloc(sizeof(char) * 1024);
    ListyString *listy = NULL; 
    int j, i = 0;

    ch = fgetc(file);
    temp[0] = ch;
    
    while(ch != EOF)//Goes unitl commands or EOF
    {
        if(temp[i] != '@' && temp[i] != '+' && temp[i] != '-' && temp[i] != '~' && temp[i] != '?' && temp[i] != '!')
        {
            ch = fgetc(file);
            temp[++i] = ch;
        }
        else
        {
            word = malloc(sizeof(char) * i);//Fills string

            for(j = 0; j < i; j++)
                word[j] = temp[j];
        
            i = 0;
            word[j-1] = '\0';

            while(ch != EOF)
            {
                temp[i++] = ch; 
                ch = fgetc(file);
            }
            
            commands = malloc(sizeof(char) * i);//Fills commands
            
            for(j = 0; j < i; j++)
                commands[j] = temp[j];

            commands[j-1] = '\0';
        }
    }

    fclose(file);
    free(temp);  
    listy = createListyString(word);
    free(word);
   
    for(j = 0; j < i; j++)
    {
        com = commands[j];
        
        switch(com)
        {
            case '@':
                setsUpReplaceCat(listy, commands[j+2], commands, j+4, i, 0);
                break;
            case '+':
                setsUpReplaceCat(listy, ' ', commands, j+2, i, 1);
                break;
            case '-':
                replaceChar(listy, commands[j+2], NULL);
                break;
            case '~':
                reverseListyString(listy);
                break;
            case '?':
                printf("%d\n", listyLength(listy));
                break;
            case '!':
                printListyString(listy);
                break;
            default:
                break;
        }
    }

    free(commands);
    destroyListyString(listy);
    return 0;
}

void nodeInsert(ListyNode *head, char data)//Inserts next charcter in function
{
        ListyNode *node = malloc(sizeof(ListyNode));
        node->data = data;
        node->next = NULL;
        head->next = node;
} 

ListyString *createListyString(char *str)//Creates ListyString and ListyNodes
{
    int i = 1, length;    
    
    ListyString *list = malloc(sizeof(ListyString)), *send; 
    ListyNode *node = malloc(sizeof(ListyNode));

    if(str == NULL || str[0] == '\0')
    {
        list->head = NULL;
        list->length = 0;
        return list;
    }

    node->data = str[0];
    node->next = NULL;

    length = strlen(str);
    list->head = node;
    list->length = length;

    while(str[i] != '\0')
    {
        nodeInsert(node, str[i++]);
        node = node->next;
    }   
    return list;
} 

void destroyListyNode(ListyNode *node)//Destroys the nodes
{
    ListyNode *temp;

    if(node != NULL)
    {
        temp = node->next;
        free(node);
        destroyListyNode(temp);
    }   
}

ListyString *destroyListyString(ListyString *listy)//Destroys the Listy
{
    if(listy == NULL || listy->head == NULL)
    {
        free(listy);
        return NULL;
    }

    destroyListyNode(listy->head);   
    free(listy);
    return NULL;
}

ListyString *cloneListyString(ListyString *listy)//Clones a copy of Listy
{
    if(listy == NULL)
        return NULL;

    ListyString *cloneList = malloc(sizeof(ListyString));
    ListyNode *temp, *node = malloc(sizeof(ListyNode));

    if(listy->head == NULL)
    {
        cloneList->head = NULL;
        cloneList->length = 0;
        return cloneList;
    }   

    node->data = listy->head->data;
    node->next = NULL;    

    cloneList->head = node;
    cloneList->length = listy->length;

    temp = listy->head->next;
    
    while(temp != NULL)
    {
        nodeInsert(node, temp->data);
        temp = temp->next;
    }

    return cloneList;
}

void replaceChar(ListyString *listy, char key, char *str)//Replaces characters with string
{

    ListyNode *first = listy->head, *temp = listy->head;
    int i = 0, length;
 
    if(listy == NULL || listy->head == NULL)
        return;

    if(str == NULL || str[0] == '\0')//If it is empty of not 
    { 
        while(first != NULL && first->data == key)//Gets the first non-key charcter
        { 
            listy->head = first->next;
            free(first);
            first = listy->head;
            listy->length--;  
        }

        if(first == NULL || first->next == NULL)
            return;
        else
            temp = first->next;

        while(temp != NULL)//Goes throught the linked list and removes key
        {
            if(temp->data == key)
            {
                first->next = temp->next;
                free(temp);
                temp = first->next; 
                listy->length--;            
            }
            else
            {
                temp = temp->next;
                first = first->next;
            }
        }
        return;
    }

    length = strlen(str)-1;

    while(temp != NULL)
    { 
        if(temp->data == key)
        {
            first->data = str[i++];
            temp = first->next; 

            while(str[i] != '\0')//Adds the instert
            {
                nodeInsert(first, str[i++]);
                first = first->next;
            }

            i = 0;        
            first->next = temp;
            first = first->next;
            listy->length = listy->length + length;

        }
        else
        {
            temp = temp->next;
            first = first->next;
        }
    }
}

void reverseListyString(ListyString *listy)//Reverse Listy
{
    ListyNode *temp; 
    char *str;
    int i = 0;    

    if(listy == NULL || listy->head == NULL)
        return;
    
    temp = listy->head;

    while(temp != NULL)
    {
        i++;
        temp = temp->next;
    }

    str = malloc(sizeof(char) * i);
    temp = listy->head;
    i = 0;

    while(temp != NULL)
    {
        str[i++] = temp->data;
        temp = temp->next;
    }

    i--;
    temp = listy->head;

    while(temp != NULL)
    {
        temp->data = str[i--]; 
        temp = temp->next;
    }

    free(str);
}

ListyString *listyCat(ListyString *listy, char *str)//Combine String to Nodes
{
    ListyNode *temp;
    int length, i = 0;

    if(listy == NULL && str == NULL)
        return NULL;

    if(str == NULL || str[0] == '\0')
        return listy;

    if(listy == NULL)
        return createListyString(str);

    temp = listy->head;
    length = strlen(str);    

    if(listy->head == NULL)
    {
        temp = malloc(sizeof(ListyNode));
        temp->data = str[i++];
        temp->next = NULL;
        listy->head = temp;
    }

    listy->length = listy->length + length;

    while(temp->next != NULL)
        temp = temp->next;

    while(str[i] != '\0')
    {
        nodeInsert(temp, str[i++]);
        temp = temp->next;
    }
    return listy;
}

int listyCmp(ListyString *listy1, ListyString *listy2)//Compare Listys
{
    ListyNode *node1, *node2;

    if(listy1 == NULL && listy2 == NULL)
        return 0;

    if(listy1 == NULL && listy2 != NULL || listy1 != NULL && listy2 == NULL)
        return 1;

    if(listy1->head == NULL && listy2->head == NULL)
        return 0;        
    
    if(listy1->length != listy2->length)
        return 1;

    node1 = listy1->head;
    node2 = listy2->head;

    while(node1 != NULL)
    {
        if(node1->data != node2->data)
            return 1;

        node1 = node1->next;
        node2 = node2->next;
    }

    return 0;
}

int listyLength(ListyString *listy)//Gets the length
{
    if(listy == NULL)
        return -1;
    else if(listy->head == NULL)
        return 0;
    else
        return listy->length;
}

void printListyString(ListyString *listy)// Prints the Listy
{
    ListyNode *temp;

    if(listy == NULL || listy->head == NULL)
    {
        printf("(empty string)\n"); 
        return;
    }

    temp = listy->head;

    while(temp != NULL)
    {
        printf("%c", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

double difficultyRating(void)
{
    return 3.0;
}

double hoursSpent(void)
{
    return 12.0;
}

