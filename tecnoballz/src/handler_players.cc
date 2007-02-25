/** 
 * @file handler_players.cc 
 * @brief players handler 
 * @date 2007-02-14
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.10 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_players.cc,v 1.10 2007/02/25 20:33:37 gurumeditation Exp $
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
#include "../include/handler_players.h"
#include "../include/controller_gems.h"
#include "../include/controller_sides_bricks.h"
#include "../include/controller_gems.h"

Uint32 handler_players::max_of_players = 0;
Sint32 handler_players::joueur_run = 0;
handler_players * handler_players::first_player = NULL;
Sint32 handler_players::best_score = 0;
char handler_players::bestPlayer[6] = { ' ', ' ', ' ', ' ', ' ', ' ' };
handler_players ** handler_players::players_list = NULL;

/* 
 * Create a player object
 */
handler_players::handler_players ()
{
  object_init ();

  /* 
   * add a new player
   */
  
  /* first player */
  if (0 == max_of_players)
    {
      first_player = this;
      next_player = this;
      previous_player = this;
    }
  else
    {
      next_player = first_player;
      handler_players *prev = first_player->get_previous_player ();
      previous_player = prev;
      first_player->set_previous_player (this);
      prev->set_next_player (this);
    }
  max_of_players++;

  /* 
   * clear members members
   */
  player_num = max_of_players;
  reset_members ();
  /* clear name of the player */
  Uint32 i;
  for (i = 0; i < 6; i++)
    {
      player_name[i] = ' ';
    }
  player_name[i] = 0;
}

/* 
 * Release a player object
 */
handler_players::~handler_players ()
{
  max_of_players--;
  if (max_of_players > 0)
    {
      next_player->set_previous_player (previous_player);
      previous_player->set_next_player (next_player);
      if (first_player == this)
        {
          first_player = next_player;
        }
    }
  else
    {
      first_player = NULL;
    }
  object_free ();
}

/**
 * Initialize a player object before a new game
 * @param lifes number of lifes
 * @param area area number (1 to 5)
 * @param level level number in current area (1 to 12)
 * @param money amount of money
 * @param grdPt level_list of the guards
 */
void
handler_players::initialize (Uint32 lifes, Uint32 area, Uint32 level,
                             Uint32 money, Uint32 grdPt)
{
  reset_members ();
  number_of_lifes = lifes;
  area_number = area;
  level_number = level;
  amount_of_money = money;
  guardianPt = grdPt;
}

/**
 * Reset some members values
 */
void
handler_players::reset_members ()
{
  Uint32 z = 0;
  superScore = z;               //reset score of the player
  score_life = 0;
  area_number = 1;
  /* level number into the current area */
  level_number = 1;
  number_of_lifes = initial_num_of_lifes;
  clear_shopping_cart ();
  amount_of_money = 500;
  for (Uint32 i = 0; i < controller_sides_bricks::MAX_OF_SIDES_BRICKS; i++)
    {
      bricotLeft[i] = 1;        //state of the left wall
      bricotRigh[i] = 1;        //state of the right wall
      bricot_top[i] = 1;        //state of the top wall
    }
  /* disable right, top and left paddles */
  right_paddle_alive_counter = 0;
  top_paddle_alive_counter = 0;
  left_paddle_alive_counter = 0;
  rebuild_ok = z;               //reset flag "rebuild wall" option
  less_brick = z;               //reset flag "less brick" option
  life_bonus = z;               //counter of points giving a free life
  /* width of the horizontal paddles
   * and height of the vertical paddles */
  paddle_length = 32 * resolution;
  bonusPrice = z;               //reset flag "bonus price" option
  guardianPt = 0;
  RAZgemlist ();
}

/**
 * Set the player name
 * @param name the name of the player
 */
void
handler_players::set_name (char *name)
{
  for (Uint32 i = 0; i < 6; i++)
    {
      player_name[i] = ' ';
    }
  for (Uint32 i = 0; i < 6; i++)
    {
      char c = name[i];
      if (0 == c)
        {
          return;
        }
      if (c >= 'a' && c <= 'z')
        {
          c = c - ('a' - 'A');
        }
      if ((c >= ' ' && c <= '!') ||
          (c >= '-' && c <= '.') ||
          (c >= '0' && c <= ':') || (c >= 'A' && c <= 'Z') || c == '\'')
        {
          player_name[i] = c;
        }
      else
        {
          player_name[i] = ' ';
        }
    }
}

