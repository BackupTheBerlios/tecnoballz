/** 
 * @file sprite_ship.cc 
 * @brief A flying enemy ships sprite 
 * @date 2007-04-12
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.10 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_ship.cc,v 1.10 2007/09/12 06:32:49 gurumeditation Exp $
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
#include "../include/sprite_ship.h"

/**
 * Create the flying enemy ships sprite
 */
sprite_ship::sprite_ship ()
{
  atom_xmini = ATOM_XMINI * resolution;
  atom_xmaxi = ATOM_XMAXI * resolution;
  atom_ymini = ATOM_YMINI * resolution;
  atom_ymaxi = ATOM_YMAXI * resolution;
  clear_sprite_members ();
  littleInit (0, 0, 0, 0, 0, 0, 0);
}

/**
 * Release the flying enemy ships sprite
 */
sprite_ship::~sprite_ship ()
{
}

//-----------------------------------------------------------------------------
// displacement of the "Bouiboui"
//-----------------------------------------------------------------------------
void
sprite_ship::gere_atome ()
{
  //###################################################################
  // explosion of the BouiBoui
  //###################################################################
  if (atom_explo)
    atom_explo = play_animation_once ();

  else
    {
      //###############################################################
      // BouiBoui appears
      //###############################################################
      if (atom_actif)           // atom_actif > 0 BouiBoui's not active
        {
          if (!(--atom_actif))
            {
              if (!is_collisions_with_bricks (x_coord, y_coord))
                {
                  is_enabled = 1;
                  Sint32 *monPT = ghost_bobs + (random_counter & 31);
#if __WORDSIZE == 64
                  random_counter += (long) this;
#else
                  random_counter += (Sint32) this;
#endif
                  Sint32 k = *monPT;
                  frame_index_min = k;
                  frame_index = k;
                  frame_index_max = k + ATOM_ANIMA - 1;
                  sprite_has_shadow = 1;
                  set_image (k);
#ifndef SOUNDISOFF
                  audio->play_sound (S_ATOMAPPA);
#endif
                }
              else
                atom_actif = 1;
            }
        }

      //###############################################################
      // BouiBoui change its trajectory 
      //###############################################################
      if (!(--atom_count))
        {
          atom_ghost++;
          atom_ghost &= 15;     //16 values possible of standby
          Sint32 *depla = tableGhost + atom_ghost;
          atom_count = *depla;
          atom_traje += 2;
          atom_traje &= 30;     //16 values possible of trajectory
          depla = ghost_traj + atom_traje;
          atom_deplX = *(depla++) * resolution;
          atom_deplY = *depla * resolution;

          if (object_pos > 3)
            {
              atom_deplX = -atom_deplX;
              atom_deplY = -atom_deplY;
            }

        }
      play_animation_loop ();


      //###############################################################
      // initialize position of the Bouiboui
      //###############################################################
      Sint32 x = x_coord + atom_deplX;
      if (x < atom_xmini)
        x = atom_xmini;
      else if (x > atom_xmaxi)
        x = atom_xmaxi;
      Sint32 y = y_coord + atom_deplY;
      if (y < atom_ymini)
        y = atom_ymini;
      else if (y > atom_ymaxi)
        y = atom_ymaxi;
      if (!is_collisions_with_bricks (x, y) || atom_actif)
        {
          x_coord = x;
          y_coord = y;
        }
    }
}

/**
 * Check collisions between a ship and bricks
 * @param xcoord x coordinate of the ship
 * @param ycoord y coordinate of the ship
 * @return true if collision detected, otherwise false
 */
bool
sprite_ship::is_collisions_with_bricks (Uint32 xcoord, Uint32 ycoord)
{
  controller_bricks* bricks = controller_bricks::get_instance ();
  Sint32 bwght = bricks->get_brick_width ();     //brick's width in pixels
  Sint32 byoff = bricks->getYOffset ();     //y-offset between 2 bricks
  brick_info *tMega = bricks->get_bricks_map ();
  Sint32 c = 0;
  for (Sint32 i = 0; i < 4; i++)
    {
      Sint32 x = xcoord + collisionT[c++];
      Sint32 y = ycoord + collisionT[c++];
      x /= bwght;               // /32
      y /= byoff;               // /16
      y *= controller_bricks::NB_BRICKSH;
      x += y;
      brick_info *megaT = (tMega + x);
      x = megaT->brique_rel;
      if (x)
        {
          return true;
        }
    }
  return false;
}

