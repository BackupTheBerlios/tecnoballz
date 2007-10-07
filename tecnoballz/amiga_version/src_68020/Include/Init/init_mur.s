**>--<**>--<**>--<**>--<**>--<**>--<**>--<**>--<**>--<**>--<**>--<**>--<**>
*>>->>->>->>->>->>->>- INITIALISE LE MUR DU BAS -<<-<<-<<-<<-<<-<<-<<-<<-<*
**>--<**>--<**>--<**>--<**>--<**>--<**>--<**>--<**>--<**>--<**>--<**>--<**>
init_mur:
		*/ Affiche le mur en BOB */
		clr.w	(mur_on-t,a5)
		clr.w	(mur_off-t,a5)
		clr.w	(mur_du_bas-t,a5)
		clr.w	(flag_flip-t,a5)
		moveq	#BOB.MUR,d0
		moveq	#0,d1
		move.w	#MUR_POSY,d2
		moveq	#0,d3
		moveq	#0,d4
		bsr.l	bob32.ajoute_bob
		bne.s	.exit
		bsr.l	shado.ajoute
		bne.s	.exit
		move.w	#1,(BOB.ACTIF,a1)
		move.w	#MUR_POSY,(BOB.POSY,a1)
		move.w	#32,(BOB.POSX,a1)
		lea	(.bob,pc),a0
		move.l	a1,(a0)
		*/ Recopie le mur OFF */
		lea	(mur_restore-t,a5),a4
		bsr	init_xxx
		bne.s	.exit
		*/ Recopie le mur ON */
		bsr.l	shado_affiche
		bsr.l	bob32_affiche
		lea	(mur_affiche-t,a5),a4
		bsr	init_xxx
		bne.s	.exit
		move.l	(.bob,pc),a1
		clr.l	(BOB.ACTIF,a1)
		move.w	#3,(mur_off-t,a5)
		moveq	#0,d7
.exit		rts
		CNOP	0,4
.bob		dc.l	0

*********************** Initialise & copie Blitter ************************
* Entre	=> a4.l	pt/structure RESA
init_xxx:
		*/ Calcul fenêtre Blitter */
		move.l	(pt_screen-t,a5),a1
		moveq	#BOB.MUR,d0
		bsr.l	bob32_get.bob
		bne	.exit
		move.w	(TBOB_HEIGHT,a0),d6	d6=hauteur du BOB en lignes
		add.w	#ODECA,d6
		mulu	(SPPLAN,a1),d6
		move.w	d6,d4
		lsl.w	#6,d6
		move.w	(TBOB_WIDTH,a0),d0	d0=largeur BOB en mots
		add.w	d0,d6			d6=fenêtre blitter
		move.w	d6,(mur_window-t,a5)
		*/ Calcul modulos Blitter */
		add.w	d0,d0
		mulu	d0,d4			d4=taille du Buffer
		moveq	#0,d5
		move.w	(SP_SPLAN,a1),d5
		sub.w	d0,d5
		move.w	d5,(2+mur_modulos-t,a5)
		swap	d5
		*/ Calcul source & destination */
		move.w	#MUR_POSY,d0
		mulu	(SP_SLINE,a1),d0
		addq.l	#4,d0
		move.l	d0,(mur_adresse-t,a5)
		add.l	(SPECRAN1,a1),d0
		*/ Reserve le Buffer */
		move.l	d4,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		bne.s	.exit
		move.l	(RESA.ADR,a4),d1
		*/ Initialise le Blitter */
.wait		btst	#14,(DMACONR,a6)
		bne.s	.wait
		move.l	d5,(BLTAMOD,a6)		
		move.l	#$09F00000,(BLTCON0,a6)
		move.l	#-1,(BLTAFWM,a6)
		move.l	d0,(BLTAPTH,a6)
		move.l	d1,(BLTDPTH,a6)
		move.w	d6,(BLTSIZE,a6)
		moveq	#0,d7
.exit		rts
