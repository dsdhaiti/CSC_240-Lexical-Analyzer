/*

Filename: LexicalAnalyzer.cpp

CSC 340 - Programming Languages

@Author David D'Haiti

A lexical analyzer system for simple arithmetic expressions.



///////rules that must be followed //////////////

<ifstmt> -> if (<boolexpr>) '{'<assign>'}' [else '{'<assign>'}']
<boolexpr> -> <boolterm> {|| <boolterm>}
<boolterm> -> <boolfactor> { && <boolfactor> }
<boolfactor> -> TRUE | FALSE | ! <boolfactor> | (<boolexpr>)
<assign> -> id = <expr>; | id = <expr>; <assign>
<expr> -> <term> { ( + | - ) <term>}
<term> -> <factor> {( *|/|% ) <factor>}
<factor>-> id | int_constant | (<expr>)


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
ifstream in_fp("syntax.txt");



/* Function declarations */
void getChar();
void addChar();
void getNonBlank();
int lex();      // to get the nextToken
void expr();
void assign();
void term();
void factor();



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
#define IF_STMT  31
#define ELSE_STMT 32
#define TRUE 32
#define FALSE 33

#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define MOD_OP 27
#define ASSIGN_OP 28
#define AND_OP 30


#define SEMICOLON 29
#define LEFT_PAREN 25
#define RIGHT_PAREN 26




/******************************************************/

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

		lex();  // Getting the nextToken

		do {

			assign();

			lex();

		} while (nextToken != EOF);

	}

	in_fp.close();



	system("PAUSE");

}



/*****************************************************/

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

		nextToken = ASSIGN_OP;

		break;

	case ';':

		addChar();

		nextToken = SEMICOLON;

		break;

	default:

		addChar();

		nextToken = EOF;

		break;

	}

	return nextToken;

}

/*****************************************************/

/* addChar - a function to add nextChar to lexeme */

void addChar() {

	if (lexLen <= 98) {

		lexeme[lexLen++] = nextChar;

		lexeme[lexLen] = 0;

	}

	else
		cout <<  " Error - lexeme is too long \n";

}

/*****************************************************/

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

/*****************************************************/

/* getNonBlank - a function to call getChar until it

returns a non-whitespace character */

void getNonBlank() {

	while (isspace(nextChar))

		getChar();

}

/***************************************************** /

/* lex - a simple lexical analyzer for arithmetic

expressions */

int lex() {

	lexLen = 0;

	getNonBlank();

	switch (charClass) {

		/* Parse identifiers */

	case LETTER:

		// bring in the whole word first 
		while (charClass == LETTER) {

			addChar();

			getChar();

		}

		if (lexeme[0] == 'i' && lexeme[1] == 'f' )
		{
			
			// defines this postestion it is at to be an if statment 
			nextToken = IF_STMT;
			
			// runs the iff statement function 
			ifstmt();
		}

		nextToken = IDENT;
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

		// add the character to lexeme
		// then use lexeme to do the comparasions here
		// Errors: what type does data does lexeme bring in?
		addChar();
			//////// && if statement goes somewhere around here
			if ((lexeme[0] == '&') && (lexeme[1] == '&')  )
			  {
				  // run the and progrgram
				  nextToken = AND_OP;
			  }
		// will look to see if it knows the character comiing in
		// if does not it return end of file
		// must run at the end of the file in an else statment
			else
			{
				lookup(nextChar);
			}

		getChar();


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

	cout << "Next token is: " << nextToken << "       Next lexeme is " << lexeme << "\n";
	
	return nextToken;

} /* End of function lex */



/*

<assign> = id = <expr>;  |  id = <expr> ;

*/

void assign()

{

	if (nextToken == IDENT)

	{

		lex();



		if (nextToken == ASSIGN_OP)

		{

			lex();

			expr();



			if (nextToken != SEMICOLON)

			{

				cout << "*** ERROR | Missing semi-colon" << endl;

			}

		}
		else
		{

			cout << "*** ERROR| Missing an assignment  ***" << endl;

		}

	}

	else

	{

		cout << "*** ERROR| Must be an ID  ***" << endl;

	}

}





/* Function expr

Parses strings in the language

generated by the rule:

<expr> ->  <term> { (+ | -) <term>}

*/

void expr()

{

	/* Parse the first term */

	term();

	/* As long as the next token is + or -, call

	lex to get the next token and parse the

	next term */

	while (nextToken == ADD_OP ||

		nextToken == SUB_OP) {

		lex(); // to get the nextToken

		term();

	}

}



/* function term

Parses strings in the language generated by the rule:

<term> -> <factor> {   (  * | / ) <factor>}



*/

void term() {

	/* Parse the first factor */

	factor();

	/* As long as the next token is * or /,

	next token and parse the next factor */

	while (nextToken == MULT_OP ||

		nextToken == DIV_OP ||

		nextToken == MOD_OP

		)

	{

		lex();

		factor();

	}

} /* End of function term */





/* Function factor

Parses strings in the language generated by the rule:



<factor> -> id | int_constant | ( <expr>)



*/

void factor()

{

	/* Determine which RHS */

	if (nextToken == IDENT || nextToken == INT_LIT)

		/* For the RHS id or int, just call lex */

		lex();



	/* If the RHS is (<expr>) – call lex to pass over

	the left parenthesis, call expr, and check for

	the right parenthesis */

	else if (nextToken == LEFT_PAREN) {



		lex();

		expr();



		if (nextToken == RIGHT_PAREN)

		{

			lex();

		}

		else

		{

			cout << "Error #1 " << endl;

		}      // error statement



	} /* End of else if (nextToken == ... */

	else

	{

		cout << "Error #2 " << endl; /* Neither RHS matches */

	}
	TRUE
}



/******************************* Bool functions***************************/



// Function boolTerm

/*Parses strings in the language generated by the rule:



<boolterm> -> <boolterm> && <boolfactor> | <boolfactor>

*/

void boolTerm()

{



	boolFactor();



	while (nextToken == AND_OP)

	{

		lex();

		boolFactor();

	}

}



//Function boolExpr

/* Parses strings in the language generated by the rule:



<boolexpr> -> <boolexpr> || <boolterm> | <boolterm>

*/

void boolExpr()

{



}



//Function boolFactor

/* Parses strings in the language generated by the rule:



<boolfactor> -> TRUE | FALSE | ! <boolfactor> | (<boolexpr>)

*/
void boolFactor() 
{

}



//Function ifstmt

/*Parses strings in the language generated by the rule:


<ifstmt> -> if (<boolexpr>) '{'<assign>'}' [else '{'<assign>'}']

*/

void ifstmt() 
{
	cout << "im at the if statment: STARTED " << endl; 
	//if(nextToken == ifstmt)

}