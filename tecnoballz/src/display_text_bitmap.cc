/** 
 * @file display_text_bitmap.cc 
 * @brief Display bitmap strings 
 * @date 2007-02-21
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: display_text_bitmap.cc,v 1.3 2007/02/21 21:07:11 gurumeditation Exp $
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
#include "../include/display_text_bitmap.h"
#include "../include/handler_resources.h"


/**
 * Create the display text object
 */
display_text_bitmap::display_text_bitmap ()
{
  initial_me ();
}

/**
 * Release the display text object
 */
display_text_bitmap::~display_text_bitmap ()
{
  destroy_me ();
}

//------------------------------------------------------------------------------
// small initialization
//------------------------------------------------------------------------------
void
display_text_bitmap::initial_me ()
{
  object_init ();
  bitmap_fonts = (bitmap_data *) NULL;
  off_source = 0;
  off_desti1 = 0;
}

//------------------------------------------------------------------------------
// release handler_resources
//------------------------------------------------------------------------------
void
display_text_bitmap::destroy_me ()
{
  if (bitmap_fonts)
    delete bitmap_fonts;
  bitmap_fonts = (bitmap_data *) NULL;
  object_free ();
}

/**
 * Load fontes used in the shop and the right panel
 */
void
display_text_bitmap::initialize ()
{
  load_bitmap_fonts (handler_resources::BITMAP_GAME_FONTS);
}

/**
 * Return height of chars in pixels
 * @return height of the chars in pixels 
 */
Uint32
display_text_bitmap::get_char_height ()
{
  return char_height;
}

/**
 * Load bitmap fontes and initialize the object
 * @param resource_id bitmap fonts resource identifier
 */
void
display_text_bitmap::load_bitmap_fonts (Uint32 resource_id)
{
  bitmap_fonts = new bitmap_data ();
  bitmap_fonts->load (resource_id);
  surface_fonts = bitmap_fonts->get_surface ();
  fontes_adr = bitmap_fonts->get_pixel_data (216 * resolution, 0);        //characters '0' to '9'
  caract_adr = bitmap_fonts->get_pixel_data (0, 0);       //characters 'A' to 'Z'
  off_desti1 = game_screen->get_row_size ();    //modulo destination
  off_source = bitmap_fonts->get_row_size ();     //modulo source
  
  char_width = CHAR_WIDTH * resolution;
  char_height = bitmap_fonts->get_height ();

  rect_fonts.y = 0; 
  rect_fonts.w = rect_destination.w = char_width;
  rect_destination.h = rect_fonts.h = char_height;
}

// -----------------------------------------------------------------------------
// display number into the "buffer" memory 
//              input   => x            : abscissa
//                              => y            : ordonnee
//                              => value        : value to display
//                              => baseN        : number of characters (10 = 2; 100 = 3; 1000 = 4;..)
// -----------------------------------------------------------------------------
void
display_text_bitmap::bufferAff1 (Sint32 x, Sint32 y, Sint32 value,
                                 Sint32 baseN)
{
  char *desP1 = game_screen->get_pixel_data (x, y);
  affNombre1 (desP1, value, baseN);
}

// -----------------------------------------------------------------------------
// display number into the "tampon" memory 
//              input   => x            : abscissa
//                              => y            : ordinate
//                              => value        : value to display
//                              => baseN        : number of characters (10 = 2; 100 = 3; 1000 = 4;..)
// -----------------------------------------------------------------------------
void
display_text_bitmap::tamponAff1 (Sint32 x, Sint32 y, Sint32 value,
                                 Sint32 baseN)
{
  char *desP1 = background_screen->get_pixel_data (x, y);
  affNombre1 (desP1, value, baseN);
}

