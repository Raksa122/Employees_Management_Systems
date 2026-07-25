#include <iostream> // For input/output in c++
#include <string>   // For string class
#include <fstream>  // For File I/O
#include <sstream>  // For stringstream operations (parsing CSV)
#include <iomanip>  // For output formatting (setw)

using namespace std; // avoid using std repeat

// Structure for date is used for identify the data type and make it organize
struct date
{
    int day, month, year;
};

// Structure for Employee
struct Employee
{
    string ID;
    string name;
    int age;
    string phoneNumber;
    string email;
    date hireDate;
    string employeeStatus;
    string position;
    string department;
    double salary;
    Employee *next; // record pointer to the next employee
};

// Structure for EmployeeList
struct EmployeeList
{
    int size;
    Employee *head;
    Employee *tail;
};

// Function prototypes
EmployeeList *createEmptyList();
date inputDate(const string &prompt);
void addEmployee(EmployeeList *list, Employee emp);
void displayEmployee(EmployeeList *list);
void searchEmployeeByID(EmployeeList *list, const string &id);
void deleteEmployeeByID(EmployeeList *list, const string &id);
void updateEmployeeByID(EmployeeList *list, const string &id);
int countEmployee(EmployeeList *list);
void addToFile(EmployeeList *list, const string &filename);
string getCSVField(stringstream &ss);
void displayFromFile(EmployeeList *list, const string &filename);
void clearList(EmployeeList *list);
void displayHeader();
void displayOptions();
void sortEmployees(EmployeeList *list, int sortBy);
void EmployeeInfo(Employee &emp);

// create empty list
EmployeeList *createEmptyList()
{
    EmployeeList *list = new EmployeeList; // list is a pointer to the new object that will allocate memory in EmployeeList
    list->head = nullptr;                  // No element in the first
    list->tail = nullptr;                  // No element in the end
    list->size = 0;                        // no element in the list
    return list;
}

// Prompts user to input a date and returns it
date inputDate(const string &prompt) // use prompt as parameter to customize messages Ex: hire date, date of birth
{
    date d; // Use struct data as d
    do
    { // This hold code will loop until user input valid date
        cout << ">>> Enter " << prompt << " (DD MM YYYY) : ";
        if (!(cin >> d.day >> d.month >> d.year)) // IT means that user cannot put letter
        {
            string temp;

            cout << "\n--->>> Invalid input. Please enter a number. <<<---" << endl;
            cin.clear();        // Use to clear the error input
            getline(cin, temp); // This will check the line and ensure that it is not letter
            cout << "\n--->>> Press Enter to input the date again...";
            cin.get();
            continue;
        }
        cin.ignore(); // skip that leftover from getline()

        // Check the data if it valid
        if ((d.year % 4 == 0 && d.year % 100 != 0) || (d.year % 400 == 0))
        { // Check if it leap year
            if (d.day > 0 && d.month > 0 && d.year > 0)
            { // Data must be positive number
                if (d.month == 2 && d.day <= 29)
                { // special month leap year
                    return d;
                }
                else if ((d.month == 1 || d.month == 3 || d.month == 5 || d.month == 7 || d.month == 8 || d.month == 10 || d.month == 12) && d.day <= 31)
                { // month that has 31 days
                    return d;
                }
                else if ((d.month == 4 || d.month == 6 || d.month == 9 || d.month == 11) && d.day <= 30)
                {             // month that has 30 days
                    return d; // print the date
                }
                else
                {
                    cout << "\n--->>> Invalid data. Please Check Again! <<<---\n\n ";
                }
            }
            else
            {
                cout << "\n--->>> Invalid data. Please Check Again! <<<---\n\n ";
            }
        }
        else
        {
            if (d.day > 0 && d.month > 0 && d.year > 0)
            {
                if (d.month == 2 && d.day <= 28)
                { // Not leap year
                    return d;
                }
                else if ((d.month == 1 || d.month == 3 || d.month == 5 || d.month == 7 || d.month == 8 || d.month == 10 || d.month == 12) && d.day <= 31)
                { // month with 31 days
                    return d;
                }
                else if ((d.month == 4 || d.month == 6 || d.month == 9 || d.month == 11) && d.day <= 30)
                { // month with 30
                    return d;
                }
                else
                { // Input is greater than 31
                    cout << "\n--->>> Invalid data. Please Check Again! <<<---\n\n ";
                }
            }
            else
            { // Input is negative
                cout << "\n--->>> Invalid data. Please Check Again! <<<---\n\n ";
            }
        }
    } while (true); // Loop repeats until a valid date is returned
}

