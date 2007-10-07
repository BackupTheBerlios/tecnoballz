*/\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//*
*---------------------------*INITIALISE PRINT*----------------------------*
*/\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//*
* Entrée	=> a5.l	Adr. 't'
init_print:
		*/ Génére une routine PRINT 4 couleur */
		move.l	(pt_screen-t,a5),a0
		move.l	(fonte4.pagegfx-t,a5),a4
		move.w	(PG_NPLAN,a4),d2
		subq.w	#1,d2
		move.w	(PG_SLINE,a4),d3
		move.w	(PG_SPLAN,a4),d4
		move.w	(SP_SLINE,a0),d5
		move.w	(SP_SPLAN,a0),d6
		lea	(hedgehog-t,a5),a4
		bsr	genere.print
		bne.s	.exit

		*/ Calcul position d'affichage */
		move.l	(pt_screen-t,a5),a0
		move.w	#76,d0
		mulu	(SP_SLINE,a0),d0
		add.w	#33,d0
		move.l	d0,(pos.bricks-t,a5)

		*/ Calcul position d'affichage */
		move.l	(pt_screen-t,a5),a0
		move.w	#58,d0
		mulu	(SP_SLINE,a0),d0
		add.w	#33,d0
		move.l	d0,(pos.score-t,a5)

		*/ Calcul position d'affichage */
		move.l	(pt_screen-t,a5),a0
		move.w	#76,d0
		mulu	(SP_SLINE,a0),d0
		add.w	#37,d0
		move.l	d0,(pos.lifes-t,a5)
		moveq	#0,d7
.exit		rts
