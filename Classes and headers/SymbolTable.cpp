#include "stdafx.h"
#include "SymbolTable.h"

// Might not need constructor and destructor.
SymbolTable::SymbolTable()
{
}


SymbolTable::~SymbolTable()
{
}

/**/
/*
SymbolTable::RecordVariableValue()

NAME
		SymbolTable::RecordVariableValue - record the value of a variable
SYNOPSIS
		void SymbolTable::RecordVariableValue(string a_variable, double a_value);
			a_variable             --> the string/key to access the map
			a_value             --> the value to be stored
DESCRIPTION
		uses a_variable as a key to store the value in m_SymnolTable
RETURNS
		void
AUTHOR
		Prabal Chhatkuli
DATE
		2:45am 4/19/2019
*/
/**/
void SymbolTable::RecordVariableValue(string a_variable, double a_value)
{
	m_SymbolTable[a_variable] = a_value;
}/*void SymbolTable::RecordVariableValue(string a_variable, double a_value);*/

/**/
/*
SymbolTable::replaceVariableValue()

NAME
		SymbolTable::replaceVariableValue - record the value of a variable
SYNOPSIS
		void SymbolTable::RecordVariableValue(string a_variable, double a_value);
			a_variable             --> the string/key to access the map
			a_value             --> the value to be stored
DESCRIPTION
		replaces the value of a map/key.
RETURNS
		void
AUTHOR
		Prabal Chhatkuli
DATE
		5:00pm 4/20/2019
*/
/**/
void SymbolTable::replaceVariableValue(string a_variable, double a_value)
{
	m_SymbolTable[a_variable] = a_value;
}

/**/
/*
SymbolTable::GetVariableValue()

NAME
		SymbolTable::GetVariableValue - Get the value of a variable.  Returns false if the variable does not exist.
SYNOPSIS
		bool SymbolTable::GetVariableValue(string a_variable, double &a_value);
			a_variable             --> the variable whose value is needed
			a_value             --> the container variable passed by reference which will contain the value of the variable
DESCRIPTION
		searches the m_SymbolTable map for a variabel if found passes it by reference, if not, returns a boolean (false)
RETURNS
		true if a value exists, false if it does not; passed by reference is the value of variable in a_value if it exists
AUTHOR
		Prabal Chhatkuli
DATE
		11:30pm 5/29/2019
*/
/**/
bool SymbolTable::GetVariableValue(string a_variable, double &a_value)
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
}/*bool SymbolTable::GetVariableValue(string a_variable, double &a_value);*/