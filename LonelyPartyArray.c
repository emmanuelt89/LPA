// Emmanuel Tarantino
// NID: em083040
// COP3502 Szumlanski
// Assignment 02


/*
	A programmer's wife is having a baby. 
	The doctor immediately hands the newborn to the dad.
	The wife asks the dad, "Is it a boy or a girl?" 
	The programmer says, "Yes."
*/

#include <stdio.h>
#include <stdlib.h>
#include "LonelyPartyArray.h"


// ==============================================================================================
LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
{
	int i;

	LonelyPartyArray *party = malloc(sizeof(LonelyPartyArray));

	// if party malloc() fails, return NULL
	if (party == NULL || num_fragments <= 0 || fragment_length <= 0) 
	{
		return NULL;
	}

	party->size = 0;

	party->num_fragments = num_fragments; 

	party->fragment_length = fragment_length; 

	party->num_active_fragments = 0;

	party->fragments = malloc(sizeof(int *) * num_fragments); 

	// if party->fragments malloc() failes, free memory up to this point and return NULL
	if (party->fragments == NULL)
	{
		free(party);
		party = NULL;
		return NULL;
	}

	party->fragment_sizes = malloc(sizeof(int) * num_fragments);

	// if party->fragment_sizes malloc() failes, free memory up to this point and return NULL
	if (party->fragment_sizes == NULL)
	{
		free(party->fragments);
		party->fragments = NULL;

		free(party);
		party = NULL;

		return NULL;
	}

	for (i = 0; i < num_fragments; i++)
	{
		party->fragments[i] = NULL;
		party->fragment_sizes[i] = 0;
	}

	printf("-> A new LonelyPartyArray has emerged from the void. (capacity: %d, fragments: %d)\n",
		num_fragments * fragment_length, num_fragments);

	return party;
}


// ==============================================================================================
LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
{
	if (party != NULL)
	{
		int i;

		// loop to free every pointer in party->fragments and set to NULL
		for (i = 0; i < party->num_fragments; i++)
		{
				free(party->fragments[i]);
				party->fragments[i] = NULL;
		}

		// free all else dynamically allocated memory and set to NULL
		free(party->fragments);
		party->fragments = NULL;

		free(party->fragment_sizes);
		party->fragment_sizes = NULL;

		// set ints within party to 0
		party->size = 0;
		party->num_fragments = 0;
		party->fragment_length = 0;
		party->num_active_fragments = 0;

		// lastly free party and set to NULL
		free(party);
		party = NULL;

		printf("-> The LonelyPartyArray has returned to the void.\n");
	}
	
	return NULL;
}


// ==============================================================================================
int set(LonelyPartyArray *party, int index, int key)
{

	int i;
	int row = 0, col = 0;

	// NULL party check
	if (party == NULL)
	{		
		printf("-> Bloop! NULL pointer detected in set().\n");
		return LPA_FAILURE;
	}

	// set array row
	row = index / party->fragment_length;

	//set array col
	if (index < party->fragment_length && index > 0)
		col = index;
	else
		col = index % party->fragment_length;

	// out of bounds index check
	if (index < 0 || index > (party->num_fragments * party->fragment_length - 1))
	{
		printf("-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n",
			index, row, col);
		return LPA_FAILURE;
	}

	// check if party->fragments[] is NULL, increase num_active_fragments if it isn't and malloc()
	if (party->fragments[row] == NULL) 
	{		
		party->fragments[row] = malloc(sizeof(int *) * party->fragment_length);

		// malloc() fail return
		if (party->fragments[row] == NULL)
			return LPA_FAILURE;

		party->num_active_fragments++;

		for (i = 0; i < party->fragment_length; i++) 
			party->fragments[row][i] = UNUSED;

		// successfull fragment created output
		printf("-> Spawned fragment %d. (capacity: %d, indices: %d..%d)\n", 
			row, party->fragment_length, (party->fragment_length * row), (party->fragment_length * (row + 1) - 1));
	}

	// check if cell is UNUSED, if so, increase size and fragment_sizes[row] by 1
	if (party->fragments[row][col] == UNUSED)
	{
		// increase party->size, increase party->fragment_sizes 
		party->size++;
		party->fragment_sizes[row]++;		
	}

	// set the value at index
	party->fragments[row][col] = key;

	return LPA_SUCCESS;
}

