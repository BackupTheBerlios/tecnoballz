*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
********************** ROUT=15 : Levels-Editors ***************************
*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
interupt15:
		clr.w	(flag_flip-t,a5)
		bsr.l	init_pts_lutins
		bsr.l	wait_vbl
		bsr.l	fire.color
		bsr	yourris			"interupt3.s"|-> Déplacement souris
		bsr	yneil3			£=> Dépassement souris
		bsr	gad			£=> Test selection gadget
		bsr	clara.ed
		bsr	gaga_fada		|-> Fade couleurs briques
		bsr	hasard_ok
		bsr	pertedememoire
		bsr	sh.mercryl		"interupt3.s" |-> Animation sprite pt souris
		bsr	ed.mercryl		£=> Calcul mots ctrl lutins
		bsr	quit
		RASTER	#$000
		bra	irq

*--------------------------------------------------------------------------*
*                    Test dépassement pointeur souris			   *
*--------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
yneil3		
		tst.w	(sh.mousex-t,a5)
		bpl.s	.ndxg_bis
		clr.w	(sh.mousex-t,a5)
.ndxg_bis	tst.w	(sh.mousey-t,a5)
		bmi.s	.factor2
		cmp.w	#1,(sh.mousey-t,a5)
		bhi.s	.ndyb_bis
.factor2	move.w	#1,(sh.mousey-t,a5)
.ndyb_bis	cmp.w	#319,(sh.mousex-t,a5)
		blt.s	.ndyh_bis
		move.w	#319,(sh.mousex-t,a5)
.ndyh_bis	cmp.w	#255,(sh.mousey-t,a5)
		blt.s	.ncoor
		move.w	#255,(sh.mousey-t,a5)
.ncoor		rts

