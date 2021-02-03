/*
* File name: buffer.c
* Compiler: MS Visual Studio 2019
* Author: Kevin Truong, 040937076 & Dante Beltran, 040921470
* Course: CST 8152 - Compilers, Lab Section: 021
* Assignment: A1
* Date: Oct 11th, 2020
* Professor: Paulo Sousa / Haider Miraj
* Purpose: The purpose of this file is to create a file that contains functions to create and load a buffer with passed in parameters
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
* - unsigned short bufferGetFlags()nt
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

#include "buffer.h"

/*
* Function name: bufferCreate()
* Purpose: The purpose of this function is to create an empty buffer with passed in parameters
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: malloc(), calloc() and abs()
* Parameters: short - initCapacity <Capacity for a new empty buffer>, char - incFactor <Increment factor for a new empty buffer>, char opMode <Mode (f for fixed, a for additive, m for multiplicative)>
* Return value: pBuffer - a new empty buffer with passed in parameters
* Algorithm:
* - Allocate memory for one Buffer structure using calloc() and validate if calloc() works
* - Validate parameter initCapacity and check if it equals to 0, if not then set the buffer capacity equals to it
* - If the initCapacity value is 0, it will set the buffer capacity and increment to its default values (200 and 15 respectively). Although for f mode, it will set increment to 0
* - Validate parameter opMode and if opMode is 'f' or incFactor equals to and initCapacity does not equal to 0 then it will set the buffer opMode and increment to 0
* - If opMode is 'a' and the range of incFactor is valid then set the buffer opMode to 1 and increment to incFactor
* - Notes: To validate incFactor, it uses function abs() to get a positive integer. I.e: if incFactor is 128, its actual value will become -128 because of its datatype
* - By using function asb(), it will ensure the number does not get out of ranged
* - If opMode is 'm' and the range of incFactor is valid then set the buffer opMode to -1 and increment to incFactor
* - To allocate the character array with the passed in capacity, it will create a temporary character array and use malloc() to allocate memory.
* - If successfuly, it will set buffer string array equals to the temporary one
* - Set other values of the buffer to their default value and return the buffer
*/
pBuffer bufferCreate(short initCapacity, char incFactor, char opMode) {
	/* Allocating using calloc() for one  Buffer structure */
	pBuffer buffer = (pBuffer)calloc(1, sizeof(*buffer));

	/* Check if calloc() works, if not return NULL */
	if (!buffer)
		return NULL;

	/* Check if parameter initCapacity is valid (initCapacity must be between 0 and the maximum allowed postive value - 1) */
	if (initCapacity < 0 && initCapacity > MAX_ALLOWED)
		return NULL;

	/* If parameter initCapacity is 0 */
	if (initCapacity == 0) {
		/* Set the buffer capacity to its default size (which is 200) */
		buffer->capacity = DEFAULT_INIT_CAPACITY;
		/* If the mode is f */
		if (opMode == 'f')

			/* Set the increment to 0 */
			buffer->increment = 0;
		else {
			/* Else set the increment to its default value for 'a' and 'm' mode */
			buffer->increment = DEFAULT_INC_FACTOR;
			incFactor = DEFAULT_INC_FACTOR;
		}
	}
	else {
		buffer->capacity = initCapacity;
	}

	/* Check if parameter opMode is valid (opMode must be between 3 letter 'f', 'a', 'm') */
	if (opMode != 'f' && opMode != 'a' && opMode != 'm')
		return NULL;

	/* If the parameter opMode is in fixed mode */
	if (opMode == 'f') {
		/* Set the mode and the increment to 0 */
		buffer->opMode = 0;
		buffer->increment = 0;
	}

	/* If the parameter incFactor is 0 and parameter and initCapcity is not 0 */
	else if (incFactor == 0 && initCapacity != 0) {
		/* Set the mode and the increment to 0 */
		buffer->opMode = 0;
		buffer->increment = 0;
	}

	/* If the parameter opMode is in additive mode and the paramter incFactor range is between 1 and 255 inclusive */
	else if (opMode == 'a' && (abs(incFactor) >= 1 && abs(incFactor) <= 255)) {
		/* Set the mode to 1 and the increment to the valid parameter incFactor */
		buffer->opMode = 1;
		buffer->increment = incFactor;
	}

	/* If the parameter opMode is in multiplicative mode and the parameter incFactor range is between 1 and 100 inclusive */
	else if (opMode == 'm' && (incFactor >= 1 && incFactor <= 100)) {
		/* Set the mode to -1 and the increment to the valid parameter incFactor */
		buffer->opMode = -1;
		buffer->increment = incFactor;
	}

	else
		return NULL;

	/* Creating a temporary pointer character array and allocating the valid capacity that has passed in as a capacity */
	char* temp = (char*)malloc(buffer->capacity * sizeof(char));

	/* Check if malloc() works, if not return NULL */
	if (!temp)
		return NULL;

	/* If the temporary pointer character array has been allocated successfully, set buffer character array to the temp */
	buffer->string = temp;

	/* Set the buffer structure to their default value */
	buffer->addCPosition = 0;
	buffer->getCPosition = 0;
	buffer->markCPosition = 0;
	buffer->flags = DEFAULT_FLAGS;

	return buffer;
}

