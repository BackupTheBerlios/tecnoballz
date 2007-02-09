/** 
 * @file controller_guardians.cc 
 * @brief Guardians controller 
 * @created 2003-01-10 
 * @date 2007-02-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.5 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_guardians.cc,v 1.5 2007/02/09 17:05:29 gurumeditation Exp $
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
#include "../include/controller_guardians.h"
#include "../include/handler_resources.h"
#include "../include/level_data.h"

/**
 * Create the guardians controller 
 */
controller_guardians::controller_guardians ()
{
  littleInit ();
  max_of_sprites = 2;
  sprites_have_shades = true;
  sprite_type_id = BOB_INTE1A;
  offset_ptr = 0;
  scrollTemp = 0;
}

/**
 * Release the guardians controller 
 */
controller_guardians::~controller_guardians ()
{
  if (NULL != displacement_curve)
    {
      delete[](char *) displacement_curve;
      displacement_curve = NULL;
    }
  if (NULL != life_gauges_list)
    {
      for (Uint32 i = 0; i < max_of_sprites; i++)
        {
          sprite_object *sprite = life_gauges_list[i];
          if (NULL != sprite)
            {
              delete sprite;
              life_gauges_list[i] = NULL;
            }
        }
      delete[]life_gauges_list;
      life_gauges_list = NULL;
    }
  release_sprites_list ();
}

/**
 * Create and initialize the guardians and life gauges sprites
 * @param pMiss
 * @param grdPt
 * @param pBliz
 * @param pExpl
 */
void
controller_guardians::create_guardians_list (controller_bullets * pMiss, Sint32 grdPt,
                                  controller_gigablitz * pBliz, controller_explosions * pExpl)
{
  ptGigaBlit = pBliz;
  pexplosion = pExpl;

  /* count the number of guardians, 1 or 2 guardians */
  max_of_sprites = -1;
  Uint32 i;
  Sint32 j = grdPt;
  do
    {
      i = level_list[j++];
      max_of_sprites++;
    }
  while (i >= 0);

  /* loading curves file of all guards (23304 bytes) */
  displacement_curve =
    (unsigned char *) resources->load_data (handler_resources::RESGCURVES);

  /* allocate memory for the "sprite_guardian" object(s) */
  alloc_sprites_list ();

  /* initialize the guardian(s) sprite(s) */
  for (i = 0; i < max_of_sprites; i++)
    {
      Sint32 p = level_list[grdPt++];
      sprite_guardian *guard = new sprite_guardian ();
      guard->set_object_pos (i);
      guard->create_sprite (guard_list[p].para_nsbob, sprites_bitmap, true);
      sprites_list[i] = guard;
      sprites->add (guard);
      guard->enable ();
      guard->init_guard (&guard_list[p],
                         getLissaPt (guard_list[p].para_lissa), pMiss,
                         ptGigaBlit, pexplosion);
    }

  /*
   * Create the life gauges sprites
   */
  try
  {
    life_gauges_list = new sprite_object *[max_of_sprites];
  }
  catch (std::bad_alloc &)
  {
    std::
      cerr << "(!)controller_guardians::create_guardians_list() "
      "not enough memory to allocate " <<
      max_of_sprites << " list elements!" << std::endl;
    throw;
  }
 for (i = 0; i < max_of_sprites; i++)
    {
      sprite_object *sprite = new sprite_object ();
      life_gauges_list[i] = sprite;
      sprite->set_object_pos (i);
      sprite->create_sprite (BOB_GRDNRJ, sprites_bitmap, false);
      sprites->add (sprite);
      sprite->set_coordinates (i * 16 * resolution, 0);
      sprite->enable ();
      sprite->enable_vertical_repeat (20);
    }
  set_gauge_height ();
}

/**
 * Calcul the position and number of repetition of the life gauges 
 */
