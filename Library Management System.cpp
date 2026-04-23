#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <windows.h> 
//Soulut Abbas Mirza
using namespace std;

class Book {
public:
    string title;
    string author;
    string ISBN;
    bool isAvailable;

    Book(string t, string a, string isbn) : title(t), author(a), ISBN(isbn), isAvailable(true) {}

    void display() const {
        cout << "Title: " << title << "\nAuthor: " << author 
             << "\nISBN: " << ISBN << "\nStatus: " 
             << (isAvailable ? "Available" : "Checked Out") << "\n\n";
    }
};

struct ListNode {
    Book* book;
    ListNode* next;

    ListNode(Book* b) : book(b), next(nullptr) {}
};

struct TreeNode {
    Book* book;
    TreeNode* left;
    TreeNode* right;

    TreeNode(Book* b) : book(b), left(nullptr), right(nullptr) {}
};

class LibrarySystem {
private:
    ListNode* head;
    TreeNode* root;
    vector<Book*> booksArray; 
    int capacity;
    int size;

    TreeNode* insertBST(TreeNode* node, Book* book) {
        if (node == nullptr) return new TreeNode(book);

        if (book->ISBN < node->book->ISBN) {
            node->left = insertBST(node->left, book);
        } else if (book->ISBN > node->book->ISBN) {
            node->right = insertBST(node->right, book);
        }

        return node;
    }

    TreeNode* searchBST(TreeNode* node, const string& ISBN) {
        if (node == nullptr || node->book->ISBN == ISBN) {
            return node;
        }

        if (ISBN < node->book->ISBN) {
            return searchBST(node->left, ISBN);
        } else {
            return searchBST(node->right, ISBN);
        }
    }

    void inOrderTraversal(TreeNode* node) {
        if (node != nullptr) {
            inOrderTraversal(node->left);
            node->book->display();
            inOrderTraversal(node->right);
        }
    }

