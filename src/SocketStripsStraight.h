/*
 * SocketStripsStraight.h
 *
 *  Created on: 25-08-2015
 *      Author: michal
 */

#ifndef SOCKETSTRIPSSTRAIGHT_H_
#define SOCKETSTRIPSSTRAIGHT_H_

#include "Footprint.h"

class SocketStripsStraight: public Footprint {
private:
	double pads_rows_spacing;
	int pads_per_row;
	int rows_count;
	double pitch;
	double pad_length;
	double pad_width;
	double pad_drill;
	double body_width_max;
	double body_length_max;

	double tht_offs_x;
	double tht_offs_y;

public:
	SocketStripsStraight();

	void ReadParameters(char table[100][100][500], int row);
//	void CalcLandPattern(void);
	void CalcAssemblyScale(void);
	void GeneratePads(FILE *footprint_file);
	void PrintFootprint(FILE *footprint_file);
	void GenerateAssembly(FILE *footprint_file, double assem_scale);
	void GenerateCourtyard(FILE *footprint_file);
	void GenerateSilkScreen(FILE *footprint_file);

	void CalcOffset(void);

	virtual ~SocketStripsStraight();
};


#endif /* SOCKETSTRIPSSTRAIGHT_H_ */
