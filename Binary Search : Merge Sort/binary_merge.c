/***************************************
*
*  Mustafa Teber mst89
*  This program accepts two files as an
*  argument and finds words that start with
*  queries located in the query file, from the
*  dictionary file.
*
***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

typedef struct item
{
  char* word;
  int weight;
} Item;

//Binary search recursively searchs for a target
int binarySearch(Item* arr[], int start, int end, char* target)
{
    if (end >= start) {
        int mid = start + (end - start) / 2;

        if(strncmp(target, arr[mid]->word, strlen(target)) == 0)
            return mid;

        if (strncmp(target, arr[mid]->word, strlen(target)) < 0)
            return binarySearch(arr, start, mid - 1, target);

        return binarySearch(arr, mid + 1, end, target);
    }
    return -1;
}

//Merge function merges two array 
void merge(Item* arr[],int start, int mid, int end, int opt) 
{
    int firstLen = mid-start+1;
    int secondLen = end-mid;

    Item** first=malloc(sizeof(Item *) * firstLen);
    Item** second=malloc(sizeof(Item *) * secondLen);

    for(int i=0; i<firstLen; i++)
    {
        first[i]=arr[start+i];
    }
    for(int i=0;i<secondLen;i++)
    {
        second[i]=arr[mid+i+1];                
    }
    int j=0,k;
	 int i=0;
    k=start;
    while(i<firstLen && j<secondLen)
    {
		if(opt == 0){
        if(strcmp(first[i]->word,second[j]->word)<=0)arr[k++]=first[i++];
        else arr[k++]=second[j++];
		} else {
			if(first[i]->weight >= second[j]->weight)arr[k++]=first[i++];
        else arr[k++]=second[j++];
		}
    }
    while(i<firstLen)arr[k++]=first[i++];
    while(j<secondLen)arr[k++]=second[j++];
	 
	 free(first);
	 free(second);
}

void mergeSort(Item* arr[], int start, int end, int opt)
{
    if(start<end)
    {
        int mid = (start+end)/2;
        mergeSort(arr,start,mid,opt);
        mergeSort(arr,mid+1,end,opt);
        merge(arr,start,mid,end,opt);
    }
}

int main(int argc, char **argv)
{
    char* dictionaryFilePath = argv[1]; // To keep the path to dictionary file
    char* queryFilePath = argv[2]; // To keep the path to the file with a list of query wrods
    int wordCount = 0; // To keep a count of words in the dictionary file
    int queryCount = 0; // To keep a count of queries in the query file
    
    FILE* fp = fopen(dictionaryFilePath, "r");
    char* line = NULL; // To be used for line counting
    size_t lineBuffSize = 0; // To be used for line counting
    ssize_t lineSize; //To be used for line counting
    
    // For checking if the path to the dictionary file is valid
    if(fp == NULL)
    {
        fprintf(stderr, "Error opening file:%s\n", dictionaryFilePath);
        return -1;
    }

    // For counting the number of lines in the dictionary file
    while ((lineSize = getline(&line, &lineBuffSize, fp)) != -1)
    {
        wordCount++;
    }
    
	Item** entries  = malloc(wordCount * sizeof(Item*));;

    fseek(fp, 0, SEEK_SET); // Using the fseek function, we reset the file position indicator back to the beginning of the file
    char word[BUFSIZE]; // For reading lines in the loop below
    int weight;

    for (int i = 0; i < wordCount; i++)
    {
        fscanf(fp, "%s %d\n", word, &weight);
		  Item* entry  = malloc(sizeof(Item));
		  entry -> weight = weight;
		  entry -> word = malloc(sizeof(char) * (strlen(word) + 1));
		  strcpy(entry -> word, word);
		  entries[i] = entry;
    }
    // Closing the dictionary file
    fclose(fp);

    // Opening the query file
    fp = fopen(queryFilePath, "r");
        
    // For checking if the path to the dictionary file is valid
    if(fp == NULL)
    {
        fprintf(stderr, "Error opening file:%s\n", queryFilePath);
        return -1;
    }

    // Counting the number of queries
    while ((lineSize = getline(&line, &lineBuffSize, fp)) != -1)
    {
        queryCount++;
    }

    free(line); // getline internally allocates memory, so we need to free it here so as not to leak memory!!

	char** queries = malloc(queryCount * sizeof(char*));

    fseek(fp, 0, SEEK_SET); // Using the fseek function, we reset the file position indicator back to the beginning of the file.
    for (int i = 0; i < queryCount; i++)
    {
        fscanf(fp, "%s\n",word);
        //This might be a good place to store the query words in a list like data structure
		  queries[i] = malloc(sizeof(char) * (strlen(word) + 1));
		  strcpy(queries[i], word);
    }

    // Closing the query file
    fclose(fp);

    //Sort the dictionary to be able to use binary search
	mergeSort(entries, 0, wordCount-1,0);
	 
	for (int i = 0; i < queryCount; i++)
    {
		printf("Query word:%s\n",queries[i]);

        //Use binary search to find the first word matching the query
		int index = binarySearch(entries, 0, wordCount-1, queries[i]);
		if(index < 0){
			printf("No suggestion!\n");
			continue;
		}
        //Use linear search to find the boundaries of matching interval
		int startIndex = 0;
		int endIndex = wordCount-1;
		for(int j=index; j<wordCount; j++){
			if(strncmp(queries[i], entries[j]->word, strlen(queries[i])) != 0){
				endIndex = j-1;
				break;
			}
		}
		for(int j=index; j>0; j--){
         if(strncmp(queries[i], entries[j]->word, strlen(queries[i])) != 0){
            startIndex = j+1;
				break;
            }
        }
        //Store the matching words in a different array
		Item** matches = malloc(sizeof(Item*) * (endIndex-startIndex+1));

		for(int j=0; j<=endIndex-startIndex; j++){
			matches[j] = entries[j+startIndex];
		}

        //Sort by weight
		mergeSort(matches, 0, endIndex-startIndex, 1);

        //Print the results
		for(int j=0; j<=endIndex-startIndex; j++){
			if(j==10)break;
         printf("%s %d\n", matches[j]->word, matches[j]->weight);
      }
		free(matches);
	 } 

	 // Free all the memory allocated
	 for(int i=0; i<wordCount; i++){
      free(entries[i]->word);
      free(entries[i]);
	 }
	 for(int i=0; i<queryCount; i++){
		free(queries[i]);
	 }
	 free(entries);
	 free(queries);
    return 0;
}
