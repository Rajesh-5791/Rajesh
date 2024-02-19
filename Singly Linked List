// To perform CRUDS operations on item domain using linked list

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID_LENGTH 20
#define MAX_DESCRIPTION_LENGTH 50
#define SORT_BY_ATTRIBUTE_NAME_LENGTH 10
#define ITEM_NOT_FOUND_MESSAGE printf("Item not found with id: %s\n", targetItemId)
#define DATA_FILE "items.dat"

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
void saveItemsLinkedList();
void loadItemsLinkedList();
ItemDetails getItemDetails();
void showSortMenu();
void sortItemsLinkedList(int option, char *);

int main() 
{
    loadItemsLinkedList();
    showMenu();
    return 0;
}

void showSortMenu()
{
    int sortOption;
    char *sortByAttributeName = (char *)malloc(SORT_BY_ATTRIBUTE_NAME_LENGTH);
    printf("Sort menu: \n");
    printf("1. Sort by id\n");
    printf("2. Sort by price\n\n");

    printf("Enter your option: ");
    scanf("%d", &sortOption);
    switch (sortOption)
    {
        case 1: sortByAttributeName = "id";
            sortItemsLinkedList(sortOption, sortByAttributeName);
            break;
        case 2: sortByAttributeName = "price";
            sortItemsLinkedList(sortOption, sortByAttributeName);
            break;
        default : printf("You entered Invalid choice.\n");
    }
}

void showMenu() 
{
    int choice;
    do 
    {
        printf("\n====== Rajesh Mart ======\n");
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
                printf("You chose exit.\n");
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
                showSortMenu();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}

void saveItemsLinkedList() 
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

void loadItemsLinkedList() 
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
    
    saveItemsLinkedList();
    printf("Item added successfully with id: %s\n", newNode->itemRecord.itemId);
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
            saveItemsLinkedList();
            printf("Item deleted successfully with id: %s\n", targetItemId);
            break;
        }
        previousNode = currentNode;
        currentNode = currentNode->nextNode;
    }
    ITEM_NOT_FOUND_MESSAGE;
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
            saveItemsLinkedList();
            printf("Price updated successfully.\n");
            break;
        }
        currentNode = currentNode->nextNode;
    }
    ITEM_NOT_FOUND_MESSAGE;
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
            break;
        }
        currentNode = currentNode->nextNode;
    }
    ITEM_NOT_FOUND_MESSAGE;
}

void sortItemsLinkedList(int option, char *sortByAttributeName)
{
    printf("Sorting items...\n");
    if (startNode == NULL)
    {
        printf("No items to sort.\n");
        return;
    }
    printf("You chose Sort by %s.\n", sortByAttributeName);

    ItemNode *currentNode;
    ItemDetails tempNode;
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
                tempNode = currentNode->itemRecord;
                currentNode->itemRecord = currentNode->nextNode->itemRecord;
                currentNode->nextNode->itemRecord = tempNode;
                swapped = 1;
            }
            currentNode = currentNode->nextNode;
        }
    } while (swapped);

    printf("Items sorted by %s successfully.\n", sortByAttributeName);
    showAllItems();
    startNode = NULL; 
    loadItemsLinkedList();
}
