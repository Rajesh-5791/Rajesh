// To perform CRUDS operations using binary tree

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <psapi.h>

#define MAX_OPERATION_LENGTH 10
#define MAX_ID_LENGTH 10
#define MAX_NAME_LENGTH 100
#define DATA_FILE "Songs.dat"

typedef struct SongDetails 
{
    char SongId[MAX_ID_LENGTH];
    char SongName[MAX_NAME_LENGTH];
    float SongLength;
} SongDetails;

typedef struct SongNode 
{
    struct SongNode *LeftNode;
    SongDetails SongRecord;
    struct SongNode *RightNode;
} SongNode;

FILE *FpSongs;
SongNode *RootNode = NULL;
char *Operation;

void ShowMenu();
void LoadBinaryTree();
void GetDetails(SongDetails *Song);
void AddSong();
void ShowAllSongs();
void UpdateSongDetails(char *);
void DeleteSong(SongNode **, const char *);
void SearchSong(const char *);
char *GetSongId(char *);
void PrintSuccessMessage(char *, const char *);
void GetProcessMemoryUsage();

int main() 
{
	time_t StartTime, EndTime;
	StartTime = clock();
    Operation = malloc(MAX_OPERATION_LENGTH);
    LoadBinaryTree();
    ShowMenu();
    EndTime = clock();
	double ExecutionTimeInSeconds = (double)(EndTime - StartTime) / CLOCKS_PER_SEC;
    double ExecutionTimeInMilliseconds = (double)(EndTime - StartTime) / (CLOCKS_PER_SEC / 1000);
    double ExecutionTimeInMinutes = ExecutionTimeInSeconds / 60;
    printf("Execution time: %.3f minutes\n", ExecutionTimeInMinutes);
    printf("Execution time: %.3f seconds\n", ExecutionTimeInSeconds);
    printf("Execution time: %.3f milliseconds\n", ExecutionTimeInMilliseconds);
    GetProcessMemoryUsage();
    free(Operation);
    return 0;
}

char* GenerateRandomId() 
{
    static char RandomSongId[MAX_ID_LENGTH]; 
    srand(time(NULL));
    int Counter;
    for (Counter = 0; Counter < MAX_ID_LENGTH; ++Counter) 
    {
        RandomSongId[Counter] = '0' + rand() % 10;
    }
    RandomSongId[MAX_ID_LENGTH - 1] = '\0';
    return RandomSongId;
}

void GetProcessMemoryUsage() 
{
    PROCESS_MEMORY_COUNTERS_EX MemoryCounters;

    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*) MemoryCounters, sizeof MemoryCounters))) 
    {
        SIZE_T memoryUsedBytes = MemoryCounters.PrivateUsage;
        SIZE_T memoryUsedMB = memoryUsedBytes / (1024 * 1024);
        SIZE_T memoryUsedKB = memoryUsedBytes / 1024;
        printf("Memory used by the process: %u bytes\n", (unsigned int)memoryUsedBytes);
        printf("Memory used by the process: %u KB\n", (unsigned int)memoryUsedKB);
        printf("Memory used by the process: %u MB\n", (unsigned int)memoryUsedMB);
    }
    else
    {
        printf("Error getting process memory info.\n");
    }
}

void LoadBinaryTree() 
{
    SongDetails SongRecord;
    SongNode **CurrentNodePtr, *NewSongNode, *CurrentSongNode;
    FpSongs = fopen(DATA_FILE, "rb");
    while (fread(&SongRecord, sizeof(SongDetails), 1, FpSongs)) 
    {
        NewSongNode = (SongNode *)malloc(sizeof(SongNode));
        NewSongNode->SongRecord = SongRecord;
        NewSongNode->LeftNode = NULL;
        NewSongNode->RightNode = NULL;

        CurrentNodePtr = &RootNode;

        while (*CurrentNodePtr != NULL) 
        {
            CurrentSongNode = *CurrentNodePtr;
            if (strcmp(CurrentSongNode->SongRecord.SongId, SongRecord.SongId) < 0) 
            {
                CurrentNodePtr = &CurrentSongNode->RightNode;
            } 
            else 
            {
                CurrentNodePtr = &CurrentSongNode->LeftNode;
            }
        }
        *CurrentNodePtr = NewSongNode;
    }
    fclose(FpSongs);
}

