/** 
 * @file controller_bricks.cc 
 * @brief Control the bricks in bricks levels
 * @created 1996-11-13
 * @date 2007-09-12
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.26 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_bricks.cc,v 1.26 2007/09/12 06:32:48 gurumeditation Exp $
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
#include "../include/controller_bricks.h"
#include "../include/handler_resources.h"
#include "../include/handler_display.h"

/**
 * create the bricks controller
 */
controller_bricks::controller_bricks ()
{
  littleInit ();
  if (bob_ground)
    {
      max_of_sprites = NB_BRICKSH * NB_BRICKSV;
    }
  else
    {
      max_of_sprites = 0;
    }
  sprites_have_shades = true;
  sprite_type_id = BOB_BRICK1;
  bricks_redraw = (brick_redraw *) NULL;
  brikTampon = (char *) NULL;
  bricks_map = (brick_info *) NULL;
  bitmap_bricks = (bitmap_data *) NULL;
  briqueSave = 0;
  brique_clr = 0;
  less_bricks_count = 0;
  less_bricks_delay = 10;
  bricks_height = 112 * resolution;
  bricks_width = 63 * resolution;
  brick_width = BRICK_WIDTH * resolution;
  brick_height = BRICK_HEIGHT * resolution;
  brick_size = brick_width * brick_height;
  brkyoffset = BRKYOFFSET * resolution;
  brickIndus = 8 * brick_height * bricks_height;
  ombre_deca = 3 * resolution;
  ombre_left = (BRICK_HEIGHT * resolution) - ombre_deca;
  ombre_yoff = (BRKYOFFSET - BRICK_HEIGHT) * resolution;
  ombre_top1 = ombre_deca - ombre_yoff;
}

/**
 * release the bricks controller
 */
controller_bricks::~controller_bricks ()
{
  if (bricks_redraw != NULL)
    {
      delete[]bricks_redraw;
      bricks_redraw = NULL;
    }
  if (bricks_map != NULL)
    {
      delete[]bricks_map;
      bricks_map = NULL;
    }
  if (brikTampon != NULL)
    {
      delete[]brikTampon;
      brikTampon = NULL;
    }
  if (bitmap_bricks != NULL)
    {
      delete bitmap_bricks;
      bitmap_bricks = NULL;
    }
  release_sprites_list ();
}

/**
 * First initialization
 */
void
controller_bricks::first_init ()
{
  /* allocate memory for the redraw bricks table */
  if (NULL == bricks_redraw)
    {
      bricks_redraw = new brick_redraw[MAXBRIKCLR];
    }

  /* allocate memory to save background under bricks */
  if (NULL == brikTampon)
    {
      brikTampon = new char[brick_size * NB_BRICKST];
    }

  /* allocate memory for current brick level */
  if (NULL == bricks_map)
    {
      bricks_map = new brick_info[NB_BRICKST];
    }
}

/**
 * Initialize new level and draw bricks
 * @param areaN area number
 * @param tablo level number
 * @param lbrik true if "less bricks" bonus bought into the shop 
 */
