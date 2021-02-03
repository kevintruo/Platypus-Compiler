/*******************************************************************
 * File name: scanner.c
 * Compiler: MS Visual Studio 2019
 * Author: Kevin Truong, 040937076 - Dante Beltran, 040921470
 * Course: CST 8152 - Compilers, Lab Section: 021
 * Assignment: A2
 * Date: November 15, 2020
 * Professor: Paulo Sousa / Khadiidja Taouch
 * Purpose: Functions implementing a Lexical Analyzer (Scanner)
 *    as required for CST8152, Assignment #2
 *    scanner_init() must be called before using the scanner.
 * Function list: aStateFuncAVID, aStateFuncSVID, aStateFuncIL, aStateFuncFPL, aStateFuncSL,
 *              aStateFuncErr, getNextState, initScanner, isKeyword, nextToken, processToken
 */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */
  /*#define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"

#define DEBUG  /* for conditional processing */
#undef  DEBUG

/* Global objects - variables */
/* This buffer is used as a repository for string literals.
   It is defined in platy_st.c */
extern pBuffer stringLiteralTable;		/* String literal table */
int line;								/* current line number of the source code */
extern int errorNumber;					/* defined in platy_st.c - run-time error number */

static char debugMode = 0;				/* optional for debugging */

/* Local(file) global objects - variables */
static pBuffer lexemeBuffer;			/* pointer to temporary lexeme buffer */
static pBuffer sourceBuffer;			/* pointer to input source buffer */
/* No other global variable declarations/definitiond are allowed */

/* scanner.c static(local) function  prototypes */
static int nextTokenClass(char c);		/* character class function */
static int getNextState(int, char);		/* state machine function */
static int isKeyword(char* kw_lexeme);	/* keywords lookup functuion */

/* TODO_02: Follow the standard and adjust all function headers */

/*
* Function name: bufferCreate()
* Purpose: The purpose of this function is to initialize scanner
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: bufferIsEmpty(), bufferRewind(), bufferClear()
* Parameters: pBuffer psc_buf
* Return value: 1 if fail and 0 if success
*/
int initScanner(pBuffer psc_buf) {
	if (bufferIsEmpty(psc_buf)) return EXIT_FAILURE;/*1*/
	/* in case the buffer has been read previously  */
	bufferRewind(psc_buf);
	bufferClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS;/*0*/
/*   scerrnum = 0;  *//*no need - global ANSI C */
}

