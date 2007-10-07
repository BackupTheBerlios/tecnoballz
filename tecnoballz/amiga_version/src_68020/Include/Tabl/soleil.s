;=========================================================================*
;                 Décrèmente le nbre de brique à détruire		  ;
;=========================================================================*
; entrée	=> a5.l	adr. table 't'
soleil:		
		tst.w	(brick0-t,a5)
		beq.s	.soph0
		subq.w	#1,(brick0-t,a5)
		rts
.soph0		move.w	#9,(brick0-t,a5)
		tst.w	(brick1-t,a5)
		beq.s	.soph1
		subq.w	#1,(brick1-t,a5)
		rts
.soph1		move.w	#9,(brick1-t,a5)
		subq.w	#1,(brick2-t,a5)
		rts
