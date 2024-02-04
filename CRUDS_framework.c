// CRUDS framework program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXFIELDVALUELENGTH 20
#define MAXFIELDNAMELENGTH 100
#define MAXMESSAGELENGTH 200
#define OPERATIONLENGTH 10
#define FIELDSFILE "fields.cfg"
#define MENUFILE "menu.cfg"
#define DATAFILE "framework.dat"
#define MESSAGESFILE "customizedUserMessages.cfg"

FILE *fpFramworkRead, *fpFramworkWrite, *fpFields, *fpMenu, *fpMessages;
char fieldValue[MAXFIELDVALUELENGTH], fieldName[MAXFIELDNAMELENGTH], userMessage[MAXMESSAGELENGTH];
char **fieldNames, **userMessages, targetRecordId[MAXFIELDVALUELENGTH], updatedValue[MAXFIELDVALUELENGTH], operation[OPERATIONLENGTH];
int countOfFields, countOfMessages;

void showMenu();
char **getFieldsArray();
char **getCustomizedUserMessagesArray();
int getFieldsCount();
int getCustomizedUserMessagesCount();
char **getRecordDetails();
void addRecord(char **);
int getRecordsCount();
void showAllRecords();
char *getTargetRecordId(int choice);
long int getTargetRecordPosition(char *);
char *getUpdatedFieldValue();
void printStatusMessage(int isRecordFound, char *, char operation[]);
int update(char *, char operation[]);
void updateRecord(char *);
void deleteRecord(char *);
void printSearchRecord(long int targetRecordPosition);
void searchRecord(char *);

void main()
{
	fpFields = fopen(FIELDSFILE, "r");
	fpMessages = fopen(MESSAGESFILE, "r");
	countOfFields = getFieldsCount();
	fieldNames = getFieldsArray();
	countOfMessages = getCustomizedUserMessagesCount();
	userMessages = getCustomizedUserMessagesArray();
	do
	{
		showMenu();
	} while (1);
}

int getFieldsCount()
{
	int fieldsCounter = 0;
	while (fgets(fieldName, sizeof(fieldName), fpFields))
	{
		fieldsCounter++;
	}
	return fieldsCounter;
}

char** getFieldsArray()
{
	int fieldsCounter;
	rewind(fpFields);
	fieldNames = (char **)malloc(countOfFields * sizeof(char *));
	for (fieldsCounter = 0; fieldsCounter < countOfFields; fieldsCounter++)
	{
		fieldNames[fieldsCounter] = malloc(MAXFIELDNAMELENGTH);
		fgets(fieldName, sizeof(fieldName), fpFields);
		int fieldNameLength = strlen(fieldName);
		fieldName[fieldNameLength - 1] = '\0';
		strcpy(fieldNames[fieldsCounter], fieldName);
	}
	return fieldNames;
}

int getCustomizedUserMessagesCount()
{
	int messagesCounter = 0;
	while (fgets(userMessage, sizeof(userMessage), fpMessages))
	{
		messagesCounter++;
	}
	return messagesCounter;
}

char** getCustomizedUserMessagesArray()
{
	int messagesCounter;
	rewind(fpMessages);
	userMessages = (char **)malloc(countOfMessages * sizeof(char *));
	for (messagesCounter = 0; messagesCounter < countOfMessages; messagesCounter++)
	{
		userMessages[messagesCounter] = malloc(MAXMESSAGELENGTH);
		fgets(userMessage, sizeof(userMessage), fpMessages);
		int userMessageLength = strlen(userMessage);
		userMessage[userMessageLength - 1] = '\0';
		strcpy(userMessages[messagesCounter], userMessage);
	}
	return userMessages;
}

