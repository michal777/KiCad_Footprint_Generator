/*
 * CapacitorsChipSMD.cpp
 *
 *  Created on: 30-06-2015
 *      Author: michal
 */

#include "CapacitorsChipSMD.h"

CapacitorsChipSMD::CapacitorsChipSMD() {
	manufacturer[MAX_TEXT_LENGTH] = '\0';
	pad_length = 0;
	pad_width = 0;
	pad_dist = 0;
	pad_dist_end2end = 0;
	body_width_max = 0;
	body_length_max = 0;
	body_height_max = 0;
	toe_to_toe_min = 0;
	toe_to_toe_max = 0;
	terminal_length_min = 0;
	terminal_length_max = 0;
	terminal_width_min = 0;
	terminal_width_max = 0;
}


void CapacitorsChipSMD::ReadParameters(char table[100][100][500], int row) {
	int column;
	for (column = 0; '\0' != table[0][column][0]; ++column)
	{
		if (!strcmp(table[0][column], "Footprint name"))
			strcpy(footprint_name, table[row][column]);
		if (!strcmp(table[0][column], "Library name"))
			strcpy(library_name, table[row][column]);
		if (!strcmp(table[0][column], "Package type"))
			strcpy(package_type, table[row][column]);
		if (!strcmp(table[0][column], "Description"))
			strcpy(description, table[row][column]);
		if (!strcmp(table[0][column], "Manufacturer"))
			strcpy(manufacturer, table[row][column]);
		if (!strcmp(table[0][column], "Datasheet"))
			strcpy(datasheet, table[row][column]);
		if (!strcmp(table[0][column], "Tags"))
			strcpy(tags, table[row][column]);
		if (!strcmp(table[0][column], "Pad length"))
			pad_length = atof (table[row][column]);
		if (!strcmp(table[0][column], "Pad width"))
			pad_width = atof (table[row][column]);
		if (!strcmp(table[0][column], "Distance between pads centers"))
			pad_dist = atof (table[row][column]);
		if (!strcmp(table[0][column], "Distance between pads - end to end"))
			pad_dist_end2end = atof (table[row][column]);

		if (!strcmp(table[0][column], "Overall width toe-to-toe min"))
			toe_to_toe_min = atof (table[row][column]);
		if (!strcmp(table[0][column], "Overall width toe-to-toe max"))
			toe_to_toe_max = atof (table[row][column]);
		if (!strcmp(table[0][column], "Terminal length min"))
			terminal_length_min = atof (table[row][column]);
		if (!strcmp(table[0][column], "Terminal length max"))
			terminal_length_max = atof (table[row][column]);
		if (!strcmp(table[0][column], "Terminal width min"))
			terminal_width_min = atof (table[row][column]);
		if (!strcmp(table[0][column], "Terminal width max"))
			terminal_width_max = atof (table[row][column]);

		if (!strcmp(table[0][column], "Pad shape"))
			strcpy(pad_shape, table[row][column]);
		if (!strcmp(table[0][column], "Silkscreen option"))
			strcpy(silks_option, table[row][column]);
		if (!strcmp(table[0][column], "Land pattern option"))
			strcpy(land_pattern_option, table[row][column]);

		if (!strcmp(table[0][column], "Body width max"))
			body_width_max = atof (table[row][column]);
		if (!strcmp(table[0][column], "Body length max"))
			body_length_max = atof (table[row][column]);
		if (!strcmp(table[0][column], "Body height max"))
			body_height_max = atof (table[row][column]);
	}
}


