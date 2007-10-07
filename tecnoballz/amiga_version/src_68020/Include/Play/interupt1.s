*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
**************************** ROUT=1 : Tableaux ****************************
*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
interupt1:
		tst.b	(flag_pause-t,a5)
		bne.s	.nuke
		clr.l	(blitz.motctrl-t,a5)
		bsr	chunky			|-> Gestion GigaBlitz
.nuke		bsr	init_pts_lutins
		bsr	wait_ze_scan
		bsr	gaga_fada		|-> Fade couleurs briques
		bsr	blitzalair
		bsr	hasard_ok
		RASTER	#$00F
		bsr	flip_screens
		bsr	goblins2
		bsr	bump_bump
		tst.b	(flag_pause-t,a5)
		bne.s	.nuke2
		bsr	bp_depl1		|-> Test déplacement Bumper
.nuke2		bsr	etb_end
		bsr.l	fire.color
		bsr	choupette		|-> efface bricotes
		bsr	choucroutte		|-> efface les briques
		bsr	choucroutte		|-> efface les briques
		bsr	msx.off			|-> efface pointeurs de balle
		bsr.l	tableau_end
		bsr	bob32_restore
		bsr	shado_restore
		bsr	gestion_mur
		RASTER	#$FFF
		tst.b	(flag_pause-t,a5)
		bne.s	.pause
		bsr	byte_rtel		|-> LessBricks
		bsr	no_dream1		|-> affiche messages
		bsr	msdos_beurk		|-> déplace monnaies
		bsr.l	gadget_dep		|-> déplace malus/bonus
		bsr.l	gestion_oeil
		bsr.l	gad_re			|-> collision balles/briques
		bsr.l	gere_robot
		bsr.l	atom_collision1		|-> collision balles/atoms
		bsr.l	atom_collision2		|-> collision tirs/atoms
		bsr.l	laser			|-> Test si une raquette tir
		bsr.l	fire_bricks		|-> collision tirs/briques
		bsr.l	tir_deplacement
		bsr	atom_dep		|-> Déplace les 4 atoms
		bsr	mouse_buttons		|-> Test les boutons souris
		bsr.l	mouse_blitz		|-> Test bouton GigaBlitz
		bsr.l	mouse_fire		|-> Test bouton Tir
		bsr.l	lache_balle		|-> Test bouton Lache balle
		bsr.l	tilt		 	|-> Test bouton Tilt
		bsr	gere_balle		|-> déplace balle
		bsr	bob_anim
		bsr	pub1			|-> Inverseur
		bsr.l	gestion_gemme
.pause		RASTER	#$F00
		bsr	shado_affiche
		bsr	bob32_affiche
		RASTER	#$0F0
		bsr.l	no_dream2		|-> lache balle si lost fini
		bsr	msx			|-> affiche pointeurs de balles
		bsr	pub3			|-> affiche le crédit
		bsr	print_bricks
		bsr	print_score
		bsr	print_lifes
		bsr	total_recal		|-> affiche tête
		bsr.l	extralife
		bsr	pertedememoire
		bsr	klara.triche
		bsr.l	np_sfx
		bsr	quit
		RASTER	#$000
		bra	irq


*/ VOIR PAGE BOB & PAGE MSK */
goblins2	cmp.l	#ETB,(triche-t,a5)
		bne.s	.exit
		move.l	(atable_clavier-t,a5),a0
		tst.b	(TOUCHE_F1,a0)
		bne.s	.gfx
		tst.b	(TOUCHE_F2,a0)
		bne.s	.msk
.exit		rts
.gfx		move.l	(bob32_pagegfx-t,a5),a3
		bra.s	.ii
.msk		move.l	(bob32_pagemsk-t,a5),a3
.ii		move.l	a3,d0
		add.l	#PG_DATA,d0
		tst.b	(TOUCHE_SHIFT_G,a0)	bas de la page ?
		beq.s	.jj			-> non
		move.w	(PG_SLINE,a3),d1
		mulu	#256,d1
		add.l	d1,d0
.jj		move.l	(pt_screen-t,a5),a0
		move.l	(SPPOINTEUR,a0),a1
		addq.l	#2,a1
		bsr.l	pointe.x
		rts


*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
********************* ECHANGE LES DEUX LISTES COPPERS *********************
*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l CUSTOM
flip_screens:
		move.l	(pt_screen-t,a5),a0
		tst.w	(flag_flip-t,a5)
		beq.w	.gx
		clr.w	(flag_flip-t,a5)
		move.l	(SPECRAN1,a0),(ecran_travail-t,a5)
		move.l	(SACOPPER,a0),a0
		bra.s	.go_xmen
.gx		move.w	#4,(flag_flip-t,a5)
		move.l	(SPECRAN2,a0),(ecran_travail-t,a5)
		move.l	(SACOPPER2,a0),a0
.go_xmen	move.l	a0,(acopper-t,a5)
		move.l	a0,(COP1LCH,a6)
		rts

*/ ATTEND VBL */
wait_ze_scan:
		lea	(b1-t,a5),a0
		move.w	(BUMP.X,a0),d1
		lea	(hasard2-t,a5),a1
		lea	(vbl_counter-t,a5),a0
		move.w	(a0),d0
.wait_vbl	add.w	d1,(a1)
		or.w	d0,(a1)
		cmp.w	(a0),d0
		beq.s	.wait_vbl
		rts
