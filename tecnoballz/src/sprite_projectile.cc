/**
 * @file sprite_projectile.cc 
 * @brief The fire sprite of the paddle into the bricks level
 * @date 2007-04-13
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.13 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_projectile.cc,v 1.13 2007/09/12 06:32:49 gurumeditation Exp $
 *
 * TecnoballZ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * TecnoballZ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */
#include "../include/sprite_projectile.h"

/**
 * Create the fire sprite object
 */
sprite_projectile::sprite_projectile ()
{
  if (total_fire < MAXI_TOTAL_OF_PROJECTILES)
    {
      projectiles_list[total_fire] = this;
      total_fire++;
    }
  set_draw_method (DRAW_COLOR_CYCLING_MASK);
}

/**
 * Release the fire sprite object
 */
sprite_projectile::~sprite_projectile ()
{
}

/**
 * Enable the power 1 of the projectiles
 */
void
sprite_projectile::set_power1 ()
{
  frame_index_min = 4;
  frame_index_max = 7;
  frame_index = 4;
  can_destroy_indestructible = false;
  power = 1;
  pt_cycling = &sprite_object::cycling_02[0];
}

/**
 * Enable the power 2 of the projectiles
 */
void
sprite_projectile::set_power2 ()
{
  frame_index_min = 0;
  frame_index_max = 3;
  frame_index = 0;
  can_destroy_indestructible = true;
  power = 2;
  pt_cycling = &sprite_object::cycling_01[0];
}

/**
 * Clear member a simple
 */
void
sprite_projectile::init_members (sprite_paddle * pad)
{
  paddle = pad;
  indexSinus = 0;
  fire_Xscie = 0;
  fire_Yscie = 0;
  frame_index = 0;
  frame_index_max = 3;
  frame_index_min = 0;
  frame_delay = 10;
  frame_period = 10;
  can_destroy_indestructible = false;
  power = 0;
}

/**
 * Static method which initialize all projectiles before a bricks level
 */
void
sprite_projectile::start_list (controller_bricks * brick,
                               controller_ships * atoms)
{
  brickObjet = brick;
  atomsObjet = atoms;
  total_fire = 0;
  for (Uint32 i = 0; i < MAXI_TOTAL_OF_PROJECTILES; i++)
    {
      projectiles_list[i] = NULL;
    }
}

/**
 * Static method which manage all projectiles
 */
void
sprite_projectile::gestionTir ()
{
  check_outside ();
  play_projectiles_animations ();
  check_collisions_with_bricks ();
  check_collisions_with_ships ();
}

/**
 * Static method which check if all projectiles go out of the screen of game 
 */
void
sprite_projectile::check_outside ()
{
  sprite_projectile **projectiles = projectiles_list;
  Sint32 y1 = 15 * resolution;
  Sint32 y2 = 232 * resolution;
  Sint32 x1 = 15 * resolution;
  Sint32 x2 = 228 * resolution;
  for (Uint32 i = 0; i < total_fire; i++)
    {
      sprite_projectile *blast = *(projectiles++);
      Sint32 a = blast->y_coord;
      if (a < y1 || a > y2)
        {
          blast->is_enabled = false;
          continue;
        }
      a = blast->x_coord;
      if (a < x1 || a > x2)
        {
          blast->is_enabled = false;
        }
    }
}

/**
 * Static method which manages the animation played in loop-mode
 */
void
sprite_projectile::play_projectiles_animations ()
{
  sprite_projectile **projectiles = projectiles_list;
  sprite_projectile *blast = projectiles[0];
  blast->play_animation_loop ();
  Sint32 index = blast->get_frame_index ();
  Sint32 cycle = index & 0X1;
  if (0 == cycle)
    {
      cycle = sprite_object::DRAW_WITH_TABLES;
    }
  else
    {
      cycle = sprite_object::DRAW_COLOR_CYCLING_MASK;
    }
  blast->draw_method = cycle;
  for (Uint32 i = 1; i < total_fire; i++)
    {
      blast = projectiles[i];
      blast->set_image (index);
      blast->draw_method = cycle;
    }
}

/**
 * Static method which check collision projectiles between bricks  
 */ 
