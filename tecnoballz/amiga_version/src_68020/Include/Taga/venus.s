;==========================================================================;
*                   Routines qui incrémenentent le score		   *
;==========================================================================;
		*-=- + 1 point -=-*
venus:		cmp.w	#9,(score0-t,a5)
		bge.s	.soph4
		addq.w	#1,(score0-t,a5)
		rts
.soph4		move	#0,(score0-t,a5)

		*-=- + 10 points -=-*
venus1:		cmp.w	#9,(score1-t,a5)
		bge.s	.soph5
		addq.w	#1,(score1-t,a5)
		rts
.soph5		move.w	#0,(score1-t,a5)

		*-=- + 100 points -=-*
venus2:		cmp.w	#9,(score2-t,a5)
		bge.s	.soph6
		addq.w	#1,(score2-t,a5)
		rts
.soph6		move.w	#0,(score2-t,a5)

		*-=- + 1000 points -=-*
venus3:		cmp.w	#9,(score3-t,a5)
		bge.s	.soph7
		addq.w	#1,(score3-t,a5)
		rts
.soph7		move.w	#0,(score3-t,a5)

		*-=- + 10000 points -=-*
venus4:		cmp.w	#9,(score4-t,a5)
		bge.s	.soph8
		addq.w	#1,(score4-t,a5)
		rts
.soph8		move.w	#0,(score4-t,a5)

		*-=- + 100000 points -=-*
venus5:		addq.w	#1,(score5-t,a5)
		rts