void
controller_guardians::set_gauge_height ()
{
  Uint32 y_base = 238 * resolution;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_guardian *guard_sprite = sprites_list[i];
      sprite_object *gauge_sprite = life_gauges_list[i];
      if (!guard_sprite->gard_power)
        {
          gauge_sprite->disable ();
          break;
        }
      Uint32 h = guard_sprite->gard_power / 4;
      gauge_sprite->enable_vertical_repeat (h);
      if (0 == h)
        {
          h = 1;
        }
      Sint32 y = y_base - h * gauge_sprite->get_sprite_height ();
      gauge_sprite->set_y_coord (y);
    }
}

/**
 * Move the guardians, and fire bullets and gigablitz
 */
void
controller_guardians::run ()
{
  offset_ptr++;
  offset_ptr = offset_ptr & 255;
  Uint32 voffset = offzetbyte[offset_ptr] * resolution;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_guardian *guard_sprite = sprites_list[i];
      guard_sprite->run (voffset);
    }
  set_gauge_height ();
}

/**
 * check if the guardian's level is finished
 * @return true if all guardians was destroyed
 */
bool
controller_guardians::is_guardians_destroyed ()
{
  bool is_finished = true;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_guardian *guard_sprite = sprites_list[i];
      if (guard_sprite->gard_power > 0)
        {
          is_finished = false;
        }
    }
  return is_finished;
}

void
controller_guardians::killguards (Sint32 numGa)
{
  switch (numGa)
    {
    case 2:
      if (max_of_sprites > 1)
        {
          if (sprites_list[1]->gard_power)
            {
              sprites_list[1]->gard_power = 0;
              sprites_list[1]->explo_time = 500;
            }
        }
      break;

    case 1:
      if (sprites_list[0]->gard_power)
        {
          sprites_list[0]->gard_power = 0;
          sprites_list[0]->explo_time = 500;
        }
      break;

    default:
      if (max_of_sprites > 1)
        {
          if (sprites_list[1]->gard_power)
            {
              sprites_list[1]->gard_power = 0;
              sprites_list[1]->explo_time = 500;
            }
        }

      if (sprites_list[0]->gard_power)
        {
          sprites_list[0]->gard_power = 0;
          sprites_list[0]->explo_time = 500;
        }
      break;
    }

}

