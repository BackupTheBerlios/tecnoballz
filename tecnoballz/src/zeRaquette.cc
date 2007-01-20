/** 
 * @file zeRaquette.cc
 * @brief Manage the bumpers 
 * @date 2007-01-13
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.9 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: zeRaquette.cc,v 1.9 2007/01/20 16:16:06 gurumeditation Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */
#include "../include/zeRaquette.h"
#include "../include/ressources.h"
#include "../include/joueurData.h"
#include "../include/zeFireBump.h"
#include "../include/handler_keyboard.h"

//-----------------------------------------------------------------------------
// bricks levels: create the object
//-----------------------------------------------------------------------------
zeRaquette::zeRaquette()
{
	littleInit();
	tecBumper1 = NULL;
	tecBumper2 = NULL;
	tecBumper3 = NULL;
	tecBumper4 = NULL;
	tec_robot0 = NULL;
	objetTotal = 5;
	objects_have_shades = true;
	BOBtypeNum = BOB_BUMPHR;
	raket_team = 0;
	raketLarge = 64 * resolution;
	rak_invers = 0;
	zeFireBump *fBump = new zeFireBump();
	tecBumper1 = new tecno_bump(fBump);
	fBump = new zeFireBump();
	tecBumper2 = new tecno_bump(fBump);
	fBump = new zeFireBump();
	tecBumper3 = new tecno_bump(fBump);
	fBump = new zeFireBump();
	tecBumper4 = new tecno_bump(fBump);
	tec_robot0 = new tecno_bump(NULL);      
	bumperMini = 32 * resolution;
	bumperMaxi = 224 * resolution;
	bumperYhau = (20 - 8) * resolution;
	bumperYbas = (232 - 8) * resolution; 
	bumperXgau = 20 * resolution;
	bumperXdro = 228 * resolution;
	width_maxi = 64 * resolution;
	width_mini = 16 * resolution;
}

//-----------------------------------------------------------------------------
// gards levels: create the object
//-----------------------------------------------------------------------------
zeRaquette::zeRaquette(Sint32 nBob)
{ 
	littleInit();
	tecBumper1 = NULL;
	tecBumper2 = NULL;
	tecBumper3 = NULL;
	tecBumper4 = NULL;
	tec_robot0 = NULL;
	objetTotal = 1;
	objects_have_shades = true;
	fTableByte = 0;
	BOBtypeNum = BOB_BUMPER;
	raket_team = 0;
	raketLarge = 32* resolution;
	rak_invers = 0;
	tecBumper1 = new tecno_bump(NULL);
	width_maxi = 32 * resolution;
	width_mini = 32 * resolution;
	bumperYbas = (232 - 8) * resolution; 
	bumperMini = 16 * resolution;
	bumperMaxi = 300 * resolution;
}


//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
zeRaquette::~zeRaquette()
{
	littleDead();
}

//------------------------------------------------------------------------------
// bricks levels: intialize fires of bumpers
//------------------------------------------------------------------------------
Sint32 zeRaquette::init_fires()
{
	error_init(tecBumper1->init_fires());
	if(erreur_num)
		return (erreur_num);
	error_init(tecBumper2->init_fires());
	if(erreur_num)
		return (erreur_num);
	error_init(tecBumper3->init_fires());
	if(erreur_num)
		return (erreur_num);
	error_init(tecBumper4->init_fires());
	return (erreur_num);
}

