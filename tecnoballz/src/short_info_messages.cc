/** 
 * @file short_info_messages.cc 
 * @brief Display short info messages in bricks levels 
 * @created 2002-12-30
 * @date 2007-02-26
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: short_info_messages.cc,v 1.2 2007/02/26 17:39:39 gurumeditation Exp $
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
#include "../include/short_info_messages.h"
#include "../include/handler_resources.h"

short_info_messages * short_info_messages::info_messages_singleton = NULL;

/**
 * Create the info messages object
 */
short_info_messages::short_info_messages ()
{
  messages = NULL;
  bitmap_small_fonts = (bitmap_data *) NULL;
  messa_fond = (bitmap_data *) NULL;
  delay_counter = 0;
  object_init ();
}

/**
 * Relesae the info messages object
 */
short_info_messages::~short_info_messages ()
{
  if (NULL != bitmap_small_fonts)
    {
      delete bitmap_small_fonts;
      bitmap_small_fonts = (bitmap_data *) NULL;
    }
  if (NULL != messa_fond)
    {
      delete messa_fond;
      messa_fond = (bitmap_data *) NULL;
    }
  if (NULL != messages)
    {
      delete[](char *)messages ;
      messages = NULL;
    }
  object_free ();
  info_messages_singleton = NULL;
}

/**
 * Get the object instance
 * short_info_messages is a singleton
 * @return the short_info_messages object 
 */
short_info_messages *
short_info_messages::get_instance ()
{
  if (NULL == info_messages_singleton)
    {
      info_messages_singleton = new short_info_messages ();
    }
  return info_messages_singleton;
}



/**
 * Initialize, load fonts bitmap, create offscreen
 */
void
short_info_messages::intialize ()
{
  if (NULL == bitmap_small_fonts)
    {
      bitmap_small_fonts = new bitmap_data ();
      bitmap_small_fonts->load (handler_resources::BITMAP_SMALL_FONTS);

      off_desti1 = game_screen->get_row_size ();        // modulo destination
      off_source = bitmap_small_fonts->get_row_size (); // modulo source
      ptr_buffer = game_screen->get_pixel_data (MESSAGEPOSX * resolution,
                                                MESSAGEPOSY * resolution);
      ptr_tampon =
        background_screen->get_pixel_data (MESSAGEPOSX * resolution,
                                           MESSAGEPOSY * resolution);
      ptr_fontes = bitmap_small_fonts->get_pixel_data ();
      ft_hauteur = 6 * resolution;      // 6 or 12
      ft_largeur = 4 * resolution;      // 4 or 8
      fonteslarg = ft_largeur * MAX_OF_CHARS;    // 16*4=64 or 16*8=128 
    }

  if (NULL == messa_fond)
    {
      messa_fond = new bitmap_data ();
      messa_fond->create_surface (fonteslarg, ft_hauteur);
      pt_mesfond = messa_fond->get_pixel_data ();
    }

  //###################################################################
  // save the background
  //###################################################################
  char *pfond = pt_mesfond;
  char *ptamp = ptr_tampon;
  Sint32 zemod = background_screen->get_row_size ();
  for (Sint32 y = 0; y < ft_hauteur; y++)
    {
      for (Sint32 x = 0; x < fonteslarg; x++)
        pfond[x] = ptamp[x];
      pfond = pfond + fonteslarg;
      ptamp = ptamp + zemod;
    }

  if (NULL == messages)
    {
       messages = resources->load_texts (handler_resources::TEXTS_MESSAGES, MAX_OF_MESSAGES, MAX_OF_CHARS, 0);
       for (Uint32 i = 0; i < MAX_OF_MESSAGES; i++)
        {
          printf("%02d): %s \n", i,messages[i]);
          char *str = messages[i];
          for (Uint32 j = 0; j < MAX_OF_CHARS; j++)
            {
              char c = str[j];
              if (' ' == c)
                { 
                  str[j] = '[';
                  continue;
                }
              if ('!' == c)
                { 
                  str[j] = '\\';
                  continue;
                }
              if ('?' == c)
                { 
                  str[j] = ']';
                  continue;
                }
              if ('.' == c)
                { 
                  str[j] = '^';
                  continue;
                }
            }
          printf("%02d): %s \n", i,messages[i]);
        }
    }


  clear_messages_request ();
}

/**
 * Clear all message request 
 */
void
short_info_messages::clear_messages_request ()
{
  for (Uint32 i = 0; i < MAX_OF_MESSAGES; i++)
    {
      messages_request[i] = false;
    }
}

/** 
 * Request for display a message
 * @param id message identifier from 0 to 34
 */
void
short_info_messages::send_message_request (Uint32 id)
{
  messages_request[id] = true;
}

/**
 * Display short info messages
 */