void
controller_bricks::initialize ()
{
  if (is_verbose)
    {
      std::cout << ">controller_bricks::initialize() start!" << std::endl;
    }
  if (bob_ground)
    {
      alloc_sprites_list ();
    }


  if (NULL == bitmap_bricks)
    {
      less_bricks_count = current_player->get_less_bricks ();
      current_player->set_less_bricks (0);

      /* clear restauration list */
      briqueSave = 0;
      brique_clr = 0;
      brick_redraw *redraw = bricks_redraw;
      for (Uint32 j = 0; j < MAXBRIKCLR; j++, redraw++)
        {
	  redraw->is_indestructible = false;
	  redraw->is_gigablitz_destroyed = false;
          redraw->xcoord_collision = 0;
          redraw->ycoord_collision = 0;
          redraw->paddle = (sprite_paddle *) NULL;
          redraw->number = 0;
          redraw->is_background = false;
          redraw->pixel_offset = 0;
          redraw->brick_map = (brick_info *) NULL;
        }

      /* initialize current brick level */
      brick_info *map = bricks_map;
      Sint32 c = 0;
      Uint32 color = 239;
      for (Uint32 j = 0; j < NB_BRICKSV * brkyoffset; j += brkyoffset)
        {
          for (Uint32 i = 0; i < NB_BRICKSH * brick_width; i += brick_width)
            {
              map->brique_rel = 0;
              map->is_displayed = false;
              map->pixel_offset = game_screen->get_offset (i, j);
              map->h_pos = 0;
              map->v_pos = 0;
              map->number = c++;
              map->color = color;
              map++;
            }
          if (++color > 255)
            {
              color = 239;
            }
        }

      /* 
       * Select one of 10 sets of bricks
       */
      /* load the 10 sets of bricks */
      bitmap_data *bmp_bricks = new bitmap_data ();
      bmp_bricks->load (handler_resources::BITMAP_BRICKS);
      Sint32 i = random_counter & 0x0F;
      if (i >= 10)
        {
          i = i - 10;
        }
      Sint32 bposy = 0;
      Sint32 bposx = 0;
      if (i >= 5)
        {
          bposy = 63 * resolution;
          i = i - 5;
        }
      bposx = i * 112 * resolution;

      //bposx = 0 * 112 * resolution;     //test only
      //bposy = 1 * 63 * resolution;      //test only

      if (is_verbose)
	{
	  std::cout << "controller_bricks::initialize() " <<
	    "bposx: " << bposx << " bposy: " << bposy << std::endl;
	}
      bitmap_bricks =
        bmp_bricks->cut_to_surface (bposx, bposy, bricks_height, bricks_width);
      delete bmp_bricks;

      /* load one bricks level from the file "tableau.data" */
      Uint32 area_num = current_player->get_area_number ();
      Uint32 level_num = current_player->get_level_number ();
      load_level (area_num, level_num);

      /* read somes values for the graphic routine */
      offsSource = bitmap_bricks->get_line_modulo (brick_width);
      offsDestin = game_screen->get_line_modulo (brick_width);
      adr_source = (Sint32 *) bitmap_bricks->get_pixel_data ();
      adr_desti1 = (Sint32 *) game_screen->get_pixel_data ();
      adr_desti2 = (Sint32 *) background_screen->get_pixel_data ();

      set_bricks_palette ();
    }
  save_background ();           // save background under bricks 
  draw_bricks_shadows ();       // display bricks shadows
  draw_bricks ();               // display bricks
}

/**
 * Return the number of bricks in the current level
 * @return number of bricks
 */
Uint32 controller_bricks::get_num_of_bricks ()
{
  return num_of_bricks;
}

/**
 * Load and initialize a new level
 * @param area_nu area number from 1 to 5
 * @param level_nu level number from 1 to 12
 */
void
controller_bricks::load_level (Sint32 area_nu, Sint32 level_nu)
{
  if (is_verbose)
    {
      std::cout << "controller_bricks::load_level() area_nu: " <<
        area_nu << "level_nu: " << level_nu << std::endl;
    }
  /* a table has 17 lines of 10 columns, 172 * 2 = 340 bytes */ 
  /* clear he number of bricks of the level */
  num_of_bricks = 0;

  /* load bricks levels (34000 bytes => 100 levels) */
  char *all_levels = resources->load_data (handler_resources::DATA_BRICKS_LEVELS);
  if (level_nu >= 6)
    {
      level_nu--;
    }
  char *tabPT = all_levels + (SIZEOFAREA * 2 * (area_nu - 1)) +
    (LEVEL_SIZE * 2 * (level_nu - 1));

  /* Select a level at random.
   * Two different levels are possible for a level */
  if ((random_counter & 0x001))
    {
      tabPT = tabPT + LEVEL_SIZE * 2 * 50;
    }

  /*
   * copy all bricks of the level in the structure "bricks_map"
   */
  brick_info *map = bricks_map;
  /* 6 first lines are always empty */
  map += (6 * NB_BRICKSH);
  Sint32 ycoord = 6 * brkyoffset;
  Sint32 xcoord = 0;
  Uint32 bobindex = 6 * NB_BRICKSH;
  for (Uint32 j = 0; j < BRICKS_MAP_HEIGHT; j++, map += 3, bobindex += 3, ycoord += brkyoffset)
    {
      /* the first 3 columns are always empty */
      map += 3;
      xcoord = brick_width * 3;
      bobindex += 3;
      for (Uint32 i = 0; i < BRICKS_MAP_WIDTH; i++, map++, bobindex++, xcoord += brick_width)
        {
          Sint32 adres = 0;
          /* x position in the bitmap source from 0 to 8 */
          char pos_y = *(tabPT++);
          /* y position in the bitmap source from 0 to 12, step 2 */
          char pos_x = *(tabPT++);
          if (pos_x > 0 || pos_y > 0)
            {                   //pos_x = 12;   // test only
              //pos_y = 1;    // test only
              /* save X-coordinate into bricks_map */
              map->h_pos = pos_x;
              /* save Y-coordinate into bricks_map */
              map->v_pos = pos_y;
              adres =
                bitmap_bricks->get_offset (pos_x * 8 * resolution,
                                           pos_y * brick_height);
              /* it's a indestructible brick? */
              if (adres < brickIndus)
                {
                  /* not, counter's incremented */
                  num_of_bricks++;
                }

              if (bob_ground)
                {
                  sprite_brick *ptbob = new sprite_brick ();
                  ptbob->create_sprite (BOB_BRICK1, bitmap_bricks, 1, 0);
                  sprites_list[bobindex] = ptbob;
                  sprites->add (ptbob);
                  ptbob->set_x_coord(xcoord);
                  ptbob->set_y_coord(ycoord);
                  ptbob->enable ();
                  ptbob->set_image (2);
                }
            }
          map->brique_rel = adres;
          map->is_displayed = adres > 0 ? true : false;
        }

    }
  if (all_levels != NULL)
    {
      delete[](char *) all_levels;
    }

  /* Initialize the number of total bricks to destroy */
  right_panel_score *panel = right_panel_score::get_instance ();
  panel->set_bricks_counter (num_of_bricks);
}

