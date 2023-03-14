#pragma once
#include "Letter.h"
#include <iostream>
using namespace std; 

class CodedLetter : public Letter
{
private: 
	int cypher = 00000; 
	string text; 
	string reminder; 

public: 
	CodedLetter(string title, string text, int length, int cypher){
		this->title = title; 
		this->text = text; 
		this->length = length; 
		this->cypher = cypher; 
	}
	~CodedLetter(){}
	
	void CodeMessage(); 


	void DecodeMessage(); 

	void getCodedLetter();
	
};

