/*  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 	*
 *	Title			:	calc Header File							        *
 *	Author			: 	Maria Vinokur	                                    *
 *  Date            :   14/06/2023        							   	
 *	Description  	: 	Contains type and function declarations				*
 *					  	required by calc 			                        *
 *					  														*
 *	HRD30	Infinity Labs R&D		   										*
 *					  														*
 *  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 	*/
 
#include <assert.h>
#include <stdlib.h> /*malloc, free*/
#include <string.h> /*strlen, strtod*/

#include "calc.h"
#include "stack.h" 

enum{CHAR_SIZE = sizeof(char), INT_SIZE = sizeof(int)};

/*Not A Number*/
#define NAN (1.0 / 0.0)

typedef struct
{
	stack_ty *nums;
	stack_ty *ops;
	int accum;
}calc_ty;

/*WFD: Waiting For Number*/
/*MON: Middle Of Number*/
/*WFO: Waiting For Operand*/
typedef enum {WFD, BAD, MON, WFO}state_ty;

/*OP: Operator*/
/*INVALID: invalid char, not a digit and not an operand*/
typedef enum {OP, DIGIT, SPACE, INVALID}event_ty;

/*precedence of operators*/
typedef enum {LOW, HIGH, NON}prec_ty;

typedef int(*action_func_ty)(calc_ty *calc, char input);

typedef double(*operation_func_ty)(double num1, double num2);

/*transition struct*/
typedef struct
{
	state_ty state;
	action_func_ty action;
}trans_ty;

static event_ty GetEventFromChar(const char ch_);
static trans_ty GetTransiotionIMP(state_ty curr_state, event_ty event);
static prec_ty GetPrecedenceIMP(const char ch_);
static operation_func_ty GetOperationFuncIMP(char ch_);

static double OperationMultIMP(double num1, double num2);
static double OperationAddIMP(double num1, double num2);
static double OperationSubIMP(double num1, double num2);
static double OperationDivIMP(double num1, double num2);

static int ActErrorIMP(calc_ty *calc, char ch);
static int ActAccumeIMP(calc_ty *calc, char ch);
static int ActNothingIMP(calc_ty *calc, char ch);
static int ActSaveNumNOpIMP(calc_ty *calc, char ch);
static int ActSaveNumIMP(calc_ty *calc, char ch);
static int ActSaveOpIMP(calc_ty *calc, char ch);
static double CalcStacksIMP(calc_ty *calc);
static void CalcDestroyIMP(calc_ty *calc);
static calc_ty *InitCalcIMP(size_t exp_len_);

static calc_ty *InitCalcIMP(size_t exp_len_)
{
	calc_ty *calc = NULL;
	
	calc = malloc(sizeof(calc_ty));
	if(NULL == calc)
	{
		return NULL;
	}
	
	calc->nums = StackCreate(exp_len_, INT_SIZE);
	if(NULL == calc->nums)
	{
		return NULL;
	}
		
	calc->ops = StackCreate(exp_len_, CHAR_SIZE);
	if(NULL == calc->ops)
	{
		return NULL;
	}
	
	calc->accum = 0;
	
	return calc;
}


double Calculate(const char *math_exp)
{
	calc_ty *calc = NULL;
	
	int exp_idx = 0;
	
	int exp_len = strlen(math_exp);
	
	double ret = 0;

	/*curr state = WFD*/
	state_ty curr_state = WFD;
	
	/*assert*/
	assert(math_exp);
	
	/*init calc*/
	calc = InitCalcIMP(exp_len);
	if(NULL == calc)
	{
		return NAN;
	}

	/*while(input not '\0' && state is not BAD)*/
	for(exp_idx = 0; exp_idx < exp_len && curr_state != BAD; ++exp_idx)
	{
		event_ty event = 0;
		trans_ty trans = {0};
		
		/*event = CharToEvent()*/
		event = GetEventFromChar(math_exp[exp_idx]);
		
		/*curr_state = GetNextState()*/
		trans = GetTransiotionIMP(curr_state, event);
		
		/*update curr state*/
		curr_state = trans.state;

		/*do action()*/
		ret = trans.action(calc, math_exp[exp_idx]);
		if(0 != ret)
		{
			curr_state = BAD;
		}
	}
	
	if(BAD == curr_state)
	{
		ret = NAN;
	}
	
	else if(MON == curr_state)
	{
		/*push num*/
		StackPush(calc->nums, &calc->accum);
		ret = CalcStacksIMP(calc);
	}
	
	else
	{
		ret = CalcStacksIMP(calc);
	}
	
	CalcDestroyIMP(calc);
	return ret;
}