*-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-*
*/-/-/-/-/-/-/-/-/-/-/-/-* test selection gadget *-/-/-/-/-/-/-/-/-/-/-/-/*
*-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-*
gad:	
		lea	(gadget,pc),a0
		moveq	#(tov-gadget)/24-1,d0
.bois		moveq	#0,d1
		move.w	(sh.mousex-t,a5),d1
		moveq	#0,d2
		move.w	(sh.mousey-t,a5),d2
.test		cmp.l	(a0),d1
		blt.s	.pasca
		cmp.l	(4,a0),d2
		blt.s	.pasca
		cmp.l	(8,a0),d1
		bhi.s	.pasca
		cmp.l	(12,a0),d2
		bhi.s	.pasca
		tst.b	(viking.right-t,a5)	Bouton droit ?
		bne.s	.light			-> oui
		tst.b	(viking.left-t,a5)	Bouton gauche ?
		beq.s	.hicar			-> non
		move.l	(16,a0),a1
		add.l	a5,a1
		jmp	(a1)
.light		move.l	(20,a0),a1
		add.l	a5,a1
		jmp	(a1)
		rts
.pasca		lea	(24,a0),a0	
		dbf	d0,.test
.hicar		rts
		*	 x1, y1, x2, y2,  Left,Right
		CNOP	0,4
gadget:		dc.l	256,176,319,183,star2-t,star2-t	* Level Play *
		dc.l	256,016,287,023,sax-t,sax-t	* Save File * 
		dc.l	256,024,287,031,lax-t,lax-t	* Load File *
		dc.l	256,048,287,055,snm-t,snm2-t	* Fichier MOINS *
		dc.l	288,048,319,055,snp-t,snp2-t	* Fichier PLUS *
		dc.l	256,072,287,079,lem-t,lem2-t	* TABLEAU MOINS *
		dc.l	288,072,319,079,lep-t,lep2-t	* TABLEAU PLUS *
		dc.l	256,088,287,095,aem-t,aem2-t	* AREA + *
		dc.l	288,088,319,095,aep-t,aep2-t	* AREA - *
		dc.l	256,104,287,111,lem-t,lem2-t	* LEVEL + *
		dc.l	288,104,319,111,lep-t,lep2-t	* LEVEL - *
		dc.l	256,128,287,135,cosm-t,cosm2-t	* COPIE SOURCE MOINS *
		dc.l	288,128,319,135,cosp-t,cosp2-t	* COPIE SOURCE PLUS *
		dc.l	256,136,287,143,codm-t,codm2-t	* COPIE DESTI. MOINS *
		dc.l	288,136,319,143,codp-t,codp2-t	* COPIE DESTI. PLUS *
		dc.l	256,152,287,159,ecsm-t,ecsm2-t	* ECHANGE SOURCE MOINS *
		dc.l	288,152,319,159,ecsp-t,ecsp2-t	* ECHANGE SOURCE PLUS *
		dc.l	256,160,287,167,ecdm-t,ecdm2-t	* ECHANGE DESTI. MOINS *
		dc.l	288,160,319,167,ecdp-t,ecdp2-t	* ECHANGE DESTI. PLUS *
		dc.l	256,120,319,127,copy-t,copy-t	* COPIE *
		dc.l	256,144,319,151,zwap-t,zwap-t	* ECHANGE *
		dc.l	048,BRIY,207,BRIY2,etb3-t,etb1-t	* AFF. BRIQUE / POS. CURSEUR *
		dc.l	016,000,240,034,etb2-t,etb2-t	* SELECTIONNE BRIQUE *
		dc.l	256,000,319,007,kit-t,kit-t	* EXIT *
		dc.l	256,192,319,199,cls-t,set-t	* REMPLIT ECRAN *
tov:		dc.l	000,010,000,010,rt-t,rt-t
gadkey

*/=|=\=/=|=\=/=|=\=/=|=\=/=|=\=/=|=\=/=|=\=/=|=\=/=|=\=/=|=\=/=|=\=/=|=\=/*
*--------------------------* Fonctions gadgets *--------------------------*
*/=|=\=/=|=\=/=|=\=/=|=\=/=|=\=/=|=\=/=|=\=/=|=\=/=|=\=/=|=\=/=|=\=/=|=\=/*

*-----------------------------* Level Play *------------------------------*
* Entrée	=> a5.l	Adr. 't'
star2:
		bsr.l	go_fade_off
.test		bsr.l	wait_vbl
		bsr	gaga_fada		|-> Fade couleurs briques
		tst.w	(flag_fade_off-t,a5)
		bne.s	.test
		bsr	rupt15.off
		bsr.l	np_end
		clr.w	(niv00-t,a5)		raz pt/level
		clr.l	(triche-t,a5)
		IFEQ	A_TRICHE
		move.l	#ETB,(triche-t,a5)
		move.w	#1,(sh.code-t,a5)
		ENDC
		bsr	init_players
		bsr	recopie_noms
		clr.b	(flag.quit-t,a5)
		move.l	(niveau-t,a5),d2
		divu	#TB.SIZE,d2
		divu	#10,d2
		and.l	#$FFFF,d2
		addq.w	#1,d2
		move.w	d2,(area-t,a5)		sauve n° area
		move.l	(niveau-t,a5),d2
		divu	#TB.SIZE,d2
		divu	#10,d2
		swap	d2
		and.l	#$FFFF,d2
		move.w	d2,d0
		mulu	#TB.SIZE,d0
		move.w	d0,(niv00-t,a5)
		addq.w	#1,d2
		cmp.w	#6,d2
		blt.s	.nogardien
		addq.w	#1,d2
.nogardien	move.w	d2,(level-t,a5)
		bsr.l	convertit_decimal
		move.b	d3,(1+lev0-t,a5)
		lsr.w	#4,d3
		move.b	d3,(1+lev1-t,a5)
		clr.w	(mur.kas_lf-t,a5)
		clr.w	(mur.kas_up-t,a5)
		clr.w	(mur.kas_rg-t,a5)
		clr.w	(score.life-t,a5)
		clr.w	(lessbrick-t,a5)
		clr.w	(gameover-t,a5)
		clr.w	(musiclost-t,a5)
		move.w	#10,(lessbrick.count-t,a5)
		bsr	raton_laveur		|-> init. 6 structures joueurs
		bsr	playerlive		|-> init. le nbre de vie 
		bsr	firstfirst		|-> pointe le 1er joueur
		clr.w	(gd.pt-t,a5)		raz pt/sur les listes gardiens
		bsr	super_famicom		|-> copie n° level & area dans liste joueur
		lea	(b1-t,a5),a0
		moveq	#2,d1
		move.w	(area-t,a5),d0		n° area
		subq.w	#2,d0
		bmi.s	.sex
		move.w	d1,(2*BUMP.SIZE+BUMP.ACTIF,a0)
		subq.w	#1,d0
		bmi.s	.sex
		move.w	d1,(1*BUMP.SIZE+BUMP.ACTIF,a0)
		subq.w	#1,d0
		bmi.s	.sex
		move.w	d1,(3*BUMP.SIZE+BUMP.ACTIF,a0)
.sex		clr.w	(rout-t,a5)		>flag prepare level<
		move.l	#irq,(a7)
		rts
*-------------------------------*Save File*-------------------------------*
sax:		
		bsr.l	clear_ecran1
		bsr.l	sprites_off
		moveq	#0,d0
		moveq	#124,d1
		lea	(axx1-t,a5),a0
		bsr.l	printf
.l6		bsr.l	wait_vbl
		lea	(COLOR01,a6),a2
		bsr.l	xxxcolor
		move.l	(atable_clavier-t,a5),a0
		tst.b	(TOUCHE_N,a0)
		bne.s	.exit
		tst.b	(TOUCHE_Y,a0)
		beq.s	.l6
		lea	(filex-t,a5),a0
		move.l	(pt_tableaux-t,a5),d0
		move.l	#TB.SIZE*50,d1
		bsr.l	save_file
.exit		bsr	topla
		rts
**------------------------------* Load File *----------------------------**
lax:
		bsr.l	clear_ecran1
		bsr.l	sprites_off
		moveq	#0,d0
		moveq	#124,d1
		lea	(axx2-t,a5),a0
		bsr.l	printf
.l7		bsr.l	wait_vbl
		lea	(COLOR01,a6),a2
		bsr.l	xxxcolor
		move.l	(atable_clavier-t,a5),a0
		tst.b	(TOUCHE_N,a0)
		bne.s	.exit
		tst.b	(TOUCHE_Y,a0)
		beq.s	.l7
		lea	(filex-t,a5),a0
		lea	(pt_tableaux-t,a5),a4
		bsr.l	charge.pp
.exit		bsr	topla
		rts
*-----------------------------* Fichier +/- *-----------------------------*
snp		moveq	#1,d1
		bra.s	nafnaf
snp2		moveq	#10,d1
		bra.s	nafnaf
snm		moveq	#-1,d1
		bra.s	nafnaf
snm2		moveq	#-10,d1
nafnaf		move.w	#100,d0
		lea	(snif-t,a5),a0
		add.w	d1,(a0)
		bpl.s	.positif
		add.w	d0,(a0)
.positif	cmp.w	(a0),d0
		bhi.s	.ok
		sub.w	d0,(a0)
.ok		moveq	#0,d2
		move.w	(a0),d2
		bsr.l	convertit_decimal
		moveq	#1,d1
		lea	(3+fix-t,a5),a0
		bsr.l	conv_number_ascii
		lea	(3+fix-t,a5),a0
		lea	(filexx-t,a5),a1
		move.b	(a0)+,(a1)+
		move.b	(a0)+,(a1)+
		bsr	edmenu.print
		rts

*********************************/ EXIT */*********************************
kit:		
		move.b	#1,(flag.quit-t,a5)
		rts

**************/ Sélectionne une brique dans le menu brique /***************
* Entrée	=> a5.l	Adr. 't'
etb2:
		move.w	(sh.mousex-t,a5),d0
		move.w	(sh.mousey-t,a5),d1
		cmp.w	#028,d1
		blt.s	.geo
		cmp.w	#128,d0
		blt.s	.geo
		rts
.geo		bra	calcul.neo	

*-------------------*/ Affiche brique avec pointeur /*--------------------*
* Entrée	=> a5.l	Adr. 't'
etb3:
		bsr	pointe.mouse
		move.w	d0,(nix-t,a5)
		move.w	d1,(niy-t,a5)
		move.w	(sh.mousex-t,a5),(spx+2-t,a5)
		move.w	(sh.mousey-t,a5),(spy+2-t,a5)
		and.w	#$FFF0,(spx+2-t,a5)
		and.w	#$FFF8,(spy+2-t,a5)
outsiders	bsr	pointetab
		move.w	(cbs-t,a5),d0
		move.w	d0,(a0)
		bsr	set.bricks
		rts

*----------/* Positionne curseur à l'aide du pointeur souris /*-----------*
* Entrée	=> a5.l	Adr. 't'
etb1:
		bsr	pointe.mouse
		move.l	(pt_tableaux-t,a5),a0
		add.l	(niveau-t,a5),a0
		add.w	d0,a0
		add.w	d1,a0
		move.w	(a0),d0			d0=code brique selectionné
		moveq	#0,d1
		move.w	#-2,d1
		lea	(bg-t,a5),a1
.for		addq.w	#2,d1
		cmp.w	(a1)+,d0
		bne.s	.for
		tst.b	d0			x=0 (pas de brique)
		bne.s	.no0
		move.w	d0,d2
		lsr.w	#8,d2
		cmp.b	#8,d2			dernière ligne (brique indes.)
		beq.s	.no0			-> non
		clr.w	d0
.no0		move.w	d0,(cbs-t,a5)
		move.w	d1,(neo-t,a6)
		lsr.w	#1,d1
		divu	#14,d1
		move.l	d1,d0
		swap	d0
		mulu	#7,d1	
		lsl.w	#4,d0
		add.w	#16,d0
		move.w	d0,(zpx+2-t,a5)
		move.w	d1,(zpy+2-t,a5)
		rts

*-------------------------*/ Efface le tableau /*-------------------------*
set:		
		move.w	(cbs-t,a5),d1
		bra.s	more
cls:		moveq	#0,d1
more		move.l	(pt_tableaux-t,a5),a0
		add.l	(niveau-t,a5),a0
		move.w	#NIVEA-1,d0
.clz		move.w	d1,(a0)+
		dbf	d0,.clz
		bsr	set.bricks
		rts

**************************/ Echange 2 tableaux /***************************
zwap:	
		move.l	(pt_tableaux-t,a5),a0
		move.l	a0,a1	
		add.l	(ecs-t,a5),a0
		add.l	(ecd-t,a5),a1
		move.w	#NIVEA-1,d0
.swaz		move.w	(a0),d1
		move.w	(a1),(a0)+
		move.w	d1,(a1)+
		dbf	d0,.swaz
		bsr	set.bricks
		rts

***************************/ Copie 2 tableaux /****************************
copy:
		move.l	(pt_tableaux-t,a5),a0
		move.l	a0,a1	
		add.l	(cos-t,a5),a0
		add.l	(cod-t,a5),a1
		move.w	#NIVEA-1,d0
.copi		move.w	(a0)+,(a1)+
		dbf	d0,.copi
		bsr	set.bricks
		rts

*/----------- "Copie" & "Echange" modifie source-destination ------------/*
cosm		lea	(cos-t,a5),a0		COPY-SOURCE - 1
		moveq	#-1,d0
		bra	sof1
cosp		lea	(cos-t,a5),a0		COPY-SOURCE + 1
		moveq	#1,d0
		bra	sof1
codm		lea	(cod-t,a5),a0		COPY-DEST.  - 1
		moveq	#-1,d0
		bra	sof1
codp		lea	(cod-t,a5),a0		COPY-DEST.  + 1
		moveq	#1,d0
		bra	sof1
cosm2		lea	(cos-t,a5),a0		COPY-SOURCE - 10
		moveq	#-BEAU,d0
		bra.s	sof1
cosp2		lea	(cos-t,a5),a0		COPY-SOURCE + 10
		moveq	#BEAU,d0
		bra.s	sof1
codm2		lea	(cod-t,a5),a0		COPY-DEST.  - 10
		moveq	#-BEAU,d0
		bra.s	sof1
codp2		lea	(cod-t,a5),a0		COPY-DEST.  + 10
		moveq	#BEAU,d0
		bra.s	sof1
ecsm		lea	(ecs-t,a5),a0		SWAP-SOURCE -1
		moveq	#-1,d0
		bra.s	sof1
ecsp		lea	(ecs-t,a5),a0		SWAP-SOURCE +1
		moveq	#1,d0
		bra.s	sof1
ecdm		lea	(ecd-t,a5),a0		SWAP-DEST.  - 1
		moveq	#-1,d0
		bra.s	sof1
ecdp		lea	(ecd-t,a5),a0		SWAP-DEST. +1
		moveq	#1,d0
		bra.s	sof1
ecsm2		lea	(ecs-t,a5),a0		SWAP-SOURCE +10
		moveq	#-BEAU,d0
		bra.s	sof1
ecsp2		lea	(ecs-t,a5),a0		SWAP-SOURCE +10
		moveq	#BEAU,d0
		bra.s	sof1
ecdm2		lea	(ecd-t,a5),a0		SWAP-DEST. -10
		moveq	#-BEAU,d0
		bra.s	sof1
ecdp2		lea	(ecd-t,a5),a0		SWAP-DEST. +10
		moveq	#BEAU,d0
sof1		bsr	gonivea
		bsr	peste
		bsr	edmenu.print
		rts	

*/**************************/ Change tableau */**************************/*
tam		moveq	#-BEAU/2,d0		LEVEL - 05
		bra.s	soft
