*=========================================================================*
*>>>>>>>>>>>>>>>>>> Gestion de la vitesse de défilement <<<<<<<<<<<<<<<<<<*
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
gere_scroll:
		*- gère vitesse : saute à la routine -*
		move.w	(gd.pt-t,a5),d0		pointeur gardien (00 to 40 step 4)
		jsr	([.rscroll,pc,d0])

		*- gère vitesse : test dépassement vitesse -*
		move	(gd.sspeed-t,a5),d0
		cmp.w	#15,d0			V < ou = 15 ?
		ble.s	.gs1			-> oui
		moveq	#15,d0			non, V = 15
.gs1		cmp.w	#-15,d0			V > ou = -15 ?
		bge.s	.gs2			-> oui
		moveq	#-15,d0			non, V = -15
.gs2		move.w	d0,(gd.sspeed-t,a5)
.gs_end		rts
		* gère vitesse : liste des routines pour chaque gardien *
.rscroll:	dc.l	scroll_00		gard_i1=00	intermédiare area 1
		dc.l	scroll_04		gard_f1=04	final area 1
		dc.l	scroll_08		gard_i2=08	intermédiaire area 2
		dc.l	scroll_12		gard_f2=12	final area 2
		dc.l	scroll_16		gard_i3=16	intermédiaire area 3
		dc.l	scroll_20		gard_f3=20	final area 3
		dc.l	scroll_24		gard_i4=24	intermédiaire area 4
		dc.l	scroll_28		gard_f4=28	final area 4
		dc.l	scroll_32		gard_i5=32	intermédiaire area 5
		dc.l	scroll_36		gard_f5=36	final area 5
		dc.l	scroll_40		gard_ff=40	final TecnoballZ

******************Intermédiaire (Area 1) = SCROLL DOWN =*******************
* entrée	=> a5.l	adr. table 't'
scroll_00:
		tst.w	(gd.sspeed-t,a5)	vitesse défilement ?
		beq.s	.scroll_002		-> non, pas de défilement vertical
		tst.w	([gd1-t,a5],GARD.POWER)	gardien 1 est détruit ?
		bne.s	.scroll_001		-> non
		tst.w	([gd2-t,a5],GARD.POWER)	gardien 2 est détruit ?
		bne.s	.scroll_001		-> non
		move.w	#2,(gd.sspeed-t,a5)	V = 2
		bra.s	.scroll_001		-> sort
.scroll_002:	tst.w	([gd1-t,a5],GARD.POWER)	gardien 1 est detruit ?
		bne.s	.scroll_003		-> non
		move.w	#1,(gd.sspeed-t,a5)	gardien 1 détruit, V = 1
		bra.s	.scroll_001		-> sort
.scroll_003:	tst.w	([gd2-t,a5],GARD.POWER)	gardien 2 est detruit ?
		bne.s	.scroll_001		-> non
		move.w	#1,(gd.sspeed-t,a5)	gardien 2 détruit, V = 1
.scroll_001:	rts

********************Final (Area 1) = SCROLL MONSTER 1 =********************
* Entrée	=> a5.l	adr. table 't'
scroll_04:	
		tst.w	([gd1-t,a5],GARD.POWER)	gardien detruit ?
		beq.s	.scroll_042		-> oui, sort
		move.l	(gd1-t,a5),a0
		move.w	(GARD.Y,a0),d0		Y Gardien
		move	(gd.lasty-t,a5),d1	dernière pos. y du gardien
		beq.s	.scroll_041		-> pas de dernière pos. y
		sub.w	d0,d1
		asr.w	#1,d1
		beq.s	.scroll_041
		move.w	d1,(gd.sspeed-t,a5)	vitesse de défilement
.scroll_041	move.w	d0,(gd.lasty-t,a5)	sauve pos. y
.scroll_042	rts

