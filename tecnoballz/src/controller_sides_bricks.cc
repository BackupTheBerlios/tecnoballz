/** 
 * @file controller_sides_bricks.cc 
 * @brief Sides bricks controller. The small bricks on the side, the walls top
 *        left and right
 * @date 2007-02-14
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_sides_bricks.cc,v 1.2 2007/02/14 07:15:30 gurumeditation Exp $
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
#include "../include/controller_sides_bricks.h"
#include "../include/handler_players.h"
#include "../include/handler_display.h"

/**
 * Create the sides bricks controller
 */
controller_sides_bricks::controller_sides_bricks ()
{
  littleInit ();
  if (bob_ground)
    {
      max_of_sprites = (MAX_OF_SIDES_BRICKS + 4) * 3;
    }
  else
    {
      max_of_sprites = 2;
    }
  sprites_have_shades = true;
  
  horizontal_brick = (sprite_object *) NULL;
  vertical_brick = (sprite_object *) NULL;
  is_top_wall_breakable = false;
  is_right_wall_breakable = false;
  is_left_wall_breakable = false;
  fond_sauve = (char *) NULL;   //buffer to save background under bricks
  pCoteTable = (coteStruct *) NULL;
  iCoteSauve = 0;
  iCoteRemap = 0;
  
  for (Uint32 i = 0; i < (MAX_OF_SIDES_BRICKS + 4); i++)
    {
      map_top_side[i] = map_right_side[i] = map_left_side[i] = false;
    }
  map_top_side[0] = map_right_side[0] = map_left_side[0] = true;
  map_top_side[MAX_OF_SIDES_BRICKS + 1] = map_right_side[MAX_OF_SIDES_BRICKS + 1] = map_left_side[MAX_OF_SIDES_BRICKS + 1] = true;
  
  map_left_wall = map_left_side + 2;
  map_right_wall = map_right_side + 2;
  map_top_wall = map_left_side + 2;
  
  for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)
    {
      bobwal_top[i] = NULL;
      bobwal_lef[i] = NULL;
      bobwal_rgh[i] = NULL;
    }
}

/**
 * Release the sides bricks controller
 */
controller_sides_bricks::~controller_sides_bricks ()
{
  memory->release ((char *) pCoteTable);
  if (horizontal_brick)
    {
      delete horizontal_brick;        // release sprite object
      horizontal_brick = (sprite_object *) NULL;
    }
  if (vertical_brick)
    {
      delete vertical_brick;        // release sprite object
      vertical_brick = (sprite_object *) NULL;
    }
  if (fond_sauve)
    {
      memory->release (fond_sauve);
      fond_sauve = NULL;
    }
  for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)
    {
      if (bobwal_top[i] != NULL)
        {
          delete bobwal_top[i];
          bobwal_top[i] = NULL;
        }
      if (bobwal_lef[i] != NULL)
        {
          delete bobwal_lef[i];
          bobwal_lef[i] = NULL;
        }
      if (bobwal_rgh[i] != NULL)
        {
          delete bobwal_rgh[i];
          bobwal_rgh[i] = NULL;
        }
    }
  object_free ();
  release_sprites_list ();
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
Sint32
controller_sides_bricks::initialise (Sint32 build)
{
  //###################################################################
  // create a sprite object for small horizontal bricks 
  //###################################################################
  horizontal_brick = new sprite_object ();
  horizontal_brick->create_sprite (BOB_BRICKH, sprites_bitmap, 1);
  horizontal_brick_width = horizontal_brick->get_sprite_width ();
  horizontal_brick_height = horizontal_brick->get_sprite_height ();

  //###################################################################
  // create a sprite object for small vertical bricks 
  //###################################################################
  vertical_brick = new sprite_object ();
  vertical_brick->create_sprite (BOB_BRICKV, sprites_bitmap, 1);
  vertical_brick_width = vertical_brick->get_sprite_width ();
  vertical_brick_height = vertical_brick->get_sprite_height ();

  collisionG = (BRICOTEGAX * resolution) + vertical_brick_width;
  collisionD = BRICOTEDRX * resolution;
  collisionH = BRICOTEHRY * resolution + horizontal_brick_height;


  //###################################################################
  // area 2 to 4; rebuild wall
  //###################################################################
  Sint32 area = current_player->get_area_number ();
  if (area < 5 || build > 0)
    {
      for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)
        {
          map_left_wall[i] = true;
          map_top_wall[i] = true;
          map_right_wall[i] = true;
        }
    }

  //###################################################################
  // area 5: recopy the state of the wall of the preceding level
  //###################################################################
  else
    {
      char *bleft = current_player->getBriLeft ();
      char *brigh = current_player->getBriRigh ();
      char *b_top = current_player->getBri_top ();
      for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)
        {
          map_left_wall[i] = bleft[i];
          map_top_wall[i] = b_top[i];
          map_right_wall[i] = brigh[i];
        }
    }

  /* determine the behavior of the three walls, according to the area */
  switch (area)
    {
    case 2:
      is_top_wall_breakable = true;
      is_right_wall_breakable = false;
      is_left_wall_breakable = false;
      break;

    case 3:
      is_top_wall_breakable = true;
      is_right_wall_breakable = true;
      is_left_wall_breakable = false;
      break;

    case 4:
      is_top_wall_breakable = true;
      is_right_wall_breakable = true;
      is_left_wall_breakable = true;
      break;

    case 5:
      is_top_wall_breakable = true;
      is_right_wall_breakable = true;
      is_left_wall_breakable = true;
      break;

    default:
      is_top_wall_breakable = false;
      is_right_wall_breakable = false;
      is_left_wall_breakable = false;
      break;
    }
