#include "stdafx.h"
#include "Statement.h"


Statement::Statement()
{
}


Statement::~Statement()
{
}


/**/
/*
Statement::GetStatement()

NAME
		Statement::GetStatement - get the statement of a specific number in the record
SYNOPSIS
		string Statement::GetStatement(int a_statementNum);
			a_statementNum             --> the number of statement in the vector
DESCRIPTION
		gets a statement from m_statements vector.
RETURNS
		a string that contains a statement
AUTHOR
		Prabal Chhatkuli
DATE
		8:45pm 4/18/2019
*/
/**/
string Statement::GetStatement(int a_statementNum)
{
	return m_statements[a_statementNum];
}/*string Statement::GetStatement(int a_statementNum);*/

/**/
/*
Statement::GetLabelLocation()

NAME
		Statement::GetLabelLocation - get the statement of a specific number in the record
SYNOPSIS
		int Statement::GetLabelLocation(string a_string);
			a_string             --> the label whose value needs to be found
DESCRIPTION
		if a_string is found in the m_labelToStatement map, the integer is returned , if not found an error message is given and the
		program is terminated.
RETURNS
		integer that gives the location of the label in the map of labels
AUTHOR
		Prabal Chhatkuli
DATE
		8:45pm 4/18/2019
*/
/**/
int Statement::GetLabelLocation(string a_string)
{
	if (m_labelToStatement.find(a_string) != m_labelToStatement.end())
	{
		return m_labelToStatement.find(a_string)->second;
	}
	else
	{
		cerr << "BugBug : the label was not found in the map of the labels" << endl;
		exit(0);
	}
}/*int Statement::GetLabelLocation(string a_string);*/


/**/
/*
Statement::colonPresence()

NAME
		Statement::colonPresence - check if a colon exits in a string
SYNOPSIS
		bool Statement::colonPresence(string findcolon);
			findcolon             --> a string/statement
DESCRIPTION
		iterates through a string and checks each character with colon
RETURNS
		a boolean value: true if colon exits , false if does not
AUTHOR
		Prabal Chhatkuli
DATE
		5:35pm 5/7/2019
*/
/**/
bool Statement::colonPresence(string findcolon)
{
	for (int i = 0; i < findcolon.length(); i++)
	{
		if (findcolon[i] == ':')
		{
			return true;
		}
	}
	return false;
}/*bool Statement::colonPresence(string findcolon)*/

/**/
/*
Statement::quotesBeforeColon()

NAME
		Statement::quotesBeforeColon - check if a colon exits in a string before encountering a quotes
SYNOPSIS
		bool Statement::quotesBeforeColon(string findcolon);
			findcolon             --> a string/statement
DESCRIPTION
		iterates through a string and checks each character for quotes, if a quotes shows up a flag called colonBeforeQuotes is set
		if a colon is found before this happens then the function returns true. Then the functions iterates until the function ends.
RETURNS
		a boolean value: true if colon exists before quotes , false if quotes exists before colon
AUTHOR
		Prabal Chhatkuli
DATE
		8:45pm 4/18/2019
*/
/**/
bool Statement::quotesBeforeColon(string findcolon)
{
	bool colonBeforeQuotes = false;
	for (int i = 0; i < findcolon.length(); i++)
	{
		if (findcolon[i] == '"')
		{
			colonBeforeQuotes = true;
		}
		if (findcolon[i]==':'&& colonBeforeQuotes==false)
		{
			return true;
		}
	}
	return false;
}/*bool Statement::quotesBeforeColon(string findcolon);*/


/**/
/*
Statement::RecordStatements()

NAME
		Statement::RecordStatements - check if a colon exits in a string before encountering a quotes
SYNOPSIS
		void Statement::RecordStatements(string a_sourceFileName);
			a_sourceFilename             --> a string that contains the name of the filename to record statements from
DESCRIPTION
		Opens and reads the file one at a time, removes any empty lines, then iterates through each statement and uses
		functions colonPresence and quotesBeforeColon to find labels and record them in a map
RETURNS
		void
AUTHOR
		Prabal Chhatkuli
DATE
		3:40pm 4/15/2019
*/
/**/
void Statement::RecordStatements(string a_sourceFileName)//passing to the function the source file
{
	ifstream inf(a_sourceFileName);
	if (!inf)
	{
		cerr << "could not open the file: " << a_sourceFileName << endl;
		exit(1);//we are exiting because we are terminating the whole program, 0 if terminated the nice way, 1 if not
	}
	// Read through the file one line at a time.  Find labs and record them.
	string copyString;
	while (!inf.eof())
	{
		getline(inf, copyString);
		string statement;
		for (int i = copyString.length() - 1; i < copyString.length(); i--)
		{
			if (copyString[i] == ';')
			{
				statement = copyString.substr(0, i + 1);
				m_statements.push_back(statement);
				break;
			}
		}
	}

	// delete any label and record the remainder of the statement.
	int counter = 0;
	for (vector<string>::iterator it = m_statements.begin(); it != m_statements.end(); ++it)
	{
		if (colonPresence(*it) && quotesBeforeColon(*it))
		{
			int i = 0;
			for (i = 0; i < (*it).length(); i++)
			{
				if ((*it)[i] == ':')
				{
					break;
				}
			}//index of the colon on the statement is in i
			string mapKey = (*it).substr(0, i);
			int mapValue = counter;
			//insert the label into the map
			m_labelToStatement.insert(pair<string, int>(mapKey, mapValue));
			m_statements[counter] = (*it).substr(i + 1);
		}
		counter++;
	}
	//checking by printing
	for (vector<string>::iterator it = m_statements.begin(); it != m_statements.end(); ++it)
	{
		cout << (*it) << endl;
	}
	cout << "-------------------------------------------------------------------------------------------------------------" << endl;
	for (map<string, int>::iterator it = m_labelToStatement.begin(); it != m_labelToStatement.end(); ++it)
	{
		cout << (*it).first << "\t" <<(*it).second<<endl;
	}
	
}/*void Statement::RecordStatements(string a_sourceFileName);*/
