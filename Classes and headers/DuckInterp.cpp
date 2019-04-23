// DuckInterp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include"DuckInterpreter.h"


int main( int argc, char *argv[] )
{
	if (argc != 2)
	{
		cerr << "Usage: DuckInterp <filename>" << endl;
		return 1;
	}
	// Create the interpreter object and use it to record the statements and 
	// execute them.
	DuckInterpreter duckInt;  //this class will hold all the elements

	duckInt.RecordStatements( argv[1]);//the commandline has the statement file
	duckInt.RunInterpreter();

    return 0;
}

