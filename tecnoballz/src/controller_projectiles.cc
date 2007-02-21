/** 
 * @file controller_projectiles.cc 
 * @brief Projectiles controller for a single paddle! 
 * @date 2007-02-20
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.4 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_projectiles.cc,v 1.4 2007/02/21 14:22:11 gurumeditation Exp $
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
#include "../include/controller_projectiles.h"

/**
 * Create the projectiles controller
 */
controller_projectiles::controller_projectiles ()
{
  littleInit ();
  countTempo = 0;
  max_of_sprites = MAX_OF_PROJECTILES;
  sprites_have_shades = false;
  sprite_type_id = BOB_FIREBP;
  if (resolution == 1)
    {
      scie_sinus = &scieSin320[0];
    }
  else
    {
      scie_sinus = &scieSin640[0];
    }
}

/**
 * Release the projectiles controller
 */
controller_projectiles::~controller_projectiles ()
{
  release_sprites_list ();
}

/**
 * Create and initialize the list of projectiles sprites
 * @param paddle a pointer to a paddle sprite
 */ 
void
controller_projectiles::create_projectiles_list (sprite_paddle * paddle)
{
  create_sprites_list ();
  gun_paddle = paddle;
  sprite_projectile **projectiles = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_projectile *projectile = *(projectiles++);
      projectile->init_members (paddle);
    }
  fire1RunOn ();
}

/**
 * Check if fire is available
 */
void
controller_projectiles::disponible ()
{

  /* return if bumper has no fire (bumperFire = 0) */
  if (gun_paddle->bumperFire == 0)
    {
      return;
    }

  Uint32 t = max_of_sprites;
  sprite_projectile **liste = sprites_list;

  //###################################################################
  // special fire 7 (circular fire)
  //###################################################################
  if (gun_paddle->length == gun_paddle->width_maxi)
    {
      Sint32 j = 1;
      for (Uint32 i = 0; i < t; i++)
        {
          sprite_projectile *xFire = *(liste++);
          if (xFire->is_enabled == j)
            return;
        }
    }

  //###################################################################
  // other fires
  //###################################################################
  else
    {
      for (Uint32 i = 0; i < t; i++)
        {
          sprite_projectile *xFire = *(liste++);
          if (xFire->is_enabled)
            return;
        }
    }
  countTempo = 0;
  gun_paddle->bumperFire = 3;   //fire is requested
#ifndef SOUNDISOFF
  audio->play_sound (S_RAK_TIRS);
#endif
}

//-----------------------------------------------------------------------------
// new fire start
//-----------------------------------------------------------------------------
void
controller_projectiles::fire ()
{
  if (gun_paddle->bumperFire)
    {
      Sint32 i = gun_paddle->length;
      paddle_length = i;
      i -= gun_paddle->width_mini;      //smallest bumper is of 16/32 pixels width
      i >>= gun_paddle->width_deca;     //size of bumper step by 8/16 pixels
      switch (i)
        {
        case 0:
          init_type1 ();
          break;
        case 1:
          init_type2 ();
          break;
        case 2:
          init_type3 ();
          break;
        case 3:
          init_type4 ();
          break;
        case 4:
          init_type5 ();
          break;
        case 5:
          init_type6 ();
          break;
        case 6:
          init_type7 ();
          break;
        }
    }
}

//-----------------------------------------------------------------------------
// fire 1: 16/32 pixels bumper's whidth
//-----------------------------------------------------------------------------
void
controller_projectiles::init_type1 ()
{
  sprite_paddle *raket = gun_paddle;
  if (raket->bumperFire == 3)
    {
      raket->bumperFire = 1;
      Sint32 x = raket->x_coord;
      Sint32 y = raket->y_coord;
      if (raket->bumperType)    //vertical bumper ?
        y += (paddle_length / 2) - (SIZE_OF_PROJECTILE / 2);
      else
        x += (paddle_length / 2) - (SIZE_OF_PROJECTILE / 2);
      sprite_projectile *xFire = sprites_list[0];
      xFire->is_enabled = 1;
      xFire->x_coord = x;
      xFire->y_coord = y;
    }
}

