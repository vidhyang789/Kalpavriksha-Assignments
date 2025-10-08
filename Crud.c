#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User
{
    int id;
    char name[50];
    int age;
};

const char *USER_DATA_FILE = "users.txt";

FILE *openFile(const char *filename, const char *mode)
{
    FILE *filePtr = fopen(filename, mode);
    if (!filePtr)
    {
        printf("Error: Unable to open file '%s'!\n", filename);
    }
    return filePtr;
}

void closeFile(FILE *filePtr)
{
    if (filePtr != NULL)
    {
        fclose(filePtr);
    }
}

struct User readUserData()
{
    struct User newUser;
    printf("Enter User ID: ");
    scanf("%d", &newUser.id);
    printf("Enter Name: ");
    scanf("%s", newUser.name);
    printf("Enter Age: ");
    scanf("%d", &newUser.age);
    return newUser;
}

void createUser()
{
    FILE *filePtr = openFile(USER_DATA_FILE, "a");
    if (!filePtr)
    {
        return;
    }
    struct User newUser = readUserData();
    fprintf(filePtr, "%d %s %d\n", newUser.id, newUser.name, newUser.age);
    closeFile(filePtr);
    printf("✅ User added successfully!\n");
}

void readAllUsers()
{
    FILE *filePtr = openFile(USER_DATA_FILE, "r");
    if (!filePtr)
    {
        printf("No users found (file missing).\n");
        return;
    }
    struct User currentUser;
    printf("\n--- User Records ---\n");
    while (fscanf(filePtr, "%d %s %d", &currentUser.id, currentUser.name, &currentUser.age) != EOF)
    {
        printf("ID: %d | Name: %s | Age: %d\n", currentUser.id, currentUser.name, currentUser.age);
    }
    closeFile(filePtr);
}

void updateUser()
{
    FILE *filePtr = openFile(USER_DATA_FILE, "r");
    if (!filePtr)
    {
        printf("No users found.\n");
        return;
    }
    FILE *tempFile = openFile("temp.txt", "w");
    if (!tempFile)
    {
        closeFile(filePtr);
        return;
    }
    struct User currentUser;
    int userIdToUpdate, isFound = 0;
    printf("Enter User ID to update: ");
    scanf("%d", &userIdToUpdate);
    while (fscanf(filePtr, "%d %s %d", &currentUser.id, currentUser.name, &currentUser.age) != EOF)
    {
        if (currentUser.id == userIdToUpdate)
        {
            printf("Enter new details for this user:\n");
            currentUser = readUserData();
            currentUser.id = userIdToUpdate;
            isFound = 1;
        }
        fprintf(tempFile, "%d %s %d\n", currentUser.id, currentUser.name, currentUser.age);
    }
    closeFile(filePtr);
    closeFile(tempFile);
    remove(USER_DATA_FILE);
    rename("temp.txt", USER_DATA_FILE);
    if (isFound)
    {
        printf("User updated successfully!\n");
    }
    else
    {
        printf("User with ID %d not found.\n", userIdToUpdate);
    }
}

void deleteUser()
{
    FILE *filePtr = openFile(USER_DATA_FILE, "r");
    if (!filePtr)
    {
        printf("No users found.\n");
        return;
    }
    FILE *tempFile = openFile("temp.txt", "w");
    if (!tempFile)
    {
        closeFile(filePtr);
        return;
    }
    struct User currentUser;
    int userIdToDelete, isFound = 0;
    printf("Enter User ID to delete: ");
    scanf("%d", &userIdToDelete);
    while (fscanf(filePtr, "%d %s %d", &currentUser.id, currentUser.name, &currentUser.age) != EOF)
    {
        if (currentUser.id == userIdToDelete)
        {
            isFound = 1;
            continue;
        }
        fprintf(tempFile, "%d %s %d\n", currentUser.id, currentUser.name, currentUser.age);
    }
    closeFile(filePtr);
    closeFile(tempFile);
    remove(USER_DATA_FILE);
    rename("temp.txt", USER_DATA_FILE);
    if (isFound)
    {
        printf("User deleted successfully!\n");
    }
    else
    {
        printf("User with ID %d not found.\n", userIdToDelete);
    }
}

int main()
{
    int menuChoice;
    enum CrudOperation
    {
        ADD_USER = 1,
        DISPLAY_USERS = 2,
        UPDATE_USER = 3,
        DELETE_USER = 4,
        EXIT_APP = 5
    };
    while (1)
    {
        printf("\n--- USER MANAGEMENT MENU ---\n");
        printf("1. Create User\n");
        printf("2. Display All Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &menuChoice);
        switch ((enum CrudOperation)menuChoice)
        {
        case ADD_USER:
        {
            createUser();
            break;
        }
        case DISPLAY_USERS:
        {
            readAllUsers();
            break;
        }
        case UPDATE_USER:
        {
            updateUser();
            break;
        }
        case DELETE_USER:
        {
            deleteUser();
            break;
        }
        case EXIT_APP:
        {
            printf("Exiting program...\n");
        }
        default:
        {
            printf("Invalid choice! Please try again.\n");
        }
        }
    }
    return 0;
}
