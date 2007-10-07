*--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-*
*«O»«O»«O»«O»«O»«O»«O» Collisions balles et briques «O»«O»«O»«O»«O»«O»«O»«*
*--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-*
* Entrée	=> a5.l	adr. table 't'
gad_re:
		move.l	a6,-(a7)
		lea	(bri_pnt-t,a5),a1
		move.l	(pt_balle-t,a5),a3
		lea	(2+set_table-t,a5),a6
		move.w	#56*TLIGNEB,d3
		move.w	(brick.save-t,a5),d6	table effacement briques
		moveq	#NBALLE-1,d7

.loop:		tst.w	(a3)
		beq	.next
		move.l	(BALLE.POI,a3),a4
		moveq	#0,d1			Flag Brique indestructible touchée
		moveq	#1,d2			Incrémentation du flag rebond
		moveq	#3,d4
		moveq	#0,d5			Flag rebond
.col_loop:	move.l	(BALLE.X,a3),d0
		add.l	(a4)+,d0
		lea	(a1,d6),a2
		move.l	d0,(a2)+		"BRI.X & BRI.Y"
		move.l	a6,a0
		add.w	(set.y-t,a5,d0*2),a0
		swap	d0
		add.w	(set.x-t,a5,d0*2),a0
		move.w	(a0),d0
		beq.s	.nocol			-> Pas de brique ici !
		move.l	(BALLE.RAK,a3),(a2)+	"BRI.RAK"
		sub.w	d3,d0			Brique indestructible ?
		blt.s	.noindes		-> non
		moveq	#1,d1			Brique indestructible touché
		subq.w	#2,d0			Indestructible destructible ?
		ble.s	.des1			-> non
		cmp.w	#6,(BALLE.PT,a3)	Balle puissance 2 ?
		bne.s	.des2			-> non
		move.w	#-1,(-8,a2)		"BRI.X" =Brique type 2 détruite
		bra.s	.clear
.des1		move.b	#1,(sound+S_TOUCHE.INDES1-t,a5)
		bra.s	.rebond
.des2		move.b	#1,(sound+S_TOUCHE.INDES2-t,a5)
		bra.s	.rebond
.noindes	move.w	(BALLE.VAL,a3),d0
		sub.w	d0,(JMM-2,a0)
		bgt.s	.encore
.clear		clr.w	(a0)
		clr.w	(JMM-2,a0)
		move.w	(-2,a0),(a2)+		"BRI.NUM"  = Numéro brique
		move.w	#1,(a2)+		"BRI.FLAG" = Affiche décor
		bra.s	.inc
.encore		sub.w	d0,(a0)
		move.w	(a0),(a2)+		"BRI.NUM" = Numéro brique
		clr.w	(a2)+			"BRI.FLAG"= Affiche brique
.inc		move.w	(set_guide-set_table,a0),(a2)	"BRI.ADR"
		add.w	#BRI.SIZE,d6
		and.w	#BRI.MSK,d6
.rebond		add.w	d2,d5			Incrémente le flag rebond
.nocol		add.w	d2,d2
		dbf	d4,.col_loop
		subq.w	#1,d1			Touche brique indestructible ?
		beq.s	.ni			-> non
		cmp.w	#4,(BALLE.PT,a3)	Balle puissance 1 ou 2 ?
		bge.s	.next			-> oui, pas de rebond
.ni		subq.w	#1,d5
		bmi.s	.next
		move.l	(bri_jump-t,a5,d5*4),a0
		add.l	(BALLE.DIRECT,a3),a0
		move.l	(a0),(BALLE.DIRECT,a3)
.next		lea	(BALLE.SIZE,a3),a3
		dbf	d7,.loop
.t2		move.w	d6,(brick.save-t,a5)

