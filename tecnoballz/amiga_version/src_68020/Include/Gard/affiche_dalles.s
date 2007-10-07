*-------------------------------------------------------------------------*
******************* AFFICHE TOUTES LES DALLES A L'ECRAN *******************
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l	CUSTOM
affiche_dalles:
		move.l	(pt_screen-t,a5),a0
		move.l	(mapeditor-t,a5),a1
		move.l	(sc_mapadr-t,a5),a2
		move.l	(dalma.pagegfx-t,a5),a4
		move.w	(SPHAUTEUR,a0),d0
		lsr.w	#4,d0
		subq.w	#1,d0
		moveq	#0,d1
		move.w	(PG_SPLAN,a4),d1
		move.w	(PG_SLINE,a4),d2
		subq.w	#2,d2
		swap	d2
		move.w	(SP_SLINE,a0),d2
		subq.w	#2,d2
		move.w	(SP_SPLAN,a0),d3
		lsr.w	#1,d3
		subq.w	#1,d3
		move.w	#16*64+1,d4
		moveq	#0,d5
		move.w	(SP_SPLAN,a0),d5
		move.l	d5,d6
		add.w	d6,d6
		subq.w	#2,d6
		moveq	#0,d7
		move.w	(SP_SLINE,a0),d7
		lsl.w	#4,d7
		sub.w	d5,d7
		move.l	(SPECRAN1,a0),a0
		lea	(PG_DATA,a4),a4
		WAITBLITTER
		move.l	#$FFFFFFFF,(BLTAFWM,a6)
		move.l	#$09F00000,(BLTCON0,a6)
		move.l	d2,(BLTAMOD,a6)

.sc_init.loop5	swap	d0
		move.w	d3,d0

		*/ calcul adr. de la dalle à afficher */
.sc_init.loop4	move.l	a4,a3			adr. de la page dalle
		move.w	(a1)+,d2		pt/adr. page dalle
		add.l	(a2,d2.w),a3		adr. de la dalle à afficher

		*/ affiche dalle dans le plan 1 */
		WAITBLITTER
		move.l	a3,(BLTAPTH,a6)
		move.l	a0,(BLTDPTH,a6)
		move.w	d4,(BLTSIZE,a6)

		*/ aff. dalle dans le plan 2 -
		add.l	d1,a3
		add.l	d5,a0
		WAITBLITTER
		move.l	a3,(BLTAPTH,a6)
		move.l	a0,(BLTDPTH,a6)
		move.w	d4,(BLTSIZE,a6)

		*/ affice dalle dans le plan 3 */
		add.l	d1,a3
		add.l	d5,a0
		WAITBLITTER
		move.l	a3,(BLTAPTH,a6)
		move.l	a0,(BLTDPTH,a6)
		move.w	d4,(BLTSIZE,a6)

		*/ calcul adresse prochaine dalle horizontale */
		sub.l	d6,a0
		dbf	d0,.sc_init.loop4

		*/ calcul adresse prochaine ligne */
		add.l	d7,a0
		swap	d0
		dbf	d0,.sc_init.loop5

		*/ calcul pt sur map-editor */
		move.l	(mapeditor-t,a5),a0
		move.l	a0,(sc_map1-t,a5)	adresse première ligne
		lea	(M_LARGE*2*M_HAUTEUR,a0),a0
		move.l	a0,(asc_end-t,a5)	adresse dernière ligne
		rts
