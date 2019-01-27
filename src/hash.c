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
#include "../include/functions.h" // global functions declarations

// ------------------------------------------------------------------------
// inner functions declarations
// ------------------------------------------------------------------------


//------------------------------------------------------------------------
// global functions definitions
//------------------------------------------------------------------------

/**
   create and initialize hash table

   returns : pointer to table or NULL if creation error
*/
hash_table *
create_table()
{	
	hash_table *htable_ptr = malloc (sizeof (hash_table));
	if(htable_ptr == NULL)
	{
		return NULL;
	}
	htable_ptr->htable = malloc (sizeof (word_list) * MAX_ENTRIES);
	if(htable_ptr->htable == NULL)
	{
		return NULL;
	}
	htable_ptr->hsize = MAX_ENTRIES;
	int i;
	for(i = 0 ; i < htable_ptr->hsize ; i++)
	{
		htable_ptr->htable[i].first_word = NULL;
		htable_ptr->htable[i].last_word = NULL;
	}
  return htable_ptr;
}

/**
   search a word in table ; print word if found, with number of occurrences
   and file where word is found

   parameters :
   word : the word to look for
   filelist   : pointer to table of files
   htable_ptr : pointer to hash table

   returns : true if found, false otherwise
*/
int
search_word(char word[], listfile_entry * filelist, hash_table * htable_ptr)
{
	// Partie Hache Table
	int hashCode = hashcode(word, MAX_ENTRIES);
	int isHere = -1;
	word_list * word_index = &htable_ptr->htable[hashCode];
	if(word_index->first_word == NULL)
	{
		printf("null\n");
		return -1;
	}
	else
	{
		word_entry * mot = word_index->first_word;
		while(mot != NULL)
		{
			if(strcmp(mot->word, word) == 0) // Le mot à été trouvé
			{
				printf("The word %s has been find in the file '%s' and %d times\n",word, filelist[mot->in_file].filename, mot->times);
				isHere = 0;
			}
			mot = mot->next;
		}
		return isHere;
	}
}

/**
   print table contents

   parameters :
   htable_ptr : pointer to hash table
   filelist   : pointer to table of files
*/
void
print_table(hash_table * htable_ptr, listfile_entry * filelist)
{
	printf("\n \n Voici le dictionnaire : \n \n");
	int i;
	for(i = 0 ; i < MAX_ENTRIES ; i++)
	{
		word_list word = htable_ptr->htable[i];
		word_entry *mot = word.first_word;
		if(mot != NULL)
		{
			while(mot != NULL)
			{
				printf("[%d] Word : %s Filename : %s Times : %d\n", i, mot->word, filelist[mot->in_file].filename, mot->times);
				mot = mot->next;
			}
		}
	}
}


/**
   free hash table

   parameters :
   htable_ptr : pointer to hash table
*/
void
free_table(hash_table * htable_ptr)
{
	int isRemoved;
	int i;
	for(i = 0 ; i < MAX_ENTRIES ; i++)
	{
		isRemoved = 1;
		word_list * word = &htable_ptr->htable[i];
		if(word->first_word != NULL)
		{	
			word_entry *mot = word->first_word;
			while((mot != NULL) && isRemoved == 1)
			{
				if(mot->next == NULL)
				{
					free(mot);
					isRemoved = 0;
				}
				else
				{
					word_entry *mot_prev = mot;
					mot = mot->next;
					free(mot_prev);
				}
			}
			word->first_word = NULL;
			word->last_word = NULL;
		}
	}
}

// ------------------------------------------------------------------------
// inner functions definitions
// ------------------------------------------------------------------------

// TO BE COMPLETED