static void CalcDestroyIMP(calc_ty *calc)
{
	assert(calc);
	
	StackDestroy(calc->nums);
	calc->nums = NULL;
	
	StackDestroy(calc->ops);
	calc->ops = NULL;
	
	calc->accum = 0;
	free(calc);	
	
}

static operation_func_ty GetOperationFuncIMP(char ch_)
{
	/*create LUT of chars representing valid and invalid chars*/
	static const operation_func_ty LUT[] = {/*0 */NULL, /*1 */NULL, /*2 */NULL, /*3 */NULL, /*4 */NULL, /*5 */NULL, /*6 */NULL, /*7 */NULL, /*8*/NULL, /*9 */NULL, /*10 */NULL, /*11 */NULL, /*12 */NULL, /*13 */NULL, /*14 */NULL, /*15 */NULL, /*16 */NULL, /*17 */NULL, /*18 */NULL, /*19 */NULL, /*20 */NULL, /*21 */NULL, /*22 */NULL, /*23 */NULL, /*24 */NULL, /*25 */NULL, /*26 �*/NULL, /*27 */ NULL, /*28 */NULL, /*29 */NULL, /*30 */NULL, /*31 */NULL, /*32  */NULL, /*33 !*/NULL, /*34 "*/NULL, /*35 #*/NULL, /*36 $*/NULL, /*37 %*/NULL, /*38 &*/NULL, /*39 '*/NULL, /*40 (*/NULL, /*41 )*/NULL, /*42 **/OperationMultIMP, /*43 +*/OperationAddIMP, /*44 ,*/NULL, /*45 -*/OperationSubIMP, /*46 .*/NULL, /*47 /*/OperationDivIMP};
	
	/*return LUT[ch_]*/
	
	return LUT[(int)ch_];
}

static double OperationMultIMP(double num1, double num2)
{
	return num1 * num2;
}

static double OperationAddIMP(double num1, double num2)
{
	return num1 + num2;
}

static double OperationSubIMP(double num1, double num2)
{
	return num2 - num1;
}

static double OperationDivIMP(double num1, double num2)
{
	return num2 / num1;
}

static prec_ty GetPrecedenceIMP(const char ch_)
{
	/*create LUT of chars representing valid and invalid chars*/
	static const prec_ty LUT[] = {/*0 */NON, /*1 */NON, /*2 */NON, /*3 */NON, /*4 */NON, /*5 */NON, /*6 */NON, /*7 */NON, /*8*/NON, /*9 */NON, /*10 */NON, /*11 */NON, /*12 */NON, /*13 */NON, /*14 */NON, /*15 */NON, /*16 */NON, /*17 */NON, /*18 */NON, /*19 */NON, /*20 */NON, /*21 */NON, /*22 */NON, /*23 */NON, /*24 */NON, /*25 */NON, /*26 �*/NON, /*27 */ NON, /*28 */NON, /*29 */NON, /*30 */NON, /*31 */NON, /*32  */NON, /*33 !*/NON, /*34 "*/NON, /*35 #*/NON, /*36 $*/NON, /*37 %*/NON, /*38 &*/NON, /*39 '*/NON, /*40 (*/NON, /*41 )*/NON, /*42 **/HIGH, /*43 +*/LOW, /*44 ,*/NON, /*45 -*/LOW, /*46 .*/NON, /*47 /*/HIGH};
	
	/*return LUT[ch_]*/
	return LUT[(int)ch_];
}

