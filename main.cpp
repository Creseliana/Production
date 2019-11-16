#include <iostream>
#include <string.h>
#include <dirent.h>
#include <windows.h>

#define ARRAY_SIZE 50
#define NAME_SIZE 21
#define USER_SIZE 20
#define PATH_SIZE 25
#define LOGIN_PATH_MANAGER "login//login_manager.txt"
#define LOGIN_PATH_ADMIN "login//login_admin.txt"
#define ROLE_ADMIN 1
#define ROLE_MANAGER 2
#define ROLE_USER 3
#define ROLE_NONE 0

void application();
void mainMenu(int);
void adminMenu();
void managerMenu();
void userMenu();
void startMenu(int);

/*functions for working with files*/
void createFileByUser();
int createNewFile(char *);
int checkFileExist(char *);
void chooseFile();
int showAllTXTFiles();
int checkFileName(char *);
void writeFromFileToProductArray(char *);
void writeFromProductArrayToFile(char *);
void addLineToFile(char *);

/*functions for working with the array of structures*/
void addProduct();
void editProduct();
void editProductByName(char *);
void editProductById(char *, int *, int);
void deleteProduct();
void deleteProductByName(char *);
void deleteProductById(char *, int *, int *, int);
void deleteProductByIndex(int);
void sortProductsMenu();
void sortByDepartment(int);
void sortByName(int);
void sortByAmount(int);
void searchProductsMenu();
void searchByDepartment(int);
void searchByProductName(char *);
void searchByAmount(int);
void searchByAmount(int, int);

/*print functions*/
void printProduct(int);
void printAllProducts(int);

/*auxiliary functions*/
int simpleUserChoice();
int checkEnteredId(int, int *, int);
void clearProductArray();
int getNumber();
char *getString();
int isProductArrayEmpty();
void swapElements(int);
int getLastID();
void isExitToMenu(char *);
void userInputOfProduct(int);
void exitToMenu();

/*functions for work with users*/
char *encrypt(char *);
char *decrypt(char *);
void createUser();
void editUser();
int verifyUser(const char *, char *, char *);
int checkNewUserLogin(FILE *, char *);
int getUserRole();
void createEditUserMenu();
void printAllManagerLogin();

struct production
{
    int id;
    int department;
    char productName[NAME_SIZE];
    int productAmount;
};

struct authCredentials
{
    char login[NAME_SIZE];
    char password[NAME_SIZE];
};

production products[ARRAY_SIZE];
unsigned int currentPosition = 0;
unsigned int productID = 0;
char fileName[NAME_SIZE];
FILE *file;
int userRole = ROLE_NONE;

int main()
{
    SetConsoleTitle(TEXT("Production"));
    application();
    return 0;
}

/**
 * Function gets user role and makes login and password verification.
 * If user is verified it redirects user to next menu.
 */
void application()
{
    int userChoiceRole;
    int isVerified = 0;
    char login[NAME_SIZE];
    char password[NAME_SIZE];
    char filePath[PATH_SIZE];
    do
    {
        userChoiceRole = getUserRole();
        if (userChoiceRole == ROLE_USER)
        {
            userRole = userChoiceRole;
            startMenu(userRole);
            break;
        } else if (userChoiceRole == ROLE_MANAGER)
        {
            strcpy(filePath, LOGIN_PATH_MANAGER);
        } else if (userChoiceRole == ROLE_ADMIN)
        {
            strcpy(filePath, LOGIN_PATH_ADMIN);
        }
        printf("\nEnter login & password.\n");
        printf("Login: ");
        strcpy(login, getString());
        printf("Password: ");
        strcpy(password, getString());
        if (verifyUser(filePath, login, password) == 1)
        {
            isVerified = 1;
            userRole = userChoiceRole;
            startMenu(userRole);
        } else
        {
            printf("\nWrong login or password.\n");
        }
    } while (isVerified != 1);
}

/**
 * Get user input and return it if it matches the condition
 * @param userChoice - gets user input
 * @return userChoice - if user input matches the condition
 */
int getUserRole()
{
    int userChoice;
    do
    {
        printf("----\nMenu\n----\n");
        printf("1. Administrator\n2. Manager\n3. User\n0. Exit\n");
        printf("\nInput: ");
        userChoice = getNumber();
        if (userChoice == 0)
        {
            exit(EXIT_SUCCESS);
        } else if (userChoice < 1 || userChoice > 3)
        {
            printf("\nThere is no such user role. Try again.\n");
        }
    } while (userChoice < 0 || userChoice > 3);
    return userChoice;
}

/**
 * Redirects user to user menu by role parameter
 * @param role - used to redirect in certain menu
 */
void mainMenu(int role)
{
    switch (role)
    {
        case ROLE_ADMIN:
            adminMenu();
            break;
        case ROLE_MANAGER:
            managerMenu();
            break;
        case ROLE_USER:
            userMenu();
            break;
        default:
            printf("\nUnexpected error.\n");
            application();
    }
}

/**
 * Start menu for each user. For admin create file, choose file, create/edit manager.
 * For manager create or choose file. For common user only choose file.
 * @param role
 */
