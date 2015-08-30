/*
 * DualFlatNoLead.cpp
 *
 *  Created on: 30-06-2015
 *      Author: michal
 */

#include "DualFlatNoLead.h"

DualFlatNoLead::DualFlatNoLead() {
	x_pads_center = 0;
	x_pads_outer = 0;
	pads_count_x = 0;
	pads_count_y = 0;
	pitch = 0;
	pad_length = 0;
	pad_width = 0;
	exposed_pad_x = 0;
	exposed_pad_y = 0;
	exp_pad_subdiv_x = 0;
	exp_pad_subdiv_y = 0;
	exp_pad_paste_ratio = 0;
	body_size_max_x = 0;
	body_size_max_y = 0;
	toe_to_toe_min = 0;
	toe_to_toe_max = 0;
	terminal_length_min = 0;
	terminal_length_max = 0;
	terminal_width_min = 0;
	terminal_width_max = 0;
}


void DualFlatNoLead::ReadParameters(char table[100][100][500], int row)
{
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
		if (!strcmp(table[0][column], "Tags"))
			strcpy(tags, table[row][column]);

		if (!strcmp(table[0][column], "Distance between vertical pads rows"))
			x_pads_center = atof (table[row][column]);
		if (!strcmp(table[0][column], "Land pattern X dimension (if distance between vertical pads rows not specified)"))
			x_pads_outer = atof (table[row][column]);
		if (!strcmp(table[0][column], "Number of pads in horizontal axis"))
			pads_count_x = atoi (table[row][column]);
		if (!strcmp(table[0][column], "Number of pads in vertical axis"))
			pads_count_y = atoi (table[row][column]);
		if (!strcmp(table[0][column], "Pitch"))
			pitch = atof (table[row][column]);
		if (!strcmp(table[0][column], "Pad length"))
			pad_length = atof (table[row][column]);
		if (!strcmp(table[0][column], "Pad width"))
			pad_width = atof (table[row][column]);

		if (!strcmp(table[0][column], "Exposed pad X"))
			exposed_pad_x = atof (table[row][column]);
		if (!strcmp(table[0][column], "Exposed pad Y"))
			exposed_pad_y = atof (table[row][column]);
		if (!strcmp(table[0][column], "Exposed pad subdivision X"))
			exp_pad_subdiv_x = atoi (table[row][column]);
		if (!strcmp(table[0][column], "Exposed pad subdivision Y"))
			exp_pad_subdiv_y = atoi (table[row][column]);
		if (!strcmp(table[0][column], "Exposed pad solder paste ratio"))
			exp_pad_paste_ratio = atof (table[row][column]);

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
		if (!strcmp(table[0][column], "Body dimension X max"))
			body_size_max_x = atof (table[row][column]);
		if (!strcmp(table[0][column], "Body dimension Y max"))
			body_size_max_y = atof (table[row][column]);
	}
}


void DualFlatNoLead::CalcAssemblyScale(void)
{
	while (0.5 * assem_scale * ASSEM_TEXT_SIZE > MIN_ASSEM_TEXT_SIZE)
		if ((body_size_max_y < 5 * assem_scale * ASSEM_TEXT_SIZE) || (body_size_max_x < 1.5 * assem_scale * ASSEM_TEXT_SIZE))
			assem_scale = 0.5 * assem_scale;
		else
			break;
}


void DualFlatNoLead::CalcLandPattern(void)
{
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

	if (!pad_length || !pad_width || !(x_pads_center || x_pads_outer))
	{
		F = 0.1;
		P = 0.1;
		j_t = 0.30;
		j_h = 0;
		j_s = -0.04;

		L_tol = toe_to_toe_max - toe_to_toe_min;
		W_tol = terminal_width_max - terminal_width_min;
		S_tol = sqrt (pow(L_tol, 2) + 2 * pow((terminal_length_max - terminal_length_min), 2));
		S_mean = ((toe_to_toe_min - 2 * terminal_length_max) + (toe_to_toe_max - 2 * terminal_length_min)) / 2;
		S_max = S_mean + S_tol / 2;
		Z_max = toe_to_toe_min + sqrt (pow(L_tol, 2) + pow(F, 2) + pow(P, 2)) + 2 * j_t;
		G_min = S_max - sqrt (pow(S_tol, 2) + pow(F, 2) + pow(P, 2)) - 2 * j_h;

		pad_length = (Z_max - G_min) / 2;
		x_pads_center = G_min + pad_length;
		pad_width = terminal_width_min + sqrt (pow(W_tol, 2) + pow(F, 2) + pow(P, 2)) + 2 * j_s;
	}
	else if(pad_length && pad_width && x_pads_outer)
		x_pads_center = x_pads_outer - pad_length;	//if distance between rows of pads are not specified, calculate it using distance between ends of pads and dimension of pad
	else if(pad_length && pad_width && x_pads_center)
		{}
	else
		printf("warning: bad pads parameters ");
}