tap		moveq	#BEAU/2,d0		LEVEL + 05
		bra.s	soft
aem		moveq	#-BEAU,d0		LEVEL - 10
		bra.s	soft
aep		moveq	#BEAU,d0		LEVEL + 10
		bra.s	soft
aem2		moveq	#-BEAU*2,d0		LEVEL - 20
		bra.s	soft
aep2		moveq	#BEAU*2,d0		LEVEL + 20
		bra.s	soft
lem		moveq	#-1,d0			LEVEL - 01
		bra.s	soft
lep		moveq	#1,d0			LEVEL + 01
		bra.s	soft
lem2		moveq	#-2,d0			LEVEL - 02
		bra.s	soft
lep2		moveq	#2,d0			LEVEL + 02
		bra.s	soft
rt:		rts
soft		lea	(niveau-t,a5),a0
		bsr	gonivea
		bsr	topazimov
		bsr	set.bricks
		bsr	edmenu.print
		rts

*><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*
*--------------------------*  TEST DU CLAVIER  *--------------------------*
*><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*
* Entrée	=> a5.l	Adr. 't'
clara.ed:
		move.l	(atable_clavier-t,a5),a0
		tst.b	(TOUCHE_RETURN,a0)
		bne	outsiders

*------------------*/ Selectionne couleur des briques */------------------*
		lea	(kx8-t,a5),a0
		tst.b	(a0)+
		beq.s	.fnu287a
		tst.w	(avu-t,a5)
		bne.s	.no0
		move.w	#10,(avu-t,a5)
