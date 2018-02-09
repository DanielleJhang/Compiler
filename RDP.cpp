/* ================================================================================================================================================================================================================================================================================================ */
/* ================================================================================================================================================================================================================================================================================================ */
/* ================================================================================================================================================================================================================================================================================================ */
/*
[CSC 446] Construct Compiler

Assignment # 7 : Intermediate Code Generation

Student ID 7354802
Ji eun Jhang
*/
/* ================================================================================================================================================================================================================================================================================================ */
/* ================================================================================================================================================================================================================================================================================================ */
/* ================================================================================================================================================================================================================================================================================================ */



#include "Lexical.h"
#include "RDP.h"            //================================= 3.16
#include "Hash.h"			//================================= 3.27

#define SIZE 5

using namespace std;

int t_num = 1;											//Index # for TOKEN array
int var_idx = 0;										//If index is 1 = int , 2 = float , 3 = char, 4 =booleant, voidt 7=LITERAL
char * mtdname = NULL, *mtdname_ = NULL;										//Store method name here;
char * clsname;											//Store class name here;
int ofs = 0, TAC_num = 0;
int mtd_idx = 0;										// 0= No mathod progress going on || 1= yes, going on
int prm_idx = 0;										// 0= No parameter progress going on || 1= yes, going on
int t_numback = 0, r_numback = 0;											// After procs are done, renew the t_num as the number bafore procs are executed
int line = 0;
char * TAC_start = NULL;
int ProcR_idx = 0, minus_idx = 0, ret_idx = 0;										// If procedure R was executed =>1, no=>0 // void ret =0, not void =1
char * par_stk[10];
int par_s = 0;
int wrS = 0, wrI = 0, rdI = 0, w_iint;
char * w_istr;

struct STK stack[1000];

Push_ptr SP = NULL, PP = NULL;
Push_ptr BP = NULL;
Push_ptr RP = NULL;
Push_ptr TP = NULL;

int top = 0;

int Goal(ofstream& outFile)												//This function call the whole process of my parser
{
	InitTable();
	base_set_push();
	Prog(outFile);											//Just call the progress function

	return 0;
}

int match(int token)									//:::::::::::::::::::::::::::::: MATCH FUNCTION : will detect current token is what I desired or not::::::::::::::::::::::::::::::
{

	if (0 <= token&&token <= 44)					// If current token in the element of lex token table
	{

		if (TOKEN[t_num] == token)					//======================= CASE :: Match success
		{
			//cout << "Token # " << t_num << " : match success" << endl;			// Let user know that match progress is successed

			t_num++;								//If current token is what I desired, I move the TOKEN index to right once to read next token
			return 1;
		}

		else										//======================= CASE :: Match failed
		{
			if (TOKEN[t_num] == eoft)
			{
				cout << "Program ended in illegal condition : we expect to complete the grammar" << endl;

				exit(1);
			}

			else if (TOKEN[t_num] != numt)				//If token is not our desired token and it is not numt
			{
				if (0 <= token && token <= 19)		//If error is detected, program ends
				{
					cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : " << "Error : instead of [ " << LEXEME[t_num] << " ], we expect [ " << resword[token] << " ]" << endl;			//If current token isn't the token that I want, just print error msg

					exit(1);
				}

				else if (20 <= token && token <= 39)
				{
					cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : " << "Error : instead of [ " << LEXEME[t_num] << " ], we expect [ " << token_t[token - 20] << " ]" << endl;		//If current token isn't the token that I want, just print error msg

					exit(1);
				}

				else if (40 <= token && token <= 44)
				{
					cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : " << "Error : instead of [ " << LEXEME[t_num] << " ], we expect [ " << need_RDP[token - 20] << " ]" << endl;		//If current token isn't the token that I want, just print error msg

					exit(1);
				}

			}
			else									//If token is not our desired token and it is numt : Because I don't store numt in TOKEN array so I have to seperate this case
			{
				if (0 <= token && token <= 19)		//If error is detected, program ends
				{
					cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : Error : instead of [ NUMBER token ] we expect [ " << resword[token] << " ]" << endl;

					exit(1);							//If error is detected, program ends
				}

				else if (20 <= token && token <= 39)		//If error is detected, program ends
				{
					cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : Error : instead of [ NUMBER token ] we expect [ " << token_t[token - 20] << " ]" << endl;

					exit(1);							//If error is detected, program ends
				}
			}
		}

	}


	else												// IF INDEX REACHED TO EOF : END FUNCTION AND DO NOTHING
		exit(1);

}

int Prog(ofstream& outFile)												//:::::::::::::::::::::::::::::: PROGRESS FUNCTION : will going into progress till EOF ::::::::::::::::::::::::::::::
{
	int depth = 1;
	int i = 0, j = 0;
	char *stringo[20];
	char code[MAXSTRING], intt[2];


	for (i = 0; i < 20; i++)
	{
		stringo[i] = NULL;
	}

	i = 1;

	while (LEXEME[i])
	{
		if (TOKEN[i]==literalt)
		{
			stringo[j] = (char *)malloc(strlen(LEXEME[i])+1);
			strcpy(stringo[j], LEXEME[i]);
			j++;
		}
		i++;
	}


	strcpy(code, "		.model small");
	strcat(code, "\n");
	strcat(code, "		.stack 100h");
	strcat(code, "\n");
	strcat(code, "		.data");
	strcat(code, "\n");
	if (stringo[0] != NULL)
	{
		i = 0;
		while (stringo[i])
		{
			strcat(code, "S");
			sprintf(intt, "%d", i);
			strcat(code, intt);
			strcat(code, "		DB		");
			strcat(code, stringo[i]);
			strcat(code, ",");
			strcat(code, "\"");
			strcat(code, "$");
			strcat(code, "\"");
			strcat(code, "\n");
			i++;
		}
	}
	strcat(code, "		.code");
	strcat(code, "\n");
	strcat(code, "		include io.asm");
	strcat(code, "\n");
	emit(outFile, code);


	MoreClasses(depth, outFile);										// Unconditional start
	MainClass(depth, outFile);
	//TAC 제너레이트 할 지점
	//print_test();
		if (TOKEN[t_num] != eoft)						// To double check that file is read completely and not to read the array after eof token it may occur pointer error
		{
			cout << "[ERROR] YOUR PROGRAM NOT ENDED IN PROPER CONDITION" << endl << endl;
			exit(1);
		}

		else											// IF INDEX REACHED TO EOF : END FUNCTION AND DO NOTHING
			return 0;

}

// =================================== depth 1 == classname depth

int MainClass(int depth, ofstream& outFile)											//:::::::::::::::::::::::::::::::::::::::::::::: MAINCLASS GRAMMAR ::::::::::::::::::::::::::::::::::::::::::::::
{
	char code[MAXSTRING];

	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num]==finalt)		// If first condition is satisfied : final token detected first -> go into progress
		{

			match(finalt);			//depth 1				
			match(classt);
			match(idt);
			Insert(LEXEME[t_num - 1], TOKEN[t_num - 1], depth, classEntry, 0, ofs, prm_idx, mtd_idx, mtdname);
			clsname = LEXEME[t_num - 1];
			match(cr_op_brkt);
			depth++;
			match(publict);			//depth 2
			match(statict);
			match(voidt);
			var_idx = 5;
			match(maint);
			Insert(LEXEME[t_num - 1], TOKEN[t_num - 1], depth, functionEntry, var_idx, ofs, prm_idx, mtd_idx, mtdname);
			cls_Tailing(clsname, LEXEME[t_num - 1], functionEntry, var_idx);
			mtdname = LEXEME[t_num - 1];			
			strcpy(code, "		mov		ax,@data");
			strcat(code, "\n");
			strcat(code, "		mov		ds,ax");
			strcat(code, "\n");
			strcat(code, mtdname);
			strcat(code, "		");
			strcat(code, "PROC");
			emit(outFile, code);
			match(sm_op_brkt);
			DeleteDepth(depth);			//delete to loopup for undeclared idt : on our language, class name and fuction name can be duplicated : 2-1=1
			depth++;
			match(stringt);			//depth 3
			match(sq_op_brkt);
			match(sq_cl_brkt);
			match(idt);
			match(sm_cl_brkt);
			match(cr_op_brkt);
			SeqOfStatements(depth, outFile);
			match(cr_cl_brkt);
			DeleteDepth(depth);			//delete to loopup for undeclared idt : on our language, class name and fuction name can be duplicated : 2-1+1-1=1
			depth--;
			match(cr_cl_brkt);
			depth--;
			ofs = 0;
			strcpy(code, "		mov		ax,04ch");
			strcat(code, "\n");
			strcat(code, "		int		21h");
			strcat(code, "\n");
			strcat(code, mtdname);
			strcat(code, "		");
			strcat(code, "ENDP");
			strcat(code, "\n");
			strcat(code, "		END");
			strcat(code, "		");
			strcat(code, "start");
			emit(outFile, code);

			return 1;
		}

		else											//======================= CASE :: ERROR DETECTED
		{
			cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : " << "Error : instead of [ " << LEXEME[t_num] << " ], we expect [ final ]" << endl;		//If current token isn't the token that I want, just print error msg

			exit(1);									//If error is detected, program ends

		}

	}

	else												// IF INDEX REACHED TO EOF : END FUNCTION AND DO NOTHING
	{
		cout << "Program ended in illegal condition : You should complete the MainClass grammar" << endl;
		exit(1);
	}
}

