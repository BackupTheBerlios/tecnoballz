*-------------------------------------------------------------------------*
*	INTRODUCTION D'EBONSTAR 94
*		Commencé le 25 février 1994
*			(c) 1994 NOUNOURS CONCEPT
*				Version du 1er mars 1994
*-------------------------------------------------------------------------*
		SECTION	NOUNOURS,CODE
		*OPT	P+		Pas de table de relogement
		OPT	C20+		Labels et symboles 20 caractères maxi.,  majuscules/minuscules respectées
		OPT	O-		Pas d'optimisation
		OPT	p=68020	
		INCDIR	DH0:Data/

*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONSTANTES <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*
NReservation	=50	50 adresses mémoires réservées maximums
FLAG_ARP	=0	1=ouvre la librairie ARP
FLAG_TRACKDISK	=0	1=assemble programme pour être chargé en "TrackDisk"
FLAG_MEMORY	=1	1=information sur la mémoire possible
FLAG_ILBM	=1	1=assemble routines décompactage IFF-ILBM
FLAG_PP		=0	1=assemble routines décompactage PowerPacker
FLAG_MOUSE0	=0	1=assemble la routine déplacement souris port 0
FLAG_AMIGADOS	=1	1=assemble routines chargement AmigaDOS
FLAG_EXIT	=0	0=sortie sous système possible avec la touche "ESC"
FLAG_MUSIC	=0	1=assemble routines gestion module "ProTracker"
FLAG_KEY	=1	1=assemble routines de test du clavier
FLAG_SOUND	=0	1=assemble routines gestion des bruitages
FLAG_LUTIN	=1	1=assemble routines gestion des lutins
FLAG_TDISK	=0	1=assemble routines de lecture en "TrackDisk"
FLAG_TDISKW	=0	1=assemble routines d'écriture en "TrackDisk"
FLAG_SCROLL	=0	1=assemble routines gestion défilement écran
FLAG_BOB32	=0	Si=1 Alors assemble routines bobs (aff. écran 32 bits)
FLAG_FREQ	=0	Si=1 Alors assemble routine demande fréquence balayage
FLAG_CLOSEWB	=0	Si=1 Alors ferme l'écran WorkBench
FLAG_DMACON.MSK	=0


IE.HEIGHT1	=215	Nombre de lignes dégradé dans couleurs lutins
IE.HEIGHT2	=41	Nombre de lignes dégardé couleur 255 (scroll-text)
IE.COPSIZ1	=11*4	Taille d'une "ligne" Copper dégradé lutin
IE.COPSIZ2	=5*4	Taille d'une "ligne" Copper dégradé défilement
IE.NMENU	=8	Nombre de lignes menu


*>>>>>>>>>>>>>>>>>>>>>>>>>> Debut code MC68020 <<<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	
		INCLUDE	"Mentat/Include/mentat.s"
msg.pers
		dc.b	"                                           "	¤-128
		EVEN

************* Routine appelée avant de quitter le multitâche **************
sys_extra:	
		lea	(name.ilbm,pc),a0
		moveq	#0,d0
		bsr	charge_fichier_dos
		bne.s	.exit
		move.l	d0,(adresse.ilbm-t,a5)

		*/ CHARGE FONTES /*
		lea	(name2,pc),a0
		sub.l	a2,a2
		bsr	charge_page
		bne.s	.exit
		move.l	d0,(garnet_pagegfx-t,a5)

		*/ CHARGE TEXT ASCII /*
		lea	(name3,pc),a0
		moveq	#0,d0
		bsr	charge_fichier_dos
		bne.s	.exit
		move.l	d0,(garnet_txt-t,a5)

		*/ CHARGE BROSSE "EBONSTAR94" 512*41 4 couleurs /*
		lea	(name4,pc),a0
		sub.l	a2,a2
		bsr	charge_page
		bne.s	.exit
		move.l	d0,(title_pagegfx-t,a5)

		*/ CHARGE BROSSE FONTES 640*8 4 couleurs /*
		lea	(name5,pc),a0
		sub.l	a2,a2
		bsr	charge_page
		bne.s	.exit
		move.l	d0,(fontes_pagegfx-t,a5)

		*/ CHARGE MUSIQUE */
		lea	(name6,pc),a0
		moveq	#MEMF_CHIP,d0
		bsr	charge_fichier_dos
		bne.s	.exit		
		move.l	d0,(music.adr-t,a5)


		moveq	#0,d7