.no0		subq.w	#1,(avu-t,a5)
		bra	topla
.fnu287a	tst.b	(a0)+
		beq.s	joseph
		cmp.w	#9,(avu-t,a5)
		bne.s	.nox
		move.w	#-1,(avu-t,a5)
.nox		addq.w	#1,(avu-t,a5)
		bra	topla

*--------------------*/ Selectionne couleur de fond */--------------------*
joseph:
		lea	(kx7-t,a5),a0
		tst.b	(a0)+
		beq.s	.fnu2
		tst.w	(avo-t,a5)
		bne.s	.no0
		move.w	#60,(avo-t,a5)
.no0		subq.w	#1,(avo-t,a5)
		bra	topla
.fnu2		tst.b	(a0)+
		beq.s	jfk
		cmp.w	#59,(avo-t,a5)
		bne.s	.nox
		move.w	#-1,(avo-t,a5)
.nox		addq.w	#1,(avo-t,a5)
		bra	topla

*------------------------*/ Selectionne le fond */------------------------*
jfk:
		lea	(kx6-t,a5),a0
		tst.b	(a0)+
		beq.s	.fondu2
		tst.w	(ava-t,a5)
		bne.s	.no0
		move.w	#60,(ava-t,a5)
.no0		subq.w	#1,(ava-t,a5)
		bra	topla
