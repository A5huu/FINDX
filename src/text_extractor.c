#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "text_extractor.h"

const char *filler[] = {
    "a", "about", "above", "actually", "after", "again", "against", "ah", "all", "almost",
    "also", "although", "always", "am", "an", "and", "another", "any", "anybody", "anyone",
    "anything", "anywhere", "are", "aren", "around", "as", "at", "away", "be", "became",
    "because", "become", "becomes", "been", "before", "behind", "being", "below", "beside", "besides",
    "between", "beyond", "both", "but", "by", "can", "cannot", "could", "couldn", "did",
    "didn", "do", "does", "doesn", "doing", "don", "down", "during", "each", "either",
    "else", "elsewhere", "essentially", "even", "ever", "every", "everybody", "everyone", "everything", "everywhere",
    "few", "finally", "for", "from", "further", "get", "gets", "go", "goes", "going",
    "good", "got", "had", "hadn", "has", "hasn", "have", "haven", "having", "he",
    "her", "here", "hers", "herself", "him", "himself", "his", "how", "however", "i",
    "if", "in", "into", "is", "isn", "it", "its", "itself", "just", "know",
    "like", "literally", "basically", "maybe", "me", "might", "mightn", "more", "most", "mostly",
    "much", "must", "mustn", "my", "myself", "near", "neither", "never", "no", "nobody",
    "none", "noone", "nor", "not", "nothing", "now", "nowhere", "of", "off", "often",
    "ok", "okay", "on", "once", "one", "only", "onto", "or", "other", "others",
    "otherwise", "our", "ours", "ourselves", "out", "over", "own", "right", "same", "seriously",
    "should", "shouldn", "so", "some", "somebody", "someone", "something", "somewhere", "still", "such",
    "than", "that", "the", "their", "theirs", "them", "themselves", "then", "there", "therefore",
    "these", "they", "this", "those", "though", "through", "throughout", "thru", "thus", "to",
    "together", "too", "toward", "towards", "uh", "um", "under", "until", "up", "upon",
    "us", "very", "was", "wasn", "we", "well", "were", "weren", "what", "whatever",
    "when", "whenever", "where", "wherever", "whether", "which", "while", "who", "whoever", "whom",
    "whose", "why", "will", "with", "within", "without", "won", "would", "wouldn", "yes",
    "yet", "you", "your", "yours", "yourself", "yourselves", "you-know", "mean", "like-i-said", "totally"
};

#define FILLER_COUNT (sizeof(filler) / sizeof(filler[0]))

//function to check filler word
int isFiller(const char *word)
{
    for(int i = 0; i < FILLER_COUNT; i++ ){
        if(strcmp(word, filler[i]) == 0){
            return 1;
        }
    }
    return 0;
}

void extractText(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if(file == NULL)
    {
        printf("An Error Occured, Can't open file!");
        return;
    }
    char word[100];
    int index = 0;
    char ch;

    //start reading the file
    while((ch = fgetc(file)) != EOF)
    {
        if(isalnum((unsigned char)ch))
        {
            //check for buffer overflow
            if(index < 99)
            {
                //convert to lower and store
                word[index] = tolower((unsigned char)ch);
                index++;
            }
        }
        else
        {
            if(index > 0)
            {
                word[index] = '\0';

                //check for filler
                if(!isFiller(word))
                {
                    printf("%s\n", word);
                }
                //reset buffer
                index = 0;
            }
        }
    }
    // Process the final word if the file ends without punctuation
    if (index > 0)
    {
        word[index] = '\0';

        if (!isFiller(word))
        {
            printf("%s\n", word);
        }
    }
    //close file
    fclose(file);
}