*««««««««««««««««««««««««« Collision mur du bas »»»»»»»»»»»»»»»»»»»»»»»»»»*
test_mur:	
		move.w	#224,d3
		move.w	#32,d4
		move.w	(mur_du_bas-t,a5),d6
		moveq	#NBALLE-1,d7
		lea	(sound+S_BRICOTE-t,a5),a1
		move.l	(pt_cote-t,a5),a2	Table Bricotes à effacer
		move.l	(pt_balle-t,a5),a3
		lea	(mur.kas_lf-t,a5),a4
.bri_col	tst.w	(a3)			Balle active ?
		beq	.next			-> non, sort
		move.w	(BALLE.X,a3),d0
		move.w	(BALLE.Y,a3),d1
		move.w	(BALLE.PT,a3),d5
		tst.w	d6
		beq.s	.mur_h1
		move.w	#240,d2
		sub.w	(BOB.WIDTH,a3),d2
		cmp.w	d2,d1
		blt.s	.mur_h1
		cmp.w	d4,d0
		blt.s	.mur_h1
		cmp.w	d3,d0
		bhi.s	.mur_h1
		move.b	#1,(a1)
		lea	(rb7-t,a5),a0
		bra	.re_cas0

*»»»»»»»»»»»»»»»»»»»»»»»» Collision mur de gauche ««««««««««««««««««««««««*
.mur_h1		cmp.w	d3,d1
		bhi	.next
		cmp.w	d4,d1
		blt.s	.mur_h3
		cmp.w	#15,d0
		bge.s	.mur_h2
		move.b	#1,(a1)
		tst.w	(a4)
		beq.s	.well_p1
		lsr.w	#4,d1
		lea	(bri_h1-t,a5,d1),a0
		tst.b	(a0)
		beq	.next
		clr.b	(a0)
		move.w	#3,(a2)+
		move.w	d1,(a2)+
		subq.w	#6,d5
		beq	.next
.well_p1	lea	(rb5-t,a5),a0
		bra	.re_cas0

*»»»»»»»»»»»»»»»»»»»»»»»» Collision mur de droite ««««««««««««««««««««««««*
.mur_h2:	move.w	#240,d2
		sub.w	(BOB.WIDTH,a3),d2
		cmp.w	d0,d2
		bhi.s	.mur_h3
		move.b	#1,(a1)
		tst.w	(mur.kas_rg-t,a5)
		beq.s	.well_p2
		lsr.w	#4,d1
		lea	(bri_h2-t,a5,d1),a0
		tst.b	(a0)
		beq.s	.next
		clr.b	(a0)
		move.w	#2,(a2)+
		move.w	d1,(a2)+
		subq.w	#6,d5
		beq.s	.next
.well_p2	lea	(rb1-t,a5),a0
		bra	.re_cas0

*»»»»»»»»»»»»»»»»»»»»»»»»» Collision mur du haut «««««««««««««««««««««««««*
.mur_h3:	sub.w	#15,d1
		bge.s	.next
		cmp.w	#15,d0
		blt.s	.next
		cmp.w	d3,d0
		bhi.s	.next
		move.b	#1,(a1)
		tst.w	(mur.kas_up-t,a5)
		beq.s	.well_p3
		lsr.w	#4,d0
		lea	(bri_h3-t,a5,d0),a0
		tst.b	(a0)
		beq	.next
		clr.b	(a0)
		move.w	#1,(a2)+
		move.w	d0,(a2)+
		subq.w	#6,d5
		beq.s	.next
.well_p3	lea	(rb3-t,a5),a0
.re_cas0:	add.l	(BALLE.DIRECT,a3),a0
		move.l	(a0),(BALLE.DIRECT,a3)

		**** col briks : fin de la boucle ****
.next		tst.w	(BALLE.ATOM,a3)
		beq.s	.p1
		subq.w	#1,(BALLE.ATOM,a3)
.p1		lea	(BALLE.SIZE,a3),a3
		dbf	d7,.bri_col
		move.l	a2,(pt_cote-t,a5)
		move.l	(a7)+,a6
		rts