// ==============================================================================================
int get(LonelyPartyArray *party, int index)
{
	int row = 0, col = 0;

	// check if party is NULL
	if (party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in get().\n");
		return LPA_FAILURE;
	}

	// get array row
	row = index / party->fragment_length;

	// get array col
	if (index < party->fragment_length && index > 0)
		col = index;
	else
		col = index % party->fragment_length;

	// out of bounds index check
	if (index < 0 || index > (party->num_fragments * party->fragment_length - 1))
	{
		printf("-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n",
			index, row, col);
		return LPA_FAILURE;
	}

	// if within bounds of array and *fragments is NULL, by definition the index is not being used
	if (party->fragments[row] == NULL)
		return UNUSED;

	return party->fragments[row][col];
}


// ==============================================================================================
int delete(LonelyPartyArray *party, int index)
{
	int row = 0, col = 0;

	// check if party is NULL
	if (party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in delete().\n");
		return LPA_FAILURE;
	}

	// set array row
	row = index / party->fragment_length;

	//set array col
	if (index < party->fragment_length && index > 0)
		col = index;
	else
		col = index % party->fragment_length;

	// out of bounds index check
	if (index < 0 || index > (party->num_fragments * party->fragment_length - 1))
	{
		printf("-> Bloop! Invalid access in delete(). (index: %d, fragment: %d, offset: %d)\n", 
			index, row, col);
		return LPA_FAILURE;
	}	

	// check if fragment is not allocated
	if (party->fragments[row] == NULL)
		return LPA_FAILURE;

	// set value at index to UNUSED and decrease struct's size and fragment_sizes
	if (party->fragments[row][col] != UNUSED)
	{
		party->fragments[row][col] = UNUSED;
		party->size--;
		party->fragment_sizes[row]--;

		// if deleting index causes fragment_sizes array == 0, free pointer and set to null
		if (party->fragment_sizes[row] == 0)
		{
			free(party->fragments[row]);
			party->fragments[row] = NULL;
			party->num_active_fragments--;

			printf("-> Deallocated fragment %d. (capacity: %d, indices: %d..%d)\n", 
			row, party->fragment_length, party->fragment_length * row, party->fragment_length * (row * 2) - 1);	
		}
		}
	// if fragment was already set to UNUSED
	else 
		return LPA_FAILURE;

	return LPA_SUCCESS;
}


// ==============================================================================================
int printIfValid(LonelyPartyArray *party, int index)
{
	int row = 0, col = 0;

	// check if party is NULL
	if (party == NULL)
		return LPA_FAILURE;

	// set array row
	row = index / party->fragment_length;

	//set array col
	if (index < party->fragment_length)
		col = index;
	else
		col = index % party->fragment_length;

	// out of bounds index check
	if (index < 0 || index > (party->num_fragments * party->fragment_length - 1))
		return LPA_FAILURE;

	// check if fragment is not allocated
	if (party->fragments[row] == NULL)
		return LPA_FAILURE;

	// check if index is UNUSED
	if (party->fragments[row][col] == UNUSED)
		return LPA_FAILURE;

	printf("%d\n", party->fragments[row][col]);

	return LPA_SUCCESS;
}


// ==============================================================================================
LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *party)
{
	int i, j;

	// check for NULL party
	if (party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in resetLonelyPartyArray().\n");
		return party;
	}

	// set any values on fragment arrays that are not NULL to UNUSED
	for (i = 0; i < party->num_fragments; i++)
	{
		if (party->fragments[i] != NULL)
			for (j = 0; j < party->fragment_length; j++)
				party->fragments[i][j] = UNUSED;
	}

	// de-allocate all fragments and reset each corresponding fragment_size value to 0
	for (i = 0; i < party->num_fragments; i++)
	{
		free(party->fragments[i]);
		party->fragments[i] = NULL;
		party->fragment_sizes[i] = 0;
	}

	party->size = 0;

	party->num_active_fragments = 0;

	printf("-> The LonelyPartyArray has returned to its nascent state. (capacity: %d, fragments: %d)\n", 
		(party->num_fragments * party->fragment_length), party->num_fragments);

	return party;
}


