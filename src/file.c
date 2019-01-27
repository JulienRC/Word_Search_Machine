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
#include "../include/functions.h" // extern functions declarations

// ------------------------------------------------------------------------
// inner functions declaratheaderions
// ------------------------------------------------------------------------

int load_file(char filename[], listfile_entry * filelist, int indice, hash_table * htable_ptr, FILE *fichier);
void remove_file_from_filelist(char filename[], listfile_entry * filelist, int i);
//------------------------------------------------------------------------
// global functions definitions
//------------------------------------------------------------------------

/**
   Create and initialize file table of capacity maxfiles

   parameters :
   maxfiles : capacity of file table

   returns : pointer to table or NULL in case of error
 */
listfile_entry *
create_filelist(int maxfiles)
{
	listfile_entry * filelist = malloc(sizeof(listfile_entry) * maxfiles);
	int i;
	for(i = 0 ; i < maxfiles ; i++)
	{
		strcpy(filelist[i].filename, "");
		filelist[i].loaded = 0;
	}
  return filelist;
}

/**
   add words from file to table
   - checks if the file has already been loaded
   - updates the file table (if file not already loaded)
   - reads every word in the file (idem)
   - updates the hash table (idem)

   parameters :
   filename   : name of file :)
   filelist   : pointer to table of files
   htable_ptr : pointer to hash table
word_list word = htable_ptr->htable[hashCode];
   returns :
    1 if file already present in table
    2 if no space left in filelist
   -1 if file doesn't exist or can't be read
   -2 if allocation error
    0 if everything ok
*/
int
add_file(char filename[], listfile_entry * filelist, hash_table * htable_ptr)
{
	FILE *fichier = NULL;
	fichier = fopen(filename,  "r");
	if (fichier == NULL) // On test si le fichier a bien été lu 
	{
		printf("le fichier n'existe pas !\n");
		return -1;
	}
    int j;
	for(j = 0; j < MAX_FILES; j++) // Test si le fichier est déjà présent 
	{
		if(filelist[j].loaded != 0) // Si un filelist_entry a son loaded à 0
		{
			if((strcmp(filename, filelist[j].filename) == 0)) // On test si filename et le filename dans le filelist sont les même 
			{
				printf("Le fichier a déjà été chargé\n");
				return 1;
			}
		}
	} //Le fichier n'a pas été trouver donc on peut l'ajouter ! 
	int i;
	for(i = 0; i < MAX_FILES; i++) // 
	{
		if(filelist[i].loaded == 0) // On trouve un emplacement vide
		{
			return load_file(filename, filelist, i,  htable_ptr, fichier);
		}
	}
	printf("Il n'y a plus de place disponible dans filelist\n");
	return 2;
}

/**
   remove file from file table

   parameters :
   filename   : name of file to remove
   filelist   : pointer to table of files
   htable_ptr : pointer to hash table

   returns :
   -1 if file not in table
    0 if file removed
*/
int
remove_file(char filename[], listfile_entry * filelist, hash_table * htable_ptr)
{
    int isRemoved = 1;
    int i;
    for(i = 0 ; i < MAX_FILES && isRemoved == 1 ; i ++) //Parcours de filelist
    {
        if(strcmp(filelist[i].filename, filename) == 0) //Recherche du fichier
        {
            if(filelist[i].loaded == 1) //Le fichier a bien été chargé
            {
                FILE *fichier = NULL;
                fichier = fopen(filename,  "r");
                if (fichier == NULL) //Vérification du fichier
                {
                    printf("Le fichier n'existe pas !\n");
                    return -1;
                }
                char chaine[MAX_LENGTH] = "";
                while(fgets(chaine, MAX_LENGTH, fichier) != NULL)
                {
					char * tmp = strchr(chaine, '\n');
					if(tmp) *tmp = '\0';
                    int hashCode = hashcode(chaine, MAX_ENTRIES);
                    word_list * word = &htable_ptr->htable[hashCode];
                    word_entry *current = word->first_word;
                    word_entry *pred = current;
                    int avancer = 0;
                    while(current != NULL)
                    {
                        if( (strcmp(chaine, current->word) == 0) 
                        && (strcmp(filename, filelist[current->in_file].filename) == 0) )
                        {
                            // Cas de first_word && last_word à supprimer
                            if(current == word->first_word && current == word->last_word)
                            {
                                word->first_word = NULL;
                                word->last_word = NULL;
                                free(current);
                                break;
                            }
                            // Cas de first_word à supprimer
                            else if(current == word->first_word)
                            {
                                word->first_word = current->next;
                                free(current);
                                break;
                            }
                            // Cas de last_word à supprimer
                            else if(current == word->last_word)
                            {
                                word->last_word = pred;
                                pred->next = NULL;
                                free(current);
                                break;
                            }
                            // Cas d'un mot en milieu de liste
                            else
                            {
                                pred->next = current->next;
                                free(current);
                                break;
                            }
                        }
                        current = current->next;
                        if(avancer != 0)
                        {
							pred = pred->next;
						}
						avancer++;
					}
                }
                remove_file_from_filelist(filename, filelist, i);  // Supprime le fichier dans filelist
                isRemoved = 0;  
            }
        }
    }
    if(isRemoved == 1)
    {
		printf("Le fichier n'est pas dans le dictionnaire !\n");
		return -1;
	}
    return 0;
}

