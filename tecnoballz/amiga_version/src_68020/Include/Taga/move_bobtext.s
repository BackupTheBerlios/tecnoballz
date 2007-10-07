*-------------------------------------------------------------------------*
*---------------------*  Déplace les Lettres BOBs   *---------------------*
*-------------------------------------------------------------------------*
move_bobtext:
		move.l	(pt_cos-t,a5),a2
		lea	(PT_SIN,a2),a1

		move.l	(pt_bobtext-t,a5),a4
		moveq	#6,d5
		moveq	#60,d7
		move.w	(tb.l1-t,a5),d6
		bsr.s	.loop1
		moveq	#8,d5
		moveq	#80,d7
		move.w	(tb.l2-t,a5),d6


.loop1		move.w	#1,(a4)

		*/ Déplacement linéaire en Y */
		cmp.w	(BOB.POSY1,a4),d7
		bhi.s	.noy1
		sub.w	d5,(BOB.POSY1,a4)
.noy1		
		*/ Déplacement sinus en Y */
		move.w	(BOB.POSY2,a4),d0
		add.w	#10,d0
		and.w	#1022,d0
		move.w	d0,(BOB.POSY2,a4)
		moveq	#10,d1
		muls	(a1,d0),d1		sin(a)*rayon
		asr.l	#7,d1
		add.w	(BOB.POSY1,a4),d1
		move.w	d1,(BOB.POSY,a4)

		*/ Déplacement en X */
		move.w	(BOB.POSX1,a4),d0	d0=X arrivé
		sub.w	(BOB.POSX,a4),d0
		beq.s	.zozo
		move.w	(BOB.POSX2,a4),d0
		add.w	d0,(BOB.POSX,a4)
.zozo
		lea	(BT.SIZE,a4),a4
		dbf	d6,.loop1
		rts