static trans_ty GetTransiotionIMP(state_ty curr_state, event_ty event)
{
	/*create LUT of chars representing valid and invalid chars*/
	static const trans_ty LUT[][4] = 
	/*        OP                        DIGIT                SPACE                 INVALID*/
		{
/*WFD*/  {{WFD, ActErrorIMP},       {MON, ActAccumeIMP}, {WFD, ActNothingIMP}, {BAD, ActErrorIMP}},
/*BAD*/  {{BAD, ActErrorIMP},       {BAD, ActErrorIMP},  {BAD, ActErrorIMP},   {BAD, ActErrorIMP}},
/*MON*/	 {{WFD, ActSaveNumNOpIMP},  {BAD, ActErrorIMP},  {WFO, ActSaveNumIMP},   {BAD, ActErrorIMP}},
/*WFO*/	 {{WFD, ActSaveOpIMP},      {BAD, ActErrorIMP},  {WFO, ActNothingIMP}, {BAD, ActErrorIMP}}
		};
	
	/*return LUT[curr_state][event]*/
	return LUT[curr_state][event];
}

static event_ty GetEventFromChar(const char ch_)
{
	/*create LUT of chars representing valid and invalid chars*/
	static const event_ty LUT[] = {/*0 */INVALID, /*1 */INVALID, /*2 */INVALID, /*3 */INVALID, /*4 */INVALID, /*5 */INVALID, /*6 */INVALID, /*7 */INVALID, /*8*/INVALID, /*9 */INVALID, /*10 */INVALID, /*11 */INVALID, /*12 */SPACE, /*13 */INVALID, /*14 */INVALID, /*15 */INVALID, /*16 */INVALID, /*17 */INVALID, /*18 */INVALID, /*19 */INVALID, /*20 */INVALID, /*21 */INVALID, /*22 */INVALID, /*23 */INVALID, /*24 */INVALID, /*25 */INVALID, /*26 �*/INVALID, /*27 */ INVALID, /*28 */INVALID, /*29 */INVALID, /*30 */INVALID, /*31 */INVALID, /*32  */SPACE, /*33 !*/INVALID, /*34 "*/INVALID, /*35 #*/INVALID, /*36 $*/INVALID, /*37 %*/INVALID, /*38 &*/INVALID, /*39 '*/INVALID, /*40 (*/INVALID, /*41 )*/INVALID, /*42 **/OP, /*43 +*/OP, /*44 ,*/INVALID, /*45 -*/OP, /*46 .*/INVALID, /*47 /*/OP, /*48 0*/DIGIT, /*49 1*/DIGIT, /*50 2*/DIGIT, /*51 3*/DIGIT, /*52 4*/DIGIT, /*53 5*/DIGIT, /*54 6*/DIGIT, /*55 7*/DIGIT, /*56 8*/DIGIT, /*57 9*/DIGIT, /*58 :*/INVALID, /*59 ;*/INVALID, /*60 <*/INVALID, /*61 =*/INVALID, /*62 >*/INVALID, /*63 ?*/INVALID, /*64 @*/INVALID, /*65 A*/INVALID, /*66 B*/INVALID, /*67 C*/INVALID, /*68 D*/INVALID, /*69 E*/INVALID, /*70 F*/INVALID, /*71 G*/INVALID, /*72 H*/INVALID, /*73 I*/INVALID, /*74 J*/INVALID, /*75 K*/INVALID, /*76 L*/INVALID, /*77 M*/INVALID, /*78 N*/INVALID, /*79 O*/INVALID, /*80 P*/INVALID, /*81 Q*/INVALID, /*82 R*/INVALID, /*83 S*/INVALID, /*84 T*/INVALID, /*85 U*/INVALID, /*86 V*/INVALID, /*87 W*/INVALID, /*88 X*/INVALID, /*89 Y*/INVALID, /*90 Z*/INVALID, /*91 [*/INVALID, /*92 \*/INVALID, /*93 ]*/INVALID, /*94 ^*/INVALID, /*95 _*/INVALID, /*96 `*/INVALID, /*97 a*/INVALID, /*98 b*/INVALID, /*99 c*/INVALID, /*100 d*/INVALID, /*101 e*/INVALID, /*102 f*/INVALID, /*103 g*/INVALID, /*104 h*/INVALID, /*105 i*/INVALID, /*106 j*/INVALID, /*107 k*/INVALID, /*108 l*/INVALID, /*109 m*/INVALID, /*110 n*/INVALID, /*111 o*/INVALID, /*112 p*/INVALID, /*113 q*/INVALID, /*114 r*/INVALID, /*115 s*/INVALID, /*116 t*/INVALID, /*117 u*/INVALID, /*118 v*/INVALID, /*119 w*/INVALID, /*120 x*/INVALID, /*121 y*/INVALID, /*122 z*/INVALID, /*123 {*/INVALID, /*124 |*/INVALID, /*125 }*/INVALID, /*126 ~*/INVALID, /*127 */INVALID};
	
	/*return LUT[ch_]*/
	return LUT[(int)ch_];
}

