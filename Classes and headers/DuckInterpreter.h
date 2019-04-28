#pragma once
#include "Statement.h"
#include "SymbolTable.h"
class DuckInterpreter
{
public:
	DuckInterpreter();
	~DuckInterpreter();

	// Records the statements that are in the specified file.
	void RecordStatements(string a_fileName)
	{
		m_statements.RecordStatements(a_fileName);
	}
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

	// Determines the type of the next statement.
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
	}

	enum class ElementType
	{
		StringType,
		NumberType,
	};

	// Executes the statement at the specified location.  Returns the location of the next statement to be executed.
	int ExecuteStatement(string a_statement, int a_StatementLoc);

	// Returns the next element in the statement.  Returns the next location to be accessed.
	int ParseNextElement(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue)
	{
		string tempString="";
		bool ignoredSpace = false;
		for (unsigned int i = a_nextPos; i < a_statement.length(); i++)
		{
			if (false == ignoredSpace && a_statement[i] != ' ')
			{
				ignoredSpace = true;
			}
			if (true == ignoredSpace && (isalpha(a_statement[i]) || isdigit(a_statement[i]) || a_statement[i] == '.'))
			{
				tempString += a_statement[i];
			}
			if (true == ignoredSpace && (a_statement[i] == '='))
			{
				if (tempString.length() == 0)
				{
					tempString += a_statement[i];
					a_nextPos = i + 1;
				}
				else
				{
					a_nextPos = i;
				}
				break;
			}
			if (true == ignoredSpace && !(isalpha(a_statement[i]) || isdigit(a_statement[i])) && a_statement[i] != '='&&a_statement[i] != '.')
			{
				a_nextPos = i;
				break;
			}
		}
		//checking if the value stores in the tempString is double or string
		bool isNum = true;
		for (unsigned int i = 0; i < tempString.length(); i++)
		{
			if (!isdigit(tempString[i]))
			{
				if (tempString[i] == '.')
				{
					continue;
				}
				else
				{
					isNum = false;
					break;
				}
			}
		}
		if (true == isNum)
		{
			stringstream ss;
			ss << tempString;
			ss >> numValue;
		}
		else
		{
			a_stringValue = tempString;
		}
		return a_nextPos;
	}

	// Returns the precedence of an operator.
	int FindPrecedence(string op) { return 1; }

	// Evaluate an arithmetic statement.
	void EvaluateArithmentStatment( const string &a_statement );

	// Evaluate an arithmetic expression.  Return the value.  The variable a_nextPos is index to the next  
	double EvaluateArithmenticExpression(const string &a_statement, int a_nextPos) 
	{
		double finalValue=0;
		while (a_statement[a_nextPos] != ';')
		{
			string valueOrOperator;
			double numericalOperand;
			ParseNextElement(a_statement, a_nextPos, valueOrOperator, numericalOperand);
			//if single letter operator push to operator stack
			if (!valueOrOperator.empty() && valueOrOperator.length() == 1)
			{
				m_operatorStack.push_back(valueOrOperator[0]);
			}
			//else push to the operand stack
			else
			{
				double returnValue;
				if (!valueOrOperator.empty())
				{
					bool isExits = m_symbolTable.GetVariableValue(valueOrOperator, returnValue);
					if (isExits)
					{
						m_numberStack.push_back(returnValue);
					}
					else
					{
						cerr << "BUGBUG - program terminate: provided variable does not exist " << a_statement << endl;
						exit(1);
					}
				}
				else
				{
					m_numberStack.push_back(numericalOperand);
				}
			}
		}
		return finalValue; 
	}

	int EvaluateIfStatement(string a_statement, int a_nextStatement);

};

