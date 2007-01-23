//*****************************************************************************
// copyright (c) 1991-2006 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "tableaux_Z.cc"
// created	: ?
// updates	: 2006-10-02
// fonctions	: manage bricks levels
// id		: $Id: tableaux_Z.cc,v 1.14 2007/01/23 10:11:22 gurumeditation Exp $
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
#include "../include/tableaux_Z.h"
#include "../include/ressources.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
tableaux_Z::tableaux_Z()
{
	initialise();
	gereBricot = new briqueCote();
	ecranfond4 = new fond_ecran();
	tecZ_barre = new barreScore();
	gereEjects = new ejectBalls();
	gereCapsul = new zeCapsules();
	gereGadget = new ze_gadgets(6);
	ptGemstone = new zeGemstone();
	briquesTab = new lesBriques();
	tete_gugus = new head_anima();
	les_atomes = new zeBouiBoui(gereCapsul, gereGadget, ptGemstone, briquesTab);
	pt_magneye = new ze_magneye();
	BottomWall = new sprite_object(); 
	ptMiniMess = new zeMiniMess();
	gereBalles = new zeNewBalls(gereEjects, briquesTab, gereBricot, tete_gugus,
					les_atomes, tecZ_barre, BottomWall,
					ptMiniMess, pt_magneye);
	ptBaDirect = new ballDirect();
	theBumpers = new zeRaquette();
	gere_texte = new zeMoveText();
	ptGigaBlit = new zeGigaBlit();
	ptPrntmney = new printmoney();
	ptGameOver = new zeGameOver();

	ptBobMoney = new sprite_object();
	ptBobRever = new tecno_gads();
	ptrEscMenu = new escapeMenu();
	
	tecno_fire::start_list(briquesTab, les_atomes, tecZ_barre);
	levelTecno = 1;
	areaNumber = 1;
	next_level = 0;
	isgameover = 0;
	devel_keyw = 0;
	devel_keyx = 0;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
tableaux_Z::~tableaux_Z()
{
	if (BottomWall)
	{	BottomWall->BOBdestroy();
		BottomWall = (sprite_object*)NULL;
	}
	delete ptrEscMenu;
	delete ptBobRever;
	delete ptBobMoney;
	delete ptGameOver;
	delete ptPrntmney;
	delete ptGigaBlit;
	delete gere_texte;
	delete theBumpers;
	delete ptBaDirect;
	delete gereBalles;
	delete ptMiniMess;
	delete BottomWall;
	delete pt_magneye;
	delete les_atomes;
	delete tete_gugus;
	delete briquesTab;
	delete ptGemstone;
	delete gereGadget;
	delete gereCapsul;
	delete gereEjects;
	delete tecZ_barre;
	delete ecranfond4;
	delete gereBricot;
	liberation();
}

//-----------------------------------------------------------------------------
// initialization of a bricks level
//-----------------------------------------------------------------------------
Sint32 tableaux_Z::first_init()
{
	sprites->reset();
	areaNumber = joueurGere->getAreaNum();
	levelTecno = joueurGere->getLevelNu();
	//levelTecno = 5; //test only
#ifndef SOUNDISOFF
	audio->play_level_music (areaNumber, levelTecno);
	audio->startSound();
#endif
	
	count_next = 0;
	end_return = 0;
	isgameover = 0;
	if(is_verbose)
		fprintf(stdout,
			"tableaux_Z::first_init() areaNumber=%i "
			"levelTecno=%i hardChoice=%i \n",
			areaNumber, levelTecno, hardChoice);

	//###################################################################
	// generation of bumpers graphics shapes tables
	//###################################################################
	error_init(theBumpers->init_liste());
	if(erreur_num) return erreur_num;

	//###################################################################
	// generation of gigablitz graphics shapes tables
	//###################################################################
	error_init(ptGigaBlit->init_liste(theBumpers, tete_gugus, briquesTab));
	if(erreur_num) return(erreur_num);

	//###################################################################
	// load page of sprites in memory (all other sprites)
	//###################################################################
	error_init(pRessource->loadSprite());
	if(erreur_num) return (erreur_num);

	//###################################################################
	// generation of graphics shapes tables
	//###################################################################

	//wall of bottom 
	error_init(BottomWall->create_sprite(BOB_WALLBO, image_BOBs, 0));
	if(erreur_num) return erreur_num;
	sprites->add(BottomWall);
	BottomWall->coordonnee(32 * resolution, 232 * resolution);
	//robot bumper
	error_init(theBumpers->init_robot());
	if(erreur_num) return erreur_num;
	Sint32 build = joueurGere->getRebuild();
	joueurGere->setRebuild(0);
	error_init(gereBricot->initialise(build));
	if(erreur_num) return erreur_num;
	//ejectors 
	error_init(gereEjects->initialise());
	if(erreur_num)return erreur_num;
	//all balls
	error_init(gereBalles->init_liste());
	if(erreur_num) return erreur_num;
	//atoms (aka "bouisbouis")
	error_init(les_atomes->init_liste());
	if(erreur_num) return erreur_num;
	//eye magneto 
	error_init(pt_magneye->initialize());
	if(erreur_num) return erreur_num;
	//capsules of money
	error_init(gereCapsul->init_liste());
	if(erreur_num) return erreur_num;
	//gadgets (bonuses and maluses)
	error_init(gereGadget->init_liste());
	if(erreur_num) return erreur_num;
	//gems stones
	error_init(ptGemstone->init_liste());
	if(erreur_num) return erreur_num;
	//mobiles characters
	error_init(gere_texte->init_liste());
	if(erreur_num) return erreur_num;
	//weapons's bumpers
	error_init(theBumpers->init_fires());
	if(erreur_num) return erreur_num;
	//credits value (left-bottom)
	error_init(ptPrntmney->init_liste());
	if(erreur_num) return erreur_num;
	//GAME OVER sprites
	error_init(ptGameOver->init_liste());
	if(erreur_num) return erreur_num; 
	//money sprite (left-bottom)
	error_init(ptBobMoney->create_sprite(BOB_MONEYS, image_BOBs, 0));
	if(erreur_num) return erreur_num;
	sprites->add(ptBobMoney);
	//reverser sprite (right-bottom)
	error_init(ptBobRever->create_sprite(BOB_GADGET, image_BOBs, 0));
	if(erreur_num) return erreur_num;
	sprites->add(ptBobRever);
	//bumper's viewfinder
	error_init(ptBaDirect->init_liste());
	if(erreur_num) return erreur_num; 
	//ESC menu
	error_init(ptrEscMenu->first_init(image_BOBs, 0, 256 * resolution));
	if(erreur_num) return erreur_num; 
	pRessource->freeSprite();
	display->verouiller();
	
	//###################################################################
	// initialize "Game Over"
	//###################################################################
	error_init(ptGameOver->first_init());
	if(erreur_num) return (erreur_num);

	//###################################################################
	// initialize the head animation
	//###################################################################
	error_init(tete_gugus->initialise());
	if(erreur_num)
		return (erreur_num);
	
	init_level();


	//###################################################################
	// initialize left scores pannel
	//###################################################################
	error_init(tecZ_barre->first_init(joueurGere, ptGigaBlit, gereBalles));
	if(erreur_num) return erreur_num;
	error_init(tecZ_barre->affiche_me());
	if(erreur_num) return erreur_num;
	
	background();
	theBumpers->initBumper(tecZ_barre, ptGigaBlit, gereBalles);

	//##############################################################
	// balls initialization
	//##############################################################
	gereBalles->init_balle(theBumpers,
		//time before the ball leaves bumper (at the game beginning) 
		levelParam->startCount,
		//time before the ball leaves (glue option)
		levelParam->glue_count / hardChoice,
		//time before the ball accelerates
		levelParam->speedCount / hardChoice,
		//time before "tilt" is available
		levelParam->tilt_count,		
		levelParam->speedBall1);

	//##############################################################
	// initialization "atoms" (aka "bouibouis") 
	//##############################################################
	les_atomes->initialise(levelParam->apparition / hardChoice,
		levelParam->atom1Count / hardChoice,
		levelParam->atom2Count / hardChoice,
		levelParam->atom3Count / hardChoice,
		levelParam->atom4Count / hardChoice,
		levelParam->resistance * hardChoice);

	//##############################################################
	//initialize capsules of money
	//##############################################################
	gereCapsul->initialise(
			levelParam->monayCount * hardChoice,
			tecZ_barre, ptPrntmney);

	//##############################################################
	//Initialize the object which handles gadgets (bonus and malus)
	//##############################################################
	Sint32 *cours = joueurGere->get_course();
	Sint32 counb = joueurGere->get_cou_nb();
	Sint32 brCnt = briquesTab->getbrikCnt();
	gereGadget->initialise(
		//frequency of appearance of malus 
		levelParam->malusCount * hardChoice,
		//number of bonus bought in the shop
		counb,
		//number of bricks which it is necessary to break
		brCnt,
		//the list of malus
		levelParam->malusListe,
		//the list of bonus bought in the shop
		cours,	
		//the object which displays the small messages
		ptMiniMess,
		//the object which handles the bumpers
		theBumpers,
		//the object which handles the balls
		gereBalles,
		//the object which handles the text on left scores panel
		tecZ_barre,
		BottomWall,
		pt_magneye);

	//##############################################################
	//initialize the gems tones
	//##############################################################
	error_init(ptGemstone->initialise(
			joueurGere,
			tecZ_barre,
			ptPrntmney,
			theBumpers));
	if(erreur_num) return erreur_num;

	//##############################################################
	// initialize mobiles characters ("LEVEL x COMPLETED")
	//##############################################################
	gere_texte->initialise(levelTecno);

	ptPrntmney->initialise(joueurGere, theBumpers, ptBobMoney, ptBobRever);

	
	error_init(ptBaDirect->initialize(theBumpers, 4));
	if(erreur_num) return (erreur_num);
	
	display->tamponBuff();	// recopie le tampon dans le buffer
	display->deverouill();
	Sint32 k = memory->get_total_size();
	if(is_verbose)
		printf("tableaux_Z::first_init(): memory size allocated : %i \n",k);
	keyboard->clear_command_keys();
	keyboard->set_grab_input (true);
	ptMiniMess->mesrequest(1);
	return erreur_num;
}

//-------------------------------------------------------------------------------
// bricks levels : reads the parameters of the current level
//-------------------------------------------------------------------------------
void tableaux_Z::init_level()
{ 
	levelParam = ptLev_data->bricklevel(areaNumber, levelTecno);
}

//------------------------------------------------------------------------------
// bricks levels : main loop of the game
//------------------------------------------------------------------------------
Sint32 tableaux_Z::zeMainLoop()
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
			audio->stopModule();
#endif
			theBumpers->bumpersOff();
			briquesTab->clr_bricks();
			gereGadget->disable_sprites();
			gereCapsul->disable_sprites();
			gereBalles->disable_sprites();
			tecno_fire::disable_sprites();
			ptMiniMess->erase_mess();
			isgameover++;
		}
		ptMiniMess->execution1();
		display->wait_frame();
		display->verouiller();
		tete_gugus->execution1();
		ptGigaBlit->execution1();
		sprites->clear();
		if(!(hasard_val & 0x00f))
			tete_gugus->teteparasi();
		if(isgameover >= 2) {
			isgameover++;
			ptGameOver->execution1();
		}
		if (!briquesTab->brickRemap() && isgameover < 2)	//restore bricks
			isgameover = 2;
		gereBricot->execution1();
		ptBaDirect->execution1();	//handle ball viewfinder
		les_atomes->atom_depla();
		sprites->draw();
		tecZ_barre->scoreEcran();
		tecZ_barre->barreTemoin();
		ptPrntmney->execution1(joueurGere->creditFric);
		display->deverouill();
		display->bufferCTab();
		if(keyboard->is_left_button() && isgameover > 60)
			joueurGere = joueurData::nextplayer(joueurGere, &end_return, 1);
	}
	
	//###################################################################	
	// game is running
	//###################################################################	
	else
	{	display->wait_frame();
		display->verouiller();
		sprites->clear();
		briquesTab->brickRemap();	//restore bricks
		gereBricot->execution1();	//restore bricks on side
		changebkgd();

		if(!keyboard->command_is_pressed(handler_keyboard::COMMAND_KEY_PAUSE))
		{	ptMiniMess->execution1();
			tete_gugus->execution1();
			ptGigaBlit->execution1();
			
			//handle the "less bricks" option
			briquesTab->lessbricks();
			
			theBumpers->bp_deplace();	//move bumpers
			if(tecZ_barre->resteBrick())
			{	theBumpers->lacheBalle();
				theBumpers->lache_tirs();
			}
			theBumpers->move_robot();
			gereBalles->vitusBalle();	//move balls
			ptBaDirect->execution1();	//handle ball viewfinder
			tecno_fire::gestionTir();
			les_atomes->atom_depla();
			pt_magneye->execution1();
			gereCapsul->bouge_fric();	//move capsules of money
			gereGadget->bouge_gads();	//move bonuses and maluses
			gereGadget->gadgetKeys();
			ptGemstone->moving_gem();	//move gems
			gere_texte->goMoveText();
			if(BottomWall->thecounter < 1)
				BottomWall->disable();
			else
				BottomWall->thecounter--;
			
			tecZ_barre->barreTemoin();
			ptPrntmney->execution1(joueurGere->creditFric);
		}

		sprites->draw();
		tecZ_barre->scoreEcran();
		Ecode = ptrEscMenu->execution1();
		display->deverouill();
		display->bufferCTab();

		//###################################################################
		// next level or next player
		//###################################################################
		if(!tecZ_barre->resteBrick())
		{	if(count_next > 0)
 			{	count_next++;
				Sint32 endmu = 0;
				if(count_next > 350)
				{	endmu = 1;
#ifndef SOUNDISOFF	
					endmu = audio->winn_isend();
#endif
				}
				if(count_next > 20000000 ||
					keyboard->key_is_pressed(SDLK_SPACE) || endmu)
				{	gereBricot->sauve_etat();
					joueurGere = joueurData::nextplayer(joueurGere,
						&end_return, 1);
#ifndef SOUNDISOFF	
					audio->stopModule();
#endif
				}
				gereBalles->disable_sprites();
				tecno_fire::disable_sprites();
			}
			else
			{	gere_texte->activeText();
				tecno_fire::disable_sprites();
				gereBalles->disable_sprites();
#ifndef SOUNDISOFF	
				audio->play_win_music();
#endif
				ptMiniMess->mesrequest(17);
#ifndef SOUNDISOFF	
				audio->stop_sound();
#endif
				count_next = 1;
			}
		}
	}

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

	//###################################################################
 	// control position music's module
	//###################################################################
