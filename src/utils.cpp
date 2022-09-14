#include "utils.h"

char** split(const char* char_array, const char delimiter, int& splits) {
	// Get length of char array
	int length = sizeof(char_array) / sizeof(char);

	// Initialise indexes of delimiters
	int indexes[length];
	// Do as if the character before the first character is a delimiter
	indexes[0] = -1;
	int nFound = 1;

	int i = 0;

	// Find all delimiters
	for (i = 0; i < length; i++) {
		if (char_array[i] == delimiter) {
			indexes[nFound] = i;
			nFound++;
		}
	}
	// Do as if the character after the last character is a delimiter
	indexes[nFound] = length;

	//Create return 2D array
	char** split_array = (char**) malloc(nFound*length*sizeof(char));

	// Copy the substring to the return array
	for (i = 0; i < nFound; i++) {
		memcpy(split_array[i], char_array + indexes[i] + 1, indexes[i + 1] - indexes[i] - 1);
	}

	// Update the number of splits found
	splits = nFound;

	return split_array;
}