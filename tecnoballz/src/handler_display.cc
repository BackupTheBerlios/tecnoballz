/** 
 * @file handler_display.cc 
 * @briefi Handle displaying and updating with SDL 
 * @created 2002-08-17 
 * @date 2007-02-17
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.18 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_display.cc,v 1.18 2007/02/18 12:47:23 gurumeditation Exp $
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
#include "../include/handler_display.h"
#include "../include/handler_keyboard.h"
#include "../include/handler_audio.h"
#include "../include/handler_resources.h"

char handler_display::window_title[25] = "TecnoballZ by TLK Games\0";
bool handler_display::optionfull = false;
bool handler_display::optionsync = true;

/**
 * Create the object
 */
handler_display::handler_display ()
{
  sdl_screen = (SDL_Surface *) NULL;
  bufSurface = (SDL_Surface *) NULL;
  tamSurface = (SDL_Surface *) NULL;
  sdl_ticks_amount = 0;
  frames_counter_modulo = 0;
  tilt_offset = 0;
  game_speed = 20;              //1000 / 50
  //game_speed = 17;      //1000 / 60
  //game_speed = 12;      //1000 / 85
  delay_change_counter = 0;
  delay_ticks_amount = 0;
  delay_change_counter = DELAY_CHANGE_MAX;
}

/**
 * Release the display handler object
 */
handler_display::~handler_display ()
{
  if (NULL != game_screen)
    {
      delete game_screen;
      game_screen = NULL;
    }
  if (NULL != background_screen)
    {
      delete background_screen;
      background_screen = NULL;
    }
 SDL_Quit ();
}

/**
 * Initialize the SDL display and allocate surfaces
 */
Sint32
handler_display::initialize ()
{

  if (set_video_mode () != E_NO_ERROR)
    {
      return E_SDLERROR;
    }

  /* allocate "buffer" surface */
  bufLargeur = window_width;
  bufHauteur = window_height + offsetplus * 2;

  game_screen = new offscreen_surface (bufLargeur, bufHauteur, bitspixels, offsetplus);

  bufSurface = game_screen->get_surface ();

  buf_nextLn = bufSurface->pitch;
  bufAdresse = (char *) bufSurface->pixels + buf_nextLn * offsetplus;
  bufProfond = bufSurface->format->BytesPerPixel;

  background_screen = new offscreen_surface (bufLargeur, bufHauteur, bitspixels, offsetplus);
  tamSurface = background_screen->get_surface ();


  /* allocate "tampon" surface */
  tam_nextLn = tamSurface->pitch;
  tamAdresse = (char *) tamSurface->pixels + tam_nextLn * offsetplus;

  previous_sdl_ticks = SDL_GetTicks ();
  delay_value = 0;
  return 0;
}

/**
 * Initializes the video with SDL
 */
Sint32
handler_display::set_video_mode ()
{
  window_width = 320 * resolution;
  window_height = 240 * resolution;
  offsetplus = 64 * resolution;
  if (is_verbose)
    {
      std::cout << ">handler_display::set_video_mode() " << window_width
        << "x" << window_height << std::endl;
  }

  /* initializes SDL */
  if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE | SDL_INIT_AUDIO) < 0)
    {
      fprintf (stderr, "handler_display::initialize() : sdl_init: %s",
               SDL_GetError ());
      return (erreur_num = E_SDLERROR);
    }

  /* test if the video mode is available */
  Uint32 flag = SDL_ANYFORMAT;
  if (optionfull)
    {
      flag = flag | SDL_FULLSCREEN;
    }
  Uint32 bpp = SDL_VideoModeOK (window_width, window_height, bitspixels, flag);
  if (bpp == 0)
    {
      fprintf (stderr, "(!)handler_display::set_video_mode() "
               "SDL_VideoModeOK() Mode not available: %s", SDL_GetError ());
      return (erreur_num = E_SDLERROR);
    }

  /* initialize the video mode */
  sdl_screen = SDL_SetVideoMode (window_width, window_height, bitspixels, flag);
  if (sdl_screen == NULL)
    {
      fprintf (stderr, "(!)handler_display::set_video_mode()" 
               "SDL_SetVideoMode() return %s\n", SDL_GetError ());
      return (erreur_num = E_SDLERROR);
    }
  SDL_WM_SetCaption (window_title, window_title);

