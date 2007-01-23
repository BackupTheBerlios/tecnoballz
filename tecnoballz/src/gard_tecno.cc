//******************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "gard_techno.cc"
// created	: 2003-01-09
// updates	: 2005-01-18
// fonction	: support the guards levels
// id		: $Id: gard_tecno.cc,v 1.12 2007/01/23 10:11:22 gurumeditation Exp $
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
//******************************************************************************
#include "../include/gard_tecno.h"
#include "../include/ressources.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
gard_tecno::gard_tecno()
{
	initialise();
	defilement = new lastScroll();
	ptguardian = new zeguardian();
	ptRaquette = new zeRaquette(BOB_BUMPER);
	ptMoveText = new zeMoveText();
	pExplosion = new zexplosion();
	tecno_bump *pBump = ptRaquette->demandeRak(1);
	ptMissiles = new zeMissiles(pBump, pExplosion);
	ptCapsules = new zeCapsules();
	pt_gadgets = new ze_gadgets(6);
	ptNewBalls = new zeNewBalls(ptguardian, ptCapsules,  pt_gadgets);
	ptBaDirect = new ballDirect();
	ptPrntmney = new printmoney();
	ptMiniMess = new zeMiniMess();
	ptGigaBlit = new zeGigaBlit();
	ptBobMoney = new sprite_object();
	ptBobLifes = new tecno_gads();
	ptGameOver = new zeGameOver();
	ptCongBall = new zeCongBall();
	ptrEscMenu = new escapeMenu();
	ptBob_name = NULL;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
gard_tecno::~gard_tecno()
{
	if(ptBob_name)
		delete ptBob_name;
	delete ptrEscMenu;
	delete ptCongBall;
	delete ptGameOver;
	delete ptBobLifes;
	delete ptBobMoney;
	delete ptGigaBlit;
	delete ptMiniMess;
	delete ptPrntmney;
	delete ptBaDirect;
	delete ptNewBalls;
	delete pt_gadgets;
	delete ptCapsules;
	delete ptMissiles;
	delete pExplosion;
	delete ptMoveText;
	delete ptRaquette;
	delete ptguardian;
	delete defilement;
	liberation();
}

// -----------------------------------------------------------------------------
// initializations of the guards's level
// -----------------------------------------------------------------------------
Sint32 gard_tecno::first_init()
{
#ifndef SOUNDISOFF
	audio->play_music(MUSICGUARD);
	audio->startSound();
#endif
	sprites->reset();
	end_return = 0;
	isgameover = 0; 
	count_next = 0;
	tecnwinner = 0;
	areaNumber = joueurGere->getAreaNum();
	levelTecno = joueurGere->getLevelNu();
	Sint32 grdP =  joueurGere->getGuardPt();
	//levelTecno = 6; //test only
	if(is_verbose)
		printf("gard_tecno::first_init() : areaNumber=%i, "
			"levelTecno=%i grdP=%i\n",
			areaNumber, levelTecno, grdP);
    
  	//###################################################################
	// initialize gigablitz
  	//###################################################################
	error_init(ptGigaBlit->init_liste(ptRaquette, pExplosion));
	if(erreur_num) return erreur_num; 

  	//###################################################################
	// intialize the sprites objects lists
  	//###################################################################
	error_init(pRessource->loadSprite());
	if(erreur_num)
		return (erreur_num);
	error_init(ptMissiles->init_liste());
	if(erreur_num)
		return (erreur_num);
	error_init(ptguardian->init_liste(ptMissiles, grdP, ptGigaBlit, pExplosion));
	if(erreur_num)
		return (erreur_num);
	error_init(ptRaquette->init_liste());
	if(erreur_num)
		return (erreur_num);
	error_init(ptNewBalls->init_liste());
	if(erreur_num)
		return (erreur_num);
	error_init(ptCapsules->init_liste());
	if(erreur_num)
		return (erreur_num);
	error_init(pt_gadgets->init_liste());
	if(erreur_num)
		return (erreur_num);
	error_init(ptPrntmney->init_liste());
	if(erreur_num)
		return (erreur_num);
	error_init(pExplosion->init_liste());
	if(erreur_num)
		return (erreur_num);

	// Initialize money sprite
	error_init(ptBobMoney->create_sprite(BOB_MONEYS, image_BOBs, 0));
	if(erreur_num)
		return (erreur_num);
	sprites->add(ptBobMoney);
	
	// Initialize extra life sprite
	error_init(ptBobLifes->create_sprite(BOB_GADGET, image_BOBs, 0));
	if(erreur_num)
		return (erreur_num);
	sprites->add(ptBobLifes);

	// initialize
	error_init(ptBaDirect->init_liste());
	if(erreur_num) return erreur_num; 
	
	//mobile characters at the end of the level
	error_init(ptMoveText->init_liste());
	if(erreur_num) return (erreur_num);

	error_init(ptGameOver->init_liste());
	if(erreur_num)
		return (erreur_num); 

	error_init(ptCongBall->init_liste());
	if(erreur_num)
		return (erreur_num); 

	// intialize escape menu
	error_init(ptrEscMenu->first_init
	(	image_BOBs,
		0,			//menu number
		320 * resolution,	//width of screen (center)
		0,			//don't restaure background where leaves
		1			//initialize color table
	));
	if(erreur_num) return (erreur_num); 

	pRessource->freeSprite();
	
	//###################################################################
	// initialize "Game Over"
	//###################################################################
	error_init(ptGameOver->first_init(32 * resolution));
	if(erreur_num) return (erreur_num);
	init_level();
	display->verouiller();
	
  	//###################################################################
  	// initialize background vertical scrolling
	//###################################################################
	defilement->initialise();

  	//###################################################################
	// initialization balls
  	//###################################################################
	ptNewBalls->init_balle(ptRaquette,
		levelParam->startCount,		//time before ball leaves bumper
		0,				//(not applicable)
		50 * 99,			//time before ball accelerates
		levelParam->tilt_count,		//time before "tilt"  available
		levelParam->speedBall1);	//speed ball (3 or 4)
	
   	//###################################################################
   	// force "powerball 2" (guards levels only)
	//###################################################################
	ptNewBalls->run_power2();

   	//###################################################################
	// Initialize the object which handles the "capsules of money"
  	//###################################################################
	ptCapsules->initialise(3 + hardChoice, ptPrntmney);
  
  	//###################################################################
	// Initialize the object which handles gadgets (bonus and malus) 
  	//###################################################################
	pt_gadgets->initialise(
		levelParam->malusCount * hardChoice, 	//frequency of appearance of malus 
		0,				//number of bonus bought in the shop (not applicable)	
		0,				//number of bricks which it is necessary to break (not applicable)
		levelParam->malusListe,		//the list of malus
		NULL,				//the list of bonus bought in the shop (not applicable)
		NULL,				//object which displays the small messages (not applicable)
		ptRaquette,			//object which handles the bumpers
		ptNewBalls,			//object which handles the balls
		NULL,				//object which handles the display of the text (not applicable)
		NULL,
		NULL);
  
  	//###################################################################
  	//
	//###################################################################
	ptPrntmney->init_guard(joueurGere, ptRaquette, ptBobMoney, ptBobLifes);

	//initialize mobile characters at the end of the level
	ptMoveText->initialise(levelTecno, 32 * resolution);
	
	error_init(ptBaDirect->initialize(ptRaquette, 1));
	if(erreur_num) return (erreur_num);

	display->deverouill();
	display->bufferCopy();	//copy "buffer memory" to "screen memory"
	keyboard->clear_command_keys();
	keyboard->set_grab_input (true);
	
	score_over* pOver = ptGameOver->gtScorOver();
	ptBob_name = pOver->string2bob(joueurGere->returnName());
	sprites->add(ptBob_name);
	ptBob_name->enable();
	ptBob_name->coordonnee((display->get_width() - ptBob_name->getLargeur()) / 2, resolution);
	ptBob_name->set_method(sprite_object::METHOD_MSK);
	return erreur_num;
}

//------------------------------------------------------------------------------
// main loop of the guards's phase
//------------------------------------------------------------------------------
Sint32 gard_tecno::zeMainLoop()
{
	Sint32 Ecode = -1; 
	//###################################################################	
	// gameover : the player has no more lives
	//###################################################################	
	if(joueurGere->getLifeNum() <= 0)
	{	if(!isgameover)
		{	
#ifndef SOUNDISOFF
			audio->stop_sound();
#endif
			ptRaquette->bumpersOff();
			pt_gadgets->disable_sprites();
			ptCapsules->disable_sprites();
			ptguardian->disable_sprites();
			pExplosion->disable_sprites();
			ptGigaBlit->disable_sprites();
			ptNewBalls->disable_sprites();
			ptMissiles->disable_sprites();
			if(tecnwinner)
			{	defilement->swapScroll(2, ressources::RESEDMAP02);
				ptCongBall->initialize(); //congra
				scrolSpeed = 1;
				scrollTemp = 300;
			}
		}
		isgameover++;
		display->wait_frame();
		display->verouiller();

		if (scrollTemp > 0)
		{	scrollTemp--;
			defilement->scrolling1(0);
		}
		else
			defilement->scrolling1(scrolSpeed);
		
		if(isgameover >= 1)
		{	ptGameOver->execution1(tecnwinner);
			if(tecnwinner)
				ptCongBall->execution1(); //congra
		}
		ptBaDirect->execution1();	//handle ball viewfinder
		ptPrntmney->execution2(joueurGere->creditFric, joueurGere->superLifes);
		ptMoveText->goMoveText();
		sprites->draw();
		display->deverouill();
		display->bufferCTab();
		if(keyboard->is_left_button() && isgameover > 150)
		{	joueurGere = joueurData::nextplayer(
				joueurGere,
				&end_return,
				1, ptguardian->totalObjet() + 1);
		}
	}
	else
	{
		//######################################################
		// game running !
		//######################################################
		display->wait_frame();
		display->verouiller();
		if(!keyboard->command_is_pressed(handler_keyboard::COMMAND_KEY_PAUSE))
		{	run_scroll();
			defilement->scrolling1(scrolSpeed);
			ptRaquette->bp_deplac2();
			ptRaquette->lacheBall2();
			ptNewBalls->vitusBall2();	//moving ball(s)
			ptBaDirect->execution1();	//handle ball viewfinder
			ptguardian->execution1();	//moving the guard(s)
			ptMissiles->execution1();	//moving the guards's weapons
			ptMissiles->bumper_col();	//collision weapons with the bumper
			ptCapsules->bougefric2();
			pt_gadgets->bougegads2();
			ptMoveText->goMoveText();
			ptPrntmney->execution2(joueurGere->creditFric, joueurGere->superLifes);
			ptGigaBlit->execution2();	//move the Gigablitz from guards
			pExplosion->execution1();	//explosion animations
			ptMissiles->anim_fires();	//the animation of the guards's weapons
	
		}
		else
		{	ptMissiles->anim_fires();
			defilement->scrolling1(0);
		}
	
		//###################################################################
		// display all sprites in the buffer's memory
		//###################################################################
		sprites->draw();
		Ecode = ptrEscMenu->execution1();
		display->deverouill();
		display->bufferCTab();	//copy buffer's memory in the screen
	}
	

	//##############################################################
	// next levels
	//##############################################################
	if(ptguardian->next_level() && !tecnwinner)
	{	if(count_next > 0)
 		{	count_next++;
			ptGigaBlit->disable_sprites();
			ptNewBalls->disable_sprites();
			ptMissiles->disable_sprites();
			if(count_next > 500 ||
				keyboard->key_is_pressed(SDLK_SPACE))
			{
				tecnwinner = joueurGere->zlastlevel();
				if(tecnwinner)
				{	joueurGere->lifesReset();
					count_next = 0;
				}
				else
				{	joueurGere = joueurData::nextplayer
					(	joueurGere, 
						&end_return,
						1,
						ptguardian->totalObjet() + 1);
				}
			}
		}
		else
		{	ptMoveText->activeText();
			ptGigaBlit->disable_sprites();
			ptNewBalls->disable_sprites();
			ptMissiles->disable_sprites();
			count_next = 1;
		}
	}
	
	//###################################################################
	// cheat mode: F2=destroyed the two guards / F3=guard 1 / F4=guard 2 
	//###################################################################
	cheat_keys();
	
	//###################################################################
	// escape key to quit the game !
	//###################################################################
	if(keyboard->command_is_pressed(handler_keyboard::TOEXITFLAG) ||
		Ecode == escapeMenu::WEQUITGAME)
		end_return = -1;
	if(keyboard->command_is_pressed(handler_keyboard::TOOVERFLAG) ||
		Ecode == escapeMenu::GOGAMEOVER)
		joueurGere->lifesReset();
	if(keyboard->command_is_pressed(handler_keyboard::TOMENUFLAG) ||
		Ecode == escapeMenu::EXITTOMENU)
		end_return = 4;

	

	return (end_return);
}

//------------------------------------------------------------------------------
// Calcul le pointeur sur le niveau de la diffculte des gardiens
//------------------------------------------------------------------------------
void gard_tecno::init_level()
{ 
	levelParam = ptLev_data->guardlevel(areaNumber, levelTecno);
	scrollType = levelParam->scrollType;
	scrollTemp = levelParam->scrolCount;
	scrolSpeed = 0;
}

//------------------------------------------------------------------------------
// manage scrolling speed
//------------------------------------------------------------------------------
void gard_tecno::run_scroll()
{	
	if (scrollTemp > 0)
	{	scrollTemp--;
		scrolSpeed = 0;
		return;
	} 
	tecno_miss *weapo = ptMissiles->getWeapOne();
	sprite_ball *balle = ptNewBalls->first_ball();
	scrolSpeed = ptguardian->run_scroll(scrollType, scrolSpeed, balle, weapo);
}

//------------------------------------------------------------------------------
// cheat mode: F2=destroyed the two guards / F3=guard 1 / F4=guard 2 
//------------------------------------------------------------------------------
void gard_tecno::cheat_keys()
{
	if (!cheat_flag) return;
	if(	!keyboard->key_is_pressed(SDLK_RSHIFT) ||
		keyboard->key_is_pressed(SDLK_LSHIFT) ||
		!keyboard->key_is_pressed(SDLK_RCTRL) ||
		keyboard->key_is_pressed(SDLK_LCTRL) ||
		keyboard->key_is_pressed(SDLK_RALT) ||
		keyboard->key_is_pressed(SDLK_LALT))
		return;

	if(keyboard->key_is_pressed(SDLK_F2))
		ptguardian->killguards();
	if(keyboard->key_is_pressed(SDLK_F3))
		ptguardian->killguards(1);
	if(keyboard->key_is_pressed(SDLK_F4))
		ptguardian->killguards(2);
}
