//*****************************************************************************
// copyright (c) 1991-2003 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "liste_BOBs.cpp"
// created      : ?
// updates      : 2003-02-25
// function     : gestion de l'affichage des BOBs
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
#include "../include/liste_BOBs.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
liste_BOBs::liste_BOBs(Sint32 nombr)
{
	mentatInit();
	BOBsnombre = nombr;
	clear_list();
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
liste_BOBs::~liste_BOBs()
{	mentatKill();
}

//-----------------------------------------------------------------------------
// clear the list
//-----------------------------------------------------------------------------
void liste_BOBs::clear_list()
{
	BOB_nombre = 0;
	SHA_nombre = 0;
	BOB_killer *z = NULL;
	for(Sint32 i = 0; i < BOBNumMaxi; i++)
	{	BOB__liste[i] = z;
		SHA__liste[i] = z;
	}
}

//-----------------------------------------------------------------------------
// add a sprite to the list
//-----------------------------------------------------------------------------
void liste_BOBs::ajoute_BOB(BOB_killer * unBOB)
{
  if (BOB_nombre >= BOBsnombre -1)
    {
      fprintf (stderr, "liste_BOBs::ajoute_BOB() "
               "max. number reached %i\n", BOB_nombre);
      return;
    } 
	BOB__liste[BOB_nombre] = unBOB;
	BOB_nombre++;
	unBOB->impose_num(BOB_nombre);
	if(unBOB->shadow_BOB())
	{	SHA__liste[SHA_nombre] = unBOB;
		SHA_nombre++;
	}
}

//-----------------------------------------------------------------------------
// display graphic objets (sprites)
//-----------------------------------------------------------------------------
void liste_BOBs::listeBOBgo()
{
	// display the shahows of the objects
	for(Sint32 i = 0; i < SHA_nombre; i++)
	{	BOB_killer *leBOB = SHA__liste[i];
		leBOB->afficheSHA();
	}

	// display the objects
	for(Sint32 i = 0; i < BOB_nombre; i++)
	{	BOB_killer *leBOB = BOB__liste[i];
		leBOB->afficheMSK();
	}
}

//-----------------------------------------------------------------------------
// restaure background under the sprites
//-----------------------------------------------------------------------------
void liste_BOBs::listeBOBrz()
{
	for(Sint32 i = 0; i < BOB_nombre; i++)
	{	BOB_killer *leBOB = BOB__liste[i];
		leBOB->efface_SHA();
		leBOB->efface_MSK();
	}
}
