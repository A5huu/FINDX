#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include"text_extractor.h"

int tokenizeQuery(const char *query, char tokens[][100], int max_token_size)
{
    if(query == NULL || tokens == NULL || max_token_size <= 0)
    {
        return -1;
    }
    char current[100];
    int token_count = 0, length = 0, too_long = 0;
    for(int i = 0; ;i++)
    {
        unsigned char ch = (unsigned char)query[i];
        if(isalnum(ch))
        {
            if(length < 99)
            {
                current[length] = (char)tolower(ch);
                length++;
            }
            else
            {
                too_long = 1;
            }
        }
        else
        {
            if(length > 0 || too_long)
            {
                if(too_long || token_count >= max_token_size)
                {
                    return -1;
                }
                current[length] = '\0';
                strcpy(tokens[token_count], current);
                token_count++;
            }

            length = 0;
            too_long = 0;

            if(query[i] == '\0')
            {
                break;
            }
        }
    }
    return token_count;
}

int filterTokens(char tokens[][100], char filtered[][100],
                 int token_count, int max_filtered_size)
{
    if(tokens == NULL || filtered == NULL ||
        token_count < 0 || max_filtered_size < 0)
        {
            return -1;
        }
        int filtered_count = 0;
        for(int i = 0; i < token_count; i++)
        {
            if(!isFiller(tokens[i]))
            {
                if(filtered_count >= max_filtered_size)
                {
                    return -1;
                }
                strcpy(filtered[filtered_count], tokens[i]);
                filtered_count++;

            }
        }
    return filtered_count;
}

// int main()
// {   
//     char query[200];
//     char tokens[50][100];
//     while(1)
//     {
//         printf("\nEnter query (type exit to quit): ");

//         if(fgets(query, sizeof(query), stdin) == NULL)
//             break;

//         int count = tokenizeQuery(query, tokens, 50);
//         for(int i = 0; i < count; i++)
//         {
//             printf("%d. %s", i+1, tokens[i]);
//         }
//     }
// }