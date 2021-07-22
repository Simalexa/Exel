#include <stdio.h>
#include <iostream>
#include <string>
#include "table.cpp"
using namespace std;
int main()
{
	string filePath,file;
	Table t;
	line *l;
	nameOfColumn *n;
	cout<<"File Name:";
	cin >> file;
	filePath.append("./test_files/");
	filePath.append(file);
	n = t.readColumnName(filePath);
	l = t.readTable(filePath, n);
	t.analyzeFile(l,n);
	t.printTable(l,n);
	return 0;
}
