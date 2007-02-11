/** 
 * @file controller_bricks.cc 
 * @brief Control the bricks in bricks levels
 * @created 1996-11-13
 * @date 2007-02-11
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.9 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_bricks.cc,v 1.9 2007/02/11 21:03:24 gurumeditation Exp $
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
#include "../include/controller_bricks.h"
#include "../include/handler_resources.h"
#include "../include/handler_memory.h"
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
  brique_pnt = (brickClear *) NULL;
  brikTampon = (char *) NULL;
  mega_table = (brickInfos *) NULL;
  bitmap_bricks = (bitmap_data *) NULL;
  briqueSave = 0;
  brique_clr = 0;
  less_bricks_count = 0;
  less_bricks_delay = 10;
  bricks_height = 112 * resolution;
  bricks_width = 63 * resolution;
  brick_width = BRICKWIDTH * resolution;
  brick_height = BRICKHEIGH * resolution;
  brick_size = brick_width * brick_height;
  brkyoffset = BRKYOFFSET * resolution;
  brickIndus = 8 * brick_height * bricks_height;
  ombre_deca = 3 * resolution;
  ombre_left = (BRICKHEIGH * resolution) - ombre_deca;
  ombre_yoff = (BRKYOFFSET - BRICKHEIGH) * resolution;
  ombre_top1 = ombre_deca - ombre_yoff;
}

/**
 * release the bricks controller
 */