void DualFlatNoLead::PrintFootprint(FILE *footprint_file)
{
	fprintf(footprint_file, "(module %s (layer F.Cu) (tedit 54130A77)\n", footprint_name);
	fprintf(footprint_file, "  (descr \"%s\")\n", description);
	fprintf(footprint_file, "  (tags \"%s\")\n", tags);
	fprintf(footprint_file, "  (attr smd)\n");
	fprintf(footprint_file, "  (fp_text reference REF** (at 0 %s) (layer F.SilkS)\n", f2c(-body_size_max_y / 2 - SILKS_TEXT_SIZE));
	fprintf(footprint_file, "    (effects (font (size %s %s) (thickness %s)))\n  )\n", f2c(SILKS_TEXT_SIZE), f2c(SILKS_TEXT_SIZE), f2c(SILKS_LINE_WIDTH));
	fprintf(footprint_file, "  (fp_text value %s (at 0 %s) (layer F.SilkS) hide\n", footprint_name, f2c(body_size_max_y / 2 + SILKS_TEXT_SIZE));
	fprintf(footprint_file, "    (effects (font (size %s %s) (thickness %s)))\n  )\n", f2c(SILKS_TEXT_SIZE), f2c(SILKS_TEXT_SIZE), f2c(SILKS_LINE_WIDTH));
	fprintf(footprint_file, "  (fp_text user %%R (at 0 0 90) (layer F.Fab)\n");
	fprintf(footprint_file, "    (effects (font (size %s %s) (thickness %s)))\n  )\n", f2c(assem_scale * ASSEM_TEXT_SIZE), f2c(assem_scale * ASSEM_TEXT_SIZE), f2c(assem_scale * ASSEM_LINE_WIDTH));
	GenerateCourtyard (footprint_file);
	GenerateAssembly (footprint_file, assem_scale);
	GenerateSilkScreen (footprint_file);
	GeneratePads (footprint_file);
	fprintf(footprint_file, "  (model %s.3dshapes/%s.wrl\n", library_name, footprint_name);	//adds path to 3D module which, I hope, will exist in future
	fprintf(footprint_file, "    (at (xyz 0 0 0))\n");
	fprintf(footprint_file, "    (scale (xyz 1 1 1))\n");
	fprintf(footprint_file, "    (rotate (xyz 0 0 0))\n");
	fprintf(footprint_file, "  )\n");
	fprintf(footprint_file, ")\n");
}


