#pragma once
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include<sstream>
#include "lex.h"
using namespace std;

string ErrorMessage = "";
int currentToken;
int nextToken;

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
	bool functionPrint();
	bool exprlog();
	bool opand();
	bool opno();
	bool expr();
	bool exprel();
	bool product();
	bool functionSign();
	bool functionTerm();
	void skipLine();

template<typename T>
std::string to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void backToken(){
	currentToken--; 
}

//Skip Line
 void skipLine(){
 	int line;
     line = tokenVector[currentToken].tokenLineNumber;
	 cout<<"in SL, old line : "<<tokenVector[currentToken].tokenLineNumber<<endl;
     while(line==tokenVector[currentToken].tokenLineNumber)
	 {
        currentToken++;
     }	
	 cout<<"in SL, new line : "<<tokenVector[currentToken].tokenLineNumber<<endl;
	 cout<<"in SL, new word : "<<tokenVector[currentToken].tokenWord<<endl;
}

void nextTokenValue(){
	//cout<<"CT is : "<<currentToken<<" : "<<tokenVector.size()<<endl;
	if(size_t(currentToken) < tokenVector.size()-1)
	{
		currentToken = currentToken + 1;
	}
	else
	{
		cout<<"in here"<<endl;
		throw 1;
	}
} 

//definition for functionTerm()
bool functionTerm()
{
    cout<<"inside Term function :"<<tokenVector[currentToken].tokenWord<<endl;
	if(tokenVector[currentToken].tokenWord=="(")
	{
		nextTokenValue();
        if(exprlog())
		{
            nextTokenValue();
            if(tokenVector[currentToken].tokenWord==")")
            {
				return true;
            }
            else
            {
                ErrorMessage = ErrorMessage + string("Line Term: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter )") + string(" ") + string("\n");
                return false;
            }
        }
	}
	else if(tokenVector[currentToken].tokenWord=="true"||tokenVector[currentToken].tokenWord=="false")
    {
        //cout<<"if token is true or false"<<endl;
        return true;
    }
    else if(tokenVector[currentToken].tokenName=="CHAR"||tokenVector[currentToken].tokenName=="INTEGER"||tokenVector[currentToken].tokenName=="FLOAT"||tokenVector[currentToken].tokenName=="IDENTIFIER" ||tokenVector[currentToken].tokenName=="STRING")
    {
		//cout<<"if token is char or int or float or identifier or string : "<<tokenVector[currentToken].tokenWord<<endl;
        return true;
    }
    else if(call_function())
    {
        //cout<<"I went to call function"<<endl;
        return true;
    }
    else
    {
     //    ErrorMessage = ErrorMessage + string("Line FT1: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Identifier or Value") + string(" ");
        return false;
    }
    return false;
}

//function definition for sign()
bool functionSign()
{
//	cout<<"inside sign function :"<<tokenVector[currentToken].tokenWord<<endl;
	if(tokenVector[currentToken].tokenWord=="-")
	{
		nextTokenValue();
		if(functionTerm()) return true;
	}
	else
	{
		if(functionTerm())
		{
			return true;
		}
	}
	return false;
}

//function definition for product()
bool product()
{
//	cout<<"inside product function : "<<tokenVector[currentToken].tokenWord<<endl;
	do{
		if(!functionSign())	return false;
		nextTokenValue();
		nextToken=currentToken;
		nextTokenValue();
	} while((tokenVector[nextToken].tokenWord=="/")||(tokenVector[nextToken].tokenWord=="*")||(tokenVector[nextToken].tokenWord=="%"));
	backToken();
	backToken();
	return true;
}

//function definition for expr()
bool expr()
{
//	cout<<"inside expr function :"<<tokenVector[currentToken].tokenWord<<endl;
	do{
		if(!product()) return false;
		nextTokenValue();
		nextToken = currentToken;
		nextTokenValue();
		cout<<"****************** :"<<tokenVector[nextToken].tokenWord<<endl;
	} while((tokenVector[nextToken].tokenWord=="-") || (tokenVector[nextToken].tokenWord=="+"));
	backToken();
	backToken();
	return true;
}

