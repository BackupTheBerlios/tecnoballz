*=========================================================================*
*               Recopie les n° area & level > table joueur		  *
*=========================================================================*
* entrée	a5.l	=> adr. table 't'
* utilise	d1,d2 / a0,a1,a5
super_famicom:
		moveq	#0,d1
		lea	(liste_joueur-t,a5),a0
.konami		addq.w	#1,d1
		move.l	(a0,d1*4),a1		lit adr. liste joueur
		tst.b	(J.LIFE,a1)		unité, nbre de vie à zéro ?
		bne.s	.sandy2			-> non, joueur est encore vivant
		tst.b	(1+J.LIFE,a1)		décimal, nbre de vie à zéro ?
		bne.s	.sandy2			-> non, joueur est encore vivant
.nintendo:	cmp.w	#NJOUEUR,d1		dernier joueur ?
		bne.s	.konami			-> non
		rts
.sandy2		move.b	(1+area-t,a5),(J.AREA,a1)	n° area de 1 à 5
		move.b	(1+lev1-t,a5),(0+J.LEV,a1)	dizaine n° du level
		move.b	(1+lev0-t,a5),(1+J.LEV,a1)	unité n° du level
		bra.s	.nintendo
