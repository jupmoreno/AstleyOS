#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>

#define FALSE 0
#define TRUE !FALSE

#define BUFFER_SIZE 128

#define DEFAULT_OUTPUT_FILE "system.bin"

typedef struct {
	char **elements;
	int length;
} array_t;

static int checkFiles(array_t fileArray);
static int buildImage(array_t fileArray, char * output);
static void write_size(FILE * target, char * filename);
static int write_file(FILE * target, FILE * source);

int main(int argc, char *argv[]) {
	char * output = NULL;
	int i, c;
	array_t fileArray = {NULL, 0};

	printf("~~ Alma's MP v0.1 ~~\n");
	printf("Modified version of x64BareBones's Module Packer (C) v0.1.1\n");

	opterr = 0;
	while((c = getopt(argc, argv, "o:")) != -1) {
		switch(c) {
			case 'o':
				output = optarg;
				break;
			case '?':
				if(optopt == 'o')
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				else if(isprint (optopt))
					fprintf(stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
				return 1;
			default:
				abort();
		}
	}

	if(argc - optind < 1) {
		fprintf(stderr, "Usage:\n  $> mp <optionals> <kernel_binary> <user_binary_0> ... <user_binary_N>\n");
		fprintf(stderr, "Optionals:\n  '-o <output_name>': custom output name\n");
		return 1;
	}

	if(output == NULL) {
		output = DEFAULT_OUTPUT_FILE;
	}

	fileArray.elements = &argv[optind];
	fileArray.length = argc - optind;

	printf("Binaries:");
	for (i = 0; i < fileArray.length; i++) {
		printf(" %s", fileArray.elements[i]);
	}
	printf("\nOutput: %s\n", output);

	if(!checkFiles(fileArray)) {
		return 1;
	}

	i = buildImage(fileArray, output);

	if(i) {
		printf("~~       OK       ~~\n");
	} else {
		printf("~~      FAIL      ~~\n");
	}

	return !i;
}

static int checkFiles(array_t fileArray) {
	int i;

	for(i = 0; i < fileArray.length ; i++) {
		if(access(fileArray.elements[i], R_OK)) {
			fprintf(stderr, "Can't open file: %s\n", fileArray.elements[i]);

			return FALSE;
		}
	}

	return TRUE;
}

static int buildImage(array_t fileArray, char * output) {
	int i, extraBinaries;
	FILE * target, * source;

	if((target = fopen(output, "w")) == NULL) {
		fprintf(stderr, "Can't create output file: %s\n", output);

		return FALSE;
	}

	// Write kernel binary
	source = fopen(fileArray.elements[0], "r");
	write_file(target, source);

	// Write user binaries
	extraBinaries = fileArray.length - 1;
	fwrite(&extraBinaries, sizeof(extraBinaries), 1, target);
	fclose(source);

	for (i = 1 ; i < fileArray.length ; i++) {
		source = fopen(fileArray.elements[i], "r");

		// Write file size
		write_size(target, fileArray.elements[i]);
		// Write file data
		write_file(target, source);

		fclose(source);
	}

	fclose(target);

	return TRUE;
}

static void write_size(FILE * target, char * filename) {
	struct stat st;
	uint32_t size;

	stat(filename, &st);
	size = st.st_size;
	fwrite(&size, sizeof(uint32_t), 1, target);
}

static int write_file(FILE * target, FILE * source) {
	char buffer[BUFFER_SIZE];
	int read;

	while (!feof(source)) {
		read = fread(buffer, 1, BUFFER_SIZE, source);
		fwrite(buffer, 1, read, target);
	}

	return TRUE;
}
