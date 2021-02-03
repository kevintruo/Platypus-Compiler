/*
* File name: parser.c
* Compiler: MS Visual Studio 2019
* Author: Kevin Truong, 040937076 & Dante Beltran, 040921470
* Course: CST 8152 - Compilers, Lab Section: 021
* Assignment: A1
* Date: Dec 17th, 2020
* Professor: Paulo Sousa / Haider Miraj
* Purpose: This file is the main source file for Parser (.c)
* Function list:
- program
- optionalStatements
- statements
- statements_prime
- statement
- assignment_statement
- assignment_expression
- arithmetic_expression
- unary_arithmetic_expression
- primary_arithmetic_expression
- additive_arithmetic_expression
- multiplicative_arithmetic_expression
- multiplicative_arithmetic_expression_prime
- additive_arithmetic_expression_prime
- selection_statement
- conditional_expression
- logical_OR_expression
- logical_AND_expression
- relational_expression
- primary_a_relational_expression
- primary_a_relational_expression_prime
- primary_s_relational_expression
- primary_string_expression
- primary_s_relational_expression_prime
- pre_condition
- iteration_statement
- input_statement
- variable_list
- variable_identifier
- variable_list_prime
- output_statement
- output_list
- string_expression
- string_expression_prime
- logical_OR_expression_prime
- logical_AND_expression_prime
*/


#include "parser.h"

/*************************************************************
 * Process Parser
 ************************************************************/
void processParser(void) {
	/*if (debugMode)
		fprintf(stderr, "[00] Processing parser...\n");*/
	lookahead = processToken();
	program();
	matchToken(SEOF_T, NO_ATTR);
	printf("%s\n", "PLATY: Source file parsed");
}

/*************************************************************
 * Match Token
 ************************************************************/
void matchToken(int tokenCode, int tokenAttribute) {
	int matchFlag = 1;
	switch (lookahead.code) {
	//TODO 5
	case KW_T:
		if (tokenAttribute != lookahead.attribute.kwt_idx)
			matchFlag = 0;
		break;
	case REL_OP_T:
		if (tokenAttribute != lookahead.attribute.rel_op)
			matchFlag = 0;
		break;
	case ART_OP_T:
		if (tokenAttribute != lookahead.attribute.arr_op)
			matchFlag = 0;
		break;
	case LOG_OP_T:
		if (tokenAttribute != lookahead.attribute.log_op)
			matchFlag = 0;
		break;
	default:
		//TODO_06
		if (tokenCode != lookahead.code)
			matchFlag = 0;
		break;
	}

	if (matchFlag && lookahead.code == SEOF_T)
		//TODO_07
		return;

	if (matchFlag) {
		//TODO_08
		lookahead = processToken();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = processToken();
			syntaxErrorNumber++;
			return;
		}
	}
	else
		// TODO_09
		syncErrorHandler(tokenCode);
}

/*************************************************************
 * Syncronize Error Handler
 ************************************************************/
void syncErrorHandler(int syncTokenCode) {
	// TODO_10
	printError();
	syntaxErrorNumber++;

	while (lookahead.code != syncTokenCode) {
		// TODO_11
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);

		lookahead = processToken();
	}
	if (lookahead.code != SEOF_T)
		lookahead = processToken();
	else
		return;
}


/*************************************************************
 * Print Error
 ************************************************************/
