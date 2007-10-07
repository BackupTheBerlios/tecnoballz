*-------------------------------------------------------------------------*
* ----> SUPER BOULDER DASH <----
* 	---> COMMENCE LE 7 FEVRIER 1994 <---
* 		--> ALL CODING BY NOUNOURS <--
* 			-> MUSICS, SFX, & GFX BY ZBB <-
*-------------------------------------------------------------------------*
		SECTION	BRIDERS,CODE
		OPT	P+		pas de table de relogement
		OPT	C20+		labels et symboles 20 caractères maxi.,  majuscules/minuscules respectées
		OPT	p=68020		assemble les instructions MC68020
		INCDIR	DH0:Data/
		INCDIR	DH1:Data/

*>>>>>>>>>>>>>>>>>>>>>>>>>>* CONSTANTES MENTAT *<<<<<<<<<<<<<<<<<<<<<<<<<<*
NReservation	=50	50 adresses mémoires réservées maximums
FLAG_AMIGADOS	=1	1=assemble routines chargement AmigaDOS
FLAG_ARP	=0	1=ouvre la librairie ARP
FLAG_BOB32	=0	1=assemble routines BOBs (aff. écran 32 bits)
FLAG_BOBSTRUCT	=0	1=Assemble la structure BOB
FLAG_CLOSEWB	=0	1=ferme l'écran WorkBench
FLAG_DMACON.MSK	=0	1=ferme dma bitplane lors d'un accès disque-système
FLAG_EXIT	=0	0=sortie sous système possible avec la touche "ESC"
FLAG_FREQ	=0	1=assemble routine demande fréquence balayage
FLAG_ILBM	=1	1=assemble routines décompactage IFF-ILBM
FLAG_KEY	=1	1=assemble routines de test du clavier
FLAG_LUTIN	=1	1=assemble routines gestion des lutins
FLAG_MEMORY	=1	1=information sur la mémoire possible
FLAG_MOUSE0	=0	1=assemble la routine déplacement souris port 0
FLAG_MUSIC	=0	1=assemble routines gestion module "ProTracker"
FLAG_PP		=0	1=assemble routines décompactage PowerPacker
FLAG_SOUND	=0	1=assemble routines gestion des bruitages
FLAG_SCROLL	=0	1=assemble routines gestion défilement écran
FLAG_TDISK	=0	1=assemble routines de lecture en "TrackDisk"
FLAG_TDISKW	=0	1=assemble routines d'écriture en "TrackDisk"
FLAG_TRACKDISK	=0	1=assemble programme pour être chargé en "TrackDisk"

*/ CONSTANTES BOBS /*
SBOB_MAXI	=50
SBOB_MAXI16	=20
SBOB_MAXI32	=20
SBOB_MAXI48	=0
SBOB_MAXI64	=0
SBOB_MAXI80	=0
SBOB_MAXI96	=0
VSBOB_MAXI16	=0
VSBOB_MAXI32	=0
VSBOB_MAXI48	=0
VSBOB_MAXI64	=0
VSBOB_MAXI80	=0
VSBOB_MAXI96	=0

SE_COPPER	=2*4+10*5+1*4+2*4+2*4+10*5+1000
ZPEED		=7

*>>>>>>>>>>>>>>>>>>>>>>>>>* Debut code MC68020 *<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	INCLUDE	"Mentat/Include/mentat.s"
msg.pers	dc.b	"???????????????????????????????????????????"	¤-128
		EVEN

************* routine appelée avant de quitter le multitâche **************
sys_extra:
		*/ Charge la page des motifs */
		lea	(.name_motifs,pc),a0
		moveq	#0,d0
		bsr	charge_fichier_dos
		bne.s	.exit
		lea	(palette_jeu,pc),a2
		bsr	init_motifs
		bne.s	.exit
		move.l	d0,(pt_motifs-t,a5)
		move.w	d1,(wd_motifs-t,a5)

		*/ Charge la page SBOBs */
		lea	(.name_sbobs,pc),a0
		sub.l	a2,a2
		bsr	charge_page
		bne.s	.exit	
		move.l	d0,(sbob_pagegfx-t,a5)
		moveq	#0,d7
