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

void DuckInterpreter::RunInterpreter()
{
	int nextStatement = 0;

	while (true)
	{
		string statement = m_statements.GetStatement(nextStatement);

		nextStatement = ExecuteStatement(statement, nextStatement);
	}
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
		/*checkin if value exists in symbol table
		double value;
		bool isexists;
		isexists = m_symbolTable.GetVariableValue("m", value);
		if (isexists)
		{
			cout << "value set in the symbol table for k is " << value << endl;
		}
		else
		{
			cout << "value does not exits in symbol table" << endl;
		}
		*/
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

	default:
		cerr << "BUGBUG - program terminate: invalid return value from GetStatementStype for the statement: " << a_statement << endl;
		exit(1);
	}
}

void DuckInterpreter::EvaluateReadStatement(const string &a_statement)
{
	/*
	int nextPos = 0;
	string resultVariable;
	double placeHolder;
	nextPos = ParseNextElement(a_statement, nextPos, resultVariable, placeHolder);
	cout << "here the value of resultVariable is" << resultVariable << endl;
	assert(!resultVariable.empty());
	*/
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
	cout << "here the value of resultVariable is"<<resultVariable << endl;
	assert(!resultVariable.empty());

	string assigmentOp;
	nextPos = ParseNextElement(a_statement, nextPos, assigmentOp, placeHolder);
	cout << "here the value of assignment operator is " << assigmentOp << endl;
	assert(assigmentOp == "=");//correction made here

	double result = EvaluateArithmenticExpression(a_statement, nextPos);
	cout << "here the result of the arithemetic evaluation is::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
	cout << result <<endl;
	cout << ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;

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
		cout << "goto found at:" << foundLabel << endl;
		labelPosition = foundLabel + 4;
		//find the label
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
		cout << "::::::::::::::::::::::::::::::::" << endl;
		cout << label << "oo" << endl;
		cout << ":::::::::::::::::::::::::::::::::" << endl;
	}
	else
	{
		cerr << "bugbug : goto statement not found in if clause" << endl;
		exit(1);
	}
	//replace everything from(goto to end of string) with ;
	a_statement.replace(a_statement.begin() + foundLabel, a_statement.end(), ";");
	//----------------------------------------------------------------------------------
	int labelLocation = m_statements.GetLabelLocation(label);

	// Verify that the label from the goto exists.

	// Evaluate the emaining arithmentic expression. 
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

			else if (resultString == ","&& firstPrint==true)
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
				//disregarding the comma that comes right after
				resultString.clear();
				if (a_statement[nextPos] == ';')
				{
					break;
				}
				nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
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
}//end of function