void CapacitorsChipSMD::CalcLandPattern(void) {
	double F;
	double P;
	double j_t;
	double j_h;
	double j_s;
	double L_tol;
	double W_tol;
	double S_tol;
	double S_mean;
	double S_max;
	double Z_max;
	double G_min;

	if (!strcmp(land_pattern_option, "ipc_a"))
	{
		F = 0.1;
		P = 0.1;
		j_h = 0;
		j_s = 0.05;
		if(body_length_max < 0.2 * 2.54)
			j_t = 0.10;
		else if(body_length_max < 0.4 * 2.54)
			j_t = 0.12;
		else if(body_length_max < 0.6 * 2.54)
			j_t = 0.15;
		else
			j_t = 0.55;

		L_tol = toe_to_toe_max - toe_to_toe_min;
		W_tol = terminal_width_max - terminal_width_min;
		S_tol = sqrt (pow(L_tol, 2) + 2 * pow((terminal_length_max - terminal_length_min), 2));
		S_mean = ((toe_to_toe_min - 2 * terminal_length_max) + (toe_to_toe_max - 2 * terminal_length_min)) / 2;
		S_max = S_mean + S_tol / 2;
		Z_max = toe_to_toe_min + sqrt (pow(L_tol, 2) + pow(F, 2) + pow(P, 2)) + 2 * j_t;
		G_min = S_max - sqrt (pow(S_tol, 2) + pow(F, 2) + pow(P, 2)) - 2 * j_h;

		pad_length = (Z_max - G_min) / 2;
		pad_dist = G_min + pad_length;
		pad_width = terminal_width_min + sqrt (pow(W_tol, 2) + pow(F, 2) + pow(P, 2)) + 2 * j_s;
	}
	else if (!strcmp(land_pattern_option, "ipc_b"))
	{
		F = 0.1;
		P = 0.1;
		j_h = 0;
		j_s = 0;
		if(body_length_max < 0.2 * 2.54)
			j_t = 0.10;
		else if(body_length_max < 0.4 * 2.54)
			j_t = 0.12;
		else if(body_length_max < 0.6 * 2.54)
			j_t = 0.15;
		else
			j_t = 0.35;

		L_tol = toe_to_toe_max - toe_to_toe_min;
		W_tol = terminal_width_max - terminal_width_min;
		S_tol = sqrt (pow(L_tol, 2) + 2 * pow((terminal_length_max - terminal_length_min), 2));
		S_mean = ((toe_to_toe_min - 2 * terminal_length_max) + (toe_to_toe_max - 2 * terminal_length_min)) / 2;
		S_max = S_mean + S_tol / 2;
		Z_max = toe_to_toe_min + sqrt (pow(L_tol, 2) + pow(F, 2) + pow(P, 2)) + 2 * j_t;
		G_min = S_max - sqrt (pow(S_tol, 2) + pow(F, 2) + pow(P, 2)) - 2 * j_h;

		pad_length = (Z_max - G_min) / 2;
		pad_dist = G_min + pad_length;
		pad_width = terminal_width_min + sqrt (pow(W_tol, 2) + pow(F, 2) + pow(P, 2)) + 2 * j_s;
	}
	else if (!strcmp(land_pattern_option, "ipc_c"))
	{
		F = 0.1;
		P = 0.1;
		j_h = 0;
		j_s = -0.05;
		if(body_length_max < 0.2 * 2.54)
			j_t = 0.10;
		else if(body_length_max < 0.4 * 2.54)
			j_t = 0.12;
		else if(body_length_max < 0.6 * 2.54)
			j_t = 0.15;
		else
			j_t = 0.15;

		L_tol = toe_to_toe_max - toe_to_toe_min;
		W_tol = terminal_width_max - terminal_width_min;
		S_tol = sqrt (pow(L_tol, 2) + 2 * pow((terminal_length_max - terminal_length_min), 2));
		S_mean = ((toe_to_toe_min - 2 * terminal_length_max) + (toe_to_toe_max - 2 * terminal_length_min)) / 2;
		S_max = S_mean + S_tol / 2;
		Z_max = toe_to_toe_min + sqrt (pow(L_tol, 2) + pow(F, 2) + pow(P, 2)) + 2 * j_t;
		G_min = S_max - sqrt (pow(S_tol, 2) + pow(F, 2) + pow(P, 2)) - 2 * j_h;

		pad_length = (Z_max - G_min) / 2;
		pad_dist = G_min + pad_length;
		pad_width = terminal_width_min + sqrt (pow(W_tol, 2) + pow(F, 2) + pow(P, 2)) + 2 * j_s;
	}
	else if (!strcmp(land_pattern_option, "specified"))
	{
		if (pad_length && pad_width && pad_dist_end2end)
			pad_dist = pad_dist_end2end - pad_length;	//if distance between rows of pads are not specified, calculate it using distance between ends of pads and dimension of pad
		else if (!strcmp(land_pattern_option, "specified") && (pad_length && pad_width && pad_dist))
			{}	//use dimensions read from csv
		else
			printf("warning: bad pads parameters ");
	}
	else
		printf("warning: bad pads parameters ");
}