#ifdef UNDER_DEVELOPMENT
  SDL_ShowCursor (SDL_ENABLE);
#else
  SDL_ShowCursor (SDL_DISABLE);
#endif
  SDL_EnableUNICODE (1);
  return E_NO_ERROR;
}

/**
 * Return the screen's width 
 * @return the width of the screen in pixels
 */
Uint32
handler_display::get_width ()
{
  return (Sint32) (sdl_screen->w);
}

/**
 * Return the screen's height
 * @return the height of the screen in lines
 */
Uint32
handler_display::get_height ()
{
  return (Sint32) (sdl_screen->h);
}

/**
 * Lock surfaces of the game offscreen and the background offscreen
 */
void
handler_display::lock_surfaces ()
{
  game_screen->lock_surface ();
  background_screen->lock_surface ();
}

/**
 * Unlock surfaces of the game offscreen and background offscreen
 */
void
handler_display::unlock_surfaces ()
{
  game_screen->unlock_surface ();
  background_screen->unlock_surface ();
}
/**
 * Return the number of bits per pixel
 * @return then number of bits per pixel, 8 for 256 colors
 */
Uint32
handler_display::get_bits_per_pixel ()
{
  return bitspixels;
}

// -----------------------------------------------------------------------------
// affiche des informations sur SDL
// -----------------------------------------------------------------------------
Sint32
handler_display::SDL_informations ()
{
  const SDL_VideoInfo *vi;
  char namebuf[32] = { "123456789012345678901234567890\0" };
  SDL_Rect **modes;
  Sint32 i;
  modes = SDL_ListModes (NULL, SDL_FULLSCREEN | SDL_HWSURFACE);
  if (modes == (SDL_Rect **) 0)
    {
      printf ("No modes available!\n");
      return 1;
    }
  if (modes == (SDL_Rect **) - 1)
    printf ("All resolutions available.\n");
  else
    {
      printf ("Available Modes\n");
      for (i = 0; modes[i]; ++i)
        printf (" %d x %d\n", modes[i]->w, modes[i]->h);
    }
  SDL_WM_SetCaption ("TecnoballZ", 0);
  vi = SDL_GetVideoInfo ();
  printf
    ("The number of bits used to represent each pixel in a surface.Usually 8, 16, 24 or 32. BitsPerPixel=%d \n",
     vi->vfmt->BitsPerPixel);
  printf
    ("The number of bytes used to represent each pixel in a surface. Usually one to four BytesPerPixel=%d\n",
     vi->vfmt->BytesPerPixel);
  printf ("Rmask=%i Gmask=%i Bmask=%i Amask=%i \n", vi->vfmt->Rmask,
          vi->vfmt->Gmask, vi->vfmt->Bmask, vi->vfmt->Amask);
  printf ("Rshift=%i Gshift=%i Bshift=%i Ashift=%i \n", vi->vfmt->Rshift,
          vi->vfmt->Gshift, vi->vfmt->Bshift, vi->vfmt->Ashift);
  printf ("Rloss=%i Gloss=%i Bloss=%i Aloss=%i \n", vi->vfmt->Rloss,
          vi->vfmt->Gloss, vi->vfmt->Bloss, vi->vfmt->Aloss);
  printf ("Pixel value of transparent pixels. colorkey=%i \n",
          vi->vfmt->colorkey);
  printf ("Overall surface alpha value. alpha=%i \n", vi->vfmt->alpha);
  printf ("Is it possible to create hardware surfaces? hw_available=%i \n",
          vi->hw_available);
  printf ("Is there a window manager available wm_available=%i \n",
          vi->wm_available);
  printf ("Are hardware to hardware blits accelerated? blit_hw=%i \n",
          vi->blit_hw);
  printf
    ("Are hardware to hardware colorkey blits accelerated? blit_hw_CC=%i \n",
     vi->blit_hw_CC);
  printf ("Are hardware to hardware alpha blits accelerated? blit_hw_A=%i\n",
          vi->blit_hw_A);
  printf ("Are software to hardware blits accelerated? blit_sw=%i \n",
          vi->blit_sw);
  printf
    ("Are software to hardware colorkey blits accelerated? blit_sw_CC=%i \n",
     vi->blit_sw_CC);
  printf ("Are software to hardware alpha blits accelerated? blit_sw_A=%i \n",
          vi->blit_sw_A);
  printf ("Are color fills accelerated? blit_fill=%i \n", vi->blit_fill);
  printf ("Total amount of video memory in Kilobytes. video_mem=%i\n",
          vi->video_mem);
  char *c = SDL_VideoDriverName (namebuf, 32);
  printf ("the name of the video driver=%s\n", c);
  return 0;
}


