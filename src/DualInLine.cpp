/*
 * DualInLine.cpp
 *
 *  Created on: 30-06-2015
 *      Author: michal
 */

#include "DualInLine.h"


DualInLine::DualInLine() {
	pads_rows_spacing = 0;
	pads_count = 0;
	pitch = 0;
	pad_length = 0;
	pad_width = 0;
	pad_drill = 0;
	body_width_max = 0;
	body_length_max = 0;

	tht_offs_x = 0;
	tht_offs_y = 0;
}


void DualInLine::ReadParameters(char table[100][100][500], int row)
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

		if (!strcmp(table[0][column], "Spacing between rows"))
			pads_rows_spacing = atof (table[row][column]);
		if (!strcmp(table[0][column], "Number of pads"))
			pads_count = atoi (table[row][column]);
		if (!strcmp(table[0][column], "Pitch"))
			pitch = atof (table[row][column]);
		if (!strcmp(table[0][column], "Pad length"))
			pad_length = atof (table[row][column]);
		if (!strcmp(table[0][column], "Pad width"))
			pad_width = atof (table[row][column]);
		if (!strcmp(table[0][column], "Drill size"))
			pad_drill = atof (table[row][column]);
		if (!strcmp(table[0][column], "Pad shape"))
			strcpy(pad_shape, table[row][column]);

		if (!strcmp(table[0][column], "Body width max"))
			body_width_max = atof (table[row][column]);
		if (!strcmp(table[0][column], "Body length max"))
			body_length_max = atof (table[row][column]);
	}
}


void DualInLine::CalcAssemblyScale(void)
{
	while (0.5 * assem_scale * ASSEM_TEXT_SIZE > MIN_ASSEM_TEXT_SIZE)
		if ((body_length_max < 5 * assem_scale * ASSEM_TEXT_SIZE) || (body_width_max < 1.5 * assem_scale * ASSEM_TEXT_SIZE))
			assem_scale = 0.5 * assem_scale;
		else
			break;
}


void DualInLine::PrintFootprint(FILE *footprint_file)
{
	fprintf(footprint_file, "(module %s (layer F.Cu) (tedit 54130A77)\n", footprint_name);
	fprintf(footprint_file, "  (descr \"%s\")\n", description);
	fprintf(footprint_file, "  (tags \"%s\")\n", tags);
	fprintf(footprint_file, "  (attr smd)\n");
	fprintf(footprint_file, "  (fp_text reference REF** (at %s %s) (layer F.SilkS)\n", f2c(tht_offs_x), f2c(-body_length_max / 2 - SILKS_TEXT_SIZE + tht_offs_y));
	fprintf(footprint_file, "    (effects (font (size %s %s) (thickness %s)))\n  )\n", f2c(SILKS_TEXT_SIZE), f2c(SILKS_TEXT_SIZE), f2c(SILKS_LINE_WIDTH));
	fprintf(footprint_file, "  (fp_text value %s (at %s %s) (layer F.SilkS) hide\n", footprint_name, f2c(tht_offs_x), f2c(body_length_max / 2 + SILKS_TEXT_SIZE + tht_offs_y));
	fprintf(footprint_file, "    (effects (font (size %s %s) (thickness %s)))\n  )\n", f2c(SILKS_TEXT_SIZE), f2c(SILKS_TEXT_SIZE), f2c(SILKS_LINE_WIDTH));
	fprintf(footprint_file, "  (fp_text user %%R (at %s %s 90) (layer F.Fab)\n", f2c(tht_offs_x), f2c(tht_offs_y));
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


void DualInLine::GeneratePads(FILE *footprint_file)
{
	double pad_position_x;
	double pad_position_y;
	int pad_number;

	pad_position_x = pads_rows_spacing / -2;	//first pad x position
	pad_position_y = pitch * (pads_count / 2 - 1) / -2;	//first pad y position

	for(pad_number = 1; pad_number <= pads_count / 2; ++pad_number)	//generate rows of pads
	{
		if(pad_number == 1)
			fprintf (footprint_file, "  (pad %d thru_hole %s (at %s %s%s) (size %s %s) (drill %s) (layers *.Cu *.Mask F.SilkS))\n", pad_number, !strcmp(pad_shape, "oval") ? "rect" : "oval", f2c(pad_position_x + tht_offs_x), f2c(pad_position_y + tht_offs_y), "", f2c(pad_length), f2c(pad_width), f2c(pad_drill));
		else
			fprintf (footprint_file, "  (pad %d thru_hole %s (at %s %s%s) (size %s %s) (drill %s) (layers *.Cu *.Mask F.SilkS))\n", pad_number, pad_shape, f2c(pad_position_x + tht_offs_x), f2c(pad_position_y + tht_offs_y), "", f2c(pad_length), f2c(pad_width), f2c(pad_drill));
		pad_position_y = pad_position_y + pitch;
	}
	pad_position_x = pads_rows_spacing / 2;
	pad_position_y = pitch * (pads_count / 2 - 1) / 2;
	for(; pad_number <= (2*pads_count / 2); ++pad_number)
	{
		fprintf (footprint_file, "  (pad %d thru_hole %s (at %s %s%s) (size %s %s) (drill %s) (layers *.Cu *.Mask F.SilkS))\n", pad_number, pad_shape, f2c(pad_position_x + tht_offs_x), f2c(pad_position_y + tht_offs_y), "", f2c(pad_length), f2c(pad_width), f2c(pad_drill));
		pad_position_y = pad_position_y - pitch;
	}
}


void DualInLine::GenerateAssembly(FILE *footprint_file, double assem_scale)
{
	double start_line_x;
	double end_line_x;
	double start_line_y;
	double end_line_y;

	start_line_x = end_line_x = body_width_max / 2;
	start_line_y = body_length_max / 2;
	end_line_y = -start_line_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(assem_scale * ASSEM_LINE_WIDTH));
	start_line_x = end_line_x = -start_line_x;
	end_line_y = -body_length_max / 2 + r05(assem_scale * ASSEM_CHAMFER / sqrt(2));
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(assem_scale * ASSEM_LINE_WIDTH));

	start_line_x = body_width_max / 2;
	end_line_x = -start_line_x;
	start_line_y = end_line_y = body_length_max / 2;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(assem_scale * ASSEM_LINE_WIDTH));
	start_line_y = end_line_y = -start_line_y;
	end_line_x = -body_width_max / 2 + r05(assem_scale * ASSEM_CHAMFER / sqrt(2));
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(assem_scale * ASSEM_LINE_WIDTH));

	start_line_x = -body_width_max / 2;
	end_line_x = -body_width_max / 2 + r05(assem_scale * ASSEM_CHAMFER / sqrt(2));
	start_line_y = -body_length_max / 2 + r05(assem_scale * ASSEM_CHAMFER / sqrt(2));
	end_line_y = -body_length_max / 2;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(assem_scale * ASSEM_LINE_WIDTH));
}