// ==============================================================================================
int getSize(LonelyPartyArray *party)
{

	if (party == NULL)
		return -1;
	else
		return party->size;
}


// ==============================================================================================
int getCapacity(LonelyPartyArray *party)
{

	if (party == NULL)
		return -1;
	else
		return (party->fragment_length * party->num_fragments);
}


// ==============================================================================================
int getAllocatedCellCount(LonelyPartyArray *party)
{

	if (party == NULL)
		return -1;
	else
		return (party->fragment_length * party->num_active_fragments);
}


// ==============================================================================================
long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
{

	if (party == NULL)
		return 0;
	else
		return (long long unsigned int)((party->fragment_length * party->num_fragments) * sizeof(int));
}


// ==============================================================================================
long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
{
	if (party == NULL)
		return 0;
	else
	{
		long long unsigned int total = 0;

		// bytes taken up by the LPA pointer
		total += (long long unsigned int)(sizeof(LonelyPartyArray *));

		// byts taken up by the LPA struct
		total += (long long unsigned int)(sizeof(LonelyPartyArray));

		// bytes taken up by fragments array
		total += (long long unsigned int)(sizeof(int *) * party->num_fragments);

		// bytes taken up by fragment_sizes
		total += (long long unsigned int)(sizeof(int) * party->num_fragments);

		// bytes taken up by active fragments (bytes by all integers cells in the individual array fragments)
		total += (long long unsigned int)(sizeof(int) * (party->num_active_fragments * party->fragment_length));

		return total;
	}
}


// ==============================================================================================
double difficultyRating(void)
{
	return 4.0;
}


// ==============================================================================================
double hoursSpent(void)
{
	return 16.0;
}


// ==============================================================================================
LonelyPartyArray *cloneLonelyPartyArray(LonelyPartyArray *party)
{
	int i = 0, j = 0;

	// check if party passed in is NULL
	if (party == NULL)
	{
		return NULL;
	}

	// allocate space for new LPA struct
	LonelyPartyArray *clonedParty = malloc(sizeof(LonelyPartyArray));
	// check if malloc() failed
	if (clonedParty == NULL)
	{
		return NULL;
	}

	// copy int values from original party to cloned
	clonedParty->size = party->size;
	clonedParty->num_fragments = party->num_fragments;
	clonedParty->fragment_length = party->fragment_length;
	clonedParty->num_active_fragments = party->num_active_fragments;

	// allocate space for new LPA struct's **fragments
	clonedParty->fragments = malloc(sizeof(int *) * clonedParty->num_fragments); 

	// if clonedParty->fragments malloc() failes, free memory up to this point and return NULL
	if (clonedParty->fragments == NULL)
	{
		free(clonedParty);
		clonedParty = NULL;
		return NULL;
	}

	// allocate space for new LPA struct's *fragment_sizes
	clonedParty->fragment_sizes = malloc(sizeof(int) * clonedParty->num_fragments);

	// if clonedParty->fragment_sizes malloc() failes, free memory up to this point and return NULL
	if (clonedParty->fragment_sizes == NULL)
	{
		free(clonedParty->fragments);
		clonedParty->fragments = NULL;

		free(clonedParty);
		clonedParty = NULL;

		return NULL;
	}

	// copy all values of party passed into function to cloned one, malloc() *fragment[i] if need be
	for (i = 0; i < clonedParty->num_fragments; i++)
	{
		clonedParty->fragment_sizes[i] = party->fragment_sizes[i];

		if (clonedParty->fragment_sizes[i] > 0)
		{
			clonedParty->fragments[i] = malloc(sizeof(int *) * clonedParty->fragment_length);

			for (j = 0; j < clonedParty->fragment_length; j++)
			{
				clonedParty->fragments[i][j] = party->fragments[i][j];
			}
		}
	}

	printf("-> Successfully cloned the LonelyPartyArray. (capacity: %d, fragments: %d)\n", 
		(clonedParty->num_fragments * clonedParty->fragment_length), clonedParty->num_fragments);

	return clonedParty;
}