void startMenu(int role)
{
    int userChoice;
    if (role == ROLE_ADMIN)
    {
        do
        {
            printf("----\nMenu\n----\n");
            printf("1. Create file\n2. Choose file\n3. Create/edit manager\n0. Exit to menu\n");
            printf("\nInput: ");
            userChoice = getNumber();
            switch (userChoice)
            {
                case 1:
                    createFileByUser();
                    break;
                case 2:
                    chooseFile();
                    break;
                case 3:
                    createEditUserMenu();
                    break;
                case 0:
                    exitToMenu();
                    break;
                default:
                    printf("\nWrong input. Try again.\n");
            }
        } while (userChoice < 0 || userChoice > 3);
    } else if (role == ROLE_MANAGER)
    {
        do
        {
            printf("----\nMenu\n----\n");
            printf("1. Create file\n2. Choose file\n0. Exit to menu\n");
            printf("\nInput: ");
            userChoice = getNumber();
            switch (userChoice)
            {
                case 1:
                    createFileByUser();
                    break;
                case 2:
                    chooseFile();
                    break;
                case 0:
                    exitToMenu();
                    break;
                default:
                    printf("\nWrong input. Try again.\n");
            }
        } while (userChoice < 0 || userChoice > 2);
    } else if (role == ROLE_USER)
    {
        do
        {
            printf("----\nMenu\n----\n");
            printf("1. Choose file\n0. Exit to menu\n");
            printf("\nInput: ");
            userChoice = getNumber();
            switch (userChoice)
            {
                case 1:
                    chooseFile();
                    break;
                case 0:
                    exitToMenu();
                    break;
                default:
                    printf("\nWrong input. Try again.\n");
            }
        } while (userChoice < 0 || userChoice > 1);
    }
}

/**
 * Menu for admin user. By user input redirects to certain action.
 */
void adminMenu()
{
    printf("\n----\nMenu\n----\n");
    printf("1. Create file\n2. Choose file\n3. Add product\n4. Edit product\n"
           "5. Delete product\n6. View products\n7. Sort products\n8. Search\n"
           "9. Create/edit manager\n0. Exit to menu\n");
    printf("\nInput: ");
    switch (getNumber())
    {
        case 1:
            createFileByUser();
            break;
        case 2:
            chooseFile();
            break;
        case 3:
            addProduct();
            break;
        case 4:
            editProduct();
            break;
        case 5:
            deleteProduct();
            break;
        case 6:
            printAllProducts(1);
            break;
        case 7:
            sortProductsMenu();
            break;
        case 8:
            searchProductsMenu();
            break;
        case 9:
            createEditUserMenu();
            break;
        case 0:
            exitToMenu();
            break;
        default:
            printf("\nWrong input. Try again.\n");
            adminMenu();
    }
}

/**
 * Menu for manager user. By user input redirects to certain action.
 */
void managerMenu()
{
    printf("\n----\nMenu\n----\n");
    printf("1. Create file\n2. Choose file\n3. Add product\n4. Edit product\n"
           "5. Delete product\n6. View products\n7. Sort products\n8. Search\n0. Exit to menu\n");
    printf("\nInput: ");
    switch (getNumber())
    {
        case 1:
            createFileByUser();
            break;
        case 2:
            chooseFile();
            break;
        case 3:
            addProduct();
            break;
        case 4:
            editProduct();
            break;
        case 5:
            deleteProduct();
            break;
        case 6:
            printAllProducts(1);
            break;
        case 7:
            sortProductsMenu();
            break;
        case 8:
            searchProductsMenu();
            break;
        case 0:
            exitToMenu();
            break;
        default:
            printf("\nWrong input. Try again.\n");
            managerMenu();
    }
}

/**
 * Menu for common user. By user input redirects to certain action.
 */
void userMenu()
{
    printf("\n----\nMenu\n----\n");
    printf("1. Choose file\n2. View products\n3. Search\n0. Exit to menu\n");
    printf("\nInput: ");
    switch (getNumber())
    {
        case 1:
            chooseFile();
            break;
        case 2:
            printAllProducts(1);
            break;
        case 3:
            searchProductsMenu();
            break;
        case 0:
            exitToMenu();
            break;
        default:
            printf("\nWrong input. Try again.\n");
            userMenu();
    }
}

/**
 * Get file name from user. Check if it is correct (name only with '.txt'.
 * If correct check does file with such name exist. If file with such name doesn't exist,
 * File will be created. If file with such name exists - propose user to use it.
 */
void createFileByUser()
{
    char userFileName[NAME_SIZE];
    int flagCheckFileName;
    do
    {
        printf("\nEnter file name.");
        printf("\nInput: ");
        strcpy(userFileName, getString());
        flagCheckFileName = checkFileName(userFileName);
        if (flagCheckFileName != 1)
        {
            printf("\nFile name should contain '.txt'. Try again.\n");
        }
    } while (flagCheckFileName != 1);

    if (checkFileExist(userFileName) == 0)
    {
        if (createNewFile(userFileName) == 1)
        {
            strcpy(fileName, userFileName);
            printf("\nFile %s is created.\n", fileName);
            mainMenu(userRole);
        } else
        {
            if (fileName[0] == '\0')
            {
                startMenu(userRole);
            } else
            {
                mainMenu(userRole);
            }
        }
    } else
    {
        printf("\nFile with such name already exists.\n");
        printf("Do you want use file %s?\n", userFileName);
        printf("1. Yes\n2. No\n");
        printf("\nInput: ");
        if (simpleUserChoice() == 1)
        {
            strcpy(fileName, userFileName);
            writeFromFileToProductArray(fileName);
            productID = getLastID();
            printf("\nFile %s is selected.\n", fileName);
            mainMenu(userRole);
        } else
        {
            if (fileName[0] == '\0')
            {
                startMenu(userRole);
            } else
            {
                printf("\nCurrent file is %s.\n"
                       "You can continue to use this file, "
                       "or create new file, or choose from existing.\n", fileName);
                mainMenu(userRole);
            }
        }
    }
}