int MoreClasses(int depth, ofstream& outFile)										//:::::::::::::::::::::::::::::::::::::::::::::: MORECLASSES GRAMMAR ::::::::::::::::::::::::::::::::::::::::::::::
{

	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == classt)						// If first condition is satisfied : class token detected first -> go into progress
		{
			ClassDecl(depth, outFile);
			MoreClasses(depth, outFile);

			return 1;
		}

		else											// nullable phase
		{
			return 0;
		}
	}

	else												// IF INDEX REACHED TO EOF : END FUNCTION AND DO NOTHING
		return 0;

}

int ClassDecl(int depth, ofstream& outFile)											//:::::::::::::::::::::::::::::::::::::::::::::: CLASSDECLARATION GRAMMAR ::::::::::::::::::::::::::::::::::::::::::::::
{

	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		match(classt);									// Do two match first that are the common factor
		match(idt);										// [Depth 1]
		Insert(LEXEME[t_num - 1], TOKEN[t_num - 1], depth, classEntry, 0, ofs, prm_idx, mtd_idx, mtdname);
		clsname = LEXEME[t_num - 1];

		if (match(cr_op_brkt))								// If first condition of first rule is satisfied : { detected first -> go into progress
		{
			depth++;
			VarDecl(depth, outFile);								// [Depth 2]
			MethodDecl(depth, outFile);
			match(cr_cl_brkt);
			DeleteDepth(depth);			//delete to loopup for undeclared idt : on our language, class name and fuction name can be duplicated : 2-1=1
			depth--;

			return 1;
		}

		else if (match(extendst))		// If first condition of second rule is satisfied : extends token detected first -> go into progress
		{
			t_num++;

			match(idt);					// [Depth 1]
			match(cr_op_brkt);
			depth++;
			VarDecl(depth, outFile);				// [Depth 2]
			MethodDecl(depth, outFile);
			match(cr_cl_brkt);
			DeleteDepth(depth);			//delete to loopup for undeclared idt : on our language, class name and fuction name can be duplicated : 2-1=1
			depth--;

			return 1;
		}

		else											//======================= CASE :: ERROR DETECTED

		{
			cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : " << "Error : instead of [ " << LEXEME[t_num] << " ], we expect [ extends ]" << endl;		//If current token isn't the token that I want, just print error msg
			
			exit(1);									//If error is detected, program ends
		}
	}

	else return 0;										// IF INDEX REACHED TO EOF : END FUNCTION AND DO NOTHING
}

int VarDecl(int depth, ofstream& outFile)											//:::::::::::::::::::::::::::::::::::::::::::::: VARIABLE DECLARATION GRAMMAR ::::::::::::::::::::::::::::::::::::::::::::::
{

	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == intt || TOKEN[t_num] == booleant || TOKEN[t_num] == voidt)		// If first condition of first rule is satisfied : intt || booleant || voidt is detected first -> go into progress
		{
			if (TOKEN[t_num] == intt)
			{
				var_idx = 1;
				ofs = ofs + 2;
			}
			else if (TOKEN[t_num] == booleant)
			{
				var_idx = 4;
				ofs = ofs + 1;
			}
			else if (TOKEN[t_num] == voidt)
			{
				var_idx = 5;
			}

			Type();
			IdentifierList(depth, outFile);
			match(semicolt);
			VarDecl(depth, outFile);

			return 1;
		}

		else if (strcmp(LEXEME[t_num], "final") == 0)	// If first condition of second rule is satisfied : final token detected first -> go into progress
		{
			match(finalt);
			Type();
			if (TOKEN[t_num - 1] == intt)
			{
				var_idx = 1;
				ofs = ofs + 2;
			}

			else if (TOKEN[t_num - 1] == booleant)
			{
				var_idx = 4;
				ofs = ofs + 1;
			} 

			else if (TOKEN[t_num - 1] == voidt)
			{
				var_idx = 5;
			}

			Insert(LEXEME[t_num], TOKEN[t_num], depth, constEntry, var_idx, ofs, prm_idx, mtd_idx, mtdname);
			procedure_A(outFile);
			match(idt);
			if (mtd_idx == 0)
			{
				cls_Tailing(clsname, LEXEME[t_num - 1], constEntry, var_idx);
			}
			push(mtdname);
			match(assignopt);
			match(numt);
			match(semicolt);
			VarDecl(depth, outFile);

			return 1;
		}

		else											// nullable phase
		{
			return 0;
		}
	}

	else return 0;										// IF INDEX REACHED TO EOF : END FUNCTION AND DO NOTHING
}

int IdentifierList(int depth, ofstream& outFile)									//:::::::::::::::::::::::::::::::::::::::::::::: IDENTIFIERLIST GRAMMAR ::::::::::::::::::::::::::::::::::::::::::::::
{														// I-> I,idt | idt == LEFT RECURSIVE, WE NEED TO CHANGE THIS >>>>>>>>>>>>>>>>>>>>>>> I -> idt I' && I'-> ,idt I' | lamda

	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		prm_idx = 0;

		if (TOKEN[t_num] == idt)
		{
			match(idt);
			Insert(LEXEME[t_num - 1], TOKEN[t_num - 1], depth, varEntry, var_idx, ofs, prm_idx, mtd_idx, mtdname);
			if (mtd_idx == 0)
			{
				cls_Tailing(clsname, LEXEME[t_num - 1], varEntry, var_idx);
			}
			push(mtdname);
			Identi_sub(depth, outFile);

			return 1;
		}

		else											//======================= CASE :: ERROR DETECTED
		{
			cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : " << "Error : instead of [ " << LEXEME[t_num] << " ], we expect [ idt ]" << endl;		//If current token isn't the token that I want, just print error msg

			exit(1);									//If error is detected, program ends
		}
	}

	else												// IF INDEX REACHED TO EOF : END FUNCTION AND DO NOTHING
		return 0;
}

int Identi_sub(int depth, ofstream& outFile)
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		prm_idx = 0;

		if (TOKEN[t_num] == commt)
		{
			match(commt);
			match(idt);

			if (var_idx == 1)
			{
				ofs = ofs + 2;
			}

			else if (var_idx == 4)
			{
				ofs = ofs + 1;
			}

			Insert(LEXEME[t_num - 1], TOKEN[t_num - 1], depth, varEntry, var_idx, ofs, prm_idx, mtd_idx, mtdname);
			if (mtd_idx == 0)
			{
				cls_Tailing(clsname, LEXEME[t_num - 1], varEntry, var_idx);
			}
			push(mtdname);
			Identi_sub(depth, outFile);

			return 1;
		}
		
		else											// nullable phase
		{
			if (var_idx == 1)
			{
				ofs = ofs + 2;
			}

			else if (var_idx == 4)
			{
				ofs = ofs + 1;
			}

			return 0;
		}


	}

	else												// IF INDEX REACHED TO EOF : END FUNCTION AND DO NOTHING
		return 0;
}

int Type()												//:::::::::::::::::::::::::::::::::::::::::::::: TYPE GRAMMAR ::::::::::::::::::::::::::::::::::::::::::::::
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == intt)
		{
			match(intt);

			return intt;
		}

		else if (TOKEN[t_num] == booleant)
		{
			match(booleant);

			return booleant;
		}

		else if (TOKEN[t_num] == voidt)
		{
			match(voidt);

			return voidt;
		}

		else											//======================= CASE :: ERROR DETECTED
		{
			cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : " << "Error : instead of [ " << LEXEME[t_num] << " ], we expect data type token" << endl;		//If current token isn't the token that I want, just print error msg

			exit(1);									//If error is detected, program ends
		}
	}

	else
		return 0;										// IF INDEX REACHED TO EOF : END FUNCTION AND DO NOTHING
}

// =================================== depth 2 == methodname depth

