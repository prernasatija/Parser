#pragma once
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include "lex.h"
#include <map>

extern int Error = 0;
extern int OPERATOR = 1;
extern int DELIMITER = 2;
extern int INTEGER = 3;
extern int FLOAT = 4;
extern int HEXADECIMAL = 5;
extern int OCTAL = 6;
extern int STRING = 7;
extern int CHAR = 8;
extern int IDENTIFIER = 9;
extern int KEYWORD = 10;
extern int char1 = 12;
extern int char2 = 13;
extern int char3 = 14;
extern int char4 = 15;
extern int space = 16;
extern int char5 = 17;
extern int int1 = 18;
extern int int2 = 19;
extern int float1 = 20;
extern int float2 = 21;
extern int str1 = 22;
extern int str2 = 23;
extern int str3 = 24;
extern int oct1 = 25;
extern int hex1 = 26;
extern int hex2 = 27;
extern int str4 = 28;
extern int int3 = 29;

string keywords[13]  = {"read", "print", "if", "else", "while", "void", "int", "char", "string", "float", "boolean", "true", "false"};
string finaloutput = "";
struct Key
{
	int state; 
	int newchar; 
	Key(const int A,const int B) : 
		state(A),newchar(B) {}
	bool operator<(const Key& A) const 
	{	
		if (this->state < A.state)
		{ 
			return true;
		}else 
			if (this->state == A.state)
			{
				if (this->newchar < A.newchar){
					return true;
				} 
			}
		return false;
	}
};

typedef std::map<Key,int> Transitions;
Transitions transitions;
bool isDelimiter(char c);
int nextStateFinder(Key key);
int calculate_next_state(int current_state,char letter);
string lexer(string word);
void writeList(string output, string word, int lineCount);
vector<Token> tokenVector;

