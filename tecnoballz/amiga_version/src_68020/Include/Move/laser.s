*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*
*«-»«-»«-»«-»«-»«-* Test si une Raquette tire *«-»«-»«-»«-»«-»«-»«-»«-»«-»*
*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*
* Entrée	=> a5.l	Adr. table 't'
laser:
		tst.w	(tableau.end-t,a5)
		bne	.exit
		move.w	(rak.large-t,a5),d0
		sub.w	#16,d0
		lsr.w	#1,d0
		move.l	(.fire_toto,pc,d0),a0

		*/ Prépare Tir Raquette 1 (bas) */
		lea	(b1-t,a5),a1
		tst.w	(BUMP.FIRE,a1)
		beq.s	.laser1
		move.l	(BUMP.OBJET,a1),a4
		move.w	(BUMP.X,a1),d2		pos. x raquette
		move.w	(BUMP.Y,a1),d3		pos. y raquette
		moveq	#1,d1			flag 1 = active bob tir
		jsr	(a0)

.laser1		*/ Prépare Tir Raquette 2 (droite) */
		lea	(b2-t,a5),a1
		tst.w	(BUMP.FIRE,a1)
		beq.s	.laser2
		move.l	(BUMP.OBJET,a1),a4
		move.w	(BUMP.X,a1),d2		pos. x raquette
		move.w	(BUMP.Y,a1),d3		pos. y raquette
		moveq	#1,d1			flag 1 = active bob tir
		jsr	(a0)

.laser2		*/ Prépare Tir Raquette 3 (haut) */
		lea	(b3-t,a5),a1
		tst.w	(BUMP.FIRE,a1)
		beq.s	.laser3
		move.l	(BUMP.OBJET,a1),a4
		move.w	(BUMP.X,a1),d2		pos. x raquette
		move.w	(BUMP.Y,a1),d3		pos. y raquette
		moveq	#1,d1			flag 1 = active bob tir
		jsr	(a0)
.laser3
		*/ Prépare Tir Raquette 4 (gauche) */
		lea	(b4-t,a5),a1
		tst.w	(BUMP.FIRE,a1)
		beq.s	.exit
		move.l	(BUMP.OBJET,a1),a4
		move.w	(BUMP.X,a1),d2		pos. x raquette
		move.w	(BUMP.Y,a1),d3		pos. y raquette
		moveq	#1,d1			flag 1 = active bob tir
		jsr	(a0)
.exit		rts
		CNOP	0,4
.fire_toto	dc.l	fire_p1		00 : raqu. 16 pixels
		dc.l	fire_p2		04 : raqu. 24 pixels
		dc.l	fire_p3		08 : raqu. 32 pixels
		dc.l	fire_p4		12 : raqu. 40 pixels
		dc.l	fire_p5		16 : raqu. 48 pixels
		dc.l	fire_p6		20 : raqu. 56 pixels
		dc.l	fire_p7		24 : raqu. 64 pixels

*---------------------* TIR (1) Raquette 16 pixels *----------------------*
* Entrée	=> d1.w #1
*		=> d2.w BUMP.X
*		=> d3.w BUMP.Y
*		=> a1.l	Structure "BUMP"
*		=> a4.l	Structure "FIRE"
fire_p1:
		cmp.w	#3,(BUMP.FIRE,a1)	Tirs disponible ?
		bne	fire_end		-> non
		move.w	#1,(BUMP.FIRE,a1)
		cmp.w	#2,(BUMP.NUMBER,a1)	Raquette droite ?
		beq.s	.fire_p1a		-> oui
		cmp.w	#4,(BUMP.NUMBER,a1)	Raquette gauche ?
		beq.s	.fire_p1a		-> oui
		addq.w	#6,d2
		bra	fire_p1x
.fire_p1a	addq.w	#6,d3
		bra	fire_p1x

*---------------------* TIR (2) Raquette 24 pixels *----------------------*
fire_p2:	
		cmp.w	#3,(BUMP.FIRE,a1)
		bne	fire_end
