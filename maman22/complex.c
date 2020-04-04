

#include <stdio.h>
#include <stdlib.h> /* abs() sqrt()*/
#include <math.h>

#include "complex.h"

#define powf(a) ((a)*(a))
/***********************************************************/
/***********************************************************/

void read_comp(complex* comp,float real,float img)
{
	comp->img  = img;
	comp->real = real;
}

void print_comp(complex comp)
{
	if (comp.img >= 0)
		printf(YELLOW"%.2f + (%.2f)i\n"RESET,comp.real,comp.img);
	else
		printf(YELLOW"%.2f - (%.2d)i\n"RESET,comp.real,abs(comp.img));
}

void add_comp(complex comp_1, complex comp_2)
{
	complex temp;
	
	temp.real = comp_1.real + comp_2.real;
	temp.img  = comp_1.img  + comp_2.img;

	print_comp(temp);
}

void sub_comp(complex comp_1, complex comp_2)
{
	complex temp;
	
	temp.real = comp_1.real - comp_2.real;
	temp.img  = comp_1.img  - comp_2.img;

	print_comp(temp);
}

void mult_comp_real(complex comp, float real)
{
	complex temp;
	
	temp.real = comp.real * real;
	temp.img  = comp.img  * real;

	print_comp(temp);
}


void mult_comp_img(complex comp, float img)
{
	complex temp;
	
	temp.real = (-1) * comp.img * img;
	temp.img  = comp.real * img;

	print_comp(temp);
}

void mult_comp_comp(complex comp_1, complex comp_2)
{
	complex temp;
	
	temp.real = (comp_1.real * comp_2.real) -  ( comp_1.img  - comp_2.img);  /*ac - bd*/
	temp.img  = (comp_1.real * comp_2.img)  +(comp_1.img * comp_2.real);	 /*ad + bc*/

	print_comp(temp);	
}

void abs_comp(complex comp)
{
	float sqr = sqrt(powf(comp.real)+powf(comp.img));
	printf(YELLOW"%.2f\n"RESET,sqr);
}

/***********************************************************/
/***********************************************************/
