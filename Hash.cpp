

#include "Hash.h"

// MY PROGRAM DESIGNED AS NOT TO PRINT OUT THE ELEMENTS AT SAME DEPTH IF PROGRAM HAS MULTIPLE DECLARATION
// And I don't know JAVA at all so I only know that final class main is the class that cannot be inherited so I cannot sure that to insert in class or not so I didn't do anything yet

using namespace std;

char* print_type[7] = {"intType", "floatType", "charType", "boolType", "voidType", "unknownType", "literalType" };

Entry_ptr SYMBOLTABLE[TableSize];		//Symboltable

void InitTable() // Cleaning the pointer array (symboltable) as null from 0~200
{
	int i;

	for (i = 0; i < TableSize; i++)
	{
		SYMBOLTABLE[i] = NULL;
	}
}

unsigned int hashfunction(char* s)				// [PRIVATE] : should be internal routine only, do not list in the interface section :: s=LEXEME[tonum]
{												// THIS FUNCTION DETERMINE EACH ENTRY INDEX TO START -> returns value between 0~200

	char *p;									//Currunt ASCII code value adding
	unsigned int h = 0, g;

	for (p = s; *p != EOS; p = p + 1)			
	{
		h = (h << 4) + (*p);
		if (g = h & 0xf0000000) 
		{
			h = h ^ (g >> 24);
			h = h ^ g;
		}
	}
	return (h % TableSize);
}

Entry_ptr Lookup(char* lex)						// Search the lex in the table
{												
	
	Entry_ptr look_ptr= NULL;
	char* cmp_ptr = NULL;
	
	if (SYMBOLTABLE[hashfunction(lex)] == NULL)			// If our lex's hash entry is empty : print out error message
	{
		return NULL;									// we don't have the entry that matches with our lex value : NULL
	}

	else
	{
		cmp_ptr = SYMBOLTABLE[hashfunction(lex)]->LEX;		//[WATCH OUT] for hash collision! 

		if (strcmp(lex, cmp_ptr) == 0)						// If our lex's hash entry is filled we might have 2 cases
		{
			look_ptr = SYMBOLTABLE[hashfunction(lex)];		// 1. lex and our entry node's lex are same : Just return that location
			
			
			return look_ptr;
		}
		
		else												// 2. lex and our entry node's lex are different but hash value is same
		{
			look_ptr = SYMBOLTABLE[hashfunction(lex)]->NEXT;		

			while (look_ptr != NULL)						// search till we find same lexeme as ours
			{		
				cmp_ptr = look_ptr->LEX;

				if (strcmp(cmp_ptr, lex) == 0)				// IF WE FOUND 1 : Just return that location
				{
					//cout << "This is the address of your lexeme : " << look_ptr << endl;
					return look_ptr;
				}

				look_ptr = look_ptr->NEXT;
			}

															//	we don't have such LEX in our table : Print out error msg
			//cout << "[ERROR] : we don't have '" << lex << "' in our table" << endl;
			return NULL;									// Because we don't have such lex in our table : NULL pointer return
		}
	}
}

