//============================================================================
// Name        : KiCad_Footprint_Generator.cpp
// Author      : Michal Stec
// Version     : 2.0
// Copyright   : copyright Michal Stec 2015
// Description : Footprints generator for KiCad
//============================================================================

using namespace std;
#include <sys/stat.h>

#include "SmallOutlinePackage.h"
#include "QuadFlatPackage.h"
#include "QuadFlatNoLead.h"
#include "DualFlatNoLead.h"
#include "DualInLine.h"
#include "CapacitorsElectrolyticSMD.h"
#include "CapacitorsPolarizedSMD.h"
#include "CapacitorsChipSMD.h"
#include "ResistorsChipSMD.h"
#include "PinHeadersStraight.h"
#include "PinHeadersAngled.h"
#include "SocketStripsStraight.h"
#include "SocketStripsAngled.h"


#define SILKS_LINE_WIDTH 0.15
#define CRTYD_LINE_WIDTH 0.05
#define ASSEM_LINE_WIDTH 0.1
#define SILKS_TEXT_SIZE 1
#define ASSEM_TEXT_SIZE 1
#define MIN_ASSEM_TEXT_SIZE 0.127
#define CRTYD_CLEARANCE 0.25

#define S(i) ((i >= 0) ? i : 0)
#if defined(__MINGW32__)
#define MKDIR(filename) (mkdir (filename));
#else
#define MKDIR(filename) (mkdir (file_name, 0751));
#endif

