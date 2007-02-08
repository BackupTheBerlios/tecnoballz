/** 
 * @file handler_players.h
 * @brief players handler 
 * @date 2007-02-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.4 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_players.h,v 1.4 2007/02/08 07:33:07 gurumeditation Exp $
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
  friend class barreScore;
  friend class supervisor_guards_level;
  friend class supervisor_bricks_level;
  friend class printmoney;
  friend class scoretable;

public:
  static handler_players **playerlist;

private:
  static Sint32 totalActif;     // Nombre de joueurs actifs
  static Sint32 joueur_run;     // Numero du joueur en cours
  static handler_players *player_one;
  static Sint32 best_score;
  static char bestPlayer[6];

  handler_players *playerNext;  // Pointeur sur le joueur suivant
  handler_players *playerPrev;  // Pointeur sur le joueur precedent
  /** Name of the player */
  char player_name[7];
  Sint32 player_num;

  Sint32 superScore;            // Score accumule
  Sint32 score_life;            // Score pour une vie
  Sint32 areaNumber;            // Numero de l'Area
  Sint32 levelTecno;            // Niveau dans l'Area
  Sint32 superLifes;            // Nombre de vies
  Sint32 activation;            // Flag joueur actif
  Sint32 courseList[NB_OPTIONS + 1];    // Options achetes au magasin
  Sint32 courseNmbr;            // Nombre d'options achetee(s)
  Sint32 creditFric;            // Credit argent
  char bricotLeft[12];          // Etat du mur de gauche
  char bricotRigh[12];          // Etat du mur de droite
  char bricot_top[12];          // Etat du mur dy haut
  Sint32 bump2Actif;            // Etat raquette de droite
  Sint32 bump3Actif;            // Etat raquette du haut
  Sint32 bump4Actif;            // Etat raquette de droite
  Sint32 rebuild_ok;            // Flag reconstruit les murs
  Sint32 less_brick;            // Flag "less brick"
  Sint32 life_bonus;            // Compteur point une vie gratuite
  Sint32 paddle_length;         // Largeur des raquettes
  Sint32 bonusPrice;            // Bonus prix du magasin a 1
  char gemmeActif[6];           // Etat des 6 gemmes
  Sint32 gemmeNombr;            // Nombre de gemme(s) ramassee(s)
  Sint32 guardianPt;            //pt / level_list of the guards

private:
    handler_players ();
   ~handler_players ();
public:
  void initialise (Sint32 lifes, Sint32 areaN,
                   Sint32 level, Sint32 monay, Sint32 grdPt);
  void set_name (char *playername);
  char *returnName ();
  Uint32 get_area_number ();
  Uint32 get_level_number ();
  Sint32 get_num_of_lifes ();
  Uint32 get_paddle_width ();
  void setLargeur (Sint32 largeur);
  Sint32 get_credit ();
  Sint32 sub_credit (Sint32 value);
  void add_credit (Sint32 value);

  void add_scores (Sint32 value);
  void RAZ_course ();
  Sint32 *get_course ();
  Sint32 get_cou_nb ();
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
  Sint32 level2jump ();

  void lifes_plus (Sint32 ajout);
  Sint32 lifesMoins (Sint32 retra);
  void lifesReset ();

  Sint32 getGuardPt ();
  void setGuardPt (Sint32 grdPt);

  static handler_players *firstGamer ();
  static void joueursRAZ ();
  static handler_players *init_numof_players (Uint32 numof);
  static handler_players *nextplayer (handler_players *, Sint32 *, Sint32,
                                      Sint32 grdNx = 0);


private:
    handler_players * prevPlayer ();
  handler_players *nextPlayer ();
  void nextPlayer (handler_players * gamer);
  void prevPlayer (handler_players * gamer);
  void reset_members ();


};
#endif