void Insert(char* lex, int t_type, int depth, EntryType entry, int v_index, int ofs, int prm_idx, int mtd_idx, char* mtdname)					// Need to allocate memory :: CAUTION :: When do insert, must initialize last node's ptr as null!!! 브이인덱스 추후 테일링으로 옮기고 삭제
{
	Entry_ptr Insert_ptr=NULL, Current_ptr = NULL;				// Insert_ptr for insult the node at first of the entry | Crt_ptr for null the last node's NEXT
	Entry_ptr clsloc = NULL;									// Indicate the location of class ptr to maintain the location till the all of class information gathered and class is ended.
	Entry_ptr funcloc = NULL;									// Indicate the location of function ptr to maintain the location till the all of function information gathered and function is ended.
	VarType typeindicator = unknownType;						// Using this to represent returntype of method

	int i = 0;
	int size = 0;
	char* name, *rtrname = NULL;								// Using when printing
	unsigned int h = hashfunction(lex);							// STORE THE HASH VALUE

	Insert_ptr = new TableEntry;								// Memory allocate

	if (SYMBOLTABLE[h] != NULL)
	{
		if (Lookup(lex) == NULL)
		{
			// DO NOTHING
		}

		else if (Lookup(lex)->Depth == depth)						//[Watch out] for hash collision
		{
			if (Lookup(lex)->TypeOfEntry == entry)
			{
				cout << "[ERROR] you have multiple declaration for lexeme : " << lex << endl;
				exit(1);
			}
		}
	}

	if (v_index != 7)
	{
		Insert_ptr->LEX = lex;										// Assign basic values : lexeme. token, depth
	}

	else if (v_index == 7)
	{
		Insert_ptr->LEX= (char *)malloc(50);
		strcpy(Insert_ptr->LEX, lex);										// Assign basic values : lexeme. token, depth
	}
	Insert_ptr->Token = t_type;
	Insert_ptr->Depth = depth;
	Insert_ptr->TypeOfEntry = entry;
	Insert_ptr->NEXT = NULL;									// Null the pointer for safety


	if (entry == constEntry)			// We don't have to make a list for var and const
	{
		name = "Constant";

		if (VALUE[tonum - 1] != NULL)	//토넘 순서 맞는지 추후 확인
		{
			Insert_ptr->CONSTANT.TypeOfConst = intType;
			Insert_ptr->CONSTANT.realV = NULL;
			Insert_ptr->CONSTANT.intV = VALUE[tonum-1];
			Insert_ptr->CONSTANT.Offset = ofs;
			if (mtd_idx == 1 && prm_idx == 0)
			{
				Lookup(mtdname)->METHOD.SizeOfLocal = Lookup(mtdname)->METHOD.SizeOfLocal + 2;
			}

			//cout << "Inserted offset " << Insert_ptr->CONSTANT.Offset << endl;
		}

		else if (VALUER[tonum - 1] != NULL);
		{
			Insert_ptr->CONSTANT.TypeOfConst = floatType;
			Insert_ptr->CONSTANT.intV = NULL;
			Insert_ptr->CONSTANT.realV = VALUER[tonum-1];
			Insert_ptr->CONSTANT.Offset = ofs;
			if (mtd_idx == 1 && prm_idx == 0)
			{
				Lookup(mtdname)->METHOD.SizeOfLocal = Lookup(mtdname)->METHOD.SizeOfLocal + 4;
			}
			//cout << "Inserted offset " << Insert_ptr->CONSTANT.Offset << endl;
		}
	}

	else if (entry == varEntry)								
	{

		name = "Variable";


		if (v_index == 1)//intt
		{
			Insert_ptr->VAR.TypeOfVar = intType;			// :::::::::::::::::: 
			Insert_ptr->VAR.size = 2;						// Size of vars
			Insert_ptr->VAR.Offset = ofs;
			size = 2;
			//cout << "Inserted lexeme " << Insert_ptr->LEX << endl;
			//cout << "Inserted offset " << Insert_ptr->VAR.Offset << endl;
		}

		else if (v_index == 2)//floatt
		{
			Insert_ptr->VAR.TypeOfVar = floatType;
			Insert_ptr->VAR.size = 4;
			size = 4;
		}

		else if (v_index == 3)//chart
		{
			Insert_ptr->VAR.TypeOfVar = charType;
			Insert_ptr->VAR.size = 1;
			size = 1;
		}

		else if (v_index == 4)//booleant
		{
			Insert_ptr->VAR.TypeOfVar = boolType;
			Insert_ptr->VAR.size = 1;
			Insert_ptr->VAR.Offset = ofs;
			size = 1;
		}

		else if (v_index == 5)//voidt
		{
			// void will be used in only fuction in our compiler I guess
		}

		else if (v_index == 7)//literal
		{
			Insert_ptr->VAR.TypeOfVar = literalType;
			Insert_ptr->VAR.size = 0;//this is not a local var
			Insert_ptr->VAR.Offset = ofs;
			size = 0;
		}

		else//etc
		{

		}

		if (mtd_idx == 1 && prm_idx == 0)
		{
			Lookup(mtdname)->METHOD.SizeOfLocal += size;
		}
	}

	else if (entry == functionEntry)
	{
		name = "Method";
		funcloc = SYMBOLTABLE[hashfunction(lex)];						// now point symbol table from [0]~[200] in for loop to find the all the elements that has same depth

		if (v_index == 1)//intt
		{
			typeindicator = intType;
			rtrname = "int";
		}
		else if (v_index == 2)//float
		{
			typeindicator = floatType;
			rtrname = "float";
		}
		else if (v_index == 3)//chart
		{
			typeindicator = charType;
			rtrname = "char";
		}
		else if (v_index == 4)//booleant
		{
			typeindicator = boolType;
			rtrname = "bool";
		}

		else if (v_index == 5)//voidt
		{
			typeindicator = voidType;
			rtrname = "void";
		}

		Insert_ptr->METHOD.SizeOfLocal = 0;								// Initialize function information to null at first and using funcloc ptr, I will renew them constantly
		Insert_ptr->METHOD.NumberOFParam = 0;
		Insert_ptr->METHOD.ReturnType = typeindicator;						// EXCEPT RETURN TYPE : returntype determined at declaration
		Insert_ptr->METHOD.ParamList = NULL;

	}

	else if (entry == classEntry)
	{
		name = "Class";
		clsloc = SYMBOLTABLE[hashfunction(lex)];						// now point symbol table from [0]~[200] in for loop to find the all the elements that has same depth

		Insert_ptr->CLASS.SizeOfLocal_C = 0;
		Insert_ptr->CLASS.ClassType = unknownType;
		Insert_ptr->CLASS.FUNC.next_F = NULL;
		Insert_ptr->CLASS.FUNC.function_name = NULL;
		Insert_ptr->CLASS.VARI.next_V = NULL;
		Insert_ptr->CLASS.VARI.variable_name = NULL;
	}

	//print

	/*if (0 <= t_type && t_type <= 21)								// If token is one of the reserved word index number
	{
		if (entry == functionEntry)
		{
			cout << " Lexeme [ " << lex << " ] " << endl;
			cout << " Depth : " << depth << endl << " Token : " << resword[t_type] << endl << " Entrytype : " << name << endl << " Returntype : " << rtrname << endl << endl;
		}
		else
		{
			cout << " Lexeme [ " << lex << " ] " << endl;
			cout << " Depth : " << depth << endl << " Token : " << resword[t_type] << endl << " Entrytype : " << name << endl << endl;
		}
	}

	else if (20 <= t_type && t_type <= 39)								// If token is one of the reserved word index number
	{
		if (entry == functionEntry)
		{
			cout << " Lexeme [ " << lex << " ] " << endl;
			cout << " Depth : " << depth << endl << " Token : " << resword[t_type] << endl << " Entrytype : " << name << endl << " Returntype : " << rtrname << endl << endl;
		}

		else
		{
			cout << " Lexeme [ " << lex << " ] " << endl;
			cout << " Depth : " << depth << endl << " Token : " << token_t[t_type - 20] << endl << " Entrytype : " << name << endl << endl;
		}
	}

	else if (t_type == 41)//finalt
	{
		if (entry == functionEntry)
		{
			cout << " Lexeme [ " << lex << " ] " << endl;
			cout << " Depth : " << depth << endl << " Token : " << resword[t_type] << endl << " Entrytype : " << name << endl << " Returntype : " << rtrname << endl << endl;
		}
		else
		{
			cout << " Lexeme [ " << lex << " ] " << endl;
			cout << " Depth : " << depth << endl << " Token : Finalt " << endl << " Entrytype : " << name << endl << endl;
		}
	}*/

	//list connecting


	if (SYMBOLTABLE[h] != NULL)									// If there is something already : 
	{
		Current_ptr = SYMBOLTABLE[h];							// Put ours at first :

		Insert_ptr->NEXT = Current_ptr;							// using our next tag to point already existed index
		SYMBOLTABLE[h] = Insert_ptr;							// and put ours in that position (not to lose link)
	}

	else														// If our hash th entry is empty
	{

		SYMBOLTABLE[h] = Insert_ptr;							// just put our value in there
		Insert_ptr->NEXT = NULL;								// Double check for null the last Next

	}


	//================================================================================================================================//

}