**************Intermédiaire (Area 2)  = SCROLL COLLISION 1 =***************
* Entrée	=> a5.l	adr. table 't'
scroll_08:	
		move.l	(gd1-t,a5),a0		adr. structure gardien 1
		tst.w	(GARD.TOUCH,a0)		gardien 1 touché ?
		beq.s	.scroll_082		-> non
		move.w	#-1,(gd.sspeed-t,a5)	Défilement vers le haut
		bra.s	.scroll_081		-> non
.scroll_082	move.l	(gd2-t,a5),a0		Adr. structure gardien 2
		tst.w	(GARD.TOUCH,a0)		Gardien 2 touché ?
		beq.s	.scroll_081		-> non
		move.w	#1,(gd.sspeed-t,a5)	Défilement vers le bas
.scroll_081	rts

*********************Final (Area 2)  = SCROLL BALL 1 =*********************
* entrée	=> a5.l	adr. table 't'
scroll_12:	
		tst.w	([gd1-t,a5],GARD.POWER)	gardien detruit ?
		beq.s	.scroll_122		-> oui
		move.l	(pt_balle-t,a5),a0
		tst.w	(SBALLE.GLUE,a0)	balle collée ?
		beq.s	.scroll_120		-> non
		move.w	#-1,(gd.sspeed-t,a5)	Si balle collée V = -1 
		clr.w	(gd.lasty-t,a5)
		bra.s	.scroll_122
.scroll_120	move.w	(SBALLE.Y,a0),d0	Pos. y de la balle
		move.w	(gd.lasty-t,a5),d1	Dernière pos. y de la balle
		beq.s	.scroll_121		-> pas de dernière pos. y
		sub.w	d0,d1
		asr.w	#1,d1
		beq.s	.scroll_121
		move.w	d1,(gd.sspeed-t,a5)	vitesse de défilement
.scroll_121	move.w	d0,(gd.lasty-t,a5)	sauve pos. y
.scroll_122	rts

***************Intermédiaire (Area 3) = SCROLL COLLISION 2 = **************
* Entrée	=> a5.l	adr. table 't'
scroll_16:
		move.l	(gd1-t,a5),a0		adr. structure gardien 1
		cmp.w	#1,(GARD.TOUCH,a0)	gardien 1 touché ?
		bne.s	.scroll_162		-> non
		subq.w	#1,(gd.sspeed-t,a5)	V = V - 1
		cmp.w	#-4,(gd.sspeed-t,a5)
		bge.s	.scroll_161
		move.w	#-4,(gd.sspeed-t,a5)
		bra.s	.scroll_161
.scroll_162	move.l	(gd2-t,a5),a0		adr. structure gardien 2
		cmp.w	#1,(GARD.TOUCH,a0)	gardien 2 touché ?
		bne.s	.scroll_161		-> non
		addq.w	#1,(gd.sspeed-t,a5)	V = V + 1
		cmp.w	#4,(gd.sspeed-t,a5)
		ble.s	.scroll_161
		move.w	#4,(gd.sspeed-t,a5)
.scroll_161	rts

******************* Final (Area 3) = SCROLL MONSTER 2 = *******************
* Entrée	=> a5.l	adr. table 't'
scroll_20:	
		tst.w	([gd1-t,a5],GARD.POWER)	gardien detruit ?
		beq.s	.scroll_222		-> oui
		move.l	(gd1-t,a5),a0		adr. structure gardien 1
		move.w	(GARD.Y,a0),d0		pos. y du gardien
		move	(gd.lasty-t,a5),d1	Dernière pos. y du gardien
		beq.s	.scroll_221		-> pas de dernière pos. y
		sub.w	d0,d1
		beq.s	.scroll_221
		neg.w	d1
		add.w	d1,d1
		move.w	d1,(gd.sspeed-t,a5)	vitesse de défilement
.scroll_221	move.w	d0,(gd.lasty-t,a5)	sauve pos. y
.scroll_222	rts

