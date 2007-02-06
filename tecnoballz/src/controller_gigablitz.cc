/** 
 * @file controller_gigablitz.cc 
 * @brief Gigablitz controller 
 * @date 2007-02-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_gigablitz.cc,v 1.1 2007/02/06 20:41:33 gurumeditation Exp $
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
#include "../include/controller_gigablitz.h"
#include "../include/handler_resources.h"
//..............................................................................
Sint32
  controller_gigablitz::numeroBOBs[NOMBREGIGA] = { BOB_GIGAB7,
  BOB_GIGAB6,
  BOB_GIGAB5,
  BOB_GIGAB4,
  BOB_GIGAB3,
  BOB_GIGAB2,
  BOB_GIGAB1
};

//..............................................................................

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
controller_gigablitz::controller_gigablitz ()
{
  littleInit ();
  max_of_sprites = NOMBREGIGA;  // there are 7 different Gigablitz
  /* shadow disable */
  sprites_have_shades = false;
  brickObjet = (controller_bricks *) NULL;
  gugusObjet = (head_anima *) NULL;
  ptRaquette = (controller_paddles *) NULL;
  paddle_bottom = (sprite_paddle *) NULL;
  paddle_top = (sprite_paddle *) NULL;
  blitz_posy = 0;
  blitz_haut = 0;
  bitz_ystop = 0;
  bitz_maxiy = 0;
  bitz_miniy = 0;
  blitz_seta = 0;
  blitz_brik = 0;
  blitz_xsin = 0;
  blitz_colx = 0;
}

//-------------------------------------------------------------------------------
// release the object
//-------------------------------------------------------------------------------
controller_gigablitz::~controller_gigablitz ()
{
  release_sprites_list ();
}

//-------------------------------------------------------------------------------
// bricks levels: perform some initializations of gigablitz
//-------------------------------------------------------------------------------
Sint32
controller_gigablitz::init_liste (controller_paddles * zeRak,
                                  head_anima * gugus,
                                  controller_bricks * brick)
{
  gugusObjet = gugus;
  ptRaquette = zeRak;
  brickObjet = brick;
  paddle_bottom = ptRaquette->get_paddle (1);   // top bumper
  paddle_top = ptRaquette->get_paddle (3);      // bottom bumper

  if (max_of_sprites)
    {


      alloc_sprites_list ();
      // unpack bitmap of the Gigablitz
      resources->load_sprites_bitmap (handler_resources::RESGIGABLZ);

      // initialize the objects "sprite_gigablitz"
      for (Sint32 i = 0; i < max_of_sprites; i++)
        {
          sprite_gigablitz *g = new sprite_gigablitz ();
          g->set_object_pos (i);
          //g->afflignesF = 1;
          g->set_draw_method (sprite_object::DRAW_LINE_BY_LINE);
          Sint32 n = numeroBOBs[i];
          g->create_sprite (n, sprites_bitmap, 0);
          sprites->add (g);
          sprites_list[i] = g;
        }

      // release the bitmap page of gigablitz
      resources->release_sprites_bitmap ();
    }
  return (erreur_num);
}

//-------------------------------------------------------------------------------
// bricks levels : start a new Gigablitz
//-------------------------------------------------------------------------------
void
controller_gigablitz::initDepart ()
{
  if (!blitz_haut)
    {
      Sint32 large = paddle_bottom->get_length ();
      Sint32 l = large;
      l -= paddle_bottom->width_mini;   //smallest bumper is of 16/32 pixels width
      l >>= paddle_bottom->width_deca;  //size of bumper step by 8/16 pixels
      l = NOMBREGIGA - l - 1;
      sprite_gigablitz *g = sprites_list[l];
      blitzobjet = g;
      blitz_haut = g->get_sprite_height ();
      Sint32 x = paddle_bottom->get_x_coord ();
      Sint32 y = paddle_bottom->get_y_coord ();
      blitz_posx = x;
      blitz_colx = x;           //special collision
      g->set_coordinates (x, y);
      //bitz_ystop = paddle_top->get_y_coord() - blitz_haut;
      bitz_ystop = 8 * resolution - blitz_haut;
      bitz_maxiy = paddle_bottom->get_y_coord ();
      //bitz_miniy = paddle_top->get_y_coord();
      bitz_miniy = 8 * resolution;
      y = large;
      if (resolution == 1)
        {
          y = y >> 4;           //in 320 width bricks = 16 pixels
          x = x & 0x000f;
        }
      else
        {
          y = y >> 5;           //in 640 width bricks = 32 pixels
          x = x & 0x001f;
        }
      if (x)
        y++;
      blitz_brik = y;
#ifndef SOUNDISOFF
      audio->play_sound (S_TIR_GARD);
#endif
      gugusObjet->teterigole ();
    }
}