/**
 * Creating file with given name
 * @param newFileName - name for the new file
 * @return 0 - if creation failed
 * @return 1 - if creation succeeded
 */
int createNewFile(char *newFileName)
{
    if (!(file = fopen(newFileName, "w")))
    {
        printf("\nFile cannot be created.\n");
        return 0;
    } else
    {
        clearProductArray();
        productID = 0;
        return 1;
    }
}

/**
 * Check is file with given name exist by trying to open it
 * @param checkName - file name that should be checked
 * @return 1 - if file with such name already exists
 */
int checkFileExist(char *checkName)
{
    if (!(file = fopen(checkName, "r")))
    {
        return 0;
    }
    fclose(file);
    return 1;
}

/**
 * Show available files, get file name from user, if file with this name exists,
 * get information to struct array from file and return to menu.
 * If file doesn't exist then return to current user menu.
 */
void chooseFile()
{
    char userFileName[NAME_SIZE];
    int flagChooseFile = 0;
    do
    {
        printf("\nAvailable files:\n");
        if (showAllTXTFiles() == 0)
        {
            printf("There are no available files. Create new file.\n");
            break;
        }
        printf("\nEnter file name.");
        printf("\nInput: ");
        strcpy(userFileName, getString());
        if (checkFileExist(userFileName) == 1)
        {
            strcpy(fileName, userFileName);
            writeFromFileToProductArray(fileName);
            productID = getLastID();
            printf("\nFile %s is selected.\n", fileName);
            flagChooseFile = 1;
            mainMenu(userRole);
        } else
        {
            printf("\nFile %s doesn't exist.\n", userFileName);
            if (fileName[0] == '\0')
            {
                startMenu(userRole);
            } else
            {
                printf("\nCurrent file is %s.\n", fileName);
                mainMenu(userRole);
            }
        }
    } while (flagChooseFile != 1);
}

/**
 * Print all files with '.txt' in current directory
 */
int showAllTXTFiles()
{
    DIR *dir;
    struct dirent *dirn;
    dir = opendir("./");
    int counter = 0;
    if (dir)
    {
        while ((dirn = readdir(dir)))
        {
            int length = strlen(dirn->d_name);
            if (strncmp(dirn->d_name + length - 4, ".txt", 4) == 0)
            {
                if (strcmp(dirn->d_name, "CMakeCache.txt") == 0) continue;
                counter++;
                printf("%s\n", dirn->d_name);
            }
        }
        closedir(dir);
    }
    return counter;
}

/**
 * Check given file name if it contains '.txt'.
 * @param userFileName - name that should be checked
 * @return 1 - if file name contain '.txt'
 */
int checkFileName(char *userFileName)
{
    int strLength = strlen(userFileName);
    if (strncmp(userFileName + strLength - 4, ".txt", 4) == 0)
    {
        return 1;
    }
    return 0;
}

/**
 * Add product to struct array. If number of products in array achieve it's length - return to menu.
 * Add input data to the array current empty position and add this line to the current file.
 */
void addProduct()
{
    int userChoice;
    int productsLength = sizeof(products) / sizeof(*products);
    do
    {
        if (currentPosition >= productsLength)
        {
            printf("\nThe limit of products has been reached.\n"
                   "You can create new file, or open another file, or delete some products.\n");
            mainMenu(userRole);
            return;
        }
        products[currentPosition].id = productID;
        userInputOfProduct(currentPosition);
        currentPosition++;
        productID++;
        addLineToFile(fileName);
        printf("\nProduct is added.\n");
        printf("\nDo you want to add another product?\n");
        printf("1. Yes\n2. No\n");
        printf("\nInput: ");
        userChoice = simpleUserChoice();
    } while (userChoice == 1);
    mainMenu(userRole);
}

/**
 * Editing of the existing product. If there are no products then return to the menu.
 * Get product name from user input.
 */
void editProduct()
{
    char userChoice;
    if (isProductArrayEmpty() == 1)
    {
        printf("\nThere are no products to edit. Add products or choose another file.\n");
        mainMenu(userRole);
        return;
    }
    do
    {
        printf("\nEnter the product name, that you want to edit.");
        printf("\nInput: ");
        editProductByName(getString());
        printf("\nDo you want to edit another product?\n");
        printf("1. Yes\n2. No\n");
        printf("\nInput: ");
        userChoice = simpleUserChoice();
    } while (userChoice == 1);
    mainMenu(userRole);
}

/**
 * Edit product by given name. Count number of products with such name.
 * If there is no products then return to @editProduct().
 * If there is one then edit product and rewrite current file.
 * If there are more then one then redirect to another function.
 * @param productName - name of the product that should be edited
 */
void editProductByName(char *productName)
{
    int counter = 0;
    int idArray[ARRAY_SIZE];
    for (int i = 0; i < currentPosition; i++)
    {
        if (strcmp(products[i].productName, productName) == 0)
        {
            idArray[counter] = products[i].id;
            counter++;
        }
    }
    if (counter == 0)
    {
        printf("\nThere is no product with name %s. Try again.\n", productName);
        editProduct();
        return;
    } else if (counter == 1)
    {
        for (int i = 0; i < currentPosition; i++)
        {
            if (strcmp(products[i].productName, productName) == 0)
            {
                printProduct(i);
                userInputOfProduct(i);
                break;
            }
        }
        writeFromProductArrayToFile(fileName);
        printf("\nProduct is edited.\n");
    } else if (counter > 1)
    {
        editProductById(productName, idArray, counter);
    }
}

