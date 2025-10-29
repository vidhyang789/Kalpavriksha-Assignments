#include <stdio.h>
#include <stdlib.h>

typedef struct Product
{
    int id;
    char name[50];
    int price;
    int quantity;
} Product;

int lastProductIndex = 0;

void printMenu(){
    printf("\n========= INVENTORY MENU =========\n");
    printf("1. Add New Product\n");
    printf("2. View All Products\n");
    printf("3. Update Quantity\n");
    printf("4. Search Product by ID\n");
    printf("5. Search Product by Name\n");
    printf("6. Search Product by Price Range\n");
    printf("7. Delete Product\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
}

void inputProductDetails(Product* listOfProducts, int index){
    printf("Product ID : ");
    scanf("%d",&((listOfProducts + index)->id));
    printf("\nProduct Name : ");
    scanf("%s",(listOfProducts + index)->name);
    printf("\nProduct Prict : ");
    scanf("%d",&((listOfProducts + index)->price));
    printf("\nProduct Quantity : ");
    scanf("%d",&((listOfProducts + index)->quantity));
    printf("\n");
}

void addNewProduct(Product **listOfProducts) {
    Product *temp = realloc(*listOfProducts, (lastProductIndex + 1) * sizeof(Product));
    if (temp == NULL) {
        printf("Memory allocation failed!\n");
    }
    else{
        *listOfProducts = temp;

        printf("Enter new product details: \n");
        inputProductDetails(*listOfProducts, lastProductIndex);
        lastProductIndex++;

        printf("Product added successfully!\n");
    }
}

void viewAllProduct(Product *listOfProducts){
    printf("Product Details\n");
    printf("Last Product Index : %d\n", lastProductIndex);
    for(int index=0;index<lastProductIndex;index++){
        printf("Product ID: %d | Name: %s | Price: %d | Quantity: %d \n", 
            (listOfProducts + index)->id, (listOfProducts + index)->name,
            (listOfProducts + index)->price, (listOfProducts + index)->quantity);
    }
}

void updateQuantity(Product **listOfProducts){
    int productIdForUpdate,newQuantity;
    printf("Enter Product ID to update quantity : ");
    scanf("%d", &productIdForUpdate);
    printf("Enter New Quantity : ");
    scanf("%d", &newQuantity);
    
}

void displayMenuTakingChoice(Product **listOfProducts){

    int choice = -1;
    
    while(1){
        printMenu();
        scanf("%d", &choice);
        switch(choice){
            case 1:
                addNewProduct(listOfProducts);
                break;
            case 2:
                viewAllProduct(*listOfProducts);
                break;
            case 3:
                updateQuantity(listOfProducts);
                break;
            case 4:
                // searchProductById();
                break;
            case 5:
                // searchProductByName();
                break;
            case 6:
                // searchProductByPriceRange();
                break;
            case 7:
                // deleteProduct();
                break;
            case 8:
                exit(0);
                break;
            default:
                printf("Enter valid choice : ");

        }
    }
}

void inputInitialProducts(Product *listOfProducts,int initialNumberOfProducts){
    for(int index = 0;index < initialNumberOfProducts;index++){
        printf("Enter details for product %d: \n", index+1);
        inputProductDetails(listOfProducts, index); 
    }
    lastProductIndex = initialNumberOfProducts;
}

int main(){

    int initialNumberOfProducts;

    printf("Enter the initial number of products : ");
    scanf("%d", &initialNumberOfProducts);

    Product* listOfProducts = calloc(initialNumberOfProducts, sizeof(Product));

    inputInitialProducts(listOfProducts, initialNumberOfProducts);
    displayMenuTakingChoice(&listOfProducts);

    return 0;
}