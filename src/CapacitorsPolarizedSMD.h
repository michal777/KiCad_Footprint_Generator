/*
 * CapacitorsPolarizedSMD.h
 *
 *  Created on: 30-06-2015
 *      Author: michal
 */

#ifndef CAPACITORSPOLARIZEDSMD_H_
#define CAPACITORSPOLARIZEDSMD_H_

#include "Footprint.h"

class CapacitorsPolarizedSMD : public Footprint {
private:
	char manufacturer[MAX_TEXT_LENGTH];
	char datasheet[MAX_TEXT_LENGTH];
	double pad_length;
	double pad_width;
	double pad_dist;
	double pad_dist_end2end;
	double body_width_max;
	double body_length_max;
	double body_height_max;
	double toe_to_toe_min;
	double toe_to_toe_max;
	double terminal_length_min;
	double terminal_length_max;
	double terminal_width_min;
	double terminal_width_max;

public:
	CapacitorsPolarizedSMD();

	void ReadParameters(char table[100][100][500], int row);
	void CalcLandPattern(void);
	void CalcAssemblyScale(void);
	void GeneratePads(FILE *footprint_file);
	void PrintFootprint(FILE *footprint_file);
	void GenerateAssembly(FILE *footprint_file, double assem_scale);
	void GenerateCourtyard(FILE *footprint_file);
	void GenerateCourtyardSilkScreenOutline(FILE *footprint_file);
	void GenerateCourtyardNoSilkScreen(FILE *footprint_file);
	void GenerateSilkScreen(FILE *footprint_file);
	void GenerateSilkScreenOutline(FILE *footprint_file);
	void GenerateSilkScreenMinimal(FILE *footprint_file);

	virtual ~CapacitorsPolarizedSMD();
};

#endif /* CAPACITORSPOLARIZEDSMD_H_ */
