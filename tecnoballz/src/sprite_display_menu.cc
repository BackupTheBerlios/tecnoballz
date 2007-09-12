/** 
 * @file sprite_display_menu.cc 
 * @brief Sprite wich display text of the menu in the menu principal 
 * @date 2007-04-03
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.14 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_display_menu.cc,v 1.14 2007/09/12 06:32:48 gurumeditation Exp $
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
#include "../include/sprite_display_menu.h"
#include "../include/handler_resources.h"
#include "../include/handler_keyboard.h"
#include "../include/handler_players.h"
#include "../include/handler_high_score.h"
#include "../include/supervisor_main_menu.h"

/**
 * Create the sprite displaying text
 */
sprite_display_menu::sprite_display_menu ()
{
  clear_sprite_members ();
  current_menu_section = 0;
  text_offscreen = NULL;
  y_coord_left_down = -10240;
  y_coord_right_down = -10240;
  font_width = 8 * resolution;
  font_height = 8 * resolution;
  if (resolution == 2)
    {
      line_spacing = 17;
    }
  else
    {
      line_spacing = 8;
    }
  menu_colww = 0;
  menu_color = 0;

  clear_addr = (char *) NULL;
  clear_zone_height = 0;
  clear_zone_width = 0;
  clear_zone_xcoord = 0;
  clear_zone_ycoord = 0;
  blink_cursor_delay = 0;
}

/**
 * Release the sprite displaying text
 */
sprite_display_menu::~sprite_display_menu ()
{
  if (NULL != text_offscreen)
    {
      delete text_offscreen;
      text_offscreen = NULL;
    }
  if (NULL != bitmap_fonts)
    {
      delete bitmap_fonts;
      bitmap_fonts = NULL;
    }
}

/**
 * Create offscreen and perform some initializations
 */