/*
* Function name: bufferAddChar()
* Purpose: The purpose of this function is to calculate the new capacity if the buffer is full to add a new character into the buffer
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: realloc() and bufferIsFull()
* Parameters: pBuffer const - pBE (passed in buffer), char - symbol (character is going to be added)
* Return value: pBE (with added character symbol)
* Algorithm:
* - Validate the parameter buffer'
* - Reset the r_flag bit to 0 by doing the AND operation with RESET_R_FLAG value
* - If the buffer is not full, it will add character symbol to the next available index in the character array
* - Then return the buffer with added character symbol
* - If the buffer is full and if the mode is 'f', it will return null because it is not supposed to resize in fixed mode
* - If the buffer is in 'a' or 'm' mode, it wil calculate the new capacity to reallocate memory for the character array
* - Note: In 'm' mode, when calculating new increment, the value maybe below 1 but still greater than 0 and because the datatype is short, it will become 0.
* - To fix this problem, simply use an if() statement and cast(float) to set new increment to 1 when the result is greater than 0 and less than 1
* - With the new calculated capacity, create a new temporary character array to reallocate memory with new capacity
* - Check if the memory address has been changed, if it is changed, set the r_flag bit to 1 using OR operation with the SET_R_FLAG value
* - If successful, set the buffer array equals to the temporary one and add the character symbol to the next available index and return buffer
*/
pBuffer bufferAddChar(pBuffer const pBE, char symbol) {
	/* Declare and initialize the new capacity */
	short newCapacity = 0;
	/* Validate if the parameter buffer */
	if (!pBE)
		return NULL;

	/* Reset the flags field r_flag bit to 0 by doing the AND operation with RESET_R_FLAG */
	pBE->flags = pBE->flags & RESET_R_FLAG;

	/* If the buffer is not full */
	if (bufferIsFull(pBE) == 0) {
		/* Increment addCPosition by 1 and add parameter symbol to that index of the character array */
		pBE->string[pBE->addCPosition++] = symbol;
		return pBE;
	}

	/* If the buffer is full */
	else {
		/* If the mode is in fiexed mode */
		if (pBE->opMode == 0)
			return NULL;

		/* If the mode is in additive mode */
		else if (pBE->opMode == 1) {
			/* Calculate the new capacity to reallocate */
			newCapacity = (short)(pBE->capacity + (float)abs(pBE->increment));

			/* Validate if the newCapacity */
			if (newCapacity < 0)
				return NULL;

			/* If the new capacity exceeds the maximum allowed positive value - 1 */
			if (newCapacity > MAX_ALLOWED)

				/* Set the new capacity equals to the maximum allowed positive value - 1 */
				newCapacity = MAX_ALLOWED;
		}

		/* If the mode is in multiplicative mode */
		else if (pBE->opMode == -1) {
			/* If the current capacity is its max */
			if (pBE->capacity == SHRT_MAX - 1)
				return NULL;

			/* Calculate the new capacity */
			short availableSpace = MAX_ALLOWED - pBE->capacity;
			/* Use float datatype for newInc because the value may be less than 1 but greater than  0*/
			float newInc = availableSpace * (float)pBE->increment / 100;
			if (newInc > 0 && newInc < 1)
				newInc = 1;
			/* Because newInc is float, cast short() before to adding the new increment */
			newCapacity = pBE->capacity + (short)newInc;

			/* If the new capacity exceeds the maximum allowed positive value - 1 */
			if (newCapacity > MAX_ALLOWED)

				/* Set the new capacity equals to the maximum allowed positive value - 1 */
				newCapacity = MAX_ALLOWED;
		}

		/* Should not reach here */
		else
			return NULL;

		/* After calculating and validating the new capacity, set the current capacity to the new capacity */
		pBE->capacity = newCapacity;

		/* If the mode is in additive or multiplicative mode */
		if (pBE->opMode == 1 || pBE->opMode == -1) {
			/* Create the temporary pointer character array and realloc with the new capacity */
			char* temp = (char*)realloc(pBE->string, sizeof(char) * pBE->capacity);

			/* Validate the temp*/
			if (!temp)
				return NULL;

			/* If the location is not the same */
			if (temp != pBE->string)

				/* Set the r_flag to 0 */
				pBE->flags = pBE->flags | SET_R_FLAG;

			/* Assign the character buffer with the new capacity */
			pBE->string = temp;

			/* Append new char to next available index */
			pBE->string[pBE->addCPosition++] = symbol;
			return pBE;
		}
	}
	/* Return NULL by default*/
	return NULL;
}

