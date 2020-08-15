//Connor Rinaldo
//co897824
//COP 3502 Spring 2018 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "TriePrediction.h"

int main(int argc, char **argv)//Main calls and gets it done
{
    TrieNode *root = buildTrie(argv[1]);

    processInputFile(root, argv[2]);

    root = destroyTrie(root);

    free(root);

    return 0;
}

void printerHelper(TrieNode *root, char *words, int k, int op)//Helps prints all the information
{
    int i;

    if(root == NULL)
        return;

    if(root->count > 0)
    {
        if(op == 1) //subtries have an option(op) anf it add the "- "
            printf("- ");

        printf("%s (%d)\n", words, root->count);
    }

    words[k + 1] = '\0';

    for(i = 0; i < 26; i++)
    {
        words[k] = i + 'a';
        printerHelper(root->children[i], words, k + 1, op);
    }

    words[k] = '\0';
}


void printer(TrieNode *root, int op)
{
    char words[MAX_CHARACTERS_PER_WORD + 1];

    printerHelper(root, words, 0, op);
}

TrieNode *create(void)//Makes it 
{
    return calloc(1, sizeof(TrieNode));
}

TrieNode *insert(TrieNode *root, char *word, char *subWord)
{
    int i, index, len = strlen(word);
    TrieNode *temp;

    if(root == NULL)//Check for NULL
        root = create();

    temp = root;

    if(strcmp(word, "") != 0)//Makes sure blank words dont get in
    {
        for(i = 0; i < len; i++)
        {
            if(isalpha(word[i]))//checks for puncs
            {
                index = tolower(word[i]) - 'a';

                if(temp->children[index] == NULL)
                    temp->children[index] = create();

                temp = temp->children[index]; 
            }
        }
    
    if(strcmp(subWord, "") != 0) // Checks for subtrie 
        temp->subtrie = insert(temp->subtrie, subWord, "");

    temp->count++;

    }
    return root;
}

TrieNode *buildTrie(char *filename)
{ 
    char word[MAX_CHARACTERS_PER_WORD + 1], subWord[MAX_CHARACTERS_PER_WORD + 1], ch;
    int i = 0, k = 0, end = 0;
    TrieNode *root = NULL;
    FILE *file;

    if((file = fopen(filename, "r")) == NULL)
        return NULL;

    while((ch = fgetc(file)) != EOF)//fills first word
    {
        if(ch != ' ')
            word[i++] = ch;
        else
            break;
    }
       
    while((ch = fgetc(file)) != EOF)//gets next word
    {
        if(ch == '.' || ch == '!' || ch == '?')//checks for spaces and addes the values
        {
            root = insert(root, word, subWord);
            root = insert(root, subWord, "");

            memset(subWord, 0, sizeof(subWord));//reseest the values
            memset(word, 0, sizeof(word));

            k = 0;
            i = 0;
        
            while((ch = fgetc(file)) != EOF && end == 0)//refills the first word
            {
                if(ch != ' ')
                    word[i++] = ch;
                else
                    end = 1;
            }
        }

        if(ch != EOF && ch != ' ')
            subWord[k++] = ch;//adds to the subword
        else if(ch != EOF)
        {
            root = insert(root, word, subWord);//addes the word and subwords
            memset(word, 0, sizeof(word));
            strcpy(word, subWord);
            memset(subWord, 0, sizeof(subWord));
            k = 0;
        }
    }
  
    fclose(file);
    return root;
}

void nextWord(TrieNode *root, char *str, int end, int start)//look for the next word
{
    TrieNode *node;
    if(start < end)
    {
        if(getNode(root, str) != NULL)//checks for nodes
        {
            node = getNode(root, str);//checks subtrie
            if(node->subtrie != NULL)
            {
                getMostFrequentWord(node->subtrie, str);//gets word and prints it 
                printf(" %s", str);
                nextWord(root, str, end, start+1);//next word
            }else
                printf("\n");
        }
    }else
        printf("\n");
}

