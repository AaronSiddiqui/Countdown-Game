#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define SIZE 9

int amount, length;
char CDletters[SIZE];
char **longestWords;		//Double pointer to allocate memory based on how many strings there are and the size of each string
char *fileName = "C:\\Countdown\Dictionary\webster.txt";		//Dictionary file

int instructions();
int testLetter(char lett, int num);
int generateConsonant(int val);
int generateVowel(int val);
int readFile();
int initialiseWords(int num);
int copyWords(int val);
int checkWord(char *word);
int printWords();

int main()
{
	int i;
	int count;
	char letter;
	char userWord[SIZE];

	srand(time(NULL));

	instructions();		//Instructions

	count = 0;

	printf("\nWould you like a consonant or a vowel(Enter either c or v)? ");
	scanf("%c", &letter);
	count = testLetter(letter, count);		//To ensure the user entered c or v and incremented count (size of the array)

	while (count < SIZE - 1) {
		scanf("%c", &letter);

		if (letter != '\n') count = testLetter(letter, count);
	}

	CDletters[SIZE - 1] = '\0';
	
	printf("\nYour letters are: \n");
	printf("----------------\n");
	
	for (i = 0; i < SIZE - 1; i++) printf("%c ", CDletters[i]);		//Printing the Countdown letters

	printf("\n----------------\n\n");

	for (i = 0; i < SIZE - 1; i++) CDletters[i] = CDletters[i] + 32;	//Changing the letter to lowercase
	
	readFile();		//Reads the dictionary file and finds the longest words from the Countdown letters

	printf("Enter the longest word possible: \n");
	scanf("%s", userWord);

	while (checkWord(userWord) != 2) {		//To ensure the user used the Countdown letters and didn't fabricate their word
		printf("Enter the longest word possible: \n");
		scanf("%s", userWord);
	}
	
	printWords();	//Prints the longest words

	if (strlen(userWord) == length) printf("Congratulations! You have the highest letter word!\n");
	else printf("Close! You are %d letter(s) off the highest letter word!\n", length - strlen(userWord));
	
	free(longestWords);		//Deallocates memory

	return 0;
}

int instructions()
{
	printf("Welcome to Countdown!\n\n");
	printf("The objective of the game is to produce the largest word from the eight letters(consonants and/or vowels) that are chosen at random by you. ");
	printf("The computer will then find the longest word available from these letters to compare with your word.\n");
	printf("Let's begin!\n");

	return 1;
}

int testLetter(char letter, int num)
{
	if (letter == 'c')
	{
		generateConsonant(num);	//Generated a consonant if the user entered c and increments
		num++;

		if (num < SIZE - 1) printf("\nWould you like another consonant or a vowel? ");
	}
	else if (letter == 'v')
	{
		generateVowel(num);	//Generated a vowel if the user entered v and increments
		num++;

		if (num < SIZE - 1) printf("\nWould you like another consonant or a vowel? ");
	}
	else
	{
		printf("\nERROR! Please enter either c or v: ");	//Returned an error if the user didn't enter c or v
	}

	return num;
}

int generateConsonant(int val)
{
	char consonants[22] = "BCDFGHJKLMNPQRSTVWXYZ";

	CDletters[val] = consonants[rand() % 21];

	return 1;
}

int generateVowel(int val)
{
	char vowels[6] = "AEIOU";

	CDletters[val] = vowels[rand() % 5];

	return 1;
}