//------------------------------------------------------------------------------
// bricks levels: intialize bumpers
//------------------------------------------------------------------------------
Sint32 zeRaquette::init_liste()
{
	if(!objetTotal) return erreur_num;
	
	//###################################################################
	// allocate list of sprites memory
	//###################################################################
	objetListe =
		(tecno_bump **) (memory->alloc(sizeof(tecno_bump *) * objetTotal,
			0x4F424A47));
	error_init(memory->retour_err());
	if(erreur_num) return erreur_num;

	//###################################################################
	// gards levels: create one simple bumper
	//###################################################################
    if(objetTotal == 1)
    {	tecBumper1->set_object_pos(0);
		error_init(tecBumper1->initialise(BOBtypeNum, image_BOBs, 1, 0));
		if(erreur_num) return erreur_num;
		sprites->add(tecBumper1);
		objetListe[0] = tecBumper1;
		tecBumper1->coordonnee(keyboard->get_mouse_x(), bumperYbas);
		tecBumper1->colLargeur = raketLarge;	// bumper's width
		tecBumper1->bumpNumero = 1;
		tecBumper1->bumperType = 0;
		tecBumper1->rebonds_Ga = midi1_left;	//ball rebounds table
		tecBumper1->rebonds_Dr = midi1Right;	//ball rebounds table 
		tecBumper1->bump_actif = 1;
		tecBumper1->enable();
		tecBumper1->direct_tab = ballePets1;	// table direction balle collee
    }
 	else
    {
		//###################################################################
		// bricks levels: create 4 bumpers sprites
		//###################################################################

		// load bumpers graphic page
		Uint32 npage;
		//if((hasard_val & 0x001))
		if(joueurGere->getAreaNum() > 2)	
			npage = ressources::RESBUMPER1;
		else
			npage = ressources::RESBUMPER2;
		error_init(pRessource->loadSprite(npage));
		if(erreur_num) return erreur_num;

		// create bottom bumper sprite
		tecBumper1->set_object_pos(0);
		error_init(tecBumper1->initialise(BOB_BUMPHR, image_BOBs, 1, 0));
		if(erreur_num) return erreur_num;
		sprites->add(tecBumper1);
		objetListe[0] = tecBumper1;

		// create left bumper sprite
		tecBumper2->set_object_pos(1);
		error_init(tecBumper2->initialise(BOB_BUMPVT, image_BOBs, 1, 0));
		if(erreur_num) return erreur_num;
		sprites->add(tecBumper2);
		objetListe[1] = tecBumper2;

		// create top bumper sprite
		tecBumper3->set_object_pos(2);
		tecBumper1->duplicaBOB(tecBumper3);
		sprites->add(tecBumper3);
		objetListe[2] = tecBumper3;

		// create right bumper sprite
		tecBumper4->set_object_pos(3);
		tecBumper2->duplicaBOB(tecBumper4);
		sprites->add(tecBumper4);
		objetListe[3] = tecBumper4;

		// release bumpers graphic page
		pRessource->freeSprite();
	}
	return erreur_num;
}

//-------------------------------------------------------------------------------
// bricks levels: generate robot bumper sprite
//-------------------------------------------------------------------------------
Sint32 zeRaquette::init_robot()
{
	tec_robot0->set_object_pos(4);
	error_init(tec_robot0->initialise(BOB_ROBOT0, image_BOBs, 1, 0));
	if(erreur_num) return erreur_num;
	sprites->add(tec_robot0);
	objetListe[4] = tec_robot0;
	return erreur_num;
}

//-------------------------------------------------------------------------------
// bricks levels: activate robot bumper
//-------------------------------------------------------------------------------
void zeRaquette::activrobot()
{
	Sint32 centre;
	tec_robot0->enable();
	centre = bumperMini + (bumperMaxi  - bumperMini) / 2 -
				(tec_robot0->colLargeur / 2);
	tec_robot0->coordonnee(centre, 232 * resolution);
	tec_robot0->bump_actif = 1;
}

//-------------------------------------------------------------------------------
// bricks levels: deactivate robot bumper
//-------------------------------------------------------------------------------
void zeRaquette::deactrobot()
{
	tec_robot0->disable();
	tec_robot0->bump_actif = 0;
}


