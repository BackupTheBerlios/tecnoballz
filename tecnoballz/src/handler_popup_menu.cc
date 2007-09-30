/** 
 * @file handler_popup_menu.cc 
 * @brief popup menu handler (When the [Esc] is pressed)
 * @created 2004-08-08 
 * @date 2007-09-27
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.8 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_popup_menu.cc,v 1.8 2007/09/30 07:23:39 gurumeditation Exp $
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
#include "../include/handler_resources.h"
#include "../include/handler_popup_menu.h"
#include "../include/sprite_object.h"
#include "../include/handler_keyboard.h"

/**
 * Create the popup menu handler
 */
handler_popup_menu::handler_popup_menu ()
{
  yPressLeft = YCOORDNULL;
  yPressRigh = YCOORDNULL;
  menu_color = 0;
  menu_number = 0;
  texts_of_menus = NULL;
}

/**
 * Release the popup menu handler
 */
handler_popup_menu::~handler_popup_menu ()
{
  if (screen_menu != NULL)
    {
      delete screen_menu;
      screen_menu = (bitmap_data *) NULL;
    }
  if (bitmap_fonts != NULL)
    {
      delete bitmap_fonts;
      bitmap_fonts = (bitmap_data *) NULL;
    }
  if (texts_of_menus != NULL)
    {
      delete[](char *) texts_of_menus;
      texts_of_menus = NULL;
    }
}

/**
 * Initialize the popup menu
 * @param bmp Bitmap where are graphic elements of the menu
 */
void
handler_popup_menu::load_text_file ()
{
  if (texts_of_menus != NULL)
    {
      return;
    }
  texts_of_menus =
    resources->load_texts (handler_resources::TEXTS_POPUP_MENU,
			   MAX_OF_LINES, MAX_OF_CHARS, 0);

  //menu_texts[0] = texts_of_menus[MENU_00 * MAX_OF_CHARS];
  //menu_texts[1] = texts_of_menus[MENU_01 * MAX_OF_CHARS];
}

/**
 * Initialize the popup menu
 * @param bmp Bitmap where are graphic elements of the menu
 */
