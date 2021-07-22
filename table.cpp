#include "table.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <cstring>

Table::Table()
{
	maxRowNumber = 4;
}
nameOfColumn *Table::readColumnName(string fileName)
{
	ifstream file;
	file.open(fileName);
	int size = 0 ;
	nameOfColumn *n;
	maxColumnNumber=getMaxColumnNumber(fileName);
	n = new (nothrow) nameOfColumn [maxColumnNumber + 1];
	if(file.is_open())
	{
		string currentLine;
		while(getline(file, currentLine))
		{
			stringstream words(currentLine);
			string data;
			while(getline(words, data , ',') )
			{
				if(size == 0 && data != "") 
				{
					cout<<"First element in the column is not a NULL."<<endl;
					exit(0);
				}
				n[size].Name=data;
				size++;
				for(int i=0;i<size-1;i++)
					if(n[i].Name == n[size-1].Name)
					{
						cout<<"Error! Program found two same column name."<<endl;
						exit(0);
					}
				for(int i=0;i<data.length();i++)
					if(isdigit(data[i]))
					{
						cout<<"Error! Not correct column name "<<data<< " .";
						exit(0);
					}
			}
			return n;
		}
	}else
	{
		cout<<"File is not exist."<<endl;
		exit(0);
	}
	return 0;
}


line *Table::readTable(string fileName, struct nameOfColumn *n)
{
	ifstream file;
	file.open(fileName);
	int size = 0, k = 0 ;
	line *l;
	l = new (nothrow) line [maxRowNumber];
	if(file.is_open())
	{
		string currentLine;
		while(getline(file, currentLine))
		{
			stringstream words(currentLine);
			string data;
			if(k == maxRowNumber) getNewMassive(l);
			if(k > 0)
			{
				while(getline(words, data , ',') )
				{
					if(size==0) 
					{
						if(isNumber(data))
							l[k].ID = stoi(data);
						else
						{
							cout<<"Not coorect ID in the line "<<k<<"."<<endl;
							exit(0);
						}
						for(int i=1;i<k;i++)
							if(l[i].ID==l[k].ID)
							{
								cout<<"Line with ID "<< l[k].ID <<" is already exist."<<endl;
								exit(0);
							}
					}
					else if (size -1 < maxColumnNumber)
					{
						if(data == "") l[k].OP.push_back(" ");
						else l[k].OP.push_back(data);
					}
					size++;
				}
				if(size-1 < maxColumnNumber)
				{
					for(int i=0; i < maxColumnNumber - size + 1; i++)
					{
						cout<<"Program found NULL element. It is impossible to use element "<<n[size+i].Name<<k<<" as an adress."<<endl;
						l[k].OP.push_back(" ");
					}
				}
				size=0;
			}
			k++;
		}
	}else
	{
		cout<<"File is not exist."<<endl;
		exit(0);
	}
	maxRowNumber = k;
	return l;
}

void Table::printTable(struct line *&l, struct nameOfColumn *&n)
{
	for(int i=0;i<maxColumnNumber + 1;i++)
		cout<<n[i].Name<<" ";
	cout<<endl;
	for(int i=1;i<maxRowNumber;i++)
	{
		cout<<l[i].ID<<" ";
		for(int j=0; j<maxColumnNumber;j++)
			cout<<l[i].OP[j]<<" ";
		cout<<endl;
	}
}

void Table::getNewMassive(struct line *&l)
{
	line *l1;
	maxRowNumber = maxRowNumber * 2;
	l1 = new (nothrow) line [maxRowNumber];
	for(int i = 1; i < maxRowNumber/2; i++)
		for(int j = 0; j < l[i].OP.size(); j++)
		{
			l1[i].OP.push_back(l[i].OP[j]);
			l1[i].ID = l[i].ID;
		}
	delete[] l;
	l=l1;
}

bool Table::isNumber(string s)
{
	if(s[0]=='-')
		for(int i=1; i < s.length(); i++)
			if(isdigit(s[i])==false)
				return false;
	if(s[0]!='-')
		for(int i=0; i < s.length(); i++)
			if(isdigit(s[i])==false)
				return false;
	if(s.length()==0)
		return false;
	return true;
}

