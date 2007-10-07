*=========================================================================*
***-----------------------Initialise LE BOB TEXT -----------------------***
*=========================================================================*
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d7.l 0 pas d'erreur
init_bobtext:
		lea	(ze_bobtext-t,a5),a0
		move.b	(1+lev0-t,a5),(7,a0)
		move.b	(1+lev1-t,a5),(6,a0)
		add.b	#'\',(6,a0)
		add.b	#'\',(7,a0)
		move.w	#256,(tb.la-t,a5)


		*/ Compte le nombre de lettres /* 
		move.l	a0,(pt_tb-t,a5)
		move.l	(pt_tb-t,a5),a0
		moveq	#0,d0
.n0		tst.b	(a0)+
		beq.s	.n2
		addq.w	#1,d0
		bra.s	.n0
.n2		move.w	d0,(tb.l1-t,a5)
		subq.w	#1,(tb.l1-t,a5)
		moveq	#0,d1
.n3		tst.b	(a0)+
		beq.s	.n4
		addq.w	#1,d1
		bra.s	.n3
.n4		move.w	d1,(tb.l2-t,a5)
		subq.w	#1,(tb.l2-t,a5)
		add.w	d1,d0
		move.w	d0,(tb.lx-t,a5)
		subq.w	#1,(tb.lx-t,a5)

		*/ Réserve les structures  */
		mulu	#BT.SIZE,d0
		lea	(pt_bobtext-t,a5),a4
		move.l	d0,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne	.exit

		*/ Init Les structures */
		move.l	(pt_tb-t,a5),a2
		move.w	(tb.lx-t,a5),d6
		move.l	(pt_bobtext-t,a5),a1
.loopinit	moveq	#BOB.TEXT,d0
		bsr	bob32.only
		bne	.exit
		bsr	shado.ajoute
		bne	.exit
		clr.l	(BOB.ACTIF,a1)
.nnn		move.b	(a2)+,d0
		beq.s	.nnn
		sub.b	#'A',d0
		lsl.w	#3,d0
		move.l	(BOB.TABLE,a1),a0	a0=table adr. gfx & msk
		move.w	d0,(BOB.PTANIM,a1)
		move.l	(0,a0,d0),(BOB.GFX,a1)
		move.l	(4,a0,d0),(BOB.MSK,a1)
		lea	(BT.SIZE,a1),a1
		dbf	d6,.loopinit

		*/ Init structures */
		moveq	#0,d7			d7=pt/table sinus
		move.l	(pt_bobtext-t,a5),a1
		move.w	(tb.l1-t,a5),d6
		move.w	#200,d2
		bsr.s	.sex
		move.w	(tb.l2-t,a5),d6
		move.w	#220,d2
.sex		moveq	#0,d5
		move.w	(tb.la-t,a5),d5
		sub.w	#16,d5
		divu	d6,d5
		moveq	#0,d4
		move.w	d6,d3
		addq.w	#1,d3
		lsl.w	#4,d3
		sub.w	(tb.la-t,a5),d3
		neg.w	d3
		lsr.w	#1,d3			d3=Pos. X
.lo1		move.w	d4,(BOB.POSX,a1)	X départ
		move.w	d2,(BOB.POSY1,a1)
		move.w	d3,(BOB.POSX1,a1)	X arrivé
		move.w	d7,(BOB.POSY2,a1)
		addq.w	#8,d7
		and.w	#1022,d7
		clr.w	(BOB.POSX2,a1)
		cmp.w	d4,d3			X arrivé > X départ ?
		beq.s	.oks
		bhi.s	.sup
		move.w	#-1,(BOB.POSX2,a1)
		bra.s	.oks
.sup		move.w	#1,(BOB.POSX2,a1)
.oks		add.w	d5,d4
		add.w	#16,d3
		lea	(BT.SIZE,a1),a1
		dbf	d6,.lo1
		moveq	#0,d7
.exit		rts	