/**
 * Print all products with such name. Get ID of product from user,
 * that should be edited. Check if input ID is correct. If correct then edit.
 * @param productName - name of the product that should be edited
 * @param idArray - keeps ID of products with such name
 * @param index - number of elements in array
 */
void editProductById(char *productName, int *idArray, int index)
{
    int userInputId;
    int isIdSuitable = 0;
    printf("\nThere are more than one product with name %s.\n", productName);
    for (int i = 0; i < currentPosition; i++)
    {
        if (strcmp(products[i].productName, productName) == 0)
            printProduct(i);
    }
    do
    {
        printf("\nEnter the ID of the product, that you want to edit.");
        printf("\nInput: ");
        userInputId = getNumber();
        isIdSuitable = checkEnteredId(userInputId, idArray, index);
        if (isIdSuitable == 1)
        {
            for (int i = 0; i < currentPosition; i++)
            {
                if (products[i].id == userInputId)
                {
                    userInputOfProduct(i);
                    break;
                }
            }
            writeFromProductArrayToFile(fileName);
            printf("\nProduct is edited.\n");
        } else
        {
            printf("\nWrong ID. Try again.\n");
        }
    } while (isIdSuitable != 1);
}

/**
 * Deleting of the existing product. If there are no products then return to the menu.
 * Get product name from user input.
 */
void deleteProduct()
{
    char userChoice;
    if (isProductArrayEmpty() == 1)
    {
        printf("\nThere are no products to delete. Add products or choose another file.\n");
        mainMenu(userRole);
        return;
    }
    do
    {
        printf("\nEnter the product name, that you want to delete.");
        printf("\nInput: ");
        deleteProductByName(getString());
        printf("\nDo you want to delete another product?\n");
        printf("1. Yes\n2. No\n");
        printf("\nInput: ");
        userChoice = simpleUserChoice();
    } while (userChoice == 1);
    mainMenu(userRole);
}

/**
 * Delete product by given name. Count number of products with such name.
 * If there is no products then return to @deleteProduct().
 * If there is one then confirm the deletion, delete product and rewrite file.
 * If there are more then one then redirect to another function.
 * @param productName - name of the product that should be deleted
 */
void deleteProductByName(char *productName)
{
    int counter = 0;
    int idArray[ARRAY_SIZE];
    int indexArray[ARRAY_SIZE];
    for (int i = 0; i < currentPosition; i++)
    {
        if (strcmp(products[i].productName, productName) == 0)
        {
            idArray[counter] = products[i].id;
            indexArray[counter] = i;
            counter++;
        }
    }
    if (counter == 0)
    {
        printf("\nThere is no product with name %s. Try again.\n", productName);
        deleteProduct();
        return;
    } else if (counter == 1)
    {
        printf("\nThis product will be deleted:\n");
        printProduct(indexArray[0]);
        printf("\nAre you sure that you want to delete this product?\n");
        printf("1. Yes\n2. No\n");
        printf("\nInput: ");
        if (simpleUserChoice() == 1)
        {
            deleteProductByIndex(indexArray[0]);
            writeFromProductArrayToFile(fileName);
            printf("\nProduct is deleted.\n");
        } else
        {
            mainMenu(userRole);
            return;
        }
    } else if (counter > 1)
        deleteProductById(productName, idArray, indexArray, counter);
}

/**
 * Print all products with such name. Get ID of product from user,
 * that should be deleted. Check if input ID is correct.
 * If correct then confirm the deletion, delete product and rewrite file.
 * @param productName - name of the product that should be deleted
 * @param idArray - keeps ID of products with such name
 * @param indexArray - keeps indexes of products in struct array
 * @param index - number of elements in array
 */
void deleteProductById(char *productName, int *idArray, int *indexArray, int index)
{
    int userInputId;
    int isIdSuitable = 0;
    printf("\nThere are more than one product with name %s.\n", productName);
    for (int i = 0; i < currentPosition; i++)
    {
        if (strcmp(products[i].productName, productName) == 0)
        {
            printProduct(i);
        }
    }
    do
    {
        printf("\nEnter the ID of the product, that you want to delete.");
        printf("\nInput: ");
        userInputId = getNumber();
        isIdSuitable = checkEnteredId(userInputId, idArray, index);
        if (isIdSuitable == 1)
        {
            printf("\nAre you sure that you want to delete this product?\n");
            printf("1. Yes\n2. No\n");
            printf("\nInput: ");
            if (simpleUserChoice() == 1)
            {
                for (int i = 0; i < index; i++)
                {
                    if (userInputId == idArray[i])
                    {
                        deleteProductByIndex(indexArray[i]);
                        writeFromProductArrayToFile(fileName);
                        printf("\nProduct is deleted.\n");
                        break;
                    }
                }
            } else
            {
                mainMenu(userRole);
                return;
            }
        } else
            printf("Wrong ID. Try again.\n");
    } while (isIdSuitable != 1);
}

/**
 * Delete position from struct array by given index.
 * @param index
 */
void deleteProductByIndex(int index)
{
    for (int i = index; i < currentPosition; i++)
    {
        products[i] = products[i + 1];
    }
    currentPosition--;
}

/**
 * Sort menu for user to choose required options.
 * If there are no products then return to the menu.
 */