static int ActErrorIMP(calc_ty *calc, char ch)
{
	(void)calc;
	(void)ch;
	
	return -1;
}

static int ActAccumeIMP(calc_ty *calc, char ch)
{
	char *end_ptr = NULL;
	char str[2] = {'\0', '\0'};
	
	assert(calc);
	
	str[0] = ch;

	calc->accum *= 10;
	calc->accum += strtod(str, &end_ptr);
	
	return 0;
}

static int ActNothingIMP(calc_ty *calc, char ch)
{	
	(void)calc;
	(void)ch;
	return 0;
}

static int ActSaveNumNOpIMP(calc_ty *calc, char ch)
{
	int ret = 0;
	
	assert(calc);

	ret = ActSaveNumIMP(calc, ch);
	ret = ActSaveOpIMP(calc, ch);
	
	return ret;
}

static int ActSaveNumIMP(calc_ty *calc, char ch)
{
	assert(calc);
	
	(void)ch;

	/*if stack not full*/
	if(StackSize(calc->nums) < StackCapacity(calc->nums))
	{
		/*push num*/
		StackPush(calc->nums, &calc->accum);
		
		/*reset accum*/
		calc->accum = 0;
		
		return 0;
	}
	
	return -1;
}

static int ActSaveOpIMP(calc_ty *calc, char ch)
{
	char peeked_op = 0;
	
	int peeked_num1 = 0;
	
	int peeked_num2 = 0;
	
	operation_func_ty op_func = NULL;
	
	int res = 0;
	
	/*assert*/
	assert(calc);
	
	/*if stack not full and not empty*/
	if(StackSize(calc->ops) < StackCapacity(calc->ops))
	{
		/*while top op has higher precedence than ch*/
		while(!StackIsEmpty(calc->ops) && GetPrecedenceIMP(*(char *)StackPeek(calc->ops)) > GetPrecedenceIMP(ch))
		{
			peeked_op = *(char *)StackPeek(calc->ops);

			/*pop two numbers*/
			peeked_num1 = *(int *)StackPeek(calc->nums);
			StackPop(calc->nums);
			
			
			peeked_num2 = *(int *)StackPeek(calc->nums);
			StackPop(calc->nums);

			/*do action*/
			op_func = GetOperationFuncIMP(peeked_op);
			res = op_func(peeked_num1, peeked_num2);
			StackPop(calc->ops);
			
			/*push result back*/
			StackPush(calc->nums, &res);
		}
			
		/*push to ops stack*/
		StackPush(calc->ops, &ch);	
		return 0;
	}
	
	return -1;

}

static double CalcStacksIMP(calc_ty *calc)
{	
	char peeked_op = 0;
	
	int peeked_num1 = 0;
	
	int peeked_num2 = 0;

	operation_func_ty op_func = NULL;
	int res = 0;
	
	/*assert*/
	assert(calc);
	
	/*while top op has higher precedence than ch*/
	while(!StackIsEmpty(calc->nums))
	{
			/*pop two numbers*/
			peeked_num1 = *(int *)StackPeek(calc->nums);
			StackPop(calc->nums);
			
			peeked_num2 = *(int *)StackPeek(calc->nums);
			StackPop(calc->nums);
			
			peeked_op = *(char *)StackPeek(calc->ops);
			StackPop(calc->ops);
			
			/*do action*/
			op_func = GetOperationFuncIMP(peeked_op);
			res = op_func(peeked_num1, peeked_num2);
			
			/*push result back*/
			if(!StackIsEmpty(calc->nums))
			{
				StackPush(calc->nums, &res);
			}
	}
	
	return res;
}










