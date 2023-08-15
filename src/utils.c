#include "utils.h"

struct mg_str* split(const struct mg_str char_array, const char delimiter, int* splits) {
	// Initialise indexes of delimiters
	int indexes[char_array.len];
	// Do as if the character before the first character is a delimiter
	size_t nFound = 0;

	size_t i = 0;

	// Find all delimiters
	for (i = 0; i < char_array.len; i++) {
		if (char_array.ptr[i] == delimiter) {
			indexes[nFound] = i;
			nFound++;
		}
	}
	// Do as if the character after the last character is a delimiter
	indexes[nFound] = char_array.len;

	//Create return 2D array
	struct mg_str* split_array = (struct mg_str*) malloc(nFound * sizeof(struct mg_str));

	// Copy the substring to the return array
	for (i = 0; i < nFound; i++) {
		split_array[i].ptr = (char*)malloc((indexes[i + 1] - indexes[i]) * sizeof(char));
		split_array[i].len = indexes[i + 1] - indexes[i];
		memcpy((char*)split_array[i].ptr, char_array.ptr + indexes[i] + 1, (split_array[i].len - 1) * sizeof(char));
		((char*)split_array[i].ptr)[split_array[i].len - 1] = '\0';
	}

	// Update the number of splits found
	*splits = (int)nFound;

	return split_array;
}