//******************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "ze_gadgets.cc"
// created	: ?
// updates	: 2005-01-18
// fonction	: manage gadgets (malus & bonus)
// id		: $Id: ze_gadgets.cc,v 1.8 2007/01/18 17:09:53 gurumeditation Exp $
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
#include "../include/ze_gadgets.h"
#include "../include/handler_display.h"
#include "../include/handler_keyboard.h"
#include "../include/joueurData.h"
#include "../include/tecno_fire.h"
#include "../include/joueurData.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
ze_gadgets::ze_gadgets(Sint32 total, Sint32 vShad)
{
	littleInit();
	Sint16 *monPT;
	monPT = courseList;
	// clear the list of bonuses bought in shop
	for(Sint32 i = 0; i < NB_OPTIONS; i++)
		*(monPT++) = 0;
	*(monPT++) = -1;
	anim_tempo = 0;
	init_tempo = 5;
	animOffset = 0;
	malus_step = 0;
	malus_frek = 0;
	brick_kass = 0;
	bonusAchet = 0;
	bonusTombe = 0;
	bonus_step = 0;
	course_ptr = 0;
	objetTotal = 6;
	objects_have_shades = vShad;
	fTableByte = 1;
	//fTableByte = 0;	//test only
	BOBtypeNum = BOB_GADGET;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
ze_gadgets::~ze_gadgets()
{
	littleDead();
}

//------------------------------------------------------------------------------
// perform some initializations
// input	=> mStep: frequence de chute des malus
//		=> bKauf: number of bonuses bought in shop 
//		=> brCnt: Number of bricks to be destroyed
//		=> table: pointer to the list of "malus"
//		=> cours: pointer to list of bonuses bought in shop
//------------------------------------------------------------------------------
void ze_gadgets::initialise(Sint32 mStep, Sint32 bKauf, Sint32 brCnt, const Sint16 *table,
	Sint32 *cours, zeMiniMess* ptMes, zeRaquette *pRaqu, zeNewBalls* pBall,
	barreScore* pScor, sprite_object *pWall, ze_magneye* ptEye)
{
	ptRaquette = pRaqu;
	ptNewBalls = pBall;
	ptbarreScr = pScor;
	ptBob_wall = pWall;
	pt_magneye = ptEye;
	bumpSelect = ptRaquette->demandeRak(1);
	malus_frek = mStep;
	bonusAchet = bKauf;
	bonus_step = 0;
	malusTable = table;
	malus_step = 0;	//"malus" drop frequency 
	brick_kass = 0;	//number of bricks current destroyed
	bonusTombe = 0;	//number of bonuses dropped 
	ptMiniMess = ptMes;
	tecno_gads *bonus = objetListe[0];
	for(Sint32 i = 0; i < objetTotal; i++)
	{	bonus = objetListe[i];
		bonus->littleInit();
	}
	init_tempo = bonus->init_tempo;
  
	//###################################################################
	// reset list of bonuses bought in the shop
	//###################################################################
	for(Sint32 i=0; i < NB_OPTIONS; i++)
		courseList[i] = 0;
	courseList[NB_OPTIONS] = -1;	//end of the table

	//###################################################################
	// copy list of bonuses bought in the shop
	//###################################################################
	if(cours) 
	{	for(Sint32 i=0; i<bonusAchet ; i++)
			courseList[i] = cours[i];
	}
	else
		courseList[0] = -1;
  
	if(bonusAchet >0)	//at least a bought bonus?
	{	Sint32 v = brCnt - (brCnt / 2);
		bonus_step = v / bonusAchet;
	}
	else
	{
		//no bonus bought in the shop. Initialize with the maximum value.
		bonus_step = brCnt + 1;
	}
}

//-------------------------------------------------------------------------------
// bricks level: test if send a gadget (malus or bonus)
//-------------------------------------------------------------------------------
void ze_gadgets::envoieGads(brickClear * briPT)
{
	brick_kass++;
	for(Sint32 i = 0; i < objetTotal; i++)
	{	tecno_gads *gadg = objetListe[i];
		if(!gadg->flag_actif)
		{	//###########################################################
			// handle maluses
			//###########################################################
			malus_step++;
			if(malus_step > malus_frek)
			{	Sint16 j = hasard_val & 0x3F;	//value 0 to 63 
				j = *(malusTable + j);
				malus_step = 0;
				//j = GAD_MEGA00;	//test only
				gadg->nouveauGad(briPT, j);
			}
			else
			//###########################################################
			// handle bonuses
			//###########################################################
			{	if(bonusAchet > bonusTombe)
				{	if(brick_kass > bonus_step)
					{	brick_kass = 0;
						bonusTombe++;
						Sint32 j = courseList[course_ptr];
						if(!j)
						{	j = courseList[0];
							course_ptr = 0;
						}
						course_ptr++;
						gadg->nouveauGad(briPT, j);
					}
				}
			}
			return;
		}
	}
}

//-------------------------------------------------------------------------------
// bricks levels: send a malus (from BouiBoui)
//-------------------------------------------------------------------------------
void ze_gadgets::send_malus(technoBall *pball)
{
	for(Sint32 i = 0; i < objetTotal; i++)
	{	tecno_gads *gadg = objetListe[i];
		if(!gadg->flag_actif)
		{	Sint16 j = hasard_val & 0x1F;	//value 0 to 31 
			j = *(malusTable + j);
			gadg->nouveauGad(pball, j);
			return;
		}
	}
}
//-------------------------------------------------------------------------------
// bricks levels: send a malus (from BouiBoui)
//-------------------------------------------------------------------------------
void ze_gadgets::send_malus(tecno_fire *pfire)
{
	for(Sint32 i = 0; i < objetTotal; i++)
	{	tecno_gads *gadg = objetListe[i];
		if(!gadg->flag_actif)
		{	Sint16 j = hasard_val & 0x1F;	//value 0 to 31 
			j = *(malusTable + j);
			gadg->nouveauGad(pfire, j);
			return;
		}
	}
}


//-------------------------------------------------------------------------------
// guards level : test if send a gadget (malus or bonus)
//-------------------------------------------------------------------------------
void ze_gadgets::envoieGads(technoBall *pball)
{
	malus_step++;
	if(malus_step <= malus_frek) return;
	for(Sint32 i = 0; i < objetTotal; i++)
	{	tecno_gads *gadg = objetListe[i];
		if(!gadg->flag_actif)
		{	Sint16 j = hasard_val & 0x1F;	//value 0 to 31 
			j = *(malusTable + j);
			malus_step = 0;
			gadg->new_gadget(pball, j);
			return;
		}
	}
}

//-------------------------------------------------------------------------------
// shop : initialize the positions of bonuses
//-------------------------------------------------------------------------------
Sint32 ze_gadgets::gadgetShop()
{
	init_total(NB_OPTIONS + 2);
	error_init(init_liste());
	if(erreur_num)
		return (erreur_num);
	Sint32 t = NB_OPTIONS;
	tecno_gads **liste = objetListe;
	tecno_gads *bonus = *liste;
	Sint32 h = bonus->BOBhauteur + 1;
	Sint32 x = SGADGET_X1 * resolution;
	Sint32 y = SGADGET_Y1 * resolution;
	for(Sint32 i = 0; i < t; i++)
	{	bonus = *(liste++);
		bonus->coordonnee(x, y);
		bonus->disable();
		y += h;
	}
	
	//initialize bonus sprite object for the drag and drop 
	bonus = *(liste++);
	bonus->coordonnee(0, 0);
	bonus->disable();

	//initialize bonus sprite object that display indicator   
	bonus = *liste;
	bonus->coordonnee(SGADGET_X2 * resolution, SGADGET_Y2 * resolution);
	bonus->disable();
	temoin_gad = bonus;
	return (erreur_num);
}

//-------------------------------------------------------------------------------
// shop : change bonus indicator
//-------------------------------------------------------------------------------
void ze_gadgets::gadgetShop(Sint32 nuGad)
{
	temoin_gad->nouveauGad(nuGad);
}

//-------------------------------------------------------------------------------
// shop : initialize the list of bonus 
//-------------------------------------------------------------------------------
void ze_gadgets::gadgetShop(joueurData * gamer)
{
	Sint32 t = NB_OPTIONS;
	tecno_gads **liste = objetListe;
	Sint32 *cours = gamer->get_course();
	for(Sint32 i = 0; i < t; i++)
	{	tecno_gads *bonus = *(liste++);
		Sint32 n = *(cours++);
		bonus->nouveauGad(n);
	}
}

//-------------------------------------------------------------------------------
// bricks levels: move gadgets (bonus or malus) and collision with bumpers
//-------------------------------------------------------------------------------
void ze_gadgets::bouge_gads()
{
	for(Sint32 i = 0; i < objetTotal; i++)
	{	tecno_gads *bonus = objetListe[i];
		bonus->animRepete();
		tecno_bump *raket = bonus->deplaceMoi();
		if(raket)
		{	Sint32 g = bonus->get_gadget();
			gadget_run(raket, g);
		}
	}
}

//-------------------------------------------------------------------------------
// gards levels: move gadgets (bonus or malus) and collision with bumper
//-------------------------------------------------------------------------------
void ze_gadgets::bougegads2()
{
	for(Sint32 i = 0; i < objetTotal; i++)
	{	tecno_gads *bonus = objetListe[i];
		bonus->animRepete();
		tecno_bump *raket = bonus->deplaceMoi();
		if(raket)
		{	Sint32 g = bonus->get_gadget();
			gadgetrun2(raket, g);
		}
	}
}

//-------------------------------------------------------------------------------
// shop: animation of bonus
//-------------------------------------------------------------------------------
void ze_gadgets::animations(Sint32 value)
{
	if((--anim_tempo) <= 0)
	{	anim_tempo = init_tempo / value;
			if(++animOffset >= XXX_IMAGES)
				animOffset = 0;
	}
	Sint32 a = animOffset;
	tecno_gads **liste = objetListe;
	for(Sint32 i = 0; i < objetTotal; i++)
	{	tecno_gads *bonus = *(liste++);
		Sint32 o = bonus->miniOffset + a;
		bonus->change_GFX(o);
	}
}

//-------------------------------------------------------------------------------
// bricks levels: active a bonus or malus with keyboard (cheat mode)
//-------------------------------------------------------------------------------
void ze_gadgets::gadgetKeys()
{
	if (!cheat_flag) return;
	if(	keyboard->key_is_pressed(SDLK_LSHIFT) ||
		keyboard->key_is_pressed(SDLK_LCTRL) ||
		keyboard->key_is_pressed(SDLK_RALT) ||
#ifndef TU_TRICHES
		!keyboard->key_is_pressed(SDLK_RSHIFT) ||
		!keyboard->key_is_pressed(SDLK_RCTRL) ||
#else
		keyboard->key_is_pressed(SDLK_RSHIFT) ||
		keyboard->key_is_pressed(SDLK_RCTRL) ||
#endif
		keyboard->key_is_pressed(SDLK_LALT))
		return;
	Sint16 *liste = keysTriche;
	while (Sint16 k = *(liste++)) 		//k = SDL key code
	{	Sint16 g = *(liste++);		//g = gadget code (bonus or malus)
		if(keyboard->key_is_pressed(k))	//key pressed ?
			*(liste++) = 1;		//yes, set key state pressed
		else
		{	if (*liste)
				gadget_run(bumpSelect, g);
			*(liste++) = 0;
		}
	}
}

//-------------------------------------------------------------------------------
// bricks levels: active a gadget (bonus or malus)
//-------------------------------------------------------------------------------
void ze_gadgets::gadget_run(tecno_bump * raket, Sint32 nuGad)
{
	bumpSelect = raket;
	barreScore *barre = ptbarreScr;
	zeRaquette *oBump = ptRaquette;
	zeNewBalls *oBall = ptNewBalls;

	if(nuGad == GAD_RANDOM)
		nuGad = randomlist[hasard_val & 127];
	
	switch (nuGad)
	{ 
		
	// bumper glue
	case GAD_GLUE00:
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_TRANSFOR);
#endif
		ptMiniMess->mesrequest(16);
		raket->bumpGoGlue();
		break;

	// next level
	case GAD_NEXTLV:
		ptMiniMess->mesrequest(17);
		barre->scoreBrick(0);
		break;

	// fire power 1
	case GAD_FIRE01:
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_TRANSFOR);
#endif
		ptMiniMess->mesrequest(4);
		raket->bump_fire1();
		break;
	
	// fire power 2
	case GAD_FIRE02:
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_TRANSFOR);
#endif
		ptMiniMess->mesrequest(5);
		raket->bump_fire2();
		break;

	// shrink bumper
	case GAD_SIZE_M:
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_TRANSFOR);
#endif
		ptMiniMess->mesrequest(18);
		oBump->decremente();
		break;

	// expand bumper
	case GAD_SIZE_P:
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_TRANSFOR);
#endif
		ptMiniMess->mesrequest(19);
		oBump->incremente();
		break;

	// lose a life
	case GAD_LIFE_M:
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_ENLEVVIE);
#endif
		ptMiniMess->mesrequest(10);
		//barre->lifesMoins(1);
		joueurGere->lifesMoins(1);
		break;

	// extra life
	case GAD_LIFE_P:
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_AJOUTVIE);
#endif
		ptMiniMess->mesrequest(11);
		//barre->lifes_plus(1);
		joueurGere->lifes_plus(1);
		break;

	// extra balls
	case GAD_BALLE2:
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_GADGETGO);
#endif
		ptMiniMess->mesrequest(20);
		oBall->run_2balls();
		break;

	// multi balls
	case GAD_BALLE3:
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_GADGETGO);
#endif
		ptMiniMess->mesrequest(21);
		oBall->run_3balls();
		break;

	// power ball 1
	case GAD_POWER1:
		ptMiniMess->mesrequest(22);
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_GADGETGO);
#endif
		oBall->run_power1();
		break;
	
	// power ball 2
	case GAD_POWER2:
		ptMiniMess->mesrequest(23);
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_GADGETGO);
#endif
		oBall->run_power2();
		break;

	// inverse control
	case GAD_INVERS:
		ptMiniMess->mesrequest(24);
		oBump->initinvers(50*4);
		break;

	// maxi ball speed (no gadget)
	case GAD_SPEEDM:
		ptMiniMess->mesrequest(25);
		oBall->maxi_speed();
		break;

	// bottom bumper[1] enable (no gadget)
	case GAD_BUMP01:
		bumpSelect = oBump->demandeRak(1);
		break;

	// right bumper[2] enable (no gadget)
	case GAD_BUMP02:
		ptMiniMess->mesrequest(26);
		bumpSelect = oBump->demandeRak(2);
		bumpSelect->enable();
		break;

	// top bumper[3] enable (no gadget)
	case GAD_BUMP03:
		ptMiniMess->mesrequest(27);
		bumpSelect = oBump->demandeRak(3);
		bumpSelect->enable();
		break;

	// right bumper[4] enable (no gadget)
	case GAD_BUMP04:
		ptMiniMess->mesrequest(28);
		bumpSelect = oBump->demandeRak(4);
		bumpSelect->enable();
		break;

	// ball size 2
	case GAD_SIZE01:
		ptMiniMess->mesrequest(29);
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_GADGETGO);
#endif
		oBall->run_size01();
		break;

	// ball size 3
	case GAD_SIZE02:
		ptMiniMess->mesrequest(30);
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_GADGETGO);
#endif
		oBall->run_size02();
		break;

	// random
	case GAD_RANDOM:
		break;

	// all options
	case GAD_MEGA00:
		ptMiniMess->mesrequest(31);
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_GADGETGO);
#endif
		oBump->maxi_bumps();
		oBall->run_nballs();
		oBall->run_power2();
		oBall->run_size02();
	
		raket->bumpGoGlue();
		raket->bump_fire2();
	
		bumpSelect = oBump->demandeRak(2);
		bumpSelect->enable();
		bumpSelect->bumpGoGlue();
		bumpSelect->bump_fire2();
	
		bumpSelect = oBump->demandeRak(3);
		bumpSelect->enable();
		bumpSelect->bumpGoGlue();
		bumpSelect->bump_fire2();

		bumpSelect = oBump->demandeRak(4);
		bumpSelect->enable();
		bumpSelect->bumpGoGlue();
		bumpSelect->bump_fire2();
		break;

	// Bonus price (shop's price at 1 in the shop) (no gadget)
	case GAD_PRICE1:
		ptMiniMess->mesrequest(15);
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_GADGETGO);
#endif
		joueurGere->set_Bprice(1);
		break;
	
	// bottom wall enable
	case GAD_WALL01:
		ptMiniMess->mesrequest(32);
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_GADGETGO);
#endif
		ptBob_wall->enable();
		ptBob_wall->thecounter += 500; 
		oBump->deactrobot();
		break;

	// robot bumper enable
	case GAD_ROBOT1:
		ptMiniMess->mesrequest(33);
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_GADGETGO);
#endif
		oBump->activrobot();
		ptBob_wall->disable();
		ptBob_wall->thecounter = 0;
		break;
	
	// balls control 
	case GAD_CONTRO:
		oBall->runcontrol();
		ptMiniMess->mesrequest(34);
