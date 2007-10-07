*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
**************************** ROUT=5 : Gardien *****************************
*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
* Entrée	=> a5.l	adresse 't'
*		=> a6.l CUSTOM
* £=> Signifie que la routine est dans le fichier 'interupt5.s'
interupt5:
		bsr.l	hasard_ok
		bsr	wait_vbl
		bsr.l	init_pts_lutins2
		bsr	ga.blitz2		£=> Déplace Blitz
		bsr	ga.blitz1		£=> Envoie Blitz
		bsr	yesfire			£=> Cycle la couleur 30
		bsr	last_flip		|-> Echange listes Copper
		bsr	datastorm		£=> Calcul mots ctrl gardiens
		bsr	last_datastorm		£=> Calcul mots ctrl bumper
		RASTER	#$00F
		bsr	goscroll		£=> Affiche défilement
		RASTER	#$000
		bsr	goblins			£=> Visualise page BOB
		RASTER	#$0F0
		bsr	msx2.off		£=> Efface pointeur de balle
		bsr	ssha_restore
		bsr	sbob_restore
		RASTER	#$000
		tst.b	(flag_pause-t,a5)
		bne.s	.yapudecoca
		bsr	animyes			£=> Animation capsules monnaies
		bsr	gere_scroll		£=> Gère vitesse défilement
		bsr	last_move		|-> Déplacement Gardiens
		bsr	pierre_le_clebs		£=> Envoie tirs Gardiens
		RASTER	#$FFF
		bsr	last_missile		|-> Déplace tirs Gardiens
		RASTER	#$000
		bsr	windows_beurk		|-> déplace monnaies
		bsr	windows_shit		|-> déplace Gadgets
		bsr	last_test_souris	|-> Test déplacement souris
		bsr.l	mouse_buttons		|-> Test boutons souris
		bsr	pc_merde		£=> lache balle
		bsr	last_vitus		|-> Déplace balle
		bsr	elegy			|-> Collision balle/gardien
		bsr	explo.anim		£=> Animation explosions
		bsr	bercy			£=> Gestion du Tilt
.yapudecoca	RASTER	#$F00
		bsr	ssha_affiche		|-> Affiche Shadows-SBOBs
		bsr	sbob_affiche		|-> Affiche Scroll-BOBs
		bsr	msx2			£=> Affiche pointeur de balle
		RASTER	#$000
		bsr	last_pub		£=> Affiche le crédit
		bsr	last_pub2		£=> Affiche les vies
		bsr.l	pertedememoire
		bsr	va_ici_con		£=> Test si va en "black"
		bsr	super_tiga		£=> Test si GameOver
		bsr	extralife
		bsr	powindah		£=> Efface barre puissance
		bsr.l	np_sfx
		bsr.l	quit
		bra	irq

*=========================================================================*
*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/**/SROLLING/**/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/
*=========================================================================*
goscroll:	
		*/ SCROLLING PAUSE */
		tst	(gd.stemp-t,a5)		tempo avant défilement		
		beq.s	.no_wait		-> non
		subq.w	#1,(gd.stemp-t,a5)	Déc. tempo
		moveq	#0,d7			vitesse défilement = 0
		bra.s	.no_pause
.no_wait
		*/ SCROLLING WINNER */
		tst.w	(tecnowinner-t,a5)
		beq.s	.notec
		move.l	(atable_clavier-t,a5),a0
		tst.b	(TOUCHE_ESPACE,a0)
		bne.s	.no_war
		not.w	(gd.clito-t,a5)		tempo avant défilement		
		beq.s	.one
.no_war		moveq	#0,d7
		bra.s	.no_pause
.one		move.w	(gd.sspeed-t,a5),d7	vitesse de défilement
		bra.s	.no_pause
.notec	
		move.w	(gd.sspeed-t,a5),d7	vitesse de défilement
		tst.b	(flag_pause-t,a5)	pause activée ?
		beq.s	.no_pause		-> non
		moveq	#0,d7			si pause vitesse défilement nulle
.no_pause	bsr	last_scroll		|-> défilement écran
		rts

*=========================================================================*
*/*/*/*/*/*/ Gestion des differents envois de tirs du gardien */*/*/*/*/*/*
*=========================================================================*
pierre_le_clebs:
		move.l	(gd1-t,a5),a1		Structure Gardien 1
		bsr.s	.gofire
.pierre_le_dieu	cmp.w	#1,(gd.numb-t,a5)	Un seul Gardien ?
		beq.s	.exit			-> oui
		move.l	(gd2-t,a5),a1		Structure Gardien 2
.gofire		tst.w	(GARD.POWER,a1)		Gardien détruit ?
		beq.s	.exit			-> oui
		cmp.w	#80,(GARD.Y,a1)
		bge.s	.exit
		subq.w	#1,(GARD.WAIT,a1)
		bne.s	.exit
		move.w	(GARD.WAIT2,a1),(GARD.WAIT,a1)
		move.b	#1,(sound+S_TIR.GARDIEN-t,a5)
		addq.w	#4,(GARD.PTFIRE,a1)
		move.w	(GARD.PTFIRE,a1),d0
		move.l	(GARD.FIRE,a1),a0
		move.l	(a0,d0),d0		Pointeur Tir 0, 4, 8 à n
		bge.s	.be_over
		moveq	#0,d0
		move.w	d0,(GARD.PTFIRE,a1)
		move.l	(a0),d0
.be_over	lea	(tir00_rout,pc),a0
		move.l	(a0,d0),d0
		add.l	a5,d0
		move.l	d0,a0
		jmp	(a0)			|-> initialise le tir
.exit		rts

*/ VOIR PAGE BOB & PAGE MSK */
goblins		cmp.l	#ETB,(triche-t,a5)
		bne.s	.exit
		move.l	(atable_clavier-t,a5),a0
		tst.b	(TOUCHE_F1,a0)
		bne.s	.gfx
		tst.b	(TOUCHE_F2,a0)
		bne.s	.msk
.exit		rts
.gfx		move.l	(sbob_pagegfx-t,a5),a3
		bra.s	.ii
.msk		move.l	(sbob_pagemsk-t,a5),a3
.ii		move.l	a3,d0
		add.l	#PG_DATA,d0
		tst.b	(TOUCHE_SHIFT_G,a0)	bas de la page ?
		beq.s	.jj			-> non
		move.w	(PG_SLINE,a3),d1
		mulu	#256,d1
		add.l	d1,d0
.jj		move.l	(pt_screen-t,a5),a0
		move.l	(sc_u1-t,a5),a1
		bsr	pointe.x
		move.l	(sc_w1-t,a5),a1
		move.l	#$FFFFFFFE,(a1)
		rts

