#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

// Forward declarations for class dependencies
class Book;
class Member;

// Base class 
class Person
{
public:
    string name;
    string address;
    string email;
};

// Derived class 
class Member : public Person
{
private:
    int memberID;
    vector<Book *> booksLoaned; // Collection of books borrowed by the member

public:
    // Constructor 
    Member(int memberID, string name, string address, string email) : memberID(memberID)
    {
        this->memberID = memberID;
        this->name = name;
        this->address = address;
        this->email = email;
    }

    // Getter for member ID
    int getMemberID()
    {
        return memberID;
    }

    // Getter for the list of borrowed books
    vector<Book *> getBooksBorrowed()
    {
        return booksLoaned;
    }

    // Method to add a book to the borrowed books
    void setBooksBorrowed(Book *book);

    // Method to return a book based on its ID
    void returnBook(int bookID);
};

// Class representing a book in the library
class Book
{
private:
    int bookID;
    string bookName;
    string genre;
    time_t dueDate;
    Member *borrower; // Pointer to the member who borrowed the book

public:
    // Constructor 
    Book(int bookID, string bookName, string genre) : bookID(bookID), bookName(bookName), genre(genre), dueDate(0), borrower(nullptr) {}

    // Getter for book ID
    int getBookID()
    {
        return bookID;
    }

    // Getter for book name
    string getBookName()
    {
        return bookName;
    }

    // Getter for book genre
    string getGenre()
    {
        return genre;
    }

    // Getter for the due date of the book
    time_t getDueDate()
    {
        return dueDate;
    }

    // Setter for the due date of the book
    void setDueDate(time_t dueDate)
    {
        this->dueDate = dueDate;
    }

    // Method to mark the book as returned
    void returnBook();

    // Method to handle the borrowing of the book by a member
    void borrowBook(Member *borrower, time_t dueDate);
};

// Inherit from the baswe class
class Librarian : public Person
{
private:
    int staffID;
    int salary;
    vector<Book *> booksInventory;  
    vector<Book *> booksBorrowed;  
public:
    // Constructor
    Librarian(int staffID, string name, string address, string email, int salary) : staffID(staffID)
    {
        this->name = name;
        this->address = address;
        this->email = email;
        this->salary = salary;
        initializeBooksInventory(); // Initialize the library inventory
    }

    // Getter for staff ID
    int getStaffID()
    {
        return staffID;
    }

    // Getter for salary
    int getSalary()
    {
        return salary;
    }

    // Setter for salary
    void setSalary(int salary)
    {
        this->salary = salary;
    }

    // Setter for staff ID
    void setStaffID(int staffID)
    {
        this->staffID = staffID;
    }

    // Method to calculate fines for overdue books of a member
    void calcFine(Member *member);

    // Method to add a new member to the library system
    void addMember(Member *member);

    // Method to handle the issuance of a book to a member
    void issueBook(Member *member);

    // Method to handle the return of a book by a member
    void returnBook(Member *member);

    // Method to display the list of borrowed books by a member
    void displayBorrowedBooks(Member *member);

private:
    // Method to initialize the library inventory with sample books
    void initializeBooksInventory();
};

// Method to add a book to the list of borrowed books by a member
void Member::setBooksBorrowed(Book *book)
{
    booksLoaned.push_back(book);
}

// Method to return a book by a member based on its ID
void Member::returnBook(int bookID)
{
    for (auto it = booksLoaned.begin(); it != booksLoaned.end(); ++it)
    {
        if ((*it)->getBookID() == bookID)
        {
            (*it)->returnBook(); 
            booksLoaned.erase(it); // Remove the book from the list of borrowed books
            break;
        }
    }
}

// Method to mark a book as returned
void Book::returnBook()
{
    borrower = nullptr;
    dueDate = 0;
}

// Method to handle the borrowing of the book by a member
void Book::borrowBook(Member *borrower, time_t dueDate)
{
    this->borrower = borrower;
    this->dueDate = dueDate;
    borrower->setBooksBorrowed(this); // Add the book to the list of borrowed books by the member
}

// Method to calculate fines for overdue books of a member
void Librarian::calcFine(Member *member)
{
    vector<Book *> borrowedBooks = member->getBooksBorrowed();

    for (const auto &book : borrowedBooks)
    {
        time_t currentDate = time(0);
        if (currentDate > book->getDueDate())
        {
            int overdueDays = difftime(currentDate, book->getDueDate()) / (60 * 60 * 24);
            int fineAmount = overdueDays * 1.0; // Fine calculation (for illustration purposes)
            cout << "Fine for Book ID " << book->getBookID() << ": $" << fineAmount << endl;
        }
    }
}

// Method to add a new member to the library system
void Librarian::addMember(Member *member)
{
    // Input for member details is commented out for simplicity

    cout << "Member " << member->getMemberID() << " added to the library system." << endl;
    // issueBook(member);
}

// Method to handle the issuance of a book to a member
void Librarian::issueBook(Member *member)
{
    cout << "Available Genres:" << endl;
    for (const auto &book : booksInventory)
    {
        cout << "Genre: " << book->getGenre() << endl;
    }

    string selectedGenre;
    cout << "Enter the Genre you want to borrow: ";
    cin >> selectedGenre;

    vector<Book *> availableBooks;
    for (const auto &book : booksInventory)
    {
        if (book->getGenre() == selectedGenre)
        {
            availableBooks.push_back(book);
        }
    }

    if (!availableBooks.empty())
    {
        cout << "Books available in the selected genre:" << endl;
        for (const auto &book : availableBooks)
        {
            cout << "Book ID: " << book->getBookID() << ", Name: " << book->getBookName() << endl;
        }

        int bookID;
        cout << "Enter the Book ID you want to borrow: ";
        cin >> bookID;

        Book *selectedBook = nullptr;
        for (const auto &book : availableBooks)
        {
            if (book->getBookID() == bookID)
            {
                selectedBook = book;
                break;
            }
        }

        if (selectedBook)
        {
            selectedBook->borrowBook(member, time(0)); // Use the current time as the due date
            booksBorrowed.push_back(selectedBook);
            cout << "Book borrowed successfully!" << endl;
        }
        else
        {
            cout << "Invalid Book ID. Book not found." << endl;
        }
    }
    else
    {
        cout << "No books available in the selected genre." << endl;
    }
}

