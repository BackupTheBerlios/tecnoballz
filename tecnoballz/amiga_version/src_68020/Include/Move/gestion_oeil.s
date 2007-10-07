**------------------------------------------------------------------------*
*****************************  GESTION OEIL  ******************************
*-------------------------------------------------------------------------*
gestion_oeil:
		move.l	(pt_oeil-t,a5),a4
		tst.w	(a4)			Oeil actif ?
		beq	.exit			-> non
		lea	(tabsin-t,a5),a1
		lea	(tabcos-t,a5),a2

		*/ Test si le centre du cercle est arrivé */
		move.w	(BOB.POSX1,a4),d0
		move.w	(OEIL.XA,a4),d2
		move.w	d2,d4
		addq.w	#3,d4
		cmp.w	d4,d0			BOB.POSX1>OEIL.XA+3
		bhi.s	.ytst
		subq.w	#6,d4
		cmp.w	d4,d0
		bgt	.new			BOB.POSX1>=OEIL.XA-3
.ytst		move.w	(BOB.POSY1,a4),d1
		move.w	(OEIL.YA,a4),d3
		move.w	d3,d4
		addq.w	#3,d4
		cmp.w	d4,d1			BOB.POSY1>OEIL.YA+3
		bhi.s	.line
		subq.w	#6,d4
		cmp.w	d4,d1
		bgt	.new			BOB.POSY1>=OEIL.YA-3

		*/ Déplace le centre (POSX1,POSY1) vers sa destination */
.line		moveq	#1,d4			s1=1
		moveq	#1,d5			s2=2
		sub.w	d0,d2
		bpl.s	.pos1
		neg.w	d2			d2=DeltaX
		moveq	#-1,d4			s1=-1
.pos1		sub.w	d1,d3
		bpl.s	.pos2
		neg.w	d3			d3=DeltaY
		moveq	#-1,d5			s2=-1
.pos2		moveq	#0,d7			Interchange=0		
		cmp.w	d2,d3			DeltaY<DeltaX ?
		blt.s	.oui
		exg	d2,d3
		moveq	#1,d7			Interchange=1
.oui		add.w	d3,d3
		move.w	d3,d1
		sub.w	d2,d1			d1=2*DeltaY-DeltaX
		add.w	d2,d2
		move.w	d2,d0
		subq.w	#1,d0
.ka		tst.w	d7
		beq.s	.i1
		add.w	d5,(BOB.POSY1,a4)
		bra.s	.n1
.i1		add.w	d4,(BOB.POSX1,a4)	
.n1		sub.w	d2,d1
		bmi.s	.retour
		dbf	d0,.ka	
		rts
.retour		tst.w	d7
		beq.s	.i2
		add.w	d4,(BOB.POSX1,a4)
		bra.s	.n2
.i2		add.w	d5,(BOB.POSY1,a4)	
.n2		add.w	d3,d1
		bra.s	.koko

		*/ Nouveau point de destination pour (POSX1,POSY1) */
.new		move.w	(hasard-t,a5),d0
		add.w	(VHPOSR,a6),d0
		add.w	(VPOSR,a6),d0
		add.w	d6,d0
		and.w	#31,d0
		move.w	(bu.x-t,a5,d0*2),(OEIL.XA,a4)
		add.w	(VHPOSR,a6),d0
		add.w	(VPOSR,a6),d0
		add.w	d6,d0
		and.w	#31,d0
		move.w	(bu.y-t,a5,d0*2),(OEIL.YA,a4)

		*/ Cercle autour de (POSX1,POSY1) */
.koko		move.w	(BOB.POSX2,a4),d0
		addq.w	#8,d0
		cmp.w	#360*2,d0
		blt.s	.danger
		sub.w	#360*2,d0
.danger		move.w	d0,(BOB.POSX2,a4)
		moveq	#10,d1
		moveq	#10,d2
		muls	(a1,d0),d1
		muls	(a2,d0),d2
		asr.l	#7,d1
		asr.l	#7,d2
		add.w	(BOB.POSX1,a4),d1
		add.w	(BOB.POSY1,a4),d2
		sub.w	#15,d1
		sub.w	#15,d2
		move.w	d1,(OEIL.X,a4)
		move.w	d2,(OEIL.Y,a4)

		*/ Animation de l'oeil */
		lea	(BOB.TEMPO,a4),a0
		subq.w	#1,(a0)+		tempo=tempo-1
		bne.s	.noanim			-> tempo > 0
		move.w	(a0)+,(BOB.TEMPO,a4)
		move.w	(a0)+,d0		d0=pt/animation
		cmp.w	#252,d0			pt < au pt maxi ?
		bls.s	.no_maxi2		-> oui
		moveq	#0,d0
		move.w	d0,(BOB.PTANIM,a4)
