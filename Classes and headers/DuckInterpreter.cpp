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

/**/
/*
DuckInterpreter::RecordStatements()

NAME
		DuckInterpreter::RecordStatements - Records the statement that are in the specified file.
SYNOPSIS
		void DuckInterpreter::RecordStatements(string a_fileName);
			a_filename             --> the file to record the statements from.
DESCRIPTION
		calls the RecordStatements() function for the statement class object: m_statements, which is a
		private member variable of the calling class.
RETURNS
		void
AUTHOR
		Prabal Chhatkuli
DATE
		10:55pm 4/16/2019
*/
/**/
void DuckInterpreter::RecordStatements(string a_fileName)
{
	m_statements.RecordStatements(a_fileName);
}/*void DuckInterpreter::RecordStatements(string a_fileName);*/


/**/
/*
DuckInterpreter::EvaluateArithmenticExpression()

NAME
		DuckInterpreter::EvaluateArithmenticExpression -  Evaluate an arithmetic statement
SYNOPSIS
		double DuckInterpreter::EvaluateArithmenticExpression(const string &a_statement, int a_nextPos);
			a_statement             --> the statement whose value we need to evaluate
			a_nextPos             --> the position of next character to be read in the statement
DESCRIPTION
		iterates through an arithmetic statement to find value and operators, inserting them in respective operator stack
		or number stack and evaluate the final result by evaluating according to the precedence of the operators and placement 
		of numbers in the number stack
RETURNS
		the final result of evaluating the expression which is stored at the end of the number stack at the end of the function
AUTHOR
		Prabal Chhatkuli
DATE
		8:30pm 4/17/2019
*/
/**/
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
}/*double DuckInterpreter::EvaluateArithmenticExpression(const string &a_statement, int a_nextPos);*/


/**/
/*
DuckInterpreter::RunInterpreter()

NAME
		DuckInterpreter::RunInterpreter -  Runs the interpreter on the recorded statements.
SYNOPSIS
		void DuckInterpreter::RunInterpreter();
DESCRIPTION
		goes through the statements atrting at statement 0, recorded in the m_statements object of Statement class, and
		progressing according to the number of next statement that is returned after the execution of the statement
RETURNS
		void
AUTHOR
		Prabal Chhatkuli
DATE
		9:45pm 4/16/2019
*/
/**/
void DuckInterpreter::RunInterpreter()
{
	int nextStatement = 0;

	while (true)
	{
		string statement = m_statements.GetStatement(nextStatement);

		nextStatement = ExecuteStatement(statement, nextStatement);
	}
}/*void DuckInterpreter::RunInterpreter();*/

/**/
/*
DuckInterpreter::FindPrecedence()

NAME
		DuckInterpreter::FindPrecedence -  Returns the precedence of an operator
SYNOPSIS
		int DuckInterpreter::FindPrecedence(string op);
			op             --> a string which contains a operator
DESCRIPTION
		finds the precedence of the operator based on a couple of if statements that determine
		the hirearchy of the operators
RETURNS
		an integer value that shows the operators position in the hirearchy
AUTHOR
		Prabal Chhatkuli
DATE
		9:45pm 4/21/2019
*/
/**/
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
	if (op[0] == '[' || op[0] == ';')
	{
		return 1;
	}
}/*int DuckInterpreter::FindPrecedence(string op)*/


/**/
/*
DuckInterpreter::findValue()
NAME
		DuckInterpreter::findValue -  performs arithmetic operation
SYNOPSIS
		double DuckInterpreter::findValue(double val1, double val2, char operate);
			val1, val2             --> number variables that are on either side of an operator
			operate             --> the operator which determines the operation
DESCRIPTION
		switch statement changes based on the different possibility of operate variable and accordingly arithmetic values are calculated
RETURNS
		a double value calculated by using a specific operation based on a operator between two numbers
AUTHOR
		Prabal Chhatkuli
DATE
		1:10pm 4/21/2019
*/
/**/
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
}/*double DuckInterpreter::findValue(double val1, double val2, char operate);*/

/**/
/*
DuckInterpreter::ParseNextElement()
NAME
		DuckInterpreter::ParseNextElement -  Returns the next element in the statement.  Returns the next location to be accessed.
SYNOPSIS
		int DuckInterpreter::ParseNextElement(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue);
			a_statement             --> the statement to be parsed
			a_nextPos             --> the position of the next parsing to start from
			a_stringValue             -->  contains the returned value of the parsed value if it is a string
			numValue             --> contains returned value of the parsed value if it is a number
DESCRIPTION
		iterates through a_statement(starting from a_nextPos place) and parses each string, operator, or number. Parsing is based on
		the space in the statement or specific characters in the statement. Also, this function determines the type of the parsed
		element and passed by reference through the appropriate variable.
RETURNS
		an integer that determines the next position from where the parsing should start from. Also, passed by reference is the 
		value of element that was parsed. it is in a_stringVlaue if it is a string or an operator and in numValue if it has a number value
AUTHOR
		Prabal Chhatkuli
DATE
		12:30am 4/23/2019
*/
/**/
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
}/*int DuckInterpreter::ParseNextElement(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue)*/

