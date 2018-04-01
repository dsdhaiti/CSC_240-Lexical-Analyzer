/* 

Filename: Lexical_Analyzer .cpp
CSC 340 - Programming Languages
David D'Haiti 

<ifstmt>-> if (<boolexpr>) '{'<assign>'}' [else '{'<assign>'}']
<boolexpr>-> <boolterm> {|| <boolterm>}
<boolterm>-> <boolfactor> {&& <boolfactor>}
<boolfactor>-> TRUE | FALSE | ! <boolfactor> | (<boolexpr>)
<assign>-> id = <expr>; | id = <expr>; <assign>
<expr>-> <term> {( +|-) <term>}
<term>-> <factor> {( *|/|% ) <factor>}
<factor>-> id | int_constant | (<expr>)

PURPOSE:
This program will take a txt file of c++ syntax and parse through it and find all error based on the EBNF rule above. 

*/

#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

/* Global declarations */
/* Variables */

int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
int errCount = 0;
ifstream in_fp("Syntax.txt");
/* Function declarations */
void getChar();
void addChar();
void getNonBlank();
int lex();      // to get the nextToken
void expr();
void term();
void factor();
void assign();
void boolExpr();
void boolTerm();
void boolFactor();
void ifstmt();


/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define MOD_OP 27
#define ASGN_OP 28
#define SEMICOLON 29
#define IF_TOKEN 30
#define ELSE_TOKEN 31
#define TRUE 32
#define FALSE 33
#define NOT 34
#define AND_OP 35
#define OR_OP 36
#define LEFT_BRACK 37
#define RIGHT_BRACK 38



/* main driver */
void main()
{
	/* Open the input data file and process its contents */

	if (in_fp.fail())
	{
		cout << "File could not be opened\n";
		cin.get();
		exit(1);
	}
	else {
		getChar();
		do {
			lex();  // Getting the nextToken
			ifstmt();
		} while (nextToken != EOF);

	}


	cout << "*************** There are " << errCount << " ERRORS***************************" << endl;
	in_fp.close();

	system("PAUSE");
}
////////////////////////////////////////////////////////////////////////
/* lookup - a function to lookup operators and parentheses
and return the token */
int lookup(char ch) {
	switch (ch) {
	case '(':
		addChar();
		nextToken = LEFT_PAREN;
		break;
	case ')':
		addChar();
		nextToken = RIGHT_PAREN;
		break;
	case '+':
		addChar();
		nextToken = ADD_OP;
		break;
	case '-':
		addChar();
		nextToken = SUB_OP;
		break;
	case '*':
		addChar();
		nextToken = MULT_OP;
		break;
	case '/':
		addChar();
		nextToken = DIV_OP;
		break;
	case '%':
		addChar();
		nextToken = MOD_OP;
		break;
	case '=':
		addChar();
		nextToken = ASGN_OP;
		break;
	case ';':
		addChar();
		nextToken = SEMICOLON;
		break;
	case '!':
		addChar();
		nextToken = NOT;
		break;
	case '{':
		addChar();
		nextToken = LEFT_BRACK;
		break;
	case '}':
		addChar();
		nextToken = RIGHT_BRACK;
		break;
	default:
		addChar();
		nextToken = EOF;
		break;
	}
	return nextToken;
}

void addChar() {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	}
	else
		cout << " Error - lexeme is too long \n";
}

/* getChar - a function to get the next character of
input and determine its character class */
void getChar() {
	in_fp.get(nextChar);
	if (in_fp.eof())   // if no more character in the file
		nextChar = EOF;

	if (nextChar != EOF) {
		if (isalpha(nextChar))
			charClass = LETTER;
		else if (isdigit(nextChar))
			charClass = DIGIT;
		else charClass = UNKNOWN;
	}
	else
		charClass = EOF;
}

/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void getNonBlank() {
	while (isspace(nextChar))
		getChar();
}

