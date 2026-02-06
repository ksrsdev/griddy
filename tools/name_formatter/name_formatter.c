#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *inputFile = fopen("last_names.txt", "r");
	FILE *outputFile = fopen("last_names_formatted.txt", "w");
	if (inputFile == NULL || outputFile == NULL) {
		printf("UNABLE TO OPEN FILE!\n");
		return 1;
	}
	int numLines = 0;
	char inputString[128];
	char outputString[128];
	while (fgets(inputString, 128, inputFile)) {
		int i = 0;
		int j = 0;
// 1. Skip any leading whitespace before the rank
    while (inputString[i] == ' ' || inputString[i] == '\t') i++;

    // 2. Skip the rank digits (skip until we hit whitespace)
    while (inputString[i] != ' ' && inputString[i] != '\t' && inputString[i] != '\0') {
        i++;
    }

    // 3. Skip the whitespace BETWEEN rank and name
    while (inputString[i] == ' ' || inputString[i] == '\t') {
        i++;
    }
		//Copy Name into outputString
		while (inputString[i] != ' ' 
				&& inputString[i] != '\0' 
				&& inputString[i] != '\t'
				&& inputString[i] != '\n') {
			outputString[j] = inputString[i];
			i++;
			j++;
		}
		//past the name portion
		outputString[j] = '\0';
		if (j>0) {
			fprintf(outputFile, "%s\n", outputString);
		}
		//printf("%s\n", outputString);
		numLines++;
	}
	printf ("Total Num Lines: %d\n", numLines);
	int currentLine = 0;
//	char *outputStrings[];
//
	fclose(inputFile);
	fclose(outputFile);
	return 0;
}
