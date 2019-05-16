#include "stdafx.h"
#include "DuckInterpreter.h"

// Can eliminate if nothing needs to be initialized.
DuckInterpreter::DuckInterpreter()
{
}

// Can eliminate if nothing needs to be clean up.
DuckInterpreter::~DuckInterpreter()
{
}

void DuckInterpreter::RecordStatements(string a_fileName)
{
	m_statements.RecordStatements(a_fileName);
}

double DuckInterpreter::EvaluateArithmenticExpression(const string &a_statement, int a_nextPos)
{
	double finalValue = 0;
	m_operatorStack.push_back('[');
	//read values until the semicolon is read
	while (a_statement[a_nextPos] != ';')
	{
		string valueOrOperator;
		double numericalOperand;
		//parsed one thing from the statement at a time and inserted into respective stack: operator, number
		a_nextPos = ParseNextElement(a_statement, a_nextPos, valueOrOperator, numericalOperand);
		if (valueOrOperator == ";")
		{
			break;
		}
		/*
		cout << "-------------------------" << endl;
		cout << "valueOrOperator---->" << valueOrOperator << endl;
		cout << "numericalOperand---->" << numericalOperand << endl;
		*/
		//if single letter operator push to operator stack
		if (!valueOrOperator.empty() && valueOrOperator.length() > 0 && valueOrOperator.length() <= 2 && !isalnum(valueOrOperator[0]))
		{
			if (valueOrOperator.length() == 2 && isalnum(valueOrOperator[1]))
			{
			}
			else
			{
				//if an opening parenthesis is found
				if (valueOrOperator[0] == '(')
				{
					m_operatorStack.push_back(valueOrOperator[0]);
					continue;
				}
				//when a closing parenthesis is found
				if (valueOrOperator[0] == ')')/*also need a condition to satisfy if '(' exists or not in the stack? if not display error*/
				{
					//Note:: to complete after debugging:::a function would be great to do this redundant operation 
					while (m_operatorStack.size() > 1 && m_operatorStack.back() != '(')
					{
						//the operands
						double double_value2 = m_numberStack.back();
						m_numberStack.pop_back();
						double double_value1 = m_numberStack.back();
						m_numberStack.pop_back();
						//the operator
						char operate = m_operatorStack.back();
						m_operatorStack.pop_back();
						m_numberStack.push_back(findValue(double_value1, double_value2, operate));
					}
					m_operatorStack.pop_back();
					continue;
				}
				//if the size of the operator stack is more than one and the precedence of the current operator is less than 
				//the previous operator or equal to it
				string previousPrecedence(1, m_operatorStack.back());
				while (m_operatorStack.size() > 1 && FindPrecedence(valueOrOperator)/*new*/ <= /*from stack*/FindPrecedence(previousPrecedence))
				{
					//the operands
					double double_value2 = m_numberStack.back();
					m_numberStack.pop_back();
					double double_value1 = m_numberStack.back();
					m_numberStack.pop_back();
					//the operator
					char operate = m_operatorStack.back();
					m_operatorStack.pop_back();
					m_numberStack.push_back(findValue(double_value1, double_value2, operate));
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
				bool isExists = m_symbolTable.GetVariableValue(valueOrOperator, returnValue);
				if (isExists)
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
				m_numberStack.push_back(numericalOperand);
			}
		}
	}
	//once semi-colon is found then finish calculating any remaining stuff in the stacks
	while (m_operatorStack.size() > 1)
	{
		//the operands
		double double_value2 = m_numberStack.back();
		m_numberStack.pop_back();
		double double_value1 = m_numberStack.back();
		m_numberStack.pop_back();
		//the operator
		char operate = m_operatorStack.back();
		m_operatorStack.pop_back();
		m_numberStack.push_back(findValue(double_value1, double_value2, operate));
	}
	//the return value is the last remaining thing in the number stack
	return m_numberStack.back();
}

void DuckInterpreter::RunInterpreter()
{
	int nextStatement = 0;

	while (true)
	{
		string statement = m_statements.GetStatement(nextStatement);

		nextStatement = ExecuteStatement(statement, nextStatement);
	}
}

int DuckInterpreter::FindPrecedence(string op)
{
	if (op[0] == '*' || op[0] == '/')
	{
		return 5;
	}
	if (op[0] == '-' || op[0] == '+')
	{
		return 4;
	}
	if (op[0] == '<' || op[0] == '>')
	{
		return 3;
	}
	if (op[0] == '(' || op[0] == ')')
	{
		return 2;
	}
	//need something for > and < operators
	if (op[0] == '[' || op[0] == ';')
	{
		return 1;
	}
}

double DuckInterpreter::findValue(double val1, double val2, char operate)
{
	switch (operate)
	{
	case '+':
		return val1 + val2;
	case '-':
		return val1 - val2;
	case '*':
		return val1 * val2;
	case '/':
		return val1 / val2;
	case '>':
		if (val1 > val2) { return 1; }
		else { return 0; }
	case '<':
		if (val1 < val2) { return 1; }
		else { return 0; }
	}
}

int DuckInterpreter::ParseNextElement(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue)
{
	string tempString = "";
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
		if (true == ignoredSpace && (a_statement[i] == '=') || (a_statement[i] == '>') || (a_statement[i] == '<') || (a_statement[i] == ',') || (a_statement[i] == '"') || (a_statement[i] == '+') || (a_statement[i] == '-') || (a_statement[i] == '*') || (a_statement[i] == '/') || (a_statement[i] == ';') || (a_statement[i] == '(') || (a_statement[i] == ')'))
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

int DuckInterpreter::ExecuteStatement(string a_statement, int a_nextStatement)
{
	// Clear the stacks
	m_numberStack.clear();
	m_operatorStack.clear();
	int nextElement = 0;

	// Get the first syntactic element of the statement.  
	StatementType type = GetStatementStype(a_statement );
	
	// Based on the type, execute the remainder of the statement.
	switch (type)
	{
	case StatementType::ArithmeticStat:

		EvaluateArithmentStatment(a_statement);
		return a_nextStatement + 1;
	case StatementType::IfStat:
		return EvaluateIfStatement(a_statement, a_nextStatement);
	case StatementType::StopStat:
		exit(0);
	case StatementType::EndStat: 
		exit(0);
	case StatementType::PrintStat:
		EvaluatePrintStatement(a_statement);
		return a_nextStatement + 1;
	case StatementType::ReadStat:
		EvaluateReadStatement(a_statement);
		return a_nextStatement + 1;

	case StatementType::gotoStat:
		return EvaluateGotoStatement(a_statement);

	default:
		cerr << "BUGBUG - program terminate: invalid return value from GetStatementStype for the statement: " << a_statement << endl;
		exit(1);
	}
}

int DuckInterpreter::EvaluateGotoStatement(string a_statement)
{
	int nextPos = 0;
	string resultString;
	double placeHolder;

	nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
	assert(!resultString.empty());
	assert(resultString == "goto");

	resultString.clear();
	nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
	assert(!resultString.empty());

	int labelLocation = m_statements.GetLabelLocation(resultString);

	resultString.clear();
	nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
	if (resultString != ";")
	{
		cerr << "BUGBUG - program terminate: semicolon must follow the goto label " << a_statement << endl;
		exit(1);
	}
	return labelLocation;
}

void DuckInterpreter::EvaluateReadStatement(const string &a_statement)
{
	int nextPos = 0;
	string resultString;
	double placeHolder;

	nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
	assert(!resultString.empty());
	assert(resultString == "read");

	resultString.clear();
	nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
	assert(resultString == "\"");
	//display prompt between two quotes
	string displayString;
	for (unsigned int i = nextPos; i < a_statement.length(); i++)
	{
		if (a_statement[i] == '"')
		{
			nextPos = i + 1;
			break;
		}
		else
		{
			displayString += a_statement[i];
		}
	}
	cout << displayString;
	//assert that there is a comma after the quotes
	while (a_statement[nextPos] != ';')
	{
		resultString.clear();
		nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
		if (resultString == ";")
		{
			break;
		}
		assert(resultString == ",");
		//parse next element again
		resultString.clear();
		nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
		assert(!resultString.empty());

		double value;
		double result;
		bool isExists = m_symbolTable.GetVariableValue(resultString, value);
		if (isExists)
		{
			cin >> result;
			m_symbolTable.replaceVariableValue(resultString, result);
		}
		else
		{
			cin >> result;
			m_symbolTable.RecordVariableValue(resultString, result);
		}
	}

}

// we know at this point that we have an arithementic expression.  Excute this statement.  Any error
// will perminate the program.
void DuckInterpreter::EvaluateArithmentStatment(const string &a_statement)
{
	// Record the variable that we will be assignning a value.
	int nextPos = 0;
	string resultVariable;
	double placeHolder;
	nextPos = ParseNextElement(a_statement, nextPos, resultVariable, placeHolder);
	assert(!resultVariable.empty());

	string assigmentOp;
	nextPos = ParseNextElement(a_statement, nextPos, assigmentOp, placeHolder);
	assert(assigmentOp == "=");//correction made here

	double result = EvaluateArithmenticExpression(a_statement, nextPos);

	// Record the result.
	m_symbolTable.RecordVariableValue(resultVariable, result);

}
// Evaluates an if statement to determine if the goto should be executed.
int DuckInterpreter::EvaluateIfStatement(string a_statement, int a_nextStatement)
{
	// Get past of the "if" in the if statement.
	int nextPos = 0;
	string resultString;
	double placeHolder;
	nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
	assert(resultString == "if" );

	// Searching from the end, fine the goto statement and replace it by ";"  Record
	// the label in the goto.
	string label;
	int labelPosition = 0;
	//-------------------------------------------------------------------------------------------
	//found label has the integer position of goto in the string a_statement
	unsigned int foundLabel = a_statement.find("goto");
	if (foundLabel != string::npos)
	{
		labelPosition = foundLabel + 4;
		//find the label(following goto)
		for (unsigned int i = labelPosition; i < a_statement.length(); i++)
		{
			if (!isspace(a_statement[i]) && (isalnum(a_statement[i]) || a_statement[i] == '_'))
			{
				label += a_statement[i];
			}
			else if (isspace(a_statement[i]))
			{
				continue;
			}
			else
			{
				break;
			}
		}
		if (label.length() == 0)
		{
			cerr << "bugbug : missing label in goto statement" << endl;
			exit(1);
		}
	}
	else
	{
		cerr << "bugbug : goto statement missing in if clause" << endl;
		exit(1);
	}
	//replace everything from(goto to end of string) with a semicolon(;)
	a_statement.replace(a_statement.begin() + foundLabel, a_statement.end(), ";");
	//----------------------------------------------------------------------------------
	int labelLocation = m_statements.GetLabelLocation(label);
	//here error will be displayed if label is not found

	// Evaluate the remaining arithmentic expression. 
	double result = EvaluateArithmenticExpression(a_statement, nextPos);
	// If the result is zero, don't execute the goto.
	if (result == 0)
	{
		return a_nextStatement + 1;
	}
	return labelLocation;
}

void DuckInterpreter::EvaluatePrintStatement(string a_statement)
{
	int nextPos = 0;
	string resultString;
	double placeHolder;

	nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
	assert(resultString == "print");
	string displayString;

	bool firstPrint = false;

	while (a_statement[nextPos]!=';')
	{
		resultString.clear();
		nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
		if (resultString == ";")
		{
			break;
		}
		if (!resultString.empty())
		{
			if (resultString == "\"")
			{
				for (unsigned int i = nextPos; i < a_statement.length(); i++)
				{
					if (a_statement[i] == '"')
					{
						nextPos = i + 1;
						break;
					}
					else
					{
						displayString += a_statement[i];
					}
				}
				cout << displayString;
				firstPrint = true;
				displayString.clear();
			}

			else if (resultString == "," && firstPrint==true)
			{
				int prevPos = nextPos;
				resultString.clear();
				nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
				//if number turn into string and append
				if (!resultString.empty() && resultString!="\"")
				{
					double value;
					bool isExists = m_symbolTable.GetVariableValue(resultString, value);
					if (isExists)
					{
						cout << value;
					}
					else
					{
						cerr << "BUGBUG - program terminate: undefined variable " << a_statement << endl;
						exit(1);
					}
				}
				else if (resultString == "\"")
				{
					nextPos = prevPos;
					continue;
				}
				else
				{
					cerr << "BUGBUG - No numbers allowed in print statements" << a_statement << endl;
					exit(1);
				}
				//dealing with the comma or the semicolon that comes right after
				resultString.clear();
				if (a_statement[nextPos] == ';'){	break;}
				nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
				if (resultString == ";") { break; }
				if (resultString.empty() || resultString != ",")
				{
					cerr << "BUGBUG - a comma or semicolon must follow after each argument of a print statement" << a_statement << endl;
					exit(1);
				}
			}
			else
			{
				double value;
				bool isExists = m_symbolTable.GetVariableValue(resultString, value);
				if (isExists)
				{
					cout << value;
					firstPrint = true;
				}
				else
				{
					cerr << "BUGBUG - program terminate: undefined variable " << a_statement << endl;
					exit(1);
				}
				resultString.clear();
				nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
				if (resultString != ";" && resultString != ",")
				{
					cerr << "BUGBUG - program terminate: must be followed by , or \"."<< a_statement << endl;
					exit(1);
				}
			}
		}
		else
		{
			cerr << "BUGBUG - no variable type or statement type in print statement" << a_statement << endl;
			exit(1);
		}
	}//end of while
	cout << endl;
}//end of function