//-------------------------------------------------------------------------------
// handle background scrolling (direction and speed)
//-------------------------------------------------------------------------------
Sint32
controller_guardians::run_scroll (Uint32 ntype, Sint32 speed,
                                  sprite_ball * balle, sprite_bullet * weapo)
{
  sprite_guardian *pGua1, *pGua2;
  if (max_of_sprites < 1)
    return speed;
  pGua1 = sprites_list[0];
  if (max_of_sprites > 1)
    pGua2 = sprites_list[1];
  else
    pGua2 = pGua1;

  switch (ntype)
    {
      //###############################################################
      // medium area 1 (SCROLL DOWN)
      //###############################################################
    case 1:
      if (!pGua1->gard_power && !pGua2->gard_power)
        speed = 2;
      else
        {
          if (!pGua1->gard_power || !pGua2->gard_power)
            speed = 1;
          else
            speed = 0;
        }
      break;

      //###############################################################
      // final area 1 (SCROLL GUARD 1)
      //###############################################################
    case 2:
      if (pGua1->gard_power)
        {
          Sint32 gposy = pGua1->y_coord;
          if (scrollTemp)
            {
              gposy = scrollTemp - gposy;
              gposy = gposy >> 1;
              if (gposy)
                speed = gposy;
            }
          scrollTemp = pGua1->y_coord;
        }
      break;

      //###############################################################
      // medium area 2 (SCROLL COLLISION 1)
      //###############################################################
    case 3:
      if (pGua1->gard_touch && pGua1->gard_power)
        speed = -1;
      else
        {
          if (pGua2->gard_touch && pGua2->gard_power)
            speed = 1;
        }
      break;

      //###############################################################
      // final area 2 (SCROLL BALL 1)
      //###############################################################
    case 4:
      if (pGua1->gard_power)
        {
          if (balle->colleBallF || !balle->is_enabled)
            {
              speed = -1;
              scrollTemp = 0;
            }
          else
            {
              Sint32 gposy = balle->y_coord;
              if (scrollTemp)
                {
                  gposy = scrollTemp - gposy;
                  gposy = gposy >> 1;
                  if (gposy)
                    speed = gposy;
                }
              scrollTemp = balle->y_coord;
            }
        }
      break;

      //###############################################################
      // medium area 3 (SCROLL COLLISION 2)
      //###############################################################
    case 5:
      if (pGua1->gard_touch && pGua1->gard_power)
        {
          if (--speed < (-4 * resolution))
            speed = (-4 * resolution);
        }
      else
        {
          if (pGua2->gard_touch && pGua2->gard_power)
            {
              if (++speed > (4 * resolution))
                speed = (4 * resolution);
            }
        }
      break;

      //###############################################################
      // final area 3 (SCROLL GUARD 2)
      //###############################################################
    case 6:
      if (pGua1->gard_power)
        {
          Sint32 gposy = pGua1->y_coord;
          if (scrollTemp)
            {
              gposy = gposy - scrollTemp;
              if (gposy)
                speed = gposy * 2;
            }
          scrollTemp = pGua1->y_coord;
        }
      break;

      //###############################################################
      // medium area 4 (SCROLL FIRE 1)
      //###############################################################
    case 7:
      if (weapo->is_enabled)
        {
          if (scrollTemp)
            {
              Sint32 gposy = weapo->y_coord;
              gposy = gposy - scrollTemp;
              speed += gposy;
              if (speed > 15 * resolution)
                speed = 15 * resolution;
              if (speed < (-15 * resolution))
                speed = -15 * resolution;
            }
          scrollTemp = weapo->y_coord;
        }
      else
        scrollTemp = 0;
      break;

      //###############################################################
      // final area 4 (SCROLL FIRE 2)
      //###############################################################
    case 8:
      if (weapo->is_enabled)
        {
          if (scrollTemp)
            {
              Sint32 gposy = weapo->y_coord;
              gposy = scrollTemp - gposy;
              speed += gposy;
              if (speed > 15 * resolution)
                speed = 15 * resolution;
              if (speed < (-15 * resolution))
                speed = -15 * resolution;
            }
          scrollTemp = weapo->y_coord;
        }
      else
        scrollTemp = 0;
      break;

      //###############################################################
      // medium area 5 (SCROLL COLLISION 3)
      //###############################################################
    case 9:
      if (pGua1->gard_touch && pGua1->gard_power)
        {
          if (--speed < (-16 * resolution))
            speed = (-16 * resolution);
        }
      else
        {
          if (pGua2->gard_touch && pGua2->gard_power)
            {
              if (++speed > (16 * resolution))
                speed = (16 * resolution);
            }
        }
      break;

      //###############################################################
      // final area 5 (SIMPLE SCROLL)
      //###############################################################
    case 10:
      speed = 8 * resolution;
      break;

      //###############################################################
      // final-final area 5 (SIMPLE SCROLL)
      //###############################################################
    case 11:
      speed = -8 * resolution;
      break;
    }
  return speed;
}