int readFile()
{
	int i, j, k;
	int len;
	long int n[SIZE - 1] = { 0 };	//An array to store the amount of each number of letters e.g. n[2] stores three lettered words
	char line[SIZE + 2];
	char temp[SIZE];

	FILE *fp = fopen(fileName, "r");

	if (fp == NULL)
	{
		printf("Error opening file!\n");
	}
	else
	{
		while (fgets(line, sizeof(line), fp)) {
			k = 0;
			len = strlen(line) - 1;		//-1 because of '\n'
			
			strcpy(temp, CDletters);	//Copied the letters to a temporary string

			for (i = 0; i < len; i++)
			{
				for (j = 0; j < SIZE - 1; j++)
				{
					if (line[i] == temp[j])
					{
						temp[j] = NULL;		//If the character is found I eliminate it from the temporary string to prevent duplicated letters from affecting the results
						k++;	//k is incremented every time a letter in the temporary string is the same as the letter in line
						break;
					}
				}
			}

			for (i = SIZE - 1; i > 0; i--)
			{
				if (k == i && len == i)		//If k is equal to i and the same length as i, an n value is incremented e.g. n[4]++ if a 5 letter word is found. The reason I also use strlen in the if condition is because sometimes it may find a word has the same mutual amount of letters but it is a longer length e.g. the longest word for "nailhobq" is hobnail but it may also increment with hobnails
				{
					n[i - 1]++;
					break;
				}
			}
		}

		for (i = SIZE - 2; i >= 0; i--)
		{
			if (n[i] != 0)
			{
				initialiseWords(n[i]);		//Allocates memory to **longestWords based on the size of n[i]
				copyWords(i);		//This function is then used to copy the longest words to **longestWords
				amount = n[i];		//To declare the size of **longestWords as a global variable
				length = i + 1;		//To declare the length of a word in **longestWords as a global variable
				break;
			}
		}
	}

	fclose(fp);

	return 1;
}

int initialiseWords(int num)
{
	int i;

	longestWords = (char**)malloc(num * sizeof(char*));

	for (i = 0; i < num; i++) longestWords[i] = (char*)malloc((SIZE)* sizeof(char));

	return 1;
}

int copyWords(int val)
{
	int i, j, k, l;
	int len;
	char line[SIZE + 2];
	char temp[SIZE];

	FILE *fp = fopen(fileName, "r");

	if (fp == NULL)
	{
		printf("Error opening file!\n");
	}
	else
	{	
		l = 0;

		while (fgets(line, sizeof(line), fp)) {
			k = 0;
			len = strlen(line) - 1;

			strcpy(temp, CDletters);

			for (i = 0; i < len; i++)
			{
				for (j = 0; j < SIZE - 1; j++)
				{
					if (line[i] == temp[j])
					{
						temp[j] = NULL;		//Repeat the same process as readFile()
						k++;
						break;
					}
				}
			}

			if (k == val + 1 && len == val + 1)		//val is the highest value of the i in n[i], therefore the highest lettered word
			{
				strcpy(longestWords[l], line);	//Copy the word to longestWords and increment
				l++;
			}
		}
	}

	fclose(fp);

	return 1;
}

int checkWord(char *word)
{
	int i, j;
	int len, numMatch;
	char line[SIZE + 2];
	char temp[SIZE];

	len = strlen(word);
	strcpy(temp, CDletters);

	for (i = 0; i < len; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			if (word[i] == temp[j])
			{
				temp[j] = NULL;		//Check if the user is using letters that aren't in the Countdown letters
				break;
			}
			else if (j == 8)
			{
				printf("\nStop Cheating! That word doesn't have all the Countdown letters!\n\n");
				return 1;
			}
		}
	}

	FILE *fp = fopen(fileName, "r");

	numMatch = 0;

	if (fp == NULL)
	{
		printf("Error opening file!\n");
	}
	else
	{
		while (fgets(line, sizeof(line), fp)) {
			if (strncmp(line, word, len) == NULL) numMatch++;	//Check to see if the user isn't fabricating words 
		}
	}

	if (numMatch == 0)
	{
		printf("\nStop Cheating! That word doesn't exist!\n\n");
		return 1;
	}

	fclose(fp);

	return 2;
}

int printWords()
{
	int i;
	
	if (amount == 1) printf("\nThere is %d %d lettered word and it is: \n\n", amount, length);
	else printf("\nThere are %d %d lettered words and they are: \n\n", amount, length);

	for (i = 0; i < amount; i++) printf("%20s\n", longestWords[i]);

	return 1;
}