void printError() {
	Token t = lookahead;
	/*if (debugMode)
		fprintf(stderr, "[39] PrintError\n");*/
	printf("PLATY: Syntax error:  Line:%3d\n", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	// TODO_13
	/* Keyword token */
	case KW_T:
		printf("%s\n", keywordTable[t.attribute.get_int]);
		break;
	/* All symbols token */
	case LPR_T: case RPR_T: case LBR_T: case RBR_T:case COM_T: case EOS_T: case SCC_OP_T: case ASS_OP_T:
		printf("NA\n");
		break;
	case RTE_T:
		printf("\n");
		break;
	/* Arithmetic operator token */
	case ART_OP_T:
		printf("%d\n", t.attribute.get_int);
		break;
	/* Relational operator token */
	case REL_OP_T:
		printf("%d\n", t.attribute.get_int);
		break;
	/* Logical operator token */
	case LOG_OP_T:
		printf("%d\n", t.attribute.get_int);
		break;
	/* Arithmetic/String Variable Identifier token */
	case AVID_T: case SVID_T:
		printf("%s\n", t.attribute.vid_lex);
		break;
	/* Integer literal token */
	case INL_T:
		printf("%d\n", t.attribute.get_int);
		break;
	/* Floating point literal token */
	case FPL_T:
		printf("%5.1f\n", t.attribute.flt_value);
		break;
	/* String literal token */
	case STR_T:
		bufferSetMarkPosition(stringLiteralTable, t.attribute.str_offset);
		printf("%s\n", bufferGetString(stringLiteralTable, (short)t.attribute.get_int));
		break;
	/* End of file token */
	case SEOF_T:
		printf("SEOF_T		%d\n", t.attribute.seof);
		break;
	/* Error token */
	case ERR_T:
		printf("%s\n", t.attribute.err_lex);
		break;
	/* Default cases */
	default:
		printf("PLATY: Scanner error: invalid token code: %d\n", t.code);
	}
}


/*************************************************************
 * Program statement
 * BNF: <program> -> PLATYPUS { <opt_statements> }
 * FIRST(<program>)= {KW_T (PLATYPUS)}.
 ************************************************************/
void program(void) {
	//if (debugMode)
	//	fprintf(stderr, "[01] Program\n");
	matchToken(KW_T, PROGRAM);
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);
	printf("%s\n", "PLATY: Program parsed");
}

// TODO_14

/*************************************************************
 * optionalStatements statement
 * BNF: <opt_statements> ->  <statements> | e
 * FIRST(<opt_statements>) = { SVID_T, AVID_T, KW_T(IF),  KW_T(WHILE) , KW_T(INPUT), KW_T(OUTPUT),  e }
 ************************************************************/
void optionalStatements(void)
{
	switch (lookahead.code){
	case AVID_T: case SVID_T:
		statements();
		break;
	case KW_T:
		if (lookahead.attribute.get_int == IF ||
			lookahead.attribute.get_int == WHILE ||
			lookahead.attribute.get_int == INPUT ||
			lookahead.attribute.get_int == OUTPUT)
		{
			statements();
			break;
		}
	default:
		printf("PLATY: Opt_statements parsed\n");
	}
}

/*************************************************************
 * statements statement
 * BNF: <statements> -> <statement><statements_prime>
 * FIRST(<statements>) = { SVID_T, AVID_T, KW_T(IF),  KW_T(WHILE) , KW_T(INPUT), KW_T(OUTPUT) }
 ************************************************************/
void statements(void) {
	statement();
	statements_prime();
}

/*************************************************************
 *  statements_prime statement
 * BNF: <statements_prime> -> <statement><statements_prime> | e
 * FIRST(<statements_prime>) = { SVID_T, AVID_T, KW_T(IF),  KW_T(WHILE) , KW_T(INPUT), KW_T(OUTPUT),  e }
 ************************************************************/
void statements_prime(void) {

	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.kwt_idx) {
		case IF: case WHILE: case INPUT: case OUTPUT:
			statement();
			statements_prime();
			return;
			break;
		default: break;
		}
		break;
	case AVID_T: case SVID_T:
		statement();
		statements_prime();
		break;
	}
}

/*************************************************************
 * statement statement
 * BNF:<statement> ->  <assignment statement> | <selection statement> | <iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { SVID_T, AVID_T, KW_T(IF),  KW_T(WHILE) , KW_T(INPUT), KW_T(OUTPUT) }
 ************************************************************/
void statement(void) {
	switch (lookahead.code) {
	case AVID_T : case SVID_T:
		assignment_statement();
		break;
	case KW_T:	
		switch (lookahead.attribute.kwt_idx) {
		case IF:
			selection_statement();
			break;
		case WHILE:
			iteration_statement();
			break;
		case INPUT:
			input_statement();
			break;
		case OUTPUT:
			output_statement();
			break;
		default:
			printError();
			break;
		}
		break;
	default:
		printError();
		break;
	}
}

/*************************************************************
 * assignment_statement statement
 * BNF: <assignment statement>  ->   <assignment expression>
 * FIRST(<assignment statement>) = { AVID_T, SVID_T }
 ************************************************************/