#ifndef SOUNDISOFF
		ptAudiomix->sound_play(S_GADGETGO);
#endif
		break;

	// enable eye (no gadget)
	case GAD_OEIL00:
		pt_magneye->create_eye();
		break;
	}
}
//-------------------------------------------------------------------------------
// guards levels: active a gadget (bonus)
//-------------------------------------------------------------------------------
void ze_gadgets::gadgetrun2(tecno_bump *raket, Sint32 nuGad)
{
	bumpSelect = raket;
	zeNewBalls *oBall = ptNewBalls;
	switch (nuGad)
	{
		
		// bumper protect
		case GAD_PROTEC:
			//ptbumper01 = PtRaquette->demandeRak(1);
			raket->setInvncbl(200);
			break;

		// extra life
		case GAD_LIFE_P:
#ifndef SOUNDISOFF
			ptAudiomix->sound_play(S_AJOUTVIE);
#endif
			joueurGere->lifes_plus(1);
			break;
	
		// multi balls
		case GAD_BALLE3:
#ifndef SOUNDISOFF
			ptAudiomix->sound_play(S_GADGETGO);
#endif
			oBall->run_3balls();
			break;

		// power ball 1 (ball size 2)
		case GAD_POWER1:
#ifndef SOUNDISOFF
			ptAudiomix->sound_play(S_GADGETGO);
#endif
			oBall->run_size01();
			break;
	
		// power ball 2 (ball size 3)
		case GAD_POWER2:
#ifndef SOUNDISOFF
			ptAudiomix->sound_play(S_GADGETGO);
#endif
			oBall->run_size02();
			break;
	}
}