/*
* Function name: bufferIsFull()
* Purpose: The purpose of this function is to check if the buffer is full
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: N/A
* Parameters: Buffer* - pBe (buffer)
* Return value: 1 - true, 0 - false (default)
* Algorithm:
* - Validate the parameter buffer
* - Check if addCPosition is equal to the current capacity of the buffer, if true then return 1, return 0 by default
*/
int bufferIsFull(Buffer* const pBE) {
	/* Validate if the parameter buffer */
	if (!pBE)
		return RT_FAIL_1;

	/* Compare addCPosition with the current capacity, if they are equal then the buffer is full */
	if (pBE->addCPosition == pBE->capacity)

		/* Return 1 if it is full */
		return 1;

	/* Return 0 by default */
	return 0;
}

/*
* Function name: bufferIsEmpty()
* Purpose: The purpose of this function is check if the buffer is empty
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: N/A
* Parameters: Buffer* - pBE (buffer)
* Return value: 1 - true, 0 - false (default)
* Algorithm:
* - Validate the parameter buffer
* - Check if addCPosition is equal to 0, if true then return 1, return 0 by default
*/
int bufferIsEmpty(Buffer* const pBE) {
	/* Validate if the parameter buffer */
	if (!pBE)
		return RT_FAIL_1;

	/* Check if addCPosition is 0 (means there are no characters that has been added to the buffer array */
	if (pBE->addCPosition == 0)

		/* Return 1 if it is empty */
		return 1;

	/* Return 0 by default */
	return 0;
}

/*
* Function name: bufferLoad()
* Purpose: The purpose of this function is to load the characters into the buffer
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: bufferAddChar(), fgetc(), feof(), ungetc()
* Parameters: File* - fi (File pointer), Buffer* - buffer (buffer)
* Return value: The number of characters that has been added in the buffer successfully
* Algorithm:
* - Validate the parameter buffer and file
* - In a while loop, firstly, it will check if it reaches the end of the file, if true then return the current number of added characters
* - If false, it will get the next character in the file and add the character into the buffer using bufferAddChar()
* - If successful, increment the number of added characters by 1 and restart the loop
* - Else, it will use function ungetc() and return LOAD_FAIl
*/
int bufferLoad(FILE* const fi, Buffer* const pBE) {
	/* Declare and initialize the number of add characters and a temporary character */
	int numb = 0, temp = 0;

	/* Validate if the parameter buffer and file pointer is null or not */
	if (!pBE && !fi)
		return RT_FAIL_1;

	/* Start a while loop */
	do {
		/* Get the next character */
		temp = fgetc(fi);

		/* Check if it reaches the end of the file */
		if (feof(fi))
			break;

		/* Adding the new character to the next possible index, if the return value is NULL then use ungetc() function and return -2 */
		if (bufferAddChar(pBE, (char)temp) == NULL) {
			/* ungetc() and return - 2*/
			ungetc(temp, fi);
			return LOAD_FAIL;
		}

		/* If adding the new character successfully, increment the number of characters by 1 */
		else
			numb++;
	} while (1);

	/* Return the number of character that were added to the character buffer */
	return numb;
}