.exit		rts
name.ilbm	dc.b	"DH2:pic/Agurk",0
name3		dc.b	"EBONSTAR94:Ebonstar94/Data/Garnet.text",0
name2		dc.b	"EBONSTAR94:Ebonstar94/Gfx/Garnet-fontes.ilbm",0
name4		dc.b	"EBONSTAR94:Ebonstar94/GFX/Title2.ilbm",0
name5		dc.b	"EBONSTAR94:Ebonstar94/GFX/Fontes.ilbm",0
name6		dc.b	"Ebonstar94:Ebonstar94/Sfx/mod.Eq2",0

		EVEN

*>>>>>>>>>>>>>>>>>>>>>>>>>> début du programme <<<<<<<<<<<<<<<<<<<<<<<<<<<*
run:	
		bsr	first_init
		bsr	sprites_off
		bsr	screen_reserve
		bne	sys_erreur
		move.w	#1,(SPBUFFER,a0)
		move.w	#3,(SFMODE_PLANS,a0)	mode X4
		move.w	#3,(SFMODE_LUTINS,a0)	lutin de 64 pixels de large
		move.w	#1,(SLUTIN,a0)
		move.w	#2,(SLUTIN_RES,a0)
		move.w	#$0002,(SBPLCON3,a0)	BRDSPRT=1 (lutins affichagle sur les bords)
		move.w	#$24,(SBPLCON2,a0)
		move.w	#$FF,(SBPLCON4,a0)	Couleur lutin couleurs 64 à 79
		move.w	#1,(SFREQ,a0)		Force le 50Hz
		move.w	#IE.HEIGHT1*IE.COPSIZ1,(KKCOPPER,a0)
		add.w	#IE.HEIGHT2*IE.COPSIZ2,(KKCOPPER,a0)
		add.w	#80,(KKCOPPER,a0)
		move.l	(adresse.ilbm-t,a5),d0
		bsr	affiche_image_ilbm
		bne	sys_erreur
		clr.w	(flag_fade_on-t,a5)
		bsr	colors_on
		bsr	full_color
		bne	sys_erreur
		bsr	nostradamus
		bne	sys_erreur
		bsr	sprites_on
.the_sine	tst.w	(flag_fade_on-t,a5)
		bne.s	.the_sine
		bsr	init_garnet
		bne	sys_fin
		move.l	(music.adr,pc),a0
		bsr	MT_Init

*¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ BOUCLE PRINCIPALE HORS INTERRUPTION ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤*
main_loop:
		bsr	wait_vbl
		bsr	memory_help
		bsr	garnet_scroll
		bsr	lutin_move
		bsr	garnet.cycling
		bsr	cycle
		bsr	MT_Music
		btst	#6,$bfe001
		bne.s	main_loop
		bsr	MT_End
		bra	sys_fin


*/////////////////////////////////////////////////////////////////////////*
*»»»»»»»»»»»»»»»»»»»»» Initialise les listes lutins ««««««««««««««««««««««*
*/////////////////////////////////////////////////////////////////////////*
* Entrée	=> a5.l adr. 't'
nostradamus:
		*/ Réserve les 8 listes lutins */
		move.l	(title_pagegfx-t,a5),a4
		moveq	#0,d0
		move.w	(PG_HEIGHT,a4),d0	hauteur du logo EBONSTAR
		addq.l	#1,d0			+ mot de ctrl
		move.l	d0,d3
		add.l	#IE.NMENU*9,d0		+ nombre de lignes Menu
		addq.l	#1,d0			+ fin
		lsl.l	#4,d0			16 octets par lignes
		move.l	d0,d2			taille d'une liste lutin
		lsl.l	#3,d0			8 lutins
		moveq	#MEMF_CHIP,d1
		tst.l	(nost0-t,a5)		mémoire dèjà réservée ?
		bne.s	.rita			-> oui
		bsr	reserve_memoire
		move.l	d0,(nost0-t,a5)		dépassement mémoire ?
		beq	.outmem			-> oui