#ifndef SOUNDISOFF	
 	Uint32 phase = audio->get_mphase();
	if(phase == PHASE_LOST && 
		phase != audio->get_mphase())
	{	ptMiniMess->mesrequest(2);
		theBumpers->free_balls();
	}
#endif
	return end_return;
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
 void tableaux_Z::changebkgd()
{
#ifdef TU_TRICHES
	if(	keyboard->key_is_pressed(SDLK_RSHIFT) ||
		keyboard->key_is_pressed(SDLK_LSHIFT) ||
		keyboard->key_is_pressed(SDLK_RCTRL) ||
		keyboard->key_is_pressed(SDLK_LCTRL) ||
		keyboard->key_is_pressed(SDLK_RALT) ||
		!keyboard->key_is_pressed(SDLK_LALT))
	return;

	if(keyboard->key_is_pressed(SDLK_w)) devel_keyw = 1;
	if(keyboard->key_is_pressed(SDLK_x)) devel_keyx = 1;
		 
	if((keyboard->key_is_released(SDLK_w) && devel_keyw) ||
		(keyboard->key_is_released(SDLK_x) && devel_keyx))
	{
		display->buffer_RAZ();
		if(devel_keyw)
		{	devel_keyw = 0;
			if(--indexbgrnd < 0) indexbgrnd = 49;
		}
		if(devel_keyx)
		{	devel_keyx = 0;
			if(++indexbgrnd > 49) indexbgrnd = 0;
		}
		if(is_verbose)
			printf("tableaux_Z::background() : changebkgd:%i\n", indexbgrnd);
		background(indexbgrnd);
		display->tamponBuff();
	 }

	if(keyboard->key_is_pressed(SDLK_v))
		tete_gugus->teterigole();
	if(keyboard->key_is_pressed(SDLK_b))
		tete_gugus->tetebaille();
	if(keyboard->key_is_pressed(SDLK_n))
		tete_gugus->teteparasi();
	if(keyboard->key_is_pressed(SDLK_g))
		ptGigaBlit->initDepart();
/* 

	if(keyboard->key_is_pressed(SDLK_w))
 { 
 }

 if(keyboard->key_is_pressed(SDLK_w))
 { keyw = 1;
 }


 if(keyboard->key_is_released(SDLK_w) && keyw == 1)
 { ecranfond4->prev_color();
 keyw = 0;
 }
 if(keyboard->key_is_released(SDLK_x) && keyx == 1)
 { ecranfond4->next_color();
 keyx = 0;
 }
*/


#endif
} 

