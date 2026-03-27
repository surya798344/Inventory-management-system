 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct user {
    char username[30];
    char password[30];
};

struct product {
    int id;
    char name[50];
    int quantity;
    float price;
};

int login();
void registerUser();
void addProduct();
void displayProducts();
void searchProduct();
void updateProduct();
void deleteProduct();

int login() {
    FILE *fp;
    struct user u;
    char uname[30], pass[30];
    int found = 0;

    fp = fopen("users.dat", "rb");
    if(fp == NULL) {
        printf("No users found. Please register first.\n");
        return 0;
    }

    printf("Username: ");
    scanf("%s", uname);
    printf("Password: ");
    scanf("%s", pass);

    while(fread(&u, sizeof(u), 1, fp)) {
        if(strcmp(uname, u.username) == 0 && strcmp(pass, u.password) == 0) {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if(found) {
        printf("Login Successful!\n");
        return 1;
    } else {
        printf("Invalid Credentials!\n");
        return 0;
    }
}

void registerUser() {
    FILE *fp;
    struct user u;

    fp = fopen("users.dat", "ab");

    printf("Enter Username: ");
    scanf("%s", u.username);
    printf("Enter Password: ");
    scanf("%s", u.password);

    fwrite(&u, sizeof(u), 1, fp);
    fclose(fp);

    printf("Registration Successful!\n");
}

void addProduct() {
    FILE *fp;
    struct product p;

    fp = fopen("inventory.dat", "ab");

    printf("Enter Product ID: ");
    scanf("%d", &p.id);
    printf("Enter Name: ");
    scanf("%s", p.name);
    printf("Enter Quantity: ");
    scanf("%d", &p.quantity);
    printf("Enter Price: ");
    scanf("%f", &p.price);

    fwrite(&p, sizeof(p), 1, fp);
    fclose(fp);

    printf("Product Added Successfully!\n");
}

void displayProducts() {
    FILE *fp;
    struct product p;

    fp = fopen("inventory.dat", "rb");
    if(fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("\nID\tName\tQty\tPrice\n");

    while(fread(&p, sizeof(p), 1, fp)) {
        printf("%d\t%s\t%d\t%.2f\n", p.id, p.name, p.quantity, p.price);
    }

    fclose(fp);
}

void searchProduct() {
    FILE *fp;
    struct product p;
    int id, found = 0;

    printf("Enter Product ID to search: ");
    scanf("%d", &id);

    fp = fopen("inventory.dat", "rb");

    while(fread(&p, sizeof(p), 1, fp)) {
        if(p.id == id) {
            printf("Found: %d %s %d %.2f\n", p.id, p.name, p.quantity, p.price);
            found = 1;
        }
    }

    fclose(fp);

    if(!found)
        printf("Product not found!\n");
}

void updateProduct() {
    FILE *fp;
    struct product p;
    int id, found = 0;

    printf("Enter Product ID to update: ");
    scanf("%d", &id);

    fp = fopen("inventory.dat", "rb+");

    while(fread(&p, sizeof(p), 1, fp)) {
        if(p.id == id) {
            printf("Enter New Quantity: ");
            scanf("%d", &p.quantity);
            printf("Enter New Price: ");
            scanf("%f", &p.price);

            fseek(fp, -sizeof(p), SEEK_CUR);
            fwrite(&p, sizeof(p), 1, fp);

            found = 1;
            break;
        }
    }

    fclose(fp);

    if(found)
        printf("Product Updated!\n");
    else
        printf("Product not found!\n");
}

void deleteProduct() {
    FILE *fp, *temp;
    struct product p;
    int id, found = 0;

    printf("Enter Product ID to delete: ");
    scanf("%d", &id);

    fp = fopen("inventory.dat", "rb");
    temp = fopen("temp.dat", "wb");

    while(fread(&p, sizeof(p), 1, fp)) {
        if(p.id != id) {
            fwrite(&p, sizeof(p), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("inventory.dat");
    rename("temp.dat", "inventory.dat");

    if(found)
        printf("Product Deleted!\n");
    else
        printf("Product not found!\n");
}

int main() {
    int choice;
    int loggedIn = 0;

    do {
        printf("\n--- Inventory Management System ---\n");
        printf("1. Register\n2. Login\n3. Add Product\n4. Display Products\n");
        printf("5. Search Product\n6. Update Product\n7. Delete Product\n8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: registerUser(); break;
            case 2: loggedIn = login(); break;
            case 3: if(loggedIn) addProduct(); else printf("Login first!\n"); break;
            case 4: if(loggedIn) displayProducts(); else printf("Login first!\n"); break;
            case 5: if(loggedIn) searchProduct(); else printf("Login first!\n"); break;
            case 6: if(loggedIn) updateProduct(); else printf("Login first!\n"); break;
            case 7: if(loggedIn) deleteProduct(); else printf("Login first!\n"); break;
            case 8: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }

    } while(choice != 8);

    return 0;
}