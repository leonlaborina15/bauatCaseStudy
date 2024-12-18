        #include <iostream>
        #include <iomanip>
        #include <vector>
        #include <string>
        #include <stack>
        #include <map>
        #include <limits>
        #include <unistd.h>
        using namespace std;

        double totdiscount;
        char ans;
        const string adminUsername = "admin";
        const string adminPassword = "leon15";

        const string COLOR_RESET = "\033[0m";
        const string COLOR_BLUE = "\033[34m";
        const string COLOR_RED = "\033[1;31m";
        const string COLOR_GREEN = "\033[1;32m";
        const string COLOR_YELLOW = "\033[33m";
        const string BOLD = "\033[1m";
        void printDivider(int width)
        {
            int margin = 15;
            cout << string(margin, ' ')
                 << COLOR_GREEN
                 << "*" << COLOR_GREEN << string(width - 2, '=') << COLOR_RESET << COLOR_GREEN << "*" << COLOR_RESET
                 << string(margin, ' ') << endl;
        }

        void printCentered(const string& text, int width)
        {
            int margin = 15;
            int padding = (width - text.length() - 2) / 2;
            cout << string(margin, ' ')
                 << COLOR_GREEN << "|" << COLOR_RESET << string(padding, ' ')
                 << text
                 << string(width - text.length() - padding - 2, ' ')
                 << COLOR_GREEN << "|" << COLOR_RESET
                 << string(margin, ' ') << endl;
        }

        struct User
        {
            string username;
            string password;
        };

        struct Item
        {
            string name;
            int quantity;
            double price;
        };
        vector<Item> cart;
        vector<User> users ;
        stack<Item> actionStack;
        map<string, int> productStock = {{"T-shirt", 100}, {"Long Pants", 200}, {"Shorts", 150}};
        map<string, double> productPrice = {{"T-shirt", 350}, {"Long Pants", 1200}, {"Shorts", 500}};

        int getValidatedIntegerInput(const string& prompt)
        {
            int input;
            while (true)
            {
                cout << "\t\t" << COLOR_YELLOW << prompt << COLOR_RESET;
                cin >> input;

                if (cin.fail() || cin.peek() != '\n')
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\t\t"  << COLOR_RED << "Invalid input. Please enter a valid integer." << COLOR_RESET << endl;
                }
                else
                {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    return input;
                }
            }
        }
        void registerUser()
        {
            User newUser;
            cout << "\t\t" << "Enter a username: ";
            cin >> newUser.username;
            cout << "\t\t" << "Enter a password: ";
            cin >> newUser.password;
            users.push_back(newUser);
            cout << endl;
            cout << "\t\t" << COLOR_GREEN << "User registered successfully!" << COLOR_RESET << endl;
            cout << endl;

            cout << "\t\t" << "Press any key to continue...";
            cin.ignore();
            cin.get();
        }
        bool loginUser()
        {
            string username, password;
            cout << "\t\t" << "Enter username: ";
            cin >> username;
            cout << "\t\t" << "Enter password: ";
            cin >> password;

            bool userFound = false;
            for (const auto& user : users)
            {
                if (user.username == username && user.password == password)
                {
                    cout <<"\t\t"<< COLOR_GREEN << "Login successful! Redirecting to main menu...." << COLOR_RESET << endl;
                    usleep(3000000);
                    system("clear");
                    system("cls");
                    return true;
                }
                else if (user.username == username)
                {
                    userFound = true;
                }
            }

            if (userFound)
            {
                cout<< "\t\t" << COLOR_RED << "Invalid password." << COLOR_RESET << endl;
            }
            else
            {
                cout << "\t\t"<< COLOR_RED << "User not found. Please register first." << COLOR_RESET << endl;
            }
            return false;
        }

        bool adminLogin()
        {
            string username, password;
            cout << "\t\t" << COLOR_YELLOW << "Enter admin username: " << COLOR_RESET;
            cin >> username;
            cout << "\t\t" << COLOR_YELLOW << "Enter admin password: " << COLOR_RESET;
            cin >> password;
            return (username == adminUsername && password == adminPassword);
        }

        void viewUsers()
        {
            cout << "\t\t"<< BOLD << "Registered Users:" << COLOR_RESET << endl;
            for (const auto& user : users)
            {
                cout << user.username << endl;
            }
        }
        void deleteUser()
        {
            string username;
            cout << "\t\t" << "Enter the username of the user to delete: ";
            cin >> username;

            for (auto it = users.begin(); it != users.end(); ++it)
            {
                if (it->username == username)
                {
                    users.erase(it);
                    cout<< "\t\t" << COLOR_GREEN << "User deleted successfully!" << COLOR_RESET << endl;
                    return;
                }
            }
            cout << "\t\t"<< COLOR_RED << "User not found." << COLOR_RESET << endl;
        }

        void addToCart(const Item& item)
        {
            bool itemExists = false;
            for (auto& cartItem : cart)
            {
                if (cartItem.name == item.name)
                {
                    cartItem.quantity += item.quantity;
                    itemExists = true;
                    break;
                }
            }

            if (!itemExists)
            {
                cart.push_back(item);
            }
            actionStack.push(item);
            cout << "\t\t" << COLOR_GREEN << "Item added to cart: " << item.name << COLOR_RESET << endl << endl;
        }

        void checkCart()
        {
            const int consoleWidth = 80;
            const int tableWidth = 60;
            int leftMargin = (consoleWidth - 46) / 2;

            cout << "\t\t" << BOLD << "Items in your cart:" << COLOR_RESET << endl;
            printDivider(consoleWidth);

            cout << setw(leftMargin) << ""
                 << setw(5) << left << "Id."
                 << setw(25) << left << "Item"
                 << setw(15) << left << "Quantity"
                 << setw(15) << left << "Price"
                 << setw(15) << left << "Total" << endl;

            printDivider(consoleWidth);
            for (size_t i = 0; i < cart.size(); ++i)
            {
                const Item& item = cart[i];
                double totalItemCost = item.price * item.quantity;

                cout << setw(leftMargin) << ""
                     << setw(5) << left << (i + 1)
                     << setw(25) << left << item.name
                     << setw(15) << left << item.quantity
                     << setw(15) << left << fixed << setprecision(2) << item.price
                     << setw(15) << left << fixed << setprecision(2) << totalItemCost << endl;
            }

            printDivider(consoleWidth);
        }
        void updateCart(int index, int newQuantity)
        {
            if (index >= 0 && index < cart.size())
            {
                string productName = cart[index].name;

                if (newQuantity > productStock[productName] + cart[index].quantity)
                {
                    cout << "\t\t" << COLOR_RED << "Not enough quantity available in stock." << COLOR_RESET << endl;
                    return;
                }

                productStock[productName] += cart[index].quantity;
                cart[index].quantity = newQuantity;
                productStock[productName] -= newQuantity;

                cout << "\t\t" << COLOR_GREEN << "Item quantity updated: " << cart[index].name << " - Quantity: " << newQuantity << COLOR_RESET << endl;
            }
            else
            {
                cout << "\t\t "<< COLOR_RED << "Invalid index." << COLOR_RESET << endl;
            }
        }
        void deleteFromCart(int index)
        {
            if (index >= 0 && index < cart.size())
            {
                cart.erase(cart.begin() + index);
                cout << "\t\t "<< COLOR_GREEN << "Item removed from cart." << COLOR_RESET << endl;
            }
            else
            {
                cout << "\t\t"<< COLOR_RED << "Invalid index." << COLOR_RESET << endl;
            }
        }

        double calculateTotalCost()
        {
            double totalCost = 0;
            for (const auto& item : cart)
            {
                totalCost += item.price * item.quantity;
            }
            return totalCost;
        }
        double disc()
        {
            double totalDiscount = 0;
            cout << "\t\t" << BOLD << "Applying discounts:" << COLOR_RESET << endl;

            for (const auto& item : cart)
            {
                double discount = 0;
                if (item.quantity > 10)
                {
                    discount = item.price * item.quantity * 0.3;
                }
                else if (item.quantity > 5)
                {
                    discount = item.price * item.quantity * 0.2;
                }
                else
                {
                    discount = item.price * item.quantity * 0.1;
                }
                totalDiscount += discount;
                cout << "\t\t" << item.name << " - Discount: " << discount << "php" << endl;
            }
            return totalDiscount;
        }
        void undoLastAction()
        {
            if (!actionStack.empty())
            {
                Item lastAction = actionStack.top();
                actionStack.pop();

                for (auto it = cart.begin(); it != cart.end(); ++it)
                {
                    if (it->name == lastAction.name && it->quantity == lastAction.quantity)
                    {
                        cart.erase(it);
                        cout << "\t\t" << COLOR_BLUE << "Last action undone: " << lastAction.name << " removed from the cart." << COLOR_RESET << endl;
                        break;
                    }
                }
            }
            else
            {
                cout << "\t\t" << COLOR_RED << "No actions to undo." << COLOR_RESET << endl;
            }
        }

