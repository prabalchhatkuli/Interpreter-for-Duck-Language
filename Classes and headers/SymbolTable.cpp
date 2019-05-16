#include "stdafx.h"
#include "SymbolTable.h"

// Might not need constructor and destructor.
SymbolTable::SymbolTable()
{
}


SymbolTable::~SymbolTable()
{
}

void SymbolTable::RecordVariableValue(string a_variable, double a_value)
{
	m_SymbolTable[a_variable] = a_value;
}

void SymbolTable::replaceVariableValue(string a_variable, double a_value)
{
	m_SymbolTable[a_variable] = a_value;
}

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
}