    TreeNode* minValueNode(TreeNode* node) {
        TreeNode* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    TreeNode* deleteBST(TreeNode* root, const string& ISBN) {
        if (root == nullptr) return root;

        if (ISBN < root->book->ISBN) {
            root->left = deleteBST(root->left, ISBN);
        } else if (ISBN > root->book->ISBN) {
            root->right = deleteBST(root->right, ISBN);
        } else {
            if (root->left == nullptr) {
                TreeNode* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                TreeNode* temp = root->left;
                delete root;
                return temp;
            }

            TreeNode* temp = minValueNode(root->right);
            root->book = temp->book;
            root->right = deleteBST(root->right, temp->book->ISBN);
        }
        return root;
    }

    void insertList(Book* book) {
        ListNode* newNode = new ListNode(book);
        if (head == nullptr) {
            head = newNode;
        } else {
            ListNode* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void deleteList(const string& ISBN) {
        ListNode* current = head;
        ListNode* prev = nullptr;

        while (current != nullptr && current->book->ISBN != ISBN) {
            prev = current;
            current = current->next;
        }

        if (current == nullptr) return;

        if (prev == nullptr) {
            head = current->next;
        } else {
            prev->next = current->next;
        }

        delete current;
    }

    void insertArray(Book* book) {
        if (size < capacity) {
            booksArray.push_back(book);
            size++;
        } else {
            cout << "Library capacity reached!\n";
        }
    }

    void deleteArray(const string& ISBN) {
        auto it = find_if(booksArray.begin(), booksArray.end(), 
            [&ISBN](Book* b) { return b->ISBN == ISBN; });

        if (it != booksArray.end()) {
            booksArray.erase(it);
            size--;
        }
    }

public:
    LibrarySystem(int cap = 1000) : head(nullptr), root(nullptr), capacity(cap), size(0) {}

    ~LibrarySystem() {
        ListNode* current = head;
        while (current != nullptr) {
            ListNode* next = current->next;
            delete current->book;
            delete current;
            current = next;
        }
    }

    void addBook(const string& title, const string& author, const string& ISBN) {
        Book* newBook = new Book(title, author, ISBN);
        insertList(newBook);
        root = insertBST(root, newBook);
        insertArray(newBook);
        cout << "Book added successfully!\n";
    }

    void removeBook(const string& ISBN) {
        deleteList(ISBN);
        root = deleteBST(root, ISBN);
        deleteArray(ISBN);
        cout << "Book removed successfully!\n";
    }

    Book* searchBook(const string& ISBN) {
        TreeNode* result = searchBST(root, ISBN);
        if (result != nullptr) {
            return result->book;
        }
        return nullptr;
    }

    void updateBook(const string& ISBN, const string& newTitle = "", 
                   const string& newAuthor = "", bool newStatus = true) {
        Book* book = searchBook(ISBN);
        if (book != nullptr) {
            if (!newTitle.empty()) book->title = newTitle;
            if (!newAuthor.empty()) book->author = newAuthor;
            book->isAvailable = newStatus;
            cout << "Book updated successfully!\n";
        } else {
            cout << "Book not found!\n";
        }
    }

    void displayAllBooks() {
        if (root == nullptr) {
            cout << "No books in the library!\n";
            return;
        }
        cout << "=== Library Catalog ===\n";
        inOrderTraversal(root);
    }

    void checkOutBook(const string& ISBN) {
        Book* book = searchBook(ISBN);
        if (book != nullptr) {
            if (book->isAvailable) {
                book->isAvailable = false;
                cout << "Book checked out successfully!\n";
            } else {
                cout << "Book is already checked out!\n";
            }
        } else {
            cout << "Book not found!\n";
        }
    }

    void returnBook(const string& ISBN) {
        Book* book = searchBook(ISBN);
        if (book != nullptr) {
            if (!book->isAvailable) {
                book->isAvailable = true;
                cout << "Book returned successfully!\n";
            } else {
                cout << "Book was not checked out!\n";
            }
        } else {
            cout << "Book not found!\n";
        }
    }
};

void maximizeConsoleWindow() {
    HWND consoleWindow = GetConsoleWindow();
    ShowWindow(consoleWindow, SW_MAXIMIZE);
}

void displayMenu() {
    cout << "\n=== Library Management System ===\n";
    cout << "1. Add a new book\n";
    cout << "2. Remove a book\n";
    cout << "3. Search for a book\n";
    cout << "4. Update book information\n";
    cout << "5. Display all books\n";
    cout << "6. Check out a book\n";
    cout << "7. Return a book\n";
    cout << "8. Exit\n";
    cout << "Enter your choice: ";
}

void addPredefinedBooks(LibrarySystem& library) {
    library.addBook("To Kill a Mockingbird", "Soulut Abbas ", "1");
    library.addBook("1984", "George Orwell", "2");
    library.addBook("The Great Gatsby", "F. Scott Fitzgerald", "3");
    library.addBook("Pride and Prejudice", "Jane Austen", "4");
    library.addBook("The Catcher in the Rye", "J.D. Salinger", "5");
    library.addBook("The Hobbit", "J.R.R. Tolkien", "6");
    library.addBook("Fahrenheit 451", "Ray Bradbury", "7");
    library.addBook("Moby-Dick", "Herman Melville", "8");
    library.addBook("War and Peace", "Leo Tolstoy", "9");
    library.addBook("The Odyssey", "Homer", "10");
}

int main() {
    maximizeConsoleWindow(); // Maximize console window at start

    LibrarySystem library;
    addPredefinedBooks(library);
    
    int choice;
    string title, author, ISBN;

    do {
        displayMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter book title: ";
                getline(cin, title);
                cout << "Enter author: ";
                getline(cin, author);
                cout << "Enter ISBN: ";
                getline(cin, ISBN);
                library.addBook(title, author, ISBN);
                break;
            case 2:
                cout << "Enter ISBN of book to remove: ";
                getline(cin, ISBN);
                library.removeBook(ISBN);
                break;
            case 3:
                cout << "Enter ISBN to search: ";
                getline(cin, ISBN);
                {
                    Book* book = library.searchBook(ISBN);
                    if (book != nullptr) {
                        cout << "\nBook found:\n";
                        book->display();
                    } else {
                        cout << "Book not found!\n";
                    }
                }
                break;
            case 4:
                cout << "Enter ISBN of book to update: ";
                getline(cin, ISBN);
                {
                    string newTitle, newAuthor;
                    cout << "Enter new title (leave blank to keep current): ";
                    getline(cin, newTitle);
                    cout << "Enter new author (leave blank to keep current): ";
                    getline(cin, newAuthor);
                    library.updateBook(ISBN, newTitle, newAuthor);
                }
                break;
            case 5:
                library.displayAllBooks();
                break;
            case 6:
                cout << "Enter ISBN of book to check out: ";
                getline(cin, ISBN);
                library.checkOutBook(ISBN);
                break;
            case 7:
                cout << "Enter ISBN of book to return: ";
                getline(cin, ISBN);
                library.returnBook(ISBN);
                break;
            case 8:
                cout << "Exiting system...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 8);

    return 0;
}
