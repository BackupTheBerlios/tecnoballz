//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "runGestion.h"
// created      : ?
// updates      : 2004-04-12
//-----------------------------------------------------------------------------
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
//
//*****************************************************************************
#include "../include/runGestion.h"
// Constructeur ----------------------------------------------------------------
   runGestion::runGestion()
{  initialise();
}
// Destructeur -----------------------------------------------------------------
    runGestion::~runGestion()
{	liberation();
}
// Initialise ------------------------------------------------------------------
void runGestion::initialise()
{	mentatInit();
	end_return = 0;
}
// Vire -------------------------------------------------------------------------
void runGestion::liberation()
{	mentatKill();
}
//------------------------------------------------------------------------------
Sint32 runGestion::first_init()
{	return(erreur_num);
}
//------------------------------------------------------------------------------
Sint32 runGestion::zeMainLoop() 
{	return(erreur_num);
}