/*
* Function name: bufferGetAddCPosition()
* Purpose: The purpose of this function is to return the addCPosition value
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: N/A
* Parameters: Buffer* - pBE (buffer)
* Return value: The value of the addCPosition
* Algorithm: Validate the parameter buffer and return addCPosition
*/
short bufferGetAddCPosition(Buffer* const pBE) {
	/* Validate parameter buffer */
	if (!pBE)
		return RT_FAIL_1;

	return pBE->addCPosition;
}

/*
* Function name: bufferGetCapacity()
* Purpose: The purpose of this function is to return the capacity value
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: N/A
* Parameters: Buffer* - pBE (buffer)
* Return value: The value of the capacity
* Algorithm: Validate the parameter buffer and return capacity
*/
short bufferGetCapacity(Buffer* const pBE) {
	/* Validate parameter buffer */
	if (!pBE)
		return RT_FAIL_1;

	return pBE->capacity;
}

/*
* Function name: bufferGetOpMode()
* Purpose: The purpose of this function is to return the opMode value
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: bufferIsEmpty()
* Parameters: Buffer* - pBE (buffer)
* Return value: The value of the opMode
* Algorithm: Validate the parameter buffer and return opMode
*/
int bufferGetOpMode(Buffer* const pBE) {
	/* Validate parameter buffer */
	if (!pBE)
		return RT_FAIL_1;

	if (bufferIsEmpty(pBE) == 1) {
		pBE->opMode = 0;
	}

	return (int)pBE->opMode;
}

/*
* Function name: bufferGetIncrement()
* Purpose: The purpose of this function is to return the increment value
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: bufferIsEmpty()
* Parameters: Buffer* - pBE (buffer)
* Return value: The value of the increment
* Algorithm: Validate the parameter buffer and return increment
*/
size_t bufferGetIncrement(Buffer* const pBE) {
	/* Validate parameter buffer */
	if (!pBE)
		return RT_FAIL_3;

	/* Return data type size_t of the absolute value of increment
	 * The reason we want to return absolute value here is because size_t is unsigned and char is signed data type
	 * So when we convert, it will become negative and we want the positive value */

	if (bufferIsEmpty(pBE) == 1) {
		pBE->increment = 0;
	}
	return (size_t)abs(pBE->increment);
}

/*
* Function name: bufferRewind()
* Purpose: The purpose of this function is to reset the getCPosition and markCPosition to 0 so it can re-read the buffer again
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: N/A
* Parameters: Buffer* pBE
* Return value: return 0 by default and RT_FAIL_1 if error occurred
* Algorithm: N/A
*/
int bufferRewind(Buffer* const pBE) {
	/* Validate parameter buffer */
	if (!pBE)
		return RT_FAIL_1;

	pBE->getCPosition = 0;
	pBE->markCPosition = 0;

	return 0;
}

/*
* Function name: bufferGetString()
* Purpose: The purpose of this function is to get the character given the index of the character array
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: N/A
* Parameters: Buffer* - pBE, short - charPosition
* Return value: Return the character of the given index of the array, return null if error occurred
* Algorithm: N/A
*/
char* bufferGetString(Buffer* const pBE, short charPosition) {
	/* Validate parameter buffer */
	if (!pBE)
		return NULL;

	return &pBE->string[charPosition];
}

/*
* Function name: bufferGetFlags()
* Purpose: The purpose of this function is to get the flag of the buffer
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: N/A
* Parameters: Buffer* - pBE
* Return value: Return the value of the flag of the buffer structure
* Algorithm: N/A
*/
unsigned short bufferGetFlags(pBuffer const pBE) {
	/* Validate parameter buffer */
	if (!pBE)
		return 0; /* Can not return -1 because the data type is unsigned short */

	return pBE->flags;
}