/*
	is_top_wall_breakable = 1;	//1=wall of the top is breakable (test only)
	is_right_wall_breakable = 1;	//1=wall of the right is breakable (test only)
	is_left_wall_breakable = 1;	//1=wall of the left is breakable (test only)
*/

  //###################################################################
  // allocate memory to save background under bricks
  //###################################################################
  Uint32 _iS1 = horizontal_brick_width * horizontal_brick_height;        // size of horizontal bricks
  Uint32 _iS2 = horizontal_brick_width * vertical_brick_height;        // size of vertical bricks
  _iS2 = (_iS1 * MAX_OF_SIDES_BRICKS) + (_iS2 * MAX_OF_SIDES_BRICKS) + (_iS2 * MAX_OF_SIDES_BRICKS);
  fond_sauve = memory->alloc (_iS2);
  error_init (memory->retour_err ());
  if (erreur_num)
    return (erreur_num);

  //###################################################################
  // initialize the sprites of the walls (bob_ground = 1)
  //###################################################################
  bobbg_init ();

  //###################################################################
  //allocate memory for the redraw bricks table
  //###################################################################
  pCoteTable =
    (coteStruct *) memory->alloc (iMAXBRICOT * sizeof (coteStruct),
                                  0x54425243);
  error_init (memory->retour_err ());
  return erreur_num;
}

//-----------------------------------------------------------------------------
// initialize the sprites of the walls (bob_ground = 1)
//-----------------------------------------------------------------------------
void
controller_sides_bricks::bobbg_init ()
{
  if (!bob_ground)
    {
      return;
    }

  Sint32 x = BRICOTEHRX * resolution;
  Sint32 yg = BRICOTEGAY * resolution;
  Sint32 yd = BRICOTEDRY * resolution;
  Sint32 lg = horizontal_brick->get_sprite_width ();
  Sint32 ht = vertical_brick->get_sprite_height ();
  for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)       //12 bricks per wall
    {
      bobwal_top[i] = new sprite_object ();
      bobwal_top[i]->create_sprite (BOB_BRICKH, sprites_bitmap, 1);
      sprites->add (bobwal_top[i]);
      bobwal_top[i]->set_coordinates (x, BRICOTEHRY * resolution);
      if (map_top_wall[i])
        bobwal_top[i]->enable ();

      bobwal_lef[i] = new sprite_object ();
      bobwal_lef[i]->create_sprite (BOB_BRICKV, sprites_bitmap, 1);
      sprites->add (bobwal_lef[i]);
      bobwal_lef[i]->set_coordinates (BRICOTEGAX * resolution, yg);
      if (map_left_wall[i])
        bobwal_lef[i]->enable ();

      bobwal_rgh[i] = new sprite_object ();
      bobwal_rgh[i]->create_sprite (BOB_BRICKV, sprites_bitmap, 1);
      sprites->add (bobwal_rgh[i]);
      bobwal_rgh[i]->set_coordinates (BRICOTEDRX * resolution, yd);
      if (map_right_wall[i])
        bobwal_rgh[i]->enable ();

      x += lg;
      yd += ht;
      yg += ht;
    }
}

//-----------------------------------------------------------------------------
// recopy the state of walls after a level 
//-----------------------------------------------------------------------------
void
controller_sides_bricks::sauve_etat ()
{
  char *bleft = current_player->getBriLeft ();
  char *brigh = current_player->getBriRigh ();
  char *b_top = current_player->getBri_top ();
  for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)
    {
      bleft[i] = map_left_wall[i];
      brigh[i] = map_right_wall[i];
      b_top[i] = map_top_wall[i];
    }
}