//-------------------------------------------------------------------------------
// bricks levels: gadgets list of the random gadget
//-------------------------------------------------------------------------------
const Uint16 ze_gadgets::randomlist[128] =
{	GAD_WALL01,GAD_OEIL00,GAD_FIRE02,GAD_SIZE_M,GAD_ROBOT1,GAD_BALLE2,GAD_BALLE3,
	GAD_POWER1,GAD_CONTRO,GAD_SIZE01,GAD_SIZE02,GAD_OEIL00,GAD_BUMP04,GAD_SPEEDM,
	GAD_SPEEDM,GAD_SPEEDM,GAD_LIFE_M,GAD_CONTRO,GAD_GLUE00,GAD_FIRE01,GAD_FIRE02,
	GAD_SIZE_M,GAD_SIZE_P,GAD_BALLE2,GAD_BALLE3,GAD_POWER1,GAD_SPEEDM,GAD_SIZE01,
	GAD_SIZE02,GAD_SIZE02,GAD_BUMP04,GAD_OEIL00,GAD_NEXTLV,GAD_CONTRO,GAD_LIFE_M,
	GAD_SPEEDM,GAD_ROBOT1,GAD_FIRE01,GAD_FIRE02,GAD_SIZE_M,GAD_WALL01,GAD_BALLE2,
	GAD_ROBOT1,GAD_ROBOT1,GAD_ROBOT1,GAD_SIZE01,GAD_SIZE02,GAD_SPEEDM,GAD_BUMP02,
	GAD_BUMP02,GAD_BUMP03,GAD_INVERS,GAD_LIFE_P,GAD_LIFE_P,GAD_WALL01,GAD_OEIL00,
	GAD_PRICE1,GAD_SIZE_M,GAD_ROBOT1,GAD_BALLE2,GAD_BALLE3,GAD_POWER1,GAD_SPEEDM,
	GAD_SIZE01,GAD_WALL01,GAD_FIRE01,GAD_ROBOT1,GAD_OEIL00,GAD_ROBOT1,GAD_BALLE2,
	GAD_BALLE3,GAD_POWER1,GAD_WALL01,GAD_SIZE01,GAD_SIZE02,GAD_BUMP03,GAD_BUMP04,
	GAD_SPEEDM,GAD_SPEEDM,GAD_OEIL00,GAD_LIFE_M,GAD_INVERS,GAD_GLUE00,GAD_FIRE01,
	GAD_FIRE02,GAD_SIZE_M,GAD_SIZE_P,GAD_BALLE2,GAD_BALLE3,GAD_POWER1,GAD_SPEEDM,
	GAD_SIZE01,GAD_SIZE02,GAD_SIZE02,GAD_BUMP04,GAD_WALL01,GAD_NEXTLV,GAD_INVERS,
	GAD_LIFE_M,GAD_WALL01,GAD_ROBOT1,GAD_FIRE01,GAD_FIRE02,GAD_SIZE_M,GAD_SIZE_P,
	GAD_BALLE2,GAD_BALLE3,GAD_POWER1,GAD_SPEEDM,GAD_SIZE01,GAD_WALL01,GAD_WALL01,
	GAD_BUMP02,GAD_BUMP02,GAD_BUMP03,GAD_INVERS,GAD_WALL01,GAD_WALL01,GAD_GLUE00,
	GAD_FIRE01,GAD_PRICE1,GAD_SIZE_M,GAD_SIZE_P,GAD_BALLE2,GAD_BALLE3,GAD_POWER1,
	GAD_WALL01,GAD_WALL01
};

