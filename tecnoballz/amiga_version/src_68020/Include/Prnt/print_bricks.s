*-------------------------------------------------------------------------*
*****************  Affiche le nombre de briques restant *******************
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
print_bricks:
		move.w	(brick.n-t,a5),d0
		tst.w	(flag_flip-t,a5)
		bne.s	.falentin2
		addq.w	#1,d0
		cmp.w	#3,d0
		bne.s	.falentin1
		moveq	#0,d0
.falentin1	move.w	d0,(brick.n-t,a5)
.falentin2	move.l	(ecran_travail-t,a5),a2
		add.l	(pos.bricks-t,a5),a2
		add.w	d0,a2
		lea	(brick2-t,a5),a0	pt/100aine
		add.w	d0,d0
		sub.w	d0,a0
		move.w	(a0),d0
		move.l	(fonte4.pagegfx-t,a5),a1
		lea	(PG_DATA,a1),a1
		lea	(27,a1,d0),a1
		move.l	(hedgehog-t,a5),a0
		jmp	(a0)