//-----------------------------------------------------------------------------
// fire 2: 24/48 pixels bumper's whidth
//-----------------------------------------------------------------------------
void
controller_projectiles::init_type2 ()
{
  sprite_paddle *raket = gun_paddle;
  if (raket->bumperFire == 3)
    {
      raket->bumperFire = 1;
      Sint32 x = raket->x_coord;
      Sint32 y = raket->y_coord;
      if (raket->bumperType)    //vertical bumper ?
        y += (paddle_length / 2) - (SIZE_OF_PROJECTILE / 2);
      else
        x += (paddle_length / 2) - (SIZE_OF_PROJECTILE / 2);
      sprite_projectile **liste = sprites_list;
      sprite_projectile *xFire;
      Sint32 f = 1;
      xFire = *(liste++);
      xFire->is_enabled = f;
      xFire->x_coord = x;
      xFire->y_coord = y;
      xFire = *(liste++);
      xFire->is_enabled = f;
      xFire->x_coord = x;
      xFire->y_coord = y;
    }
}

//-----------------------------------------------------------------------------
// fire 3: 24/48 pixels bumper's whidth
//-----------------------------------------------------------------------------
void
controller_projectiles::init_type3 ()
{
  sprite_paddle *raket = gun_paddle;
  if (raket->bumperFire == 3)
    {
      raket->bumperFire = 1;
      Sint32 x = raket->x_coord;
      Sint32 y = raket->y_coord;
      if (raket->bumperType)    //vertical bumper ?
        y += (paddle_length / 2) - (SIZE_OF_PROJECTILE / 2);
      else
        x += (paddle_length / 2) - (SIZE_OF_PROJECTILE / 2);
      sprite_projectile **liste = sprites_list;
      sprite_projectile *xFire;
      Sint32 f = 1;
      xFire = *(liste++);
      xFire->is_enabled = f;
      xFire->x_coord = x;
      xFire->y_coord = y;
      xFire = *(liste++);
      xFire->is_enabled = f;
      xFire->x_coord = x;
      xFire->y_coord = y;
      xFire = *liste;
      xFire->is_enabled = f;
      xFire->x_coord = x;
      xFire->y_coord = y;
    }
}

//-----------------------------------------------------------------------------
// fire 4: 40/80 pixels bumper's whidth
//-----------------------------------------------------------------------------
void
controller_projectiles::init_type4 ()
{
  sprite_paddle *raket = gun_paddle;
  if (raket->bumperFire == 3)
    {
      raket->bumperFire = 1;
      Sint32 x = raket->x_coord;
      Sint32 y = raket->y_coord;
      sprite_projectile **liste = sprites_list;
      sprite_projectile *xFire;
      Sint32 f = 1;
      xFire = *(liste++);
      xFire->is_enabled = f;
      xFire->x_coord = x;
      xFire->y_coord = y;
      if (raket->bumperType)    //vertical bumper ?
        y += 18 * resolution;
      else
        x += 18 * resolution;
      xFire = *(liste++);
      xFire->is_enabled = f;
      xFire->x_coord = x;
      xFire->y_coord = y;
      xFire = *(liste++);
      xFire->is_enabled = f;
      xFire->x_coord = x;
      xFire->y_coord = y;
      if (raket->bumperType)    //vertical bumper ?
        y = raket->y_coord + raket->length - 4;
      else
        x = raket->x_coord + raket->length - 4;
      xFire = *liste;
      xFire->is_enabled = f;
      xFire->x_coord = x;
      xFire->y_coord = y;
    }
}