// ------------------------------------------------------------------------------
// bricks levels: move the Gigablitz
// ------------------------------------------------------------------------------
void
controller_gigablitz::execution1 ()
{
  if (!blitz_haut)
    return;
  sprite_gigablitz *g = blitzobjet;

  //###################################################################
  // vertical moving
  //###################################################################
  Sint32 y = g->get_y_coord ();
  y = y - (8 * resolution);
  if (y <= bitz_ystop)
    {
      g->disable ();
      blitz_haut = 0;
    }
  else
    {
      if (y >= bitz_maxiy)
        g->disable ();
      else
        g->enable ();
    }

  //###################################################################
  // determine last line of the gigablitz sprite
  //###################################################################
  Sint32 h = g->get_sprite_height ();
  Sint32 l = bitz_maxiy - y;
  if (l > h)
    l = h;
  if (l < 1)
    l = 1;
  g->affligLast = l;
  l = bitz_miniy - y;
  if (l >= h)
    l = h - 1;
  if (l < 0)
    l = 0;
  g->affligFrst = l;

  //###################################################################
  // horizontal move
  //###################################################################
  blitz_xsin = (blitz_xsin + 50) & SINUS_MASK;
  Sint32 x = (table_cosL[blitz_xsin] * 5 * resolution) >> SINUS_DECA;
  x = blitz_posx + x;

  blitzobjet->set_coordinates (x, y);
  if (y >= 0)
    collision1 ();
}

//-------------------------------------------------------------------------------
// bricks levels: collision with the gigablitz and briks
//-------------------------------------------------------------------------------
void
controller_gigablitz::collision1 ()
{

  Sint32 bwght = brickObjet->get_brick_width ();        //brick's width in pixels
  Sint32 byoff = brickObjet->getYOffset ();     //y-offset between 2 bricks
  Sint32 indus = brickObjet->getBkIndus ();     //first indestructible brick    
  if (blitz_brik > 0)
    {                           //Sint32 x = blitzobjet->get_x_coord();
      Sint32 x = blitz_colx;
      Sint32 y = blitzobjet->get_y_coord ();
      x /= bwght;               // x = x / 32 (width of a brick)
      y /= byoff;               // y = y / 16 (space between two bricks in height)
      y *= controller_bricks::NB_BRICKSH;       // y = y * 16 (number of bricks on the same line)
      x += y;
      brickInfos *tMega = brickObjet->mega_table;
      Sint32 save = brickObjet->briqueSave;     // save => offset on "brique_pnt"
      brickClear *briPT = brickObjet->brique_pnt;       // pointer to structure "brickClear" (display and clear the bricks)
      for (Sint32 i = 0; i < blitz_brik; i++)
        {
          brickInfos *megaT = (tMega + x);
          brickClear *briP2 = briPT + save;
          Sint32 v = megaT->brique_rel;
          if (v)
            {
              if (v < indus)
                briP2->balle_posX = 512;        // flag brick blitz destroy
              else
                briP2->balle_posX = -1;
              briP2->adresseAff = megaT->adresseAff;
              briP2->adresseTab = megaT;
              megaT->briquePosX = -1;
              megaT->brique_rel = 0;    // RAZ brick code
              briP2->brique_num = megaT->brique_num;    // brick number
              briP2->briqueFlag = 1;    // flag restaure background
              save += 1;        // inc. pt restaure table
              save &= (controller_bricks::MAXBRIKCLR - 1);
            }
          x++;
        }
      brickObjet->briqueSave = save;
    }
}



//-------------------------------------------------------------------------------
// guards levels: perform some initializations of gigablitz
//-------------------------------------------------------------------------------
Sint32
controller_gigablitz::init_liste (controller_paddles * zeRak,
                                  zexplosion * pexpl)
{
  pexplosion = pexpl;
  ptRaquette = zeRak;
  paddle_bottom = ptRaquette->get_paddle (1);   // top bumper
  if (max_of_sprites)
    {

      alloc_sprites_list ();
      // unpack bitmap of the Gigablitz
      resources->load_sprites_bitmap (handler_resources::RESGIGABLZ);

      // initialize the objects "sprite_gigablitz"
      for (Sint32 i = 0; i < max_of_sprites; i++)
        {
          sprite_gigablitz *g = new sprite_gigablitz ();
          g->set_object_pos (i);
          g->mirrorVert = 1;
          g->set_draw_method (sprite_object::DRAW_LINE_BY_LINE);
          Sint32 n = numeroBOBs[i];
          g->create_sprite (n, sprites_bitmap, 0);
          sprites->add (g);
          sprites_list[i] = g;
        }

      // release the bitmap page of gigablitz
      resources->release_sprites_bitmap ();
    }
  return (erreur_num);
}

