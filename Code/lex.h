#pragma once
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
using namespace std;
void split(string line, int lineCount);
extern string finaloutput;

class Token
{
public:
    string tokenName;
    string tokenWord;
	int tokenLineNumber;
	Token(string tName, string tWord, int tLineNo){
		this->tokenName = tName;
		this->tokenWord = tWord;
		this->tokenLineNumber = tLineNo; 
	}

	void display();
	void displayCurrent();
	bool program();
	bool dvar_global();
	bool dvar_local();
	bool dmethod();
	bool parameter();
	bool functionLine();
	bool functionIf();
	bool functionWhile();
	bool functionAssign();
	bool functionReturn();
	bool call_function();
	bool exprlog();
	bool opand();
	bool opno();
	bool expr();
	bool exprel();
	bool product();
	bool functionSign();
	bool functionTerm();
};

extern vector<Token> tokenVector;