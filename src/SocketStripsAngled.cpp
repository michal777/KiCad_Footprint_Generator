/*
 * SocketStripsAngled.cpp
 *
 *  Created on: 25-08-2015
 *      Author: michal
 */

#include "SocketStripsAngled.h"

SocketStripsAngled::SocketStripsAngled() {
	pads_rows_spacing = 0;
	pads_per_row = 0;
	rows_count = 0;
	pitch = 0;
	pad_length = 0;
	pad_width = 0;
	pad_drill = 0;
	pin_thickness = 0;
	body_height_max = 0;
	body_width_max = 0;
	body_length_max = 0;
	overall_length = 0;

	tht_offs_x = 0;
	tht_offs_y = 0;
}

void SocketStripsAngled::ReadParameters(char table[100][100][500], int row) {
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
		if (!strcmp(table[0][column], "Number of pads in row"))
			pads_per_row = atoi (table[row][column]);
		if (!strcmp(table[0][column], "Number of rows"))
			rows_count = atoi (table[row][column]);
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

		if (!strcmp(table[0][column], "Pin thickness"))
			pin_thickness = atof (table[row][column]);
		if (!strcmp(table[0][column], "Body height"))
			body_height_max = atof (table[row][column]);
		if (!strcmp(table[0][column], "Body width max"))
			body_width_max = atof (table[row][column]);
		if (!strcmp(table[0][column], "Body length max"))
			body_length_max = atof (table[row][column]);
		if (!strcmp(table[0][column], "Body height max"))
			body_height_max = atof (table[row][column]);
		if (!strcmp(table[0][column], "Overall length"))
			overall_length = atof (table[row][column]);
	}
}

void SocketStripsAngled::CalcAssemblyScale(void) {
	while (0.5 * assem_scale * ASSEM_TEXT_SIZE > MIN_ASSEM_TEXT_SIZE)
		if ((body_length_max < 5 * assem_scale * ASSEM_TEXT_SIZE) || (body_width_max < 1.5 * assem_scale * ASSEM_TEXT_SIZE))
			assem_scale = 0.5 * assem_scale;
		else
			break;
}

void SocketStripsAngled::GeneratePads(FILE* footprint_file) {
	double pad_position_x;
	double pad_position_y;
	int pad_number;

	pad_position_y = pitch * (pads_per_row - 1) / -2;	//first pad y position
	for(pad_number = 1; pad_number <= rows_count * pads_per_row; ++pad_number)
	{
		pad_position_x = (rows_count - 1) * pads_rows_spacing / 2;	//first row x position
		for(; pad_number % rows_count; ++pad_number)	//generate rows of pads
		{
			if(pad_number == 1)
				fprintf (footprint_file, "  (pad %d thru_hole %s (at %s %s%s) (size %s %s) (drill %s) (layers *.Cu *.Mask F.SilkS))\n", 1, !strcmp(pad_shape, "oval") ? "rect" : "oval", f2c(pad_position_x + tht_offs_x), f2c(pad_position_y + tht_offs_y), "", f2c(pad_length), f2c(pad_width), f2c(pad_drill));
			else
				fprintf (footprint_file, "  (pad %d thru_hole %s (at %s %s%s) (size %s %s) (drill %s) (layers *.Cu *.Mask F.SilkS))\n", pad_number, pad_shape, f2c(pad_position_x + tht_offs_x), f2c(pad_position_y + tht_offs_y), "", f2c(pad_length), f2c(pad_width), f2c(pad_drill));
			pad_position_x = pad_position_x - pitch;
		}
		if(pad_number == 1)
			fprintf (footprint_file, "  (pad %d thru_hole %s (at %s %s%s) (size %s %s) (drill %s) (layers *.Cu *.Mask F.SilkS))\n", 1, !strcmp(pad_shape, "oval") ? "rect" : "oval", f2c(pad_position_x + tht_offs_x), f2c(pad_position_y + tht_offs_y), "", f2c(pad_length), f2c(pad_width), f2c(pad_drill));
		else
			fprintf (footprint_file, "  (pad %d thru_hole %s (at %s %s%s) (size %s %s) (drill %s) (layers *.Cu *.Mask F.SilkS))\n", pad_number, pad_shape, f2c(pad_position_x + tht_offs_x), f2c(pad_position_y + tht_offs_y), "", f2c(pad_length), f2c(pad_width), f2c(pad_drill));
		pad_position_y = pad_position_y + pitch;
	}
}

