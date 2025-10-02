#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

struct User {
    int id;
    char name[50];
    int age;
};

const char *FILENAME = "users.txt";

void createUser() {
    FILE *fp = fopen(FILENAME, "a");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    struct User u;
    printf("Enter ID: ");
    scanf("%d", &u.id);
    printf("Enter Name: ");
    scanf("%s", u.name);
    printf("Enter Age: ");
    scanf("%d", &u.age);

    fprintf(fp, "%d %s %d\n", u.id, u.name, u.age);
    fclose(fp);
    printf("User added successfully!\n");
}

// Function to display all users
void readUsers() {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No users found (file missing).\n");
        return;
    }

    struct User u;
    printf("\n--- User Records ---\n");
    while (fscanf(fp, "%d %s %d", &u.id, u.name, &u.age) != EOF) {
        printf("ID: %d | Name: %s | Age: %d\n", u.id, u.name, u.age);
    }
    fclose(fp);
}

// Function to update a user by ID
void updateUser() {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No users found.\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    struct User u;
    int id, found = 0;

    printf("Enter ID to update: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %d", &u.id, u.name, &u.age) != EOF) {
        if (u.id == id) {
            found = 1;
            printf("Enter new Name: ");
            scanf("%s", u.name);
            printf("Enter new Age: ");
            scanf("%d", &u.age);
        }
        fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found)
        printf("User updated successfully!\n");
    else
        printf("User with ID %d not found.\n", id);
}

// Function to delete a user by ID
void deleteUser() {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No users found.\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    struct User u;
    int id, found = 0;

    printf("Enter ID to delete: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %d", &u.id, u.name, &u.age) != EOF) {
        if (u.id == id) {
            found = 1;
            continue; // skip writing this user
        }
        fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found)
        printf("User deleted successfully!\n");
    else
        printf("User with ID %d not found.\n", id);
}

// Main menu
int main() {
    int choice;
    while (1) {
        printf("\n--- CRUD Menu ---\n");
        printf("1. Create User\n");
        printf("2. Read Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createUser(); break;
            case 2: readUsers(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 5: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}