.fondu2		tst.b	(a0)+
		beq.s	jb1
		cmp.w	#59,(ava-t,a5)
		bne.s	.nox
		move.w	#-1,(ava-t,a5)
.nox		addq.w	#1,(ava-t,a5)
		bra	topla

*--------------------*/ Selection du jeu de briques */--------------------*
jb1:
		lea	(kx5-t,a5),a0
		tst.b	(a0)+
		beq.s	.jb2
		tst.w	(avi-t,a5)
		bne.s	.no0	
		move.w	#10,(avi-t,a5)
.no0		subq.w	#1,(avi-t,a5)
		bra	topla
.jb2		tst.b	(a0)+
		beq	wings
		cmp.w	#9,(avi-t,a5)
		bne.s	.nox
		move.w	#-1,(avi-t,a5)
.nox		addq.w	#1,(avi-t,a5)
topla:		move.w	#$0180,(DMACON,a6)	désactive DMA copper & plans
		and.w	#$FE7F,(dmacon-t,a5)
		bsr.l	raz_256colors
		move.l	(pt_screen-t,a5),a0
		bsr.l	raz_scolor_table2
		bsr.l	clear_ecran1
		bsr	install_ecran3		|-> remplit plan 3
		bsr.l	init_fond
		bne.l	sys_erreur
		bsr	init_degrade
		bsr.l	copy_s1_to_s2
		bsr	init_bricks
		bne.l	sys_erreur
		bsr	broque			|=> Affiche le menu Brique
		bsr	topazimov		|=> Affiche le numéro du level
		bsr	peste			|=> SWAP & COPY
		bsr	cheval			£=> N° Fond & Briques
		bsr	edmenu.print		|=> Affiche barre des menus
		bsr	filou			|=> Couleur menus
		bsr	set.bricks		£=> Affiche les briques
		bsr.l	colors_on
		bsr	gagaona			£=> Initialise dégradé briques
		move.l	(pt_screen-t,a5),a0
		move.l	(SACOPPER,a0),d0
		move.l	d0,(COP1LCH,a6)
		move.l	d0,(acopper-t,a5)
		moveq	#0,d0
		move.w	d0,(COPJMP1,a6)
		move.w	(SBPLCON3,a0),(BPLCON3,a6)
		move.w	#$8380,(DMACON,a6)	active DMA copper + plans
		or.w	#$0180,(dmacon-t,a5)
		bsr.l	sprites_on
		rts

*>>>>>>>>>>>>>>>*/ Mouvement du curseur sélection brique /*<<<<<<<<<<<<<<<*
wings:		
		lea 	(kx2-t,a5),a0
		moveq	#0,d2
		moveq	#0,d3
		tst.b	(a0)+			\/
		beq.s	.win2
		move.w	#7,d3
		bra.s	.cine
.win2		tst.b	(a0)+			/\
		beq.s	.win3
		move.w	#-7,d3
		bra.s	.cine
.win3		tst.b	(a0)+			<-
		beq.s	.win4
		move.w	#-16,d2
		bra.s	.cine
.win4		tst.b	(a0)+			->
		beq.s	.win5
		move.w	#16,d2
		bra.s	.cine
.win5		bra	clav2
.cine		move.l	(zpx-t,a5),d0		d0=pos. X curseur
		move.l	(zpy-t,a5),d1		d1=pos. Y curseur
		add.w	d2,d0			+déplacement X	
		add.w	d3,d1			+déplacement Y
		blt	rt			dépassement en haut
		cmp.w	#16,d0			dépassement à gauche ?
		blt	rt			-> oui
		cmp.w	#224,d0			dépassement à droite ?
		bhi	rt			-> oui
		cmp.w	#28,d1			dépassement en bas ?
		bhi	rt			-> oui
		cmp.w	#28,d1
		blt.s	.geo2
		cmp.w	#128,d0
		blt.s	.geo2
		rts
.geo2		move.w	d0,d2
		move.w	d1,d3
		bsr	calcul.neo
		move.w	d2,(zpx+2-t,a5)
		and.w	#$FFF0,(zpx+2-t,a5)
		moveq	#0,d0
		move.w	d3,d0
		divu	#7,d0
		mulu	#7,d0
		move.w	d0,(zpy+2-t,a5)
		rts

*-------------------*/ Mouvement du curseur tableau */--------------------*
clav2:
		lea	(kx1-t,a5),a0
		tst.b	(a0)+			<-
		beq.s	clav3
		tst.w	(nix-t,a5)
		beq.s	clav2f
		subq.w	#2,(nix-t,a5)
		sub.l	#16,(spx-t,a5)
