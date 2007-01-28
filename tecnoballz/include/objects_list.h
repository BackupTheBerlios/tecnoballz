/** 
 * @file objects_list.h 
 * @brief Template of management of objects list 
 * @date 2007-01-28
 * @copyright 1998-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.7 $
 */
/* 
 * copyright (c) 1998-2007 TLK Games all rights reserved
 * $Id: objects_list.h,v 1.7 2007/01/28 21:31:56 gurumeditation Exp $
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
#ifndef __OBJECTS_LIST__
#define __OBJECTS_LIST__

template < class X > class objects_list;
#include "../include/list_sprites.h"
#include "../include/handler_display.h"
#include "../include/mentatCode.h"

template < class X > class objects_list:public virtual mentatCode
{
protected:
  X ** sprites_list;
  Sint32 max_of_sprites;
  Sint32 num_of_sprites;
  bool sprites_have_shades;
  Sint32 sprite_type_id;
  bool is_draw_pixel_by_pixel;

public:
  objects_list ();
  ~objects_list ();
  void littleInit ();
  void release_sprites_list ();
  void alloc_sprites_list ();
  void create_sprites_list ();
  void disable_sprites ();
  void enable_sprites ();
  X **get_sprites_list ();
  Sint32 get_max_of_sprites ();
  void set_max_of_sprites (Sint32 total);
};

/**
 * Create the list of objects
 */ 
template < class X > objects_list < X >::objects_list ()
{
}

/** 
 * Release the lits objects
 */
template < class X > objects_list < X >::~objects_list ()
{
}

/**
 * Clear some members
 */
template < class X > void objects_list < X >::littleInit ()
{
  mentatInit ();
  max_of_sprites = 0;
  num_of_sprites = 0;
  sprites_list = NULL;
  sprites_have_shades = false;
  sprite_type_id = 0;
  is_draw_pixel_by_pixel = false;
}

/**
 * Release the list of sprite object
 */
template < class X > void objects_list < X >::release_sprites_list ()
{
  if (NULL == sprites_list)
    {
      return;
    }
  for (Sint32 i = 0; i < max_of_sprites; i++)
    {
      X *sprite = sprites_list[i];
      if (sprite != NULL)
        {
          delete sprite;
        }
        sprites_list[i] = (X *) NULL;
    }
    delete[]sprites_list;
    sprites_list = NULL;
    mentatKill ();
}

/**
 * Return list of the sprites objects
 * @return pointer to the list of sprites objects
 */
template < class X > X ** objects_list < X >::get_sprites_list ()
{
  return sprites_list;
}

/**
 * Return the maxium number of sprites objects
 * @return the maxium number of sprites objects
 */
template < class X > Sint32 objects_list < X >::get_max_of_sprites ()
{
  return max_of_sprites;
}

/** 
 * Initialize the maximum number of sprites
 * @param maxof maximum number of sprites
 */
template < class X > void objects_list < X >::set_max_of_sprites (Sint32 maxof)
{
  max_of_sprites = maxof;
}

/**
 * Allocate memory for the list of sprites
 */
template < class X > void objects_list < X >::alloc_sprites_list ()
{
  if (max_of_sprites <= 0)
    {
      std::cerr << "(!)objects_list::alloc_sprites_list() " <<
          "Our array should always have at least one element in it!" <<
          std::endl;
      throw ("(!)objects_list::alloc_sprites_list() failed! "
          "At least one element is required");
    }
  try
    {
       sprites_list = new X*[max_of_sprites];
    }
  catch (std::bad_alloc &)
    {
      std::cerr << "(!)(!)objects_list::::alloc_sprites_list() " <<
        "not enough memory to allocate " <<
        max_of_sprites << " elements!" << std::endl;
      throw;
    }
  for (Sint32 i = 0; i < max_of_sprites; i++)
    {
      sprites_list[i] = NULL;
    }
}

/**
 * Initialize the list of sprites objects
 */
template < class X > void objects_list < X >::create_sprites_list ()
{
  bitmap_data *image = image_BOBs;
 alloc_sprites_list ();
 
  X *sprite_template = new X ();
  sprite_template->set_object_pos (0);
  sprites_list[0] = sprite_template;
  
  /* reserves only once the memory required for the
   * graphic data of the sprite */
  sprite_template->create_sprite (sprite_type_id, image, sprites_have_shades, is_draw_pixel_by_pixel);
  sprite_template->set_draw_method (sprite_object::DRAW_WITH_TABLES);
  sprites->add (sprite_template);
  for (Sint32 i = 1; i < max_of_sprites; i++)
    {
      X *sprite = new X ();
      sprite->set_object_pos (i);
      sprite_template->duplicaBOB (sprite);
      //sprite = sprite_template;

      sprites_list[i] = sprite;
      sprites->add (sprite);
    }
}

/**
 * Enable all sprites objects
 */
template < class X > void objects_list < X >::enable_sprites ()
{
  for (Sint32 i = 0; i < max_of_sprites; i++)
    {
      X *sprite = sprites_list[i];
      sprite->enable ();
    }
}

/**
 * Disable all sprites objects
 */
template < class X > void objects_list < X >::disable_sprites ()
{
  for (Sint32 i = 0; i < max_of_sprites; i++)
    {
      X *sprite = sprites_list[i];
      sprite->disable ();
    }
}

#endif
