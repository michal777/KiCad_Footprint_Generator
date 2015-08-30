/*
 * SmallOutlinePackage.h
 *
 *  Created on: 29-06-2015
 *      Author: michal
 */

#ifndef SMALLOUTLINEPACKAGE_H_
#define SMALLOUTLINEPACKAGE_H_

#include "Footprint.h"

class SmallOutlinePackage : public Footprint {
private:
	int pads_count_x;
	int pads_count_y;
	double x_pads_center;
	double x_pads_outer;
	double pitch;
	double pad_length;
	double pad_width;
	double exposed_pad_x;
	double exposed_pad_y;
	double exp_pad_subdiv_x;
	double exp_pad_subdiv_y;
	double exp_pad_paste_ratio;
	double body_size_max_x;
	double body_size_max_y;
	double toe_to_toe_min;
	double toe_to_toe_max;
	double terminal_length_min;
	double terminal_length_max;
	double terminal_width_min;
	double terminal_width_max;

public:
	SmallOutlinePackage();

	void ReadParameters(char table[100][100][500], int row);
	void CalcLandPattern(void);
	void CalcAssemblyScale(void);
	void GeneratePads(FILE *footprint_file);
	void PrintFootprint(FILE *footprint_file);
	void GenerateAssembly(FILE *footprint_file, double assem_scale);
	void GenerateCourtyard(FILE *footprint_file);
	void GenerateSilkScreen(FILE *footprint_file);

	virtual ~SmallOutlinePackage();
};

#endif /* SMALLOUTLINEPACKAGE_H_ */
