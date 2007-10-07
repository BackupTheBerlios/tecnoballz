*=========================================================================*
*                       Collision des deux Gardiens			  *
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
elegy:
		move.l	(gd1-t,a5),a1		Structure Gardien 1
		bsr.s	last_collision
		cmp.w	#1,(gd.numb-t,a5)	Un seul Gardien ?
		beq	last_col_end		-> oui
		move.l	(gd2-t,a5),a1		Structure Gardien 2

*/////////////////////////////////////////////////////////////////////////*
*------------* Routine de collision + Gestion des explosions *------------*
*/////////////////////////////////////////////////////////////////////////*
* Entrée	=> a5.l	adr. table 't'
*		=> a1.l	adr. structure GARD
last_collision:
*------------------------* Envoie une Explosion *-------------------------*
		tst.w	(GARD.POWER,a1)		gardien 1 détruit ?
		bne	.pierre_le_chien	-> non
		tst.w	(GARD.EXPTIME,a1)	tempo = 0 : fin explosions ?
		beq	last_col_end		-> oui, on sort
		subq.w	#1,(GARD.EXPTIME,a1)	tempo = tempo - 1
		move.w	(GARD.EXPTIME,a1),d0	lit tempo
		and.w	#$0003,d0		multiple de 4 ?
		bne	last_col_end		-> non, on sort
		move.l	(explomon-t,a5),a4
		moveq	#NBR_EXPLOMON-1,d0
		move.w	(GARD.X,a1),d6
		move.w	(GARD.Y,a1),d7
		moveq	#0,d4
		lea	(elegy_table-t,a5),a0
		addq.w	#4,(GARD.PTEXP,a1)
		add.w	(GARD.PTEXP,a1),a0
		cmp.w	#99,(a0)		fin de la table ?
		bne.s	.elegy_l1		-> non
		move.w	d4,(GARD.PTEXP,a1)
		lea	(elegy_table-t,a5),a0
.elegy_l1	tst.w	(SBOB.ACTIF,a4)		SBOB déjà actif ?
		bne.s	.elegy_1		-> oui, passe au suivant
		move.w	(a0)+,(SBOB.POSX,a4)	pos. x
		add.w	d6,(SBOB.POSX,a4)	+ pos. x du gardien
		move.w	(a0),(SBOB.POSY,a4)	pos. y relative
		add.w	d7,(SBOB.POSY,a4)	+ pos. y du gardien
		move.w	#1,(SBOB.ACTIF,a4)	active le bob
		move.w	d4,(SBOB.PTANIM,a4)	raz pointeur gfx animation
		move.w	(SBOB.INITTEMPO,a4),(SBOB.TEMPO,a4)
		move.l	(SBOB.TABLE,a4),a2
		move.l	(a2)+,(SBOB.GFX,a4)
		move.l	(a2),(SBOB.MSK,a4)
		subq.w	#1,(gd.explospace-t,a5)
		bne	last_col_end
		move.w	#5,(gd.explospace-t,a5)
		move.b	#1,(sound+S_BIG.EXPLO-t,a5)
		bra	last_col_end
.elegy_1	lea	(SBOB.SIZE,a4),a4
		dbf	d0,.elegy_l1
		bra	last_col_end

*----------------* Gestion collisions balles & Gardien *------------------*
.pierre_le_chien:
		moveq	#0,d7			Flag collision
		move.w	(GARD.X,a1),d4		x gardien
		move.w	d4,d0
		add.w	(GARD.X1,a1),d4		x debut zone sensible
		add.w	(GARD.X2,a1),d0		x fin zone sensible
		move.w	(GARD.Y,a1),d5		y gardien
		move.w	d5,d1
		add.w	(GARD.Y1,a1),d5		y debut zone sensible
		add.w	(GARD.Y2,a1),d1		y fin zone sensible
		move.l	(pt_balle-t,a5),a4	adr. structure raquette
		moveq	#NSBALL-1,d6
.mai		tst.w	(SBALLE.ACTIF,a4)
		beq	.kolax
		move.w	(SBALLE.X,a4),d2
		cmp.w	d0,d2			Bx1>Gx2
		bgt	.kolax			-> pas de col.
		move.w	(SBALLE.Y,a4),d3
		cmp.w	d1,d3			By1>Gy2
		bgt	.kolax			-> pas de col.
		add.w	(SBOB.WIDTH,a4),d2
		cmp.w	d4,d2			Bx2 < Gx1
		blt	.kolax			-> pas de col.
		add.w	(SBOB.WIDTH,a4),d3
		cmp.w	d5,d3			By2 < Gy1 
		blt	.kolax			-> pas de col.
		move.b	#1,(sound+S_GARDIEN.TOUCH-t,a5)
		move.w	(hasard-t,a5),d0
		add.w	d6,d0
		and.w	#$F,d0
		lsl.w	#2,d0
		move.w	d0,(2+SBALLE.DIRECT,a4)
		move.w	#5,(GARD.TOUCH,a1)
		move.w	(SBALLE.POWER,a4),d0
		add.w	d0,d7
		sub.w	d0,(GARD.POWER,a1)
		beq.s	.dead7
		bpl.s	.kolax