void assignment_statement(void) {
	assignment_expression();
	matchToken(EOS_T, NO_ATTR);
	printf("PLATY: Assignment statement parsed\n");
}

/*************************************************************
 * assignment_expression statement
 * BNF: <assignment expression> ->  AVID = <arithmetic expression> | SVID = <string expression>
 * FIRST(<assignment expression>) = { AVID_T, SVID_T }
 ************************************************************/
void assignment_expression(void) {
	switch (lookahead.code) {
	case AVID_T:
		matchToken(AVID_T, NO_ATTR);
		matchToken(ASS_OP_T, NO_ATTR);
		arithmetic_expression();
		printf("PLATY: Assignment expression (arithmetic) parsed\n");
		break;
	case SVID_T:
		matchToken(SVID_T, NO_ATTR);
		matchToken(ASS_OP_T, NO_ATTR);
		string_expression();
		printf("PLATY: Assignment expression (string) parsed\n");
		break;
	default:
		printError();
		return;
	}
}

/*************************************************************
 * arithmatic_expression statement
 * BNF: <arithmetic expression> ->  <unary arithmetic expression> | <additive arithmetic expression>
 * FIRST(<arithmetic expression>) = { ART_OP_T(ADD), ART_OP_T(SUB), AVID_T, FPL_T, INL_T, LPR_T  }
 ************************************************************/
void arithmetic_expression(void) {
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.arr_op) {
		case MUL : case DIV:
			printError();
			return;
		default: break;
		}
		unary_arithmetic_expression();
		break;
	case AVID_T: case FPL_T : case INL_T : case LPR_T:
		additive_arithmetic_expression();
		break;
	default:
		printError();
		return;
	}
	printf("PLATY: Arithmetic expression parsed\n");
}

/*************************************************************
 * unary_arithmatic_expression statement
 * BNF: <unary arithmetic expression> -> - <primary arithmetic expression>
				| + <primary arithmetic expression>

 * FIRST(<unary arithmetic expression>) = { ART_OP_T(ADD), ART_OP_T(SUB) }
 ************************************************************/
void unary_arithmetic_expression(void) {
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.arr_op) {
		case MUL: case DIV:
			printError();
			return;
		default: break;
		}
		matchToken(lookahead.code, lookahead.attribute.arr_op);
		primary_arithmetic_expression();
		printf("PLATY: Unary arithmetic expression parsed\n");
		break;
	default:
		printError();
		return;
	}
}

/*************************************************************
 * primary_arithmetic_expression statement
 * BNF: <primary arithmetic expression> -> AVID_T | FPL_T | INL_T
					| (<arithmetic expression>)

 * FIRST(<primary arithmetic expression>) = { AVID_T, FPL_T, INL_T , e }
 ************************************************************/
void primary_arithmetic_expression(void) {
	switch (lookahead.code) {
	case AVID_T:
		matchToken(AVID_T, NO_ATTR);
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	case FPL_T:
		matchToken(FPL_T, NO_ATTR);
		break;
	case LPR_T:
		matchToken(LPR_T, NO_ATTR);
		arithmetic_expression();
		matchToken(RPR_T, NO_ATTR);
		break;
	default:
		printError();
		return;
	}
	printf("PLATY: Primary arithmetic expression parsed\n");
}

/*************************************************************
 * additive_arithmetic_expression statement
 * BNF: <additive arithmetic expression> ->  <multiplicative arithmetic expression><additive arithmetic expression prime>
 * FIRST(<additive arithmetic expression>) = { AVID_T, FPL_T, INL_T, LPR_T }
 ************************************************************/
void additive_arithmetic_expression(void) {
	multiplicative_arithmetic_expression();
	additive_arithmetic_expression_prime();
}

/*************************************************************
 * multiplicative_arithmetic_expression statement
 * BNF: <multiplicative arithmetic expression> -> <primary arithmetic expression><multiplicative arithmetic expression prime>
 * FIRST(<multiplicative arithmetic expression>) = {  AVID_T, FPL_T, INL_T }
 ************************************************************/
void multiplicative_arithmetic_expression(void) {
	primary_arithmetic_expression();
	multiplicative_arithmetic_expression_prime();
}

