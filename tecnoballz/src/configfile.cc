//******************************************************************************
// Copyright (C) 1998-2005 TLK Games all rights reserved.
//------------------------------------------------------------------------------
// file         : "configfile.cpp"
// created      : 2005-01-19
// updates      : 2005-01-20
// id		: $Id: configfile.cc,v 1.3 2005/01/20 07:24:49 gurumeditation Exp $
//------------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//  
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA  02111-1307, USA.
//******************************************************************************
#include "../include/configfile.h"
#include "../include/ecran_hard.h"
#include "../include/audiomixer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//..............................................................................

//------------------------------------------------------------------------------
// create object
//------------------------------------------------------------------------------
	configfile::configfile()
{
	resetvalue();
}


//------------------------------------------------------------------------------
// destroy object
//------------------------------------------------------------------------------
	configfile::~configfile()
{
}

//------------------------------------------------------------------------------
// reset all values
//------------------------------------------------------------------------------
void	configfile::resetvalue()
{
	audiomixer::audioactif = 1;
	resolution = 2;
	is_verbose = 0;
	ecran_hard::optionfull = 0;
	hardChoice = 1;	
	vieInitial = 8;
}

//------------------------------------------------------------------------------
// display values
//------------------------------------------------------------------------------
void configfile::configinfo()
{
	fprintf(stdout, "  <config info>\n"
			"- optionfull : %i\n- audioactif: %i\n- resolution:%i\n"
		"- is_verbose: %i\n hardChoice : %i\n lang:%s\n", 
		ecran_hard::optionfull, audiomixer::audioactif, resolution, is_verbose, hardChoice);
}

//------------------------------------------------------------------------------
// check if config directory exists; if not create it and set config_dir 
//------------------------------------------------------------------------------
Sint32 configfile::tocheckdir()
{
	snprintf(config_dir, sizeof(config_dir) - 1, "%s/%s",
		(getenv( "HOME" )?getenv( "HOME" ):"."), CONFIG_DIR_NAME );
	/* test and create .lgames */
	if(!opendir(config_dir)) {
		fprintf(stderr, "couldn't find/open config directory '%s'\n", config_dir);
		fprintf(stderr, "attempting to create it... " );
		MKDIR(config_dir, S_IRWXU );
		if(!opendir(config_dir))
		{	fprintf(stderr, "failed\n");
			return 0;
		}
		else
			fprintf(stderr, "ok\n");
	} 
	return 1;
}


//------------------------------------------------------------------------------
// load config file "~/.tlkgames/powermanga.conf"
//------------------------------------------------------------------------------
void configfile::loadconfig()
{
	FILE *pfile = NULL;
	resetvalue();
	if (!tocheckdir()) return;
	sprintf(configname, "%s/%s", config_dir, CONFIG_FILE_NAME);
	pfile = fopen_data(configname, "r");
	if (pfile == NULL) return;
	lisp_stream_t stream;	
	lisp_object_t *root_obj = NULL;
	lisp_stream_init_file (&stream, pfile);
	root_obj = lisp_read (&stream);

	if (root_obj->type == LISP_TYPE_EOF || root_obj->type == LISP_TYPE_PARSE_ERROR)
	{	fprintf(stderr, "configfile::loadconfig() / conf parsing failed\n");
		return;
	}

	if (strcmp(lisp_symbol(lisp_car(root_obj)), "tecnoballz-config") != 0)
	{	fprintf(stderr, "configfile::loadconfig() / conf parsing failed\n");
		return;
	}
	LispReader reader(lisp_cdr(root_obj));
	if (!reader.read_bool("fullscreen", &ecran_hard::optionfull))
		ecran_hard::optionfull = -1;
	if (!reader.read_bool("sound", &audiomixer::audioactif))
		audiomixer::audioactif = 1;
	if (!reader.read_bool("verbose", &is_verbose))
		is_verbose = 0;
	if (!reader.read_int("resolution", &resolution))
		resolution = 2;
	if(resolution < 1 || resolution > 2) resolution = 2;
	if (!reader.read_int("lifes", &vieInitial))
		vieInitial = 8 ;
	if(vieInitial < 1 || vieInitial > 9) vieInitial = 8;
	
	if (!reader.read_int("difficulty", &hardChoice))
		hardChoice = 1 ;
	if(hardChoice < 1 || hardChoice > 4) hardChoice = 1;
	
	lisp_free(root_obj);
}

