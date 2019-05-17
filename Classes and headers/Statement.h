/**/
/*
Statement.h

NAME:	Statement -	This class will manage statements of the instructions inserted. 
					it will record statements and labels in those statements as well.

SYNOPSIS:	class Statement{};
				Member Variables:
					Private:
							m_statements      -> a vector of all statements
							m_labelToStatement    ->contains the mapping of specific labels and their location
				Member Functions:
					Public:
							RecordStatements()    -> check if a colon exits in a string before encountering a quotes
							GetStatement()    -> get the statement of a specific number in the record
							GetLabelLocation()    -> get the statement number of a specific label in the record
							colonPresence()    -> check if a colon exits in a string
							quotesBeforeColon()    -> check if a colon exits in a string before encountering a quotes
AUTHOR:
			Prabal Chhatkuli
DATE:
			04/2019-05/2019
*/
/**/
#pragma once
class Statement
{
public:
	Statement();
	~Statement();

	void RecordStatements(string a_sourceFileName);
	string GetStatement(int a_statementNum);

	int GetLabelLocation(string a_string);

	//these are used to assist the recordStatement function
	bool colonPresence(string findcolon);
	bool quotesBeforeColon(string findcolon);

private:
	vector<string> m_statements;//stores each line of the text each statements
	map<string, int> m_labelToStatement;//index, given a label, find the correspoponding label according to the label

};/*class Statement{};*/

