//============================================================================
// Name        : AdvisingAssistant.cpp
// Author      : Phillip Cowan
// Version     : 1.0
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <thread>

using namespace std;

//default time for sleep
const int GLOBAL_SLEEP_TIME = 5000;

/*
Course/Class Stucture- object to hold course information
*/
struct Course {

    string courseId;
    string courseName;
    vector<string> prereqList;
};

class BinarySearchTree {

private:
    // Define structures to hold courses
    struct Node {
        Course course;
        Node* right;
        Node* left;

        // default constructor
        Node() {
            left = nullptr;
            right = nullptr;
        }

        // initialize with a course
        Node(Course aCourse) {
            course = aCourse;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;
    void inOrder(Node* node);
    int size = 0;

public:
    BinarySearchTree();
    void InOrder();
    void Insert(Course aCourse);
    void Remove(string courseId);
    Course Search(string courseId);
    int Size();
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    this->root = nullptr;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    inOrder(root);
}

  /**
   * Insert a course
   */
void BinarySearchTree::Insert(Course aCourse) {

    Node* currentNode = root;

    if (root == NULL) {

        root = new Node(aCourse);

    }
    else {

        while (currentNode != NULL) {

            if (aCourse.courseId < currentNode->course.courseId) {

                if (currentNode->left == nullptr) {

                    currentNode->left = new Node(aCourse);
                    currentNode = NULL;
                }
                else {

                    currentNode = currentNode->left;
                }
            }
            else {

                if (currentNode->right == nullptr) {

                    currentNode->right = new Node(aCourse);
                    currentNode = NULL;
                }
                else {

                    currentNode = currentNode->right;
                }
            }
        }
    }
    size++;
}


// Search for a course by courseId
Course BinarySearchTree::Search(string courseIdFind) {

    Course aCourse;

    Node* currentNode = root;

    while (currentNode != NULL) {

        if (currentNode->course.courseId == courseIdFind) {

            return currentNode->course;
        }
        else if (courseIdFind < currentNode->course.courseId) {

            currentNode = currentNode->left;
        }
        else {

            currentNode = currentNode->right;
        }
    }

    return aCourse;
}

// sort the tree in order and print the courseId and courseName
void BinarySearchTree::inOrder(Node* node) {

    if (node == NULL) {

        return;
    }
    inOrder(node->left);

    //print the node
    cout << node->course.courseId << ", " << node->course.courseName << endl;

    inOrder(node->right);
}

int BinarySearchTree::Size() {

    return size;
}

// Trim whitespace from both ends of a string
static inline string trim(string s) {
	auto is_space = [](unsigned char c) { return std::isspace(c); };
	while (!s.empty() && is_space(s.front())) s.erase(s.begin());
	while (!s.empty() && is_space(s.back())) s.pop_back();
	return s;
}

// Split a string into tokens using a delimiter (e.g., comma)
static vector<string> split(const string& line, char delimiter) {
	vector<string> tokens;
	string current;
	stringstream ss(line);
	while (getline(ss, current, delimiter)) {
		current = trim(current);
		if (!current.empty()) tokens.push_back(current);
	}
	return tokens;
}


/*
function to load courses
Making the assumption that the CourseID will always be in uppercase 
and the user will always not always enter the CourseID in uppercase.*/

void loadCourses(string csvPath, BinarySearchTree* courseList) {
    // Create a data structure and add to the collection of courses 

    ifstream inFS; //insteam to read file
    string line; //line feed 
    vector<string> stringTokens; 

    inFS.open(csvPath); //open the read file

    if (!inFS.is_open()) {//small error handler
        cout << "Could not open file. Please check inputs. " << endl;
        return;
    }

    // read the file line by line and store the data in a struct
    while (!inFS.eof()) {

        Course aCourse;//create a new struct for each "line"

        getline(inFS, line);
        stringTokens = split(line, ','); //split the line into tokens via the delimiter

        if (stringTokens.size() < 2) {
            // invalid line, not enough tokens to form a course end user doesn't need to see this error message, just skip the line
            continue; // skip invalid lines
        }

        else {
            aCourse.courseId = stringTokens.at(0);
            aCourse.courseName = stringTokens.at(1);
        }
       
        

        for (unsigned int i = 2; i < stringTokens.size(); i++) {

            aCourse.prereqList.push_back(stringTokens.at(i));
        }

        // push this course to the end
        courseList->Insert(aCourse);
    }

    inFS.close(); //close the file
}

//Display a course and its prerequisites
void displayCourse(Course aCourse) {

    cout << aCourse.courseId << ", " << aCourse.courseName << endl;
    cout << "Prerequisites: ";

    // if prerequisites length is less than 1 then there are no prerequisites,
    // otherwise print the list of prerequisites
    if (aCourse.prereqList.size() < 1) {

        cout << "None";
    }
    else {

        for (unsigned int i = 0; i < aCourse.prereqList.size(); i++) {
            
            cout << aCourse.prereqList.at(i);
            
            if (aCourse.prereqList.size() > 1 && i < aCourse.prereqList.size() - 1) {//put a comma for any elements greater than 1

                cout << ", ";
            }            
        }
    }

    cout << endl;
}
// Sanitize user input by converting to uppercase
string convertCase(string& aCourseKey) {
    for (unsigned int i = 0; i < aCourseKey.length(); i++) {
        aCourseKey[i] = toupper(aCourseKey[i]);
    }
    return aCourseKey;
}



// Clear the console screen on Windows and POSIX systems
void clearScreen() {
    #ifdef _WIN32
        // Command for Windows
        system("cls");
    #else
        // Assume POSIX (Mac, Linux, etc.)
        system("clear");
    #endif
}

int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, aCourseKey;

    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        aCourseKey = argv[2];
        break;
    default:
        csvPath = "finalProject300.csv";
    }