.rita		lea	(nost0,pc),a0
		move.l	(a0),d0
		lea	(table_lutins-t,a5),a1
		moveq	#8-1,d1
.loopxxx	move.l	d0,(a0)+
		move.l	d0,(a1)+
		add.l	d2,d0
		dbf	d1,.loopxxx

		*/ sauve adresses des listes lutin menu */
		lsl.l	#4,d3			16 octets par lignes
		lea	(menu0-t,a5),a1
		move.l	#9*16,d4		taille d'un lutin
		moveq	#IE.NMENU-1,d5
		move.l	d3,d6
.tecno		moveq	#8-1,d2			8 lutins pour une ligne menu
		lea	(nost0-t,a5),a0
.cocoto		move.l	(a0)+,d0		d0=adr. de la liste lutin
		add.l	d6,d0			saute le lutin logo EBONSTAR
		move.l	d0,(a1)+
		dbf	d2,.cocoto
		add.l	d4,d6
		dbf	d5,.tecno

		*/ Recopie le logo dans les listes lutins */
		move.w	(PG_HEIGHT,a4),d0	hauteur du logo EBONSTAR
		subq	#1,d0
		move.w	(PG_SPLAN,a4),d1
		move.w	(PG_SLINE,a4),d2
		moveq	#8-1,d3
		lea	(nost0-t,a5),a1
		lea	(PG_DATA,a4),a2
.night		move.l	(a1)+,a0
		bsr	copytolutin4.3
		addq.l	#8,a2
		dbf	d3,.night

		*/ Positionne les lutins */
		lea	(nost0-t,a5),a1
		move.l	(pt_screen-t,a5),a0
		moveq	#0,d4
		move.w	(PG_HEIGHT,a4),d4	hauteur du logo EBONSTAR
		moveq	#8-1,d5
		moveq	#0,d6
		moveq	#0,d7
		move.w	(SCOORDX,a0),d6
		lsl.l	#2,d6
		move.w	(SCOORDY,a0),d7
.koko		move.l	d4,d2
		move.l	d6,d0
		move.l	d7,d1
		bsr	calcul_ctrlaga
		move.l	(a1)+,a0
		move.w	d3,(8,a0)
		swap	d3
		move.w	d3,(a0)	
		add.l	#64*2,d6
		dbf	d5,.koko
YM=88

		move.l	#256,(menu0.x-t,a5)
		move.l	#YM,(menu0.y-t,a5)
		move.l	#256,(menu1.x-t,a5)
		move.l	#YM+14,(menu1.y-t,a5)
		move.l	#256,(menu2.x-t,a5)
		move.l	#YM+14*2,(menu2.y-t,a5)
		move.l	#256,(menu3.x-t,a5)
		move.l	#YM+14*3,(menu3.y-t,a5)
		move.l	#256,(menu4.x-t,a5)
		move.l	#YM+14*4,(menu4.y-t,a5)
		move.l	#256,(menu5.x-t,a5)
		move.l	#YM+14*5,(menu5.y-t,a5)
		move.l	#256,(menu6.x-t,a5)
		move.l	#YM+14*6,(menu6.y-t,a5)
		move.l	#256,(menu7.x-t,a5)
		move.l	#YM+14*7,(menu7.y-t,a5)
		bsr	set_line_menu		-> calul lignes menu
		bsr	zaffiche
		bsr	init_pts_lutins2
		moveq	#0,d7
		rts
