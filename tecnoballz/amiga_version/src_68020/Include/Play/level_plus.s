*=========================================================================*
*		Incrémente le n° du level				  *
*=========================================================================*
* entrée	=> a5.l	adr. table 't'
level_plus:
		cmp.w	#9,(lev0-t,a5)		unité maxi ?
		blt.s	.lup1			-> non
		addq.w	#1,(lev1-t,a5)		inc. dizaine
		move.w	#-1,(lev0-t,a5)
.lup1		addq.w	#1,(lev0-t,a5)		inc. unité
		addq.w	#1,(level-t,a5)		inc. n° du tableau
		rts
