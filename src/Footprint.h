/*
 * Footprint.h
 *
 *  Created on: 29-06-2015
 *      Author: michal
 */

#ifndef FOOTPRINT_H_
#define FOOTPRINT_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define SILKS_LINE_WIDTH 0.15
#define CRTYD_LINE_WIDTH 0.05
#define ASSEM_LINE_WIDTH 0.1
#define SILKS_TEXT_SIZE 1
#define ASSEM_TEXT_SIZE 1
#define MIN_ASSEM_TEXT_SIZE 0.127
#define CRTYD_CLEARANCE 0.25
#define ASSEM_CHAMFER 1
#define CRTYD_CLEARANCE_2 0.5

#define MAX_TEXT_LENGTH 500	//size of most of strings
#define MAX(X,Y) (((X) > (Y)) ? (X) : (Y))


class Footprint {
protected:
	char footprint_name[MAX_TEXT_LENGTH];
	char library_name[MAX_TEXT_LENGTH];
	char package_type[MAX_TEXT_LENGTH];
	char description[MAX_TEXT_LENGTH];
	char tags[MAX_TEXT_LENGTH];
	char pad_shape[MAX_TEXT_LENGTH];
	char silks_option[MAX_TEXT_LENGTH];
	char land_pattern_option[MAX_TEXT_LENGTH];
	double assem_scale;

public:
	Footprint();

	double r01(double unrounded);
	double r05(double unrounded);
	double r05up(double unrounded);
	char *f2c(double float_val);

	char *GetLibraryName(void);
	char *GetFootprintName(void);

	virtual ~Footprint();
};

#endif /* FOOTPRINT_H_ */
