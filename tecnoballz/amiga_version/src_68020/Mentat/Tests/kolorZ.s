

s:		lea	s1,a0
		lea	s2,a1
		moveq	#59,d6
		moveq	#-1,d7


		*/ LIT LA PALETTE */
.l1		move.l	(a0)+,d1
		move.l	(a0)+,d2
		move.l	(a0)+,d3
		move.l	(a0)+,d4

		*/ COMPARE LA PALETTE */
		move.w	d7,d0
		bmi.s	.k
		lea	s2,a2
		lea	t1,a4
.l2		move.l	a2,a3
		cmp.l	(a3)+,d1
		bne.s	.n2
		cmp.l	(a3)+,d2
		bne.s	.n2
		cmp.l	(a3)+,d3
		bne.s	.n2
		cmp.l	(a3),d4
		beq.s	.nx2
.n2		lea	16(a2),a2
		addq.l	#1,a4
		dbf	d0,.l2

		*/ NOUVELLE PALETTE ON LA SAUVE */
.k		move.l	d1,(a1)+
		move.l	d2,(a1)+
		move.l	d3,(a1)+
		move.l	d4,(a1)+
		addq.w	#1,d7
.nx		dbf	d6,.l1
		rts
.nx2		addq.b	#1,(a4)
		bra.s	.nx

s1	INCBIN	TECNOBALLZ:TECNOBALLZ/GFX/PAL_60MAPS.LIST
s2	ds.b	960
t1	ds.b	60


