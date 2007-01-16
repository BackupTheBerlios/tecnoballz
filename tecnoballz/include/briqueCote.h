//******************************************************************************
// copyright (c) 1991-2006 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "briqueCote.h"
// created	: ?
// updates	: 2006-10-02
// fonction	: handle small bricks on the side (walls top, left and right)
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
//******************************************************************************
#ifndef __BRIQUECOTE__
#define __BRIQUECOTE__
//...............................................................................
class briqueCote;
//...............................................................................
#include "../include/sprite_object.h"
#include "../include/mentatCode.h"
//...............................................................................

//-----------------------------------------------------------------------------
// struct of the redraw bricks table
//-----------------------------------------------------------------------------
typedef struct
{	Sint32 iCote_type;	//1=bricote haut ; 2=droite ; 3=gauche
	Sint32 iMapOffset;	//pointer to the background buffer
} coteStruct;

class briqueCote:public mentatCode
{
	public:
		static const Uint32	BRICOTENUM = 12;	//number bricks per wall

	private:
		static const Uint32	BRICOTEGAX = 12;	//x coordinate (bricks of left)
		static const Uint32	BRICOTEGAY = 24;	//y coordinate (bricks of left)
		static const Uint32	BRICOTEDRX = 240;	//x coordinate (bricks of right)
		static const Uint32	BRICOTEDRY = 24;	//y coordinate (bricks of right)
		static const Uint32	BRICOTEHRX = 32;	//x coordinate (bricks of top)
		static const Uint32	BRICOTEHRY = 4;		//y coordinate (bricks of top)
	
		static const Uint32	iMAXBRICOT = 64;
	
		sprite_object*			bricote_hz;	//small horizontal bricks
		sprite_object*			bricote_vt;	//small vertical bricks
		char*				fond_sauve;	//buffer to save background under bricks
		Sint32				mur_duhaut;	//1=wall of the top is breakable
		Sint32				mur_droite;	//1=wall of the right is breakable
		Sint32				mur_gauche;	//1=wall of the left is breakable
		Sint32				fconstruit;	//1=rebuild wall

		Sint32				collisionG;	//x coordinate of the wall of the left
		Sint32				collisionD;	//x coordinate of the wall of the right
		Sint32				collisionH;	//y coordinate of the wall of the top
 
		Sint32				mapgauche0[BRICOTENUM+4];
		Sint32				mapdroite0[BRICOTENUM+4];
		Sint32				mapduHaut0[BRICOTENUM+4];
		
		// wall of sprites (bob_ground = 1)
		sprite_object*			bobwal_lef[BRICOTENUM+4];
		sprite_object*			bobwal_rgh[BRICOTENUM+4];
		sprite_object*			bobwal_top[BRICOTENUM+4];
 
		Sint32*				map_gauche;
		Sint32*				map_droite;
		Sint32*				map_duHaut;

		char*				adr_gauche[BRICOTENUM];
		char*				adr_droite[BRICOTENUM];
		char*				adr_duHaut[BRICOTENUM];

		// state the 3 walls, 2 bricks before and after prevent overflows 
		Sint32				pos_gauche[BRICOTENUM][2];
		Sint32				pos_droite[BRICOTENUM][2];
		Sint32				pos_duHaut[BRICOTENUM][2];

		Uint32				iLargeurBH;	//width of horizontal brick
		Uint32				iHauteurBH;	//height of horizontal brick
		Uint32				iLargeurBV;	//width of vertical brick
		Uint32				iHauteurBV;	//height of vertical brick

		coteStruct*			pCoteTable;	//redraw bricks table
		Sint32				iCoteSauve;
		Sint32				iCoteRemap;

	public:
						briqueCote();
						~briqueCote();
		Sint32				initialise(Sint32 build);
		void				sauve_etat();
		void				sauveFond();
		void				execution1();
		void				afficheSha();
		void				afficheGfx();
		Sint32				getCollisH();
		Sint32				getCollisD();
		Sint32				getCollisG();
		Sint32				collGauche(Sint32 _iPosY);
		Sint32				collDroite(Sint32 _iPosY);
		Sint32				coll_haute(Sint32 _iPosX);
	
	private:
		Sint32				bobbg_init();
		void				execution2();
		void				execution3();
};

#endif
