*=========================================================================*
*                Copie le nbre de vies dans listes joueurs		  *
*=========================================================================*
* entrée	=> a5.l	adr. table 't'
* utilise	d0,d1 / a0,a1,a5
playerlive:	
		lea	(liste_joueur-t,a5),a0
		moveq	#0,d0
.playerlive2	addq.w	#1,d0
		move.l	(a0,d0*4),a1		Adr. liste joueur
		move.b	(1+love1-t,a5),(1+J.LIFE,a1)
		move.b	(1+love-t,a5),(0+J.LIFE,a1)
		cmp.l	#$0413011A,(J.NAME,a1)
		beq.s	.etb
		cmp.l	#$0309081A,(J.NAME,a1)
		beq.s	.dji
		cmp.l	#$090C0C1A,(J.NAME,a1)
		beq.s	.jmm
		cmp.l	#$1919011A,(J.NAME,a1)
		beq.s	.zbb
		cmp.l	#$1104061A,(J.NAME,a1)
		beq.s	.reg
.cont		cmp.w	(player.num-t,a5),d0	Dernier joueur ?
		bne.s	.playerlive2		-> non
		rts
.etb		bsr.s	.lune1
.dji		bsr.s	.lune1
.jmm		bsr.s	.lune1
.zbb		bsr.s	.lune1
.reg		bsr.s	.lune1
		bra.s	.cont
.lune1		cmp.b	#9,(0+J.LIFE,a1)
		beq.s	.soph3
		addq.b	#1,(0+J.LIFE,a1)
		rts
.soph3		cmp.b	#9,(1+J.LIFE,a1)		limite à 99 vies
		beq.s	.sopha3
		clr.b	(0+J.LIFE,a1)
		addq.b	#1,(1+J.LIFE,a1)
.sopha3		rts

