*-------------------------------------------------------------------------*
********************> Initialise la table "sc_mapadr" <********************
*-------------------------------------------------------------------------*
* Entrée	=> a5.l adr. 't'
* Sortie	<= d7.l	#0 = pas d'erreur
init.sc_mapadr:
		moveq	#0,d0
		move.w	d0,(nu_video-t,a5)
		move.w	d0,(afline-t,a5)
		move.w	d0,(sc_y1-t,a5)
		move.w	d0,(ffecran-t,a5)
		move.w	d0,(flag_co-t,a5)

		*/ Calcul le nombre total de dalles */
		move.l	(dalma.pagegfx-t,a5),a4
		move.w	(PG_HEIGHT,a4),d0
		lsr.w	#4,d0			dalle de 16 lignes de haut
		move.w	(PG_SPLAN,a4),d1
		lsr.w	#1,d1			dalle de 2 octets de large
		mulu	d1,d0			d0=nombre de dalles
		move.w	d0,(gd.total-t,a5)

		*/ Reserve mémoire table "sc_mapadr" */
		lea	(sc_mapadr-t,a5),a4
		lsl.l	#2,d0
		move.l	d0,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		bne.s	.exit

		*/ Initialise la table des pointeurs de dalles */
		move.l	(dalma.pagegfx-t,a5),a4
		move.l	(sc_mapadr-t,a5),a0
		move.w	(PG_HEIGHT,a4),d0
		lsr.w	#4,d0
		subq.w	#1,d0
		move.w	(PG_SPLAN,a4),d2
		lsr.w	#1,d2
		subq.w	#1,d2
		moveq	#0,d3
		moveq	#0,d4
		move.w	(PG_SLINE,a4),d4
		lsl.l	#4,d4
		sub.w	(PG_SPLAN,a4),d4
.sc_init.loop1	move.w	d2,d1			nombre de dalle par ligne - 1
.sc_init.loop2	move.l	d3,(a0)+
		addq.l	#2,d3
		dbf	d1,.sc_init.loop2
		add.l	d4,d3
		dbf	d0,.sc_init.loop1
		moveq	#0,d7
.exit		rts

