//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "ressources.cc"
// created	: 2004-04-20
// updates	: 2005-01-15
// fonctions	: manage ressources
// id		: $Id: ressources.cc,v 1.6 2005/07/21 15:03:48 sukria Exp $
//-----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA 02111-1307, USA.
//*****************************************************************************
#include "../include/ressources.h"
#include "../include/GIF_bitMap.h"
#include "../config.h"

#ifndef DATADIR
	#define DATADIR "/usr/share/games/tecnoballz"
#endif

#ifndef SCOREFILE
	#define SCOREFILE "/var/lib/games/tecnoballz.hi"
#endif

//...............................................................................

char* ressources::fnamescore = SCOREFILE;
const char* ressources::folderlist[] =
{	"/",		// Normally unused, except when running from the source directory...
	DATADIR,
	"./TecnoballZ/",
	0		// Special value meaning "$(PREFIX)/share/games/tecnoballz/"
			// Also marks end of list
};
//const char*	ressources::folderdata = "TecnoballZ/";
const char*	ressources::folder_640 = "hires/";
const char*	ressources::folder_320 = "lores/";
char 		ressources::stringtemp[512];
char 		ressources::pathstring[512];

const char* ressources::standfiles[] =
{	"cosinus128.list",	//RESCOSLIST
	"edmap01.data",		//RESEDMAP01
	"edmap02.data",		//RESEDMAP02
	"edmap03.data",		//RESEDMAP03
	"gard_lissa.list",	//RESGCURVES
	"tableau.data",		//RESBLEVELS: all bricks levels
	"min60map.bmp"		//RES60BACKG
};

const char* ressources::musicfiles[] =
{	"area1-game2.mod",
	"area2-game.mod",
	"area3-game.mod",
	"area4-game.mod",
	"area5-game.mod",
	"gardien-go.mod",
	"high-score.mod",
	"over-theme.mod",
	"tecnoballz.mod",
	"tecno-winn.mod"
};

const char* ressources::soundfiles[] =
{	"rlife_moins.wav",	// 01
	"rexplo_rak.wav",
	"rlife_plus.wav",
	"rmoney.wav",
	"ralarm.wav",		//05
	"rgadget_sound.wav",
	"rtecno.wav",
	"rappar.wav",
	"rtransfo.wav",
	"rtir_monstre.wav",	//10
	"rtir_rak.wav",
	"rexplo_big.wav",
	"rdetruit_indes.wav",
	"rexeplo_atom.wav",
	"rmontre_touche.wav",	//15
	"ratom.wav",
	"rindes_1.wav",
	"rindes_2.wav",
	"rraquette.wav",
	"rbricote.wav",		//20
	"rbrique1.wav",
	"rbrique2.wav",
	"rbrique3.wav",
	"rbrique4.wav",
	"rbrique5.wav"	,	//25
	"raspire.wav",
	"reject.wav",		//27
};

const char* ressources::graphfiles[] =
{	"ani_head.bmp",		//RESHEADANI
	"ba_score2.bmp",	//RESBASCORE
	"bumper_1.bmp",		//RESBUMPER1
	"bumper_2.bmp",		//RESBUMPER2
	"fontgame.bmp",		//RESFONTGAM
	"fontmenu.bmp",		//RESFONTMEN
	"font_messa.bmp",	//RESFONTMES
	"gigablzt.bmp",		//RESGIGABLZ
	"map_edit.bmp",		//RESMAPEDIT
	"money.bmp",		//RESMONEYFT
	"new_shop.bmp",		//RESNEWSHOP
	"pagezbob.bmp",		//RESPAGEBOB
	"zebricks.bmp",		//RESZEBRICK
	"font_score.bmp"	//RESFONTSCR
};
 
char ressources::ze_mapfile[] = "map??.bmp";
 
//------------------------------------------------------------------------------
// create the object
//------------------------------------------------------------------------------
  ressources::ressources()
{
}

//------------------------------------------------------------------------------
// release the object
//------------------------------------------------------------------------------
  ressources::~ressources()
{
	if(table_cosL)
	{	memGestion->liberation((char*)table_cosL);
		table_cosL = (Sint16 *)NULL;
	}
	freeSprite();
}

