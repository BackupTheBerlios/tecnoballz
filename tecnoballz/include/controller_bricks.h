/** 
 * @file controller_bricks.h
 * @brief Control the bricks in bricks levels
 * @created 1996-11-13
 * @date 2007-01-30
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.7 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_bricks.h,v 1.7 2007/02/13 17:11:02 gurumeditation Exp $
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
#ifndef __CONTROLLER_BRICKS__
#define __CONTROLLER_BRICKS__
class controller_bricks;
class sprite_paddle;
#include "../include/tecnoballz.h"

//-----------------------------------------------------------------------------
// structure of one simple bricks into current level
//-----------------------------------------------------------------------------
typedef struct
{
  Sint32 brique_rel;            // adresse source relative Gfx de la brique (collision)
  Sint32 brique_aff;            // adresse source relative Gfx de la brique (reaffichage)
  Sint32 brique_num;            // numero de la brique par rapport a la premiere
  Sint32 adresseAff;            // adresse ecran relative d'affichage
  Sint32 briquePosX;            // abscisse de la brique dans la page Gfx brique
  Sint32 briquePosY;            // ordonnee de la brique dans la page Gfx brique
  Sint32 *briqueFond;           // adresse du fond de la brique (modulo 0)
  Sint32 brickcolor;            // color's bricks (17 colors possibles)
}
brickInfos;                     // used into "mega_table"

//-----------------------------------------------------------------------------
// structure for draw brick / restaure background
//-----------------------------------------------------------------------------
typedef struct
{
  Sint32 balle_posX;            // ball screen X-coordinate 
  Sint32 balle_posY;            // ball screen Y-coordinate 
  sprite_paddle *raquettePT;    // pointeur sur la raquette qui a touche cette balle en dernier
  Sint32 brique_num;            // numero de la brique touchee
  Sint32 briqueFlag;            // 1=affiche le decor du fond ou 0=affiche la brique
  Sint32 adresseAff;            // offset d'affichage ecran de la brique
  brickInfos *adresseTab;       // adresse de la brique dans "mega_table"
}
brickClear;
//...............................................................................
#include "../include/sprite_paddle.h"
#include "../include/bitmap_data.h"
#include "../include/right_panel_score.h"
#include "../include/controller_moneys.h"
#include "../include/controller_capsules.h"
#include "../include/sprite_ship.h"
//...............................................................................
class controller_bricks:public objects_list < sprite_object, controller_bricks >
{
  friend class controller_balls;
  friend class sprite_projectile;
  friend class controller_gigablitz;
  friend class sprite_ship;

public:

  static const Sint32 MAXBRIKCLR = 2 << 8;      //maximum number of bricks to erase 
  static const Sint32 NB_BRICKSH = 16;  //numbers of bricks peer line
  static const Sint32 NB_BRICKSV = 30;  //numbers of lines of bricks
  static const Sint32 NB_BRICKST = NB_BRICKSH * (NB_BRICKSV + 8);       //numbers total of bricks

private:
  static const Sint32 BRICKWIDTH = 16;  // brick's width in pixels
  static const Sint32 BRICKHEIGH = 7;   // brick's height in pixels
  static const Sint32 offBri_DD = 1;
  static const Sint32 offBri_GG = -1;
  static const Sint32 offBri_HH = -NB_BRICKSH;
  static const Sint32 offBri_BB = NB_BRICKSH;
  static const Sint32 offBri_BG = NB_BRICKSH - 1;
  static const Sint32 offBri_BD = NB_BRICKSH + 1;
  static const Sint32 offBri_HG = -NB_BRICKSH - 1;
  static const Sint32 offBri_HD = -NB_BRICKSH + 1;
  static const Sint32 LEVELWIDTH = 10;  //width of a simple level
  static const Sint32 LEVELHEIGH = 17;  //height of a simple level
  static const Sint32 LEVEL_SIZE = LEVELWIDTH * LEVELHEIGH;
  static const Sint32 LEVELSAREA = 10;  //number of levels in a area
  static const Sint32 SIZEOFAREA = LEVELSAREA * LEVEL_SIZE;
  static const Sint32 BRKYOFFSET = 8;   //y-offset between 2 bricks 

private:
  /** Bitmap of the set of current bricks */
  bitmap_data *bitmap_bricks;
  right_panel_score *barreObjet;
  controller_moneys *caps_objet;
  controller_capsules *gads_objet;

  /** Number of bricks in the current level */
  Sint32 num_of_bricks;
  /** Less bricks counter */
  Sint32 less_bricks_count;
  /** Time delay for the "less bricks" option */
  Sint32 less_bricks_delay;
  brickInfos *mega_table;       // tableau de 16*30

  /** Width in pixels of a set of bricks */
  Sint32 bricks_height;
  /** Height in pixels of a set of bricks */
  Sint32 bricks_width;
  /** Brick's width in pixels */
  Sint32 brick_width;
  /** Brick's height in pixels */
  Sint32 brick_height;
  /** Brick's size in bytes */
  Sint32 brick_size;
  Sint32 brickIndus;            //first indestructible brick
  Sint32 brkyoffset;            //y-offset between 2 bricks 
  Sint32 ombre_deca;            //size of shadow in pixels (3 or 6)
  Sint32 ombre_left;            //(4 or 8)
  Sint32 ombre_yoff;            //space between 2 bricks (1 or 2)
  Sint32 ombre_top1;            //(2 or 4)

