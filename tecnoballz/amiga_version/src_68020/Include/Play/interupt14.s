*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
*--------------* ROUT=14 Initialisation éditeur de levels *---------------*
*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
interupt14:
		move.w	#1,(editor-t,a5)
		clr.w	(niv00-t,a5)		raz pt/level
		clr.l	(triche-t,a5)
		clr.b	(flag.quit-t,a5)
		move.w	#1,(level-t,a5)
		move.w	#1,(lev0-t,a5)
		clr.w	(lev1-t,a5)
		move.w	#1,(area-t,a5)
		move.w	#1,(newtab-t,a5)
		bsr.l	raz_table_lutins
		bsr	gladia			£=> Réserve mémoire tableaux
		bne.l	sys_erreur
		move.w	#1,(music.on-t,a5)
		bsr.s	rupt14			£=> initialise avant tableau
		bne.l	sys_erreur
		move.w	#15,(rout-t,a5)
		clr.w	(musiclost-t,a5)
		move.w	#255,(flag_fade_on-t,a5)
		bra	irq

*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\
*------------------> Initialise l'editeur de tableaux <-------------------*
*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l	CUSTOM
rupt14:		
		clr.b	(flag_pause-t,a5)
		lea	(pt_screen-t,a5),a4
		bsr.l	screen_res
		bne	.exit
		clr.w	(SFMODE_LUTINS,a0)
		bsr	install_ecran2		"install_ecran.s"
		bne	.exit
		lea	(tbob-t,a5),a0
		move.l	(bob32_pagegfx-t,a5),d0
		bsr.l	bob32_main.init
		bne	.exit
		bsr	shado_initbob
		bne	.exit
		bsr	init_print
		bne.s	.exit
		bsr	init_fond
		bne.s	.exit
		bsr.l	copy_s1_to_s2
		bsr	init_bricks
		bne.s	.exit
		bsr	broque			£=> Affiche le menu Brique
		bsr	topazimov		£=> Affiche le numéro du level
		bsr	peste			£=> SWAP & COPY
		bsr	edmenu.print		£=> Affiche barre des menus
		bsr	filou			£=> Couleur menus
		bsr	shop.lulu		"interupt2.s" init. sprite souris
		bne.s	.exit
		bsr	ed.lulu			£=> Curseur lutins
		bne.s	.exit
		bsr	set.bricks		|-> Affiche les briques
		bsr.l	sprites_on
		bsr.l	pointe_ecran1
		bsr	init_degrade
		moveq	#0,d7
.exit		rts

*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*
*--------------------> Test s'il exite des tableaux <---------------------*
*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*
gladia:
		*/ Réserve espace mémoire nouveaux tableaux */
		moveq	#0,d7
		lea	(pt_tableaux-t,a5),a4
		tst.l	(a4)			déjà réservé ?
		bne.s	.exit			-> oui
		move.l	#TB.SIZE*50,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		bne.s	.exit
		*/ Décompacte les 100 tableaux */
		lea	(tablo.pp-t,a5),a4
		move.l	(RESA.ADR,a4),d0
		move.l	(RESA.SIZ,a4),d1
		lea	(tableaux-t,a5),a4
		bsr.l	decrunch.pp
		bne.s	.exit
		*/ Recopie 50 tableaux */
		move.l	(tableaux-t,a5),a0
		move.l	(pt_tableaux-t,a5),a1
		move.l	#TB.SIZE*50,d0
		bsr.l	copy.memory
		*/ Libère mémoire des 100 tableaux */
		lea	(tableaux-t,a5),a0
		bsr.l	libere_mem
.exit		tst.l	d7
		rts

*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*
**********************-*> RECOPIE LA PAGE BRIQUE <*-***********************
*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*
* Entrée	=> a5.l	Adr. 't'
broque:
		move.l	(bk.pagegfx-t,a5),a3
		move.l	(pt_screen-t,a5),a4
		move.w	(PG_NPLAN,a3),d1
		mulu	#7,d1
		lsl.w	#6,d1
		addq.w	#1,d1
		moveq	#0,d0
		move.w	(PG_SPLAN,a3),d0
		subq.w	#2,d0
		swap	d0
		move.w	(SP_SPLAN,a4),d0
		subq.w	#2,d0
		move.w	(PG_SLINE,a3),d3
		mulu	#BK.HEIGHT,d3
		move.w	(SP_SLINE,a4),d4

		lea	(PG_DATA,a3),a3
		move.l	(SPECRAN1,a4),a4
.wait3		btst	#14,(DMACONR,a6)
		bne.s	.wait3
		move.l	#$09F00000,(BLTCON0,a6)
		move.l	#-1,(BLTAFWM,a6)
		move.l	d0,(BLTAMOD,a6)
		moveq	#0,d2
		lea	(sg-t,a5),a1
		lea	(bg-t,a5),a2
		moveq	#62,d6
		*/ Boucle d'affichage */
.lala		moveq	#0,d5
		move.b	(a2)+,d5		pos. y
		mulu	d3,d5
		moveq	#0,d2
		move.b	(a2)+,d2		pos. x
		add.l	d2,d5
		add.l	a3,d5
		moveq	#0,d0
		move.b	(a1)+,d0		pos. y destination
		sub.w	#32,d0
		mulu	d4,d0
		move.b	(a1)+,d2		pos. x destination
		add.l	d2,d0
		add.l	a4,d0
.wait31		btst	#14,(DMACONR,a6)
		bne.s	.wait31
		move.l	d5,(BLTAPTH,a6)
		move.l	d0,(BLTDPTH,a6)
		move.w	d1,(BLTSIZE,a6)
		dbf	d6,.lala
		rts

