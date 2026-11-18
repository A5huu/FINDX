#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hash.h"
#include "indexing.h"
#include "text_extractor.h"
#include "searchengine.h"

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
                // check for dupes so repeated query words don't count twice toward the threshold
                int is_duplicate = 0;
                for(int j = 0; j < filtered_count; j++)
                {
                    if(strcmp(filtered[j], tokens[i]) == 0)
                    {
                        is_duplicate = 1;
                        break;
                    }
                }
                if(!is_duplicate)
                {
                    if(filtered_count >= max_filtered_size)
                    {
                        return -1;
                    }
                    strcpy(filtered[filtered_count], tokens[i]);
                    filtered_count++;
                }

            }
        }
    return filtered_count;
}

//heper function to check if filtered query appears in exact consecutive order
int checkExactPhrase(DocNode *doc, char filtered[][100], int filtered_count)
{
    if(filtered_count < 2) return 0;

    //we verify if consecutive terms appear on the same line with sequential seq IDs 
    for(int i = 0; i <filtered_count - 1; i++)
    {
        //find position for filtered[i] and filtered[i+1]
        WordNode *node1 = lookUpHash(filtered[i]);
        WordNode *node2 = lookUpHash(filtered[i+1]);
        if(!node1 || !node2) return 0;
        
        DocNode *d1 = node1->document;
        while(d1 && strcmp(d1->filename, doc->filename) != 0) d1 = d1->next;

        DocNode *d2 = node2->document;
        while(d2 && strcmp(d2->filename, doc->filename) != 0) d2 = d2->next;

        if(!d1 || !d2) return 0;

        //check if any occurrence matches same line and sequential order
        int matched_pair = 0;
        PosNode *p1 = d1->positions;
        while (p1 != NULL)
        {
            PosNode *p2 = d2->positions;
            while (p2 != NULL)      
            {
                if(p1->line == p2->line && p2->seq == p1->seq + 1)
                {
                    matched_pair = 1;
                    break; 
                }
                p2 = p2->next;
            }
            if(matched_pair) break;
            p1 = p1->next;
        }
        if(!matched_pair) return 0;
    }
    return 1;
}

