#ifndef _HASH_H
#define _HASH_H

#include "Lexical.h"
#include "RDP.h"

#define TableSize 211	// Use Prime #
#define EOS '\0'

enum VarType						// Types of variables
{
	intType, floatType, charType, boolType, voidType, unknownType, literalType
};

enum EntryType						// Types of Entry
{
	constEntry, varEntry, functionEntry, classEntry
};

struct ParamNode;					//PARAMETER NODE
typedef ParamNode *Param_ptr;
struct ParamNode
{
	VarType TypeOfParam;
	Param_ptr NEXT;
};

struct funcname {
		char* function_name;
		funcname* next_F;
		};
			
struct varname {
 	char* variable_name;
	varname* next_V;
	};	

struct STK;
typedef STK *Push_ptr;
struct STK {
	char* data_value;
	int Offset;
	Push_ptr next_S;
};



struct TableEntry;					// 0-210
typedef TableEntry *Entry_ptr;		// Pointer to actual table entry
struct TableEntry {					// [CAUTION] Do not forget to null the last next everytime in implement step!

	int Token;						// TOKEN[tonum]
	char* LEX;						// LEXEME[tonum]
	int Depth;						// index of stack

	EntryType TypeOfEntry;			// tag field for the union

	union {
		struct {					// :::::VARIABLE::::: 
			VarType TypeOfVar;		// :::::::::::::::::: 
			int Offset;				// if professor adds float and void in type, we should add offset increase in that part on RDP also
			int size;				
		}VAR;

		struct {					// :::::CONSTANT:::::
			VarType TypeOfConst;	// :::::::::::::::::: int or float constant
			int Offset;				// if professor adds float and void in type, we should add offset increase in that part on RDP also
			union {					
				int intV;			// VALUE[tonum]
				float realV;		// VALUER[tonum]
			};
		}CONSTANT;

		struct {					// :::::FUNCTION::::: - 
			int SizeOfLocal;		// Total required size of all the parameters (int*4=16)
			int NumberOFParam;		
			VarType ReturnType;
			Param_ptr ParamList;	// Linked list of parameter types (if we have parameter in fuction, connect them in single linked list
		}METHOD;

		struct {					// :::::CLASS:::::
			int SizeOfLocal_C;
			VarType ClassType;
			struct funcname FUNC;	
			struct varname VARI;								//Linked list of parameter types
		}CLASS;


	}; // End of UNION
	Entry_ptr NEXT;					// Points to next list item
};


extern Entry_ptr SYMBOLTABLE[];
extern Entry_ptr Lookup(char *);
extern void Insert(char *, int, int, EntryType, int, int, int, int, char *);
extern void WriteTable(int);
extern void DeleteDepth(int);
extern void InitTable();
extern void cls_Tailing(char *, char *, EntryType, int);
extern void param_dangling(char*, char *, EntryType);
unsigned int hashfunction(char*);
extern void print_test();
extern void base_set_push();
extern void push(char *);
extern void emit(std::ofstream&, char *);
extern void procedure_A(std::ofstream&);
extern void procedure_E(Entry_ptr&, std::ofstream&);
extern void procedure_T(Entry_ptr&, std::ofstream&);
extern void procedure_R(Entry_ptr&, std::ofstream&);
extern void procedure_M(Entry_ptr&, std::ofstream&);
extern Push_ptr newtemp();
extern Push_ptr push_TAC(char*);
extern Push_ptr Pop(char* );
extern void Delete_stk(Push_ptr);

#endif // !_HASH_H