//-------------------------------------------------------------------------------
// select curve's gard
//-------------------------------------------------------------------------------
unsigned char *
controller_guardians::getLissaPt (Sint32 lissa)
{
  // calcul le pointeur sur la courbe
  Uint32 p = 0;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
  lissa = lissa * 4;
  p = (unsigned char) displacement_curve[lissa + 0];
  p = p << 8;
  p = p | (unsigned char) displacement_curve[lissa + 1];
  p = p << 8;
  p = p | (unsigned char) displacement_curve[lissa + 2];
  p = p << 8;
  p = p | (unsigned char) displacement_curve[lissa + 3];
#else
  Uint32 *ptr32 = (Uint32 *) displacement_curve;
  p = ptr32[lissa];
#endif
  return displacement_curve + p;
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
const char
  controller_guardians::offzetbyte[256] = {
  0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3e, 0x3e, 0x3e, 0x3e,
  0x3d, 0x3e, 0x3d, 0x3d, 0x3c, 0x3d, 0x3c, 0x3c, 0x3b, 0x3b, 0x3a, 0x3a,
  0x39, 0x3a, 0x38, 0x39, 0x37, 0x38, 0x36, 0x37, 0x35, 0x36, 0x34, 0x35,
  0x33, 0x33, 0x32, 0x32, 0x30, 0x31, 0x2f, 0x30, 0x2e, 0x2e, 0x2c, 0x2d,
  0x2b, 0x2b, 0x29, 0x2a, 0x28, 0x28, 0x26, 0x27, 0x25, 0x25, 0x23, 0x24,
  0x22, 0x22, 0x20, 0x21, 0x1f, 0x1f, 0x1d, 0x1e, 0x1b, 0x1c, 0x1a, 0x1b,
  0x18, 0x19, 0x17, 0x18, 0x15, 0x16, 0x14, 0x15, 0x13, 0x13, 0x11, 0x12,
  0x10, 0x10, 0x0e, 0x0f, 0x0d, 0x0e, 0x0c, 0x0d, 0x0b, 0x0b, 0x0a, 0x0a,
  0x08, 0x09, 0x07, 0x08, 0x06, 0x07, 0x06, 0x06, 0x05, 0x05, 0x04, 0x04,
  0x03, 0x04, 0x03, 0x03, 0x02, 0x02, 0x01, 0x02, 0x01, 0x01, 0x01, 0x01,
  0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01, 0x02, 0x02,
  0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05, 0x05, 0x06, 0x06, 0x07, 0x06,
  0x08, 0x07, 0x09, 0x08, 0x0a, 0x0a, 0x0b, 0x0b, 0x0d, 0x0c, 0x0e, 0x0d,
  0x0f, 0x0e, 0x10, 0x10, 0x12, 0x11, 0x13, 0x13, 0x15, 0x14, 0x16, 0x15,
  0x18, 0x17, 0x19, 0x18, 0x1b, 0x1a, 0x1c, 0x1b, 0x1e, 0x1d, 0x1f, 0x1f,
  0x21, 0x20, 0x22, 0x22, 0x24, 0x23, 0x25, 0x25, 0x27, 0x26, 0x28, 0x28,
  0x2a, 0x29, 0x2b, 0x2b, 0x2d, 0x2c, 0x2e, 0x2e, 0x30, 0x2f, 0x31, 0x30,
  0x32, 0x32, 0x33, 0x33, 0x35, 0x34, 0x36, 0x35, 0x37, 0x36, 0x38, 0x37,
  0x39, 0x38, 0x3a, 0x39, 0x3a, 0x3a, 0x3b, 0x3b, 0x3c, 0x3c, 0x3d, 0x3c,
  0x3d, 0x3d, 0x3e, 0x3d, 0x3e, 0x3e, 0x3e, 0x3e, 0x3f, 0x3f, 0x3f, 0x3f,
  0x3f, 0x3f, 0x3f, 0x3f
};

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
gardlevel
  controller_guardians::guard_list[] = {
  /* AREA 1 : intermediaires */
  {25,                          // resistance
   16,                          // centre X depart tir
   555,                         // frequence tir gigaBlitz
   2,                           // vitesse de deplacement
   7,                           // fenetre de collision x-mini
   5,                           // fenetre de collision y-mini
   25,                          // fenetre de collision x-maxi
   17,                          // fenetre de collision y-maxi
   11,                          // centre Y depart tir
   400,                         // frequence tir
   BOB_INTE1A,                  // numero du BOB
   LISSA_NB01,                  // numero de la courbe
   {0, -1}                      // table pointeur des tirs
   },
  {30,                          // resistance du gardien
   16,
   666,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   8,                           // xmin zone sensible
   16,                          // ymin zone sensible
   24,                          // xmax zone sensible
   42,                          // ymax zone sensible
   24,
   240,                         //delai entre 2 tirs
   BOB_INTE1B,
   LISSA_NB02,
   {4, -1}
   },

  /* AERA 1 : Final */
  {50,                          // resistance du gardien
   32,
   333,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   13,                          // xmin zone sensible
   34,                          // ymin zone sensible
   52,                          // xmax zone sensible
   77,                          // ymax zone sensible
   52,
   200,                         //delai entre 2 tirs
   BOB_FINAL1,
   LISSA_NB03,
   {0, 4, 8, -1}
   },


  /* AERA 2 : intermediaires */
  {30,                          // resistance
   16,                          // centre X depart tir
   444,                         // frequence tir gigaBlitz
   2,                           // vitesse de deplacement
   9,                           // fenetre de collision x-mini
   5,                           // fenetre de collision y-mini
   23,                          // fenetre de collision x-maxi
   30,                          // fenetre de collision y-maxi
   21,                          // centre Y depart tir
   300,                         // frequence tir
   BOB_INTE2A,                  // numero du BOB
   LISSA_NB02,                  // numero de la courbe
   {12, -1}                     // table pointeur des tirs
   },
  {30,                          // resistance du gardien
   15,
   333,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   8,                           // xmin zone sensible
   10,                          // ymin zone sensible
   24,                          // xmax zone sensible
   49,                          // ymax zone sensible
   28,
   280,                         //delai entre 2 tirs
   BOB_INTE2B,
   LISSA_NB04,
   {8, -1}
   },

  /* AERA 2 : final */
  {100,                         // resistance du gardien
   32,
   555,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   14,                          // xmin zone sensible
   17,                          // ymin zone sensible
   50,                          // xmax zone sensible
   66,                          // ymax zone sensible
   45,
   200,                         //delai entre 2 tirs
   BOB_FINAL2,
   LISSA_NB15,
   {16, 0, 8, 4, 12, -1}
   },



  /* AERA 3 : intermediaires */
  {35,                          // resistance
   16,                          // centre X depart tir
   333,                         // frequence tir gigaBlitz
   2,                           // vitesse de deplacement
   6,                           // fenetre de collision x-mini
   5,                           // fenetre de collision y-mini
   26,                          // fenetre de collision x-maxi
   30,                          // fenetre de collision y-maxi
   19,                          // centre Y depart tir
   300,                         // frequence tir
   BOB_INTE3A,                  // numero du BOB
   LISSA_NB11,                  // numero de la courbe
   {20, 8, -1}                  // table pointeur des tirs
   },
  {30,                          // resistance du gardien
   15,
   555,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   10,                          // xmin zone sensible
   7,                           // ymin zone sensible
   22,                          // xmax zone sensible
   78,                          // ymax zone sensible
   46,
   200,                         //delai entre 2 tirs
   BOB_INTE3B,
   LISSA_NB13,
   {24, 4, -1}
   },
  /* AERA 3 : final */
  {70,                          // resistance du gardien
   32,
   444,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   13,                          // xmin zone sensible
   22,                          // ymin zone sensible
   52,                          // xmax zone sensible
   61,                          // ymax zone sensible
   41,
   250,                         //delai entre 2 tirs
   BOB_FINAL3,
   LISSA_NB07,
   {24, 0, 8, 12, 4, 16, -1}
   },


  /* AERA 4 : intermediaires */
  {30,                          // resistance
   16,                          // centre X depart tir
   222,                         // frequence tir gigaBlitz
   2,                           // vitesse de deplacement
   10,                          // fenetre de collision x-mini
   6,                           // fenetre de collision y-mini
   22,                          // fenetre de collision x-maxi
   42,                          // fenetre de collision y-maxi
   23,                          // centre Y depart tir
   300,                         // frequence tir
   BOB_INTE4A,                  // numero du BOB
   LISSA_NB08,                  // numero de la courbe
   {16, 28, 8, -1}              // table pointeur des tirs
   },
  {40,                          // resistance du gardien
   16,
   333,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   7,                           // xmin zone sensible
   12,                          // ymin zone sensible
   25,                          // xmax zone sensible
   68,                          // ymax zone sensible
   42,
   400,                         //delai entre 2 tirs
   BOB_INTE4B,
   LISSA_NB04,
   {12, 32, 8, -1}
   },
  /* AERA 4 : final */
  {60,                          // resistance du gardien
   32,
   222,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   13,                          // xmin zone sensible
   30,                          // ymin zone sensible
   52,                          // xmax zone sensible
   73,                          // ymax zone sensible
   49,
   160,                         //delai entre 2 tirs
   BOB_FINAL4,
   LISSA_NB05,
   {0, 4, 32, 16, 20, 8, 28, 12, 24, -1}
   },

  /* AERA 5 : intermediaires */
  {40,                          // resistance
   16,                          // centre X depart tir
   222,                         // frequence tir gigaBlitz
   2,                           // vitesse de deplacement
   7,                           // fenetre de collision x-mini
   14,                          // fenetre de collision y-mini
   25,                          // fenetre de collision x-maxi
   55,                          // fenetre de collision y-maxi
   31,                          // centre Y depart tir
   360,                         // frequence tir
   BOB_INTE5A,                  // numero du BOB
   LISSA_NB08,                  // numero de la courbe
   {36, 20, -1}                 // table pointeur des tirs
   },
  {40,                          // resistance du gardien
   16,
   333,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   6,                           // xmin zone sensible
   14,                          // ymin zone sensible
   26,                          // xmax zone sensible
   77,                          // ymax zone sensible
   41,
   180,                         //delai entre 2 tirs
   BOB_INTE5B,
   LISSA_NB12,
   {40, 0, -1}

   },

  /* AERA 5 : final */
  {70,                          // resistance du gardien
   31,
   333,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   22,                          // xmin zone sensible
   82,                          // ymin zone sensible
   42,                          // xmax zone sensible
   102,                         // ymax zone sensible
   55,
   200,                         //delai entre 2 tirs
   BOB_FINAL5,
   LISSA_NB09,
   {24, 8, 40, 12, 4, 0, 16, 20, 28, 36, 32, -1}
   },

  /* Final TecnoballZ  */
  {150,                         //resistance du gardien
   32,
   222,                         //delai entre 2 tirs
   2,                           //vit. déplacement
   11,                          //xmin zone sensible
   68,                          //ymin zone sensible
   54,                          //xmax zone sensible
   128,                         //ymax zone sensible
   83,
   150,                         //delai entre 2 tirs
   BOB_FINAL6,
   LISSA_NB12,
   {20, 4, 36, 8, 32, 12, 24, 16, 0, 28, 40, -1}
   },

};

//-------------------------------------------------------------------------------
// list of all levels gardians
//-------------------------------------------------------------------------------
Sint32
  controller_guardians::level_list[] = {
  0, 1, -1,                     //area 1
  2, -1,

  3, 4, -1,                     //area 2
  5, -1,

  6, 7, -1,                     //area 3
  8, -1,

  9, 10, -1,                    //area 4
  11, -1,

  12, 13, -1,                   //area 5
  14, -1,
  15, -1, -1
};

//-------------------------------------------------------------------------------
// convert couple (area, level) to gardian level pointer
//-------------------------------------------------------------------------------
Sint32
controller_guardians::level2gdpt (Uint32 arean, Uint32 level)
{
  if (arean < 1)
    arean = 1;
  if (level < 1)
    level = 1;
  if (arean > level_data::NUMOFAREAS)
    arean = level_data::NUMOFAREAS;
  Uint32 vloop = (level / 6) - 1 + (arean - 1) * 2;
  if (level > level_data::NUMOFLEVEL)
    vloop++;
  Uint32 index = 0;
  Sint32 gdptr = 0;
  for (;;)
    {
      if (index++ == vloop)
        break;                  //pointer is found :-)
      while (level_list[++gdptr] != -1);
      if (level_list[gdptr + 1] == -1)
        {
          fprintf (stderr,
                   "controller_guardians::level2gdpt(%i, %i) end of table\n",
                   arean, level);
          gdptr = 0;
          break;
        }
      gdptr++;
    }
  return gdptr;
}
