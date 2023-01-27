
// HEADER FILES

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// CONSTANT DECLARATIONS

#define INITIAL_SIZE 2

// STRUCT DECLARATIONS

typedef struct entry
{
	char* name;
	char* lastname;
	float height;
	int age;
} Entry;

typedef struct list
{
	int capacity;
	int size;
	Entry** data;
} List;

// FUNCTION DEFINTIONS

// Returns a pointer to an Entry struct initialized with the specified values for each of the 4 members of the struct (NO NEED TO CHANGE).
Entry* initializeEntry(char* name, char* lastname, float height, int age)
{
	Entry* newEntry = malloc(sizeof(Entry));

	newEntry -> name = malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(newEntry -> name, name);

	newEntry -> lastname = malloc(sizeof(char) * (strlen(lastname) + 1));
	strcpy(newEntry -> lastname, lastname);

	newEntry -> height = height;

	newEntry -> age = age;

	return newEntry;
}

// Given a pointer to an Entry struct, this function frees the memory allocated for that Entry struct (NO NEED TO CHANGE).
void freeEntry(Entry* entry)
{
	if (entry != NULL)
	{
		free(entry -> name);
		free(entry -> lastname);
		free(entry);
	}
}

List* initializeList()
{
	List* newList = malloc(sizeof(List));

	newList -> size = 0;

	newList->capacity =  INITIAL_SIZE;

	newList->data = malloc(sizeof(Entry *) * newList-> capacity);

	return newList;
}

void deleteList(List* myList)
{
	for(int i=0; i < myList -> size; i++)
	{
		freeEntry(myList -> data[i]);
	}
	free(myList -> data);
	free(myList);
}

void doubleCapacity(List* myList)
{
	Entry** entries = malloc(sizeof(Entry*) * myList -> capacity * 2);

	for(int i=0; i < myList -> size; i++)
	{
		entries[i] = myList -> data[i];
	}

	free(myList->data);
	myList->data = entries;

	myList -> capacity = myList -> capacity * 2;
}


void halveCapacity(List* myList)
{
	Entry** entries = malloc(sizeof(Entry*) * myList -> capacity / 2);

   for(int i=0; i < myList -> size; i++)
   {
      entries[i] = myList -> data[i];
   }

   free(myList->data);
   myList->data = entries;

   myList -> capacity = myList -> capacity / 2;
}

void insertToHead(List* myList, char* name, char* lastname, float height, int age)
{
	if(myList -> size == myList -> capacity){
		doubleCapacity(myList);
	}

	Entry* newEntry = initializeEntry(name, lastname, height, age);

	if(myList -> size == 0)
	{
		myList -> data[0] = newEntry;
	}
	else
	{
		Entry** entries = malloc(sizeof(Entry*) * myList -> capacity);
		entries[0] = newEntry;

		for(int i=0; i < myList -> size; i++)
		{
			entries[i+1] = myList -> data[i];
		}

		free(myList->data);
		myList->data = entries;
	}

	myList -> size = (myList -> size) + 1;
}



void insertToTail(List* myList, char* name, char* lastname, float height, int age)
{
	if(myList -> size == myList -> capacity){
      doubleCapacity(myList);
   }

   Entry* newEntry = initializeEntry(name, lastname, height, age);

   myList -> data[myList -> size] = newEntry;
   myList -> size = (myList -> size) + 1;
}

void insertToPosition(List* myList, int position, char* name, char* lastname, float height, int age)
{
	if(position < 0 || position > myList -> size)
	{
		printf("Error invalid position!");
		return;
	}

	if(myList -> size == myList -> capacity){
      doubleCapacity(myList);
   }

   Entry* newEntry = initializeEntry(name, lastname, height, age);

	Entry** entries = malloc(sizeof(Entry*) * myList -> capacity);

	for(int i=0; i < position; i++)
	{
		entries[i] = myList -> data[i];
	}

	entries[position] = newEntry;

	for(int i=position; i < myList -> size; i++)
   {
      entries[i+1] = myList -> data[i];
   }

	free(myList->data);
	myList->data = entries;

   myList -> size = (myList -> size) + 1;
}

int findPosition(List* myList, char* name)
{
	for(int i=0; i < myList -> size; i++){
		if(strcmp(name,myList->data[i]->name) == 0)
		{
			return i;
		}
	}
	return -1;
}

void deleteFromHead(List* myList)
{
	if(myList -> size == 0)
   {
      printf("List is empty!");
		return;
   }

	Entry** entries = malloc(sizeof(Entry*) * myList -> capacity);

	for(int i=0; i < myList -> size - 1; i++)
	{
		entries[i] = myList -> data[i+1];
	}

	freeEntry(myList -> data[0]);
	free(myList->data);
	myList->data = entries;

   myList -> size = (myList -> size) - 1;
	if(myList -> size < myList -> capacity / 2)
	{
		halveCapacity(myList);
	}
}