*=========================================================================*
*-------------------* test si joueur lache la balle *---------------------*
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
pc_merde:
		cmp.w	#BOUTON_LACHE,(mouse0-t,a5)
		bne.s	.ecreve_3
		moveq	#0,d0
		moveq	#100,d1
		moveq	#NSBALL-1,d7
		move.l	(pt_balle-t,a5),a3
.loop		move.w	d0,(SBALLE.GLUE,a3)	raz flag balle collée
		move.w	d1,(SBALLE.START,a3)	réinit. compt temps
		lea	(SBALLE.SIZE,a3),a3
		dbf	d7,.loop
		lea	(b1-t,a5),a3
		move.l	d0,(BUMP.BALLE,a3)
.ecreve_3	rts

*/////////////////////////////////////////////////////////////////////////*
************************ ANIMATION DES EXPLOSIONS *************************
*/////////////////////////////////////////////////////////////////////////*
* Entrée	=> a5.l	adr. table 't'
explo.anim:	
		move.l	(explomon-t,a5),a3
		moveq	#NBR_EXPLOMON-1,d2
		moveq	#0,d1
.sandra		lea	(SBOB.TEMPO,a3),a0
		subq.w	#1,(a0)+		tempo=tempo-1
		bne.s	.noamim			-> tempo > 0
		move.w	(a0)+,(SBOB.TEMPO,a3)
		move.w	(a0)+,d0		d0=pt/animation
		cmp.w	(a0)+,d0		pt < au pt maxi ?
		bls.s	.no_maxi		-> oui
		move.w	d1,(SBOB.ACTIF,a3)
		bra.s	.noamim			-> tempo > 0
.no_maxi	addq.w	#8,(SBOB.PTANIM,a3)	
		move.l	(SBOB.TABLE,a3),a0	a0=table adr. gfx & msk
		add.w	d0,a0
		move.l	(a0)+,(SBOB.GFX,a3)
		move.l	(a0),(SBOB.MSK,a3)
.noamim		lea	(SBOB.SIZE,a3),a3
		dbf	d2,.sandra
		rts

*=========================================================================*
*                    Test sort de la boucle du gardien			  *
*=========================================================================*
va_ici_con:
		move.l	(gd1-t,a5),a1
		move.l	(gd2-t,a5),a2
		move.w	(gd.numb-t,a5),d7	Nombre de Gardien(s) 1 ou 2 ?
		subq.w	#1,d7			Deux Gardiens ?
		bne.s	.cont			-> Non
		move.l	a1,a2
.cont		tst.w	(GARD.POWER,a1)		Gardien<1> détruit ? 
		bne	.exit			-> Non
		tst.w	(GARD.POWER,a2)		Gardien<2> détruit ? 
		bne	.exit			-> Non
		move.l	(missile-t,a5),a0
		move.w	#NBR_MISSILE-1,d0
		moveq	#0,d1
.ssloop		move.l	d1,(a0)
		lea	(MIS.SIZE,a0),a0
		dbf	d0,.ssloop
		move.l	(atable_clavier-t,a5),a0
		tst.b	(TOUCHE_ESPACE,a0)	Le joueur veux finir ?
		bne.s	.go
		bsr	move_sbobtext		£=> Lettres "LEVEL COMPLETED"
		tst.w	(GARD.EXPTIME,a1)	Gardien<1> Fini explosions ?
		bne	.exit			-> Non
		tst.w	(GARD.EXPTIME,a2)	Gardien<2> Fini explosions ?
		bne	.exit			-> Non
.go
*------------------------> GARDIEN(s) DETRUIT(s) <------------------------*
		*- gadien final détruit : tecnoball fini -> gameover -*
		cmp.w	#GARD_FF,(gd.pt-t,a5)	gardien final ?
		bne.s	.non			-> non
		moveq	#0,d0
		move.l	d0,(life0-t,a5)		nombre de vie = 00
		tst.w	(gameover-t,a5)
		bne.s	.exit
		moveq	#1,d0
		move.w	d0,(gameover-t,a5)
		move.w	#20,(tecnowinner-t,a5)
		bra.s	.sangle
.non		move.w	(rak.slarge-t,a5),(rak.large-t,a5)
		move.w	#11,(rout-t,a5)		>flag fin gardien<
.sangle		bsr.l	go_fade_off
.test786	tst.w	(flag_fade_off-t,a5)
		bne.s	.test786
.exit		rts

*=========================================================================*
*----------------*/ CALCUL MOT DE CONTROLES DES LUTINS /*-----------------*
*=========================================================================*
* Entrée	=> a5.l	adresse 't'
datastorm:	
		not.w	(gd.clito-t,a5)
		move.l	(coordx-t,a5),d6
		move.l	(coordy-t,a5),d7
		move.l	(gd1-t,a5),a0
		bsr.s	.pierre_denis
		cmp.w	#1,(gd.numb-t,a5)	1 gardien seulemenent ?
		beq	.exit		 	-> oui
		move.l	(gd2-t,a5),a0		adr. structure gardien 2
.pierre_denis	moveq	#0,d0
		move.w	(GARD.X,a0),d0
		tst.w	(GARD.POWER,a0)		Gardien mort ?
		beq.s	.dead
		tst.w	(gd.clito-t,a5)		Clignote ON ?
		beq.s	.yapakeleku		-> Oui affiche
		tst.w	(GARD.TOUCH,a0)		Gardien touché ?
		beq.s	.yapakeleku		-> non
		subq.w	#1,(GARD.TOUCH,a0)
.dead		move.w	#-65,d0
.yapakeleku	lsl.w	#2,d0
		moveq	#0,d1
		move.w	(GARD.Y,a0),d1
		move.w	(GARD.HEIGHT,a0),d2
		bsr.l	calcul_mot_ctrl_aga
		move.l	(GARD.SPR1,a0),a1
		move.l	(GARD.SPR0,a0),a0
		move.w	d3,(8,a0)
		or.w	#$80,d3
		move.w	d3,(8,a1)
		swap	d3
		move.w	d3,(a0)	
		move.w	d3,(a1)	
.exit		rts

*=========================================================================*
*-------------------------*/ ANIMATION MONNAIE /*-------------------------*
*=========================================================================*
* Entrée	=> a5.l	adresse 't'
animyes:	move.l	(pt_capsule-t,a5),a3
		moveq	#NMONNAIE-1,d4
.loop2		lea	(SBOB.TEMPO,a3),a0
		subq.w	#1,(a0)+		tempo=tempo-1
		bne.s	.noanim			-> tempo > 0
		move.w	(a0)+,(SBOB.TEMPO,a3)
		move.w	(a0)+,d0		d0=pt/animation
		cmp.w	(a0)+,d0		pt < au pt maxi ?
		bls.s	.no_maxi		-> oui
		moveq	#0,d0
		move.w	d0,(SBOB.PTANIM,a3)
.no_maxi	addq.w	#8,(SBOB.PTANIM,a3)	
		move.l	(SBOB.TABLE,a3),a0	a0=table adr. gfx & msk
		add.w	d0,a0
		move.l	(a0)+,(SBOB.GFX,a3)
		move.l	(a0),(SBOB.MSK,a3)
.noanim		lea	(SCAP.SIZE,a3),a3
		dbf	d4,.loop2

		*/ ANIMATION DES MISSILES */
		move.l	(missile-t,a5),a3
		lea	(SBOB.TEMPO,a3),a0
		subq.w	#1,(a0)+		tempo=tempo-1
		bne.s	.noanim2		-> tempo > 0
		move.w	(a0)+,(SBOB.TEMPO,a3)
		move.w	(a0)+,d0		d0=pt/animation
		cmp.w	(a0)+,d0		pt < au pt maxi ?
		bls.s	.no_maxi2		-> oui
		moveq	#0,d0
		move.w	d0,(SBOB.PTANIM,a3)
