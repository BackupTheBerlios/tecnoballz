//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "print_menu.cc"
// created	: ?
// updates	: 2005-01-15
// fonctions	: display of the text of the menu in the menu principal
// id		: $Id: print_menu.cc,v 1.17 2007/01/29 12:30:26 gurumeditation Exp $
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
#include "../include/print_menu.h"
#include "../include/handler_resources.h"
#include "../include/handler_keyboard.h"
#include "../include/joueurData.h"
#include "../include/scoretable.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
print_menu::print_menu()
{
	clear_sprite_members();
	numeroMenu = 0;
	graphTexte = (bitmap_data*)NULL;
	yPressLeft = -10240;
	yPressRigh = -10240;
	width_font = 8 * resolution;
	heightfont = 8 * resolution;
	if(resolution == 2)
		space2next = 17;
	else
		space2next = 8;
	menu_colww = 0;
	menu_color = 0;

	clear_addr = (char *)NULL;
	clearHeigh = 0;
	clearWidth = 0;
	curs_tempo = 0;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
print_menu::~print_menu()
{
	if(graphTexte)
		delete graphTexte;
	if(GFX_fontes)
		delete GFX_fontes;
	GFX_fontes = (bitmap_data *)NULL;
	graphTexte = (bitmap_data *)NULL;
}

//-----------------------------------------------------------------------------
//	perform some initializations
//-----------------------------------------------------------------------------
Sint32 print_menu::first_init()
{

	error_init(init_print(handler_resources::RESFONTMEN));
	if(erreur_num) return (erreur_num);
		
	//###################################################################
	// allocate 512 * 323 pixels buffer for text menu 
	//###################################################################
	graphTexte = new bitmap_data();
	graphTexte->create(MENU_LARGE * width_font, MENU_HAUTE * space2next , 1);

	//###################################################################
	// initialize sprite object
	//###################################################################
	make_sprite(graphTexte);
	enable();
	set_coordinates(32 * resolution, 80 * resolution);
	
	//###################################################################
	// initialize palette color chars
	//###################################################################
	SDL_Color *palPT = display->paletteAdr();
	SDL_Color *palP1 = palPT + 239;
	Sint32 i = hasard_val & 0x0F;
	if(i >= 10)
		i = i - 10;
	const Uint32 *ptpal = (handler_resources::tabledegas + i * 18);
	for(i = 0; i < 17; i++)
	{	Uint32 vacol = ptpal[i];
		Uint32 vablu = vacol & 0x000000ff;
		Uint32 vagre = vacol & 0x0000ff00;
		vagre = vagre >> 8;
		Uint32 vared = vacol & 0x00ff0000;
		vared = vared >> 16;
		palP1->r = vared;
		palP1->g = vagre;
		palP1->b = vablu;
		palP1++;
	}
	display->enable_palette(palPT); 
	return erreur_num;
}

//------------------------------------------------------------------------------
// display the text menu into buffer memory
//------------------------------------------------------------------------------
Sint32 print_menu::afficheTxt()
{ 
	clear_zone();
	mis_a_jour();
	Sint32 mousY = keyboard->get_mouse_y();
	Sint32 y = (mousY - y_coord) / space2next;
	Sint32 zeRet = testLeMenu();

	//###################################################################
	// read color table offset (color line hover by mouse )
	//###################################################################
	if(menu_color++ > 32)
		menu_color = 0;
	Sint32 color = menu_color;

	//###################################################################
	// display menu text	
	//###################################################################
	char *desP1 = pixel_data;
	Sint32 offSc = off_source;
	Sint32 offDs = srceNextLn;
	Sint32 offD2 = srceNextLn * (space2next - 1);
	Sint32 *basPT = (Sint32 *)caract_adr;
	char *p = menu_liste[numeroMenu];
	char *c = ascii2code;
	Sint32 a, b, j;

	if(resolution == 1)
	{
		//######################################################
		// mode low-res (320 x 200)
		//######################################################
		for(Sint32 k = 0; k < MENU_HAUTE; k++, desP1 += offD2)
		{	if(y != k)
			{	
				//######################################
				// display normal line of 32 characters
				//######################################
				for(j = 0; j < MENU_LARGE; j++)
				{	a = *(p++) - 32;
					if(a)
					{	b = c[a];
						b = b << 3;
#ifndef BYTES_COPY
						Sint32 *s = (Sint32 *)basPT;
						Sint32 *d = (Sint32 *)desP1;
						s = (Sint32 *)((char *)s + b);
						for(b = 0; b < 8; b++)
						{	d[0] = s[0];
							d[1] = s[1];
							s = (Sint32 *)((char *)s + offSc);
							d = (Sint32 *)((char *)d + offDs);
						}
#else

						char *s = (char *)basPT;
						char *d = desP1;
						s += b;
						for(b = 0; b < 8; b++)
						{	d[0] = s[0];
							d[1] = s[1];
							d[2] = s[2];
							d[3] = s[3];
							d[4] = s[4];
							d[5] = s[5];
							d[6] = s[6];
							d[7] = s[7];
							s += offSc;
							d += offDs;
						}
#endif
					}
					desP1 = desP1 + 8;
				}
			}
			else
			{	//######################################
				// display selected line of 32 characters
				//######################################
				for(j = 0; j < MENU_LARGE; j++)
				{	unsigned char pixel = cyclingtab[color];
					char a = *(p++) - 32;
					if(a)
					{	b = c[a];
						unsigned char *s = (unsigned char *)basPT;
						unsigned char *d = (unsigned char *)desP1;
						b = b << 3;
						s = s + b;
						for(b = 0; b < 8; b++)
						{	for(Sint32 z = 0; z < 8; z++)
							{	a = s[z];
								if(a)
								{	a = pixel;
									d[z] = pixel;
								}
							}
							s = s + offSc;
							d = d + offDs;
						}
					}
					desP1 = desP1 + 8;
					if(color++ > 32) color = 0;
				}
			}
		}
	}



	//##############################################################
	// mode hi-res (640 x 400)
	//##############################################################
	else
	{
	
		for(Sint32 k = 0; k < MENU_HAUTE; k++, desP1 += offD2)
		{	if(y != k)
			{	
				//###########################################################
				// display normal line of 32 characters
				//###########################################################
				for(j = 0; j < MENU_LARGE; j++)
				{	a = *(p++) - 32;
					if(a)
					{	b = c[a];
						b = b << 4;
#ifndef BYTES_COPY
						Sint32 *s = (Sint32 *)basPT;
						Sint32 *d = (Sint32 *)desP1;
						s = (Sint32 *)((char *)s + b);
						for(b = 0; b < 16; b++)
						{	d[0] = s[0];
							d[1] = s[1];
							d[2] = s[2];
							d[3] = s[3];
							s = (Sint32 *)((char *)s + offSc);
							d = (Sint32 *)((char *)d + offDs);
						}
#else

						char *s = (char *)basPT;
						char *d = desP1;
						s += b;
						for(b = 0; b < 16; b++)
						{	d[0] = s[0];
							d[1] = s[1];
							d[2] = s[2];
							d[3] = s[3];
							d[4] = s[4];
							d[5] = s[5];
							d[6] = s[6];
							d[7] = s[7];
							d[8] = s[8];
							d[9] = s[9];
							d[10] = s[10];
							d[11] = s[11];
							d[12] = s[12];
							d[13] = s[13];
							d[14] = s[14];
							d[15] = s[15];
							s += offSc;
							d += offDs;
						}
#endif
					}
						desP1 = desP1 + 16;
				}
			}
			else
			{	//###########################################################
				// display selected line of 32 characters
				//###########################################################
				for(j = 0; j < MENU_LARGE; j++)
				{	unsigned char pixel = cyclingtab[color];
					char a = *(p++) - 32;
					if(a)
					{	b = c[a];
						unsigned char *s = (unsigned char *)basPT;
						unsigned char *d = (unsigned char *)desP1;
						b = b << 4;
						s = s + b;
						for(b = 0; b < 16; b++)
						{	for(Sint32 z = 0; z < 16; z++)
							{	a = s[z];
								if(a)
								{	a = pixel;
									d[z] = pixel;
								}
							}
							s = s + offSc;
							d = d + offDs;
						}
					}
					desP1 = desP1 + 16;
					if(color++ > 32) color = 0;
				}
			}
		}

	}
	curs_print();
	return zeRet;
}

//------------------------------------------------------------------------------
// handle menu events
//------------------------------------------------------------------------------
Sint32 print_menu::testLeMenu()
{
	Sint32 zeRet = 0;
	Sint32 mposx, pos_y, freeL, freeR;
	
	//##############################################################
	// check if right or left button are pressed
	//##############################################################
	Sint32 presL = keyboard->is_left_button();
	Sint32 presR = keyboard->is_right_button();
	
	//##############################################################
	// read y where is pressed 
	//##############################################################
	if(presL && yPressLeft == YCOORDNULL)
	{	yPressLeft = keyboard->get_mouse_y();
		//printf("yPressLeft: %i \n",yPressLeft);
	}
	else
	{	if(presR && yPressRigh == YCOORDNULL)
		{	yPressRigh = keyboard->get_mouse_y();
			//printf("yPressRigh: %i \n",yPressRigh);
		}
	}

	freeR = 0;	
	freeL = keyboard->is_left_button_up(&mposx, &pos_y);
	if(!freeL)
		freeR = keyboard->is_right_button_up(&mposx, &pos_y);	

	if((freeL && pos_y == yPressLeft) || (freeR && pos_y == yPressRigh))
	{	Sint32 incre = 0;
		if(freeL)
		{	incre = 1;
			yPressLeft = YCOORDNULL;
		}
		if(freeR)
		{	incre = -1;
			yPressRigh = YCOORDNULL;
		}
		
		//printf("pos_y: %i \n", pos_y);
		pos_y = (pos_y - y_coord) / space2next;
		switch (numeroMenu)
		{
			
			//###########################################################
			// the main menu
			//###########################################################
			case MENU_ENTRE:
				switch (pos_y)
				{	case LINE_START:
						efface_BOB();
						clear_stop();
						zeRet = 2;
						break;
					case LINE_PARAM:
						efface_BOB();
						clear_stop();
						numeroMenu = MENU_PARAM;
						break;
					case LINE_ABOUT:
						efface_BOB();
						clear_stop();
						numeroMenu = MENU_ABOUT;
						break;
					case LINE_SALUT:
						efface_BOB();
						clear_stop();
						numeroMenu = MENU_SALUT;
						break;
					case LINE_INFOS:
						efface_BOB();
						clear_stop();
						numeroMenu = MENU_INFOS;
						break;
					case LINE_SCORE:
						efface_BOB();
						clear_stop();
						copyScores();
						numeroMenu = MENU_SCORE;
						break;
					
					// input area password
					case LINE_CODE1:
					case LINE_CODE2:
						input_init(10, LINE_CODE2, 10, &zeAreaCode[0]);
						break;
					case LINE_SORTI:
						zeRet = 1;
					break;
				}
			break;

			//###########################################################
			// options
			//###########################################################
			case MENU_PARAM:
				switch (pos_y)
				{	
					case 5:
						clear_init(24, 5, 1, 1);
						if(++nuOfPlayer > MAX_PLAYER)
							nuOfPlayer = 1;
						break;
				
					// input players names
					case 6:
						input_init(24, 6, 6,
							joueurData::playerlist[0]->returnName());
						break;
					case 7:
						input_init(24, 7, 6,
							joueurData::playerlist[1]->returnName());
						break;
					case 8:
						input_init(24, 8, 6,
							joueurData::playerlist[2]->returnName());
						break;
					case 9:
						input_init(24, 9, 6,
							joueurData::playerlist[3]->returnName());
						break;
					case 10:
						input_init(24, 10, 6,
							joueurData::playerlist[4]->returnName());
						break;
					case 11:
						input_init(24, 11, 6,
							joueurData::playerlist[5]->returnName());
						break;

					//
					case 12:
						clear_init(24, 12, 4, 1);
						hardChoice += incre;
						if(hardChoice > 4)
							hardChoice = 1;
						if(hardChoice < 1)
							hardChoice = 4;
						mis_a_jour();
						break;

					case 13:
						clear_init(24, 13, 2, 1);
						vieInitial += incre;
						if(vieInitial > 9)
							vieInitial = 1;
						if(vieInitial < 1)
							vieInitial = 9;
						mis_a_jour();
						break;
						
					//return to main menu
					case 14:
						efface_BOB();
						numeroMenu = MENU_ENTRE;		
						clear_stop();
						break;
				}
				break;
		
			case MENU_ABOUT:
				efface_BOB();
				numeroMenu = MENU_ENTRE;
				break;

			case MENU_INFOS:
				efface_BOB();
				numeroMenu = MENU_ENTRE;
				break;

			case MENU_SALUT:
				efface_BOB();
				numeroMenu = MENU_ENTRE;
				break;

			case MENU_SCORE:
				efface_BOB();
				numeroMenu = MENU_ENTRE;
				break;
		}
	}
	if(!presL)
		yPressLeft = YCOORDNULL;
	if(!presR)
		yPressRigh = YCOORDNULL;

	return zeRet;
}

//------------------------------------------------------------------------------
// update strings menu (passwords, players names, difficulty, num of lifes)
//------------------------------------------------------------------------------
void print_menu::mis_a_jour()
{
	const char *s;
	char *d;

	//###########################################################
	// copy password
	//###########################################################
	d = menuTexte0 + (MENU_LARGE * LINE_CODE2) + 10;
	s = &zeAreaCode[0];
	for(Sint32 i = 0; i < 10; i++)
		d[i] = s[i];

	//###########################################################
	// number of players
	//###########################################################
	d = menuTexte1 + (MENU_LARGE * 5) + 24;
	intToASCII(nuOfPlayer, d, 0);
	
	//###########################################################
	// copy playes names
	//###########################################################
	d = menuTexte1 + (MENU_LARGE * 6) + 24;
	for(Sint32 i = 0; i < MAX_PLAYER; i++)
	{	s = joueurData::playerlist[i]->returnName();
		for(Uint32 j = 0; j < 6; j++)
			d[j] = s[j];
		d += MENU_LARGE;
	}
	s = &difficulte[(hardChoice - 1) * 4];
	d = menuTexte1 + (MENU_LARGE * 12) + 24;
	for(Sint32 i = 0; i < 4; i++)
		d[i] = s[i];
	d = menuTexte1 + (MENU_LARGE * 13) + 24;
	intToASCII(vieInitial, d, 1);

	birth_flag = 1;
	//for(Uint32 i = 0; i < 1; i++) //test only
	for(Sint32 i = 0; i < MAX_PLAYER; i++)
	{	s = joueurData::playerlist[i]->returnName();
		if(s[0] != '0' || s[1] != '4' || s[2] != '0' || 
			s[3] != '6' || s[4] != '7' || s[5] != '0')
			birth_flag = 0;
	}		
}

//------------------------------------------------------------------------------
// erase all memory buffer (used to display the text of the menu)
//------------------------------------------------------------------------------
void print_menu::efface_BOB()
{
	Sint32 *d = (Sint32 *)pixel_data;
	Sint32 p = 0;
	Sint32 n = srceNextLn / 4;
	Sint32 h = sprite_height;
	Sint32 l = srceNextLn / 4;
	for(Sint32 i = 0; i < h; i++)
	{	for(Sint32 j = 0; j < l; j++)
			d[j] = p;
		d += n;
	}
}

//------------------------------------------------------------------------------
// clear a zone of the menu (used to modify a string of the menu)
//------------------------------------------------------------------------------
void print_menu::clear_zone()
{
	if(!clear_addr) return;
	Uint32 n = srceNextLn / 4;
	Uint32 z = 0x0;
	Uint32 *d = (Uint32 *)clear_addr;
	for(Uint32 h = 0; h < clearHeigh; h++)
	{	for(Uint32 w = 0; w < clearWidth; w++)
			d[w] = z;
		d += n;
	}
}

//------------------------------------------------------------------------------
// display the cursor (input string)
//------------------------------------------------------------------------------
void print_menu::curs_print()
{
	if(!clear_addr) return;
	Sint32 xcurs = keyboard->get_input_cursor_pos();
	if(xcurs < 0) return;
	if(--curs_tempo < 1) curs_tempo = 50;
	if(curs_tempo > 30) return;
	char z= 0xEE;
	char *d = clear_addr + (xcurs * width_font);
	Uint32 n = srceNextLn;
	for(Sint32 h = 0; h < heightfont; h++)
	{	for(Sint32 w = 0; w < width_font; w++)
		{	if (!d[w])
				d[w] = z;
		}
		d += n;
	}
}

//------------------------------------------------------------------------------
// initialize string input
//------------------------------------------------------------------------------
void print_menu::input_init(Uint32 xcoor, Uint32 ycoor, Uint32 width,
								char* strng)
{
	clear_init(xcoor, ycoor, width, 1);
	if(!strng) return;
	curs_tempo = 50;
	keyboard->set_input_string(strng, width);
}

//------------------------------------------------------------------------------
// initialize a "zone clear" (used to modify a string of the menu)
//------------------------------------------------------------------------------
void print_menu::clear_init(Uint32 xcoor, Uint32 ycoor, Uint32 width,
								Uint32 lines)
{
	clear_stop();
	clear_addr = pixel_data + (ycoor * space2next * srceNextLn) +
					(xcoor * width_font);
	clearWidth = (width * width_font) / 4;
	clearHeigh = lines * heightfont;
}

//------------------------------------------------------------------------------
// stop the "zone clear"
//------------------------------------------------------------------------------
void print_menu::clear_stop()
{
	clear_addr = (char *)NULL;
	keyboard->stop_string_input();
}

//------------------------------------------------------------------------------
// copy scores table into menu text
//------------------------------------------------------------------------------
void print_menu::copyScores()
{
	score_list* score = ptScoreTab->getScrList();
	if(!score) return;
	char* ptext = menuTexte5 + MENU_LARGE * 6;
	for(Uint32 i = 0; i < scoretable::NUMBSCORES; i++)
	{	char *pName = score[i].playerName;
	
		for(Uint32 j = 0; j < 6; j++)
			ptext[6 + j] = pName[j];
		intToASCII(score[i].scoreValue, &ptext[24], 5);
		intToASCII(score[i].score_area, &ptext[19], 0);
		intToASCII(score[i].scoreLevel, &ptext[13], 1);
		ptext += MENU_LARGE;
	}
}

//------------------------------------------------------------------------------
// strings of the main menu
//------------------------------------------------------------------------------
char const print_menu::difficulte[] = "EASY" "HARD" "MAD " "DEAD";


char print_menu::menuTexte0[] =
	"                                "	//0
	"                                "	//1
	"                                "	//2
	"                                "	//3
	"                                "	//4
	"        START THE GAMEq         "	//5
	"          ? OPTIONS ?           "	//6
	"          > CREDITS <           "	//7
	"         g GREETINGS g          "	//8
	"             INFOS              "	//9
	"          VIEW-SCORES           "	//10 
	"           PASSWORDq            "	//11
	"        u ----------? t         "	//12
	"          GAME  EXITw           "	//13
	"                                "	//14
	"                                "	//15
	"                                "	//16
	"                                "	//17 
	"                                ";	//18

char print_menu::menuTexte1[] =
	"                                "
	"                                "
	"                                "
	"                                "
	"--------------------------------"
	" e NUMBER OF PLAYERS  u 1 t     "
	" b PLAYER 1.......... u ......t "	//6
	" b PLAYER 2.......... u ......t "
	" b PLAYER 3.......... u ......t "
	" b PLAYER 4.......... u ......t "
	" b PLAYER 5.......... u ......t "
	" b PLAYER 6.......... u ......t "
	" e DIFFICULTY........ u EASY t  "	// 12
	" e NUMBER OF LIFES... u 08 t    "	// 13
	" c MAIN MENUqq                  "
	"--------------------------------"
	"                                "
	"                                "
	"                                ";

char print_menu::menuTexte2[] =
	"--- MAIN CODING - LINUX-PORT ---"
	"      b BRUNO  ETHVIGNOT b      "
	"---- BASED ON AMIGA MC680X0 ----" 
	"b BRUNO ETHVIGNOT;JEROME BOLOT b"
	"                                "
	"----------- MAIN GFX -----------"
	"bJEAN MICHEL  MARTIN DE SANTEROb"
	" --------- OTHER  GFX --------- "
	"       b RODOLPHE  BONO b       "
	"                                "
	"---------- ALL MUSICS ----------"
	"        b REGIS PARRET b        "
	"---------SOUNDS EFFECTS---------"
	"       b LAURENT  GUYON  b      "
	"                                "
	"COPYRIGHT (C)1992-2005 TLK-GAMES"
	"TLK-GAMES/BP 24/F81150 FLORENTIN"
	"         LINUX.TLK.FR           "
	"                                ";

char print_menu::menuTexte3[] =
	"                                "
	" BUMPER CONTROL:                "
	" LEFT MOUSE BUTTON : FIRE       "
	" RIGHT MOUSE BUTTON: DROP BALL  "
	" LEFT AND RIGHT    : TILT       "
	"                     GIGABLITZ  "
	"--------------------------------"
	"KEYS RECOGNIZED DURING THE GAME:"
	" F       : FULL SCREEN          "
	" P       : PAUSE                "
	" CTRL ESC: QUIT TECNOBALLZ      "
	" CTRL D  : DISABLE MFX          "
	" CTRL F  : DISABLE SFX          "
	" CTRL S  : DISABLE SFX - MFX    "
	" CTRL Q  : EXIT TO MENU         "
	" CTRL X  : GO TO GAME-OVER      "
	"--------------------------------"
	" 25,000 POINTS      : BONUS LIFE"
	"                                "
	"                                ";

char print_menu::menuTexte4[] =
	"                                "
	"---> BEST GREETINGS  FLY TO <---"
	"                                "
	" b ALEXIS       b AURELIEN      "
	" b BLACKGUARD   b BARTI         "
	" b DAUBMAN      b DELPHINUS     "
	" b DARK NIGHT   b FELBRAN       "
	" b DJI          b JRC           "
	" b MA DANONE    b LE CHACAL     "
	" b LE TEXAN     b YANIS         "
	" b JMM          b ROY           "
	" b PAT          b PROPERMAN     "
	" b PASCAL L.    b PASCAL E.     "
	" b PIXELMAN     b PIERRE DENIS  "
	" b STEPHANE C.  b POPOLON       "
	" b ZIBIBI       b SHAD          "
	" b REGLIS       b ZE-KING       "
	"                                "
	"                                ";

char print_menu::menuTexte5[] =
	"                                "
	"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
	"b        BEST  TECNOBALLZ      b"
	"b                              b"
	"b POS NAME   LEVEL AREA SCORE  b"
	"b                              b"
	"b 01  ?????? ??    ?    ?????? b"
	"b 02  ?????? ??    ?    ?????? b"
	"b 03  ?????? ??    ?    ?????? b"
	"b 04  ?????? ??    ?    ?????? b"
	"b 05  ?????? ??    ?    ?????? b"
	"b 06  ?????? ??    ?    ?????? b"
	"b 07  ?????? ??    ?    ?????? b"
	"b 08  ?????? ??    ?    ?????? b"
	"b 09  ?????? ??    ?    ?????? b"
	"b 10  ?????? ??    ?    ?????? b"
	"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
	"                                "
	"                                ";


char * print_menu::menu_liste[MENU_TOTAL] =
{	menuTexte0, menuTexte1, menuTexte2,
	menuTexte3, menuTexte4, menuTexte5
};

//------------------------------------------------------------------------------
// offsets of the first 128 ASCII characters 
//------------------------------------------------------------------------------
char print_menu::ascii2code[128] =
{	26,	// 32 ' '
	37,	// 33 '!'
	40,	// 34 '"'
	26,	// 35 '#' space
	26,	// 36 '$' space
	26,	// 37 '%' space
	26,	// 38 '&' space
	40,	// 39 "'"
	48,	// 40 '('
	49,	// 41 ')'
	47,	// 42 '*'
	26,	// 43 '+' space
	26,	// 44 ',' 
	41,	// 45 '-'
	42,	// 46 '.'
	26,	// 47 '/' space
	27,	// 48 '0'
	28,	// 49 '1'
	29,	// 50 '2'
	30,	// 51 '3'
	31,	// 52 '4'
	32,	// 53 '5'
	33,	// 54 '6'
	34,	// 55 '7'
	35,	// 56 '8'
	36,	// 57 '9'
	39,	// 58 ':'
	38,	// 59 ';'
	44,	// 60 '<'
	26,	// 61 '=' space
	45,	// 62 '>'
	43,	// 63 '?'
	26,	// 64 '@' space
	0,	// 65 'A'
	1,	// 66 'B'
	2,	// 67 'C'
	3,	// 68 'D'
	4,	// 69 'E'
	5,	// 70 'F'
	6,	// 71 'G'
	7,	// 72 'H'
	8,	// 73 'I'
	9,	// 74 'J'
	10,	// 75 'K'
	11,	// 76 'L'
	12,	// 77 'M'
	13,	// 78 'N'
	14,	// 79 'O'
	15,	// 80 'P'
	16,	// 81 'Q'
	17,	// 82 'R'
	18,	// 83 'S'
	19,	// 84 'T'
	20,	// 85 'U'
	21,	// 86 'V'
	22,	// 87 'W'
	23,	// 88 'X'
	24,	// 89 'Y'
	25,	// 90 'Z'
	48,	// 91 '['
	26,	// 92 '\' space
	49,	// 93 ']'
	26,	// 94 '^' space
	26,	// 95 '_' space
	26,	// 96 '`' space
	26,	// 97 'a' space
	47,	// 98 'b' gray star 
	52,	// 99 'c' horizontal lines 
	26,	// 100 'd' space
	50,	// 101 'e' full square
	26,	// 102 'f' space
	46,	// 103 'g' gray heart 
	26,	// 104 'h' space
	26,	// 105 'i' space
	26,	// 106 'j' space
	26,	// 107 'k' space
	26,	// 108 'l' space
	26,	// 109 'm' space
	26,	// 110 'n' space
	26,	// 111 'o' space
	26,	// 112 'p' space
	55,	// 113 'q' ! white
	26,	// 114 'r' space
	26,	// 115 's' space
	53,	// 116 't' < white
	54,	// 117 'u' > white
	26,	// 118 'v' space
	51,	// 119 'w' pink ellipsis 
	26,	// 120 'x' space
	26,	// 121 'y' space
	26,	// 122 'z' space
	26,	// 123 '{' space
	26,	// 124 '|' space
	26,	// 125 '}' space
	26,	// 126 '~' space
	26	// 127 ' ' space
};

const unsigned char print_menu::cyclingtab[] =
{	239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
	253, 254, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244,
	243, 242, 241, 240, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
	249, 250, 251, 252, 253, 254, 255
} ;
