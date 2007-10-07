***************************************************************************
***************** Convertit un nombre DCB en chaine ascii *****************
***************************************************************************
* Entrée	=> d1.w	Nombre de chiffre - 1
*		=> d3.l	Nombre DCB
*		=> a0.l	Chaine ascii
* Modifie	d0,d1,d2,d3,d4 / a0
conv_number_ascii:
		and.w	#$0007,d1		8 chiffres maximums
		moveq	#$000F,d2
		move.w	#'0',d4
.loop		move.l	d3,d0
		lsr.l	d1,d0
		lsr.l	d1,d0
		lsr.l	d1,d0
		lsr.l	d1,d0
		and.l	d2,d0
		add.w	d4,d0
		cmp.w	#'9',d0
		bls.s	.ok
		add.w	#'A'-'9'-1,d0
.ok		move.b	d0,(a0)+
		dbf	d1,.loop
		rts
