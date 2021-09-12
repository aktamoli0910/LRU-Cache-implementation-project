#include<bits/stdc++.h>
using namespace std;

// Load the data from the file in Global Variable named 'data'
map <string, string> data;

void initialiseData() {
    ifstream fin("IP Addresses.txt");
    string URL, IP;
    while (fin >> URL >> IP) {
        data[URL] = IP;
    }
    return;
}

// Class node for storing the pages in the cache
class Node {
public:
    string key;
    string value;
    Node *prev, *next;

    Node(string key, string value): key(key), value(value), prev(NULL), next(NULL) {
    }
};

// Class to create an instance of LRU Cache
class LRUCache {
public:
    Node *front, *rear;
    int cacheSize, currentNodes;
    map<string, Node *> hashMap;

    // LRUCache constructor
    LRUCache(int size): front(NULL), rear(NULL), cacheSize(size), currentNodes(0) {
        hashMap.clear();
    }

    // Delete a page from the cache
    void deleteLRUPage() {
        currentNodes--;
        cout << "\nDelete " << rear -> key << " from hashMap\n";
        hashMap[rear -> key] = NULL;
        rear = rear -> prev;
        rear -> next = NULL;
        return;
    }

    // Insert a page into the cache
    Node* insertPage(string key, string value) {

        // If size of cache is full then delete LRU page
        if(currentNodes == cacheSize) {
            deleteLRUPage();
        }

        Node *newNode = new Node(key, value);

        // If cache is empty insert the first page
        if(front == NULL) {
            front = newNode;
            rear = front;
            currentNodes++;
            hashMap[key] = newNode;
            return newNode;
        }

        // If the cache is not empty
        Node *prevFront = front;
        front = newNode;
        prevFront -> prev = front;
        front -> next = prevFront;

        // Add the key in the cache to the hashmap
        currentNodes++;
        hashMap[key] = newNode;

        // Return the newly created node
        return newNode;
    }

    // Move the Most Recently Used page to the front of the cache
    void moveToFront(Node *node) {
        string key = node -> key;
        string value = node -> value;
        if(node == front) {
            return;
        }
        if(node == rear) {
            deleteLRUPage();
            insertPage(key, value);
            return;
        }

        node -> prev -> next = node -> next;
        node -> next -> prev = node -> prev;
        currentNodes--;

        insertPage(key, value);
    }

    // Search a page in cache
    Node *searchPage(string key) {

        map<string, Node *>::iterator it = hashMap.find(key);

        // If page found return the address of the page
        if (it != hashMap.end()) {
            if(it -> second == NULL) {
            	cout << "Finding in directory...\n";
                map<string, string>::iterator datait = data.find(key);
                string value;
                value = datait -> second;
                return insertPage(key, value);
            }
          	cout << "\nFound " << key << " in cache with value: " << it -> second -> value << "\n";
            moveToFront(it -> second);
            return it -> second;
        }

        // If page not found then get the page from data and load
        map<string, string>::iterator datait = data.find(key);

        // If data not found in directory
        if(datait == data.end()) {
        	cout << "\nCould not resolve the URL to an IP Address\n";
        	return NULL;
		}

        string value;
        value = datait -> second;
        return insertPage(key, value);
    }

    // Print the current state of the cache
    void currentState() {
    	cout << "\nCACHE : ";
        if(front == NULL) {
            cout << "EMPTY\n";
            return;
        }
        Node *temp = front;
        while(temp -> next != NULL) {
            cout << "| " << temp -> key << ", " << temp -> value << " |  => ";
            temp = temp -> next;
        }
        cout << "| " << temp -> key << ", " << temp -> value << " |\n";
        cout << "\n";
    }

    // Flush the cache
    void flushCache() {
    	cout << "\nFLUSHING\n";
    	hashMap.clear();
        front = NULL;
        rear = NULL;
        currentNodes = 0;
    }

    // Print hashMap
    void printHashMap() {
        cout << "PRINT HASHMAP\nSIZE: " << hashMap.size() << "\n";
        map<string, Node *>::iterator it = hashMap.begin();
        while (it != hashMap.end()) {
          	cout << it -> first << "\n";
          	it++;
        }
        return;
    }
};

int main() {
    cout << "DNS Caching\n";

    // Initialise a cache of size 3
    LRUCache cache(3);

    // Initialise the global data variable with the data in the directory
    initialiseData();

    // Menu
    int ch = INT_MAX;

    while(ch != 4) {

        cout << "--------------------------------------------------------------------------------------\n";
        cout << "1) Fetch URL\n";
        cout << "2) Get Cache Current State\n";
        cout << "3) Flush DNS Cache\n";
        cout << "4) Exit\n";

        cout << "\nEnter your choice: ";
        cin >> ch;

        if(ch == 1) {
            string URL;
            cout << "Enter URL: ";
            cin >> URL;
            Node *page = cache.searchPage(URL);
            cout << "\nIP Address: ";
            if(page != NULL) {
                cout << page -> value << "\n";
            }
            else {
                cout << "NULL\n";
            }
            cache.currentState();
        }

        else if(ch == 2) {
            cache.currentState();
        }

        else if(ch == 3) {
            cache.flushCache();
        }

        else if(ch == 4) {
            exit(1);
        }
    }
}
