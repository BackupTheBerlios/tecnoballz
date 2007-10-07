
**>>**>>**>>**>>**>>**>>**>>**>>**>¤<**<<**<<**<<**<<**<<**<<**<<**<<**<<**
*>>****************> VBL : Test clavier de l'éditeur <*****************<<**
**>>**>>**>>**>>**>>**>>**>>**>>**>¤<**<<**<<**<<**<<**<<**<<**<<**<<**<<**
* Entrée	=> a5.l	Adr. 't' 
vbl.editor:
		*/ Test boutons souris */
		lea	(viking.left-t,a5),a2
		lea	(viki0-t,a5),a1
		lea	(mouse0_left-t,a5),a0
		bsr	jerry
		lea	(viking.right-t,a5),a2
		lea	(viki1-t,a5),a1
		lea	(mouse0_right-t,a5),a0
		bsr	jerry

		*/ Touches curseurs */
		lea	test_clavier,a2
		move.l	(atable_clavier-t,a5),a0
		lea	(kx1-t,a5),a3
		lea	(TOUCHE_GAUCHE,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+
		lea	(TOUCHE_DROITE,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+
		lea	(TOUCHE_HAUT,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+
		lea	(TOUCHE_BAS,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+

		*/ Touches curseurs pavé-numérique */
		lea	(kx2-t,a5),a3
		lea	(TOUCHE_NUM_2,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+
		lea	(TOUCHE_NUM_8,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+
		lea	(TOUCHE_NUM_4,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+
		lea	(TOUCHE_NUM_6,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+

		*/ Touche +/- tableaux */
		lea	(kx3-t,a5),a3
		lea	(TOUCHE_3,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+
		lea	(TOUCHE_4,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+

		*/ Touche Change résistance des briques */
		lea	(kx4-t,a5),a3
		lea	(TOUCHE_1,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+
		lea	(TOUCHE_2,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+

		*/ Touche Change jeu de briques */
		lea	(kx5-t,a5),a3
		lea	(TOUCHE_5,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+
		lea	(TOUCHE_6,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+

		*/ Touche Change couleur briques */
		lea	(kx8-t,a5),a3
		lea	(TOUCHE_7,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+
		lea	(TOUCHE_8,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+

		*/ Touche Change le fond */
		lea	(kx6-t,a5),a3
		lea	(TOUCHE_NUM_CROG,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+
		lea	(TOUCHE_NUM_CROD,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+

		*/ Touche Change couleur fond */
		lea	(kx7-t,a5),a3
		lea	(TOUCHE_NUM_SLAS,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+
		lea	(TOUCHE_NUM_ETOI,a0),a1
		jsr	(a2)
		move.b	d0,(a3)+

		*/ Quit */
		bsr	transcefusion		|-> Touche AmigaLeft ?
		beq.s	.exit			-> non
		tst.b	(TOUCHE_Q,a0)
		beq.s	.no_q
		move.b	#1,(flag.quit-t,a5)
.no_q		bsr	mushrooms		|-> Memory Info
.exit		rts

*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*
*°.°.°.°.°.°.°.°.°.°.°.°.° Test clavier du menu °.°.°.°.°.°.°.°.°.°.°.°.°.*
*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*
vbl_clavier4:
		IFEQ	A_INTRO
		move.l	(viking.menu-t,a5),a4
		move.l	(atable_clavier-t,a5),a0
		lea	(viking.left-t,a5),a2
		lea	(viki0-t,a5),a1
		lea	(mouse0_left-t,a5),a0
		bsr	jerry
		lea	(viking.right-t,a5),a2
		lea	(viki1-t,a5),a1
		lea	(mouse0_right-t,a5),a0
		bsr	jerry

		*/ Entre chaine */
.charinput	tst.l	(viking.input-t,a5)
		beq.s	.ex
		move.w	(viking.lenght-t,a5),d7
		move.l	(viking.input-t,a5),a4
		move.b	(TOUCHE_CAPSLOCK,a0),d5
		move.b	#1,(TOUCHE_CAPSLOCK,a0)
		bsr.l	input
		move.l	(atable_clavier-t,a5),a0
		move.b	d5,(TOUCHE_CAPSLOCK,a0)
		tst.l	d0			Touche RETURN=FIN
		beq.s	.dd			-> non
		bsr	hyper1
		clr.l	(viking.input-t,a5)
		clr.w	(viking.curs-t,a5)
		bra.s	.exit
.dd		move.w	d6,(viking.curs-t,a5)
.ex		bsr	transcefusion		|-> Touche AmigaLeft ?
		beq.s	.exit			-> non
		tst.w	(sh.code-t,a5)
		beq.s	.exit
		bsr	mushrooms		|-> Memory Info
		ENDC
.exit		rts

*-------------------- Gestion pression bouton souris ---------------------*
* entrée	=> a0.l	Flag bouton souris
*		=> a1.l Flag temps pression
*		=> a2.l	Flag final
jerry:	
		moveq	#0,d0
		tst.b	(a0)			Bouton souris pressé?
		beq	.clear0			-> non
		tst.w	(a1)			Première pression ?
		bne.s	.go0			-> non
		move.w	#1,(a1)			oui
.go0		moveq	#1,d0			d0=1=touche prise en compte
		cmp.w	(a1),d0			1ere pression ?
		beq.s	.ok_press		-> oui, pris en compte
		cmp.w	#TC1,(a1)		infer. delai de répétition ?
		blt.s	.no_press		-> oui, pas pris en compte
		beq.s	.ok_press		-> égal, pris en compte
		cmp.w	#TC2,(a1)		fin compteur de répétion ?
		bne.s	.no_press		-> non
		move.w	#TC1-1,(a1)
.no_press	add.w	d0,(a1)			inc. compteur
.no_flag	moveq	#0,d0			d0=0=touche non prise en compte
		bra.s	.alors
.ok_press	add.w	d0,(a1)			inc. compteur
		bra.s	.alors
.clear0		move.w	d0,(a1)
.alors		move.b	d0,(a2)
		rts

*-------------------------------------------------------------------------*
*»»»»»»»»»»»»»»»» TEST CLAVIER AVEC TEST MEMOIRE ONLY ««««««««««««««««««««*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
vbl_clavier3:	
		bsr	transcefusion		|-> Touche AmigaLeft ?
		beq.s	.exit			-> non
		tst.b	(TOUCHE_Q,a0)
		beq.s	.no_q
		move.b	#1,(flag.quit-t,a5)
.no_q		tst.w	(sh.code-t,a5)
		beq.s	.exit
		bsr	mushrooms		|-> Memory Info
.exit		rts

*-------------------------------------------------------------------------*
*»»»»»»»»»»»»»»»» TEST CLAVIER PENDANT LA VBL GARDIEN ««««««««««««««««««««*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
vbl_clavier2:	
		bsr	transcefusion		|-> Touche AmigaLeft ?
		beq.s	.exit			-> non
		bsr	lupvsunge		|-> Pause/Exit/Musique/Sound
		cmp.l	#ETB,(triche-t,a5)
		bne	.exit
		bsr	mushrooms		|-> Memory Info
		tst.b	(TOUCHE_SHIFT_D,a0)	touche SHIFT-DROITE ?
		beq	.exit

		*/ TOUCHE F10 => VA AU WINNER */
		tst.b	(TOUCHE_F10,a0)
		beq.s	.no222x
		move.w	#GARD_FF,(gd.pt-t,a5)	pt/table gardien final area 5
		move.w	#20,(tecnowinner-t,a5)
		move.w	#1,(gameover-t,a5)
		bra.s	.la
.no222x
		*/ TOUCHE 2 = EXPLOSE GARDIENS */
		tst.b	(TOUCHE_2,a0)
		beq.s	.no222
.la		move.l	(gd1-t,a5),a1
		clr.w	(GARD.POWER,a1)
		move.w	#99,(GARD.EXPTIME,a1)
		clr.w	(GARD.PTEXP,a1)
		move.l	(GARD.BOB,a1),a1
		clr.l	(SBOB.ACTIF,a1)
		tst.l	(gd2-t,a5)
		beq.s	.no222
		move.l	(gd2-t,a5),a1
		clr.w	(GARD.POWER,a1)
		move.w	#55,(GARD.EXPTIME,a1)
		clr.w	(GARD.PTEXP,a1)
		move.l	(GARD.BOB,a1),a1
		clr.l	(SBOB.ACTIF,a1)
.no222
.exit		rts

*-------------------------------------------------------------------------*
*»»»»»»»»»»»»»»»»»» TEST CLAVIER PENDANT LA VBL TABLEAUX «««««««««««««««««*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
vbl_clavier:	
		bsr	transcefusion		|-> Touche AmigaLeft ?
		beq	.exit			-> non
		bsr	lupvsunge		|-> Pause/Exit/Musique/Sound
		cmp.l	#ETB,(triche-t,a5)
		bne	.exit
		bsr	mushrooms		|-> Memory Info / Raster
		*/ GADGET */
		move.w	#TOUCHE_DOLLAR,d4
.loopaaaazzaz	lea	(a0,d4),a1
		jsr	(a2)
		beq.s	.no_gadget
		move.w	d4,(gadget.triche-t,a5)
.no_gadget	dbf	d4,.loopaaaazzaz
.exit		rts

*----------------Test si la Touche AmigaGauche est préssée----------------*
* Entrée	=> a5.l Adr. 't'
* Sortie	<= a0.l 'table_clavier'
*		<= a2.l test_clavier
*		<= Z=0  Touche AmigaGauche préssée 
transcefusion:
		move.l	(atable_clavier-t,a5),a0
		lea	test_clavier,a2
		tst.b	(TOUCHE_AMIGA_D,a0)	touche AMIGA-DROITE ?
		rts

*---------------------*  Touche touches autorisées  *---------------------*
* Entrée	=> a0.l 'table_clavier'
*		=> a2.l test_clavier
* 		=> a5.l Adr. 't'
lupvsunge:
		*/ Touche GameOver */
		tst.b	(TOUCHE_X,a0)
		beq.s	.nox399
		tst.w	(gameover-t,a5)
		bne.s	.nox399
		move.w	#1,(gameover-t,a5)
		clr.l	(life0-t,a5)
.nox399
		*/ Pause on/off */
		lea	(TOUCHE_P,a0),a1
		jsr	(a2)
		beq.s	.no_p
		not.b	(flag_pause-t,a5)
.no_p
		*/ Son + Musique */
		tst.b	(TOUCHE_S,a0)
		beq.s	.no_s
		clr.w	(music.flag-t,a5)
		bsr.l	ajust_music
.no_s
		*/ 'D' Musique Seulement */
		tst.b	(TOUCHE_D,a0)
		beq.s	.no_d
		move.w	#2,(music.flag-t,a5)
		bsr.l	ajust_music
.no_d
		*/ 'F' Sons Seulement */
		tst.b	(TOUCHE_F,a0)
		beq.s	.no_f
		move.w	#1,(music.flag-t,a5)
		bsr.l	ajust_music
.no_f
		*/ 'Q' Retour au menu */
		tst.b	(TOUCHE_Q,a0)
		beq.s	.no_q
		move.b	#1,(flag.quit-t,a5)
.no_q		rts

*---------------------------* Touches mémoire *---------------------------*
* Entrée	=> a0.l 'table_clavier'
*		=> a2.l test_clavier
* 		=> a5.l Adr. 't'
mushrooms:
		tst.b	(TOUCHE_AMIGA_G,a0)	touche AMIGA-GAUCHE ?
		beq	.no_l

		*/ INFO MEMOIRE */
		clr.b	(flag.help-t,a5)
		tst.b	(TOUCHE_HELP,a0)
		beq.s	.no_help
		move.b	#1,(flag.help-t,a5)
.no_help
		*/ TAB => MEMORY-LIST */
		clr.b	(flag.tab-t,a5)
		lea	(TOUCHE_TAB,a0),a1
		jsr	(a2)
		bne.s	.notab
		move.b	#1,(flag.tab-t,a5)
.notab
		*/ Raster on/off */
		lea	(TOUCHE_L,a0),a1
		jsr	(a2)
		beq.s	.no_l
		not.b	(flag.raster-t,a5)
.no_l		rts
