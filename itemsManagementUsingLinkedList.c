// To perform CRUDS operations on item domain using linked list

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID_LENGTH 20
#define MAX_DESCRIPTION_LENGTH 50
#define DATA_FILE "items4.dat"

typedef struct ItemDetails 
{
    char itemId[MAX_ID_LENGTH];
    char itemDescription[MAX_DESCRIPTION_LENGTH];
    float itemPrice;
} ItemDetails;

typedef struct ItemNode 
{
    ItemDetails itemRecord;
    struct ItemNode *nextNode;
} ItemNode;

ItemNode *startNode = NULL;

void showMenu();
void addItem();
void showAllItems();
void updateItemPrice(char *);
void deleteItem(char *);
void searchItem(char *);
char *getTargetItemId();
void saveLinkedList();
void loadLinkedList();
ItemDetails getItemDetails();
int showSortMenu();
void sortItems(int option);

int main() 
{
    loadLinkedList();
    showMenu();
    return 0;
}

int showSortMenu()
{
    int sortOption;
    printf("Sort menu: \n");
    printf("1. Sort by id\n");
    printf("2. Sort by price\n\n");

    printf("Enter your option: ");
    scanf("%d", &sortOption);
    return sortOption;
}

void showMenu() 
{
    int choice;
    do 
    {
        printf("====== Rajesh Mart ======\n");
        printf("0. Exit\n");
        printf("1. Add an item\n");
        printf("2. Show all items\n");
        printf("3. Update item price\n");
        printf("4. Delete an item\n");
        printf("5. Search an item\n");
        printf("6. Sort all items\n");
        printf("=========================\n\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 0:
                printf("Exiting...\n");
                break;
            case 1:
                addItem();
                break;
            case 2:
                showAllItems();
                break;
            case 3:
                updateItemPrice(getTargetItemId());
                break;
            case 4:
                deleteItem(getTargetItemId());
                break;
            case 5:
                searchItem(getTargetItemId());
                break;
            case 6: 
                sortItems(showSortMenu());
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}

void saveLinkedList() 
{
    FILE *fpItems = fopen(DATA_FILE, "wb");
    ItemNode *currentNode = startNode;
    while (currentNode != NULL) 
    {
        fwrite(&(currentNode->itemRecord), sizeof(ItemDetails), 1, fpItems);
        currentNode = currentNode->nextNode;
    }
    fclose(fpItems);
}

void loadLinkedList() 
{
    FILE *fpItems = fopen(DATA_FILE, "rb");
    ItemDetails tempItem;
    ItemNode *currentNode = NULL;
    while (fread(&tempItem, sizeof(ItemDetails), 1, fpItems)) 
    {
        ItemNode *newNode = (ItemNode *)malloc(sizeof(ItemNode));
        newNode->itemRecord = tempItem;
        newNode->nextNode = NULL;
        if (startNode == NULL)
        {
            startNode = newNode;
        }
        else
        {
            currentNode->nextNode = newNode;
        }
        currentNode = newNode;
    }
    fclose(fpItems);
}

void addItem() 
{
    ItemNode *newNode = (ItemNode *)malloc(sizeof(ItemNode));
    newNode->itemRecord = getItemDetails();
    newNode->nextNode = NULL;
    
    if (startNode == NULL)
    {
        startNode = newNode;
    }
    else
    {
        ItemNode *currentNode = startNode;
        while (currentNode->nextNode != NULL)
        {
            currentNode = currentNode->nextNode;
        }
        currentNode->nextNode = newNode;
    }
    
    saveLinkedList();
    printf("Item added successfully.\n");
}

void showAllItems() 
{
    if (startNode == NULL) 
    {
        printf("No items found.\n");
        return;
    }
    ItemNode *currentNode = startNode;
    int counter = 0;
    while (currentNode != NULL) 
    {
        counter++;
        printf("Details of item - %d:\n", counter);
        printf("ID: %s\n", currentNode->itemRecord.itemId);
        printf("Description: %s\n", currentNode->itemRecord.itemDescription);
        printf("Price: %.2f\n", currentNode->itemRecord.itemPrice);
        printf("-------------------------\n");
        currentNode = currentNode->nextNode;
    }
}

void deleteItem(char *targetItemId) 
{
    if (startNode == NULL) 
    {
        printf("No items to delete.\n");
        return;
    }
    ItemNode *currentNode = startNode;
    ItemNode *previousNode = NULL;
    while (currentNode != NULL) 
    {
        if (strcmp(currentNode->itemRecord.itemId, targetItemId) == 0) 
        {
            if (previousNode == NULL) 
            {
                startNode = currentNode->nextNode;
            } 
            else 
            {
                previousNode->nextNode = currentNode->nextNode;
            }
            free(currentNode);
            saveLinkedList();
            printf("Item deleted successfully.\n");
            return;
        }
        previousNode = currentNode;
        currentNode = currentNode->nextNode;
    }
    printf("Item not found.\n");
}

char *getTargetItemId() 
{
    char *targetItemId = (char *)malloc(MAX_ID_LENGTH * sizeof(char));
    printf("Enter item id: ");
    scanf("%s", targetItemId);
    return targetItemId;
}

ItemDetails getItemDetails() 
{
    ItemDetails newItem;
    printf("Enter item ID: ");
    scanf("%s", newItem.itemId);
    printf("Enter item description: ");
    scanf("%s", newItem.itemDescription);
    printf("Enter item price: ");
    scanf("%f", &newItem.itemPrice);
    return newItem;
}

void updateItemPrice(char *targetItemId)
{
    if (startNode == NULL)
    {
        printf("No items to update.\n");
        return;
    }
    ItemNode *currentNode = startNode;
    while (currentNode != NULL)
    {
        if (strcmp(currentNode->itemRecord.itemId, targetItemId) == 0)
        {
            printf("Enter the new price for the item: ");
            scanf("%f", &currentNode->itemRecord.itemPrice);
            saveLinkedList();
            printf("Price updated successfully.\n");
            return;
        }
        currentNode = currentNode->nextNode;
    }
    printf("Item not found.\n");
}

void searchItem(char *targetItemId)
{
    if (startNode == NULL)
    {
        printf("No items to search.\n");
        return;
    }

    ItemNode *currentNode = startNode;
    while (currentNode != NULL)
    {
        if (strcmp(currentNode->itemRecord.itemId, targetItemId) == 0)
        {
            printf("Item details:\n");
            printf("ID: %s\n", currentNode->itemRecord.itemId);
            printf("Description: %s\n", currentNode->itemRecord.itemDescription);
            printf("Price: %.2f\n", currentNode->itemRecord.itemPrice);
            return;
        }
        currentNode = currentNode->nextNode;
    }
    printf("Item not found.\n");
}

void sortItems(int option)
{
    printf("hi\n");
    printf("Sorting items...\n");
    if (startNode == NULL)
    {
        printf("No items to sort.\n");
        return;
    }

    printf("Sorting option: %d\n", option);

    ItemNode *currentNode;
    ItemDetails temp;
    int swapped;

    do
    {
        swapped = 0;
        currentNode = startNode;

        while (currentNode->nextNode != NULL)
        {
            if ((option == 1 && strcmp(currentNode->itemRecord.itemId, currentNode->nextNode->itemRecord.itemId) > 0) ||
                (option == 2 && currentNode->itemRecord.itemPrice > currentNode->nextNode->itemRecord.itemPrice))
            {
                temp = currentNode->itemRecord;
                currentNode->itemRecord = currentNode->nextNode->itemRecord;
                currentNode->nextNode->itemRecord = temp;
                swapped = 1;
            }
            currentNode = currentNode->nextNode;
        }
    } while (swapped);

    printf("Items sorted successfully.\n");
}