// Method to handle the return of a book by a member
void Librarian::returnBook(Member *member)
{
    cout << "Books borrowed by Member " << member->getMemberID() << ":" << endl;
    vector<Book *> borrowedBooks = member->getBooksBorrowed();
    for (const auto &book : borrowedBooks)
    {
        cout << "Book ID: " << book->getBookID() << ", Name: " << book->getBookName() << endl;
    }

    if (!borrowedBooks.empty())
    {
        int bookID;
        cout << "Enter the Book ID you want to return: ";
        cin >> bookID;

        Book *selectedBook = nullptr;
        for (const auto &book : borrowedBooks)
        {
            if (book->getBookID() == bookID)
            {
                selectedBook = book;
                break;
            }
        }

        if (selectedBook)
        {
            selectedBook->returnBook(); 
            member->returnBook(bookID); // Remove the book from the list of borrowed books by the member

            for (auto it = booksBorrowed.begin(); it != booksBorrowed.end(); ++it)
            {
                if ((*it)->getBookID() == bookID)
                {
                    booksBorrowed.erase(it); // Remove the book from the list of borrowed books by the librarian
                    break;
                }
            }

            cout << "Book returned successfully!" << endl;

            cout << "Remaining books to return:" << endl;
            for (const auto &book : borrowedBooks)
            {
                cout << "Book ID: " << book->getBookID() << ", Name: " << book->getBookName() << endl;
            }
        }
        else
        {
            cout << "Invalid Book ID. Book not borrowed by the member." << endl;
        }
    }
    else
    {
        cout << "No books borrowed by the member." << endl;
    }
}

// Method to display the list of borrowed books by a member
void Librarian::displayBorrowedBooks(Member *member)
{
    vector<Book *> borrowedBooks = member->getBooksBorrowed();
    if (!borrowedBooks.empty())
    {
        cout << "Books borrowed by Member " << member->getMemberID() << ":" << endl;
        for (const auto &book : borrowedBooks)
        {
            cout << "Book ID: " << book->getBookID() << ", Name: " << book->getBookName() << endl;
            time_t dueDate = book->getDueDate();
            cout << "Due Date: " << ctime(&dueDate);
        }
    }
    else
    {
        cout << "No books borrowed by the member." << endl;
    }
}

// Method to initialize the library inventory with sample books
void Librarian::initializeBooksInventory()
{
    booksInventory.push_back(new Book(1, "Science Fiction", "Science Fiction"));
    booksInventory.push_back(new Book(2, "Satire", "Satire"));
    booksInventory.push_back(new Book(3, "Drama", "Drama"));
    booksInventory.push_back(new Book(4, "Action and Adventure", "Action and Adventure"));
    booksInventory.push_back(new Book(5, "Romance", "Romance"));
    booksInventory.push_back(new Book(6, "Mystery", "Mystery"));
    booksInventory.push_back(new Book(7, "Horror", "Horror"));
    booksInventory.push_back(new Book(8, "Health", "Health"));
    booksInventory.push_back(new Book(9, "Guide", "Guide"));
    booksInventory.push_back(new Book(10, "Diaries", "Diaries"));
    booksInventory.push_back(new Book(11, "Comics", "Comics"));
    booksInventory.push_back(new Book(12, "Journals", "Journals"));
    booksInventory.push_back(new Book(13, "Biographies", "Biographies"));
    booksInventory.push_back(new Book(14, "Fantasy", "Fantasy"));
    booksInventory.push_back(new Book(15, "History", "History"));
    booksInventory.push_back(new Book(16, "Science", "Science"));
    booksInventory.push_back(new Book(17, "Art", "Art"));
}

// Method to display the menu options
void displayMenu()
{
    cout << " Library Management System " << endl;
    cout << "1. Add Member" << endl;
    cout << "2. Issue Book" << endl;
    cout << "3. Return Book" << endl;
    cout << "4. Display Borrowed Books" << endl;
    cout << "5. Calculate Fine" << endl;
    cout << "6. Exit" << endl;
    cout << "Enter your choice: ";
}

// Main function
int main()
{
    Librarian librarian(1, "John Thomas", "13 West St", "johnthomash12@gmail.com", 50000);

    int memberID;
    string name, address, email;
    Member *member = new Member(0, "", "", ""); 

    while (true)
    {
        displayMenu();

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            cout << "Enter Member ID: ";
            cin >> memberID;
            cout << "Enter Name: ";
            cin.ignore(); 
            getline(cin, name);
            cout << "Enter Address: ";
            getline(cin, address);
            cout << "Enter Email: ";
            cin >> email;

            // Create a new Member 
            member = new Member(memberID, name, address, email);

            librarian.addMember(member);
            break;
        }

        case 2:
        {
            librarian.issueBook(member);
            break;
        }
        case 3:
        {
            librarian.returnBook(member);
            break;
        }
        case 4:
        {
            librarian.displayBorrowedBooks(member);
            break;
        }
        case 5:
        {
            librarian.calcFine(member);
            break;
        }
       case 6:
            cout << "Exiting Library Management System. Goodbye!" << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
