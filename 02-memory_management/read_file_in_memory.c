#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

extern int errno;

char *read_file_in_memory(const char *filename) { 
	int size = 0;
    size_t bytes_read;
    FILE *f;
    char* buffer;
	
    f = fopen(filename, "rb");
	if (f == NULL) { 
		buffer = NULL;
		perror("Cannot open file!");
        exit(errno);
	} 

    /* Get the size of the file. */
	fseek(f, 0, SEEK_END);
	size = ftell(f);
    
    /* Go back to the start of the file. */
	fseek(f, 0, SEEK_SET);

    /* Allocate our buffer to that size. */
	buffer = (char *)malloc(size + 1);
    
    /* Read the entire file into memory. */
	bytes_read = fread(buffer, sizeof(char), size, f);
	if (size != bytes_read) { 
		free(buffer);
		perror("Cannot read file!");
        exit(errno);
	} 
	fclose(f);
    
    return buffer;
}


int search_in_file(char *filename, char *str) {
	FILE *fp;
	int line_num = 1;
	int find_result = 0;
	char temp[512];
    char* first_occurence;
	
	while(fgets(temp, 512, fp) != NULL) {
        first_occurence = strstr(temp, str);
		if(first_occurence != NULL) {
			printf("A match found on line: %d\n", line_num);
			printf("\n%s\n", temp);
			find_result++;
		}
		line_num++;
	}

	if(find_result == 0) {
		printf("\nSorry, couldn't find a match.\n");
	}
	
	//Close the file if still open.
	if(fp) {
		fclose(fp);
	}
   	return(0);
}


int main(int argc, char **argv) {
    char *file_contents;
    char *filename = argv[1]; 
    if (filename == NULL) {
        fputs("Please specify an existing file to be read!", stderr);
        exit(1);
    }

    file_contents = read_file_in_memory(filename);
    printf("%s", file_contents);

    free(file_contents);
    return 0;
}