/**
 * Draw the shadows of bricks
 */
void
controller_bricks::draw_bricks_shadows ()
{
  if (bob_ground)
    {
      return;
    }
  brick_info *megaT = bricks_map;
  Sint32 xmax = NB_BRICKSH * brick_width - ombre_deca;
  for (Uint32 j = ombre_deca; j < NB_BRICKSV * brkyoffset + ombre_deca;
       j += brkyoffset)
    {
      for (Sint32 i = -ombre_deca; i < xmax; i += brick_width)
        {
          if (megaT->brique_rel)
            {
               background_screen->fill_shadow_rect (i, j, brick_width, brick_height);
            }
          megaT++;
        }
    }
}

/**
 * Draw all bricks
 */
void
controller_bricks::draw_bricks ()
{
  brick_info *megaT = bricks_map;
  for (Uint32 j = 0; j < NB_BRICKSV * brkyoffset; j += brkyoffset)
    {
      for (Uint32 i = 0; i < NB_BRICKSH * brick_width;
           i += brick_width, megaT++)
        {
          Sint32 pos_x = megaT->h_pos;     // x >= 0 and x < 14
          Sint32 pos_y = megaT->v_pos;     // y >= 0 and y < 9
          if (pos_x || pos_y)
            {
              pos_x *= 8 * resolution;  // planar -> chunky
              pos_y *= brick_height;
              char *source = bitmap_bricks->get_pixel_data (pos_x, pos_y);
              draw_brick (source, megaT->pixel_offset, megaT->color);
            }
        }
    }
}

/**
 * Draw one brick
 * @param pixels pointer on the graphics of the brick
 * @param offset relative pointer on the graphics of the offscreen 
 * @param color colormap number from 239 to 255
 */
void
controller_bricks::draw_brick (char *pixels, Sint32 offset, Sint32 color)
{
  Sint32 offset_src = offsSource;
  Sint32 offset_dst = offsDestin;
  char *screen1 = (char *) adr_desti1;  // pointer to the "buffer"
  char *screen2 = (char *) adr_desti2;  // pointer to the "tampon"
  screen1 += offset;
  screen2 += offset;
  for (Uint32 j = 0; j < brick_height; j++)
    {
      for (Uint32 i = 0; i < brick_width; i++)
        {
          char p = *(pixels++);
          /* color 0 */
          if (p == 0)
            {
              /* replace by the color of the bricks */
              p = color;
            }
          *(screen1++) = p;
          *(screen2++) = p;
        }
      pixels += offset_src;
      screen1 += offset_dst;
      screen2 += offset_dst;
    }
}

/**
 * Initialize the 17 colors of the bricks
 */
void
controller_bricks::set_bricks_palette ()
{
  SDL_Color *palette = display->get_palette ();
  SDL_Color *bricks_pal = palette + 239;
  Uint32 i = random_counter & 0x0f;
  if (i >= 10)
    {
      i = i - 10;
    }
  const Uint32 *color_scale = (handler_resources::tabledegas + i * 18);
  for (i = 0; i < 17; i++)
    {
      Uint32 color = color_scale[i];
      Uint32 blue = color & 0x000000ff;
      Uint32 green = color & 0x0000ff00;
      green = green >> 8;
      Uint32 red = color & 0x00ff0000;
      red = red >> 16;
      bricks_pal->r = red;
      bricks_pal->g = green;
      bricks_pal->b = blue;
      bricks_pal++;
    }
  display->enable_palette (palette);
}