/*
* Function name: bufferPrint()
* Purpose: The purpose of this function is to print the buffer structure into the console log and returned the number of printed characters
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: bufferGetChar()
* Parameters: Buffer* - pBE, char - newLine
* Return value: Return the number of printed characters
* Algorithm:
* - Validate the buffer
* - In while loop, it will get the character of the buffer using bufferGetChar()
* - If the return value is 0 or eob_flag is 1 then it will exit the loop return current the number of printed characters
* - Otherwise, it will print the character and increment the number of printed characters by 1
* - If the parameter newLine does not equal to 0, it will print a new line - \n
*/
int bufferPrint(Buffer* const pBE, char newLine) {
	/* Validate parameter buffer */
	if (!pBE)
		return RT_FAIL_1;

	/* Declare and initialize the temporary character from the character array and the number of printed characters */
	int  numb = 0;
	char temp = 0;

	/* Start of for loop */
	do {
		/* If the buffer reaches the end (check if bufferGetChar() returns 0 and check the flag if it is EOB (= 1)) */
		if (bufferGetCPosition(pBE) >= bufferGetAddCPosition(pBE))

			/* Exit loops */
			break;

		/* Get the last character in the buffer */
		temp = bufferGetChar(pBE);

		if ((pBE->flags & CHECK_EOB) == 1)

			break;

		/* Else printing the character */
		else {
			printf("%c", temp);

			/* Increment the number of printed characters by 1 */
			numb++;
		}
	} while (1);
	/* End of for loop */

	if (newLine != 0)
		printf("\n");

	/* Return the number of characters printed */
	return numb;
}

/*
* Function name: bufferSetEnd()
* Purpose: The purpose of this function is to calculate the new capacity to reallocate the array and add � at the end of the buffer
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: realloc()
* Parameters: Buffer* - pBE, char - symbol
* Return value: Retunr the buffer with added � at the end of the buffer
* Algorithm:
* - Validate the parameter buffer and calculate the new capcity by increment the current capacity by 1
* - Creat a temporary character array and realloc with the new capcity and check if the address changes, if true then set r_flag bit to 1
* - If realloc() successful, it will set the character array equal to the temporary one
* - Then, it will add the symbol � to the next available index (which is the last index in the array) and then return the buffer with added symbol
*/
Buffer* bufferSetEnd(Buffer* const pBE, char symbol) {
	/* Validate parameter buffer */
	if (!pBE)
		return NULL;

	/* Reset flags */
	pBE->flags = pBE->flags & RESET_R_FLAG;

	/* Calculate the new capacity by increasing the current limit plus a space */
	short newCapacity = 0;

	newCapacity = pBE->addCPosition + 1;

	/* Create a temporary character array and test realloc() before pointing pbe->string to the same location */
	char* temp = realloc(pBE->string, sizeof(char) * newCapacity);

	if (!temp)
		return NULL;

	/* Check if the location is the same or not */
	if (temp != pBE->string)

		/* Set the flags r_flag bit to 1 by using or operation with set_r_flag */
		pBE->flags = pBE->flags | SET_R_FLAG;

	/* Assign the character buffer with the new capacity and the current capacity with the new capacity */
	pBE->string = temp;

	/* Add symbol to the end of the character buffer and increment addcposition */
	pBE->string[pBE->addCPosition++] = symbol;

	pBE->capacity = newCapacity;
	return pBE;
}

/*
* Function name: bufferGetChar()
* Purpose: The purpose of this function is to return the buffer character
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: bufferIsEmpty()
* Parameters: Buffer* pBE
* Return value: Return the character of the buffer
* Algorithm:
* - Validate the buffer and check if the buffer is empty, if true then return 0
* - Else, it will check if the buffer has reaches the end of the file, if true then it will set the eob_flag bit to 1
* - Else, it will set the eob_flag bit to 0
* - It will increment the getCPosition by 1 and return the character in that index
*/
char bufferGetChar(Buffer* const pBE) {
	/* Validate parameter buffer */
	if (!pBE)
		return RT_FAIL_2;

	///* If the buffer is empty */
	//if (bufferIsEmpty(pBE) == 1) {
	//	/* Return without adding anything*/
	//	return 0;
	//}

	/* Check if getCPosition and addCPosition are equal, reaches the end of file */
	if (pBE->getCPosition == pBE->addCPosition) {
		/* Set the flags field EOB is 1 */
		pBE->flags |= SET_EOB;
		return 0;
	}
	else

		/* Set the flags field EOB is 0 */
		pBE->flags &= RESET_EOB;

	/* Increment getCPosition by 1 and return the character of the chararacter array in that index */
	return pBE->string[pBE->getCPosition++];
}

