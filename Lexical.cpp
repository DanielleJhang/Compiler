/* ================================================================================================================================================================================================================================================================================================ */
/* ================================================================================================================================================================================================================================================================================================ */
/* ================================================================================================================================================================================================================================================================================================ */
/*
	[CSC 446] Construct Compiler

	Assignment # 1 : Lexical analyzer

	Student ID 7354802
	Ji eun Jhang
*/
/* ================================================================================================================================================================================================================================================================================================ */
/* ================================================================================================================================================================================================================================================================================================ */
/* ================================================================================================================================================================================================================================================================================================ */




#include "Lexical.h"


using namespace std;


char *resword[RESNUM] = { "class", "public", "static", "void", "main",							//string array of reserved words that paired with enumerated one
																								/*  0		 1        2			3		  4    */
	"String", "extends", "return", "int", "boolean",
	/*  5        6			 7       8        9    */
	"if", "else", "while", "System.out.println", "length",
	/* 10   11       12			 13			    14    */
	"true", "false", "this", "new", "unknown" };
/* 15		16	    17	   18		19*/



char *token_t[RESNUM] = { "idt", "numt", "literalt", "relopt", "addopt",						//string array of other tokens that paired with enumerated one
																								/* 20     21         22        23			24 */
	"mulopt", "assignopt", "sm_op_brkt", "sm_cl_brkt", "sq_op_brkt",
	/* 25		  26           27             28		     29  */
	"sq_cl_brkt", "cr_op_brkt", "cr_cl_brk", "quot", "eoft",
	/*   30             31         32	       33		34 */
	"dbrelopt", "semicolt", "punct", "commt", "dbquot" };
/*    35		 36     37		38		39 */

char *need_RDP[5] = { "final", "exclt", "read", "write", "writeln"								// From exclt elements are devided two : former=>resword type : later=>token_t type : caution for recog and print

};


char* LEXEME[1000];				// LEXEME ARRAY THAT STORE ALL THE CHAR THING
int VALUE[1000];				// ARRAY OF INTEGER VALUE
float VALUER[1000];				// ARRAY OF REAL VALUE
int LINE[1000];					// It needed for RDPO
std::string LITERAL;			// STRING THAT STORE THE CONTENTS OF STRING EACH TIME WE READ IT
int TOKEN[1000];				// STORE THE TOKEN NUMBER TO MATCHING WITH 2 TOKEN TABLES : I will fix it to allocate it later.
int tonum = 1;			        // Count all the token
int blank = 0;					// Count blank


void Initialization()								// This function initialize all the global array that we use as null value : for safety
{
	int i;
	for (i = 0; i < 1000; i++)						// Later, I will change all the array thing to another data structur that has more efficiency
	{
		LEXEME[i] = NULL;
		VALUE[i] = 0;
		VALUER[i] = 0.0;
		TOKEN[i] = 0;
	}
}

void lex_err(int n)										// This function print out the error msg when error occured
{
	switch (n)
	{

	case 1:												// Over MAXNUM length idt

		cout <<endl << endl << "Error : Identfier cannot be longer than 31 letters" << endl << endl;

		break;

	case 2:												// String that not ended in same line

		cout << endl << endl << "Error : String should be ended in same line with double quote" << endl << endl;

		break;

	default:
		break;
	}
};