void deleteFromTail(List* myList)
{
   if(myList -> size == 0)
   {
      printf("List is empty!");
      return;
   }

	freeEntry(myList -> data[myList -> size - 1]);

	myList -> size = myList -> size - 1;

	if(myList -> size < myList -> capacity / 2)
   {
      halveCapacity(myList);
   }
}


void deleteFromPosition(List* myList, int position)
{
	if(position < 0 || position >= myList -> size)
   {
      printf("Error invalid position!");
      return;
   }

	Entry** entries = malloc(sizeof(Entry*) * myList -> capacity);

   for(int i=0; i < position; i++)
   {
      entries[i] = myList -> data[i];
   }

   for(int i=position+1; i < myList -> size; i++)
   {
      entries[i-1] = myList -> data[i];
   }

	freeEntry(myList -> data[position]);
   free(myList->data);
   myList->data = entries;

   myList -> size = (myList -> size) - 1;
	if(myList -> size < myList -> capacity / 2)
   {
      halveCapacity(myList);
   }
}

// Given a pointer to a List struct, this function prints each Entry in that list (NO NEED TO CHANGE).
void printList(List* myList)
{
	if (myList -> size == 0)
	{
		printf("List is empty!\n");
	}
	else
	{
		for (int i = 0; i < myList -> size; i++)
		{
			printf("[%d]\t%s\t%s\t%0.2f\t%d\n", i, myList -> data[i] -> name, myList -> data[i] -> lastname, myList ->
			data[i] -> height, myList -> data[i] -> age);
		}
	}
}

// Given a pointer to a List struct, this function prints out the size and capacity of that List (NO NEED TO CHANGE).
void printListInfo(List* myList)
{
	printf("size:%d, capacity:%d\n", myList -> size, myList -> capacity);
}

int main(int argc, char** argv)
{
	FILE* fp = fopen(argv[1], "r");
	char* line = NULL;
	size_t lineBuffSize = 0;
	ssize_t lineSize;

	if (fp == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		return -1;
	}

	List* myList;

	myList = initializeList();

	while ((lineSize = getline(&line, &lineBuffSize, fp)) != -1)
	{
		char* token;
		const char delimiter[2] = " ";

		if (line[strlen(line) - 1] == '\n')
		{
			line[strlen(line) - 1] = '\0';
		}

		token = strtok(line, delimiter);

		if (strcmp(token, "insertToHead") == 0)
		{
			char* name;
			char* lastname;
			float height;
			int age;

			name = strtok(NULL, delimiter);
			lastname = strtok(NULL, delimiter);
			height = atof(strtok(NULL, delimiter));
			age = atoi(strtok(NULL, delimiter));

			insertToHead(myList, name, lastname, height, age);
		}
		else if (strcmp(token, "insertToTail") == 0)
		{
			char* name;
			char* lastname;
			float height;
			int age;

			name = strtok(NULL, delimiter);
			lastname = strtok(NULL, delimiter);
			height = atof(strtok(NULL, delimiter));
			age = atoi(strtok(NULL, delimiter));

			insertToTail(myList, name, lastname, height, age);
		}
		else if (strcmp(token, "insertToPosition") == 0)
		{
			char* name;
			char* lastname;
			float height;
			int age;
			int position;

			position = atoi(strtok(NULL, delimiter));
			name = strtok(NULL, delimiter);
			lastname = strtok(NULL, delimiter);
			height = atof(strtok(NULL, delimiter));
			age = atoi(strtok(NULL, delimiter));
			insertToPosition(myList, position, name, lastname, height, age);
		}
		else if (strcmp(token, "findPosition") == 0)
		{
			char* name;

			name = strtok(NULL, delimiter);
			printf("%d\n", findPosition(myList, name));
		}
		else if (strcmp(token, "deleteFromHead") == 0)
		{
			deleteFromHead(myList);
		}
		else if (strcmp(token, "deleteFromTail") == 0)
		{
			deleteFromTail(myList);
		}
		else if (strcmp(token, "deleteFromPosition") == 0)
		{
			int position;

			position = atoi(strtok(NULL, delimiter));

			deleteFromPosition(myList, position);
		}
		else if (strcmp(token, "printList") == 0)
		{
			printList(myList);
		}
		else if(strcmp(token, "printListInfo") == 0)
		{
			printListInfo(myList);
		}
		else if (strcmp(token, "deleteList") == 0)
		{
			deleteList(myList);
		}
		else
		{
			printf("Invalid command: <%s>\n", token);
		}
	}

	return 0;
}
