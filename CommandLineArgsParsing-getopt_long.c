#include <stdio.h>
#include <stdlib.h> // For exit()
#include <getopt.h>

// Emulating command line arguments for an online compiler
static char *argv[] = {"program_name.out", "--start", "10", "potato", "--end", "20", "spud", NULL};
static int argc = 7;

// Modified from http://carbidelabs.com/tutorials/c-argument-parsing/
int main() {
	int opts         = 0;
	int longOptIndex = 0;
	int prevIndex    = 0;
	int start        = 0;
	int end          = 0;

	struct option longOpts[] = /*   const char*  int                 int*    int 
				        name         has_arg             flag    val    */
				    {{ "start",      required_argument,  NULL,   's' },
				    {  "end",        required_argument,  NULL,   'e' },
				    {  "help",       no_argument,        NULL,   'h' },
				    {  NULL,         0,                  NULL,    0  }};

	while(prevIndex = optind, (opts = getopt_long(argc, argv, "s:e:h", longOpts, &longOptIndex)) != -1) {
		// if-statement below is to make sure that options that require arguments get them.
		// not sure of the purpose of "required_argument" setting if it does not check this...
		/*
		POSIX standard definition for getopt():
		If the option was the last character in the string pointed to by an element
		of argv, then optarg shall contain the next element of argv, and optind
		shall be incremented by 2. If the resulting value of optind is greater than
		argc, this indicates a missing option-argument, and getopt() shall return
		an error indication.
		Otherwise, optarg shall point to the string following the option character
		in that element of argv, and optind shall be incremented by 1.
		*/
		if(optind == prevIndex + 2 && *optarg == '-') {
			opts = ':';
			--optind;
		}
		switch(opts) {
			case 's':
				start = strtol(optarg, NULL, 10);
				break;
			case 'e':
				end = strtol(optarg, NULL, 10);
				break;
			case 'h':
				printf("Help selected.\n");
				exit(EXIT_SUCCESS);
				break;
			// User has input some option not defined e.g. "--potato" or "-p"
			// Or not provided a value after an argument that requires it
			case '?':
				switch(optopt) {
					case 's':
					case 'e':
						printf("-%c requires an argument.\n", optopt);
						break;
					default:
						printf("Unknown flag '%c'.\n", optopt);
						break;
				}
			// Fall-through here is intentional 
			default:
				// Hit some unexpected case - quit.
				printf("Unknown error ocurred.\n");
				exit(EXIT_FAILURE);
				break;
		}
	}

	printf("start = %d\n", start);
	printf("end = %d\n", end);

	return 0;
}