.dead7		clr.w	(GARD.POWER,a1)
		move.w	#500,(GARD.EXPTIME,a1)
		clr.w	(GARD.PTEXP,a1)
		move.l	(GARD.BOB,a1),a0
		clr.l	(SBOB.ACTIF,a0)
.kolax		lea	(SBALLE.SIZE,a4),a4
		dbf	d6,.mai
		tst.w	d7
		beq	last_col_end

		*/ Test si envoie capsules de monnaie */
		bsr	what_that		|-> envoie gadgets
		cmp.w	#5,(area-t,a5)
		bne.s	.goxx
		cmp.w	#6,(level-t,a5)
		bne	last_col_end		-> Area 5 Level 12 & 13 
.goxx		lea	(cap.frek-t,a5),a0
		add.w	d7,(a0)
		moveq	#3,d0
		add.w	(hard-t,a5),d0
		cmp.w	(a0),d0			Lache capsule monnaie ?
		bhi.s	last_col_end		-> Non
		clr.w	(a0)
		move.l	(pt_capsule-t,a5),a0
		moveq	#NMONNAIE-1,d7
.capsloop	tst.w	(SCAP.ACTIF,a0)		Capsule Monnaie libre ?
		beq.s	.aha_yoyo		-> Oui
		lea	(SCAP.SIZE,a0),a0
		dbf	d7,.capsloop
		bra.s	last_col_end
.aha_yoyo	move.w	(GARD.XFIRE,a1),d0
		add.w	(GARD.X,a1),d0
		subq.w	#7,d0
		move.w	d0,(SCAP.X,a0)
		move.w	(GARD.YFIRE,a1),d0
		add.w	(GARD.Y,a1),d0
		subq.w	#3,d0
		move.w	d0,(SCAP.Y,a0)
		move.w	#1,(SCAP.ACTIF,a0)
		move.w	(hasard-t,a5),d0
		add.w	d7,d0
		and.w	#$3,d0
		lea	(cap.val-t,a5,d0*4),a2
		move.w	(a2)+,(SCAP.VAL,a0)
		move.w	(a2),(SCAP.SPEED,a0)
last_col_end:	rts

*=========================================================================*
*********************** Test si envoie Bonus/Malus ************************
*=========================================================================*
* Entrée	=> d7.w	Nombre de collisions
*		=> a1.l Adr. Structure GARD
*		=> a5.l	Adr. table 't'
what_that:
		add.w	d7,(malus.step-t,a5)	Incrèmente compteur malus
		move.w	(malus.frek-t,a5),d0	Fréquence des malus
		cmp.w	(malus.step-t,a5),d0	On envoie un malus ?
		bge.s	.exit			-> Non
		move.l	(pt_gadget-t,a5),a0
		moveq	#NGADGET-1,d7
.loop		tst.w	(SGAD.ACTIF,a0)		Gadget libre ?
		beq.s	.toto_what		-> oui
		lea	(SGAD.SIZE,a0),a0
		dbf	d7,.loop
.exit		rts
.toto_what	clr.w	(malus.step-t,a5)	RAZ compt. malus
		move.w	(hasard-t,a5),d0
		and.w	#31,d0
		add.w	d0,d0
		move.l	(table.malus-t,a5),a2	Adresse table malus
		add.w	d0,a2		
		move.w	(a2),d0
		*move.w	d0,(gadget.num-t,a5)	n° bonus/malus
		move.w	d0,(SGAD.NUM,a0)
		lea	(lauly-t,a5),a2
		move.w	(a2,d0),d0
		move.w	d0,(SGAD.PTBOB,a0)	Pt/Animation du BOB
		move.w	(GARD.XFIRE,a1),d0
		move.w	(GARD.YFIRE,a1),d1
		add.w	(GARD.X,a1),d0
		subq.w	#7,d0
		move.w	d0,(SGAD.X,a0)		X Bonus/Malus
		add.w	(GARD.Y,a1),d1
		subq.w	#3,d1
		move.w	d1,(SGAD.Y,a0)		Y Bonus/Malus
		move.w	#1,(SGAD.ACTIF,a0)	Active Bonus/Malus
.no_what	rts
