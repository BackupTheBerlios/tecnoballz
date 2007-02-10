/** 
 * @file controller_paddles.h
 * @brief Paddles controller 
 * @date 2007-02-10
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.6 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_paddles.h,v 1.6 2007/02/10 20:22:17 gurumeditation Exp $
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
#ifndef __CONTROLLER_PADDLES__
#define __CONTROLLER_PADDLES__
class controller_paddles;
#include "../include/sprite_paddle.h"
#include "../include/right_panel_score.h"
#include "../include/objects_list.h"
#include "../include/controller_balls.h"
#include "../include/controller_gigablitz.h"

class controller_paddles:public objects_list < sprite_paddle >
{
  friend class sprite_paddle;

private:
  Sint32 width_mini;          //bumper mini width (32 or 64)
  Sint32 width_maxi;            //bumper maxi width (64 or 128)

  Sint32 bumperMini;
  Sint32 bumperMaxi;
  Sint32 bumperYhau;
  Sint32 bumperYbas;
  Sint32 bumperXgau;
  Sint32 bumperXdro;
  list_sprites *objet_draw;
  /** True if the mode team is enable, no implemented 
   * (on Amiga I had two mice simultaneously) */
  bool is_team_mode;
  /** Length of the paddles in pixels */
  Sint32 paddle_length;
  Sint32 raketDepla;            //sens de deplacement 1 <- 0 -> 2
  Sint32 rakVgauche;            //vitesse gauche
  Sint32 rakVdroite;            //vitesse droite
  Sint32 balle_glue;            //temps de la colle
  /** if > 0 the movements of the paddle(s) are reversed */ 
  Uint32 reverse_counter;
  sprite_paddle *paddle_bottom;
  sprite_paddle *paddle_right;
  sprite_paddle *paddle_top;
  sprite_paddle *paddle_left;
  sprite_paddle *tec_robot0;
  controller_gigablitz *gigablitz;
  controller_balls *balls;


  static Sint32 ballePets1[14];
  static Sint32 ballePets2[14];
  static Sint32 ballePets3[14];
  static Sint32 ballePets4[14];

  static const Sint32 rb09[16];
  static const Sint32 rb10[16];
  static const Sint32 rb11[16];
  static const Sint32 rb12[16];
  static const Sint32 rb13[16];
  static const Sint32 rb14[16];
  static const Sint32 rb15[16];
  static const Sint32 rb16[16];
  static const Sint32 rb17[16];
  static const Sint32 *midi1_left[];
  static const Sint32 *midi1Right[];

  static const Sint32 rb18[];
  static const Sint32 rb19[];
  static const Sint32 rb20[];
  static const Sint32 rb21[];
  static const Sint32 rb22[];
  static const Sint32 rb23[];
  static const Sint32 rb24[];
  static const Sint32 rb25[];
  static const Sint32 rb26[];
  static const Sint32 *midi2_left[];
  static const Sint32 *midi2Right[];


  static const Sint32 rb27[];
  static const Sint32 rb28[];
  static const Sint32 rb29[];
  static const Sint32 rb30[];
  static const Sint32 rb31[];
  static const Sint32 rb32[];
  static const Sint32 rb33[];
  static const Sint32 rb34[];
  static const Sint32 rb35[];
  static const Sint32 *midi3_left[];
  static const Sint32 *midi3Right[];

  static const Sint32 rb36[];
  static const Sint32 rb37[];
  static const Sint32 rb38[];
  static const Sint32 rb39[];
  static const Sint32 rb40[];
  static const Sint32 rb41[];
  static const Sint32 rb42[];
  static const Sint32 rb43[];
  static const Sint32 rb44[];
  static const Sint32 *midi4_left[];
  static const Sint32 *midi4Right[];

public:
  typedef enum {
    BOTTOM_PADDLE = 1,
    RIGHT_PADDLE,
    TOP_PADDLE,
    LEFT_PADDLE,
    ROBOT_PADDLE
  }
  ENUM_PADDLES;

public:
    controller_paddles ();
    controller_paddles (Uint32 numof_paddles);
   ~controller_paddles ();
  void create_projectiles_list ();
  void create_paddles_sprites ();
  void fire_projectiles ();
  void check_if_release_balls ();
  void check_if_release_ball ();
  void release_all_balls ();
  void move_paddles ();
  void move_paddle ();
  void init_paddles (controller_gigablitz * blitz, controller_balls * balls);
  void init_robot ();
  void activrobot ();
  void deactrobot ();
  void move_robot ();
  sprite_paddle *get_paddle (Uint32 id);
  void maxi_bumps ();
  void expand_paddles ();
  void shrink_paddles ();
  void set_reverse_counter (Uint32 counter);
  Uint32 get_reverse_counter ();
  void disable_all_paddles ();
};
#endif
