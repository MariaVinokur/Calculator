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
 

/*gd -I ds/include ds/src/calc.c ds/src/stack.c ds/test/test_calc.c -o calc
*/
#ifndef __ILRD_CALC_H
#define __ILRD_CALC_H

/*in case of error returns NAN*/
double Calculate(const char *str);

#endif