void CapacitorsChipSMD::CalcAssemblyScale(void) {
	while (0.5 * assem_scale * ASSEM_TEXT_SIZE > MIN_ASSEM_TEXT_SIZE)
		if ((body_length_max < 5 * assem_scale * ASSEM_TEXT_SIZE) || (body_width_max < 1.5 * assem_scale * ASSEM_TEXT_SIZE))
			assem_scale = 0.5 * assem_scale;
		else
			break;
}

void CapacitorsChipSMD::GeneratePads(FILE* footprint_file) {
	fprintf (footprint_file, "  (pad 1 smd %s (at %s 0) (size %s %s) (layers F.Cu F.Paste F.Mask))\n", pad_shape, f2c(-pad_dist / 2), f2c(pad_length), f2c(pad_width));
	fprintf (footprint_file, "  (pad 2 smd %s (at %s 0) (size %s %s) (layers F.Cu F.Paste F.Mask))\n", pad_shape, f2c(pad_dist / 2), f2c(pad_length), f2c(pad_width));
}

void CapacitorsChipSMD::PrintFootprint(FILE* footprint_file) {
	fprintf(footprint_file, "(module %s (layer F.Cu) (tedit 54130A77)\n", footprint_name);
	//	fprintf(footprint_file, "  (descr \"%s\")\n", description);
	fprintf (footprint_file, "  (descr \"%s\")\n", description);
	fprintf(footprint_file, "  (tags \"%s\")\n", tags);
	fprintf(footprint_file, "  (attr smd)\n");
	fprintf(footprint_file, "  (fp_text reference REF** (at 0 %s) (layer F.SilkS)\n", f2c(-body_width_max / 2 - SILKS_TEXT_SIZE));
	fprintf(footprint_file, "    (effects (font (size %s %s) (thickness %s)))\n  )\n", f2c(SILKS_TEXT_SIZE), f2c(SILKS_TEXT_SIZE), f2c(SILKS_LINE_WIDTH));
	fprintf(footprint_file, "  (fp_text value %s (at 0 %s) (layer F.SilkS) hide\n", footprint_name, f2c(body_width_max / 2 + SILKS_TEXT_SIZE));
	fprintf(footprint_file, "    (effects (font (size %s %s) (thickness %s)))\n  )\n", f2c(SILKS_TEXT_SIZE), f2c(SILKS_TEXT_SIZE), f2c(SILKS_LINE_WIDTH));
	fprintf(footprint_file, "  (fp_text user %%R (at 0 0) (layer F.Fab)\n");
	fprintf(footprint_file, "    (effects (font (size %s %s) (thickness %s)))\n  )\n", f2c(assem_scale * ASSEM_TEXT_SIZE), f2c(assem_scale * ASSEM_TEXT_SIZE), f2c(assem_scale * ASSEM_LINE_WIDTH));
	GenerateAssembly (footprint_file, assem_scale);
	if (!strcmp(silks_option, "visible_after_assembly"))
	{
		GenerateSilkScreen (footprint_file);
		GenerateCourtyard (footprint_file);
	}
	else if (!strcmp(silks_option, "outline"))
	{
		GenerateSilkScreenOutline (footprint_file);
		GenerateCourtyardSilkScreenOutline (footprint_file);
	}
	else if (!strcmp(silks_option, "minimal"))
	{
		GenerateSilkScreenMinimal (footprint_file);
		GenerateCourtyardNoSilkScreen (footprint_file);
	}
	else
		GenerateCourtyardNoSilkScreen (footprint_file);
	GeneratePads (footprint_file);
	fprintf(footprint_file, "  (model %s.3dshapes/%s.wrl\n", library_name, footprint_name);	//adds path to 3D module which, I hope, will exist in future
	fprintf(footprint_file, "    (at (xyz 0 0 0))\n");
	fprintf(footprint_file, "    (scale (xyz 1 1 1))\n");
	fprintf(footprint_file, "    (rotate (xyz 0 0 0))\n");
	fprintf(footprint_file, "  )\n");
	fprintf(footprint_file, ")\n");
}