/**
 * save background under bricks 
 */
void
controller_bricks::save_background ()
{
  Sint32 *bufPT = (Sint32 *) brikTampon;
  brick_info *megaT = bricks_map;
  Sint32 offs = game_screen->get_row_size () - brick_width;
  for (Uint32 j = 0; j < NB_BRICKSV * brkyoffset; j += brkyoffset)
    {
      for (Uint32 i = 0; i < NB_BRICKSH * brick_width;
           i += brick_width, megaT++)
        {
          Sint32 *monPT = (Sint32 *) background_screen->get_pixel_data (i, j);
          megaT->briqueFond = bufPT;
          for (Uint32 k = 0; k < brick_height;
               k++, monPT = (Sint32 *) ((char *) monPT + offs))
            {
              for (Uint32 l = 0; l < brick_width / 4; l++)
                {
                  *(bufPT++) = *(monPT++);
                }
            }
        }
    }
}

/**
 * Decrease the counter of bricks to be destroyed
 * It is an option bought from the shop 
 */
void
controller_bricks::less_bricks ()
{
  if (less_bricks_count < 1)
    {
      return;
    }
  if (--less_bricks_delay > 0)
    {
      return;
    }
  less_bricks_delay = 10;
  less_bricks_count--;
  right_panel_score *panel = right_panel_score::get_instance ();
  panel->decrease_bricks_counter (1);
#ifndef SOUNDISOFF
  audio->play_sound (S_TOUBRIK1);
#endif
}

/**
 * Display a brick or restore the background under the brick
 * @return true if the update is not finished 
 */
bool controller_bricks::update ()
{

  right_panel_score *
    panel = right_panel_score::get_instance ();
  controller_capsules *
    capsules = controller_capsules::get_instance ();
  controller_moneys *
    moneys = controller_moneys::get_instance ();


  brick_redraw *
    briPT = bricks_redraw + brique_clr;
  Sint32 adres = briPT->pixel_offset;  // adresse affichage relative
  if (0 == adres)
    {
      return false;
    }
  brique_clr += 1;              // augmente le pointeur sur la table brique a effacer
  brique_clr &= (MAXBRIKCLR - 1);       // limite le compte a 512 (de 0 a 511)
  briPT->pixel_offset = 0;
  brick_info *
    megaT = briPT->brick_map;

  /**
   * restaure background
   */
  if (briPT->is_background)
    {
      Sint32
        line2 = offsDestin;
      Sint32 *
        desP1 = adr_desti1;
      Sint32 *
        desP2 = adr_desti2;
      desP1 = (Sint32 *) ((char *) desP1 + adres);
      desP2 = (Sint32 *) ((char *) desP2 + adres);
      Sint32 *
        srcPT = megaT->briqueFond;


      megaT->is_displayed = 0;
      for (Uint32 j = 0; j < brick_height; j++)
        {
          for (Uint32 i = 0; i < brick_width / 4; i++)
            {
              *(desP1++) = *srcPT;
              *(desP2++) = *(srcPT++);
            }
          desP1 = (Sint32 *) ((char *) desP1 + line2);
          desP2 = (Sint32 *) ((char *) desP2 + line2);
        }
      Sint32
        j;

      //##########################################################
      // clear drop shadow (if needed)
      //##########################################################

      // 1. bottom
      if ((megaT + offBri_BB)->is_displayed)
        j = ombre_yoff;         // brique en bas (2 lignes)
      else
        j = ombre_deca;         // pas de brique en bas (6 lignes)
      Sint32
        decal = display->ecran_next (adres, 0, brick_height);
      display->clr_shadow (decal, brick_width - ombre_deca, j);

      // 2. left-bottom
      if ((megaT + offBri_BG)->is_displayed)
        j = ombre_yoff;
      else
        j = ombre_deca;
      decal = display->ecran_next (adres, -ombre_deca, brick_height);
      display->clr_shadow (decal, ombre_deca, j);

      // 3. left              
      if (!(megaT + offBri_GG)->is_displayed)
        {
          decal = display->ecran_next (adres, -ombre_deca, ombre_deca);
          display->clr_shadow (decal, ombre_deca, ombre_left);
        }

      //##########################################################
      // redraw drop shadow (if needed)
      //##########################################################

      // 1. exist top brick ? 
      if ((megaT + offBri_HH)->is_displayed)
        display->set_shadow (adres, brick_width - ombre_deca, ombre_top1);

      // Il existe une brique en haut a droite ?
      if ((megaT + offBri_HD)->is_displayed)
        {
          decal = display->ecran_next (adres, brick_width - ombre_deca, 0);
          display->set_shadow (decal, ombre_deca, ombre_top1);
        }

      // Il existe une brique a droite ?
      if ((megaT + offBri_DD)->is_displayed)
        {
          decal =
            display->ecran_next (adres, brick_width - ombre_deca, ombre_deca);
          display->set_shadow (decal, ombre_deca, ombre_left);
        }

      /* destroyed indestructible brick */
      if (briPT->is_indestructible)
        {
          current_player->add_score (100);
#ifndef SOUNDISOFF
          audio->play_sound (S_CASSEIND);
#endif
        }
      else
        {
          current_player->add_score (20);
          panel->decrease_bricks_counter (1);
#ifndef SOUNDISOFF
          audio->play_sound (S_TOUBRIK1);
#endif
          //if (briPT->xcoord_collision != 512)
	  if (!briPT->is_gigablitz_destroyed) 
            {
              moneys->send_money_from_brick (briPT);
              capsules->send_capsule_from_bricks (briPT);
            }
        }
    }

  //###############################################################
  // redraw a new brick
  //###############################################################
  else
    {
      char *
        gfxad = bitmap_bricks->get_pixel_data ();
      brick_info *
        megaT = briPT->brick_map;
      draw_brick (gfxad + briPT->number, adres, megaT->color);
      current_player->add_score (10);
#ifndef SOUNDISOFF
      audio->play_sound (S_TOUBRIK1);
#endif
      moneys->send_money_from_brick (briPT);
    }
  return true;
}