//function definition for exprel()
bool exprel()
{
//	cout<<"inside exprel function : "<<tokenVector[currentToken].tokenWord<<endl;
	do{
		if(!expr()) return false;
		nextTokenValue();
		nextToken=currentToken;
		nextTokenValue();
	} while((tokenVector[nextToken].tokenWord==">")||(tokenVector[nextToken].tokenWord=="<")||(tokenVector[nextToken].tokenWord=="!=")||(tokenVector[nextToken].tokenWord=="==")||(tokenVector[nextToken].tokenWord==">=")||(tokenVector[nextToken].tokenWord=="<="));
	backToken();
	backToken();
	return true;
}

//function definition for opno()
bool opno()
{
//	cout<<"inside opno function : "<<tokenVector[currentToken].tokenWord<<endl;
	if(tokenVector[currentToken].tokenWord=="!")
	{
		nextTokenValue();
		if(exprel()) return true;
	}
	else
	{
		if(exprel())
		{
			return true;
		}
	}
	return false;
}

//function definition for opand()
bool opand()
{
	do{
		if(!opno()){
			return false;
		}
		nextTokenValue();
		nextToken=currentToken;
		nextTokenValue();
	} while(tokenVector[nextToken].tokenWord=="&");
	backToken();
	backToken();
	return true;
}

//function definition for experlog()
bool exprlog()
{
	do{
		if(!opand()) return false;
		nextTokenValue();
		nextToken=currentToken;
		nextTokenValue();
	} while(tokenVector[nextToken].tokenWord=="|");
	backToken();
	backToken();
	return true;
}

//function definition for call function
bool call_function()
{
    cout<<"inside call function"<<tokenVector[currentToken].tokenName<<endl;
    if(tokenVector[currentToken].tokenName=="IDENTIFIER" || tokenVector[currentToken].tokenWord=="print" || tokenVector[currentToken].tokenWord=="read" )
	{
		nextTokenValue();
		if(tokenVector[currentToken].tokenWord=="(")
		{
			nextTokenValue();          
			if(tokenVector[currentToken].tokenName=="STRING")
			{
				do{
					if(tokenVector[currentToken].tokenName=="STRING")
					{
						nextTokenValue();
					}
					nextToken = currentToken;
					nextTokenValue();
				}while(tokenVector[nextToken].tokenWord != ")" || tokenVector[nextToken].tokenWord ==",");
				backToken();
			}
            if(tokenVector[currentToken].tokenWord==")")
			{
				nextTokenValue();
				if(tokenVector[currentToken].tokenWord==";")
				{
				//	nextTokenValue();
					return true;
				}
				else
				{
					backToken();
					 ErrorMessage = ErrorMessage + string("Line CallFunc1: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter ;") + string(" ") + string("\n");
					 skipLine();
			
				}
			}
			else 
			{
				do{
					if(!exprlog())
					{
						return false;
					}
					else
					{
						nextTokenValue();
						if(tokenVector[currentToken].tokenWord==")")
                        {
                            nextTokenValue();
                            if(tokenVector[currentToken].tokenWord==";")
                            {
                                return true;
                            }
                            else
                            {
								backToken();
                                 ErrorMessage = ErrorMessage + string("Line CallFunc2: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter ;") + string(" ") + string("\n");
								 skipLine();
								
                            }
                        }
					}
				}while(tokenVector[currentToken++].tokenWord==",");
			}
		}
	}
}

// function definition for dvar_global()
bool dvar_global()
{
	cout<<"inside dvar global function : "<<tokenVector[currentToken].tokenWord<<endl;
    if(tokenVector[currentToken].tokenWord==";")
	{
		//nextTokenValue();
		nextTokenValue();
        return true;
    }
    else if(tokenVector[currentToken].tokenWord==",")
	{
		nextTokenValue();
        if(tokenVector[currentToken].tokenName=="IDENTIFIER")
		{
            nextTokenValue();
			dvar_global();
            return true;
        }
        else
		{
            ErrorMessage = ErrorMessage + string("Line : ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Identifier") + string(" ") + string("\n");
            return false;
 	    }
	}
	else
	{
		if(tokenVector[currentToken].tokenWord!="(")
		{
			//currentToken++;
			ErrorMessage = ErrorMessage + string("Line hi : ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter ;") + string(" ") + string("\n");
		}
		 return false;   
    }
}