/**
 * Switch to fullscreen or windows mode
 */
void
handler_display::check_if_toggle_fullscreen ()
{
  if (keyboard->command_is_pressed (handler_keyboard::FULLSCFLAG) &&
      keyboard->get_input_cursor_pos () < 0)
    {
      if (optionfull)
        {
          optionfull = false;
        }
      else
        {
          optionfull = true;
        }
      display->set_video_mode ();
      display->enable_palette (ze_palette);
    }
}

/**
 * Synchronize the game's speed to the machine it's running on
 */
void
handler_display::wait_frame ()
{

  frame_counter++;

  /* Get the number of milliseconds since the SDL library initialization */
  Uint32 sdl_ticks = SDL_GetTicks () - previous_sdl_ticks;
  optionsync = true;
  if (optionsync)
    {
      delay_ticks_amount += sdl_ticks;
      if (--delay_change_counter <= 0)
        {
          //printf("((%i * %i) - %i) / %i)\n", game_speed, DELAY_CHANGE_MAX, delay_ticks_amount, DELAY_CHANGE_MAX);
          delay_value = ((game_speed * DELAY_CHANGE_MAX) - delay_ticks_amount) / DELAY_CHANGE_MAX;
          delay_change_counter = DELAY_CHANGE_MAX;
          delay_ticks_amount = 0;
          if (delay_value <= 0)
            {
              delay_value = 1;
            }
          //printf("delay_value:%i \n", delay_value);
        }
      if (delay_value > 0)
        {
          SDL_Delay (delay_value);
        }
    }
  previous_sdl_ticks = SDL_GetTicks ();
  
  /** Calculate the number of frames per second */
  sdl_ticks_amount += sdl_ticks;
  if (++frames_counter_modulo >= 100)
    {
      if (0 != sdl_ticks_amount)
        {
          frames_per_second = 1000 * frames_counter_modulo / sdl_ticks_amount;
        }
      frames_counter_modulo = 0;
      sdl_ticks_amount = 0;
    }

  keyboard->read_events ();
  check_if_toggle_fullscreen ();
#ifndef SOUNDISOFF
  audio->run ();
#endif
  return;
}

/**
 * Return tne number of frames per second 
 * @return the frame frequency
 */
Uint32
handler_display::get_frames_per_second ()
{
  return frames_per_second;
}




//------------------------------------------------------------------------------
// buffer & tampon: convert (x,y) to offset
//      input   => zbase: start offset
//                      => offsx: x coordinate
//                      => offsy: y coordinate
//------------------------------------------------------------------------------
Sint32
handler_display::ecran_next (Sint32 zbase, Sint32 offsx, Sint32 offsy)
{
  return (zbase + offsy * buf_nextLn + offsx);
}

/**
 * Initialize color palette for the current screen
 * @param pal pointer to color components 
 */
void
handler_display::enable_palette (unsigned char *pal)
{
  printf(">>>>>>>>>> handler_display::enable_palette \n");
  unsigned char *p = pal;
  SDL_Color *color = &ze_palette[0];
  Uint32 k = 0;
  for (Uint32 i = 0; i < 256; i++)
    {
      color->r = p[k++];
      color->g = p[k++];
      color->b = p[k++];
      color++;
    }

  game_screen->set_palette (ze_palette);
  background_screen->set_palette (ze_palette);
  SDL_SetPalette (sdl_screen, SDL_LOGPAL | SDL_PHYSPAL, ze_palette, 0, 256);
}

/**
 * Initialize color palette for the current screen
 * @param pal pointer to color components of a SDL_Color structure
 */
void
handler_display::enable_palette (SDL_Color * pal)
{
  printf("===============> handler_display::enable_palette *SDL* \n");
  game_screen->set_palette (pal);
  background_screen->set_palette (pal);
  SDL_SetPalette (sdl_screen, SDL_LOGPAL | SDL_PHYSPAL, pal, 0, 256);
  for (Uint32 i = 0; i < 256; i++)
    {
      printf("%03d:%x%x%x; ", i, pal[i].r, pal[i].g, pal[i].b); 
      if(i % 11 == 0)
	printf("\n");
    }
   printf("\n");
}

