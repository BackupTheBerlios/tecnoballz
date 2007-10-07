*.««»».««»».««»».««»».««»».««»».««»».««»».««»».««»».««»».««»».««»».««»».««*
*«»*«»*«»*«»*«»*«* Déplacement des Tirs des 4 Raquettes *«»*«»*«»*«»*«»*«»*
*.««»».««»».««»».««»».««»».««»».««»».««»».««»».««»».««»».««»».««»».««»».««*
* Entrée	=> a5.l	Adr. table 't'
tir_deplacement:
		move.l	a6,-(a7)
		tst.w	(tableau.end-t,a5)
		bne.s	.exit
		move.w	(rak.large-t,a5),d0	longeur raquette
		sub.w	#16,d0
		lsr.w	#1,d0
		move.l	(.fire_tata,pc,d0),a4
		lea	(b1-t,a5),a1
		move.l	(BUMP.OBJET,a1),a6
		jsr	(a4)			Raquette n°1 (bas)
		lea	(b2-t,a5),a1
		move.l	(BUMP.OBJET,a1),a6
		jsr	(a4)			Raquette n°2 (droite)
		lea	(b3-t,a5),a1
		move.l	(BUMP.OBJET,a1),a6
		jsr	(a4)			Raquette n°3 (haut)
		lea	(b4-t,a5),a1
		move.l	(BUMP.OBJET,a1),a6
		jsr	(a4)			Raquette n°4 (droite)
.exit		move.l	(a7)+,a6
		rts
		CNOP	0,4
.fire_tata	dc.l	move_1		00 : raqu. 16 pixels
		dc.l	move_2		04 : raqu. 24 pixels
		dc.l	move_3		08 : raqu. 32 pixels
		dc.l	move_4		12 : raqu. 40 pixels
		dc.l	move_5		16 : raqu. 48 pixels
		dc.l	move_6		20 : raqu. 56 pixels
		dc.l	move_7		24 : raqu. 64 pixels

*-------------------------------------------------------------------------*
*        TIR 1 : Déplace 1 tir en ligne droite (Bumper 16 pixels)	  *
*-------------------------------------------------------------------------*
* Entrée	=> a6.l	Adr. structure "FIRE"
move_1:		move.l	a6,a3
		tst.w	(FIRE.ACTIF,a3)		Tir actif ?
		beq	move_fin		-> non
		move.w	(BUMP.FX0,a1),d0	Inc. x
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY0,a1),d0	Inc. en y
		add.w	d0,(FIRE.Y,a3)
		bra	move_o5

*-------------------------------------------------------------------------*
*          TIR 2 : Déplace 2 tirs en zig-zag (Bumper 24 pixels)		  *
*-------------------------------------------------------------------------*
* Entrée	=> a6.l	Adr. structure "FIRE"
		*-=- 1er tir -=-*
move_2:		move.l	a6,a3
		tst.w	(FIRE.ACTIF,a3)		Tir actif ?
		beq	move_2c			-> non
		cmp.w	#10,(BUMP.TFIRE,a1)
		ble.s	move_2b
