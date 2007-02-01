/** 
 * @file tiles_background.h
 * @brief Draw tiles background in bricks levels 
 * @date 2007-02-01
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: tiles_background.h,v 1.1 2007/02/01 11:17:24 gurumeditation Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */
#ifndef __TILES_BACKGROUND__
#define __TILES_BACKGROUND__
class tiles_background;
#include "../include/mentatCode.h"
class tiles_background:public virtual mentatCode
{
private:
  static Sint32 table_pos1[16];
  static Sint32 table_pos2[16];
  static char couleurs[448];
  static const Sint32 FONDLARGE1 = 32;
  static const Sint32 FONDHAUTE1 = 32;
  static const Sint32 FONDLARGE2 = 64;
  static const Sint32 FONDHAUTE2 = 64;

  typedef enum {
    TILES_32x32_WITH_4_COLORS,
    TILES_64x64_WITH_16_COLORS
  }
  TYPES_OF_TILES;
  /** Type of tiles TILES_64x64_WITH_16_COLORS
   * or TILES_32x32_WITH_4_COLORS */
  Uint32 type_of_tiles;
  /** 4 color palette index */
  Uint32 palette_index;

private:
  void set_4_color_palette (Uint32 pal_index);

public:
    tiles_background ();
   ~tiles_background ();
  void setup (Uint32 titles_num);
  void set_4_color_palette ();
  void next_4_color_palette ();
  void prev_4_color_palette ();
};
#endif