.exit		rts
.name_motifs:	dc.b	"DASH:Dash/Gfx/Motifs.ilbm",0
.name_sbobs:	dc.b	"DASH:Dash/Gfx/SBOB.ilbm",0
		EVEN
sys_extra2:	rts

*>>>>>>>>>>>>>>>>>>>>>>>>>* début du programme *<<<<<<<<<<<<<<<<<<<<<<<<<<*
* Entrée	a5.l	=> adr. 't'
*		a6.l	=> CUSTOM
run:		bsr	first_init
		bsr	sprites_off
		bsr	install_ecran
		bne	sys_erreur
		bsr	spritex
		bne	sys_erreur
		move.l	(pt_motifs-t,a5),d0
		move.w	(wd_motifs-t,a5),d1
		bsr	init_scroll
		bne	sys_erreur
		bsr	cartez
		bsr	scroll_affmap
		*bsr	go_fade_on
		bsr	colors_on
		bsr	sprites_on

		*/ Initialise les SBOBs /*
		lea	(tsbob-t,a5),a0
		move.l	(sbob_pagegfx-t,a5),d0
		bsr	sbob_main.init
		bne	sys_erreur
		moveq	#0,d0
		bsr	sbob.ajoute
		move.w	#100,(SBOB.POSX,a1)
		move.w	#100,(SBOB.POSY,a1)

		moveq	#1,d0
		bsr	sbob.ajoute
		move.w	#64,(SBOB.POSX,a1)
		move.w	#32,(SBOB.POSY,a1)

		moveq	#2,d0
		bsr	sbob.ajoute
		move.w	#50,(SBOB.POSX,a1)
		move.w	#100,(SBOB.POSY,a1)

		moveq	#3,d0
		bsr	sbob.ajoute
		move.w	#200,(SBOB.POSX,a1)
		move.w	#100,(SBOB.POSY,a1)
		move.l	a1,(boby-t,a5)

*¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤* BOUCLE PRINCIPALE HORS INTERRUPTION *¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤*
main_loop:	
		bsr	scroll_flip
		bsr	wait_vbl
		bsr	sbob_restore
		*/ Défilement de l'écran */
		move.w	(scroll.xspeed-t,a5),d4
		move.w	(scroll.yspeed-t,a5),d5
		bsr	scroll
		bsr	sbob_affiche
		move.l	(boby-t,a5),a3
		bsr	absolut_joymove

		*»»» INFORMATIONS SUR LA MEMOIRE «««*
		bsr	memory_help

		bsr	affx

		btst	#6,$bfe001
		bne.s	main_loop
		bra	sys_fin

******** Routine appelée par l'interruption du temps mort vertical ********
* Entrée	a5.l	=> Adr. 't'
*		a6.l	=> CUSTOM
extra_vbl:
		move.w	(vbl_counter-t,a5),d0
		addq.w	#1,d0
		add.w	d0,(hasard-t,a5)
		bsr	keytest
		rts


*/ Z'ON TEST LE CLAVIER (SANS BLAGUE!) */
keytest:
		moveq	#0,d1
		moveq	#0,d2

		move.l	(atable_clavier,pc),a0
		tst.w	(TOUCHE_SHIFT_G,a0)	SHIFT_G ou SHIFT_D ?
		bne.s	.yashift		-> oui
		tst.b	(TOUCHE_CAPSLOCK,a0)
		beq.s	.yapasshift
.yashift	
		lea	(TOUCHE_HAUT,a0),a1
		bsr	test_clavier
		beq.s	.no_haut
		moveq	#-1,d2
.no_haut	lea	(TOUCHE_BAS,a0),a1
		bsr	test_clavier
		beq.s	.no_bas
		moveq	#1,d2
.no_bas		lea	(TOUCHE_GAUCHE,a0),a1
		bsr	test_clavier
		beq.s	.no_gauche
		moveq	#-1,d1
.no_gauche	lea	(TOUCHE_DROITE,a0),a1
		bsr	test_clavier
		beq.s	.no_droite
		moveq	#1,d1
