//******************************************************************************
// copyright (c) 1991-2006 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "supervisor_shop.cc"
// created	: ?
// updates	: 2006-10-04
// fonction	: manage the shop
// id		: $Id: supervisor_shop.cc,v 1.7 2007/02/08 17:00:33 gurumeditation Exp $
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
#include "../include/supervisor_shop.h"
#include "../include/handler_resources.h"
#include "../include/supervisor_main_menu.h"
#include "../include/print_menu.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
supervisor_shop::supervisor_shop()
{
	initialise();
	ecranfond4 = new tiles_background();
	mouse_pointer = new sprite_mouse_pointer();
	BOB_allume = new sprite_object();
	gereGadget = new controller_capsules(NB_OPTIONS + 2, 0);
	mega_print = new print_text();
	ptrEscMenu = new handler_popup_menu();
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
	bobclignot = (sprite_capsule*)NULL;

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
supervisor_shop::~supervisor_shop()
{
	delete ptrEscMenu;
	delete mega_print;
	delete gereGadget;
	if(BOB_allume)
	{	delete BOB_allume;
		BOB_allume = (sprite_object*)NULL;
	}
	delete mouse_pointer;
	delete ecranfond4;
	liberation();
}

//-------------------------------------------------------------------------------
// Initialize the shop
//-------------------------------------------------------------------------------
Sint32 supervisor_shop::first_init()
{
	Sint32 arean = current_player->get_area_number();
	Sint32 level = current_player->get_level_number();
#ifndef SOUNDISOFF
	audio->play_level_music (arean, level);
	audio->play_shop_music (arean);
#endif
	sprites->reset();

	//###################################################################
	// copy name player into menu text
	//###################################################################
	char *ptDes;
	const char *ptSrc;
	ptDes = current_player->returnName();
	for(Sint32 i = 0; i < 6; i++)
		shoptext00[8 + i] = ptDes[i];
	intToASCII(NB_OPTIONS, &shoptext63[48], 1); 	
	intToASCII(current_player->get_num_of_lifes(), &info_text1[STEXTWIDHT * 4 + 5], 1);

	if(arean > 1)
	{	const char* pPass = supervisor_main_menu::getTheCode(arean, hardChoice);
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
	
	resources->load_sprites_bitmap();

	//###################################################################
	// Initialize LED 
	//###################################################################
	if(resolution == 1) 
		BOB_allume->create_sprite(BOB_LEDSHP, sprites_bitmap, 0);
	else 
		BOB_allume->create_sprite(BOB_LEDSH2, sprites_bitmap, 0);
	sprites->add(BOB_allume);
	BOB_allume->enable();

	//###################################################################
	// initialize the gadgets
	//###################################################################
	gereGadget->create_shop_sprites_list();
	current_player->clear_shopping_cart();
	Sint32* tp = coursetemp;
	for(Sint32 i = 0; i < NB_OPTIONS; i++)
		*(tp++) = 0;
	sprite_capsule **liste = gereGadget->get_sprites_list();
	bob_volant = liste[(NB_OPTIONS + 2) - 1 - 1];

	/* initialize the mouse pointer */
	mouse_pointer->create_pointer_sprite(sprites_bitmap);

	//###################################################################
	// intialize the "escape menu"
	//###################################################################
	ptrEscMenu->first_init
	(	sprites_bitmap,
		1,			//menu number
		320 * resolution,	//width of screen (center)
		1,			//restaure background where leaves
		1			//initialize color table
	);

	resources->release_sprites_bitmap();
	error_init(mega_print->initialise());
	if(erreur_num) return erreur_num;
	
	//###################################################################
	// load bitmap background of the shop
	//###################################################################
	bitmap_data *gfxPT = new bitmap_data();
	gfxPT->load(handler_resources::RESNEWSHOP);
	gfxPT->copyTampon();
	delete gfxPT;

        background_screen->blit_surface(game_screen);
	display->bufferCopy();		//copy buffer memory into the screen


	putthetext(shoptext00);
	if(current_player->get_Bprice())
		shop_line3 = &shoptext00[STEXTWIDHT * 3];

	keyboard->set_grab_input (false);
	ecranfond4->set_4_color_palette();
	return erreur_num;
}

//-------------------------------------------------------------------------------
// shop main loop
//-------------------------------------------------------------------------------
Sint32 supervisor_shop::main_loop()
{
	Sint32 Ecode = -1; 
	display->wait_frame();

        /* copy the background offscreen to the game offscreen */
        background_screen->blit_surface(game_screen, 290* resolution, 3 * resolution, 26 * resolution, 180 * resolution);
	display->lock_surfaces();
	end_return = 0;
	sprites->clear();
	

	if(!ptrEscMenu->is_enable())
	{	pos_select();
		Sint32 x = mouse_pointer->get_x_coord();
		Sint32 y = mouse_pointer->get_y_coord();
		if (get_object == -1)	//-1 = not a drag object
		{	Sint32 x2, y2;
			bool mousreleas = keyboard->is_left_button_up(&x2, &y2);
			if(mousreleas)
			{	Sint32 shoppoint2 = testkursor(x, y);
				if(shoppoint2 == shop_point)
				{	Sint32 gadnu = case_types[shoppoint2];
					faitcourse(gadnu);
				}
			}
			else
			{	if(!keyboard->is_left_button())
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
		current_player->get_credit(), 100000);
	mouse_pointer->move();
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
	sprites->draw();
	aff_course();

	Ecode = ptrEscMenu->execution1();

	//###################################################################
	// copy buffer surface to screen surface
	//###################################################################
	display->unlock_surfaces();
	display->bufferCTab();

	//###################################################################
	// escape key to quit the game !
	//###################################################################
	if(keyboard->command_is_pressed(handler_keyboard::TOEXITFLAG) ||
		Ecode == handler_popup_menu::WEQUITGAME)
		end_return = -1;
	if(keyboard->command_is_pressed(handler_keyboard::TOMENUFLAG) ||
		Ecode == handler_popup_menu::EXITTOMENU)
		end_return = 4;

	tu_triches();
	return (end_return);
}


//------------------------------------------------------------------------------
// display list bonus bought in the shop (on the right of the screen)
//------------------------------------------------------------------------------
void supervisor_shop::aff_course()
{	
	Sint32 *p = current_player->get_course();
 	sprite_capsule **liste = gereGadget->get_sprites_list();
 	Sint32 pos_y = 4 * resolution;
 	for(Sint32 i = 0; i < NB_OPTIONS; i++)
	{	sprite_capsule *gadgt = *(liste++);
		gadgt->set_coordinates(294 * resolution, pos_y); 
		pos_y = pos_y + 9 * resolution;
		Sint32 o = *(p++);
		gadgt->nouveauGad(o);
	}
}

//------------------------------------------------------------------------------
// return index of the selected bonus, and calculate position of LED indicator
//	input	=>	x: abscissa of the mouse pointer
//		=>	y: ordinate of the mouse pointer
//	output	<=	index: 0 to 24 (-1: disable)
//------------------------------------------------------------------------------
Sint32 supervisor_shop::testkursor(Sint32 x, Sint32 y)
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
		
		if(resolution == 1)
			curseur_y1 = (y * (44 * resolution)) + (36 * resolution);
		else
 			curseur_y1 = (y * (44 * resolution)) + (35 * resolution);
		return i;
	}
}

//-------------------------------------------------------------------------------
// set LED indicator, and gadget indicator
//	input	=>	index: index of the selected bonus; 0 to 24 (-1: disable)
//	output	<=	price: price of the selected bonus 
//-------------------------------------------------------------------------------
Sint32 supervisor_shop::led_moving(Sint32 index)
{
	if(index < 0)
	{	BOB_allume->disable();
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
		BOB_allume->enable();
		BOB_allume->set_coordinates(curseur_x1, curseur_y1);
		if(current_player->get_Bprice())
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
void supervisor_shop::faitcourse(Sint32 gadnu)
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
		{	Sint32 arean = current_player->get_area_number();
			char *ptSrc = &info_text2[0];
			char *ptDes= &info_text1[0];
			for(Sint32 i = 2; i <= 4; i++) 
			{	char *ptTxt = ptSrc;
				if(current_player->get_bumpOn(i) <= 0)
				{	ptTxt += STEXTWIDHT;
					if(arean >= 2 && i == 3)
						ptTxt += STEXTWIDHT;
					if(arean >= 3 && i == 2)
						ptTxt += STEXTWIDHT;
					if(arean >= 4 && i == 4)
						ptTxt += STEXTWIDHT;
					//area 2 => bumper 3 
					//area 3 => bumper 3 & 2 
					//area 4 & 5 => bumper 3 & 2 & 4 
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
		if(current_player->get_area_number() < 5) 
			putthetext(shoptext56);
		else 
		{	if(current_player->getRebuild() <= 0)
			{	if(sub_credit(prixActuel))
				{	current_player->setRebuild(1);
					message_ok();
				}
			}
		}
		break;

		//###############################################################
		// less bricks option
		//###############################################################
		case GAD_LESSBR:
			if(current_player->get_lessBk() <= 0)
			{	if(sub_credit(prixActuel))
				{	current_player->set_lessBk(10);
					message_ok();
				}
			}
			break;

		//###############################################################
		// exit from the shop
		//###############################################################
		case GAD_EXITSH:
			current_player->set_Bprice(0);
				current_player = handler_players::nextplayer(current_player, &end_return, 2);
			break;

		default:
		{ 
			//############################################################
			// buy a right, up or left bumper
			//############################################################
			if(gadnu >= GAD_BUMP02 && gadnu <= GAD_BUMP04)
			{	Sint32 i = (gadnu - GAD_BUMP02) / 2 + 2;
				if(current_player->get_bumpOn(i) <= 0)
				{	if(sub_credit(prixActuel))
					{	current_player->set_bumpOn(i, 3);
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
Sint32 supervisor_shop::sub_credit(Sint32 value)
{
	if(current_player->sub_credit(prixActuel))
		return 1;
	else
	{	putthetext(shoptext41);
		return 0;
	}
}

//------------------------------------------------------------------------------
// purchase a bonus if possible
//------------------------------------------------------------------------------
void supervisor_shop::achete_gad(Sint32 gadnb)
{
	//gadgets maximum number ?
	if(bonusachat >= NB_OPTIONS)
	{	putthetext(shoptext63);
		return;
	}

	//  purchase is possible ?
	if(!sub_credit(prixActuel)) return;
	
	Sint32 *p = current_player->get_course();
	p[bonusachat] = gadnb;
	sh_tablept[bonusachat] = shop_point;
	sprite_capsule **liste = gereGadget->get_sprites_list();
	sprite_capsule *gadgt = liste[bonusachat++];
	gadgt->nouveauGad(gadnb);
	message_ok();
	current_player->set_cou_nb(bonusachat);
}

//------------------------------------------------------------------------------
// change the 3 lines of the text
//------------------------------------------------------------------------------
void supervisor_shop::message_ok()
{
	Sint32 i = shoptextPT[shop_point] * STEXTWIDHT * 3;
	char *texte = shoptext12 + i;
	putthetext(texte);
}


//------------------------------------------------------------------------------
// pointe les trois lignes a afficher 
//------------------------------------------------------------------------------
void supervisor_shop::putthetext(char *ligne)
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
void supervisor_shop::affichtext()
{
	Uint32 charH = mega_print->getCharHgt();
	Sint32 x_pos = 60 * resolution;
	Sint32 y_pos = 180 * resolution;
	Sint32 yspac = charH + resolution;
  game_screen->clear (0, x_pos, y_pos, 22 * 8 * resolution, 3 * yspac);
	mega_print->bufferAff2(x_pos, y_pos, shop_line1, 22);
	mega_print->bufferAff2(x_pos, y_pos + yspac, shop_line2, 22);
	mega_print->bufferAff2(x_pos, y_pos + yspac * 2 , shop_line3, 22);
}

//-------------------------------------------------------------------------------
// drag and drop the gadget
//-------------------------------------------------------------------------------
void supervisor_shop::sh_ballade()
{
	if(get_object >= 0)	//pointer to the table "case_price" (-1 = no drag object)
	{	if(keyboard->is_left_button())
		{	bob_volant->enable();
			bob_volant->set_coordinates(mouse_pointer->get_x_coord(), mouse_pointer->get_y_coord());
			if(bobclignot->is_enabled)
				bobclignot->is_enabled = 0;
			else
				bobclignot->is_enabled = 1;
		}
		else
		{	
		
		
			bob_volant->disable();
			bobclignot->is_enabled = 1;
			Sint32 i = cadre_offs;
			if(i >= 0)
			{	if(i >= bonusachat)
					i = bonusachat - 1; 
				Sint32 *p0 = current_player->get_course();
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
					p0 = current_player->get_course();
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
				current_player->set_cou_nb(bonusachat);
				
				//check bonus price
				Sint32 price = 0;
				if(current_player->get_Bprice())
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
				current_player->add_credit(price);
			}
			get_object = -1;
		}
	}

	//###################################################################
	// get a object with the mouse
	//###################################################################
	else
	{	bob_volant->disable();
		pt_get_obj = -1;
		if(keyboard->is_left_button())
		{	Sint32 i = cadre_offs;
			if(i >=0) 
			{	Sint32 *p = current_player->get_course();
				sprite_capsule **liste = gereGadget->get_sprites_list();
				courseList = p + i;
				bobclignot = *(liste + i);
				get_object = *(sh_tablept + i);
				bob_volant->copiegadet(bobclignot);
				bobclignot->is_enabled = 1;
			}
		}
	}
}

//-------------------------------------------------------------------------------
// calculate position of box cursor
//-------------------------------------------------------------------------------
void supervisor_shop::pos_select()
{ 
	Sint32 y = mouse_pointer->get_y_coord() - cadre_ymin;
	Sint32 o = (y / cadre_haut);			//y / 9 (height of cursor)
	y = o * cadre_haut + cadre_ymin;
	if(y < cadre_ymin)
		y = cadre_ymin;
	if(y > cadre_ymax)
		y = cadre_ymax;
	cadre_posy = y ;				//cursor y coordinate
	
	Sint32 x = mouse_pointer->get_x_coord();
	y = mouse_pointer->get_y_coord();
	cadre_flag = 0;					//don't display cursor (by default)
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
void supervisor_shop::aff_select()
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
		char* pBuff = game_screen->get_pixel_data(x, y);
		Sint32 nextl = game_screen->get_row_size();
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
void supervisor_shop::tu_triches()
{
	mouse_pointer->set_frame_period(3);
	if(cheat_flag) return;
	if(!birth_flag) return;
	if(!mouse_pointer->get_x_coord() && !mouse_pointer->get_y_coord())
	{	mouse_pointer->set_frame_period(20);
		Sint32 k = keyboard->get_key_down_code();
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
Sint32 supervisor_shop::case_types[] =
{	GAD_SIZE_P, GAD_FIRE01, GAD_FIRE02, GAD_REBUIL, GAD_BALLE2, GAD_BALLE3,
 	GAD_POWER1, GAD_POWER2, GAD_LESSBR, GAD_LIFE_P, GAD_INFORM, GAD_WALL01,
	GAD_BUMP04, GAD_BUMP03, GAD_BUMP02, GAD_SIZE01, GAD_SIZE02, GAD_ROBOT1,
	GAD_CONTRO, GAD_GLUE00, GAD_EXITSH, GAD_EXITSH, GAD_EXITSH, GAD_EXITSH,
	GAD_EXITSH, GAD_EXITSH, GAD_EXITSH, GAD_EXITSH, GAD_EXITSH, GAD_EXITSH
};

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
Sint32 supervisor_shop::sh_tablept[NB_OPTIONS];

//-------------------------------------------------------------------------------
// Prix des gadgets 
//-------------------------------------------------------------------------------
Sint32 supervisor_shop::case_price[] =
{	60, 75, 150, 350, 25, 50,
	250, 500, 400, 450, 10, 75,
	100, 100, 100, 60, 75, 100,
	60, 150, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0
};

//-------------------------------------------------------------------------------
// texts 
//-------------------------------------------------------------------------------
char supervisor_shop::shoptext00[] =
{	"WELCOME ...... TO THE " \
	"  TECNOBALL-Z SHOP    " \
	"                      " \
	"PRICE BONUS IS ENABLE "
};
//...............................................................................
char supervisor_shop::shoptext41[] =
{	"SORRY, BUT YOU HAVEN'T" \
	"   GOT ENOUGH MONEY   " \
	"  TO BUY THIS OPTION  "
};
//...............................................................................
char supervisor_shop::shoptext63[] =
{	"                     " \
	"  YOU CAN ONLY BUY   " \
	"      .. GADGETS     " \
	"                     "
};
//...............................................................................
char supervisor_shop::shoptext56[] =
{	"  SORRY  THIS OPTION  " \
	"   CAN ONLY BE USED   " \
	"      FOR AREA 5      "
};
//...............................................................................
char supervisor_shop::shoptextPT[]=
{	 0, 1, 2, 3, 4, 5,		//S+/F1/F2/RW/B2/B3
	 6, 7, 8, 9,10,11,		//P1/P2/LB/L+/??/WA
	12,13,14,15,16,17,		//BL/BU/BR/S2/S3/RB
	18,19,20,20,20,20,		//CT/GL/XX/XX/XX/XX
	20,20,20,20,20,20,		//XX/XX/XX/XX/XX/XX
};

//*- S+ --------------------------------------------------------------------*
char supervisor_shop::shoptext12[] =
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
	"     ROBOT BUMPER!    " \
//*- CT --------------------------------------------------------------------*
	"          OK          " \
	"       FOR  THE       " \
	" CONTROL BALL OPTION  " \
//*- GL --------------------------------------------------------------------*
	"          OK          " \
	"       FOR  THE       " \
	"     GLUE  OPTION     " \
//*- XX --------------------------------------------------------------------*
	"      GO AND SEE      " \
	"        MY LOVE       " \
	"                      " \
};

char supervisor_shop::info_text1[] =
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
	"      TECNOBALLZ      " \
	"      TLK  GAMES      " \

	"  LA CORRUPTION REVET " \
	"   DES  DEGUISEMENTS  " \
	"        INFINIS       " \
};

char supervisor_shop::info_text2[] =
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
char supervisor_shop::info_text3[] =
{	
	" THE PASSWORD FOR THIS" \
	"  AREA IS ..........  " \
	" NO AVAILABLE PASSWORD" \
	" FOR THIS AREA.       " \
};

const unsigned char supervisor_shop::cyclingtab[] =
{	239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
	253, 254, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244,
	243, 242, 241, 240, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
	249, 250, 251, 252, 253, 254, 255
};