// function definition for parameter()
bool parameter(){
	
    cout<<"inside parameter function "<<tokenVector[currentToken].tokenWord<<endl;
	do{
		currentToken++;
       if(tokenVector[currentToken].tokenWord=="integer"||tokenVector[currentToken].tokenWord=="char"||tokenVector[currentToken].tokenWord=="float"||tokenVector[currentToken].tokenWord=="string"||tokenVector[currentToken].tokenWord=="boolean")
		{
            currentToken++;
			if(tokenVector[currentToken].tokenName=="IDENTIFIER")
			{
				//do nothing
			}
			else
			{
				ErrorMessage = ErrorMessage + string("Line p1: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Identifier") + string(" ");
				return false;
			}
		}
		else
		{
			if(tokenVector[--currentToken].tokenWord!="(")
			{
				ErrorMessage = ErrorMessage + string("Line p2: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Type") + string(" ");
				nextTokenValue();
			}
			nextTokenValue();
			return false;
		}
		currentToken++;
	}while(tokenVector[currentToken].tokenWord == ",");
	return true;
}


//Function dvar_local
bool dvar_local()
{
	cout<<"inside dvar local function : "<<tokenVector[currentToken].tokenWord<<endl;
   if(tokenVector[currentToken].tokenWord=="integer"||tokenVector[currentToken].tokenWord=="char"||tokenVector[currentToken].tokenWord=="float"||tokenVector[currentToken].tokenWord=="string"||tokenVector[currentToken].tokenWord=="boolean"||tokenVector[currentToken].tokenWord=="void")
	{
dvar_local_ID:		nextTokenValue();
        if(tokenVector[currentToken].tokenName=="IDENTIFIER")
        {
			nextTokenValue();
			if(tokenVector[currentToken].tokenWord=="=")
            {

                nextTokenValue();
                if(exprlog())
				{
					nextTokenValue();
					if(tokenVector[currentToken].tokenWord==",")
					{
						goto dvar_local_ID;
					}		
                    if(tokenVector[currentToken].tokenWord==";"){
                        return true;
                    }
					else
					{	
						 ErrorMessage = ErrorMessage + string("Line local1: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter") + string(" ") + string(";") + "\n";
					}
				}
				else
				{
					ErrorMessage = ErrorMessage + string("Line local2: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Identifier or Value") + string(" ")+ "\n";
					return false;
				}
            }
            if(tokenVector[currentToken].tokenWord==";")
            {
                return true;
            }
            else
            {
                ErrorMessage = ErrorMessage + string("Line local3: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter") + string(" ")+ string(";") + "\n";
            return false;
            }
        }
        else
		{
            ErrorMessage = ErrorMessage + string("Line local4: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Identifier") + string(" ")+ "\n";
            return false;
        }
	}
	else
	{
	//	 ErrorMessage = ErrorMessage + string("Line local5: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Type") + string(" ")+ "\n";
         return false;
	}
	return false;
}

bool functionAssign()
{
	cout<<"inside assign function : "<<tokenVector[currentToken].tokenName<<endl;
    if(tokenVector[currentToken].tokenName=="IDENTIFIER")
	{
		nextTokenValue();
		if(tokenVector[currentToken].tokenWord=="=")
		{
			nextTokenValue();
            if(exprlog())
            {
				nextTokenValue();
                if(tokenVector[currentToken].tokenWord==";")
                {
                   // nextTokenValue();
                    return true;
                }
                else
                {
                    backToken();
					ErrorMessage = ErrorMessage + string("Line Asgn1: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter ;") + string(" ")+ string("\n");
                    skipLine();
					return false;
                }
            }
			else
			{
				 ErrorMessage = ErrorMessage + string("Line Asgn2: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Identifier or Value") + string(" ")+ string("\n");
                 skipLine();
				 return false;
			}
		}
        else
        {
			backToken();
            return false;
        }
	}
    else
    {
        return false;
    }
}

