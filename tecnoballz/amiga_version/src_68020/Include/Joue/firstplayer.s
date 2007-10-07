*-------------------------------------------------------------------------*
*«»«»«»«»«»«»«»«»«»«»«»«» Passe au premier joueur «»«»«»«»«»«»«»«»«»«»«»«»*
*-------------------------------------------------------------------------*
* sortie	<= d7	-1 tous les joueurs sont mort		
firstplayer:	
		bsr	pointeplayer
		bsr	xcopy			save last player
firstfirst:	move.w	#1,(player-t,a5)	joueur n°1
.firstplayer2:	bsr	pointeplayer
		tst.b	(0+J.LIFE,a0)		joueur mort ?
		bne.s	.gofirstplayer
		tst.b	(1+J.LIFE,a0)
		bne.s	.gofirstplayer
		move.w	(player.num-t,a5),d0
		cmp.w	(player-t,a5),d0
		beq.s	.gogameover
		addq.w	#1,(player-t,a5)
		bra.s	.firstplayer2
.gofirstplayer	bsr	marauder
		moveq	#0,d7
		rts
.gogameover	moveq	#-1,d7
		rts