//-------------------------------------------------------------------------------
// bricks levels: key enable options (for test only)
//-------------------------------------------------------------------------------
Sint16 ze_gadgets::keysTriche[] =
{	SDLK_F1,	GAD_GLUE00, 0,  
	SDLK_F2,	GAD_NEXTLV, 0,  
	SDLK_F3,	GAD_FIRE01, 0,  
	SDLK_F4,	GAD_FIRE02, 0, 
	SDLK_F5,	GAD_SIZE_M, 0, 
	SDLK_F6,	GAD_SIZE_P, 0, 
	SDLK_F7,	GAD_LIFE_M, 0, 
	SDLK_F8,	GAD_LIFE_P, 0, 
	SDLK_F9,	GAD_BALLE2, 0,  
	SDLK_F10,	GAD_BALLE3, 0,
	SDLK_F11,	GAD_POWER1, 0,  
	SDLK_F12,	GAD_POWER2, 0, 
	SDLK_a,		GAD_INVERS, 0, 
	SDLK_z,		GAD_SPEEDM, 0, 
	SDLK_e,		GAD_BUMP01, 0, 
	SDLK_r,		GAD_BUMP02, 0, 
	SDLK_t,		GAD_BUMP03, 0, 
	SDLK_y,		GAD_BUMP04, 0, 
	SDLK_u,		GAD_SIZE01, 0, 
	SDLK_i,		GAD_SIZE02, 0,
	SDLK_o,		GAD_RANDOM, 0,
	SDLK_q,		GAD_MEGA00, 0,
	SDLK_s,		GAD_PRICE1, 0,
	SDLK_d,		GAD_WALL01, 0,
	SDLK_g,		GAD_CONTRO, 0,
	SDLK_h,		GAD_OEIL00, 0,
	SDLK_j,		GAD_OEIL00, 0,
	SDLK_k,		GAD_ROBOT1, 0,
	0, 0, 0
};
