/** 
 * @file list_sprites.cc 
 * @brief Call the drawing methods of all sprites
 * @date 2007-01-16
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: list_sprites.cc,v 1.3 2007/01/23 14:08:51 gurumeditation Exp $
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
#include "../include/list_sprites.h"

/**
 * Create the object
 */
list_sprites::list_sprites ()
{
  shapes = NULL;
  shadows = NULL;
  num_of_shapes = 0;
  num_of_shadows = 0;
  max_of_shapes = 0;
  mentatInit ();
}

/**
 * Release the object
 */
list_sprites::~list_sprites ()
{
  if (shapes != NULL)
    {
      memory->release ( (char *) shapes);
      shapes = NULL;
    }
  if (shadows != NULL)
    {
      memory->release ( (char *) shadows);
      shadows = NULL;
    }
  mentatKill ();
}

/**
 * Allocate memory for the list of shapes and shadow
 * @params numof max numbers of shapes 
 */
Sint32 list_sprites::init (Sint32 numof)
{
  printf("list_sprites::init: %i\n", numof);
  if (shapes == NULL)
    {
      max_of_shapes = numof;
      printf("list_sprites::init %i\n", max_of_shapes);
      shapes = (sprite_object **) memory->alloc (sizeof (sprite_object *) * numof);
      printf("list_sprites::init %i\n", max_of_shapes);
      if (shapes == NULL) return memory->retour_err ();
      printf("list_sprites::init %i\n", max_of_shapes);
    }
  if (shadows == NULL)
    {
      printf("list_sprites::init %i\n", max_of_shapes);
      shadows = (sprite_object **) memory->alloc (sizeof (sprite_object *) * numof);
      printf("list_sprites::init %i\n", max_of_shapes);
      if (shadows == NULL) return memory->retour_err ();
      printf("list_sprites::init %i\n", max_of_shapes);
    }
  printf("list_sprites::init %i\n", max_of_shapes);
  reset ();
  printf("list_sprites::init\n");
  return 0;
}

/**
 * Clear the list
 */
void
list_sprites::reset ()
{
  num_of_shapes = 0;
  num_of_shadows = 0;
  for (Sint32 i = 0; i < max_of_shapes; i++)
    {
      shapes[i] = NULL;
      shadows[i] = NULL;
    }
}

/**
 * Add a sprite to the display list to draw the sprites
 * @param sprite pointer to a sprite object 
 */
void
list_sprites::add (sprite_object * sprite)
{
  if (num_of_shapes >= max_of_shapes - 1)
    {
      fprintf (stderr, "list_sprites::ajoute_BOB() "
               "max. number reached %i\n", num_of_shapes);
      return;
    }
  shapes[num_of_shapes] = sprite;
  num_of_shapes++;
  sprite->set_display_pos (num_of_shapes);
  if (sprite->has_shadow ())
    {
      shadows[num_of_shadows] = sprite;
      num_of_shadows++;
    }
}

/**
 *  Draw all sprites into the main offscreen
 */
void
list_sprites::draw ()
{
  /* draw the shahows of the objects */
  for (Sint32 i = 0; i < num_of_shadows; i++)
    {
      sprite_object *sprite = shadows[i];
      sprite->afficheSHA ();
    }

  /* draw the objects */
  for (Sint32 i = 0; i < num_of_shapes; i++)
    {
      sprite_object *sprite = shapes[i];
      sprite->draw ();
    }
}

/**
 * Restore background under the sprites
 */
void
list_sprites::clear ()
{
  for (Sint32 i = 0; i < num_of_shapes; i++)
    {
      sprite_object *sprite = shapes[i];
      sprite->restore_background_under_shadow ();
      sprite->restore_background_under_sprite ();
    }
}


