*--------------------------------------------------------------------------*
**»»»»»»»»»»»»»» Donne le nombre puissance de 2 supérieur «««««««««««««««**
*--------------------------------------------------------------------------*
* Entrée	=> d0.l Nombre
* Sortie	<= d0.l Nombre puissance de 2 supérieur 
*		        ou égal si déjà puissance de 2
binarise:
		movem.l	d1/d2,-(a7)
		moveq	#31,d1		commence par le bit 31
.lp		btst	d1,d0		bit à 1 ?
		bne.s	.nb		-> oui
		dbf	d1,.lp
		bra.s	.ok
.nb		subq.w	#1,d1
		bmi.s	.ex
		move.w	d1,d2
.tt		btst	d2,d0		bit à 1 ?
		bne.s	.sb		-> oui
		dbf	d2,.tt
		bra.s	.ex		-> nombre déjà une puissance de 2
.sb		bset	d1,d0
		dbf	d1,.sb
.ok		addq.l	#1,d0
.ex		movem.l	(a7)+,d1/d2
		rts
