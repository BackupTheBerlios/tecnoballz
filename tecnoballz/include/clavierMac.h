//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "clavierMac.h"
// created		: ?
// updates		: 2004-10-24
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
#ifndef __CLAVIERMAC__
#define __CLAVIERMAC__
//...............................................................................
#include "../include/mentatCode.h"
//...............................................................................
class clavierMac:public virtual mentatCode
{
	public:
		static const Uint32	PAUSE_FLAG = 0;
		static const Uint32	TOEXITFLAG = 1;
		static const Uint32	TOMENUFLAG = 2;
		static const Uint32	TOOVERFLAG = 3;
		static const Uint32	MFXSFXFLAG = 4;
		static const Uint32	SOUND_FLAG = 5;
		static const Uint32	MUSIC_FLAG = 6;
		static const Uint32	FULLSCFLAG = 7;
		static const Uint32	ESCAPEMENU = 8;
		static const Uint32	NUMOFFLAGS = 9;

	private:
		static Uint32		keys_lasts[NUMOFFLAGS];
		static Uint32		keys_flags[NUMOFFLAGS];

	private:
		Sint32				grabs_flag;	//1 =  grabs mouse and keyboard input.
		Sint32				sourisOffx;
		Sint32				sourisOffy;
		Sint32				sourisPosX;
		Sint32				sourisPosY;
		Sint32				mo_point_h;	//x coordinates of the mouse
		Sint32				mo_point_v;	//y coordinates of the mouse
		Sint32				mo_pointhr;	//relative motion in the X direction
		Sint32				mo_pointvr;	//relative motion in the Y direction
		Sint32				bou_gauche;
		Sint32				bou_droite;
		Sint32				bou_milieu;
		Sint32				mouspressx;	//pos x when left button was pressed
		Sint32				mouspressy;	//pos y when left button was pressed
		Sint32				mouspresse;
		Sint32				mousreleas;
		Sint32				releaseRgt;
		
		Sint32				inputTempo;	//keyboard input temporization
		Uint32				codePress1;
		Uint32				codePress2;
		Uint32				code_keyup;
		Sint32				cursor_pos;	//cursor position into input string
		Uint32				curs_state;	//1 = left overflow / 2 = right overflow
		Sint32				input_size;	//input string size
		char*				ptStrInput;	//pointer to input string
		
		
	
	public:
							clavierMac();
							~clavierMac();
		void				setGrab_On();
		void				setGrabOff();
		void				lit_keymap();
		void				resetpause();
		Uint32				specialKey(Uint32);
		Uint32				specKeyRaz(Uint32);
		Sint32				leftButton();
		Sint32				righButton();
		Sint32				buttonBlit();
		Sint32				sourisRela(Sint32 *off_x, Sint32 *off_y);
		Sint32				isReleaseR(Sint32 *off_x, Sint32 *off_y);
		void				souris_off(Sint32 *off_x, Sint32 *off_y);
		void				souris_off();
		Sint32				sourisoffX();
		Sint32				sourisoffY();
		Sint32				sourisGetY();
		Sint32				sourisGetX();
		char				test_Kcode(Sint16);
		char				test_Rcode(Sint16);
	
		void				init_input(char* strng, Uint32 ssize);
		Sint32				getCursPos();
		void				stop_input();
		Sint32				get_Kcode1();

	private:
		void				init_kcode(Sint32 kCode);
		Sint32				inputKCode();
		void				inputKCode(Sint32);
		void				initKcode1(Sint32);
		void				initKcode2(Sint32);

};

#endif
