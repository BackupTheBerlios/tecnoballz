*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><*
*---------------------> AFFICHE LA BANDE DES SCORES <---------------------*
*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><*
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l CUSTOM
init_score:
		*/ Décompacte la page graphique */
		sub.l	a2,a2
		lea	(score.pp-t,a5),a3
		lea	(score.pagegfx-t,a5),a4
		bsr	decode.pagepp
		bne	.exit

		*/ Init */
		move.l	(pt_screen-t,a5),a0
		move.l	(score.pagegfx-t,a5),a4
		moveq	#0,d1
		move.w	(SP_SPLAN,a0),d1
		sub.w	(PG_SPLAN,a4),d1
		move.w	(PG_HEIGHT,a4),d0
		mulu	(PG_NPLAN,a4),d0
		swap	d0
		move.w	(PG_SPLAN,a4),d0
		lsr.w	#1,d0
		move.l	(SPECRAN1,a0),a0
		lea	(32,a0),a0
		lea	(PG_DATA,a4),a4

		*/ Affiche */
.wait2		btst	#14,(DMACONR,a6)
		bne.s	.wait2
		move.l	#$09F00000,(BLTCON0,a6)
		move.l	#-1,(BLTAFWM,a6)
		move.l	d1,(BLTAMOD,a6)
		move.l	a4,(BLTAPTH,a6)
		move.l	a0,(BLTDPTH,a6)
		move.l	d0,(BLTSIZV,a6)

		*/ Libère la page graphique */
		lea	(score.pagegfx-t,a5),a0
		bsr	libere_mem
.exit		rts