.no_droite	bra	.summer
.yapasshift

		moveq	#1,d3
		moveq	#-1,d4
		tst.b	(TOUCHE_ALT_G,a0)
		bne.s	.okag
		tst.b	(TOUCHE_AMIGA_G,a0)
		bne.s	.okamg
		tst.b	(TOUCHE_AMIGA_D,a0)
		beq.s	.bibi
		move.w	(hasard-t,a5),d3
		and.w	#7,d3
		addq.w	#1,d3
		move.w	d3,d4
		neg.w	d4
		bra.s	.bibi	
.okamg		moveq	#3,d3
		moveq	#-3,d4
		bra.s	.bibi
.okag		moveq	#8,d3
		moveq	#-8,d4
.bibi		tst.b	(TOUCHE_HAUT,a0)
		beq.s	.nohaut
		move.w	d4,d2
.nohaut		tst.b	(TOUCHE_BAS,a0)
		beq.s	.nobas
		move.w	d3,d2
.nobas		tst.b	(TOUCHE_GAUCHE,a0)
		beq.s	.nogauche
		move.w	d4,d1
.nogauche	tst.b	(TOUCHE_DROITE,a0)
		beq.s	.nodroite
		move.w	d3,d1
.nodroite
.summer		move.w	d1,(scroll.xspeed-t,a5)
		move.w	d2,(scroll.yspeed-t,a5)

		tst.b	(TOUCHE_R,a0)
		beq.s	.nor	
		not.b	(flag.raster-t,a5)
.nor		rts

************************* Installe l'écran de jeu *************************
* Entrée	a5.l	=> adr. table 't'
install_ecran:
		bsr	screen_reserve
		bne	.exit			-> oui
		move.w	#$0020,(SBPLCON3,a0)	COULEUR BORD NOIRE
		move.w	#3,(SFMODE_PLANS,a0)
		move.w	#3,(SFMODE_LUTINS,a0)
		move.w	#320,(SNPIXEL,a0)
		move.w	#256,(SNLIGNE,a0)
		move.b	#7,(SNPLAN,a0)
		move.w	#1,(SHARDSCROLL.F,a0)
		move.w	#SE_COPPER,(KKCOPPER,a0)
		move.w	#1,(NBCOPPER,a0)	Deux listes Copper
		move.w	#3,(SPBUFFER,a0)
		move.w	#320+64+64+64,(SPLARGE,a0)
		move.w	#256+16+64,(SPHAUTEUR,a0)
		move.w	#1,(SLUTIN,a0)		Pts lutins dans liste Copper
		move.w	#$24,(SBPLCON2,a0)
		move.w	#64,(SPRIGHT,a0)		64 pixels à droite
		move.w	#64+64,(SPLEFT,a0)		64 pixels à gauche
		move.w	#32,(SPUP,a0)		32 lignes en haut
		move.w	#32,(SPDOWN,a0)		32 lignes en bas
		move.w	(carte_width-t,a5),(SP_HSCROLL,a0)
		move.w	(carte_height-t,a5),(SP_VSCROLL,a0)
		bsr	init_screen
		bne.s	.exit
		move.w	#128-1,d0
		lea	(palette_jeu-t,a5),a1
		lea	(SCOLOR_TABLE,a0),a2
.zzloop		move.l	(a1)+,(a2)+
		dbf	d0,.zzloop
		lea	(SCOLOR_TABLE,a0),a2
		move.l	#$08000000,(a2)
		;move.l	(SADIW1,a0),a1
		;move.l	(SADIW2,a0),a2
		;sub.w	#16,(2,a1)
		;sub.w	#16,(2,a2)
		moveq	#0,d7
.exit		rts
cartez:
		move.l	(pt_scroll-t,a5),a4
		move.w	(SCROLL_WIDTH,a4),d0
		move.w	(SCROLL_HEIGHT,a4),d1
		lsr.w	#5,d0
		lsr.w	#5,d1
		subq.w	#1,d0
		subq.w	#1,d1
		move.w	#$7F,d3
		move	#38,d4
		move.l	(SCROLL_CARTE,a4),a0
		move.w	(SCROLL_LARGE,a4),d7
		lea	(VPOSR,a6),a1
		lea	(tablexx,pc),a2
