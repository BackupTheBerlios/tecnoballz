*-------------------------------------------------------------------------*
*			DYNABLAST (Version on 31 octobre 1994)		  *
*		Hazardous-Coding By ETB the lemon-squeezer 		  *
*-------------------------------------------------------------------------*
		SECTION	PRESARTIF,CODE
		OPT	C20+		labels et symboles 20 caractères maxi.,  majuscules/minuscules respectées
		OPT	O-		pas d'optimisation
		OPT	p=68020		assemble les instructions MC68020
		INCDIR	DH0:Data/
		INCDIR	DH1:Data/
		INCLUDE	"Bomb/Include/Constantes.s"
		INCLUDE	"Bomb/IncludeCommun/Constantes.s"
		INCLUDE	"Bomb/IncludeCommun/Structures.s"

*>>>>>>>>>>>>>>>>>>>>>>>>>* Debut code MC680x0 *<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	INCLUDE	"DH0:Data/Mentat/Include/mentat.s"
msg.pers	dc.b	"GERE_JOUEURS : plus de structures 'BOMB'   "	¤-128
		dc.b	"BOMB_DETRUITE : erreur dans la carte bombe "	¤-129
		dc.b	"                                           "	¤
		EVEN

************* routine appelée avant de quitter le multitâche **************
sys_extra:	
		lea	(namebob,pc),a0
		lea	(table_couleur,pc),a2
		bsr	charge_page
		bne.s	.exit
		move.l	d0,(bob32_pagegfx-t,a5)

		* Charge la page graphique *
		lea	(namegfx,pc),a0
		lea	(table_couleur,pc),a2
		bsr	charge_page
		bne.s	.exit
		move.l	d0,(pagegfx-t,a5)
		moveq	#0,d7
.exit		rts
namebob:	dc.b	"BOMB:ILBM/PAGEBOB.ILBM",0
namegfx:	dc.b	"BOMB:ILBM/PAGEGFX.ILBM",0


		EVEN
sys_extra2:	rts

*>>>>>>>>>>>>>>>>>>>>>>>>>* début du programme *<<<<<<<<<<<<<<<<<<<<<<<<<<*
* Entrée	a5.l	=> adr. 't'
*		a6.l	=> CUSTOM
run:		bsr	first_init
		bsr	sprites_off
		bsr	first_init2
		bne	sys_erreur
		bsr	menu
		bsr	init_players
		bsr	install_carte
		bne	sys_erreur


*¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤* BOUCLE PRINCIPALE HORS INTERRUPTION *¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤*
main_loop:	
		bsr	flip_ecrans
		bsr	wait_vertical_blank
		RASTER	#$00F
		bsr	bob32_restore		bleu 	=> EFFACE LES BOBS
		RASTER	#$F0F
		bsr	restore_flamme		violet 	=> EFFACE LES FLAMMES
		RASTER	#$0F0
		bsr	reaffiche_dalles
		bsr	anim_desagrege		vert
		RASTER	#$FFF
		bsr	prepare_flamme		=> PREPARE AFF. FLAMMES
		bsr	anim_flamme		blanc	=> AFFICHE FLAMMES
		RASTER	#$FF0
		bsr	anim_bomb		jaune	=> ANIM. BOMBES
		RASTER	#$00F
		bsr	bob32_affiche		bleu	=> AFFICHE LES BOBS
		RASTER	#$F00
		bsr	gere_joueurs		rouge	=> GESTION JOUEURS
		RASTER	#$000

		move.l	(atable_clavier-t,a5),a1
		tst.b	(TOUCHE_DEL,a1)
		beq.s	.okgirl
		bsr	affiche_maps
.okgirl
		btst	#6,$bfe001
		bne	main_loop
		bsr	init_players
		bsr	install_carte
		bra	main_loop

*¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤*


*=========================================================================*
******** Routine appelée par l'interruption du temps mort vertical ********
*=========================================================================*
* Entrée	a5.l	=> Adr. 't'
*		a6.l	=> CUSTOM
extra_vbl:
		bsr	vblkeys
		rts
		
*===================* Test de touches pendant la VBL *====================*
* Entrée	=> a5.l	table 't'
vblkeys:	
		move.l	(atable_clavier-t,a5),a1
		lea	(TOUCHE_HELP,a1),a1
		bsr	test_clavier
		tst	d0
		beq.s	.no
		not.b	(raster.f-t,a5)
.no		rts






*¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤*
********* Attend une nouvelle interruption du temps mort vertical *********
*¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤*
* Entrée	=> a5.l	Adr. 't'
wait_vertical_blank:
		move.w	(vbl_counter-t,a5),d0	d0=n° dernière VBL
.wait_vbl	add.w	d0,(hasard-t,a5)
		cmp.w	(vbl_counter-t,a5),d0	Nouvelle VBL passée ?
		beq.s	.wait_vbl		-> Non
		rts