/**
 * Return the current SDL palette
 * @return a pointer to a SDL_Color structure 
 */
SDL_Color *
handler_display::get_palette ()
{
  return ze_palette;
}

//------------------------------------------------------------------------------
// buffer memory: copy buffer to screen
//------------------------------------------------------------------------------
void
handler_display::bufferCopy ()
{
  SDL_Rect r;
  r.x = 0;
  r.y = 0;
  r.w = 640;
  r.h = 480;
  Sint32 v = SDL_BlitSurface (bufSurface, &r, sdl_screen, &r);
  if (v < 0)
    fprintf (stderr, "handler_display::bufferCopy() BlitSurface error: %s\n",
             SDL_GetError ());
}

//-----------------------------------------------------------------------------
// buffer memory: copy buffer to screen
//------------------------------------------------------------------------------
void
handler_display::bufferCTab ()
{
  SDL_Rect rsour;
  rsour.x = 0;
  rsour.y = offsetplus + tilt_offset;
  rsour.w = 640;
  rsour.h = offsetplus + 480 + tilt_offset;

  SDL_Rect rdest;
  rdest.x = 0;
  rdest.y = 0;
  rdest.w = 640;
  rdest.h = 480;

  Sint32 v = SDL_BlitSurface (bufSurface, &rsour, sdl_screen, &rdest);
  if (v < 0)
    fprintf (stderr,
             "handler_display::bufferCTab() : BlitSurface error: %s\n",
             SDL_GetError ());
  SDL_UpdateRect (sdl_screen, 0, 0, sdl_screen->w, sdl_screen->h);
  if (tilt_offset > 0)
    tilt_offset--;
}

//-------------------------------------------------------------------------------
// tampon memory: display shadow box
//-------------------------------------------------------------------------------
void
handler_display::rectShadow (Sint32 pos_x, Sint32 pos_y, Sint32 width,
                             Sint32 heigh)
{
  char k = (char) (0x80);
  Sint32 l = width;
  Sint32 m = pos_y + heigh;
  for (Sint32 j = pos_y; j < m; j++)
    {
      //char *monPT = tampon_pos (pos_x, j);
      char *monPT = background_screen->get_pixel_data (pos_x, j);
      for (Sint32 i = 0; i < l; i++)
        *(monPT)++ |= k;
    }
}

//-------------------------------------------------------------------------------
// buffer & tampon: clear shadow box
//      input   =>      offst: offset adresse ecran
//                              large: width box in pixels
//                              haute: height box en pixels
//-------------------------------------------------------------------------------
void
handler_display::clr_shadow (Sint32 offst, Sint32 large, Sint32 haute)
{
  char zmask = 0x7F;
  char *bffer = bufAdresse + offst;
  char *tmpon = tamAdresse + offst;
  Sint32 a = large;
  Sint32 b = haute;
  Sint32 n = buf_nextLn - a;
  for (Sint32 j = 0; j < b; j++, bffer += n, tmpon += n)
    {
      for (Sint32 i = 0; i < a; i++)
        {
          char pixel = *bffer;
          pixel &= zmask;
          *(bffer++) = pixel;
          pixel = *tmpon;
          pixel &= zmask;
          *(tmpon++) = pixel;
        }
    }
}

/**
 * Clear a shadow region
 * @param xcoord
 * @param ycoord
 * @param width
 * @param height
 */
void
handler_display::clr_shadow (Sint32 xcoord, Sint32 ycoord, Sint32 width,
                             Sint32 height)
{
  char zmask = 0x7F;
  char *screen = background_screen->get_pixel_data (xcoord, ycoord);
  char *bkgd = game_screen->get_pixel_data (xcoord, ycoord);

  Sint32 h = width;
  Sint32 v = height;
  Sint32 n = buf_nextLn - h;
  for (Sint32 j = 0; j < v; j++, screen += n, bkgd += n)
    {
      for (Sint32 i = 0; i < h; i++)
        {
          char pixel = *screen;
          pixel &= zmask;
          *(screen++) = pixel;
          pixel = *bkgd;
          pixel &= zmask;
          *(bkgd++) = pixel;
        }
    }
}