.loopxx		
		move.w	d0,d6
.mortal		add.w	(a1),d2
		mulu.w	d6,d2
		add.w	d6,d2
		add.w	d1,d2
		and.w	d3,d2
		move.w	(a2,d2*2),d2
		*-*
		move.w	d2,(a0)
		addq.w	#2,d2
		move.w	d2,(2,a0)
		add.w	d4,d2
		move.w	d2,(a0,d7)
		addq.w	#2,d2
		move.w	d2,(2,a0,d7)
		*-*
		addq.l	#4,a0
		dbf	d6,.mortal
		add.w	d7,a0
		dbf	d1,.loopxx

		move.w	(SCROLL_HEIGHT,a4),d1
		lsr.w	#5,d1
		subq.w	#1,d1
		move.l	(SCROLL_CARTE,a4),a0
.aaa		move.l	#$00000002,(a0)
		move.l	#$0028002A,(a0,d7)
		add.w	d7,a0
		add.w	d7,a0
		dbf	d1,.aaa
		rts
tablexx		

V 	SET 0
	REPT	8
	dc.w	00+V,04+V,08+V,12+V,16+V,20+V,24+V,28+V,32+V,36+V
V 	SET V+80
	ENDR
V 	SET 0
	REPT	4
	dc.w	00+V,04+V,08+V,12+V,16+V,20+V,24+V,28+V,32+V,36+V
V 	SET V+80
	ENDR
	dc.w	00,04,08,12,16,20,24,28


***********************> Change la couleur de fond <***********************
* Entrée	=> d0.w	couleur de fond
*		=> a5.l	Adr. 't'
raster_line:
		tst.b	(flag.raster-t,a5)
		beq.s	.norast2
		move.l	(pt_screen-t,a5),a0
		move.w	(SBPLCON3,a0),(BPLCON3,a6)
		move.w	d0,(COLOR00,a6)
.norast2	rts

***************************************************************************
********************* MET EN PLACE UN LUTIN DE 64*256 *********************
***************************************************************************
spritex:
		move.l	#258*8*2,d0
		moveq	#2,d1
		bsr	reserve_memoire
		move.l	d0,(sprite0-t,a5)
		beq.s	.outmem
		lea	(table_lutins-t,a5),a0
		move.l	d0,(a0)
		bsr	init_pts_lutins2

		*/ Positionne le lutin en (0,0) */
		moveq	#0,d0
		moveq	#0,d1
		move.w	#256,d2
		move.l	(pt_screen-t,a5),a0
		move.w	(SCOORDX,a0),d6
		lsl.w	#2,d6
		move.w	(SCOORDY,a0),d7
		bsr	calcul_mot_ctrl_aga
		move.l	(sprite0-t,a5),a0
		move.w	d3,(8,a0)
		swap	d3
		move.w	d3,(a0)	
		moveq	#0,d7
		rts
.outmem		moveq	#-12,d7
		rts
		EVEN
sprite0		dc.l	0


affx:		rts
		move.l	(pt_scroll-t,a5),a4
		moveq	#0,d2
		move.w	(SCROLL_X,a4),d2
		bsr	convertit_decimal
		lea	(.t1+5,pc),a0
		moveq	#3-1,d1
		bsr	conv_number_ascii

		moveq	#0,d2
		move.w	(SCROLL_Y,a4),d2
		bsr	convertit_decimal
		lea	(.t2+5,pc),a0
		moveq	#3-1,d1
		bsr	conv_number_ascii

		lea	(.t3+4,pc),a0
		move.l	(SCROLL_OBORD,a4),d3
		moveq	#4-1,d1
		bsr	conv_number_ascii

		lea	(.t6+4,pc),a0
		moveq	#0,d3
		move.w	(t1,pc),d3
		moveq	#4-1,d1
		bsr	conv_number_ascii

		moveq	#0,d2
		move.w	(SCROLL_DECAL,a4),d2
		bsr	convertit_decimal
		lea	(.t4+6,pc),a0
		moveq	#2-1,d1
		bsr	conv_number_ascii

		moveq	#0,d2
		*move.w	(SCROLL_DECA2,a4),d2
		bsr	convertit_decimal
		lea	(.t5+6,pc),a0
		moveq	#2-1,d1
		bsr	conv_number_ascii

		move.l	(SCROLL_SBOB,a4),d2
		lsr.w	#2,d2
		bsr	convertit_decimal
		lea	(.t61+5,pc),a0
		moveq	#3-1,d1
		bsr	conv_number_ascii

		moveq	#0,d2
		move.w	(SCROLL_PTVIDEO,a4),d2
		bsr	convertit_decimal
		lea	(.t7+5,pc),a0
		moveq	#3-1,d1
		bsr	conv_number_ascii

		lea	(.text,pc),a0
		moveq	#0,d0
		moveq	#0,d1
		bsr	prints
		rts