/*************************************************************
 * multiplicative_arithmetic_expression_prime statement
 * BNF: <multiplicative arithmetic expression prime> -> *<primary arithmetic expression><multiplicative arithmetic expression prime> | / <primary arithmetic expression><multiplicative arithmetic expression prime> | e
 * FIRST(<multiplicative arithmetic expression prime>) = { ART_OP_T(MULTI), ART_OP_T(DIV) , e }
 ************************************************************/
void multiplicative_arithmetic_expression_prime(void) {
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.arr_op) {
		case MUL: case DIV:
			matchToken(ART_OP_T, lookahead.attribute.arr_op);
			primary_arithmetic_expression();
			multiplicative_arithmetic_expression_prime();
			break;
		default: return;
		}
		break;
	default: return;
	}
	printf("PLATY: Multiplicative arithmetic expression parsed\n");
}

/*************************************************************
 * additive_arithmetic_expression_prime statement
 * BNF: <additive arithmetic expression prime> -> + <multiplicative arithmetic expression><additive arithmetic expression prime> | -  <multiplicative arithmetic expression><additive arithmetic expression prime> | e
 * FIRST(<additive arithmetic expression prime>) = { ART_OP_T(PLUS), ART_OP_T(MINUS), e }
 ************************************************************/
void additive_arithmetic_expression_prime(void){
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.arr_op) {
		case ADD: case SUB:
			matchToken(ART_OP_T, lookahead.attribute.arr_op);
			multiplicative_arithmetic_expression();
			additive_arithmetic_expression_prime();
			break;
		default: return;
		}
		break;
	default: return;
	}
	printf("PLATY: Additive arithmetic expression parsed\n");
}

/*************************************************************
 * selection_statement statement
 * BNF: <selection statement> ->  IF <pre-condition>  (<conditional expression>)
		THEN { <opt_statements> }
		ELSE { <opt_statements> } ;

 * FIRST(<selection statement>) = { KW_T(IF) }
 ************************************************************/
void selection_statement(void) {
	matchToken(KW_T, IF); 
	pre_condition();
	matchToken(LPR_T, NO_ATTR);
	conditional_expression();
	matchToken(RPR_T, NO_ATTR);

	matchToken(KW_T, THEN);
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);

	matchToken(KW_T, ELSE);
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);

	matchToken(EOS_T, NO_ATTR);
	printf("PLATY: Selection statement parsed\n");
}

/*************************************************************
 * conditional_expression statement
 * BNF: <conditional expression> ->  <logical OR  expression>
 * FIRST(<conditional expression>) = { AVID_T, FPL_T, INL_T, SVID_T, STR_T }
 ************************************************************/
void conditional_expression(void) {
	logical_OR_expression();
 	printf("PLATY: Conditional expression parsed\n");
}

/*************************************************************
 * logical_OR_expression statement
 * BNF: <logical  OR expression> -> <logical  AND expression><logical  OR expression prime>
 * FIRST(<logical  OR expression>) = { AVID_T, FPL_T, INL_T, SVID_T, STR_T }
 ************************************************************/
void logical_OR_expression(void) {
	logical_AND_expression();
	logical_OR_expression_prime();
}

/*************************************************************
 * logical_AND_expression statement
 * BNF: <logical AND expression>-> <logical NOT expression> <logical AND expression prime>
 * FIRST(<logical AND expression>) = { AVID_T, FPL_T, INL_T, SVID_T, STR_T }
 ************************************************************/
void logical_AND_expression(void) {
	relational_expression();
	logical_AND_expression_prime();
}

/*************************************************************
 * relational_expression statement
 * BNF: <relational expression> -> <primary a_relational expression>  <operators>  <primary a_relational expression> | <primary s_relational expression>  <operators>   <primary s_relational expression>
 * FIRST(<relational expression>) = { AVID_T, FPL_T, INL_T, SVID_T, STR_T }
 ************************************************************/
void relational_expression(void) {
	switch (lookahead.code) {
	case AVID_T: case FPL_T: case INL_T:
		primary_a_relational_expression();
		primary_a_relational_expression_prime();
		break;
	case SVID_T: case STR_T:
		primary_s_relational_expression();
		primary_s_relational_expression_prime();
		break;
	default:
		printError();
	}
	printf("PLATY: Relational expression parsed\n");
}