bool functionLine()
{
	cout<<"inside line function "<<tokenVector[currentToken].tokenWord<<endl;
//	if(!dvar_local() || !functionIf() || !functionWhile() || !functionAssign() || !functionReturn() || !call_function()||functionPrint())
	if(functionReturn())
	{
		cout<<"*********function Return is true**********"<<endl;
		return true;
	}
/*	else if(functionPrint())
	{
		cout<<"*********function print is true**********"<<endl;
		return true;
	} */
	else if(functionIf())
	{
		cout<<"*********function IF is true**********"<<endl;
		return true;
	}
	else if(functionWhile())
	{
		cout<<"*********function While is true**********"<<endl;
		return true;
	}
	else if(functionAssign())
	{
		cout<<"*********function Assign is true**********"<<endl;
		return true;
	}
	else if(dvar_local())
	{
		cout<<"*********function dvar_local is true**********"<<endl;
		return true;
	}
	else if(call_function())
	{
		cout<<"*********function CALL is true**********"<<endl;
		return true;
	}
	else
	{
		cout<<"I dint go to any function"<<endl;
		return false;
	}
	cout<<"end function line"<<endl;
	return true;
}

//function definition for functionIf()
bool functionIf()
{
    cout<<"inside if function :"<<tokenVector[currentToken].tokenWord<<endl;
    if(tokenVector[currentToken].tokenWord=="if")
	{
		nextTokenValue();
		if(tokenVector[currentToken].tokenWord=="(")
		{
			cout<<"inside if inside ("<<endl;
			nextTokenValue();
			if(exprlog())
				nextTokenValue();
            cout<<"in if 1 : "<<tokenVector[currentToken].tokenWord<<endl;
			if(tokenVector[currentToken].tokenWord==")")
			{
				nextTokenValue();
                if(functionLine())
                {
					cout<<"some statement after if "<<tokenVector[currentToken].tokenWord<<endl;
					 goto functionIf_ELSE;
			//		return true;
				}
                else if(tokenVector[currentToken].tokenWord=="{")
				{
					nextTokenValue();
					cout<<"{ has started : "<<tokenVector[currentToken].tokenWord<<endl;
					if(tokenVector[currentToken].tokenWord=="}")
					{
						cout<<"empty braces"<<endl;
						goto functionIf_ELSE;
						// return true;
					}
					else
					{
						while(functionLine())
						{
							cout<<"multiple statements inside {}"<<endl;
							nextTokenValue();
						}
						if(tokenVector[currentToken].tokenWord=="}")
						{
functionIf_ELSE:            nextTokenValue();
				            if(tokenVector[currentToken].tokenWord=="else")
                            {
								cout<<"****** ELSE ******"<<endl;
				                nextTokenValue();
                                if(functionLine())
                                {
									cout<<"inside else functionLine 1 "<<endl;
									return true;
                                    
                                }
                                else if(tokenVector[currentToken].tokenWord=="{")
                                {
                                    nextTokenValue();
                                }
                                if(tokenVector[currentToken].tokenWord=="}")
    				            {       
						            cout<<"empty braces"<<endl;
						            return true;
					            }
                                else
                                {
                                    while(functionLine())
                                    {
                                        
                                        nextTokenValue();
                                    }
                                    if(tokenVector[currentToken].tokenWord=="}")
                                    {
                                        return true;
                                    }
									else
                                    {
                                        ErrorMessage = ErrorMessage + string("Line If1: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter }") + string(" ") +string("\n");
										return false;
                                    }
                                }
                            }
                            else
                            {
								backToken();
                                return true;
                            }
						}
                        else
                        {
							ErrorMessage = ErrorMessage + string("Line If2: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter }") + string(" ") +string("\n");
                            return false;
                        }
					}
				}
                else
                {
					ErrorMessage = ErrorMessage + string("Line If3: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter {") + string(" ") +string("\n");
                    return false;
                }
			}
            else
            {
				ErrorMessage = ErrorMessage + string("Line If4: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter )") + string(" ") +string("\n");
                return false;
            }
		}
        else
        {
			ErrorMessage = ErrorMessage + string("Line If5: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter (") + string(" ") +string("\n");
            return false;            
        }
	}
}

