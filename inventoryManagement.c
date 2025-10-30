#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Product
{
    int id;
    char name[50];
    float price;
    int quantity;
} Product;

typedef enum
{
    ADD_PRODUCT = 1,
    VIEW_ALL_PRODUCTS,
    UPDATE_QUANTITY,
    SEARCH_PRODUCT_BY_ID,
    SEARCH_PRODUCT_BY_NAME,
    SEARCH_PRODUCT_BY_PRICE,
    DELETE_PRODUCT,
    EXIT
} MenuOptions;

int totalProducts = 0;

void printMenu()
{
    printf("\n\n========= INVENTORY MENU =========\n");
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

void inputProductDetails(const Product *listOfProducts, int index)
{
    printf("Product ID : ");
    scanf("%d", &((listOfProducts + index)->id));
    printf("Product Name : ");
    scanf("%s", (listOfProducts + index)->name);
    printf("Product Price : ");
    scanf("%f", &((listOfProducts + index)->price));
    printf("Product Quantity : ");
    scanf("%d", &((listOfProducts + index)->quantity));
}

void addProduct(Product **listOfProducts)
{
    Product *temp = realloc(*listOfProducts, (totalProducts + 1) * sizeof(Product));
    if (temp == NULL)
    {
        printf("Memory allocation failed!\n");
    }
    else
    {
        *listOfProducts = temp;
        printf("\nEnter new product details: \n");
        inputProductDetails(*listOfProducts, totalProducts);
        totalProducts++;

        printf("Product added successfully!\n");
    }
}

void displayProducts(const Product *listOfProducts)
{
    printf("\n");
    printf("========= PRODUCT LIST ========= \n");
    for (int index = 0; index < totalProducts; index++)
    {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d \n",
               (listOfProducts + index)->id, (listOfProducts + index)->name,
               (listOfProducts + index)->price, (listOfProducts + index)->quantity);
    }
}

void updateQuantity(Product *listOfProducts)
{
    int productId, newQuantity;

    printf("\nEnter Product ID to update quantity : ");
    scanf("%d", &productId);
    printf("Enter New Quantity : ");
    scanf("%d", &newQuantity);

    bool updated = false;

    for (int index = 0; index < totalProducts; index++)
    {
        if ((listOfProducts + index)->id == productId)
        {
            (listOfProducts + index)->quantity = newQuantity;
            updated = true;
            break;
        }
    }

    if (!updated)
    {
        printf("There is no Product With This Id");
    }
    else
    {
        printf("Quantity Updated Successfully");
    }
}

void searchProductById(const Product *listOfProducts)
{
    int productId;

    printf("\nEnter Product Id To Search : ");
    scanf("%d", &productId);

    bool found = false;

    for (int index = 0; index < totalProducts; index++)
    {
        if ((listOfProducts + index)->id == productId)
        {
            printf("Product Found : ");
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d \n",
                   (listOfProducts + index)->id, (listOfProducts + index)->name,
                   (listOfProducts + index)->price, (listOfProducts + index)->quantity);
            found = true;
            break;
        }
    }

    if (!found)
    {
        printf("There is no Product With this Id");
    }
}

bool isSubstring(const char *productName, const char *searchName)
{
    int productNameLen = 0, searchNameLen = 0;

    while (productName[productNameLen] != '\0')
    {
        productNameLen++;
    }
    while (searchName[searchNameLen] != '\0')
    {
        searchNameLen++;
    }

    bool ans = false;

    if (searchNameLen != 0 && searchNameLen <= productNameLen)
    {
        for (int index = 0; index <= productNameLen - searchNameLen; index++)
        {
            int index2;
            for (index2 = 0; index2 < searchNameLen; index2++)
            {
                if (productName[index + index2] != searchName[index2])
                {
                    break;
                }
            }
            if (index2 == searchNameLen)
            {
                ans = true;
            }
        }
    }

    return ans;
}