    // Define a table to hold all the courses
    BinarySearchTree* courseList = new BinarySearchTree();

    Course course;  
    bool validInput = false;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course Details" << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do? ";

        aCourseKey = ""; //clear the string        
        string anyKey = " "; //clear the string
        choice = 0; //clear the choice

        try {
            cin >> choice;

            if ((choice > 0 && choice < 5) || (choice == 9)) {// limit the user menu inputs to good values
                validInput = true;
            }
            else {//throw error for catch
                validInput = false;
                throw 1;
            }

            switch (choice) {
            case 1:
                
                // Complete the method call to load the courses
                loadCourses(csvPath, courseList);
                cout << courseList->Size() << " courses read" << endl;
                
                this_thread::sleep_for(chrono::milliseconds(500));

                break;

            case 2:
                cout << "\nHere is a sample schedule:" << endl;
                courseList->InOrder();

                cout << "\n Press 'y' and Enter to continue..." << endl;

                cin >> anyKey;

                break;

            case 3:

                cout << "\nWhat course do you want to know about? (e.g., CSCI101): ";
                cin >> aCourseKey;

                course = courseList->Search(convertCase(aCourseKey));

                if (!course.courseId.empty()) {
                    displayCourse(course);
                }
                else {
                    cout << "\nCourse ID " << aCourseKey << " not found." << endl;
                }

                this_thread::sleep_for(chrono::milliseconds(500));

                cout << "\nPress 'y' and Enter to continue..." << endl;
                cin >> anyKey;

                break;

            case 9:
                exit(0);
                break;

            default:

                throw 2;
            }
        }

        catch (int err) {
            cout << choice << " is not a valid choice. Please try again." << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
        }

        //need to clear the cin operator of extra input, e.g., 9 9, or any errors generated by bad input, e.g., 'a'
        cin.clear();
        cin.ignore();        

        //clear terminal screen for next menu display
        clearScreen();
    }

    cout << "Thank you for using the course planner!" << endl;

    this_thread::sleep_for(chrono::milliseconds(500));

    return 0;
}
