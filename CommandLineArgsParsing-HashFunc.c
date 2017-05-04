#include <stdio.h>
#include <stdlib.h> // For exit()

// Calcluated hashes
#define START_SHORT_HASH                5859067         // "-s"
#define START_LONG_HASH                 229293502026981 // "--start"
#define END_SHORT_HASH                  5859053         // "-e"
#define END_LONG_HASH                   210554161514    // "--end"
#define HELP_SHORT_HASH                 5859040         // "-h"
#define HELP_LONG_HASH                  6948287704628   // "--help"

// Emulating command line arguments for an online compiler
static char *argv[] = {"program_name.out", "--start", "10", "potato", "--end", "20", "spud", NULL};
static int argc = 7;

// From http://www.cse.yorku.ca/~oz/hash.html
// Slightly modified to remove compiler warnings
unsigned long hash(char *str) {
	// Casting string to unsigned char* "safely" here to avoid compiler warning
	unsigned char *string = (unsigned char*) str;
	unsigned long hash = 5381;
	int c;

	// Extra brackets are to get rid of:
	// "warning: suggest parentheses around assignment used as truth value [-Wparentheses]"
	while ((c = *string++))
		// Equivalent to 
		//	hash = hash * 33 ^ c;
		// but faster implementation
		hash = ((hash << 5) + hash) ^ c;

	return hash;
}

/*
Simple check using strtol()
strtol() converts a string to a long and writes to a pointer which contains the
rest of the string after the number
if only a number came in, then the pointer string should be empty, if not, then
the input string wasn't a number!
*/
int isValidNum(char *strToCheck) {
	char *afterStr;
	int val = strtol(strToCheck, &afterStr, 10);
	int result = 0;

	// Check for empty string and characters left after conversion.
	if ((afterStr == strToCheck) || (*afterStr != '\0')) {
		result = 0;
	} else {
		result = 1;
	}
	return result;
}

// Used to validate that a flag that requires args has a valid number after it
// Exits with failure status if invalid
void checkIfRequiredArgsPresent(char *flag, char *str) {
	if(!isValidNum(str)) {
		printf("'%s' is not a valid input for %s\n", str, flag);
		exit(EXIT_FAILURE);
	}
}

int main() {
	// Args to ignore parsing because they have already been processed
	// i.e. the required args after a flag
	int skipArg = 0;
	int i = 0;
	int start = 0;
	int end = 0;

	for(i=0; i<argc; i++) {
		switch(hash(argv[i])) {
			case START_SHORT_HASH:
			case START_LONG_HASH:
				checkIfRequiredArgsPresent(argv[i], argv[i+1]);
				start = strtol(argv[i+1], NULL, 10);
				skipArg = i+1;
				break;
			case END_SHORT_HASH:
			case END_LONG_HASH:
				checkIfRequiredArgsPresent(argv[i], argv[i+1]);
				end = strtol(argv[i+1], NULL, 10);
				skipArg = i+1;
				break;
			case HELP_SHORT_HASH:
			case HELP_LONG_HASH:
				printf("Help selected.\n");
				exit(EXIT_SUCCESS);
				break;
			default: // Non-flag arguments
				if(i != skipArg) {
					printf("Invalid arg '%s'\n", argv[i]);
				}
				break;
		}
	}

	printf("start = %d\n", start);
	printf("end = %d\n", end);

	return 0;
}