void
sprite_display_menu::first_init ()
{

  load_bitmap_fonts (handler_resources::BITMAP_MENU_FONTS);

  /* allocate 512 * 323 pixels buffer for text menu  */
  text_offscreen = new bitmap_data ();
  text_offscreen->create_surface (NUM_OF_COLUMNS * font_width,
                              NUM_OF_ROWS * line_spacing);

  /* initialize sprite object */
  make_sprite (text_offscreen);
  enable ();
  set_coordinates (32 * resolution, 80 * resolution);

  /* initialize palette color chars */
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

/**
 * Check events and draw menu text
 * @return exit code DO_NO_EXIT, PROGRAM_EXIT, or START_GAME
 */
Uint32
sprite_display_menu::check_and_display ()
{
  clear_input_zone ();
  mis_a_jour ();
  Sint32 mousY = keyboard->get_mouse_y ();
  Sint32 y = (mousY - y_coord) / line_spacing;
  Uint32 exit_code = check_events ();

  //###################################################################
  // read color table offset (color line hover by mouse )
  //###################################################################
  if (menu_color++ > 32)
    {
      menu_color = 0;
    }
  Sint32 color = menu_color;

  //###################################################################
  // display menu text    
  //###################################################################
  char *desP1 = pixel_data;
  Sint32 offSc = off_source;
  Sint32 offDs = row_size;
  Sint32 offD2 = row_size * (line_spacing - 1);
  Sint32 *basPT = (Sint32 *) caract_adr;
  char *p = menu_liste[current_menu_section];
  char *c = ascii2code;
  Sint32 a, b, j;

  if (resolution == 1)
    {
      //######################################################
      // mode low-res (320 x 200)
      //######################################################
      for (Sint32 k = 0; k < NUM_OF_ROWS; k++, desP1 += offD2)
        {
          if (y != k)
            {
              //######################################
              // display normal line of 32 characters
              //######################################
              for (j = 0; j < NUM_OF_COLUMNS; j++)
                {
                  a = *(p++) - 32;
                  if (a)
                    {
                      b = c[a];
                      b = b << 3;
#ifndef BYTES_COPY
                      Sint32 *s = (Sint32 *) basPT;
                      Sint32 *d = (Sint32 *) desP1;
                      s = (Sint32 *) ((char *) s + b);
                      for (b = 0; b < 8; b++)
                        {
                          d[0] = s[0];
                          d[1] = s[1];
                          s = (Sint32 *) ((char *) s + offSc);
                          d = (Sint32 *) ((char *) d + offDs);
                        }
#else

                      char *s = (char *) basPT;
                      char *d = desP1;
                      s += b;
                      for (b = 0; b < 8; b++)
                        {
                          d[0] = s[0];
                          d[1] = s[1];
                          d[2] = s[2];
                          d[3] = s[3];
                          d[4] = s[4];
                          d[5] = s[5];
                          d[6] = s[6];
                          d[7] = s[7];
                          s += offSc;
                          d += offDs;
                        }
#endif
                    }
                  desP1 = desP1 + 8;
                }
            }
          else
            {                   //######################################
              // display selected line of 32 characters
              //######################################
              for (j = 0; j < NUM_OF_COLUMNS; j++)
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



  //##############################################################
  // mode hi-res (640 x 400)
  //##############################################################
  else
    {

      for (Sint32 k = 0; k < NUM_OF_ROWS; k++, desP1 += offD2)
        {
          if (y != k)
            {
              //###########################################################
              // display normal line of 32 characters
              //###########################################################
              for (j = 0; j < NUM_OF_COLUMNS; j++)
                {
                  a = *(p++) - 32;
                  if (a)
                    {
                      b = c[a];
                      b = b << 4;
#ifndef BYTES_COPY
                      Sint32 *s = (Sint32 *) basPT;
                      Sint32 *d = (Sint32 *) desP1;
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
#else

                      char *s = (char *) basPT;
                      char *d = desP1;
                      s += b;
                      for (b = 0; b < 16; b++)
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
                          s += offSc;
                          d += offDs;
                        }
#endif
                    }
                  desP1 = desP1 + 16;
                }
            }
          else
            {                   //###########################################################
              // display selected line of 32 characters
              //###########################################################
              for (j = 0; j < NUM_OF_COLUMNS; j++)
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
  draw_input_cursor ();
  return exit_code;
}

/**
 * Check mouse events 
 * @return exit code DO_NO_EXIT, PROGRAM_EXIT, or START_GAME
 */
Uint32
sprite_display_menu::check_events ()
{
  Uint32 exit_code = DO_NO_EXIT;
  Sint32 mposx, pos_y;

  /* check if right or left button are pressed */
  bool is_left_down = keyboard->is_left_button ();
  bool is_right_down = keyboard->is_right_button ();

  //##############################################################
  // read y where is pressed 
  //##############################################################
  if (is_left_down && y_coord_left_down == YCOORDNULL)
    {
      y_coord_left_down = keyboard->get_mouse_y ();
    }
  else
    {
      if (is_right_down && y_coord_right_down == YCOORDNULL)
        {
          y_coord_right_down = keyboard->get_mouse_y ();
        }
    }

  bool is_right_up = false;
  bool is_left_up = keyboard->is_left_button_up (&mposx, &pos_y);
  if (!is_left_up)
    {
      is_right_up = keyboard->is_right_button_up (&mposx, &pos_y);
    }

  if ((is_left_up && pos_y == y_coord_left_down) || (is_right_up && pos_y == y_coord_right_down))
    {
      Sint32 incre = 0;
      if (is_left_up)
        {
          incre = 1;
          y_coord_left_down = YCOORDNULL;
        }
      if (is_right_up)
        {
          incre = -1;
          y_coord_right_down = YCOORDNULL;
        }

      pos_y = (pos_y - y_coord) / line_spacing;
      switch (current_menu_section)
        {

          /* main menu */
        case MAIN_SECTION:
          switch (pos_y)
            {
            case LINE_START:
              clear_text_offscreen ();
              clear_stop ();
              exit_code = START_GAME;
              break;
            case LINE_PARAM:
              clear_text_offscreen ();
              clear_stop ();
              current_menu_section = OPTIONS_SECTION;
              break;
            case LINE_ABOUT:
              audio->play_music (FRIDGE_IN_SPACE_MUSIC);
              clear_text_offscreen ();
              clear_stop ();
              current_menu_section = ABOUT_SECTION;
              break;
            case LINE_SALUT:
              audio->play_music (MON_LAPIN_MUSIC);
              clear_text_offscreen ();
              clear_stop ();
              current_menu_section = GREETINGS_SECTION;
              break;
            case LINE_INFOS:
              audio->play_music (IN_GAME_MUSIC);
              clear_text_offscreen ();
              clear_stop ();
              current_menu_section = INFOS_SECTION;
              break;
            case LINE_SCORE:
              audio->play_music (TERMIGATOR_MUSIC);
              clear_text_offscreen ();
              clear_stop ();
              copyScores ();
              current_menu_section = SCORE_SECTIONS;
              break;

              // input area password
            case LINE_CODE1:
            case LINE_CODE2:
              {
                char *area_code = supervisor_main_menu::get_current_area_code ();
                input_init (10, LINE_CODE2, supervisor_main_menu::AREA_CODE_LENGTH, area_code);
              }
              break;
            case LINE_SORTI:
              exit_code = PROGRAM_EXIT;
              break;
            }
          break;

          /*
           * options
           */
        case OPTIONS_SECTION:
          switch (pos_y)
            {
            case 5:
              clear_init (24, 5, 1, 1);
              if (++number_of_players > (Sint32)handler_players::MAX_OF_PLAYERS)
                {
                  number_of_players = 1;
                }
              break;

              // input players names
            case 6:
              input_init (24, 6, 6,
                          handler_players::players_list[0]->get_name ());
              break;
            case 7:
              input_init (24, 7, 6,
                          handler_players::players_list[1]->get_name ());
              break;
            case 8:
              input_init (24, 8, 6,
                          handler_players::players_list[2]->get_name ());
              break;
            case 9:
              input_init (24, 9, 6,
                          handler_players::players_list[3]->get_name ());
              break;
            case 10:
              input_init (24, 10, 6,
                          handler_players::players_list[4]->get_name ());
              break;
            case 11:
              input_init (24, 11, 6,
                          handler_players::players_list[5]->get_name ());
              break;

              //
            case 12:
              clear_init (24, 12, 4, 1);
              difficulty_level += incre;
              if (difficulty_level > 4)
                difficulty_level = 1;
              if (difficulty_level < 1)
                difficulty_level = 4;
              mis_a_jour ();
              break;

            case 13:
              clear_init (24, 13, 2, 1);
              initial_num_of_lifes += incre;
              if (initial_num_of_lifes > 9)
                initial_num_of_lifes = 1;
              if (initial_num_of_lifes < 1)
                initial_num_of_lifes = 9;
              mis_a_jour ();
              break;

              //return to main menu
            case 14:
              clear_text_offscreen ();
              current_menu_section = MAIN_SECTION;
              clear_stop ();
              break;
            }
          break;

        case ABOUT_SECTION:
          clear_text_offscreen ();
          current_menu_section = MAIN_SECTION;
          break;

        case INFOS_SECTION:
          clear_text_offscreen ();
          current_menu_section = MAIN_SECTION;
          break;

        case GREETINGS_SECTION:
          clear_text_offscreen ();
          current_menu_section = MAIN_SECTION;
          break;

        case SCORE_SECTIONS:
          clear_text_offscreen ();
          current_menu_section = MAIN_SECTION;
          break;
        }
    }
  if (!is_left_down)
    {
      y_coord_left_down = YCOORDNULL;
    }
  if (!is_right_down)
    {
      y_coord_right_down = YCOORDNULL;
    }
  return exit_code;
}

//------------------------------------------------------------------------------
// update strings menu (passwords, players names, difficulty, num of lifes)
//------------------------------------------------------------------------------
void
sprite_display_menu::mis_a_jour ()
{
  const char *s;
  char *d;

  /* copy current area code */
  d = menuTexte0 + (NUM_OF_COLUMNS * LINE_CODE2) + 10;
  supervisor_main_menu::copy_current_area_code (d);

  //###########################################################
  // number of players
  //###########################################################
  d = menuTexte1 + (NUM_OF_COLUMNS * 5) + 24;
  intToASCII (number_of_players, d, 0);

  //###########################################################
  // copy playes names
  //###########################################################
  d = menuTexte1 + (NUM_OF_COLUMNS * 6) + 24;
  for (Uint32 i = 0; i < handler_players::MAX_OF_PLAYERS; i++)
    {
      s = handler_players::players_list[i]->get_name ();
      for (Uint32 j = 0; j < 6; j++)
        d[j] = s[j];
      d += NUM_OF_COLUMNS;
    }
  s = &difficulte[(difficulty_level - 1) * 4];
  d = menuTexte1 + (NUM_OF_COLUMNS * 12) + 24;
  for (Sint32 i = 0; i < 4; i++)
    d[i] = s[i];
  d = menuTexte1 + (NUM_OF_COLUMNS * 13) + 24;
  //intToASCII (initial_num_of_lifes, d, 1);
  integer_to_ascii (initial_num_of_lifes, 2, d);


  birth_flag = 1;
  for (Uint32 i = 0; i < handler_players::MAX_OF_PLAYERS; i++)
    {
      s = handler_players::players_list[i]->get_name ();
      if (s[0] != '0' || s[1] != '4' || s[2] != '0' ||
          s[3] != '6' || s[4] != '7' || s[5] != '0')
        birth_flag = 0;
    }
}

/**
 * Erase the entirety of the offscreen before drawing a new menu section 
 */
void
sprite_display_menu::clear_text_offscreen ()
{
  text_offscreen->clear();
}

/**
 * Clear a zone of the menu (used to modify a string of the menu)
 */
void
sprite_display_menu::clear_input_zone ()
{
  if (!clear_addr)
    return;
  text_offscreen->clear(0, clear_zone_xcoord, clear_zone_ycoord, clear_zone_width, clear_zone_height);
}

//------------------------------------------------------------------------------
// display the cursor (input string)
//------------------------------------------------------------------------------
/**
 * Draw the cursor
 */
void
sprite_display_menu::draw_input_cursor ()
{
  if (!clear_addr)
    return;
  Sint32 xcurs = keyboard->get_input_cursor_pos ();
  if (xcurs < 0)
    {
      return;
    }
  if (--blink_cursor_delay == 0)
    {
      blink_cursor_delay = 50;
    }
  if (blink_cursor_delay > 30)
    {
      return;
    }
  char z = 0xEE;
  char *d = text_offscreen->get_pixel_data(clear_zone_xcoord + xcurs * font_width, clear_zone_ycoord);
  Uint32 n = row_size;
  for (Uint32 h = 0; h < font_height; h++)
    {
      for (Uint32 w = 0; w < font_width; w++)
        {
          if (0 == d[w])
	    {
              d[w] = z;
	    }
        }
      d += n;
    }
}

//------------------------------------------------------------------------------
// initialize string input
//------------------------------------------------------------------------------
void
sprite_display_menu::input_init (Uint32 xcoor, Uint32 ycoor, Uint32 width,
                                 char *strng)
{
  clear_init (xcoor, ycoor, width, 1);
  if (!strng)
    return;
  blink_cursor_delay = 50;
  keyboard->set_input_string (strng, width);
}

//------------------------------------------------------------------------------
// initialize a "zone clear" (used to modify a string of the menu)
//------------------------------------------------------------------------------
void
sprite_display_menu::clear_init (Uint32 xcoor, Uint32 ycoor, Uint32 width,
                                 Uint32 lines)
{
  clear_stop ();
  clear_addr = pixel_data + (ycoor * line_spacing * row_size) +
    (xcoor * font_width);
  clear_zone_width = (width * font_width);
  clear_zone_height = lines * font_height;
  clear_zone_xcoord = xcoor * font_width;
  clear_zone_ycoord = ycoor * line_spacing;
}

//------------------------------------------------------------------------------
// stop the "zone clear"
//------------------------------------------------------------------------------
void
sprite_display_menu::clear_stop ()
{
  clear_addr = (char *) NULL;
  keyboard->stop_string_input ();
}

//------------------------------------------------------------------------------
// copy scores table into menu text
//------------------------------------------------------------------------------
void
sprite_display_menu::copyScores ()
{
  player_score *score = high_score->get_high_score_table ();
  if (NULL == score)
    {
      return;
    }
  char *ptext = menuTexte5 + NUM_OF_COLUMNS * 6;
  for (Uint32 i = 0; i < handler_high_score::MAX_OF_HIGH_SCORES; i++)
    {
      char *pName = score[i].player_name;
      for (Uint32 j = 0; j < handler_players::PLAYER_NAME_LENGTH; j++)
        {
          ptext[6 + j] = pName[j];
        }
      integer_to_ascii (score[i].value, 6,  &ptext[24]);
      integer_to_ascii (score[i].area_number, 1, &ptext[19]);
      integer_to_ascii (score[i].level_number, 2, &ptext[13]);
      ptext += NUM_OF_COLUMNS;
    }
}

//------------------------------------------------------------------------------
// strings of the main menu
//------------------------------------------------------------------------------
char const
  sprite_display_menu::difficulte[] = "EASY" "HARD" "MAD " "DEAD";


char
  sprite_display_menu::menuTexte0[] = "                                "        //0
  "                                "    //1
  "                                "    //2
  "                                "    //3
  "                                "    //4
  "        START THE GAMEq         "    //5
  "          ? OPTIONS ?           "    //6
  "          > CREDITS <           "    //7
  "         g GREETINGS g          "    //8
  "             INFOS              "    //9
  "          VIEW-SCORES           "    //10 
  "           PASSWORDq            "    //11
  "        u ----------? t         "    //12
  "          GAME  EXITw           "    //13
  "                                "    //14
  "                                "    //15
  "                                "    //16
  "                                "    //17 
  "                                ";   //18

char
  sprite_display_menu::menuTexte1[] = "                                " "                                " "                                " "                                " "--------------------------------" " e NUMBER OF PLAYERS  u 1 t     " " b PLAYER 1.......... u ......t "      //6
  " b PLAYER 2.......... u ......t " " b PLAYER 3.......... u ......t " " b PLAYER 4.......... u ......t " " b PLAYER 5.......... u ......t " " b PLAYER 6.......... u ......t " " e DIFFICULTY........ u EASY t  "     // 12
  " e NUMBER OF LIFES... u 08 t    "    // 13
  " c MAIN MENUqq                  "
  "--------------------------------"
  "                                "
  "                                " "                                ";

char
  sprite_display_menu::menuTexte2[] =
  "--- MAIN CODING - LINUX-PORT ---"
  "      b BRUNO  ETHVIGNOT b      "
  "---- BASED ON AMIGA MC680X0 ----"
  "b BRUNO ETHVIGNOT;JEROME BOLOT b"
  "                                "
  "----------- MAIN GFX -----------"
  "bJEAN MICHEL  MARTIN DE SANTEROb"
  " --------- OTHER  GFX --------- "
  "       b RODOLPHE  BONO b       "
  "                                "
  "---------- ALL MUSICS ----------"
  "        b REGIS PARRET b        "
  "---------SOUNDS EFFECTS---------"
  "       b LAURENT  GUYON  b      "
  "                                "
  "COPYRIGHT (C)1992-2005 TLK-GAMES"
  "TLK-GAMES/BP 24/F81150 FLORENTIN"
  "         LINUX.TLK.FR           " "                                ";

char
  sprite_display_menu::menuTexte3[] =
  "                                "
  " BUMPER CONTROL:                "
  " LEFT MOUSE BUTTON : FIRE       "
  " RIGHT MOUSE BUTTON: DROP BALL  "
  " LEFT AND RIGHT    : TILT       "
  "                     GIGABLITZ  "
  "--------------------------------"
  "KEYS RECOGNIZED DURING THE GAME:"
  " F       : FULL SCREEN          "
  " P       : PAUSE                "
  " CTRL ESC: QUIT TECNOBALLZ      "
  " CTRL D  : DISABLE MFX          "
  " CTRL F  : DISABLE SFX          "
  " CTRL S  : DISABLE SFX - MFX    "
  " CTRL Q  : EXIT TO MENU         "
  " CTRL X  : GO TO GAME-OVER      "
  "--------------------------------"
  " 25,000 POINTS      : BONUS LIFE"
  "                                " "                                ";

char
  sprite_display_menu::menuTexte4[] =
  "                                "
  "---> BEST GREETINGS  FLY TO <---"
  "                                "
  " b ALEXIS       b AURELIEN      "
  " b BLACKGUARD   b BARTI         "
  " b DAUBMAN      b DELPHINUS     "
  " b DARK NIGHT   b FELBRAN       "
  " b DJI          b JRC           "
  " b MA DANONE    b LE CHACAL     "
  " b LE TEXAN     b YANIS         "
  " b JMM          b ROY           "
  " b PAT          b PROPERMAN     "
  " b PASCAL L.    b PASCAL E.     "
  " b PIXELMAN     b PIERRE DENIS  "
  " b STEPHANE C.  b POPOLON       "
  " b ZIBIBI       b SHAD          "
  " b REGLIS       b ZE-KING       "
  "                                " "                                ";

char
  sprite_display_menu::menuTexte5[] =
  "                                "
  "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
  "b        BEST  TECNOBALLZ      b"
  "b                              b"
  "b POS NAME   LEVEL AREA SCORE  b"
  "b                              b"
  "b 01  ?????? ??    ?    ?????? b"
  "b 02  ?????? ??    ?    ?????? b"
  "b 03  ?????? ??    ?    ?????? b"
  "b 04  ?????? ??    ?    ?????? b"
  "b 05  ?????? ??    ?    ?????? b"
  "b 06  ?????? ??    ?    ?????? b"
  "b 07  ?????? ??    ?    ?????? b"
  "b 08  ?????? ??    ?    ?????? b"
  "b 09  ?????? ??    ?    ?????? b"
  "b 10  ?????? ??    ?    ?????? b"
  "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
  "                                " "                                ";


char *
  sprite_display_menu::menu_liste[NUM_OF_SECTIONS] =
  { menuTexte0, menuTexte1, menuTexte2,
  menuTexte3, menuTexte4, menuTexte5
};

//------------------------------------------------------------------------------
// offsets of the first 128 ASCII characters 
//------------------------------------------------------------------------------
char
  sprite_display_menu::ascii2code[128] = { 26,  // 32 ' '
  37,                           // 33 '!'
  40,                           // 34 '"'
  26,                           // 35 '#' space
  26,                           // 36 '$' space
  26,                           // 37 '%' space
  26,                           // 38 '&' space
  40,                           // 39 "'"
  48,                           // 40 '('
  49,                           // 41 ')'
  47,                           // 42 '*'
  26,                           // 43 '+' space
  26,                           // 44 ',' 
  41,                           // 45 '-'
  42,                           // 46 '.'
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
  39,                           // 58 ':'
  38,                           // 59 ';'
  44,                           // 60 '<'
  26,                           // 61 '=' space
  45,                           // 62 '>'
  43,                           // 63 '?'
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
  48,                           // 91 '['
  26,                           // 92 '\' space
  49,                           // 93 ']'
  26,                           // 94 '^' space
  26,                           // 95 '_' space
  26,                           // 96 '`' space
  26,                           // 97 'a' space
  47,                           // 98 'b' gray star 
  52,                           // 99 'c' horizontal lines 
  26,                           // 100 'd' space
  50,                           // 101 'e' full square
  26,                           // 102 'f' space
  46,                           // 103 'g' gray heart 
  26,                           // 104 'h' space
  26,                           // 105 'i' space
  26,                           // 106 'j' space
  26,                           // 107 'k' space
  26,                           // 108 'l' space
  26,                           // 109 'm' space
  26,                           // 110 'n' space
  26,                           // 111 'o' space
  26,                           // 112 'p' space
  55,                           // 113 'q' ! white
  26,                           // 114 'r' space
  26,                           // 115 's' space
  53,                           // 116 't' < white
  54,                           // 117 'u' > white
  26,                           // 118 'v' space
  51,                           // 119 'w' pink ellipsis 
  26,                           // 120 'x' space
  26,                           // 121 'y' space
  26,                           // 122 'z' space
  26,                           // 123 '{' space
  26,                           // 124 '|' space
  26,                           // 125 '}' space
  26,                           // 126 '~' space
  26                            // 127 ' ' space
};

const unsigned char
  sprite_display_menu::cyclingtab[] =
  { 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
  253, 254, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244,
  243, 242, 241, 240, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
  249, 250, 251, 252, 253, 254, 255
};
