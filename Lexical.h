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


#ifndef _LEXICAL_H
#define _LEXICAL_H

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>


#define MAXIDT 31			// Maximum identifier length number = 31
#define RESNUM 20			// Number of reserved word that we use in this program
#define MAXSTRING 300		// Maximum number for char in string for AS7


/* Keyword and symbol table ======================================================================================================================================================================================================================================================================= */
/* ================================================================================================================================================================================================================================================================================================ */


enum resword_t {
	classt, publict, statict, voidt, maint,										//enumerated data type for reserved word tokens
																				/*  0		1        2		3		 4	*/
	stringt, extendst, returnt, intt, booleant,
	/*  5		6		 7       8        9	*/
	ift, elset, whilet, SOPt, lengtht,
	/*10	11    12    13     14 */
	truet, falset, thist, newt, unknownt
};
/*  15	 16		 17	   18	19*/




enum token {
	idt = 20, numt, literalt, relopt, addopt,											//enumerated data type for other tokens
																						/* 20	21       22      23       24 */												//this type starts from 20 as index number to be distinguished with reserved words
	mulopt, assignopt, sm_op_brkt, sm_cl_brkt, sq_op_brkt,
	/*   25         26           27          28      29 */
	sq_cl_brkt, cr_op_brkt, cr_cl_brkt, quot, eoft,
	/*     30         31         32	    33	  34*/
	dbrelopt, semicolt, punct, commt, dbquot
};


enum needforRDP { finalt = 40, exclt, readt, writet, writelnt };							//Make another token table that we need in parser


/* ======================================================================================================================================================================================================================================================================= */
/* ======================================================================================================================================================================================================================================================================= */




	void Initialization();  // Initialize all the global variables to null

	void lex_err(int);		// Error detect function

	int GetNextToken(int, char*);	// Generate the next token if document is fully scanned

	int check_letter(char ch);

	int check_digit(char ch);

	int check_underscore(char ch);

	int check_dot(char ch);

	int check_relational(char ch);

	int check_operator(char ch);

	int check_slash(char ch);

	int check_star(char ch);

	int check_symbol(char ch);

	int check_quote(char ch);

	void PrintToken(int, int, char*);		// Take the token number that assigned by enum data type and matching with it to token name string

	void ProcessToken(std::ifstream&);


// =============   Functions which are defined in RDParser.cpp   3.16

int Goal(std::ofstream&);
int match(int);
int Prog(std::ofstream&);
int MoreClasses(int, std::ofstream&);
int MainClass(int, std::ofstream&);
int ClassDecl(int, std::ofstream&);
int VarDecl(int, std::ofstream&);
int IdentifierList(int, std::ofstream&);
int Identi_sub(int, std::ofstream&);
int Type();
int MethodDecl(int, std::ofstream&);
int FormalList(int, std::ofstream&);
int FormalRest(int, std::ofstream&);
int SeqOfStatements(int, std::ofstream&);
int Expr(int, std::ofstream&);
int Statement(int, std::ofstream&);
int StatTail(int, std::ofstream&);
int AssignStat(int, std::ofstream&);
int IOStat(int, std::ofstream&);
int Relation(int, std::ofstream&);
int SimpleExpr(int, std::ofstream&);
int Term(int, std::ofstream&);
int MoreTerm(int, std::ofstream&);
int Factor(int, std::ofstream&);
int MoreFactor(int, std::ofstream&);
int Addop(int, std::ofstream&);
int Mulop(int, std::ofstream&);
int Signop(int, std::ofstream&);
int MethodCall(int, std::ofstream&);
int ClassName(int, std::ofstream&);
int Params(int, std::ofstream&);
int ParamsTail(int, std::ofstream&);
int In_Stat(int, std::ofstream&);
int Id_List(int, std::ofstream&);
int Id_List_Tail(int, std::ofstream&);
int Out_Stat(int, std::ofstream&);
int Write_List(int, std::ofstream&);
int Write_List_Tail(int, std::ofstream&);
int Write_Token(int, std::ofstream&);


#endif