clav2f		rts
clav3		tst.b	(a0)+			->
		beq.s	clav6
		cmp.w	#18,(nix-t,a5)
		beq.s	clav2f
		addq.w	#2,(nix-t,a5)
		add.l	#16,(spx-t,a5)
		rts
clav6		tst.b	(a0)+			/\
		beq.s	clav7
		tst.w	(niy-t,a5)
		beq.l	clav2f
		sub.w	#20,(niy-t,a5)
		subq.l	#8,(spy-t,a5)
		rts
clav7		tst.b	(a0)+			\/
		beq.s	clav4
		cmp.w	#320,(niy-t,a5)
		beq.l	clav2f
		add.w	#20,(niy-t,a5)
		addq.l	#8,(spy-t,a5)
		rts

***********************/ Change résistance brique */***********************
clav4:		
		lea	(kx4-t,a5),a0
		tst.b	(a0)+			Décrèmente la résistance  ?
		beq.s	.clav5			-> non
		bsr	pointetab
		addq.l	#1,a0
		tst.b	(a0)			X=0 ?
		bne.s	.nomini
		move.b	#BK.SPLAN,(a0)
.nomini		subq.b	#2,(a0)			X=X-2
		bne.s	.ok
		move.b	#BK.SPLAN-2,(a0)
.ok		bsr	set.bricks
		rts
.clav5		tst.b	(a0)+			Incrèmente la résistance  ?
		beq.s	clav18			-> Non
		bsr	pointetab
		addq.l	#1,a0
		addq.b	#2,(a0)
		cmp.b	#BK.SPLAN,(a0)		X=maxi ?
		blt.s	.nomaxi			-> non
		move.b	#2,(a0)
.nomaxi		bsr	set.bricks
		rts

****************************/ Change tableau */****************************
clav18:
		lea	(kx3-t,a5),a0
		tst.b	(a0)+
		bne	lem
		tst.b	(a0)+
		bne	lep
		rts

*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
*---------------------*Calcul les mots de contrôles*----------------------*
*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
ed.mercryl:
		*/ Curseur tableau */
		move.l	(pt_screen-t,a5),a0
		move.w	(SCOORDX,a0),d6
		move.w	(SCOORDY,a0),d7
		moveq	#7,d2
		move.l	(spx-t,a5),d0
		move.l	(spy-t,a5),d1
		bsr.l	calcul_mot_ctrl
		move.l	(ed.sprite1-t,a5),a0
		move.l	d3,(a0)
		or.w	#$80,d3
		lea	(4*9,a0),a0
		move.l	d3,(a0)

		*/ Curseur brique */
		moveq	#7,d2
		move.l	(zpx-t,a5),d0
		move.l	(zpy-t,a5),d1
		bsr.l	calcul_mot_ctrl
		move.l	(ed.sprite2-t,a5),a0
		move.l	d3,(a0)
		or.w	#$80,d3
		lea	(4*9,a0),a0
		move.l	d3,(a0)
		rts

***************************************************************************

*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*
*---------------------->Pointe le tableau en cours<-----------------------*
*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*
* Entrée	=> a5.l	adr. 't'.
* Sortie	<= a0.l pt/la brique
pointetab:
		move.l	(pt_tableaux-t,a5),a0
		add.l	(niveau-t,a5),a0
		add.w	(nix-t,a5),a0
		add.w	(niy-t,a5),a0
		rts

*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*
*                      Calcul le numéro de la brique			  *
*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*
* Entrée	=> d0.w	Position X curseur menu brique (zpx)
* 		=> d1.w	Position Y curseur menu brique (zpy)
*		=> a5.l	adr. 't'.
* Sortie	<= d1.w	Numéro de la brique
*		<= d0.w	Code brique
calcul.neo:
		move.l	d2,-(a7)
		move.w	d0,(zpx+2-t,a5)
		and.w	#$FFF0,(zpx+2-t,a5)
		moveq	#0,d2
		move.w	d1,d2
		divu	#7,d2
		mulu	#7,d2
		move.w	d2,(zpy+2-t,a5)
		sub.w	#16,d0
		lsr.w	#4,d0
		and.l	#$FFFF,d1
		divu	#7,d1
		mulu	#14,d1
		add.w	d0,d1
		add.w	d1,d1
		move.w	d1,(neo-t,a5)
		move.w	(bg-t,a5,d1),d0		code brique (y,x)
		tst.b	d0			x=0 ?
		bne.s	.no0
		move.w	d0,d1
		lsr.w	#8,d1
		cmp.b	#8,d1			dernière ligne (brique indes.)
		beq.s	.no0			-> non
		clr.w	d0