/*
* Function name: processToken
* Purpose: The purpose of this function is to process token
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: bufferGetChar(), bufferRetract(), bufferGetCPosition(), bufferSetMarkPosition(), bufferReset(), bufferAdddChar(), bufferGetString(), bufferFree(), getNextState()
* Parameters: void
* Return value: processed token
*/
Token processToken(void) {
	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	unsigned char c;	/* input symbol */
	int state = 0;		/* initial state of the FSM */
	short lexStart;		/* start offset of a lexeme in the input char buffer (array) */
	short lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

	int lexLength;		/* token length */
	int i;				/* counter */
	int isError;		/* error identification */
	unsigned char newc;	/* new char */

	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = bufferGetChar(sourceBuffer);

		/* Part 1: Implementation of token driven scanner */
		/* every token is possessed by its own dedicated code */
		/* TODO_03: Token driven scanner implementation using switch */

		switch (c) {
			/* Left parenthesis */
		case '(':
			currentToken.code = LPR_T;
			return currentToken;
			/* Right parenthesis */
		case ')':
			currentToken.code = RPR_T;
			return currentToken;
			/* Left curly bracket */
		case '{':
			currentToken.code = LBR_T;
			return currentToken;
			/* Right curly bracket */
		case '}':
			currentToken.code = RBR_T;
			return currentToken;
			/* Comma */
		case ',':
			currentToken.code = COM_T;
			return currentToken;
			/* Semi-colon */
		case ';':
			currentToken.code = EOS_T;
			return currentToken;
			/* Plus */
		case '+':
			currentToken.code = ART_OP_T;
			currentToken.attribute.arr_op = ADD;
			return currentToken;
			/* Minus */
		case '-':
			currentToken.code = ART_OP_T;
			currentToken.attribute.arr_op = SUB;
			return currentToken;
			/* Multiply */
		case '*':
			currentToken.code = ART_OP_T;
			currentToken.attribute.arr_op = MUL;
			return currentToken;
			/* Divide */
		case '/':
			currentToken.code = ART_OP_T;
			currentToken.attribute.arr_op = DIV;
			return currentToken;
			/* Greater Than */
		case '>':
			currentToken.code = REL_OP_T;
			currentToken.attribute.rel_op = GT;
			return currentToken;
			/* Less Than OR Not Equal */
		case '<':
			/* If bufferGetChar returns '>', Not Equal */
			if (bufferGetChar(sourceBuffer) == '>') {
				currentToken.code = REL_OP_T;
				currentToken.attribute.rel_op = NE;
				return currentToken;
			}
			bufferRetract(sourceBuffer);
			/* If the bufferGetChar returns everything else other than '>', Less Than */
			if (bufferGetChar(sourceBuffer) != '>') {
				bufferRetract(sourceBuffer);
				currentToken.code = REL_OP_T;
				currentToken.attribute.rel_op = LT;
				return currentToken;
			}
			/* Assignment Operator OR Equal */
		case '=':
			/* If bufferGetChar returns '=', Equal */
			if (bufferGetChar(sourceBuffer) == '=') {
				currentToken.code = REL_OP_T;
				currentToken.attribute.rel_op = EQ;
				return currentToken;
			}
			bufferRetract(sourceBuffer);
			/* If the bufferGetChar returns everything else other than '=', Error Token */
			if (bufferGetChar(sourceBuffer) != '=') {
				bufferRetract(sourceBuffer);
				currentToken.code = ASS_OP_T;
				return currentToken;
			}
			/* String concatenation */
		case '$':
			/* If bufferGetChar returns '$', String concatenation */
			if (bufferGetChar(sourceBuffer) == '$') {
				currentToken.code = SCC_OP_T;
				return currentToken;
			}
			bufferRetract(sourceBuffer);
			/* If the bufferGetChar returns everything else other than '$', Error Token */
			if (bufferGetChar(sourceBuffer) != '$') {
				bufferRetract(sourceBuffer);
				currentToken.code = ERR_T;
				currentToken.attribute.err_lex[0] = c;
				currentToken.attribute.err_lex[1] = CHARSEOF0;
				return currentToken;
			}
			/* Comment */
		case '!':
			c = bufferGetChar(sourceBuffer);
			/* If bufferGetChar returns '!', Comment */
			if (c == '!') {
				/* Ignore until new line  */
				while (c != '\n') {
					c = bufferGetChar(sourceBuffer);
				}
				line++;
				continue;
			}
			/* Else, Error Token */
			else {
				currentToken.code = ERR_T;
				currentToken.attribute.err_lex[0] = '!';
				currentToken.attribute.err_lex[1] = c;
				currentToken.attribute.err_lex[2] = CHARSEOF0;
				while (c != '\n') {/* ingnore entire invalid comment till new line*/
					c = bufferGetChar(sourceBuffer);
				}
				line++;
				return currentToken;
			}
			/* End of file 0 */
		case CHARSEOF0:
			currentToken.code = SEOF_T;
			currentToken.attribute.seof = SEOF_0;
			return currentToken;
			break;
			/* End of file 255 */
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			currentToken.attribute.seof = SEOF_EOF;
			return currentToken;
			break;
			/* White space */
		case ' ': case '\t': case '\v': case '\f': case '\r':
			continue;
			/* New line */
		case '\n':
			line++;
			continue;
			/* _AND_ or _OR_ or _NOT_ */
		case '_':
			/* Set Mark position */
			newc = bufferGetChar(sourceBuffer);
			bufferSetMarkPosition(sourceBuffer, bufferGetCPosition(sourceBuffer) - 1);
			/* If _AND_ */
			if (newc == 'A' && bufferGetChar(sourceBuffer) == 'N' && bufferGetChar(sourceBuffer) == 'D' && bufferGetChar(sourceBuffer) == '_') {
				currentToken.code = LOG_OP_T;
				currentToken.attribute.log_op = AND;
				return currentToken;
			}
			/* Else if _OR_ */
			else if (newc == 'O' && bufferGetChar(sourceBuffer) == 'R' && bufferGetChar(sourceBuffer) == '_') {
				currentToken.code = LOG_OP_T;
				currentToken.attribute.log_op = OR;
				return currentToken;
			}
			/* Else if _NOT_ */
			else if (newc == 'N' && bufferGetChar(sourceBuffer) == 'O' && bufferGetChar(sourceBuffer) == 'T' && bufferGetChar(sourceBuffer) == '_') {
				currentToken.code = LOG_OP_T;
				currentToken.attribute.log_op = NOT;
				return currentToken;
			}
			/* Error token */
			bufferReset(sourceBuffer);
			currentToken.code = ERR_T;
			currentToken.attribute.err_lex[0] = c;
			currentToken.attribute.err_lex[1] = CHARSEOF0;
			return currentToken;

			/* Part 2: Implementation of Finite State Machine (DFA)
					   or Transition Table driven Scanner
					   Note: Part 2 must follow Part 1 to catch the illegal symbols
			*/
			/* TODO_04: Transition driven scanner implementation inside default */

		default: // general case
			/* Initialize state by using getNextState() */
			bufferRetract(sourceBuffer);
			//state = getNextState(state, c);

			lexStart = bufferGetCPosition(sourceBuffer);

			/* Set mark position */
			bufferSetMarkPosition(sourceBuffer, lexStart);

			/* Loop until NOAS state */
			while (stateType[state] == NOAS) {
				/* Error checking bufferGetChar here */
				c = bufferGetChar(sourceBuffer);
				state = getNextState(state, c);
			}
			/* Accepting state with retract or accepting state with no retract */
			/* If reaches a final state ASWR (with retract) */
			if (stateType[state] == ASWR)
				bufferRetract(sourceBuffer);

			/* Calculate lexLength */
			lexEnd = bufferGetCPosition(sourceBuffer);
			/* Plus one */
			lexLength = (lexEnd - lexStart) + 1;

			/* Check if the lexeme is separated from each other (check if you need to include + 1 for lexLength because of CHARSEOF0 at the end)*/
			lexemeBuffer = bufferCreate(lexLength, 0, 'f');

			/* Fail to create buffer */
			if (!lexemeBuffer) {
				strcpy(currentToken.attribute.err_lex, "run time rror ");
				currentToken.code = RTE_T;
				return currentToken;
			}

			/* Buffer reset */
			bufferReset(sourceBuffer);
			/* Copy LEXEME to lexemeBuffer */
			for (i = lexStart; i < lexEnd; i++) {
				bufferAddChar(lexemeBuffer, bufferGetChar(sourceBuffer));
			}
			/* Include CHARSEOF0 at the end of the lexemeBuffer */
			bufferAddChar(lexemeBuffer, CHARSEOF0);
			/* Please check if the function is being called, check if you need to include '*' at the beginning */
			currentToken = finalStateTable[state](bufferGetString(lexemeBuffer, 0));
			bufferFree(lexemeBuffer);
			return currentToken;
		}
	} // switch
} //while
 //processToken

