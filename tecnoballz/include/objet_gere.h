//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "objet_gere.h"
// created		: ?
// updates		: 2004-10-27
// function	: template of management of object list
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
//
//*****************************************************************************
#ifndef __OBJET_GERE__
#define __OBJET_GERE__

template < class X > class objet_gere;
#include "../include/liste_BOBs.h"
#include "../include/ecran_hard.h"
#include "../include/mentatCode.h"

template < class X > class objet_gere:public virtual mentatCode
{
	protected:
		X**					objetListe;
		Sint32				objetTotal;
		Sint32				objetNombr;
		bool 				objects_have_shades;
		Sint32				BOBtypeNum;
		Sint32				fTableByte;
	
	public:
							objet_gere();
							~objet_gere();
		void				littleInit();
		void				littleDead();
		Sint32				init_liste();
		void				xDesactive();
		void				xBOBactive();
		X**					listeObjet();
		Sint32				totalObjet();
		void				init_total(Sint32 total);
		void				xAnimation();
		X*					get_object(Uint32 index);
};

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
template < class X > objet_gere < X >::objet_gere()
{
}

//-------------------------------------------------------------------------------
// release the object
//-------------------------------------------------------------------------------
template < class X > objet_gere < X >::~objet_gere()
{
}

//-------------------------------------------------------------------------------
// Reset some members
//-------------------------------------------------------------------------------
template < class X > void objet_gere < X >::littleInit()
{
	mentatInit();
	objetTotal = 0;
	objetNombr = 0;
	objetListe = NULL;
	objects_have_shades = 0;
	BOBtypeNum = 0;
	fTableByte = 0;
}

//-------------------------------------------------------------------------------
// release the list of sprite object
//-------------------------------------------------------------------------------
template < class X > void objet_gere < X >::littleDead()
{
	if(objetListe)
	{	for(Sint32 i = 0; i < objetTotal; i++)
		{	X *monPT = objetListe[i];
			if(monPT)
				delete monPT;
			objetListe[i] = (X *) NULL;
		}
		memory->release((char *)objetListe);
		objetListe = NULL;
	}
	mentatKill();
}

//-------------------------------------------------------------------------------
// return list address
//-------------------------------------------------------------------------------
template < class X > X ** objet_gere < X >::listeObjet()
{
	return objetListe;
}

//-------------------------------------------------------------------------------
// return number of objects
//-------------------------------------------------------------------------------
template < class X > Sint32 objet_gere < X >::totalObjet()
{
	return objetTotal;
}
//-------------------------------------------------------------------------------
// initialize number of objects
//-------------------------------------------------------------------------------
template < class X > void objet_gere < X >::init_total(Sint32 total)
{
	objetTotal = total;
}
//-------------------------------------------------------------------------------
// initialize the list of sprite object
//-------------------------------------------------------------------------------
template < class X > Sint32 objet_gere < X >::init_liste()
{
	GIF_bitMap *image = image_BOBs;
	if(objetTotal)
	{	objetListe = (X **) (memory->alloc(sizeof(X *) * objetTotal,
			0x4F424A47));
		error_init(memory->retour_err());
		if(erreur_num)
			return (erreur_num);
		X *zeBOB = new X();
		zeBOB->setListNum(0);
		objetListe[0] = zeBOB;
		// one reserves only once the memory of the graphics
		error_init(zeBOB->initialise(BOBtypeNum, image, objects_have_shades, fTableByte));
		if(erreur_num) return erreur_num;
		BOBgestion->ajoute_BOB(zeBOB);
		liste_BOBs *oDraw = BOBgestion;
		for(Sint32 i = 1; i < objetTotal; i++)
		{	X *monPT = new X();
			zeBOB->duplicaBOB(monPT);
			objetListe[i] = monPT;
			oDraw->ajoute_BOB(monPT);
			monPT->setListNum(i);
			monPT->set_method(BOB_killer::METHOD_TAB);
		}
	}
	return erreur_num;
}

//-------------------------------------------------------------------------------
// enable all sprite object
//-------------------------------------------------------------------------------
template < class X > void objet_gere < X >::xBOBactive()
{
	for(Sint32 i = 0; i < objetTotal; i++)
	{	X *monPT = objetListe[i];
		monPT->BOB_active();
	}
}

//-------------------------------------------------------------------------------
// disable all sprite object
//-------------------------------------------------------------------------------
template < class X > void objet_gere < X >::xDesactive()
{
	for(Sint32 i = 0; i < objetTotal; i++)
	{	X *monPT = objetListe[i];
		monPT->BOB_desact();
	}
}
//-------------------------------------------------------------------------------
// once time animation for all sprites 
//-------------------------------------------------------------------------------
template < class X > void objet_gere < X >::xAnimation()
{
	X *monPT = objetListe[0];
	monPT->animRepete();
	Sint32 o = monPT->litAnimOff();
	for(Sint32 i = 1; i < objetTotal; i++)
	{	monPT = objetListe[i];
		monPT->change_GFX(o);
	}
}

//-------------------------------------------------------------------------------
// return an object
//-------------------------------------------------------------------------------
template < class X > X* objet_gere < X >::get_object(Uint32 index)
{
	if(index >= objetTotal) return NULL;
	else return objetListe[index];
}


#endif