/* lex - a simple lexical analyzer for arithmetic
expressions */
int lex() {
	lexLen = 0;
	getNonBlank();
	switch (charClass) {
		/* Parse identifiers */
	case LETTER:
		addChar();
		getChar();
		while (charClass == LETTER || charClass == DIGIT) {
			addChar();
			getChar();
		}

		nextToken = IDENT;

		if (lexeme[0] == 'i' && lexeme[1] == 'f' && lexeme[2] == 0) {
			nextToken = IF_TOKEN;
		}
		else if (lexeme[0] == 'e' && lexeme[1] == 'l' && lexeme[2] == 's' && lexeme[3] == 'e' && lexeme[4] == 0) {
			nextToken = ELSE_TOKEN;
		}
		else if (lexeme[0] == 'T' && lexeme[1] == 'R' && lexeme[2] == 'U' && lexeme[3] == 'E' && lexeme[4] == 0) {
			nextToken = TRUE;
		}
		else if (lexeme[0] == 'F' && lexeme[1] == 'A' && lexeme[2] == 'L' && lexeme[3] == 'S' && lexeme[4] == 'E' && lexeme[5] == 0) {
			nextToken = FALSE;
		}

		break;
		/* Parse integer literals */
	case DIGIT:
		addChar();
		getChar();
		while (charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = INT_LIT;
		break;
		/* Parentheses and operators */

	case UNKNOWN:
		lookup(nextChar);
		getChar();

		if (nextToken == EOF)
		{	addChar();
			getChar();  // need to read 2 characters for && and ||
			
			if (lexeme[0] == '&' && lexeme[1] == '&' && lexeme[2] == 0)
			{
				nextToken = AND_OP;
				
			}
			else if (lexeme[0] == '|' && lexeme[1] == '|' && lexeme[2] == 0)
			{
				nextToken = OR_OP;
			}
		}

		break;




		break;

		/* EOF */
	case EOF:
		nextToken = EOF;
		lexeme[0] = 'E';
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = 0;
		break;
	} /* End of switch */
	cout << "Next token is: " << nextToken
		<< "       Next lexeme is " << lexeme << "\n";
	return nextToken;
} /* End of function lex */


////////////////////////////////////////////////////////////////////////
  /* Function expr
  Parses strings in the language
  generated by the rule:
  <expr> → <term> {(+ | -) <term>}
  */
void expr()
{
	term();

	while (nextToken == ADD_OP || nextToken == SUB_OP)
	{
		lex(); // to get the nextToken
		term();
	}
}

/* term
Parses strings in the language generated by
the rule:
<term> -> <factor> {(* | /) <factor>}
*/
void term() {
	factor();
	
	while (nextToken == MULT_OP || nextToken == DIV_OP || nextToken == MOD_OP)
	{
		lex();
		factor();
	}
} 

/* Factor 
Parses strings in the language generated by
the rule:
<term> -> <factor> {(* | /) <factor>}
*/
void factor()
{
	if (nextToken == IDENT || nextToken == INT_LIT)
	{
		lex();
	}
	else if (nextToken == LEFT_PAREN)
	{
		lex();
		expr();
		if (nextToken == RIGHT_PAREN)
			lex();
		else
			cout << "Error: Right Paren Missing" << endl;
	} 

	else
	{
		cout << "Something went wrong" << endl; 

	}
}

/* assign
Parses strings in the language generated by
the rule:
<term> -> <factor> {(* | /) <factor>}
*/
void assign()
{
	if (nextToken == IDENT)
	{
		lex();
		if (nextToken == ASGN_OP)
		{
			lex();
			expr();
			if (nextToken == SEMICOLON)
			{
				lex();
				assign();
			}
			else {
				cout << "****Error: Missing Semicolon****" << endl;
				errCount++;
			}
		}
		else
		{
			cout << " ****Error: Assignment Missing ****" << endl;
			errCount++;
		}

	}
	


}

///////////////////////////////////////////////////////////////////////

/* boolFactor
Parses strings in the language generated by
the rule:
<boolfactor>->TRUE | FALSE | !<boolfactor> | (<boolexpr>)
*/
void boolFactor()
{

	if (nextToken == TRUE || nextToken == FALSE )
	{
		lex();
		
	}
	else if (nextToken == NOT)
	{
		lex();
		boolFactor();
	}
	else if (nextToken == LEFT_PAREN)
	{
		lex();
		boolExpr();
		
		if (nextToken == RIGHT_PAREN)
		{
			lex();
		}
		else
		{
			cout << "Error: Missing Right Paranthese" << endl;
			errCount++;
		}
	}
	else
	{
		cout << "****Error: No proper start ****" << endl;
		errCount++;
	}
	
	
}

/* boolTerm
Parses strings in the language generated by
the rule:
<boolterm>-> <boolfactor> {&& <boolfactor>}*/
void boolTerm()
{
	boolFactor();
	while (nextToken == AND_OP) {
		lex();
		boolTerm();
	}
}

/* boolExpr
Parses strings in the language generated by
the rule:
<boolexpr>-> <boolterm> { || <boolterm>}
*/
void boolExpr()
{
	boolTerm();
	while (nextToken == OR_OP) {
		lex();
		boolTerm();
	}
}

/* ifstmt
Parses strings in the language generated by
the rule:
<ifstmt>-> if (<boolexpr>) '{'<assign>'}'[else '{'<assign>'}']
*/
void ifstmt()
{
	if (nextToken == IF_TOKEN) 
	{
		lex();
		if (nextToken == LEFT_PAREN) 
		{
			lex();
			boolExpr();
			if (nextToken == RIGHT_PAREN)
			{
				lex();
				if (nextToken == LEFT_BRACK)
				{
					lex();
					assign();
					if (nextToken == RIGHT_BRACK)
					{
						lex();
						if (nextToken == ELSE_TOKEN)
						{
							lex();
							if (nextToken == LEFT_BRACK) {
								lex();
								assign();
								if (nextToken == RIGHT_BRACK) {
									lex();
								}
								else
								{
									cout << "****Error:  Expected RIGHT_BRACK****" << endl;
									errCount++;
								}
							}
							else
							{
								cout << "****Error:  Expected LEFT_BRACK****" << endl;
								errCount++;
							}
						}
					}
					else
					{
						cout << "****Error:  Expected RIGHT_BRACK****" << endl;
						errCount++;
					}
				}
				else
				{
					cout << "****Error:  Expected LEFT_BRACK****" << endl;
					errCount++;
				}
			}
			else
			{
				cout << "****Error:  Expected RIGHT_PAREN****" << endl;
				errCount++;
			}
		}
		else 
		{
			cout << "****Error:Expected LEFT_PAREN****" << endl;
			errCount++;
		}
	}
	else
	{
		cout << "****Error: Expected IF_STMNT****" << endl;
		errCount++;
	}
}


