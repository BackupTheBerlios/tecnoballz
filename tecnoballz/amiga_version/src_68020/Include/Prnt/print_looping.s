*-------------------------------------------------------------------------*
*-----------------* AFFICHE UNE CHAINES DE CARACTERES *-------------------*
*-------------------------------------------------------------------------*
* Entrée	=> d0.w	Pos. X en octets
*		=> d1.w Pos. Y en lignes
*		=> d2.w Nombre de caractère(s) - 1
*		=> a0.l pt/texte
* Modifie	d0/d1/d2/a0/a1/a2/a3/a4
print_looping:
		move.l	(pt_screen-t,a5),a3
		mulu	(SP_SLINE,a3),d1
		move.l	(SPECRAN1,a3),a2
		add.l	d1,a2
		add.w	d0,a2
		move.l	(fonte4.pagegfx-t,a5),a4
		lea	(PG_DATA,a4),a4
		move.l	(hedgehog-t,a5),a3
.loop		move.w	(a0),d0
		lea	(27,a4,d0),a1
		jsr	(a3)
		subq.w	#2,a0
		addq.l	#1,a2
		dbf	d2,.loop
		rts