void searchProductByName(const Product *listOfProducts)
{
    char productName[50];
    bool found = false;

    printf("\nEnter Name To search (Partial Allowed): ");
    scanf("%s", productName);

    for (int index = 0; index < totalProducts; index++)
    {
        if (isSubstring((listOfProducts + index)->name, productName))
        {
            if (!found)
            {
                printf("\nProducts Found:\n");
            }
            displayProduct(listOfProducts, index);
            found = true;
        }
    }

    if (!found)
    {
        printf("\nNo Products Found.\n");
    }
}

void searchProductByPriceRange(const Product *listOfProducts)
{
    float maxPrice, minPrice;
    bool found = false;

    printf("\nEnter Minimum Price : ");
    scanf("%f", &minPrice);

    printf("Enter Maximum Price : ");
    scanf("%f", &maxPrice);

    for (int index = 0; index < totalProducts; index++)
    {
        if ((listOfProducts + index)->price >= minPrice &&
            (listOfProducts + index)->price <= maxPrice)
        {
            if (!found)
            {
                printf("Products In Price Range : \n");
            }
            displayProduct(listOfProducts, index);
            found = true;
        }
    }

    if (!found)
    {
        printf("No Products Found In Range");
    }
}

void deleteProduct(Product **listOfProducts)
{
    int productId;
    int found = -1;

    printf("\nEnter Product ID to delete: ");
    scanf("%d", &productId);

    for (int index = 0; index < totalProducts; index++)
    {
        if ((*listOfProducts)[index].id == productId)
        {
            found = index;
            break;
        }
    }

    if (found == -1)
    {
        printf("Product not found! with given id \n");
    }
    else
    {
        for (int index = found; index < totalProducts - 1; index++)
        {
            (*listOfProducts)[index] = (*listOfProducts)[index + 1];
        }

        totalProducts--;

        Product *temp = realloc(*listOfProducts, totalProducts * sizeof(Product));
        if (temp != NULL || totalProducts == 0)
        {
            *listOfProducts = temp;
        }

        printf("Product deleted successfully!\n");
    }
}

void displayMenuTakingChoice(Product **listOfProducts)
{
    int choice = -1;

    while (choice != 8)
    {
        printMenu();
        scanf("%d", &choice);
        switch (choice)
        {
        case ADD_PRODUCT:
            addProduct(listOfProducts);
            break;
        case VIEW_ALL_PRODUCTS:
            displayProducts(*listOfProducts);
            break;
        case UPDATE_QUANTITY:
            updateQuantity(*listOfProducts);
            break;
        case SEARCH_PRODUCT_BY_ID:
            searchProductById(*listOfProducts);
            break;
        case SEARCH_PRODUCT_BY_NAME:
            searchProductByName(*listOfProducts);
            break;
        case SEARCH_PRODUCT_BY_PRICE:
            searchProductByPriceRange(*listOfProducts);
            break;
        case DELETE_PRODUCT:
            deleteProduct(listOfProducts);
            break;
        case EXIT:
            printf("\nMemory released successfully. Exiting program... ");
            break;
        default:
            printf("Enter valid choice : ");
        }
    }
}

void inputInitialProducts(Product *listOfProducts, int initialNumberOfProducts)
{
    for (int index = 0; index < initialNumberOfProducts; index++)
    {
        printf("\nEnter details for product %d: \n", index + 1);
        inputProductDetails(listOfProducts, index);
    }
    totalProducts = initialNumberOfProducts;
}

int main()
{

    int initialNumberOfProducts;

    printf("Enter the initial number of products : ");
    scanf("%d", &initialNumberOfProducts);

    Product *listOfProducts = calloc(initialNumberOfProducts, sizeof(Product));

    inputInitialProducts(listOfProducts, initialNumberOfProducts);
    displayMenuTakingChoice(&listOfProducts);

    free(listOfProducts);

    return 0;
}