void SaveBinaryTreeHelper(SongNode *Node, FILE *FpSongs) 
{
    if (Node == NULL) 
    {
        return;
    }
    SaveBinaryTreeHelper(Node->LeftNode, FpSongs);
    SaveBinaryTreeHelper(Node->RightNode, FpSongs);
    fwrite(&(Node->SongRecord), sizeof(SongDetails), 1, FpSongs);
}

void SaveBinaryTree() 
{
    FpSongs = fopen(DATA_FILE, "wb");
    SaveBinaryTreeHelper(RootNode, FpSongs);
    fclose(FpSongs);
}

SongNode* GetMatchingNode(SongNode* Node, const char* SongId) 
{
    if (Node == NULL || strcmp(Node->SongRecord.SongId, SongId) == 0) 
    {
        return Node;
    }

    if (strcmp(SongId, Node->SongRecord.SongId) < 0) 
    {
        return GetMatchingNode(Node->LeftNode, SongId);
    } 
    else 
    {
        return GetMatchingNode(Node->RightNode, SongId);
    }
}

void GetDetails(SongDetails *Song) 
{
    strcpy(Song->SongId, GenerateRandomId());
    printf("Enter song name: ");
    scanf(" %99[^\n]s", Song->SongName);
    printf("Enter song length: ");
    scanf("%f", &Song->SongLength);
}

void AddSong() 
{
    SongNode* NewSongNode = (SongNode*)malloc(sizeof(SongNode));
    GetDetails(&NewSongNode->SongRecord);
    NewSongNode->LeftNode = NULL;
    NewSongNode->RightNode = NULL;

    SongNode *TargetNode = GetMatchingNode(RootNode, NewSongNode->SongRecord.SongId);
    if (TargetNode != NULL)
    {
        strcpy(NewSongNode->SongRecord.SongId, GenerateRandomId());
    }
    if (RootNode == NULL) 
    {
        RootNode = NewSongNode;
    }
    else 
    {
        SongNode* ParentNode = NULL;
        SongNode* CurrentNode = RootNode;
        while (CurrentNode != NULL) 
        {
            ParentNode = CurrentNode;
            if (strcmp(NewSongNode->SongRecord.SongId, CurrentNode->SongRecord.SongId) < 0) 
            {
                CurrentNode = CurrentNode->LeftNode;
            } 
            else 
            {
                CurrentNode = CurrentNode->RightNode;
            }
        }
        if (strcmp(NewSongNode->SongRecord.SongId, ParentNode->SongRecord.SongId) < 0) 
        {
            ParentNode->LeftNode = NewSongNode;
        } 
        else 
        {
            ParentNode->RightNode = NewSongNode;
        }
    }
    Operation = "adde";
    PrintSuccessMessage(Operation, NewSongNode->SongRecord.SongId);
    SaveBinaryTree();
}

void ShowAllSongsHelper(SongNode *Node, int *Counter) 
{
    if (Node == NULL) 
    {
        return;
    }
    ShowAllSongsHelper(Node->LeftNode, Counter);
    printf("Details of Song %d: \n", ++(*Counter));
    printf("Song ID: %s\n", Node->SongRecord.SongId);
    printf("Song Name: %s\n", Node->SongRecord.SongName);
    printf("Song Length: %.2f minutes\n", Node->SongRecord.SongLength);
    printf("-----------------------------\n");
    ShowAllSongsHelper(Node->RightNode, Counter);
}

void ShowAllSongs() 
{
    printf("Showing all Songs:\n");
    if (RootNode == NULL) 
    {
        printf("No Songs found.\n");
        return;
    }
    int Counter = 0;
    ShowAllSongsHelper(RootNode, &Counter);
}