void SocketStripsAngled::PrintFootprint(FILE* footprint_file) {
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

void SocketStripsAngled::GenerateAssembly(FILE* footprint_file, double assem_scale) {
	double start_line_x;
	double end_line_x;
	double start_line_y;
	double end_line_y;
	double na_x = body_width_max / 2;
	double na_y = body_length_max / 2;
	double nb_x = body_width_max / 2 - assem_scale * ASSEM_CHAMFER / sqrt(2);
	double nc_y = body_length_max / 2 - assem_scale * ASSEM_CHAMFER / sqrt(2);
	double nd_x = overall_length - pin_thickness / 2 - pitch * (rows_count - 1) / 2;

	start_line_x = end_line_x = -nd_x;
	start_line_y = na_y;
	end_line_y = -na_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(assem_scale * ASSEM_LINE_WIDTH));
	start_line_x = end_line_x = na_x;
	end_line_y = -nc_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(assem_scale * ASSEM_LINE_WIDTH));

	start_line_x = -nd_x;
	end_line_x = na_x;
	start_line_y = end_line_y = na_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(assem_scale * ASSEM_LINE_WIDTH));
	start_line_y = end_line_y = -na_y;
	end_line_x = nb_x;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(assem_scale * ASSEM_LINE_WIDTH));

	start_line_x = na_x;
	end_line_x = nb_x;
	start_line_y = -nc_y;
	end_line_y = -na_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.Fab) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(assem_scale * ASSEM_LINE_WIDTH));
}

void SocketStripsAngled::GenerateCourtyard(FILE* footprint_file) {
	double start_line_x;
	double end_line_x;
	double start_line_y;
	double end_line_y;
	double na_x;
	double na_y;
	double nb_x;

	na_x = (body_width_max / 2 + CRTYD_CLEARANCE_2);
	na_y = (body_length_max / 2 + CRTYD_CLEARANCE_2);
	nb_x = overall_length - pin_thickness / 2 - pitch * (rows_count - 1) / 2 + CRTYD_CLEARANCE_2;

	start_line_x = end_line_x = na_x;
	start_line_y = na_y;
	end_line_y = -na_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x + tht_offs_x)), f2c(r05(start_line_y + tht_offs_y)), f2c(r05(end_line_x + tht_offs_x)), f2c(r05(end_line_y + tht_offs_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_x = end_line_x = -nb_x;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x + tht_offs_x)), f2c(r05(start_line_y + tht_offs_y)), f2c(r05(end_line_x + tht_offs_x)), f2c(r05(end_line_y + tht_offs_y)), f2c(CRTYD_LINE_WIDTH));

	start_line_y = end_line_y = na_y;
	start_line_x = -nb_x;
	end_line_x = na_x;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x + tht_offs_x)), f2c(r05(start_line_y + tht_offs_y)), f2c(r05(end_line_x + tht_offs_x)), f2c(r05(end_line_y + tht_offs_y)), f2c(CRTYD_LINE_WIDTH));
	start_line_y = end_line_y = -na_y;
	fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.CrtYd) (width %s))\n", f2c(r05(start_line_x + tht_offs_x)), f2c(r05(start_line_y + tht_offs_y)), f2c(r05(end_line_x + tht_offs_x)), f2c(r05(end_line_y + tht_offs_y)), f2c(CRTYD_LINE_WIDTH));
}