.outmem		moveq	#-12,d7
		rts


lutin_move:
		lea	(ppp,pc),a0
		move.l	(-4,a0),d0
		add.l	d0,(a0)
		beq.s	.okd
		cmp.l	#640*2-512*2,(a0)
		blt.s	.sas
.okd		neg.l	(-4,a0)
.sas

		move.l	(title_pagegfx-t,a5),a4
		lea	(nost0-t,a5),a1
		move.l	(pt_screen-t,a5),a0
		moveq	#0,d4
		move.w	(PG_HEIGHT,a4),d4	hauteur du logo EBONSTAR
		moveq	#8-1,d5
		moveq	#0,d6
		moveq	#22,d7
		move.w	(SCOORDX,a0),d6
		lsl.l	#2,d6
		add.w	(SCOORDY,a0),d7
		add.l	(ppp-t,a5),d6
.koko		move.l	d4,d2
		move.l	d6,d0
		move.l	d7,d1
		bsr	calcul_ctrlaga
		move.l	(a1)+,a0
		move.w	d3,(8,a0)
		swap	d3
		move.w	d3,(a0)	
		add.l	#64*2,d6
		dbf	d5,.koko
		rts

*»»»»»»»»»»»»»»»»»»					««««««««««««««««««*
*--*--*--*--*--*--* POSITIONNE LES LUTINS "LIGNES MENU" *--*--*--*--*--*--*
*»»»»»»»»»»»»»»»»»»					««««««««««««««««««*
* Entrée	=> a5.l	Adr. "t"
set_line_menu:	bsr	menu.ctrl_init
		lea	(menu.lutin_ctrl-t,a5),a3
		lea	(menu0-t,a5),a1
		move.l	(menu0.x-t,a5),d4
		move.l	(menu0.y-t,a5),d5
		jsr	(a3)
		move.l	(menu1.x-t,a5),d4
		move.l	(menu1.y-t,a5),d5
		jsr	(a3)
		move.l	(menu2.x-t,a5),d4
		move.l	(menu2.y-t,a5),d5
		jsr	(a3)
		move.l	(menu3.x-t,a5),d4
		move.l	(menu3.y-t,a5),d5
		jsr	(a3)
		move.l	(menu4.x-t,a5),d4
		move.l	(menu4.y-t,a5),d5
		jsr	(a3)
		move.l	(menu5.x-t,a5),d4
		move.l	(menu5.y-t,a5),d5
		jsr	(a3)
		move.l	(menu6.x-t,a5),d4
		move.l	(menu6.y-t,a5),d5
		jsr	(a3)
		move.l	(menu7.x-t,a5),d4
		move.l	(menu7.y-t,a5),d5
		jmp	(a3)

***************** Initialise la routine "menu.lutin_ctrl" *****************
menu.ctrl_init:
		move.l	(pt_screen-t,a5),a0
		moveq	#0,d6
		moveq	#0,d7
		move.w	(SCOORDX,a0),d6
		lsl.l	#2,d6
		move.w	(SCOORDY,a0),d7
		lea	(calcul_mot_ctrl_aga-t,a5),a2
		rts

*-*-*-*-*-*-*> CALCUL LES MOTS DE CTRL POUR UNE LIGNE LUTIN *-*-*-*-*-*-*-*
* Entrée	=> d4.l	position x en "superhires"
*		=> d5.l position y
*		=> d6.l coordonnée x bord gauche de l'écran en "supehires"
*		=> d7.l coordonnée y bord haut de l'écran
*		=> a1.l pointeur sur les 8 adresses des 8 lutins
*		=> a2.l routine "calcul_mot_ctrl_aga"
menu.lutin_ctrl:
		REPT	8
		moveq	#8,d2
		move.l	d4,d0
		move.l	d5,d1
		jsr	(a2)
		move.l	(a1)+,a0
		move.w	d3,(8,a0)
		swap	d3
		move.w	d3,(a0)	
		add.l	#128,d4
		ENDR
		rts