// Adds a new employee to the end of the EmployeeList
void addEmployee(EmployeeList *list, Employee emp)
{
    Employee *newEmployee = new Employee(emp); // Employee(emp) = Employee emp means struct Employee as emp
    newEmployee->next = nullptr;               // This new employee is the last person
    if (list->head == nullptr)
    { // This employee is the first
        list->head = newEmployee;
        list->tail = newEmployee;
    }
    else
    {                                   // If it is not the first
        list->tail->next = newEmployee; // link the new employee to the end of the list
        list->tail = newEmployee;       // Update the tail
    }
    list->size++; // update the size
}

// Displays all employees in a formatted table
void displayEmployee(EmployeeList *list)
{
    if (list == nullptr || list->head == nullptr)
    {                                                          // This check if the list is NULL or the list is empty
        cout << "\n>>> No employees in the list! <<<" << endl; // Display the message if the condition is true
        return;                                                // Return the message
    }

    cout << "\n"
         << string(150, '=') << endl; // print "=" = 150
    cout << left                      // align the the text to the left in each column
         << setw(6) << "ID"
         << setw(16) << "Name"
         << setw(8) << "Age"
         << setw(15) << "Phone Number"
         << setw(25) << "Email"
         << setw(20) << "Department"
         << setw(20) << "Position"
         << setw(15) << "Hire Date"
         << setw(10) << "Salary"
         << setw(10) << "Status"
         << endl;
    // setw(n) set n space for each column
    cout << string(150, '-') << endl; // print "-" = 150

    Employee *tmp = list->head; // identify tmp = head of list
    while (tmp != nullptr)
    {                      // Loop tmp until the list is NULL/ loop when tmp = employee is not empty
        ostringstream oss; // use to build a date in format DD/MM/YYYY
        oss << setw(2) << setfill('0') << tmp->hireDate.day << "/"
            << setw(2) << setfill('0') << tmp->hireDate.month << "/"
            << tmp->hireDate.year;
        string formattedDate = oss.str();
        // setfill('0') ensures the day and month have two digits.
        cout << left
             << setw(6) << tmp->ID
             << setw(16) << tmp->name
             << setw(8) << tmp->age
             << setw(15) << tmp->phoneNumber
             << setw(25) << tmp->email
             << setw(20) << tmp->department
             << setw(20) << tmp->position
             << setw(15) << formattedDate
             << setw(10) << fixed << setprecision(2) << tmp->salary
             // fixed << setprecision(2)
             // fixed : use to print the floating point number in fixed point (declear the number after point'.')
             // setprecision(n) = display the number after '.' 2 number Ex: 12.00
             << setw(10) << tmp->employeeStatus
             << endl;
        tmp = tmp->next; // update to the new employee
    }
    cout << string(150, '=') << endl;
}

