//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "shop_tecno.cc"
// created		: ?
// updates		: 2004-10-23
// fonction	: manage the shop
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
#include "../include/shop_tecno.h"
#include "../include/ressources.h"
#include "../include/menu_tecno.h"
#include "../include/print_menu.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
shop_tecno::shop_tecno()
{
	initialise();
	ecranfond4 = new fond_ecran();
	objetMouse = new mousePoint();
	BOB_allume = new BOB_killer();
	gereGadget = new ze_gadgets(NB_OPTIONS + 2, 0);
	mega_print = new print_text();
	ptrEscMenu = new escapeMenu();
	shop_point = 0;
	optioninfo = 0;
	infodejavu = 0;
	prixActuel = 0;
	bonusachat = 0;
	curseur_x1 = 0;
	curseur_y1 = 0;
	get_object = -1;
	pt_get_obj = 0;
	shop_point = 0;
	shoppoint3 = 0;
	bobclignot = (tecno_gads*)NULL;

	shop_xmini = 6 * resolution;
	shop_xmaxi = 283 * resolution;
	shop_xmax2 = 51 * resolution;
	shop_ymax1 = 173 * resolution;
	shop_ymax2 = 218 * resolution;

	cadre_offs = 0;
	cadre_flag = 0;
	cadre_ymin = 3 * resolution;
	cadre_ymax = 171 * resolution + cadre_ymin;
	cadre_posx = 292 * resolution;;
	cadre_posy = 0;
	cadre_larg = 21 * resolution;
	cadre_haut = 9 * resolution;
	box_colour = 0;
	angleValue = 0;

	triche_key = 0;		//last key pressed
	triche_etb = 0;		//input (triche_etb == tricheCode then cheat_flag = 1)
	tricheCode = SDLK_e << 24 | SDLK_t << 16 | SDLK_b << 8 | SDLK_RETURN;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
shop_tecno::~shop_tecno()
{
	delete ptrEscMenu;
	delete mega_print;
	delete gereGadget;
	if(BOB_allume)
	{	delete BOB_allume;
		BOB_allume = (BOB_killer*)NULL;
	}
	delete objetMouse;
	delete ecranfond4;
	liberation();
}

//-------------------------------------------------------------------------------
// Initialize the shop
//-------------------------------------------------------------------------------
Sint32 shop_tecno::first_init()
{
	Sint32 arean = joueurGere->getAreaNum();
	Sint32 level = joueurGere->getLevelNu();
#ifndef SOUNDISOFF
	ptAudiomix->levelmusic (arean, level);
	ptAudiomix->shop_music (arean);
#endif
	ecran_hard *ecran = ecran_gere;
	BOBgestion->clear_list();

	//###################################################################
	// copy name player into menu text
	//###################################################################
	char *ptDes;
	const char *ptSrc;
	ptDes = joueurGere->returnName();
	for(Sint32 i = 0; i < 6; i++)
		shoptext00[8 + i] = ptDes[i];
	intToASCII(NB_OPTIONS, &shoptext63[48], 1); 	
	intToASCII(joueurGere->getLifeNum(), &info_text1[STEXTWIDHT * 4 + 5], 1);

	if(arean > 1)
	{	const char* pPass = menu_tecno::getTheCode(arean, hardChoice);
		ptDes = &info_text3[1 * STEXTWIDHT + 10];
		for(Sint32 i = 0; i < 10; i++)
			ptDes[i] = pPass[i];
		ptSrc = &info_text3[0];
	}
	else
	{	ptSrc = &info_text3[STEXTWIDHT * 2];
	}
	ptDes = &info_text1[6 * STEXTWIDHT];
	for(Sint32 i = 0; i < (STEXTWIDHT * 2); i++)
		ptDes[i] = ptSrc[i];
	
	
	ptSrc= &print_menu::difficulte[(hardChoice - 1) * 4];
	ptDes = &info_text1[8 * STEXTWIDHT + 16];
	for(Sint32 i = 0; i < 4; i++)
		ptDes[i] = ptSrc[i];	
	
	//###################################################################
	// load page of sprites in memory
	//###################################################################
	error_init(pRessource->loadSprite());
	if(erreur_num)
		return (erreur_num);

	//###################################################################
	// Initialize LED 
	//###################################################################
	error_init(BOB_allume->initialise(BOB_LEDSHP, image_BOBs, 0));
	if(erreur_num) return erreur_num;
	BOBgestion->ajoute_BOB(BOB_allume);
	BOB_allume->BOB_active();

	//###################################################################
	// initialize the gadgets
	//###################################################################
	error_init(gereGadget->gadgetShop());
	if(erreur_num) return erreur_num;
	joueurGere->RAZ_course();
	Sint32* tp = coursetemp;
	for(Sint32 i = 0; i < NB_OPTIONS; i++)
		*(tp++) = 0;
	tecno_gads **liste = gereGadget->listeObjet();
	bob_volant = liste[(NB_OPTIONS + 2) - 1 - 1];

	//###################################################################
	// Initialize the mouse pointer
	//###################################################################
	error_init(objetMouse->create_BOB(image_BOBs));
	if(erreur_num) return erreur_num;
	
	//###################################################################
	// intialize the "escape menu"
	//###################################################################
	error_init(ptrEscMenu->first_init
	(	image_BOBs,
		1,					//menu number
		320 * resolution,	//width of screen (center)
		1,					//restaure background where leaves
		1					//initialize color table
	));
	

	pRessource->freeSprite();
	error_init(mega_print->initialise());
	if(erreur_num) return erreur_num;
	
	//###################################################################
	// load bitmap background of the shop
	//###################################################################
	GIF_bitMap *gfxPT = new GIF_bitMap();
	error_init(gfxPT->decompacte(ressources::RESNEWSHOP));
	if(erreur_num) return erreur_num;
	gfxPT->copyTampon();
	delete gfxPT;

	ecran->tamponBuff();		//copy tampon memory into buffer memory  
	ecran->bufferCopy();		//copy buffer memory into the screen


	putthetext(shoptext00);
	if(joueurGere->get_Bprice())
		shop_line3 = &shoptext00[STEXTWIDHT * 3];

	keyGestion->setGrabOff();
	ecranfond4->coulDuFond();
	return erreur_num;
}

//-------------------------------------------------------------------------------
// shop main loop
//-------------------------------------------------------------------------------
Sint32 shop_tecno::zeMainLoop()
{
	Sint32 Ecode = -1; 
	ecran_gere->waitVBlank();
	ecran_gere->verouiller();
	end_return = 0;
	BOBgestion->listeBOBrz();
	
	//###################################################################
	// copy the "tampon" memory in the "buffer" memory
	//###################################################################
	
	/*ecran_gere->tampon_RAZ(246, 290 * resolution, 3 * resolution, 
		26 * resolution, 180 * resolution);*/
	
	ecran_gere->tamponBuff(290* resolution, 3 * resolution, 
		26 * resolution, 180 * resolution);
	
	if(!ptrEscMenu->BOBisactiv())
	{	pos_select();
		Sint32 x = objetMouse->retournePX();
		Sint32 y = objetMouse->retournePY();
		if (get_object == -1)	//-1 = not a drag object
		{	Sint32 x2, y2;
			Sint32 mousreleas = keyGestion->sourisRela(&x2, &y2);
			if(mousreleas == 1)
			{	Sint32 shoppoint2 = testkursor(x, y);
				if(shoppoint2 == shop_point)
				{	Sint32 gadnu = case_types[shoppoint2];
					faitcourse(gadnu);
				}
			}
			else
			{	if(!keyGestion->leftButton())
				{	shop_point = testkursor(x, y);
					prixActuel = led_moving(shop_point);
					//prixActuel = 0;	//test only
					if(shoppoint3 != shop_point)
						shoppoint3 = shop_point;
				}
			}
			
		}
		else
			prixActuel = led_moving(-1);
		sh_ballade();
	}

	//###################################################################
	// display price and credit
	//###################################################################
	mega_print->bufferAff1(263 * resolution, 227 * resolution,
		prixActuel, 100000);
	mega_print->bufferAff1(263 * resolution, 183 * resolution,
		joueurGere->get_credit(), 100000);
	objetMouse->bouge_test();
	if(cheat_flag)
		gereGadget->animations(2);
	else
		gereGadget->animations(1);
	
	//###################################################################
	// display the 3 lines of text 
	//###################################################################
	affichtext();
	
	//###################################################################
	// display the cursor of the bonus selected in the list on the right 
	//###################################################################
	aff_select();

	//###################################################################
	// display all sprites
	//###################################################################
	BOBgestion->listeBOBgo();
	aff_course();

	Ecode = ptrEscMenu->execution1();

	//###################################################################
	// copy buffer surface to screen surface
	//###################################################################
	ecran_gere->deverouill();
	ecran_gere->bufferCTab();

	//###################################################################
	// escape key to quit the game !
	//###################################################################
	if(keyGestion->specialKey(clavierMac::TOEXITFLAG) ||
		Ecode == escapeMenu::WEQUITGAME)
		end_return = -1;
	if(keyGestion->specialKey(clavierMac::TOMENUFLAG) ||
		Ecode == escapeMenu::EXITTOMENU)
		end_return = 4;

	tu_triches();
	return (end_return);
}


//------------------------------------------------------------------------------
// display list bonus bought in the shop (on the right of the screen)
//------------------------------------------------------------------------------
void shop_tecno::aff_course()
{	
	Sint32 *p = joueurGere->get_course();
 	tecno_gads **liste = gereGadget->listeObjet();
 	Sint32 pos_y = 4 * resolution;
 	for(Sint32 i = 0; i < NB_OPTIONS; i++)
	{	tecno_gads *gadgt = *(liste++);
		gadgt->coordonnee(294 * resolution, pos_y); 
		pos_y = pos_y + 9 * resolution;
		Sint32 o = *(p++);
		gadgt->nouveauGad(o);
	}
}

//------------------------------------------------------------------------------
// return index of the selected bonus, and calculate position of LED indicator
//	input	=>	x: abscissa of the mouse pointer
//			=>	y: ordinate of the mouse pointer
//	output	<=	index: 0 to 24 (-1: disable)
//------------------------------------------------------------------------------
Sint32 shop_tecno::testkursor(Sint32 x, Sint32 y)
{
	if(x < shop_xmini || x > shop_xmaxi || y > shop_ymax2 ||
		(x > shop_xmax2 && y > shop_ymax1))
		return -1;
	else
	{	x = (x - (6 * resolution)) / (48 * resolution);
		y = (y / (44 * resolution));
		if(y > 4)
			x = 0;
		Sint32 i = x + 6 * y;
		curseur_x1 = (x * (48 * resolution)) + (17 * resolution);
		curseur_y1 = (y * (44 * resolution)) + (36 * resolution);
		return i;
	}
}

//-------------------------------------------------------------------------------
// set LED indicator, and gadget indicator
//	input	=>	index: index of the selected bonus; 0 to 24 (-1: disable)
//	output	<=	price: price of the selected bonus 
//-------------------------------------------------------------------------------
Sint32 shop_tecno::led_moving(Sint32 index)
{
	if(index < 0)
	{	BOB_allume->BOB_desact();
		Sint32 i = 0;
		gereGadget->gadgetShop(0);
		return i;
	}
	else
	{	
		// set gadget indicator
		Sint32 i = case_types[index];
		gereGadget->gadgetShop(i);
		
		// set LED indicator
		BOB_allume->BOB_active();
		BOB_allume->coordonnee(curseur_x1, curseur_y1);
		if(joueurGere->get_Bprice())
			i = 1;
		else
			i = case_price[index];

		// info already seen at least once ?
		if(index == 10 && infodejavu)
			i = 0;
		
		return i;
	}
}

//-------------------------------------------------------------------------------
// bought a gadget
//	input	=> gadnu: option number
//-------------------------------------------------------------------------------
void shop_tecno::faitcourse(Sint32 gadnu)
{
	if(gadnu != GAD_INFORM)
		optioninfo = 0;

	switch (gadnu)
	{ 

		//###############################################################
		// expand bumper
		//###############################################################
		case GAD_SIZE_P:
			achete_gad(gadnu);
			break;

		//###############################################################
		// fire power 1
		//###############################################################
		case GAD_FIRE01:
			achete_gad(gadnu);
			break;

		//###############################################################
		// fire power 2
		//###############################################################
		case GAD_FIRE02:
			achete_gad(gadnu);
			break;

		//###############################################################
		// extra balls
		//###############################################################
		case GAD_BALLE2:
			achete_gad(gadnu);
			break;

		//###############################################################
		// multi balls
		//###############################################################
		case GAD_BALLE3:
			achete_gad(gadnu);
			break;

		//###############################################################
		// power ball 1
		//###############################################################
		case GAD_POWER1:
			achete_gad(gadnu);
			break;

		//###############################################################
		// power ball 2
		//###############################################################
		case GAD_POWER2:
			achete_gad(gadnu);
			break;

		//###############################################################
		// extra life
		//###############################################################
		case GAD_LIFE_P:
			achete_gad(gadnu);
			break;

		//###############################################################
		// extra life
		//###############################################################
		case GAD_WALL01:
			achete_gad(gadnu);
			break;
		
		//###############################################################
		// ball size 2
		//###############################################################
		case GAD_SIZE01:
			achete_gad(gadnu);
			break;

		//###############################################################
		// ball size 3
		//###############################################################
		case GAD_SIZE02:
			achete_gad(gadnu);
			break;

		//###############################################################
		// robot bumper option
		//###############################################################
		case GAD_ROBOT1:
			achete_gad(gadnu);
			break;

		//###############################################################
		// balls control option
		//###############################################################
		case GAD_CONTRO:
			achete_gad(gadnu);
			break;

		//###############################################################
		// glue option
		//###############################################################
		case GAD_GLUE00:
			achete_gad(gadnu);
			break;

		//###############################################################
		// somes infos
		//###############################################################
		case GAD_INFORM:
		{	Sint32 arean = joueurGere->getAreaNum();
			char *ptSrc = &info_text2[0];
			char *ptDes= &info_text1[0];
			for(Sint32 i = 2; i <= 4; i++) 
			{	char *ptTxt = ptSrc;
				if(joueurGere->get_bumpOn(i) <= 0)
				{	ptTxt += STEXTWIDHT;
					if(arean >= 2 && i == 3)
						ptTxt += STEXTWIDHT;
					if(arean >= 3 && i == 2)
						ptTxt += STEXTWIDHT;
					if(arean >= 4 && i == 4)
						ptTxt += STEXTWIDHT;
						
					// area 2 => bumper 3 
					// area 3 => bumper 3 & 2 
					// area 4 & 5 => bumper 3 & 2 & 4 
				}
				for(Sint32 j = 0; j < STEXTWIDHT; j++)
					*(ptDes++) = *(ptTxt++);
				ptSrc += STEXTWIDHT * 3;
			}

		
			if(!infodejavu)
			{	if(!sub_credit(prixActuel))
					break;
			}
			infodejavu = 1;
			if(optioninfo < 3)
				putthetext (&info_text1[optioninfo * STEXTWIDHT * 3]);
			else 
			{	
				if(cheat_flag)
					putthetext(&info_text1[(optioninfo + 2)* STEXTWIDHT * 3]);
				else 
				{	if(birth_flag)
						putthetext(&info_text1[(optioninfo + 1)* STEXTWIDHT * 3]);
					else
						putthetext(&info_text1[optioninfo * STEXTWIDHT * 3]);
				}
			}
			optioninfo++;
			optioninfo &= 3;
		}
		break;

		//###############################################################
		// rebuild the wall
		//###############################################################
		case GAD_REBUIL:
		if(joueurGere->getAreaNum() < 5) 
			putthetext(shoptext56);
		else 
		{	if(joueurGere->getRebuild() <= 0)
			{	if(sub_credit(prixActuel))
				{	joueurGere->setRebuild(1);
					message_ok();
				}
			}
		}
		break;

		//###############################################################
		// less bricks option
		//###############################################################
		case GAD_LESSBR:
			if(joueurGere->get_lessBk() <= 0)
			{	if(sub_credit(prixActuel))
				{	joueurGere->set_lessBk(10);
					message_ok();
				}
			}
			break;

		//###############################################################
		// exit from the shop
		//###############################################################
		case GAD_EXITSH:
			joueurGere->set_Bprice(0);
				joueurGere = joueurData::nextplayer(joueurGere, &end_return, 2);
			break;

		default:
		{ 
			//############################################################
			// buy a right, up or left bumper
			//############################################################
			if(gadnu >= GAD_BUMP02 && gadnu <= GAD_BUMP04)
			{	Sint32 i = (gadnu - GAD_BUMP02) / 2 + 2;
				if(joueurGere->get_bumpOn(i) <= 0)
				{	if(sub_credit(prixActuel))
					{	joueurGere->set_bumpOn(i, 3);
						message_ok();
					}
				}
			}
		}
		break;
	}
}

//------------------------------------------------------------------------------
// check if a purchase is possible, so decrement the credit
//------------------------------------------------------------------------------
Sint32 shop_tecno::sub_credit(Sint32 value)
{
	if(joueurGere->sub_credit(prixActuel))
		return 1;
	else
	{	putthetext(shoptext41);
		return 0;
	}
}

//------------------------------------------------------------------------------
// purchase a bonus if possible
//------------------------------------------------------------------------------
void shop_tecno::achete_gad(Sint32 gadnb)
{
	//gadgets maximum number ?
	if(bonusachat >= NB_OPTIONS)
	{	putthetext(shoptext63);
		return;
	}

	//  purchase is possible ?
	if(!sub_credit(prixActuel)) return;
	
	Sint32 *p = joueurGere->get_course();
	p[bonusachat] = gadnb;
	sh_tablept[bonusachat] = shop_point;
	tecno_gads **liste = gereGadget->listeObjet();
	tecno_gads *gadgt = liste[bonusachat++];
	gadgt->nouveauGad(gadnb);
	message_ok();
	joueurGere->set_cou_nb(bonusachat);
}

//------------------------------------------------------------------------------
// change the 3 lines of the text
//------------------------------------------------------------------------------
void shop_tecno::message_ok()
{
	Sint32 i = shoptextPT[shop_point] * STEXTWIDHT * 3;
	char *texte = shoptext12 + i;
	putthetext(texte);
}


//------------------------------------------------------------------------------
// pointe les trois lignes a afficher 
//------------------------------------------------------------------------------
void shop_tecno::putthetext(char *ligne)
{
	shop_line1 = ligne;
	ligne += STEXTWIDHT;
	shop_line2 = ligne;
	ligne += STEXTWIDHT;
	shop_line3 = ligne;
}

//-------------------------------------------------------------------------------
// display the three lines of text in the box in bottom
//-------------------------------------------------------------------------------
void shop_tecno::affichtext()
{
	Uint32 charH = mega_print->getCharHgt();
	Sint32 x_pos = 60 * resolution;
	Sint32 y_pos = 180 * resolution;
	Sint32 yspac = charH + resolution;
	ecran_gere->buffer_RAZ(0, x_pos, y_pos, 22 * 8 * resolution, 3 * yspac);
	mega_print->bufferAff2(x_pos, y_pos, shop_line1, 22);
	mega_print->bufferAff2(x_pos, y_pos + yspac, shop_line2, 22);
	mega_print->bufferAff2(x_pos, y_pos + yspac * 2 , shop_line3, 22);
}

//-------------------------------------------------------------------------------
// drag and drop the gadget
//-------------------------------------------------------------------------------
void shop_tecno::sh_ballade()
{
	if(get_object >= 0)	//pointer to the table "case_price" (-1 = no drag object)
	{	if(keyGestion->leftButton())
		{	bob_volant->BOB_active();
			bob_volant->coordonnee(objetMouse->retournePX(), objetMouse->retournePY());
			if(bobclignot->flag_actif)
				bobclignot->flag_actif = 0;
			else
				bobclignot->flag_actif = 1;
		}
		else
		{	
		
		
			bob_volant->BOB_desact();
			bobclignot->flag_actif = 1;
			Sint32 i = cadre_offs;
			if(i >= 0)
			{	if(i >= bonusachat)
					i = bonusachat - 1; 
				Sint32 *p0 = joueurGere->get_course();
				Sint32 *p1 = courseList;	//source
				Sint32 *p2 = p0 + i;		//destination 
				Sint32 *tp = coursetemp;
				Sint32 value = *p1;			//source code
				if(p1 != p2)
				{ 	Sint32 i = 0;
					do
					{	if(p0 == p1)	//emplacement source ?
						{	p0++;
							i++;
						}
						else
						{ if(p0 == p2)	//emplacement destination ?
							{	if(p1 <= p2)	// source <= destination
								{ if(i++ < NB_OPTIONS)
									{	*(tp++) = *(p0++);
									}
								}
								if(i++ < NB_OPTIONS)
								{	*(tp++) = value;
								}
							}
							if(i++ < NB_OPTIONS)
							{	*(tp++) = *(p0++);
							}
						}
					}
					while(i < NB_OPTIONS);
					tp = coursetemp;
					p0 = joueurGere->get_course();
					for(Sint32 i = 0; i < NB_OPTIONS; i++)
					{	*(p0++) = *(tp++);
					}
				}
			}
			else
			{	Sint32 *p1 = courseList;
				Sint32 *p2 = p1;
				Sint32 gadnu = *p1;	//drop bonus code
				*(p1++) = 0;
				while (*p1 >= 0)
				{	*(p2++) = *p1;
					*(p1++) = 0;
				}
				bonusachat--;
				joueurGere->set_cou_nb(bonusachat);
				
				//check bonus price
				Sint32 price = 0;
				if(joueurGere->get_Bprice())
					price = 1;
				else
				{	while (case_types[i] != GAD_EXITSH)
					{	if(case_types[i] == gadnu) 
						{	price = case_price[i]; 
							break;
						}
						i++;
					}
				}
				joueurGere->add_credit(price);
			}
			get_object = -1;
		}
	}

	//###################################################################
	// get a object with the mouse
	//###################################################################
	else
	{	bob_volant->BOB_desact();
		pt_get_obj = -1;
		if(keyGestion->leftButton())
		{	Sint32 i = cadre_offs;
			if(i >=0) 
			{	Sint32 *p = joueurGere->get_course();
				tecno_gads **liste = gereGadget->listeObjet();
				courseList = p + i;
				bobclignot = *(liste + i);
				get_object = *(sh_tablept + i);
				bob_volant->copiegadet(bobclignot);
				bobclignot->flag_actif = 1;
			}
		}
	}
}

//-------------------------------------------------------------------------------
// calculate position of box cursor
//-------------------------------------------------------------------------------
void shop_tecno::pos_select()
{ 
	Sint32 y = objetMouse->retournePY() - cadre_ymin;
	Sint32 o = (y / cadre_haut);			//y / 9 (height of cursor)
	y = o * cadre_haut + cadre_ymin;
	if(y < cadre_ymin)
		y = cadre_ymin;
	if(y > cadre_ymax)
		y = cadre_ymax;
	cadre_posy = y ;						//cursor y coordinate
	
	Sint32 x = objetMouse->retournePX();
	y = objetMouse->retournePY();
	cadre_flag = 0;							//don't display cursor (by default)
	cadre_offs = -1;

	//at least one bonus ?
	if(bonusachat < 1) return;

	// calculate max. y coordinate
	Sint32 y2 = bonusachat * cadre_haut + cadre_ymin;
	
	// drag object ? then add a possible position, all in bottom. 
	if(get_object >= 0)
		y2 = y2 + cadre_haut;
	
	if(x >= cadre_posx && y < y2)
	{	cadre_flag = 1;
		cadre_offs = o;
	}
}

//-------------------------------------------------------------------------------
// display the cursor of the bonus selected in the list on the right 
//-------------------------------------------------------------------------------
void shop_tecno::aff_select()
{ 
	if(box_colour++ > 32)
		box_colour = 0;
			
	Sint32 a = angleValue + 5;
	a &= 511;
	angleValue = a;
	if(cadre_flag > 0)
	{	Sint16 *s = table_cosL + a;
		a = *s;
		a *= (cadre_larg / 5);
		a >>= 7;
		a = a + cadre_larg / 5;
		Sint32 b = angleValue;
		s = table_cosL + b;
		b = *s;
		b *= (cadre_haut / 5);
		b >>= 7;
		b = b + cadre_haut / 5;
		Sint32 l = cadre_larg - a;
		Sint32 h = cadre_haut - b;
		Sint32 x = cadre_posx + (cadre_larg - l) / 2;
		Sint32 y = cadre_posy + (cadre_haut - h) / 2;

		Sint32 tmpco = 0;
		Sint32 color = box_colour;
		char* pBuff = ecran_gere->buffer_pos(x, y);
		Sint32 nextl = ecran_gere->bufferNext();
		for(Sint32 v = 0; v < h; v++)
		{	for(Sint32 h = 0; h < l; h++)
			{	unsigned char pixel = cyclingtab[color];
				pBuff[h] = pixel;
			}
			if(++tmpco == 5)
			{	tmpco = 0;
				if(color++ > 32) color = 0;
			}
			pBuff += nextl;
		}
	}
}

//-------------------------------------------------------------------------------
// test the cheat code
//-------------------------------------------------------------------------------
void shop_tecno::tu_triches()
{
	objetMouse->tempoinit2(3);
	if(cheat_flag) return;
	if(!birth_flag) return;
	if(!objetMouse->retournePX() && !objetMouse->retournePY())
	{	objetMouse->tempoinit2(20);
		Sint32 k = keyGestion->get_Kcode1();
		if(triche_key != k && k)
		{	triche_key = k;
			triche_etb = triche_etb << 8 | k;
		}
	}
	if(triche_etb == tricheCode)
		cheat_flag = 1;
	else 
		cheat_flag = 0;
}
	
//==============================================================================
// statics data
//==============================================================================

//-------------------------------------------------------------------------------
// Contenu de chaque case 
//-------------------------------------------------------------------------------
Sint32 shop_tecno::case_types[] =
{	GAD_SIZE_P, GAD_FIRE01, GAD_FIRE02, GAD_REBUIL, GAD_BALLE2, GAD_BALLE3,
 	GAD_POWER1, GAD_POWER2, GAD_LESSBR, GAD_LIFE_P, GAD_INFORM, GAD_WALL01,
	GAD_BUMP04, GAD_BUMP03, GAD_BUMP02, GAD_SIZE01, GAD_SIZE02, GAD_ROBOT1,
	GAD_CONTRO, GAD_GLUE00, GAD_EXITSH, GAD_EXITSH, GAD_EXITSH, GAD_EXITSH,
	GAD_EXITSH, GAD_EXITSH, GAD_EXITSH, GAD_EXITSH, GAD_EXITSH, GAD_EXITSH
};

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
Sint32 shop_tecno::sh_tablept[NB_OPTIONS];

//-------------------------------------------------------------------------------
// Prix des gadgets 
//-------------------------------------------------------------------------------
Sint32 shop_tecno::case_price[] =
{	60, 75, 150, 350, 25, 50,
	250, 500, 400, 450, 10, 75,
	100, 100, 100, 60, 75, 100,
	60, 150, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0
};

//-------------------------------------------------------------------------------
// texts 
//-------------------------------------------------------------------------------
char shop_tecno::shoptext00[] =
{	"WELCOME ...... TO THE " \
	"  TECNOBALL-Z SHOP    " \
	"                      " \
	"PRICE BONUS IS ENABLE "
};
//...............................................................................
char shop_tecno::shoptext41[] =
{	"SORRY, BUT YOU HAVEN'T" \
	"   GOT ENOUGH MONEY   " \
	"  TO BUY THIS OPTION  "
};
//...............................................................................
char shop_tecno::shoptext63[] =
{	"                     " \
	"  YOU CAN ONLY BUY   " \
	"      .. GADGETS     " \
	"                     "
};
//...............................................................................
char shop_tecno::shoptext56[] =
{	"  SORRY  THIS OPTION  " \
	"   CAN ONLY BE USED   " \
	"      FOR AREA 5      "
};
//...............................................................................
char shop_tecno::shoptextPT[]=
{	 0, 1, 2, 3, 4, 5,		//S+/F1/F2/RW/B2/B3
	 6, 7, 8, 9,10,11,		//P1/P2/LB/L+/??/WA
	12,13,14,15,16,17,		//BL/BU/BR/S2/S3/RB
	18,19,20,20,20,20,		//CT/GL/XX/XX/XX/XX
	20,20,20,20,20,20,		//XX/XX/XX/XX/XX/XX
};

//*- S+ --------------------------------------------------------------------*
char shop_tecno::shoptext12[] =
{	"    AH YEAH IT IS     " \
	"  MUCH BETTER WITH A  " \
	" SUCH LONGER BUMPER   " \
//*- F1 --------------------------------------------------------------------*
	"THIS FIRE IS NOT AS SO" \
	" POWERFULL AS FIRE 1  " \
	"  BUT IT IS CHEAPER   " \
//*- F2 --------------------------------------------------------------------*
	"   BIGGER POWER  TO   " \
	"  DESTROY MORE BRICS  " \
	"                      " \
//*- RW --------------------------------------------------------------------*
	"   OK, THE RED WALLS  " \
	"   WILL BE REBUILT    " \
	"                      " \
//*- B1 --------------------------------------------------------------------*
	"                      " \
	"  TWO BALLS TO CATCH  " \
	"                      " \
//*- B2 --------------------------------------------------------------------*
	"    THREE BALLS NOW   " \
	" YOU WILL BRAKE MORE  " \
	" AND FASTER THE WALL  " \
//*- P1 --------------------------------------------------------------------*
	"  A VERY STRONG BALL  " \
	"    TO BREAK BRIKS    " \
	"                      " \
//*- P2 --------------------------------------------------------------------*
	"  WITH THAT YOU WILL  " \
	"  BREAK ALL YOU WANT  " \
	"                      " \
//*- LB --------------------------------------------------------------------*
	" THERE WILL 10 BRICKS " \
	"    LESS TO BREAK     " \
	"                      " \
//*- L+ --------------------------------------------------------------------*
	" -------------------- " \
	"   EXTRA LIFE ADDED   " \
	" -------------------- " \
//*- ?? --------------------------------------------------------------------*
//text.info	
	"                      " \
	"                      " \
	"                      " \
//*- WA --------------------------------------------------------------------*
	"          OK          " \
	"       FOR  THE       " \
	"         WALL         " \
//*- BL --------------------------------------------------------------------*
	" OK FOR A LEFT BUMPER " \
	"BUT DON'T FORGET IT IS" \
	" OVER AFTER 3 LEVELS  " \
//*- BU --------------------------------------------------------------------*
	"  OK FOR  UP  BUMPER  " \
	"BUT DON'T FORGET IT IS" \
	" OVER AFTER 3 LEVELS  " \
//*- BR --------------------------------------------------------------------*
	"OK  FOR A RIGHT BUMPER" \
	"BUT DON'T FORGET IT IS" \
	" OVER AFTER 3 LEVELS  " \
//*- S2 --------------------------------------------------------------------*
	"                      " \
	"    BIGSIZE  BALLS!   " \
	"                      " \
//*- S3 --------------------------------------------------------------------*
	"                      " \
	"   MAXISIZE  BALLS!   " \
	"                      " \
//*- RB --------------------------------------------------------------------*
	"          OK          " \
	"       FOR  THE       " \
	"        ROBOT!        " \
//*- CT --------------------------------------------------------------------*
	"          OK          " \
	"       FOR  THE       " \
	"     CONTROLATOR!     " \
//*- GL --------------------------------------------------------------------*
	"                      " \
	"   OK FOR THE GLUE    " \
	"                      " \
//*- XX --------------------------------------------------------------------*
	"      GO AND SEE      " \
	"        MY LOVE       " \
	"                      " \
};

char shop_tecno::info_text1[] =
{	
	"RIGHT BUMPER:NO ACTIVE" \
	"UP BUMPER   :NO ACTIVE" \
	"LEFT BUMPER :NO ACTIVE" \
	
	"     YOU ONLY HAVE    " \
	"     00 LIVES LEFT    " \
	"                      " \

	"                      " \
	"                      " \
	"  DIFFICULTY IS ....  " \

	"I HOPE THIS ASSISTANCE" \
	"     WILL HELP YOU    " \
	"                      " \

	"      TLK  GAMES      " \
	"     TECNOBALL  Z     " \
	"      TLK  GAMES      " \

	"  LA CORRUPTION REVET " \
	"   DES  DEGUISEMENTS  " \
	"        INFINIS       " \
};

char shop_tecno::info_text2[] =
{	
	"RIGHT BUMPER:  ACTIVE " \
	"RIGHT BUMPER:NO ACTIVE" \
	"  BUY A RIGHT BUMPER  "
	"UP BUMPER   :  ACTIVE " \
	"UP BUMPER   :NO ACTIVE" \
	"     BUY A UP BUMPER  " \
	"LEFT[BUMPER :  ACTIVE " \
	"LEFT BUMPER :NO ACTIVE" \
	"  BUY A LEFT BUMPER   " \
};
char shop_tecno::info_text3[] =
{	
	" THE PASSWORD FOR THIS" \
	"  AREA IS ..........  " \
	" NO AVAILABLE PASSWORD" \
	" FOR THIS AREA.       " \
};

const unsigned char shop_tecno::cyclingtab[] =
{	239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
	253, 254, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244,
	243, 242, 241, 240, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
	249, 250, 251, 252, 253, 254, 255
};
