*=========================================================================*
*		Incr�mente le n� du level				  *
*=========================================================================*
* entr�e	=> a5.l	adr. table 't'
level_plus:
		cmp.w	#9,(lev0-t,a5)		unit� maxi ?
		blt.s	.lup1			-> non
		addq.w	#1,(lev1-t,a5)		inc. dizaine
		move.w	#-1,(lev0-t,a5)
.lup1		addq.w	#1,(lev0-t,a5)		inc. unit�
		addq.w	#1,(level-t,a5)		inc. n� du tableau
		rts
