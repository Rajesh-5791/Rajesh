// To get meaning of any word using dictionary API

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORDLENGTH 50
#define MAXURLLENGTH 100
#define MAXDEFINITIONLENGTH 200
#define MAXAUDIOURLLENGTH 200

char  *audioUrl, word[MAXWORDLENGTH], *definition;
FILE *fpDictionary;
int fileSize;
void playPronunciation(char *);
char* getDefinition(char *);
char* getAudioUrl(char *);

void main()
{
	char url[MAXURLLENGTH];
	printf("Enter a word to get it's meaning: ");
	scanf("%s", word);

	sprintf(url, "%s%s%s", "curl -s \"https://api.dictionaryapi.dev/api/v2/entries/en/", word, "\" > dictionaryApiResponse.txt");
	system(url);

	fpDictionary = fopen("dictionaryApiResponse.txt", "r");
	fseek(fpDictionary, 0, SEEK_END);
	fileSize = ftell(fpDictionary);
	fseek(fpDictionary, 0, SEEK_SET);
	char dictionaryApiResponse[fileSize];
	fgets(dictionaryApiResponse, fileSize, fpDictionary);

	definition = malloc(MAXDEFINITIONLENGTH);
	definition = getDefinition(dictionaryApiResponse);
	if (strcmp(definition, "Not found") == 0)
	{
		printf("%s not found in the dictionary.\n", word);
	}
	else
	{
		printf("\nDefinition of %s: %s\n", word, definition);
	}

	fseek(fpDictionary, 0, SEEK_SET);
	fgets(dictionaryApiResponse, fileSize, fpDictionary);
	audioUrl = malloc(MAXAUDIOURLLENGTH);
	audioUrl = getAudioUrl(dictionaryApiResponse);
	if (strcmp(audioUrl, "Not found") == 0)
	{
		printf("%s pronunciation not found in the dictionary.\n", word);
	}
	else
	{
		playPronunciation(audioUrl);
	}
	fclose(fpDictionary);
}

char* getAudioUrl(char *dictionaryApiResponse)
{
	int isAudioFound = 0;
	strcpy(audioUrl, "Not found");

    char *token = strtok(dictionaryApiResponse, "\"");
    while (token != NULL) 
    {
        if (strcmp(token, "audio") == 0) 
        {
            token = strtok(NULL, "\"");
            token = strtok(NULL, "\"");
            strcpy(audioUrl, token);
            if (strlen(audioUrl) > 10) 
            {
                break;
            } 
            else 
            {
	            isAudioFound = 1;
            	while (token != NULL)
            	{	
	                if (strcmp(token, "audio") == 0) 
		            {
		                token = strtok(NULL, "\"");
		                token = strtok(NULL, "\"");
			            strcpy(audioUrl, token);		                
		                isAudioFound = 0;
		                break;
		            }	            
			        token = strtok(NULL, "\"");
            	}
            }
            break;
        }
        token = strtok(NULL, "\"");
    }
    if (isAudioFound == 1)
    {
    	fseek(fpDictionary, 0, SEEK_SET);
		fgets(dictionaryApiResponse, fileSize, fpDictionary);
        char *token = strtok(dictionaryApiResponse, ":");
        while (token != NULL) 
        {
            if (strcmp(token, "[{\"audio\"") == 0) 
            {
                token = strtok(NULL, "\"");
                strcpy(audioUrl, token);
                break;
            }
            token = strtok(NULL, ":");
        }
    }
    return audioUrl;
}

char* getDefinition(char *dictionaryApiResponse)
{
	strcpy(definition, "Not found");
	char *token = (char *)strtok(dictionaryApiResponse, ":");
    while (token != NULL) 
    {
        if (strcmp(token, "[{\"definition\"") == 0) 
        {
            token = strtok(NULL, "\";.");
            strcpy(definition, token);
            break;
        }
        token = strtok(NULL, ":");
    }
    return definition;
}

void playPronunciation(char *audioUrl)
{
    printf("\nPlaying %s pronunciation...\n", word);
    char playCommand[MAXURLLENGTH];
    sprintf(playCommand, "start %s", audioUrl);
    system(playCommand);
}