//-----------------------------------------------------------------------------
// fire 5: 48/96 pixels bumper's whidth
//-----------------------------------------------------------------------------
void
controller_projectiles::init_type5 ()
{
  sprite_paddle *raket = gun_paddle;
  if (raket->bumperFire == 3)
    {
      raket->bumperFire = 1;
      Sint32 x = raket->x_coord;
      Sint32 y = raket->y_coord;
      sprite_projectile **liste = sprites_list;
      sprite_projectile *xFire;
      Sint32 f = 1;
      xFire = *(liste++);
      xFire->is_enabled = f;
      xFire->x_coord = x;
      xFire->y_coord = y;
      Sint32 quart = paddle_length / 4;
      if (raket->bumperType)    //vertical bumper ?
        {
          Sint32 i = raket->bumper_FX1;
          x += i;
          y += quart;
          xFire = *(liste++);
          xFire->is_enabled = f;
          xFire->x_coord = x;
          xFire->y_coord = y;
          x += i;
          y += quart;
          xFire = *(liste++);
          xFire->is_enabled = f;
          xFire->x_coord = x;
          xFire->y_coord = y;
          x -= i;
          y += quart;
          xFire = *(liste++);
          xFire->is_enabled = f;
          xFire->x_coord = x;
          xFire->y_coord = y;
          x -= i;
          y += quart - 2 * resolution;
          xFire = *(liste++);
          xFire->is_enabled = f;
          xFire->x_coord = x;
          xFire->y_coord = y;
        }
      else
        {
          Sint32 i = raket->bumper_FY1;
          y += i;
          x += quart;
          xFire = *(liste++);
          xFire->is_enabled = f;
          xFire->x_coord = x;
          xFire->y_coord = y;
          y += i;
          x += quart;
          xFire = *(liste++);
          xFire->is_enabled = f;
          xFire->x_coord = x;
          xFire->y_coord = y;
          y -= i;
          x += quart;
          xFire = *(liste++);
          xFire->is_enabled = f;
          xFire->x_coord = x;
          xFire->y_coord = y;
          y -= i;
          x += quart - 2 * resolution;
          xFire = *(liste++);
          xFire->is_enabled = f;
          xFire->x_coord = x;
          xFire->y_coord = y;
        }
    }
}

//-----------------------------------------------------------------------------
// fire 6: 56/112 pixels bumper's whidth
//-----------------------------------------------------------------------------
void
controller_projectiles::init_type6 ()
{
  sprite_paddle *raket = gun_paddle;
  if (raket->bumperFire == 3)
    {
      raket->bumperFire = 1;
      sprite_projectile **liste = sprites_list;
      Sint32 x = raket->x_coord;
      Sint32 y = raket->y_coord;
      Sint32 offst = 22 * resolution;

      if (raket->bumperType)    //vertical bumper ?
        {
          sprite_projectile *xFire;
          Sint32 a = x + raket->bump_xdeca;
          xFire = *(liste++);
          xFire->is_enabled = 1;
          xFire->x_coord = a;
          xFire->y_coord = y;
          xFire = *(liste++);
          xFire->is_enabled = 1;
          xFire->x_coord = x;
          xFire->y_coord = y;
          y += offst;
          xFire = *(liste++);
          xFire->is_enabled = 1;
          xFire->x_coord = x;
          xFire->y_coord = y;
          xFire = *(liste++);
          xFire->is_enabled = 1;
          xFire->x_coord = x;
          xFire->y_coord = y;
          y += offst;
          xFire = *(liste++);
          xFire->is_enabled = 1;
          xFire->x_coord = a;
          xFire->y_coord = y;
          xFire = *(liste++);
          xFire->is_enabled = 1;
          xFire->x_coord = x;
          xFire->y_coord = y;
        }
      else
        {
          sprite_projectile *xFire;
          Sint32 o = y + raket->bump_ydeca;
          xFire = *(liste++);
          xFire->is_enabled = 1;
          xFire->x_coord = x;
          xFire->y_coord = y;
          xFire = *(liste++);
          xFire->is_enabled = 1;
          xFire->x_coord = x;
          xFire->y_coord = o;
          x += offst;
          xFire = *(liste++);
          xFire->is_enabled = 1;
          xFire->x_coord = x;
          xFire->y_coord = y;
          xFire = *(liste++);
          xFire->is_enabled = 1;
          xFire->x_coord = x;
          xFire->y_coord = y;
          x += offst;
          xFire = *(liste++);
          xFire->is_enabled = 1;
          xFire->x_coord = x;
          xFire->y_coord = o;
          xFire = *(liste++);
          xFire->is_enabled = 1;
          xFire->x_coord = x;
          xFire->y_coord = y;
        }
    }
}

