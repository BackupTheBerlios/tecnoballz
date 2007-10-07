*//////////////////////////////////////////////////////////////////////////*
*-------------------------* INITIALISE LE MENU *---------------------------*
*//////////////////////////////////////////////////////////////////////////*
* Entrée	=> a5.l	Adr. 't'
menu.init:
		clr.w	(editor-t,a5)
		bsr.s	.init
		bne.l	sys_erreur
		bsr.l	copy_s1_to_s2s3
		bsr	sprites_on
		move.w	#255,(flag_fade_on-t,a5)
		move.w	#7,(rout-t,a5)
		lea	(mod.tecno-t,a5),a3
		bsr.l	np_init
		moveq	#0,d0
		move.w	d0,(music.on-t,a5)
		move.w	d0,(music4voix-t,a5)	Musique 4 voies
		move.w	d0,(musicsound-t,a5)	Son + musique
		bra	irq
*-------------------------------------------------------------------------*
.init		bsr	ii.decrunch		£=> Décompacte Dalles & MapEditor
		bne	.exit
		bsr	ii.screen00		£=> Initialise l'écran
		bne	.exit
		bsr	morfalou		£=> Remplit le plan 4
		bsr	init.sc_mapadr		|-> Calcul pt/dalle
		bne.s	.exit
		bsr	bruno			|-> Calcul valeur Blitter
		bsr	affiche_dalles		|-> Affiche dalles fonds
		move.l	(bob32_pagegfx-t,a5),d0
		lea	(tsbob-t,a5),a0
		bsr	sbob_main.init		|-> Initialise ScrollBOBs
		bne.s	.exit
		bsr	ssha_initsha		|-> Init. SahdowScrollBOBs
		bne.s	.exit
		bsr	ii.sigletec
		bne.s	.exit
		bsr	ii.bobmouse
		bne.s	.exit
		bsr	ii.textinit
		bne.s	.exit
		bsr	ii.iiiiiiii
		bne.s	.exit
		bsr	rekop58
		lea	(viki.main-t,a5),a0
		move.l	a0,(viking.menu-t,a5)
		clr.w	(acty-t,a5)
		moveq	#0,d7
.exit		rts
*-------------------------------------------------------------------------*

*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
*----------------* Décompacte les dalles & le map-editor *----------------*
*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
* Entrée	a5.l	=> adr. table 't'
* Sortie	d7.l	<=0 pas d'erreur
ii.decrunch:
		*/ Décompacte les dalles */
		sub.l	a2,a2
		lea	(dalma.pp-t,a5),a3
		lea	(dalma.pagegfx-t,a5),a4
		bsr.l	decode.pagepp
		bne.s	.exit

		*/ Décompacte le map-editor */
		lea	(maped3.pp-t,a5),a4
		move.l	(RESA.ADR,a4),d0
		move.l	(RESA.SIZ,a4),d1
		lea	(mapeditor-t,a5),a4
		bsr.l	decrunch.pp
		bne.s	.exit
		moveq	#0,d7
.exit		rts


*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
*------------------------ Installe l'écran de jeu ------------------------*
*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
* Entrée	a5.l	=> adr. table 't'
* Sortie	d7.l	<=0 pas d'erreur
ii.screen00:
		lea	(pt_screen-t,a5),a4
		bsr.l	screen_res
		bne	.exit
		move.w	#1,(SFREQ,a0)		force 50Hz
		move.w	#3,(SFMODE_PLANS,a0)
		move.w	#3,(SFMODE_LUTINS,a0)
		move.w	#320,(SNPIXEL,a0)
		move.w	#256,(SNLIGNE,a0)
		move.b	#5,(SNPLAN,a0)
		move.w	#3,(SPBUFFER,a0)
		move.w	#320,(SPLARGE,a0)
		move.w	#256+32,(SPHAUTEUR,a0)
		move.w	#16,(SPDOWN,a0)
		move.w	#1,(SLUTIN,a0)		pt lutins dans la liste
		move.w	#$24,(SBPLCON2,a0)
		move.w	#$FF,(SBPLCON4,a0)	Couleur lutin couleurs 64 à 79
		move.w	#1,(SP_VSCROLL,a0)	Scrolling vertical
		move.w	#512,(KKCOPPER,a0)	Intructions en +
		move.w	#1,(NBCOPPER,a0)	deux liste Copper
		move.w	#4,(SFADEOFFSPEED,a0)
		move.w	#0,(SFADEONSPEED,a0)
		bsr.l	init_screen
		bne	.exit
		move.w	#255,(NREGISTRE,a0)	nombre de couleurs-1

		*/ Recopie la palette de couleurs */
		move.l	(pt_menupal-t,a5),a1
		lea	(SCOLOR_TABLE,a0),a2
		moveq	#31,d0