move_2a		move.w	(BUMP.FX1,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY1,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bsr	move_o5			limite 1er tir
		bra.s	move_2c
move_2b		move.w	(BUMP.FX2,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY2,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bsr	move_o5			limite 1er tir

		*-=- 2ieme tir -=-*
move_2c		lea	(FIRE.SIZE,a6),a3
		tst.w	(FIRE.ACTIF,a3)		Tir actif ?
		beq	move_2e			-> non
		cmp.w	#10,(BUMP.TFIRE,a1)
		ble.s	move_2d
		move.w	(BUMP.FX2,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY2,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bsr	move_o5			limite 1er tir
		bra.s	move_2e
move_2d		move.w	(BUMP.FX1,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY1,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bsr	move_o5			limite 1er tir
move_2e		addq.w	#1,(BUMP.TFIRE,a1)
		cmp.w	#21,(BUMP.TFIRE,a1)
		bne	move_fin
		clr.w	(BUMP.TFIRE,a1)
		bra	move_fin

*-------------------------------------------------------------------------*
*          TIR 3 : Déplace 2 tirs en triangle (Bumper 32 pixels)	  *
*-------------------------------------------------------------------------*
* Entrée	=> a6.l	Adr. structure "FIRE"
		*-=- 1er tir ,vers la gauche -=-*
move_3		move.l	a6,a3
		tst.w	(FIRE.ACTIF,a3)
		beq	move_3b
move_3a		move.w	(BUMP.FX1,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY1,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bsr	move_o5			limite 1er tir

		*-=- 2ieme tir, tout droit -=-*
move_3b		lea	(FIRE.SIZE,a6),a3
		tst.w	(FIRE.ACTIF,a3)
		beq	move_3c
		move.w	(BUMP.FX0,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY0,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bsr	move_o5			limite 2eme tir

		*-=- 3ieme tir, vers la droite -=-*
move_3c		lea	(FIRE.SIZE*2,a6),a3
		tst.w	(FIRE.ACTIF,a3)
		beq	move_o5
		move.w	(BUMP.FX2,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY2,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bra	move_o5			limite 3eme tir

*-------------------------------------------------------------------------*
*           TIR 4 : Déplace 4 tirs combinés (Bumper 40 pixels)		  *
*-------------------------------------------------------------------------*
* Entrée	=> a6.l	Adr. structure "FIRE"
		*-=- 1er tir, tout droit -=-*
move_4:		move.l	a6,a3
		tst.w	(FIRE.ACTIF,a3)
		beq	move_4b
move_4a		move.w	(BUMP.FX0,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY0,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bsr	move_o5			limite 1er tir

		*-=- 2ieme tir, triangle -=-*
move_4b		lea	(FIRE.SIZE,a6),a3
		tst.w	(FIRE.ACTIF,a3)
		beq	move_4c
		cmp.w	#10,(BUMP.TFIRE,a1)
		ble.s	move_4b1
		move.w	(BUMP.FX1,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY1,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bsr	move_o5
		bra.s	move_4c
move_4b1	move.w	(BUMP.FX2,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY2,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bsr	move_o5

		*-=- 3ieme tir, triangle -=*
move_4c		lea	(FIRE.SIZE*2,a6),a3
		tst.w	(FIRE.ACTIF,a3)
		beq	move_4d
		cmp.w	#10,(BUMP.TFIRE,a1)
		ble.s	move_4c1
		move.w	(BUMP.FX2,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY2,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bsr	move_o5
		bra.s	move_4c2
move_4c1	move.w	(BUMP.FX1,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY1,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bsr	move_o5
move_4c2	addq.w	#1,(BUMP.TFIRE,a1)
		cmp.w	#21,(BUMP.TFIRE,a1)
		bne	move_4d
		clr.w	(BUMP.TFIRE,a1)

		*-=- 4ieme tir, tout droit -=-*
move_4d		lea	(FIRE.SIZE*3,a6),a3
		tst.w	(FIRE.ACTIF,a3)
		beq	move_o5
		move.w	(BUMP.FX0,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY0,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bra	move_o5			limite 4eme tir

*-------------------------------------------------------------------------*
*           TIR 5 : Déplace 5 tirs combinés (Bumper 48 pixels)		  *
*-------------------------------------------------------------------------*
* Entrée	=> a6.l	Adr. structure "FIRE"
		*-=- 1er projectile ,tout doit -=-*
move_5:		move.l	a6,a3
		tst.w	(FIRE.ACTIF,a3)
		beq	move_5a
		move.w	(BUMP.FX0,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY0,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bsr	move_o5			limite 1er tir

		*-=- 2eme projectile ,tout doit -=-*
move_5a		lea	(FIRE.SIZE,a6),a3
		tst.w	(FIRE.ACTIF,a3)
		beq	move_5b
		move.w	(BUMP.FX0,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY0,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bsr	move_o5			limite 1er tir

		*-=- 3eme projectile ,tout doit -=-*
move_5b		lea	(FIRE.SIZE*2,a6),a3
		tst.w	(FIRE.ACTIF,a3)
		beq	move_5c
		move.w	(BUMP.FX0,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY0,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bsr	move_o5			limite 1er tir

		*-=- 4eme projectile ,tout doit -=-*
move_5c		lea	(FIRE.SIZE*3,a6),a3
		tst.w	(FIRE.ACTIF,a3)
		beq	move_5d
		move.w	(BUMP.FX0,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY0,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bsr	move_o5			limite 1er tir

		*-=- 5eme projectile ,tout doit -=-*
move_5d		lea	(FIRE.SIZE*4,a6),a3
		tst.w	(FIRE.ACTIF,a3)
		beq	move_o5
		move.w	(BUMP.FX0,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY0,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bra	move_o5			limite 1er tir

*-------------------------------------------------------------------------*
*         TIR 6 : Déplace 6 tirs en zig-zag (Bumper de 56 pixels)	  *
*-------------------------------------------------------------------------*
* Entrée	=> a6.l	Adr. structure "FIRE"
move_6		move.l	a6,a3
		bsr	move_62
		lea	(FIRE.SIZE,a6),a3
		bsr	move_62
		lea	(FIRE.SIZE*2,a6),a3
		bsr	move_61
		lea	(FIRE.SIZE*3,a6),a3
		bsr	move_6b
		lea	(FIRE.SIZE*4,a6),a3
		bsr	move_62
		lea	(FIRE.SIZE*5,a6),a3
		bsr	move_62
		addq.w	#1,(BUMP.TFIRE,a1)
		cmp.w	#21,(BUMP.TFIRE,a1)
		bne	move_fin
		clr.w	(BUMP.TFIRE,a1)
		bra	move_fin
move_61		tst.w	(FIRE.ACTIF,a3)
		beq.s	move_6d
		cmp.w	#10,(BUMP.TFIRE,a1)
		ble.s	move_6a
		move.w	(BUMP.FX1,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY1,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bra	move_o5
move_6a		move.w	(BUMP.FX2,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY2,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bra	move_o5
move_6d		bra	move_fin
move_62		tst.w	(FIRE.ACTIF,a3)
		beq.s	move_6d
		move.w	(BUMP.FX0,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY0,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bra	move_o5
move_6b		tst.w	(FIRE.ACTIF,a3)
		beq.s	move_6d
		cmp.w	#10,(BUMP.TFIRE,a1)
		ble.s	move_6c
		move.w	(BUMP.FX2,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY2,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bra	move_o5
move_6c		move.w	(BUMP.FX1,a1),d0
		add.w	d0,(FIRE.X,a3)
		move.w	(BUMP.FY1,a1),d0
		add.w	d0,(FIRE.Y,a3)
		bra	move_o5

*-------------------------------------------------------------------------*
*               TIR 7 : scie circulaire (Bumper 64 pixels)		  *
*-------------------------------------------------------------------------*
* Entrée	=> a6.l	Adr. structure "FIRE"
move_7		lea	(sinus-t,a5),a0
		move.l	a6,a3
		bsr	move_71
		lea	(FIRE.SIZE,a6),a3
		bsr	move_71
		lea	(FIRE.SIZE*2,a6),a3
		bsr	move_71
		lea	(FIRE.SIZE*3,a6),a3
		bsr	move_71
		lea	(FIRE.SIZE*4,a6),a3
		bsr	move_71
		lea	(FIRE.SIZE*5,a6),a3
		bsr	move_71
		lea	(FIRE.SIZE*6,a6),a3
move_71		tst	(FIRE.ACTIF,a3)
		beq	move_fin
		addq.w	#4,(FIRE.SINUS,a3)
		move.w	(FIRE.SINUS,a3),d4
		cmp.w	#99,(a0,d4)
		bne.s	move_72
		clr.w	d4
		move.w	d4,(FIRE.SINUS,a3)
move_72		cmp.w	#2,(FIRE.ACTIF,a3)
		bne.s	move_73
		move.w	(BUMP.FX0,a1),d0
		add.w	d0,(FIRE.XSCIE,a3)
		move.w	(BUMP.FY0,a1),d0
		add.w	d0,(FIRE.YSCIE,a3)
		move.w	(a0,d4),d0
		add.w	(FIRE.XSCIE,a3),d0
		move.w	d0,(FIRE.X,a3)
		move.w	(2,a0,d4),d0
		add.w	(FIRE.YSCIE,a3),d0
		move.w	d0,(FIRE.Y,a3)
		bra.s	move_o5
move_73		move.w	(BUMP.X,a1),d2
		add.w	(BUMP.XSCIE,a1),d2
		move.w	(BUMP.Y,a1),d3
		add.w	(BUMP.YSCIE,a1),d3
		move.w	(a0,d4),(FIRE.X,a3)
		add.w	d2,(FIRE.X,a3)
		move.w	(2,a0,d4),(FIRE.Y,a3)
		add.w	d3,(FIRE.Y,a3)

*---------------------------> VERIFIE LIMITES <---------------------------*
move_o5:
		cmp.w	#YRAKH,(FIRE.Y,a3)
		blt.s	.test_b0
		cmp.w	#XRAKG,(FIRE.X,a3)
		ble.s	.test_b0
		cmp.w	#YRAKB,(FIRE.Y,a3)
		bge.s	.test_b0
		cmp.w	#XRAKD,(FIRE.X,a3)
		ble.s	move_fin
.test_b0	clr.w	(FIRE.ACTIF,a3)
move_fin:	rts
