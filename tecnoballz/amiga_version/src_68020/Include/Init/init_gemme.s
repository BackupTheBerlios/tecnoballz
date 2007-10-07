*=========================================================================*
*--------------------------Initialise les gemmes--------------------------*
*=========================================================================*
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d7.l 0 pas d'erreur
init_gemme:
		*/ Réserve les structures gemmes */
		lea	(pt_gemme-t,a5),a4
		move.l	#GEM.SIZE*NGEMME,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne	.exit
		*/ Init. les structures */
		move.l	(pt_gemme-t,a5),a1
		moveq	#NGEMME-1,d6
.loopinit	moveq	#BOB.GEMME,d0
		bsr	bob32.only
		bne	.exit
		bsr	shado.ajoute
		bne	.exit
		clr.l	(BOB.ACTIF,a1)
		lea	(GEM.SIZE,a1),a1
		dbf	d6,.loopinit
		*/ Calcul adresse source & destination */
		moveq	#BOB.GEMME,d0
		bsr	bob32_get.bob
		bne	.exit
		lea	(TBOB_TABLE,a0),a1
		move.l	(pt_screen-t,a5),a0
		move.w	(SP_SLINE,a0),d1
		mulu	#172,d1
		add.w	#33,d1		
		lea	(gemme.adr-t,a5),a2
		moveq	#5,d0
.loop		move.l	(a1)+,(a2)+		sauve adr. gfx
		move.l	d1,(a2)+		sauve adr. écran
		addq.l	#1,d1			x=x+8
		addq.l	#4,a1			saute le msk
		dbf	d0,.loop
		*/ Génére une routine PRINT 32 couleurs */
		move.l	(pt_screen-t,a5),a0
		move.l	(bob32_pagegfx-t,a5),a4
		move.w	(PG_NPLAN,a4),d2
		subq.w	#1,d2
		move.w	(PG_SLINE,a4),d3
		move.w	(PG_SPLAN,a4),d4
		move.w	(SP_SLINE,a0),d5
		move.w	(SP_SPLAN,a0),d6
		lea	(gprint-t,a5),a4
		bsr	genere.print
		bne	.exit
		*/ Affichage des Gemmes  */ 
		move.l	(ecran1-t,a5),d6
		move.l	(gprint-t,a5),a3
		lea	(gemme.adr-t,a5),a0
		lea	(gemme_du_j1-t,a5),a4
		moveq	#5,d4
.ggloop		move.l	(a0)+,a1
		move.l	(a0)+,d7
		tst.b	(a4)+
		beq.s	.ggnext
		move.l	d6,a2
		add.l	d7,a2
		jsr	(a3)
.ggnext		dbf	d4,.ggloop

		*/ Clear la liste des bonus à envoyer si atom mort */
		lea	(gemeau-t,a5),a0
		moveq	#NATOM-1,d0
.ka		clr.l	(a0)+
		clr.l	(a0)+
		dbf	d0,.ka
		moveq	#0,d7
.exit		rts