// Searches for an employee by ID
void searchEmployeeByID(EmployeeList *list, const string &id)
{
    for (Employee *tmp = list->head; tmp != nullptr; tmp = tmp->next)
    { // Loop to check the employee from head to tail
        if (tmp->ID == id)
        {                                               // Check the input ID and the exist ID
            cout << "\n>>> Employee Found <<<" << endl; // If condition is true
            cout << string(150, '=') << endl;
            cout << left
                 << setw(6) << "ID"
                 << setw(16) << "Name"
                 << setw(8) << "Age"
                 << setw(15) << "Phone Number"
                 << setw(25) << "Email"
                 << setw(20) << "Department"
                 << setw(20) << "Position"
                 << setw(15) << "Hire Date"
                 << setw(10) << "Salary"
                 << setw(10) << "Status"
                 << endl;
            cout << string(150, '-') << endl;

            ostringstream oss;
            oss << setw(2) << setfill('0') << tmp->hireDate.day << "/"
                << setw(2) << setfill('0') << tmp->hireDate.month << "/"
                << tmp->hireDate.year;
            string formattedDate = oss.str();

            cout << left
                 << setw(6) << tmp->ID
                 << setw(16) << tmp->name
                 << setw(8) << tmp->age
                 << setw(15) << tmp->phoneNumber
                 << setw(25) << tmp->email
                 << setw(20) << tmp->department
                 << setw(20) << tmp->position
                 << setw(15) << formattedDate
                 << setw(10) << fixed << setprecision(2) << tmp->salary
                 << setw(10) << tmp->employeeStatus
                 << endl;
            cout << string(150, '=') << endl;
            return;
        }
    }
    cout << "\n>>> Employee ID " << id << " not found. <<<" << endl; // If condition is false
}

// Deletes an employee by ID
void deleteEmployeeByID(EmployeeList *list, const string &id)
{
    Employee *prev = nullptr;
    Employee *current = list->head; // current starts from head

    while (current)
    { // This loop until current = nullptr
        if (current->ID == id)
        {
            if (prev)
            { // check if prev not nullptr (Not first element)
                prev->next = current->next; // data before found ID move to found ID after delete
            }
            else
            { // it is the first element
                list->head = current->next; 
            }
            if (current == list->tail)
            {
                list->tail = prev; // If that element at the end of the list
            }
            delete current;
            list->size--;
            cout << "\n>>> Deleted employee with ID " << id << ". <<<" << endl;
            return;
        }
        prev = current;
        current = current->next;
    }
    cout << "\n>>> Employee ID " << id << " not found. <<<" << endl;
}

// Updates employee fields by ID
void updateEmployeeByID(EmployeeList *list, const string &id)
{
    Employee *emp = list->head;
    while (emp != nullptr)
    {
        if (emp->ID == id)
        {
            cout << "\n>>> Updating employee with ID : " << id << " <<<" << endl;
            int choice;
            do
            {
                cout << "\n"
                     << string(40, '=') << endl;
                cout << ">>> Choose field to update: <<<" << endl;
                cout << string(40, '-') << endl;
                cout << "1. Name\n2. Age\n3. Phone Number\n4. Email\n5. Department\n"
                     << "6. Position\n7. Hire Date\n8. Salary\n9. Employee Status\n0. Exit\n";
                cout << string(40, '-') << endl;
                cout << ">>> Enter choice : ";
                cin >> choice; // choose the choice to be update
                cin.ignore();

                switch (choice)
                {
                case 1:
                    cout << ">>> Enter new Name: ";
                    getline(cin, emp->name);
                    break;
                case 2:
                    cout << ">>> Enter new Age: ";
                    cin >> emp->age;
                    cin.ignore();
                    break;
                case 3:
                    cout << ">>> Enter new Phone Number: ";
                    getline(cin, emp->phoneNumber);
                    break;
                case 4:
                    cout << ">>> Enter new Email: ";
                    getline(cin, emp->email);
                    break;
                case 5:
                    cout << ">>> Enter new Department: ";
                    getline(cin, emp->department);
                    break;
                case 6:
                    cout << ">>> Enter new Position: ";
                    getline(cin, emp->position);
                    break;
                case 7:
                    emp->hireDate = inputDate("hire date");
                    break;
                case 8:
                    cout << ">>> Enter new Salary: ";
                    cin >> emp->salary;
                    cin.ignore();
                    break;
                case 9:
                    cout << ">>> Enter new Employee Status: ";
                    getline(cin, emp->employeeStatus);
                    break;
                case 0:
                    cout << "\n>>> Update finished. <<<" << endl; // Exit the loop
                    break;
                default:
                    cout << "\n>>> Invalid choice. <<<" << endl;
                }
            } while (choice != 0);
            return;
        }
        emp = emp->next;
    }
    cout << "\n>>> Employee ID " << id << " not found. <<<" << endl; // If the ID is not match
}