/**
 * Return the current player name
 * @return the current name the player
 */
char *
handler_players::get_name ()
{
  return &player_name[0];
}

/**
 * Return the area number
 * @raturn area number, from 1 to 5
 */
Uint32
handler_players::get_area_number ()
{
  return area_number;
}

/** 
 * Return the level number
 * @return level number, from 1 to 13
 */
Uint32
handler_players::get_level_number ()
{
  return level_number;
}

/**
 * Return the number of life(s)
 * @return the number of life(s)
 */
Sint32
handler_players::get_num_of_lifes ()
{
  return number_of_lifes;
}

/**
 * return the paddle's length
 * @return the length of the paddle in pixels
 */
Uint32
handler_players::get_paddle_length ()
{
  return paddle_length;
}

/**
 * Initialize paddle's length
 * @param length the length of the paddle in pixels
 */
void
handler_players::set_paddle_length (Uint32 length)
{
  paddle_length = length;
}

/**
 * Return the current amount of money
 * @return the amount of money
 */
Uint32
handler_players::get_money_amount ()
{
  return amount_of_money;
}

/**
 * Decrease the amount of money
 * @param value money amount
 * @return true if the money amount could be descreased 
 */
bool
handler_players::decrease_money_amount (Uint32 value)
{
  if (value > amount_of_money)
    {
      return false;
    }
  amount_of_money -= value;
  return true;
}

/**
 * Increase the amount of money
 * @param value money amount
 */
void
handler_players::increase_money_amount (Uint32 value)
{
  amount_of_money += value;
}

/**
 * Increase the score
 * @param value is the number of points to increase the score by
 */
void
handler_players::add_score (Uint32 value)
{
  superScore += value;
  score_life += value;
  if (score_life > 25000)
    {
      add_life (1);
      score_life -= 25000;
    }
}

/**
 * Clear the shopping cart 
 */
void
handler_players::clear_shopping_cart ()
{
  Sint32 *cart = shopping_cart;
  for (Uint32 i = 0; i < supervisor_shop::MAX_OF_CAPSULES_BOUGHT; i++)
    {
      *(cart++) = 0;
    }
  /* clear the number of items bought */
  shopping_cart_items = 0;
  /* end of the shopping cart */
  *cart = -1;
}

/**
 * Return the list of items bought in th shop
 * @return a pointer to the list of itemps bought in th shop
 */
Sint32 *
handler_players::get_shopping_cart ()
{
  return shopping_cart;
}

/** 
 * Return the number of itemp bought in th shop
 * @return the number of items bought in th shop
 */
Uint32
handler_players::get_cou_nb ()
{
  return shopping_cart_items;
}

//-----------------------------------------------------------------------------
// set number of bonuses bought
//      input   => value: number of bonuses
//-----------------------------------------------------------------------------
void
handler_players::set_cou_nb (Sint32 value)
{
  shopping_cart_items = value;
}

//-----------------------------------------------------------------------------
// Reset the list of gems collected
//-----------------------------------------------------------------------------
void
handler_players::RAZgemlist ()
{
  for (Uint32 i = 0; i < controller_gems::MAX_OF_GEMS; i++)
    {
      /* states of the 6 gems */
      gemmeActif[i] = false;
    }
  gemmeNombr = 0;
}

//-----------------------------------------------------------------------------
// verify if the 6 gemstones are collected
//-----------------------------------------------------------------------------
Sint32
handler_players::gem_enable (Sint32 gemNu)
{
  gemmeActif[gemNu] = true;
  for (Uint32 i = 0; i < controller_gems::MAX_OF_GEMS; i++)
    {
      if (!gemmeActif[i])
    {
        return 0;
    }
    }
  RAZgemlist ();
  return 1;

}

//-----------------------------------------------------------------------------
// return the state of one of six gemstones
//-----------------------------------------------------------------------------
Sint32
handler_players::gem_is_set (Sint32 gemNu)
{
  return gemmeActif[gemNu];
}

//-----------------------------------------------------------------------------
// return the state of one of the bumper
// input        => bumpN: bumper number (2 = right, 3 = top, or 4 left)
// output       <= 0(disable) or > 1 (enable)
//-----------------------------------------------------------------------------
Sint32
handler_players::get_bumpOn (Sint32 bumpN)
{
  switch (bumpN)
    {
    case 2:
      return right_paddle_alive_counter;
      break;
    case 3:
      return top_paddle_alive_counter;
      break;
    default:
      return left_paddle_alive_counter;
      break;
    }
}