void CapacitorsChipSMD::GenerateAssembly(FILE* footprint_file, double assem_scale) {
	double start_line_x;
	double end_line_x;
	double start_line_y;
	double end_line_y;
	double na_x = body_length_max / 2;
	double na_y = body_width_max / 2;

	start_line_x = end_line_x = na_x;
	start_line_y = na_y;
	end_line_y = -na_y;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x), f2c(start_line_y), f2c(end_line_x), f2c(end_line_y), f2c(assem_scale * ASSEM_LINE_WIDTH));
	start_line_x = end_line_x = -na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x), f2c(start_line_y), f2c(end_line_x), f2c(end_line_y), f2c(assem_scale * ASSEM_LINE_WIDTH));

	start_line_x = na_x;
	end_line_x = -na_x;
	start_line_y = end_line_y = na_y;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x), f2c(start_line_y), f2c(end_line_x), f2c(end_line_y), f2c(assem_scale * ASSEM_LINE_WIDTH));
	start_line_y = end_line_y = -na_y;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x), f2c(start_line_y), f2c(end_line_x), f2c(end_line_y), f2c(assem_scale * ASSEM_LINE_WIDTH));
}

void CapacitorsChipSMD::GenerateCourtyard(FILE* footprint_file) {
	double start_line_x;
	double end_line_x;
	double start_line_y;
	double end_line_y;
	double na_x = (pad_dist + pad_length) / 2 + CRTYD_CLEARANCE;
	double na_y = r05up (MAX (pad_width / 2 + 2.5 * SILKS_LINE_WIDTH, MAX (body_width_max, pad_width) / 2 + CRTYD_CLEARANCE));


	start_line_y = end_line_y = na_y;
	start_line_x = na_x;
	end_line_x = -na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_y = end_line_y = -na_y;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_y = na_y;
	end_line_y = -na_y;
	start_line_x = end_line_x = -na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_x = end_line_x = na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
}

void CapacitorsChipSMD::GenerateCourtyardSilkScreenOutline(FILE* footprint_file) {
	double start_line_x;
	double end_line_x;
	double start_line_y;
	double end_line_y;
	double na_x = r05up (MAX ((pad_length + pad_dist) / 2 + 2.5 * SILKS_LINE_WIDTH, (pad_length + pad_dist) / 2 + CRTYD_CLEARANCE));
	double na_y = r05up (MAX (MAX (body_width_max, pad_width) / 2 + 2.5 * SILKS_LINE_WIDTH, MAX (body_width_max, pad_width) / 2 + CRTYD_CLEARANCE));

	start_line_y = end_line_y = na_y;
	start_line_x = na_x;
	end_line_x = -na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_y = end_line_y = -na_y;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_y = na_y;
	end_line_y = -na_y;
	start_line_x = end_line_x = -na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_x = end_line_x = na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
}

