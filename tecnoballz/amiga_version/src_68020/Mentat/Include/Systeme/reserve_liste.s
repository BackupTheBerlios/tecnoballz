*�									 �*
************************ R�serve une liste m�moire ************************
*�									 �*
* Entr�e	=> a0.l pt/adresse de la liste
*		=> d0.l taille de la liste en octets
* Sortie	<= d7.l=0 / Z=1 : pas d'erreur
reserve_liste:
		movem.l	d0/d1/d2/a0,-(a7)
		move.l	d0,d2			d2=taille en octets
		tst.l	(a0)			m�moire d�j� r�serv�e
		bne.s	.deja			-> oui
		moveq	#0,d1			m�moire FAST ou CHIP
		bsr	reserve_memoire
		move.l	d0,(a0)			sauve adresse m�moire
		beq.s	.outmem			-> d�passement m�moire
		bra.s	.ok
.deja		move.l	(a0),a0
		move.l	d2,d0
		bsr	clear.memory
.ok		moveq	#0,d7
.exit		movem.l	(a7)+,d0/d1/d2/a0
		tst.l	d7
		rts
.outmem		moveq	#-12,d7			�D�passement m�moire
		bra.s	.exit