int GetNextToken(int t, char* word)						// THIS FUNCTION DETERMINE AND ASSIGN ALL THE PROPER TOKEN VALUE TO EACH VALUE AND STORE THEM IN TOKEN ARRAY IN ORDER
{
	int i = 0;
				
	char *allocated_memory;											 // **** 2.18
	
														// 0=idt 1=digit 3=realv 4=relop 5=operator 6=symbol
	switch (t)
	{
		// ================================================================================================================== 

	case 0:												//(reserved || idt)

		for (i = 0; i < RESNUM; i++)
		{
			if (strcmp(word, resword[i]) == 0)						// if our read value is a reserved token
			{
				TOKEN[tonum] = i;

				allocated_memory = (char *)malloc(strlen(word) + 1);		 // **** 2.18  : +1 for NULL
				strcpy(allocated_memory, word);                              // **** 2.18 
				LEXEME[tonum] = allocated_memory;                            // **** 2.18 

				return i;
			}
		}

		if (strcmp(word, need_RDP[0]) == 0)						// if our read value is a true or false
		{
			TOKEN[tonum] = finalt;

			allocated_memory = (char *)malloc(strlen(word) + 1);		 // **** 2.18  : +1 for NULL
			strcpy(allocated_memory, word);                              // **** 2.18 
			LEXEME[tonum] = allocated_memory;                            // **** 2.18 

			return finalt;
		}

		else if (strcmp(word, need_RDP[2]) == 0)						// if our read value is a true or false
		{
			TOKEN[tonum] = readt;

			allocated_memory = (char *)malloc(strlen(word) + 1);		 // **** 2.18  : +1 for NULL
			strcpy(allocated_memory, word);                              // **** 2.18 
			LEXEME[tonum] = allocated_memory;                            // **** 2.18 

			return readt;
		}

		else if (strcmp(word, need_RDP[3]) == 0)						// if our read value is a true or false
		{
			TOKEN[tonum] = writet;

			allocated_memory = (char *)malloc(strlen(word) + 1);		 // **** 2.18  : +1 for NULL
			strcpy(allocated_memory, word);                              // **** 2.18 
			LEXEME[tonum] = allocated_memory;                            // **** 2.18 

			return writet;
		}

		else if (strcmp(word, need_RDP[4]) == 0)						// if our read value is a true or false
		{
			TOKEN[tonum] = writelnt;

			allocated_memory = (char *)malloc(strlen(word) + 1);		 // **** 2.18  : +1 for NULL
			strcpy(allocated_memory, word);                              // **** 2.18 
			LEXEME[tonum] = allocated_memory;                            // **** 2.18 

			return writelnt;
		}
		
		TOKEN[tonum] = idt;											// else, those are idt

		allocated_memory = (char *)malloc(strlen(word) + 1);		 // **** 2.18  : +1 for NULL
		strcpy(allocated_memory, word);                              // **** 2.18 
		LEXEME[tonum] = allocated_memory;                            // **** 2.18 

		return idt;

		break;

		// ================================================================================================================== // 1 : digits : digit digit*

	case 1:												

		TOKEN[tonum] = numt;

		allocated_memory = (char *)malloc(strlen(word) + 1);		 // **** 2.18  : +1 for NULL
		strcpy(allocated_memory, word);                              // **** 2.18 
		LEXEME[tonum] = allocated_memory;                            // **** 2.18 

		VALUE[tonum] = atoi(word);						// Convert read char value as integer value and store them in value array in tonum'th order

		return numt;									// Later if I have to distunguish integer and float :  Integer type

		break;

		// ================================================================================================================== // 2 : num : digits optional_fraction


	case 2:											          // ===================================== ?

		TOKEN[tonum] = numt;

		allocated_memory = (char *)malloc(strlen(word) + 1);		 // **** 2.18  : +1 for NULL
		strcpy(allocated_memory, word);                              // **** 2.18 
		LEXEME[tonum] = allocated_memory;                            // **** 2.18 

		VALUER[tonum] = atof(word);						// Convert read char value as float value and store them in value array in tonum'th order

		return numt;									// Later if I have to distunguish integer and float : Float type

		break;

		// ================================================================================================================== // 3 : optional fraction : .digit

	case 3:												       // =================================== ?

		TOKEN[tonum] = numt;

		allocated_memory = (char *)malloc(strlen(word) + 1);		 // **** 2.18  : +1 for NULL
		strcpy(allocated_memory, word);                              // **** 2.18 
		LEXEME[tonum] = allocated_memory;                            // **** 2.18 

		VALUER[tonum] = atof(word);						// Convert read char value as float value and store them in value array in tonum'th order

		return numt;

		break;

		// ================================================================================================================== // 4 : Dbrelop or assigne or single relop

	case 4:												

		allocated_memory = (char *) malloc( strlen(word) + 1 );      // **** 2.18  : +1 for NULL
		strcpy(allocated_memory, word);                              // **** 2.18 
		LEXEME[tonum] = allocated_memory;                            // **** 2.18 
		
		if (strcmp(word, "==") == 0 || strcmp(word, "<=") == 0 || strcmp(word, ">=") == 0 || strcmp(word, "!=") == 0)		//Distinguish double relop from relop labled value
		{
			TOKEN[tonum] = dbrelopt;		
			return dbrelopt;
		}

		else if (strcmp(word, "=") == 0)																					//Distinguish assignop from relop labled value
		{
			TOKEN[tonum] = assignopt;
			return assignopt;
		}

		else if (strcmp(word, "!") == 0)																					//Distinguish exclt from relop labled value
		{
			TOKEN[tonum] = exclt;
			return exclt;
		}

		else																												//Left values are single relop
		{
			TOKEN[tonum] = relopt;
			return relopt;
		}


		break;

		// ================================================================================================================== // 5 : Mulop addop

	case 5:												
		
		allocated_memory = (char *) malloc( strlen(word) + 1 );      // **** 2.18  : +1 for NULL
		strcpy(allocated_memory, word);                              // **** 2.18 
		LEXEME[tonum] = allocated_memory;                            // **** 2.18 

		if (strcmp(word, "+") == 0 || strcmp(word, "-") == 0 || strcmp(word, "||") == 0)			//Find addop
		{
			TOKEN[tonum] = addopt;
			return addopt;
		}

		else if (strcmp(word, "*") == 0 || strcmp(word, "&&") == 0 || strcmp(word, "/") == 0)		//Find mulop
		{
			TOKEN[tonum] = mulopt;
			return mulopt;
		}

		else if (strcmp(word, "&") == 0 || strcmp(word, "|") == 0 || strlen(word) > 2)

		{
			TOKEN[tonum] = unknownt;
			return unknownt;
		}

		else
		{
			TOKEN[tonum] = unknownt;
			return unknownt;
		}

		break;

		// ================================================================================================================== // 6 : Symbols : ( ) { } , ; " [ ] 

	case 6:												

		allocated_memory = (char *) malloc( strlen(word) + 1 );      // **** 2.18  : +1 for NULL
		strcpy(allocated_memory, word);                              // **** 2.18 
		LEXEME[tonum] = allocated_memory;                            // **** 2.18 

		if (*word == '(')
		{
			TOKEN[tonum] = sm_op_brkt;
			return sm_op_brkt;
		}

		else if (*word == ')')
		{
			TOKEN[tonum] = sm_cl_brkt;
			return sm_cl_brkt;
		}

		else if (*word == '[')
		{
			TOKEN[tonum] = sq_op_brkt;
			return sq_op_brkt;
		}

		else if (*word == ']')
		{
			TOKEN[tonum] = sq_cl_brkt;
			return sq_cl_brkt;
		}

		else if (*word == '{')
		{
			TOKEN[tonum] = cr_op_brkt;
			return cr_op_brkt;
		}

		else if (*word == '}')
		{
			TOKEN[tonum] = cr_cl_brkt;
			return cr_cl_brkt;
		}

		else if (*word == ';')
		{
			TOKEN[tonum] = semicolt;
			return semicolt;
		}

		else if (*word == ',')
		{
			TOKEN[tonum] = commt;
			return commt;
		}

		else if (*word == '.')
		{
			TOKEN[tonum] = punct;
			return punct;
		}

		break;

		// ================================================================================================================== // 7 : Literal

	case 7:													
		
		allocated_memory = (char *) malloc( strlen(word) + 1 );      // **** 2.18 : +1 for NULL
		strcpy(allocated_memory, word);                              // **** 2.18
		LEXEME[tonum] = allocated_memory;                            // **** 2.18

		TOKEN[tonum] = literalt;
		return literalt;
		break;

		// ================================================================================================================== // 8 : Unknown

	case 8:													// Unknown
		
		TOKEN[tonum] = unknownt;

		allocated_memory = (char *)malloc(strlen(word) + 1);		 // **** 2.18  : +1 for NULL
		strcpy(allocated_memory, word);                              // **** 2.18 
		LEXEME[tonum] = allocated_memory;                            // **** 2.18 

		return unknownt;
		break;

		// ================================================================================================================== // Our default token is unknown

	default:

		TOKEN[tonum] = unknownt;

		allocated_memory = (char *)malloc(strlen(word) + 1);		 // **** 2.18  : +1 for NULL
		strcpy(allocated_memory, word);                              // **** 2.18 
		LEXEME[tonum] = allocated_memory;                            // **** 2.18 

		return unknownt;
		break;

	}
}

