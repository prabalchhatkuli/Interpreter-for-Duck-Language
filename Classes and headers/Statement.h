#pragma once
class Statement
{
public:
	Statement();
	~Statement();

	void RecordStatements(string a_sourceFileName);
	string GetStatement(int a_statementNum) {
		return "";
	}
	int GetLabelLocation(string a_string) { return 1; }//name with a colon after is called a label
	bool colonPresence(string findcolon);
	bool quotesBeforeColon(string findcolon);

private:
	vector<string> m_statements;//stores each line of the text each statements
	map<string, int> m_labelToStatement;//index, given a label, find the correspoponding label according to the label

};