void adminPanel()
{
    int choice;
    do
    {
        printDivider(80);
        printCentered("Admin Panel", 80);
        printDivider(80);

        cout << "\t\t" << BOLD << "Options:" << COLOR_RESET << endl;
        cout << "\t\t" << "1. View Stock" << endl;
        cout << "\t\t" << "2. Update Stock" << endl;
        cout << "\t\t" << "3. Change Prices" << endl;
        cout << "\t\t" << "4. View Users" << endl;
        cout << "\t\t" << "5. Delete User" << endl;
        cout << "\t\t" << "6. Logout" << endl;

        choice = getValidatedIntegerInput("Choose an option: ");

        switch (choice)
        {
        case 1:
            cout << "\t\t" << BOLD << "Current Stock Levels:" << COLOR_RESET << endl;
            for (const auto& product : productStock)
            {
                cout << "\t\t" << product.first << ": " << product.second << " items" << endl;
            }
            cout << "\t\t" << "Press Enter to go back..." << endl;
            cin.ignore();
            cin.get();
            break;

        case 2:
        {
            int productChoice;
            int quantityToAdd;

            cout << "\t\t" << "Select a product to add stock:" << endl;
            cout << "\t\t" << "1. T-shirt" << endl;
            cout << "\t\t" << "2. Long Pants" << endl;
            cout << "\t\t" << "3. Shorts" << endl;
            cout << "\t\t" << "Enter your choice: ";
            cin >> productChoice;

            string productName;

            switch (productChoice)
            {
                case 1:
                    productName = "T-shirt";
                    break;
                case 2:
                    productName = "Long Pants";
                    break;
                case 3:
                    productName = "Shorts";
                    break;
                default:
                    cout << "\t\t" << COLOR_RED << "Invalid choice. Please select between 1-3." << COLOR_RESET << endl;
                    continue;
            }

            quantityToAdd = getValidatedIntegerInput("Enter quantity to add: ");
            productStock[productName] += quantityToAdd;
            cout << "\t\t" << COLOR_GREEN << "Stock updated successfully!" << COLOR_RESET << endl;

            break;
        }

        case 3:
        {
            int productChoice;
            double newPrice;

            cout << "\t\t" << "Select a product to update the price:" << endl;
            cout << "\t\t" << "1. T-shirt" << endl;
            cout << "\t\t" << "2. Long Pants" << endl;
            cout << "\t\t" << "3. Shorts" << endl;
            cout << "\t\t" << "Enter your choice: ";
            cin >> productChoice;

            string productName;

            switch (productChoice)
            {
                case 1:
                    productName = "T-shirt";
                    break;
                case 2:
                    productName = "Long Pants";
                    break;
                case 3:
                    productName = "Shorts";
                    break;
                default:
                    cout << "\t\t" << COLOR_RED << "Invalid choice. Please select between 1-3." << COLOR_RESET << endl;
                    continue;
            }

            newPrice = getValidatedIntegerInput("Enter new price: ");
            productPrice[productName] = newPrice;
            cout << "\t\t" << COLOR_GREEN << "Price updated successfully!" << COLOR_RESET << endl;

            break;
        }

        case 4:
            viewUsers();
            cout << "\t\t" << "Press Enter to go back..." << endl;
            cin.ignore();
            cin.get();
            break;

        case 5:
            deleteUser();
            break;

        case 6:
            cout << "\t\t" << COLOR_BLUE << "Logging out as admin..." << COLOR_RESET << endl;
            break;

        default:
            cout << "\t\t" << COLOR_RED << "Invalid option. Please select between 1-6." << COLOR_RESET << endl;
            break;
        }
    } while (choice != 6);
}
        int main()
        {
            const int consoleWidth = 80;

            while (true)
            {
                int option;
                printDivider(consoleWidth);

                printCentered("Ashton Clothing", consoleWidth);
                printDivider(consoleWidth);

                printCentered("1. Register", consoleWidth);
                printCentered("2. Login", consoleWidth);
                printDivider(consoleWidth);
                option = getValidatedIntegerInput("Choose 1 if you want to Register and 2 if Login: ");

                if (option == 1)
                {
                    printDivider(consoleWidth);
                    printCentered("Register", consoleWidth);
                    printDivider(consoleWidth);
                    registerUser();
                    printDivider(consoleWidth);
                }
                else if (option == 2)
                {
                    printDivider(consoleWidth);
                    printCentered("Login", consoleWidth);
                    printDivider(consoleWidth);
                    cout <<"\t\t" <<"Login as:\n\t\t1. User\n\t\t2. Admin\n";
                    int loginOption = getValidatedIntegerInput("");

                    if (loginOption == 1)
                    {
                        if (loginUser())
                        {
                            bool logout = false;
                            while (!logout)
                            {
                                printDivider(consoleWidth);
                                printCentered("Ashton Clothing", consoleWidth);
                                printDivider(consoleWidth);

                                int leftMargin = (consoleWidth - 46) / 2;
                                cout << setw(leftMargin) << "" << left << setw(30) << "1. T-shirt "
                                     << COLOR_YELLOW << setw(10) << fixed << setprecision(2)
                                     << productPrice["T-shirt"] << COLOR_RESET
                                     << "php (Available: " << productStock["T-shirt"] << ")" << endl;

                                cout << setw(leftMargin) << "" << left << setw(30) << "2. Long Pants "
                                     << COLOR_YELLOW << setw(10) << fixed << setprecision(2)
                                     << productPrice["Long Pants"] << COLOR_RESET
                                     << "php (Available: " << productStock["Long Pants"] << ")" << endl;

                                cout << setw(leftMargin) << "" << left << setw(30) << "3. Shorts "
                                     << COLOR_YELLOW << setw(10) << fixed << setprecision(2)
                                     << productPrice["Shorts"] << COLOR_RESET
                                     << "php (Available: " << productStock["Shorts"] << ")" << endl;
                                printDivider(consoleWidth);
                                ans = 'Y';
                                while (ans == 'Y' || ans == 'y')
                                {
                                    cout << endl;
                                    int product = getValidatedIntegerInput("Enter the product you want to order: ");

                                    Item newItem;
                                    string productName;

                                    switch (product)
                                    {
                                    case 1:
                                        productName = "T-shirt";
                                        break;
                                    case 2:
                                        productName = "Long Pants";
                                        break;
                                    case 3:
                                        productName = "Shorts";
                                        break;
                                    default:
                                        cout << "\t\t" << COLOR_RED << "Invalid product choice." << COLOR_RESET << endl;
                                        continue;
                                    }

                                    int quantity = getValidatedIntegerInput("Enter the quantity of the product you want to order: ");

                                    if (quantity > productStock[productName])
                                    {
                                        cout << "\t\t" << COLOR_RED << "Not enough quantity available." << COLOR_RESET << endl;
                                        continue;
                                    }

                                    newItem = {productName, quantity, productPrice[productName]};
                                    productStock[productName] -= quantity;
                                    addToCart(newItem);

                                    cout << endl;
                                    cout << "\t\t" << COLOR_YELLOW << "Do you want to order another product? (Y/N): " << COLOR_RESET;
                                    cin >> ans;
                                }

                                char cartOrProceed;
                                cout << endl;
                                cout << "\t\tWould you like to check your cart or proceed to checkout? (C for Cart, P for Proceed): ";
                                cin >> cartOrProceed;

                                if (cartOrProceed == 'C' || cartOrProceed == 'c')
                                {
                                    char updateOrDelete;
                                    do
                                    {
                                        checkCart();

                                        cout << endl;
                                        cout << "\t\t" << "Do you want to update or delete an item from the cart? (U for Update, D for Delete, P to Proceed to checkout): ";
                                        cin >> updateOrDelete;

                                        if (updateOrDelete == 'U' || updateOrDelete == 'u')
                                        {
                                            int index = getValidatedIntegerInput("Enter the number of the item to update: ") - 1;
                                            int newQuantity = getValidatedIntegerInput("Enter the new quantity: ");
                                            updateCart(index, newQuantity);
                                        }
                                        else if (updateOrDelete == 'D' || updateOrDelete == 'd')
                                        {
                                            int index = getValidatedIntegerInput("Enter the number of the item to delete: ") - 1;
                                            deleteFromCart(index);

                                            cout << "\t\t"<< COLOR_GREEN << "Item deleted successfully!" << COLOR_RESET << endl;
                                            checkCart();
                                        }
                                        else if (updateOrDelete == 'P' || updateOrDelete == 'p')
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            cout << "\t\t"<< COLOR_RED << "Invalid choice. Please enter U, D, or P." << COLOR_RESET << endl;
                                        }

                                    }
                                    while (updateOrDelete != 'P' && updateOrDelete != 'p');

                                    printDivider(consoleWidth);
                                    printCentered("Checkout Information", consoleWidth);
                                    printDivider(consoleWidth);

                                    //Calculate total cost and apply discount
                                    double totalCost = calculateTotalCost();
                                    double totdiscount = disc();

                                    cout << setw(leftMargin) << "" << left << setw(30) << "Your total cost is:" << fixed << setprecision(2) << totalCost << " php" << endl;
                                    cout << setw(leftMargin) << "" << left << setw(30) << "Total discount:" << fixed << setprecision(2) << totdiscount << " php" << endl;
                                    cout << setw(leftMargin) << "" << left << setw(30) << "Total cost after discount:" << fixed << setprecision(2) << totalCost - totdiscount << " php" << endl;

                                    printDivider(consoleWidth);
                                    cout<< "\t\t"  << COLOR_GREEN << BOLD << "Proceeding to checkout..." << COLOR_RESET << endl;
                                    printDivider(consoleWidth);

                                    // Undo last action
                                    char undoAction;
                                    cout << "\t\t" << "Undo last action? (Y/N): ";
                                    cin >> undoAction;
                                    if (undoAction == 'Y' || undoAction == 'y')
                                    {
                                        undoLastAction();

                                        char orderAgain;
                                        cout << "\t\t " << "Do you want to order another product? (Y/N): ";
                                        cin >> orderAgain;
                                        if (orderAgain == 'Y' || orderAgain == 'y')
                                        {
                                            cart.clear();
                                            actionStack = stack<Item>();
                                            totdiscount = 0;

                                            continue;
                                        }
                                        else
                                        {
                                            break;
                                        }
                                    }
                                    else
                                    {

                                        char orderAgain;
                                        cout << "\t\t" << "Do you want to order another product? (Y/N): ";
                                        cin >> orderAgain;
                                        if (orderAgain == 'Y' || orderAgain == 'y')
                                        {

                                            cart.clear();
                                            actionStack = stack<Item>();
                                            totdiscount = 0;


                                            continue;
                                        }
                                        else
                                        {

                                            break;
                                        }
                                    }
                                }
                                else if (cartOrProceed == 'P' || cartOrProceed == 'p')
                                {
                                    printDivider(consoleWidth);
                                    printCentered("Checkout Information", consoleWidth);
                                    printDivider(consoleWidth);

                                    double totalCost = calculateTotalCost();
                                    double totdiscount = disc();

                                    cout << setw(leftMargin) << "" << left << setw(30) << "Your total cost is:" << fixed << setprecision(2) << totalCost << " php" << endl;
                                    cout << setw(leftMargin) << "" << left << setw(30) << "Total discount:" << fixed << setprecision(2) << totdiscount << " php" << endl;
                                    cout << setw(leftMargin) << "" << left << setw(30) << "Total cost after discount:" << fixed << setprecision(2) << totalCost - totdiscount << " php" << endl;

                                    printDivider(consoleWidth);
                                    cout << "\t\t" << COLOR_GREEN << BOLD << "Proceeding to checkout..." << COLOR_RESET << endl;
                                    printDivider(consoleWidth);


                                    char undoAction;
                                    cout  << "\t\t" << "Undo last action? (Y/N): ";
                                    cin >> undoAction;
                                    if (undoAction == 'Y' || undoAction == 'y')
                                    {
                                        undoLastAction();


                                        char orderAgain;
                                        cout << "\t\t" << "Do you want to order another product? (Y/N): ";
                                        cin >> orderAgain;
                                        if (orderAgain == 'Y' || orderAgain == 'y')
                                        {

                                            cart.clear();
                                            actionStack = stack<Item>();
                                            totdiscount = 0;


                                            continue;
                                        }
                                        else
                                        {

                                            break;
                                        }
                                    }
                                    else
                                    {
                                        char orderAgain;
                                        cout << "\t\t" << "Do you want to order another product? (Y/N): ";
                                        cin >> orderAgain;
                                        if (orderAgain == 'Y' || orderAgain == 'y')
                                        {

                                            cart.clear();
                                            actionStack = stack<Item>();
                                            totdiscount = 0;


                                            continue;
                                        }
                                        else
                                        {

                                            break;
                                        }
                                    }
                                }
                            }
                        }

                    }
                    else if (loginOption == 2 && adminLogin())
                    {
                        adminPanel();
                    }
                    else
                    {
                        cout << "\t\t"<< COLOR_RED << "Invalid login attempt." << COLOR_RESET << endl;
                    }
                }
                else
                {
                    cout << "\t\t"<< COLOR_RED << "Invalid choice." << COLOR_RESET << endl;
                }

                cout << endl;

                usleep(2000000);
                system("clear");
                system("cls");
            }
            return 0;
        }


























