
#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// Course information model
struct CourseInfo {
	string id;
	string name;
	vector<string> prereqs;
};

// Load courses into the map
void ImportCourses(const string& filePath, map<string, CourseInfo>& courseCatalog) {
	ifstream input(filePath);
	if (!input) {
		cout << "[Error] Unable to open file: " << filePath << endl;
		return;
	}

	string record;
	while (getline(input, record)) {
		stringstream lineStream(record);
		string id, title, prereqItem;
		getline(lineStream, id, ',');
		getline(lineStream, title, ',');

		CourseInfo newCourse;
		newCourse.id = id;
		newCourse.name = title;

		while (getline(lineStream, prereqItem, ',')) {
			if (!prereqItem.empty() && find(newCourse.prereqs.begin(), newCourse.prereqs.end(), prereqItem) == newCourse.prereqs.end()) {
				newCourse.prereqs.push_back(prereqItem);
			}
		}

		// Insert course
		courseCatalog[id] = newCourse;
	}

	cout << "[Success] Courses imported successfully.\n";
}

// Display all courses sorted by course ID
void ShowAllCourses(const map<string, CourseInfo>& courseCatalog) {
	cout << "\n--- Complete Course List ---\n";
	for (const auto& entry : courseCatalog) {
		cout << entry.second.id << " - " << entry.second.name << endl;
	}
}

// Show details of a specific course
void ShowCourseDetails(const map<string, CourseInfo>& courseCatalog, const string& courseID) {
	auto course = courseCatalog.find(courseID);
	if (course == courseCatalog.end()) {
		cout << "[Notice] Course " << courseID << " not found.\n";
		return;
	}

	cout << "\nCourse: " << course->second.id << " - " << course->second.name << endl;
	cout << "Prerequisites:\n";
	if (course->second.prereqs.empty()) {
		cout << "  None\n";
	}
	else {
		for (const auto& prereqID : course->second.prereqs) {
			auto prereqCourse = courseCatalog.find(prereqID);
			if (prereqCourse != courseCatalog.end()) {
				cout << "  " << prereqCourse->second.id << " - " << prereqCourse->second.name << endl;
			}
			else {
				cout << "  " << prereqID << " (details not available)\n";
			}
		}
	}
}

// Main interaction menu
void AdvisingSystem() {
	map<string, CourseInfo> courseCatalog;
	string userInput;
	int option = 0;

	do {
		cout << "\n==== ABCU Course Advising Menu ====\n";
		cout << " 1. Load Course Data\n";
		cout << " 2. Display All Courses\n";
		cout << " 3. View a Specific Course\n";
		cout << " 9. Quit\n";
		cout << "Choose an option: ";
		cin >> option;
		cin.ignore(); // Clear leftover newline

		switch (option) {
		case 1:
			cout << "Enter course data file name: ";
			getline(cin, userInput);
			ImportCourses(userInput, courseCatalog);
			break;
		case 2:
			if (courseCatalog.empty()) {
				cout << "[Warning] Load data first.\n";
			}
			else {
				ShowAllCourses(courseCatalog);
			}
			break;
		case 3:
			if (courseCatalog.empty()) {
				cout << "[Warning] Load data first.\n";
			}
			else {
				cout << "Enter course ID (e.g., CSCI101): ";
				getline(cin, userInput);
				ShowCourseDetails(courseCatalog, userInput);
			}
			break;
		case 9:
			cout << "Exiting system. Have a great day!\n";
			break;
		default:
			cout << "[Error] Invalid option. Try again.\n";
			break;
		}
	} while (option != 9);
}

// Entry point
int main() {
	AdvisingSystem();
	return 0;
}
