/*  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 	*
 *	Title			:	calc Header File							        *
 *	Author			: 	Maria Vinokur	                                    *
 *  Date            :   14/06/2023        							   	*
 *	Description  	: 	Contains type and function declarations				*
 *					  	required by calc 			                        *
 *					  														*
 *	HRD30	Infinity Labs R&D		   										*
 *					  														*
 *  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 	*/
 
 
#include "calc.h"

#include <stdio.h>  /*  printf, puts  */


int main(void)
{
	printf("calc res: %f\n", Calculate("2 + 7 * 4 * 2 - 6  "));
	return 0;
}