//-----------------------------------------------------------------------------
// perform some initializations
//      input   => time0: time before activation
//                      => appar: time before the first activation
//                      => index: number of the table of standby values (0 to 3)
//                      => power: strength
//                      => pos_x: absciss 
//                      => pos_y: ordinate
//                      => offst: number of the image's explosion
//-----------------------------------------------------------------------------
void
sprite_ship::littleInit (Sint32 time0, Sint32 appar, Sint32 index,
                         Sint32 power, Sint32 pos_x, Sint32 pos_y,
                         Sint32 offst)
{
  atom_actif = appar;           //time before activation
  apparition = time0;
  tableGhost = ghost_wait[index];       //table of the 16 standby values 
  atom_power = power;           //strength
  init_power = power;           //strength
  x_coord = pos_x;
  y_coord = pos_y;
  atom_oexpl = offst;           //number of the image's explosion
  atom_explo = 0;
  atom_ghost = 0;               //pointer on "tableGhost"
  atom_deplX = 0;               //offset X
  atom_deplY = 0;               //offset Y
  Sint32 i = random_counter & 15;
  i++;
  i *= object_pos;
  atom_count = i;               //trajectory change counter 
  frame_period = 3;             //rate animation
  frame_delay = 1;              //rate animation counter
  i *= 2;
  i &= 31;
  atom_traje = i;
#if __WORDSIZE == 64
  random_counter += (long) this;
#else
  random_counter += (Sint32) this;
#endif
  i = 4 * resolution;

  collisionT[0] = i;
  collisionT[1] = i;

  collisionT[2] = collision_width - i;
  collisionT[3] = i;

  collisionT[4] = collision_width - i;
  collisionT[5] = collision_height - i;

  collisionT[6] = i;
  collisionT[7] = collision_height - i;

}

/**
 * Ship destroyed by a projectile: send gem, money or malus
 * @param blast a pointer to a prjectile sprite
 */
void
sprite_ship::explosion1 (sprite_projectile * blast)
{
  if (atom_actif > 0)
    return;
  explosion2 ();
  Sint32 h = codeBounty[random_counter & 0xF];
  //h = CODE_GEMME; //test only
  switch (h)
    {
    case CODE_GEMME:
      {
        controller_gems* gems = controller_gems::get_instance ();
        gems->send (blast);
      }
      break;
    case CODE_MALUS:
      {
        controller_capsules* capsules = controller_capsules::get_instance ();
        capsules->send_malus (blast);
      }
      break;
    case CODE_MONEY:
      { 
        controller_moneys* moneys = controller_moneys::get_instance ();
        moneys->send_money (blast);
      }
      break;
    }
}

/**
 * Ship destroyed by a ball: send gem, money or malus
 * @param ball a pointer to a balll sprite
 */
void
sprite_ship::explosion1 (sprite_ball * ball)
{
  if (atom_actif > 0)
    {
      return;
    }
  explosion2 ();
  Uint32 h = codeBounty[random_counter & 0xF];
  //h = CODE_GEMME; //test only
  switch (h)
    {
    case CODE_GEMME:
      {
        controller_gems* gems = controller_gems::get_instance ();
        gems->send (ball);
      }
      break;
    case CODE_MALUS:
      {
        controller_capsules* capsules = controller_capsules::get_instance ();
        capsules->send_malus (ball);
      }
      break;
    case CODE_MONEY:
      { 
        controller_moneys* moneys = controller_moneys::get_instance ();
        moneys->send_money (ball);
      }
      break;
    }
}

