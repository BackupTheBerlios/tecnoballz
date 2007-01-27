/** 
 * @file sprite_projectile.cc 
 * @brief The fire sprite of the paddle into the bricks level
 * @date 2007-01-25
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_projectile.cc,v 1.2 2007/01/27 15:12:36 gurumeditation Exp $
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
#include "../include/sprite_projectile.h"

/**
 * Create the fire sprite object
 */
sprite_projectile::sprite_projectile ()
{
  if (total_fire < maxi_fires)
    {
      list_fires[total_fire] = this;
      total_fire++;
    }
  set_method (METHOD_CC1);
}

/**
 * Release the fire sprite object
 */
sprite_projectile::~sprite_projectile ()
{
}

//-----------------------------------------------------------------------------
// enable a simple bumper's fire power 1
//-----------------------------------------------------------------------------
void
sprite_projectile::firePower1 ()
{
  frame_index_min = 4;
  frame_index_max = 7;
  frame_index = 4;
  fire_power = 0;
  firePowerX = 1;
  pt_cycling = &sprite_object::cycling_02[0];
}

//-----------------------------------------------------------------------------
// enable a simple bumper's fire power 2
//-----------------------------------------------------------------------------
void
sprite_projectile::firePower2 ()
{
  frame_index_min = 0;
  frame_index_max = 3;
  frame_index = 0;
  fire_power = 1;
  firePowerX = 2;
  pt_cycling = &sprite_object::cycling_01[0];
}

//-----------------------------------------------------------------------------
// intialize a simple bumper's fire
//-----------------------------------------------------------------------------
void
sprite_projectile::littleInit (sprite_paddle * raket)
{
  raquettePT = raket;
  indexSinus = 0;
  fire_Xscie = 0;
  fire_Yscie = 0;
  frame_index = 0;
  frame_index_max = 3;
  frame_index_min = 0;
  frame_delay = 10;
  frame_period = 10;
  fire_power = 0;
  firePowerX = 0;
}

//-----------------------------------------------------------------------------
// static method: initialise all bumper's fires before a bricks level
//-----------------------------------------------------------------------------
void
sprite_projectile::start_list (lesBriques * brick, zeBouiBoui * atoms,
                         barreScore * score)
{
  brickObjet = brick;
  atomsObjet = atoms;
  scoreObjet = score;
  total_fire = 0;
  sprite_projectile **liste = list_fires;
  Sint32 t = maxi_fires;
  sprite_projectile *nFire = (sprite_projectile *) 0x0;
  for (Sint32 i = 0; i < t; i++)
    *(liste++) = nFire;
}

//-----------------------------------------------------------------------------
// static method: manage all bumper's fires
//-----------------------------------------------------------------------------
void
sprite_projectile::gestionTir ()
{
  hors_ecran ();
  anim_fires ();
  collision1 ();
  collision2 ();
}

//-----------------------------------------------------------------------------
// static method: test if all bumper's fire go out of the screen of game
//-----------------------------------------------------------------------------
void
sprite_projectile::hors_ecran ()
{
  sprite_projectile **liste = list_fires;
  Sint32 t = total_fire;
  Sint32 y1 = 15 * resolution;
  Sint32 y2 = 232 * resolution;
  Sint32 x1 = 15 * resolution;
  Sint32 x2 = 228 * resolution;
  for (Sint32 i = 0; i < t; i++)
    {
      sprite_projectile *xFire = *(liste++);
      Sint32 a = xFire->y_coord;
      if (a < y1 || a > y2)
        xFire->is_enabled = 0;
      else
        {
          a = xFire->x_coord;
          if (a < x1 || a > x2)
            xFire->is_enabled = 0;
        }
    }
}

//-----------------------------------------------------------------------------
// static method: animation of all bumper's fire
//-----------------------------------------------------------------------------
void
sprite_projectile::anim_fires ()
{
  sprite_projectile **liste = list_fires;
  Sint32 t = total_fire;
  sprite_projectile *xFire = liste[0];
  xFire->play_animation_loop ();
  Sint32 o = xFire->get_frame_index ();
  Sint32 cycle = o & 0X1;
  if (!cycle)
    cycle = sprite_object::METHOD_TAB;
  else
    cycle = sprite_object::METHOD_CC1;
  for (Sint32 i = 1; i < t; i++)
    {
      xFire = liste[i];
      xFire->set_image (o);
      xFire->put_method = cycle;
    }
}