/*************************************************************
 * primary_a_relational_expression statement
 * BNF: <primary a_relational expression> ->  AVID_T  | FPL_T  | INL_T
 * FIRST(<primary a_relational expression>) = { AVID_T, FPL_T, INL_T }
 ************************************************************/
void primary_a_relational_expression(void) {
	switch (lookahead.code) {
	case AVID_T: case FPL_T: case INL_T:
		matchToken(lookahead.code, NO_ATTR);
		break;
	default:
		printError();
	}
	printf("PLATY: Primary a_relational expression parsed\n");
}

/*************************************************************
 * primary_a_relational_expression_prime statement
 * BNF: <primary a_relational expression prime > ->  EQ | NE | GT | LT
 * FIRST(<primary a_relational expression prime >) = { EQ, NE, GT, LT }
 ************************************************************/
void primary_a_relational_expression_prime(void) {
	switch (lookahead.code) {
	case REL_OP_T:
		switch (lookahead.attribute.rel_op) {
		case EQ: case NE: case GT: case LT:
			matchToken(REL_OP_T, lookahead.attribute.rel_op);
			primary_a_relational_expression();
			break;
		default:
			printError();
		}
		break;
	default:
		printError();
	}
}

/*************************************************************
 * primary_s_relational_expression statement
 * BNF: <primary s_relational expression> -> <primary string expression>
 * FIRST(<primary s_relational expression>) = {  SVID_T, STR_T }
 ************************************************************/
void primary_s_relational_expression(void) {
	switch (lookahead.code) {
	case SVID_T: case STR_T:
		matchToken(lookahead.code, NO_ATTR);
		break;
	}
	primary_string_expression();
	printf("PLATY: Primary s_relational expression parsed\n");
}

/*************************************************************
 * primary_string_expression statement
 * BNF: <primary string expression>  ->  SVID_T  |  STR_T
 * FIRST(<primary string expression>) = { SVID_T, STR_T }
 ************************************************************/
void primary_string_expression(void) {
	switch (lookahead.code) {
	case SVID_T: case STR_T:
		matchToken(lookahead.code, NO_ATTR);
		break;
	}
	printf("PLATY: Primary string expression parsed\n");
}

/*************************************************************
 * primary_string_expression_prime statement
 * BNF: <primary string expression prime>  ->  EQ | NE | GT | LT
 * FIRST(<primary string expression prime>) = { EQ, NE, GT, LT }
 ************************************************************/
void primary_s_relational_expression_prime(void) {
	switch (lookahead.code) {
	case REL_OP_T:
		switch (lookahead.attribute.rel_op) {
		case EQ: case NE: case GT: case LT:
			matchToken(REL_OP_T, lookahead.attribute.rel_op);
			primary_s_relational_expression();
			break;
		default:
			printError();
		}
		break;
	default:
		printError();
	}
}

/*************************************************************
 * pre_condition statement
 * BNF: <pre-condition> -> TRUE | FALSE
 * FIRST(<pre-condition>) = { KW_T(TRUE), KW_T(FALSE) }
 ************************************************************/
void pre_condition(void) {
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.get_int) {
		case TRUE: case FALSE:
			matchToken(KW_T, lookahead.attribute.get_int);
			break;
		default: printError();
		}
		break;
	default: printError();
	}
}

/*************************************************************
 * iteration_statement statement
 * BNF: <iteration statement> ->   WHILE <pre-condition> (<conditional expression>)
	  DO { <statements>};R

 * FIRST(<iteration statement>) = { KW_T(WHILE) }
 ************************************************************/
void iteration_statement(void) {
	matchToken(KW_T, WHILE); 
	pre_condition();
	matchToken(LPR_T, NO_ATTR);
	conditional_expression();
	matchToken(RPR_T, NO_ATTR);

	matchToken(KW_T, DO);
	matchToken(LBR_T, NO_ATTR);
	statements();
	matchToken(RBR_T, NO_ATTR);

	matchToken(EOS_T, NO_ATTR);

	printf("PLATY: Iteration statement parsed\n");
}

/*************************************************************
 * input_statement statement
 * BNF: <input statement> -> INPUT (<variable list>);
 * FIRST(<input statement>) = { KW_T(INPUT) }
 ************************************************************/
void input_statement(void) {
	matchToken(KW_T, INPUT);
	matchToken(LPR_T, NO_ATTR);
	variable_list();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("PLATY: Input statement parsed\n");
}

