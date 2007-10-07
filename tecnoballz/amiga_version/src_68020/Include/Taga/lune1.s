*=========================================================================*
*                       Routine qui ajoute une vie			  *
*=========================================================================*
lune1:		move.b	#1,(sound+S_AJOUTE.VIE-t,a5)
		cmp.w	#9,(life0-t,a5)
		beq.s	.soph3
		addq.w	#1,(life0-t,a5)
		rts
.soph3		cmp.w	#9,(life1-t,a5)		limite à 99 vies
		beq.s	.sopha3
		clr.w	(life0-t,a5)
		addq.w	#1,(life1-t,a5)
.sopha3		rts
