*=========================================================================*
*======================* SYSTEME : RESERVE MEMOIRE *======================*
*=========================================================================*
* Entr�e	=> d0.l	taille en octet
*		=> d1.l	caracteristique	#4  Fast
*					#2  Chip
*					#0  Fast ou Chip
*					#-1 Absolue
*		=> a1.l	adr. absolue (si d1 = -1)
*		=> a5.l	adr. table 't'
* Sortie	<= d0.l	Adresse m�moire demand�e ou #0 si erreur
reserve_memoire:
		movem.l	d1-d7/a0-a6,-(a7)
		add.l	#8*4,d0		��	4 longs mots en + avant & apr�s
		move.l	(mem_liste.adr-t,a5),a0
		cmp.l	#NReservation,(a0)
		beq	.error			-> buffer adresse plein
		move.l	d0,d2			sauve taille dans d2
		tst.l	d1			r�serve adresse absolue ?
		bmi.s	.absolue		-> oui

		* r�servation adresse ind�termin�e *
		movem.l	d1-d7/a0-a6,-(a7)
		move.l	4.w,a6
		jsr	(AllocMem,a6)
		movem.l	(a7)+,d1-d7/a0-a6
		tst.l	d0			reservation �choue ?
		beq	.exit			-> oui, erreur
		bra.s	.ok

		* r�servation adresse absolue *
.absolue	movem.l	d1-d7/a0-a6,-(a7)
		move.l	4.w,a6
		moveq	#0,d1
		sub.l	#4*4,a1		��
		jsr	(AllocAbs,a6)
		movem.l	(a7)+,d1-d7/a0-a6
		tst.l	d0			reservation �choue ?
		beq	.exit			-> oui, erreur

		* sauve adresse + taille *
		* d0 => adresse m�moire
.ok		move.l	(a0),d3			d3=nbre d'adr. dans table
		lsl.l	#4,d3			*16 (4 longs mots)		
		addq.l	#1,(a0)			compteur nombre adresse + 1
		move.l	(mem_liste.adr-t,a5),a0
		addq.l	#4,a0			saute compteur adresse
		add.l	d3,a0
		move.l	d0,(a0)+		sauve adresse
		move.l	d2,(a0)			sauve taille en octet
		move.l	d0,a0
		move.l	#'MENT',(a0)+
		move.l	#'AT*M',(a0)+
		move.l	#'EM*S',(a0)+
		move.l	#'TART',(a0)+
		sub.l	#8*4,d2
		move.l	d2,d0			d0=taille m�moire
		bsr	clear.memory		|-> RAZ m�moire
		move.l	a0,d0
		add.l	d2,a0

		* sort de la routine *
.zend:		move.l	#'STOP',(a0)+
		move.l	#'*MEN',(a0)+
		move.l	#'TAT*',(a0)+
		move.l	#'MEM*',(a0)+
.exit		movem.l	(a7)+,d1-d7/a0-a6
		rts

		* erreur d'allocation m�moire *
.error:		moveq	#0,d0
		bra.s	.exit

*----------------* SYSTEME : Structure table 'mem_liste' *----------------*
*******	dc.l	0		nbre de r�servation effectu�e		***
*******	ds.l	NReservation*4	buffer : adresse m�moire, taille octets ***
*******				       : pointeur , identificateur
				* 0,0