.ggloop		move.l	(a1)+,(a2)+
		dbf	d0,.ggloop

		*/ Recopie la palette de couleurs */
		move.l	(pt_logopal-t,a5),a1
		lea	(SCOLOR_TABLE,a0),a2
		lea	(240*4,a2),a2
		moveq	#15,d0
.dggloop	move.l	(a1)+,(a2)+
		dbf	d0,.dggloop


		*/ Sauve quelques valeurs */
		move.l	(SPECRAN1,a0),(ecran1-t,a5)
		move.l	(SPECRAN2,a0),(ecran2-t,a5)
		move.l	(SPECRAN3,a0),(ecran3-t,a5)
		moveq	#0,d0
		moveq	#0,d1
		move.w	(SCOORDX,a0),d0
		move.w	(SCOORDY,a0),d1
		lsl.w	#2,d0
		move.l	d0,(coordx-t,a5)
		move.l	d1,(coordy-t,a5)
		move.w	(SP_SPLAN,a0),d0
		mulu	#NOPLAN,d0
		move.l	d0,(ecran_shadow-t,a5)
		moveq	#0,d7
.exit		rts
*								 	  *
*                          *INITIALISE LE LOGO*				  *
*								 	  *
* Entrée	a5.l	=> adr. table 't'
* Sortie	d7.l	<=0 pas d'erreur
ii.sigletec
		moveq	#SBOB.TECN,d0
		bsr	ssha.ajoute
		bne	.exit
		move.w	#32,(SBOB.POSX,a1)
		move.w	#8,(SBOB.POSY,a1)
		move.l	a1,(ii.bob-t,a5)

		*/ RESERVE LISTE LUTIN */
		moveq	#SBOB.TECN,d0
		bsr	sbob.get
		bne	.exit
		move.w	(TSBOB_HEIGHT,a0),d0
		move.w	d0,(ii.haut-t,a5)
		addq.w	#1,d0
		add.w	#VKN*VKH+2,d0
		mulu	#16,d0
		move.l	d0,d1
		mulu	#8,d1
		lea	(ii.lutin-t,a5),a4
		move.l	d1,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		bne	.exit
		move.l	(RESA.ADR,a4),d1
		lea	(ii.luti-t,a5),a1
		lea	(table_lutins-t,a5),a2
		moveq	#7,d2
.loop		move.l	d1,(a1)+
		move.l	d1,(a2)+
		add.l	d0,d1
		dbf	d2,.loop

		*/ Sauve adresse des textes menu */
		move.l	(RESA.ADR,a4),d1
		move.w	(TSBOB_HEIGHT,a0),d2
		addq.w	#1,d2
		mulu	#16,d2
		add.l	d2,d1
		lea	(ii.luti2-t,a5),a1
		moveq	#7,d2
.loophe		move.l	d1,(a1)+
		add.l	d0,d1
		dbf	d2,.loophe

		*/ Recopie le BOB dans les lutins */
		move.w	(TSBOB_HEIGHT,a0),d0
		subq.w	#1,d0
		move.l	(TSBOB_TABLE,a0),a2
		move.l	(sbob_pagegfx-t,a5),a4
		move.w	(PG_SPLAN,a4),d1
		move.w	(PG_SLINE,a4),d2
		lea	(ii.luti-t,a5),a4
		moveq	#3,d6
.xxloop		move.l	(a4)+,a0
		move.l	(a4)+,a1
		bsr.l	copylutin16.3
		addq.l	#8,a2
		dbf	d6,.xxloop

		bsr.l	init_pts_lutins2
		moveq	#0,d7
.exit		rts

*xoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxx*
*-------------------* Initialise BOB pointeur souris *--------------------*
*xoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxx*
* Entrée	a5.l	=> adr. table 't'
* Sortie	d7.l	<=0 pas d'erreur
ii.bobmouse:
		moveq	#SBOB.MOUS,d0
		bsr	sbob.ajoute
		bne.s	.exit
		bsr	ssha.only
		bne.s	.exit
		move.l	a1,(ii.bob2-t,a5)
		move.w	#5,(SBOB.INITTEMPO,a1)
		move.w	#10,(mouse_minix-t,a5)
		move.w	#300,(mouse_maxix-t,a5)
		move.w	#80,(mouse_miniy-t,a5)
		move.w	#235,(mouse_maxiy-t,a5)
		moveq	#0,d7
.exit		rts