/**/
/*
DuckInterpreter::ExecuteStatement()
NAME
		DuckInterpreter::ExecuteStatement -  Executes the statement at the specified location.  Returns the location of the next statement to be executed.
SYNOPSIS
		int DuckInterpreter::ExecuteStatement(string a_statement, int a_nextStatement);
			a_statement             --> the statement to be executed
			a_nextStatement             --> postion of the statement in the record
DESCRIPTION
		Gets the statement type of the function. Then based on the type of function calls functions and executes statements using the 
		function.
RETURNS
		Returns the location of the next statement to be executed. It is an integer value.
AUTHOR
		Prabal Chhatkuli
DATE
		4:30pm 4/20/2019
*/
/**/
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
}/*int DuckInterpreter::ExecuteStatement(string a_statement, int a_nextStatement);*/

/**/
/*
DuckInterpreter::EvaluateGotoStatement()
NAME
		DuckInterpreter::EvaluateGotoStatement - finds the location of the label of a goto statement 
SYNOPSIS
		int DuckInterpreter::EvaluateGotoStatement(string a_statement);
			a_statement             --> the statement to find the label from
DESCRIPTION
		Parses the elements and finds the name of label and find the line that the label is located in by comparing to 
		the map of labels, from the record.
RETURNS
		Returns the location of the label in the goto. Integer, to locate the line to go to.
AUTHOR
		Prabal Chhatkuli
DATE
		10:30pm 4/26/2019
*/
/**/
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
}/*int DuckInterpreter::EvaluateGotoStatement(string a_statement);*/

/**/
/*
DuckInterpreter::EvaluateReadStatement()
NAME
		DuckInterpreter::EvaluateReadStatement - evaluates a statement of read type
SYNOPSIS
		void DuckInterpreter::EvaluateReadStatement(const string &a_statement);
			a_statement             --> the statement to execute the read operation from
DESCRIPTION
		takes a statement and parses through it to display the input prompt in between two double quotes and 
		takes input from user based on the variables in between commas and inserts them in our map of symbol
RETURNS
		void
AUTHOR
		Prabal Chhatkuli
DATE
		7:30pm 5/6/2019
*/
/**/
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


/**/
/*
DuckInterpreter::EvaluateArithmentStatment()
NAME
		DuckInterpreter::EvaluateArithmentStatment - finds value of an arithmetic expression and assigns it to a variable
SYNOPSIS
		void DuckInterpreter::EvaluateArithmentStatment(const string &a_statement);
			a_statement             --> the statement to execute the arithmetic statement
DESCRIPTION
		takes a statement and parses through it to find the variable to place the final results in and takes the position of 
		next statement after the assignment operator and passes it to evaluateArithmeticExpression to find the value and assign it to
		a variable in the symbol table map.
RETURNS
		void
AUTHOR
		Prabal Chhatkuli
DATE
		11:45pm 5/6/2019
*/
/**/
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

}/*void DuckInterpreter::EvaluateArithmentStatment(const string &a_statement);*/

/**/
/*
DuckInterpreter::EvaluateIfStatement()
NAME
		DuckInterpreter::EvaluateIfStatement - Evaluates an if statement to determine if the goto should be executed.
SYNOPSIS
		int DuckInterpreter::EvaluateIfStatement(string a_statement, int a_nextStatement);
			a_statement             --> contains an expression of type integer
			a_nextStatement             -->contains integer value of next instruction to be executed
DESCRIPTION
		parses through a_statement and determines the arithmetic value of the string of arithmetic expressions after
		"if" word, Also, finds the goto statement following the if clause by searching the string, and replaces goto statement by ";"
		Then finds the label location of the goto label
RETURNS
		integer value of the next statement to be executed, it will be the next_statement if the "if" clause
		evaluates as 0, otherwise it will return the label location of the goto label.
AUTHOR
		Prabal Chhatkuli
DATE
		1:15am 4/29/2019
*/
/**/
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
}/*int DuckInterpreter::EvaluateIfStatement(string a_statement, int a_nextStatement);*/


/**/
/*
DuckInterpreter::EvaluatePrintStatement()
NAME
		DuckInterpreter::EvaluatePrintStatement - finds the specific string to print on to the screen
SYNOPSIS
		void DuckInterpreter::EvaluatePrintStatement(string a_statement);
			a_statement             --> contains an expression of type print
DESCRIPTION
		parses through the statement to find strings between the double quotes and finds the value of variables using the
		symbol table to display, which is determines by parsing words after commas
RETURNS
		void
AUTHOR
		Prabal Chhatkuli
DATE
		9:45pm 5/5/2019
*/
/**/
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
}/*void DuckInterpreter::EvaluatePrintStatement(string a_statement)*/
