//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Destinee Herrera ??
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Academic Advising Program
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> 
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Course {
    string courseNumber; // unique identifier
    string title;

    //  initialize 3 prerequisite array
    string prerequisites[3];
    int numberOfPrerequisites = 0;
    
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold courses
    struct Node {
        Course course;
        unsigned int key;
        Node* next;
        
        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a course
        Node(Course aCourse) : Node() {
            course = aCourse;
        }

        // initialize with a course and a key
        Node(Course aCourse, unsigned int aKey) : Node(aCourse) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(string key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Course course);
    void PrintAll();
    void Remove(string courseNumber);
    Course Search(string courseNumber);
    size_t Size();
};

/**
 * Default constructor
 *///Build "bucket" or "filing cabinet system"
HashTable::HashTable() {

        // Initalize node structure by resizing tableSize
        //*Setting the number of drawers(nodes) to default size
    nodes.resize(tableSize);
}
/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    // invoke local tableSize to size with this->
    // *resize nodes size
    //*updat the tableSize variable and resize the drawers to match
    this->tableSize = size;
    nodes.resize(size);
}


/**
 ** Destructor- when program ends, clean up any chained nodes that were created
 */
HashTable::~HashTable() {
    // Implement logic to free storage when class is destroyed
    // *erase nodes beginning
    for (unsigned int i = 0; i < nodes.size(); i++) {
        Node* current = nodes[i].next;
        while (current != nullptr) {
            Node* dummyNode = current;
            current = current->next;
            delete dummyNode;
        }
    }
}

/**
 * Calculate the hash value of a given key.
 
 */
unsigned int HashTable::hash(string key) {
    
    unsigned int hashVal = 0;

    //turn string into a integer drawer number
    // to avoid collisions: take running total, * prime number + current charater
    for (char c : key) { 
        hashVal = hashVal * 31 + c;
    }
    return hashVal % tableSize;
}

/**
 ** Insert a course "file it away"
 *
 * @param course The course to insert
 */
void HashTable::Insert(Course course) {
    //  Implement logic to insert a course
    // *create the key for the given course
    unsigned int key = hash (course.courseNumber);

    // retrieve node using key "go to specific drawer"
    Node* oldNode = &(nodes.at(key));

    // if no entry found for the key
    if (oldNode->key == UINT_MAX) {

        // *assign this node to the key position
        oldNode->key = key;
        oldNode->course = course;
        oldNode->next = nullptr;

    }
    // *else if node is not used
         // passing old node key to UNIT_MAX, set to key, set old node to course and old node next to null pointer

    else {
        while (oldNode->next != nullptr) {
            oldNode = oldNode->next;
        }
            // *add new newNode to end
        oldNode->next = new Node(course, key);
    }

}
/**
 * Print all courses
 */
void HashTable::PrintAll() {
    
    //  Implement logic to print all course-open drawers and see whats inside.
    //  basic array to hold and sort courses
    Course sortedCourses[500];
    int courseCount = 0;
    // *for node begin to end iterate
    for (unsigned int i = 0; i < nodes.size(); i++) {

        //   *if key not equal to UINT_MAx
        if (nodes[i].key != UINT_MAX) {
            // Put the main drawer in array
            sortedCourses[courseCount] = nodes[i].course;
            courseCount++;

            // *node is equal to next iter
            Node* current = nodes[i].next;

            // *while node not equal to nullptr
            while (current != nullptr) {
                //Put away additional chained files away  
                sortedCourses[courseCount] = current->course;
                courseCount++;

                // node is equal to next node
                current = current->next;
            }
        }

    }
    //Bubble sort: located zybooks additional
        for (int i = 0; i < courseCount - 1; i++) {
            for (int j = 0; j < courseCount - i - 1; j++) {
                if (sortedCourses[j].courseNumber > sortedCourses[j + 1].courseNumber) {
                    Course temp = sortedCourses[j];
                        sortedCourses[j] = sortedCourses[j + 1];
                        sortedCourses[j + 1] = temp;
                }
            }
        }
//Print list
        cout << "Sample Schudule: " << endl;
        for (int i = 0; i < courseCount; i++) {
            cout << sortedCourses[i].courseNumber << ", " << sortedCourses[i].title << endl;
    }
}
/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Course HashTable::Search(string courseNumber) {
    Course course;

    // Implement logic to search for and return a course

    // *create the key for the given course
    unsigned int key = hash(courseNumber);
    Node* node = &(nodes.at(key));

    //* if entry found for the key
         //return node bid
    if (node->key != UINT_MAX && node->course.courseNumber == courseNumber) {
        return node->course;
    }
    //if not in the first file search through the links
    if (node->key != UINT_MAX && node->next != nullptr) {
        Node* current = node->next;

        //*if it matches return
        while (current != nullptr) {
            if (current->course.courseNumber == courseNumber) {
                return current->course;
            }
            //node is equal to the next node
            current = current->next;
        }
    }
    // if no entry found for the key
      // return course 
   
    return course;
}
//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {
    cout << course.courseNumber << ", " << course.title << endl;

    if (course.numberOfPrerequisites == 0) {
        cout << "Prerquisites = none" << endl;
    }
    else {
        cout << "Prerequisites: ";
        for (int i = 0; i < course.numberOfPrerequisites; i++) {
            cout << course.prerequisites[i];

        }
    }
    cout << endl;
    }
/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 */

void loadCourses(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;
    //Personal note: Ifstream- Input File Stream
            //*open the file
    ifstream file(csvPath);
    if (!file.is_open()) {
        cout << "Error!" << csvPath << endl;
        return;
    }
    string line;

    // search file
    while (getline(file, line)) {

        //skip blank lines
        if (line.length() == 0) {
            continue;
        }

        //split line
        stringstream lineStream(line);
        string current;

        Course course;

        //get courseID
        getline(lineStream, course.courseNumber, ',');

        //Get course title
        getline(lineStream, course.title, ',');

        //get prereq's
        while (getline(lineStream, current, ',')) {

            // Then add to array
            if (course.numberOfPrerequisites < 3) {
                course.prerequisites[course.numberOfPrerequisites] = current;
                course.numberOfPrerequisites++;
            }
        }
        hashTable->Insert(course);
    }

    file.close();
}


/**
 * The one and only main() method
 */

int main() {
    //define hash table
    HashTable* courseTable = new HashTable();
    Course course;

    string csvPath;
    string courseKey;
    int choice = 0;

    cout << "Welcome to course planner. ??" << endl;
    // Loop until user chooses 9

    while (choice != 9) {
        cout << "  1. Load course." << endl;
        cout << "  2. Search for a course." << endl;
        cout << "  3. Print all courses sorted" << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do? ";

        string input;
        cin >> input;

        //Try/catch 
        try {
            choice = stoi(input);
        }
        catch (...) {
            choice = 0;
        }
    
   


    switch (choice) {


    case 1:
        
        cout << " Enter name of file to load: ";
        cin.ignore();
        getline(cin, csvPath);

        loadCourses(csvPath, courseTable);
        break;



    case 2:

        courseTable->PrintAll();
        break;

    case 3:

        cout << "What course do you want to know about? ??";
        cin >> courseKey;

        //search the table with user input
        course = courseTable->Search(courseKey);

        if (!course.courseNumber.empty()) {
            displayCourse(course);

        }
        else {
            cout << "Error: Course " << courseKey << "not found ??" << endl;
        
        }
        break;

    case 9:
        cout << "Good bye! ??" << endl;
        break;
     
    default:
        cout << input << "Not valid. Try again! ??" << endl;
        break;
    }
}

//Dont forget to clear
    delete courseTable;
return 0;
}