/*
* Function name: bufferFree()
* Purpose: The purpose of this function is to free the memory of the buffer
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: free()
* Parameters: Buffer* - pBE
* Return value: Return void
* Algorithm: Validate the parameter buffer, then free the character array and the buffer structure
*/
void bufferFree(Buffer* const pBE) {
	/* Validate parameter buffer */
	if (!pBE)
		return;

	/* De-allocates (frees) character buffer array and the buffer structure */
	free(pBE->string);
	free(pBE);

	/* Return by default */
	return;
}

/*
* Function name: bufferClear()
* Purpose: The purpose of this function is to clear the offsets of the character position and the flag of the buffer
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: N/A
* Parameters: Buffer* - pBE
* Return value: Return 0 by default and RT_FAIL_1 if error occurred
* Algorithm:
*/
int bufferClear(Buffer* const pBE) {
	/* Validate parameter buffer */
	if (!pBE)
		return RT_FAIL_1;

	pBE->addCPosition = 0;
	pBE->markCPosition = 0;
	pBE->getCPosition = 0;
	//pBE->flags = DEFAULT_FLAGS;
	pBE->flags = pBE->flags & RESET_EOB;
	pBE->flags = pBE->flags & RESET_R_FLAG;
	/* Return 0 by default */
	return 0;
}

/*
* Function name: bufferRetract()
* Purpose: The purpose of this function is to validate and return the getCPosition
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: N/A
* Parameters: Buffer* pBE
* Return value: Return the getCPosition - 1
* Algorithm: Validate the parameter buffer and check if getCPosition - 1 is greater than SHRT_MIN, if true the return getCPosition - 1
*/
short bufferRetract(Buffer* const pBE) {
	/* Validate parameter buffer */
	if (!pBE)
		return RT_FAIL_1;
	return pBE->getCPosition = pBE->getCPosition - 1;
}

/*
* Function name: bufferReset()
* Purpose: The purpose of this function is to set getCPosition equal to markCPosition and return getCPosition
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: N/A
* Parameters:Buffer* pBE
* Return value: Return the value of getCPosition
* Algorithm: N/A
*/
short bufferReset(Buffer* const pBE) {
	/* Validate parameter buffer */
	if (!pBE)
		return RT_FAIL_1;

	pBE->getCPosition = pBE->markCPosition;

	return pBE->getCPosition;
}

/*
* Function name: bufferGetCPosition()
* Purpose: The purpose of this function is to return the getCPosition()
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: N/A
* Parameters: Buffer* - pBE
* Return value: Return getCPosition of the buffer structure
* Algorithm: N/A
*/
short bufferGetCPosition(Buffer* const pBE) {
	/* Validate parameter buffer */
	if (!pBE)
		return RT_FAIL_1;

	return pBE->getCPosition;
}

/*
* Function name: bufferSetMarkPosition()
* Purpose: The purpose of this function is to set the markCPosition
* Author: Kevin Truong / Dante Beltran
* History/Versions: 1.0.0
* Called functions: N/A
* Parameters: pBuffer - pBE, short - mark
* Return value: Return the buffer after setting markCPosition
* Algorithm: Validate the parameter buffer and the parameter mark position, then return the buffer after setting the markCPosition
*/
pBuffer bufferSetMarkPosition(pBuffer const pBE, short mark) {
	/* Validate parameter buffer */
	if (!pBE)
		return NULL;

	if (mark < 0 && mark > pBE->getCPosition)
		return NULL;

	pBE->markCPosition = mark;

	return pBE;
}