.no_maxi2	addq.w	#8,(SBOB.PTANIM,a3)	
		move.l	(SBOB.TABLE,a3),a0	a0=table adr. gfx & msk
		add.w	d0,a0
		move.l	(a0)+,d0
		move.l	(a0),d1
		moveq	#NBR_MISSILE-1,d7
		lea	(SBOB.GFX,a3),a3
.loopx		move.l	d0,(a3)+
		move.l	d1,(a3)
		lea	(MIS.SIZE-4,a3),a3
		dbf	d7,.loopx
.noanim2	
		*/ GADGET */
		move.l	(pt_gadget-t,a5),a4
		moveq	#NGADGET-1,d4
.vloop3		subq.w	#1,(SGAD.TEMPO,a4)	tempo=tempo-1
		bne.s	.vnoamim		-> tempo > 0
		move.w	#3,(SGAD.TEMPO,a4)
		move.w	(SGAD.PTANIM,a4),d0	d0=pt/animation
		cmp.w	#7*8,d0			pt < au pt maxi ?
		bne.s	.vno_maxi		-> oui
		moveq	#0,d0
		move.w	d0,(SGAD.PTANIM,a4)
.vno_maxi	addq.w	#8,(SGAD.PTANIM,a4)	
		move.l	(SBOB.TABLE,a4),a0	a0=table adr. gfx & msk
		add.w	d0,a0
		add.w	(SGAD.PTBOB,a4),a0	pt/le bonus
		move.l	(a0)+,(SBOB.GFX,a4)
		move.l	(a0),(SBOB.MSK,a4)
.vnoamim	lea	(SGAD.SIZE,a4),a4
		dbf	d4,.vloop3
		rts

*-------------------------------------------------------------------------*
*-------------------------->CYCLE LA COULEUR 30<--------------------------*
*-------------------------------------------------------------------------*
yesfire:
		moveq	#0,d0			pointeur sur la 1er table
		lea	(cycl1-t,a5),a0
		addq.w	#2,(-2,a0)
		lea	(cycl2-cycl1,a0),a1
		add.w	(-2,a0),a0
		cmp.l	a0,a1
		bhi.s	.siemens
		lea	(cycl1-t,a5),a0
		clr.w	(-2,a0)
.siemens	move.l	(pt_screen-t,a5),a1
		move.w	(SBPLCON3,a1),(BPLCON3,a6)
		move.w	(a0,d0),(COLOR30,a6)
		rts

***************************************************************************
****************************AFFICHE LES CREDITS****************************
***************************************************************************
last_pub:
		move.w	(score.n-t,a5),d0	n° du caractère à afficher
		addq.w	#1,d0
		cmp.w	#6,d0
		bne.s	.falentin1
		moveq	#0,d0
.falentin1	move.w	d0,(score.n-t,a5)
		lea	(credit5-t,a5),a0	pt/100aine de millier
		sub.w	d0,a0
		sub.w	d0,a0
		move.w	(a0),d1
		move.l	(fontex.pagegfx-t,a5),a3
		moveq	#0,d3
		move.w	(PG_SPLAN,a3),d3
		lea	(PG_DATA,a3,d1),a3
		move.l	(cred.lutin0-t,a5),a0
		move.l	(cred.lutin1-t,a5),a1
		lea	(16+2,a0,d0),a0
		lea	(16+2,a1,d0),a1
		moveq	#0,d0
		moveq	#7,d1
.bree		move.b	(a3),(a0)		Plan 1
		addq.l	#8,a0
		add.l	d3,a3
		move.b	(a3),(a0)		Plan 2
		addq.l	#8,a0
		add.l	d3,a3
		move.b	(a3),(a1)		Plan 3
		addq.l	#8,a1
		add.l	d3,a3
		move.b	(a3),(a1)		Plan 4
		addq.l	#8,a1
		add.l	d3,a3
		dbf	d1,.bree
		rts


***************************************************************************
*******************************AFFICHE LES VIES****************************
***************************************************************************
last_pub2:
		move.w	(lifes.n-t,a5),d0	n° du caractère à afficher
		addq.w	#1,d0
		cmp.w	#2,d0
		bne.s	.falentin1
		moveq	#0,d0
.falentin1	move.w	d0,(lifes.n-t,a5)
		lea	(life1-t,a5),a0		pt/décimal
		sub.w	d0,a0
		sub.w	d0,a0
		move.w	(a0),d1
		move.l	(fontex.pagegfx-t,a5),a3
		moveq	#0,d3
		move.w	(PG_SPLAN,a3),d3
		lea	(PG_DATA,a3,d1),a3
		move.l	(life.lutin2-t,a5),a0
		move.l	(life.lutin3-t,a5),a1
		lea	(16+2,a0,d0),a0
		lea	(16+2,a1,d0),a1
		moveq	#0,d0
		moveq	#7,d1
.bree		move.b	(a3),(a0)		Plan 1
		addq.l	#8,a0
		add.l	d3,a3
		move.b	(a3),(a0)		Plan 2
		addq.l	#8,a0
		add.l	d3,a3
		move.b	(a3),(a1)		Plan 3
		addq.l	#8,a1
		add.l	d3,a3
		move.b	(a3),(a1)		Plan 4
		addq.l	#8,a1
		add.l	d3,a3
		dbf	d1,.bree

		*/ Animation du Gadget Life */
		lea	(ss.credit-t,a5),a3
		bsr.s	.techno		
		lea	(ss.life-t,a5),a3
.techno		move.l	(SS.ADR,a3),a0
		lea	(SS.PT,a3),a2
		add.l	(a2)+,a0
		subq.w	#1,(a2)
		bne.s	.goto
		move.w	(SS.INIT,a3),(a2)
		move.l	(SS.TAILLE2,a3),d0
		add.l	d0,-(a2)
		move.l	(SS.SIZ,a3),d0
		cmp.l	(a2),d0
		bne.s	.goto
		clr.l	(a2)