//------------------------------------------------------------------------------
// load a resource file in memoryflag
//	input	=> ident: resource ID
//	output	<=	
//------------------------------------------------------------------------------
char* ressources::getResData(Sint32 ident)
{
	char* pfile = getResFile(ident);
	if (!pfile)
		return NULL;
	return loadZeFile(pfile);
}

//------------------------------------------------------------------------------
// return valid name with full path
//------------------------------------------------------------------------------
char* ressources::getResFile(Sint32 ident)
{	
	const char* pfile;
	//strcpy(stringtemp, folderdata);
	if (ident >= 4096)
	{	ident -= 4096;
		pfile = graphfiles[ident];
		if (resolution == 1)
			//strcat(stringtemp, folder_320);
			strcpy(stringtemp, folder_320);
		else
			//strcat(stringtemp, folder_640);
			strcpy(stringtemp, folder_640);
		
		//printf("*** ressources::getResFile() = %s \n", stringtemp);
		strcat(stringtemp, pfile);
	}
	else 
	{	pfile = standfiles[ident];
		//strcat(stringtemp, pfile);
		strcpy(stringtemp, pfile);
	}
	//printf("*** ressources::getResFile(%i) = %s / pfile=%s\n", ident, stringtemp, pfile);
	//return locateFile(stringtemp);
	return stringtemp;
}

//------------------------------------------------------------------------------
// return valid name with full path
//------------------------------------------------------------------------------
char* ressources::getMusFile(Sint32 ident)
{	
	const char* pfile;
	//strcpy(stringtemp, folderdata);
	//strcat(stringtemp, "musics/");
	strcpy(stringtemp, "musics/");
	pfile = musicfiles[ident];
	strcat(stringtemp, pfile);
	//printf("ressources::getMusFile(%i) = %s\n", ident, stringtemp);
	return locateFile(stringtemp);
	//return stringtemp;
}

//------------------------------------------------------------------------------
// return valid name with full path
//------------------------------------------------------------------------------
char* ressources::getSndFile(Sint32 ident)
{	
	const char* pfile;
	//strcpy(stringtemp, folderdata);
	//strcat(stringtemp, "sounds/");
	strcpy(stringtemp, "sounds/");
	pfile = soundfiles[ident];
	strcat(stringtemp, pfile);
	//printf("ressources::getSndFile(%i) = %s\n", ident, stringtemp);
	return locateFile(stringtemp);
	//return stringtemp;
}