//-------------------------------------------------------------------------------
// initialize background
//-------------------------------------------------------------------------------
Sint32 tableaux_Z::background(Sint32 nbkdg)
{
	if(nbkdg == -1)
	{	nbkdg = ((areaNumber - 1) * 10) + levelTecno;
		if(levelTecno > 5) nbkdg--;
		if(is_verbose)
			printf("tableaux_Z::background() : nbkdg = %i\n",
				nbkdg);
	}

	//###################################################################
	// display background
 	//###################################################################
	error_init(ecranfond4->instalFond(nbkdg));
	if(erreur_num) return erreur_num;
	
 	//###################################################################
	// initialize the mini messages
 	//###################################################################
	error_init(ptMiniMess->intialise1());
	if(erreur_num) return erreur_num;

 	//###################################################################
	// display the ejectors and small bricks
 	//###################################################################
	gereEjects->afficheSha();	//display ejectors shadows
	gereBricot->sauveFond();	//save background under small bricks
	gereBricot->afficheSha();	//display small bricks shadows
	gereBricot->afficheGfx();	//display small bricks of the three walls
	gereEjects->afficheGfx();	//display ejectors

	//###################################################################
	// intialize the bricks level
	//###################################################################
	error_init(briquesTab->first_init(tecZ_barre, gereCapsul, gereGadget));
	if(erreur_num) return erreur_num;
	Sint32 lbrik = joueurGere->get_lessBk();
	joueurGere->set_lessBk(0);
	error_init(briquesTab->initialise(areaNumber, levelTecno, lbrik));
	if(erreur_num) return erreur_num;
	return erreur_num;
}