.goto		move.l	a0,a1
		add.l	(SS.TAILLE,a3),a1
		move.w	(SS.HEIGHT,a3),d1
		move.l	(SS.LUTIN0,a3),a2
		move.l	(SS.LUTIN1,a3),a3
.lala		move.w	(a0)+,(a2)
		addq.l	#8,a2
		move.w	(a0)+,(a2)
		addq.l	#8,a2
		move.w	(a1)+,(a3)
		addq.l	#8,a3
		move.w	(a1)+,(a3)
		addq.l	#8,a3
		dbf	d1,.lala
		rts

*/>»«<\*/>»«<\*/>»«<\*/>»«<\*/>»«<\*/>»«<\*/>»«<\*/>»«<\*/>»«<\*/>»«<\*/>»*
***********************/ CACLUL POSITION BUMPER */*************************
*/>»«<\*/>»«<\*/>»«<\*/>»«<\*/>»«<\*/>»«<\*/>»«<\*/>»«<\*/>»«<\*/>»«<\*/>»*
* Entrée	=> a5.l	Adr. 't'
last_datastorm:
		move.l	(coordx-t,a5),d6
		move.l	(coordy-t,a5),d7
		move.l	(bump1.lutin0-t,a5),a0
		move.l	(bump1.lutin1-t,a5),a1
		lea	(b1-t,a5),a4
		bsr.s	.hydro
		tst.w	(rak.team-t,a5)
		beq.s	.exit
		move.l	(bump2.lutin2-t,a5),a0
		move.l	(bump2.lutin3-t,a5),a1
		lea	(b4-t,a5),a4
.hydro		moveq	#0,d0
		move.w	(BUMP.X,a4),d0
		tst.w	(gd.clito-t,a5)		Clignote ON ?
		beq.s	.yapakeleku		-> Oui affiche
		tst.w	(BUMP.TOUCH,a4)		Gardien touché ?
		beq.s	.yapakeleku		-> non
		subq.w	#1,(BUMP.TOUCH,a4)
.dead		move.w	#-65,d0
.yapakeleku	lsl.w	#2,d0
		moveq	#0,d1
		move.w	(BUMP.Y,a4),d1
		moveq	#8,d2
		bsr.l	calcul_mot_ctrl_aga
		move.w	d3,(8,a0)
		or.w	#$80,d3
		move.w	d3,(8,a1)
		swap	d3
		move.w	d3,(a0)	
		move.w	d3,(a1)	
.exit		rts

*=========================================================================*
*---------------------*2 Boutons même temps => Tilt*----------------------*
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
bercy:
		cmp.w	#BOUTON_TILT,(mouse0-t,a5)
		bne.s	.skrov2
		move.w	(balle_tilt-t,a5),d0
		addq.w	#1,d0
		move.w	(hasard-t,a5),d1
		lsl.w	#2,d1
		moveq	#60,d2
		and.w	d2,d1
		move.l	(pt_balle-t,a5),a3
		moveq	#NSBALL-1,d7
.loop		tst.w	(SBALLE.ACTIF,a3)	Balle active ?
		beq.s	.skrov			-> Non
		cmp.w	(SBALLE.TILT,a3),d0
		bne.s	.skrov
		clr.w	(SBALLE.TILT,a3)
		move.b	#1,(sound+S_TECNO-t,a5)
		lea	(2+SBALLE.DIRECT,a3),a0
		cmp.w	(a0),d1			Même direction ?
		bne.s	.krov2			-> Non
		move.w	(a0),d1
		addq.w	#8,d1
.krov2		and.w	d2,d1
		move.w	d1,(a0)			Nouvelle direction
		addq.w	#4,d1
.skrov		lea	(SBALLE.SIZE,a3),a3
		dbf	d7,.loop
.skrov2
		*/ Test si on peut Tilter */
		moveq	#NSBALL-1,d7
		move.w	(balle_tilt-t,a5),d0	Temps avant tilt
		moveq	#1,d1
		lea	(sound+S_TILT.ALARM-t,a5),a1
		move.l	(pt_balle-t,a5),a4
.loop2		tst.w	(SBALLE.ACTIF,a4)	Balle active ?
		beq.s	.no_tilt2		-> non
		tst.w	(SBALLE.GLUE,a4)	Balle collée ?
		bne.s	.no_tilt2		-> oui
		cmp.w	(SBALLE.TILT,a4),d0	autorise tilt ?
		blt.s	.no_tilt2		-> tilt dèjà autorisé
		bne.s	.no_tilt		-> non
		move.b	d1,(a1)			demande son alarm
.no_tilt	addq.w	#1,(SBALLE.TILT,a4)
.no_tilt2	lea	(SBALLE.SIZE,a4),a4
		dbf	d7,.loop2
		rts

*=========================================================================*
*                    Déplacement des capsules monnaie			  *
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
windows_beurk:
		moveq	#0,d6
		move.l	(pt_capsule-t,a5),a4
		moveq	#NMONNAIE-1,d7
.capsloop	tst.w	(SCAP.ACTIF,a4)		Capsule Monnaie active ?
		beq	.cap_fin		-> Non

		*-=- déplacement en bas + test dépass. -=-*
		move.w	(SCAP.SPEED,a4),d0
		add.w	d0,(SCAP.Y,a4)
		cmp.w	#NLIGNE-10,(SCAP.Y,a4)
		bge	.cap_god		-> Sort de l'écran

		*-=- test collisions capsule/raquette -=-*
		move.w	(rak.large-t,a5),d4
		moveq	#8,d5
		tst.w	(rak.team-t,a5)
		beq.s	.cap_idem
		lea	(b4-t,a5),a1
		move.w	(BUMP.X,a1),d0		x raquette
		move.w	(BUMP.Y,a1),d1		y raquette
		move.w	(SCAP.X,a4),d2		x capsule
		move.w	(SCAP.Y,a4),d3		y capsule
		add.w	#16,d2
		cmp.w	d0,d2
		blt	.cap_idem
		addq.w	#7,d3
		cmp.w	d1,d3
		blt.s	.cap_idem
		add.w	d4,d0
		move.w	(SCAP.X,a4),d2
		cmp.w	d0,d2
		bgt.s	.cap_idem
		add.w	d5,d1
		move.w	(SCAP.Y,a4),d3
		cmp.w	d1,d3
		blt.s	.cap_touch
	
		*-=- test collisions capsule/raquette -=-*
