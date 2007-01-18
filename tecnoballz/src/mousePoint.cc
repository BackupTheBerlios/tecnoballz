//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "mousePoint.cc"
// created		: ?
// updates		: 2004-05-13
// fonction	: shop mouse pointer
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
#include "../include/mousePoint.h"
#include "../include/handler_keyboard.h"
#include "../include/handler_display.h"
#include "../include/list_sprites.h"
//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
mousePoint::mousePoint()
{
	BOBprepare();
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
mousePoint::~mousePoint()
{
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
Sint32 mousePoint::create_BOB(GIF_bitMap * image)
{
	Sint32 b;
	if(hasard_val & 1)
		b = BOB_MOUSE1;
	else
		b = BOB_MOUSE2;
	
	error_init(initialise(b, image, 0));
	if(erreur_num) return erreur_num;
	sprites->add(this);
	Sint32 x = display->bufferLarg() / 2;
	Sint32 y = display->bufferHaut() / 2;

	coordonnee(x, y);
	enable();
	tempo_init(3);
	return erreur_num;
}

//-----------------------------------------------------------------------------
// sprite moving
//-----------------------------------------------------------------------------
void mousePoint::bouge_test()
{
	Sint32 offsX = keyboard->get_mouse_x();
	Sint32 offsY = keyboard->get_mouse_y();
	deplace_pX(offsX);
	deplace_pY(offsY);
	coordonnee(offsX, offsY);
	out_screen();
	animRepete();
}
