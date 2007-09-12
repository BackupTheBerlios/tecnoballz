//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "controller_fontes_game.h"
// created      : ?
// updates      : 2005-01-23
// fonction     : manage mobiles characters ("LEVEL x COMPLETED")
// id           : $Id: controller_fontes_game.h,v 1.3 2007/09/12 06:32:48 gurumeditation Exp $
//-----------------------------------------------------------------------------
// TecnoballZ is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 3 of the License, or (at your option) any later
// version.
// 
// TecnoballZ is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA 02111-1307, USA.
//*****************************************************************************
#ifndef __ZEMOVETEXT__
#define __ZEMOVETEXT__
//...............................................................................
class controller_fontes_game;
//...............................................................................
#include "../include/objects_list.h"
#include "../include/sprite_fonte_game.h"
//...............................................................................
class controller_fontes_game:public objects_list < sprite_fonte_game, controller_fontes_game >
{
private:
  Sint32 size_line1;
  Sint32 size_line2;
  Sint32 size_total;
  Sint32 horz_large;
  Sint32 chrOffsetX;
  static char ze_bobText[];
  static char ze_endText[];

public:
    controller_fontes_game ();
   ~controller_fontes_game ();
  void initialise (Sint32 level, Sint32 offzt = 0);
  Sint32 startValue (Sint32 n, Sint32 a, Sint32 j, Sint32 y,
                     Sint32 i3, Sint32 y3);
  void goMoveText ();
  void activeText ();
};
#endif
