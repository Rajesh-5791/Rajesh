// To get temperature of any city

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCITYLENGTH 30
#define MAXURLLENGTH 200
#define MAXTEMPERATURELENGTH 10

void main()
{
	char cityName[MAXCITYLENGTH], url[MAXURLLENGTH], temperature[MAXTEMPERATURELENGTH];
	printf("Please enter the name of a city to get its current temperature: ");
	scanf("%s", cityName);
	sprintf(url, "%s%s%s", "curl -s \"https://api.openweathermap.org/data/2.5/weather?q=", cityName, "&appid=abe3a0f4d0b6cebfbe7393b4b4e3aa28&units=metric\" > weather.txt");
	// printf("%s", url);
	system(url);

	FILE *fpWeather = fopen("weather.txt", "r");
	fseek(fpWeather, 0, SEEK_END);
	int fileSize = ftell(fpWeather);
	fseek(fpWeather, 0, SEEK_SET);
	char weatherApiResponse[fileSize];
	fgets(weatherApiResponse, fileSize, fpWeather);
    fclose(fpWeather);

    char *token = (char *)strtok(weatherApiResponse, "\"");
    while (token != NULL) 
    {
        if (strcmp(token, "temp") == 0)
        {
            token = (char *)strtok(NULL, ":,");
            strcpy(temperature, token);
            printf("Current temperature of %s: %s\n", cityName, temperature);
            break;
        }
        token = (char *)strtok(NULL, "\"");
    }
}