//-----------------------------------------------------------------------------
// fire 7: 64/128 pixels bumper's whidth
//-----------------------------------------------------------------------------
void
controller_projectiles::init_type7 ()
{
  sprite_paddle *raket = gun_paddle;

  //###################################################################
  // bumper is shotting ?
  //###################################################################
  if (raket->bumperFire == 3)   //fire is requested
    {
      raket->bumperFire = 1;
      sprite_projectile **liste = sprites_list;
      Sint32 x = raket->x_coord + raket->bump_Xscie;
      Sint32 y = raket->y_coord + raket->bump_Yscie;
      Sint32 j = 0;
      for (Sint32 i = 0; i < 7; i++, j += 8)
        {
          sprite_projectile *xFire = *(liste++);
          xFire->is_enabled = 1;
          xFire->fire_Xscie = x;
          xFire->fire_Yscie = y;
          xFire->indexSinus = j;
        }
    }

  //###################################################################
  // fire on the bumper
  //###################################################################
  else
    {
      sprite_projectile **liste = sprites_list;
      raket->bumperFire = 1;
      for (Sint32 i = 0; i < 7; i++)
        {
          sprite_projectile *xFire = *(liste++);
          if (xFire->is_enabled)
            return;
        }
      liste = sprites_list;
      Sint32 x = raket->x_coord + raket->bump_Xscie;
      Sint32 y = raket->y_coord + raket->bump_Yscie;
      Sint32 j = 0;
      for (Sint32 i = 0; i < 7; i++, j += 8)
        {
          sprite_projectile *xFire = *(liste++);
          xFire->is_enabled = 2;
          xFire->fire_Xscie = x;
          xFire->fire_Yscie = y;
          xFire->indexSinus = j;
        }

    }
}

/**
 * Move paddle's projectiles
 */
void
controller_projectiles::move ()
{
  Sint32 i = gun_paddle->length;
  paddle_length = i;
  /* smallest paddle width is of 16/32 pixels */
  i -= gun_paddle->width_mini;
  /* size of paddle step by 8/16 pixels */
  i >>= gun_paddle->width_deca;
  switch (i)
    {
    case 0:
      move_type1 ();
      break;
    case 1:
      move_type2 ();
      break;
    case 2:
      move_type3 ();
      break;
    case 3:
      move_type4 ();
      break;
    case 4:
      move_type5 ();
      break;
    case 5:
      move_type6 ();
      break;
    case 6:
      move_type7 ();
      break;
    }
}

//-----------------------------------------------------------------------------
// bumper of 16/32 pixels width: "linear fire"
//-----------------------------------------------------------------------------
void
controller_projectiles::move_type1 ()
{
  sprite_projectile *xFire = sprites_list[0];
  sprite_paddle *raket = gun_paddle;
  Sint32 i = raket->bumper_FX0;
  xFire->x_coord += i;
  i = raket->bumper_FY0;
  xFire->y_coord += i;
}

//-----------------------------------------------------------------------------
// bumper of 24/48 pixels width: "fishtail fire"
//-----------------------------------------------------------------------------
void
controller_projectiles::move_type2 ()
{
  sprite_projectile **liste = sprites_list;
  sprite_paddle *raket = gun_paddle;
  Sint32 a = countTempo;
  Sint32 b, c, d;
  a++;
  if (a > 20)
    a = 0;
  countTempo = a;
  if (a > 10)
    {
      a = raket->bumper_FX1;
      b = raket->bumper_FY1;
      c = raket->bumper_FX2;
      d = raket->bumper_FY2;
    }
  else
    {
      a = raket->bumper_FX2;
      b = raket->bumper_FY2;
      c = raket->bumper_FX1;
      d = raket->bumper_FY1;
    }
  sprite_projectile *xFire;
  xFire = *(liste++);
  xFire->x_coord += a;
  xFire->y_coord += b;
  xFire = *liste;
  xFire->x_coord += c;
  xFire->y_coord += d;
}

//-----------------------------------------------------------------------------
// bumper of 32/64 pixels width: "triangle fire"
//-----------------------------------------------------------------------------
void
controller_projectiles::move_type3 ()
{
  sprite_projectile **liste = sprites_list;
  sprite_paddle *raket = gun_paddle;
  Sint32 i, j;
  sprite_projectile *xFire;
  xFire = *(liste++);           //shot leaves to the left
  i = raket->bumper_FX1;
  j = raket->bumper_FY1;
  xFire->x_coord += i;
  xFire->y_coord += j;
  xFire = *(liste++);           //shot leaves any right
  i = raket->bumper_FX0;
  j = raket->bumper_FY0;
  xFire->x_coord += i;
  xFire->y_coord += j;
  xFire = *liste;               //shot leaves to the right
  i = raket->bumper_FX2;
  j = raket->bumper_FY2;
  xFire->x_coord += i;
  xFire->y_coord += j;
}

