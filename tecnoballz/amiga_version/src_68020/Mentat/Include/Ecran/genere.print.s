*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
*==----------------> Génère une routine "print" 680x0 <-----------------==*
*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
* Entrée	=> d2.w	Nombre de plan(s) - 1
*		=> d3.w Taille ligne page fonte
*		=> d4.w Largeur page fonte
*		=> d5.w Taille ligne écran
*		=> d6.w Largeur page écran
*		=> a4.l Structure 'RESA'
*		=> a5.l	Adr. 't'
* Sortie	<= d7.l=0 / Z=1 Pas d'erreur
genere.print:
		movem.l	d0-d6/a0,-(a7)
		move.w	d2,d0
		addq.w	#1,d0			d0=nombre de plan(s)
		lsl.w	#3,d0			*8 (hauteur d'une fonte)
		subq.w	#1,d0
		mulu	#6,d0			6 octets par instructions
		addq.l	#4,d0			+ MOVE.B (a1),(a2) & RTS
		move.l	d0,(4,a4)
		bsr	reserve.mem
		bne	.exit
		move.w	d2,d7
		move.w	d4,d1			d1=Largeur page fonte
		swap	d1
		move.w	d6,d1 			d1=Largeur page écran
		move.w	d3,d2 			d2=Taille ligne page fonte
		swap	d2
		move.w	d5,d2 			d2=Taille ligne écran
		move.l	(a4),a0			a0=pt/routine
		move.w	#$1491,(a0)+		code "MOVE.B (A1),(A2)"
		move.w	d7,d4			d4=nombre de plan(s)
		moveq	#0,d3
		move.l	d3,d0
		moveq	#8-1,d5
		bra.s	.next
.xloop		move.l	d3,d0
		move.w	d7,d4			d4=nombre de plan(s)
.loop		move.w	#$1569,(a0)+		code "MOVE.B (X,A1),(X,A2)"
		move.l	d0,(a0)+
.next		add.l	d1,d0
		dbf	d4,.loop
		add.l	d2,d3
		dbf	d5,.xloop
		move.w	#$4E75,(a0)		code "RTS"
		moveq	#0,d7
.exit		movem.l	(a7)+,d0-d6/a0
		tst.l	d7
		rts