void UpdateSongDetails(char *SongId) 
{
    float NewSongLength;
    SongNode *TargetNode = GetMatchingNode(RootNode, SongId);
    if (TargetNode == NULL) 
    {
        printf("Song with ID %s does not exist.\n", SongId);
        return;
    }
    printf("Enter the new length of the Song: ");
    scanf("%f", &NewSongLength);

    TargetNode->SongRecord.SongLength = NewSongLength;
    Operation = "update";
    PrintSuccessMessage(Operation, SongId);
    SaveBinaryTree();
}

SongNode* FindLeafNode(SongNode* Node) 
{
    while (Node->LeftNode != NULL) 
    {
        Node = Node->LeftNode;
    }
    return Node;
}

void DeleteSong(SongNode** RootPtr, const char* SongId) 
{
    if (*RootPtr == NULL) 
    {
        printf("No Songs found.\n");
        return;
    }

    SongNode* NodeToDelete = GetMatchingNode(*RootPtr, SongId);
    if (NodeToDelete == NULL) 
    {
        printf("Song with ID %s does not exist.\n", SongId);
        return;
    }

    if (NodeToDelete->LeftNode == NULL) 
    {
        *RootPtr = NodeToDelete->RightNode;
        free(NodeToDelete);
    } 
    else if (NodeToDelete->RightNode == NULL) 
    {
        *RootPtr = NodeToDelete->LeftNode;
        free(NodeToDelete);
    } 
    else 
    {
        if (*RootPtr == NodeToDelete) 
        {
            SongNode* Temp = FindLeafNode(NodeToDelete->RightNode);
            strcpy(NodeToDelete->SongRecord.SongId, Temp->SongRecord.SongId);
            DeleteSong(&(NodeToDelete->RightNode), Temp->SongRecord.SongId);
        } 
        else 
        {
            SongNode* Temp = FindLeafNode(NodeToDelete->RightNode);
            strcpy(NodeToDelete->SongRecord.SongId, Temp->SongRecord.SongId);
            DeleteSong(&(NodeToDelete->RightNode), Temp->SongRecord.SongId);
        }
    }
    Operation = "delete";
    PrintSuccessMessage(Operation, SongId);
    SaveBinaryTree();
}

void SearchSong(const char *SongId) 
{
    SongNode* result = GetMatchingNode(RootNode, SongId);
    if (result != NULL) 
    {
        printf("Song found:\n");
        printf("Song ID: %s\n", result->SongRecord.SongId);
        printf("Song name: %s\n", result->SongRecord.SongName);
        printf("Song length: %.2f minutes\n", result->SongRecord.SongLength);
    } 
    else 
    {
        printf("Song with ID %s not found.\n", SongId);
    }
}

char *GetSongId(char *Operation)
{
    char *SongId = (char *)malloc(MAX_ID_LENGTH);
    printf("Enter Song id to %s: ", Operation);
    scanf("%s", SongId);
    return SongId;
}

void PrintSuccessMessage(char *Operation, const char *SongId)
{
    printf("Song with id - %s %sd successfully.\n", SongId, Operation);
}

void ShowMenu() 
{
    int choice;
    do 
    {
        printf("\n---------- Magical Music ----------\n");
        printf("0. Exit\n");
        printf("1. Add a Song\n");
        printf("2. Show all Songs\n");
        printf("3. Update Song details\n");
        printf("4. Delete a Song\n");
        printf("5. Search a Song\n");
        printf("------------------------------------\n\n");

        printf("Choose any option: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 0: 
                printf("You chose to exit.\n");
                break;
            case 1: 
                AddSong();
                break;
            case 2: 
                ShowAllSongs();
                break;
            case 3: 
                Operation = "update";
                UpdateSongDetails(GetSongId(Operation));
                break;
            case 4: 
                Operation = "delete";
                DeleteSong(&RootNode, GetSongId(Operation));
                break;
            case 5: 
                Operation = "search";
                SearchSong(GetSongId(Operation));
                break;
            default: 
                printf("You entered an invalid choice.\n");
        }
    } while (choice != 0);
}