//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "clavierMac.cc"
// created	: ?
// updates	: 2005-06-17
// fonction	: handle keyboard and mouse
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
#include "../include/clavierMac.h"
#include "../include/handler_display.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
clavierMac::clavierMac()
{
	mentatInit();
	grabs_flag = 0;
	sourisPosX = 0;
	sourisPosY = 0;
	sourisOffx = 0;
	sourisOffy = 0;
	mo_point_h = 0;		//x coordinates of the mouse
	mo_point_v = 0;		//y coordinates of the mouse
	mo_pointhr = 0;		//relative motion in the X direction
	mo_pointvr = 0;		//relative motion in the Y direction
	bou_gauche = 0;
	bou_droite = 0;
	bou_milieu = 0;
	mouspressx = -1024;	//position x when the left button was pressed
	mouspressy = -1024;	//position y when the left button was pressed
	mouspresse = 0;
	mousreleas = 0;
	releaseRgt = 0;
	inputTempo = 0;
	codePress1 = 0;
	codePress2 = 0;
	cursor_pos = 0;
	curs_state = 0;
	input_size = 0;
	code_keyup = 0;
	ptStrInput = NULL;
	souris_off();

	for(Uint32 i = 0; i < NUMOFFLAGS; i++)
	{	keys_flags[i] = 0;
		keys_lasts[i] = 0;
	}
}

//-------------------------------------------------------------------------------
// release the object
//-------------------------------------------------------------------------------
clavierMac::~clavierMac()
{
	mentatKill();
}

//-------------------------------------------------------------------------------
// grabs mouse and keyboard input
//-------------------------------------------------------------------------------
void clavierMac::setGrab_On()
{
	grabs_flag = 1;
	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_GetRelativeMouseState(0,0);
}

//-------------------------------------------------------------------------------
// disable grabs mouse and keyboard input
//-------------------------------------------------------------------------------
void clavierMac::setGrabOff()
{
	grabs_flag = 0;
	SDL_WM_GrabInput(SDL_GRAB_OFF);
}