protected:
  brickClear * brique_pnt;    // table de reaffichage 
  Sint32 briqueSave;            // pointeur sur "brique_pnt"
  Sint32 brique_clr;            // pointeur sur "brique_pnt"
  char *brikTampon;             // sauvegarde briques
  Sint32 offsSource;            // adresse de la page brique
  Sint32 offsDestin;            //
  Sint32 *adr_source;           // adresse de la page brique
  Sint32 *adr_desti1;           // adresse du buffer (ecran travail)
  Sint32 *adr_desti2;           // adresse du tampon (ecran restitution) 

public:
    controller_bricks ();
   ~controller_bricks ();
  void first_init (right_panel_score * barre, controller_moneys * capsu,
                     controller_capsules * gadge);
  void initialize (Sint32 area_nu, Sint32 level_nu, Sint32 lbrik);
  Sint32 brickRemap ();
  Sint32 get_num_of_bricks ();
  void less_bricks ();
  void draw_brick (char *srcPT, Sint32 adres, Sint32 colbr);
  void initpalett ();
  void clr_bricks ();


  Sint32 get_brick_width ();
  Sint32 getBkIndus ();
  Sint32 getYOffset ();

private:
  void load_level (Sint32 area_nu, Sint32 level_nu);
  void draw_bricks_shadows ();
  void draw_bricks ();
  void sauve_fond ();           // save background under bricks
};
#endif




