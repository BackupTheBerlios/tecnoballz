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
	clear_sprite_members();
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
void mousePoint::create_BOB(bitmap_data * image)
{
	Sint32 b;
	if(hasard_val & 1)
          {
		b = BOB_MOUSE1;
          }
	else
          {
		b = BOB_MOUSE2;
          }
	create_sprite(b, image, 0);
	sprites->add(this);
	Sint32 x = game_screen->get_width() / 2;
	Sint32 y = game_screen->get_height() / 2;

	set_coordinates(x, y);
	enable();
	set_frame_delay(3);
}

//-----------------------------------------------------------------------------
// sprite moving
//-----------------------------------------------------------------------------
void mousePoint::bouge_test()
{
	Sint32 offsX = keyboard->get_mouse_x();
	Sint32 offsY = keyboard->get_mouse_y();
	move_x(offsX);
	move_y(offsY);
	set_coordinates(offsX, offsY);
	clip_coordinates();
	play_animation_loop();
}
