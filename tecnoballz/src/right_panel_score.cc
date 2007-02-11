/** 
 * @file right_panel_score.cc 
 * @brief The right panel score in the bricks levels 
 * @date 2007-02-11
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: right_panel_score.cc,v 1.3 2007/02/11 21:03:24 gurumeditation Exp $
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
#include "../include/right_panel_score.h"
#include "../include/handler_resources.h"
#include "../include/scoretable.h"

/**
 * Create the right panel score
 */
right_panel_score::right_panel_score ()
{
  GFX_Sbarre = (bitmap_data *) NULL;
  initial_me ();
  temoinhaut = TEMOINHAUT * resolution;
  blitzcount = temoinhaut;
  blitztempo = TEMPOBLITZ;
  flip_white = 0;
}

/**
 * Release the right panel score
 */
right_panel_score::~right_panel_score ()
{
  if (NULL != GFX_Sbarre)
    {
      delete GFX_Sbarre;
      GFX_Sbarre = (bitmap_data *) NULL;
    }
  destroy_me ();
}

//-------------------------------------------------------------------------------
// first initialization
//-------------------------------------------------------------------------------
void
right_panel_score::first_init (controller_gigablitz * blitz,
                               controller_balls * b)
{
  error_init (initialise ());
  gigablitz = blitz;
  balls = b;
  scoreAdres = game_screen->get_pixel_data
    (POSX_SCORE * resolution, POSY_SCORE * resolution);
  lifesAdres = game_screen->get_pixel_data
    (POSX_LIFES * resolution, POSY_LIFES * resolution);
  brickAdres = game_screen->get_pixel_data
    (POSX_BRICK * resolution, POSY_BRICK * resolution);
  temoinAdrs = game_screen->get_pixel_data
    (TEMOINPOSX * resolution, TEMOINPOSY * resolution);
  draw_background ();
}

/**
 * Draw the background of the panel score
 */
void
right_panel_score::draw_background ()
{
  GFX_Sbarre = new bitmap_data ();
  GFX_Sbarre->load (handler_resources::RESBASCORE);
  GFX_Sbarre->copyTampon (0, 0, 256 * resolution, 0, 64 * resolution,
                          240 * resolution);
  tamponAff1 (POSX_AREAN * resolution, POSY_AREAN * resolution,
              current_player->area_number, 10);
  tamponAff1 (POSX_LEVEL * resolution, POSY_LEVEL * resolution,
              current_player->level_number, 10);
  tamponAff2 (POSX_NOMJO * resolution, POSY_NOMJO * resolution,
              current_player->player_name, 6);
  tamponAff1 (POSX_BESTS * resolution, POSY_BESTS * resolution,
              ptScoreTab->best_score (), 100000);
  tamponAff2 (POSX_BESTN * resolution, POSY_BESTN * resolution,
              ptScoreTab->bestPlayer (), 6);

  barreTemoin ();
  delete GFX_Sbarre;
  GFX_Sbarre = (bitmap_data *) NULL;
}

//-------------------------------------------------------------------------------
// display sore, number of lifes, number of bricks
//-------------------------------------------------------------------------------
void
right_panel_score::scoreEcran ()
{
  affNombre1 (scoreAdres, current_player->superScore, 100000);
  affNombre1 (lifesAdres, superBrick, 100);
  affNombre1 (brickAdres, current_player->number_of_lifes, 10);
}

/*
void
right_panel_score::scoreAjout (Sint32 ajout)
{
  current_player->add_scores (ajout);
}
*/

/*
//-------------------------------------------------------------------------------
// increase the number of lifes
//-------------------------------------------------------------------------------
void
right_panel_score::add_life (Sint32 ajout)
{
  current_player->number_of_lifes += ajout;
}

//-------------------------------------------------------------------------------
// decrease the number of lifes
//-------------------------------------------------------------------------------
Sint32
right_panel_score::remove_life (Sint32 retra)
{
  current_player->number_of_lifes -= retra;
  if (current_player->number_of_lifes > 0)
    return 1;
  else
    {
      current_player->number_of_lifes = 0;
      return 0;
    }
}
*/

//-------------------------------------------------------------------------------
// decrease the number of bricks
//-------------------------------------------------------------------------------
void
right_panel_score::brickMoins (Sint32 retra)
{
  superBrick -= retra;
  if (superBrick < 1)
    superBrick = 0;
}

//-------------------------------------------------------------------------------
// return the number of bricks
//-------------------------------------------------------------------------------
Sint32
right_panel_score::resteBrick ()
{
  return (superBrick);
}