// -----------------------------------------------------------------------------
// display number into the "buffer" memory or the "tampon" memory 
//              input   => desP1 : pointer to the buffer or the tampon
//                              => value : value to display
//                              => baseN : number of characters (10 = 2; 100 = 3; 1000 = 4;..)
// -----------------------------------------------------------------------------
void
display_text_bitmap::affNombre1 (char *desP1, Sint32 value, Sint32 baseN)
{
  Sint32 offSc = off_source;
  Sint32 offDs = off_desti1;
#ifndef BYTES_COPY
  Sint32 *basPT = (Sint32 *) fontes_adr;
  if (resolution == 1)
    {
      while (baseN > 0)
        {
          Sint32 i = 0;
          while (value >= baseN)
            {
              value -= baseN;
              i++;
            }
          baseN /= 10;
          Sint32 *s = (Sint32 *) basPT;
          Sint32 *d = (Sint32 *) desP1;
          s = (Sint32 *) ((char *) s + (i << 3));
          for (i = 0; i < 8; i++)
            {
              d[0] = s[0];
              d[1] = s[1];
              s = (Sint32 *) ((char *) s + offSc);
              d = (Sint32 *) ((char *) d + offDs);
            }
          desP1 = desP1 + 8;
        }
    }
  else
    {
      while (baseN > 0)
        {
          Sint32 i = 0;
          while (value >= baseN)
            {
              value -= baseN;
              i++;
            }
          baseN /= 10;
          Sint32 *s = (Sint32 *) basPT;
          Sint32 *d = (Sint32 *) desP1;
          s = (Sint32 *) ((char *) s + (i << 4));
          for (i = 0; i < 16; i++)
            {
              d[0] = s[0];
              d[1] = s[1];
              d[2] = s[2];
              d[3] = s[3];
              s = (Sint32 *) ((char *) s + offSc);
              d = (Sint32 *) ((char *) d + offDs);
            }
          desP1 = desP1 + 16;
        }
    }
#else
  char *basPT = fontes_adr;
  if (resolution == 1)
    {
      while (baseN > 0)
        {
          Sint32 i = 0;
          while (value >= baseN)
            {
              value -= baseN;
              i++;
            }
          baseN /= 10;
          char *s = basPT;
          char *d = desP1;
          s = s + (i << 3);
          s += 8;
          for (i = 0; i < 8; i++)
            {
              d[0] = s[0];
              d[1] = s[1];
              d[2] = s[2];
              d[3] = s[3];
              d[4] = s[4];
              d[5] = s[5];
              d[6] = s[6];
              d[7] = s[7];
              s = s + offSc;
              d = d + offDs;
            }
          desP1 = desP1 + 8;
        }
    }
  else
    {
      while (baseN > 0)
        {
          Sint32 i = 0;
          while (value >= baseN)
            {
              value -= baseN;
              i++;
            }
          baseN /= 10;
          char *s = basPT;
          char *d = desP1;
          s = s + (i << 4);
          for (i = 0; i < 16; i++)
            {
              d[0] = s[0];
              d[1] = s[1];
              d[2] = s[2];
              d[3] = s[3];
              d[4] = s[4];
              d[5] = s[5];
              d[6] = s[6];
              d[7] = s[7];
              d[8] = s[8];
              d[9] = s[9];
              d[10] = s[10];
              d[11] = s[11];
              d[12] = s[12];
              d[13] = s[13];
              d[14] = s[14];
              d[15] = s[15];
              s = s + offSc;
              d = d + offDs;
            }
          desP1 = desP1 + 16;
        }
    }
#endif
}

void
display_text_bitmap::draw (surface_sdl *dest, Uint32 xcoord, Uint32 ycoord, const char* str, Uint32 length)
{

  SDL_Surface *surface_dest = dest->get_surface ();
  rect_destination.x = xcoord;
  rect_destination.y = ycoord;

  if (0 == length) 
    {
      length = strlen(str);
    }


  for (Uint32 i = 0; i < length; i++, rect_destination.x += char_width)
    {
      Uint32 x = str[i] - 32;
      if (x == 0) 
        {  
           continue;
        }
       rect_fonts.x = ascii2code[x] * char_width;
       if (SDL_BlitSurface
           (surface_fonts, &rect_fonts, surface_dest, &rect_destination) < 0)
       {
         std::cerr << "(!)display_text_bitmap::draw() " <<
           "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
         break;
       }
    }
}


