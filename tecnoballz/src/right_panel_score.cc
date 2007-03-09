/** 
 * @file right_panel_score.cc 
 * @brief The right panel score in the bricks levels 
 * @date 2007-02-28
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.12 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: right_panel_score.cc,v 1.12 2007/03/09 17:18:34 gurumeditation Exp $
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
#include "../include/handler_high_score.h"

right_panel_score * right_panel_score::panel_score_singleton = NULL;

/**
 * Create the right panel score
 */
right_panel_score::right_panel_score ()
{
  initial_me ();
  gauge_height = GAUGE_HEIGHT * resolution;
  gigablitz_countdown = gauge_height;
  delay_gigablitz_countdown = 0; 
  flip_white = 0;
}

/**
 * Release the right panel score
 */
right_panel_score::~right_panel_score ()
{
  destroy_me ();
  panel_score_singleton = NULL;
}

/**
 * Get the object instance
 * right_panel_score is a singleton
 * @return the right_panel_score object 
 */
right_panel_score *
right_panel_score::get_instance ()
{
  if (NULL == panel_score_singleton)
    {
      panel_score_singleton = new right_panel_score ();
    }
  return panel_score_singleton;
}

//-------------------------------------------------------------------------------
// first initialization
//-------------------------------------------------------------------------------
void
right_panel_score::first_init (controller_balls* b)
{
  initialize ();
  balls = b;
  scoreAdres = game_screen->get_pixel_data
    (POSX_SCORE * resolution, POSY_SCORE * resolution);
  lifesAdres = game_screen->get_pixel_data
    (POSX_LIFES * resolution, POSY_LIFES * resolution);
  brickAdres = game_screen->get_pixel_data
    (POSX_BRICK * resolution, POSY_BRICK * resolution);
  temoinAdrs = game_screen->get_pixel_data
    (GAUGE_XCOORD * resolution, GAUGE_YCOORD * resolution);
  draw_background ();
}

/**
 * Draw the background of the panel score
 */
void
right_panel_score::draw_background ()
{
  bitmap_data *bmp = new bitmap_data ();

  bmp->load (handler_resources::BITMAP_RIGHT_PANEL);
  bmp->copyTampon (0, 0, 256 * resolution, 0, 64 * resolution,
                          240 * resolution);
  tamponAff1 (POSX_AREAN * resolution, POSY_AREAN * resolution,
              current_player->area_number, 10);
  tamponAff1 (POSX_LEVEL * resolution, POSY_LEVEL * resolution,
              current_player->level_number, 10);
  tamponAff2 (POSX_NOMJO * resolution, POSY_NOMJO * resolution,
              current_player->player_name, 6);
  tamponAff1 (POSX_BESTS * resolution, POSY_BESTS * resolution,
              high_score->get_best_score (), 100000);
  tamponAff2 (POSX_BESTN * resolution, POSY_BESTN * resolution,
              high_score->get_best_playername (), 6);

  draw_gigablizt_gauge ();
  delete bmp;
  bmp = (bitmap_data *) NULL;
}

/**
 * Display score, number of lifes and number of bricks
 */
void
right_panel_score::text_refresh ()
{
  affNombre1 (scoreAdres, current_player->score_value, 100000);
  affNombre1 (lifesAdres, bricks_counter, 100);
  affNombre1 (brickAdres, current_player->number_of_lifes, 10);
}

/**
 * Decrease the number of bricks
 * @param dec number to substract
 */
void
right_panel_score::decrease_bricks_counter (Uint32 dec)
{
  if (dec >= bricks_counter)
    {
      bricks_counter = 0;
    }
  else
    {
      bricks_counter -= dec;
    }
}

/**
 * Return the number of bricks
 * @return the number of bricks
 */
Uint32
right_panel_score::get_bricks_counter ()
{
  return bricks_counter;
}

/** 
 * Intialize the bricks counter
 * @param counter the number of bricks
 */
void
right_panel_score::set_bricks_counter (Uint32 counter)
{
  bricks_counter = counter;
}

/**
 * Draw the gauge of the Gigablitz
 */
void
right_panel_score::draw_gigablizt_gauge ()
{
  controller_gigablitz *gigablitz = controller_gigablitz::get_instance ();
  if (keyboard->is_right_left_buttons () && !gigablitz->isactivate () &&
      !balls->least_glue ())
    {
      if (++delay_gigablitz_countdown >= DELAY_GIGABLITZ_COUNTDOWN)
        {
          if (gigablitz_countdown > 0)
            {
              delay_gigablitz_countdown = 0;
              gigablitz_countdown = gigablitz_countdown - resolution;
              /* gauge maximum height reached? */ 
              if (gigablitz_countdown == 0)
                {
                  /* yes, launch the gigablitz!*/
                  gigablitz->initDepart ();
                }
            }
        }
    }
  else
    {
      if (gigablitz_countdown < gauge_height)
        {
          delay_gigablitz_countdown = 0;
          gigablitz_countdown = gigablitz_countdown + resolution;
        }
    }

  char *d = temoinAdrs;
  Sint32 m = game_screen->get_width ();
  char p = 0;


  if (resolution == 1)
    {
      for (Uint32 i = 0; i < gigablitz_countdown; i++)
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
      for (Uint32 i = 0; i < gigablitz_countdown; i++)
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



  Sint32 h = (gauge_height) - gigablitz_countdown;

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

/**
 * Reset the countdown of the Gigablitz
 */
void
right_panel_score::reset_gigablitz_countdown ()
{
  gigablitz_countdown = gauge_height;
}

/** List of 54 couleurs from gigablitz */
unsigned char right_panel_score::temoinCol1[GAUGE_HEIGHT] =
{ 
  255,
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

unsigned char right_panel_score::temoinCol2[GAUGE_HEIGHT * 2] =
{ 255,
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
