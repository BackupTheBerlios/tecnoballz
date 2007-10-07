*--===---===---===---===---===---===---===---===---===---===---===---===--*
*-=-=-=-=-=-=-=-=-=-=-=-=-> Efface zone mémoire <-=-=-=-=-=-=-=-=-=-=-=-=-*
*--===---===---===---===---===---===---===---===---===---===---===---===--*
* Entrée	=> d0.l	taille de la zone mémoire en octets
*		=> a0.l	pointeur sur la zone mémoire
clear.memory:
		tst.l	d0
		beq.s	.exit
		movem.l	d0/d1/d2/a0,-(a7)
		moveq	#0,d2
		move.w	d0,d1
		lsr.l	#4,d0
		subq.l	#1,d0
		bmi.s	.noloop1
.loop1		move.l	d2,(a0)+
		move.l	d2,(a0)+
		move.l	d2,(a0)+
		move.l	d2,(a0)+
		dbf	d0,.loop1
.noloop1	and.w	#$F,d1
		beq.s	.noloop2
		subq.w	#1,d1
.loop2		move.b	d2,(a0)+
		dbf	d1,.loop2
.noloop2	movem.l	(a7)+,d0/d1/d2/a0
.exit		rts