//-----------------------------------------------------------------------------
// static method: collision of all bumper's fire with the bricks
//-----------------------------------------------------------------------------
void
sprite_projectile::collision1 ()
{

  Sint32 bwght = brickObjet->getBkWidth ();     //brick's width in pixels
  Sint32 byoff = brickObjet->getYOffset ();     //y-offset between 2 bricks
  Sint32 indus = brickObjet->getBkIndus ();     //first indestructible brick
  sprite_projectile **liste = list_fires;
  Sint32 t = total_fire;
  brickInfos *tMega = brickObjet->mega_table;
  brickClear *briPT = brickObjet->brique_pnt;
  Sint32 save = brickObjet->briqueSave;
  for (Sint32 i = 0; i < t; i++)
    {
      sprite_projectile *xFire = *(liste++);
      if (xFire->is_enabled)
        {
          Sint32 x = xFire->x_coord + 2;
          Sint32 y = xFire->y_coord + 2;
          brickClear *briP2 = briPT + save;
          briP2->balle_posX = x;
          briP2->balle_posY = y;
          x /= bwght;
          y /= byoff;
          y *= lesBriques::NB_BRICKSH;
          x += y;
          brickInfos *megaT = (tMega + x);
          x = megaT->brique_rel;
          if (x)
            {
              if (xFire->is_enabled == 1)
                xFire->is_enabled = 0;
              briP2->raquettePT = xFire->raquettePT;
              if ((x -= indus) >= 0)
                {               //###################################################
                  // touched indestructible brick
                  //###################################################                                   
                  if ((x -= bwght) > 0) //indestructible-destructible bricks?
                    {
                      // fire destroys the indestructibles-destructibles bricks
                      if (xFire->fire_power)
                        {
                          briP2->balle_posX = -1;
                          briP2->adresseAff = megaT->adresseAff;
                          briP2->adresseTab = megaT;
                          megaT->brique_rel = 0;        // RAZ code brique
                          briP2->brique_num = megaT->brique_num;
                          briP2->briqueFlag = 1;        //1 = restore background
                          save += 1;    // inc. pt/restaure table
                          save &= (lesBriques::MAXBRIKCLR - 1);
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
                      x = 1;    //the brick is really indestructible
#ifndef SOUNDISOFF
                      audio->play_sound (S_TOINDES1);
#endif
                    }
                }
              else
                {               //###################################################
                  // touched normal brick
                  //###################################################
                  briP2->adresseAff = megaT->adresseAff;
                  briP2->adresseTab = megaT;
                  x = xFire->firePowerX;        // fire power : 1 or 2
                  megaT->briquePosX = megaT->briquePosX - (x * 2);
                  if (megaT->briquePosX <= 0)
                    {
                      megaT->briquePosX = 0;
                      megaT->brique_rel = 0;
                      briP2->brique_num = megaT->brique_num;
                      briP2->briqueFlag = 1;    // flag restaure background 
                    }
                  else
                    {
                      megaT->brique_rel = megaT->brique_rel - (x * bwght);
                      briP2->brique_num = megaT->brique_rel;
                      briP2->briqueFlag = 0;    // flag display brick
                    }
                  save += 1;    // inc. pt/restaure table
                  save &= (lesBriques::MAXBRIKCLR - 1);
                }
            }
        }
    }
  brickObjet->briqueSave = save;
}

//-----------------------------------------------------------------------------
// static method: collision of all bumper's fire with the BouiBouis
//-----------------------------------------------------------------------------
void
sprite_projectile::collision2 ()
{
  sprite_projectile **liste = list_fires;
  Sint32 t = atomsObjet->get_max_of_sprites ();
  sprite_ship **aList = atomsObjet->get_sprites_list ();
  for (Sint32 i = 0; i < total_fire; i++)
    {
      sprite_projectile *xFire = *(liste++);
      if (!xFire->is_enabled)
        {
          continue;
        }
      sprite_ship **monPT = aList;
      Sint32 y1 = xFire->y_coord;
      Sint32 y2 = y1 + 3;
      y1 -= 26;
      Sint32 x1 = xFire->x_coord;
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
          if (xFire->is_enabled == 1)
            {
              xFire->is_enabled = 0;
            }
          scoreObjet->scoreAjout (100);
          k = xFire->firePowerX;
          atome->atom_power -= k;
          if (atome->atom_power < 1)
            {
              atome->explosion1 (xFire);
            }
        }
    }
}

//-----------------------------------------------------------------------------
// static method: collision of all bumper's fire with the BouiBouis
//-----------------------------------------------------------------------------
void
sprite_projectile::disable_sprites ()
{
  sprite_projectile **liste = list_fires;
  for (Sint32 i = 0; i < total_fire; i++)
    {
      sprite_projectile *xFire = *(liste++);
      xFire->is_enabled = 0;
    }
}

Sint32 sprite_projectile::total_fire = 0;
sprite_projectile *
  sprite_projectile::list_fires[maxi_fires];
lesBriques *
  sprite_projectile::brickObjet;
zeBouiBoui *
  sprite_projectile::atomsObjet;
barreScore *
  sprite_projectile::scoreObjet;