//calculate weighted score for ranking
int calculateScore(SearchCandidate *candidate)
{
    int score = 0;
    if(candidate->exact_match) score += 100000;
    score += (candidate->matched_terms_count * 1000);
    score += candidate->total_frequency;
    return score;
 }

 //helper to sort candidate max - heap 
 void heapify(SearchCandidate candidates[], int n, int i)
 {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    int score_largest = calculateScore(&candidates[largest]);
    int score_left = (left < n) ? calculateScore(&candidates[left]) : -1;
    int score_right = (right < n) ? calculateScore(&candidates[right]) : -1;

    if(left < n && score_left > score_largest)
    {
        largest = left;
        score_largest = score_left;
    }

    if(right < n && score_right > score_largest)
    {
        largest = right;
    }

    if(largest != i)
    {
        SearchCandidate temp = candidates[i];
        candidates[i] = candidates[largest];
        candidates[largest] = temp;
        heapify(candidates, n, largest);
    }
 }

 void sortCandidatesHeap(SearchCandidate candidates[], int n)
 {
    //build Max- Heap
    for(int i = n / 2 - 1; i >= 0; i--)
    {
        SearchCandidate temp = candidates[0];
        candidates[0] = candidates[i];
        candidates[i] = temp;
        heapify(candidates, i, 0);
    }
 }

 //open the document file 
 void printSnippet(const char *filename, long offset, int line)
 {
    FILE *file = fopen(filename, "r");
    if(file == NULL)
    {
        printf("    [Snippet unavailable: Could not open file %s]\n", filename);
        return;
    }

    if(fseek(file, offset, SEEK_SET) != 0)
    {
        printf("    [Snippet unavailable: Seek Failed]\n", filename);
        fclose(file);
        return;
    }

    char line_buffer[512];
    if(fgets(line_buffer, sizeof(line_buffer), file) != NULL)
    {
        //remove trailing newline
        size_t len = strlen(line_buffer);
        if(len > 0 && (line_buffer[len - 1] == '\n' || line_buffer[len - 1] == '\r'))
        {
            line_buffer[len - 1] = '\0';
        }
        printf("    Line %d: %s\n", line, line_buffer);
    }
    else
    {
        printf("    [Snippet unavailable]\n");
    }
    fclose(file);
 }

 void searchMultiWord(const char *query)
{
    char tokens[50][100];
    char filtered[50][100];
    int token_count = tokenizeQuery(query, tokens, 50);
    if(token_count <= 0)
    {
        printf("Invalid or empty query.\n");
        return;
    }
    int filtered_count = filterTokens(tokens, filtered, token_count, 50);
    if(filtered_count <= 0)
    {
        printf("\nQuery contains only filler words. No results found.\n");
        return;
    }
    int threshold_50 = (filtered_count * 50 + 99) / 100;
    int threshold_35 = (filtered_count * 35 + 99) / 100;
    int threshold_25 = (filtered_count * 25 + 99) / 100;

    if(threshold_50 < 1) threshold_50 = 1;
    if(threshold_35 < 1) threshold_35 = 1;
    if(threshold_25 < 1) threshold_25 = 1;

    SearchCandidate candidates[100];
    int candidate_count = 0;

    //step 1 : colleect all candidates
    for(int i = 0; i < filtered_count; i++)
    {
        WordNode *word_node = lookUpHash(filtered[i]);
        if(word_node != NULL)
        {
            DocNode *doc_curr = word_node->document;
            while(doc_curr != NULL)
            {
                int found_idx = -1;
                for(int c = 0; c < candidate_count; c++)
                {
                    if(strcmp(candidates[c].filename, doc_curr->filename) == 0)
                    {
                        found_idx = 0;
                        break;
                    }
                }
                if(found_idx != -1)
                {
                    candidates[found_idx].matched_terms_count++;
                    candidates[found_idx].total_frequency += doc_curr->frequency;
                }
                else
                {
                    if(candidate_count < 100)
                    {
                        strcpy(candidates[candidate_count].filename, doc_curr -> filename);
                        candidates[candidate_count].matched_terms_count = 1;
                        candidates[candidate_count].total_frequency = doc_curr->frequency;
                        candidates[candidate_count].exact_match = 0;
                        candidate_count ++;
                    }
                }
                doc_curr = doc_curr->next;
            }
        }
    }

    //Step 2: Apply thresholds
    int active_threshold = threshold_50;
    for(int tier = 1; tier <= 3; tier++)
    {
        if(tier == 1) active_threshold = threshold_50;
        else if (tier == 2) active_threshold = threshold_35;
        else if (tier ==3) active_threshold = threshold_25;

        int surviving_check = 0;
        for(int c = 0; c <candidate_count; c++)
        {
            if(candidates[c].matched_terms_count >= active_threshold)
            {
                surviving_check++;
            }
        }
        if(surviving_check > 0) break;
    }

    //filter array down to only surviving candidate
    SearchCandidate surviving[100];
    int surviving_count = 0;
    for(int c = 0; c < candidate_count; c++)
    {
        if(candidates[c].matched_terms_count >= active_threshold)
        {
            surviving[surviving_count] = candidates[c];
            //find docnode pointer to check exact phrase
            WordNode *w_node = lookUpHash(filtered[0]);
            if(w_node)
            {
                DocNode *d_node = w_node->document;
                while (d_node && strcmp(d_node->filename, surviving[surviving_count].filename) != 0)
                {
                    d_node = d_node->next;
                }
                if(d_node)
                {
                    surviving[surviving_count].exact_match = checkExactPhrase(d_node, filtered, filtered_count);
                }  
            }
            surviving_count++;
        }
    }

    if(surviving_count == 0)
    {
        printf("\nNo matching document found for your query. \n");
        return;
    }
    
    //step 3: sort using maxheap
    sortCandidatesHeap(surviving, surviving_count);

    //step4: Display results with snipppets
    printf("\n==================================================\n");
    printf("SEARCH RESULTS FOR: \"%s\"\n", query);
    printf("\n==================================================\n");
    for(int c = 0; c < surviving_count; c++)
    {
        printf("\n[%d] File: %s | Score: %d (Matched: %d/%d terms), Freq: %d%s)\n",
        c + 1, surviving[c].filename,
        calculateScore(&surviving[c]),
        surviving[c].matched_terms_count,
        filtered_count,
        surviving[c].total_frequency,
        surviving[c].exact_match ? ",Exact Phrase Boost!" : "");
        
        WordNode *w_node = lookUpHash(filtered[0]);
        if(w_node)
        {
            DocNode *d_node = w_node->document;
            while (d_node && strcmp(d_node->filename, surviving[c].filename) != 0)
            {
                d_node = d_node->next;
            }
            if(d_node && d_node->positions)
            {
                printSnippet(d_node->filename, d_node->positions->offset, d_node->positions->line);
            }
            
        }
    }
    printf("\n==================================================\n");
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