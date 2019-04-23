#include "stdafx.h"
#include "Statement.h"


Statement::Statement()
{
}


Statement::~Statement()
{
}

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
}

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
}

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
		m_statements.push_back(copyString);
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
	/*  //checking by printing
	for (vector<string>::iterator it = m_statements.begin(); it != m_statements.end(); ++it)
	{
		cout << (*it) << endl;
	}
	cout << "-------------------------------------------------------------------------------------------------------------" << endl;
	for (map<string, int>::iterator it = m_labelToStatement.begin(); it != m_labelToStatement.end(); ++it)
	{
		cout << (*it).first << "\t" <<(*it).second<<endl;
	}
	*/
}