void CapacitorsChipSMD::GenerateCourtyardNoSilkScreen(FILE* footprint_file) {
	double start_line_x;
	double end_line_x;
	double start_line_y;
	double end_line_y;
	double na_x = (pad_dist + pad_length) / 2 + CRTYD_CLEARANCE;
	double na_y = r05up (MAX (body_width_max, pad_width) / 2 + CRTYD_CLEARANCE);

	start_line_y = end_line_y = na_y;
	start_line_x = na_x;
	end_line_x = -na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_y = end_line_y = -na_y;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_y = na_y;
	end_line_y = -na_y;
	start_line_x = end_line_x = -na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_x = end_line_x = na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
}

void CapacitorsChipSMD::GenerateSilkScreen(FILE* footprint_file) {
	double start_line_x;
	double end_line_x;
	double start_line_y;
	double end_line_y;
	double na_x = (pad_dist - pad_length) / 2;
	double na_y = MAX ((pad_width / 2 + 1.5 * SILKS_LINE_WIDTH), (body_width_max / 2 + 0.5 * SILKS_LINE_WIDTH));

	start_line_y = end_line_y = na_y;
	start_line_x = na_x;
	end_line_x = -na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x), f2c(start_line_y), f2c(end_line_x), f2c(end_line_y), f2c(SILKS_LINE_WIDTH));
	start_line_y = end_line_y = -na_y;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x), f2c(start_line_y ), f2c(end_line_x), f2c(end_line_y), f2c(SILKS_LINE_WIDTH));
}

void CapacitorsChipSMD::GenerateSilkScreenOutline(FILE* footprint_file) {
	double start_line_x;
	double end_line_x;
	double start_line_y;
	double end_line_y;
	double na_x = (pad_dist + pad_length) / 2 + 1.5 * SILKS_LINE_WIDTH;
	double na_y = MAX (body_width_max, pad_width) / 2 + 1.5 * SILKS_LINE_WIDTH;

	start_line_y = end_line_y = na_y;
	start_line_x = na_x;
	end_line_x = -na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x), f2c(start_line_y), f2c(end_line_x), f2c(end_line_y), f2c(SILKS_LINE_WIDTH));
	start_line_y = end_line_y = -na_y;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x), f2c(start_line_y ), f2c(end_line_x), f2c(end_line_y), f2c(SILKS_LINE_WIDTH));
	start_line_y = na_y;
	end_line_y = -na_y;
	end_line_x = start_line_x = na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x), f2c(start_line_y), f2c(end_line_x), f2c(end_line_y), f2c(SILKS_LINE_WIDTH));
	end_line_x = start_line_x = -na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x), f2c(start_line_y ), f2c(end_line_x), f2c(end_line_y), f2c(SILKS_LINE_WIDTH));

}

void CapacitorsChipSMD::GenerateSilkScreenMinimal(FILE* footprint_file) {
	double start_line_x;
	double end_line_x;
	double start_line_y;
	double end_line_y;
	double na_x = (pad_dist - pad_length) / 2 - 1.5 * SILKS_LINE_WIDTH;
	double na_y = body_width_max / 2 - 0.5 * SILKS_LINE_WIDTH;

	start_line_y = end_line_y = na_y;
	start_line_x = na_x;
	end_line_x = -na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x), f2c(start_line_y), f2c(end_line_x), f2c(end_line_y), f2c(SILKS_LINE_WIDTH));
	start_line_y = end_line_y = -na_y;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x), f2c(start_line_y ), f2c(end_line_x), f2c(end_line_y), f2c(SILKS_LINE_WIDTH));
	start_line_y = na_y;
	end_line_y = -na_y;
	end_line_x = start_line_x = na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x), f2c(start_line_y), f2c(end_line_x), f2c(end_line_y), f2c(SILKS_LINE_WIDTH));
	end_line_x = start_line_x = -na_x;
	fprintf (footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x), f2c(start_line_y ), f2c(end_line_x), f2c(end_line_y), f2c(SILKS_LINE_WIDTH));
}

CapacitorsChipSMD::~CapacitorsChipSMD() {
	// TODO Auto-generated destructor stub
}

