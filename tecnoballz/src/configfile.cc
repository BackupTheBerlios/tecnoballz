//******************************************************************************
// Copyright (C) 1998-2005 TLK Games all rights reserved.
//------------------------------------------------------------------------------
// file         : "configfile.cpp"
// created      : 2005-12-12
// updates      : 2005-01-08
// id		: $Id: configfile.cc,v 1.1 2005/01/19 20:38:11 gurumeditation Exp $
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
	resolution = 640;
	is_verbose = 0;
	ecran_hard::optionfull = 0;
	hardChoice = 1;	
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
	if(ecran_hard::optionfull) ecran_hard::optionfull  = -1;
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
	for(Sint32 _iIndex = 1; _iIndex < nbArg; _iIndex++)
	{	
		if(*ptArg[_iIndex] != '-')
			continue;

		//######################################################
		// help
		//######################################################
		if(!strcmp(ptArg[_iIndex], "-h") ||
			!strcmp(ptArg[_iIndex], "--help"))
		{
			fprintf(stdout, "\noptions:\n"
				"-h, --help     print Help (this message) and exit\n"
  				"--version      print version information and exit\n"
				"--320          game run in a 320*200 window (slow machine)\n");
#ifdef SDL_TLK
			fprintf(stdout,
				"--window       windowed mode (full screen by default) \n");
#endif
			fprintf(stdout,
				"-v             verbose mode\n"
				"--verbose      verbose mode (more messages)\n"
				"--nosound      force no sound\n"
				"--nosync       disable timer\n"
				"--easy         easy bonuses\n"
				"--hard         hard bonuses\n"
				"--------------------------------------------------------------\n"
				"keys recognized during the game:\n"
				"CTRL+S         enable/disable the music\n"
				"CTRL+Q         finish the play current\n"
				"CTRL+A         about Powermanga\n"
				"F10            quit Powermanga\n"
				"P              enable/disable pause\n");
#ifdef SDL_TLK
			fprintf(stdout,
				"F              switch between full screen and windowed mode\n");
#endif
return 0;
			
		}

	
		if(!strcmp(ptArg[_iIndex], "--version"))
		{	//printf(POWERMANGA_VERSION);
			printf("\n");
			printf("copyright (c) 1998-2005 TLK Games\n");
			printf("website: http://linux.tlk.fr\n");
			return 0;
		}	

		//######################################################
		//force window mode
		//######################################################
		if(!strcmp(ptArg[_iIndex], "--window"))
		{	ecran_hard::optionfull = 0;
			continue;
		}

		//######################################################
		// enable verbose mode		
		//######################################################
		if(!strcmp(ptArg[_iIndex], "-v"))
		{	is_verbose = 1;
			continue;
		}
		if(!strcmp(ptArg[_iIndex], "--verbose"))
		{	is_verbose = 1;
			continue;
		}

		//######################################################
		// disable sound
		//######################################################
		if(!strcmp(ptArg[_iIndex], "--nosound"))
		{	audiomixer::audioactif = 0 ;
			continue;
		}
	}
	return 1;
}