//-----------------------------------------------------------------------------
// explosion of a BouiBoui
//-----------------------------------------------------------------------------
void
sprite_ship::explosion2 ()
{
  sprite_has_shadow = 0;        // no shadow
  random_counter = random_counter + frame_index;
  atom_explo = 1;
  atom_power = init_power;      // strength
  atom_actif = apparition + (random_counter & 63);  // time before activation
  frame_index = atom_oexpl;
  frame_index_min = atom_oexpl;
  frame_index_max = atom_oexpl + ATOM_ANIMA - 1;
  set_image (frame_index);
  frame_delay = frame_period;
#ifndef SOUNDISOFF
  audio->play_sound (S_ATOM_EXP);
#endif
}

//-----------------------------------------------------------------------------
// values of possible time during which a BouiBoui keep the same trajectory
//-----------------------------------------------------------------------------
Sint32
sprite_ship::ghostWait1[16] =
  { 50, 50, 30, 50, 20, 12, 8, 50, 45, 32, 12, 56, 33, 22, 22, 20
};
Sint32
sprite_ship::ghostWait2[16] =
  { 40, 20, 30, 47, 22, 33, 50, 50, 55, 52, 42, 32, 42, 32, 42, 32
};
Sint32
sprite_ship::ghostWait3[16] =
  { 10, 10, 20, 27, 12, 13, 20, 10, 10, 20, 10, 25, 26, 12, 32, 33
};
Sint32
sprite_ship::ghostWait4[16] =
  { 60, 20, 50, 37, 42, 33, 33, 20, 20, 22, 33, 70, 20, 22, 32, 32
};
Sint32 *
sprite_ship::ghost_wait[4] =
  { ghostWait1, ghostWait2, ghostWait3, ghostWait4
};

//-----------------------------------------------------------------------------
// offsets x, y 
//-----------------------------------------------------------------------------
Sint32
  sprite_ship::ghost_traj[32] = { 0, 1,
  0, 0,
  -1, 0,
  0, 0,
  0, -1,
  -1, 1,
  -1, -1,
  -1, 0,
  -1, 1,
  1, -1,
  1, -1,
  1, 1,
  1, 1,
  1, 0,
  0, 1,
  0, 1
};

//-----------------------------------------------------------------------------
// index animations images of sprites (20 BouiBouis differents)
//-----------------------------------------------------------------------------
Sint32
  sprite_ship::ghost_bobs[32] =
  { 0 * ATOM_ANIMA, 1 * ATOM_ANIMA, 2 * ATOM_ANIMA, 3 * ATOM_ANIMA,
  4 * ATOM_ANIMA, 5 * ATOM_ANIMA, 6 * ATOM_ANIMA, 7 * ATOM_ANIMA,
  8 * ATOM_ANIMA, 9 * ATOM_ANIMA, 10 * ATOM_ANIMA, 11 * ATOM_ANIMA,
  12 * ATOM_ANIMA, 13 * ATOM_ANIMA, 14 * ATOM_ANIMA, 15 * ATOM_ANIMA,
  16 * ATOM_ANIMA, 17 * ATOM_ANIMA, 18 * ATOM_ANIMA, 19 * ATOM_ANIMA,
  0 * ATOM_ANIMA, 1 * ATOM_ANIMA, 2 * ATOM_ANIMA, 3 * ATOM_ANIMA,
  4 * ATOM_ANIMA, 5 * ATOM_ANIMA, 6 * ATOM_ANIMA, 7 * ATOM_ANIMA,
  8 * ATOM_ANIMA, 9 * ATOM_ANIMA, 10 * ATOM_ANIMA, 11 * ATOM_ANIMA
};

//-----------------------------------------------------------------------------
// code "bounty" when a BouiBoui is destroyed 
//-----------------------------------------------------------------------------
Sint32
  sprite_ship::codeBounty[16] = { CODE_GEMME,
  CODE_MONEY,
  CODE_MALUS,
  CODE_MONEY,
  CODE_MALUS,
  CODE_MALUS,
  CODE_MONEY,
  CODE_MALUS,
  CODE_MALUS,
  CODE_MALUS,
  CODE_MONEY,
  CODE_MALUS,
  CODE_MALUS,
  CODE_MALUS,
  CODE_MALUS,
  CODE_MONEY
};