//------------------------------------------------------------------------------
// buffer & tampon: display shadow box
//      input   => offst: buffer offset in bytes
//                      => large: width box in pixels
//                      => haute: height box en pixels
//------------------------------------------------------------------------------
void
handler_display::set_shadow (Sint32 offst, Sint32 large, Sint32 haute)
{
  char zmask = (char) (0x80);
  char *bffer = bufAdresse + offst;
  char *tmpon = tamAdresse + offst;
  Sint32 a = large;
  Sint32 b = haute;
  Sint32 n = buf_nextLn - a;
  for (Sint32 j = 0; j < b; j++, bffer += n, tmpon += n)
    {
      for (Sint32 i = 0; i < a; i++)
        {
          char pixel = *bffer;
          pixel |= zmask;
          *(bffer++) = pixel;
          pixel = *tmpon;
          pixel |= zmask;
          *(tmpon++) = pixel;
        }
    }
}

//------------------------------------------------------------------------------
// buffer: copy an image of 32 width pixels into buffer memory 
//------------------------------------------------------------------------------
void
handler_display::buf_affx32 (char *srcPT, char *desPT, Sint32 width,
                             Sint32 heigh)
{
  Sint32 *d = (Sint32 *) desPT;
  Sint32 *s = (Sint32 *) srcPT;
  Sint32 j = heigh;
  Sint32 ms = width;
  Sint32 md = buf_nextLn;
  for (Sint32 i = 0; i < j; i++)
    {
      d[0] = s[0];
      d[1] = s[1];
      d[2] = s[2];
      d[3] = s[3];
      d[4] = s[4];
      d[5] = s[5];
      d[6] = s[6];
      d[7] = s[7];
      s = (Sint32 *) ((char *) s + ms);
      d = (Sint32 *) ((char *) d + md);
    }
}

//------------------------------------------------------------------------------
// buffer: copy an image of 64 width pixels into buffer memory
//------------------------------------------------------------------------------
void
handler_display::buf_affx64 (char *srcPT, char *desPT, Sint32 width,
                             Sint32 heigh)
{
  double *d = (double *) desPT;
  double *s = (double *) srcPT;
  Sint32 j = heigh;
  Sint32 ms = width;
  Sint32 md = buf_nextLn;
  for (Sint32 i = 0; i < j; i++)
    {
      d[0] = s[0];
      d[1] = s[1];
      d[2] = s[2];
      d[3] = s[3];
      d[4] = s[4];
      d[5] = s[5];
      d[6] = s[6];
      d[7] = s[7];
      s = (double *) ((char *) s + ms);
      d = (double *) ((char *) d + md);
    }
}

//------------------------------------------------------------------------------
// copy an image
//------------------------------------------------------------------------------
void
handler_display::genericGFX (char *sAdre, Sint32 sLarg, Sint32 sHaut,
                             Sint32 sNext, char *dAdre, Sint32 dLarg,
                             Sint32 dHaut, Sint32 dNext)
{
  Sint32 h, l;
  Sint32 a = sNext;
  Sint32 b = dNext;
  if (sLarg > dLarg)
    l = sLarg;
  else
    l = dLarg;
  if (sHaut > dHaut)
    h = dHaut;
  else
    h = sHaut;
  char *d = dAdre;
  char *s = sAdre;
  for (Sint32 i = 0; i < h; i++, s += a, d += b)
    {
      for (Sint32 j = 0; j < l; j++)
        d[j] = s[j];
    }
}

/**
 * Tilt the screen 10 or 20 pixels upwards
 */
void
handler_display::tilt_screen ()
{
  tilt_offset = 10 * resolution;
}

//------------------------------------------------------------------------------
// select colour gradation
//------------------------------------------------------------------------------
void
handler_display::gradation1 ()
{
  SDL_Color *palPT = display->get_palette ();
  SDL_Color *palP1 = palPT + 239;
  Sint32 i = random_counter & 0x0F;
  if (i >= 10)
    i = i - 10;
  const Uint32 *ptpal = (handler_resources::tabledegas + i * 18);
  for (i = 0; i < 17; i++)
    {
      Uint32 vacol = ptpal[i];
      Uint32 vablu = vacol & 0x000000ff;
      Uint32 vagre = vacol & 0x0000ff00;
      vagre = vagre >> 8;
      Uint32 vared = vacol & 0x00ff0000;
      vared = vared >> 16;
      palP1->r = vared;
      palP1->g = vagre;
      palP1->b = vablu;
      palP1++;
    }
  display->enable_palette (palPT);
}
