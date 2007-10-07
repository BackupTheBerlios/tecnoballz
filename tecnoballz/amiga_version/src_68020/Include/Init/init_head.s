***************************************************************************
*------------------- Initialise l'affichage de la tête -------------------*
***************************************************************************
* Entrée	=> a5.l	adr. 't'
init_head:
		*/ Décompacte */
		sub.l	a2,a2
		lea	(heads.pp-t,a5),a3
		lea	(heads.pagegfx-t,a5),a4
		bsr	decode.pagepp
		bne	.exit

		*/ Adresse page tête */
		move.l	(heads.pagegfx-t,a5),a4
		move.l	(pt_screen-t,a5),a0
		move.l	a4,(head.adr-t,a5)
		add.l	#PG_DATA,(head.adr-t,a5)

		*/ Modulo */
		move.w	(PG_SPLAN,a4),d0
		subq.w	#4,d0
		swap	d0
		move.w	(SP_SPLAN,a0),d0
		subq.w	#4,d0
		move.l	d0,(head.mod-t,a5)

		*/ Fenêtre Blitter */
		move.w	(PG_HEIGHT,a4),d0
		mulu	(SPPLAN,a0),d0
		lsl.w	#6,d0
		addq.w	#2,d0
		move.w	d0,(head.win-t,a5)

		*/ ADRESSE ECRAN RELATIVE D'AFFICHAGE *
		move.w	#106,d0
		mulu	(SP_SLINE,a0),d0
		add.w	#34,d0
		move.l	d0,(head.ecr-t,a5)
		moveq	#0,d7
.exit		rts
