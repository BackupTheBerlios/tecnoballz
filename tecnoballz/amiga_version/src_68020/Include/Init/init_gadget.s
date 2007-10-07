*-------------------------------------------------------------------------*
*----------------* Initialise les gadgets bonus/malus *-------------------*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d7.l	0=pas d'erreur
init_gadget:
		*/ Réserve les structures */
		lea	(pt_gadget-t,a5),a4
		move.l	#GAD.SIZE*NGADGET,(RESA.SIZ,a4)
		bsr.l	reserve.mem

		*/ Init. les structures */
		move.l	(pt_gadget-t,a5),a1
		moveq	#NGADGET-1,d6
.loopinit	moveq	#BOB.GADGET,d0
		bsr	bob32.only
		bne	.exit
		bsr	shado.ajoute
		bne	.exit
		move.w	#1,(GAD.TEMPO,a1)
		clr.l	(BOB.ACTIF,a1)
		move.w	#128,(GAD.X,a1)
		lea	(b1-t,a5),a0
		move.l	a0,(GAD.RAK,a1)
		lea	(GAD.SIZE,a1),a1
		dbf	d6,.loopinit
		bsr	double_action4
		moveq	#0,d7
.exit		rts

*-------------------------------------------------------------------------*
************************** Allocations des bonus **************************
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
double_action4:
		move.w	(brick.count-t,a5),d2	nombre de briques
		moveq	#0,d0
		move.w	d0,(pt.course-t,a5)
		move.w	d0,(bonus.tombe-t,a5)	RAZ nbre de bonus tombes
		move.w	d2,d0			nombre de briques à casser
		lsr.w	#1,d0			/2
		move.w	d0,d1
		move.w	d2,d0
		sub.w	d1,d0
		move.w	(bonus.achete-t,a5),d1	nombre de bonus à distribuer
		tst.w	d1
		bne.s	.alloc_plus
		move.w	d2,d1
		addq.w	#1,d1
		move.w	d1,(bonus.step-t,a5)
		bra.s	.alloc_fin
.alloc_plus	addq.w	#1,d1
		divu	d1,d0
		move.w	d0,(bonus.step-t,a5)	Fréquence laché de bonus
.alloc_fin	rts