// -----------------------------------------------------------------------------
// display string into the "buffer" memory or the "tampon" memory 
//              input   => desP1        : pointer to the buffer or the tampon
//                              => chain        : string to display
//                              => total        : number of characters
// -----------------------------------------------------------------------------
void
display_text_bitmap::aff_texte1 (char *desP1, char *chain, Sint32 total)
{
  Sint32 offSc = off_source;
  Sint32 offDs = off_desti1;
  Sint32 *basPT = (Sint32 *) caract_adr;
  Sint32 a, b;
  char *c = ascii2code;
  char *p = chain;
  if (resolution == 1)
    {
      for (Sint32 j = 0; j < total; j++)
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
      for (Sint32 j = 0; j < total; j++)
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
}

// -----------------------------------------------------------------------------
// display string into the "tampon" memory
//              input   => x            : abscissa
//                              => y            : ordinate
//                              => chain        : string to display
//                              => total        : number of characters
// -----------------------------------------------------------------------------
void
display_text_bitmap::tamponAff2 (Sint32 x, Sint32 y, char *chain,
                                 Sint32 total)
{
  char *desP1 = background_screen->get_pixel_data (x, y);
  aff_texte1 (desP1, chain, total);
}

//------------------------------------------------------------------------------
// display string to buffer
//              input   => x            : abscissa
//                              => y            : ordinate 
//                              => chain        : string to display
//                              => total        : number of characters
//------------------------------------------------------------------------------
void
display_text_bitmap::bufferAff2 (Sint32 x, Sint32 y, char *chain,
                                 Sint32 total)
{
  char *desP1 = game_screen->get_pixel_data (x, y);
  aff_texte1 (desP1, chain, total);
}




//------------------------------------------------------------------------------
// create a BOB to display string (used into guards levels)
//------------------------------------------------------------------------------
sprite_object *
display_text_bitmap::string2bob (const char *ptStr)
{
  Sint32 numch = strlen (ptStr);
  bitmap_data *pBmap = new bitmap_data ();
  //pBmap->create(numch * char_height, char_height, 1);
  pBmap->create_surface (numch * char_height, char_height);
  Sint32 *basPT = (Sint32 *) caract_adr;
  char *desP1 = pBmap->get_pixel_data ();
  Sint32 offSc = off_source;
  Sint32 offDs = pBmap->get_row_size ();
  char *c = ascii2code;
  Sint32 a, b, h;
  if (resolution == 1)
    {
      for (h = 0; h < numch; h++)
        {
          a = *(ptStr++) - 32;
          b = c[a];
          Sint32 *s = (Sint32 *) basPT;
          Sint32 *d = (Sint32 *) desP1;
          b = b << 3;
          s = (Sint32 *) ((char *) s + b);
          for (b = 0; b < 8; b++)
            {
              d[0] = s[0];
              d[1] = s[1];
              d[2] = s[2];
              s = (Sint32 *) ((char *) s + offSc);
              d = (Sint32 *) ((char *) d + offDs);
            }
          desP1 = desP1 + 8;
        }
    }
  else
    {
      for (h = 0; h < numch; h++)
        {
          a = *(ptStr++) - 32;
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
          desP1 = desP1 + 16;
        }
    }

  sprite_object *ptBob = new sprite_object ();
  char *pixel = pBmap->duplicate_pixel_data ();
  ptBob->set_pixel_data (pixel, 1);
  ptBob->sprite_width = pBmap->get_width ();
  ptBob->sprite_height = pBmap->get_height ();
  ptBob->srceNextLn = pBmap->get_row_size ();
  ptBob->max_of_images = 1;
  ptBob->destNextLn = game_screen->get_row_size ();
  delete pBmap;
  return ptBob;
}

//------------------------------------------------------------------------------
// offsets  of the first 128 ASCII characters 
// ; = .
// : = '
// ' = :
// - = ,
// . = -
//------------------------------------------------------------------------------
char
  display_text_bitmap::ascii2code[128] = { 26,  // 32 ' '
  37,                           // 33 '!'
  40,                           // 34 '"'
  26,                           // 35 '#' space
  26,                           // 36 '$' space
  26,                           // 37 '%' space
  26,                           // 38 '&' space
  39,                           // 39 "'"               //40
  26,                           // 40 '(' space
  26,                           // 41 ')' space
  26,                           // 42 '*' space
  26,                           // 43 '+' space
  41,                           // 44 ','       //26
  42,                           // 45 '-'       //41
  38,                           // 46 '.'               //42
  26,                           // 47 '/' space
  27,                           // 48 '0'
  28,                           // 49 '1'
  29,                           // 50 '2'
  30,                           // 51 '3'
  31,                           // 52 '4'
  32,                           // 53 '5'
  33,                           // 54 '6'
  34,                           // 55 '7'
  35,                           // 56 '8'
  36,                           // 57 '9'
  40,                           // 58 ':'               //39
  42,                           // 59 ';'               //38
  26,                           // 60 '<' space
  26,                           // 61 '=' space
  26,                           // 62 '>' space
  26,                           // 63 '?' space
  26,                           // 64 '@' space
  0,                            // 65 'A'
  1,                            // 66 'B'
  2,                            // 67 'C'
  3,                            // 68 'D'
  4,                            // 69 'E'
  5,                            // 70 'F'
  6,                            // 71 'G'
  7,                            // 72 'H'
  8,                            // 73 'I'
  9,                            // 74 'J'
  10,                           // 75 'K'
  11,                           // 76 'L'
  12,                           // 77 'M'
  13,                           // 78 'N'
  14,                           // 79 'O'
  15,                           // 80 'P'
  16,                           // 81 'Q'
  17,                           // 82 'R'
  18,                           // 83 'S'
  19,                           // 84 'T'
  20,                           // 85 'U'
  21,                           // 86 'V'
  22,                           // 87 'W'
  23,                           // 88 'X'
  24,                           // 89 'Y'
  25,                           // 90 'Z'
  26,                           // 91 '[' space
  26,                           // 92 '\' space
  26,                           // 93 ']' space
  26,                           // 94 '^' space
  26,                           // 95 '_' space
  26,                           // 96 '`' space
  26,                           // 97 'a' space
  26,                           // 98 'b' space
  26,                           // 99 'c' space
  26,                           // 100 'd' space
  26,                           // 101 'e' space
  26,                           // 102 'f' space
  26,                           // 103 'g' space
  26,                           // 104 'h' space
  26,                           // 105 'i' space
  26,                           // 106 'j' space
  26,                           // 107 'k' space
  26,                           // 108 'l' space
  26,                           // 109 'm' space
  26,                           // 110 'n' space
  26,                           // 111 'o' space
  26,                           // 112 'p' space
  26,                           // 113 'q' space
  26,                           // 114 'r' space
  26,                           // 115 's' space
  26,                           // 116 't' space
  26,                           // 117 'u' space
  26,                           // 118 'v' space
  26,                           // 119 'w' space
  26,                           // 120 'x' space
  26,                           // 121 'y' space
  26,                           // 122 'z' space
  26,                           // 123 '{' space
  26,                           // 124 '|' space
  26,                           // 125 '}' space
  26,                           // 126 '~' space
  26                            // 127 ' ' space
};