controller_bricks::~controller_bricks ()
{
 if (brique_pnt != NULL)
    {
      delete[]brique_pnt;
      brique_pnt = NULL;
    }
  if (mega_table != NULL)
    {
      delete[]mega_table;
      mega_table = NULL;
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

//-------------------------------------------------------------------------------
// first initialization
//-------------------------------------------------------------------------------
void
controller_bricks::first_init (right_panel_score * barre, controller_moneys * capsu,
                               controller_capsules * gadge)
{
  barreObjet = barre;
  caps_objet = capsu;
  gads_objet = gadge;

  /* allocate memory for the redraw bricks table */
  if (NULL == brique_pnt)
    {
      brique_pnt = new brickClear[MAXBRIKCLR];
    }

  /* allocate memory to save background under bricks */
  if (NULL == brikTampon)
    {
      brikTampon = new char[brick_size * NB_BRICKST];
   }

  /* allocate memory for current brick level */
  if (NULL == mega_table)
    {
      mega_table = new brickInfos[NB_BRICKST]; 
   }
}

/**
 * Initialize new level and draw bricks
 * @param areaN area number
 * @param tablo level number
 * @param lbrik true if "less bricks" bonus bought into the shop 
 */
void
controller_bricks::initialize (Sint32 areaN, Sint32 tablo, Sint32 lbrik)
{

  if (bob_ground)
    {
      alloc_sprites_list ();
    }

  if (NULL == bitmap_bricks)
    {
      less_bricks_count = lbrik;

      /* clear restauration list */
      briqueSave = 0;
      brique_clr = 0;
      brickClear *briPT = brique_pnt;
      for (Sint32 j = 0; j < MAXBRIKCLR; j++, briPT++)
        {
          briPT->balle_posX = 0;
          briPT->balle_posY = 0;
          briPT->raquettePT = (sprite_paddle *) 0x0;
          briPT->brique_num = 0;
          briPT->briqueFlag = 0;
          briPT->adresseAff = 0;
          briPT->adresseTab = (brickInfos *) 0x0;
        }

      /* initialize current brick level */
      brickInfos *megaT = mega_table;
      Sint32 c = 0;
      Sint32 vacol = 239;
      for (Sint32 j = 0; j < NB_BRICKSV * brkyoffset; j += brkyoffset)
        {
          for (Sint32 i = 0; i < NB_BRICKSH * brick_width; i += brick_width)
            {
              megaT->brique_rel = 0;
              megaT->brique_aff = 0;
              megaT->adresseAff = game_screen->get_offset (i, j);
              megaT->briquePosX = 0;
              megaT->briquePosY = 0;
              megaT->brique_num = c++;
              megaT->brickcolor = vacol;
              megaT++;
            }
          if (++vacol > 255)
            {
              vacol = 239;
            }
        }

      /* 
       * Select one of 10 sets of bricks
       */
      /* load the 10 sets of bricks */
      bitmap_data* bmp_bricks = new bitmap_data ();
      bmp_bricks->load (handler_resources::RESZEBRICK);
      Sint32 i = hasard_val & 0x0F;
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

      bposx = 0 * 112 * resolution;     //test only
      bposy = 1 * 63 * resolution;      //test only

      if (is_verbose)
        printf ("controller_bricks::initialise()  bposx=%i / bposy=%i \n",
                bposx, bposy);
      bitmap_bricks = bmp_bricks->cut (bposx, bposy, bricks_height, bricks_width);
      delete bmp_bricks;

      /* load one bricks level from the file "tableau.data" */
      load_level (areaN, tablo);

      /* read somes values for the graphic routine */
      offsSource = bitmap_bricks->get_line_modulo (brick_width);
      offsDestin = game_screen->get_line_modulo (brick_width);
      adr_source = (Sint32 *) bitmap_bricks->get_pixel_data ();
      adr_desti1 = (Sint32 *) game_screen->get_pixel_data ();
      adr_desti2 = (Sint32 *) background_screen->get_pixel_data ();

      initpalett ();
    }
  sauve_fond ();                // save background under bricks 
  draw_bricks_shadows ();                // display bricks shadows
  draw_bricks ();                // display bricks
}

/**
 * Return the number of bricks in the current level
 * @return number of bricks
 */
Sint32
controller_bricks::get_num_of_bricks ()
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
  // un tableau fait 17 lignes sur 10 colonnes = 170 * 2 = 340 octets  
  // a table makes 17 lines out of 10 columns 
  /* clear he number of bricks of the level */
  num_of_bricks = 0;

  /* load bricks levels (34000 bytes => 100 levels) */
  char *all_levels = resources->load_data (handler_resources::RESBLEVELS);
  if (level_nu >= 6)
    {
      level_nu--;
    }
  char *tabPT = all_levels + (SIZEOFAREA * 2 * (area_nu - 1)) +
    (LEVEL_SIZE * 2 * (level_nu - 1));

  /* Select a level at random.
   * Two different levels are possible for a level */
  if ((hasard_val & 0x001))
    {
      tabPT = tabPT + LEVEL_SIZE * 2 * 50;
    }

  //###################################################################
  // copy all bricks of the level in the structure "mega_table"
  //###################################################################
  brickInfos *megaT = mega_table;
  megaT += (6 * NB_BRICKSH);    // 6 first lines are always empty
  Uint32 bobindex = 6 * NB_BRICKSH;
  for (Sint32 j = 0; j < LEVELHEIGH; j++, megaT += 3, bobindex += 3)
    {
      megaT += 3;               // the first 3 columns are always empty 
      bobindex += 3;
      for (Sint32 i = 0; i < LEVELWIDTH; i++, megaT++, bobindex++)
        {
          Sint32 adres = 0;
          char pos_y = *(tabPT++);      // position y dans page brique de 0 a 8
          char pos_x = *(tabPT++);      // position x dans page brique de 0 a 12 (step 2) 12*8*2=192
          if (pos_x || pos_y)
            {                   //pos_x = 12;   // test only
              //pos_y = 1;    // test only
              megaT->briquePosX = pos_x;        // save X-coordinate into mega_table
              megaT->briquePosY = pos_y;        // save Y-coordinate into mega_table
              adres =
                bitmap_bricks->get_offset (pos_x * 8 * resolution,
                                        pos_y * brick_height);
              if (adres < brickIndus)   // it's a indestructible brick?
                num_of_bricks++;   // not, counter's incremented

              if (bob_ground)
                {
                  sprite_object *ptbob = new sprite_object ();
                  ptbob->create_sprite (BOB_BRICK1, bitmap_bricks, 1, 0);
                  sprites_list[bobindex] = ptbob;
                  sprites->add (ptbob);
                  ptbob->enable ();
                  ptbob->set_image (2);
                }


            }
          megaT->brique_rel = adres;
          megaT->brique_aff = adres;
        }

    }
  if (all_levels != NULL)
    {
      delete[](char *) all_levels;
    }

  /* Initialize the number of total bricks to destroy */
  barreObjet->scoreBrick (num_of_bricks);
}

/**
 * Draw the shadows of bricks
 */
void
controller_bricks::draw_bricks_shadows ()
{
  brickInfos *megaT = mega_table;
  for (Sint32 j = ombre_deca; j < NB_BRICKSV * brkyoffset + ombre_deca;
       j += brkyoffset)
    {
      for (Sint32 i = -ombre_deca; i < NB_BRICKSH * brick_width - ombre_deca;
           i += brick_width)
        {
          if (megaT->brique_rel)
            display->rectShadow (i, j, brick_width, brick_height);
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
  brickInfos *megaT = mega_table;
  for (Sint32 j = 0; j < NB_BRICKSV * brkyoffset; j += brkyoffset)
    {
      for (Sint32 i = 0; i < NB_BRICKSH * brick_width;
           i += brick_width, megaT++)
        {
          Sint32 pos_x = megaT->briquePosX;     // x >= 0 and x < 14
          Sint32 pos_y = megaT->briquePosY;     // y >= 0 and y < 9
          if (pos_x || pos_y)
            {
              pos_x *= 8 * resolution;  // planar -> chunky
              pos_y *= brick_height;
              char *srcPT = bitmap_bricks->get_pixel_data (pos_x, pos_y);
              draw_brick (srcPT, megaT->adresseAff, megaT->brickcolor);
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
  char *screen1 = (char *) adr_desti1;    // pointer to the "buffer"
  char *screen2 = (char *) adr_desti2;    // pointer to the "tampon"
  screen1 += offset;
  screen2 += offset;
  for (Sint32 j = 0; j < brick_height; j++)
    {
      for (Sint32 i = 0; i < brick_width; i++)
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

// -----------------------------------------------------------------------------
// initialize the 17 colors of the bricks
// -----------------------------------------------------------------------------
void
controller_bricks::initpalett ()
{
  SDL_Color *palPT = display->paletteAdr ();
  SDL_Color *palP1 = palPT + 239;
  Sint32 i = hasard_val;
  i &= 0x0f;
  //Sint32 i = hasard_val & 0x0F;
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

//------------------------------------------------------------------------------
// save background under bricks 
//------------------------------------------------------------------------------
void
controller_bricks::sauve_fond ()
{
  Sint32 *bufPT = (Sint32 *) brikTampon;
  brickInfos *megaT = mega_table;
  Sint32 offs = game_screen->get_row_size () - brick_width;
  for (Sint32 j = 0; j < NB_BRICKSV * brkyoffset; j += brkyoffset)
    {
      for (Sint32 i = 0; i < NB_BRICKSH * brick_width;
           i += brick_width, megaT++)
        {
          Sint32 *monPT = (Sint32 *) background_screen->get_pixel_data (i, j);
          megaT->briqueFond = bufPT;
          for (Sint32 k = 0; k < brick_height;
               k++, monPT = (Sint32 *) ((char *) monPT + offs))
            {
              for (Sint32 l = 0; l < brick_width / 4; l++)
                *(bufPT++) = *(monPT++);
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
  barreObjet->brickMoins (1);
#ifndef SOUNDISOFF
  audio->play_sound (S_TOUBRIK1);
#endif
}

//------------------------------------------------------------------------------
// display a brick or restore the background under the brick
//------------------------------------------------------------------------------
Sint32
controller_bricks::brickRemap ()
{
  brickClear *briPT = brique_pnt + brique_clr;
  Sint32 adres = briPT->adresseAff;     // adresse affichage relative
  if (!adres)
    return 0;
  {
    brique_clr += 1;            // augmente le pointeur sur la table brique a effacer
    brique_clr &= (MAXBRIKCLR - 1);     // limite le compte a 512 (de 0 a 511)
    briPT->adresseAff = 0;
    brickInfos *megaT = briPT->adresseTab;

    //###############################################################
    // restaure background
    //###############################################################
    if (briPT->briqueFlag)      // 0 = redraw brick / 1 = restore background
      {
        Sint32 line2 = offsDestin;
        Sint32 *desP1 = adr_desti1;
        Sint32 *desP2 = adr_desti2;
        desP1 = (Sint32 *) ((char *) desP1 + adres);
        desP2 = (Sint32 *) ((char *) desP2 + adres);
        Sint32 *srcPT = megaT->briqueFond;


        megaT->brique_aff = 0;
        for (Sint32 j = 0; j < brick_height; j++)
          {
            for (Sint32 i = 0; i < brick_width / 4; i++)
              {
                *(desP1++) = *srcPT;
                *(desP2++) = *(srcPT++);
              }
            desP1 = (Sint32 *) ((char *) desP1 + line2);
            desP2 = (Sint32 *) ((char *) desP2 + line2);
          }
        Sint32 j;

        //##########################################################
        // clear drop shadow (if needed)
        //##########################################################

        // 1. bottom
        if ((megaT + offBri_BB)->brique_aff)
          j = ombre_yoff;       // brique en bas (2 lignes)
        else
          j = ombre_deca;       // pas de brique en bas (6 lignes)
        Sint32 decal = display->ecran_next (adres, 0, brick_height);
        display->clr_shadow (decal, brick_width - ombre_deca, j);

        // 2. left-bottom
        if ((megaT + offBri_BG)->brique_aff)
          j = ombre_yoff;
        else
          j = ombre_deca;
        decal = display->ecran_next (adres, -ombre_deca, brick_height);
        display->clr_shadow (decal, ombre_deca, j);

        // 3. left              
        if (!(megaT + offBri_GG)->brique_aff)
          {
            decal = display->ecran_next (adres, -ombre_deca, ombre_deca);
            display->clr_shadow (decal, ombre_deca, ombre_left);
          }

        //##########################################################
        // redraw drop shadow (if needed)
        //##########################################################

        // 1. exist top brick ? 
        if ((megaT + offBri_HH)->brique_aff)
          display->set_shadow (adres, brick_width - ombre_deca, ombre_top1);

        // Il existe une brique en haut a droite ?
        if ((megaT + offBri_HD)->brique_aff)
          {
            decal = display->ecran_next (adres, brick_width - ombre_deca, 0);
            display->set_shadow (decal, ombre_deca, ombre_top1);
          }

        // Il existe une brique a droite ?
        if ((megaT + offBri_DD)->brique_aff)
          {
            decal =
              display->ecran_next (adres, brick_width - ombre_deca,
                                   ombre_deca);
            display->set_shadow (decal, ombre_deca, ombre_left);
          }

        // destroyed indestructible brick
        if (briPT->balle_posX < 0)
          {
            current_player->add_score (100);
#ifndef SOUNDISOFF
            audio->play_sound (S_CASSEIND);
#endif
          }
        else
          {
            current_player->add_score (20);
            barreObjet->brickMoins (1);
#ifndef SOUNDISOFF
            audio->play_sound (S_TOUBRIK1);
#endif
            if (briPT->balle_posX != 512)
              {
                caps_objet->send_money_from_brick (briPT);
                gads_objet->envoieGads (briPT);
              }
          }

      }

    //###############################################################
    // redraw a new brick
    //###############################################################
    else
      {
        char *gfxad = bitmap_bricks->get_pixel_data ();
        brickInfos *megaT = briPT->adresseTab;
        draw_brick (gfxad + briPT->brique_num, adres, megaT->brickcolor);
        current_player->add_score (10);
#ifndef SOUNDISOFF
        audio->play_sound (S_TOUBRIK1);
#endif
        caps_objet->send_money_from_brick (briPT);
      }
  }                             // (adresseAff!=0)
  return 1;
}

//------------------------------------------------------------------------------
// clear all bricks (Game Over)
//------------------------------------------------------------------------------
void
controller_bricks::clr_bricks ()
{
  Sint32 save = briqueSave;
  brickClear *briPT = brique_pnt;
  brickInfos *tMega = mega_table;
  tMega += (6 * NB_BRICKSH);    // 6 first lines are always empty
  for (Sint32 j = 0; j < LEVELHEIGH; j++, tMega += 3)
    {
      tMega += 3;               // the first 3 columns are always empty 
      for (Sint32 i = 0; i < LEVELWIDTH; i++, tMega++)
        {
          brickInfos *megaT = tMega;
          brickClear *briP2 = briPT + save;
          Sint32 v = megaT->brique_rel;
          if (v)
            {
              briP2->balle_posX = 512;  // flag brick blitz destroy
              briP2->adresseAff = megaT->adresseAff;
              briP2->adresseTab = megaT;
              megaT->briquePosX = -1;
              megaT->brique_rel = 0;    //RAZ brick code
              briP2->brique_num = megaT->brique_num;    //brick number
              briP2->briqueFlag = 1;    //flag restore background
              save += 1;        // inc. pt restaure table
              save &= (MAXBRIKCLR - 1);
            }
        }
    }
}

/**
 * Return the width of a brick in pixels
 * @return width of a brick in pixels
 */
Sint32
controller_bricks::get_brick_width ()
{
  return brick_width;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Sint32
controller_bricks::getBkIndus ()
{
  return brickIndus;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Sint32
controller_bricks::getYOffset ()
{
  return brkyoffset;
}
