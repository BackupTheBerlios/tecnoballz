*=========================================================================*
*                        Passe au prochain joueur			  *
*=========================================================================*
* entrée	=> a5.l	adr. table 't'
* sortie	<= d7.l	-1 & Z=0 pas de prochain joueur 
*		<= a0.l	adr. liste joueur
* utilise	d0,d7 / a0,a5
*
nextplayer:
		move.l	(player-t,a5),-(a7)	Sauve n° du joueur
		bsr	pointeplayer
		bsr	xcopy			|-> sauve données table joueur actuel
.nextplayer2	move.w	(player.num-t,a5),d0	nbre de joueur de 1 à 6
		cmp.w	(player-t,a5),d0	Dernier n° de joueur ?
		beq.s	.nonextplayer		-> oui, pas de prochain joueur
		addq.w	#1,(player-t,a5)	inc. n° de joueur
		bsr	pointeplayer		|-> calcul adr. liste joueur
		tst.b	(0+J.LIFE,a0)		unité nul, joueur mort ?
		bne.s	.nextplayer3		-> non
		tst.b	(1+J.LIFE,a0)		dizaine nul, joueur mort ?
		beq.s	.nextplayer2		-> oui
.nextplayer3	bsr	marauder		|-> recopie données table joueur
		addq.l	#4,a7
		moveq	#0,d7
		rts
.nonextplayer	move.l	(a7)+,(player-t,a5)	restitue n° du joueur
		moveq	#-1,d7			flag : pas de prochain joueur
		rts