void DualFlatNoLead::GeneratePads(FILE *footprint_file)
{
	double pad_position_x;
	double pad_position_y;
	int pad_number;
	double ep_cnt_x;
	double ep_cnt_y;
	double exposed_pad_part_1_x;
	double exposed_pad_part_size_x;
	double exposed_pad_part_1_y;
	double exposed_pad_part_size_y;

	double pitch_x = pitch;
	double pitch_y = pitch;
	double y_pads_center = 0;

	pad_position_x = x_pads_center / -2;	//first pad x position
	pad_position_y = pitch_y * (pads_count_y - 1) / -2;	//first pad y position

	for(pad_number = 1; pad_number <= pads_count_y; ++pad_number)	//generate rows of pads
	{
		fprintf(footprint_file, "  (pad %d smd %s (at %s %s%s) (size %s %s) (layers F.Cu F.Paste F.Mask))\n", pad_number, pad_shape, f2c(pad_position_x), f2c(pad_position_y), "", f2c(pad_length), f2c(pad_width));
		pad_position_y = pad_position_y + pitch_y;
	}
	pad_position_x = pitch_x * (pads_count_x - 1) / -2;
	pad_position_y = y_pads_center / 2;
	for(; pad_number <= (pads_count_y + pads_count_x); ++pad_number)
	{
		fprintf(footprint_file, "  (pad %d smd %s (at %s %s%s) (size %s %s) (layers F.Cu F.Paste F.Mask))\n", pad_number, pad_shape, f2c(pad_position_x), f2c(pad_position_y), " 90", f2c(pad_length), f2c(pad_width));
		pad_position_x = pad_position_x + pitch_x;
	}
	pad_position_x = x_pads_center / 2;
	pad_position_y = pitch_y * (pads_count_y - 1) / 2;
	for(; pad_number <= (2*pads_count_y + pads_count_x); ++pad_number)
	{
		fprintf(footprint_file, "  (pad %d smd %s (at %s %s%s) (size %s %s) (layers F.Cu F.Paste F.Mask))\n", pad_number, pad_shape, f2c(pad_position_x), f2c(pad_position_y), "", f2c(pad_length), f2c(pad_width));
		pad_position_y = pad_position_y - pitch_y;
	}
	pad_position_x = pitch_x * (pads_count_x - 1) / 2;
	pad_position_y = y_pads_center / -2;
	for(; pad_number <= (2*pads_count_y + 2*pads_count_x); ++pad_number)
	{
		fprintf(footprint_file, "  (pad %d smd %s (at %s %s%s) (size %s %s) (layers F.Cu F.Paste F.Mask))\n", pad_number, pad_shape, f2c(pad_position_x), f2c(pad_position_y), " 90", f2c(pad_length), f2c(pad_width));
		pad_position_x = pad_position_x - pitch_x;
	}

	if((exposed_pad_x != 0) && (exposed_pad_y != 0))	//if there is exposed pad, make subdivision
	{
		exposed_pad_part_size_x = exposed_pad_x / exp_pad_subdiv_x;	//size of one part of exposed pad
		exposed_pad_part_size_y = exposed_pad_y / exp_pad_subdiv_y;

		exposed_pad_part_1_x = exposed_pad_part_size_x / 2 + exposed_pad_part_size_x * exp_pad_subdiv_x / 2 - exposed_pad_part_size_x;	//calculate positon of first part of exposed pad
		exposed_pad_part_1_y = exposed_pad_part_size_y / 2 + exposed_pad_part_size_y * exp_pad_subdiv_y / 2 - exposed_pad_part_size_y;

		for(ep_cnt_x = 0; ep_cnt_x < exp_pad_subdiv_x; ++ep_cnt_x)	//generate exposed pad array
		{
			for(ep_cnt_y = 0; ep_cnt_y < exp_pad_subdiv_y; ++ep_cnt_y)
			{
				fprintf(footprint_file, "  (pad %d smd rect (at %s %s) (size %s %s) (layers F.Cu F.Paste F.Mask)\n", pad_number, f2c(exposed_pad_part_1_x - ep_cnt_x * exposed_pad_part_size_x), f2c(exposed_pad_part_1_y - ep_cnt_y * exposed_pad_part_size_y), f2c(exposed_pad_part_size_x), f2c(exposed_pad_part_size_y));
				fprintf(footprint_file, "    (solder_paste_margin_ratio %s))\n", f2c(exp_pad_paste_ratio));
			}
		}
	}
}