void DualInLine::GenerateCourtyard(FILE *footprint_file)
{
	double start_line_x;
	double end_line_x;
	double start_line_y;
	double end_line_y;
	double na_x;
	double na_y;
	double na1_x;
	double na1_y;
	double nb1_x;
	double nb1_y;
	double nb_x;
	double nb_y;
	double nc_x;
	double nc_y;

	na_x = na1_x = pads_rows_spacing / 2 + pad_length / 2 + CRTYD_CLEARANCE;
	na1_y = nb1_y = r05up (pitch * (pads_count / 2 - 1) / 2 + pad_width / 2 + MAX (2.5 * SILKS_LINE_WIDTH, CRTYD_CLEARANCE));
	na_y = nb_y = pitch * (pads_count / 2 - 1) / 2 + pad_width / 2 + CRTYD_CLEARANCE;
	nb_x = nb1_x = nc_x = r05up (body_width_max / 2 + 1.5 * SILKS_LINE_WIDTH);
	nc_y = r05up (body_length_max / 2 + 1.5 * SILKS_LINE_WIDTH);

	if (nc_y <= na1_y)	//if pads exceeds body courtyard is based on pads (the same condition as for choice of silkscreen is used)
		na_y = nb_y = nc_y = na1_y;

	start_line_x = end_line_x = -na1_x;
	start_line_y = -na1_y;
	end_line_y = na_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x + tht_offs_x)), f2c(r05(start_line_y + tht_offs_y)), f2c(r05(end_line_x + tht_offs_x)), f2c(r05(end_line_y + tht_offs_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_x = end_line_x = na_x;
	start_line_y = -na_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x + tht_offs_x)), f2c(r05(start_line_y + tht_offs_y)), f2c(r05(end_line_x + tht_offs_x)), f2c(r05(end_line_y + tht_offs_y)), f2c(CRTYD_LINE_WIDTH));

	start_line_y = end_line_y = -nc_y;
	start_line_x = nc_x;
	end_line_x = -nc_x;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x + tht_offs_x)), f2c(r05(start_line_y + tht_offs_y)), f2c(r05(end_line_x + tht_offs_x)), f2c(r05(end_line_y + tht_offs_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_y = end_line_y = nc_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x + tht_offs_x)), f2c(r05(start_line_y + tht_offs_y)), f2c(r05(end_line_x + tht_offs_x)), f2c(r05(end_line_y + tht_offs_y)), f2c(CRTYD_LINE_WIDTH));
	//----
	start_line_x = na_x;
	end_line_x = nb_x;
	start_line_y = end_line_y = na_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x + tht_offs_x)), f2c(r05(start_line_y + tht_offs_y)), f2c(r05(end_line_x + tht_offs_x)), f2c(r05(end_line_y + tht_offs_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_y = end_line_y = -na_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x + tht_offs_x)), f2c(r05(start_line_y + tht_offs_y)), f2c(r05(end_line_x + tht_offs_x)), f2c(r05(end_line_y + tht_offs_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_x = -na_x;
	end_line_x = -nb_x;
	start_line_y = end_line_y = na_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x + tht_offs_x)), f2c(r05(start_line_y + tht_offs_y)), f2c(r05(end_line_x + tht_offs_x)), f2c(r05(end_line_y + tht_offs_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_y = end_line_y = -na1_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x + tht_offs_x)), f2c(r05(start_line_y + tht_offs_y)), f2c(r05(end_line_x + tht_offs_x)), f2c(r05(end_line_y + tht_offs_y)), f2c(CRTYD_LINE_WIDTH));
	//----
	start_line_x = end_line_x = nc_x;
	start_line_y = nc_y;
	end_line_y = nb_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x + tht_offs_x)), f2c(r05(start_line_y + tht_offs_y)), f2c(r05(end_line_x + tht_offs_x)), f2c(r05(end_line_y + tht_offs_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_x = end_line_x = -nc_x;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x + tht_offs_x)), f2c(r05(start_line_y + tht_offs_y)), f2c(r05(end_line_x + tht_offs_x)), f2c(r05(end_line_y + tht_offs_y)), f2c(CRTYD_LINE_WIDTH));

	start_line_x = end_line_x = nc_x;
	start_line_y = -nc_y;
	end_line_y = -nb_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x + tht_offs_x)), f2c(r05(start_line_y + tht_offs_y)), f2c(r05(end_line_x + tht_offs_x)), f2c(r05(end_line_y + tht_offs_y)), f2c(CRTYD_LINE_WIDTH));
	end_line_y = -nb1_y;
	start_line_x = end_line_x = -nc_x;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x + tht_offs_x)), f2c(r05(start_line_y + tht_offs_y)), f2c(r05(end_line_x + tht_offs_x)), f2c(r05(end_line_y + tht_offs_y)), f2c(CRTYD_LINE_WIDTH));
}

