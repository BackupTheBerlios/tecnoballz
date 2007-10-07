*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--
;************************>* RAZ LA TABLE LUTIN *<*************************;
*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--
* Entrée	=> a5.l	Adr. 't'
raz_table_lutins:
		movem.l	d0/a0,-(a7)
		lea	(table_lutins-t,a5),a0	a0=Adr. table des 8 adresses
		moveq	#0,d0
		move.l	d0,(a0)+		lutin 0 
		move.l	d0,(a0)+		lutin 1
		move.l	d0,(a0)+		lutin 2
		move.l	d0,(a0)+		lutin 3
		move.l	d0,(a0)+		lutin 4
		move.l	d0,(a0)+		lutin 5
		move.l	d0,(a0)+		lutin 6
		move.l	d0,(a0)			lutin 7
		movem.l	(a7)+,d0/a0
		rts