void
short_info_messages::run ()
{
  if (delay_counter > 0)
    {
      draw ();
      return;
    }
  Sint32 i = MAX_OF_MESSAGES;
  while (--i >= 0)
    {
      if (!messages_request[i])
        {
          continue;
        }
      messages_request[i] = false;
      mess_pnter = 0;
      delay_counter = 66;
      mess_reque = zemessage0[i];
      draw ();
      return;
    }
}

/**
 * Draw or clear a info message
 */
void
short_info_messages::draw ()
{
  if (mess_pnter < 16)
    {
      /* draw a message */
      displaymes ();
    }
  else
    {
      delay_counter--;
      if (delay_counter < ft_hauteur)
        {
          clear_mess ();          //clear the message
        }
    }
}

//------------------------------------------------------------------------------
// display the message character by character 
//------------------------------------------------------------------------------
void
short_info_messages::displaymes ()
{
  const char *ptmes = mess_reque;
  ptmes = ptmes + mess_pnter;
  Sint32 chara = *ptmes - 'A';
  char *ptfnt = ptr_fontes + chara * 16 * resolution;
  Sint32 offSc = off_source;
  Sint32 offDs = off_desti1;
  char *ptbuf = ptr_buffer + (mess_pnter * ft_largeur);
  char *ptamp = ptr_tampon + (mess_pnter * ft_largeur);
  for (Sint32 y = 0; y < ft_hauteur; y++)
    {
      for (Sint32 x = 0; x < ft_largeur; x++)
        {
          char pixel = ptfnt[x];
          if (pixel > 0)
            {
              ptbuf[x] = pixel;
              ptamp[x] = pixel;
            }
        }
      ptfnt = ptfnt + offSc;
      ptbuf = ptbuf + offDs;
      ptamp = ptamp + offDs;
    }
  mess_pnter++;
}

//------------------------------------------------------------------------------
// clear the message line by line
//------------------------------------------------------------------------------
void
short_info_messages::clear_mess ()
{
  Sint32 pos_x = MESSAGEPOSX * resolution;
  Sint32 pos_y = (MESSAGEPOSY * resolution) + delay_counter;
  char *pbuff = game_screen->get_pixel_data (pos_x, pos_y);
  char *ptamp = background_screen->get_pixel_data (pos_x, pos_y);
  char *pfond = pt_mesfond + (delay_counter * fonteslarg);
  for (Sint32 x = 0; x < fonteslarg; x++)
    {
      char pixel = pfond[x];
      pbuff[x] = pixel;
      ptamp[x] = pixel;
    }
}

//------------------------------------------------------------------------------
// Messages  displayed on the screen
// A to Z = A to Z
// [ = space
// \ = exclamation mark 
// ] = point d'interrogation
// ^ = dot
//------------------------------------------------------------------------------
const char *
  short_info_messages::zemessage0[MAX_OF_MESSAGES] = {
  "[[[[[[[[[[[[[[[[",  //0
  "[ARE[YOU[READY[]",           //1
  "[YEAH[[YOU[WERE\\",          //2
  "[[[[[[[[[[[[[[[[",           //3
  "[[[SIMPLE[FIRE[[",           //4
  "[[[COOL[FIRE[[\\[",          //5
  "[[COOL[GUY[[\\[[",           //6
  "[[[[[[[[[[[[[[[[",           //7
  "[[[[[[[[[[[[[[[[",           //8
  "[[[[[[[[[[[[[[[[",           //9
  "[YOU[LOST[A[LIFE",           //10
  "[YOU[WON[A[LIFE\\",          //11
  "[[[[[[[[[[[[[[[[",           //12
  "[[[[[[[[[[[[[[[[",           //13
  "[[[[TOO[LATE^^^[",           //14
  "[BONUS[PRICE[[^[",           //15
  "[[GLUE[[OPTION[[",           //16
  "[[NEXT[LEVEL^^^[",           //17
  "[[[MALUS[SIZE[[[",           //18
  "[[[BONUS[SIZE[[[",           //19
  "[[BONUS[BALLS[[[",           //20
  "[[[MULTIBALLS[[[",           //21
  "[[[POWERBALLS[[[",           //22
  "[MEGAPOWERBALLS[",           //23
  "[INVERSOR[MALUS[",           //24
  "[SPEEDBALLS[MAXI",           //25
  "[[BUMPER[RIGHT[[",           //26
  "[[[BUMPER[[UP[[[",           //27
  "[[BUMPER[[LEFT[[",           //28
  "[BIGSIZE[[BALLS[",           //29
  "[MAXISIZE[BALLS[",           //30
  "[[MEGA[OPTIONS[[",           //31
  "[[WALL[[ENABLE[[",           //32
  "[[ROBOT[ENABLE[[",           //33
  "[CONTROL[[BALLS[",           //34
};