void sortProductsMenu()
{
    if (isProductArrayEmpty() == 1)
    {
        printf("\nThere are no products to sort. Add products or choose another file.\n");
        mainMenu(userRole);
        return;
    }
    printf("\n---------\nSort menu\n---------\n");
    printf("1. By department\n2. By product name\n3. By product amount\n4. View products\n0. Exit to menu\n");
    printf("\nInput: ");
    switch (getNumber())
    {
        case 1:
            printf("\nIn what order it should be sorted?\n");
            printf("1. In ascending order\n2. In descending order\n");
            printf("\nInput: ");
            sortByDepartment(simpleUserChoice());
            break;
        case 2:
            printf("\nIn what order it should be sorted?\n");
            printf("1. In ascending order\n2. In descending order\n");
            printf("\nInput: ");
            sortByName(simpleUserChoice());
            break;
        case 3:
            printf("\nIn what order it should be sorted?\n");
            printf("1. In ascending order\n2. In descending order\n");
            printf("\nInput: ");
            sortByAmount(simpleUserChoice());
            break;
        case 4:
            printAllProducts(2);
            break;
        case 0:
            mainMenu(userRole);
            break;
        default:
            printf("\nWrong input. Try again.\n");
            sortProductsMenu();
    }
}

/**
 * Sort products in given order by department
 * @param direction - if 1 - sort in ascending order, if 2 - sort in descending order.
 */
void sortByDepartment(int direction)
{
    int isSorted = 0;
    int counter = 0;
    writeFromFileToProductArray(fileName);
    if (direction == 1)
    {
        while (isSorted != 1)
        {
            for (int i = 0; i < currentPosition - 1; i++)
            {
                if (products[i].department > products[i + 1].department)
                {
                    swapElements(i);
                    counter++;
                }
            }
            if (counter == 0)
            {
                isSorted = 1;
            } else
            {
                counter = 0;
            }
        }
    }
    if (direction == 2)
    {
        while (isSorted != 1)
        {
            for (int i = 0; i < currentPosition - 1; i++)
            {
                if (products[i].department < products[i + 1].department)
                {
                    swapElements(i);
                    counter++;
                }
            }
            if (counter == 0)
            {
                isSorted = 1;
            } else
            {
                counter = 0;
            }
        }
    }
    printf("\nProducts are sorted.\n");
    writeFromProductArrayToFile(fileName);
    sortProductsMenu();
}

/**
 * Sort products in given order by product name
 * @param direction - if 1 - sort in ascending order, if 2 - sort in descending order.
 */
void sortByName(int direction)
{
    int isSorted = 0;
    int counter = 0;
    writeFromFileToProductArray(fileName);
    if (direction == 1)
    {
        while (isSorted != 1)
        {
            for (int i = 0; i < currentPosition - 1; i++)
            {
                if (strcmp(products[i].productName, products[i + 1].productName) > 0)
                {
                    swapElements(i);
                    counter++;
                }
            }
            if (counter == 0)
            {
                isSorted = 1;
            } else
            {
                counter = 0;
            }
        }
    }
    if (direction == 2)
    {
        while (isSorted != 1)
        {
            for (int i = 0; i < currentPosition - 1; i++)
            {
                if (strcmp(products[i].productName, products[i + 1].productName) < 0)
                {
                    swapElements(i);
                    counter++;
                }
            }
            if (counter == 0)
            {
                isSorted = 1;
            } else
            {
                counter = 0;
            }
        }
    }
    printf("\nProducts are sorted.\n");
    writeFromProductArrayToFile(fileName);
    sortProductsMenu();
}

/**
 * Sort products in given order by product amount
 * @param direction - if 1 - sort in ascending order, if 2 - sort in descending order.
 */
void sortByAmount(int direction)
{
    int isSorted = 0;
    int counter = 0;
    writeFromFileToProductArray(fileName);
    if (direction == 1)
    {
        while (isSorted != 1)
        {
            for (int i = 0; i < currentPosition - 1; i++)
            {
                if (products[i].productAmount > products[i + 1].productAmount)
                {
                    swapElements(i);
                    counter++;
                }
            }
            if (counter == 0)
            {
                isSorted = 1;
            } else
            {
                counter = 0;
            }
        }
    }
    if (direction == 2)
    {
        while (isSorted != 1)
        {
            for (int i = 0; i < currentPosition - 1; i++)
            {
                if (products[i].productAmount < products[i + 1].productAmount)
                {
                    swapElements(i);
                    counter++;
                }
            }
            if (counter == 0)
            {
                isSorted = 1;
            } else
            {
                counter = 0;
            }
        }
    }
    printf("\nProducts are sorted.\n");
    writeFromProductArrayToFile(fileName);
    sortProductsMenu();
}

/**
 * Swap two positions in product array by given index.
 * @param index
 */
void swapElements(int index)
{
    production tempProduct[1];
    tempProduct[0] = products[index];
    products[index] = products[index + 1];
    products[index + 1] = tempProduct[0];
}

/**
 * Search menu for user to choose required options.
 * If there are no products then return to the menu.
 */