void DualInLine::GenerateSilkScreen(FILE *footprint_file)
{
	double start_line_x;
	double end_line_x;
	double start_line_y;
	double end_line_y;

	double na_x = body_width_max / 2 + SILKS_LINE_WIDTH / 2;
	double na_y = body_length_max / 2 + SILKS_LINE_WIDTH / 2;
	double nb_y = pitch * (pads_count / 2 - 1) / 2 + pad_width / 2 + 1.5 * SILKS_LINE_WIDTH;
	double nc_x = pads_rows_spacing / 2 + pad_length / 2;

	start_line_y = end_line_y = na_y;
	if(start_line_y <= nb_y)	//choose bigger Y position for silkscreen line (based on pads or body size)
	{
		start_line_y = end_line_y = nb_y;
		start_line_x = -nc_x;
		end_line_x = nc_x;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_y = end_line_y = -nb_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
	}
	else
	{
		start_line_x = end_line_x = -na_x;
		start_line_y = -na_y;
		end_line_y = -nb_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_x = end_line_x = na_x;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_y = na_y;
		end_line_y = nb_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_x = end_line_x = -na_x;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));

		start_line_y = end_line_y = -na_y;
		start_line_x = -na_x;
		end_line_x = na_x;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_y = end_line_y = na_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));

		start_line_y = end_line_y = -nb_y;
		start_line_x = -na_x;
		end_line_x = -nc_x;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
	}
}



void DualInLine::CalcOffset(void)
{
	tht_offs_x = pads_rows_spacing / 2;
	tht_offs_y = pitch * (pads_count / 2 - 1) / 2;
}

DualInLine::~DualInLine() {
	// TODO Auto-generated destructor stub
}