*{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{*
*------------*Initialise la table d'affichage des Menu/Lutins*------------*
*{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{=}{*
* Entrée	a5.l	=> adr. table 't'
* Sortie	d7.l	<=0 pas d'erreur
ii.textinit:

		lea	(ii.table-t,a5),a4
		bsr.l	reserve.mem		|-> Reserve table de 5Ko
		bne.s	.exit
		move.l	(ii.table-t,a5),a4
		moveq	#16,d5
		moveq	#VKN-1,d2		20 Lignes
.loop3		lea	(ii.luti2-t,a5),a3
		moveq	#3,d3			4 Lutins 16 couleurs
.loop2		move.l	(a3)+,d6		d6=adresse lutin paire
		move.l	(a3)+,d7		d7=adresse lutin impaire
		add.l	d5,d6			+ Offset Hauteur
		add.l	d5,d7			+ Offset Hauteur
		moveq	#7,d4			8 caractères par lutin
.loop1		move.l	d6,(a4)+		Lutin Paire
		move.l	d7,(a4)+		Lutin Impaire
		addq.l	#1,d6
		addq.l	#1,d7
		dbf	d4,.loop1
		dbf	d3,.loop2
		add.l	#16*VKH,d5		Prochaine ligne
		dbf	d2,.loop3		
		moveq	#0,d7
.exit		rts

*OoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*
*---------------------* Initialise le scroll text *-----------------------*
*OoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*
* Entrée	a5.l	=> adr. table 't'
* Sortie	d7.l	<=0 pas d'erreur
ii.iiiiiiii:
		moveq	#AFNOR-1,d6
		move.w	#256/AFNOR,d5
		add.w	d5,d5
		lea	(iii-t,a5),a4
		moveq	#0,d4
.loop		moveq	#SBOB.FONT,d0
		bsr	sbob.ajoute
		bne	.exit
		bsr	ssha.only
		bne	.exit
		move.w	d4,(SBOB.POSX1,a1)
		move.w	d4,(SBOB.POSX2,a1)
		move.l	a1,(a4)+
		add.w	d5,d4
		dbf	d6,.loop

		*/ Réserve les caches */
		moveq	#SBOB.FONT,d0
		bsr	sbob.ajoute
		bne	.exit
		bsr	ssha.only
		bne	.exit
		move.l	a1,(ii1-t,a5)
		move.l	(SBOB.TABLE,a1),a0
		lea	(42*8,a0),a0
		move.l	(a0)+,(SBOB.GFX,a1)
		move.l	(a0),(SBOB.MSK,a1)
		moveq	#SBOB.FONT,d0
		bsr	sbob.ajoute
		bne	.exit
		bsr	ssha.only
		bne	.exit
		move.l	a1,(ii2-t,a5)
		move.l	(SBOB.TABLE,a1),a0
		lea	(42*8,a0),a0
		move.l	(a0)+,(SBOB.GFX,a1)
		move.l	(a0),(SBOB.MSK,a1)

		move.l	(scroll.text-t,a5),a2
		lea	(AFNOR,a2),a2
		move.l	a2,(iis-t,a5)
		lea	(iii-t,a5),a4
		moveq	#AFNOR-1,d6
		moveq	#0,d0
.loop2		move.b	-(a2),d0
		sub.w	#32,d0
		move.w	(skid-t,a5,d0*2),d1
		move.l	a4,(pt.iii-t,a5)
		move.l	(a4)+,a3
		move.l	(SBOB.TABLE,a3),a0	a0=table adr. gfx & msk
		add.w	d1,a0
		move.l	(a0)+,(SBOB.GFX,a3)
		move.l	(a0),(SBOB.MSK,a3)
		dbf	d6,.loop2
		moveq	#0,d7
.exit		rts

*xoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxx*
*--------------------------*Recopie les scores*---------------------------*
*xoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxx*
* Entrée	=> a5.l	Adr. 't'
rekop58:
		move.l	(pt_score-t,a5),a2
		move.w	(hard-t,a5),d0
		mulu	#OFF.SCORE,d0
		lea	(IS+KS+NS,a2,d0),a2
		lea	(lesscores-t,a5),a3
		moveq	#9,d7
.loop		moveq	#5,d0			NOM
		lea	(4,a2),a0
		lea	(6,a3),a1
		bsr	convertj
		moveq	#5,d0			SCORE
		lea	(11,a2),a0
		lea	(24,a3),a1
		bsr	convertj
		moveq	#0,d0			AREA
		lea	(20,a2),a0
		lea	(19,a3),a1
		bsr	convertj
		moveq	#1,d0			LEVEL
		lea	(25,a2),a0
		lea	(13,a3),a1
		bsr	convertj
		lea	(NS,a2),a2
		lea	(32,a3),a3
		dbf	d7,.loop
		rts

*xoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxx*
*                      * LIBERE MEMOIRE DE L'INTRO *			  *
*xoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxxoxx*
* Entrée	=> a5.l	Adr. 't'
ii.kill:	
		bsr.s	.freemem
		bne.l	sys_fin
		rts
.freemem	bsr.l	kill_screen
		bne.s	.exit
		bsr	sbob.freemem2
		bne.s	.exit
		bsr	ssha.freemem
		bne.s	.exit
		bsr.l	np_end
		bne.s	.exit
		lea	libere_mem,a4
		lea	(ii.lutin-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		lea	(dalma.pagegfx-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		lea	(mapeditor-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		lea	(sc_mapadr-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		lea	(ii.table-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		moveq	#0,d7
.exit		rts