.no0		move.w	d0,(cbs-t,a5)
		move.l	(a7)+,d2
		rts

*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*
*           Calcul pointeurs sur level à partir position souris		  *
*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*-*+*
* Entrée	=> a5.l	adr. 't'.
* Sortie	<= d0.w	pointeur colonne
*		<= d1.w pointeur ligne
pointe.mouse:
		move.w (sh.mousex-t,a5),d0
		sub.w	#48,d0
		lsr.w	#4,d0
		add.w	d0,d0
		move.w	(sh.mousey-t,a5),d1
		sub.w	#BRIY,d1
		lsr.w	#3,d1
		mulu	#20,d1
		rts

*-------------------------------------------------------------------------*
******************* Change un pointeur sur les tableaux *******************
*-------------------------------------------------------------------------*
* Entrée	=> a0.l pt/pointeur sur le tableau
*		=> d0.l	offset (1=1 tableau)
gonivea:		
		move.l	#TNT*TB.SIZE,d2
		muls	#TB.SIZE,d0
		move.l	(a0),d1
		add.l	d0,d1
		bpl.s	.okmylove
		add.l	d2,d1
.okmylove	cmp.l	d1,d2
		bhi.s	.go
		sub.l	d2,d1
.go		move.l	d1,(a0)
		rts

*=**********************************************************************=*
**------------------------* Affiche un tableau *------------------------**
*=**********************************************************************=*
* Entrée	=> a5.l	adr. 't'.
*		=> a6.l CUSTOM
set.bricks:
		move.l	(pt_tableaux-t,a5),a0
		add.l	(niveau-t,a5),a0
		bsr	tab2settable
		bsr	petits_poids		£=> Efface les briques
		bsr	legume			£=> Affiche les ombres
		bsr	poireau			£=> Affiche les briques
		rts

***************************************************************************
*******-----------------*/ Affiche les briques  */------------------*******
***************************************************************************
* Entrée	=> a5.l	adr. 't'.
petits_poids:
		move.l	(pt_screen-t,a5),a0
		move.l	(SPECRAN2,a0),a1
		move.l	(SPECRAN1,a0),a2
		move.w	(SP_SLINE,a0),d0
		mulu	#BRIY,d0
		add.l	d0,a1
		add.l	d0,a2
		move.w	(SP_SPLAN,a0),d0
		sub.w	#28,d0
		move.w	#BRIY2-BRIY,d1
		addq.w	#ODECA,d1
		subq.w	#1,d1
		moveq	#0,d2
		moveq	#-1,d3
		move.w	(SP_SPLAN,a0),d4
		mulu	#3,d4
.loop		move.l	(a1)+,(a2)+		PLAN 1
		move.l	(a1)+,(a2)+
		move.l	(a1)+,(a2)+
		move.l	(a1)+,(a2)+
		move.l	(a1)+,(a2)+
		move.l	(a1)+,(a2)+
		move.l	(a1)+,(a2)+
		add.w	d0,a1
		add.w	d0,a2
		move.l	(a1)+,(a2)+		PLAN 2
		move.l	(a1)+,(a2)+
		move.l	(a1)+,(a2)+
		move.l	(a1)+,(a2)+
		move.l	(a1)+,(a2)+
		move.l	(a1)+,(a2)+
		move.l	(a1)+,(a2)+
		add.w	d0,a1
		add.w	d0,a2
		move.l	d2,(a2)+		PLAN 3
		move.l	d2,(a2)+
		move.l	d2,(a2)+
		move.l	d2,(a2)+
		move.l	d2,(a2)+
		move.l	d2,(a2)+
		move.l	d2,(a2)+
		add.w	d0,a2
		move.l	d3,(a2)+		PLAN 4
		move.l	d3,(a2)+
		move.l	d3,(a2)+
		move.l	d3,(a2)+
		move.l	d3,(a2)+
		move.l	d3,(a2)+
		move.l	d3,(a2)+
		add.w	d0,a2
		move.l	d2,(a2)+		PLAN 5
		move.l	d2,(a2)+
		move.l	d2,(a2)+
		move.l	d2,(a2)+
		move.l	d2,(a2)+
		move.l	d2,(a2)+
		move.l	d2,(a2)+
		add.w	d4,a1
		add.w	d0,a2
		dbf	d1,.loop
		rts

***************************************************************************
*-------------------------* Affiche les ombres *--------------------------*
***************************************************************************
* Entrée	=> a5.l	adr. 't'.
legume:
		move.l	(pt_screen-t,a5),a0
		move.l	(SPECRAN1,a0),d3
		move.w	#NOPLAN,d0
		mulu	(SP_SPLAN,a0),d0
		add.l	d0,d3
		move.w	#ODECA,d0
		mulu	(SP_SLINE,a0),d0
		add.l	d0,d3
		subq.l	#2,d3
		lea	(set_guide-t,a5),a1
		lea	(2+set_table-t,a5),a0
		move.l	#$FFF80007,d0
		move.w	#NID-1,d2