void
sprite_projectile::check_collisions_with_bricks ()
{

  controller_bricks *bricks = controller_bricks::get_instance ();

  /* brick's width in pixels */
  Uint32 brick_width = bricks->get_brick_width ();
  Uint32 byoff = bricks->getYOffset (); //y-offset between 2 bricks
  Sint32 indus = bricks->getBkIndus (); //first indestructible brick
  sprite_projectile **projectiles = projectiles_list;
  brick_info *bricks_map = bricks->get_bricks_map ();
  for (Uint32 i = 0; i < total_fire; i++)
    {
      sprite_projectile *projectile = *(projectiles++);
      if (!projectile->is_enabled)
        {
          continue;
        }
      Sint32 x = projectile->x_coord + 2;
      Sint32 y = projectile->y_coord + 2;
      brick_redraw *redraw = bricks->get_bricks_redraw ();
      redraw->xcoord_collision = x;
      redraw->ycoord_collision = y;
      x /= brick_width;
      y /= byoff;
      y *= controller_bricks::NB_BRICKSH;
      x += y;
      brick_info *map = (bricks_map + x);
      x = map->brique_rel;
      if (x == 0)
        {
          continue;
        }
      if (projectile->is_enabled == 1)
        {
          projectile->is_enabled = 0;
        }
      redraw->paddle = projectile->paddle;
      redraw->is_gigablitz_destroyed = false;
      if ((x -= indus) >= 0)
        {
          /* 
           * indestructible brick touched!
           */
	  /* indestructible-destructible bricks? */
          if ((x -= brick_width) > 0) 
            {
              /* fire destroys the indestructibles-destructibles bricks? */
              if (projectile->can_destroy_indestructible)
                {
                  //redraw->xcoord_collision = -1;
		  redraw->is_indestructible = true;
                  redraw->pixel_offset = map->pixel_offset;
                  redraw->brick_map = map;
                  map->brique_rel = 0;        // RAZ code brique
                  redraw->number = map->number;
		  /* restore background under brick */
                  redraw->is_background = true;
		  bricks->bricks_redraw_next ();
                }
              else
                {
                  x = 2;
#ifndef SOUNDISOFF
                  audio->play_sound (S_TOINDES2);
#endif
                }
            }
          else
            {
              /* the brick is really indestructible */
              x = 1;
#ifndef SOUNDISOFF
              audio->play_sound (S_TOINDES1);
#endif
            }
        }
      /* 
       * normal brick touched
       */
      else
        {
	  redraw->is_indestructible = false;
          redraw->pixel_offset = map->pixel_offset;
          redraw->brick_map = map;
	  /* fire power: 1 or 2 */
          x = projectile->power;
          map->h_pos = map->h_pos - (x * 2);
          if (map->h_pos <= 0)
            {
              map->h_pos = 0;
              map->brique_rel = 0;
              redraw->number = map->number;
	      /* restore background under brick */
              redraw->is_background = true;
            }
          else
            {
              map->brique_rel = map->brique_rel - (x * brick_width);
              redraw->number = map->brique_rel;
              /* redraw a new brick */
	      redraw->is_background = false;
            }
	  bricks->bricks_redraw_next ();
        }
    }
}

/**
 * Static method which check collisions between projectiles and ships 
 */ 
void
sprite_projectile::check_collisions_with_ships ()
{
  sprite_projectile **projectiles = projectiles_list;
  Sint32 t = atomsObjet->get_max_of_sprites ();
  sprite_ship **aList = atomsObjet->get_sprites_list ();
  for (Uint32 i = 0; i < total_fire; i++)
    {
      sprite_projectile *blast = *(projectiles++);
      if (!blast->is_enabled)
        {
          continue;
        }
      sprite_ship **monPT = aList;
      Sint32 y1 = blast->y_coord;
      Sint32 y2 = y1 + 3;
      y1 -= 26;
      Sint32 x1 = blast->x_coord;
      Sint32 x2 = x1 + 3;
      x1 -= 20;
      for (Sint32 j = 0; j < t; j++)
        {
          sprite_ship *atome = *(monPT++);
          if (atome->atom_actif > 0)
            {
              continue;
            }
          Sint32 k = atome->y_coord;
          if (k >= y2 || k <= y1)
            {
              continue;
            }
          k = atome->x_coord;
          if (k >= x2 || k <= x1)
            {
              continue;
            }
          if (blast->is_enabled == 1)
            {
              blast->is_enabled = 0;
            }
          current_player->add_score (100);
          k = blast->power;
          atome->atom_power -= k;
          if (atome->atom_power < 1)
            {
              atome->explosion1 (blast);
            }
        }
    }
}

/**
 * Static method which disables all projectiles
 */ 
void
sprite_projectile::disable_sprites ()
{
  sprite_projectile **projectiles = projectiles_list;
  for (Uint32 i = 0; i < total_fire; i++)
    {
      sprite_projectile *blast = *(projectiles++);
      blast->is_enabled = false;
    }
}

Uint32 sprite_projectile::total_fire = 0;
sprite_projectile * sprite_projectile::projectiles_list[MAXI_TOTAL_OF_PROJECTILES];
controller_bricks * sprite_projectile::brickObjet;
controller_ships * sprite_projectile::atomsObjet;