int check_letter(char ch)							//Determine read character as letter
{
	if ((ch >= 'a') && (ch <= 'z'))
		return 1;
	else if ((ch >= 'A') && (ch <= 'Z'))
		return 1;
	else
		return 0;
}

int check_digit(char ch)							//Determine read character as digit
{
	if ((ch >= '0') && (ch <= '9'))
		return 1;
	else
		return 0;
}

int check_underscore(char ch)						//Determine read character as '_'
{
	if (ch == '_')
		return 1;
	else
		return 0;
}

int check_dot(char ch)								//Determine read character as '.'
{
	if (ch == '.')
		return 1;
	else
		return 0;
}

int check_relational(char ch)						//Determine read character as relational operator
{
	if ((ch == '=') || (ch == '!') || (ch == '<') || (ch == '>'))
		return 1;
	else
		return 0;
}

int check_operator(char ch)							//Determine read character as not relational operator
{
	if ((ch == '+') || (ch == '-') || (ch == '|') || (ch == '*') || (ch == '&'))
		return 1;
	else
		return 0;
}

int check_slash(char ch)							//Determine read character as '/'
{
	if (ch == '/')
		return 1;
	else
		return 0;
}

int check_star(char ch)								//Determine read character as '*'
{
	if (ch == '*')
		return 1;
	else
		return 0;
}