******** Routine appelée par l'interruption du temps mort vertical ********
* Entrée	a5.l	=> Adr. 't'
extra_vbl:	rts
		CNOP	0,4
		dc.l	1
ppp		dc.l	0
nost0		dc.l	0	adresse liste lutin 0
nost1		dc.l	0	adresse liste lutin 1
nost2		dc.l	0	adresse liste lutin 2
nost3		dc.l	0	adresse liste lutin 3
nost4		dc.l	0	adresse liste lutin 4
nost5		dc.l	0	adresse liste lutin 5
nost6		dc.l	0	adresse liste lutin 6
nost7		dc.l	0	adresse liste lutin 7
menu0		dcb.l	8,0	adresse des 8 lutins composant la ligne menu 0
menu1		dcb.l	8,0	adresse des 8 lutins composant la ligne menu 1
menu2		dcb.l	8,0	adresse des 8 lutins composant la ligne menu 2
menu3		dcb.l	8,0	adresse des 8 lutins composant la ligne menu 3
menu4		dcb.l	8,0	adresse des 8 lutins composant la ligne menu 4
menu5		dcb.l	8,0	adresse des 8 lutins composant la ligne menu 5
menu6		dcb.l	8,0	adresse des 8 lutins composant la ligne menu 6
menu7		dcb.l	8,0	adresse des 8 lutins composant la ligne menu 7
menu0.x		dc.l	0
menu0.y		dc.l	0
menu1.x		dc.l	0
menu1.y		dc.l	0
menu2.x		dc.l	0
menu2.y		dc.l	0
menu3.x		dc.l	0
menu3.y		dc.l	0
menu4.x		dc.l	0
menu4.y		dc.l	0
menu5.x		dc.l	0
menu5.y		dc.l	0
menu6.x		dc.l	0
menu6.y		dc.l	0
menu7.x		dc.l	0
menu7.y		dc.l	0
coordx		dc.l	0
coordy		dc.l	0
ncopper1.color	dc.l	0	nombre de lignes avant le wait lignes 255
copper1.color	dc.l	0	pt/moves Copper couleur dans les lutins
copper2.color	dc.l	0	pt/moves Copper couleur lutins après ligne 255
copper3.color	dc.l	0	pt/moves Copper couleur défilement du texte
garnet.colorpt	dc.l	0	pt/table couleur du dégradé (couleurs lutins)
colorpt		dc.l	0	pt/table couleur du dégradé (couleurs texte)
adresse.ilbm	dc.l	0	adresse image iff-ilbm de fond
title_pagegfx	dc.l	0	adresse logo EBONSTAR94 raw-blitter 4 couleurs
fontes_pagegfx	dc.l	0
music.adr	dc.l	0

		INCLUDE	"Ebonstar94/Includes/Menu/Garnet.s"
		INCLUDE	"Ebonstar94/Includes/Menu/garnet_Data.s"
		INCLUDE	"Ebonstar94/Includes/Menu/Cycling.s"
		INCLUDE	"Mentat/Include/musique/protracker_replay.s"

*>>>>>>>>>>>>>>>>>> Adresses utilisées par le programme <<<<<<<<<<<<<<<<<<*
		INCLUDE	"Mentat/Include/Systeme/t.s"
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
		dc.b	"$VER: INTRO EBONSTAR94 (12.3.94)",0
		
		EVEN


kiss:	dc.b " > COMMENCE UNE PARTIE          "
	dc.b "   STATISTIQUE                  "
	dc.b "   OPTIONS                      "
	dc.b "   EDITEUR DE NIVEAUX           "
	dc.b "   CREDITS                      "
	dc.b "                                "
	dc.b " UTILISEZ LES TOUCHES CURSEURS  "
	dc.b " ET LA TOUCHE SHIFT.            "


