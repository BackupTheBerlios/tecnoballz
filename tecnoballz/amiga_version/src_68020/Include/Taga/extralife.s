*=========================================================================*
*            Test si le joueur gagne une vie grâce aux points		  *
*=========================================================================*
* entrée	=> a5.l	adr. table 't'
extralife:
		move.w	#LIFE_POINTS,d0
		cmp.w	(score.life-t,a5),d0
		bhi.s	.exit
		sub.w	d0,(score.life-t,a5)
		bsr	lune1
.exit		rts