//------------------------------------------------------------------------------
// bricks levels: intialize bumpers
//------------------------------------------------------------------------------
void zeRaquette::initBumper(barreScore *score, zeGigaBlit *blitz, zeNewBalls *balls)
{
	ptBarreScr = score;
	ptGigaBlit = blitz;
	ptNewBalls = balls;
	
	raketLarge = joueurGere->getLargeur();
	Sint32 centre = (bumperMaxi - bumperMini) / 2 - (raketLarge / 2);

	// bottom bumper
	tecBumper1->coordonnee(centre, bumperYbas);
	tecBumper1->colLargeur = raketLarge;	// bumper's width : 8,16,24,32,40,48,56 or 64
	tecBumper1->bumpNumero = 1;
	tecBumper1->bumperType = 0;
	tecBumper1->bumpActive(raket_team, raketLarge, 3);
	tecBumper1->bump_TFIRE = 2;
	tecBumper1->bumper_FX0 = 0;
	tecBumper1->bumper_FY0 = -5 * resolution;
	tecBumper1->bumper_FX1 = -1 * resolution;
	tecBumper1->bumper_FY1 = -4 * resolution;
	tecBumper1->bumper_FX2 = 1 * resolution;
	tecBumper1->bumper_FY2 = -4 * resolution;
	tecBumper1->bump_Xscie = 32 * resolution;
	tecBumper1->bump_Yscie = -20 * resolution;
	tecBumper1->bump_xdeca = 0;
	tecBumper1->bump_ydeca = -10 * resolution;
	tecBumper1->rebonds_Ga = midi1_left;                         // rebonds raquette va a gauche
	tecBumper1->rebonds_Dr = midi1Right;                         // rebonds raquette va a droite
	tecBumper1->direct_tab = ballePets1;                         // table direction balle collee
	tecBumper1->width_mini = width_mini;
	tecBumper1->width_maxi = width_maxi;

	// right bumper
	tecBumper2->coordonnee(bumperXdro, centre);
	tecBumper2->colHauteur = raketLarge;
	tecBumper2->bumpNumero = 2;
	tecBumper2->bumperType = 1;
	tecBumper2->bumpActive(raket_team, raketLarge, joueurGere->get_bumpOn(2));
	tecBumper2->bump_TFIRE = 2;
	tecBumper2->bumper_FX0 = -5 * resolution;
	tecBumper2->bumper_FY0 = 0;
	tecBumper2->bumper_FX1 = -4 * resolution;
	tecBumper2->bumper_FY1 = 1 * resolution;
	tecBumper2->bumper_FX2 = -4 * resolution;
	tecBumper2->bumper_FY2 = -1 * resolution;
	tecBumper2->bump_Xscie = -20 * resolution;
	tecBumper2->bump_Yscie = 32 * resolution;
	tecBumper2->bump_xdeca = -10 * resolution;
	tecBumper2->bump_ydeca = 0;
	tecBumper2->rebonds_Ga = midi2_left;
	tecBumper2->rebonds_Dr = midi2Right;
	tecBumper2->direct_tab = ballePets2;
	tecBumper2->width_mini = width_mini;
	tecBumper2->width_maxi = width_maxi;
	joueurGere->set_bumpOn(2, tecBumper2->bump_actif);

	// top bumper
	tecBumper3->coordonnee(centre, bumperYhau);
	tecBumper3->colLargeur = raketLarge;
	tecBumper3->bumpNumero = 3;
	tecBumper3->bumperType = 0;
	tecBumper3->bumpActive(raket_team, raketLarge, joueurGere->get_bumpOn(3));
	tecBumper3->bump_TFIRE = 2;
	tecBumper3->bumper_FX0 = 0;
	tecBumper3->bumper_FY0 = 5 * resolution;
	tecBumper3->bumper_FX1 = 1 * resolution;
	tecBumper3->bumper_FY1 = 4 * resolution;
	tecBumper3->bumper_FX2 = -1 * resolution;
	tecBumper3->bumper_FY2 = 4 * resolution;
	tecBumper3->bump_Xscie = 32 * resolution - 5;
	tecBumper3->bump_Yscie = 24 * resolution;
	tecBumper3->bump_xdeca = 0;
	tecBumper3->bump_ydeca = 10 * resolution;
	tecBumper3->rebonds_Ga = midi3_left;
	tecBumper3->rebonds_Dr = midi3Right;
	tecBumper3->direct_tab = ballePets3;
	tecBumper3->width_mini = width_mini;
	tecBumper3->width_maxi = width_maxi;
	joueurGere->set_bumpOn(3, tecBumper3->bump_actif);


	// Bumper de gauche
	tecBumper4->coordonnee(bumperXgau, centre);
	tecBumper4->colHauteur = raketLarge;
	tecBumper4->bumpNumero = 4;
	tecBumper4->bumperType = 1;
	tecBumper4->bumpActive(raket_team, raketLarge, joueurGere->get_bumpOn(4));
	tecBumper4->bump_TFIRE = 2;
	tecBumper4->bumper_FX0 = 5 * resolution;
	tecBumper4->bumper_FY0 = 0 * resolution;
	tecBumper4->bumper_FX1 = 4 * resolution;
	tecBumper4->bumper_FY1 = 1 * resolution;
	tecBumper4->bumper_FX2 = 4 * resolution;
	tecBumper4->bumper_FY2 = -1 * resolution;
	tecBumper4->bump_Xscie = 24 * resolution;
	tecBumper4->bump_Yscie = 32 * resolution - 5;
	tecBumper4->bump_xdeca = 10 * resolution;
	tecBumper4->bump_ydeca = 00;
	tecBumper4->rebonds_Ga = midi4_left;
	tecBumper4->rebonds_Dr = midi4Right;
	tecBumper4->direct_tab = ballePets4;
	tecBumper4->width_mini = width_mini;
	tecBumper4->width_maxi = width_maxi;
	joueurGere->set_bumpOn(4, tecBumper4->bump_actif);


	// Bumper robot du bas
	tec_robot0->coordonnee(centre, bumperYbas);
	tec_robot0->colLargeur = width_maxi;	// bumper's width always 128 pixels
	tec_robot0->bumpNumero = 5;				// bumper robot
	tec_robot0->bumperType = 0;
	tec_robot0->bump_TFIRE = 2;
	tec_robot0->bumper_FX0 = 0;
	tec_robot0->bumper_FY0 = -5 * resolution;
	tec_robot0->bumper_FX1 = -1 * resolution;
	tec_robot0->bumper_FY1 = -4 * resolution;
	tec_robot0->bumper_FX2 = 1 * resolution;
	tec_robot0->bumper_FY2 = -4 * resolution;
	tec_robot0->bump_Xscie = 32 * resolution;
	tec_robot0->bump_Yscie = -20 * resolution;
	tec_robot0->bump_xdeca = 0;
	tec_robot0->bump_ydeca = -10 * resolution;
	tec_robot0->rebonds_Ga = midi1_left;	// rebonds raquette va a gauche
	tec_robot0->rebonds_Dr = midi1Right;	// rebonds raquette va a droite
	tec_robot0->direct_tab = ballePets1;	// table direction balle collee
}