void param_dangling(char* name, char* lex, EntryType entry)	//size, PARAM_pointer connect, track the # of params
{
	Entry_ptr dangling = NULL, store = NULL;
	Param_ptr pr = NULL, curr = NULL;
	int dep = 0, numofparam = 1;

	dangling = Lookup(name);
	store = Lookup(lex);

	pr = new ParamNode;
	pr->TypeOfParam = store->VAR.TypeOfVar;
	pr->NEXT = NULL;

	curr = dangling->METHOD.ParamList;


	if (curr == NULL)
	{
		//	pr->TypeOfParam = store->VAR.TypeOfVar;          ============ 4.17
		//	pr->NEXT = NULL;                                 ============ 4.17
		dangling->METHOD.NumberOFParam = numofparam;

		dangling->METHOD.ParamList = pr;                 //  ================4.17 추가

		/*cout << "First parameter entered" << endl;
		cout << endl << "TYPE OF PARAM : " << print_type[pr->TypeOfParam] << endl;
		cout << endl << endl << "Number of dangled parameter in method [ " << name << " ] :" << Lookup(name)->METHOD.NumberOFParam << endl << endl;*/


	}

	else
	{

		while (curr->NEXT)
		{
			numofparam++;
			curr = curr->NEXT;
		}

		curr->NEXT = pr;
		numofparam++;
		dangling->METHOD.NumberOFParam = numofparam;

		/*cout << "[ " << numofparam << " th parameter entered ]" << endl;
		cout << endl << "[TYPE OF PARAM : " << print_type[pr->TypeOfParam] << " ]" << endl;
		cout << endl << "Number of dangled parameter in method [ " << name << " ] :" << Lookup(name)->METHOD.NumberOFParam << endl << endl;*/

		//  =====> pr 에 정보를 위에 처럼 넣어라, 파라미터 갯수....


		//dep = dangling->Depth;

		/*if (store->Depth == dep)
		{
			pr->TypeOfParam = store->VAR.TypeOfVar;
			pr->NEXT = NULL;

			dangling->METHOD.NumberOFParam++;

		}*/

	}

}

