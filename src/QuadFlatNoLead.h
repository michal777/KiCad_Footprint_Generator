/*
 * QuadFlatNoLead.h
 *
 *  Created on: 30-06-2015
 *      Author: michal
 */

#ifndef QUADFLATNOLEAD_H_
#define QUADFLATNOLEAD_H_

#include "Footprint.h"

class QuadFlatNoLead : public Footprint {
private:
	int pads_count_x;
	int pads_count_y;
	double x_pads_center;
	double y_pads_center;
	double x_pads_outer;
	double y_pads_outer;
	double pitch;
	double pitch_x;
	double pitch_y;
	double pad_length;
	double pad_length_x;
	double pad_length_y;
	double pad_width;
	double pad_width_x;
	double pad_width_y;
	double exposed_pad_x;
	double exposed_pad_y;
	double exp_pad_subdiv_x;
	double exp_pad_subdiv_y;
	double exp_pad_paste_ratio;
	double body_size_max_x;
	double body_size_max_y;
	double toe_to_toe_min_x;
	double toe_to_toe_max_x;
	double terminal_length_min_x;
	double terminal_length_max_x;
	double terminal_width_min_x;
	double terminal_width_max_x;
	double toe_to_toe_min_y;
	double toe_to_toe_max_y;
	double terminal_length_min_y;
	double terminal_length_max_y;
	double terminal_width_min_y;
	double terminal_width_max_y;

public:
	QuadFlatNoLead();

	void ReadParameters(char table[100][100][500], int row);
	void CalcLandPattern(void);
	void CalcAssemblyScale(void);
	void GeneratePads(FILE *footprint_file);
	void PrintFootprint(FILE *footprint_file);
	void GenerateAssembly(FILE *footprint_file, double assem_scale);
	void GenerateCourtyard(FILE *footprint_file);
	void GenerateSilkScreen(FILE *footprint_file);

	virtual ~QuadFlatNoLead();
};

#endif /* QUADFLATNOLEAD_H_ */