/* DO NOT MODIFY THE CODE OF THIS FUNCTION
YOU CAN REMOVE THE COMMENTS ONLY */
/*
* Function name: getNextState()
* Purpose: The purpose of this function is to get the next state
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: nextTokenClass()
* Parameters: int - current state, c - char
* Return value: int the next state
*/

int getNextState(int state, char c) {
	int col;
	int next;
	if (debugMode)
		fprintf(stderr, "[03] getNextState\n");
	col = nextTokenClass(c);
	next = transitionTable[state][col];
#ifdef DEBUG
	printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
#endif
	/*
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:

	Assertion failed: test, file filename, line linenum

	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	*/
	assert(next != IS);

	/*
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUF is used - see the top of the file.
	*/
#ifdef DEBUG
	if (next == IS) {
		printf("Scanner Error: Illegal state:\n");
		printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
		exit(1);
	}
#endif
	return next;
}

/*
* Function name: nextTokenClass()
* Purpose: The purpose of this function is to find the next token class
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: isalpha(), isdigit()
* Parameters: char - c
* Return value: the next class
*/
int nextTokenClass(char c) {
	int val = -1;
	if (debugMode)
		fprintf(stderr, "[04] NextTokenClass\n");

	/*
	THIS FUNCTION RETURNS THE COLUMN NUMBER IN THE TRANSITION
	TABLE st_table FOR THE INPUT CHARACTER c.
	SOME COLUMNS MAY REPRESENT A CHARACTER CLASS .
	FOR EXAMPLE IF COLUMN 2 REPRESENTS [A-Za-z]
	THE FUNCTION RETURNS 2 EVERY TIME c IS ONE
	OF THE LETTERS A,B,...,Z,a,b...z.
	PAY ATTENTION THAT THE FIRST COLOMN IN THE TT IS 0 (has index 0)
	*/

	/* TODO_05: the logic to return the next column in TT */
	/*				[A-z](0),	0(1),	[1-9](2),	.(3),	$(4),	"(5),	SEOF(6),	other(7) */
	if (isalpha(c) != 0)
		val = 0;
	else if (c == '0')
		val = 1;
	else if (isdigit(c) > 0)
		val = 2;
	else if (c == '.')
		val = 3;
	else if (c == '$')
		val = 4;
	else if (c == '"')
		val = 5;
	else if (c == CHARSEOF0 || c == CHARSEOF255)
		val = 6;
	else
		val = 7;
	return val;
}

