#pragma once
// This class will provide a mapping between the variables and their associated data.
class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable();

	// Record the value of a variable.
	void RecordVariableValue(string a_variable, double a_value);

	// Get the value of a variable.  Returns false if the variable does not exist.
	bool GetVariableValue(string a_variable, double &a_value);

	//replace the value of a varoable in the map
	void replaceVariableValue(string a_variable, double a_value);

private:
	unordered_map<string, double> m_SymbolTable;
};

