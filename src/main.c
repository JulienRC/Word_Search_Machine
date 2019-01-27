/**************************************************************************
 * L3Informatique						C/Unix
 * 			TP Hash Map
 *
 * Group 	: GROUPE 3.1
 * Name 1 : David RENARD CALZANT
 * Name 2 :	Julien ROYON CHALENDARD
 *
 **************************************************************************/



#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/functions.h"

//------------------------------------------------------------------------

int
main()
{
  // create hash table
	hash_table * htable_ptr = create_table();

  // create filelist array
	listfile_entry *filelist = create_filelist(MAX_FILES);

  // display menu
  while (1) 
  {
    int nbchoices = 0;
    fprintf(stderr, "\nChoisir une action\n");
    fprintf(stderr, "%d. Load a file in dictionary\n", ++nbchoices);
    fprintf(stderr, "%d. Search a word in dictionary\n", ++nbchoices);
    fprintf(stderr, "%d. Remove file from dictionary\n", ++nbchoices);
    fprintf(stderr, "\n");
    fprintf(stderr, "%d. Print dictionary\n", ++nbchoices);
    fprintf(stderr, "%d. Print file list\n", ++nbchoices);
    fprintf(stderr, "\n0. Quit\n");
    int choice;
    while (1) 
    {
      fprintf(stderr, "Your choice ? ");
      scanf("%d", & choice);
      if (choice >= 0 && choice <= nbchoices) { break; }
      fprintf(stderr, "\nError %d is an incorrect choice\n", choice);
    }
    if (choice == 0) { break; }

    fprintf(stderr, "-------------------------------------------------\n");

    // TO BE COMPLETED
	char filename[50];
	char word[50];
	
	// CASE DELETE
	int word_delete = 0;
	int test = 0;
	int number = 0;
    switch (choice) 
    {
      // Load a file in dictionary
    case 1:
        printf("Veuillez saisir le nom du fichier à entrer : ");
        int load = scanf("%s", filename);
		add_file(filename, filelist, htable_ptr);
		break;

      // Search a word in dictionary
    case 2:

		printf("Veuillez saisir le mot à chercher : ");
        int search = scanf("%s49", word);
		search_word(word, filelist, htable_ptr);
      break;

      // Remove file from dictionary
    case 3:

		printf("Veuillez saisir le fichier à supprimer : ");
        word_delete = scanf("%s49", word);
		remove_file(word, filelist, htable_ptr);
		break;

      // Print dictionary
    case 4:

      print_table(htable_ptr, filelist);
      break;

      // Print file list
    case 5:
    
		print_list(filelist);
		break;
    }
    fprintf(stderr, "-------------------------------------------------\n");

  }

  // the end : free allocated memory
	printf("\n \nA bientôt !\n");
	free(htable_ptr);
	free_filelist(filelist);

  return 0;
}

// compute hash value for word
// returns : N ; 0 <= N < size
int
hashcode(char word[], int size)
{
	int hashCode = 0;
	int i;
	for(i = 0 ; i < strlen(word) ; i++)
	{
		hashCode += word[i];
	}
	hashCode = hashCode % size;
	return hashCode;
}