// Returns total number of employees
int countEmployee(EmployeeList *list)
{
    return list->size; // size is number of element in the list so it's the number of employee
}

// Saves employee data from linked list to CSV
void addToFile(EmployeeList *list, const string &filename)
{
    ofstream file(filename); // open a file for writing
    if (!file)
    { // check the file if it open
        cerr << "\n>>> Error opening file for writing. <<<" << endl;
        return;
    }
    file << "ID,Name,Age,PhoneNumber,Email,HireDate,EmployeeStatus,Position,Department,Salary\n";
    Employee *current = list->head;
    while (current != nullptr)
    {
        ostringstream oss;
        oss << setw(2) << setfill('0') << current->hireDate.day << "/"
            << setw(2) << setfill('0') << current->hireDate.month << "/"
            << current->hireDate.year;
        string hireDateFormatted = oss.str();

        file << current->ID << ","
             << '"' << current->name << '"' << ","
             << current->age << ","
             << '"' << current->phoneNumber << '"' << ","
             << '"' << current->email << '"' << ","
             << hireDateFormatted << ","
             << '"' << current->employeeStatus << '"' << ","
             << '"' << current->position << '"' << ","
             << '"' << current->department << '"' << ","
             << fixed << setprecision(2) << current->salary << "\n";
        current = current->next;
    }
    file.close();
    cout << "\n>>> Employee data successfully saved to " << filename << " <<<" << endl;
}

// Extracts a CSV field
string getCSVField(stringstream &ss)
{
    string field;
    if (ss.peek() == '"')
    {
        ss.get();
        getline(ss, field, '"');
        ss.get();
    }
    else
    {
        getline(ss, field, ',');
    }
    return field;
}

// Frees memory used by EmployeeList
void clearList(EmployeeList *list)
{
    Employee *current = list->head;
    while (current != nullptr)  // Traverse the list
    {
        Employee *temp = current;
        current = current->next;
        delete temp;
    }
    list->head = list->tail = nullptr;   // Reset head and tail pointers
    list->size = 0;
}


// Loads employee data from CSV
void displayFromFile(EmployeeList *list, const string &filename)
{
    clearList(list); //  before loading from file to avoid duplicating data.
    ifstream file(filename);
    if (!file)
    {
        cerr << "\n>>> Error opening file for reading. <<<" << endl;
        return;
    }
    string line;
    getline(file, line);    // Skip the header line
    while (getline(file, line))
    {
        stringstream ss(line);  // Create a stringstream for parsing the line
        Employee emp;           // Temporary Employee object
        string hireDateStr;      // Temporary string to hold the hire date
        emp.ID = getCSVField(ss);
        emp.name = getCSVField(ss);
        emp.age = stoi(getCSVField(ss));
        emp.phoneNumber = getCSVField(ss);
        emp.email = getCSVField(ss);
        hireDateStr = getCSVField(ss);
        emp.employeeStatus = getCSVField(ss);
        emp.position = getCSVField(ss);
        emp.department = getCSVField(ss);
        emp.salary = stod(getCSVField(ss));
        stringstream dateStream(hireDateStr);      // Create stream to parse hire date
        char delimiter;
        dateStream >> emp.hireDate.day >> delimiter >> emp.hireDate.month >> delimiter >> emp.hireDate.year;
        addEmployee(list, emp);
    }
    file.close();
    cout << "\n>>> Employee data loaded successfully from " << filename << " <<<" << endl;
    displayEmployee(list);
}

