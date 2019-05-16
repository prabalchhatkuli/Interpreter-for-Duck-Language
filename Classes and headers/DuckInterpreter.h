#pragma once
#include "Statement.h"
#include "SymbolTable.h"
class DuckInterpreter
{
public:
	DuckInterpreter();
	~DuckInterpreter();

	// Records the statements that are in the specified file.
	void RecordStatements(string a_fileName);
	// Runs the interpreter on the recorded statements.
	void RunInterpreter();
private:

	// The statement object that holds the recorded statements.
	Statement m_statements;

	// The sysmbol table object that holds all the variable names and their values.
	SymbolTable m_symbolTable;

	// Stacks for the operators and numbers.  These will be used in evaluating statements.
	vector<char> m_operatorStack;
	vector<double> m_numberStack;


	enum class StatementType
	{
		ArithmeticStat,
		IfStat,
		ReadStat,
		PrintStat,
		StopStat,
		EndStat,
		gotoStat,
	};

	/**/
	/*
	GetStatementStype()
	NAME
			GetStatementStype -  Determines the type of the next statement
	SYNOPSIS
			StatementType GetStatementStype(const string &a_string);
				a_string             --> the statement whose type we need to find
	DESCRIPTION
			evaluates the first word expression form a_string, then evaluates the type of statement based on the first word
	RETURNS
			an object - enum of StatementType, which has one of seven total possibilities
	AUTHOR
			Prabal Chhatkuli
	DATE
			11:40 am 4/19/2019
	*/
	/**/
	StatementType GetStatementStype(const string &a_string)
	{
		string typeKey = "";
		bool foundType = false;
		// evaluating the first word of the statement
		for (unsigned int i = 0; i < a_string.length(); i++)
		{
			if (false == foundType && a_string[i]!=' ')
			{
				foundType = true;
			}
			//for things like end, stop, if, read, print, read
			if (true == foundType && a_string[i] != ' ')
			{
				if (isalpha(a_string[i]) || isdigit(a_string[i]))
				{
					typeKey+=a_string[i];
				}
				else
				{
					break;
				}
			}
			if (true == foundType && a_string[i] == ' ')
			{
				break;
			}
		}
		// returning statement type
		if (typeKey=="read")
		{
			return StatementType::ReadStat;
		}
		else if (typeKey=="if")
		{
			return StatementType::IfStat;
		}
		else if(typeKey=="goto")
		{
			return StatementType::gotoStat;
		}
		else if (typeKey=="print")
		{
			return StatementType::PrintStat;
		}
		else if (typeKey=="stop")
		{
			return StatementType::StopStat;
		}
		else if (typeKey=="end")
		{
			return StatementType::EndStat;
		}
		else
		{
			return StatementType::ArithmeticStat;
		}
	}/*StatementType GetStatementStype(const string &a_string);*/

	enum class ElementType
	{
		StringType,
		NumberType,
	};

	// Executes the statement at the specified location.  Returns the location of the next statement to be executed.
	int ExecuteStatement(string a_statement, int a_StatementLoc);

	// Returns the next element in the statement.  Returns the next location to be accessed.
	int ParseNextElement(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue);

	// Returns the precedence of an operator.
	int FindPrecedence(string op);

	//performs arithmetic operation
	double findValue(double val1, double val2, char operate);

	// Evaluate an arithmetic statement.
	void EvaluateArithmentStatment( const string &a_statement );

	// Evaluate an arithmetic expression.  Return the value.  The variable a_nextPos is index to the next  
	double EvaluateArithmenticExpression(const string &a_statement, int a_nextPos);

	int EvaluateIfStatement(string a_statement, int a_nextStatement);

	void EvaluatePrintStatement(string a_statement);

	void EvaluateReadStatement(const string &a_statement);

	int EvaluateGotoStatement(string a_statement);
};