void cls_Tailing(char* clsname, char* func_or_var_name, EntryType entry, int v_index)
{
	Entry_ptr dangling = NULL, name = NULL;
	funcname* fr = NULL;
	funcname* fp = NULL;
	varname* vr = NULL;
	varname* vp = NULL;
	
	dangling = Lookup(clsname);
	name = Lookup(func_or_var_name);

	fr = new funcname;										// Memore allocate
	vr = new varname;										// Memore allocate	
	
	if (entry==functionEntry)
	{
		fr->function_name = name->LEX;
		fr->next_F = NULL;
		fp = &dangling->CLASS.FUNC;

		if (fp->function_name==NULL)
		{
			dangling->CLASS.FUNC = *fr;

			//cout << "This is 1st member function of your class [ " << clsname << " ] : " << dangling->CLASS.FUNC.function_name << endl << endl;
		}
		
		else
		{
			while (fp->next_F)
			{
				fp = fp->next_F;
			}

			fp->next_F = fr;

			//cout << "This is member function of your class [ " << clsname << " ] : " << fp->next_F->function_name<< endl << endl;
		}
	}

	else if (entry==varEntry)
	{
		vr->variable_name = name->LEX;
		vr->next_V = NULL;
		vp = &dangling->CLASS.VARI;

		if (vp->variable_name==NULL)
		{		
			
			dangling->CLASS.VARI = *vr;
			dangling->CLASS.SizeOfLocal_C = name->VAR.size;

			//cout << "This is Local variable of your class [ " << clsname << " ] : " << dangling->CLASS.VARI.variable_name << endl << endl;
			//cout << "This is the size of your local variable : " << dangling->CLASS.SizeOfLocal_C << endl;
		}

		else
		{

			while (vp->next_V)
			{
				vp = vp->next_V;
			}

			dangling->CLASS.SizeOfLocal_C += name->VAR.size;
			vp->next_V = vr;

				//cout << "This is Local variable of your class [ " << clsname << " ] : " << vp->next_V->variable_name << endl << endl;
				//cout << "This is the size of your local variable : " << dangling->CLASS.SizeOfLocal_C << endl << endl;
		}
	}


}

void WriteTable(int depth)		// Include a procedure that will Write out all Vars(LEXEME) that are in the table at a SPECIFIED DEPTH
{
	int i;
	Entry_ptr to_be_written = NULL, now = NULL;		// Set null to every ptr for safety

	for (i = 0; i < TableSize; i++)
	{
		now = SYMBOLTABLE[i];						// now point symbol table from [0]~[200] in for loop to find the all the elements that has same depth

		while (now != NULL)							// If entry is empty, do nothing and go into next for loop but if we have something in that entry list,
		{											// start to find all the elements that has same depth in that entry
			if (now->Depth == depth)				// If we found :
			{
				to_be_written = now;				
				now = now->NEXT;

				cout << "This is the lexeme on depth " << depth << " : " << to_be_written->LEX << endl;				
			}
 
			else									// If we have differnt depth element just goto next
			{
				now = now->NEXT;
			}
		}

	}
}

void DeleteDepth(int depth)		// This is passed the depth and deletes all the records that are in the table at that depth 
{								// Depth = Stack ptr
	int i;
	Entry_ptr right_now = NULL, to_be_gone = NULL;


	for (i = 0; i < TableSize; i++)					// Search all the symboltable vertically (all the entries)
	{
		to_be_gone = SYMBOLTABLE[i];
		right_now = SYMBOLTABLE[i];					// To look at first node together


		while (to_be_gone != NULL)					// If entry is empty, do nothing and go into next for loop but if we have something in that entry list,
		{											// start to find all the elements that has same depth in that entry

			if (to_be_gone->Depth == depth)			// If we found same depth
			{

				if (SYMBOLTABLE[i] == to_be_gone)
				{
					SYMBOLTABLE[i] = to_be_gone->NEXT;
					delete to_be_gone;						// Delete the allocated memory but we should care about multiple depth can be exist in one entryel
					to_be_gone = SYMBOLTABLE[i];
				}

				else
				{
					right_now->NEXT = to_be_gone->NEXT;
					delete to_be_gone;						// Delete the allocated memory but we should care about multiple depth can be exist in one entryel
					to_be_gone = right_now->NEXT;
				}
			}


			else											// If we have differnt depth element just goto next
			{

				right_now = to_be_gone;
				to_be_gone = to_be_gone->NEXT;

			}

		}

	}
}

void Delete_stk(Push_ptr BASE)
{
	Push_ptr TBG = NULL, curr = NULL;

	TBG = BASE;

	while (TBG != NULL) {

		curr = TBG->next_S;
		delete TBG; // DELETE PARAMETERS
		TBG = curr;
	}

}

