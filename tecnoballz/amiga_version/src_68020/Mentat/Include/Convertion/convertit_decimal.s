***************************************************************************
******************* CONVERTIT UN NOMBRE BINAIRE EN BCD ********************
***************************************************************************
* Entrée	=> d2.l	Chiffre binaire à convertir
* Sortie	<= d3.l	Chiffre BCD
* Modifie	d0,d1,d2,d3,d4 / a0,a1
convertit_decimal:
		moveq	#0,d3
		move.l	#99999999,d0		Nombre maximum
		cmp.l	d2,d0			Dépassement ?
		bhi.s	.pas_depass		-> non
		move.l	d0,d2
.pas_depass	lea	(.chiffre,pc),a0
		lea	(.cont,pc),a1
		moveq	#8-1,d4
.loop		move.l	(a0)+,d0		d0=Chiffre binaire
		move.l	(a0)+,d1		d1=Chiffre DCB		
.cont		cmp.l	d2,d0
		bhi.s	.next
		add.l	d1,d3
		sub.l	d0,d2
		jmp	(a1)
.next		dbf	d4,.loop
		rts
.chiffre:	dc.l	10000000,$10000000,1000000,$1000000,100000,$100000
		dc.l	10000,$10000,1000,$1000,100,$100,10,$10,1,$1