*=========================================================================*
*/\/\/\/\/\/\/\/\/\/\/\ Includes des sources du jeu /\/\/\/\/\/\/\/\/\/\/\*
*=========================================================================*
		include	"bomb/include/first_init2.s"
		include	"bomb/include/menu.s"
		include	"bomb/include/init_players.s"
		include	"bomb/includeCommun/install_carte.s"
		include	"bomb/include/affiche_maps.s"
		include	"bomb/include/gere_joueurs.s"
		include	"bomb/include/init_flammes.s"
		include	"bomb/include/restore_flamme.s"
		include	"bomb/include/prepare_flamme.s"
		include	"bomb/include/anim_flamme.s"
		include	"bomb/include/anim_bomb.s"
		include	"bomb/include/dalle_detruite.s"
		include	"bomb/include/anim_desagrege.s"
		include	"bomb/include/reaffiche_dalles.s"

*:*:*:*:*:*:*:*:*:*:* Liste dalles qui se désagrégent *:*:*:*:*:*:*:*:*:*:*
		CNOP	0,4
desa_list:	ds.b	DESA_SIZE*BOMB_NBRE*4


*=========================================================================*
*>>>>>>>>>>>>>>>>>* Adresses utilisées par le programme *<<<<<<<<<<<<<<<<<*
*=========================================================================*
		CNOP	0,4
bomb_list_pt	dc.l	0
degrade_copper	dc.l	0		adresse du dégradé copper
pagegfx		dc.l	0		page graphique
page_gfx	dc.l	0		page graphique+PG_DATA
ecran:		dc.l	0	^	adresse=0
ecran1		dc.l	0	^	adr. écran 1 (double-buffering)
ecran2		dc.l	0	^	adr. écran 2 (double-buffering)
ecran3		dc.l	0	!	adr. écran de restauration
modulo1		dc.l	0		mod. copie 16x16 pagegfx->ecran
nextline_char	dc.l	0		offset 2ieme ligne police 16x20
window1		dc.w	0		fen. copie 16x16 pagegfx->ecran
window_char	dc.w	0		fen. copie 16x20 (caractère)
ecranw		dc.l	0		largeur écran en octets
ecrans		dc.l	0		taille d'une ligne en octets
pagegfx_char	dc.l	0		adr. police 16x20 dans "pagegfx"
offset16:	dc.l	0		offset de 16 lignes écran
ecrand		dc.w	0		profondeur écran

		INCLUDE	"Bomb/IncludeCommun/Donnee.s"

		* flags *
raster.f	dc.b	0		0=pas de raster-lines


* buffer flammes horizontales & buffer flammes verticales *
		CNOP	0,4
flamv_splan	dc.w	0		largeur d'un plan et d'une ligne
flamh_splan	dc.w	0		largeur d'un plan et d'une ligne
flamv_sline	dc.l	0		taille d'une ligne
flamh_sline	dc.l	0		taille d'une ligne
flamh_s16lines	dc.l	0		taille de 16 lignes
flamv_s16lines	dc.l	0		taille de 16 lignes
flamvb_adr	dc.l	0		adr. des flammes verticales basses
flamvh_adr	dc.l	0		adr. des flammes verticales hautes
flamhr_adr	dc.l	0		adr. des flammes horizontales droites
flamhl_adr	dc.l	0		adr. des flammes horizontales gauches


****************************** Liste joueurs ******************************
		CNOP	0,4
joueur1:	ds.b	J_TAIL
		CNOP	0,4
joueur2		ds.b	J_TAIL
		CNOP	0,4
joueur3		ds.b	J_TAIL
		CNOP	0,4
joueur4		ds.b	J_TAIL
		CNOP	0,4
joueur5		ds.b	J_TAIL
J_TAILLE	=joueur2-joueur1

* positions initiales des joueurs *
init_pos:	dc.w	J1_X,J1_Y
		dc.w	J2_X,J2_Y
		dc.w	J3_X,J3_Y
		dc.w	J4_X,J4_Y
		dc.w	J5_X,J5_Y

* routine de déplacement *
init_ctrl:	dc.l	ctrl_joystick1-t
		dc.l	ctrl_joystick0-t
		dc.l	ctrl_clavier1-t
		dc.l	ctrl_clavier3-t
		dc.l	ctrl_clavier2-t

* code bombe map *
init_code_bombe	dc	MAP.BOMBJ1,MAP.BOMBJ2,MAP.BOMBJ3,MAP.BOMBJ4,MAP.BOMBJ5


****** Listes utilisées pour réaffichage des dalles sur les 3 écrans ******
		CNOP	0,4
dalle_list:	ds.b	DALLE_SIZE*DALLE_NBRE
dalle_list_end

************** Listes utilisées pour les bombes & explosions **************
		CNOP	0,4
