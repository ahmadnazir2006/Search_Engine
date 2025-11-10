#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cctype>
using namespace std;

// Structure to store URLs in linked list
struct URLNode {
    string url;
    URLNode* next;

    URLNode(string u) : url(u), next(nullptr) {}
};

// BST node structure
struct BSTNode {
    string keyword;
    URLNode* urlHead;
    BSTNode* leftChild;
    BSTNode* rightChild;

    BSTNode(string k) : keyword(k), urlHead(nullptr), leftChild(nullptr), rightChild(nullptr) {}
};

// Function to convert string to lowercase
string toLower(string str) {
    for (int i = 0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

// Function to delete all URLs in linked list
void deleteURLList(URLNode* head) {
    URLNode* current = head;
    while (current != nullptr) {
        URLNode* temp = current;
        current = current->next;
        delete temp;
    }
}

// Function to find minimum node in BST
BSTNode* findMin(BSTNode* node) {
    while (node->leftChild != nullptr) {
        node = node->leftChild;
    }
    return node;
}

// Function to delete keyword from BST
BSTNode* deleteNode(BSTNode* node, string keyword) {
    if (node == nullptr) {
        return nullptr;
    }

    if (keyword < node->keyword) {
        node->leftChild = deleteNode(node->leftChild, keyword);
    }
    else if (keyword > node->keyword) {
        node->rightChild = deleteNode(node->rightChild, keyword);
    }
    else {
        // Node to delete found
        if (node->leftChild == nullptr) {
            BSTNode* temp = node->rightChild;
            deleteURLList(node->urlHead);
            delete node;
            return temp;
        }
        else if (node->rightChild == nullptr) {
            BSTNode* temp = node->leftChild;
            deleteURLList(node->urlHead);
            delete node;
            return temp;
        }

        // Node with two children
        BSTNode* temp = findMin(node->rightChild);
        node->keyword = temp->keyword;
        node->urlHead = temp->urlHead;
        node->rightChild = deleteNode(node->rightChild, temp->keyword);
    }

    return node;
}

// Function to check if URL already exists in linked list
bool urlExists(URLNode* head, string url) {
    URLNode* current = head;
    while (current != nullptr) {
        if (current->url == url) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Function to add URL to linked list
void addURL(URLNode*& head, string url) {
    if (!urlExists(head, url)) {
        URLNode* newNode = new URLNode(url);
        newNode->next = head;
        head = newNode;
    }
}

// Function to print all URLs in linked list
void printURLs(URLNode* head) {
    URLNode* current = head;
    int count = 0;

    // Count total URLs
    URLNode* temp = head;
    while (temp != nullptr) {
        count++;
        temp = temp->next;
    }

    cout << count << " result(s) found" << endl;

    int index = 1;
    while (current != nullptr) {
        cout << index << ". " << current->url << endl;
        current = current->next;
        index++;
    }
}

// Function to search for keyword in BST
BSTNode* search(BSTNode* node, string keyword) {
    if (node == nullptr) {
        return nullptr;
    }

    if (keyword < node->keyword) {
        return search(node->leftChild, keyword);
    }
    else if (keyword > node->keyword) {
        return search(node->rightChild, keyword);
    }
    else {
        return node;
    }
}

// Function to insert keyword with URL into BST
BSTNode* insert(BSTNode* node, string keyword, string url) {
    if (node == nullptr) {
        BSTNode* newNode = new BSTNode(keyword);
        addURL(newNode->urlHead, url);
        return newNode;
    }

    if (keyword < node->keyword) {
        node->leftChild = insert(node->leftChild, keyword, url);
    }
    else if (keyword > node->keyword) {
        node->rightChild = insert(node->rightChild, keyword, url);
    }
    else {
        // Keyword already exists, just add URL
        addURL(node->urlHead, url);
    }

    return node;
}

// Function to read data from file
void readDataFile(BSTNode*& root, string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file '" << filename << "'" << endl;
        cout << "Make sure the file is in the same folder as this program." << endl;
        cout << "File name must be exactly: Assignment_data.txt" << endl;
        return;
    }

    string line1, line2, emptyLine;
    int blankCount = 0;

    while (getline(file, line1)) {
        if (line1.empty()) {
            blankCount++;
            if (blankCount == 2) {
                break;
            }
            continue;
        }

        blankCount = 0;

        // Extract URL from first line (format: id url)
        int spacePos = line1.find(' ');
        string url = "";
        if (spacePos != string::npos) {
            url = line1.substr(spacePos + 1);
        }

        // Read keywords line
        if (!getline(file, line2)) {
            break;
        }

        // Parse keywords from second line
        string keyword = "";
        for (int i = 0; i < line2.length(); i++) {
            if (line2[i] == ' ') {
                if (!keyword.empty()) {
                    keyword = toLower(keyword);
                    root = insert(root, keyword, url);
                    keyword = "";
                }
            }
            else {
                keyword += line2[i];
            }
        }

        // Add last keyword
        if (!keyword.empty()) {
            keyword = toLower(keyword);
            root = insert(root, keyword, url);
        }

        // Read blank line between entries
        getline(file, emptyLine);
    }

    file.close();
}

int main() {
    BSTNode* root = nullptr;
    string filename;

    // Ask for filename
    cout << "Please enter a filename: " << endl;
    cout << "> ";
    getline(cin, filename);

    // Read data file
    readDataFile(root, filename);
    cout << "File loaded successfully." << endl;

    // Search loop - runs continuously
    while (true) {
        cout << "Please enter a word to search: " << endl;
        cout << "> ";

        string keyword;
        getline(cin, keyword);

        // Check for valid input
        if (keyword.empty()) {
            cout << "Error: Please enter a valid keyword." << endl;
            continue;
        }

        // Convert to lowercase
        keyword = toLower(keyword);

        // Search
        BSTNode* result = search(root, keyword);

        if (result != nullptr) {
            printURLs(result->urlHead);
        }
        else {
            cout << "0 result(s) found" << endl;
        }

        cout << endl;
    }

    return 0;
}