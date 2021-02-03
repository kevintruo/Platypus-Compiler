/*
* File name: parser.h
* Compiler: MS Visual Studio 2019
* Author: Kevin Truong, 040937076 & Dante Beltran, 040921470
* Course: CST 8152 - Compilers, Lab Section: 021
* Assignment: A1
* Date: Dec 17th, 2020
* Professor: Paulo Sousa / Haider Miraj
* Purpose: This file is the main header for Parser (.h)
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

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "token.h"
#include "buffer.h"

/* Global vars */

// TODO_01: lookahead
static Token lookahead;

// TODO_02: syntaxErrorNumber
int syntaxErrorNumber;

//Extern
extern Buffer* stringLiteralTable;
extern int line;
extern Token processToken();
extern char* keywordTable[];

/* Constants */
#define	NO_ATTR (-1)

// TODO_03: Create ALL constants for all keywords
//Define in order of keyword table
#define PROGRAM 0
#define IF 1
#define THEN 2
#define ELSE 3
#define WHILE 4
#define DO 5
#define INPUT 6
#define OUTPUT 7
#define TRUE 8
#define FALSE 9

/* Function definitions */
void processParser(void);
void matchToken(int, int);
void syncErrorHandler(int);
void printError();

// TODO_04 All FunctionsForNonTerminals
void program(void);
void optionalStatements(void);
void statements(void);
void statements_prime(void);
void statement(void);
void assignment_statement(void);
void assignment_expression(void);
void arithmetic_expression(void);
void unary_arithmetic_expression(void);
void primary_arithmetic_expression(void);
void additive_arithmetic_expression(void);
void multiplicative_arithmetic_expression(void);
void multiplicative_arithmetic_expression_prime(void);
void additive_arithmetic_expression_prime(void);
void selection_statement(void);
void conditional_expression(void);
void logical_OR_expression(void);
void logical_AND_expression(void);
void relational_expression(void);
void primary_a_relational_expression(void);
void primary_a_relational_expression_prime(void);
void primary_s_relational_expression(void);
void primary_string_expression(void);
void primary_s_relational_expression_prime(void);
void pre_condition(void);
void iteration_statement(void);
void input_statement(void);
void variable_list(void);
void variable_identifier(void);
void variable_list_prime(void);
void output_statement(void);
void output_list(void);
void string_expression(void);
void string_expression_prime(void);
void logical_OR_expression_prime(void);
void logical_AND_expression_prime(void);