*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*
*----------------------=- AFFICHE NUMERO TABLEAU -=-----------------------*
*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*
topazimov:	
		move.l	(niveau-t,a5),d2
		divu	#TB.SIZE,d2
		and.l	#$FFFF,d2
		bsr.l	convertit_decimal
		moveq	#2,d1
		lea	(tz1+3-t,a5),a0		Tableau
		bsr.l	conv_number_ascii

		move.l	(niveau-t,a5),d2
		divu	#TB.SIZE,d2
		divu	#10,d2
		and.l	#$FFFF,d2
		addq.w	#1,d2
		bsr.l	convertit_decimal
		moveq	#1,d1
		lea	(tz2+3-t,a5),a0		Area
		bsr.l	conv_number_ascii

		move.l	(niveau-t,a5),d2
		divu	#TB.SIZE,d2
		divu	#10,d2
		swap	d2
		and.l	#$FFFF,d2
		addq.w	#1,d2
		cmp.w	#6,d2
		blt.s	.ss
		addq.w	#1,d2
.ss		bsr.l	convertit_decimal
		moveq	#1,d1
		lea	(tz3+3-t,a5),a0		Level
		bsr.l	conv_number_ascii
		rts

*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*
*-------------------*  Convertit nombres Copy & Swap  *-------------------*
*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*
peste:	
		move.l	(cos-t,a5),d2
		divu	#TB.SIZE,d2
		and.l	#$FFFF,d2
		bsr.l	convertit_decimal
		moveq	#2,d1
		lea	(pe1+3-t,a5),a0
		bsr.l	conv_number_ascii

		move.l	(cod-t,a5),d2
		divu	#TB.SIZE,d2
		and.l	#$FFFF,d2
		bsr.l	convertit_decimal
		moveq	#2,d1
		lea	(pe2+3-t,a5),a0
		bsr.l	conv_number_ascii

		move.l	(ecs-t,a5),d2
		divu	#TB.SIZE,d2
		and.l	#$FFFF,d2
		bsr.l	convertit_decimal
		moveq	#2,d1
		lea	(pe3+3-t,a5),a0
		bsr.l	conv_number_ascii

		move.l	(ecd-t,a5),d2
		divu	#TB.SIZE,d2
		and.l	#$FFFF,d2
		bsr.l	convertit_decimal
		moveq	#2,d1
		lea	(pe4+3-t,a5),a0
		bsr.l	conv_number_ascii
		rts

*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*
*---------------------------> Affiche le menu <---------------------------*
*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*?*
edmenu.print:
		move.w	#256,d0
		moveq	#0,d1
		lea	(edmenu-t,a5),a0
		bsr.l	printf
		rts

filou:		move.w	#255,d0
		moveq	#0,d1
		moveq	#3,d2
		bsr	filature

		move.w	#7,d0
		moveq	#0,d1
		moveq	#4,d2
		bsr	filature
		move.w	#7,d0
		moveq	#0,d1
		moveq	#1,d2
		bsr	filature

		move.w	#15,d0
		moveq	#16,d1
		moveq	#1,d2
		bsr	filature
		rts
*----------------------------* Remplit menu *-----------------------------*
* Entrée	=> d0.w	Hauteur à remplir - 1
*		=> d1.w	N° ligne
*		=> d2.w N° du plan
filature:
		move.l	(pt_screen-t,a5),a0
		move.l	(SPECRAN1,a0),a1
		lea	(32,a1),a1
		move.w	(SP_SLINE,a0),d7
		mulu	d7,d1
		add.l	d1,a1
		mulu	(SP_SPLAN,a0),d2
		add.l	d2,a1
		subq.w	#4,d7	
		moveq	#-1,d6
.loop		move.l	d6,(a1)+
		move.l	d6,(a1)
		add.w	d7,a1
		dbf	d0,.loop
		rts

*/////////////////////////////////////////////////////////////////////////*
************************ Crée les lutins curseurs *************************
*/////////////////////////////////////////////////////////////////////////*
* Entrée	=> a5.l	adr. table 't'
ed.lulu:	
		*/ Reserve les lutins /*
		lea	(ed.sprite1-t,a5),a4
		bsr.l	reserve.mem
		bne.s	.exit
		lea	(ed.sprite2-t,a5),a4
		bsr.l	reserve.mem
		bne.s	.exit
	
		*/ CONVERTIT BOB -> SPRITE */
		moveq	#BOB.CURS,d0
		bsr	bob32_get.bob
		lea	(TBOB_TABLE,a0),a4
		move.l	(bob32_pagegfx-t,a5),a0
		move.w	(PG_SPLAN,a0),d1
		move.w	(PG_SLINE,a0),d2
		move.l	(ed.sprite1-t,a5),a0
		lea	(4*9,a0),a1
		move.l	(a4),a2
		moveq	#6,d0
		bsr.l	copytolutin16.0

		*/ CONVERTIT BOB -> SPRITE */
		moveq	#BOB.CURS,d0
		bsr	bob32_get.bob
		lea	(TBOB_TABLE,a0),a4
		move.l	(bob32_pagegfx-t,a5),a0
		move.w	(PG_SPLAN,a0),d1
		move.w	(PG_SLINE,a0),d2
		move.l	(ed.sprite2-t,a5),a0
		lea	(4*9,a0),a1
		move.l	(a4),a2
		moveq	#6,d0
		bsr.l	copytolutin16.0

		*/ Copie lutins dans la liste lutin */
		lea	(8+table_lutins-t,a5),a3
		move.l	(ed.sprite1-t,a5),a0
		move.l	a0,(a3)+
		lea	(4*9,a0),a0
		move.l	a0,(a3)+
		move.l	(ed.sprite2-t,a5),a0
		move.l	a0,(a3)+
		lea	(4*9,a0),a0
		move.l	a0,(a3)+

		moveq	#0,d7
.exit		rts