int MethodDecl(int depth, ofstream& outFile)										//:::::::::::::::::::::::::::::::::::::::::::::: METHODCECLARATION GRAMMAR ::::::::::::::::::::::::::::::::::::::::::::::
{
	char func[3];
	char code[MAXSTRING];
	Entry_ptr temp;

	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == publict)
		{
			mtd_idx = 1;
			match(publict);								// [Depth 1]

			Type();

			if (TOKEN[t_num - 1] == intt || TOKEN[t_num - 1] == booleant || TOKEN[t_num - 1] == voidt)		// To determine return type if function 
			{
				if (TOKEN[t_num - 1] == intt)
				{
					var_idx = 1;
				}
				else if (TOKEN[t_num - 1] == booleant)
				{
					var_idx = 4;
				}
				else if (TOKEN[t_num - 1] == voidt)
				{
					var_idx = 5;
				}
			}

			match(idt);	//function name
			Insert(LEXEME[t_num - 1], TOKEN[t_num - 1], depth, functionEntry, var_idx, ofs, prm_idx, mtd_idx, mtdname);
			cls_Tailing(clsname, LEXEME[t_num - 1], functionEntry, var_idx);
			mtdname = LEXEME[t_num - 1];
			strcpy(code, mtdname);
			strcat(code, "	");
			strcat(code, "Proc");
			strcat(code, "\n");
			emit(outFile, code);
			match(sm_op_brkt);
			depth++;
			FormalList(depth, outFile);							//	[Depth 2]
			strcpy(code, "		push		BP");
			strcat(code, "\n");
			strcat(code, "		mov		BP,SP");
			strcat(code, "\n");
			strcat(code, "		sub		SP,");
			prm_idx = 0;
			match(sm_cl_brkt);
			match(cr_op_brkt);
			VarDecl(depth, outFile);
			sprintf(func, "%d", Lookup(mtdname)->METHOD.SizeOfLocal);
			strcat(code, func);
			strcat(code, "\n");
			emit(outFile, code);
			SeqOfStatements(depth, outFile);
			temp = Lookup(LEXEME[t_num + 1]);
			procedure_T(temp, outFile);
			match(returnt);
			Expr(depth, outFile);
			match(semicolt);
			if (ret_idx == 1)
			{
				strcpy(code, "		mov		AX,");
				strcat(code, "[BP");
				func[0] = NULL;
				sprintf(func, "%d", Pop(RP->data_value)->Offset);
				if (Pop(RP->data_value)->Offset > 0)
				{
					strcat(code, "+");
				}
				strcat(code, func);
				strcat(code, "]");
				strcat(code, "			//Return value of method");
				strcat(code, "\n");
			}
			strcat(code, "		add		SP,");
			sprintf(func, "%d", Lookup(mtdname)->METHOD.SizeOfLocal);
			strcat(code, func);
			strcat(code, "\n");
			strcat(code, "\n");
			strcat(code, "		pop		BP");
			strcat(code, "\n");
			strcat(code, "		ret		");
			sprintf(func, "%d", (2 * Lookup(mtdname)->METHOD.NumberOFParam));
			strcat(code, func);
			strcat(code, "\n");
			strcat(code, mtdname);
			strcat(code, "	");
			strcat(code, "Endp");
			strcat(code, "\n");
			emit(outFile, code);

			match(cr_cl_brkt);
			mtd_idx = 0;
			ofs = 0;
			DeleteDepth(depth);			//delete to loopup for undeclared idt : on our language, class name and fuction name can be duplicated : 2-1=1
			if(PP != NULL)
			{
				Delete_stk(PP);
			}
			else
			{
				Delete_stk(RP);
			}
			base_set_push();
			depth--;
			MethodDecl(depth, outFile);					// 함수가 끝날때마다 오프셋이 리셋되는가

			return 1;
		}

	

		else											// nullable phase
		{
			while (depth > 1)
			{
				DeleteDepth(depth);			//delete to loopup for undeclared idt : on our language, class name and fuction name can be duplicated : 2-1=1
				depth--;
			}
			mtd_idx = 0;
			ofs = 0;

			return 0;
		}
	}

	else												// IF INDEX REACHED TO EOF : END FUNCTION AND DO NOTHING
		return 0;
}

int FormalList(int depth, ofstream& outFile)										//:::::::::::::::::::::::::::::::::::::::::::::: FORMALLIST GRAMMAR :::::::::::::::::::::::::::::::::::::::::::::://parameters
{
	prm_idx = 1;

	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == intt || TOKEN[t_num] == booleant || TOKEN[t_num] == voidt)		// If first condition of first rule is satisfied : intt || booleant || voidt is detected first -> go into progress
		{
			if (TOKEN[t_num] == intt)
			{
				ofs = ofs + 2;
				var_idx = 1;
			}

			else if(TOKEN[t_num] == booleant)
			{
				ofs = ofs + 1;
				var_idx = 4;
			}

			else if (TOKEN[t_num] == voidt)
			{
				var_idx = 5;
			}

			Type();
			match(idt);
			Insert(LEXEME[t_num - 1], TOKEN[t_num - 1], depth, varEntry, var_idx, ofs, prm_idx, mtd_idx, mtdname);
			param_dangling(mtdname, LEXEME[t_num - 1], varEntry);
			push(mtdname);
			prm_idx = 0;
			FormalRest(depth, outFile);
			prm_idx = 0;

			return 1;
		}

		else											// nullable phase
		{	
			prm_idx = 0;
			return 0;
		}
	}

	else												// IF INDEX REACHED TO EOF : END FUNCTION AND DO NOTHING
		return 0;
}

int FormalRest(int depth, ofstream& outFile)										//:::::::::::::::::::::::::::::::::::::::::::::: FORMALREST GRAMMAR ::::::::::::::::::::::::::::::::::::::::::::::
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == commt)						// If first condition of first rule is satisfied : , is detected first -> go into progress
		{
			prm_idx = 1;
			match(commt);
			
			if (TOKEN[t_num] == intt || TOKEN[t_num] == booleant || TOKEN[t_num] == voidt)		// If first condition of first rule is satisfied : intt || booleant || voidt is detected first -> go into progress
			{
				if (TOKEN[t_num] == intt)
				{
					ofs = ofs + 2;
					var_idx = 1;
				}

				else if (TOKEN[t_num] == booleant)
				{
					ofs = ofs + 1;
					var_idx = 4;
				}

				else if (TOKEN[t_num] == voidt)
				{
					var_idx = 5;
				}
			}

			Type();
			match(idt);
			Insert(LEXEME[t_num - 1], TOKEN[t_num - 1], depth, varEntry, var_idx, ofs, prm_idx, mtd_idx, mtdname);
			param_dangling(mtdname, LEXEME[t_num - 1], varEntry);
			push(mtdname);
			prm_idx = 0;
			FormalRest(depth, outFile);
			prm_idx = 0;

			return 1;
		}
		else											// nullable phase
		{
			prm_idx = 0;
			return 0;
		}
	}
	
	else												// IF INDEX REACHED TO EOF : END FUNCTION AND DO NOTHING
		return 0;
}

int SeqOfStatements(int depth, ofstream& outFile)									// Statement ; StatTail | lambda
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == idt || TOKEN[t_num] == readt || TOKEN[t_num] == writet || TOKEN[t_num] == writelnt)
		{
			Statement(depth, outFile);
			match(semicolt);
			StatTail(depth, outFile);

			return 1;
		}

		else
		{
			return 0;
		}
	}

	else
		return 0;
}							

int Expr(int depth, ofstream& outFile)												// Relation | lambda
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == idt || TOKEN[t_num] == numt || TOKEN[t_num] == sm_op_brkt || TOKEN[t_num] == exclt || TOKEN[t_num] == truet || TOKEN[t_num] == falset || TOKEN[t_num] == addopt)
		{
			Relation(depth, outFile);

			return 1;
		}

		else
		{
			return 0;
		}
	}

	else
		return 0;
}

int Statement(int depth, ofstream& outFile)										// AssignStat | IOStat
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		
		if (TOKEN[t_num] == readt || TOKEN[t_num] == writet || TOKEN[t_num] == writelnt)
		{
			IOStat(depth, outFile);

			return 1;
		}

		else if (TOKEN[t_num] == idt)
		{
			if (Lookup(LEXEME[t_num])->TypeOfEntry == varEntry || Lookup(LEXEME[t_num])->TypeOfEntry == classEntry)
			{
				AssignStat(depth, outFile);
			}

			return 1;
		}		

		else
		{
			cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : " << "Error : instead of [ " << LEXEME[t_num] << " ], we expect [ idt ] or [ readt ] or [writet] or [writelnt] or [Classname]" << endl;		//If current token isn't the token that I want, just print error msg

			exit(1);

			return 0;
		}

	}

	else
		return 0;
}

int StatTail(int depth, ofstream& outFile)											// This grammar is right for now but logically wrong _4/8		: Statement ; StatTail | lambda
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == idt || TOKEN[t_num] == readt || TOKEN[t_num] == writet || TOKEN[t_num] == writelnt)
		{
			Statement(depth, outFile);
			match(semicolt);
			StatTail(depth, outFile);

			return 1;
		}

		else
		{
			return 0;
		}
	}

	else
		return 0;
}

int AssignStat(int depth, ofstream& outFile)									// idt = Expr
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == idt)
		{
			if (TOKEN[t_num + 1] == assignopt)
			{
				if (TOKEN[t_num + 3] != punct)
				{
					procedure_A(outFile);
				}
				match(idt);

				if (Lookup(LEXEME[t_num - 1]) == NULL)
				{
					cout << "Token # " << t_num - 1 << " : " << "Line # " << LINE[t_num - 1] << " : [" << LEXEME[t_num - 1] << "] : " << "[Error] Undeclared identifier token detected" << endl;			//Undeclared token detected

					exit(1);

					return 0;
				}

				else if (Lookup(LEXEME[t_num - 1])->Depth != depth)
				{
					if (Lookup(LEXEME[t_num - 1])->Depth != 2)																									//********************if we cannot use the class declared vars in method is class, delete this!!!
					{
						cout << "Token # " << t_num - 1 << " : " << "Line # " << LINE[t_num - 1] << " : [" << LEXEME[t_num - 1] << "] : " << "[Error] Undeclared identifier token detected" << endl;			//Undeclared token detected

						exit(1);

						return 0;
					}
				}

				match(assignopt);

				if (TOKEN[t_num] == idt)
				{
					if (TOKEN[t_num + 1] == punct)
					{
						MethodCall(depth, outFile);

						return 1;
					}

					else
					{
						Expr(depth, outFile);

						return 1;
					}
				}

				else if (TOKEN[t_num] == numt || TOKEN[t_num] == sm_op_brkt || TOKEN[t_num] == exclt || TOKEN[t_num] == truet || TOKEN[t_num] == falset || TOKEN[t_num] == addopt)
				{
					Expr(depth, outFile);

					return 1;
				}

			}

			else if (TOKEN[t_num + 1] == punct)
			{
				MethodCall(depth, outFile);

				return 1;
			}

			else
			{
				cout << "Token # " << t_num + 1 << " : " << "Line # " << LINE[t_num + 1] << " : " << "Error : instead of [ " << LEXEME[t_num + 1] << " ], we expect [ = ] or [ . ]" << endl;		//If current token isn't the token that I want, just print error msg

				exit(1);

				return 0;
			}
		}

		else
		{
			cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : " << "Error : instead of [ " << LEXEME[t_num] << " ], we expect [ Identifier token ]" << endl;		//If current token isn't the token that I want, just print error msg

			exit(1);

			return 0;
		}
	}

	else return 0;
}

