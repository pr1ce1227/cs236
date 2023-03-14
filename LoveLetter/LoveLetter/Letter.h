#pragma once
#include <iostream>
#include <string>
using namespace std; 

class Letter 
{
protected:
	string title; 
	string text; 
	int length = 0;

public:
	Letter(){}
	~Letter() {}
	Letter(string letter, string text, int length) : title(title), text(text), length(length) {}

	string getTitle(); 

	string getText(); 

	int getLength(); 
};