//------------------------------------------------------------------------------
// drop fires from bumpze
//------------------------------------------------------------------------------
void zeRaquette::lache_tirs()
{
	
	// Mode  solo
	if(!raket_team)
	{	if(keyboard->is_left_button())
		{	tecBumper1->lache_tirs();	//tecno_bump::lache_tirs (bumper object)
			tecBumper2->lache_tirs();
			tecBumper3->lache_tirs();
			tecBumper4->lache_tirs();
		}
	}
	else

	// Mode Team (no implemented)
	{
	}
	tecBumper1->deplaceTir();
	tecBumper2->deplaceTir();
	tecBumper3->deplaceTir();
	tecBumper4->deplaceTir();
}

//------------------------------------------------------------------------------
// bricks levels: drop the ball
//------------------------------------------------------------------------------
void zeRaquette::lacheBalle()
{
	//###################################################################
	// release ball
	//###################################################################
	if(!raket_team)
	{	if(keyboard->is_right_button())
		{	tecBumper1->lacheBalle();
			tecBumper2->lacheBalle();
			tecBumper3->lacheBalle();
 			tecBumper4->lacheBalle();
#ifndef SOUNDISOFF
			audio->lostm_stop();
#endif
		}
	}
	//###################################################################
	// Mode Team (not implemented)
	//###################################################################
	/* else
	{

	}*/
}

//------------------------------------------------------------------------------
// guards levels: drop the ball
//------------------------------------------------------------------------------
void zeRaquette::lacheBall2()
{
	if(keyboard->is_right_button())
		tecBumper1->lacheBalle();
}

//------------------------------------------------------------------------------
// bricks levels: release all balls
//------------------------------------------------------------------------------
void zeRaquette::free_balls()
{
	tecBumper1->lacheBalle();
	tecBumper2->lacheBalle();
	tecBumper3->lacheBalle();
	tecBumper4->lacheBalle();	
}

