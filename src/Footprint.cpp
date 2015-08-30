/*
 * Footprint.cpp
 *
 *  Created on: 29-06-2015
 *      Author: michal
 */

#include "Footprint.h"

Footprint::Footprint() {
	*footprint_name = '\0';
	*library_name = '\0';
	*package_type = '\0';
	*description = '\0';
	*tags = '\0';
	*pad_shape = '\0';
	*silks_option = '\0';
	*land_pattern_option = '\0';
	assem_scale = 1;
}




//////////////////////round to 0.01mm/////////////////////////
double Footprint::r01(double unrounded)
{
	return round(unrounded * 100) / 100;
}
//////////////////round to 0.05mm/////////////////////////////
double Footprint::r05(double unrounded)
{
	return round(unrounded * 20) / 20;
}
//////////////////round UP to 0.05mm//////////////////////////
double Footprint::r05up(double unrounded)
{
	double rounded = round(unrounded * 20) / 20;
	double rounded_up;

	if((unrounded > 0) && (rounded < unrounded))
		rounded_up = rounded + 0.05;
	else if((unrounded < 0) && (rounded > unrounded))
		rounded_up = rounded - 0.05;
	else
		rounded_up = rounded;
	return rounded_up;
}
/////////////////double float to string convertion////////////
char *Footprint::f2c(double float_val)
{
	char *string_val = (char*) malloc(sizeof(char) * MAX_TEXT_LENGTH);
	if(float_val > 0)
		sprintf (string_val, "%.5lg", float_val + 0.0000001);
	else if(float_val < 0)
		sprintf (string_val, "%.5lg", float_val - 0.0000001);
	else
		sprintf (string_val, "%.5lg", float_val);
	return string_val;
}


char *Footprint::GetLibraryName(void)
{
	return library_name;
}
char *Footprint::GetFootprintName(void)
{
	return footprint_name;
}


Footprint::~Footprint() {
	// TODO Auto-generated destructor stub
}