.zet_zoop	move.l	(a1)+,a3
		add.l	d3,a3
		tst.w	(a0)
		beq	.next
		and.l	d0,(a3)
V		SET	1
		REPT 	BK.HEIGHT-1
		and.l	d0,(V*TLIGNE,a3)
V		SET	V+1
		ENDR
.next		addq.l	#4,a0
		dbf	d2,.zet_zoop
		rts

***************************************************************************
**************************/ Affiche les briques /**************************
***************************************************************************
* Entrée	=> a5.l	adr. 't'.
*		=> a6.l CUSTOM
poireau:
		movem.l	a5/a6,-(a7)
		move.l	(bk.pagegfx-t,a5),a3
		move.l	(pt_screen-t,a5),a0
		move.w	(PG_SPLAN,a3),d0
		subq.w	#2,d0
		swap	d0
		move.w	(SP_SPLAN,a0),d0
		subq.w	#2,d0
		move.w	(PG_NPLAN,a3),d5
		mulu	#BK.HEIGHT,d5
		lsl.w	#6,d5
		addq.w	#1,d5
		move.l	(SPECRAN1,a0),d3
		lea	(2+set_table-t,a5),a0
		lea	(set_guide-t,a5),a1
		move.w	#NID-1,d2
		lea	(PG_DATA,a3),a3
		WAITBLITTER
		move.l	#$09F00000,(BLTCON0,a6)
		move.l	#-1,(BLTAFWM,a6)
		move.l	d0,(BLTAMOD,a6)
		moveq	#14,d4
		lea	(DMACONR,a6),a2
		lea	(BLTAPTH,a6),a4
		lea	(BLTDPTH,a6),a5
		lea	(BLTSIZE,a6),a6
.zet_zoop	moveq	#0,d0
		move.w	(a0),d0			lit le code brique
		beq.s	.set_fin		-> 0 pas de brique
		add.l	a3,d0
		move.l	(a1),d1
		add.l	d3,d1
.wait		btst	d4,(a2)
		bne.s	.wait
		move.l	d0,(a4)
		move.l	d1,(a5)
		move.w	d5,(a6)
.set_fin	addq.l	#4,a1
		addq.l	#4,a0
		dbf	d2,.zet_zoop
		movem.l	(a7)+,a5/a6
		rts

*-------------------------------------------------------------------------*
*--------* Convertit n° briques/couleur brique/fond/couleur fond *--------*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	adr. 't'.
cheval:
		moveq	#0,d2
		move.w	(avi-t,a5),d2		Jeu briques
		bsr.l	convertit_decimal
		moveq	#0,d1
		lea	(czx-t,a5),a0
		bsr.l	conv_number_ascii
		moveq	#0,d2
		move.w	(avu-t,a5),d2		Couleur brique
		bsr.l	convertit_decimal
		moveq	#0,d1
		lea	(czx+1-t,a5),a0
		bsr.l	conv_number_ascii
		moveq	#0,d2
		move.w	(ava-t,a5),d2		Fond
		bsr.l	convertit_decimal
		moveq	#1,d1
		lea	(czx+3-t,a5),a0
		bsr.l	conv_number_ascii
		moveq	#0,d2
		move.w	(avo-t,a5),d2		Couleur fond
		bsr.l	convertit_decimal
		moveq	#1,d1
		lea	(czx+6-t,a5),a0
		bsr.l	conv_number_ascii
		rts

*-------------------------------------------------------------------------*
*//////////////////Initialise le dégradé couleur brique///////////////////*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	adr. 't'.
gagaona:
		lea	(gaga-t,a5),a4
		move.l	(degrade_copper-t,a5),a2
		lea	(16+8+2,a2),a2		bits 4 à 7
		lea	(8,a2),a1		bits 0 à 3
		moveq	#TBC-1,d7		nbre de  couleurs
.loop_master	move.w	(a4)+,(a2)
		move.w	(a4)+,(a1)
		lea	(20,a1),a1
		lea	(20,a2),a2
		dbf	d7,.loop_master
		rts

*-----------------------* TERMINE POUR L'EDITEUR *------------------------*
rupt15.off:
		bsr.s	.off
		bne.l	sys_fin
		rts		
.off		bsr.l	bob32.freemem2
		bne.s	.exit
		bsr.l	kill_screen
		bne.s	.exit
		lea	libere_mem,a4
		lea	(shop.sprite-t,a5),a0	ANIM. SPRITE CURSEUR SOURIS
		jsr	(a4)
		bne.s	.exit
		lea	(ed.sprite1-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		lea	(ed.sprite2-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		moveq	#0,d7
.exit		rts