/*

NB_BRICKST = 480 briques a l'ecran


 mega_table 
  fonction : collision des balles/tirs avec les 480 briques de l'ecran
  taille   : 7*480 = 3360 octets
  type     : type structure brickInfos 6 mots et 1 pointeur
   brique_rel : adresse relative du bitmap de la brique, sert pour les collisions/resistance de la brique controller_balls::vitusBrick()
   brique_aff : adresse relative du bitmap de la brique
   brique_num : numero de la brique (sert a rien ???)
   adresseAff : adresse ecran relative d'affichage (dans le buffer et le tampon)
   briquePosX : abscisse de la brique dans la page Gfx brique (0,1,2,3,4,5,6,7 ou 8)
   briquePosY : ordonnee de la brique dans la page Gfx brique (0,2,4,6,8,10 ou 12)
   briqueFond : adresse absolue du fond 4 couleurs sous la brique (modulo 0)



 -------------------------------------------------------------------------------
 fichier briques : "zebricks.bmp" 1220 x 254 pixels
 -------------------------------------------------------------------------------
 Ce fichier contient 10 jeux de briques (un jeu de
 brique est choisis au hasard a chaque tableau)
 
 Un jeu de brique :
 Taille : 224x126 pixels : 63 briques
 Le fichier brique contient 7 colonnes de 9 lignes briques de 32x14 pixels 
 Les 8 premieres lignes sont les briques "normales". Chaque ligne est une brique
 avec 7 niveaux de resistance. La colonne 7 represente la plus forte resistance,
 et la colonne 1 la resistance la plus faible. La colonne 0 est vide, la
 brique est detruite.
 La 9ieme ligne contient les briques indestrutibles.


 -------------------------------------------------------------------------------
 fichier tableau : "tableau.data" : 34000 octets
 -------------------------------------------------------------------------------
 Ce fichier contient 100 tableaux. Le jeu comporte 50 tableaux, repartis en
 5 zones de 10 niveaux. Pour un niveau il existe deux tableaux possibles choisi
 au hasard. Les 100 tableaux se suivent sequentiellement dans le fichier :
 les 2 tableaux du niveau 1 - zone 1, les 2 tableaux du niveau 2 - zone 1, ...
 
 Un tableau (340 octets) :
 Se compose de 10 colonnes de 17 lignes. Une brique est represente par un mot 
 de 16 bits.
 Les bits 0 a 7 contiennent la position x sur la page brique de 0 a 12 (de 2 en 2)
 Les bits 8 a 15 contiennent la position y sur la page brique de 0 a  8 (de 1 en 1) 
 Si x et y sont a 0, c'est qu'il n'existe pas de brique.
 
 Le fichier est lu au debut de chaque niveau et les valeurs sont recopiees dans
 la "mega_table" tableau de structure "brickInfos"
 
 
 -------------------------------------------------------------------------------
 tableau "mega_table" : (tableau de structures "brickInfos")
 -------------------------------------------------------------------------------
 Ce tableau est un tableau de type "brickInfos". Chaque entree du tableau
 represente une brique a l'ecran. Le tableau represente 16 colonnes, de
 30 lignes de briques, et couvre donc la totalite de l'ecran de jeu, bien que
 seulement 10 colonnes de 17 lignes soient utilisees, ce qui correspond a la
 taille d'un tableau lu dans le fichier "tableau.data". 
 
 La structure "brickInfos", contient des informations sur une brique a l'ecran
 utilisee pour les collisions avec les balles, les tirs et le gigablitz.
 
 
  brique_rel => adresse source relative de la brique (resistance)
  brique_aff => adresse source relative de la brique 
  brique_num => numero de la brique par rapport a la premiere
  adresseAff => adresse ecran relative d'affichage (buffer et tampon)
  briquePosX => abscisse de la brique dans la page graphique brique 0, 2, 4, 6, 8, 10 ou 12  
  briquePosY => ordonnee de la brique dans la page graphique brique 0, 1, 2, 3, 4, 5, 6, 7, ou 8 
 *briqueFond => adresse du fond de la brique pour l'effacer (modulo 0)


Jeu original :
3 tables de 4 octets par brique


set_table  =>
set_table2 => copie de set_table (effacement)
set_table3 => tableau actuel valeur (collision)

set_table  + 0 = numero brique
set_table  + 2 = adresse relative de la brique dans le fichier graphique brique *collision*
set_table3 + 0 = position x de la brique dans le fichier graphique brique (+JMM) *decremente resitance*
set_table3 + 2 = position y de la brique dans le fichier graphique brique (+JMM)

BRI.X		(2 octets)		Position X de la balle (-1 = brique indes. detruitre / 512 = detruit au gigablitz)
BRI.Y		(2 octets)		Position Y de la balle
BRI.RAK		(4 octets)		Adresse de la structure raquette
BRI.NUM		(2 octets)		Numéro brique
BRI.FLAG	(2 octets)		0=Affiche brique / 1=Affiche décor
BRI.ADR		(2 octets)		Adresse relative d'affichage


RIZ		=BK.X*4		Largeur en octets ligne map-editor

 -------------------------------------------------------------------------------

 -------------------------------------------------------------------------------
typedef struct
{
  Sint32                    balle_posX;                          // abcsisse ecran de la balle
  Sint32                    balle_posY;                          // ordonnee ecran de la balle
  sprite_paddle             *raquettePT;                          // pointeur sur la raquette qui a touche cette balle en dernier
  Sint32                    brique_num;                          // numero de la brique touchee
  Sint32                    briqueFlag;                          // 1=affiche le decor du fond ou 0=affiche la brique
  Sint32                    adresseAff;                          // offset d'affichage ecran de la brique
  brickInfos             *adresseTab;                          // adresse de la brique dans "mega_table"

*/
