//******************************************************************************
// copyright (c) 1991-2006 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "briqueCote.cc"
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
//
//******************************************************************************
#include "../include/briqueCote.h"
#include "../include/handler_players.h"
#include "../include/handler_display.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------	
briqueCote::briqueCote()
{
	object_init();
	bricote_hz = (sprite_object *)NULL;	//sprite object (small horizontal bricks)
	bricote_vt = (sprite_object *)NULL;	//sprite object (small vertical bricks)
	mur_duhaut = 0;				//0=wall of the top is unbreakable
	mur_droite = 0;				//0=wall of the right is unbreakable
	mur_gauche = 0;				//0=wall of the left is unbreakable
	fond_sauve = (char *)NULL;		//buffer to save background under bricks
	pCoteTable = (coteStruct *)NULL;
	iCoteSauve = 0;
	iCoteRemap = 0;
	map_gauche = mapgauche0+2;
	map_droite = mapdroite0+2;
	map_duHaut = mapduHaut0+2;
	map_duHaut[0] = map_droite[0] = mapgauche0[0] = -1;
	map_duHaut[1] = map_droite[1] = mapgauche0[1] = 0;
	map_duHaut[BRICOTENUM-2] = map_droite[BRICOTENUM-2] = mapgauche0[BRICOTENUM-2] = 0;
	map_duHaut[BRICOTENUM-1] = map_droite[BRICOTENUM-1] = mapgauche0[BRICOTENUM-1] = -1;
	for(Uint32 i = 0; i < BRICOTENUM; i++)
	{	bobwal_top[i] = NULL;
		bobwal_lef[i] = NULL;
		bobwal_rgh[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
briqueCote::~briqueCote()
{
	memory->release((char *)pCoteTable);
	if(bricote_hz)
	{	delete bricote_hz;	// release sprite object
		bricote_hz = (sprite_object *)NULL;
	}
	if(bricote_vt)
	{	delete bricote_vt;	// release sprite object
		bricote_vt = (sprite_object *)NULL;
	}
	if(fond_sauve)
	{	memory->release(fond_sauve);
		fond_sauve = NULL;
	}
	for(Uint32 i = 0; i < BRICOTENUM; i++)
	{	if(bobwal_top[i] != NULL) 
		{	delete bobwal_top[i];
			bobwal_top[i] = NULL;
		}
		if(bobwal_lef[i] != NULL) 
		{	delete bobwal_lef[i];
			bobwal_lef[i] = NULL;
		}
		if(bobwal_rgh[i] != NULL) 
		{	delete bobwal_rgh[i];
			bobwal_rgh[i] = NULL;
		}
	}
	object_free();
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
Sint32 briqueCote::initialise(Sint32 build)
{
	//###################################################################
	// create a sprite object for small horizontal bricks 
	//###################################################################
	bricote_hz = new sprite_object();
	bricote_hz->create_sprite(BOB_BRICKH, sprites_bitmap, 1);
	iLargeurBH = bricote_hz->get_sprite_width();
	iHauteurBH = bricote_hz->get_sprite_height();

	//###################################################################
	// create a sprite object for small vertical bricks 
	//###################################################################
	bricote_vt = new sprite_object();
	bricote_vt->create_sprite(BOB_BRICKV, sprites_bitmap, 1);
	iLargeurBV = bricote_vt->get_sprite_width();
	iHauteurBV = bricote_vt->get_sprite_height();

	collisionG = (BRICOTEGAX * resolution) + iLargeurBV;
	collisionD = BRICOTEDRX * resolution;
	collisionH = BRICOTEHRY * resolution + iHauteurBH;

	
	//###################################################################
	// area 2 to 4; rebuild wall
	//###################################################################
	Sint32 area = joueurGere->getareaNum();
	if(area < 5 || build > 0)
	{	Sint32 k = -1;
		for(Uint32 i = 0; i < BRICOTENUM; i++)
		{	map_gauche[i] = k;
			map_duHaut[i] = k;
			map_droite[i] = k;
		}
	}

	//###################################################################
	// area 5: recopy the state of the wall of the preceding level
	//###################################################################
	else
	{	char *bleft = joueurGere->getBriLeft();
		char *brigh = joueurGere->getBriRigh();
		char *b_top = joueurGere->getBri_top();
		for(Uint32 i = 0; i < BRICOTENUM; i++)
		{	map_gauche[i] = bleft[i];
			map_duHaut[i] = b_top[i];
			map_droite[i] = brigh[i];
		}
	}

	//###################################################################
	// determine the behavior of the three walls, according to the level 
	//###################################################################
	switch(area)
	{	case 2:
			mur_duhaut = 1;	//1=wall of the top is breakable
			mur_droite = 0;	//0=wall of the right is unbreakable
			mur_gauche = 0;	//0=wall of the left is unbreakable
			break;

		case 3:
			mur_duhaut = 1;	//1=wall of the top is breakable
			mur_droite = 1;	//1=wall of the right is breakable
			mur_gauche = 0;	//0=wall of the left is unbreakable
			break;

		case 4:
			mur_duhaut = 1;	//1=wall of the top is breakable
			mur_droite = 1;	//1=wall of the right is breakable
			mur_gauche = 1;	//1=wall of the left is breakable
			break;

		case 5:
			mur_duhaut = 1;	//1=wall of the top is breakable
			mur_droite = 1;	//1=wall of the right is breakable
			mur_gauche = 1;	//1=wall of the left is breakable
			break;

		default:
			mur_duhaut = 0;	//0=wall of the top is unbreakable
			mur_droite = 0;	//0=wall of the right is unbreakable
			mur_gauche = 0;	//0=wall of the left is unbreakable
			break;
	}
/*
	mur_duhaut = 1;	//1=wall of the top is breakable (test only)
	mur_droite = 1;	//1=wall of the right is breakable (test only)
	mur_gauche = 1;	//1=wall of the left is breakable (test only)
*/

	//###################################################################
	// allocate memory to save background under bricks
	//###################################################################
	Uint32 _iS1 = iLargeurBH * iHauteurBH;	// size of horizontal bricks
	Uint32 _iS2 = iLargeurBH * iHauteurBV;	// size of vertical bricks
	_iS2 = (_iS1 * BRICOTENUM) + (_iS2 * BRICOTENUM) + (_iS2 * BRICOTENUM);
	fond_sauve = memory->alloc(_iS2);
	error_init(memory->retour_err());
	if(erreur_num)
		return (erreur_num);
	
	//###################################################################
	// initialize the sprites of the walls (bob_ground = 1)
	//###################################################################
	bobbg_init();

	//###################################################################
	//allocate memory for the redraw bricks table
	//###################################################################
	pCoteTable =
		(coteStruct *) memory->alloc(iMAXBRICOT * sizeof(coteStruct),
			0x54425243);
	error_init(memory->retour_err());
	return erreur_num;
}

//-----------------------------------------------------------------------------
// initialize the sprites of the walls (bob_ground = 1)
//-----------------------------------------------------------------------------
void briqueCote::bobbg_init()
{
	if(!bob_ground)
          {
            return;
          }

	Sint32 x = BRICOTEHRX * resolution;
	Sint32 yg = BRICOTEGAY * resolution;
	Sint32 yd = BRICOTEDRY * resolution;
	Sint32 lg = bricote_hz->get_sprite_width();
	Sint32 ht = bricote_vt->get_sprite_height();
	for(Uint32 i = 0; i < BRICOTENUM; i++)	//12 bricks per wall
	{
		bobwal_top[i] = new sprite_object();
		bobwal_top[i]->create_sprite(BOB_BRICKH, sprites_bitmap, 1);
		sprites->add(bobwal_top[i]);
		bobwal_top[i]->set_coordinates(x, BRICOTEHRY * resolution);
		if(map_duHaut[i]) bobwal_top[i]->enable();

		bobwal_lef[i] = new sprite_object();
		bobwal_lef[i]->create_sprite(BOB_BRICKV, sprites_bitmap, 1);
		sprites->add(bobwal_lef[i]);
		bobwal_lef[i]->set_coordinates(BRICOTEGAX * resolution, yg);
		if(map_gauche[i]) bobwal_lef[i]->enable();

		bobwal_rgh[i] = new sprite_object();
		bobwal_rgh[i]->create_sprite(BOB_BRICKV, sprites_bitmap, 1);
		sprites->add(bobwal_rgh[i]);
		bobwal_rgh[i]->set_coordinates(BRICOTEDRX * resolution, yd);
		if(map_droite[i]) bobwal_rgh[i]->enable();

		x += lg;
		yd += ht;
		yg += ht;
	}
}

//-----------------------------------------------------------------------------
// recopy the state of walls after a level 
//-----------------------------------------------------------------------------
void briqueCote::sauve_etat()
{	
	char *bleft = joueurGere->getBriLeft();
	char *brigh = joueurGere->getBriRigh();
	char *b_top = joueurGere->getBri_top();
	for(Uint32 i = 0; i < BRICOTENUM; i++)
	{	bleft[i] = map_gauche[i];
		brigh[i] = map_droite[i];
		b_top[i] = map_duHaut[i];
	}
}

//-----------------------------------------------------------------------------
// save background under bricks
//-----------------------------------------------------------------------------
void briqueCote::sauveFond()
{
	
	//###################################################################
	// wall of the top
	//###################################################################
	Sint32 _iModulo = background_screen->get_line_modulo(iLargeurBH);
	Sint32 _iPosX = BRICOTEHRX * resolution;
	char* _pBuffer = fond_sauve;
	char* _pSource;
	for(Uint32 _iIndex = 0; _iIndex < BRICOTENUM; _iIndex++)
	{	_pSource = background_screen->get_pixel_data(_iPosX, BRICOTEHRY * resolution);
		adr_duHaut[_iIndex] = _pBuffer;
		pos_duHaut[_iIndex][0] = _iPosX;
		pos_duHaut[_iIndex][1] = BRICOTEHRY * resolution;
		for(Uint32 _iY = 0; _iY < iHauteurBH; _iY++)
		{	for(Uint32 _iX = 0; _iX < iLargeurBH; _iX++)
				*(_pBuffer++) = *(_pSource++);
			_pSource += _iModulo;
		}
		_iPosX += iLargeurBH;
	}
	
	//###################################################################
	// wall of the right
	//###################################################################
	_iModulo = background_screen->get_line_modulo(iLargeurBV);
	Sint32 _iPosY = BRICOTEGAY * resolution;
	for(Uint32 _iIndex = 0; _iIndex < BRICOTENUM; _iIndex++)
	{	_pSource = background_screen->get_pixel_data(BRICOTEGAX * resolution, _iPosY);
		adr_gauche[_iIndex] = _pBuffer;
		pos_gauche[_iIndex][0] = BRICOTEGAX * resolution;
		pos_gauche[_iIndex][1] = _iPosY;
		for(Uint32 _iY = 0; _iY < iHauteurBV; _iY++)
		{	for(Uint32 _iX = 0; _iX < iLargeurBV; _iX++)
				*(_pBuffer++) = *(_pSource++);
			_pSource += _iModulo;
		}
		_iPosY += iHauteurBV;
	}

	//###################################################################
	// wall of the left
	//###################################################################
	_iPosY = BRICOTEGAY * resolution;
	for(Uint32 _iIndex = 0; _iIndex < BRICOTENUM; _iIndex++)
	{	_pSource = background_screen->get_pixel_data(BRICOTEDRX * resolution, _iPosY);
		adr_droite[_iIndex] = _pBuffer;
		pos_droite[_iIndex][0] = BRICOTEDRX * resolution;
		pos_droite[_iIndex][1] = _iPosY;
		for(Uint32 _iY = 0; _iY < iHauteurBV; _iY++)
		{	for(Uint32 _iX = 0; _iX < iLargeurBV; _iX++)
				*(_pBuffer++) = *(_pSource++);
			_pSource += _iModulo;
		}
		_iPosY += iHauteurBV;
	}
}


//-----------------------------------------------------------------------------
// clear bricks
//-----------------------------------------------------------------------------
void briqueCote::execution1()
{
	if(!bob_ground) 
		execution2();
	else
		execution3();
}

//-----------------------------------------------------------------------------
// restore the background under the brick
//-----------------------------------------------------------------------------
void briqueCote::execution2()
{
	coteStruct *_pCote = pCoteTable + iCoteRemap;
	Sint32 _iType = _pCote->iCote_type;
	if(_iType)
	{	iCoteRemap++;
		iCoteRemap &= (iMAXBRICOT - 1);
		_pCote->iCote_type = 0;
		char *_pFond;	//pointer to the background buffer
		Sint32 posx1;
		Sint32 posx2;
		Sint32 _iPosY;
		Sint32 _iIndex = _pCote->iMapOffset;
		Sint32 _iTmod;
		Sint32 _iBmod;
		Sint32 widt1;
		Sint32 widt2;
		Sint32 heig1;
		Sint32 heig2;
		switch (_iType)
		{
 
 			//###########################################################
			// wall of the top
			//###########################################################
			case 1:
				_pFond = adr_duHaut[_iIndex];
				posx1 = pos_duHaut[_iIndex][0];
				posx2 = posx1 + (handler_display::SHADOWOFFX * resolution);
				_iPosY = pos_duHaut[_iIndex][1];
				_iTmod = background_screen->get_line_modulo(iLargeurBH);
				_iBmod = game_screen->get_line_modulo(iLargeurBH);
				widt1 = iLargeurBH;
				widt2 = iLargeurBH;
				heig1 = iHauteurBH;
				heig2 = iHauteurBH;
				if( _iIndex == 0)
				{	widt2 = widt2 - resolution * 4;
					posx2 = posx2 + resolution * 4;
				}
				break;

 			//###########################################################
			// wall of the left
			//###########################################################
			case 2:
				_pFond = adr_gauche[_iIndex];
				posx1 = pos_gauche[_iIndex][0];
				posx2 = posx1 + (handler_display::SHADOWOFFX * resolution);
				_iPosY = pos_gauche[_iIndex][1];
				_iTmod = background_screen->get_line_modulo(iLargeurBV);
				_iBmod = game_screen->get_line_modulo(iLargeurBV);
				widt1 = iLargeurBV;
				widt2 = iLargeurBV;
				heig1 = iHauteurBV;
				heig2 = iHauteurBV;
				break;

			//###########################################################
			// wall of the right
			//###########################################################
			default:
				_pFond = adr_droite[_iIndex];
				posx1 = pos_droite[_iIndex][0];
				posx2 = posx1 + (handler_display::SHADOWOFFX * resolution);
				_iPosY = pos_droite[_iIndex][1];
				_iTmod = background_screen->get_line_modulo(iLargeurBV);
				_iBmod = game_screen->get_line_modulo(iLargeurBV);
				widt1 = iLargeurBV;
				widt2 = iLargeurBV;
				heig1 = iHauteurBV;
				heig2 = iHauteurBV;
				if( _iIndex == BRICOTENUM - 1)
					heig2 = heig2 - resolution * 4;
				break;
		}
 
		char *_pBuffer = background_screen->get_pixel_data(posx1, _iPosY);
		char *_pTampon = game_screen->get_pixel_data(posx1, _iPosY);

		//###############################################################
		// clear brick
		//###############################################################
		for(Sint32 _iY = 0; _iY < heig1; _iY++)
		{	for(Sint32 _iX = 0; _iX < widt1; _iX++)
			{	*(_pBuffer++) = *(_pFond);
				*(_pTampon++) = *(_pFond++);
			}
			_pBuffer += _iBmod;
			_pTampon += _iTmod;
		}

		//###############################################################
		// clear shadow
		//###############################################################
		display->clr_shadow(posx2, _iPosY + (handler_display::SHADOWOFFY * resolution), widt2, heig2);
	}
}

//-----------------------------------------------------------------------------
// disable sprites of wall 
//-----------------------------------------------------------------------------
void briqueCote::execution3()
{
	for(Uint32 i = 0; i < BRICOTENUM; i++)
	{
		if(bobwal_top[i]->is_enable() && !map_duHaut[i]) bobwal_top[i]->disable();
		if(bobwal_lef[i]->is_enable() && !map_gauche[i]) bobwal_lef[i]->disable();
		if(bobwal_rgh[i]->is_enable() && !map_droite[i]) bobwal_rgh[i]->disable();

	}
}

//-----------------------------------------------------------------------------
// display bricks shadows (before a bricks level) 
//-----------------------------------------------------------------------------
void briqueCote::afficheSha()
{
	if(bob_ground) return;
	Sint32 x = BRICOTEHRX * resolution;
	Sint32 yg = BRICOTEGAY * resolution;
	Sint32 yd = BRICOTEDRY * resolution;
	Sint32 lg = bricote_hz->get_sprite_width();
	Sint32 ht = bricote_vt->get_sprite_height();
	for(Uint32 i = 0; i < BRICOTENUM; i++)	//12 bricks per wall
	{	if(map_duHaut[i])
		{	bricote_hz->set_coordinates(x, BRICOTEHRY * resolution);
			bricote_hz->affich_SHA();
		}
		if(map_gauche[i])
		{	bricote_vt->set_coordinates(BRICOTEGAX * resolution, yg);
			bricote_vt->affich_SHA();
		}
		if(map_droite[i])
		{	bricote_vt->set_coordinates(BRICOTEDRX * resolution, yd);
			bricote_vt->affich_SHA();
		}
		x += lg;
		yd += ht;
		yg += ht;
	}
}

//-----------------------------------------------------------------------------
// display bricks of the three walls (before a bricks level) 
//-----------------------------------------------------------------------------
void briqueCote::afficheGfx()
{
	if(bob_ground) return;
	Sint32 x = BRICOTEHRX * resolution;
	Sint32 yg = BRICOTEGAY * resolution;
	Sint32 yd = BRICOTEDRY * resolution;
	Sint32 lg = bricote_hz->get_sprite_width();
	Sint32 ht = bricote_vt->get_sprite_height();
	for(Uint32 i = 0; i < BRICOTENUM; i++)	// 12 bricks aside
	{	if(map_duHaut[i])
		{	bricote_hz->set_coordinates(x, BRICOTEHRY * resolution);
			bricote_hz->affich_MSK();
		}
		if(map_gauche[i])
		{	bricote_vt->set_coordinates(BRICOTEGAX * resolution, yg);
			bricote_vt->affich_MSK();
		}
		if(map_droite[i])
		{	bricote_vt->set_coordinates(BRICOTEDRX * resolution, yd);
			bricote_vt->affich_MSK();
		}
		x += lg;
		yd += ht;
		yg += ht;
	}
}

//-----------------------------------------------------------------------------
// return y coordinate of the wall of the top
//-----------------------------------------------------------------------------
Sint32 briqueCote::getCollisH()
{
	return collisionH;
}

//-----------------------------------------------------------------------------
// return x coordinate of the wall of the right
//-----------------------------------------------------------------------------
Sint32 briqueCote::getCollisD()
{
	return collisionD;
}

//-----------------------------------------------------------------------------
// return x coordinate of the wall of the left
//-----------------------------------------------------------------------------
Sint32 briqueCote::getCollisG()
{
	return collisionG;
}

//-----------------------------------------------------------------------------
// collision with the wall of the left
// output	<= 0: no collision / 1:collision
//-----------------------------------------------------------------------------
Sint32 briqueCote::collGauche(Sint32 _iPosY)
{
	if(mur_gauche == 1)
	{	Sint32 _iIndex =
			(Sint32) ( (Sint32) (_iPosY - BRICOTEGAY * resolution) /
			(Sint32) (iHauteurBV));
		if(map_gauche[_iIndex] != 0)
		{	if(_iIndex>=0 && _iIndex<(Sint32)BRICOTENUM)
			{	map_gauche[_iIndex] = 0;
				coteStruct *_pCote = pCoteTable + iCoteSauve;
				iCoteSauve++;
				iCoteSauve &= (iMAXBRICOT - 1);
				_pCote->iCote_type = 2;
				_pCote->iMapOffset = _iIndex;
			}
			return 1;
		}
		else
			return 0;//there is no more brick on side which protects
	}
	else
		return 1;
}

//-----------------------------------------------------------------------------
// collision with the wall of the right
// output	<= 0: no collision / 1:collision
//-----------------------------------------------------------------------------
Sint32 briqueCote::collDroite(Sint32 _iPosY)
{
	if(mur_droite == 1)
	{	Sint32 _iIndex =
			(Sint32) ( (Sint32) (_iPosY - BRICOTEDRY * resolution) /
			(Sint32) (iHauteurBV));
		if(map_droite[_iIndex] != 0)
		{	if(_iIndex>=0 && _iIndex<(Sint32)BRICOTENUM)
			{	map_droite[_iIndex] = 0;
				coteStruct *_pCote = pCoteTable + iCoteSauve;
				iCoteSauve++;
				iCoteSauve &= (iMAXBRICOT - 1);
				_pCote->iCote_type = 3;
				_pCote->iMapOffset = _iIndex;
			}
			return 1;
		}
		else
			return 0;	//there is no more brick on side which protects
	}
	else
		return 1;
}

//-----------------------------------------------------------------------------
// collision with the wall of the top
// output	<= 0: no collision / 1:collision
//-----------------------------------------------------------------------------
Sint32 briqueCote::coll_haute(Sint32 _iPosX)
{
	if(mur_duhaut == 1)
	{	Sint32 _iIndex =
			(Sint32) ((Sint32)(_iPosX - BRICOTEHRX * resolution) /
			(Sint32)(iLargeurBH));
		if(map_duHaut[_iIndex] != 0)
		{	if(_iIndex>=0 && _iIndex<(Sint32)BRICOTENUM)
 			{	map_duHaut[_iIndex] = 0;
				coteStruct *_pCote = pCoteTable + iCoteSauve;
				iCoteSauve++;
				iCoteSauve &= (iMAXBRICOT - 1);
				_pCote->iCote_type = 1;
				_pCote->iMapOffset = _iIndex;
			}
			return 1;
		}
		else
			return 0;	//there is no more brick on side which protects
	}
	else
		return 1;
}