zaffiche:
		move.l	(fontes_pagegfx-t,a5),a4
		lea	(PG_DATA,a4),a4
		lea	(menu0-t,a5),a0
		lea	(kiss-t,a5),a3
		moveq	#64-1,d7		64 lutins
.olivier	move.l	(a0)+,a1
		lea	(16,a1),a1
		moveq	#4-1,d2			4 caractères par lutin
.cet_arbre	moveq	#0,d0
		move.b	(a3)+,d0		d0=valeur ASCII
		move.b	(minos,pc,d0),d0
		add.w	d0,d0
		add.l	a4,d0
		move.l	d0,a2
		move.w	(a2),(a1)
		move.w	(80,a2),(8,a1)
		move.w	(80*2,a2),(8*2,a1)
		move.w	(80*3,a2),(8*3,a1)
		move.w	(80*4,a2),(8*4,a1)
		move.w	(80*5,a2),(8*5,a1)
		move.w	(80*6,a2),(8*6,a1)
		move.w	(80*7,a2),(8*7,a1)
		move.w	(80*8,a2),(8*8,a1)
		move.w	(80*9,a2),(8*9,a1)
		move.w	(80*10,a2),(8*10,a1)
		move.w	(80*11,a2),(8*11,a1)
		move.w	(80*12,a2),(8*12,a1)
		move.w	(80*13,a2),(8*13,a1)
		move.w	(80*14,a2),(8*14,a1)
		move.w	(80*15,a2),(8*15,a1)
		addq.l	#2,a1
		dbf	d2,.cet_arbre
		dbf	d7,.olivier
		rts


minos:
 dc.b 039,039,039,039,039,039,039,039,039,039,039,039,039,039,039,039
 dc.b 039,039,039,039,039,039,039,039,039,039,039,039,039,039,039,039
 dc.b 039,039,039,039,039,039,039,038,039,039,039,039,039,039,039,039
 dc.b 027,028,029,030,031,032,033,034,035,036,039,039,039,039,037,039
 dc.b 039,000,001,002,003,004,005,006,007,008,009,010,011,012,013,014
 dc.b 015,016,017,018,019,020,021,022,023,024,025,039,039,039,039,039
 dc.b 039,039,039,039,039,039,039,039,039,039,039,039,039,039,039,039
 dc.b 039,039,039,039,039,039,039,039,039,039,039,039,039,039,039,039
 dc.b 039,039,039,039,039,039,039,039,039,039,039,039,039,039,039,039
 dc.b 039,039,039,039,039,039,039,039,039,039,039,039,039,039,039,039
 dc.b 039,039,039,039,039,039,039,039,039,026,039,039,039,039,039,039
 dc.b 039,039,039,039,039,039,039,039,039,039,039,039,039,039,039,039
 dc.b 039,039,039,039,039,039,039,039,039,039,039,039,039,039,039,039
 dc.b 039,039,039,039,039,039,039,039,039,039,039,039,039,039,039,039
 dc.b 039,039,039,039,039,039,039,039,039,039,039,039,039,039,039,039
 dc.b 039,039,039,039,039,039,039,039,039,039,039,039,039,039,039,039


* A à Z 	65 à 90 : 00 à 25
* ©		169	: 26
* 0 à 9		48 à 57 : 27 à 36
* >		62	: 37
* '		39	: 38
* espace	32	: 39