int check_symbol(char ch)							//Determine read character as all the brackets and ';', ','
{
	if ((ch == '(') || (ch == ')') || (ch == ',') || (ch == ';') || (ch == '[') || (ch == ']') || (ch == '{') || (ch == '}'))   // If we need to care about ':' later, put it in here
		return 1;
	else
		return 0;
}																																

int check_quote(char ch)							//Determine read character as double quotes : to find string
{
	if (ch == '"')
		return 1;
	else
		return 0;
}

void ProcessToken(ifstream& infile)
{
	int length = 0;															// To store words or sentences that combined from each ch to array : approximate the needed space length
	char ch = NULL;															// Single character that we read right at that moment											
	char token_string[MAXIDT + 1];											// Array that store combined character values : the reason that I assign 1 in array size is to store null flag to indicate the end of word
	int token_value;														// We assume the token value is integer
	int line_counter = 1;													// count the line number
	int digit_class = 1;													// 1 : digits, 2 : num 
	int comment_end = 0;													// In case of '/*'comment, till we meet '*/', we need to ignore comment contents | see the '/' handling part below


	if (infile.is_open())
	{
		cout << endl << endl << "================================================================================================" << endl;
		cout << endl << endl << "							These are tokens from your file" << endl;
		cout << endl << endl << "================================================================================================" << endl;
		cout << endl << endl << "  #  |             LEXEME             |         TOKEN         |   Line #   |  VALUE  |  VALUER  " << endl;
		cout << endl << endl << "================================================================================================" << endl;


                infile >> noskipws >> ch;    // ########## 2.8

		while (!infile.eof())            // ########### switch from while (infile >> noskipws >>) 
		{

			length = 0;																// Initialize the length of string that I will read 

			while (ch == ' ' || ch == '\t')											//  Get rid of blank and tab 
			{
				infile >> noskipws >> ch;
				if (infile.eof())
				{
					TOKEN[tonum] = eoft;
					break;
				}
			}

			// ================================================================================================================== New line

			if (ch == '\n')															// If we meet new line, increase line counter
			{
				line_counter++;
				infile >> noskipws >> ch;
			}


			// ================================================================================================================== System token and idt starts with 'S'

			else if (ch == 'S')
			{
				{
					token_string[length++] = ch;

					while (check_letter(ch) || check_digit(ch) || check_underscore(ch) || check_dot(ch))
					{
						infile >> noskipws >> ch;


						if (length <= MAXIDT)										// Check that if the length of letter is greater than MAXIDT
							token_string[length++] = ch;						
											
						if (infile.eof())
						{
							TOKEN[tonum] = eoft;
							break;
						}
					}

					if (length > MAXIDT)											// If the length of idt is greater than MAXNUM, I call the error msg
						lex_err(1);
					else
					{
						token_string[length - 1] = NULL;


						token_value = GetNextToken(0, token_string);                // 0 : Identifier token, first argument of GetNextToken() argument makes easier to handle idt
						

						PrintToken(line_counter, token_value, token_string);        // To count the line number when we print this out, PrintToken needs 3 arguments
					}
				}
			}

			// ================================================================================================================== letter

			else if (check_letter(ch))												// Identifier : letter(letter | digit | underscore)*              
			{
				token_string[length++] = ch;

				while (check_letter(ch) || check_digit(ch) || check_underscore(ch))
				{
					infile >> noskipws >> ch;

					if (length <= MAXIDT)											// Check that if the length of letter is greater than MAXIDT
						token_string[length++] = ch;
					
					if (infile.eof())
					{
						TOKEN[tonum] = eoft;
						break;
					}

				}

				if (length > MAXIDT)												// If the length of idt is greater than MAXNUM, I call the error msg
					lex_err(1);


				else
				{
					token_string[length - 1] = NULL;
					token_value = GetNextToken(0, token_string);					// 0 : Identifier token, first argument of GetNextToken() argument makes easier to handle idt

					PrintToken(line_counter, token_value, token_string);			// To count the line number when we print this out, PrintToken needs 3 arguments
				}
			}

			// ================================================================================================================== digit

			else if (check_digit(ch))											// Digit : digit digit*
			{
				digit_class = 1;												// To distinguish digits and num

				token_string[length++] = ch;
				infile >> noskipws >> ch;

				while (check_digit(ch) || check_dot(ch))
				{

					if (check_dot(ch))
					   {  
						  if (digit_class == 1) digit_class = 2;								 // optional_fractions   =====> 3.12 
					      else if (digit_class == 2) digit_class = 8;                            // unknown              =====> 3.12 
					   }

					if (length <= MAXIDT)										// I assume that the length of digits also not be greater than MAXIDT
						token_string[length++] = ch;
																				// If digit is longer than MAXIDT, I store them till MAXIDT
																				// We ignore digits longer than MAXIDT === If we have some change, we can define other constant like MAXFRACTION
					else if (infile.eof())
					{
						TOKEN[tonum] = eoft;
						break;
					}

					infile >> noskipws >> ch;

				}

				if (length > MAXIDT)											// If digit is longer than MAXIDT, I store them till MAXIDT
					token_string[MAXIDT] = NULL;
				else
					token_string[length] = NULL;

				
				if ((digit_class == 2) && (!check_digit(token_string[length -1]))) digit_class = 8;   // unknown              =====> 3.12 



				token_value = GetNextToken(digit_class, token_string);			// 1 : digits : digit digit*
																				// 2 : num : digits optional_fraction
				                                                                // 8 : unknown....

				PrintToken(line_counter, token_value, token_string);			// To count the line number when we print this out, PrintToken needs 3 arguments

			}																	// We can handle the things like 360.55.
																				// ==================================================================================================================

			else if (check_dot(ch))												// optional_fraction : . digits or symbol '.'
			{
				token_string[length++] = ch;

				infile >> noskipws >> ch;

				if (!check_digit(ch))
				{
					token_string[length] = NULL;


					token_value = GetNextToken(6, token_string);				// 6 : symbol  

					PrintToken(line_counter, token_value, token_string);        // To count the line number when we print this out, PrintToken needs 3 arguments
				}
				else if (ch != EOF)												// ********************************** correct
				{
					token_string[length++] = ch;

					while (check_digit(ch))
					{
						infile >> noskipws >> ch;

						if (length <= MAXIDT)									// I assume that the length digits in optional fraction also not be greater than MAXIDT
							token_string[length++] = ch;
																				// If digit is longer than MAXIDT, I store them till MAXIDT  
																				// We ignore digits longer than MAXIDT === If we have some change, we can define other constant like MAXFRACTION
					}

					if (length > MAXIDT)										// If digit is longer than MAXIDT, I store them till MAXIDT
						token_string[MAXIDT] = NULL;
					else
						token_string[length - 1] = NULL;


					token_value = GetNextToken(8, token_string);				// 8 : optional fraction : .digit      unknown              =====> 3.12 

					PrintToken(line_counter, token_value, token_string);        // To count the line number when we print this out, PrintToken needs 3 arguments
				}
			}

			// ================================================================================================================== relop

			else if (check_relational(ch))										// relational operators : ==, !=, <, <= ,>, >=
			{
				token_string[length++] = ch;

				while (check_relational(ch))
				{
					infile >> noskipws >> ch;

					if (length <= MAXIDT)										// I assume that the length of relation operator also not be greater than MAXIDT
						token_string[length++] = ch;
																				// If relop is longer than MAXIDT, I store them till MAXIDT  
					if (infile.eof())
					{
						TOKEN[tonum] = eoft;
						break;
					}

				}

			 	if (length > MAXIDT)											// If relop is longer than MAXIDT, I store them till MAXIDT
			 		token_string[MAXIDT] = NULL;
			 	else
					token_string[length-1] = NULL;          

				token_value = GetNextToken(4, token_string);					// 4 : relational operator => If '=' appear not '==', at GetNextToken I will distinguish it to assignment operator

				PrintToken(line_counter, token_value, token_string);			// To count the line number when we print this out, PrintToken needs 3 arguments

			}

			// ================================================================================================================== operator

			else if (check_operator(ch))										// Operator : to distinguish '/', '//' and '/*', in this operator I except '/', and handle it in next else if
																				// Handle those '+', '-', '||', '*', '&&' 
			{
				token_string[length++] = ch;

				// debug from here 2.12

				if (ch == '|')
				{
					infile >> noskipws >> ch;

					while (check_operator(ch) && !infile.eof())
					{
						token_string[length++] = ch;
						infile >> noskipws >> ch;
					}

					if (ch == '|' && length == 2)								// Is this '||'?                       
					{
						token_string[length++] = ch;

						infile >> noskipws >> ch;
					}

					else if (infile.eof())
					{
						TOKEN[tonum] = eoft;
						break;
					}


				}

				else if (ch == '&')

				{
					infile >> noskipws >> ch;

					while (check_operator(ch) && !infile.eof())
					{
						token_string[length++] = ch;
						infile >> noskipws >> ch;
					}

					if (ch == '&')								// Is this '&&'?                       
					{
						token_string[length++] = ch;

						infile >> noskipws >> ch;
					}

					else if (infile.eof())
					{
						TOKEN[tonum] = eoft;
						break;
					}

				}
				
				else if (infile.eof())
				{
					TOKEN[tonum] = eoft;
					break;
				}

				else
					infile >> noskipws >> ch;


				token_string[length] = NULL;									// NULL is our flag to indicate the end of word


				token_value = GetNextToken(5, token_string);					// 5 : operator : in this case we distingush '/' at the below

				PrintToken(line_counter, token_value, token_string);			// To count the line number when we print this out, PrintToken needs 3 arguments


			}

			// ================================================================================================================== Symbol


			else if (check_symbol(ch))											// symbols : in case of ( ) { } , ; " [ ], we disconnect them in single peace
			{
				token_string[length++] = ch;
				infile >> noskipws >> ch;


				token_string[length] = NULL;


				token_value = GetNextToken(6, token_string);					// 6 : symbol  

				PrintToken(line_counter, token_value, token_string);			// To count the line number when we print this out, PrintToken needs 3 arguments
			}

			// ================================================================================================================== Underscore

			else if (check_underscore(ch))
			{
				while (check_digit(ch)|| check_underscore(ch)|| check_letter(ch)|| check_dot(ch))
				{
					token_string[length++] = ch;
					infile >> noskipws >> ch;

					if (infile.eof())
					{
						TOKEN[tonum] = eoft;
						break;
					}
				}

				token_string[length] = NULL;

				token_value = GetNextToken(8, token_string);				//   8 : Unknown

				PrintToken(line_counter, token_value, token_string);        //   To count the line number when we print this out, PrintToken needs 3 arguments

			}

			// ================================================================================================================== Slash

			else if (check_slash(ch))											//  operator : distinguish '/', '//' and '/*'
			{
				token_string[length++] = ch;

				infile >> noskipws >> ch;

				if (check_slash(ch))											//  find'//'
				{
					while ((ch != '\n') && !infile.eof())
					{
						infile >> noskipws >> ch;								//  Get rid of comment

						if (infile.eof())
						{
							TOKEN[tonum] = eoft;
							break;
						}
					}

				}
				else if (check_star(ch))										//  Find '/*' 
				{
					comment_end = 0;											//  To recognize '*/'

					infile >> noskipws >> ch;

					do {
						while (!check_star(ch) && !infile.eof())				//  find'*'        
						{
							infile >> noskipws >> ch;							//  Get rid of comment
							
							if (infile.eof())
							{
								TOKEN[tonum] = eoft;
								break;
							}
						}
						infile >> noskipws >> ch;

						if (check_slash(ch))
							comment_end = 1;
					} while (!comment_end && !infile.eof());
				
					infile >> noskipws >> ch;

					if (infile.eof())
					{
						TOKEN[tonum] = eoft;
						break;
					}

				}
				else															//   '/' operator
				{
					token_string[length] = NULL;

					token_value = GetNextToken(5, token_string);				//   5 : operator => '/' operator

					PrintToken(line_counter, token_value, token_string);        //   To count the line number when we print this out, PrintToken needs 3 arguments
				}
			}

			// ================================================================================================================== String

			else if (check_quote(ch))											// relational operators
			{
				token_string[length++] = ch;

				infile >> noskipws >> ch;
				token_string[length++] = ch;

				while (!check_quote(ch) && (ch != '\n'))
				{
					infile >> noskipws >> ch;

					if (length <= MAXIDT)										// I assume that the length of operator also not be greater than MAXIDT
						token_string[length++] = ch;

					if (infile.eof())
					{
						TOKEN[tonum] = eoft;
						break;
					}
																				// If number if operator is longer than MAXIDT, I store them till MAXIDT  
																				// We ignore things that longer than MAXIDT === If we have some change, we can define other constant like MAXFRACTION
				}

				if (length > MAXIDT)											// If length is longer than MAXIDT, I store them till MAXIDT
					token_string[MAXIDT] = NULL;
				else
					token_string[length] = NULL;

				if (ch != '\n')
				{
					token_value = GetNextToken(7, token_string);				// 7 : string .... 
					infile >> noskipws >> ch;
					PrintToken(line_counter, token_value, token_string);		// To count the line number when we print this out, PrintToken needs 3 arguments
				}				
				else
				{																
					lex_err(2);
																				// 8 : unknown .... this unknown case in string is error so I will ignore it
				}		
			}
		}
		infile.close();

		TOKEN[tonum] = eoft;

	}
}