int IOStat(int depth, ofstream& outFile)										// In_Stat | Out_Stat
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == readt)
		
		{
			In_Stat(depth, outFile);

			return 1;
		}

		else if (TOKEN[t_num] == writet || TOKEN[t_num] == writelnt)
		{
			Out_Stat(depth, outFile);

			return 1;
		}

		else
		{
			cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : [" << LEXEME[t_num] << "] : " << "[Error] We expect read or write, not : " << LEXEME[t_num] << endl;			//Undeclared token detected

			exit(1);
		}
	}

	else return 0;
}
	
int Relation(int depth, ofstream& outFile)										//	SimpleExpr
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		SimpleExpr(depth, outFile);

		return 1;
	}

	else
		return 0;
}

int SimpleExpr(int depth, ofstream& outFile)									// Term MoreTerm
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		Term(depth, outFile);
		MoreTerm(depth, outFile);

		return 1;
	}

	else
		return 0;
}

int Term(int depth, ofstream& outFile)											// Factor MoreFactor
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		Factor(depth, outFile);
		MoreFactor(depth, outFile);

		return 1;
	}

	else
		return 0;
}

int MoreTerm(int depth, ofstream& outFile)										// Addop Term MoreTerm | lambda
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == addopt)
		{
			Addop(depth, outFile);
			Term(depth, outFile);
			MoreTerm(depth, outFile);

			return 1;
		}

		else
		{
			return 0;
		}
	}

	else
		return 0;
}

int Factor(int depth, ofstream& outFile)					// id | num | (Expr) | !Factor | SignOp Factor | true | false
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == idt)
		{
			match(idt);

			if (Lookup(LEXEME[t_num - 1]) == NULL)
			{
				cout << "Token # " << t_num - 1 << " : " << "Line # " << LINE[t_num - 1] << " : [" << LEXEME[t_num - 1] << "] : " << "[Error] Undeclared identifier token detected" << endl;			//Undeclared token detected

				exit(1);
			}

			else if (Lookup(LEXEME[t_num - 1])->Depth != depth)
			{
				if (Lookup(LEXEME[t_num - 1])->Depth != 2)																									//********************if we cannot use the class declared vars in method is class, delete this!!!
				{
					cout << "Token # " << t_num - 1 << " : " << "Line # " << LINE[t_num - 1] << " : [" << LEXEME[t_num - 1] << "] : " << "[Error] Undeclared identifier token detected" << endl;			//Undeclared token detected

					exit(1);
				}
			}

			return 1;
		}
		else if (TOKEN[t_num] == numt)
		{
			match(numt);

			return 1;
		}
		else if (TOKEN[t_num] == sm_op_brkt)
		{
			match(sm_op_brkt);
			Expr(depth, outFile);
			match(sm_cl_brkt);

			return 1;
		}
		else if (TOKEN[t_num] == addopt && LEXEME[t_num] == "-")
		{
			match(addopt);
			Factor(depth, outFile);

			return 1;
		}
		else if (TOKEN[t_num] == exclt)
		{
			match(exclt);
			Factor(depth, outFile);

			return 1;
		}
		else if (TOKEN[t_num] == truet)
		{
			match(truet);

			return 1;
		}
		else if (TOKEN[t_num] == falset)
		{
			match(falset);

			return 1;
		}
	}

	else
		return 0;
}

int MoreFactor(int depth, ofstream& outFile)							// Mulop Factor MoreFactor | lambda
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == mulopt)
		{
			Mulop(depth, outFile);
			Factor(depth, outFile);
			MoreFactor(depth, outFile);

			return 1;
		}

		else
		{
			return 0;
		}
	}

	else
		return 0;
}

int Addop(int depth, ofstream& outFile)							// + | - | ||
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		match(addopt);

		return 1;
	}

	else
		return 0;
}

int Mulop(int depth, ofstream& outFile)							// * | / | &&
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		match(mulopt);

		return 1;
	}

	else
		return 0;
}

int Signop(int depth, ofstream& outFile)							// -
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (LEXEME[t_num] == "-")
		{
			match(mulopt);

			return 1;
		}

		else
		{
			cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : " << "Error : instead of [ " << LEXEME[t_num] << " ], we expect [ - ]" << endl;		//If current token isn't the token that I want, just print error msg

			exit(1);
		}
	}

	else return 0;
}

int MethodCall(int depth, ofstream& outFile)
{
	int i, idx = 0;
	char digit[3];
	char* dg, *cg;

	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num - 1] == assignopt)
		{
			idx = 623;
			cg = (char *)malloc(2);
			strcpy(cg, LEXEME[t_num - 2]);
		}

		if (TOKEN[t_num] == idt)
		{
			ClassName(depth, outFile);
			match(punct);

			for (i = 0; i < 10; i++)
			{
				par_stk[i] = NULL;
			}

			match(idt);
			mtdname_ = LEXEME[t_num - 1];
			match(sm_op_brkt);
			Params(depth, outFile);
			match(sm_cl_brkt);
			i = 0;
			while (par_stk[i] != NULL)
			{
				i++;
			}
			i--;

			while (i >= 0)
			{
				sprintf(digit, "%d", Pop(par_stk[i])->Offset);
				cout << "		push		[BP" << digit << "]" << endl;
				outFile << "		push		[BP" << digit << "]" << endl;
				i--;
			}

			cout << "		call " << mtdname_ << endl;
			outFile << "		call " << mtdname_ << endl;

			if (idx == 623)
			{
				cout << "		mov		[BP" << Pop(cg)->Offset << "],_AX" << endl;
				outFile << "		mov		[BP" << Pop(cg)->Offset << "],_AX" << endl;
			}

		}

		else
		{
			cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : " << "Error : instead of [ " << LEXEME[t_num] << " ], we expect [ Identifier token ]" << endl;		//If current token isn't the token that I want, just print error msg

			exit(1);
		}
	}

	else return 0;
}

int ClassName(int depth, ofstream& outFile)
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == idt)
		{
			if (Lookup(LEXEME[t_num])->Depth == 1)
			{
				match(idt);
			}

			else
			{
				cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : [" << LEXEME[t_num] << "] : " << "[Error] Undeclared Class name" << endl;			//Undeclared token detected

				exit(1);
			}
		}

		else
		{
			cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : " << "Error : instead of [ " << LEXEME[t_num] << " ], we expect [ Identifier token ]" << endl;		//If current token isn't the token that I want, just print error msg

			exit(1);
		}
	}

	else return 0;
}

int Params(int depth, ofstream& outFile)
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == idt)
		{

			if (Lookup(LEXEME[t_num]) == NULL)
			{
				cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : [" << LEXEME[t_num] << "] : " << "[Error] Undeclared identifier token detected" << endl;			//Undeclared token detected

				exit(1);
			}

			else if (Lookup(LEXEME[t_num])->Depth != depth)
			{
				cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : [" << LEXEME[t_num] << "] : " << "[Error] Undeclared identifier token detected" << endl;			//Undeclared token detected

				exit(1);
			}

			match(idt);
			//push(LEXEME[t_num - 1]);
			par_stk[par_s] = (char *)malloc(strlen(LEXEME[t_num - 1] + 1));
			strcpy(par_stk[par_s], LEXEME[t_num - 1]);
			par_s++;

			ParamsTail(depth, outFile);
		}

		else if (TOKEN[t_num] == numt)
		{
			match(numt);
			// If we need this part, work on later
			ParamsTail(depth, outFile);
		}

		else
		{
			return 0;
		}
	}

	else return 0;
}

int ParamsTail(int depth, ofstream& outFile)
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == commt)
		{
			match(commt);

			if (TOKEN[t_num] == idt)
			{

				if (Lookup(LEXEME[t_num]) == NULL)
				{
					cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : [" << LEXEME[t_num] << "] : " << "[Error] Undeclared identifier token detected" << endl;			//Undeclared token detected

					exit(1);
				}

				else if (Lookup(LEXEME[t_num])->Depth != depth)
				{
					if (Lookup(LEXEME[t_num])->Depth != 2)																									//********************if we cannot use the class declared vars in method is class, delete this!!!
					{
						cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : [" << LEXEME[t_num] << "] : " << "[Error] Undeclared identifier token detected" << endl;			//Undeclared token detected

						exit(1);
					}
				}

				match(idt);
				//push(LEXEME[t_num - 1]);
				par_stk[par_s] = (char *)malloc(strlen(LEXEME[t_num - 1] + 1));
				strcpy(par_stk[par_s], LEXEME[t_num - 1]);
				par_s++;
				ParamsTail(depth, outFile);
			}

			else if (TOKEN[t_num] == numt)
			{
				match(numt);
				ParamsTail(depth, outFile);
			}
		}

		else
		{
			return 0;
		}
	}

	else return 0;
}

int In_Stat(int depth, std::ofstream& outFile)
{
	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		match(readt);
		match(sm_op_brkt);
		Id_List(depth, outFile);
		match(sm_cl_brkt);
	}

	else return 0;
}

int Id_List(int depth, std::ofstream& outFile)
{
	char code[MAXSTRING];
	char digit[3];

	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		match(idt);
		if (TOKEN[t_num - 1] == idt)
		{
			strcpy(code, "		mov		AX,");
			strcat(code, "[BP");
			sprintf(digit, "%d", Pop(LEXEME[t_num - 1])->Offset);
			strcat(code, digit);
			strcat(code, "]");
			strcat(code, "\n");
			strcat(code, "		call readint");
			strcat(code, "\n");
		}
		emit(outFile, code);
		Id_List_Tail(depth, outFile);

		return 1;
	}

	else return 0;
}