// Sorts employees using optimized bubble sort
void sortEmployees(EmployeeList *list)
{
    int sortBy;
    cout << "\n--->>> Sort by : <<<---\n 1. sort by ID\n 2. sort by Name\n 3. sort by Salary\n ";
    cout << ">>> Enter your choice: ";

    while (!(cin >> sortBy) || sortBy < 1 || sortBy > 3)
    {
        cout << "\n>>> Invalid sort option. Please enter a number between 1 and 3. <<<" << endl;
        cout << "\n>>> Enter the option again : ";
        cin.clear();
        cin.ignore();
    }

    cin.ignore();

    if (list->size <= 1)       // There only 1 or 0 employee
    {
        cout << "\n>>> List is empty or has only one employee, no sorting needed. <<<" << endl;
        return;
    }
    bool swapped;   // has value true or false
    do              // Repeat loop until no more swaps (Bubble Sort logic)
    {
        swapped = false;
        Employee *current = list->head;     // Start from the head of the list

        while (current && current->next)
        {
            bool shouldSwap = false;       // Decide if we should swap current and next

            switch (sortBy)
            {
            case 1:
                shouldSwap = current->ID > current->next->ID;
                break;
            case 2:
                shouldSwap = current->name > current->next->name;
                break;
            case 3:
                shouldSwap = current->salary > current->next->salary;
                break;
            }

            if (shouldSwap)     // If current should be after next, swap them
            {
                swap(current->ID, current->next->ID);
                swap(current->name, current->next->name);
                swap(current->salary, current->next->salary);
                swapped = true;
            }

            current = current->next;  
        }
    } while (swapped);      // Repeat if any swap was made

    cout << "\n>>> Employees sorted successfully! <<<" << endl;
}


// Displays styled header
void displayHeader()
{
    cout << endl
         << "\t" << string(54, '=') << endl;
    cout << "\t|*|            Employee Management System          |*|" << endl;
    cout << "\t" << string(54, '=') << endl;
}

// Displays menu options
void displayOptions()
{
    cout << "\n\t----->>> Select an option in MENU <<<-----" << endl;
    cout << string(60, '-') << endl;
    cout << "1. Add Employee\n"
         << "2. Display Employees\n"
         << "3. Search Employee by ID\n"
         << "4. Update Employee by ID\n"
         << "5. Delete Employee by ID\n"
         << "6. Count Total Employees\n"
         << "7. Save Employees to File\n"
         << "8. Load Employees from File\n"
         << "9. Sort Employees\n"
         << "10. Exit\n";
    cout << string(60, '-') << endl;
}


void EmployeeInfo(Employee &emp)
{
    string temp;

    cout << ">>> Enter Employee ID : ";
    cin >> emp.ID;
    cin.ignore();

    cout << ">>> Enter Name : ";
    getline(cin, emp.name);
    cout << ">>> Enter Age : ";
    // To check if the age greater than 17 and lower than 65 (65 = retire)
    while (!(cin >> emp.age) || emp.age < 18 || emp.age > 64)
    {
        cout << "--->>> Invalid age.\n";
        cout << ">>> Enter the age that is 18-65 : ";
        cin.clear();
        getline(cin, temp);
    }
    cin.ignore();

    cout << ">>> Enter Phone Number : ";
    getline(cin, emp.phoneNumber);

    cout << ">>> Enter Email: ";
    cin >> emp.email;

    emp.hireDate = inputDate("hire date");

    cout << ">>> Enter Employee Status (Active/Inactive) : ";
    cin >> emp.employeeStatus;
    cin.ignore();

    cout << ">>> Enter Position : ";
    getline(cin, emp.position); // To avoid error when position has space Ex: software developer

    cout << ">>> Enter Department : ";
    getline(cin, emp.department); // To avoid error when department has space Ex: Customer Service

    cout << ">>> Enter Salary : ";
    // Check the salary wether it is positive or not
    while (!(cin >> emp.salary) || emp.salary < 0)
    {
        cout << "--->>> Invalid salary.\n";
        cout << ">>> Enter the salary that is greater than 0 : ";
        cin.clear();
        getline(cin, temp);
    }
    cin.ignore();
}