/**
 * Clear all bricks, in "Game Over"
 */
void
controller_bricks::clr_bricks ()
{
  brick_info *map = bricks_map;
  /* 6 first lines are always empty */
  map += (6 * NB_BRICKSH);
  for (Uint32 j = 0; j < BRICKS_MAP_HEIGHT; j++, map += 3)
    {
      /* the first 3 columns are always empty */
      map += 3;
      for (Uint32 i = 0; i < BRICKS_MAP_WIDTH; i++, map++)
        {
          if (0 == map->brique_rel)
            {
	      continue;
	    }
	  brick_redraw *redraw = get_bricks_redraw_next ();
          //redraw->xcoord_collision = 512;  // flag brick blitz destroy
	  redraw->is_gigablitz_destroyed = true;
          redraw->pixel_offset = map->pixel_offset;
          redraw->brick_map = map;
          map->h_pos = -1;
          map->brique_rel = 0;
          redraw->number = map->number;
	  /* restore background under brick */
          redraw->is_background = true;
        }
    }
}
 
/**
 * Return pointer to the bricks map of the current level 
 * @return a pointer to the bricks bricks_map
 */
brick_info*
controller_bricks::get_bricks_map ()
{
   return bricks_map; 
}

/**
 * Return pointer to the bricks redraw list
 * @return a pointer to the bricks 
 */
brick_redraw*
controller_bricks::get_bricks_redraw ()
{
  return bricks_redraw + (briqueSave & (MAXBRIKCLR - 1));
}

/**
 * Return pointer to the bricks redraw list
 * @return a pointer to the bricks 
 */
brick_redraw*
controller_bricks::get_bricks_redraw_next ()
{
  Sint32 save = briqueSave;
  briqueSave += 1;
  briqueSave &= (MAXBRIKCLR - 1);
  return bricks_redraw + save;
}

/**
 * Return pointer to the bricks redraw list
 * @return a pointer to the bricks 
 */
void
controller_bricks::bricks_redraw_next ()
{
  briqueSave += 1;
  briqueSave &= (MAXBRIKCLR - 1);
}

/**
 * Return the width of a brick in pixels
 * @return width of a brick in pixels
 */
Sint32 controller_bricks::get_brick_width ()
{
  return brick_width;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Sint32 controller_bricks::getBkIndus ()
{
  return brickIndus;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Sint32 controller_bricks::getYOffset ()
{
  return brkyoffset;
}
