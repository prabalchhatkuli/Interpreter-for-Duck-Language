#pragma once
// This class will provide a mapping between the variables and their associated data.
class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable();

	// Record the value of a variable.
	void RecordVariableValue(string a_variable, double a_value)
	{
		m_SymbolTable[a_variable] = a_value;
	}
	// Get the value of a variable.  Returns false if the variable does not exist.
	bool GetVariableValue(string a_variable, double &a_value)
	{
		if (m_SymbolTable.find(a_variable) == m_SymbolTable.end())
		{
			return false;
		}
		else
		{
			a_value = m_SymbolTable.find(a_variable)->second;
			return true;
		}
	};

private:
	unordered_map<string, double> m_SymbolTable;
};