//-----------------------------------------------------------------------------
// modify the state of one of the bumper
// input        => bumpN: bumper number (2 = right, 3 = top, or 4 left)
//                      => value: 0 (disable) or > 1 (enable)
//-----------------------------------------------------------------------------
void
handler_players::set_bumpOn (Sint32 bumpN, Sint32 value)
{
  switch (bumpN)
    {
    case 2:
      right_paddle_alive_counter = value;
      break;
    case 3:
      top_paddle_alive_counter = value;
      break;
    case 4:
      left_paddle_alive_counter = value;
      break;
    }
}

/**
 * Initialize less bricks option
 * @param count number of bricks in less
 */
void
handler_players::set_less_bricks (Uint32 count)
{
  less_brick = count;
}

/**
 * Return number of bricks in less
 * @return number of bricks in less
 */
Uint32
handler_players::get_less_bricks ()
{
  return less_brick;
}

//-----------------------------------------------------------------------------
// initialize state "bonus price" option
//-----------------------------------------------------------------------------
void
handler_players::set_Bprice (Sint32 value)
{
  bonusPrice = value;
}

//-----------------------------------------------------------------------------
// return state "bonus price" option
//-----------------------------------------------------------------------------
Sint32
handler_players::get_Bprice ()
{
  return bonusPrice;
}


//-----------------------------------------------------------------------------
// initialize state of "rebuild wall" option
//-----------------------------------------------------------------------------
void
handler_players::setRebuild (Sint32 build)
{
  rebuild_ok = build;
}

//-----------------------------------------------------------------------------
// return state of "rebuild wall" option
//-----------------------------------------------------------------------------
Sint32
handler_players::getRebuild ()
{
  return rebuild_ok;
}

//-----------------------------------------------------------------------------
// return state of the wall of left (small bricks)
//-----------------------------------------------------------------------------
char *
handler_players::getBriLeft ()
{
  return bricotLeft;
}

//-----------------------------------------------------------------------------
// return state of the wall of right (small bricks)
//-----------------------------------------------------------------------------
char *
handler_players::getBriRigh ()
{
  return bricotRigh;
}

//-----------------------------------------------------------------------------
// return state of the wall of top (small bricks)
//-----------------------------------------------------------------------------
char *
handler_players::getBri_top ()
{
  return bricot_top;
}

//-----------------------------------------------------------------------------
// is the lastest level of tecnoballz?
//      output <= 1: end of game :-)
//-----------------------------------------------------------------------------
Sint32
handler_players::zlastlevel ()
{
  if (area_number >= 5 && level_number >= 13)
    return 1;
  else
    return 0;
}

//-----------------------------------------------------------------------------
// next level
//      output <= 1: end of game :-)
//-----------------------------------------------------------------------------
Sint32
handler_players::next_level (Sint32 grdNx)
{
  Sint32 r = 0;
  if (is_verbose)
    printf
      ("handler_players::next_level() area_number=%i, level_number=%i grdNx=%i guardianPt =%i\n",
       area_number, level_number, grdNx, guardianPt);
  if (area_number == 5 && level_number == 13)
    {
      area_number = 1;
      level_number = 1;
      r = 1;                    //end of game
      guardianPt = 0;
    }
  else
    {
      if (area_number == 5 && level_number == 12)
        {
          level_number++;
          guardianPt += grdNx;
        }
      else
        {
          level_number++;
          if (level_number == 13)
            {
              area_number++;
              level_number = 1;
              guardianPt += grdNx;
            }
          if (level_number == 7)
            guardianPt += grdNx;
        }
    }
  if (is_verbose)
    printf
      ("handler_players::next_level() area_number=%i, level_number=%i,  guardianPt=%i\n",
       area_number, level_number, guardianPt);
  return r;
}

/**
 * Return the phase code
 * @return the next phase code GUARDS_LEVEL or SHOP
 */
Uint32
handler_players::get_next_phase ()
{
  /* MAIN_MENU is a very improbable case */
  Uint32 phase = MAIN_MENU;
  /* levels 6, 12 and the level 13 of the area 5 are guardians levels */
  if (level_number == 6 || level_number == 12 || level_number == 13)
    {
      phase = GUARDS_LEVEL;
    }
  else
    {
      /* before a level, there is always the shop,
       * except for the first level of the first area */
      if (level_number > 0 && level_number < 12)
        {
          phase = SHOP;
        }
    }
  return phase;
}

