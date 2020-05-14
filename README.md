# Interpreter-for-Duck-Language
a Language Interpreter for a fictional Duck Language


# The Duck Language
The duck language is a simple C-like language that will provide the the ability to write simple programs.  The following is a list of language elements:

#Variables:
A variable consists of a letter or "_" followed by zero or more letters, digits, and "_".  All variables are assumed to hold doubles.

An array may be created by typing: dim <array name>[<array size>];

An attempt to use a variable that does not have a value will cause an error to be reported and the program to terminate.

Variable names are case sensitive.

#Statements
A statement is terminated by a semicolon.
Lines may be labeled by a name (anything that looks like a variable) followed by a colon.
Duck supports: arithmetic expressions, goto (this is an evil duck), read, print, if, stop, and end statements.  Details to be discussed later.
All keeps words must be lower case.
Arithmetic Statement
An arithmetic statement will look just the way it does in C++.  Operators are =,  +, - (unary and binary), *, /.  Parentheses are supported, comparisons support (return value is 0 if false), logical operators supported are ==, !=, ! (not), <, <=, >=  (Note: 0==false, != 0 == true)  The only logical operators required are < and >

#If Statement
The format of an if statement is:

if( <Logical/Arithmetic Expression> ) <goto statement>;

If the <arithmetic expression> != 0, the statement is execute.

#read statement
The read statement has the format:

read ["<prompt>"] v1, v2, v3, ....;

Where v1, v2, v3, ... are variable.  The the user  will be prompted with the the option prompt and then will enter numbers to be recorded in the variables.

#print statement
The print statement has the format:

print x1, x2, x3, ...;

Where the x's may be variable names or quoted strings. 

#The stop statement
When stop is executed the program will terminate 

#End statement
This statement indicates the end of the program.  That is that there are no more lines of program that follows.

#Comments
Anything that occurs after a "//" is considered a comment.

#Error processing
Most errors will be reported when the statement that contains them are executed.  An error will terminate the program.
