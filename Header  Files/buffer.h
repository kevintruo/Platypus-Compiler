#pragma once
/*
* File name: buffer.h
* Compiler: MS Visual Studio 2019
* Author: Kevin Truong, 040937076 & Dante Beltran, 040921470
* Course: CST 8152 - Compilers, Lab Section: 021
* Assignment: A1
* Date: Oct 11th, 2020
* Professor: Paulo Sousa / Haider Miraj
* Purpose: The purpose of this file is to create a header file to declare defined value and functions as well as the buffer structure
* Function list:
* - pBuffer bufferCreate()
* - pBuffer bufferAddChar()
* - int bufferIsFull()
* - int bufferIsEmpty()
* - int bufferLoad()
* - short bufferGetAddCPosition()
* - short bufferGetCapacity()
* - int bufferGetOpMode()
* - size_t bufferGetIncrement()
* - int bufferRewind()
* - char* bufferGetString()
* - unsigned short bufferGetFlags()
* - int bufferPrint()
* - Buffer* bufferSetEnd()
* - char bufferGetChar()
* - void bufferFree()
* - int bufferClear()
* - short bufferRetract()
* - short bufferReset()
* - short bufferGetCPosition()
* - pBuffer bufferSetMarkPosition()
*/

#ifndef BUFFER_H_
#define BUFFER_H_
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make // comments an warning */

/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */
#include <stdlib.h> /* general purpose standard library in C*/
#include <math.h>   /* for mathematics purposes */
/* constant definitions */
#define RT_FAIL_1 (-1)                /* operation failure return value 1 */
#define RT_FAIL_2 (-2)                /* operation failure return value 2 */
#define LOAD_FAIL (-2)                /* load fail return value */

#define DEFAULT_INIT_CAPACITY 200     /* default initial buffer capacity */
#define DEFAULT_INC_FACTOR 15         /* default increment factor */

/* You should add your own constant definitions here */
#define MAX_ALLOWED SHRT_MAX - 1      /* MAXIMUM ALLOWED POSTIVE VALUE - 1 */
#define RT_FAIL_3 (0x100)	          /* operation failure return value 0x100 */

/* Add your bit-masks constant definitions here */
#define DEFAULT_FLAGS 0xFFFC     // 1111.1111 1111.1100
#define SET_EOB 0x0001 // 0000.0000.0000.0001  OR operation
#define RESET_EOB 0xFFFE // 1111.1111 1111.1110 AND operation
#define CHECK_EOB 0x0001 // 0000.0000.0000.0001 AND operation
#define SET_R_FLAG  0x0002 // 0000.0000.0000.0010 OR operation
#define RESET_R_FLAG  0xFFFD // 1111.1111 1111.1101 AND operation
#define CHECK_R_FLAG  0x0002 // 0000.0000.0000.0010 AND operation

/* user data type declarations */
/* user data type declarations */
typedef struct BufferEntity {
	char* string;   /* pointer to the beginning of character array (character buffer) */
	short capacity;    /* current dynamic memory size (in bytes) allocated to character buffer */
	short addCPosition;  /* the offset (in chars) to the add-character location */
	short getCPosition;  /* the offset (in chars) to the get-character location */
	short markCPosition; /* the offset (in chars) to the mark location */
	char  increment; /* character array increment factor */
	char  opMode;       /* operational mode indicator*/ //f - fixed mode a- additive m- multiplication
	unsigned short flags;     /* contains character array reallocation flag and end-of-buffer flag */
} Buffer, * pBuffer;

/* Declare functions */
pBuffer bufferCreate(short initCapacity, char incFactor, char opMode);
pBuffer bufferAddChar(pBuffer const pBE, char symbol);
int bufferIsFull(Buffer* const pBE);
int bufferIsEmpty(Buffer* const pBE);
int bufferLoad(FILE* const fi, Buffer* const pBE);
short bufferGetAddCPosition(Buffer* const pBE);
short bufferGetCapacity(Buffer* const pBE);
int bufferGetOpMode(Buffer* const pBE);
size_t bufferGetIncrement(Buffer* const pBE);
int bufferRewind(Buffer* const pBE);
char* bufferGetString(Buffer* const pBE, short charPosition);
unsigned short bufferGetFlags(pBuffer const pBE);
int bufferPrint(Buffer* const pBE, char newLine);
Buffer* bufferSetEnd(Buffer* const pBE, char symbol);
char bufferGetChar(Buffer* const pBE);
void bufferFree(Buffer* const pBE);
int bufferClear(Buffer* const pBE);
short bufferRetract(Buffer* const pBE);
short bufferReset(Buffer* const pBE);
short bufferGetCPosition(Buffer* const pBE);
pBuffer bufferSetMarkPosition(pBuffer const pBE, short mark);

#endif
