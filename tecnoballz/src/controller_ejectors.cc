/** 
 * @file controller_ejectors.cc 
 * @brief Ejectors corners controller 
 * @date 2007-02-13
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_ejectors.cc,v 1.2 2007/02/13 17:11:02 gurumeditation Exp $
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
#include "../include/controller_ejectors.h"

/**
 * Create the ejectors controller
 */
controller_ejectors::controller_ejectors ()
{
  littleInit ();
  max_of_sprites = MAX_OF_EJECTORS;
  sprites_have_shades = true;

}

/**
 * Release the ejectors controller
 */
controller_ejectors::~controller_ejectors ()
{
  release_sprites_list ();
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
void
controller_ejectors::create_ejectors_sprites ()
{

  alloc_sprites_list ();

  sprites_list[TOP_LEFT_EJECTOR] = new sprite_object ();
  sprites_list[TOP_RIGHT_EJECTOR] = new sprite_object ();
  sprites_list[BOTTOM_LEFT_EJECTOR] = new sprite_object ();
  sprites_list[BOTTOM_RIGHT_EJECTOR] = new sprite_object ();

  // top-left (1)
  sprites_list[TOP_LEFT_EJECTOR]->create_sprite (BOB_EJECT1, sprites_bitmap, 1);
  eject1PosX = EJECT_POS1 * resolution;
  eject1PosY =
    (EJECT_POS1 * resolution) - sprites_list[TOP_LEFT_EJECTOR]->get_sprite_height () / 2;
  sprites_list[TOP_LEFT_EJECTOR]->set_coordinates (eject1PosX, eject1PosY);

  // top-right (2)
  sprites_list[TOP_RIGHT_EJECTOR]->create_sprite (BOB_EJECT4, sprites_bitmap, 1);
  eject2PosX = EJECT_POS2 * resolution;
  eject2PosY =
    (EJECT_POS1 * resolution) - sprites_list[TOP_RIGHT_EJECTOR]->get_sprite_height () / 2;
  sprites_list[TOP_RIGHT_EJECTOR]->set_coordinates (eject2PosX, eject2PosY);

  // bottom-left (3)
  sprites_list[BOTTOM_LEFT_EJECTOR]->create_sprite (BOB_EJECT2, sprites_bitmap, 1);
  eject3PosX = EJECT_POS1 * resolution;
  eject3PosY =
    (EJECT_POS2 * resolution) - sprites_list[BOTTOM_LEFT_EJECTOR]->get_sprite_height () / 2;
  sprites_list[BOTTOM_LEFT_EJECTOR]->set_coordinates (eject3PosX, eject3PosY);

  // bottom-right(4)
  sprites_list[BOTTOM_RIGHT_EJECTOR]->create_sprite (BOB_EJECT3, sprites_bitmap, 1);
  eject4PosX = EJECT_POS2 * resolution;
  eject4PosY =
    (EJECT_POS2 * resolution) - sprites_list[BOTTOM_RIGHT_EJECTOR]->get_sprite_height () / 2;
  sprites_list[BOTTOM_RIGHT_EJECTOR]->set_coordinates (eject4PosX, eject4PosY);

  // bob_ground = 1: ejectors are managed like sprites  
  if (bob_ground)
    {
      sprites->add (sprites_list[TOP_LEFT_EJECTOR]);
      sprites->add (sprites_list[TOP_RIGHT_EJECTOR]);
      sprites->add (sprites_list[BOTTOM_LEFT_EJECTOR]);
      sprites->add (sprites_list[BOTTOM_RIGHT_EJECTOR]);
      sprites_list[TOP_LEFT_EJECTOR]->enable ();
      sprites_list[TOP_RIGHT_EJECTOR]->enable ();
      sprites_list[BOTTOM_LEFT_EJECTOR]->enable ();
      sprites_list[BOTTOM_RIGHT_EJECTOR]->enable ();
    }
}

/**
 * Draw ejectors shadows
 */
void
controller_ejectors::draw_shadow ()
{
  if (bob_ground)
    {
      return;
    }
  sprites_list[TOP_LEFT_EJECTOR]->affich_SHA ();
  sprites_list[TOP_RIGHT_EJECTOR]->affich_SHA ();
  sprites_list[BOTTOM_LEFT_EJECTOR]->affich_SHA ();
  sprites_list[BOTTOM_RIGHT_EJECTOR]->affich_SHA ();
}

/**
 * Draw ejectors shadows
 */
void
controller_ejectors::draw ()
{
  if (bob_ground)
    {
      return;
    }
  sprites_list[TOP_LEFT_EJECTOR]->affich_MSK ();
  sprites_list[TOP_RIGHT_EJECTOR]->affich_MSK ();
  sprites_list[BOTTOM_LEFT_EJECTOR]->affich_MSK ();
  sprites_list[BOTTOM_RIGHT_EJECTOR]->affich_MSK ();
}

/**
 * Return a ejector sprite
 * @param id identifier of the ejector
 * @return a pointer to the ejector sprite
 */
sprite_object *
controller_ejectors::get_ejector (Uint32 id)
{
  switch (id)
    {
    case TOP_LEFT_EJECTOR:
      return sprites_list[TOP_LEFT_EJECTOR];
    case TOP_RIGHT_EJECTOR:
      return sprites_list[TOP_RIGHT_EJECTOR];
    case BOTTOM_LEFT_EJECTOR: 
      return sprites_list[BOTTOM_LEFT_EJECTOR];
    case BOTTOM_RIGHT_EJECTOR:
      return sprites_list[BOTTOM_RIGHT_EJECTOR];
    }
  return NULL;
}

//-----------------------------------------------------------------------------
// initialize the positions of the balls in the ejectors 
//-----------------------------------------------------------------------------
Uint32
  controller_ejectors::balPosFlag = 0;
void
controller_ejectors::ballPosIni (furaxEject * table)
{
  if (balPosFlag)
    return;
  balPosFlag = 1;
  furaxEject *eject = table;
  eject->x_coord = (eject->x_coord * resolution) + eject1PosX;
  eject->y_coord = (eject->y_coord * resolution) + eject1PosY;

  eject = table + 1;
  eject->x_coord = (eject->x_coord * resolution) + eject3PosX;
  eject->y_coord = (eject->y_coord * resolution) + eject3PosY;

  eject = table + 2;
  eject->x_coord = (eject->x_coord * resolution) + eject4PosX;
  eject->y_coord = (eject->y_coord * resolution) + eject4PosY;

  eject = table + 3;
  eject->x_coord = (eject->x_coord * resolution) + eject2PosX;
  eject->y_coord = (eject->y_coord * resolution) + eject2PosY;

}
