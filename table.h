#ifndef TABLE_H
#define TABLE_H
#include <string>
#include <vector>
using namespace std;
struct nameOfColumn
{
	string Name;
};
struct line
{
	int ID;
	vector<string> OP;
};
struct equation
{
	vector<char> OP;
	vector<string> ARG;
};	
class Table
{
	private:
		int maxColumnNumber;
		int maxRowNumber;
		string analyzeString(string s, struct nameOfColumn *n,struct line *l);
		void getNewMassive(struct line *&l);
		bool isNumber(string s);
		int getMaxColumnNumber(string fileName);
		void moveElementInEquation(struct equation *&e, int pos);
		int solvePair(int a, char op, int b);
		int findFirstElement(struct equation *e, char c,int start,int end);
		int findLastElement(struct equation *e, char c,int start, int end);
		string addZero(string s);
	public:
		Table();
		string solveEquation(struct equation *e);
		nameOfColumn *readColumnName(string fileName);
		line *readTable(string fileName, struct nameOfColumn *n);
		void printTable(struct line *&l, struct nameOfColumn *&n);
		void analyzeFile(struct line *&l, struct nameOfColumn *&n);
};

#endif