// ------------------------------------------------------------------------------
// guards levels: move the Gigablitz
// ------------------------------------------------------------------------------
void
controller_gigablitz::execution2 ()
{
  if (blitz_haut)
    {
      sprite_gigablitz *g = blitzobjet;
      Sint32 y = g->get_y_coord ();
      y = y + (6 * resolution);
      blitz_xsin = (blitz_xsin + 50) & SINUS_MASK;
      Sint32 x = (table_cosL[blitz_xsin] * 5 * resolution) >> SINUS_DECA;
      x = blitz_posx + x;
      blitzobjet->set_coordinates (x, y);


      //###################################################################
      // determine last line of the gigablitz sprite
      //###################################################################
      Sint32 h = g->get_sprite_height ();
      Sint32 l = display->get_height () - y;
      if (l > h)
        l = h;
      if (l < 1)
        l = 1;
      g->affligLast = l;
      l = bitz_miniy - y;
      if (l >= h)
        l = h - 1;
      if (l < 0)
        l = 0;
      g->affligFrst = l;






      if (y >= 240 * resolution)
        {
          g->disable ();
          blitz_haut = 0;
        }
      collision2 ();            //collision bumper and Gigablitz
    }
  collision2 ();
}

// ------------------------------------------------------------------------------
// guards levels: collision with the gigablitz and the bumper
// ------------------------------------------------------------------------------
void
controller_gigablitz::collision2 ()
{
  if (blitz_haut && !paddle_bottom->is_invincible ())
    {
      Sint32 gx = blitzobjet->get_x_coord ();
      Sint32 gy = blitzobjet->get_y_coord ();
      //Sint32 gh = blitzobjet->get_sprite_height();
      Sint32 gw = blitzobjet->get_collision_width ();
      Sint32 bx = paddle_bottom->get_x_coord ();
      Sint32 by = paddle_bottom->get_y_coord ();
      Sint32 bw = paddle_bottom->get_length ();
      Sint32 bh = paddle_bottom->get_sprite_height ();
      /*printf("controller_gigablitz::collision2(): get_collision_width=%i / get_sprite_width=%i / get_sprite_height=%i /  blitz_haut =%i\n",
         blitzobjet->get_collision_width(), blitzobjet->get_sprite_width(),
         blitzobjet->get_sprite_height(), blitz_haut); */

      if (gy + blitz_haut >= by &&
          gx + gw >= bx && gx <= bx + bw && gy <= by + bh)
        {
          paddle_bottom->set_invincibility (100);
#ifndef SOUNDISOFF
          audio->play_sound (S_RAKEXPLO);
          audio->play_sound (S_ENLEVVIE);
#endif
          joueurGere->lifesMoins (1);
          pexplosion->add_explos (bx + paddle_bottom->get_length () / 2,
                                  by +
                                  paddle_bottom->get_sprite_height () / 2);
        }
    }
}

//-------------------------------------------------------------------------------
// guard phase : start new Gigablitz 
// input        => value : 0, 1, 2, 3, 4, 5, 6, 7
//                      => pos_x :
//                      => pos_y :
//                      => large :
//                      => haute :
//-------------------------------------------------------------------------------
Sint32
controller_gigablitz::guard_shoot (Sint32 value, Sint32 pos_x, Sint32 pos_y,
                                   Sint32 large, Sint32 haute)
{
  if (!blitz_haut)
    {
      sprite_gigablitz *g = sprites_list[value];
      //g = sprites_list[0];  // test only
      blitzobjet = g;
      blitz_haut = g->get_sprite_height ();
      Sint32 bwdth = g->get_sprite_width ();
      g->set_coordinates (pos_x, pos_y);
      pos_x = pos_x + ((large - bwdth) / 2);
      if (pos_x < 0)
        pos_x = 0;
      g->set_coordinates (pos_x, pos_y);
#ifndef SOUNDISOFF
      audio->play_sound (S_TIR_GARD);
#endif
      g->enable ();
      blitz_haut = g->get_sprite_height ();
      blitz_posx = pos_x;
      return 1;
    }
  return 0;
}

//-------------------------------------------------------------------------------
// gigablitz is active ?
//-------------------------------------------------------------------------------
Sint32
controller_gigablitz::isactivate ()
{
  return blitz_haut;
}