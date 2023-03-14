#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 
#include <stack>
#include <vector>
#include "Letter.h"
#include "CodedLetter.h"
using namespace std; 

#define SIZE 20

void importLetter(Letter*); 

void printMenu(); 

int main(int argc, char* argv[]) {
	Letter userLetter; 
	string menuSelect = "";
	vector<Letter> letters; 

	while (menuSelect  != "quit") {
		printMenu(); 
		cout << "Select an option: " << endl;  
		getline(cin,menuSelect); 

		if (menuSelect == "Add Letter") {
			importLetter(&userLetter); 
		}
		else if (menuSelect == "Decode Letter") {
			cout << "What is the title of the letter you want to decode? " << endl; 
			string title; 
			getline(cin, title); 
		}
		else if (menuSelect == "quit"){}
		else {
			cout << "Sorry that was not an option" << endl << endl; 
		}

	}

	



	

	//stack<char> letters; 
	//for (int i = 0; i < strLetter.size(); ++i) {
	//	letters.push(strLetter.at(i)); 
	//}

	//for (int i = 0; i < strLetter.size(); ++i) {
	//	strLetter.at(i) = letters.top(); 
	//	letters.pop(); 
	//}


	//fstream out("Send.txt"); 
	//if (out.fail())
	//	cout << "Failed to open";
	//else
	//	cout << "Writing to output file..." << endl << endl; 
	//out << strLetter; 

	//cout << "Coded message... " << endl << endl; 
	//cout << strLetter << endl << endl; 
	
	
	
}

void importLetter(Letter* userLetter) {
	bool success = false;
	cout << "Program running..." << endl << endl;

	// Input the original letter through command argument 
	fstream input;
	string fileLetter;
	 
	while (!success) {

		// create letter stream 
		cout << "What is the title of your letter?" << endl;

		getline(cin,fileLetter);
		fileLetter.append(".txt");
		cout << endl;
		input.open(fileLetter);

		// Verify stream was successful 
		if (input.fail()) {
			cout << "That file doesnt exist:" << endl << "- Verify the title" << endl
				<< "- Make sure letter is in directory" << endl << endl;
		}
		else {
			cout << "Importing file" << endl;
			success = true;
		}

	}

	// Import the file 
	stringstream ss;
	ss << input.rdbuf();
	string strLetter;
	strLetter = ss.str();
	input.close();
	cout << endl << strLetter << endl << endl;

	Letter temp(fileLetter, strLetter, strLetter.length()); 
	*userLetter = temp; 
	string buff;  
}

void printMenu() {
	cout << endl << "Menu: " << endl; 
	cout << "Add Letter" << endl; 
	cout << "Decode Letter" << endl; 
	cout << "quit" << endl << endl; 
}