bool functionWhile()
{
	cout<<"inside while function : "<<tokenVector[currentToken].tokenWord<<endl;
	if(tokenVector[currentToken].tokenWord=="while")
	{
		nextTokenValue();
		if(tokenVector[currentToken].tokenWord=="(")
		{
			if(exprlog())
			{
			//	backToken();
				cout<<"exprlog is true in while"<<endl;
			}
		//	nextTokenValue();
			if(tokenVector[currentToken].tokenWord==")")
			{
				nextTokenValue();
				if(tokenVector[currentToken].tokenWord=="{")
				{
					nextTokenValue();
                    while(functionLine())
                    {
                        cout<<"inside while functionLine loop"<<endl;
                        nextTokenValue();
                    }
                    if(tokenVector[currentToken].tokenWord=="}")
                    {
                        return true;
                    }
                    else
                    {
						ErrorMessage = ErrorMessage + string("Line While1: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter }") + string(" ") +string("\n");
                        return false;
                    }
				}
                else if(functionLine())
                {
                    return true;
                }
			}
            else
            {
				ErrorMessage = ErrorMessage + string("Line While2: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter )") + string(" ")+ string("\n");
                return false;
            }
		}
        else
        {
             ErrorMessage = ErrorMessage + string("Line While3: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter (") + string(" ") + string("\n");
            return false;
        }
	}
	return false;
  }

