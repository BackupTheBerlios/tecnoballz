*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
********************>> Retourne pointeur sur le BOB <<*********************
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
* Entrée	=> d0.w	numéro du bob de 0 à n (de un en un)
*		=> a5.l	adr. 't'
* Sortie	<= a0.l	pointeur sur le bob
*		<= d7.l Pas d'erreur
bob32_get.bob:
		move.l	(bob32_liste-t,a5),a0
		addq.l	#4,a0			saute id
		and.l	#$FFFF,d0
		cmp.l	(a0)+,d0		numéro de bob existe ?
		bhi.s	.no_bob			-> non
		move.l	(a0,d0*4),a0
		moveq	#0,d7
.exit		rts
.no_bob:	move.l	(a0),a0
		moveq	#-33,d7			¤ N° de bob inexistant
		rts