//------------------------------------------------------------------------------
// bricks levels: movement of bumpers
//------------------------------------------------------------------------------
void zeRaquette::bp_deplace()
{
  Sint32 speed = 0;
  const Sint32 **tabB1, **tabB2, **tabB3, **tabB4;
  Sint32 x = tecBumper1->position_x;
  Sint32 off_x = keyboard->get_mouse_x_offset();
  // Mode Solo
  if(!raket_team)
  { raketDepla = 0;                                            // pas de deplacement
    rakVgauche = 0;
    rakVdroite = 0;
    
    if(!keyboard->is_right_left_buttons() && !ptGigaBlit->isactivate() ) //if 2 mouse buttons are pressed or GigaBlitz runn also no test
    { if(rak_invers>0)
      { off_x = -off_x;
      }
      
      x += off_x;
      if(off_x < 0)
      { if(x <= bumperMini)
          x = bumperMini;
        raketDepla = 1;                                          // deplacement a gauche 
        rakVgauche = -off_x;
        speed = rakVgauche;
        tabB1 = tecBumper1->rebonds_Ga;
        tabB2 = tecBumper2->rebonds_Ga;
        tabB3 = tecBumper3->rebonds_Ga;
        tabB4 = tecBumper4->rebonds_Ga;
      }
      else
      {
        Sint32 i = bumperMaxi - raketLarge;
        if(x >= i)
          x = i;
        raketDepla = 2;                                          // deplacement a droite
        rakVdroite = off_x;
        speed = rakVdroite;
        tabB1 = tecBumper1->rebonds_Dr;
        tabB2 = tecBumper2->rebonds_Dr;
        tabB3 = tecBumper3->rebonds_Dr;
        tabB4 = tecBumper4->rebonds_Dr;
      }

      // selectionne table de rebond balle suivant le deplacement
      if(speed > 10)
        speed = 10;
      tecBumper1->rebonds_GD = *(tabB1 + speed);
      tecBumper2->rebonds_GD = *(tabB2 + speed);
      tecBumper3->rebonds_GD = *(tabB3 + speed);
      tecBumper4->rebonds_GD = *(tabB4 + speed);

      // Change position des raquettes
      tecBumper1->changePosX(x);                                 // raquette du bas
      tecBumper2->changePosY(x - 16);                            // raquette de droite
      tecBumper3->changePosX(x);                                 // raquette du haut
      tecBumper4->changePosY(x - 16);                            // raquette de gauche
    }

  }

  // Mode Team (On Amiga I had two mice simultaneously)
  else
  {

  }
}

//------------------------------------------------------------------------------
// guards levels: bumper moving
//------------------------------------------------------------------------------
void zeRaquette::bp_deplac2()
{
	Sint32 speed = 0;
	const Sint32 **tabB1;
	Sint32 x = tecBumper1->position_x;
	Sint32 off_x = keyboard->get_mouse_x_offset();
	// mode solo
	if(!raket_team)
	{	raketDepla = 0;			//no move
		rakVgauche = 0;
		rakVdroite = 0;
		if(rak_invers>0)
			off_x = -off_x;
		x += off_x;
		if(off_x < 0)
		{	if(x <= bumperMini)
			x = bumperMini;
			raketDepla = 1;		//moving on the left
			rakVgauche = -off_x;
			speed = rakVgauche;
			tabB1 = tecBumper1->rebonds_Ga;
		}
		else
		{	Sint32 i = bumperMaxi - raketLarge;
			if(x >= i)
				x = i;
			raketDepla = 2;		//moving on the right
			rakVdroite = off_x;
			speed = rakVdroite;
			tabB1 = tecBumper1->rebonds_Dr;
		}

		//select table of rebound ball according to bumper's moving 
		if(speed > 10)
			speed = 10;
		tecBumper1->rebonds_GD = *(tabB1 + speed);

		//change position of bumpers
		tecBumper1->changePosX(x);	//bottom bumper
		tecBumper1->flickerRun();	//flick the bumper
	}

	// mode team, no implemented (on Amiga I had two mice simultaneously)
	else
	{

	}
}