bomb_list:	ds.b	BOMB_SIZE*BOMB_NBRE
bomb_list_end

************************ Textes de la police 16x20 ************************
chaine		dc.b	'[NUMBER[OF[PLAYERSf',0
		dc.b	'[[[F][[[^[PLAYERS',0
		dc.b	'[[[F^[[[_[PLAYERS',0
		dc.b	'[[[F_[[[`[PLAYERS',0
		dc.b	'[[[F`[[[a[PLAYERS',0
		EVEN

		INCLUDE	"Mentat/Include/Systeme/t.s"

******************* Table des adresses des dalles du jeu ******************
		INCLUDE	"Bomb/IncludeCommun/table_dalle.s"

*** Table
exploflamme:	dc D_EXXPLO0,D_EXXPLO1,D_EXXPLO2,D_EXXPLO3,D_EXXPLO4,D_EXXPLO5,D_EXXPLO6
explof2		dc D_EXXPLO0b,D_EXXPLO1b,D_EXXPLO2b,D_EXXPLO3b,D_EXXPLO4b,D_EXXPLO5b,D_EXXPLO6b
		dc D_EXXPLO0c,D_EXXPLO1c,D_EXXPLO2c,D_EXXPLO3c,D_EXXPLO4c,D_EXXPLO5c,D_EXXPLO6c
		dc D_EXXPLO0d,D_EXXPLO1d,D_EXXPLO2d,D_EXXPLO3d,D_EXXPLO4d,D_EXXPLO5d,D_EXXPLO6d
		dc D_EXXPLO0c,D_EXXPLO1c,D_EXXPLO2c,D_EXXPLO3c,D_EXXPLO4c,D_EXXPLO5c,D_EXXPLO6c
		dc D_EXXPLO0b,D_EXXPLO1b,D_EXXPLO2b,D_EXXPLO3b,D_EXXPLO4b,D_EXXPLO5b,D_EXXPLO6b
		dc D_EXXPLO0,D_EXXPLO1,D_EXXPLO2,D_EXXPLO3,D_EXXPLO4,D_EXXPLO5,D_EXXPLO6

********************* Tables des 256 couleurs 32 bits *********************
table_couleur	ds.l	256			table des 256 couleurs

***************************** TABLE DES BOBS ******************************
		INCLUDE	"Bomb/Include/Donnees/tablebobs.s"

***************** Table des 256 couleurs 32 bits du fond ******************
		CNOP	0,4
restore_copper	ds.l	256

**************** Table sinus déplacement des barres Copper ****************
sinus_table	incbin	"BOMB/table/sinus.table"

*---------------* position Y des barres & pt/table sinus *----------------*
posy_barre:		dc	0,00
		dc	0,8*1
		dc	0,8*2
		dc	0,8*3
		dc	0,8*4
		dc	0,8*5
		dc	0,8*6
		dc	0,8*7
		dc	0,8*8
		dc	0,8*9

*---------------* Valeurs 32 bits des barres coppers menu *---------------*
* convertit au format AGA en first_init2
		CNOP	0,4
barre_copper:		
 dc.l $600000,$700000,$800000,$900000,$a00000,$b00000,$a00000,$900000,$800000,$700000,$600000
 dc.l $660000,$770000,$880000,$990000,$aa0000,$bb0000,$aa0000,$990000,$880000,$770000,$660000
 dc.l $700000,$800000,$900000,$a00000,$b00000,$c00000,$b00000,$a00000,$990000,$800000,$700000
 dc.l $770000,$880000,$990000,$aa0000,$bb0000,$cc0000,$bb0000,$aa0000,$990000,$880000,$770000
 dc.l $800000,$900000,$a00000,$b00000,$c00000,$d00000,$c00000,$b00000,$aa0000,$900000,$800000
 dc.l $880000,$990000,$aa0000,$bb0000,$cc0000,$dd0000,$cc0000,$bb0000,$aa0000,$990000,$880000
 dc.l $900000,$a00000,$b00000,$c00000,$d00000,$e00000,$d00000,$c00000,$bb0000,$a00000,$900000
 dc.l $990000,$aa0000,$bb0000,$cc0000,$dd0000,$ee0000,$dd0000,$cc0000,$bb0000,$aa0000,$990000
 dc.l $a00000,$b00000,$c00000,$d00000,$e00000,$f00000,$e00000,$d00000,$c00000,$b00000,$a00000
 dc.l $aa0000,$bb0000,$cc0000,$dd0000,$ee0000,$ff0000,$ee0000,$dd0000,$cc0000,$bb0000,$aa0000


fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
		dc.b	0,"$VER : DynaBlast 0.15 (31.10.94)",0
		EVEN

*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
* [ espace
* \ 0
* ] 1
* ^ 2
* _ 3 
* ` 4
* a 5
* b 6
* c 7
* d 8
* e 9
* f ! 
* g .
* h '