garnet_palette:
 DC.L $00000000
 DC.L $000004FF
 DC.L $00020CFF
 DC.L $00030DFF
 DC.L $000313FF
 DC.L $000414FF
 DC.L $00051BFF
 DC.L $00061CFF
 DC.L $000723FF
 DC.L $000723FF
 DC.L $000829FF
 DC.L $00092AFF
 DC.L $000A31FF
 DC.L $000B32FF
 DC.L $000B38FF
 DC.L $000C38FF
 DC.L $000D3FFF
 DC.L $000E40FF
 DC.L $000F46FF
 DC.L $001047FF
 DC.L $00104CFF
 DC.L $00114DFF
 DC.L $001254FF
 DC.L $001354FF
 DC.L $00145BFF
 DC.L $00145BFF
 DC.L $001561FF
 DC.L $001661FF
 DC.L $001768FF
 DC.L $001868FF
 DC.L $00186EFF
 DC.L $00196EFF
 DC.L $001A74FF
 DC.L $001B75FF
 DC.L $001C7BFF
 DC.L $001C7BFF
 DC.L $001D80FF
 DC.L $001E80FF
 DC.L $001F86FF
 DC.L $002087FF
 DC.L $00208CFF
 DC.L $00218DFF
 DC.L $002292FF
 DC.L $002393FF
 DC.L $002498FF
 DC.L $002599FF
 DC.L $00259EFF
 DC.L $00269EFF
 DC.L $0027A4FF
 DC.L $0028A4FF
 DC.L $0029AAFF
 DC.L $0029AAFF
 DC.L $002AAFFF
 DC.L $002BB0FF
 DC.L $002CB5FF
 DC.L $002DB5FF
 DC.L $002DBAFF
 DC.L $002EBBFF
 DC.L $002FC0FF
 DC.L $0030C0FF
 DC.L $0031C5FF
 DC.L $0031C5FF
 DC.L $0039CAFF
 DC.L $0033CBFF
 DC.L $0034D0FF
 DC.L $0035D0FF
 DC.L $0035D5FF
 DC.L $0036D5FF
 DC.L $0037DAFF
 DC.L $0038DAFF
 DC.L $0039DFFF
 DC.L $0039DFFF
 DC.L $003AE4FF
 DC.L $003BE4FF
 DC.L $003CE9FF
 DC.L $003DE9FF
 DC.L $003EEDFF
 DC.L $003EEDFF
 DC.L $003FF2FF
 DC.L $0040F2FF
 DC.L $0041F7FF
 DC.L $0042F7FF
 DC.L $0042FBFF
 DC.L $0043FBFF
 DC.L $0044FFFF
 DC.L $0045FFFF
 DC.L $0046FFFB
 DC.L $0046FFFB
 DC.L $0047FFF7
 DC.L $0048FFF7
 DC.L $0049FFF3
 DC.L $004AFFF3
 DC.L $004AFFEE
 DC.L $004BFFEF
 DC.L $004CFFEA
 DC.L $004DFFEB
 DC.L $004EFFE6
 DC.L $004EFFE6
 DC.L $004FFFE2
 DC.L $0050FFE3
 DC.L $0051FFDF
 DC.L $0052FFDF
 DC.L $0053FFDB
 DC.L $0053FFDB
 DC.L $0054FFD7
 DC.L $0055FFD7
 DC.L $0056FFD4
 DC.L $0057FFD4
 DC.L $0057FFD0
 DC.L $0058FFD0
 DC.L $0059FFCD
 DC.L $005AFFCD
 DC.L $005BFFC9
 DC.L $005BFFC9
 DC.L $005CFFC6
 DC.L $005DFFC6
 DC.L $005EFFC3
 DC.L $005FFFC3
 DC.L $005FFFBF
 DC.L $0060FFC0
 DC.L $0061FFBC
 DC.L $0062FFBD
 DC.L $0063FFBA
 DC.L $0063FFBA
 DC.L $0064FFB6
 DC.L $0065FFB7
 DC.L $0066FFB4
 DC.L $0067FFB4
 DC.L $0067FFB1
 DC.L $0068FFAE
 DC.L $0069FFAE
 DC.L $006AFFAB
 DC.L $006BFFAC
 DC.L $006CFFA9
 DC.L $006CFFA9
 DC.L $006DFFA6
 DC.L $006EFFA7
 DC.L $006FFFA4
 DC.L $0070FFA4
 DC.L $0070FFA1
 DC.L $0071FFA2
 DC.L $0072FF9F
 DC.L $0073FFA0
 DC.L $0074FF9D
 DC.L $0074FF9D
 DC.L $0075FF9B
 DC.L $0076FF9B
 DC.L $0077FF99
 DC.L $0078FF99
 DC.L $0078FF96
 DC.L $0079FF97
 DC.L $007AFF95
 DC.L $007BFF96
 DC.L $007CFF93
 DC.L $007CFF93
 DC.L $007DFF91
 DC.L $007EFF92
 DC.L $007FFF90
 DC.L $0080FF90
 DC.L $0080FF8E
 DC.L $0080FF8E
 DC.L $0081FF8C
 DC.L $0082FF8D
 DC.L $0083FF8B
 DC.L $0084FF8C
 DC.L $0084FF89
 DC.L $0085FF8A
 DC.L $0086FF88
 DC.L $0087FF89
 DC.L $0089FF88
 DC.L $0089FF88
 DC.L $008DFF89
 DC.L $008EFF8A
 DC.L $0092FF8B
 DC.L $0093FF8C
 DC.L $0095FF8C
 DC.L $0096FF8D
 DC.L $009AFF8E
 DC.L $009BFF8F
 DC.L $009EFF90
 DC.L $009EFF90
 DC.L $00A2FF91
 DC.L $00A3FF92
 DC.L $00A6FF93
 DC.L $00A7FF94
 DC.L $00AAFF95
 DC.L $00AAFF95
 DC.L $00ADFF96
 DC.L $00AEFF97
 DC.L $00B1FF98
 DC.L $00B2FF99
 DC.L $00B5FF99
 DC.L $00B5FF9A
 DC.L $00B8FF9B
 DC.L $00B9FF9C
 DC.L $00BCFF9D
 DC.L $00BCFF9D
 DC.L $00BFFF9E
 DC.L $00C0FF9F
 DC.L $00C3FFA0
 DC.L $00C3FFA1
 DC.L $00C5FFA1
 DC.L $00C6FFA2
 DC.L $00C9FFA3
 DC.L $00C9FFA4
 DC.L $00CCFFA5
 DC.L $00CCFFA5
 DC.L $00CFFFA6
 DC.L $00CFFFA7
 DC.L $00D2FFA8
 DC.L $00D2FFA9
 DC.L $00D4FFA9
 DC.L $00D5FFAA
 DC.L $00D7FFAB
 DC.L $00D8FFAC
 DC.L $00DAFFAD
 DC.L $00DBFFAE
 DC.L $00DDFFAE
 DC.L $00DDFFAF
 DC.L $00DFFFB0
 DC.L $00E0FFB1
 DC.L $00E2FFB2
 DC.L $00E2FFB2
 DC.L $00E4FFB3
 DC.L $00E5FFB4
 DC.L $00E7FFB5
 DC.L $00E7FFB6
 DC.L $00E9FFB6
 DC.L $00E9FFB7
 DC.L $00EBFFB8
 DC.L $00EBFFB9
 DC.L $00EDFFBA
 DC.L $00EDFFBA
 DC.L $00EFFFBB
 DC.L $00EFFFBC
 DC.L $00F1FFBD
 DC.L $00F1FFBE
 DC.L $00F3FFBE
 DC.L $00F3FFBF
 DC.L $00F5FFC0
 DC.L $00F5FFC1
 DC.L $00F6FFC2
 DC.L $00F7FFC3
 DC.L $00F8FFC3
 DC.L $00F8FFC4
 DC.L $00FAFFC5
 DC.L $00FAFFC6
 DC.L $00FBFFC7
 DC.L $00FBFFC7
 DC.L $00FCFFC8
 DC.L $00FDFFC9
 DC.L $00FEFFCA
 DC.L $00FEFFCB
 DC.L $00FFFFCB
 DC.L $00FFFFCC
 DC.L $00FFFFCD
garnet_palette2:
 ds.l	256
garnet_palette3:
 ds.l	512

*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><*