void SocketStripsAngled::GenerateSilkScreen(FILE* footprint_file) {
	double start_line_x;
	double end_line_x;
	double start_line_y;
	double end_line_y;
	double base_y;

	double nd_x = overall_length - pin_thickness / 2 - pitch * (rows_count - 1) / 2;
	double na_y;
	double nb_y;
	double nd_y;
	double ne_y;
	double nf_x = overall_length - pin_thickness / 2 - pitch * (rows_count - 1) / 2 - body_height_max;
	double ng_x = nf_x;
	double ng_y;
	double nh_y;
	double ni_x = pad_length / 2 + pitch * (rows_count - 1) / 2 + 1.5 * SILKS_LINE_WIDTH;

	double pad_position_y;
	int pad_number;
	int row_number;

	pad_position_y = pitch * (pads_per_row - 1) / -2;	//first pad y position
	for(pad_number = 1; pad_number <= pads_per_row; ++pad_number)
	{
		base_y = (pad_number - 1) * pitch + (pads_per_row - 1) * pitch / -2;

		na_y = base_y + pin_thickness / 2;
		nb_y = base_y - pin_thickness / 2;

		nd_y = base_y + pitch / 2;
		ne_y = base_y - pitch / 2;

		start_line_x = end_line_x = -nd_x;
		start_line_y = -nd_y;
		end_line_y = -ne_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_x = end_line_x = -nf_x;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_x = -nd_x;
		end_line_x = -nf_x;
		start_line_y = end_line_y = -nd_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_y = end_line_y = -ne_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));

		ng_y = na_y;
		nh_y = nb_y;

		start_line_x = -ng_x;
		end_line_x = -ni_x;
		start_line_y = end_line_y = -ng_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_y = end_line_y = -nh_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));

		for (row_number = 1; row_number < rows_count; ++row_number)
		{
			start_line_x = pitch * (rows_count - 1) / 2 - pad_length / 2 - 1.5 * SILKS_LINE_WIDTH - pitch * (row_number - 1);
			end_line_x = pitch * (rows_count - 1) / 2 - pitch + pad_length / 2 + 1.5 * SILKS_LINE_WIDTH - pitch * (row_number - 1);
			start_line_y = end_line_y = -na_y;
			fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
			start_line_y = end_line_y = -nb_y;
			fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		}

		pad_position_y = pad_position_y + pitch;
	}





	/*	if(rows_count == 1 && pads_per_row == 1)
	{
		start_line_x = end_line_x = -na_x;
		start_line_y = -na_y;
		end_line_y = na_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_x = end_line_x = na_x;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));

		start_line_y = end_line_y = -na_y;
		start_line_x = -na_x;
		end_line_x = na_x;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_y = end_line_y = na_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));

	}
	else if(rows_count == 1)
	{
		start_line_x = end_line_x = -na_x;
		start_line_y = -nc_y;
		end_line_y = na_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_x = end_line_x = na_x;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));

		start_line_x = -na_x;
		end_line_x = na_x;
		start_line_y = end_line_y = -nc_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_y = end_line_y = na_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));

		start_line_x = -na1_x;
		end_line_x = na1_x;
		start_line_y = end_line_y = -na1_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_x = end_line_x = -na1_x;
		start_line_y = -na1_y;
		end_line_y = -ne_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_x = end_line_x = na1_x;
		start_line_y = -na1_y;
		end_line_y = -ne_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
	}
	else
	{
		start_line_x = end_line_x = -na_x;
		start_line_y = -nc_y;
		end_line_y = na_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_x = end_line_x = na_x;
		start_line_y = -na_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));

		start_line_y = end_line_y = -na_y;
		start_line_x = -nb_x;
		end_line_x = na_x;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_x = -na_x;
		start_line_y = end_line_y = na_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));

		start_line_y = end_line_y = -na1_y;
		start_line_x = -na1_x;
		end_line_x = -nd_x;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
		start_line_x = end_line_x = -na1_x;
		start_line_y = -na1_y;
		end_line_y = -ne_y;
		fprintf(footprint_file, "  (fp_line (start %s %s) (end %s %s) (layer F.SilkS) (width %s))\n", f2c(start_line_x + tht_offs_x), f2c(start_line_y + tht_offs_y), f2c(end_line_x + tht_offs_x), f2c(end_line_y + tht_offs_y), f2c(SILKS_LINE_WIDTH));
	}*/
}

void SocketStripsAngled::CalcOffset(void) {
	tht_offs_x = -(rows_count - 1) * pads_rows_spacing / 2;
	tht_offs_y = pitch * (pads_per_row - 1) / 2;
}


SocketStripsAngled::~SocketStripsAngled() {
	// TODO Auto-generated destructor stub
}

