#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

/*
 * Course structure
 */
struct Course {
    string courseId;
    string title;
    vector<string> prerequisites;
};

/*
 * node structure for binary search tree
 */
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

/*
 * Binary search tree class
 */
class BinarySearchTree {

private:
    Node* root;

    // recursive insert helper
    Node* insert(Node* node, Course course) {
        if (node == nullptr) {
            return new Node(course);
        }

        if (course.courseId < node->course.courseId) {
            node->left = insert(node->left, course);
        }
        else {
            node->right = insert(node->right, course);
        }

        return node;
    }

    // in-order traversal (sorted output)
    void inOrder(Node* node) {
        if (node == nullptr) {
            return;
        }

        inOrder(node->left);
        cout << node->course.courseId << ", "
             << node->course.title << endl;
        inOrder(node->right);
    }

    // recursive search
    Course* search(Node* node, string courseId) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->course.courseId == courseId) {
            return &node->course;
        }

        if (courseId < node->course.courseId) {
            return search(node->left, courseId);
        }

        return search(node->right, courseId);
    }

public:
    BinarySearchTree() {
        root = nullptr;
    }

    void Insert(Course course) {
        root = insert(root, course);
    }

    void PrintAll() {
        inOrder(root);
    }

    Course* Search(string courseId) {
        return search(root, courseId);
    }
};

string toUpperCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

/*
 * load courses from csv file
 */
void loadData(string fileName, BinarySearchTree& bst) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }

    string line;

    while (getline(file, line)) {

        stringstream ss(line);
        string token;

        Course course;

        // course ID
        getline(ss, token, ',');
        course.courseId = toUpperCase(token);

        // course title
        getline(ss, token, ',');
        course.title = token;

        // prerequisites
        while (getline(ss, token, ',')) {
            // ignore blank prerequisites! so it doesnt say "Prerequisites: , "
            if (!token.empty()) {
                course.prerequisites.push_back(toUpperCase(token));
            }
        }

        bst.Insert(course);
    }

    file.close();
    cout << "Data loaded successfully." << endl;
}

/*
 * print single course information
 */
void printCourse(BinarySearchTree& bst) {
    string courseId;
    cout << "What course do you want to know about? ";
    cin >> courseId;

    courseId = toUpperCase(courseId);

    Course* course = bst.Search(courseId);

    if (course == nullptr) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course->courseId << ", "
         << course->title << endl;

    if (course->prerequisites.size() > 0) {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course->prerequisites.size(); ++i) {
            cout << course->prerequisites[i];
            if (i < course->prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
    else {
        cout << "Prerequisites: None" << endl;
    }
}

/*
 * Display Menu
 */
void displayMenu() {
    cout << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << "What would you like to do? ";
}

int main() {

    BinarySearchTree bst;
    int choice = 0;
    string fileName;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {

        displayMenu();
        cin >> choice;

        switch (choice) {

        case 1:
            cout << "Enter file name: ";
            cin >> fileName;
            loadData(fileName, bst);
            break;

        case 2:
            cout << "Here is a sample schedule:" << endl;
            bst.PrintAll();
            break;

        case 3:
            printCourse(bst);
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}