/*************************************************************
 * Acceptance State Function AVID
 ************************************************************/
 /* TODO_06: Implement the method to recognize AVID */
 /*
HERE YOU WRITE THE DEFINITIONS FOR YOUR ACCEPTING FUNCTIONS.
************************************************************
ACCEPTING FUNCTION FOR THE arithmentic variable identifier AND keywords (VID - AVID/KW)
REPLACE XX WITH THE CORRESPONDING ACCEPTING STATE NUMBER
*/
/*
* Function name:  aStateFuncAVID()
* Purpose: The purpose of this function is to define AVID
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: isKeyword(), strlen()
* Parameters: char - lexeme
* Return value: Token
*/
Token aStateFuncAVID(char lexeme[]) {
	Token currentToken = { 0 };
	int index = isKeyword(lexeme);
	int arrLength = strlen(lexeme);
	int i;
	/* If isKeyWord() does not return -1 -- meaning it is a keyword */
	if (index != RT_FAIL_1) {
		currentToken.attribute.kwt_idx = index;
		currentToken.code = KW_T;
		return currentToken;
	}

	/* AVID */
	currentToken.code = AVID_T;

	/* If lexeme is longer than VID_LEN */
	if (arrLength > VID_LEN)
		arrLength = VID_LEN;

	for (i = 0; i < arrLength; i++)
		currentToken.attribute.vid_lex[i] = lexeme[i];
	/* add string terminator */
	currentToken.attribute.vid_lex[i] = CHARSEOF0;

	return currentToken;
}

/*************************************************************
 * Acceptance State Function SVID
 ************************************************************/
 /* TODO_07: Implement the method to recognize SVID */
 /*
 ACCEPTING FUNCTION FOR THE string variable identifier (VID - SVID)
 REPLACE XX WITH THE CORRESPONDING ACCEPTING STATE NUMBER
 */
 /*
 * Function name:  aStateFuncSVID()
 * Purpose: The purpose of this function is to define SVID
 * Author: Kevin Truong / Dante Beltran
 * History/Versions: 1.0.0
 * Called functions: strlen()
 * Parameters: char - lexeme
 * Return value: Token
 */
Token aStateFuncSVID(char lexeme[]) {
	Token currentToken = { 0 };
	int arrLength = strlen(lexeme);
	int i;
	/* SVID */
	currentToken.code = SVID_T;

	if (arrLength > VID_LEN)
		arrLength = VID_LEN - 1;

	for (i = 0; i < arrLength; i++)
		currentToken.attribute.vid_lex[i] = lexeme[i];
	if (strlen(lexeme) > VID_LEN)
		/* add string concatenation */
		currentToken.attribute.vid_lex[i] = '$';
	/* add string terminator */
	currentToken.attribute.vid_lex[i + 1] = CHARSEOF0;

	return currentToken;
}

/*************************************************************
 * Acceptance State Function IL
 ************************************************************/
 /* TODO_08: Implement the method to recognize IL */
 /*
 ACCEPTING FUNCTION FOR THE integer literal(IL) - decimal constant (DIL)
 */
 /*
 * Function name: aStateFuncIL()
 * Purpose: The purpose of this function is to define IL
 * Author: Kevin Truong / Dante Beltran
 * History/Versions: 1.0.0
 * Called functions: strlen(), atol()
 * Parameters: char - lexeme
 * Return value: Token
 */
Token aStateFuncIL(char lexeme[]) {
	Token currentToken = { 0 };

	long numb = atol(lexeme);
	int arrLength = strlen(lexeme);
	int i = 0;

	/* Out of bound */
	if (numb < SHRT_MIN || numb > SHRT_MAX) {
		currentToken.code = ERR_T;
		if (arrLength > ERR_LEN) {
			arrLength = ERR_LEN - 3;
			for (i = 0; i < arrLength; i++)
				currentToken.attribute.err_lex[i] = lexeme[i];

			currentToken.attribute.err_lex[i] = '.';
			currentToken.attribute.err_lex[i + 1] = '.';
			currentToken.attribute.err_lex[i + 2] = '.';
		}
		else {
			for (i = 0; i < strlen(lexeme); i++)
				currentToken.attribute.err_lex[i] = lexeme[i];
		}
		return currentToken;
	}

	currentToken.code = INL_T;
	currentToken.attribute.int_value = (short)numb;
	return currentToken;
}

