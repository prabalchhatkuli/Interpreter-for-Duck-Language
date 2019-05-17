/**/
/*
Symboltable.h

NAME:	Symboltable -	This class will provide a mapping between the variables and their associated data,
						class that works to record, retrieve, replace values of variables

SYNOPSIS:	class SymbolTable{};
				Member Variables:
					Private:
							m_SymbolTable      -> contains the different variables(strings) and their assigned numerical values
				Member Functions:
					Public:
							RecordVariableValue()    -> Record the value of a variable.
							GetVariableValue()    -> Get the value of a variable.  Returns false if the variable does not exist.
							replaceVariableValue()    -> replace the value of a varoable in the map
AUTHOR:		
			Prabal Chhatkuli
DATE:		
			04/2019-05/2019
*/
/**/
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
};/*class SymbolTable{};*/