//-------------------------------------------------------------------------------
// read events (keyboard, mouse, exit)
//-------------------------------------------------------------------------------
void clavierMac::lit_keymap()
{
	keys_flags[FULLSCFLAG] = 0;
	keys_flags[WAITVBLOFF] = 0;
	mousreleas = 0;
	releaseRgt = 0;
	SDL_Event event;
	SDL_KeyboardEvent *kbEvt;
	while(SDL_PollEvent(&event) > 0)
	{	switch (event.type)
		{ 
			//###########################################################
			// mouse moving
			//###########################################################
			case SDL_MOUSEMOTION:
			{	mo_point_h = event.motion.x;
				mo_point_v = event.motion.y;
				mo_pointhr = event.motion.xrel;
				mo_pointvr = event.motion.yrel;
			}
			break ;

			//###########################################################
			// key down
			//###########################################################
			case SDL_KEYDOWN:
			{	kbEvt = (SDL_KeyboardEvent*) &event;
				Uint8 *keys = SDL_GetKeyState(NULL);
				
				if(	keys[SDLK_RALT] == SDL_RELEASED &&
					keys[SDLK_LALT] == SDL_RELEASED &&
					keys[SDLK_LSHIFT] == SDL_RELEASED &&
					keys[SDLK_RSHIFT] == SDL_RELEASED)
				{

					if(	keys[SDLK_RCTRL] == SDL_RELEASED &&
						keys[SDLK_LCTRL] == SDL_RELEASED)
					{
						if(keys[SDLK_p] == SDL_PRESSED)
							keys_lasts[PAUSE_FLAG] = 1;
						if(keys[SDLK_f] == SDL_PRESSED)
							keys_lasts[FULLSCFLAG] = 1;
						if(keys[SDLK_ESCAPE] == SDL_PRESSED)
							keys_lasts[ESCAPEMENU] = 1;
						if(keys[SDLK_l] == SDL_PRESSED)
							keys_lasts[WAITVBLOFF] = 1;
					}
					else
					{	if( keys[SDLK_ESCAPE] == SDL_PRESSED)
							keys_lasts[TOEXITFLAG] = 1;
						if( keys[SDLK_x] == SDL_PRESSED)
							keys_lasts[TOOVERFLAG] = 1;
						if( keys[SDLK_q] == SDL_PRESSED)
							keys_lasts[TOMENUFLAG] = 1;
						if( keys[SDLK_f] == SDL_PRESSED)
							keys_lasts[SOUND_FLAG] = 1;
						if( keys[SDLK_s] == SDL_PRESSED)
							keys_lasts[MFXSFXFLAG] = 1;
						if( keys[SDLK_d] == SDL_PRESSED)
							keys_lasts[MUSIC_FLAG] = 1;
					}
				}

				if(kbEvt->keysym.unicode > 0)
					initKcode1(kbEvt->keysym.unicode);
				else
					initKcode1(kbEvt->keysym.sym);
				
				/* fprintf(stdout,
					"keysym.unicode:%i keysym.sym:%i\n",
					kbEvt->keysym.unicode, kbEvt->keysym.sym);*/
			}
			break;

			//###########################################################
			// key up
			//###########################################################
			case SDL_KEYUP:
			{	kbEvt = (SDL_KeyboardEvent*) &event;
				Uint8 *keys = SDL_GetKeyState(NULL);

			
				// enable pause [P] key
				if(keys[SDLK_p] == SDL_RELEASED && keys_lasts[PAUSE_FLAG])
				{	keys_lasts[PAUSE_FLAG] = 0;
					if(!keys_flags[ESCAPEMENU])
					{	keys_flags[PAUSE_FLAG] = ~keys_flags[PAUSE_FLAG];
							if(grabs_flag && keys_flags[PAUSE_FLAG])
								SDL_WM_GrabInput(SDL_GRAB_OFF);
							if(grabs_flag && !keys_flags[PAUSE_FLAG])
								SDL_WM_GrabInput(SDL_GRAB_ON);
					}
				}

				// enable context menu [ESC] key
				if(keys[ESCAPEMENU] == SDL_RELEASED && keys_lasts[ESCAPEMENU])
				{	keys_lasts[ESCAPEMENU] = 0;
					keys_flags[ESCAPEMENU] = ~keys_flags[ESCAPEMENU];
					keys_flags[PAUSE_FLAG] = keys_flags[ESCAPEMENU];
						if(grabs_flag && keys_flags[PAUSE_FLAG])
							SDL_WM_GrabInput(SDL_GRAB_OFF);
						if(grabs_flag && !keys_flags[PAUSE_FLAG])
							SDL_WM_GrabInput(SDL_GRAB_ON);	

						if(!keys_flags[ESCAPEMENU])
							SDL_ShowCursor(SDL_DISABLE);
						if(keys_flags[ESCAPEMENU])
							SDL_ShowCursor(SDL_ENABLE); 						

						
						/*if(handler_display::optionfull && !keys_flags[ESCAPEMENU])
							SDL_ShowCursor(SDL_DISABLE);
						if(handler_display::optionfull && keys_flags[ESCAPEMENU])
							SDL_ShowCursor(SDL_ENABLE); */				
				}

				if(keys[SDLK_f] == SDL_RELEASED && keys_lasts[FULLSCFLAG])
				{	keys_lasts[FULLSCFLAG] = 0;
					keys_flags[FULLSCFLAG] = 1;
				}
				{	if(keys[SDLK_ESCAPE] == SDL_RELEASED && keys_lasts[TOEXITFLAG])
					{	keys_lasts[TOEXITFLAG] = 0;
						keys_flags[TOEXITFLAG] = 1;
					}
					if(keys[SDLK_x] == SDL_RELEASED && keys_lasts[TOOVERFLAG])
					{	keys_lasts[TOOVERFLAG] = 0;
						keys_flags[TOOVERFLAG] = 1;
					}
					if(keys[SDLK_q] == SDL_RELEASED && keys_lasts[TOMENUFLAG])
					{	keys_lasts[TOMENUFLAG] = 0;
						keys_flags[TOMENUFLAG] = 1;
					}
					if(keys[SDLK_f] == SDL_RELEASED && keys_lasts[SOUND_FLAG])
					{	keys_lasts[SOUND_FLAG] = 0;
						keys_flags[SOUND_FLAG] = 1;
					}
					if(keys[SDLK_s] == SDL_RELEASED && keys_lasts[MFXSFXFLAG])
					{	keys_lasts[MFXSFXFLAG] = 0;
						keys_flags[MFXSFXFLAG] = 1;
					}
					if(keys[SDLK_d] == SDL_RELEASED && keys_lasts[MUSIC_FLAG])
					{	keys_lasts[MUSIC_FLAG] = 0;
						keys_flags[MUSIC_FLAG] = 1;
					}
					if(keys[SDLK_l] == SDL_RELEASED && keys_lasts[WAITVBLOFF])
					{	keys_lasts[WAITVBLOFF] = 0;
						keys_flags[WAITVBLOFF] = 1;
					}
				}
				
				if(kbEvt->keysym.unicode > 0)
					initKcode2(kbEvt->keysym.unicode);
				else
					initKcode2(kbEvt->keysym.sym);
			}
			break;

			//###########################################################
			// mouse down
			//###########################################################
			case SDL_MOUSEBUTTONDOWN:
			{	switch(event.button.button)
				{ case SDL_BUTTON_LEFT:
					bou_gauche = 1;
					if(mouspresse == 0)
					{	mouspressx = mo_point_h;
						mouspressy = mo_point_v;
						mouspresse = 1;
					}
					break;
					case SDL_BUTTON_RIGHT:
						bou_droite = 1;
						break;
					case SDL_BUTTON_MIDDLE:
						bou_milieu = 1;
						break;
				}
			}
			break;

			//###########################################################
			// mouse up
			//###########################################################
			case SDL_MOUSEBUTTONUP:
			{	switch(event.button.button)
				{	case SDL_BUTTON_LEFT:
						bou_gauche = 0;
						if(mouspresse == 1)
							mousreleas = 1;
						mouspresse = 0;
					break;
					case SDL_BUTTON_RIGHT:
						if(bou_droite)
							releaseRgt = 1;
						bou_droite = 0;
						break;
					case SDL_BUTTON_MIDDLE:
						bou_milieu = 0;
						break;
				}
			}
			break;

			//###########################################################
			// quit the game
			//###########################################################
			case SDL_QUIT:
			{	keys_flags[TOEXITFLAG] = 1;
			}
			break ;
		}
	}
	hasard_val += mo_point_h;
	hasard_val += mo_point_v;
	inputKCode();
}