.cap_idem:	lea	(b1-t,a5),a1
		move.w	(BUMP.X,a1),d0		x raquette
		move.w	(BUMP.Y,a1),d1		y raquette
		move.w	(SCAP.X,a4),d2		x capsule
		move.w	(SCAP.Y,a4),d3		y capsule
		add.w	#16,d2
		cmp.w	d0,d2
		blt	.cap_fin
		addq.w	#7,d3
		cmp.w	d1,d3
		blt.s	.cap_fin
		add.w	d4,d0
		move.w	(SCAP.X,a4),d2
		cmp.w	d0,d2
		bgt.s	.cap_fin
		add.w	d5,d1
		move.w	(SCAP.Y,a4),d3
		cmp.w	d1,d3
		bgt.s	.cap_fin

		*-=- capsule touche raquette, augmente score -=-*
.cap_touch	move.b	#1,(sound+S_MONNAIE-t,a5)
		bsr	venus1
		bsr	venus1
		add.w	#20,(score.life-t,a5)
		lea	(jupiter10,pc),a1
		moveq	#10,d1
		cmp.w	#20,(SCAP.VAL,a4)	bourse de 20 crédits ?
		beq.s	.cap_bas1		-> oui
		cmp.w	#30,(SCAP.VAL,a4)	bourse de 30 crédits ?
		bne.s	.cap_cap		-> non
		lea	(jupiter30,pc),a1
		moveq	#30,d1
		bra.s	.cap_cap	
.cap_bas1	lea	(jupiter20,pc),a1
		moveq	#20,d1
.cap_cap	add.w	d1,(credit-t,a5)
		jsr	(a1)
		cmp.w	#3,(area-t,a5)		area 3 ?
		blt.s	.cap_god		-> non
		add.w	d1,(credit-t,a5)
		jsr	(a1)
		cmp.w	#5,(area-t,a5)		area 5 ?
		bne.s	.cap_god		-> non
		jsr	(a1)
		add.w	d1,(credit-t,a5)

		* capsule sort de l'écran ou touche raquette  *
.cap_god:	move.w	d6,(SCAP.ACTIF,a4)	RAZ flag capsule actif
.cap_fin	lea	(SCAP.SIZE,a4),a4
		dbf	d7,.capsloop
		rts

;=========================================================================;
;                     Affiche Pointeur Direction Balle			  ;
;=========================================================================;
; entrée	=> a5.l	adr. table 't'
msx2:
		moveq	#0,d5
		move.w	(nu_video-t,a5),d5
		beq.s	.no_change
		sub.w	(s_nligne-t,a5),d5
		neg.w	d5
		lsl.w	#2,d5
.no_change	move.l	(sbob_ytable-t,a5),a2
		add.l	d5,a2
		moveq	#0,d0
		moveq	#0,d1
		lea	(pixtbl-t,a5),a4
		move.l	(pt_balle-t,a5),a3
		move.l	(ecran_travail-t,a5),d7
		move.w	#%111,d3
		moveq	#NSBALL-1,d4
		move.w	(flag_flip-t,a5),d6
.rabin_loop	tst.w	(SBALLE.ACTIF,a3)
		beq.s	.nopixel
		tst.w	(SBALLE.GLUE,a3)
		beq.s	.nopixel
		move.w	(SBALLE.PBX,a3),d0
		move.w	(SBALLE.PBY,a3),d1
		move.l	(a2,d1*4),a0
		move.w	d0,d2
		and.w	d3,d2
		move.b	(a4,d2),d2
		lsr.w	#3,d0
		add.w	d0,a0	
		move.l	a0,(SBALLE.PB1,a3,d6)
		add.l	d7,a0			adresse affichage
		or.b	d2,(a0)
		or.b	d2,(40,a0)
		or.b	d2,(80,a0)
		or.b	d2,(120,a0)
		or.b	d2,(160,a0)
.nopixel	lea	(SBALLE.SIZE,a3),a3
		dbf	d4,.rabin_loop
		rts

;=========================================================================;
;                    Efface Pointeur Direction Balles			  ;
;=========================================================================;
msx2.off:
		moveq	#NSBALL-1,d4
		moveq	#0,d0
		move.l	(pt_balle-t,a5),a3
		lea	(SBALLE.PB1,a3),a3
		move.w	(flag_flip-t,a5),d6		pt écran 0 ou 4
		move.l	(ecran_travail-t,a5),d7
		move.l	(ecran3-t,a5),d1
		add.w	d6,a3
.loop		move.l	d1,a1			adr. 'ecran3'
		move.l	d7,a2			adr. 'ecran2' ou 'ecran3'
		move.l	(a3),d0			pos. pointeur balle à l'écran
		add.l	d0,a1
		add.l	d0,a2
		move.b	(a1),(a2)
		move.b	(TPLAN,a1),(TPLAN,a2)
		move.b	(TPLAN*2,a1),(TPLAN*2,a2)
		move.b	(TPLAN*3,a1),(TPLAN*3,a2)
		move.b	(TPLAN*4,a1),(TPLAN*4,a2)
		lea	(SBALLE.SIZE,a3),a3
		dbf	d4,.loop
		rts

*=========================================================================*
*                           YA-T-IL LE GAMEOVER				  *
*=========================================================================*
* entrée	=> a5.l	adr. table 't'
super_tiga:
		tst.w	(gameover-t,a5)
		beq	.new_kids
		addq.w	#1,(gameover-t,a5)
		bsr.l	raz_table_lutins
		bsr.l	init_pts_lutins2
		*/ Vire la Balle */
		moveq	#0,d0
		move.l	(pt_balle-t,a5),a4
		moveq	#NSBALL-1,d7
.balleloop	move.l	d0,(SBALLE.ACTIF,a4)
		lea	(SBALLE.SIZE,a4),a4
		dbf	d7,.balleloop
		*/ Vire les capsules */
		move.l	(pt_capsule-t,a5),a4
		moveq	#NMONNAIE-1,d7
.capsloop	move.w	d0,(SCAP.ACTIF,a4)
		lea	(SCAP.SIZE,a4),a4
		dbf	d7,.capsloop
		*/ Vire les gadgets */
		move.l	(pt_gadget-t,a5),a4
		moveq	#NGADGET-1,d7
.gadloop	move.w	d0,(SGAD.ACTIF,a4)
		lea	(SGAD.SIZE,a4),a4
		dbf	d7,.gadloop
		*/ Vire les missiles */
		move.l	(missile-t,a5),a4
		moveq	#NBR_MISSILE-1,d7
.loopx		move.w	d0,(MIS.ACTIF,a4)
		lea	(MIS.SIZE,a4),a4
		dbf	d7,.loopx
		*/ Vire les explosion */
		move.l	(explomon-t,a5),a4
		moveq	#NBR_EXPLOMON-1,d7