//-----------------------------------------------------------------------------
// bumper of 40/80 pixels width: "fishtail fire" + "linear fire"
//-----------------------------------------------------------------------------
void
controller_projectiles::move_type4 ()
{
  Sint32 i, j;
  i = countTempo;
  i++;
  if (i > 20)
    i = 0;
  countTempo = i;
  sprite_projectile **liste = sprites_list;
  sprite_paddle *raket = gun_paddle;
  sprite_projectile *xFire;

  xFire = *(liste++);           //[1] linar shot
  j = raket->bumper_FX0;
  xFire->x_coord += j;
  j = raket->bumper_FY0;
  xFire->y_coord += j;

  if (i > 10)
    {
      xFire = *(liste++);       //[2] fishtail shot
      i = raket->bumper_FX1;
      j = raket->bumper_FY1;
      xFire->x_coord += i;
      xFire->y_coord += j;
      xFire = *(liste++);       //[3] fishtail shot
      i = raket->bumper_FX2;
      j = raket->bumper_FY2;
      xFire->x_coord += i;
      xFire->y_coord += j;
    }
  else
    {
      xFire = *(liste++);       //[2] fishtail shot
      i = raket->bumper_FX2;
      j = raket->bumper_FY2;
      xFire->x_coord += i;
      xFire->y_coord += j;
      xFire = *(liste++);       //[3] fishtail shot
      i = raket->bumper_FX1;
      j = raket->bumper_FY1;
      xFire->x_coord += i;
      xFire->y_coord += j;
    }

  xFire = *liste;               //[4] linar shot
  i = raket->bumper_FX0;
  j = raket->bumper_FY0;
  xFire->x_coord += i;
  xFire->y_coord += j;
}

//-----------------------------------------------------------------------------
// bumper of 48/96 pixels width: 5 linears shots
//-----------------------------------------------------------------------------
void
controller_projectiles::move_type5 ()
{
  sprite_projectile **liste = sprites_list;
  sprite_paddle *raket = gun_paddle;
  Sint32 x = raket->bumper_FX0;
  Sint32 y = raket->bumper_FY0;
  for (Sint32 i = 0; i < 5; i++)
    {
      sprite_projectile *xFire = *(liste++);
      xFire->x_coord += x;
      xFire->y_coord += y;
    }
}

//-----------------------------------------------------------------------------
// bumper of 56/112 pixels width: 4 linears shots + 2 fishtails shots
//-----------------------------------------------------------------------------
void
controller_projectiles::move_type6 ()
{
  Sint32 a, i, j;

  a = countTempo;
  a++;
  if (a > 20)
    a = 0;
  countTempo = a;
  sprite_projectile **liste = sprites_list;
  sprite_paddle *raket = gun_paddle;

  i = raket->bumper_FX0;
  j = raket->bumper_FY0;
  sprite_projectile *xFire = *(liste++);

  xFire->x_coord += i;
  xFire->y_coord += j;
  xFire = *(liste++);
  xFire->x_coord += i;
  xFire->y_coord += j;
  if (a > 10)
    {
      xFire = *(liste++);       //[3] fishtail shot
      i = raket->bumper_FX1;
      j = raket->bumper_FY1;
      xFire->x_coord += i;
      xFire->y_coord += j;
      xFire = *(liste++);       //[4] fishtail shot
      i = raket->bumper_FX2;
      j = raket->bumper_FY2;
      xFire->x_coord += i;
      xFire->y_coord += j;
    }
  else
    {
      xFire = *(liste++);       //[3] fishtail shot
      i = raket->bumper_FX2;
      j = raket->bumper_FY2;
      xFire->x_coord += i;
      xFire->y_coord += j;
      xFire = *(liste++);       //[4] fishtail shot
      i = raket->bumper_FX1;
      j = raket->bumper_FY1;
      xFire->x_coord += i;
      xFire->y_coord += j;
    }
  i = raket->bumper_FX0;
  j = raket->bumper_FY0;
  xFire = *(liste++);
  xFire->x_coord += i;
  xFire->y_coord += j;
  xFire = *liste;
  xFire->x_coord += i;
  xFire->y_coord += j;
}