//-----------------------------------------------------------------------------
// save background under bricks
//-----------------------------------------------------------------------------
void
controller_sides_bricks::sauveFond ()
{

  //###################################################################
  // wall of the top
  //###################################################################
  Sint32 _iModulo = background_screen->get_line_modulo (horizontal_brick_width);
  Sint32 _iPosX = BRICOTEHRX * resolution;
  char *_pBuffer = fond_sauve;
  char *_pSource;
  for (Uint32 _iIndex = 0; _iIndex < MAX_OF_SIDES_BRICKS; _iIndex++)
    {
      _pSource =
        background_screen->get_pixel_data (_iPosX, BRICOTEHRY * resolution);
      adr_duHaut[_iIndex] = _pBuffer;
      pos_duHaut[_iIndex][0] = _iPosX;
      pos_duHaut[_iIndex][1] = BRICOTEHRY * resolution;
      for (Uint32 _iY = 0; _iY < horizontal_brick_height; _iY++)
        {
          for (Uint32 _iX = 0; _iX < horizontal_brick_width; _iX++)
            *(_pBuffer++) = *(_pSource++);
          _pSource += _iModulo;
        }
      _iPosX += horizontal_brick_width;
    }

  //###################################################################
  // wall of the right
  //###################################################################
  _iModulo = background_screen->get_line_modulo (vertical_brick_width);
  Sint32 _iPosY = BRICOTEGAY * resolution;
  for (Uint32 _iIndex = 0; _iIndex < MAX_OF_SIDES_BRICKS; _iIndex++)
    {
      _pSource =
        background_screen->get_pixel_data (BRICOTEGAX * resolution, _iPosY);
      adr_gauche[_iIndex] = _pBuffer;
      pos_gauche[_iIndex][0] = BRICOTEGAX * resolution;
      pos_gauche[_iIndex][1] = _iPosY;
      for (Uint32 _iY = 0; _iY < vertical_brick_height; _iY++)
        {
          for (Uint32 _iX = 0; _iX < vertical_brick_width; _iX++)
            *(_pBuffer++) = *(_pSource++);
          _pSource += _iModulo;
        }
      _iPosY += vertical_brick_height;
    }

  //###################################################################
  // wall of the left
  //###################################################################
  _iPosY = BRICOTEGAY * resolution;
  for (Uint32 _iIndex = 0; _iIndex < MAX_OF_SIDES_BRICKS; _iIndex++)
    {
      _pSource =
        background_screen->get_pixel_data (BRICOTEDRX * resolution, _iPosY);
      adr_droite[_iIndex] = _pBuffer;
      pos_droite[_iIndex][0] = BRICOTEDRX * resolution;
      pos_droite[_iIndex][1] = _iPosY;
      for (Uint32 _iY = 0; _iY < vertical_brick_height; _iY++)
        {
          for (Uint32 _iX = 0; _iX < vertical_brick_width; _iX++)
            *(_pBuffer++) = *(_pSource++);
          _pSource += _iModulo;
        }
      _iPosY += vertical_brick_height;
    }
}


//-----------------------------------------------------------------------------
// clear bricks
//-----------------------------------------------------------------------------
void
controller_sides_bricks::execution1 ()
{
  if (!bob_ground)
    execution2 ();
  else
    execution3 ();
}