void
handler_popup_menu::first_init (bitmap_data * bmp)
{
  load_text_file();
  Uint32 w;
  bool is_palette;
  switch (super_jump)
    {
    case BRICKS_LEVEL:
      /* determine if restore background (bricks levels and shop only) */
      if (has_background)
	{
          is_restore_background = true;
	}
      else
	{
          is_restore_background = false;
	}
      menu_number = 0;
      w = 256 * resolution;
      is_palette = false;
      break;

    case SHOP:
      is_restore_background = true;
      menu_number = 1;
      w = 320 * resolution;
      is_palette = true;
      break;

    case GUARDS_LEVEL:
    default: 
      is_restore_background = false;
      menu_number = 0;
      w = 320 * resolution;
      is_palette = true;
      break;
    }


  /* initialize palette color chars, if necessary (shop only) */
  if (is_palette)
    {
      SDL_Color *palPT = display->get_palette ();
      SDL_Color *palP1 = palPT + 239;
      Sint32 i = random_counter & 0x0F;
      if (i >= 10)
        {
          i = i - 10;
	}
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

  /* determine line height into menu box */
  if (resolution == 2)
    {
      vertical_space = 17;
    }
  else
    {
       vertical_space = 8;
    }

  /* load fontes bitmap files and perform some initializations */
  load_bitmap_fonts (handler_resources::BITMAP_MENU_FONTS);

  /* determine height of the menu box */
  if (menu_number == 1)
    {
      num_of_lines = 3;
    }
  else
    {
      num_of_lines = 4;
    }
  num_of_columns = MAX_OF_CHARS;
  Uint32 numof_lines = num_of_lines + 2;

  /* allocate graphic buffer of menu box */ 
  screen_menu = new bitmap_data ();
  screen_menu->create_surface ((num_of_columns + 2) * char_height, numof_lines * vertical_space);
  screen_menu->clear ();

  /* save coordinates of the sprites */
  char *ptAdd[8];
  const bb_describ *pDesc = sprite_object::zelistBOB[BOB_ESCMEN];
  Sint32 heigh = pDesc->BB_HAUTEUR;     //high in pixels
  heigh *= resolution;
  Sint32 nanim = pDesc->BB_ANIMATE;     //number of animations
  Sint32 width = pDesc->BB_LARGEUR;     //width in pixels
  width *= resolution;
  bbPosition *coord = pDesc->BBPOSITION;
  for (Sint32 i = 0; i < nanim; i++)
    {
      Sint32 pos_x = (Sint32) coord[i].BB_COORDX;
      pos_x *= resolution;
      pos_x *= 16;
      Sint32 pos_y = (Sint32) coord[i].BB_COORDY;
      pos_y *= resolution;
      ptAdd[i] = bmp->get_pixel_data (pos_x, pos_y);
    }
  Sint32 NxLine = bmp->get_row_size ();

  /* initialize sprite object */
  make_sprite (screen_menu);
  Sint32 y = (240 * resolution - numof_lines * vertical_space) / 2;
  y = (y / vertical_space) * vertical_space;
  set_coordinates ((w - sprite_width) / 2, y);

  //###################################################################
  // build the frame of menu box (with sprites)
  //###################################################################
  Sint32 m = screen_menu->get_width () / width - 2;
  Sint32 x = width;
  y = screen_menu->get_height () - heigh;
  displayBOB (ptAdd[0], 0, 0, NxLine, width, heigh);
  displayBOB (ptAdd[3], 0, y, NxLine, width, heigh);
  for (Sint32 i = 0; i < m; i++, x += width)
    {
      displayBOB (ptAdd[4], x, 0, NxLine, width, heigh);
      displayBOB (ptAdd[6], x, y, NxLine, width, heigh);
    }
  displayBOB (ptAdd[1], x, 0, NxLine, width, heigh);
  displayBOB (ptAdd[2], x, y, NxLine, width, heigh);

  m = screen_menu->get_height () / heigh - 2;
  if (screen_menu->get_height () % heigh)
    m++;

  x = screen_menu->get_width () - width;
  y = heigh;
  for (Sint32 i = 0; i < m; i++, y += heigh)
    {
      displayBOB (ptAdd[5], 0, y, NxLine, width, heigh);
      displayBOB (ptAdd[7], x, y, NxLine, width, heigh);
    }
}

/**
 * Display a sprite into the "buffer" (copy byte to byte)
 */
void
handler_popup_menu::displayBOB (char *ptSrc, Sint32 pos_x, Sint32 pos_y,
                                Sint32 NxLine, Sint32 width, Sint32 heigh)
{
  char *s = ptSrc;
  char *d = screen_menu->get_pixel_data (pos_x, pos_y);
  Sint32 m = NxLine;
  Sint32 n = screen_menu->get_row_size ();
  Sint32 h = width;
  Sint32 l = heigh;
  for (Sint32 i = 0; i < h; i++)
    {
      for (Sint32 j = 0; j < l; j++)
        {
          char p = s[j];        //read the pixel
          if (p)                //color 0 ?
            d[j] = p;           //no, put the pixel
        }
      s += m;
      d += n;
    }
}

/**
 * Display and handle menu
 */
Sint32
handler_popup_menu::execution1 ()
{
  Sint32 event = -1;

  /* [ESC] key: enable / disable menu box */
  if (keyboard->command_is_pressed (handler_keyboard::ESCAPEMENU))
    {
      is_enabled = true;
    }
  else
    {
      if (is_enabled && is_restore_background)
        {
          MSK_bitclr ();
        }
      is_enabled = false;
      return event;
    }

  /* check if right or left button are pressed */
  Sint32 mposx, pos_y, freeL, freeR;
  Sint32 presL = keyboard->is_left_button ();
  Sint32 presR = keyboard->is_right_button ();

  /* read y where is pressed */
  if (presL && yPressLeft == YCOORDNULL)
    {
      yPressLeft = keyboard->get_mouse_y ();
    }
  else
    {
      if (presR && yPressRigh == YCOORDNULL)
        yPressRigh = keyboard->get_mouse_y ();
    }

  freeR = 0;
  freeL = keyboard->is_left_button_up (&mposx, &pos_y);
  if (!freeL)
    freeR = keyboard->is_right_button_up (&mposx, &pos_y);

  if ((freeL && pos_y == yPressLeft) || (freeR && pos_y == yPressRigh))
    {
      Sint32 incre = 0;
      if (freeL)
        {
          incre = 1;
          yPressLeft = YCOORDNULL;
        }
      if (freeR)
        {
          incre = -1;
          yPressRigh = YCOORDNULL;
        }
      event = (pos_y - y_coord) / vertical_space;
      /*printf("handler_popup_menu::execution1() pos_y:%i / y_coord:%i / vertical_space:%i / event:%i\n",
         pos_y, y_coord, vertical_space, event); */
    }
  if (!presL)
    yPressLeft = YCOORDNULL;
  if (!presR)
    yPressRigh = YCOORDNULL;


  /* read color table offset (color line hover by mouse ) */
  if (menu_color++ > 32)
    {
      menu_color = 0;
    }
  if (resolution == 1)
    {
      display320 ();
    }
  else
    {
      display640 ();
    }

  /* copy menu box into screen */
  MSKbitcopy ();

  if (event >= WECONTINUE && event <= WEQUITGAME)
    {
      if (is_enabled && is_restore_background)
        {
          MSK_bitclr ();
        }
      is_enabled = false;
      keyboard->clear_command_keys ();
    }

  if (menu_number == 1 && event >= GOGAMEOVER)
    {
      event++;
    }
  if (event > 0)
    {
      SDL_ShowCursor (SDL_DISABLE);
    }
  return event;
}

//------------------------------------------------------------------------------
// display text in 640x480 mode
//------------------------------------------------------------------------------
void
handler_popup_menu::display640 ()
{
  Sint32 color = menu_color;
  char *desP1 = pixel_data + char_height + row_size * vertical_space;
  Sint32 offSc = off_source;
  Sint32 offDs = row_size;
  Sint32 offD2 = row_size * (vertical_space - 1) + (char_height * 2);
  Sint32 *basPT = (Sint32 *) caract_adr;
  const char *p = menu_texts[menu_number];
  char *c = ascii2code;


  Uint32 a, b, j;
  Uint32 y = (keyboard->get_mouse_y () - y_coord) / vertical_space;
  y--;
  for (Uint32 k = 0; k < num_of_lines; k++, desP1 += offD2)
    {
      if (y != k)
        {
          //###########################################################
          // display normal line of n characters
          //###########################################################
          for (j = 0; j < num_of_columns; j++)
            {
              a = *(p++) - 32;
              if (a)
                {
                  b = c[a];
                  Sint32 *s = (Sint32 *) basPT;
                  Sint32 *d = (Sint32 *) desP1;
                  b = b << 4;
                  s = (Sint32 *) ((char *) s + b);
                  for (b = 0; b < 16; b++)
                    {
                      d[0] = s[0];
                      d[1] = s[1];
                      d[2] = s[2];
                      d[3] = s[3];
                      s = (Sint32 *) ((char *) s + offSc);
                      d = (Sint32 *) ((char *) d + offDs);
                    }
                }
              desP1 = desP1 + 16;
            }
        }
      else
        {
          //###########################################################
          // display selected line of 32 characters
          //###########################################################
          for (j = 0; j < num_of_columns; j++)
            {
              unsigned char pixel = cyclingtab[color];
              char a = *(p++) - 32;
              if (a)
                {
                  b = c[a];
                  unsigned char *s = (unsigned char *) basPT;
                  unsigned char *d = (unsigned char *) desP1;
                  b = b << 4;
                  s = s + b;
                  for (b = 0; b < 16; b++)
                    {
                      for (Sint32 z = 0; z < 16; z++)
                        {
                          a = s[z];
                          if (a)
                            {
                              a = pixel;
                              d[z] = pixel;
                            }
                        }
                      s = s + offSc;
                      d = d + offDs;
                    }
                }
              desP1 = desP1 + 16;
              if (color++ > 32)
                color = 0;
            }
        }
    }


}

//------------------------------------------------------------------------------
// display text in 320x240 mode
//------------------------------------------------------------------------------
void
handler_popup_menu::display320 ()
{
  Sint32 color = menu_color;
  char *desP1 = pixel_data + char_height + row_size * vertical_space;
  Sint32 offSc = off_source;
  Sint32 offDs = row_size;
  Sint32 offD2 = row_size * (vertical_space - 1) + (char_height * 2);
  Sint32 *basPT = (Sint32 *) caract_adr;
  const char *p = menu_texts[menu_number];
  char *c = ascii2code;


  Uint32 a, b, j;
  Uint32 y = (keyboard->get_mouse_y () - y_coord) / vertical_space;
  y--;
  for (Uint32 k = 0; k < num_of_lines; k++, desP1 += offD2)
    {
      if (y != k)
        {
          //###########################################################
          // display normal line of n characters
          //###########################################################
          for (j = 0; j < num_of_columns; j++)
            {
              a = *(p++) - 32;
              if (a)
                {
                  b = c[a];
                  Sint32 *s = (Sint32 *) basPT;
                  Sint32 *d = (Sint32 *) desP1;
                  b = b << 3;
                  s = (Sint32 *) ((char *) s + b);
                  for (b = 0; b < 8; b++)
                    {
                      d[0] = s[0];
                      d[1] = s[1];
                      s = (Sint32 *) ((char *) s + offSc);
                      d = (Sint32 *) ((char *) d + offDs);
                    }
                }
              desP1 = desP1 + 8;
            }
        }
      else
        {
          //###########################################################
          // display selected line of 32 characters
          //###########################################################
          for (j = 0; j < num_of_columns; j++)
            {
              unsigned char pixel = cyclingtab[color];
              char a = *(p++) - 32;
              if (a)
                {
                  b = c[a];
                  unsigned char *s = (unsigned char *) basPT;
                  unsigned char *d = (unsigned char *) desP1;
                  b = b << 3;
                  s = s + b;
                  for (b = 0; b < 8; b++)
                    {
                      for (Sint32 z = 0; z < 8; z++)
                        {
                          a = s[z];
                          if (a)
                            {
                              a = pixel;
                              d[z] = pixel;
                            }
                        }
                      s = s + offSc;
                      d = d + offDs;
                    }
                }
              desP1 = desP1 + 8;
              if (color++ > 32)
                color = 0;
            }
        }
    }
}

const unsigned char
  handler_popup_menu::cyclingtab[] =
  { 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
  253, 254, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244,
  243, 242, 241, 240, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
  249, 250, 251, 252, 253, 254, 255
};


const char
handler_popup_menu::menu_text1[] =
  { "CONTINUE THE PLAY CURRENT"
"     CAUSE GAME OVER     " "      QUIT TO INTRO      " "     QUIT TECNOBALLZ     "
};
const char
handler_popup_menu::menu_text2[] =
  { "CONTINUE THE PLAY CURRENT"
"      QUIT TO INTRO      " "     QUIT TECNOBALLZ     "
};

const char *
  handler_popup_menu::menu_texts[2] = {
  &handler_popup_menu::menu_text1[0],
  &handler_popup_menu::menu_text2[0],
};