//------------------------------------------------------------------------------
// move the bottom robot bumper
//------------------------------------------------------------------------------
void zeRaquette::move_robot()
{ 
	if(tec_robot0->bump_actif)
	{	Sint32 t = ptNewBalls->totalObjet();
		sprite_ball **aList = ptNewBalls->listeObjet();
		Sint32 pos_y = 0 ;
		sprite_ball *balle = 0x0;
		for(Sint32 i = 0; i < t; i++)
		{	sprite_ball *b = *(aList++);
			if(b->flag_actif)
			{	if(b->directBall >= 36 && b->directBall <= 60)
				{	if(b->position_y > pos_y)
					{	pos_y = b->position_y;
						balle = b;
					}
				}
			}
		}

		if(pos_y > 0)
		{	Sint32 bumpx = tec_robot0->position_x;
			Sint32 ballx = balle->position_x - 64;
			Sint32 offsx = bumpx - ballx;
			if(offsx > 10)
				offsx = 10;
			if(offsx < -10)
				offsx = -10;
			bumpx = bumpx - offsx;
        
			if(bumpx < 64) 
				bumpx = 64;
			if(bumpx > 320)
				bumpx = 320;
	
			offsx = tec_robot0->position_x;
			tec_robot0->position_x = bumpx;
			offsx = bumpx - offsx;   
			const Sint32 **tabB1;
			if(offsx < 0)
			{	offsx = -offsx;
				tabB1 = tec_robot0->rebonds_Ga;
			}
			else
				tabB1 = tec_robot0->rebonds_Dr;
			tec_robot0->rebonds_GD = *(tabB1 + offsx);
		}
	}
}

//------------------------------------------------------------------------------
// return bumper object pointer
//------------------------------------------------------------------------------
tecno_bump *zeRaquette::demandeRak(Sint32 numer)
{
	switch (numer)
	{	case 1:
			return tecBumper1;
		case 2:
			return tecBumper2;
		case 3:
			return tecBumper3;
		case 4:
			return tecBumper4;
		case 5:
			return tec_robot0;
	}
	return ((tecno_bump *) NULL);
}

//------------------------------------------------------------------------------
// bricks levels: set max bumpers size
//------------------------------------------------------------------------------
void zeRaquette::maxi_bumps()
{
	if(raketLarge >= 64 * resolution) return;
	raketLarge = 64 * resolution;
	Sint32 x = tecBumper1->retournePX();
	Sint32 i = bumperMaxi - raketLarge;
	if(x >= i)
	{	x = i;
		tecBumper1->changePosX(x);
		tecBumper2->changePosY(x);
		tecBumper3->changePosX(x);
		tecBumper4->changePosY(x);
	}
	tecBumper1->bumperHgfx(raketLarge);
	tecBumper2->bumperVgfx(raketLarge);
	tecBumper3->bumperHgfx(raketLarge);
	tecBumper4->bumperVgfx(raketLarge);
	joueurGere->setLargeur(raketLarge);
	
}

//------------------------------------------------------------------------------
// bricks levels: increase bumpers's size
//------------------------------------------------------------------------------
void zeRaquette::incremente()
{
	if(raketLarge < (64 * resolution))
	{	raketLarge += (8 * resolution);
		Sint32 x = tecBumper1->retournePX();
		Sint32 i = bumperMaxi - raketLarge;
		if(x >= i)
		{	x = i;
			tecBumper1->changePosX(x);
			tecBumper2->changePosY(x);
			tecBumper3->changePosX(x);
			tecBumper4->changePosY(x);
		}
		tecBumper1->bumperHgfx(raketLarge);
		tecBumper2->bumperVgfx(raketLarge);
		tecBumper3->bumperHgfx(raketLarge);
		tecBumper4->bumperVgfx(raketLarge);
		joueurGere->setLargeur(raketLarge);
	}
}

//------------------------------------------------------------------------------
// bricks levels: decrease bumpers's size
//------------------------------------------------------------------------------
void zeRaquette::decremente()
{	
	if(raketLarge > (16 * resolution))
	{	raketLarge -= (8 * resolution);
		tecBumper1->bumperHgfx(raketLarge);
		tecBumper2->bumperVgfx(raketLarge);
		tecBumper3->bumperHgfx(raketLarge);
		tecBumper4->bumperVgfx(raketLarge);
		joueurGere->setLargeur(raketLarge);
	}
}

//------------------------------------------------------------------------------
// initialize bumper reverse
//------------------------------------------------------------------------------
void zeRaquette::initinvers(Sint32 value)
{
	rak_invers = value ;
}

//------------------------------------------------------------------------------
// return bumper reverse value
//------------------------------------------------------------------------------
Sint32 zeRaquette::get_invers()
{
  return rak_invers;
}

//------------------------------------------------------------------------------
// disable all bumpers (game over)
//------------------------------------------------------------------------------
void zeRaquette::bumpersOff()
{
	if (tecBumper1)
		tecBumper1->disable();
	if (tecBumper2)
		tecBumper2->disable();
	if (tecBumper3)
		tecBumper3->disable();
	if (tecBumper4)
		tecBumper4->disable();
	if (tec_robot0)
		tec_robot0->disable();
}