//-----------------------------------------------------------------------------
// restore the background under the brick
//-----------------------------------------------------------------------------
void
controller_sides_bricks::execution2 ()
{
  coteStruct *_pCote = pCoteTable + iCoteRemap;
  Sint32 _iType = _pCote->iCote_type;
  if (_iType)
    {
      iCoteRemap++;
      iCoteRemap &= (iMAXBRICOT - 1);
      _pCote->iCote_type = 0;
      char *_pFond;             //pointer to the background buffer
      Sint32 posx1;
      Sint32 posx2;
      Sint32 _iPosY;
      Sint32 _iIndex = _pCote->iMapOffset;
      Sint32 _iTmod;
      Sint32 _iBmod;
      Sint32 widt1;
      Sint32 widt2;
      Sint32 heig1;
      Sint32 heig2;
      switch (_iType)
        {

          //###########################################################
          // wall of the top
          //###########################################################
        case 1:
          _pFond = adr_duHaut[_iIndex];
          posx1 = pos_duHaut[_iIndex][0];
          posx2 = posx1 + (handler_display::SHADOWOFFX * resolution);
          _iPosY = pos_duHaut[_iIndex][1];
          _iTmod = background_screen->get_line_modulo (horizontal_brick_width);
          _iBmod = game_screen->get_line_modulo (horizontal_brick_width);
          widt1 = horizontal_brick_width;
          widt2 = horizontal_brick_width;
          heig1 = horizontal_brick_height;
          heig2 = horizontal_brick_height;
          if (_iIndex == 0)
            {
              widt2 = widt2 - resolution * 4;
              posx2 = posx2 + resolution * 4;
            }
          break;

          //###########################################################
          // wall of the left
          //###########################################################
        case 2:
          _pFond = adr_gauche[_iIndex];
          posx1 = pos_gauche[_iIndex][0];
          posx2 = posx1 + (handler_display::SHADOWOFFX * resolution);
          _iPosY = pos_gauche[_iIndex][1];
          _iTmod = background_screen->get_line_modulo (vertical_brick_width);
          _iBmod = game_screen->get_line_modulo (vertical_brick_width);
          widt1 = vertical_brick_width;
          widt2 = vertical_brick_width;
          heig1 = vertical_brick_height;
          heig2 = vertical_brick_height;
          break;

          //###########################################################
          // wall of the right
          //###########################################################
        default:
          _pFond = adr_droite[_iIndex];
          posx1 = pos_droite[_iIndex][0];
          posx2 = posx1 + (handler_display::SHADOWOFFX * resolution);
          _iPosY = pos_droite[_iIndex][1];
          _iTmod = background_screen->get_line_modulo (vertical_brick_width);
          _iBmod = game_screen->get_line_modulo (vertical_brick_width);
          widt1 = vertical_brick_width;
          widt2 = vertical_brick_width;
          heig1 = vertical_brick_height;
          heig2 = vertical_brick_height;
          if (_iIndex == MAX_OF_SIDES_BRICKS - 1)
            heig2 = heig2 - resolution * 4;
          break;
        }

      char *_pBuffer = background_screen->get_pixel_data (posx1, _iPosY);
      char *_pTampon = game_screen->get_pixel_data (posx1, _iPosY);

      //###############################################################
      // clear brick
      //###############################################################
      for (Sint32 _iY = 0; _iY < heig1; _iY++)
        {
          for (Sint32 _iX = 0; _iX < widt1; _iX++)
            {
              *(_pBuffer++) = *(_pFond);
              *(_pTampon++) = *(_pFond++);
            }
          _pBuffer += _iBmod;
          _pTampon += _iTmod;
        }

      //###############################################################
      // clear shadow
      //###############################################################
      display->clr_shadow (posx2,
                           _iPosY +
                           (handler_display::SHADOWOFFY * resolution), widt2,
                           heig2);
    }
}

//-----------------------------------------------------------------------------
// disable sprites of wall 
//-----------------------------------------------------------------------------
void
controller_sides_bricks::execution3 ()
{
  for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)
    {
      if (bobwal_top[i]->is_enable () && !map_top_wall[i])
        bobwal_top[i]->disable ();
      if (bobwal_lef[i]->is_enable () && !map_left_wall[i])
        bobwal_lef[i]->disable ();
      if (bobwal_rgh[i]->is_enable () && !map_right_wall[i])
        bobwal_rgh[i]->disable ();

    }
}

//-----------------------------------------------------------------------------
// display bricks shadows (before a bricks level) 
//-----------------------------------------------------------------------------
void
controller_sides_bricks::afficheSha ()
{
  if (bob_ground)
    return;
  Sint32 x = BRICOTEHRX * resolution;
  Sint32 yg = BRICOTEGAY * resolution;
  Sint32 yd = BRICOTEDRY * resolution;
  Sint32 lg = horizontal_brick->get_sprite_width ();
  Sint32 ht = vertical_brick->get_sprite_height ();
  for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)       //12 bricks per wall
    {
      if (map_top_wall[i])
        {
          horizontal_brick->set_coordinates (x, BRICOTEHRY * resolution);
          horizontal_brick->affich_SHA ();
        }
      if (map_left_wall[i])
        {
          vertical_brick->set_coordinates (BRICOTEGAX * resolution, yg);
          vertical_brick->affich_SHA ();
        }
      if (map_right_wall[i])
        {
          vertical_brick->set_coordinates (BRICOTEDRX * resolution, yd);
          vertical_brick->affich_SHA ();
        }
      x += lg;
      yd += ht;
      yg += ht;
    }
}

