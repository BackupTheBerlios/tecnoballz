*-------------------------------------------------------------------------*
*-------------------* Initialise les capsules monnaie *-------------------*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d7.l	0=pas d'erreur
init_capsule:
		*/ Réserve les Structures */
		lea	(pt_capsule-t,a5),a4
		move.l	#CAP.SIZE*NMONNAIE,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne	xssexit

		*/ Init. les structures */
		move.l	(pt_capsule-t,a5),a1
		moveq	#NMONNAIE-1,d6
.loopinit	moveq	#BOB.MONNAI,d0
		bsr	bob32.only
		bne	xssexit
		bsr	shado.ajoute
		bne	xssexit
		move.w	#3,(BOB.INITTEMPO,a1)
		clr.l	(BOB.ACTIF,a1)
		move.w	#128,(CAP.X,a1)
		lea	(b1-t,a5),a0
		move.l	a0,(CAP.RAK,a1)
		lea	(CAP.SIZE,a1),a1
		dbf	d6,.loopinit

		*/ Convertit monnaie -> Sprite */
		moveq	#BOB.MONNAI,d0
		lea	(sscap-t,a5),a3
supersprite	move.w	#1,(SS.TEMPO,a3)
		move.w	#5,(SS.INIT,a3)
		bsr	bob32_get.bob
		moveq	#0,d0
		move.w	(TBOB_WIDTH,a0),d0	d0=Largeur en mots
		add.w	d0,d0			d0=Largeur en octets
		mulu	(TBOB_HEIGHT,a0),d0	d0=Taille d'un plan
		add.w	d0,d0			d0=Taille d'un lutin
		move.l	d0,(SS.TAILLE,a3)
		add.w	d0,d0
		move.l	d0,(SS.TAILLE2,a3)
		mulu	(TBOB_NUMBER,a0),d0
		lea	(SS.ADR,a3),a4
		move.l	d0,(RESA.SIZ,a4)
		move.l	#'SPRI',(RESA.IDE,a4)
		move.l	#MEMF_CHIP,(RESA.TYP,a4)
		bsr.l	reserve.mem
		bne.s	xssexit
		move.w	(TBOB_HEIGHT,a0),d0
		subq.w	#1,d0
		move.w	d0,(SS.HEIGHT,a3)
		move.w	(TBOB_NUMBER,a0),d6
		subq.w	#1,d6
		move.l	(bob32_pagegfx-t,a5),a4
		move.w	(PG_SPLAN,a4),d1
		move.w	(PG_SLINE,a4),d2
		lea	(TBOB_TABLE,a0),a4
		move.l	(SS.ADR,a3),a0
		move.l	(SS.TAILLE,a3),d3
		lea	(a0,d3),a1
.loop		move.l	(a4),a2
		bsr.l	copylutin16.0
		add.l	d3,a0
		add.l	d3,a1
		addq.l	#8,a4
		dbf	d6,.loop
		moveq	#0,d7
xssexit		rts