void showMenu()
{
	int menuSize, choice;
	fpMenu = fopen(MENUFILE, "rb");
	fseek(fpMenu, 0, SEEK_END);
	menuSize = ftell(fpMenu);
	fseek(fpMenu, 0, SEEK_SET);
	char menu[menuSize];
	fread(&menu, sizeof(menu), 1, fpMenu);
	fwrite(&menu, sizeof(menu), 1, stdout);
	fclose(fpMenu);

	printf("\nEnter your choice: ");
	scanf("%d", &choice);

	switch (choice)
	{
		case 1: addRecord(getRecordDetails());
			break;
		case 2: showAllRecords();
			break;
		case 3: updateRecord(getTargetRecordId(choice));
			break;
		case 4: deleteRecord(getTargetRecordId(choice));
			break;
		case 5: searchRecord(getTargetRecordId(choice));
			break;
		case 0: printf("You chose exit. Thank you.\n");
				fclose(fpFields);
				fclose(fpMessages);
				exit(0);
		default: printf("You entered an invalid choice.\n");
	}
}

char** getRecordDetails()
{
	int counter;
	char **fieldValues = (char **)malloc(countOfFields * sizeof(char *));
	for (counter = 0; counter < countOfFields; counter++)
	{
		fieldValues[counter] = malloc(MAXFIELDVALUELENGTH);
		if (counter == 0)
		{
			strcpy(fieldValue, "a");
			strcpy(fieldValues[counter], fieldValue);
			continue;
		}

		printf("Enter %s: ", fieldNames[counter]);
		scanf(" %19[^\n]s", fieldValues[counter]);
	}
	return fieldValues;
}

void addRecord(char **fieldValues)
{
	int counter, addedFieldsCounter = 0;
	char currentId[MAXFIELDVALUELENGTH];
	fpFramworkWrite = fopen(DATAFILE, "a");
	for (counter = 0; counter < countOfFields; counter++)
	{
		if (counter == 1)
		{
			strcpy(currentId, fieldValues[counter]);
		}
		strcpy(fieldValue, fieldValues[counter]);
		addedFieldsCounter += fwrite(fieldValue, sizeof(fieldValue), 1, fpFramworkWrite);
	}
	fclose(fpFramworkWrite);
	if (addedFieldsCounter == counter)
	{		
		printf("%s %s.\n", userMessages[2], currentId);
	}
	else
	{
		printf("%s %s.\n", userMessages[3], currentId);
	}
}

int getRecordsCount()
{
	fpFramworkRead = fopen(DATAFILE, "r");
	fseek(fpFramworkRead, 0, SEEK_END);
	int fileSize = ftell(fpFramworkRead);
	int recordsCount = fileSize / (countOfFields * MAXFIELDVALUELENGTH);
	fclose(fpFramworkRead);
	return recordsCount;
}

void showAllRecords()
{
	int counter, recordsCounter, isRecordFound = 0;
	int recordsCount = getRecordsCount();
	if (recordsCount < 1)
	{
		printf("%s\n", userMessages[1]);
	}
	else
	{
		printf("%s \n", userMessages[0]);
		printf("---------------------------------\n");

		fpFramworkRead = fopen(DATAFILE, "r");
		for (recordsCounter = 0; recordsCounter < recordsCount; recordsCounter++)
		{
			for (counter = 0; counter < countOfFields; counter++)
			{
				fread(fieldValue, sizeof(fieldValue), 1, fpFramworkRead);
				if (counter == 0)
				{
					if (strcmp(fieldValue, "d") == 0)
					{
						fseek(fpFramworkRead, (countOfFields - 1) * MAXFIELDVALUELENGTH, SEEK_CUR);
						isRecordFound = 0;
						break;
					}
					isRecordFound = 1;
					continue;
				}
				printf("%s: %s\n", fieldNames[counter], fieldValue);
			}
			if (isRecordFound == 1)
			{
				printf("---------------------------------\n");
			}
		}
	}
}