.text		dc.b	"--------",0
.t1		dc.b	"X => 000",0
.t2		dc.b	"Y => 000",0
		dc.b	"--------",0
.t3		dc.b	"B=>$0000",0
.t6		dc.b	"P=>$0000",0
		dc.b	"--------",0
.t4		dc.b	"DECA1=00",0
.t5		dc.b	"DECA2=00",0
		dc.b	"--------",0
.t61		dc.b	"SBOB=000",0
.t7		dc.b	"PTVI=000",0

		dc.b	0
		EVEN
t1		dc.w	0


*=========================================================================*
**************** Affiche une chaine de caractère à l'écran ****************
*=========================================================================*
*----------------------- Affiche le text à l'écran -----------------------*
* Entrée	=> d0.w	Position x en pixel (multiple de 8)
*		=> d1.w	Position Y en pixel
*		=> a0.l	Adr. chaine de caractère
* 		=> a5.l	adr. table 't'
prints:
		moveq	#16,d3			d3=Taille ligne en octets
		move.l	d3,d4
		mulu	d3,d1			d1=Y * Nbre d'octets par ligne
		lsl.w	#3,d3
		move.l	(sprite0-t,a5),d2	d2=Adr. lutin
		add.l	d4,d2
		lsr.w	#3,d0
		add.w	d0,d1
		add.l	d1,d2			d2=Adr. aff. 1er caractère
		move.l	(adr_fonteraw-t,a5),d0
		moveq	#0,d1
		bra.s	.read_char

*		- Boucle d'affichage -
* Entrée	d0.l	Adr. police de 8*8
*		d1.l	#0
*		d2.l	Adr. d'affichage
;		d3.l	Offset prochaine ligne de caractère
*		d4.w	Offset prochaine ligne
*		a0.l	Adr. texte ASCII
.new_line	move.l	d2,a1			a1=Adr. d'affichage
.new_char	move.l	d0,a2			a2=Adr. police de caractère
		add.w	d1,a2			a2=Adr. caract. à afficher
		move.l	a1,a3			a3=Adr. d'affichage
		moveq	#8-1,d5			d5=7=hauteur caract. - 1
.aff_loop	move.b	(a2),(a3)		Affiche une ligne
		lea	(256,a2),a2		Ligne source suivante
		add.l	d4,a3			Ligne destination suivante
		dbf	d5,.aff_loop
		addq.l	#1,a1			X=X+1
		move.b	(a0)+,d1		Lit caractère
		bne.s	.new_char		-> Change pas de ligne
		add.l	d3,d2			d2=Adr. aff. prochaine ligne
.read_char	move.b	(a0)+,d1		Lit caractère
		bne.s	.new_line		-> Continue
.exit		rts


*-*-*-*-*-*-*-*-*-*-*-* Déplace un BOB au Joystick *-*-*-*-*-*-*-*-*-*-*-*-
* Entrée	=> a3.l Structure BOB
*		=> a5.l Adr. 't'
absolut_joymove:
		lea	(joy1_right-t,a5),a0
		tst.b	(a0)+
		beq.s	.no_right
		addq.w	#1,(SBOB.POSX,a3)
.no_right	tst.b	(a0)+
		beq.s	.no_left
		subq.w	#1,(SBOB.POSX,a3)
