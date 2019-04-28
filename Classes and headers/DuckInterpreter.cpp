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
		return a_nextStatement + 1;

	case StatementType::IfStat:

		return EvaluateIfStatement(a_statement, a_nextStatement);
	case StatementType::StopStat:
	case StatementType::EndStat:
	case StatementType::PrintStat:
	case StatementType::ReadStat:
		//EvaluateReadStatement(a_statement);
		//return a_nextStatement + 1;

	case StatementType::gotoStat:

	default:
		cerr << "BUGBUG - program terminate: invalid return value from GetStatementStype for the statement: " << a_statement << endl;
		exit(1);
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
	assert(assigmentOp != "=");

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