int processInputFile(TrieNode *root, char *filename)
{
    FILE *file;
    TrieNode *node;
    char commands[MAX_CHARACTERS_PER_WORD + 1];//gets the commands
    int i;
    int *Ptr = &i;//pointer for the amount of predictive words

     if((file = fopen(filename, "r")) == NULL)
        return 0;

    while(fscanf(file, "%s", commands) != EOF)
    {
        if(commands[0] == '!')//calls printer
            printer(root, 0);
        else if(commands[0] == '@')
        {   
            fscanf(file, "%s", commands);  //gets the words and number of following words
            fscanf(file, "%d", Ptr);

            if(getNode(root, commands) != NULL)
            {
                if(commands[0] == 'i' && strlen(commands) == 1)//checks for the word "I"
                    commands[0] = toupper(commands[0]);

                printf("%s", commands);//prints the first word
                commands[0] = tolower(commands[0]);
                nextWord(root, commands, i, 0);
            }
            else
                printf("%s\n", commands);//if word is not found            
        }
        else
        {
            printf("%s\n", commands);//prints words and then checks for any subtrie emety or not

            if(getNode(root, commands) == NULL)
                printf("(INVALID STRING)\n");
            else 
            {
                node = getNode(root, commands);
                if(node->subtrie == NULL)
                    printf("(EMPTY)\n");
                else
                    printer(node->subtrie, 1);
            }
        }  
    }    
}

TrieNode *destroyTrie(TrieNode *root)//frees all memory
{
    int i;

    if(root == NULL)
        return NULL;
    
    for(i = 0; i < 26; i++)
        if(root->children[i] != NULL)
            destroyTrie(root->children[i]);

    if(root->subtrie != NULL)
        destroyTrie(root->subtrie);

    free(root);
    return NULL;
}
    
TrieNode *getNodeHelper(TrieNode *root, char *str, int strIndex)//gets to the last node and returns it 
{
    int index;
    
    if(strIndex == strlen(str) && root->count > 0)//gets length and count
        return root;

    index = tolower(str[strIndex]) - 'a';
    
    if(root->children[index] != NULL)
        return getNodeHelper(root->children[index], str, strIndex + 1);
    else
        return NULL;
}

TrieNode *getNode(TrieNode *root, char *str)
{
    int i = 0;

    return getNodeHelper(root, str, i); 
}

void getMostFrequentWordHelper(TrieNode *root, char *word, char *str, int *Ptr, int k)//uses pointer to keep track of count
{
   int i = *Ptr;

    if(root == NULL)
        return;

    if(root->count > i)
    {
        *Ptr = root->count;
        memset(str, 0, sizeof(str));
        strcpy(str, word);
    }

    word[k + 1] = '\0';

    for(i = 0; i < 26; i++)
    {
        word[k] = i + 'a';
        getMostFrequentWordHelper(root->children[i], word, str, Ptr, k + 1);
    }

    word[k] = '\0';
}

void getMostFrequentWord(TrieNode *root, char *str)
{
    int i = 0;
    int *Ptr = &i;
    char word[MAX_CHARACTERS_PER_WORD + 1], strDup[MAX_CHARACTERS_PER_WORD + 1];

    memset(str, 0, sizeof(str));

    if(root == NULL)
        return;

    word[0] = '\0';

    getMostFrequentWordHelper(root, word, strDup, Ptr, 0);
    strcpy(str, strDup);
}

int containsWord(TrieNode *root, char *str)//uses getnode to solve
{
    if(getNode(root, str) != NULL)
        return 1;
    else
        return 0;
}

void wordcounter(TrieNode *root, int *count)//count the amount of words is a loop
{
    int i;

    if(root == NULL)
        return;

    if(root->count > 0)
        *count = *count + root->count;

    for(i = 0; i < 26; i++)
        wordcounter(root->children[i], count);
}

int prefixCountHelper(TrieNode *root, char *str, int strIndex, int *Ptr)
{
    int index;
    
    if(strIndex == strlen(str))
    {   
        wordcounter(root, Ptr);
        return *Ptr;
    }

    index = tolower(str[strIndex]) - 'a';
    
    if(root->children[index] != NULL)
        return prefixCountHelper(root->children[index], str, strIndex + 1, Ptr);
    else
        return 0;
}

int prefixCount(TrieNode *root, char *str)
{
    int i = 0, k = 0;
    int *Ptr = &k;
    
    if(root == NULL)
        return 0;

    if(str == "")
    {   
        wordcounter(root, Ptr);
        return k;
    }

    return prefixCountHelper(root, str, i, Ptr);
}

double difficultyRating(void)
{
    return 5.0;
}

double hoursSpent(void)
{
    return 15.0;
}