int Id_List_Tail(int depth, std::ofstream& outFile)
{
	char code[MAXSTRING];
	char digit[3];

	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == commt)
		{
			match(commt);
			match(idt);
			if (TOKEN[t_num - 1] == idt)
			{
				strcpy(code, "		mov		AX,");
				strcat(code, "[BP");
				sprintf(digit, "%d", Pop(LEXEME[t_num - 1])->Offset);
				strcat(code, digit);
				strcat(code, "]");
				strcat(code, "\n");
				strcat(code, "		call readint");
				strcat(code, "\n");
			}
			emit(outFile, code);
			Id_List_Tail(depth, outFile);

			return 1;
		}

		else
		{
			return 0;
		}
	}

	else return 0;
}

int Out_Stat(int depth, std::ofstream& outFile)
{
	char code[MAXSTRING];
	char inttt[4];

	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == writet)
		{
			match(writet);
			match(sm_op_brkt);
			Write_List(depth, outFile);
			if (TOKEN[t_num - 1] == idt)
			{
				strcpy(code, "		mov		AX,");
				strcat(code, "[BP");
				sprintf(inttt, "%d", Pop(LEXEME[t_num - 1])->Offset);
				strcat(code, inttt);
				strcat(code, "]");
				strcat(code, "\n");
				strcat(code, "		call writeint");
				strcat(code, "\n");
			}
			else if (TOKEN[t_num - 1] == numt)
			{
				strcpy(code, "		mov		AX,");
				sprintf(inttt, "%d", w_iint);
				strcat(code, inttt);
				strcat(code, "\n");
				strcat(code, "		call writeint");
				strcat(code, "\n");
			}
			else if (TOKEN[t_num - 1] == literalt)
			{
				strcpy(code, "		mov		dx,");
				strcat(code, "BP");
				sprintf(inttt, "%d", Pop(LEXEME[t_num - 1])->Offset);
				strcat(code, inttt);
				strcat(code, "\n");
				strcat(code, "		call writestr");
				strcat(code, "\n");
			}
			emit(outFile, code);
			match(sm_cl_brkt);
			return 1;
		}

		else if (TOKEN[t_num] == writelnt)
		{
			match(writelnt);
			match(sm_op_brkt);
			Write_List(depth, outFile);
			if (TOKEN[t_num - 1] == idt)
			{
				strcpy(code, "		mov		AX,");
				strcat(code, "[BP");
				sprintf(inttt, "%d", Pop(LEXEME[t_num - 1])->Offset);
				strcat(code, inttt);
				strcat(code, "]");
				strcat(code, "\n");
				strcat(code, "		call writeint");
				strcat(code, "\n");
			}
			else if (TOKEN[t_num - 1] == numt)
			{
				strcpy(code, "		mov		AX,");
				sprintf(inttt, "%d", w_iint);
				strcat(code, inttt);
				strcat(code, "\n");
				strcat(code, "		// this push ");
				strcat(code, "\n");
			}
			else if (TOKEN[t_num - 1] == literalt)
			{
				strcpy(code, "		mov		dx,");
				strcat(code, "BP");
				sprintf(inttt, "%d", Pop(LEXEME[t_num - 1])->Offset);
				strcat(code, inttt);
				strcat(code, "\n");
				strcat(code, "		call writestr");
				strcat(code, "\n");
			}
			strcat(code, "		call writeln");
			strcat(code, "\n");
			emit(outFile, code);
			match(sm_cl_brkt);
			return 1;
		}

	}

	else return 0;
}

int Write_List(int depth, std::ofstream& outFile)
{

	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		Write_Token(depth, outFile);
		Write_List_Tail(depth, outFile);

		return 1;
	}

	else return 0;
}

int Write_List_Tail(int depth, std::ofstream& outFile)
{

	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{
		if (TOKEN[t_num] == commt)
		{
			match(commt);
			Write_Token(depth, outFile);
			Write_List_Tail(depth, outFile);

			return 1;
		}

		else
		{
			return 0;
		}
	}

	else return 0;
}

int Write_Token(int depth, std::ofstream& outFile)
{
	char code[MAXSTRING];
	char home[4];

	if (TOKEN[t_num] != eoft)							// To double check that file is read completely and not to read the array after eof token it may occur pointer error
	{

		if (TOKEN[t_num] == idt)
		{
			match(idt);
			wrI++;
		/*	sprintf(home, "%d", Pop(LEXEME[t_num - 1])->Offset);
			strcpy(code, "		mov		AX,[BP");
			strcat(code, home);
			strcat(code, "]");
			strcat(code, "\n");
			emit(outFile, code);*/

			return idt;
		}

		else if (TOKEN[t_num] == numt)
		{
			match(numt);
			wrI++;
			w_iint = VALUE[t_num - 1];
			/*sprintf(home, "%d", VALUE[t_num - 1]);
			strcpy(code, "		mov		AX,[BP");
			strcat(code, home);
			strcat(code, "]");
			strcat(code, "\n");
			emit(outFile, code);*/

			return numt;
		}

		else if (TOKEN[t_num] == literalt)
		{
			match(literalt);
			Insert(LEXEME[t_num - 1], TOKEN[t_num - 1], depth, varEntry, 7, ofs, prm_idx, mtd_idx, mtdname);
			push(mtdname);
			wrS++;
			/*sprintf(home, "%d", Pop(LEXEME[t_num - 1])->Offset);
			strcpy(code, "		mov		dx,BP");
			strcat(code, home);
			strcat(code, "\n");
			emit(outFile, code);*/
			return literalt;
		}
	}

	else
	{
		return 0;
	}
}

void base_set_push()
{
	SP = new STK;

	SP->data_value = "Return value";
	SP->Offset = 2;
	SP->next_S = NULL;

	RP = SP;

	PP = NULL;

	SP = new STK;

	RP->next_S = SP;

	SP->data_value = "Old BP";
	SP->Offset = 0;
	SP->next_S = NULL;

	BP = SP;
}

void push(char *mtd_name)
{
	int offs=0;
	char* var = (char*)malloc(50);
	Push_ptr bridge, temp;
	int i, prm_num = 0, prm_size = 0;
	Entry_ptr right_now = NULL;
	Param_ptr pr = NULL;


	strcpy(var, LEXEME[t_num - 1]);

	if (prm_idx == 0)
	{

		SP = new STK;
		right_now = Lookup(mtd_name);					// To look at first node together

		if (right_now != NULL)					// If entry is empty, do nothing and go into next for loop but if we have something in that entry list,
		{											// start to find all the elements that has same depth in that entry
			if (right_now->Depth == ((Lookup(var)->Depth) - 1))			// If we found same depth)
			{

				if (right_now->TypeOfEntry == functionEntry)
				{
					prm_num = right_now->METHOD.NumberOFParam;
					pr = right_now->METHOD.ParamList;

					while (pr)
					{
						if (pr->TypeOfParam == intType)
						{
							prm_size = prm_size + 2;
						}

						else if (pr->TypeOfParam == floatType)
						{
							prm_size = prm_size + 4;
						}

						else if (pr->TypeOfParam == charType)
						{
							prm_size = prm_size + 1;
						}

						else if (pr->TypeOfParam == boolType)
						{
							prm_size = prm_size + 1;
						}

						pr = pr->NEXT;

					}

				}

			}
			right_now = right_now->NEXT;
		}



		if (Lookup(var)->TypeOfEntry == constEntry)
		{
			offs = -(Lookup(var)->CONSTANT.Offset - prm_size);
		}

		else if (Lookup(var)->TypeOfEntry == varEntry)
		{
			offs = -(Lookup(var)->VAR.Offset - prm_size);
		}

		bridge = BP;

		while (bridge != NULL)
		{
			temp = bridge;
			bridge = bridge->next_S;
		}

		SP->data_value = (char *)malloc(50);
		strcpy(SP->data_value, var);
		SP->Offset = offs;
		SP->next_S = NULL;
		temp->next_S = SP;



	}

	else if (prm_idx == 1)
	{
		if (PP == NULL)
		{
			bridge = RP;
		}
		else
		{
			bridge = PP;
		}

		SP = new STK;

		if (Lookup(var)->TypeOfEntry == constEntry)
		{
			offs = 2 + Lookup(var)->CONSTANT.Offset;
		}

		else if (Lookup(var)->TypeOfEntry == varEntry)
		{
			offs = 2 + Lookup(var)->VAR.Offset;
		}

		SP->data_value = var;
		SP->Offset = offs;
		SP->next_S = bridge;
		PP = SP;

	}


}

Push_ptr Pop(char* name)
{
	Push_ptr find;

	if (PP != NULL)
	{
		find = PP;
	}

	else
	{
		find = BP;
	}


	if (strcmp(find->data_value, name) == 0)
	{
		return find;
	}

	else
	{
		while (strcmp(find->data_value, name) != 0)
		{
			find = find->next_S;
		}
		return find;
	}
}

Push_ptr push_TAC(char* ac)
{
	Push_ptr take_offset, put_it_in;
	char * value = (char *)malloc(50);
	int size;

	strcpy(value, ac);

	put_it_in = new STK;
	take_offset = BP;

	while (take_offset->next_S)
	{
		take_offset = take_offset->next_S;
	}

	size = take_offset->Offset;

	put_it_in->data_value = value;
	put_it_in->Offset = size - 2;
	put_it_in->next_S = NULL;

	take_offset->next_S = put_it_in;
	SP = take_offset->next_S;

	//cout << " ********** Push [ " << SP->data_value << " ] : offset [ " << SP->Offset << " ]" << endl << endl;

	return SP;
}