long int getTargetRecordPosition(char *targetRecordId)
{
	long int targetRecordPosition = -1;
	int recordsCounter, counter;
	int recordsCount = getRecordsCount();
	fpFramworkRead = fopen(DATAFILE, "r");
	for (recordsCounter = 0; recordsCounter < recordsCount; recordsCounter++)
	{
		fread(fieldValue, sizeof(fieldValue), 1, fpFramworkRead);
		if (strcmp(fieldValue, "a") == 0)
		{
		    fread(fieldValue, sizeof(fieldValue), 1, fpFramworkRead);
			if (strcmp(fieldValue, targetRecordId) == 0)
			{
				fseek(fpFramworkRead, - (MAXFIELDVALUELENGTH * 2), SEEK_CUR);
				targetRecordPosition = ftell(fpFramworkRead);
				break;
			}
			else
			{
				fseek(fpFramworkRead, MAXFIELDVALUELENGTH * (countOfFields - 2), SEEK_CUR);
			}
		}
	}
	return targetRecordPosition;
}

char *getUpdatedFieldValue()
{
	printf("Enter %s to update: ", fieldNames[countOfFields - 1]);
	scanf(" %19[^\n]s", updatedValue);
	return updatedValue;
}

char *getTargetRecordId(int choice)
{
	printf("Enter %s ", fieldNames[1]);
	if (choice == 3)
	{
		printf("to update: ");
	}
	else if (choice == 4)
	{
		printf("to delete: ");
	}
	else
	{
		printf("to search: ");
	}
	scanf(" %19[^\n]s", targetRecordId);
	return targetRecordId;
}

int update(char *targetRecordId, char operation[])
{
	int isRecordFound;
	long int targetRecordPosition = getTargetRecordPosition(targetRecordId);
	if (targetRecordPosition != -1)
	{
		isRecordFound = 1;
		fpFramworkWrite = fopen(DATAFILE, "r+");
		fseek(fpFramworkWrite, targetRecordPosition, SEEK_SET);
		if (strcmp(operation, "delete") == 0)
		{
			fread(fieldValue, sizeof(fieldValue), 1, fpFramworkWrite);
			fseek(fpFramworkWrite, - MAXFIELDVALUELENGTH, SEEK_CUR);
			strcpy(fieldValue, "d");
		}
		else
		{
			fseek(fpFramworkWrite, (countOfFields - 1) * MAXFIELDVALUELENGTH, SEEK_CUR);
			strcpy(fieldValue, getUpdatedFieldValue());
		}
		fwrite(fieldValue, sizeof(fieldValue), 1, fpFramworkWrite);
		fclose(fpFramworkWrite);
	}
	else
	{
		isRecordFound = 0;
	}
	return isRecordFound;
}

void printStatusMessage(int isRecordFound, char *targetRecordId, char operation[])
{
	if (isRecordFound == 0)
	{
		printf("%s %s.\n", userMessages[4], targetRecordId);
	}
	else
	{
		printf("%s %sd %s %s.\n", userMessages[5], operation, userMessages[6], targetRecordId);
	}
}

void updateRecord(char *targetRecordId)
{
	char operation[OPERATIONLENGTH] = "update";
	int isRecordFound = update(targetRecordId, operation);
	printStatusMessage(isRecordFound, targetRecordId, operation);
}

void deleteRecord(char *targetRecordId)
{
	char operation[OPERATIONLENGTH] = "delete";
	int isRecordFound = update(targetRecordId, operation);
	printStatusMessage(isRecordFound, targetRecordId, operation);
}

void printSearchRecord(long int targetRecordPosition)
{
	int fieldsCounter;
	fpFramworkRead = fopen(DATAFILE, "r");
	fseek(fpFramworkRead, targetRecordPosition, SEEK_SET);
	printf("Record details: \n");

	for (fieldsCounter = 0; fieldsCounter < countOfFields; fieldsCounter++)
	{
		fread(fieldValue, sizeof(fieldValue), 1, fpFramworkRead);
		if (fieldsCounter == 0)
		{
			continue;
		}
		printf("%s: %s\n", fieldNames[fieldsCounter], fieldValue);
	}
	printf("-----------------------------------\n");
}

void searchRecord(char *targetRecordId)
{
	long int targetRecordPosition = getTargetRecordPosition(targetRecordId);
	if (targetRecordPosition != -1)
	{
		printSearchRecord(targetRecordPosition);
	}
	else
	{
		printf("%s %s.\n", userMessages[4], targetRecordId);
	}
}