.loopxxx	move.l	d0,(SBOB.ACTIF,a4)
		lea	(SBOB.SIZE,a4),a4
		dbf	d7,.loopxxx
		*/ Vire ombres des Gardiens */
		move.l	(gd1-t,a5),a4
		move.l	(GARD.BOB,a4),a1
		move.l	d0,(SBOB.ACTIF,a1)
		tst.l	(gd2-t,a5)
		beq.s	.no222
		move.l	(gd2-t,a5),a4
		move.l	(GARD.BOB,a4),a1
		move.l	d0,(SBOB.ACTIF,a1)
.no222		*/ Vire ombres des Bumpers */
		lea	(b1-t,a5),a4
		move.l	(BUMP.BOB,a4),a1
		move.l	d0,(SBOB.ACTIF,a1)
		tst.w	(rak.team-t,a5)
		beq.s	.nosex
		lea	(b4-t,a5),a4
		move.l	(BUMP.BOB,a4),a1
		move.l	d0,(SBOB.ACTIF,a1)
.nosex		cmp.w	#10,(gameover-t,a5)
		bne.s	.new_kids
		move.w	#12,(rout-t,a5)
		bsr	pointeplayer
		bsr	xcopy
		move.l	#irq,(a7)
.new_kids	rts

*?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?*
*------------------>>Efface barre puissance si besoin<<-------------------*
*?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?*
* entrée	=> a5.l	adr. table 't'
powindah:
		lea	(power1.lutin4-t,a5),a2
		lea	(power1.lutin5-t,a5),a3
		move.l	(gd1-t,a5),a4
		bsr.s	.scytale
		cmp.w	#2,(gd.numb-t,a5)
		bne	.noreal
		lea	(power2.lutin4-t,a5),a2
		lea	(power2.lutin5-t,a5),a3
		move.l	(gd2-t,a5),a4
.scytale	move.w	(GARD.POWER,a4),d1
		lsr.w	#2,d1			4 unités pour une ligne
		move.w	(GARD.POW,a4),d0
		sub.w	d1,d0
		beq.s	.noreal
		move.w	d1,(GARD.POW,a4)
		subq.w	#1,d0
		moveq	#0,d1
		move.l	(a2),a0
		move.l	(a3),a1
.loop		move.b	d1,(a0)			Plan 1
		move.b	d1,(8,a0)		Plan 2
		move.b	d1,(a1)			Plan 3
		move.b	d1,(8,a1)		Plan 4
		lea	(16,a0),a0
		lea	(16,a1),a1
		dbf	d0,.loop
		move.l	a0,(a2)
		move.l	a1,(a3)
.noreal		rts

*·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·*
*¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢ DEPLACE LE GIGABLITZ ¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢¢*
*·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·*
* Entrée	=> a5.l	Adr. 't' 
ga.blitz2:
		tst.b	(flag_pause-t,a5)
		bne	.exit
		move.w	(blitz.haut-t,a5),d2
		beq	.exit
		move.w	(sin.blitz-t,a5),d1
		add.w	#100,d1
		and.w	#1022,d1
		move.w	d1,(sin.blitz-t,a5)
		moveq	#20,d0
		move.l	(pt_cos-t,a5),a0
		muls	(a0,d1),d0
		asr.l	#7,d0
		add.w	(x.blitz-t,a5),d0
		move.w	d0,d1
		lsr.w	#2,d1
		move.w	d1,(blitz.x-t,a5)
		moveq	#0,d1
		move.w	(blitz.y-t,a5),d1
		move.l	(coordx-t,a5),d6
		move.l	(coordy-t,a5),d7
		bsr.l	calcul_mot_ctrl_aga
		move.l	(blitz.lutin0-t,a5),a0
		move.l	(blitz.lutin1-t,a5),a1
		move.w	d3,(8,a0)
		or.w	#$80,d3
		move.w	d3,(8,a1)
		swap	d3
		move.w	d3,(a0)	
		move.w	d3,(a1)	

		*/ Collision */
		move.w	(blitz.y-t,a5),d0
		add.w	(blitz.haut-t,a5),d0
		cmp.w	#YRAKB,d0
		blt	.nocol2
		lea	(b1-t,a5),a1		adr. structure raquette
		move.w	(BUMP.X,a1),d4
		move.w	(blitz.x-t,a5),d2
		add.w	(large.blitz-t,a5),d2
		cmp.w	d4,d2			d2 < d4
		blt	.nocol1
		add.w	(rak.large-t,a5),d4
		move.w	(blitz.x-t,a5),d2
		cmp.w	d4,d2			d2 > d4
		bgt	.nocol1
		tst.w	(BUMP.TOUCH,a1)		raquette invincible ?
		bne.s	.nocol1		-> oui
		move.b	#1,(sound+S_RAK.EXPLO-t,a5)
		bsr	lune			supprime une vie
		lea	(b4-t,a5),a3
		move.w	#150,(BUMP.TOUCH,a3)
		move.w	#150,(BUMP.TOUCH,a1)
		move.l	(explomon-t,a5),a3
		moveq	#NBR_EXPLOMON-1,d0
.elegy_l1	tst.w	(SBOB.ACTIF,a3)		SBOB déjà actif ?
		bne.s	.elegy_1		-> oui, passe au suivant
		move.w	(BUMP.X,a1),d0
		addq.w	#8,d0
		move.w	d0,(SBOB.POSX,a3)
		move.w	(BUMP.Y,a1),d0
		subq.w	#8,d0
		move.w	d0,(SBOB.POSY,a3)	+ pos. y du gardien
		move.w	#1,(SBOB.ACTIF,a3)	active le bob
		clr.w	(SBOB.PTANIM,a3)	raz pointeur gfx animation
		move.w	(SBOB.INITTEMPO,a3),(SBOB.TEMPO,a3)
		move.l	(SBOB.TABLE,a3),a2
		move.l	(a2)+,(SBOB.GFX,a3)
		move.l	(a2),(SBOB.MSK,a3)
		bra.s	.nocol1
.elegy_1	lea	(SBOB.SIZE,a3),a3
		dbf	d0,.elegy_l1
.nocol1		*-=- test si colision avec la raquette port 2 -=-*
		tst.w	(rak.team-t,a5)		mode team ?
		beq	.nocol2			-> non
		lea	(b4-t,a5),a1		adr. structure raquette
		move.w	(BUMP.X,a1),d4
		move.w	(blitz.x-t,a5),d2
		add.w	(large.blitz-t,a5),d2
		cmp.w	d4,d2			d2 < d4
		blt	.nocol2
		add.w	(rak.large-t,a5),d4
		move.w	(blitz.x-t,a5),d2
		cmp.w	d4,d2			d2 > d4
		bgt	.nocol2
		tst.w	(BUMP.TOUCH,a1)		raquette invincible ?
		bne.s	.nocol2			-> oui
		move.b	#1,(sound+S_RAK.EXPLO-t,a5)
		bsr	lune			supprime une vie
		lea	(b4-t,a5),a3
		move.w	#150,(BUMP.TOUCH,a3)
		move.w	#150,(BUMP.TOUCH,a1)
		move.l	(explomon-t,a5),a3
		moveq	#NBR_EXPLOMON-1,d0