//------------------------------------------------------------------------------
// save config file "~/.tlkgames/powermanga.conf"
//------------------------------------------------------------------------------
void configfile::saveconfig()
{
	FILE * config = fopen_data(configname, "w");
	if(config)
	{
		fprintf(config, "(tecnoballz-config\n");
		fprintf(config, "\t;; the following options can be set to #t or #f:\n");
		fprintf(config, "\t(fullscreen %s)\n", ecran_hard::optionfull ? "#t" : "#f");
		fprintf(config, "\t(sound %s)\n", audiomixer::audioactif ? "#t" : "#f");
		fprintf(config, "\t(verbose %s)\n", is_verbose ? "#t" : "#f");
		fprintf(config, "\n\t;; window size 1 (low-res) or 2 (high-res):\n");
	       	fprintf(config, "\t(resolution  %d)\n", resolution);
		fprintf(config, "\n\t;; difficulty 1 (easy), 2 (hard), 3 (madness) or 4 (suicide)\n");
      		fprintf(config, "\t(difficulty   %d)\n", hardChoice);
		fprintf(config, "\n\t;; number of lifes (1 to 9)\n");
      		fprintf(config, "\t(lifes   %d)\n", vieInitial);
		fprintf(config, ")\n");

	}

}

//------------------------------------------------------------------------------
// open a file
//------------------------------------------------------------------------------
FILE * configfile::fopen_data(const char *fname, const char *fmode)
{
	FILE * fi;
	fi = fopen(fname, fmode);
	if (fi == NULL)
	{	fprintf(stderr, "configfile::fopen_data(): Warning: Unable "
			"to open the file \"%s\" ", fname);
		if (strcmp(fmode, "r") == 0)
			fprintf(stderr, "for read!!!\n");
		else if (strcmp(fmode, "w") == 0)
			fprintf(stderr, "for write!!!\n");
	}
	return(fi);
}


//------------------------------------------------------------------------------
// analyse command line paramaters
//------------------------------------------------------------------------------
Sint32 configfile::scanZeArgs(Sint32 nbArg, char **ptArg) 
{
	Sint32 _iIndex;
	for(_iIndex = 1; _iIndex < nbArg; _iIndex++)
	{	
		// arguments commence tous par un tiret
		if(*ptArg[_iIndex] != '-')
			continue;
	
		if(!strcmp(ptArg[_iIndex], "-h") ||
			!strcmp(ptArg[_iIndex], "--help"))
		{	printf("\noptions:\n");
			printf("-h, --help     print Help (this message) and exit\n");
			printf("--version      print version information and exit\n");
			printf("--full         full screen\n");
			printf("--320          game run in a 320*200 window\n");
			printf("--verbose      verbose mode\n");
			printf("--nosound      force no sound\n");
			printf("--------------------------------------------------------------\n");
			printf("keys recognized during the game:\n");
			printf("CTRL+S         enable/disable sound\n");
			printf("CTRL+D         enable/disable music\n");
			printf("CTRL+F         enable/disable sound effects\n");
			printf("CTRL+X         finish the play current\n");
			printf("CTRL+Q         return to the main menu\n");
			printf("CTRL+ESC       quit TecnoballZ\n");
			printf("P              enable/disable pause\n");
			printf("F              full screen/window mode\n");
			printf("\n");
			return 0;
		}
	
		if(!strcmp(ptArg[_iIndex], "--version"))
		{	printf(TECNOBALLZ_VERSION);
			printf("\n");
			printf("copyright (c) 1991-2005 TLK Games\n");
			printf("website: http://linux.tlk.fr\n");
			return 0;
		}

		if(!strcmp(ptArg[_iIndex], "--full"))
		{	ecran_hard::optionfull = true;
			continue;
		}

		if(!strcmp(ptArg[_iIndex], "--window"))
		{	ecran_hard::optionfull = false;
			continue;
		}
		
		if(!strcmp(ptArg[_iIndex], "--verbose") ||
			!strcmp(ptArg[_iIndex], "-v"))
		{	is_verbose = true; 
			continue;
		}
		
		if(!strcmp(ptArg[_iIndex], "--320"))
		{	resolution = 1;
			continue;
		}

		if(!strcmp(ptArg[_iIndex], "--nosync"))
		{	ecran_hard::optionsync = false;
			continue;
		}
#ifndef SOUNDISOFF		
		if(!strcmp(ptArg[_iIndex], "--sound"))
		{	audiomixer::audioactif = true; 
			continue;
		}
		if(!strcmp(ptArg[_iIndex], "--nosound"))
		{	audiomixer::audioactif = false; 
			continue;
		}
#endif
		// use 4 colors background in 640x480
		if(!strcmp(ptArg[_iIndex], "--bg4"))
		{	bg4_colors = true; 
			continue;
		}

		//start at brick or guard level
		if(!strcmp(ptArg[_iIndex], "--guard"))
		{	arg_jumper = 3; 
			continue;
		}
		if(!strcmp(ptArg[_iIndex], "--brick"))
		{	arg_jumper = 1; 
			continue;
		}
	}
	return 1;
}