//-------------------------------------------------------------------------------
// intialize the number of bricks
//-------------------------------------------------------------------------------
void
right_panel_score::scoreBrick (Sint32 value)
{
  superBrick = value;
}

//-------------------------------------------------------------------------------
// display indicator bar of gigablitz
//-------------------------------------------------------------------------------
void
right_panel_score::barreTemoin ()
{
  if (keyboard->is_right_left_buttons () && !gigablitz->isactivate () &&
      !balls->least_glue ())
    {
      if (--blitztempo <= 0)
        {
          if (blitzcount > 0)
            {
              blitztempo = TEMPOBLITZ;
              blitzcount = blitzcount - resolution;
              if (blitzcount == 0)      // indicator is highest?
                gigablitz->initDepart ();      // yes, launch the gigablitz!
            }
        }
    }
  else
    {
      if (blitzcount < (temoinhaut))
        {
          blitztempo = TEMPOBLITZ;
          blitzcount = blitzcount + resolution;
        }
    }

  char *d = temoinAdrs;
  Sint32 m = game_screen->get_width ();
  char p = 0;


  if (resolution == 1)
    {
      for (Sint32 i = 0; i < blitzcount; i++)
        {
          d[0] = p;
          d[1] = p;
          d[2] = p;
          d[3] = p;
          d[4] = p;
          d += m;
        }
    }
  else
    {
      for (Sint32 i = 0; i < blitzcount; i++)
        {
          d[0] = p;
          d[1] = p;
          d[2] = p;
          d[3] = p;
          d[4] = p;
          d[5] = p;
          d[6] = p;
          d[7] = p;
          d[8] = p;
          d[9] = p;
          d += m;
        }
    }



  Sint32 h = (temoinhaut) - blitzcount;

  flip_white = ~flip_white;
  if (flip_white)
    {
      char p = 130;

      if (resolution == 1)
        {
          for (Sint32 i = 0; i < h; i++)
            {
              d[0] = p;
              d[1] = p;
              d[2] = p;
              d[3] = p;
              d[4] = p;
              d += m;
            }
        }
      else
        {
          for (Sint32 i = 0; i < h; i++)
            {
              d[0] = p;
              d[1] = p;
              d[2] = p;
              d[3] = p;
              d[4] = p;
              d[5] = p;
              d[6] = p;
              d[7] = p;
              d[8] = p;
              d[9] = p;
              d += m;
            }
        }


    }
  else
    {
      if (resolution == 1)
        {
          unsigned char *e = temoinCol1;
          for (Sint32 i = 0; i < h; i++)
            {
              char p = (char) e[i];
              d[0] = p;
              d[1] = p;
              d[2] = p;
              d[3] = p;
              d[4] = p;
              d += m;
            }
        }
      else
        {
          unsigned char *e = temoinCol2;
          for (Sint32 i = 0; i < h; i++)
            {
              char p = (char) e[i];
              d[0] = p;
              d[1] = p;
              d[2] = p;
              d[3] = p;
              d[4] = p;
              d[5] = p;
              d[6] = p;
              d[7] = p;
              d[8] = p;
              d[9] = p;
              d += m;
            }
        }
    }
}

//-------------------------------------------------------------------------------
// RAZ indicator bar of gigablitz
//-------------------------------------------------------------------------------
void
right_panel_score::resetemoin ()
{
  blitzcount = temoinhaut;
}

//-------------------------------------------------------------------------------
// list of 54 couleurs from gigablitz
//-------------------------------------------------------------------------------
unsigned char
  right_panel_score::temoinCol1[TEMOINHAUT] = { 255,
  255,
  254,
  254,
  253,
  253,
  252,
  252,
  251,
  250,
  250,
  249,
  248,
  248,
  247,
  246,
  246,
  245,
  244,
  244,
  243,
  242,
  242,
  241,
  240,
  240,
  239
};

unsigned char
  right_panel_score::temoinCol2[TEMOINHAUT * 2] = { 255,
  255,
  255,
  255,
  254,
  254,
  254,
  254,
  253,
  253,
  253,
  253,
  252,
  252,
  252,
  251,
  251,
  251,
  250,
  250,
  250,
  249,
  249,
  249,
  248,
  248,
  248,
  247,
  247,
  247,
  246,
  246,
  246,
  245,
  245,
  245,
  244,
  244,
  244,
  243,
  243,
  243,
  242,
  242,
  242,
  241,
  241,
  241,
  240,
  240,
  240,
  239,
  239,
  239
};