.zelegy_l1	tst.w	(SBOB.ACTIF,a3)		SBOB déjà actif ?
		bne.s	.zelegy_1		-> oui, passe au suivant
		move.w	(BUMP.X,a1),d0
		addq.w	#8,d0
		move.w	d0,(SBOB.POSX,a3)
		move.w	(BUMP.Y,a1),d0
		subq.w	#8,d0
		move.w	d0,(SBOB.POSY,a3)	+ pos. y du gardien
		move.w	#1,(SBOB.ACTIF,a3)	active le bob
		clr.w	(SBOB.PTANIM,a3)	raz pointeur gfx animation
		move.w	(SBOB.INITTEMPO,a3),(SBOB.TEMPO,a3)
		move.l	(SBOB.TABLE,a3),a2
		move.l	(a2)+,(SBOB.GFX,a3)
		move.l	(a2),(SBOB.MSK,a3)
		bra.s	.nocol2
.zelegy_1	lea	(SBOB.SIZE,a3),a3
		dbf	d0,.zelegy_l1
.nocol2
		*/ Déplace le Blitz */
		cmp.w	#256,(blitz.y-t,a5)
		blt.s	.go
		clr.w	(blitz.haut-t,a5)
.go		addq.w	#6,(blitz.y-t,a5)
.exit		rts

*·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·*
**--------------->>Test si on envoie un petit GigaBlitz<<----------------**
*·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·.·*
ga.blitz1:
		move.l	(gd1-t,a5),a4
		bsr.s	.lucille
		cmp.w	#2,(gd.numb-t,a5)
		bne	.noreal
		move.l	(gd2-t,a5),a4

		*/ Test si envoie le GigaBlitz */
		*/ => a4.l Structure Gardien
.lucille	tst.w	(GARD.POWER,a4)		Gardien mort ?
		beq	.noreal			-> oui
		subq.w	#1,(GARD.WAITF,a4)	Envoie le Blitz ?
		bne	.noreal			-> non
		tst.w	(blitz.haut-t,a5)	Dèjà le Blitz ?
		bne	.yais			-> oui
		move.w	(GARD.WAITF2,a4),(GARD.WAITF,a4)

		*/ Selectionne un des 7 lutins GigaBlitz */
		moveq	#0,d7
		move.w	(hasard-t,a5),d7
		and.w	#7,d7			0 à 7
		lea	(tgg1-t,a5),a0
.pas7		cmp.w	#32,(GARD.WIDTH,a4)
		bne.s	.pas32
		lea	(tgg2-t,a5),a0
.pas32		move.b	(a0,d7),d7
		move.w	d7,(large.blitz-t,a5)
		add.w	#16,(large.blitz-t,a5)
		lea	(blitz.type-t,a5),a0
		move.w	(6,a0,d7),(blitz.haut-t,a5)
		lea	(blitz.list-t,a5),a0
		add.w	d7,a0
		lea	(16+table_lutins-t,a5),a2
		move.l	(a0),(blitz.lutin0-t,a5)
		move.l	(a0)+,(a2)+
		move.l	(a0),(blitz.lutin1-t,a5)
		move.l	(a0)+,(a2)

		*/ Calcul position x du GigaBlitz */
		move.w	(GARD.X,a4),d0
		move.w	(GARD.WIDTH,a4),d1
		sub.w	(large.blitz-t,a5),d1
		asr.w	#1,d1
		add.w	d1,d0
		lsl.w	#2,d0
		move.w	d0,(x.blitz-t,a5)

		*/ Calcul position y du GigaBlitz */
		move.w	(GARD.Y,a4),d0
		move.w	(GARD.HEIGHT,a4),d1
		sub.w	(blitz.haut-t,a5),d1
		asr.w	#1,d1
		add.w	d1,d0
		move.w	d0,(blitz.y-t,a5)
		move.b	#1,(sound+S_RAK.TIR-t,a5)
.noreal		rts
.yais		move.w	#1,(GARD.WAITF,a4)
		rts

*=========================================================================*
*«»-«»-«»-«»-«»-«» Déplacement & collisions des Gadgets «»-«»-«»-«»-«»-«»-*
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
windows_shit:
		move.l	(pt_gadget-t,a5),a4
		moveq	#NGADGET-1,d7
gadsloop	tst.w	(SGAD.ACTIF,a4)		Gadget actif ?
		beq	sgad_fin		-> Non

		*-=-* Déplacement & test sortie écran *-=-*
		addq.w	#1,(SGAD.Y,a4)
		cmp.w	#NLIGNE-10,(SGAD.Y,a4)
		bge	gad_god			-> Sort de l'écran

		*-=-* Test collisions Gadget & Raquette 4 (TEAM) *-=-*
		move.w	(rak.large-t,a5),d4
		moveq	#8,d5
		tst.w	(rak.team-t,a5)
		beq.s	.gad_idem
		lea	(b4-t,a5),a1
		move.w	(BUMP.X,a1),d0		x raquette
		move.w	(BUMP.Y,a1),d1		y raquette
		move.w	(SGAD.X,a4),d2		x gadget
		move.w	(SGAD.Y,a4),d3		y gadget
		add.w	#16,d2
		cmp.w	d0,d2
		blt	.gad_idem
		addq.w	#7,d3
		cmp.w	d1,d3
		blt.s	.gad_idem
		add.w	d4,d0
		move.w	(SGAD.X,a4),d2
		cmp.w	d0,d2
		bgt.s	.gad_idem
		add.w	d5,d1
		move.w	(SGAD.Y,a4),d3
		cmp.w	d1,d3
		blt.s	.gad_touch
.gad_idem:	
		*-=-* Test collisions Gadget & Raquette 1 *-=-*
		lea	(b1-t,a5),a1
		move.w	(BUMP.X,a1),d0		x raquette
		move.w	(BUMP.Y,a1),d1		y raquette
		move.w	(SGAD.X,a4),d2		x gadget
		move.w	(SGAD.Y,a4),d3		y gadget
		add.w	#16,d2
		cmp.w	d0,d2
		blt	sgad_fin
		addq.w	#7,d3
		cmp.w	d1,d3
		blt	sgad_fin
		add.w	d4,d0
		move.w	(SGAD.X,a4),d2
		cmp.w	d0,d2
		bgt	sgad_fin
		add.w	d5,d1
		move.w	(SGAD.Y,a4),d3
		cmp.w	d1,d3
		bgt	sgad_fin

		*-=-* Gadget touche une raquette *-=-*
