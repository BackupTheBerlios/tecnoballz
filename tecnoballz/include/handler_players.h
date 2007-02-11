/** 
 * @file handler_players.h
 * @brief players handler 
 * @date 2007-02-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.8 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_players.h,v 1.8 2007/02/11 21:03:24 gurumeditation Exp $
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
#ifndef __HANDLER_PLAYERS__
#define __HANDLER_PLAYERS__
class handler_players;


#include "../include/tecnoballz.h"
#include "../include/controller_capsules.h"

class handler_players:public virtual tecnoballz
{
  friend class right_panel_score;
  friend class supervisor_guards_level;
  friend class supervisor_bricks_level;
  friend class controller_indicators;
  friend class scoretable;

public:
  /** List of all players object */
  static handler_players **players_list;

private:
  /** Maximum number of playes */
  static Uint32 max_of_players;
  static Sint32 joueur_run;     // Numero du joueur en cours
  static handler_players *first_player;
  static Sint32 best_score;
  static char bestPlayer[6];
  
  /** Pointer to the next player object */
  handler_players *next_player;
  /** Pointer to the previsous player object */
  handler_players *previous_player;
  /** Name of the player */
  char player_name[7];
  /** Player number from 1 to 6 */
  Uint32 player_num;

  Sint32 superScore;            // Score accumule
  Sint32 score_life;            // Score pour une vie
  /** Area number from 1 to 5 */
  Uint32 area_number;
  Uint32 level_number;            // Niveau dans l'Area
  /** Number of lifes remaining */
  Sint32 number_of_lifes;
  /** List ot the items bought in the shop */
  Sint32 shopping_cart[NB_OPTIONS + 1];
  /** The number of items in the shopping cart */
  Uint32 shopping_cart_items;
  /** Amount of money available to be spend in the shop */
  Uint32 amount_of_money;
  char bricotLeft[12];          // Etat du mur de gauche
  char bricotRigh[12];          // Etat du mur de droite
  char bricot_top[12];          // Etat du mur dy haut
  /** Alive counter of the right paddle, 0 = paddle disabled */ 
  Uint32 right_paddle_alive_counter;
  /** Alive counter of the top paddle, 0 = paddle disabled */ 
  Uint32 top_paddle_alive_counter;
  /** Alive counter of the left paddle, 0 = paddle disabled */ 
  Uint32 left_paddle_alive_counter;
  Sint32 rebuild_ok;            // Flag reconstruit les murs
  Sint32 less_brick;            // Flag "less brick"
  Sint32 life_bonus;            // Compteur point une vie gratuite
  /** Width of the horizontal paddles
   * and height of the vertical paddles */
  Uint32 paddle_length;
  Sint32 bonusPrice;            // Bonus prix du magasin a 1
  bool gemmeActif[6];           // Etat des 6 gemmes
  Sint32 gemmeNombr;            // Nombre de gemme(s) ramassee(s)
  Sint32 guardianPt;            //pt / level_list of the guards

private:
    handler_players ();
   ~handler_players ();
public:
  void initialize (Uint32 lifes, Uint32 areaN,
                   Uint32 level, Uint32 monay, Uint32 grdPt);
  void set_name (char *playername);
  char *get_name ();
  Uint32 get_area_number ();
  Uint32 get_level_number ();
  Sint32 get_num_of_lifes ();
  Uint32 get_paddle_length ();
  void set_paddle_length (Uint32 length);
  Uint32 get_money_amount ();
  bool decrease_money_amount (Uint32 value);
  void increase_money_amount (Uint32 value);

  void add_score (Uint32 value);
  void clear_shopping_cart ();
  Sint32 *get_shopping_cart ();
  Uint32 get_cou_nb ();
  void set_cou_nb (Sint32 nombr);

  void RAZgemlist ();
  Sint32 gem_enable (Sint32 gemNu);
  Sint32 gem_is_set (Sint32 gemNu);

  Sint32 get_bumpOn (Sint32 bumpN);
  void set_bumpOn (Sint32 bumpN, Sint32 value);
  void set_lessBk (Sint32 value);
  Sint32 get_lessBk ();
  void setRebuild (Sint32 build);
  Sint32 getRebuild ();
  void set_Bprice (Sint32 value);
  Sint32 get_Bprice ();
  char *getBriLeft ();
  char *getBriRigh ();
  char *getBri_top ();

  Sint32 zlastlevel ();
  Sint32 next_level (Sint32 grdNx = 0);
  Uint32 get_next_phase ();

  void add_life (Uint32 add);
  void remove_life (Uint32 remove);
  void remove_all_lifes ();

  Sint32 getGuardPt ();
  void setGuardPt (Sint32 grdPt);

  static void release_all_players ();
  static handler_players *create_all_players (Uint32 numof);
  static handler_players *nextplayer (handler_players *, Sint32 *, Sint32,
                                      Sint32 grdNx = 0);


private:
  handler_players *get_previous_player ();
  handler_players *get_next_player ();
  void set_next_player (handler_players *player);
  void set_previous_player (handler_players *player);
  void reset_members ();


};
#endif