void DualFlatNoLead::GenerateAssembly(FILE *footprint_file, double assem_scale)
{
	double start_line_x;
	double end_line_x;
	double start_line_y;
	double end_line_y;

	start_line_x = end_line_x = body_size_max_x / 2;
	start_line_y = body_size_max_y / 2;
	end_line_y = -start_line_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x), f2c(start_line_y), f2c(end_line_x), f2c(end_line_y), f2c(assem_scale * ASSEM_LINE_WIDTH));
	start_line_x = end_line_x = -start_line_x;
	end_line_y = -body_size_max_y / 2 + r05(assem_scale * ASSEM_CHAMFER / sqrt(2));
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x), f2c(start_line_y), f2c(end_line_x), f2c(end_line_y), f2c(assem_scale * ASSEM_LINE_WIDTH));

	start_line_x = body_size_max_x / 2;
	end_line_x = -start_line_x;
	start_line_y = end_line_y = body_size_max_y / 2;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x), f2c(start_line_y), f2c(end_line_x), f2c(end_line_y), f2c(assem_scale * ASSEM_LINE_WIDTH));
	start_line_y = end_line_y = -start_line_y;
	end_line_x = -body_size_max_x / 2 + r05(assem_scale * ASSEM_CHAMFER / sqrt(2));
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x), f2c(start_line_y), f2c(end_line_x), f2c(end_line_y), f2c(assem_scale * ASSEM_LINE_WIDTH));

	start_line_x = -body_size_max_x / 2;
	end_line_x = -body_size_max_x / 2 + r05(assem_scale * ASSEM_CHAMFER / sqrt(2));
	start_line_y = -body_size_max_y / 2 + r05(assem_scale * ASSEM_CHAMFER / sqrt(2));
	end_line_y = -body_size_max_y / 2;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x), f2c(start_line_y), f2c(end_line_x), f2c(end_line_y), f2c(assem_scale * ASSEM_LINE_WIDTH));
}

void DualFlatNoLead::GenerateCourtyard(FILE *footprint_file)
{
	double start_line_x;
	double end_line_x;
	double start_line_y;
	double end_line_y;
	double na_x;
	double na_y;
	double nb_x;
	double nb_y;
	double nc_x;
	double nc_y;
	double ne_x;
	double ne_y;


	na_x = ne_x = x_pads_center / 2 + pad_length / 2 + CRTYD_CLEARANCE;
	na_y = nb_y = pitch * (pads_count_y - 1) / 2 + pad_width / 2 + CRTYD_CLEARANCE;
	nb_x = nc_x = r05up (body_size_max_x / 2 + 1.5 * SILKS_LINE_WIDTH);
	nc_y = ne_y = r05up (body_size_max_y / 2 + 1.5 * SILKS_LINE_WIDTH);

	start_line_x = end_line_x = -na_x;
	start_line_y = -ne_y;
	end_line_y = na_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_y = -na_y;
	start_line_x = end_line_x = na_x;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));

	start_line_y = end_line_y = -nc_y;
	start_line_x = nc_x;
	end_line_x = -ne_x;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
	end_line_x = -nc_x;
	start_line_y = end_line_y = nc_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
	//----
	start_line_x = na_x;
	end_line_x = nb_x;
	start_line_y = end_line_y = na_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_y = end_line_y = -na_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_x = -na_x;
	end_line_x = -nb_x;
	start_line_y = end_line_y = na_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
	//----
	start_line_x = end_line_x = nc_x;
	start_line_y = nc_y;
	end_line_y = nb_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_x = end_line_x = -nc_x;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));

	start_line_x = end_line_x = nc_x;
	start_line_y = -nc_y;
	end_line_y = -nb_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x)), f2c(r05(start_line_y)), f2c(r05(end_line_x)), f2c(r05(end_line_y)), f2c(CRTYD_LINE_WIDTH));
}


void DualFlatNoLead::GenerateSilkScreen(FILE *footprint_file)
{
	double start_line_x;
	double end_line_x;
	double start_line_y;
	double end_line_y;

	double na_y = body_size_max_y / 2 + SILKS_LINE_WIDTH / 2;
	double nb_y = pitch * (pads_count_y - 1) / 2 + pad_width / 2 + 1.5 * SILKS_LINE_WIDTH;
	double nc_x = x_pads_center / 2 + pad_length / 2;
	double nd_x = x_pads_center / 2 - pad_length / 2;

	if(na_y < nb_y)
		start_line_y = end_line_y = nb_y;
	else
		start_line_y = end_line_y = na_y;
	start_line_x = -nd_x;
	end_line_x = nd_x;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x), f2c(start_line_y), f2c(end_line_x), f2c(end_line_y), f2c(SILKS_LINE_WIDTH));
	if(na_y < nb_y)
		start_line_y = end_line_y = -nb_y;
	else
		start_line_y = end_line_y = -na_y;
	start_line_x = -nc_x;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x), f2c(start_line_y), f2c(end_line_x), f2c(end_line_y), f2c(SILKS_LINE_WIDTH));
}


DualFlatNoLead::~DualFlatNoLead() {
	// TODO Auto-generated destructor stub
}
