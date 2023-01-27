/*************************************************************
* fsm.c - This program can be used to validate a stream of
* input according to a system of message encryption. The
* program prints output based on the validity of the messages.
*
* Mustafa Teber / mst89
* November 29, 2022
* 
* 5.4.0-126-generic x86_64 GNU/Linux
* Ubuntu 20.04.5 LTS
* gcc (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0
*
*************************************************************/

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Function declarations here
int validateAldo( char str[] );
int validateBar( char str[] );
int validateCalma( char str[] );
int validateDol( char str[] );

/********************************************************
*
*  validateMessage - This function takes a string message
*  as input and return 1 if message is valid. This main
*  validation function delegates the responsibility of 
*  validation to functions for specific message types.
*
********************************************************/

int validateMessage( char str[] ){
	char first = str[0];
	if(first == 'A'){
		return validateAldo( str );
	} else if (first == 'B'){
		return validateBar( str );
	} else if (first == 'C'){
		return validateCalma( str );
	} else if (first == 'D'){
		return validateDol( str );
	} else {
		return 0;
	}
}

/********************************************************
*
*  validateAldo - This function takes a string message
*  as input and return 1 if message is a valid aldo 
*  message and 0 otherwise.
*
********************************************************/

int validateAldo( char str[] ){
	for (int i=1; i<strlen(str)-1; i++) {
      if (!isdigit( str[i] )){
			return 0;
			}
		}
	if (str[strlen(str)-1] == 'F' && strlen(str)>=3 && str[0] == 'A'){
		return 1;
	} else {
		return 0;
	}

}  

/********************************************************
*
*  validateBar - This function takes a string message
*  as input and return 1 if message is a valid bar
*  message and 0 otherwise.
*
********************************************************/

int validateBar( char str[]){
	for (int i=0; i <strlen(str); i++) {
		if( str[i] == 'B' && str[i+1] != 'H' && str[i+1] != '\0'){
			return 0;
		} else if( str[i] == 'H' && str[i+1] != 'G' ){
			return 0;
		} else if( str[i] == 'G' && str[i+1] != 'X' && str[i+1] != '\0'){
         return 0;
		} else if( str[i] == 'X' && str[i+1] != 'H'){
         return 0;
      } else if( str[i] != 'G' && str[i] != 'H' && str[i] != 'X' && str[i] != 'B'){
			return 0;
		}
	}
	return 1;
}

/********************************************************
*
*  validateCalma - This function takes a string message
*  as input and return 1 if message is a valid calma
*  message and 0 otherwise.
*
********************************************************/

int validateCalma( char str[]){
	int count = 0;
	for (int i=1; i <strlen(str); i++) {
		if ( str[i]!='R' && str[i]!='T') {
			return 0;
		}
		if ( str[i]=='T') {
         count ++;
      }
	} 
	if (count%2==0){
		return 1;
	} else {
		return 0;
	}
}

/********************************************************
*
*  validateDol - This function takes a string message
*  as input and return 1 if message is a valid dol
*  message and 0 otherwise.
*
********************************************************/

int validateDol( char str[]){
	if (str[0] != 'D' || !isdigit( str[1] ) || !isdigit( str[2] )){
      return 0;
   }
	for (int i=3; i <strlen(str); i++) {
      str[i-3] = str[i];
      }
	str[strlen(str)-3] = '\0';
	if (str[0]=='A'){
		validateAldo( str );
	} else if (str[0]=='C'){
      validateCalma( str );
   } else {
		return 0;
	}
}

/********************************************************
*
* main - This is the main function. This function may take
* a command line argument which is a file to read or if no
* argument is provided it will read stdin. It will read each
* line and print OK or FAIl based on if the line read is a
* valid message.
*
********************************************************/

int main( int argc, char **argv )
{
	FILE* fp = NULL ;
	char* buff = NULL ;
	size_t len = 0 ;

	if( argc == 2 ) 
	{
		fp = fopen( argv[1], "r" ) ;

		if( fp == NULL )
		{
			printf( "Can not open input file for reading.\n" ) ;
			exit( 1 ) ;
		}
	}
	else if( argc > 2){
		printf( "Too many arguments!\n" ) ;
      exit( 1 ) ;
	} else {
		fp = stdin ;
	}

	while( getline( &buff, &len, fp ) != -1 )
	{
		char str[100];
		char msg[100];
		sscanf( buff, "%s", str);
		strcpy(msg,str);
		int isValid = validateMessage( str );
		if( isValid ){
			printf("%s OK\n",msg);
		} else {
			printf("%s FAIL\n",msg);
			}
   }
	free( buff ) ; 

	buff = NULL ;  
	len = 0 ;
	
	return 0 ;
}