.gad_touch	bsr	venus1
		add.w	#10,(score.life-t,a5)
		move.w	(SGAD.NUM,a4),d0
		jmp	([gad_jump2,pc,d0*2])
		CNOP	0,4
gad_jump2:	dc.l	gad_god			00 inutilisé
		dc.l	gad_god			04 inutilisé
		dc.l	gad_protect		08 inutilisé
		dc.l	gad_god			12 inutilisé
		dc.l	gad_god			16 inutilisé
		dc.l	gad_god			20 inutilisé
		dc.l	gad_god			24 inutilisé
		dc.l	gad_god			28 inutilisé
		dc.l	gad_live		32 ajoute une vie
		dc.l	gad_god			36 inutilisé
		dc.l	gad_3balls		40 ajoute 3 balles
		dc.l	gad_power7		44 Balle Power1
		dc.l	gad_power9		48 Balle Power2
		dc.l	gad_god			52 inutilisé

*-=-=-=-=-=-=-=-=-=-=-=-=-=-* Ajoute une vie *=-=-=-=-=-=-=-=-=-=-=-=-=-=-*
* entrée	=> a5.l	adr. table 't'
gad_live:
		bsr	lune1
		bra	gad_god

*-=-=-=-=-=-=-=-=-* Protection (Invinsible 2 secondes) *=-=-=-=-=-=-=-=-=-*
* entrée	=> a1.l	Structure raquette
* 		=> a5.l	adr. table 't'
gad_protect:	
		move.b	#1,(sound+S_ATOM.APPARAIT-t,a5)
		move.w	#200,(BUMP.TOUCH,a1)
		bra	gad_god

*-=-=-=-=-=-=-=-=-=-=-=-=* Balle puissance 1 & 2 *-=-=-=-=-=-=-=-=-=-=-=-=*
* entrée	=> a5.l	adr. table 't'
gad_power7:	
		moveq	#2,d0
		moveq	#7,d1
		moveq	#8,d3
		bra.s	amigapower
gad_power9	moveq	#3,d0
		moveq	#9,d1
		moveq	#16,d3
amigapower	move.b	#1,(sound+S_GADGET-t,a5)
		moveq	#NSBALL-1,d2
		move.l	(pt_balle-t,a5),a1
		move.l	(SBOB.TABLE,a1),a0	a0=table adr. gfx & msk
		add.w	d3,a0
		move.l	(a0)+,d3
		move.l	(a0),d4
.f1		tst.w	(SBALLE.ACTIF,a1)	Cherche une balle active
		beq.s	.next
		move.w	d0,(SBALLE.POWER,a1)
		move.w	d1,(SBOB.WIDTH,a1)
		move.l	d3,(SBOB.GFX,a1)
		move.l	d4,(SBOB.MSK,a1)
.next		lea	(SBALLE.SIZE,a1),a1
		dbf	d2,.f1
		bra.s	gad_god

*-=-=-=-=-=-=-=-=-=-=-=-=-=-* Ajoute 3 balles -=-=-=-=-=-=-=-=-=-=-=-=-=-=*
* entrée	=> a5.l	adr. table 't'
gad_3balls:	
		move.b	#1,(sound+S_GADGET-t,a5)
		moveq	#NSBALL-1,d2
		move.l	(pt_balle-t,a5),a1
.f1		tst.w	(SBALLE.ACTIF,a1)	Cherche une balle active
		bne.s	.go
		lea	(SBALLE.SIZE,a1),a1
		dbf	d2,.f1
		rts
.go		moveq	#NSBALL-1,d2
		move.w	(2+SBALLE.DIRECT,a1),d0
		moveq	#2,d1
		move.l	(pt_balle-t,a5),a2
.loop		tst.w	(SBALLE.ACTIF,a2)
		bne.s	.next
		move.w	#1,(SBALLE.ACTIF,a2)
		move.l	(SBALLE.X,a1),(SBALLE.X,a2)
		addq.w	#8,d0
		and.w	#60,d0
		move.w	d0,(2+SBALLE.DIRECT,a2)
		clr.w	(SBALLE.GLUE,a2)
		clr.w	(SBALLE.TILT,a2)
		move.w	(SBALLE.POWER,a1),(SBALLE.POWER,a2)
		move.w	(SBOB.WIDTH,a1),(SBOB.WIDTH,a2)
		move.l	(SBOB.GFX,a1),(SBOB.GFX,a2)
		move.l	(SBOB.MSK,a1),(SBOB.MSK,a2)
		addq.w	#1,(balle_nbre-t,a5)	une balle en plus
		dbf	d1,.next
		bra.s	gad_god
.next		lea	(SBALLE.SIZE,a2),a2
		dbf	d2,.loop

*-=-=-=-=-=-=-* Gadget sort de l'écran ou touche raquette  *-=-=-=-=-=-=-=*
gad_god:	clr.w	(SGAD.ACTIF,a4)
sgad_fin	lea	(SGAD.SIZE,a4),a4
		dbf	d7,gadsloop
		rts

*-------------------------------------------------------------------------*
*---------------------*  Déplace les Lettres BOBs   *---------------------*
*-------------------------------------------------------------------------*
move_sbobtext:
		move.l	(pt_cos-t,a5),a3
		lea	(PT_SIN,a3),a3

		move.l	(pt_bobtext-t,a5),a4
		moveq	#60,d7
		move.w	(tb.l1-t,a5),d6
		bsr.s	.loop1
		moveq	#80,d7
		move.w	(tb.l2-t,a5),d6

.loop1		move.w	#1,(a4)

		*/ Déplacement linéaire en Y */
		cmp.w	(SBOB.POSY1,a4),d7
		bhi.s	.noy1
		subq.w	#8,(SBOB.POSY1,a4)
.noy1
		*/ Déplacement sinus en Y */
		move.w	(SBOB.POSY2,a4),d0
		add.w	#10,d0
		and.w	#1022,d0
		move.w	d0,(SBOB.POSY2,a4)
		moveq	#10,d1
		muls	(a3,d0),d1		d1=sin(a)*rayon
		asr.l	#7,d1
		add.w	(SBOB.POSY1,a4),d1
		move.w	d1,(SBOB.POSY,a4)

		*/ Déplacement en X */
		move.w	(SBOB.POSX1,a4),d0	d0=X arrivé
		sub.w	(SBOB.POSX,a4),d0
		beq.s	.zozo
		move.w	(SBOB.POSX2,a4),d0
		add.w	d0,(SBOB.POSX,a4)
.zozo
		lea	(SBT.SIZE,a4),a4
		dbf	d6,.loop1
		rts
