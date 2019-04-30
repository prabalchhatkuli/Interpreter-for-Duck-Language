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
			if (true == ignoredSpace && (a_statement[i] == '=') || (a_statement[i] == '+') || (a_statement[i] == '-') || (a_statement[i] == '*') || (a_statement[i] == '/') || (a_statement[i] == ';'))
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
	int FindPrecedence(string op) 
	{
		if (op[0] == '*' || op[0] == '/')
		{
			return 5;
		}
		if (op[0] == '-' || op[0] == '+')
		{
			return 4;
		}
	/*	if (op[0] == '(' || op[0] == ')')
		{
			return 3;
		}*/
	/*	if (op[0] == '<' || op[0] == '>')
		{
			return 3;
		}*/
		//need something for > and < operators
		if (op[0]=='['||op[0]==';')
		{
			return 1;
		}
	}

	//performs arithmetic expression
	double findValue(double a, double b, char operation)
	{
		switch (operation)
		{
			case '+': return a + b;
			case '-': return a - b;
			case '*': return a * b;
			case '/': return a / b;
		}
	}

	// Evaluate an arithmetic statement.
	void EvaluateArithmentStatment( const string &a_statement );

	// Evaluate an arithmetic expression.  Return the value.  The variable a_nextPos is index to the next  
	double EvaluateArithmenticExpression(const string &a_statement, int a_nextPos) 
	{
		double finalValue=0;
		m_operatorStack.push_back('[');
		while (a_statement[a_nextPos] != ';')
		{
			string valueOrOperator;
			double numericalOperand;
			//parsed one thing from the statement at a time and inserted into respective stack: operator, number
			//cout << "number: " << a_nextPos << endl;
			a_nextPos = ParseNextElement(a_statement, a_nextPos, valueOrOperator, numericalOperand);
			
			//if single letter operator push to operator stack
			if (!valueOrOperator.empty() && valueOrOperator.length() <= 2 && valueOrOperator.length() > 0 && !isalnum(valueOrOperator[0]))
			{
				if (valueOrOperator.length()==2 && isalnum(valueOrOperator[1]))
				{

				}
				else
				{
					cout << "operator wala value" << endl;
					cout << "value:" << valueOrOperator << endl;
					//if an opening parenthesis is found
					if (valueOrOperator[0] == '(')
					{
						m_operatorStack.push_back('(');
						continue;
					}
					//if a closing parenthesis is found
					if (valueOrOperator[0] == ')')/*also need a condition to satisfy if '(' exists if not display error*/
					{
						while (m_operatorStack.size() > 1 && m_operatorStack.back() != '(')
						{
							double val2 = m_numberStack.back();
							m_numberStack.pop_back();

							double val1 = m_numberStack.back();
							m_numberStack.pop_back();

							char op = m_operatorStack.back();
							m_operatorStack.pop_back();

							m_numberStack.push_back(findValue(val1, val2, op));
						}
						m_operatorStack.pop_back();
						continue;
					}
					//if the size of the operator stack is more than one and the precedence of the current operator is less than 
					//the previous operator or equal to it
					string previousPrecedence(1, m_operatorStack.back());
					while (m_operatorStack.size() > 1 && FindPrecedence(valueOrOperator)/*new*/ <= /*from stack*/FindPrecedence(previousPrecedence))
					{
						double val2 = m_numberStack.back();
						m_numberStack.pop_back();

						double val1 = m_numberStack.back();
						m_numberStack.pop_back();

						char op = m_operatorStack.back();
						m_operatorStack.pop_back();

						m_numberStack.push_back(findValue(val1, val2, op));
					}
					m_operatorStack.push_back(valueOrOperator[0]);
				}
			}
			//if not a single letter operator push to the operand stack, either the number, or by finding the number by looking into
			//the symbol table
			else
			{
				double returnValue;
				if (!valueOrOperator.empty())
				{
					cout << "variable wala value" << endl;
					cout << "value:" << valueOrOperator << endl;
					bool isExits = m_symbolTable.GetVariableValue(valueOrOperator, returnValue);
					if (isExits)
					{
						m_numberStack.push_back(returnValue);
					}
					else
					{
						cerr << "BUGBUG - program terminate: undefined variable " << a_statement << endl;
						exit(1);
					}
				}
				else
				{
					cout << "in else" << endl;
					cout << "number:" << numericalOperand << endl;
					m_numberStack.push_back(numericalOperand);
				}
			}
		}
		//once semi-colon is found then calculate the remaining stuff in the stack
		while (m_operatorStack.size() > 1)
		{
			double val2 = m_numberStack.back();
			m_numberStack.pop_back();

			double val1 = m_numberStack.back();
			m_numberStack.pop_back();

			char op = m_operatorStack.back();
			m_operatorStack.pop_back();

			m_numberStack.push_back(findValue(val1, val2, op));
		}
		//the return value is the last remaining thing in the number stack
		return m_numberStack.back();
	}

	int EvaluateIfStatement(string a_statement, int a_nextStatement);

};