void print_test()
{
	Push_ptr temp;
	char* i;

	if (PP != NULL)
	{
		temp = PP;
	}

	else
	{
		temp = RP;
	}

	cout << endl << "<<<<<<<<<<<<<<< STACK >>>>>>>>>>>>>>>" << endl << endl;

	cout << "Ret value : " << RP->data_value << "     Stack Offset : " << RP->Offset << endl;
	cout << "BP value : " << BP->data_value << "     Stack Offset : " << BP->Offset << endl << endl;

	while (temp)
	{
		cout << "Push value : " << temp->data_value << "     Stack Offset : " << temp->Offset << endl;
		temp = temp->next_S;
	}

	cout << endl << "=> Stack Pointer now point out : " << SP->data_value << "     Stack Offset : " << SP->Offset << endl;
}

void emit(ofstream& outFile , char *code) // code : TAC
{
	int i;

	if (line < 20) {

		cout<<code<<endl;
		outFile << code << endl;
	}

	else {
		cout << endl << "Please press enter twice to continue" << endl;
		cin.ignore();
		cin.get();
		line = 1;
		cout << code << endl;
		outFile << code << endl;
	}



	code[0] = NULL; //clear the string


	line++;

}

void procedure_A(ofstream& outFile)		//idt=Expr | idt=Method call
{
	Entry_ptr Eplace;
	Entry_ptr idptr;
	Push_ptr tmpptr;

	char code[MAXSTRING];
	char offs[3];
	int i;

	for (i = 0; i < 3; i++)
	{
		offs[i] = NULL;
	}

	
	ProcR_idx = 0;
	t_numback = 0;

	idptr = Lookup(LEXEME[t_num]);		  //save 
										  //pointer to current lexeme

	match(idt);
	t_numback++;
	match(assignopt);
	t_numback++;

	procedure_E(Eplace, outFile); //synthesize a ptr      
								  //to expression
	if (ProcR_idx == 0)		// There is no calculation but just assign number to var
	{
		tmpptr = newtemp();

		strcpy(code,"		mov		");
		strcat(code, "AX");
		strcat(code, ",");

		if (Eplace->Token == numt)
		{
			strcat(code, Eplace->LEX); //numt or idt from right side of idt = Expr
		}

		else if (Eplace->Token == idt)
		{
			strcat(code, "[BP");
			sprintf(offs, "%d", Pop(Eplace->LEX)->Offset);

			if (Pop(Eplace->LEX)->Offset > 0)
			{
				strcat(code, "+");
			}
			strcat(code, offs);
			strcat(code, "]");
		}
		emit(outFile, code);

		strcpy(code, "		mov		");
		strcat(code, "[BP");
		sprintf(offs, "%d", tmpptr->Offset);
		strcat(code, offs);
		strcat(code, "],");
		strcat(code, "AX");
		strcat(code, "			//");
		strcat(code, "_bp");
		strcat(code, offs);
		strcat(code, " = ");
		if (Eplace->Token == numt)
		{
			strcat(code, Eplace->LEX); //numt or idt from right side of idt = Expr
		}

		else if (Eplace->Token == idt)
		{
			strcat(code, "_bp");
			sprintf(offs, "%d", Pop(Eplace->LEX)->Offset);
			strcat(code, offs);

		}
		emit(outFile, code);

		strcpy(code, "		mov		");
		strcat(code, "AX");
		strcat(code, ",");
		strcat(code, "[BP");
		sprintf(offs, "%d", tmpptr->Offset);
		strcat(code, offs);
		strcat(code, "]");
		emit(outFile, code);

		strcpy(code, "		mov		");
		strcat(code, "[BP");
		sprintf(offs, "%d", Pop(idptr->LEX)->Offset);
		strcat(code, offs);
		strcat(code, "],AX");
		strcat(code, "			//");
		strcat(code, "_bp");
		strcat(code, offs);
		strcat(code, " = ");
		strcat(code, "_bp");
		sprintf(offs, "%d", tmpptr->Offset);
		strcat(code, offs);

		emit(outFile, code);




		/*strcpy(code, "_bp");
		sprintf(offs, "%d", tmpptr->Offset);
		strcat(code, offs);     
		
		strcat(code, " = ");
		if (Eplace->Token == numt)
		{
			strcat(code, Eplace->LEX); //numt or idt from right side of idt = Expr
		}
		else if (Eplace->Token == idt)
		{
			strcat(code, "_bp+");
			sprintf(offs, "%d", Pop(Eplace->LEX)->Offset);
			strcat(code, offs);
			
		}
		strcat(code, "			//");	//-------------------------------------
		strcat(code, tmpptr->data_value);
		strcat(code, " = ");
		strcat(code, Eplace->LEX); //numt or idt from right side of idt = Expr
		emit(outFile, code);


		strcpy(code, "_bp-");
		sprintf(offs, "%d", idptr->VAR.Offset);
		strcat(code, offs);
		
		strcat(code, " = ");
		strcat(code, "_bp");
		sprintf(offs, "%d", tmpptr->Offset);
		strcat(code, offs);
		
		strcat(code, "			");	//-------------------------------------
		strcat(code, idptr->LEX);
		strcat(code, " = ");
		strcat(code, tmpptr->data_value); //numt or idt from right side of idt = Expr
		strcat(code, "\n");
		emit(outFile, code);*/

	}

	else if (ProcR_idx == 1)	// There are some calculations
	{

		strcpy(code, "		mov		[BP");
		sprintf(offs, "%d", Pop(idptr->LEX)->Offset);
		if (Pop(idptr->LEX)->Offset > 0)
		{
			strcat(code, "+");
		}
		strcat(code, offs);
		strcat(code, "],");
		strcat(code, "AX");

		emit(outFile, code);
		/*
		strcpy(code, "_bp-");
		sprintf(offs, "%d", idptr->VAR.Offset);
		strcat(code, offs);

		strcat(code, " = ");
		strcat(code, "_bp");
		sprintf(offs, "%d", TP->Offset);
		strcat(code, offs);

		strcat(code, "			");	//-------------------------------------
		strcat(code, idptr->LEX);
		strcat(code, "=");
		strcat(code, TP->data_value); //numt or idt from right side of idt = Expr
		strcat(code, "\n");
		emit(outFile, code);*/

	}

	t_num = t_num - t_numback;
}

void procedure_E(Entry_ptr& Eplace, ofstream& outFile)	// E -> T R
{
	Entry_ptr Tplace;

	procedure_T(Tplace, outFile);//synthesize first location
	procedure_R(Tplace, outFile);//now let R inherit and the
			  //return synthesized attribute
	Eplace = Tplace; // now return Eplace
}