string Table::addZero(string s)
{
	string newString;
	int k=0;
	if(s[0]=='-') newString.push_back('0');
	for(int i=0;i<s.size();i++)
	{
		newString.push_back(s[i]);
		if(s[i] == '(' && s[i+1] == '-')
			newString.push_back('0');
		k++;
	}
	return newString;
}
string Table::analyzeString(string s, struct nameOfColumn *n, struct line *l)
{
	equation *e;
	string operand="";
	if(s[0] != '=') return "Error.";
	s.erase(0 , 1);
	s=addZero(s);
	int result;
	e = new (nothrow) equation;
	if(s[0]!='(') e->OP.push_back(' ');
	for(int i=0; i< s.length()+1; i++)
	{
		if(s[i]!='+' && s[i] != '-' && s[i]!= '/' && s[i]!= '*' && s[i]!= '(' && s[i]!= ')' && i!=s.length())
		{
			operand+=s[i];
		}
		else
		{
			if(s[0] == '*' || s[0] == '/' || s[0]== ')') return "Error.";
			if(i!=0) e->ARG.push_back(operand);
			e->OP.push_back(s[i]);
			operand="";
		}
		if(s[i]=='/' && s[i+1] == '0') return "Error.";
	}
	if(isdigit(s[s.length()-1])) e->OP.pop_back();
	if(s[s.length()-1]==')') e->OP.pop_back();
	string num;
	for(int i=0;i < e->ARG.size(); i++)
	{
		if(isNumber(e->ARG[i])==false && e->ARG[i]!="")
		{
			num ="";
			operand="";
			for(int j=0; j < e->ARG[i].length() ; j++)
			{
				if(isdigit(e->ARG[i][j]))
					num += e->ARG[i][j];
				else
					operand += e->ARG[i][j];
			}
			if(num == "0")
				return "Error.";
			for(int j=1; j<maxColumnNumber + 1;j++)
			{
				if(operand == n[j].Name)
				{
					for(int p=1; p<maxRowNumber + 1; p++)
					{
						if(stoi(num) == l[p].ID)
						{
							if(isNumber(l[p].OP[j-1]))
								e->ARG[i]=l[p].OP[j-1];
							else
								return "Error.";
						}
					}
				}
			}
			if(!isNumber(e->ARG[i]))
				return "Error.";
		}
	}
	if(e->OP.size()!=e->ARG.size())
		return "Error.";
	if(e->OP.size()==1)
		return e->ARG[0];
	else
		return solveEquation(e);
	return "";
}
int Table::findFirstElement(struct equation *e, char c, int start, int end)
{
	for(int i=start; i< end ;i++)
		if(e->OP[i]==c)
			return i;
	return -1;
}
int Table::findLastElement(struct equation *e, char c,int start, int end)
{
	int n=-1;
	for(int i=start ; i< end ; i++)
		if(e->OP[i]==c)
			n=i;
	return n;
}
void Table::moveElementInEquation(struct equation *&e, int pos)
{
	for(int j = pos; j < e->OP.size() - 1; j++)
	{
		e->ARG[j]=e->ARG[j+1];
		e->OP[j]=e->OP[j+1];
	}
	e->ARG.pop_back();
	e->OP.pop_back();
}
string Table::solveEquation(struct equation *e)
{
	int res, bOpen = findLastElement(e,'(',0,e->OP.size()),pos,bClose;
	if(bOpen>=0)
	{
		bClose = findFirstElement(e,')',bOpen,e->OP.size());
		if(bClose == -1)
			return "Error";
		if(bClose-bOpen == 1)
		{
			pos=bClose;
			moveElementInEquation(e, pos);
			if(e->OP.size()==1)
				return e->ARG[0];
			if(bOpen==0) e->OP[0]=' ';
			else{
				e->ARG[pos-2]=e->ARG[pos-1];
				moveElementInEquation(e, pos - 1);
			}
			return solveEquation(e);
		}
	}
	if(bOpen==-1)
	{
		if(findFirstElement(e,')',0,e->OP.size()) > 0)
			return "Error";
		bOpen=0;
		bClose=e->OP.size() - 1;
	}
	if(findFirstElement(e,'*',bOpen,bClose)>=0)
		pos = findFirstElement(e,'*',bOpen,bClose);
	else if(findFirstElement(e,'/',bOpen,bClose)>=0)
		pos = findFirstElement(e,'/',bOpen,bClose);
	else
		pos=bOpen+1;
	if(!isNumber(e->ARG[pos-1]) || !isNumber(e->ARG[pos]))
		return "Error";
	if(e->OP[pos]=='/')
		if(solvePair(stoi(e->ARG[pos-1]),e->OP[pos],stoi(e->ARG[pos])) * stoi(e->ARG[pos]) !=  stoi(e->ARG[pos-1]))
			return "Error! Result is not an integer.";
	e->ARG[pos-1]=to_string(solvePair(stoi(e->ARG[pos-1]),e->OP[pos],stoi(e->ARG[pos])));
	moveElementInEquation(e, pos);
	if(e->OP.size()==0)
		return "Error.";
	if(e->OP.size()>1)
		return solveEquation(e);
	else
		return e->ARG[0];
	return "";
}
int Table::solvePair(int a, char op, int b)
{
	if(op=='+')
		return a+b;
	if(op=='*')
		return a*b;
	if(op=='/' && b!=0)
		return a/b;
	if(op=='-')
		return a-b;
	return 0;
}
void Table::analyzeFile(struct line *&l, struct nameOfColumn *&n)
{
	for(int i=1; i< maxRowNumber; i++)
	{
		for(int j = 0; j < maxColumnNumber;j++)
		{
			if(!isNumber(l[i].OP[j]) && l[i].OP[j]!=" ")
			{
				string res = analyzeString(l[i].OP[j], n,l);
				l[i].OP[j]=res;
			}
		}
	}
}

int Table::getMaxColumnNumber(string fileName)
{
	ifstream file;
	file.open(fileName);
	int size = 0;
	if(file.is_open())
	{
		string currentLine;
		while(getline(file, currentLine))
		{
			stringstream words(currentLine);
			string data;
			while(getline(words, data , ',') )
				size++;
			return size-1;
		}
	}
	else
	{
		cout<<"File is not exist."<<endl;
		exit(0);
	}
	return 0;
}