.no_left	tst.b	(a0)+
		beq.s	.no_down
		addq.w	#1,(SBOB.POSY,a3)
.no_down	tst.b	(a0)+
		beq.s	.no_up
		subq.w	#1,(SBOB.POSY,a3)
.no_up		rts

;øøøøøøøøøøøøøøøøøøøøøøøøøøø		       øøøøøøøøøøøøøøøøøøøøøøøøøøø;
;»»»»»»»»»»»»»»»»»»»»»»»»»»»  I N C L U D E S  «««««««««««««««««««««««««««;
;øøøøøøøøøøøøøøøøøøøøøøøøøøø		       øøøøøøøøøøøøøøøøøøøøøøøøøøø;
* DEFIL *
	INCLUDE	"Dash/Includes/Defil/init_motifs.s"
	INCLUDE	"Dash/Includes/Defil/init_scroll.s"
	INCLUDE	"Dash/Includes/Defil/defil_all.s"
	INCLUDE	"Dash/Includes/Defil/scroll.s"
	INCLUDE	"Dash/Includes/Defil/scroll_flip.s"
* SBOB *
	INCLUDE	"Dash/Includes/SBOB/SBob_Data.s"
	INCLUDE	"Dash/Includes/SBOB/sbob_calcul_msk.s"
	INCLUDE	"Dash/Includes/SBOB/sbob_affiche.s"
	INCLUDE	"Dash/Includes/SBOB/sbob_restore.s"
	INCLUDE	"Dash/Includes/SBOB/sbob_main.init.s"
	INCLUDE	"Dash/Includes/SBOB/sbob_init_liste.s"
	INCLUDE	"Dash/Includes/SBOB/sbob_initbob.s"
	INCLUDE	"Dash/Includes/SBOB/sbob.ajoute_bob.s"
	INCLUDE	"Dash/Includes/SBOB/tsbob.s"


*================= Structure défilement multidirectionnel ================*
*	signe 'ø' signifie que la valeur une fois initialisé dans 
*	la routine 'init_scroll' est fixe
		RSRESET