/*************************************************************
 * variable_list statement
 * BNF: <variable list> -><variable identifier><variable list prime>
 * FIRST(<variable list>) = { AVID_T , SVID_T }
 ************************************************************/
void variable_list(void) {
	variable_identifier();
	variable_list_prime();
	printf("PLATY: Variable list parsed\n");
}

/*************************************************************
 * variable_identifier statement
 * BNF: <variable identifier> -> AVID_T | SVID_T
 ************************************************************/
void variable_identifier(void) {
	switch (lookahead.code) {
	case AVID_T : case SVID_T:
		matchToken(lookahead.code, NO_ATTR);
		break;
	default: 
		printError();
		return;
	}
}

/*************************************************************
 * variable_list_prime statement
 * BNF: <variable list prime> -> ,<variable identifier><variable list prime> | e
 * FIRST(<variable list prime>) = { COM_T, e }
 ************************************************************/
void variable_list_prime(void) {
	switch (lookahead.code) {
	case COM_T:
		matchToken(COM_T, NO_ATTR);
		variable_identifier();
		variable_list_prime();
		break;
	default: break;
	}
}

/*************************************************************
 * output_statement statement
 * BNF: <output statement> -> OUTPUT (<output_list>);
 * FIRST(<output statement>) = { KW_T(OUTPUT) }
 ************************************************************/
void output_statement(void) {
	matchToken(KW_T, OUTPUT);
	matchToken(LPR_T, NO_ATTR);
	output_list();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("PLATY: Output statement parsed\n");
}

/*************************************************************
 * output_list statement
 * BNF: <output_list> -> <opt_variable list> | STR_T
 * FIRST(<output_list>) = { AVID_T, SVID_T, STR_T, e }
 ************************************************************/
void output_list(void) {
	switch (lookahead.code) {
	case AVID_T: case SVID_T:
		variable_list();
		break;
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		printf("PLATY: Output list (string literal) parsed\n");
		break;
	default:
		printf("PLATY: Output list (empty) parsed\n");
		break;
	}
}

/*************************************************************
 * string_expression statement
 * BNF: <string expression> -> <primary string expression><string expression prime>
 * FIRST(<string expression>) = { SVID_T, STR_T }
 ************************************************************/
void string_expression(void) {
	primary_string_expression();
	string_expression_prime();
}

/*************************************************************
 * string_expression_prime statement
 * BNF: <string expression prime> -> $$ <primary string expression><string expression prime> | e
 * FIRST(<string expression prime>) = { SCC_OP_T, e }
 ************************************************************/
void string_expression_prime(void) {
	switch (lookahead.code) {
	case SCC_OP_T:
		matchToken(SCC_OP_T, NO_ATTR);
		primary_string_expression();
		string_expression_prime();
		break;
	default:
		printf("PLATY: String expression parsed\n");
	}
}

/*************************************************************
 * logical_OR_expression_prime statement
 * BNF: <logical  OR expression prime> -> _OR_ <logical  AND expression><logical  OR expression prime> | e
 * FIRST(<logical  OR expression prime>) = { LOG_OP_T(OR), e }
 ************************************************************/
void logical_OR_expression_prime(void) {
	switch (lookahead.code) {
	case LOG_OP_T:
		switch (lookahead.attribute.log_op) {
		case AND:
			return;
		default: break;
		}
		matchToken(lookahead.code, lookahead.attribute.log_op);
		logical_AND_expression();
		logical_OR_expression_prime();
		printf("PLATY: Logical OR expression parsed\n");
	}
}

/*************************************************************
 * logical_AND_expression_prime statement
 * BNF: <logical AND expression prime> -> _AND_ <logical NOT expression> <logical AND expression prime> | e
 * FIRST(<logical AND expression prime>) = { LOG_OP_T(AND), e }
 ************************************************************/
void logical_AND_expression_prime(void) {
	switch (lookahead.code) {
	case LOG_OP_T:
		switch (lookahead.attribute.log_op) {
		case AND:
			matchToken(LOG_OP_T, AND);
			relational_expression();
			logical_AND_expression_prime();
			printf("PLATY: Logical AND expression parsed\n");
			break;
		default: break;
		}
		break;
	default: break;
	}
}