void procedure_T(Entry_ptr& Tplace, ofstream& outFile)	//	idt | numt | lparen
{
	char *home;
	char code[MAXSTRING];
	Entry_ptr num_temp;
	
	num_temp = new TableEntry;
	//num_temp address is possible to use not to dangle to anything?

	switch (TOKEN[t_num]) {

	case idt:   
		Tplace = Lookup(LEXEME[t_num]);
		match(idt);
		t_numback++;

		break;

	case numt:
		if (VALUE[t_num] != NULL)
		{
			home = (char *)malloc(20);                    // =================== Assume that maximum is 20
			sprintf(home, "%d", VALUE[t_num]);
			num_temp->LEX = home;
			num_temp->Token = numt;
			Tplace = num_temp;// =================== 4.22
			match(numt);
			t_numback++;
		}

		else if (VALUER[t_num] != NULL)
		{
			home = (char *)malloc(20);
			gcvt(VALUER[t_num], 5, home);
			num_temp->LEX = home;
			Tplace = num_temp;
			match(numt);
			t_numback++;
		}

		break;

	case sm_op_brkt:										

		match(sm_op_brkt);
		t_numback++;
		procedure_E(Tplace, outFile);
		match(sm_cl_brkt);
		t_numback++;
		break;

	case addopt:											

		minus_idx = 1;
		Tplace = Lookup(LEXEME[t_num+1]);
		procedure_R(Tplace, outFile);
		break;

	case returnt:								// add return case to show function's return value stored in BP

		match(returnt);
		if (TOKEN[t_num] == semicolt)
		{
			t_num = t_num - 1;
			ret_idx = 0;

			break;
		}

		else if (TOKEN[t_num] == idt)
		{
			RP->data_value = Tplace->LEX;
			/*strcpy(code, "_bp+");
			home = (char *)malloc(3);	// If we need to assign more space here, fix it
			sprintf(home, "%d", RP->Offset);
			strcat(code, home);
			strcat(code, " = ");
			strcat(code, "_bp-");
			if (Lookup(LEXEME[t_num])->TypeOfEntry == varEntry)
			{
				home = (char *)malloc(3);	// If we need to assign more space here, fix it
				sprintf(home, "%d", Lookup(LEXEME[t_num])->VAR.Offset);
				strcat(code, home);
			}

			else if (Lookup(LEXEME[t_num])->TypeOfEntry == constEntry)
			{
				home = (char *)malloc(3);
				sprintf(home, "%d", Lookup(LEXEME[t_num])->CONSTANT.Offset);
				strcat(code, home);
			}

			strcat(code, "			//");
			strcat(code, "Ret = ");
			strcat(code, RP->data_value);
			strcat(code, "\n");
			emit(outFile, code);*/
			ret_idx = 1;

			t_num = t_num - 1;

			break;
		}

	default:	   
		cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : [" << LEXEME[t_num] << "] : " << "[Error] You should put idt or numt or ( here" << endl;			//Undeclared token detected
		break;
	}
}

void procedure_R(Entry_ptr& Rplace, ofstream& outFile)	// addopt TR | lambda
{														// mulopt TR | lambda
	Entry_ptr Tplace;
	Push_ptr tmpptr;
	char *temp = NULL;
	char offs[3];
	char adop[2];
	char code[MAXSTRING];
	int digit, i;
	

	for (i = 0; i < 3; i++)
	{
		offs[i] = NULL;
	}


	if (TOKEN[t_num] == addopt) {

		if (TOKEN[t_num+2]==mulopt)
		{	// our original t_num point : idt before addopt
			i = t_num; // Remember original place
			t_num++; // idt after addopt
			Tplace = Lookup(LEXEME[t_num]);//inherit that idt's pointer
			t_num++; // mulopt
			procedure_R(Tplace, outFile); // It will calculate the Tplace's idt * idt
			t_num = i;	// Restore original value
			procedure_M(Tplace, outFile);

		}

		else if (minus_idx == 1 && strcmp(LEXEME[t_num], "-") == 0)
		{
			ProcR_idx = 1;

			//cout << "[CAME IN RIGHT POSITION FOR MINUS]" << endl;

			tmpptr = newtemp();
			temp = tmpptr->data_value;

			strcpy(code, "_bp");
			sprintf(offs, "%d", tmpptr->Offset);
			strcat(code, offs);			
			strcat(code, " = ");
			strcat(code, LEXEME[t_num]);//addopt "-"
			if (Rplace->Token == idt)
			{
				if (Pop(Rplace->LEX)->Offset > 0)
				{
					strcat(code, "_bp+");
				}

				else if (Pop(Rplace->LEX)->Offset < 0)
				{
					strcat(code, "_bp-");
				}

				if (Rplace->TypeOfEntry == varEntry)
				{
					digit = Rplace->VAR.Offset;
					sprintf(offs, "%d", digit);
					strcat(code, offs);

				}
				else if (Rplace->TypeOfEntry == constEntry)
				{
					digit = Rplace->CONSTANT.Offset;
					sprintf(offs, "%d", digit);
					strcat(code, offs);
				}
				
			}
			else if (TOKEN[t_num-1] == numt)
			{
				if (VALUE[t_num-1] != NULL)
				{
					sprintf(offs, "%d", VALUE[t_num - 1]);
					strcat(code, offs);
					
				}
				// IF WE NEED TO HANDLE FLOAT, ADD THE CODE TO HERE
			}
			strcat(code, "		//");	//-------------------------------------
			strcat(code, tmpptr->data_value);
			strcat(code, "=");
			strcat(code, LEXEME[t_num]);//addop
			match(addopt);
			t_numback++;
			strcat(code, Rplace->LEX);//add idt or numt after addopt
			match(idt);
			t_numback++;
			Rplace = Lookup(temp); //Let R inherit
			TP = tmpptr;
			emit(outFile, code);
			procedure_R(Rplace, outFile);//when we finally return
										 //Rplace will hold final
										 //synthesized attribute

			minus_idx = 0;
		}

		else
		{
			ProcR_idx = 1;
			tmpptr = newtemp();
			temp = tmpptr->data_value;
			//assumes newtemp sets class etc.

			strcpy(code, "		mov		");
			strcat(code, "AX");
			strcat(code, ",");
			if (Rplace->Token == idt)
			{
				if (Pop(Rplace->LEX)->Offset > 0)
				{
					strcat(code, "[BP+");
				}
				else if (Pop(Rplace->LEX)->Offset < 0)
				{
					strcat(code, "[BP");
				}
				digit = Pop(Rplace->LEX)->Offset;
				sprintf(offs, "%d", digit);
				strcat(code, offs);
				strcat(code, "]");

			}
			else if (Rplace->Token == numt)
			{
				if (VALUE[t_num - 1] != NULL)
				{
					sprintf(offs, "%d", VALUE[t_num - 1]);
					strcat(code, offs);

				}
				// IF WE NEED TO HANDLE FLOAT, ADD THE CODE TO HERE
			}
			emit(outFile, code);

			strcpy(code, "		add		");
			strcat(code, "AX");
			strcat(code, ",");

			match(addopt);
			t_numback++;
			procedure_T(Tplace, outFile);

			if (Tplace->Token == idt)
			{
				if (Pop(Tplace->LEX)->Offset > 0)
				{
					strcat(code, "[BP+");
				}
				else if (Pop(Tplace->LEX)->Offset < 0)
				{
					strcat(code, "[BP");
				}
				digit = Pop(Tplace->LEX)->Offset;
				sprintf(offs, "%d", digit);
				strcat(code, offs);
				strcat(code, "]");
			}
			else if (Tplace->Token == numt)
			{
				if (VALUE[t_num - 1] != NULL)
				{
					sprintf(offs, "%d", VALUE[t_num - 1]);
					strcat(code, offs);
				}
				// IF WE NEED TO HANDLE FLOAT, ADD THE CODE TO HERE
			}

			emit(outFile, code);

			strcpy(code, "		mov		");
			strcat(code, "[BP");
			sprintf(offs, "%d", tmpptr->Offset);
			strcat(code, offs);
			strcat(code, "],");
			strcat(code, "AX");
			emit(outFile, code);

			strcpy(code, "		mov		");
			strcat(code, "AX,");
			sprintf(offs, "%d", tmpptr->Offset);
			strcat(code, "[BP");
			strcat(code, offs);
			strcat(code, "]");
			emit(outFile, code);

	/*		strcpy(code, "_bp");
			sprintf(offs, "%d", tmpptr->Offset);
			strcat(code, offs);
			
			strcat(code, " = ");
			if (Rplace->Token == idt)
			{
				if (Pop(Rplace->LEX)->Offset > 0)
				{
					strcat(code, "_bp+");
				}
				else if (Pop(Rplace->LEX)->Offset < 0)
				{
					strcat(code, "_bp-");
				}
				if (Rplace->TypeOfEntry == varEntry)
				{
					digit = Rplace->VAR.Offset;
					sprintf(offs, "%d", digit);
					strcat(code, offs);
				}
				else if (Rplace->TypeOfEntry == constEntry)
				{
					digit = Rplace->CONSTANT.Offset;
					sprintf(offs, "%d", digit);
					strcat(code, offs);
				}
				
			}
			else if (Rplace->Token == numt)
			{
				if (VALUE[t_num-1] != NULL)
				{
					sprintf(offs, "%d", VALUE[t_num - 1]);
					strcat(code, offs);
					t_num--;
				}
				// IF WE NEED TO HANDLE FLOAT, ADD THE CODE TO HERE
			}
			strcat(code, LEXEME[t_num]);
			strcpy(adop, LEXEME[t_num]);

			match(addopt);
			t_numback++;
			procedure_T(Tplace, outFile);

			if (Tplace->Token == idt)
			{
				if (Pop(Rplace->LEX)->Offset > 0)
				{
					strcat(code, "_bp+");
				}
				else if (Pop(Rplace->LEX)->Offset < 0)
				{
					strcat(code, "_bp-");
				}
				if (Tplace->TypeOfEntry == varEntry)
				{
					digit = Tplace->VAR.Offset;
					sprintf(offs, "%d", digit);
					strcat(code, offs);
				}
				else if (Tplace->TypeOfEntry == constEntry)
				{
					digit = Tplace->CONSTANT.Offset;
					sprintf(offs, "%d", digit);
					strcat(code, offs);
				}
				
			}
			else if (TOKEN[t_num-1] == numt)
			{
				if (VALUE[t_num-1] != NULL)
				{
					sprintf(offs, "%d", VALUE[t_num - 1]);
					strcat(code, offs);
					
				}
				// IF WE NEED TO HANDLE FLOAT, ADD THE CODE TO HERE
			}
			strcat(code, "		//");	//-------------------------------------
			strcat(code, tmpptr->data_value);
			strcat(code, "=");
			strcat(code, Rplace->LEX);
			strcat(code, adop);//addop
			// add in the operator
			strcat(code, Tplace->LEX);//add idt or numt after addopt
			emit(outFile, code);*/

			Rplace = Lookup(temp); //Let R inherit
			TP = tmpptr;
			procedure_R(Rplace, outFile);//when we finally return
										 //Rplace will hold final
										 //synthesized attribute
		}
	}


	else if (TOKEN[t_num] == mulopt) {

		ProcR_idx = 1;

		tmpptr = newtemp();
		temp = tmpptr->data_value;
		//assumes newtemp sets class etc.

		strcpy(code, "		mov		");
		strcat(code, "AX");
		strcat(code, ",");
		if (Rplace->Token == idt)
		{
			if (Pop(Rplace->LEX)->Offset > 0)
			{
				strcat(code, "[BP+");
			}
			else if (Pop(Rplace->LEX)->Offset < 0)
			{
				strcat(code, "[BP");
			}
			digit = Pop(Rplace->LEX)->Offset;
			sprintf(offs, "%d", digit);
			strcat(code, offs);
			strcat(code, "]");

		}
		else if (Rplace->Token == numt)
		{
			if (VALUE[t_num - 1] != NULL)
			{
				sprintf(offs, "%d", VALUE[t_num - 1]);
				strcat(code, offs);

			}
			// IF WE NEED TO HANDLE FLOAT, ADD THE CODE TO HERE
		}
		emit(outFile, code);

		strcpy(code, "		imul		");
		strcat(code, "AX");
		strcat(code, ",");

		match(mulopt);
		t_numback++;
		procedure_T(Tplace, outFile);

		if (Tplace->Token == idt)
		{
			if (Pop(Tplace->LEX)->Offset > 0)
			{
				strcat(code, "[BP+");
			}
			else if (Pop(Tplace->LEX)->Offset < 0)
			{
				strcat(code, "[BP");
			}
			digit = Pop(Tplace->LEX)->Offset;
			sprintf(offs, "%d", digit);
			strcat(code, offs);
			strcat(code, "]");
			//t_num--;
		}
		else if (Tplace->Token == numt)
		{
			if (VALUE[t_num - 1] != NULL)
			{
				sprintf(offs, "%d", VALUE[t_num - 1]);
				strcat(code, offs);
				//t_num--;
			}
			// IF WE NEED TO HANDLE FLOAT, ADD THE CODE TO HERE
		}

		emit(outFile, code);

		//t_num--;
		//t_numback--;

		strcpy(code, "		mov		");
		strcat(code, "[BP");
		sprintf(offs, "%d", tmpptr->Offset);
		strcat(code, offs);
		strcat(code, "],");
		strcat(code, "AX");
		emit(outFile, code);

		/*strcpy(code, "		mov		");
		strcat(code, "AX,");
		sprintf(offs, "%d", tmpptr->Offset);
		strcat(code, "[BP");
		strcat(code, offs);
		strcat(code, "]");
		emit(outFile, code);

		strcpy(code, "_bp");
		sprintf(offs, "%d", tmpptr->Offset);
		strcat(code, offs);
		strcat(code, " = ");

		if (Rplace->Token == idt)
		{
			if (Pop(Rplace->LEX)->Offset > 0)
			{
				strcat(code, "_bp+");
			}
			else if (Pop(Rplace->LEX)->Offset < 0)
			{

				strcat(code, "_bp-");
			}
			if (Rplace->TypeOfEntry == varEntry)
			{
				digit = Rplace->VAR.Offset;
				sprintf(offs, "%d", digit);
				strcat(code, offs);
			}
			else if (Rplace->TypeOfEntry == constEntry)
			{
				digit = Rplace->CONSTANT.Offset;
				sprintf(offs, "%d", digit);
				strcat(code, offs);
			}

		}
		else if (Rplace->Token == numt)
		{
			strcat(code, Rplace->LEX);
			// IF WE NEED TO HANDLE FLOAT, ADD THE CODE TO HERE
		}
		strcat(code, LEXEME[t_num]);	//mulop
		strcpy(adop, LEXEME[t_num]);

		match(mulopt);
		t_numback++;
		procedure_T(Tplace, outFile);


		if (Tplace->Token == idt)
		{
			if (Pop(Tplace->LEX)->Offset > 0)
			{
				strcat(code, "_bp+");
			}
			else if (Pop(Tplace->LEX)->Offset < 0)
			{
				strcat(code, "_bp-");
			}

			if (Tplace->TypeOfEntry == varEntry)
			{
				digit = Tplace->VAR.Offset;
				sprintf(offs, "%d", digit);
				strcat(code, offs);
			}
			else if (Tplace->TypeOfEntry == constEntry)
			{
				digit = Tplace->CONSTANT.Offset;
				sprintf(offs, "%d", digit);
				strcat(code, offs);
			}

		}
		else if (Tplace->Token == numt)
		{
			if (VALUE[t_num] != NULL)
			{
				sprintf(offs, "%d", VALUE[t_num]);
				strcat(code, offs);
			}
			// IF WE NEED TO HANDLE FLOAT, ADD THE CODE TO HERE
		}
		strcat(code, "		//");	//-------------------------------------
		strcat(code, tmpptr->data_value);
		strcat(code, "=");
		strcat(code, Rplace->LEX);
		strcat(code, adop);//addop
						   // add in the operator
		strcat(code, Tplace->LEX);//add idt or numt after addopt
		emit(outFile, code);*/
		Rplace = Lookup(temp); //Let R inherit
		TP = tmpptr;
		procedure_R(Rplace, outFile);//when we finally return
									 //Rplace will hold final
									 //synthesized attribute

	}

	else {
		//do nothing! Rplace is now 
		//synthesized
	}
}

Push_ptr newtemp()	// Creat the _t*
{
	char temp[SIZE];
	char * value = (char *)malloc(SIZE);
	static int love = 1;
	Push_ptr temp_adrs;

	sprintf(temp, "_t%d", love++);
	strcpy(value, temp);
	temp_adrs = push_TAC(temp);
	Insert(value, idt, 3, varEntry, var_idx, -(temp_adrs->Offset), 1, mtd_idx, mtdname);

	if (love > 99)
	{
		cout << "[ERROR] temp overflow" << endl << endl;
		exit(1);
	}

	return temp_adrs;
}

// Temparary solution for multy ply first : fix it later for permanent solution

void procedure_M(Entry_ptr& Tplace, ofstream& outFile)//At the last, it has go back to Proc A to handle final assignment : ex : a=_t3+_t7
{													  //So, we out put the entire calculation like a+b*d at the last, so we have to turn the t_num back to position same as Proc T starts

	char *home;
	char code[MAXSTRING];
	Push_ptr tmpptr;
	char *temp = NULL;
	char offs[3];
	char adop[2];
	int digit, i;
	Entry_ptr num_temp;
	Entry_ptr Rplace;// Tplace : save var before addition | Rplace : save tmpptr that * executed in Proc R

	num_temp = new TableEntry;
	Rplace = Tplace;

	while (TOKEN[t_num] != assignopt)
	{
		t_num--;
	}

	t_num++;// MOVE TO FIRST VAR AFTER =

	switch (TOKEN[t_num]) {								// Mimic Proc T PART

	case idt:
		Tplace = Lookup(LEXEME[t_num]);
		match(idt);

		break;

	case numt:
		if (VALUE[t_num] != NULL)
		{
			home = (char *)malloc(20);                    // =================== Assume that maximum is 20
			sprintf(home, "%d", VALUE[t_num]);
			num_temp->LEX = home;
			Tplace = num_temp;// =================== 4.22
			match(numt);

		}

		else if (VALUER[t_num] != NULL)
		{
			home = (char *)malloc(20);
			gcvt(VALUER[t_num], 5, home);
			num_temp->LEX = home;
			Tplace = num_temp;
			match(numt);

		}

		break;

	default:
		cout << "Token # " << t_num << " : " << "Line # " << LINE[t_num] << " : [" << LEXEME[t_num] << "] : " << "[Error] You should put idt or numt or ( here" << endl;			//Undeclared token detected
		break;
	}

	if (TOKEN[t_num] == addopt)							// Mimic Proc R part
	{
		ProcR_idx = 1;

		tmpptr = newtemp();
		temp = tmpptr->data_value;
		//assumes newtemp sets class etc.

		strcpy(code, "		add		");
		strcat(code, "AX");
		strcat(code, ",");

		if (Tplace->Token == idt)
		{
			if (Pop(Tplace->LEX)->Offset > 0)
			{
				strcat(code, "[BP+");
			}
			else if (Pop(Tplace->LEX)->Offset < 0)
			{
				strcat(code, "[BP");
			}
			digit = Pop(Tplace->LEX)->Offset;
			sprintf(offs, "%d", digit);
			strcat(code, offs);
		}
		else if (Tplace->Token == numt)
		{
			if (VALUE[t_num + 1] != NULL)
			{
				sprintf(offs, "%d", VALUE[t_num + 1]);
				strcat(code, offs);
			}
			// IF WE NEED TO HANDLE FLOAT, ADD THE CODE TO HERE
		}

		strcat(code, "]");
		emit(outFile, code);
		match(addopt);

		/*strcpy(code, "_bp");
		sprintf(offs, "%d", tmpptr->Offset);
		strcat(code, offs);

		strcat(code, " = ");
		if (Tplace->Token == idt)
		{
			strcat(code, "_bp-");

			if (Tplace->TypeOfEntry == varEntry)
			{
				digit = Tplace->VAR.Offset;
				sprintf(offs, "%d", digit);
				strcat(code, offs);
			}
			else if (Tplace->TypeOfEntry == constEntry)
			{
				digit = Tplace->CONSTANT.Offset;
				sprintf(offs, "%d", digit);
				strcat(code, offs);
			}

		}
		else if (Tplace->Token == numt)
		{
			if (VALUE[t_num + 1] != NULL)
			{
				sprintf(offs, "%d", VALUE[t_num + 1]);
				strcat(code, offs);

			}
			// IF WE NEED TO HANDLE FLOAT, ADD THE CODE TO HERE
		}
		strcat(code, LEXEME[t_num]);//addopt
		strcpy(adop, LEXEME[t_num]);

		match(addopt);

		strcat(code, "_bp-");

		digit = Rplace->VAR.Offset;
		sprintf(offs, "%d", digit);
		strcat(code, offs);
		strcat(code, "		");	//-------------------------------------
		strcat(code, tmpptr->data_value);
		strcat(code, "=");
		strcat(code, Tplace->LEX);//add idt or numt after addopt
		strcat(code, adop);//addop
						   // add in the operator
		strcat(code, Rplace->LEX);
		emit(outFile, code);*/

		Rplace = Lookup(temp); //Let R inherit
		TP = tmpptr;
		t_numback--;
		//t_num++;	// restore out idx as the position that all the calculation started
	}
	

}