/**
 * Return previous player
 * @return a pointer to the previous player object
 */
handler_players *
handler_players::get_previous_player ()
{
  return previous_player;
}

/**
 * Return the next player
 * @return a pointer to the next player object
 */
/*
handler_players *
handler_players::get_next_player ()
{
  return next_player;
}
*/

/**
 * Set the next player 
 * @param player pointer to a object player
 */
void
handler_players::set_next_player (handler_players * player)
{
  next_player = player;
}

/**
 * Get the previous player
 * @param player pointer to a object player
 */
void
handler_players::set_previous_player (handler_players * player)
{
  previous_player = player;
}

//-----------------------------------------------------------------------------
// get pointer to "level_list" of the guards
//-----------------------------------------------------------------------------
Sint32
handler_players::getGuardPt ()
{
  return guardianPt;
}

//-----------------------------------------------------------------------------
// set pointer to "level_list" of the guards
//-----------------------------------------------------------------------------
void
handler_players::setGuardPt (Sint32 grdPt)
{
  guardianPt = grdPt;
}

/**
 * Add one or more lifes
 * @param add number of lifes to add
 */
void
handler_players::add_life (Uint32 add)
{
  number_of_lifes += add;
}

/**
 * Remove one or more lifes
 * @param add number of lifes to remove 
 */
void
handler_players::remove_life (Uint32 remove)
{
  number_of_lifes -= remove;
  if (number_of_lifes < 0)
    {
      number_of_lifes = 0;
    }
}

/**
 * Remove all lifes, when the game over is forced 
 */
void
handler_players::remove_all_lifes ()
{
  number_of_lifes = 0;
}

/*
 * Statics methods
 */

/**
 * Static method which return next player
 * @param  player: current "handler_players" object
 * @param  rcode: pointer to "end_return"
 * @param  vactu: current phase (1, 2, or 3)
 * @param  grdNx: pointer to "level_list" of the guards (NULL by default)
 * @return the new player object
 */
handler_players *
handler_players::nextplayer (handler_players * player, Sint32 * rcode,
                             Sint32 vactu, Sint32 grdNx)
{
  Uint32 start = player->player_num;
  Uint32 index = start;
  if (vactu != 2)
    {
      player->next_level (grdNx);
    }

  /* process each player object */
  for (Uint32 i = 0; i < max_of_players; i++)
    {
      if (++index > max_of_players)
        {
          index = 1;
        }
      handler_players *player = players_list[index - 1];
      if (player->number_of_lifes > 0)
        {
          /* get next phase: GUARDS_LEVEL or SHOP */
          *rcode = player->get_next_phase ();

          /* this player already went to the shop,
           * he jump to the bricks level */
          if (player->player_num <= start && *rcode == SHOP && vactu == SHOP)
            {
              *rcode = BRICKS_LEVEL;
             } 
          // - 1: bricks levels
          // - 2: shop
          // - 3: guards levels
          // - 4: menu
          if (player->player_num > start && vactu != SHOP)
            {
              if (*rcode == SHOP)
                {
                  *rcode = BRICKS_LEVEL;
                }
            }
          return player;
        }
    }
  *rcode = 4;                   //improbable
  return players_list[0];
}

/**
 * Static method which initializes the maximum number of players
 * @param numof maximum number of players, always 6
 */
handler_players *
handler_players::create_all_players (Uint32 numof)
{

  try
  {
    players_list = new handler_players *[numof];
  }
  catch (std::bad_alloc &)
  {
    std::
      cerr << "(!)handler_players::joueursADD() "
      "not enough memory to allocate " <<
      numof << " list elements!" << std::endl;
    throw;
  }

  /* create the players objects */
  for (Uint32 i = 0; i < numof; i++)
    {
      players_list[i] = new handler_players ();
    }
  return first_player;
}

/**
 * Static method which releases all objects players
 */
void
handler_players::release_all_players ()
{
  for (Uint32 i = 0; i < max_of_players; i++)
    {
      delete first_player;
    }
  if (NULL != players_list)
    {
      delete[]players_list;
      players_list = NULL;
    }
}