bool isDelimiter(char c){
	if(c==';'||c=='{'||c=='('||c=='['||c==':'||c=='}'||c==')'||c==']'||c==',')
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool isOperator(char c) {
	if(c=='+'||c=='-'||c=='='||c=='*'||c=='!'||c=='%'||c=='&'||c=='|'||c=='/'||c=='<'||c=='>')
	{
		return true;
	}
	else
	{
		return false;
	}
}
int nextCharClass(unsigned char c) {
	if(c=='0') {
		return int1;
	}
	if(c >= 49 && c <= 55){
		return oct1;
	}
	if(c>=49&&c<=57){
		return int2;
	}
	if(c == '"'){
		return str1;
	}
	if(c == '.'){
		return float1;
	}
	if(c == 'x'){
		return hex1;
	}
	if((c>=65&&c<=70)||(c>=97&&c<=102)){
		return hex2;
	}
	if(isalpha(c)||c=='$'||c=='_'){
		return IDENTIFIER;
	}
	if(c == 39){
		return char1;
	}
	if(c=='\\'){
		return char2;
	}
	if(c==32){
		return space;
	}
	if((c>=32 && c<=127)&&(c!=39)){  
		return str2;
	}

	return Error; 
}

void split(string line, int lineCount){
	string output="";
	string word ="";
	int len = (int)line.length();
	string strword="";

	for(int i=0;i<=len-1;i++){
	//changes for string*****************************************
	  	if(line[i]=='"'){
			if(word != "")
			{
				 i--;
				 goto LEXER;
			}
		   int j= 0;
			for(j=i+1;j<len;j++)
			{
				if(line[j]=='"' || j==len-1)
				{
						word=word+line.substr(i,j-i+1);
						i=j;
						string tmp = "\"\\\"";
						if(word == tmp && line[j+1] == '"')
						{
							word = word + "\"";
							i++;
						}
						i++;
						goto LEXER;	
			    }
		   }
		   strword = "";
	   }

//********** for ' ' char and ''' *******************
	   if(line[i]=='\''){
		   if(word != "")
			   {
				   i--;
				   goto LEXER;
			   }
		   int j= 0;
			for(j=i+1;j<len;j++){
				if(line[j]=='\''){
					word=word+line.substr(i,j-i+1);
					i=j;
					string tmp = "\'\\\'";
					if(word == tmp && line[j+1] == '\'')
					{
						word = word + "\'";
						i++;
					}
					goto LEXER;
				}
			}
	   }

	//***************************************************

	   if(line[i]==' '||isDelimiter(line[i])||isOperator(line[i])){
LEXER:		if(word!=""){
			output = lexer(word);
			cout<<"******  The OUTPUT IS : "<<output<<"\n\n\n";
			finaloutput = finaloutput + output + "\t" + word + "\n";
			writeList(output, word, lineCount);
			} 
			if(isDelimiter(line[i])){
				output = "DELIMITER";
//				cout<<"DELIMITER"<<"\t"<<line[i]<<"\n";
				finaloutput = finaloutput + output + "\t" + line[i] + "\n";
				string opword = string(1,line[i]);
				writeList(output, opword, lineCount);
			}
			else if(isOperator(line[i])){
				output = "OPERATOR";
//				cout<<"OPERATOR"<<"\t"<<line[i]<<"\n";
				finaloutput = finaloutput + output + "\t" + line[i] + "\n";
				string opword = string(1,line[i]);
				writeList(output, opword, lineCount);
			}
			if(line[i]==' ')
			{
//				cout<<"space"<<line[i];
			}
			word="";
		}
		else
		{
			word = word + line[i];
			if(i==line.length()-1){
				output = lexer(word);
				finaloutput = finaloutput + output + "\t" + word + "\n";
				writeList(output, word, lineCount);
			}
		}
	}
}

void writeList(string output, string word, int lineCount){
	cout<<"output is : "<<output<<" word is : "<<word<<" line no is : "<<lineCount<<"\n";
	tokenVector.push_back(Token(output, word, lineCount));
}


string lexer(string word){
	int state= Error;
	int wlen = (int)word.length()-1;
	for(int i=0;i<=wlen;i++)
	{
		state = calculate_next_state(state,word[i]);
	}
	// State transition for KEYWORD token
	if(state == IDENTIFIER){
		for(int i=0;i<=9;i++){
			if(word==keywords[i]){
				state = 10;
			}
		}
	}
			 if(state==3 || state == 29) return "INTEGER";
		else if(state==4) return "FLOAT";
		else if(state==5) return "HEXADECIMAL";
		else if(state==6) return "OCTAL";
		else if(state==7) return "STRING";
		else if(state==8) return "CHAR";
		else if(state==9) return "IDENTIFIER";
		else if(state==10) return "KEYWORD";
		else return "Error";
}
int calculate_next_state(int current_state,char letter)
{
	int new_state=0;
	string str="";
	str= str + letter;
	int c = nextCharClass(letter);
	Key key(current_state,c);
	new_state = nextStateFinder(key);
	if(new_state == NULL || new_state == 0){
		return 11;
	}
	return new_state;
}

int nextStateFinder(Key key){
	int newstate = 0;
	/**************************************************************************/
	// State Transitions for Integer
    transitions[Key(0,int1)] = 3;
	transitions[Key(0,oct1)] = 3;
	transitions[Key(0,int2)] = 3;
	transitions[Key(18,int1)] = 0;
	transitions[Key(19,INTEGER)] = 3;
	transitions[Key(3,int2)] = 3;
	transitions[Key(3,oct1)] = 3;
	transitions[Key(3,int1)] = 3;

	// State Transitions for Float
	transitions[Key(0,float1)] = 20;
	transitions[Key(18,float1)] = 20;
	transitions[Key(29,float1)] = 20;
	transitions[Key(6,float1)] = 20;
	transitions[Key(3,float1)] = 20;
	transitions[Key(20,int1)] = 4;
	transitions[Key(20,oct1)] = 4;
	transitions[Key(20,int2)] = 4;
	transitions[Key(4,int1)] = 4;
	transitions[Key(4,oct1)] = 4;
	transitions[Key(4,int2)] = 4;

	// State Transitions for Octal
	transitions[Key(0,int1)] = int3;
	transitions[Key(29,oct1)] = 6;
	transitions[Key(29,int1)] = 29;
	transitions[Key(int1,oct1)] = 6;
	transitions[Key(OCTAL,int1)] = 6;
	transitions[Key(OCTAL,oct1)] = 6;

	// State Transitions for Hexadecimal
	transitions[Key(int3,hex1)] = hex1;
	transitions[Key(hex1,int1)] = hex1;
	transitions[Key(hex1,oct1)] = 5;
	transitions[Key(hex1,int2)] = 5;
	transitions[Key(hex1,hex2)] = 5;
	transitions[Key(5,int1)] = 5;
	transitions[Key(5,oct1)] = 5;
	transitions[Key(5,int2)] = 5;
	transitions[Key(5,hex2)] = 5;

	// State Transitions for IDENTIFIER
	transitions[Key(0,hex1)] = 9;
	transitions[Key(0,hex2)] = 9;		
	transitions[Key(0,IDENTIFIER)] = 9;
	transitions[Key(9,int1)] = 9;
	transitions[Key(9,int2)] = 9;		
	transitions[Key(9,oct1)] = 9;
	transitions[Key(9,hex1)] = 9;
	transitions[Key(9,hex2)] = 9;		
	transitions[Key(9,IDENTIFIER)] = 9;

	// State Transitions for CHAR
	transitions[Key(0,char1)] = 12;
	transitions[Key(12,char2)] = 13;
	transitions[Key(12,space)] = 14;
	transitions[Key(12,int1)] = 14;
	transitions[Key(12,int2)] = 14;
	transitions[Key(12,oct1)] = 14;		
	transitions[Key(12,float1)] = 14;
	transitions[Key(12,hex1)] = 14;
	transitions[Key(12,hex2)] = 14;		
	transitions[Key(12,IDENTIFIER)] = 14;
	transitions[Key(12,str1)] = 14;
	transitions[Key(12,str2)] = 14;
	transitions[Key(14,char1)] = 8;	

	transitions[Key(13,int1)] = 15;
	transitions[Key(13,int2)] = 15;
	transitions[Key(13,oct1)] = 15;		
	transitions[Key(13,float1)] = 15;
	transitions[Key(13,hex1)] = 15;
	transitions[Key(13,hex2)] = 15;		
	transitions[Key(13,IDENTIFIER)] = 15;
	transitions[Key(13,str1)] = 15;
	transitions[Key(13,char2)] = 15;
	transitions[Key(13,str2)]=15;
	transitions[Key(13,char1)]=17;
	transitions[Key(17,char1)] = 8;	
	transitions[Key(15,char1)] = 8;	

	// State Transitions for string
	transitions[Key(0,str1)] = 22;
	transitions[Key(22,int1)] = 23;
	transitions[Key(22,oct1)] = 23;		
	transitions[Key(22,float1)] = 23;
	transitions[Key(22,hex1)] = 23;
	transitions[Key(22,hex2)] = 23;		
	transitions[Key(22,IDENTIFIER)] = 23;
	transitions[Key(22,str1)] = 7;
	transitions[Key(22,int2)] = 23;
	transitions[Key(22,char1)] = 23;	
	transitions[Key(22,int2)] = 23;
	transitions[Key(22,space)] = 23;
	transitions[Key(22,char2)] = 24;
	transitions[Key(22,str2)] = 23;
	
	transitions[Key(23,int1)] = 23;
	transitions[Key(23,int2)] = 23;
	transitions[Key(23,oct1)] = 23;		
	transitions[Key(23,float1)] = 23;
	transitions[Key(23,hex1)] = 23;
	transitions[Key(23,hex2)] = 23;		
	transitions[Key(23,IDENTIFIER)] = 23;
	transitions[Key(23,char1)] = 23;
	transitions[Key(23,space)] = 23;
	transitions[Key(23,str2)] = 23;
	transitions[Key(23,char2)] = 24;
	transitions[Key(23,str1)] = 7;

	transitions[Key(24,int1)] = 24;
	transitions[Key(24,int2)] = 24;
	transitions[Key(24,oct1)] = 24;		
	transitions[Key(24,float1)] = 24;
	transitions[Key(24,hex1)] = 24;
	transitions[Key(24,hex2)] = 24;		
	transitions[Key(24,IDENTIFIER)] = 24;
	transitions[Key(24,char1)] = 24;
	transitions[Key(24,space)] = 24;
	transitions[Key(24,str2)] = 24;	
	transitions[Key(24,char2)] = 23;
	transitions[Key(24,str1)] = 28;
	transitions[Key(28,str1)] = 7;

	/**************************************************************/

	Transitions::iterator vIter=transitions.find(Transitions::key_type(key));
	if (vIter!=transitions.end())
	{
		newstate = vIter->second;
	}
	return newstate;
}