//-----------------------------------------------------------------------------
// display bricks of the three walls (before a bricks level) 
//-----------------------------------------------------------------------------
void
controller_sides_bricks::afficheGfx ()
{
  if (bob_ground)
    return;
  Sint32 x = BRICOTEHRX * resolution;
  Sint32 yg = BRICOTEGAY * resolution;
  Sint32 yd = BRICOTEDRY * resolution;
  Sint32 lg = horizontal_brick->get_sprite_width ();
  Sint32 ht = vertical_brick->get_sprite_height ();
  for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)       // 12 bricks aside
    {
      if (map_top_wall[i])
        {
          horizontal_brick->set_coordinates (x, BRICOTEHRY * resolution);
          horizontal_brick->affich_MSK ();
        }
      if (map_left_wall[i])
        {
          vertical_brick->set_coordinates (BRICOTEGAX * resolution, yg);
          vertical_brick->affich_MSK ();
        }
      if (map_right_wall[i])
        {
          vertical_brick->set_coordinates (BRICOTEDRX * resolution, yd);
          vertical_brick->affich_MSK ();
        }
      x += lg;
      yd += ht;
      yg += ht;
    }
}

//-----------------------------------------------------------------------------
// return y coordinate of the wall of the top
//-----------------------------------------------------------------------------
Sint32
controller_sides_bricks::getCollisH ()
{
  return collisionH;
}

//-----------------------------------------------------------------------------
// return x coordinate of the wall of the right
//-----------------------------------------------------------------------------
Sint32
controller_sides_bricks::getCollisD ()
{
  return collisionD;
}

//-----------------------------------------------------------------------------
// return x coordinate of the wall of the left
//-----------------------------------------------------------------------------
Sint32
controller_sides_bricks::getCollisG ()
{
  return collisionG;
}

/**
 * Collision with the wall of the left
 * @param ycoord y coordinate of the ball
 * @return true if collision, otherwise false
 */
bool
controller_sides_bricks::collision_with_left_wall (Sint32 ycoord)
{
  if (!is_left_wall_breakable)
    {
      /* because the left wall is unbreakable, there is always collision */
      return true;
    }
  Sint32 index = (ycoord - BRICOTEGAY * resolution) / vertical_brick_height;
  if (!map_left_wall[index]) 
    {
       /* there is no more brick on side which protects */
       return false;
    }
  if (index < 0 || index >= (Sint32) MAX_OF_SIDES_BRICKS)
    {
      return true;
    }
  map_left_wall[index] = false;
  coteStruct *_pCote = pCoteTable + iCoteSauve;
  iCoteSauve++;
  iCoteSauve &= (iMAXBRICOT - 1);
  _pCote->iCote_type = 2;
  _pCote->iMapOffset = index;
  return true;
}

/**
 * Collision with the wall of the right 
 * @param ycoord y coordinate of the ball
 * @return true if collision, otherwise false
 */
bool
controller_sides_bricks::collision_with_right_wall (Sint32 ycoord)
{
  if (!is_right_wall_breakable)
    {
      /* because the right wall is unbreakable, there is always collision */
      return true;
    }
  Sint32 index = (ycoord - BRICOTEDRY * resolution) / vertical_brick_height; 
  if (!map_right_wall[index])
    {
       /* there is no more brick on side which protects */
       return false;
    }
  if (index < 0 || index >= (Sint32) MAX_OF_SIDES_BRICKS)
    {
      return true;
    }
  map_right_wall[index] = false;
  coteStruct *_pCote = pCoteTable + iCoteSauve;
  iCoteSauve++;
  iCoteSauve &= (iMAXBRICOT - 1);
  _pCote->iCote_type = 3;
  _pCote->iMapOffset = index;
  return true;
 }

//-----------------------------------------------------------------------------
// collision with the wall of the top
// output       <= 0: no collision / 1:collision
//-----------------------------------------------------------------------------
bool
controller_sides_bricks::collision_with_top_wall (Sint32 _iPosX)
{
  if (is_top_wall_breakable == 1)
    {
      Sint32 _iIndex =
        (Sint32) ((Sint32) (_iPosX - BRICOTEHRX * resolution) /
                  (Sint32) (horizontal_brick_width));
      if (map_top_wall[_iIndex] != 0)
        {
          if (_iIndex >= 0 && _iIndex < (Sint32) MAX_OF_SIDES_BRICKS)
            {
              map_top_wall[_iIndex] = 0;
              coteStruct *_pCote = pCoteTable + iCoteSauve;
              iCoteSauve++;
              iCoteSauve &= (iMAXBRICOT - 1);
              _pCote->iCote_type = 1;
              _pCote->iMapOffset = _iIndex;
            }
          return 1;
        }
      else
        return 0;               //there is no more brick on side which protects
    }
  else
    return 1;
}
