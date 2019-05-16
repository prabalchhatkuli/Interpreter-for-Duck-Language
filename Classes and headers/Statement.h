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

};

