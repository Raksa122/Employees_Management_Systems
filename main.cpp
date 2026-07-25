#include "EmployeeManagement.h"

int main()
{
    EmployeeList *myList = createEmptyList(); // call fuction to create the list
    int choice;

    do
    {
        displayHeader();
        displayOptions();
        cout << ">>> Enter choice : ";
        if (!(cin >> choice))
        {
            string temp;
            cout << "\n--->>> Invalid input. Please enter a number. <<<---" << endl;
            cin.clear();
            getline(cin, temp);
            cout << "\n--->>> Press Enter to return to the menu...";
            cin.get();
            continue;
        }
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            Employee emp;

            EmployeeInfo(emp);
            addEmployee(myList, emp);
            cout << "\n--->>> Employee added successfully! <<<---" << endl;
            break;
        }
        case 2:
            displayEmployee(myList);
            break;
        case 3:
        {
            string id;
            cout << ">>> Enter Employee ID to search : ";
            cin >> id; // This cannot put in function
            cin.ignore();
            searchEmployeeByID(myList, id);
            break;
        }
        case 4:
        {
            string id;
            cout << ">>> Enter Employee ID to update : ";
            cin >> id;
            cin.ignore();
            updateEmployeeByID(myList, id);
            break;
        }
        case 5:
        {
            string id;
            cout << ">>> Enter Employee ID to delete : ";
            cin >> id;
            cin.ignore();
            deleteEmployeeByID(myList, id);
            break;
        }
        case 6:
            cout << "\n>>> Total Employees : " << countEmployee(myList) << " <<<" << endl;
            break;
        case 7:
            addToFile(myList, "employees.csv");
            break;
        case 8:
            displayFromFile(myList, "employees.csv");
            break;
        case 9:
        {
            int sortBy;

            sortEmployees(myList);
            displayEmployee(myList); // Display sorted list
            break;
        }
        case 10:
            cout << "\n--->>> Exiting program... Thank you! <<<---" << endl
                 << endl;
            break;
        default:
            cout << "\n--->>> Invalid choice, please try again. <<<---" << endl;
        }
        if (choice != 10)
        {
            cout << "\n>>> Press Enter to return to the menu...";
            cin.get();
        }
    } while (choice != 10);
    return 0;
}