void searchProductsMenu()
{
    if (isProductArrayEmpty() == 1)
    {
        printf("\nThere are no products to sort. Add products or choose another file.\n");
        mainMenu(userRole);
        return;
    }
    printf("\n-----------\nSearch menu\n-----------\n");
    printf("1. By department\n2. By product name\n3. By product amount\n0. Exit to menu\n");
    printf("\nInput: ");
    switch (getNumber())
    {
        case 1:
            printf("\nEnter department.");
            printf("\nInput: ");
            searchByDepartment(getNumber());
            break;
        case 2:
            printf("\nEnter product name.");
            printf("\nInput: ");
            searchByProductName(getString());
            break;
        case 3:
            printf("\nDo you want to enter the exact amount or interval?\n");
            printf("1. Amount\n2. Interval\n");
            printf("\nInput: ");
            if (simpleUserChoice() == 1)
            {
                printf("\nEnter amount.");
                printf("\nInput: ");
                searchByAmount(getNumber());
            } else
            {
                int n1, n2;
                printf("\nEnter interval(two numbers).");
                printf("\nInput: ");
                n1 = getNumber();
                printf("Input: ");
                n2 = getNumber();
                searchByAmount(n1, n2);
            }
            break;
        case 0:
            mainMenu(userRole);
            break;
        default:
            printf("\nWrong input. Try again.\n");
            searchProductsMenu();
    }
}

/**
 * Search products in product array by given department. Print products that match.
 * @param department - search parameter
 */
void searchByDepartment(int department)
{
    int counter = 0;
    writeFromFileToProductArray(fileName);
    for (int i = 0; i < currentPosition; i++)
    {
        if (products[i].department == department)
        {
            printProduct(i);
            counter++;
        }
    }
    if (counter == 0)
    {
        printf("\nNothing found.\n");
    }
    searchProductsMenu();
}

/**
 * Search products in product array by given name. Print products that match.
 * @param productName - search parameter
 */
void searchByProductName(char *productName)
{
    int counter = 0;
    writeFromFileToProductArray(fileName);
    for (int i = 0; i < currentPosition; i++)
    {
        if (strcmp(products[i].productName, productName) == 0)
        {
            printProduct(i);
            counter++;
        }
    }
    if (counter == 0)
    {
        printf("\nNothing found.\n");
    }
    searchProductsMenu();
}

/**
 * Search products in product array by given amount. Print products that match.
 * @param amount - search parameter
 */
void searchByAmount(int amount)
{
    int counter = 0;
    writeFromFileToProductArray(fileName);
    for (int i = 0; i < currentPosition; i++)
    {
        if (products[i].productAmount == amount)
        {
            printProduct(i);
            counter++;
        }
    }
    if (counter == 0)
    {
        printf("\nNothing found.\n");
    }
    searchProductsMenu();
}

/**
 * Search products in product array by given amount interval. Print products that match.
 * @param firstAmount - interval limits
 * @param secondAmount - interval limits
 */
void searchByAmount(int firstAmount, int secondAmount)
{
    int counter = 0;
    int tempChange;
    if (firstAmount > secondAmount)
    {
        tempChange = firstAmount;
        firstAmount = secondAmount;
        secondAmount = tempChange;
    }
    writeFromFileToProductArray(fileName);
    for (int i = 0; i < currentPosition; i++)
    {
        if (products[i].productAmount >= firstAmount && products[i].productAmount <= secondAmount)
        {
            printProduct(i);
            counter++;
        }
    }
    if (counter == 0)
    {
        printf("\nNothing found.\n");
    }
    searchProductsMenu();
}

/**
 * Used to get user answer 1 or 2 (simple choice).
 * @return user choice 1 or 2
 */
int simpleUserChoice()
{
    int userChoice;
    do
    {
        userChoice = getNumber();
        switch (userChoice)
        {
            case 1:
                return 1;
            case 2:
                return 2;
            default:
                printf("\nWrong input.\n");
                printf("\nInput: ");
        }
    } while (userChoice < 1 || userChoice > 2);
    printf("\nUnexpected error.\n");
    application();
    return -1;
}

/**
 * Check if string entered by user is number. Repeat till user enter number.
 * @return number entered by user
 */
int getNumber()
{
    char *checkInput;
    int result;
    int isNumber = 0;
    do
    {
        result = strtol(getString(), &checkInput, 0);
        if (checkInput[0] != '\0')
        {
            printf("\nWrong input. Try again.\n");
            printf("\nInput: ");
        } else
        {
            isNumber = 1;
        }
    } while (isNumber != 1);
    return result;
}

/**
 * Get string entered by user and return it. Check if it is not 'exit symbol'.
 * @return string entered by user
 */
char *getString()
{
    static char userString[NAME_SIZE];
    scanf("%s", userString);
    isExitToMenu(userString);
    return userString;
}

/**
 * Check if given ID is in ID array.
 * @param userInput - ID that should be checked
 * @param idArray - keeps ID
 * @param index - number of elements in array
 * @return 1 if array contain such ID. 0 if doesn't contain
 */
int checkEnteredId(int userInput, int *idArray, int index)
{
    for (int i = 0; i < index; i++)
    {
        if (idArray[i] == userInput) return 1;
    }
    return 0;
}

/**
 * Write data from the product array to the file.
 * @param currentFileName
 */
void writeFromProductArrayToFile(char *currentFileName)
{
    if (!(fopen(currentFileName, "w")))
    {
        printf("\nFile cannot be opened.\n");
        application();
        return;
    }
    for (int i = 0; i < currentPosition; i++)
    {
        fprintf(file, "%d\t", products[i].id);
        fprintf(file, "%d\t", products[i].department);
        fprintf(file, "%s\t", products[i].productName);
        fprintf(file, "%d\n", products[i].productAmount);
    }
    fclose(file);
}

/**
 * Write data from the file to the product array.
 * @param currentFileName
 */
