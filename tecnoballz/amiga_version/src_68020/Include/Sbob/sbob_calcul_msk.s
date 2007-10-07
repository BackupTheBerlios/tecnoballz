***************************************************************************
************ RESERVE LA MEMOIRE & CALCUL LES MASQUES DES SBOBS ************
***************************************************************************
* Entrée	=> a5.l	adresse 't'
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
sbob_calcul_msk:
		* réserve mémoire pour les masques *
		tst.l	(sbob_pagegfx-t,a5)
		beq	.no_page		-> pas de page SBOB
		tst.l	(sbob_pagemsk-t,a5)
		bne	.exit			-> page masque dèjà calculée
		move.l	(sbob_pagegfx-t,a5),a3
		lea	(sbob_pagemsk-t,a5),a4
		move.l	(PG_SIZE,a3),(RESA.SIZ,a4)
		add.l	#PG_DATA,(RESA.SIZ,a4)
		move.l	#MEMF_CHIP,(RESA.TYP,a4)
		bsr	reserve.mem
		bne.s	.error

		* init registre boucle *
		move.l	a3,a0			a0=adr. source (gfx)
		lea	(sbob_pagemsk-t,a5),a4
		move.l	(RESA.ADR,a4),a1	a1=adr. destination (msk)
		moveq	#PG_DATA-1,d0
.gobot		move.b	(a0)+,(a1)+
		dbf	d0,.gobot
		moveq	#0,d3
		move.w	(PG_SPLAN,a3),d3	d3=largeur en octets de la page
		move.w	d3,d4
		lsr.w	#1,d4			convertit mots en octets
		subq.w	#1,d4	
		move.w	(PG_HEIGHT,a3),d5	d5=nbre de lignes
		subq.w	#1,d5
		moveq	#0,d6
		move.w	(PG_SLINE,a3),d6	d6=taille d'un plan
		move.w	(PG_NPLAN,a3),d7	d7=nbre de plans
		subq.w	#1,d7

		* boucle *
		movem.l	a5/a6,-(a7)
.loop_line	move.l	a0,a2			a2=source (gfx)
		move.l	a1,a3			a3=destination (msk)
		move.w	d4,d2			d2=nbre de mots par lignes
.loop_word	move.l	a2,a4			a4=source (gfx)
		move.l	a3,a5			a5=destination (msk)
		move.w	d7,d1			d1=nbre de plan-1
		moveq	#0,d0
.loop_read	or.w	(a4),d0			"OR" avec les tous les plans
		add.l	d3,a4			PLAN++
		dbf	d1,.loop_read
		move.w	d7,d1			d1=nbre de plan-1
.loop_write	move.w	d0,(a5)			Ecrit le msk pour chaque plans
		add.l	d3,a5			PLAN++
		dbf	d1,.loop_write
		addq.l	#2,a2
		addq.l	#2,a3
		dbf	d2,.loop_word
		add.l	d6,a0
		add.l	d6,a1
		dbf	d5,.loop_line
		movem.l	(a7)+,a5/a6
.exit		moveq	#0,d7
.error		rts
.no_page	moveq	#-30,d7			Pas de "sbob_pagegfx"
		rts