Sint32 zeRaquette::ballePets1[14] =
{	4, 8, 12, 16, 20, 24, 28,
 	28, 24, 20, 16, 12, 8, 4
};
Sint32 zeRaquette::ballePets2[14] =
{	20, 24, 28, 32, 36, 40, 44,
	44, 40, 36, 32, 28, 24, 20
};
Sint32 zeRaquette::ballePets3[14] =
{	60, 56, 52, 48, 44, 40, 36,
	36, 40, 44, 48, 52, 56, 60
};
Sint32 zeRaquette::ballePets4[14] =
{	12, 8, 4, 0, 60, 56, 52,
	52, 56, 60, 0, 4, 8, 12
};

//      table de rebonds de la balle raquette du bas 
const Sint32 zeRaquette::rb09[16] = { 4, 4, 8, 12, 16, 20, 24, 28, 28, 32, 32, 32, 32, 20, 24, 28 };
const Sint32 zeRaquette::rb10[16] = { 4, 4, 8, 12, 16, 20, 24, 28, 28, 32, 32, 32, 28, 20, 20, 16 };
const Sint32 zeRaquette::rb11[16] = { 4, 4, 8, 12, 16, 20, 24, 28, 28, 28, 28, 28, 24, 16, 16, 12 };
const Sint32 zeRaquette::rb12[16] = { 4, 4, 8, 12, 16, 20, 24, 28, 28, 28, 28, 24, 20, 16, 12,  8 };
const Sint32 zeRaquette::rb13[16] = { 4, 4, 8, 12, 16, 20, 24, 28, 28, 28, 24, 20, 16, 12,  8,  4 };
const Sint32 zeRaquette::rb14[16] = { 4, 4, 8, 12, 16, 20, 24, 28, 28, 20, 20, 20, 12,  8,  4,  4 };
const Sint32 zeRaquette::rb15[16] = { 4, 4, 8, 12, 16, 20, 24, 28, 28, 16, 16, 16,  8,  8,  4,  0 };
const Sint32 zeRaquette::rb16[16] = { 4, 4, 8, 12, 16, 20, 24, 28, 28,  8, 12, 12,  4,  4,  0,  0 };
const Sint32 zeRaquette::rb17[16] = { 4, 4, 8, 12, 16, 20, 24, 28, 28,  4,  8, 12,  0,  0,  0,  0 };
const Sint32 *zeRaquette::midi1_left[] = { rb13, rb14, rb14, rb15, rb15, rb16, rb16, rb16, rb16, rb16, rb17 };
const Sint32 *zeRaquette::midi1Right[] = { rb13, rb12, rb12, rb11, rb11, rb10, rb10, rb10, rb10, rb10, rb09 };

// table de rebonds de la balle raquette de droite 
const Sint32 zeRaquette::rb18[] =
{ 32 + 16, 20 + 16, 24 + 16, 28 + 16, 4 + 16, 4 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 32 + 16, 32 + 16, 32 + 16
};
const Sint32 zeRaquette::rb19[] =
{ 28 + 16, 20 + 16, 20 + 16, 16 + 16, 4 + 16, 04 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 32 + 16, 32 + 16, 32 + 16
};
const Sint32 zeRaquette::rb20[] =
{ 24 + 16, 16 + 16, 16 + 16, 12 + 16, 4 + 16, 4 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 28 + 16, 28 + 16, 28 + 16
};
const Sint32 zeRaquette::rb21[] =
{ 20 + 16, 16 + 16, 12 + 16, 8 + 16, 4 + 16, 04 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 28 + 16, 28 + 16, 24 + 16
};
const Sint32 zeRaquette::rb22[] =
{ 16 + 16, 12 + 16, 8 + 16, 4 + 16, 4 + 16, 4 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 28 + 16, 24 + 16, 20 + 16
};
const Sint32 zeRaquette::rb23[] =
{ 12 + 16, 8 + 16, 4 + 16, 4 + 16, 4 + 16, 4 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 20 + 16, 20 + 16, 20 + 16
};
const Sint32 zeRaquette::rb24[] =
{ 8 + 16, 8 + 16, 4 + 16, 0 + 16, 4 + 16, 4 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 16 + 16, 16 + 16, 16 + 16
};
const Sint32 zeRaquette::rb25[] =
  { 4 + 16, 4 + 16, 0 + 16, 0 + 16, 4 + 16, 4 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 8 + 16, 12 + 16, 12 + 16
};
const Sint32 zeRaquette::rb26[] =
{ 0 + 16, 0 + 16, 0 + 16, 0 + 16, 4 + 16, 4 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 4 + 16, 8 + 16, 12 + 16
};
const Sint32 *zeRaquette::midi2_left[] =
{ rb22, rb21, rb21, rb20, rb20, rb19, rb19, rb19, rb19, rb19, rb18 };
const Sint32 *zeRaquette::midi2Right[] =
{ rb22, rb23, rb23, rb24, rb24, rb25, rb25, rb25, rb25, rb25, rb26 };