SCROLL:		rs	0
SCROLL_WINDO	rs	1 ø	Fenêtre Blitter taille d'un motif
SCROLL_WIND2	rs	1 ø	Fenêtre Blitter copie de la bande horz.
SCROLL_NEXT	rs	1 ø	Offset écran prochaine ligne d'un motif
SCROLL_MOD	rs	2 ø	Modulo source & destination pour 1 motif
SCROLL_MOD2	rs	2 ø	Modulo recopie de la bande horizonatle
SCROLL_MOD3	rs	2 ø	Modulo recopie de la bande verticale
SCROLL_NLINE	rs	1 ø	Nbre de lignes écran + 16
SCROLL_NLINE2	rs	1 ø	Nbre de lignes de la page écran
SCROLL_SLINE	rs	1 ø	Taille d'une ligne page écran
SCROLL_LARGE	rs	1 ø	Taille d'une ligne dans la carte
SCROLL_MAXX	rs	1 ø	Pos. x maxi coin supérieur gauche écran
SCROLL_MAXY	rs	1 ø	Pos. y maxi coin supérieur gauche écran
SCROLL_DLARG	rs	1 ø	Nbre de motifs horizontalement - 1 à l'écran
SCROLL_DHAUT	rs	1 ø	Nbre de motifs verticalement - 1 à l'écran
SCROLL_WIDTH	rs	1 ø	Largeur en pixels d'un niveau
SCROLL_HEIGHT	rs	1 ø	Hauteur en lignes d'un niveau
SCROLL_LMOTIF	rs	1 ø	Largeur page motifs en octets
SCROLL_GY_MAX	rs	1 ø	Valeur maximum du compteur 'scroll_gy'
SCROLL_NPLAN	rs	1 ø	Nombre de plans - 1
SCROLL_PLAN	rs	1 ø	Nombre de plans
SCROLL_COORDY	rs	1 ø	Coordonnée Y de la première ligne
SCROLL_SPLAN	rs	1 ø	Taille d'un plan RAW-BLITTER
SCROLL_X	rs	1	Pos. x en pixels coin supérieur gauche écran
SCROLL_Y	rs	1	Pos. y en pixels coin supérieur gauche écran
SCROLL_DECAL	rs	1	Décalage hardware écran de 0 à 15
SCROLL_GY	rs	1	Comp. Y défilement vert. (sert à pointer sur la 'vtable')
SCROLL_PTVIDEO	rs	1	N° ligne chg. des pts vidéos (1=pas de chg.)
SCROLL_MOCHAN	rs	1	Nombre de motifs avant chgs pointeurs vidéos
SCROLL_MAPBAS	rs.l	1 ø	Offset dernière ligne carte à l'écran par rapport à la 1ere ligne carte
SCROLL_CARTE	rs.l	1 ø	Adr. de la carte
SCROLL_MOTIF	rs.l	1 ø	Adr. des motifs
SCROLL_ECR1	rs.l	1 ø	Adresse écran 1
SCROLL_ECR2	rs.l	1 ø	Adresse écran 2
SCROLL_ECR3	rs.l	1 ø	Adresse écran 3
SCROLL_VTABLE	rs.l	1 ø	Adresse de la 'vtable'
SCROLL_MULU	rs.l	1 ø	Adresse table multiplication de la carte
SCROLL_GAUCHE	rs.l	1	Adresse motif haut à gauche de l'écran
SCROLL_WORKS	rs.l	1	Adresse écran de travail
SCROLL_DECALAGE	rs.l	1	Adr. de la liste Copper "MOVE #$????,BPLCON1"
SCROLL_POINTEUR	rs.l	1	Adr. liste Copper "MOVES" pointeurs vidéo
SCROLL_SCROLLPT	rs.l	1	Adr. liste Copper changement pts vidéo
SCROLL_VTFIRSTL	rs.l	1	Adr. relative première ligne balayée
SCROLL_VTAFFHAUT rs.l	1	Adr. relative affichage 1ere ligne motifs
SCROLL_VTAFFBAS	rs.l	1	Adr. relative affichage dernière ligne motifs
SCROLL_AF.HAUT	rs.l	1	Sg. de 'scroll_affhaut2' (détermine si nouvel aff. de motifs)
SCROLL_AVCOPY	rs.l	1	Adr. relative de la copie verticale
SCROLL_AHCOPY	rs.l	1	Flag/adr. relative recopie bande horz.
SCROLL_OFF.UP	rs.l	1 ø	Nbre d'octets cachés en haut
SCROLL_OFF.RG	rs.l	1 ø	Offset affichage motifs à droite de l'écran
SCROLL_WINDOW3	rs.l	1 ø	Fenêtre Blitter copie de la bande vert.
SCROLL_BORD	rs.l	1
SCROLL_OBORD	rs.l	1	Offset défilement horizontal
SCROLL_OLEFT	rs.l	1 ø	Nbre d'octets "cachés" à gauche
SCROLL_SBOB	rs.l	1
SCROLL_SBOB.A1	rs.l	1	Adr. écran travail 1
SCROLL_SBOB.A2	rs.l	1	Adr. écran travail 2
SCROLL_SBOB.B1	rs.l	1	Adr. écran travail 1 (deuxième fenêtre Blitter)
SCROLL_SBOB.B2	rs.l	1	Adr. écran travail 2 (deuxième fenêtre Blitter)
SCROLL_SIZE	rs	0
*>>>>>>>>>>>>>>>>>* Adresses utilisées par le programme *<<<<<<<<<<<<<<<<<*
		CNOP	0,4
palette_jeu:	dcb.l	128,0
boby		dc.l	0
pt_motifs	dc.l	0	adr. page motif
wd_motifs	dc.w	0	largeur page motif
carte_width:	dc.w	1600
carte_height:	dc.w	1600
scroll.xspeed	dc.w	0
scroll.yspeed	dc.w	0
hasard		dc.w	0
flag.raster	dc.b	0	raster ON/OFF
		EVEN

		INCLUDE	"Mentat/Include/Systeme/t.s"
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
		dc.b	"$VER: BOULDER DASH (5.03.94)",0

*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