//-------------------------------------------------------------------------------
// test if a key is pressed
//-------------------------------------------------------------------------------
char clavierMac::test_Kcode(Sint16 x)
{
	Uint8 *keys;
	keys = SDL_GetKeyState(NULL);
	if ( keys[x] == SDL_PRESSED )
		return 1;
	else
		return 0;
}


//-------------------------------------------------------------------------------
// test if a key is released
//-------------------------------------------------------------------------------
char clavierMac::test_Rcode(Sint16 codek)
{
	Uint8 *keys;
	keys = SDL_GetKeyState(NULL);
	if ( keys[codek] == SDL_RELEASED )
		return 1;
	else
		return 0;
}

//-------------------------------------------------------------------------------
// reset pause state
//-------------------------------------------------------------------------------
void clavierMac::resetpause()
{
	keys_lasts[PAUSE_FLAG] = 0;
	keys_flags[PAUSE_FLAG] = 0;
	keys_lasts[TOMENUFLAG] = 0;
	keys_flags[TOMENUFLAG] = 0;
	keys_lasts[TOOVERFLAG] = 0;
	keys_flags[TOOVERFLAG] = 0;
	keys_lasts[MFXSFXFLAG] = 0;
	keys_flags[MFXSFXFLAG] = 0;
	keys_lasts[SOUND_FLAG] = 0;
	keys_flags[SOUND_FLAG] = 0;
	keys_lasts[MUSIC_FLAG] = 0;
	keys_flags[MUSIC_FLAG] = 0;
	keys_lasts[ESCAPEMENU] = 0;
	keys_flags[ESCAPEMENU] = 0;
	if(grabs_flag)
		SDL_WM_GrabInput(SDL_GRAB_ON);
	sourisPosX = mo_point_h;
	sourisPosY = mo_point_v;
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
Uint32 clavierMac::specialKey(Uint32 vFlag)
{
	return keys_flags[vFlag];
}
//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
Uint32 clavierMac::specKeyRaz(Uint32 vFlag)
{
	Uint32 i = keys_flags[vFlag];
	keys_flags[vFlag] = 0;
	return i;
}

//-------------------------------------------------------------------------------
// test if mouse left button is pressed
//-------------------------------------------------------------------------------
Sint32 clavierMac::leftButton()
{
	return bou_gauche;
}

//-------------------------------------------------------------------------------
// test if mouse right button is pressed
//-------------------------------------------------------------------------------
Sint32 clavierMac::righButton()
{
	return bou_droite;
}

//-------------------------------------------------------------------------------
// test if mouse right and left button are pressed (start gigablitz)
//-------------------------------------------------------------------------------
Sint32 clavierMac::buttonBlit()
{
	if(bou_droite && bou_gauche)
		return 1;
	else
		return 0;
}

//-------------------------------------------------------------------------------
// test if the mouse button were released 
//-------------------------------------------------------------------------------
Sint32 clavierMac::sourisRela(Sint32 *off_x, Sint32 *off_y)
{
	*off_x = mo_point_h;
	*off_y = mo_point_v;
	return mousreleas;
}
Sint32 clavierMac::isReleaseR(Sint32 *off_x, Sint32 *off_y)
{
	*off_x = mo_point_h;
	*off_y = mo_point_v;
	return releaseRgt;
}

//-------------------------------------------------------------------------------
// test offset of displacement of the mouse
//-------------------------------------------------------------------------------
void clavierMac::souris_off(Sint32 *off_x, Sint32 *off_y)
{
	sourisOffx = mo_point_h - sourisPosX;
	sourisOffy = mo_point_v - sourisPosY;
	sourisPosX = mo_point_h;
	sourisPosY = mo_point_v;
	hasard_val += sourisPosX;
	hasard_val += sourisPosY;
	*off_x = sourisOffx;
	*off_y = sourisOffy;
}

//-------------------------------------------------------------------------------
// test offset of displacement of the mouse
//-------------------------------------------------------------------------------
void clavierMac::souris_off()
{
	sourisOffx = mo_point_h - sourisPosX;
	sourisOffy = mo_point_v - sourisPosY;
	sourisPosX = mo_point_h;
	sourisPosY = mo_point_v;
	hasard_val += sourisPosX;
	hasard_val += sourisPosY;
}

//-------------------------------------------------------------------------------
// test offset of horizontal displacement of the mouse
//-------------------------------------------------------------------------------
Sint32 clavierMac::sourisoffX()
{
	sourisOffx = mo_point_h - sourisPosX;
	sourisOffy = mo_point_v - sourisPosY;
	sourisPosX = mo_point_h;
	sourisPosY = mo_point_v;
	hasard_val += sourisPosX;
	hasard_val += sourisPosY;
	return (sourisOffx);
}

//-------------------------------------------------------------------------------
// test offset of vertical displacement of the mouse
//-------------------------------------------------------------------------------
Sint32 clavierMac::sourisoffY()
{
	sourisOffx = mo_point_h - sourisPosX;
	sourisOffy = mo_point_v - sourisPosY;
	sourisPosX = mo_point_h;
	sourisPosY = mo_point_v;
	hasard_val += sourisPosX;
	hasard_val += sourisPosY;
	return sourisOffy;
}

//-------------------------------------------------------------------------------
// return absolute y-coordinate 
//-------------------------------------------------------------------------------
Sint32 clavierMac::sourisGetY()
{
	return mo_point_v;
}
//-------------------------------------------------------------------------------
// return absolute x-coordinate 
//-------------------------------------------------------------------------------
Sint32 clavierMac::sourisGetX()
{
	return mo_point_h;
}

//-------------------------------------------------------------------------------
// handle string input
//-------------------------------------------------------------------------------
Sint32 clavierMac::inputKCode()
{
	Sint32 kCode = 0;
	if(inputTempo < 1) 
	{	kCode = codePress1;
		if(codePress1 > 0)
		{	if(codePress2 != codePress1)	//it is key pressed for the first time? 
			{	codePress2 = codePress1;
				inputTempo = 30;
			}
			else
				inputTempo = 10;			//repeat key press times
		}
	}
	else
	{	kCode = 0;
		inputTempo--;
	}
	if(ptStrInput)
		inputKCode(kCode);
	/*
	if(codePress1 || kCode)
		printf("clavierMac::inputKCode(): codePress1:%i, kCode:%i, inputTempo=%i\n",
			codePress1, kCode, inputTempo);*/
	return kCode;
}

//-------------------------------------------------------------------------------
// new key pressed (handle string input)
//-------------------------------------------------------------------------------
void clavierMac::initKcode1(Sint32 kCode)
{
	if(	kCode != SDLK_LSHIFT && kCode != SDLK_RSHIFT && kCode != SDLK_LCTRL &&
		kCode != SDLK_RCTRL)
		codePress1 = kCode;
}
Sint32 clavierMac::get_Kcode1() 
{
	return 	codePress1;
}
//-------------------------------------------------------------------------------
// new key released (handle string input)
//-------------------------------------------------------------------------------
void clavierMac::initKcode2(Sint32 kCode)
{
	if(	kCode != SDLK_LSHIFT && kCode != SDLK_RSHIFT && kCode != SDLK_LCTRL &&
		kCode != SDLK_RCTRL)
	{	code_keyup = kCode;
		if(code_keyup == codePress1)
			codePress1 = 0;
		inputTempo = 0;
		codePress2 = 0;
	}
}

//-------------------------------------------------------------------------------
// handle string input
//-------------------------------------------------------------------------------
void clavierMac::inputKCode(Sint32 kCode)
{	Sint32 j, i;
	if(!kCode) return;
	curs_state = 0;	//position overflow
	
	//###################################################################
	// check pressed key
	//###################################################################
	switch (kCode)
	{	case SDLK_LEFT:
			cursor_pos--;
			break;
		case SDLK_RIGHT:
			cursor_pos++;
			break;

		// "backspace key" [<-]
		case SDLK_BACKSPACE:
			if(cursor_pos > 0)
				j = cursor_pos;
			else
				j = 1;
			for(i = j; i < input_size; i++)
				ptStrInput[i - 1] = ptStrInput[i];
			ptStrInput[input_size - 1] = ' ';
			cursor_pos--;
			break;

			// Test de la touche 'delete' [Suppr]
		case SDLK_DELETE:
			for(i = cursor_pos; i < input_size - 1; i++)
				ptStrInput[i] = ptStrInput[i + 1];
			ptStrInput[input_size - 1] = ' ';
			break;
			
		// [<-+']
		case SDLK_RETURN:	//ASCII code=13
			stop_input();
			return;
			break;
		
		default:
			kCode = kCode & 127;
			if(kCode >= 'a' && kCode <= 'z')
				kCode = kCode - 32;
			// space (32) / ! (33) 
			// , (44) /  - (45) / . (46) /
			// :(58) / 0-9 (48-57) 
			// A-Z (65 to 90)    
			if(	(kCode >= ' ' && kCode <= '!') ||
				(kCode >= '-' && kCode <= '.') ||
				(kCode >= '0' && kCode <= ':') ||
				(kCode >= 'A' && kCode <= 'Z') ||
				kCode == '\'')
			{	
				for(i = input_size - 1; i > cursor_pos; i--)
					ptStrInput[i] = ptStrInput[i - 1];
				ptStrInput[cursor_pos] = kCode;
				cursor_pos++;
				/* fprintf(stdout, "%s kCode:%i cursor_pos:%i\n",
					ptStrInput, kCode, cursor_pos);*/

			}
	}
  
	//###################################################################
	// verify overflow position cursor
	//###################################################################
	if(cursor_pos < 0)
	{	cursor_pos = 0;
		curs_state = 1;	//left overflow
	}
	if(cursor_pos > input_size - 1)
	{	cursor_pos = input_size - 1;
		curs_state = 2;	//right overflow
	}
}

//-------------------------------------------------------------------------------
// start string input
//-------------------------------------------------------------------------------
void clavierMac::init_input(char* strng, Uint32 ssize)
{
	//fprintf(stdout, "clavierMac::init_input: %s %i\n", strng, ssize);
	cursor_pos = 0;
	input_size = ssize;
	ptStrInput = strng;
}

//-------------------------------------------------------------------------------
// return cursor position 
//-------------------------------------------------------------------------------
Sint32 clavierMac::getCursPos()
{
	if(!ptStrInput) return -1;	//no input string 
	return 	cursor_pos;
}

//-------------------------------------------------------------------------------
// stop string input
//-------------------------------------------------------------------------------
void clavierMac::stop_input()
{
	cursor_pos = 0;
	input_size = 0;
	ptStrInput = NULL;
}

Uint32		clavierMac::keys_flags[NUMOFFLAGS];
Uint32		clavierMac::keys_lasts[NUMOFFLAGS];