//-----------------------------------------------------------------------------
// bumper of 64/128 pixels width: 7 circular shots
//-----------------------------------------------------------------------------
void
controller_projectiles::move_type7 ()
{
  sprite_projectile **liste = sprites_list;
  sprite_paddle *raket = gun_paddle;
  for (Sint32 i = 0; i < 7; i++)
    {
      sprite_projectile *xFire = *(liste++);
      if (xFire->is_enabled)
        {
          Sint32 j = xFire->indexSinus;
          j += 2;
          const Sint16 *table = scie_sinus + j;
          if (*table == 99)
            {
              j = 0;
              table = scie_sinus;
            }
          xFire->indexSinus = j;
          if (xFire->is_enabled == 2)
            {
              Sint32 k = *(table++);
              k += raket->x_coord + raket->bump_Xscie;
              xFire->x_coord = k;
              k = *table;
              k += raket->y_coord + raket->bump_Yscie;
              xFire->y_coord = k;
            }
          else
            {
              xFire->fire_Xscie += raket->bumper_FX0;
              xFire->fire_Yscie += raket->bumper_FY0;
              Sint32 k = *(table++);
              k += xFire->fire_Xscie;
              xFire->x_coord = k;
              k = *table;
              k += xFire->fire_Yscie;
              xFire->y_coord = k;
            }
        }
    }
}

/**
 * Enable fire power 1
 */
void
controller_projectiles::fire1RunOn ()
{
  sprite_projectile **projectiles = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_projectile *blast = *(projectiles++);
      blast->set_power1 ();
    }
}

/**
 * Enable fire power 2
 */
void
controller_projectiles::fire2RunOn ()
{
  sprite_projectile **projectiles = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_projectile *blast = *(projectiles++);
      blast->set_power2 ();
    }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sint16
  controller_projectiles::bob11_fire[] = { 16, 5,
  8, 5,
  3, 0,                         //fire power 2
  4, 0,
  5, 0,
  6, 0,
  7, 0,                         //fire power 1
  8, 0,
  9, 0,
  10, 0
};

//-----------------------------------------------------------------------------
// sinus fire 7 (640 pixels)
//-----------------------------------------------------------------------------
const Sint16
  controller_projectiles::scieSin640[] =
  { 15 * 2, -3 * 2, 13 * 2, -7 * 2, 11 * 2, -10 * 2, 9 * 2, -12 * 2, 7 * 2,
  13 * 2, 3 * 2, -15 * 2, 0 * 2, -15 * 2,
  -3 * 2, -15 * 2, -6 * 2, -14 * 2, -9 * 2, -12 * 2, -11 * 2, -10 * 2,
  -13 * 2, -07 * 2, -14 * 2, -04 * 2, -15 * 2, -01 * 2,
  -15 * 2, 3 * 2, -13 * 2, 7 * 2, -11 * 2, 10 * 2, -9 * 2, 12 * 2, -7 * 2,
  13 * 2, -3 * 2, 15 * 2, 0 * 2, 15 * 2,
  3 * 2, 15 * 2, 6 * 2, 14 * 2, 9 * 2, 12 * 2, 11 * 2, 10 * 2, 13 * 2, 7 * 2,
  14 * 2, 4 * 2, 15 * 2, 1 * 2,
  99, 99, 99, 99
};

//-----------------------------------------------------------------------------
// sinus fire 7 (320 pixels)
//-----------------------------------------------------------------------------
const Sint16
  controller_projectiles::scieSin320[] =
  { 15, -3, 13, -7, 11, -10, 9, -12, 7, -13, 3, -15, 0, -15,
  -3, -15, -6, -14, -9, -12, -11, -10, -13, -7, -14, -4, -15, -1,
  -15, 3, -13, 7, -11, 10, -9, 12, -7, 13, -3, 15, 0, 15,
  3, 15, 6, 14, 9, 12, 11, 10, 13, 7, 14, 4, 15, 1,
  99, 99, 99, 99
};