int main(int argc, char *argv[])
{
	FILE *param_file;
	FILE *footprint_file;
	char file_name[MAX_TEXT_LENGTH];
	int i, j, k, row, column;
	int open, q_mark_nr, ctdown, exit_flag;
	char table[100][100][500];
	char buffer[10000];


	if ((param_file = fopen(argv[1], "rt")) == NULL)	//opening file with parameters
	{
		printf ("can't open parameters\n");
		exit (1);
	}


#if defined(__MINGW32__)
	mkdir ("./footprints");
#else
	mkdir ("./footprints", 0751);
#endif


	for(i = 0; i < 100; ++i)
		for(j = 0; j < 100; ++j)
			for(k = 0; k < 500; ++k)
				table[i][j][k] = '\0';



	for (row = 0, fgets (buffer, 10000, param_file); !feof (param_file); ++row, fgets (buffer, 10000, param_file))
	{
		column = 0;
		i = 0;	//index of buffer loaded with line
		j = 0;	//index of cell in table (reset with new column)
		open = 1;	//1 when getting characters inside cell, 0 between cells
		q_mark_nr = 0;	//number of "
		exit_flag = 0;
		if(buffer[i] == '"')
			ctdown = 1;	// one character " will be discarded
		else
			ctdown = 0;	// if first cell not quoted start filling table with first character

		for (i = 0; !exit_flag && (EOF != buffer[i]); ++i, ++j)
		{
			if(buffer[i] == '"')
				++q_mark_nr;	//count " (it's repeated if appears inside cell so doesn't affect parity at position of colon)


			if((buffer[i] == ',' && buffer[i + 1] != '"' && !(q_mark_nr % 2)) && open == 0)	//check parity of " at position of colon
			{
				open = 1;
				j = 0;
				ctdown = 1;	// no " after , so discard ,
			}
			else if((buffer[i] == ',' && buffer[i + 1] == '"' && !(q_mark_nr % 2)) && open == 0)
			{
				open = 1;
				j = 0;
				ctdown = 2;	// " after , so discard , and "
			}
			else if(buffer[i] == ',' && !(q_mark_nr % 2) && open == 1)	//field not quoted closed when: colon comes AND there was even number of quotation marks before colon AND was open
			{
				open = 0;
				++column;
				--i;	//stay at colon - test for open will be done in next iteration
			}
			else if(buffer[i] == '"' && buffer[i + 1] == ',' && !(q_mark_nr % 2) && open == 1)	//field quoted closed when: quotation mark comes AND next is colon AND there was even number of quotation marks before colon AND was open
			{
				open = 0;
				++column;
			}
			else if(buffer[S(i - 1)] != ',' && buffer[i] == '"' && buffer[i + 1] == '"' && buffer[i + 2] != ',' && !(q_mark_nr % 2) && i > 1)	//replace "" with \" (kicad reads \" as " but openoffice saves " as "")
			{
				buffer[i] = '\\';
			}
			else if(buffer[i] == '\n' || (buffer[i] == '"' && buffer[i + 1] == '\n'))	//exit when: end-of-line comes (now or after quotation mark)
			{
				open = 0;
				++column;
				exit_flag = 1;
			}

			if(open && (--ctdown) < 0)
				table[row][column][j] = buffer[i];
			else
				j = -1;
		}
	}

	/*remove comment to print table*/
	/*	for(i = 0; i < 100; ++i)
		for(j = 0; j < 100; ++j)
			puts(table[j][i]);*/


	for(column = 0; strcmp(table[0][column], "Package type"); )
		++column;

	for(row = 1; table[row][column]['\0'] != '\0'; ++row)
	{
		if(!strcmp(table[row][column], "sop"))
		{
			SmallOutlinePackage SOP;
			SOP.ReadParameters(table, row);
			SOP.CalcLandPattern();
			SOP.CalcAssemblyScale();

			sprintf (file_name,"./footprints/%s.pretty", SOP.GetLibraryName());
			MKDIR(file_name);
			sprintf (file_name,"./footprints/%s.pretty/%s.kicad_mod", SOP.GetLibraryName(), SOP.GetFootprintName());
			if ((footprint_file = fopen(file_name, "wt")) == NULL)	//create footprint file
			{
				printf ("can't open footprint\n");
				exit (1);
			}
			else
				printf ("created: %s\n", file_name);
			SOP.PrintFootprint(footprint_file);
			fclose(footprint_file);
		}

		else if(!strcmp(table[row][column], "qfp"))
		{
			QuadFlatPackage QFP;
			QFP.ReadParameters(table, row);
			QFP.CalcLandPattern();
			QFP.CalcAssemblyScale();

			sprintf (file_name,"./footprints/%s.pretty", QFP.GetLibraryName());
			MKDIR(file_name);
			sprintf (file_name,"./footprints/%s.pretty/%s.kicad_mod", QFP.GetLibraryName(), QFP.GetFootprintName());
			if ((footprint_file = fopen(file_name, "wt")) == NULL)	//create footprint file
			{
				printf ("can't open footprint\n");
				exit (1);
			}
			else
				printf ("created: %s\n", file_name);
			QFP.PrintFootprint(footprint_file);
			fclose(footprint_file);
		}

		else if(!strcmp(table[row][column], "dfn"))
		{
			DualFlatNoLead DFN;
			DFN.ReadParameters(table, row);
			DFN.CalcLandPattern();
			DFN.CalcAssemblyScale();

			sprintf (file_name,"./footprints/%s.pretty", DFN.GetLibraryName());
			MKDIR(file_name);
			sprintf (file_name,"./footprints/%s.pretty/%s.kicad_mod", DFN.GetLibraryName(), DFN.GetFootprintName());
			if ((footprint_file = fopen(file_name, "wt")) == NULL)	//create footprint file
			{
				printf ("can't open footprint\n");
				exit (1);
			}
			else
				printf ("created: %s\n", file_name);
			DFN.PrintFootprint(footprint_file);
			fclose(footprint_file);
		}

		else if(!strcmp(table[row][column], "qfn"))
		{
			QuadFlatNoLead QFN;
			QFN.ReadParameters(table, row);
			QFN.CalcLandPattern();
			QFN.CalcAssemblyScale();

			sprintf (file_name,"./footprints/%s.pretty", QFN.GetLibraryName());
			MKDIR(file_name);
			sprintf (file_name,"./footprints/%s.pretty/%s.kicad_mod", QFN.GetLibraryName(), QFN.GetFootprintName());
			if ((footprint_file = fopen(file_name, "wt")) == NULL)	//create footprint file
			{
				printf ("can't open footprint\n");
				exit (1);
			}
			else
				printf ("created: %s\n", file_name);
			QFN.PrintFootprint(footprint_file);
			fclose(footprint_file);
		}

		else if(!strcmp(table[row][column], "DIL"))
		{
			DualInLine DIL;
			DIL.ReadParameters(table, row);
			DIL.CalcOffset();
			DIL.CalcAssemblyScale();

			sprintf (file_name,"./footprints/%s.pretty", DIL.GetLibraryName());
			MKDIR(file_name);
			sprintf (file_name,"./footprints/%s.pretty/%s.kicad_mod", DIL.GetLibraryName(), DIL.GetFootprintName());
			if ((footprint_file = fopen(file_name, "wt")) == NULL)	//create footprint file
			{
				printf ("can't open footprint\n");
				exit (1);
			}
			else
				printf ("created: %s\n", file_name);
			DIL.PrintFootprint(footprint_file);
			fclose(footprint_file);
		}

		else if(!strcmp(table[row][column], "Electrolytic_Capacitor_SMD"))
		{
			CapacitorsElectrolyticSMD CElectSMD;
			CElectSMD.ReadParameters(table, row);
			CElectSMD.CalcLandPattern();
			CElectSMD.CalcAssemblyScale();

			sprintf (file_name,"./footprints/%s.pretty", CElectSMD.GetLibraryName());
			MKDIR(file_name);
			sprintf (file_name,"./footprints/%s.pretty/%s.kicad_mod", CElectSMD.GetLibraryName(), CElectSMD.GetFootprintName());
			if ((footprint_file = fopen(file_name, "wt")) == NULL)	//create footprint file
			{
				printf ("can't open footprint\n");
				exit (1);
			}
			else
				printf ("created: %s\n", file_name);
			CElectSMD.PrintFootprint(footprint_file);
			fclose(footprint_file);
		}

		else if(!strcmp(table[row][column], "Chip_Polar_Capacitor"))
		{
			CapacitorsPolarizedSMD CPolarSMD;
			CPolarSMD.ReadParameters(table, row);
			CPolarSMD.CalcLandPattern();
			CPolarSMD.CalcAssemblyScale();

			sprintf (file_name,"./footprints/%s.pretty", CPolarSMD.GetLibraryName());
			MKDIR(file_name);
			sprintf (file_name,"./footprints/%s.pretty/%s.kicad_mod", CPolarSMD.GetLibraryName(), CPolarSMD.GetFootprintName());
			if ((footprint_file = fopen(file_name, "wt")) == NULL)	//create footprint file
			{
				printf ("can't open footprint\n");
				exit (1);
			}
			else
				printf ("created: %s\n", file_name);
			CPolarSMD.PrintFootprint(footprint_file);
			fclose(footprint_file);
		}

		else if(!strcmp(table[row][column], "Chip_Capacitor"))
		{
			CapacitorsChipSMD C_SMD;
			C_SMD.ReadParameters(table, row);
			C_SMD.CalcLandPattern();
			C_SMD.CalcAssemblyScale();

			sprintf (file_name,"./footprints/%s.pretty", C_SMD.GetLibraryName());
			MKDIR(file_name);
			sprintf (file_name,"./footprints/%s.pretty/%s.kicad_mod", C_SMD.GetLibraryName(), C_SMD.GetFootprintName());
			if ((footprint_file = fopen(file_name, "wt")) == NULL)	//create footprint file
			{
				printf ("can't open footprint\n");
				exit (1);
			}
			else
				printf ("created: %s\n", file_name);
			C_SMD.PrintFootprint(footprint_file);
			fclose(footprint_file);
		}

		else if(!strcmp(table[row][column], "Chip_Resistor"))
		{
			ResistorsChipSMD R_SMD;
			R_SMD.ReadParameters(table, row);
			R_SMD.CalcLandPattern();
			R_SMD.CalcAssemblyScale();

			sprintf (file_name,"./footprints/%s.pretty", R_SMD.GetLibraryName());
			MKDIR(file_name);
			sprintf (file_name,"./footprints/%s.pretty/%s.kicad_mod", R_SMD.GetLibraryName(), R_SMD.GetFootprintName());
			if ((footprint_file = fopen(file_name, "wt")) == NULL)	//create footprint file
			{
				printf ("can't open footprint\n");
				exit (1);
			}
			else
				printf ("created: %s\n", file_name);
			R_SMD.PrintFootprint(footprint_file);
			fclose(footprint_file);
		}

		else if(!strcmp(table[row][column], "pin_headers_straight"))
		{
			PinHeadersStraight PIN_STR;
			PIN_STR.ReadParameters(table, row);
			PIN_STR.CalcOffset();
			PIN_STR.CalcAssemblyScale();

			sprintf (file_name,"./footprints/%s.pretty", PIN_STR.GetLibraryName());
			MKDIR(file_name);
			sprintf (file_name,"./footprints/%s.pretty/%s.kicad_mod", PIN_STR.GetLibraryName(), PIN_STR.GetFootprintName());
			if ((footprint_file = fopen(file_name, "wt")) == NULL)	//create footprint file
			{
				printf ("can't open footprint\n");
				exit (1);
			}
			else
				printf ("created: %s\n", file_name);
			PIN_STR.PrintFootprint(footprint_file);
			fclose(footprint_file);
		}

		else if(!strcmp(table[row][column], "pin_headers_angled"))
		{
			PinHeadersAngled PIN_ANG;
			PIN_ANG.ReadParameters(table, row);
			PIN_ANG.CalcOffset();
			PIN_ANG.CalcAssemblyScale();

			sprintf (file_name,"./footprints/%s.pretty", PIN_ANG.GetLibraryName());
			MKDIR(file_name);
			sprintf (file_name,"./footprints/%s.pretty/%s.kicad_mod", PIN_ANG.GetLibraryName(), PIN_ANG.GetFootprintName());
			if ((footprint_file = fopen(file_name, "wt")) == NULL)	//create footprint file
			{
				printf ("can't open footprint\n");
				exit (1);
			}
			else
				printf ("created: %s\n", file_name);
			PIN_ANG.PrintFootprint(footprint_file);
			fclose(footprint_file);
		}

		else if(!strcmp(table[row][column], "socket_strips_straight"))
		{
			SocketStripsStraight SOCK_STR;
			SOCK_STR.ReadParameters(table, row);
			SOCK_STR.CalcOffset();
			SOCK_STR.CalcAssemblyScale();

			sprintf (file_name,"./footprints/%s.pretty", SOCK_STR.GetLibraryName());
			MKDIR(file_name);
			sprintf (file_name,"./footprints/%s.pretty/%s.kicad_mod", SOCK_STR.GetLibraryName(), SOCK_STR.GetFootprintName());
			if ((footprint_file = fopen(file_name, "wt")) == NULL)	//create footprint file
			{
				printf ("can't open footprint\n");
				exit (1);
			}
			else
				printf ("created: %s\n", file_name);
			SOCK_STR.PrintFootprint(footprint_file);
			fclose(footprint_file);
		}

		else if(!strcmp(table[row][column], "socket_strips_angled"))
		{
			SocketStripsAngled SOCK_ANG;
			SOCK_ANG.ReadParameters(table, row);
			SOCK_ANG.CalcOffset();
			SOCK_ANG.CalcAssemblyScale();

			sprintf (file_name,"./footprints/%s.pretty", SOCK_ANG.GetLibraryName());
			MKDIR(file_name);
			sprintf (file_name,"./footprints/%s.pretty/%s.kicad_mod", SOCK_ANG.GetLibraryName(), SOCK_ANG.GetFootprintName());
			if ((footprint_file = fopen(file_name, "wt")) == NULL)	//create footprint file
			{
				printf ("can't open footprint\n");
				exit (1);
			}
			else
				printf ("created: %s\n", file_name);
			SOCK_ANG.PrintFootprint(footprint_file);
			fclose(footprint_file);
		}


		else
			printf("bad footprint type\n");
	}
	fclose(param_file);

	return 0;
}
