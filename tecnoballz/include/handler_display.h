/** 
 * @file handler_display.cc 
 * @briefi Handle displaying and updating with SDL 
 * @created 2002-08-17 
 * @date 2007-01-31
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.8 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_display.h,v 1.8 2007/02/15 17:12:24 gurumeditation Exp $
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
#ifndef __HANDLER_DISPLAY__
#define __HANDLER_DISPLAY__
#include "../include/tecnoballz.h"
#include "../include/offscreen_surface.h"

class handler_display:public virtual tecnoballz
{
public:
  static const Sint32 SHADOWOFFX = -4;
  static const Sint32 SHADOWOFFY = 4;
  static const unsigned char SHADOW_PIX = 0x80;
  static const Uint32 SHADOWLONG = 0x80808080;
  static bool optionfull;       // 1 = full screen mode
  static bool optionsync;       // 1 = no synchronisation


private:
  static const Uint32 bitspixels = 8;
  static const Uint32 DELAY_CHANGE_MAX = 10;
  Sint32 offsetplus;


  SDL_Surface *bufSurface;
  SDL_Surface *tamSurface;

  Uint32 bufLargeur;
  Uint32 bufHauteur;
  char *bufAdresse;
  Sint32 buf_nextLn;
  Sint32 bufProfond;
  Sint32 tam_nextLn;
  char *tamAdresse;

  /** Main window surface */
  SDL_Surface *sdl_screen;
  /** The width of our main window in pixels */
  Uint32 window_width;
  /** The height of our main window in pixels */
  Uint32 window_height;
  /** The number of milliseconds since the SDL library initialization */
  Uint32 previous_sdl_ticks;
  /** Current frames per seconde or frames frequency */
  Uint32 frames_per_second;
  /** Amount of ticks for calculate the number of frames per second */
  Uint32 sdl_ticks_amount;
  /** Counter of frame modulo 100 */
  Uint32 frames_counter_modulo;
  /** Number of frame during the "delay_value" is used */
  Sint32 delay_change_counter;
  /** Delay value for SDL_delay() */
  Sint32 delay_value;
  /** game speed, the desired frames per second 20 <=> 50 fps */
  Uint32 game_speed;
  /** Amount of ticks for calculate "delay_value" */
  Uint32 delay_ticks_amount;
  /** The window tile and icon name */
  static char window_title[25];

  SDL_Color ze_palette[256];
  /** */
  Uint32 tilt_offset;

public:
    handler_display ();
   ~handler_display ();
  Sint32 initialize ();
  Uint32 get_width ();
  Uint32 get_height ();
  void lock_surfaces ();
  void unlock_surfaces ();
  Uint32 get_bits_per_pixel ();
  Sint32 SDL_informations ();
  void wait_frame ();
  Uint32 get_frames_per_second ();

  Sint32 ecran_next (Sint32 zbase, Sint32 offsx, Sint32 offsy);

  // gestion des buffers & des tampons
  void aff_buff32 (char *src, char *dest, Sint32 offs, Sint32 offd);
  void buf_affx32 (char *srcPT, char *desPT, Sint32 width, Sint32 heigh = 32);
  void buf_affx64 (char *srcPT, char *desPT, Sint32 width, Sint32 heigh = 64);
  void buf_clr64 (char *desPT);
  void enable_palette (unsigned char *adrPal);
  void enable_palette (SDL_Color * adrPal);
  void bufferCopy ();           // copie normale du Buffer
  void bufferCTab ();           // copie des 512 pixels gauche du Buffer
  SDL_Color *get_palette ();
  void rectShadow (Sint32 pos_x, Sint32 pos_y, Sint32 large, Sint32 haute);
  void clr_shadow (Sint32 offst, Sint32 large, Sint32 haute);
  void clr_shadow (Sint32 _iPosX, Sint32 _iPosY, Sint32 _iLarg,
                   Sint32 _iHaut);
  void set_shadow (Sint32 offst, Sint32 large, Sint32 haute);
  void genericGFX (char *sAdre, Sint32 sLarg, Sint32 sHaut, Sint32 sNext,
                   char *dAdre, Sint32 dLarg, Sint32 dHaut, Sint32 dNext);
  void tilt_screen ();
  void gradation1 ();
private:
  Sint32 set_video_mode ();
  void check_if_toggle_fullscreen ();

};
#endif
