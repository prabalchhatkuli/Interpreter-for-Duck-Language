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
	StatementType GetStatementStype(const string &a_string) { return StatementType::gotoStat; }

	enum class ElementType
	{
		StringType,
		NumberType,
	};

	// Executes the statement at the specified location.  Returns the location of the next statement to be executed.
	int ExecuteStatement(string a_statement, int a_StatementLoc);

	// Returns the next element in the statement.  Returns the next location to be accessed.
	int ParseNextElement(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue) { return 1;}

	// Returns the precedence of an operator.
	int FindPrecedence(string op) { return 1; }

	// Evaluate an arithmetic statement.
	void EvaluateArithmentStatment( const string &a_statement );

	// Evaluate an arithmetic expression.  Return the value.  The variable a_nextPos is index to the next  
	double EvaluateArithmenticExpression(const string &a_statement, int a_nextPos) { return 1; }

	int EvaluateIfStatement(string a_statement, int a_nextStatement);

};