// table de rebonds de la balle raquette du haut 
const Sint32 zeRaquette::rb27[] =
{ 60, 0, 0, 0, 0, 52, 56, 60,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32 zeRaquette::rb28[] =
{ 60, 0, 0, 0, 60, 52, 52, 48,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32 zeRaquette::rb29[] =
{ 60, 60, 60, 60, 56, 48, 48, 44,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32 zeRaquette::rb30[] =
{ 60, 60, 60, 56, 52, 48, 48, 40,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32 zeRaquette::rb31[] =
{ 60, 60, 56, 52, 48, 44, 40, 36,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32 zeRaquette::rb32[] =
{ 60, 52, 52, 48, 44, 40, 36, 36,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32 zeRaquette::rb33[] =
{ 60, 48, 48, 16 + 32, 40, 40, 32, 32,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32 zeRaquette::rb34[] =
{ 60, 40, 44, 12 + 32, 36, 36, 32, 32,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32 zeRaquette::rb35[] =
{ 60, 36, 40, 12 + 32, 32, 32, 32, 32,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32 *zeRaquette::midi3_left[] = { rb31, rb30, rb30, rb29, rb29, rb28, rb28, rb28, rb28, rb28, rb27 };
const Sint32 *zeRaquette::midi3Right[] = { rb31, rb32, rb32, rb33, rb33, rb34, rb34, rb34, rb34, rb34, rb35 };


// table de rebonds de la balle raquette de gauche 
const Sint32 zeRaquette::rb36[] =
{ 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 32 - 16, 32 - 16, 32 - 16,
  32 - 16, 20 - 16, 24 - 16, 28 - 16, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32 zeRaquette::rb37[] =
{ 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 32 - 16, 32 - 16, 32 - 16,
  28 - 16, 20 - 16, 20 - 16, 16 - 16, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32 zeRaquette::rb38[] =
{ 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 28 - 16, 28 - 16, 28 - 16,
  24 - 16, 16 - 16, 16 - 16, 12 + 48, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32 zeRaquette::rb39[] =
{ 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 28 - 16, 28 - 16, 24 - 16,
  20 - 16, 16 - 16, 12 + 48, 8 + 48, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32 zeRaquette::rb40[] =
{ 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 28 - 16, 24 - 16, 20 - 16,
  16 - 16, 12 + 48, 8 + 48, 4 + 48, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32 zeRaquette::rb41[] =
{ 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 20 - 16, 20 - 16, 20 - 16,
  12 + 48, 8 + 48, 4 + 48, 4 + 48, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32 zeRaquette::rb42[] =
{ 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 16 - 16, 16 - 16, 16 - 16,
  8 + 48, 8 + 48, 4 + 48, 0 + 48, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32 zeRaquette::rb43[] =
{ 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 8 + 48, 12 + 48, 12 + 48,
  4 + 48, 4 + 48, 0 + 48, 0 + 48, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32 zeRaquette::rb44[] =
{ 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 4 + 48, 8 + 48, 12 + 48,
  0 + 48, 0 + 48, 0 + 48, 0 + 48, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32 *zeRaquette::midi4_left[] = { rb40, rb41, rb41, rb42, rb42, rb43, rb43, rb43, rb43, rb43, rb44 };
const Sint32 *zeRaquette::midi4Right[] = { rb40, rb39, rb39, rb38, rb38, rb37, rb37, rb37, rb37, rb37, rb36 };