void writeFromFileToProductArray(char *currentFileName)
{
    clearProductArray();
    currentPosition = -1;
    if (!(fopen(currentFileName, "r")))
    {
        printf("\nFile cannot be opened.\n");
        application();
        return;
    }
    while (!feof(file))
    {
        currentPosition++;
        fscanf(file, "%d", &products[currentPosition].id);
        fscanf(file, "%d", &products[currentPosition].department);
        fscanf(file, "%s", products[currentPosition].productName);
        fscanf(file, "%d", &products[currentPosition].productAmount);
    }
    fclose(file);
}

/**
 * Write last position from the product array to the file.
 * @param currentFileName
 */
void addLineToFile(char *currentFileName)
{
    int index = currentPosition - 1;
    if (!(fopen(currentFileName, "a+")))
    {
        printf("\nFile cannot be opened.\n");
        application();
        return;
    }
    fprintf(file, "%d\t", products[index].id);
    fprintf(file, "%d\t", products[index].department);
    fprintf(file, "%s\t", products[index].productName);
    fprintf(file, "%d\n", products[index].productAmount);
    fclose(file);
}

/**
 * Clear product array.
 */
void clearProductArray()
{
    memset(products, '\0', sizeof(products));
    currentPosition = 0;
}

/**
 * Print single product from product array by given index/
 * @param index
 */
void printProduct(int index)
{
    printf("----------------------------------------\n");
    printf("ID: %d  Dept: %d  Name: %s  Amount: %d\n",
           products[index].id, products[index].department,
           products[index].productName, products[index].productAmount);
}

/**
 * Print all products from product array in table view.
 * @param call - means function where it was called
 */
void printAllProducts(int call)
{
    if (isProductArrayEmpty() == 1)
    {
        printf("\nThere are no products to show. Add products or choose another file.\n");
        mainMenu(userRole);
        return;
    }
    printf("\n%-6s%-8s%-12s%-6s\n", "ID", "Dept", "Name", "Amount");
    writeFromFileToProductArray(fileName);
    for (int i = 0; i < currentPosition; i++)
    {
        printf("---------------------------------\n");
        printf("%-6d%-8d%-12s%-6d\n",
               products[i].id, products[i].department,
               products[i].productName, products[i].productAmount);
    }
    if (call == 1)
    {
        mainMenu(userRole);
        return;
    } else if (call == 2)
    {
        sortProductsMenu();
        return;
    }
    mainMenu(userRole);
}

/**
 * Check if product array contains any products.
 * @return 1 - if empty. 0 - if contains products
 */
int isProductArrayEmpty()
{
    if (!strcmp(products[0].productName, ""))
        return 1;
    return 0;
}

/**
 * Get max value of product ID.
 * @return max ID
 */
int getLastID()
{
    int maxId = 0;
    for (int i = 0; i < currentPosition; i++)
    {
        if (products[i].id > maxId)
            maxId = products[i].id;
    }
    return maxId + 1;
}

/**
 * Encrypt given string
 * @param string - string that should be encrypted
 * @return encrypted string
 */
char *encrypt(char *string)
{
    for (int i = 0; string[i] != '\0'; i++)
    {
        string[i] = string[i] + 5;
    }
    return string;
}

/**
 * Decrypt given string
 * @param string - string that should be decrypted
 * @return decrypted string
 */
char *decrypt(char *string)
{
    for (int i = 0; string[i] != '\0'; i++)
    {
        string[i] = string[i] - 5;
    }
    return string;
}

/**
 * Menu with options to manage manager accounts.
 */
void createEditUserMenu()
{
    printf("\n----\nMenu\n----\n");
    printf("1. Create manager\n2. Edit manager\n3. View login list\n0. Exit to menu\n");
    printf("\nInput: ");
    switch (getNumber())
    {
        case 1:
            createUser();
            break;
        case 2:
            editUser();
            break;
        case 3:
            printAllManagerLogin();
            break;
        case 0:
            if (fileName[0] == '\0')
            {
                startMenu(userRole);
            } else
            {
                mainMenu(userRole);
            }
            break;
        default:
            printf("\nWrong input. Try again.\n");
            createEditUserMenu();
    }
}

/**
 * Create new manager account. Check entered login is it unique. If it is then create account.
 * If it is not then return to previous menu.
 */
void createUser()
{
    char login[NAME_SIZE];
    char password[NAME_SIZE];
    FILE *authFile;
    if (!(authFile = fopen(LOGIN_PATH_MANAGER, "a+")))
    {
        printf("\nFile with auth data cannot be opened.\n");
        createEditUserMenu();
        return;
    }
    printf("\nEnter new manager login & password.\n");
    printf("Login: ");
    strcpy(login, getString());
    printf("Password: ");
    strcpy(password, getString());
    if (checkNewUserLogin(authFile, login) == 1)
    {
        fprintf(authFile, "%s\t", encrypt(login));
        fprintf(authFile, "%s\n", encrypt(password));
        printf("\nNew manager account is created.\n");
    } else
    {
        printf("\nUser with such login already exists.\n");
        fclose(authFile);
        createEditUserMenu();
        return;
    }
    fclose(authFile);
    createEditUserMenu();
}

/**
 * Edit existing user by its login. If entered login exists, user enters new login and password.
 * Check is new login unique. If it is then change login and password. Else return to previous menu.
 */
