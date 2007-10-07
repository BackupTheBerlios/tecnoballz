*»									 «*
************************ Réserve de la mémoire chip ***********************
*»									 «*
* Entrée	=> a0.l pt/adresse de la chip
*		=> d0.l taille de la liste en octets
* Sortie	<= d7.l=0 / Z=1 : pas d'erreur
reserve_chip:
		movem.l	d0/d1/d2/a0,-(a7)
		move.l	d0,d2			d2=taille en octets
		tst.l	(a0)			mémoire déjà réservée ?
		bne.s	.deja			-> oui
		moveq	#MEMF_CHIP,d1		mémoire CHIP
		bsr	reserve_memoire
		move.l	d0,(a0)			sauve adresse mémoire
		beq.s	.outmem			-> dépassement mémoire
		bra.s	.ok
.deja		move.l	(a0),a0
		move.l	d2,d0
		bsr	clear.memory		|-> RAZ mémoire
.ok		moveq	#0,d7
.exit		movem.l	(a7)+,d0/d1/d2/a0
		tst.l	d7
		rts
.outmem		moveq	#-12,d7			¤Dépassement mémoire
		bra.s	.exit
