//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "main.cc"
// created      : 2002-08-21
// updates      : 2005-01-23
// id		: $Id: main.cc,v 1.3 2005/01/23 19:52:24 gurumeditation Exp $
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
// Place - Suite 330, Boston, MA  02111-1307, USA.
//-----------------------------------------------------------------------------
#include "../include/mentatCode.h"
#include "../include/ecran_hard.h"
#include "../include/audiomixer.h"
#include "../include/configfile.h"
//..............................................................................
configfile oConfigure;

//------------------------------------------------------------------------------
// main function
//------------------------------------------------------------------------------
int main(Sint32 nbArg, char **ptArg)
{ 
	oConfigure.loadconfig();
	if(!oConfigure.scanZeArgs(nbArg, ptArg))
		return 0;
	if(mentatCode::is_verbose) 
	{	printf("===========================================================\n");
		printf("                       TECNOBALLZ START\n");
		printf("===========================================================\n");
	}
	Sint32 error = 0;
	error = mentatCode::first_init(&oConfigure);
	if(error) return error;
	error = mentatCode::game_begin();
	if(error) return error;
	if(mentatCode::is_verbose) 
		printf("===========================================================\n");
	error = mentatCode::desinstall(&oConfigure);
	if(error) return error;
	if(mentatCode::is_verbose) 
		printf("END =======================================================\n");
	oConfigure.saveconfig();
	return error;
}