void PrintToken(int line, int token, char* word)				// THIS FUNCTION PRINT OUT THE FORM TO USER PROPERLY
{

	int i, j;
	char *temp;													// Temp char pointer that used for copy the token name and print it out


	if (0 <= token && token <= 19)								// If token is one of the reserved word index number
	{
		temp = resword[token];
		cout << setw(3) << tonum << setw(33) << word << setw(20) << temp << setw(13) << line << endl;
		LINE[tonum] = line;										
		tonum++;
	}
	else if (20 <= token && token <= 39)						// If token is not the reserved word index number
	{
		if (token != numt)										// All the other tokens that are not a number value
		{
			j = (token - 20);									// Because I cannot re-initialize the index number of array like enum type, I substitue 20 out from the enum constant to match with token name string array
			temp = token_t[j];
			cout << setw(3) << tonum << setw(33) << LEXEME[tonum] << setw(20) << temp << setw(13) << line << endl;  // These tokens are the values that are stored in LEXEME
			LINE[tonum] = line;
			tonum++;
		}
		
		else if (token == numt && VALUE[tonum] != NULL)																										// Number value
		{
			j = (token - 20);									// Because I cannot re-initialize the index number of array like enum type, I substitue 20 out from the enum constant to match with token name string array
			temp = token_t[j];
			cout << setw(3) << tonum << setw(21) << word << setw(29) << temp << setw(16) << line << setw(11) << "o" << endl;
			LINE[tonum] = line;
			tonum++;																								// Only if the token is numt, I will print out it's own value if it is integer, at Value section and if it is float, at the Valuer section
		}	

		else if (token == numt && VALUER[tonum] != NULL)																										// Number value
		{
			j = (token - 20);									// Because I cannot re-initialize the index number of array like enum type, I substitue 20 out from the enum constant to match with token name string array
			temp = token_t[j];
			cout << setw(3) << tonum << setw(21) << word << setw(29) << temp << setw(16) << line << setw(22) << "o" << endl;
			LINE[tonum] = line;
			tonum++;																								// Only if the token is numt, I will print out it's own value if it is integer, at Value section and if it is float, at the Valuer section
		}

	}

	else if (40 <= token && token <= 44)								// If token is one of the reserved word index number
	{
		temp = need_RDP[token-40];
		cout << setw(3) << tonum << setw(33) << word << setw(20) << temp << setw(13) << line << endl;
		LINE[tonum] = line;
		tonum++;
	}

}