void editUser()
{
    FILE *authFile;
    authCredentials credentials[USER_SIZE];
    int index = 0;
    int counter = 0;
    int isLoginUnique = -1;
    char editLogin[NAME_SIZE];
    char newLogin[NAME_SIZE];
    char newPassword[NAME_SIZE];
    if (!(authFile = fopen(LOGIN_PATH_MANAGER, "r")))
    {
        printf("\nFile with auth data cannot be opened.\n");
        createEditUserMenu();
        return;
    }
    while (!feof(authFile))
    {
        if (index == USER_SIZE) break;
        fscanf(authFile, "%s\t", credentials[index].login);
        fscanf(authFile, "%s\n", credentials[index].password);
        strcpy(credentials[index].login, decrypt(credentials[index].login));
        strcpy(credentials[index].password, decrypt(credentials[index].password));
        index++;
    }
    printf("\nEnter user login, that you want to edit.");
    printf("\nInput: ");
    strcpy(editLogin, getString());
    for (int i = 0; i < index; i++)
    {
        if (strcmp(credentials[i].login, editLogin) == 0)
        {
            printf("\nEnter new login & password for this user.\n");
            printf("Login: ");
            strcpy(newLogin, getString());
            printf("Password: ");
            strcpy(newPassword, getString());
            if (strcmp(editLogin, newLogin) != 0)
            {
                isLoginUnique = checkNewUserLogin(authFile, newLogin);
            }
            if (isLoginUnique == 0)
            {
                printf("\nUser with such login already exists.\n");
                fclose(authFile);
                createEditUserMenu();
                break;
            }
            counter++;
            strcpy(credentials[i].login, newLogin);
            strcpy(credentials[i].password, newPassword);
            printf("\nManger account is edited.\n");
        }
    }
    if (counter == 0)
    {
        printf("\nThere is no user with such login.\n");
        fclose(authFile);
        createEditUserMenu();
        return;
    }
    authFile = fopen(LOGIN_PATH_MANAGER, "w");
    for (int j = 0; j < index; j++)
    {
        fprintf(authFile, "%s\t", encrypt(credentials[j].login));
        fprintf(authFile, "%s\n", encrypt(credentials[j].password));
    }
    fclose(authFile);
    createEditUserMenu();
}

/**
 * Print all manager account login.
 */
void printAllManagerLogin()
{
    FILE *authFile;
    char login[NAME_SIZE];
    char password[NAME_SIZE];
    if (!(authFile = fopen(LOGIN_PATH_MANAGER, "r")))
    {
        printf("\nCannot open file with auth data.\n");
        createEditUserMenu();
        return;
    }
    printf("\nLogin\n");
    while (!feof(authFile))
    {
        fscanf(authFile, "%s\t", login);
        fscanf(authFile, "%s\n", password);
        printf("------\n");
        printf("%s\n", decrypt(login));
    }
    fclose(authFile);
    createEditUserMenu();
}

/**
 * Check is given login unique
 * @param authFile - file with auth data(all login/password)
 * @param userLogin - login that should be checked
 * @return 1 - if login is unique. 0 - if it is not unique
 */
int checkNewUserLogin(FILE *authFile, char *userLogin)
{
    char login[NAME_SIZE];
    char password[NAME_SIZE];
    authFile = fopen(LOGIN_PATH_MANAGER, "r");
    while (!feof(authFile))
    {
        fscanf(authFile, "%s", login);
        fscanf(authFile, "%s", password);
        if (strcmp(decrypt(login), userLogin) == 0)
        {
            fclose(authFile);
            return 0;
        }
    }
    fclose(authFile);
    return 1;
}

/**
 * Verify user by given login and password.
 * @param filePath - path to the file with auth data
 * @param userLogin - login that should be checked
 * @param userPassword - password that should be checked
 * @return 1 - if is verified. 0 - if verification failed
 */
int verifyUser(const char *filePath, char *userLogin, char *userPassword)
{
    char login[NAME_SIZE];
    char password[NAME_SIZE];
    FILE *authFile;
    if (!(authFile = fopen(filePath, "r")))
    {
        printf("\nFile cannot be opened.\n");
    }
    while (!feof(authFile))
    {
        fscanf(authFile, "%s", login);
        fscanf(authFile, "%s", password);
        if (strcmp(decrypt(login), userLogin) == 0 &&
        strcmp(decrypt(password), userPassword) == 0)
        {
            fclose(authFile);
            return 1;
        }
    }
    fclose(authFile);
    return 0;
}

/**
 * Check if string entered by user is sign of the return to menu.
 * @param userInput - string that should be checked
 */
void isExitToMenu(char *userInput)
{
    if (userInput[0] == 'M' && userInput[1] == '\0')
    {
        if (fileName[0] == '\0' && userRole != ROLE_NONE)
        {
            printf("\nReturn to the menu.\n");
            startMenu(userRole);
        } else if (fileName[0] == '\0' && userRole == ROLE_NONE)
        {
            printf("\nReturn to the menu.\n");
            application();
        } else
        {
            printf("\nReturn to the menu.\n");
            mainMenu(userRole);
        }
    }
}

/**
 * Add product to the product array in given position.
 * @param index - position to add product
 */
void userInputOfProduct(int index)
{
    printf("\nEnter the department number.");
    printf("\nInput: ");
    products[index].department = getNumber();
    printf("Enter product name.");
    printf("\nInput: ");
    strcpy(products[index].productName, getString());
    printf("Enter product amount.");
    printf("\nInput: ");
    products[index].productAmount = getNumber();
}

/**
 * When user exit to choose role menu, make userRole NULL, current file name empty,
 * clear product array and go to choose role menu
 */
void exitToMenu()
{
    userRole = ROLE_NONE;
    strcpy(fileName, "");
    clearProductArray();
    application();
}