****************  Intermédiaire (Area 4) = SCROLL TIR 1 = *****************
* entrée	=> a5.l	adr. table 't'
scroll_24:
		move.l	(missile-t,a5),a0	adr. structure tir 1
		tst.w	(MIS.ACTIF,a0)		tir 1 actif ?
		bne.s	.scroll_242		-> oui
		clr.w	(gd.lasty-t,a5)		raz ancienne pos. y du tir
		bra.s	.scroll_241		-> sort
.scroll_242	move.w	(MIS.Y,a0),d1		pos. y du tir
		move.w	(gd.lasty-t,a5),d0	dernière pos. y du tir
		beq.s	.scroll_240		-> pas de dernière pos. y
		sub.w	d1,d0
		neg.w	d0
		add.w	d0,(gd.sspeed-t,a5)	V = V + ( newY - oldY )
		cmp.w	#15,(gd.sspeed-t,a5)	V < ou = 15 ?
		ble.s	.scroll_243		-> oui
		move.w	#15,(gd.sspeed-t,a5)	non, V = 15
.scroll_243	cmp.w	#-15,(gd.sspeed-t,a5)	V > ou = -15 ?
		bge.s	.scroll_240		-> oui
		move.w	#-15,(gd.sspeed-t,a5)	non, V = -15
.scroll_240	move.w	d1,(gd.lasty-t,a5)
.scroll_241	rts

*********************Final (Area 4) = SCROLL TIR 2 =***********************
* entrée	=> a5.l	adr. table 't'
scroll_28:	
		move.l	(missile-t,a5),a0	adr. structure tir 1
		tst.w	(MIS.ACTIF,a0)		tir 1 actif ?
		bne.s	.scroll_282		-> oui
		clr.w	(gd.lasty-t,a5)		raz ancienne pos. y du tir
		bra.s	.scroll_281		-> sort
.scroll_282	move.w	(MIS.Y,a0),d1		Pos. y du tir
		move.w	(gd.lasty-t,a5),d0	dernière pos. y du tir
		beq.s	.scroll_280		-> pas de dernière pos. y
		sub.w	d1,d0
		add.w	d0,(gd.sspeed-t,a5)	V = V + ( oldY - newY )
		cmp.w	#15,(gd.sspeed-t,a5)	V < ou = 15 ?
		ble.s	.scroll_283		-> oui
		move.w	#15,(gd.sspeed-t,a5)	non, V = 15
.scroll_283	cmp.w	#-15,(gd.sspeed-t,a5)	V > ou = -15 ?
		bge.s	.scroll_280		-> oui
		move.w	#-15,(gd.sspeed-t,a5)	non, V = -15
.scroll_280	move.w	d1,(gd.lasty-t,a5)
.scroll_281	rts

****************Intermédiaire (Area 5) = SCROLL COLLISION =****************
* Entrée	=> a5.l	adr. table 't'
scroll_32:	
		move.l	(gd1-t,a5),a0		adresse structure gardien 1
		cmp.w	#1,(GARD.TOUCH,a0)	Gardien 1 touché ?
		bne.s	.scroll_322		-> non
		subq.w	#1,(gd.sspeed-t,a5)	V=V-1
		bra.s	.scroll_321
.scroll_322	move.l	(gd2-t,a5),a0		adresse structure gardien 2
		cmp.w	#1,(GARD.TOUCH,a0)	Gardien 2 touché ?
		bne.s	.scroll_321		-> non
		addq.w	#1,(gd.sspeed-t,a5)	V=V+1
.scroll_321	rts

***************************** Final (Area 5) ******************************
* entrée	=> a5.l	adr. table 't'
scroll_36:	
		move.w	#8,(gd.sspeed-t,a5)
		rts

************************** Final-Final (Area 5) ***************************
* entrée	=> a5.l	adr. table 't'
scroll_40:	
		move.w	#-8,(gd.sspeed-t,a5)
		rts