/*
  print file table (only loaded files)

  parameters :
   filelist : pointer to table of files
*/
void
print_list(listfile_entry * filelist)
{
	int i;
	for(i = 0 ; i < MAX_FILES ; i++)
	{
		if(filelist[i].loaded == 1)
		{
			printf("Ligne %d, nom du fichier %s\n", i, filelist[i].filename);
		}
	}
}

/**
   free file table

parameters :
   filelist   : pointer to table of files
*/
void
free_filelist(listfile_entry * filelist)
{
	int i;
	for(i = 0 ; i < MAX_FILES ; i++)
	{
		strcpy(filelist[i].filename, "");
		filelist[i].loaded = 0;
	}
	free(filelist);
}

// ************************************************************************
// inner functions
// ************************************************************************

/**
 *  load all the words of the file in wordlist
 * 
 * 	parameters :
 *	filename   : name of file :)
 *	filelist   : pointer to table of files
 *	htable_ptr : pointer to hash table
 * 
 * 	returns :
 *	-1 if file doesn't exist or can't be read
 * 	-2 if allocation error
 * 	0 if everything ok
*/
int load_file(char filename[], listfile_entry * filelist, int indice, hash_table * htable_ptr, FILE *fichier)
{
	strcpy(filelist[indice].filename, filename); // On copie à l'endroit filelist[indice] le nom de fichier qu'on a saisi
	filelist[indice].loaded = 1; // On dit qu'il y a un fichier chargé à l'intérieur 
	
	char chaine[MAX_LENGTH] = "";
	while(fgets(chaine, sizeof chaine, fichier) != NULL)
	{
		char * tmp = strchr(chaine, '\n');
		if(tmp) *tmp = '\0';
		int hashCode = hashcode(chaine, MAX_ENTRIES);
		word_list * word = &htable_ptr->htable[hashCode]; // On crée une variable de type word_list qui est la word_list à l'emplacement indice
		if(word->first_word == NULL) // On regarde s'il y a au moins un mot dedans
		{
			word_entry *mot = malloc(sizeof(word_entry)); // On crée un word_entry
			if(mot == NULL) // Il n'y a plus de place dans la mémoire pour placer un mot
			{
				printf("Plus de place");
				return -2;
			}
			strcpy(mot->word, chaine); // On copie dans mot->word la chaine
			mot->in_file = indice; // On indique l'indice où se trouve le fichier dans filelist
			mot->times = 1; // Nombre de fois qu'on l'a ajouté
			mot->next = NULL;
			word->first_word = mot; // On dit que ce premier mot est le premier et donc le dernier aussi
			word->last_word = mot;
		}
		else // Il y a déjà un ou plusieurs mots à cet emplacement 
		{
			int isAdd = 1;
			word_entry *mot = word->first_word;
			while(mot != NULL) // On parcourt nos mots juqu'à trouver le dernier ou un mot qu'on a déjà mis et du même fichier !!
			{
				if((strcmp(mot->word, chaine) == 0) && (strcmp(filelist[mot->in_file].filename, filename) == 0))
				{ // On trouve un mot qu'on a déjà ajouté et qui provient du même fichier !
					mot->times += 1; // On augmente juste le nombre de fois qu'on l'a trouvé
					isAdd = 0;
				}
				mot = mot->next; // On passe au mot suivant 
			}
			if(isAdd == 1)
			{
				word_entry *newWord = malloc(sizeof(word_entry)); // On crée un nouveau mot juste après la fin
				if(newWord == NULL) // 
				{
					return -2;
				}
				strcpy(newWord->word, chaine); // On copie 
				newWord->in_file = indice;
				newWord->times = 1;
				newWord->next = NULL;
				word->last_word->next = newWord;
				word->last_word = newWord;
			}
		}
	}
	printf("\nFichier ajouté\n");
	return 0;	
}

void
remove_file_from_filelist(char filename[], listfile_entry * filelist, int i)
{
	strcpy(filelist[i].filename, "");
	filelist[i].loaded = 0;
}
