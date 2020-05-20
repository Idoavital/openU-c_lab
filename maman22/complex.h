
#if !defined(__COMPLEX_H__)
#define __COMPLEX_H__

/**************************************************************/
/**************************************************************/

/*Type for complex number*/
typedef struct Complex
{
	float real; /*The real number*/
	float img; /*The image number*/
}complex;

/*************************************************************/
/**************************************************************/
#define COLOR
#ifdef COLOR

#define YELLOW "\033[0;33m"		/*Yellow color*/
#define GREEN  "\033[0;32m"		/*Green color*/
#define RED    "\033[0;31m"		/*Red color*/
#define RESET  "\033[0;0m"		/*Reset to defualt color*/

#else

#define YELLOW " "		/*Yellow color*/
#define GREEN  " "		/*Green color*/
#define RED    " "		/*Red color*/
#define RESET  " "		/*Reset to defualt color*/

#endif

/**************************************************************/
/**************************************************************/


void read_comp(complex* comp,float real,float img);

void print_comp(complex comp);

void add_comp(complex comp_1, complex comp_2);

void sub_comp(complex comp_1, complex comp_2);

void mult_comp_real(complex comp, float real);

void mult_comp_img(complex comp, float img);

void mult_comp_comp(complex comp_1, complex comp_2);

void abs_comp(complex comp);

#endif /* __COMPLEX_H__*/