fire_o2		move.w	#1,(BUMP.FIRE,a1)
		cmp.w	#2,(BUMP.NUMBER,a1)
		beq.s	.fire_p2a
		cmp.w	#4,(BUMP.NUMBER,a1)
		beq.s	.fire_p2a
		add.w	#10,d2
		bra	fire_p2x
.fire_p2a	add.w	#10,d3
		bra	fire_p2x

*---------------------* TIR (3) Raquette 32 pixels *----------------------*
fire_p3:
		cmp.w	#3,(BUMP.FIRE,a1)
		bne	fire_end
		move.w	#1,(BUMP.FIRE,a1)
		clr.w	(BUMP.TFIRE,a1)
		cmp.w	#2,(BUMP.NUMBER,a1)
		beq.s	.fire_p3a
		cmp.w	#4,(BUMP.NUMBER,a1)
		beq.s	.fire_p3a
		add.w	#14,d2
		bra.s	.fire_p3x
.fire_p3a	add.w	#14,d3
.fire_p3x	move.w	d1,(2*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(2*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(2*FIRE.SIZE+FIRE.Y,a4)
fire_p2x	move.w	d1,(1*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(1*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(1*FIRE.SIZE+FIRE.Y,a4)
fire_p1x	move.w	d1,(0*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(0*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(0*FIRE.SIZE+FIRE.Y,a4)
		bra	fire_end

*---------------------* TIR (4) Raquette 40 pixels *----------------------*
fire_p4:
		cmp.w	#3,(BUMP.FIRE,a1)
		bne	fire_end
		move.w	#1,(BUMP.FIRE,a1)
		clr.w	(BUMP.TFIRE,a1)
		cmp.w	#2,(BUMP.NUMBER,a1)
		beq.s	.fire_p4a
		cmp.w	#4,(BUMP.NUMBER,a1)
		beq.s	.fire_p4a
		move.w	d1,(FIRE.ACTIF,a4)
		move.w	d2,(FIRE.X,a4)
		move.w	d3,(FIRE.Y,a4)
		add.w	#18,d2
		move.w	d1,(FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(FIRE.SIZE+FIRE.Y,a4)
		move.w	d1,(2*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(2*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(2*FIRE.SIZE+FIRE.Y,a4)
		move.w	d1,(3*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	(BUMP.X,a1),d2
		add.w	(rak.large-t,a5),d2
		subq.w	#4,d2
		move.w	d2,(3*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(3*FIRE.SIZE+FIRE.Y,a4)
		bra	fire_end
.fire_p4a	move.w	d1,(FIRE.ACTIF,a4)
		move.w	d2,(FIRE.X,a4)
		move.w	d3,(FIRE.Y,a4)
		add.w	#18,d3
		move.w	d1,(FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(FIRE.SIZE+FIRE.Y,a4)
		move.w	d1,(2*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(2*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(2*FIRE.SIZE+FIRE.Y,a4)
		move.w	d1,(3*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(3*FIRE.SIZE+FIRE.X,a4)
		move.w	(BUMP.Y,a1),d3
		add.w	(rak.large-t,a5),d3
		subq.w	#4,d3
		move.w	d3,(3*FIRE.SIZE+FIRE.Y,a4)
		bra	fire_end

*---------------------* TIR (5) Raquette 48 pixels *----------------------*
fire_p5:	
		cmp.w	#3,(BUMP.FIRE,a1)
		bne	fire_end
		move.w	#1,(BUMP.FIRE,a1)
		clr.w	(BUMP.TFIRE,a1)
		cmp.w	#2,(BUMP.NUMBER,a1)
		beq.s	.fire_p5a
		cmp	#4,(BUMP.NUMBER,a1)
		beq.s	.fire_p5a
		move.w	(BUMP.FY1,a1),d4
		move.w	d1,(0*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(0*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(0*FIRE.SIZE+FIRE.Y,a4)
		add.w	d4,d3
		move.w	d1,(1*FIRE.SIZE+FIRE.ACTIF,a4)
		add.w	#12,d2
		move.w	d2,(1*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(1*FIRE.SIZE+FIRE.Y,a4)
		add.w	d4,d3
		move.w	d1,(2*FIRE.SIZE+FIRE.ACTIF,a4)
		add.w	#12,d2
		move.w	d2,(2*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(2*FIRE.SIZE+FIRE.Y,a4)
		sub.w	d4,d3
		move.w	d1,(3*FIRE.SIZE+FIRE.ACTIF,a4)
		add.w	#12,d2
		move.w	d2,(3*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(3*FIRE.SIZE+FIRE.Y,a4)
		sub.w	d4,d3
		move.w	d1,(4*FIRE.SIZE+FIRE.ACTIF,a4)
		add.w	#10,d2
		move.w	d2,(4*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(4*FIRE.SIZE+FIRE.Y,a4)
		bra	fire_end
.fire_p5a	move.w	(BUMP.FX1,a1),d4
		move.w	d1,(0*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(0*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(0*FIRE.SIZE+FIRE.Y,a4)
		add.w	d4,d2
		move.w	d1,(1*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(1*FIRE.SIZE+FIRE.X,a4)
		add.w	d4,d2
		add.w	#12,d3
		move.w	d3,(1*FIRE.SIZE+FIRE.Y,a4)
		move.w	d1,(2*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(2*FIRE.SIZE+FIRE.X,a4)
		sub.w	d4,d2
		add.w	#12,d3
		move.w	d3,(2*FIRE.SIZE+FIRE.Y,a4)
		move.w	d1,(3*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(3*FIRE.SIZE+FIRE.X,a4)
		sub.w	d4,d2
		add.w	#12,d3
		move.w	d3,(3*FIRE.SIZE+FIRE.Y,a4)
		move.w	d1,(4*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(4*FIRE.SIZE+FIRE.X,a4)
		add.w	#10,d3
		move.w	d3,(4*FIRE.SIZE+FIRE.Y,a4)
		bra	fire_end

*---------------------* TIR (6) Raquette 56 pixels *----------------------*
fire_p6:	
		cmp.w	#3,(BUMP.FIRE,a1)
		bne	fire_end
		move.w	#1,(BUMP.FIRE,a1)
		clr.w	(BUMP.TFIRE,a1)
		cmp.w	#2,(BUMP.NUMBER,a1)
		beq.s	.fire_p6a
		cmp.w	#4,(BUMP.NUMBER,a1)
		beq.s	.fire_p6a
		move.w	d3,d4
		add.w	(BUMP.YDECA,a1),d4
		move.w	d1,(FIRE.ACTIF,a4)
		move.w	d2,(FIRE.X,a4)
		move.w	d4,(FIRE.Y,a4)
		move.w	d1,(FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(FIRE.SIZE+FIRE.Y,a4)
		move.w	d1,(2*FIRE.SIZE+FIRE.ACTIF,a4)
		add.w	#22,d2
		move.w	d2,(2*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(2*FIRE.SIZE+FIRE.Y,a4)
		move.w	d1,(3*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(3*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(3*FIRE.SIZE+FIRE.Y,a4)
		move.w	d1,(4*FIRE.SIZE+FIRE.ACTIF,a4)
		add.w	#22,d2
		move.w	d2,(4*FIRE.SIZE+FIRE.X,a4)
		move.w	d4,(4*FIRE.SIZE+FIRE.Y,a4)
		move.w	d1,(5*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(5*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(5*FIRE.SIZE+FIRE.Y,a4)
		bra	fire_end
.fire_p6a	move.w	d2,d4
		add.w	(BUMP.XDECA,a1),d4
		move.w	d1,(FIRE.ACTIF,a4)
		move.w	d4,(FIRE.X,a4)
		move.w	d3,(FIRE.Y,a4)
		move.w	d1,(FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(FIRE.SIZE+FIRE.Y,a4)
		move.w	d1,(2*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(2*FIRE.SIZE+FIRE.X,a4)
		add.w	#22,d3
		move.w	d3,(2*FIRE.SIZE+FIRE.Y,a4)
		move.w	d1,(3*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(3*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(3*FIRE.SIZE+FIRE.Y,a4)
		move.w	d1,(4*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d4,(4*FIRE.SIZE+FIRE.X,a4)
		add.w	#22,d3
		move.w	d3,(4*FIRE.SIZE+FIRE.Y,a4)
		move.w	d1,(5*FIRE.SIZE+FIRE.ACTIF,a4)
		move.w	d2,(5*FIRE.SIZE+FIRE.X,a4)
		move.w	d3,(5*FIRE.SIZE+FIRE.Y,a4)
		bra	fire_end

*---------------------* TIR (7) Raquette 64 pixels *----------------------*
fire_p7		
		moveq	#2,d1
		cmp.w	#3,(BUMP.FIRE,a1)	Le joueur vient de tirer ?
		beq	.fire_p71		-> oui
		tst.w	(FIRE.ACTIF,a4)
		bne	fire_end
		tst.w	(FIRE.SIZE+FIRE.ACTIF,a4)
		bne	fire_end
		tst.w	(2*FIRE.SIZE+FIRE.ACTIF,a4)
		bne	fire_end
		tst.w	(3*FIRE.SIZE+FIRE.ACTIF,a4)
		bne	fire_end
		tst.w	(4*FIRE.SIZE+FIRE.ACTIF,a4)
		bne	fire_end
		tst.w	(5*FIRE.SIZE+FIRE.ACTIF,a4)
		bne	fire_end
		tst.w	(6*FIRE.SIZE+FIRE.ACTIF,a4)
		bne	fire_end
		moveq	#1,d1
		*/ initialise les pointeurs sinus */
		clr.w	(FIRE.SINUS,a4)
		move.w	#16,(FIRE.SIZE+FIRE.SINUS,a4)
		move.w	#32,(2*FIRE.SIZE+FIRE.SINUS,a4)
		move.w	#48,(3*FIRE.SIZE+FIRE.SINUS,a4)
		move.w	#64,(4*FIRE.SIZE+FIRE.SINUS,a4)
		move.w	#80,(5*FIRE.SIZE+FIRE.SINUS,a4)
		move.w	#96,(6*FIRE.SIZE+FIRE.SINUS,a4)
.fire_p71	move.w	#1,(BUMP.FIRE,a1)
		moveq	#0,d2
		move.w	d2,(BUMP.TFIRE,a1)
		move.w	(BUMP.X,a1),d2
		add.w	(BUMP.XSCIE,a1),d2
		swap	d2
		move.w	(BUMP.Y,a1),d2
		add.w	(BUMP.YSCIE,a1),d2
		move.w	d1,(FIRE.ACTIF,a4)
		move.l	d2,(FIRE.XSCIE,a4)
		move.w	d1,(FIRE.SIZE+FIRE.ACTIF,a4)
		move.l	d2,(FIRE.SIZE+FIRE.XSCIE,a4)
		move.w	d1,(2*FIRE.SIZE+FIRE.ACTIF,a4)
		move.l	d2,(2*FIRE.SIZE+FIRE.XSCIE,a4)
		move.w	d1,(3*FIRE.SIZE+FIRE.ACTIF,a4)
		move.l	d2,(3*FIRE.SIZE+FIRE.XSCIE,a4)
		move.w	d1,(4*FIRE.SIZE+FIRE.ACTIF,a4)
		move.l	d2,(4*FIRE.SIZE+FIRE.XSCIE,a4)
		move.w	d1,(5*FIRE.SIZE+FIRE.ACTIF,a4)
		move.l	d2,(5*FIRE.SIZE+FIRE.XSCIE,a4)
		move.w	d1,(6*FIRE.SIZE+FIRE.ACTIF,a4)
		move.l	d2,(6*FIRE.SIZE+FIRE.XSCIE,a4)
fire_end	rts