//bool Token::functionReturn()
bool functionReturn()
{
    cout<<"inside return function : "<<tokenVector[currentToken].tokenWord<<endl;
	if(tokenVector[currentToken].tokenWord=="return")
	{
		nextTokenValue();
        if(tokenVector[currentToken].tokenWord==";")
		{
            return true;
        }
        else 
        {
			cout<<"INSIDE RETURN"<<endl;
            if(exprlog())
			{
				cout<<"after exprlog call in return "<<tokenVector[currentToken].tokenWord<<endl;
     			nextTokenValue();
                if(tokenVector[currentToken].tokenWord==";")
                {
					cout<<"inside return 2 : "<<tokenVector[currentToken].tokenWord<<endl;
                    return true;                
                }
                else
                {
					ErrorMessage = ErrorMessage + string("Line Ret1: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter ;") + string(" ")+ string("\n");
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
	}
    else
    {
        return false;
    }
}

/*
bool functionPrint()
{
	cout<<"Inside Print Func :"<<tokenVector[currentToken].tokenWord<<endl;
	if(tokenVector[currentToken].tokenWord=="print")
	{
		nextTokenValue();
		cout<<"* 1 * : "<<tokenVector[currentToken].tokenWord<<endl;
//		system("PAUSE");
			if(tokenVector[currentToken].tokenWord=="\"")
			{
				cout<<"* 2 *"<<endl;
				exprlog();
				nextTokenValue();
				cout<<"* 3 *"<<endl;
//				system("PAUSE");
					if(tokenVector[currentToken].tokenWord=="\"")
					{
						nextTokenValue();
						if(tokenVector[currentToken].tokenWord==";")
						{
							return true;
						}
						else
						{
							cout<<ErrorMessage<<"Error: Expected Delimiter";
							return false;
						}
					}
					else
					{
						return false;
					}
			}
			else
			{
				return false;
			}
		}
	else
	{
//		cout<<"print keyword missing";
		return false;
	}
 return true;
}
*/

// function definition for dethod()
bool dmethod()
{
	cout<<"inside dmethod function : "<<tokenVector[currentToken].tokenWord<<endl;
	if(tokenVector[currentToken].tokenWord=="(")
	{
		if(parameter()) //do nothing
		{
		}
		cout<<tokenVector[currentToken].tokenWord<<endl;
		if(tokenVector[currentToken].tokenWord==")")
		{
			nextTokenValue();
			if(tokenVector[currentToken].tokenWord=="{")
			{
			   nextTokenValue();
			   while(tokenVector[currentToken].tokenWord != "}")
			   {
				   if(functionLine())
				   {
					   cout<<"line no is "<<tokenVector[currentToken].tokenLineNumber<<endl;
					   nextTokenValue();
				   }
				   else
				   {
					   cout<<"functionLine returned as false in dmethod"<<endl;
						return false;
				   }
			   }
			  if(tokenVector[currentToken].tokenWord=="}")
			   {
				   return true;
			   }
			   else
			   {
				   ErrorMessage = ErrorMessage + string("Line dmethod1: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter }") + string(" ")+ string("\n");
				   return false;
			   }
			}
            else
			{
				ErrorMessage = ErrorMessage + string("Line dmethod2: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter {") + string(" ")+ string("\n");
                return false;
            }
		}
        else
		{
			ErrorMessage = ErrorMessage + string("Line dmethod3: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter )") + string(" ")+ string("\n");
            return false;
        }
	}
	else
	{
		ErrorMessage = ErrorMessage + string("Line dmethod4: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Delimiter (") + string(" ")+ string("\n");
		return false;
	}

	cout<<"end dmethod : "<<tokenVector[currentToken].tokenWord<<endl;
	//return false;
}

// function definition for program()
bool program(){
while(currentToken != tokenVector.size()-1)
{
	cout<<"CT : "<<currentToken<<" : "<<tokenVector.size()<<endl;
	cout<<"RUNNING PROGRAM"<<tokenVector[currentToken].tokenWord<<endl;
    if(tokenVector[currentToken].tokenWord=="integer"||tokenVector[currentToken].tokenWord=="char"||tokenVector[currentToken].tokenWord=="float"||tokenVector[currentToken].tokenWord=="string"||tokenVector[currentToken].tokenWord=="boolean"||tokenVector[currentToken].tokenWord=="void")
        {
                //nextTokenValue();
			nextTokenValue();
        }
        else
        {
		   // ErrorMessage = ErrorMessage + string("Line prog1: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Type") + string("\n");
			return false;
        }
    if(tokenVector[currentToken].tokenName=="IDENTIFIER")
        {
			//nextTokenValue();
			nextTokenValue();
        }
        else
        {
			ErrorMessage = ErrorMessage + string("Line prog2: ") + to_string(static_cast<long long>(tokenVector[currentToken].tokenLineNumber)) + string(" ") + string("Expected Identifier") + string("\n");
			//cout<<ErrorMessage<<"Error: Identifier Expected"<<endl;
        }
	if(dvar_global())
        {
			cout<<endl<<"CT is : "<<currentToken<<endl;
            cout<<"inside program, after dvar_global"<<endl;
			continue;
        }
	else if(dmethod())
        {    
			cout<<"func Method is true in program"<<endl;
			cout<<"CT in dmethod : "<<currentToken<<" : "<<tokenVector.size()<<endl;
			nextTokenValue();
			continue;
        }      
}
return true;
}

int main(int argc, char *argv[])
{
	string line="";
	int lineCount=0;
	currentToken = 0;
	cout << "Initial size: " << tokenVector.size() <<endl;
	cout << "Before reading file!\n";
	//ifstream inputfile ("C://Users//prerna//Documents//Visual Studio 2010//Projects//Parser//Parser//input.txt");
	ifstream inputfile (argv[1]);
	while(getline(inputfile,line))
	{
		++lineCount;
		cout<<"LINE READING NOW IS : "<<lineCount<<"\n";
		split(line, lineCount);
	}
	//tokenVector.push_back(Token("-1","-1",lineCount++));
	cout << "Reading Done!\n";
	cout<<"total lines read : "<<lineCount<<"\n";

	//PARSER STARTS HERE:
	try{
		if(program() && currentToken == tokenVector.size())
		{
			cout<<"Build Successful"<<endl;
		}
		else
		{
			ofstream outputfile;
			outputfile.open(argv[2]);
			//outputfile.open("output.txt");
			outputfile<<ErrorMessage;
			outputfile.close(); 
		}
	}
	catch(int n)
	{
		ofstream outputfile;
		outputfile.open("output.txt");
		outputfile<<ErrorMessage;
		outputfile.close(); 
	}

	system("PAUSE");
	return 0;
}