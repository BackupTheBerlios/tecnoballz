/** 
 * @file handler_display.cc 
 * @briefi Handle displaying and updating with SDL 
 * @created 2002-08-17 
 * @date 2007-01-31
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.4 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_display.h,v 1.4 2007/01/31 19:49:07 gurumeditation Exp $
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
#include "../include/mentatCode.h"
#include "../include/offscreen_surface.h"

class handler_display:public virtual mentatCode
{
public:
  static const Sint32 SHADOWOFFX = -4;
  static const Sint32 SHADOWOFFY = 4;
  static const unsigned char SHADOW_PIX = 0x80;
  static const Uint32 SHADOWLONG = 0x80808080;
  static bool optionfull;       // 1 = full screen mode
  static bool optionsync;       // 1 = no synchronisation


private:
  //Uint32                        keyfscreen;     // 1 = full screen mode

  static const Uint32 bitspixels = 8;
  Sint32 offsetplus;

  SDL_Surface *sdl_screen;      // main window
  SDL_Surface *bufSurface;      // surface where all is display
  SDL_Surface *tamSurface;      // surface from restauring

  Uint32 window_width;
  Uint32 window_height;

  Uint32 bufLargeur;
  Uint32 bufHauteur;
  char *bufAdresse;
  Sint32 buf_nextLn;
  Sint32 bufProfond;
  Sint32 tam_nextLn;
  char *tamAdresse;

  Uint32 dateactuel;
  Uint32 datepreced;

  Sint32 framepeers;
  Sint32 fps_totale;
  Sint32 fpscounter;
  //Sint32                                wait_diffv;
  Sint32 wait_count;            // number of frame since "wait_value" is used
  Sint32 wait_value;            // wait value speed_game - x
  Sint32 speed_game;            // 20  (1000 / 20 = 50 fps)
  Sint32 wait_total;
  Sint32 wait_inter;

  Uint32 VBL_switch;
  /* 
     float gameSpeed;
     Sint32 gameFrame;
     Uint32 last_time;
     float fps;
     float targetAdj;
   */



  SDL_Color palette_tz[256];
  SDL_Color ze_palette[256];
  static char nomfenetre[25];
  Sint32 tiltoffset;

public:
    handler_display ();
   ~handler_display ();
  Sint32 initialize ();
  Sint32 set_video_mode ();
  Sint32 get_width ();
  Sint32 get_height ();
  void lock_surfaces ();
  void unlock_surfaces ();
  Sint32 SDL_informations ();
  Sint32 synchro_CalculDifference ();
  Sint32 synchro_processusPause (Sint32 _iTemps);
  void fullscreen ();
  void wait_frame ();
  void waitVBLchr ();
  void waitVBLtec ();
  Sint32 retour_temps ();
  Sint32 get_framepee ();
  void mise_a_zero_timer ();

  // buffer and tampon
  char *buffer_pos (Sint32 pos_x, Sint32 pos_y);
  void buffer_GFX (char *gfxPT, Sint32 large, Sint32 haute);

  char *tampon_adr ();
  char *tampon_pos (Sint32 pos_x, Sint32 pos_y);
  void tampon_RAZ (Sint32 pixel);
  void tampon_RAZ (Sint32 pixel, Sint32 x_pos, Sint32 y_pos, Sint32 width,
                   Sint32 heigh);
  Sint32 tamponNext ();
  void tamponBuff ();
  void tamponBuff (Sint32 pos_x, Sint32 pos_y, Sint32 large, Sint32 haute);
  Sint32 tampon_rel (Sint32 pos_x, Sint32 pos_y);
  Sint32 tampon_mod (Uint32 large);
  void tampon_GFX (SDL_Surface * gfxPT, Sint32 large, Sint32 haute);
  Sint32 ecran_next (Sint32 zbase, Sint32 offsx, Sint32 offsy);



  // gestion des buffers & des tampons
  void aff_buff32 (char *src, char *dest, Sint32 offs, Sint32 offd);
  void buf_affx32 (char *srcPT, char *desPT, Sint32 width, Sint32 heigh = 32);
  void buf_affx64 (char *srcPT, char *desPT, Sint32 width, Sint32 heigh = 64);
  void buf_clr64 (char *desPT);
  void enable_palette (unsigned char *adrPal);
  void enable_palette (SDL_Color * adrPal);
  void paletteAff ();
  void bufferCopy ();           // copie normale du Buffer
  void bufferCTab ();           // copie des 512 pixels gauche du Buffer
  void bufferCTab (Sint32 xpos1, Sint32 ypos1, Sint32 xpos2, Sint32 ypos2);
  void buffer_640 ();           // copie du Buffer optimise pour 640 pixels de large
  SDL_Color *paletteAdr ();     // retourne l'adresse de la palette
  void rectShadow (Sint32 pos_x, Sint32 pos_y, Sint32 large, Sint32 haute);
  void clr_shadow (Sint32 offst, Sint32 large, Sint32 haute);
  void clr_shadow (Sint32 _iPosX, Sint32 _iPosY, Sint32 _iLarg,
                   Sint32 _iHaut);
  void set_shadow (Sint32 offst, Sint32 large, Sint32 haute);
  void genericGFX (char *sAdre, Sint32 sLarg, Sint32 sHaut, Sint32 sNext,
                   char *dAdre, Sint32 dLarg, Sint32 dHaut, Sint32 dNext);
  void tiltscreen ();
  void gradation1 ();
};
#endif