//------------------------------------------------------------------------------
// return valid name with full path
//------------------------------------------------------------------------------
char*	ressources::getTexFile(Sint32 nbkdg)
{	
	//strcpy(stringtemp, folderdata);
	//strcat(stringtemp, "textures/");
	strcpy(stringtemp, "textures/");
	intToASCII(nbkdg, ze_mapfile + 3, 1);
	strcat(stringtemp, ze_mapfile);
	return stringtemp;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
char* ressources::locate_res(Sint32 ident)
{	return locateFile(getResFile(ident));
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
char *ressources::locateFile(const char *const name)
{

	if(is_verbose)
		fprintf(stdout, "ressources::locateFile(%s) [START]\n", name);

	//###################################################################
	// clear path name string
	//###################################################################
	for(Sint32 i = 0; i < 256; i++)
		pathstring[i] = 0;

	//###################################################################
	// null pointer
	//###################################################################
	if(name == NULL)
		return NULL;
	
	//###################################################################
	// if absolute path, return a pointer to a duplicate string
	//###################################################################
	char *pathname;
	if(*name == '/')
	{	pathname = &pathstring[0];
		strcpy(pathname, name);
		//printf("return %s\n", pathname);
		return pathname;
	}
	for(const char **p = folderlist;; p++)
	{	
		//printf("folderlist: %s\n", *p);
		if(*p == 0)
		{	const char *subdir = "/share/games/tecnoballz/";
			pathname = &pathstring[0];
			strcpy(pathname, nomprefix);
			strcat(pathname, subdir);
			strcat(pathname, name);
		}
		else if(**p == '~')	// Not used anymore
		{	static const char bogus = '\0';
			static const char *home_dir = &bogus;
			if(home_dir == &bogus) home_dir = getenv("HOME");
			if(home_dir == 0) continue;	// $HOME not set. Skip this directory.
			pathname = &pathstring[0];
			strcpy(pathname, home_dir);
			strcat(pathname, *p + 1);
			strcat(pathname, "/");
			strcat(pathname, name);
		}
    
		// regarde deja dans le repertoire courant. (**p == '.')
		else
		{	pathname = &pathstring[0];
			strcpy(pathname, *p);
			if(pathname[strlen(pathname) - 1] != '/')
				strcat(pathname, "/");
			strcat(pathname, name);
		}
		if(is_verbose)
			fprintf(stdout, "ressources::locateFile() try %s\n", pathname);
	
#ifdef WIN32
		struct _stat s;
		if(_stat(pathname, &s) == 0 && !_S_ISDIR(s.st_mode))
		{	return pathname;
		}
#else
		struct stat s;
		if(stat(pathname, &s) == 0 && !S_ISDIR(s.st_mode)) 
		{	if(is_verbose) fprintf(stdout, "ressources::locateFile(%s) END\n", pathname);
			return pathname;
		}
#endif
		if(*p == 0) break;
	}
	fprintf(stderr, "ressources::locateFile: %s not found\n", name);
	return 0;	//not found.
}

//------------------------------------------------------------------------------
// load page of sprites
//------------------------------------------------------------------------------
Sint32 ressources::loadSprite(Sint32 ident)
{
	freeSprite();
	image_BOBs = new GIF_bitMap();
	Sint32 error = image_BOBs->decompacte(ident);
	if(error) return error;
	image_BOBs->palette_go();
	return error;
}

//------------------------------------------------------------------------------
// release page of sprites
//------------------------------------------------------------------------------
void ressources::freeSprite()
{
	if(image_BOBs)
		delete image_BOBs;
	image_BOBs = (GIF_bitMap*)NULL;
}


//------------------------------------------------------------------------------
// load a file in memory / charge un fichier en memoire
// input	=> fname: filename
//           _pAdresse     : adresse de chargemebt
//           zsize      : taille a lire
// output <= 1             : tout c'est bien passe
//           0             : erreur
//------------------------------------------------------------------------------
char *ressources::loadZeFile(char *fname)
{
	return loadZeFile(fname, &zeLastSize);
}

//------------------------------------------------------------------------------
// load a file in memory 
// input	=> fname: filename
// output	<= fsize: zone memory where the file was loaded (0 = error)
//------------------------------------------------------------------------------
char *ressources::loadZeFile(char *fname, Uint32 *fsize)
{
	//printf("ressources::loadZeFile %s\n", fname);
  	//###################################################################
	// check the path name
  	//###################################################################
	char *pname = locateFile(fname);
	if(pname == 0)
	{	fprintf(stderr,
			"ressources::loadZeFile() : can't locate file : %s\n\n", fname);
		return NULL;
	}
  
  	//###################################################################
  	// open the file
  	//###################################################################
#ifdef WIN32
	Sint32 fhand = open(pname, O_RDONLY | O_BINARY, 0);
#else
	Sint32 fhand = open(pname, O_RDONLY, 0);
#endif
	if(fhand == -1)
	{	fprintf(stderr,
			"ressources::loadZeFile() : can't open file : %s (%s)\n\n", 
			pname, strerror(errno));
		//free(pname);
		return 0;
	}

  	//###################################################################
	// read the size of the file
  	//###################################################################
	struct stat sStat;
	if(fstat(fhand, &sStat))
	{	fprintf(stderr,
			"ressources::loadZeFile() : can't stat file : %s (%s)\n\n", 
			pname, strerror(errno));
		//free(pname);
		return 0;
	}
	(*fsize) = sStat.st_size;	//save file size
  
  	//###################################################################
  	// allocate memory
  	//###################################################################
	char *ptMem = (char *)(memGestion->reserveMem(sStat.st_size,
  		0x31313131));
	num_erreur = memGestion->retour_err();
	 if(num_erreur)       
	{	fprintf(stderr, "ressources::loadZeFile() %s : out of memory\n\n",
			pname);
		//free(pname);
		return 0;
	}

  	//###################################################################
	// read the file
  	//###################################################################
	if(read(fhand, ptMem, sStat.st_size) != sStat.st_size)
	{	//menGestion->liberation(ptMem);
		fprintf(stderr, "ressources::loadZeFile() can't read file %s (%s)\n\n",
			pname, strerror(errno));
		//free(pname);
		return 0;
	}
  
	//###################################################################
	// close the file
	//###################################################################
	close(fhand);
	/*fprintf(stdout, "ressources::loadZeFile : file %s was loaded in memory\n",
		pname);*/
	//free(pname);
	return ptMem;
}

//------------------------------------------------------------------------------
// writing a line into log file
//------------------------------------------------------------------------------
void ressources::writingLog(char *pname, char* strng)
{
	//###################################################################
	// open file
	//###################################################################
	Sint32 fhand;
	#ifdef WIN32
	  _umask(0002); //set umask so that files are group-writable
	#else
	  umask(0002);
	#endif
	fhand = open(pname , O_WRONLY | O_APPEND | O_CREAT, 00666);	
	if(fhand == -1)
	{	fprintf(stderr, "ressources::writingLog() open failed %s: %s\n",
			pname, strerror(errno)) ;
		return;

	}

	//###################################################################
	// write and close
	//###################################################################
	write(fhand, strng, strlen(strng));
	if(close(fhand) == -1)
			fprintf(stderr, "ressources::writingLog() close failed %s: %s\n",
			pname, strerror(errno)) ;
}

//------------------------------------------------------------------------------
// return size last file charged in memory
//------------------------------------------------------------------------------
Uint32 ressources::gtLastSize()
{
	return zeLastSize;
}

//------------------------------------------------------------------------------
// load sinus & cosinus table (1790 bytes <=> 895 values)
// 0 to 511 cosinus / 383 to 894 sinus
//------------------------------------------------------------------------------
Sint32 ressources::load_sinus()
{
	table_cosL = (Sint16 *) getResData(ressources::RESCOSLIST);
	if(!table_cosL)
	{	num_erreur = E_FILERROR;
		return num_erreur;
	}
	else
	{	table_sinL = table_cosL + 383;
		num_erreur = E_NO_ERROR;
	}

	//###################################################################
	// convert big endian values to little endian values	
	//###################################################################
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	Sint32 fsize = gtLastSize();
	fsize = fsize / 2;	// convert bytes => words
	for(Sint32 i = 0; i < fsize; i++) 
	{	char *p = (char *)&table_cosL[i];
		char a = p[0];
		p[0] = p[1];
		p[1] = a;
	}
#endif

	return num_erreur;
}

//------------------------------------------------------------------------------
// load scores table
//------------------------------------------------------------------------------
char* ressources::loadScores()
{
	return loadZeFile(fnamescore, &zeLastSize);	
}

//------------------------------------------------------------------------------
// save scores table
//------------------------------------------------------------------------------
Sint32 ressources::saveScores(char *ptScr, Uint32 fsize)
{
#ifdef WIN32
  _umask(0002); //set umask so that files are group-writable
#else
  umask(0002);
#endif
	Sint32 fhand = open(fnamescore , O_WRONLY | O_CREAT, 00666);
	if(fhand == -1)
	{	fprintf(stderr, "ressources::saveScores(): file:%s / error:%s\n", 
					fnamescore, strerror(errno));
		return 0;
	}
#ifdef WIN32
	_write(fhand, ptScr, fsize);
#else
	write(fhand, ptScr, fsize);
#endif
	if(close(fhand) == -1)
	{	fprintf(stderr, "ressources::saveScores(): file:%s / error:%s\n", 
		fnamescore, strerror(errno));
		return 0;
	}
	else
	{	if(is_verbose)
			fprintf(stdout, "ressources::saveScores(): "
				"file:%s size:%i\n", fnamescore, fsize);
	}
	return 1;
}

//------------------------------------------------------------------------------
// cosinus and sinus table
//------------------------------------------------------------------------------
const Sint16 ressources::cosinus360[720] =
{ 0, 2, 4, 7, 9, 11, 13, 15, 18, 20, 22, 24, 26, 29, 31, 33, 35, 37, 39, 41, 43,
  46, 48, 50, 52, 54, 56, 58, 60, 62, 63, 65, 67, 69, 71, 73, 75, 76, 78, 80,
  82, 83, 85, 87, 88, 90, 91, 93, 94, 96, 97, 99, 100, 101, 103, 104, 105, 107,
  108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 119, 120, 121,
  121, 122, 123, 123, 124, 124, 125, 125, 125, 126, 126, 126, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127, 126, 126, 126, 125, 125, 125, 124,
  124, 123, 123, 122, 121, 121, 120, 119, 119, 118, 117, 116, 115, 114, 113,
  112, 111, 110, 109, 108, 107, 105, 104, 103, 101, 100, 99, 97, 96, 94, 93, 91,
  90, 88, 87, 85, 83, 82, 80, 78, 76, 75, 73, 71, 69, 67, 65, 64, 62, 60, 58,
  56, 54, 52, 50, 48, 46, 43, 41, 39, 37, 35, 33, 31, 29, 26, 24, 22, 20, 18,
  16, 13, 11, 9, 7, 4, 2, 0, -2, -4, -7, -9, -11, -13, -15, -18, -20, -22, -24,
  -26, -29, -31, -33, -35, -37, -39, -41, -43, -45, -48, -50, -52, -54, -56,
  -58, -60, -62, -63, -65, -67, -69, -71, -73, -75, -76, -78, -80, -82, -83,
  -85, -87, -88, -90, -91, -93, -94, -96, -97, -99, -100, -101, -103, -104,
  -105, -106, -108, -109, -110, -111, -112, -113, -114, -115, -116, -117, -118,
  -119, -119, -120, -121, -121, -122, -123, -123, -124, -124, -125, -125, -125,
  -126, -126, -126, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127,
  -127, -126, -126, -126, -125, -125, -125, -124, -124, -123, -123, -122, -121,
  -121, -120, -119, -119, -118, -117, -116, -115, -114, -113, -112, -111, -110,
  -109, -108, -107, -105, -104, -103, -101, -100, -99, -97, -96, -94, -93, -91,
  -90, -88, -87, -85, -83, -82, -80, -78, -76, -75, -73, -71, -69, -67, -65,
  -64, -62, -60, -58, -56, -54, -52, -50, -48, -46, -43, -41, -39, -37, -35,
  -33, -31, -29, -26, -24, -22, -20, -18, -16, -13, -11, -9, -7, -4, -2, 127,
  127, 127, 127, 127, 127, 126, 126, 126, 125, 125, 125, 124, 124, 123, 123,
  122, 121, 121, 120, 119, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110,
  109, 108, 107, 105, 104, 103, 101, 100, 99, 97, 96, 94, 93, 91, 90, 88, 87,
  85, 83, 82, 80, 78, 76, 75, 73, 71, 69, 67, 65, 64, 62, 60, 58, 56, 54, 52,
  50, 48, 46, 43, 41, 39, 37, 35, 33, 31, 29, 26, 24, 22, 20, 18, 15, 13, 11,
  9, 7, 4, 2, 0, -2, -4, -7, -9, -11, -13, -15, -18, -20, -22, -24, -26, -29,
  -31, -33, -35, -37, -39, -41, -43, -45, -48, -50, -52, -54, -56, -58, -60,
  -62, -63, -65, -67, -69, -71, -73, -75, -76, -78, -80, -82, -83, -85, -87,
  -88, -90, -91, -93, -94, -96, -97, -99, -100, -101, -103, -104, -105, -107,
  -108, -109, -110, -111, -112, -113, -114, -115, -116, -117, -118, -119, -119,
  -120, -121, -121, -122, -123, -123, -124, -124, -125, -125, -125, -126, -126,
  -126, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -126,
  -126, -126, -125, -125, -125, -124, -124, -123, -123, -122, -121, -121, -120,
  -119, -119, -118, -117, -116, -115, -114, -113, -112, -111, -110, -109, -108,
  -107, -105, -104, -103, -101, -100, -99, -97, -96, -94, -93, -91, -90, -88, 
  -87, -85, -83, -82, -80, -78, -76, -75, -73, -71, -69, -67, -65, -64, -62,
  -60, -58, -56, -54, -52, -50, -48, -46, -43, -41, -39, -37, -35, -33, -31,
  -29, -26, -24, -22, -20, -18, -16, -13, -11, -9, -7, -4, -2, 0, 2, 4, 7, 9,
  11, 13, 15, 18, 20, 22, 24, 26, 29, 31, 33, 35, 37, 39, 41, 43, 45, 48, 50,
  52, 54, 56, 58, 60, 62, 63, 65, 67, 69, 71, 73, 75, 76, 78, 80, 82, 83, 85,
  87, 88, 90, 91, 93, 94, 96, 97, 99, 100, 101, 103, 104, 105, 106, 108, 109,
  110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 119, 120, 121, 121, 122,
  123, 123, 124, 124, 125, 125, 125, 126, 126, 126, 127, 127, 127, 127, 127
};
const Sint16 *ressources::zesinus360 = ressources::cosinus360 + 360;


const Uint32 ressources::tabledegas[180] =
{	0x0400180,0x0420290,0x0440392,0x0500494,0x0600596,0x0700698,
	0x0800795,0x0900893,0x0A00990,0x0A20A80,0x0A40B70,0x0A60C60,
	0x0A80D50,0x0AA0E40,0x0AC0F30,0x0AE1020,0x0B01110,0x0B21200,
	0x0014080,0x0024590,0x0034A92,0x0045094,0x0056096,0x0067097,
	0x0078096,0x0089093,0x009A090,0x00AA180,0x00BA270,0x00CA360,
	0x00DA450,0x00EA540,0x00FA630,0x010A720,0x011A810,0x012A900,
	0x0408001,0x0459002,0x04A9203,0x0509404,0x0609605,0x0709806,
	0x0809507,0x0909308,0x0A19009,0x0A2800A,0x0A3700B,0x0A4600C,
	0x0A5500D,0x0A6400E,0x0A7300F,0x0A82010,0x0A91011,0x0AA0012,
	0x0002080,0x0102190,0x0202292,0x0302394,0x0402496,0x0502598,
	0x0602695,0x0702793,0x0802890,0x0782980,0x0702A90,0x0602B92,
	0x0502C94,0x0402D96,0x0302E98,0x0202F95,0x0102A93,0x0002B92,
	0x0104080,0x0204590,0x0304A92,0x0405094,0x0506096,0x0607098,
	0x0708095,0x0809093,0x090A090,0x0A0A280,0x0B0A470,0x0C0A660,
	0x0B0A850,0x0A0AA40,0x090AC30,0x080AE20,0x070B010,0x060B210,
	0x04080B0,0x04590A0,0x04A9290,0x0509480,0x0609670,0x0709860,
	0x0809550,0x0909340,0x0A09030,0x0A18020,0x0A27010,0x0A36020,
	0x0A45030,0x0A54040,0x0A63050,0x0A72060,0x0A81070,0x0A90080,
	0x0101080,0x0181290,0x0201492,0x0281694,0x0301896,0x0381A98,
	0x0401C95,0x0451E93,0x04A2080,0x0502270,0x0582460,0x0602650,
	0x0702840,0x0782B30,0x0802E20,0x0853010,0x08A3205,0x0903600,
	0x0404080,0x0484890,0x0504A91,0x0605092,0x0706093,0x0807094,
	0x0908093,0x0889092,0x080A090,0x081A280,0x082A670,0x083A860,
	0x084AA50,0x085AC40,0x086AE30,0x087B020,0x088B210,0x089B405,
	0x0458040,0x04A9045,0x040924A,0x0509450,0x0609655,0x070985A,
	0x0809562,0x0909364,0x0A09066,0x0A1806A,0x0A2706C,0x0A36070,
	0x0A45073,0x0A54076,0x0A6307A,0x0A72080,0x0A81085,0x0A90590,
	0x0407080,0x0457890,0x04A8091,0x0508892,0x0609093,0x0709894,
	0x080A093,0x090A892,0x0A0B090,0x0A1B880,0x0A2C070,0x0A3B860,
	0x0A4B050,0x0A5A840,0x0A6A030,0x0A79820,0x0A89010,0x0A98805
};