.no_maxi2	addq.w	#8,(BOB.PTANIM,a4)	
		move.l	(BOB.TABLE,a4),a0	a0=table adr. gfx & msk
		add.w	d0,a0
		move.l	(a0)+,(BOB.GFX,a4)
		move.l	(a0),(BOB.MSK,a4)
.noanim

**************/ inverse les balles qui sont près de l'oeil /***************
		move.w	(OEIL.X,a4),d5
		move.w	(OEIL.Y,a4),d6
		add.w	#15,d5			d5=X1
		add.w	#15,d6			d6=X2
		move.l	#30*30,d4
		move.l	(pt_balle-t,a5),a3	adr. 1ere structure balle
		moveq	#NBALLE-1,d7
.loopball:	tst.w	(a3)
		beq	.next
		move.w	(BOB.WIDTH,a3),d0
		lsr.w	#1,d0
		move.w	(BALLE.X,a3),d1		d1=X2
		add.w	d0,d1
		sub.w	d5,d1			X2-X1
		muls	d1,d1			d1=(X2-X1)*(X2-X1)
		move.w	(BALLE.Y,a3),d2		d2=Y2
		add.w	d0,d2
		sub.w	d6,d2			Y2-Y1
		muls	d2,d2			d2=(Y2-Y1)*(Y2-Y1)
		add.l	d2,d1

		sub.l	d4,d1
		bgt	.next
		move.w	(BALLE.X,a3),d1		d1=X2
		add.w	d0,d1
		sub.w	d5,d1			X2-X1
		move.w	(BALLE.Y,a3),d2		d2=Y2
		add.w	d0,d2
		sub.w	d6,d2			Y2-Y1
		tst.w	d2			Delta Y 
		beq.s	.dy0
		bmi.s	.dyn

		*{{{{ DELTA Y>0 }}}}*
		tst.w	d1
		beq.s	.zdx0
		bmi.s	.zdxn
		cmp.w	d1,d2
		beq.s	.zdxdy2
		bgt.s	.zysx2
		moveq	#60,d0			DY>0 DX>=0 DY<DX
		bra.s	.go
.zysx2		moveq	#52,d0			DY>0 DX>=0 DY>DX
		bra.s	.go
.zdxdy2		moveq	#56,d0			DY>0 DX>=0 DX=DY
		bra.s	.go
.zdxn		neg.w	d1
		cmp.w	d1,d2
		beq.s	.zdxdy1
		bgt.s	.zysx1
		moveq	#36,d0			DY>0 DX<0 DY<DX
		bra.s	.go
.zysx1		moveq	#44,d0			DY>0 DX<0 DY>DX
		bra.s	.go
.zdxdy1		moveq	#40,d0			DY>0 DX<0 DX=DY
		bra.s	.go
.zdx0		moveq	#48,d0			DY>0 DX=0
		bra.s	.go

		*{{{{ DELTA Y<0 }}}}*
.dyn		neg.w	d2
		tst.w	d1
		beq.s	.dx0
		bmi.s	.dxn
		cmp.w	d1,d2
		beq.s	.dxdy2
		bgt.s	.ysx2
		moveq	#04,d0			DY<0 DX>=0 DY<DX
		bra.s	.go
.ysx2		moveq	#12,d0			DY<0 DX>=0 DY>DX
		bra.s	.go
.dxdy2		moveq	#08,d0			DY<0 DX>=0 DX=DY
		bra.s	.go
.dxn		neg.w	d1
		cmp.w	d1,d2
		beq.s	.dxdy1
		bgt.s	.ysx1
		moveq	#28,d0			DY<0 DX<0 DY<DX
		bra.s	.go
.ysx1		moveq	#20,d0			DY<0 DX<0 DY>DX
		bra.s	.go
.dxdy1		moveq	#24,d0			DY<0 DX<0 DX=DY
		bra.s	.go
.dx0		moveq	#16,d0			DY<0 DX=0
		bra.s	.go

.dy0		tst.w	d1
		bpl.s	.oag
		moveq	#32,d0		<-	DY=0 DX =< 0
		bra.s	.go
.oag		moveq	#0,d0		->	DY=0 DX > 0


.go		move.w	d0,(2+BALLE.DIRECT,a3)
.next		lea	(BALLE.SIZE,a3),a3
		dbf	d7,.loopball
.exit		rts
