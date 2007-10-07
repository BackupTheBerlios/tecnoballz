*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
*--------------* ROUT=0 Initialisation avant les tableaux *---------------*
*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
* Entrée	=> a5.l	Adr. 't'
interupt0:
		bsr.s	rupt0			|-> init. avant tableau
		bne.l	sys_erreur	
		move.w	#1,(rout-t,a5)		1=Tableaux
		move.w	#255,(flag_fade_on-t,a5)
		bra	irq		

*-***********************************************************************-*
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d7.l #0 pas d'erreur
rupt0:		
		bsr.l	raz_table_lutins
		bsr	glockenspiel		£=> Initialise module Proctracker
		bne	.exit
		clr.w	(musiclost-t,a5)
		clr.w	(music.on-t,a5)
		clr.b	(flag_pause-t,a5)
		clr.b	(controlator-t,a5)
		bsr	init_level		|-> Difficulté du tableau
		bsr	install_ecran
		bne	.exit
		lea	(tbob-t,a5),a0
		move.l	(bob32_pagegfx-t,a5),d0
		bsr	bob32_main.init
		bne	.exit
		bsr	shado_initbob
		bne	.exit
		bsr	init_print
		bne	.exit
		bsr	init_fond		|-> Affiche le fond 4 couleurs
		bne.s	.exit
		bsr	init_message		|-> Init. les mini-message
		bne.s	.exit
		bsr	init_score		|-> Installe la bande score
		bne.s	.exit
		bsr	init_oeject		|-> Affiche ombre éjecteurs
		bsr	init_bricks		|-> Init. Affiche briques
		bne.s	.exit
		bsr	init_mur		|-> Mur du bas
		bne.s	.exit
		bsr	init_objet		|-> BOBs tirs
		bne.s	.exit
		bsr	init_rak		|-> Lutins raquettes 
		bne.s	.exit
		bsr	init_balle
		bne.s	.exit
		bsr	init_capsule		|-> BOBs monnaies
		bne.s	.exit
		bsr	init_gadget		|-> BOBs Bonus/Malus
		bne.s	.exit
		bsr	init_head		|-> Tête dans la bande des scores
		bne.s	.exit
		bsr	init_bricotes		|-> Les 3 murs de côté
		bne.s	.exit
		bsr	init_eject		|-> Affiche éjecteurs
		bsr	init_atom
		bne.s	.exit
		bsr	init_blitz		|-> Lutin GigaBlitz
		bne.s	.exit
		bsr	init_gemme		|-> BOBs Gemme
		bne.s	.exit
		bsr	init_oeil		|-> BOB oeil
		bne.s	.exit
		bsr	init_bobtext		|-> BOBs "LEVEL COMPLETED"
		bne.s	.exit
		bsr	print_box		|-> Affiche scores/noms/levels
		bsr.l	copy_s1_to_s2s3
		bsr	sprites_on
		bsr	init_degrade		|-> Dégradé dans les briques
		moveq	#0,d7
.exit		rts

***************************************************************************
*-------------> Décompacte & Choisis une des deux musiques <--------------*
***************************************************************************
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d7.l #0 pas d'erreur
glockenspiel:	

		*/ Décompacté & Initialise le module */
		move.w	(area-t,a5),d0		n° area de 1 à 5
		subq.w	#1,d0
		mulu	#mod.area2-mod.area1,d0
		lea	(mod.area1-t,a5,d0),a3
		bsr.l	np_init			|-> Init. le module !
		bne.s	.exit
		move.w	(area-t,a5),d0		n° area de 1 à 5
		subq.w	#1,d0
		mulu	#poz2-poz,d0
		lea	(poz-t,a5,d0),a0
		move.l	a0,(pt_poz-t,a5)	Table des positions
		*/ Alterne le module 1 & 2 */
		move.l	(pt_poz-t,a5),a2 	table des positions
		move.w	(2,a2),d0
		move.w	(level-t,a5),d1		n° du tableau
		btst	#0,d1
		beq.s	.kkapone
		move.w	(a2),d0
.kkapone	bsr.l	np_change
		bsr.l	ajust_music
		moveq	#0,d7
.exit		rts