/*************************************************************
 * Acceptance State Function FPL
 ************************************************************/
 /* TODO_09: Implement the method to recognize FPL */
 /*
 ACCEPTING FUNCTION FOR THE floating-point literal (FPL)
 */
 /*
 * Function name: aStateFuncFPL()
 * Purpose: The purpose of this function is to define FPL
 * Author: Kevin Truong / Dante Beltran
 * History/Versions: 1.0.0
 * Called functions: strlen(), atof(), strcat(), strcpy()
 * Parameters: char - lexeme
 * Return value: Token
 */
Token aStateFuncFPL(char lexeme[]) {
	Token currentToken = { 0 };
	double numb = atof(lexeme);

	if (numb <= FLT_MAX && numb >= FLT_MIN || numb == 0.0) {
		currentToken.code = FPL_T;
		currentToken.attribute.flt_value = (float)numb;
	}
	else {
		currentToken.code = ERR_T;

		if (strlen(lexeme) > ERR_LEN) {
			for (int i = 0; i < ERR_LEN - 3; i++) {
				currentToken.attribute.err_lex[i] = lexeme[i];
			}
			strcat(currentToken.attribute.err_lex, "...");
		}
		else {
			strcpy(currentToken.attribute.err_lex, lexeme);
		}
		return currentToken;
	}
	return currentToken;
}

/*************************************************************
 * Acceptance State Function SL
 ************************************************************/
 /* TODO_10: Implement the method to recognize SL */
 /*
 ACCEPTING FUNCTION FOR THE string literal (SL)
 */
 /*
 * Function name: aStateFuncSL()
 * Purpose: The purpose of this function is to define SL
 * Author: Kevin Truong / Dante Beltran
 * History/Versions: 1.0.0
 * Called functions: strlen(), bufferGetAddCPosition(), bufferAddChar()
 * Parameters: char - lexeme
 * Return value: Token
 */
Token aStateFuncSL(char lexeme[]) {
	Token currentToken = { 0 };
	int arrLength = strlen(lexeme);
	currentToken.attribute.str_offset = bufferGetAddCPosition(stringLiteralTable);

	for (int i = 1; i < arrLength - 1; i++) {
		if (lexeme[i] != '"') {
			bufferAddChar(stringLiteralTable, lexeme[i]);
		}
		if (lexeme[i] == '\n')
			line++;
	}

	bufferAddChar(stringLiteralTable, CHARSEOF0);
	currentToken.code = STR_T;
	return currentToken;
}

/*************************************************************
 * Acceptance State Function Error
 ************************************************************/
 /* TODO_11: Implement the method to deal with Error Token */
 /*
 ACCEPTING FUNCTION FOR THE ERROR TOKEN
 */
 /*
 * Function name: aStateFuncErr()
 * Purpose: The purpose of this function is to define ERR
 * Author: Kevin Truong / Dante Beltran
 * History/Versions: 1.0.0
 * Called functions: strlen(), strcat(), strcpy()
 * Parameters: char - lexeme
 * Return value: Token
 */
Token aStateFuncErr(char lexeme[]) {
	Token currentToken = { 0 };
	int arrLength = strlen(lexeme);
	currentToken.code = ERR_T;

	if (strlen(lexeme) > ERR_LEN) {
		for (int i = 0; i < ERR_LEN - 3; i++) {
			currentToken.attribute.err_lex[i] = lexeme[i];
		}
		strcat(currentToken.attribute.err_lex, "...");
	}
	else {
		strcpy(currentToken.attribute.err_lex, lexeme);
	}

	for (int i = 0; i < arrLength; i++) {
		if (lexeme[i] == '\n')
			line++;
	}

	return currentToken;
}

/*************************************************************
 * Check if it is Keyword
 ************************************************************/
 /* TODO_11: Checks if a string is a keyword, returning the position in the list */
/*
* Function name:  isKeyword()
* Purpose: The purpose of this function is to define if it is a keyword
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: strcmp()
* Parameters: char - lexeme
* Return value: -1 if fail, else true
*/
int isKeyword(char* kw_lexeme) {
	int i = 0;

	/* check keyword table */
	for (i = 0; i < KWT_SIZE; i++)
		if (strcmp(keywordTable[i], kw_lexeme) == 0)
			return i;

	/* not a keyword */
	return RT_FAIL_1;
}