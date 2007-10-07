*=========================================================================*
*»»»»»»»»»»»»»»»»» Collisions des tirs avec les briques ««««««««««««««««««*
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
fire_bricks:
		move.l	a6,-(a7)
		move.l	#$00020002,d1
		move.w	#56*TLIGNEB,d2
		move.w	#BRI.MSK,d3
		move.w	(brick.save-t,a5),d4
		move.w	(fire.val-t,a5),d5
		move.w	(rak.large-t,a5),d6
		sub.w	#64,d6
		moveq	#NTIR*4-1,d7
		lea	(bri_pnt-t,a5),a1
		move.l	(pt_objet-t,a5),a3
		lea	(2+set_table-t,a5),a4
		lea	(sound+S_TOUCHE.BRIK-t,a5),a6

.loop:		tst.w	(a3)
		beq	.next
		move.l	(FIRE.X,a3),d0
		add.l	d1,d0
		move.l	a4,a0
		lea	(a1,d4),a2
		move.l	d0,(a2)+		"BRI.X & BRI.Y"
		add.w	(set.y-t,a5,d0*2),a0
		swap	d0
		add.w	(set.x-t,a5,d0*2),a0
		move.w	(a0),d0
		beq	.next			-> Pas de brique !
		move.l	(FIRE.RAK,a3),(a2)+	"BRI.RAK"
		sub.w	d2,d0			Brique indestructible ?
		blt.s	.no_kasse		-> non
		subq.w	#2,d0			Brique indestruct. destruct. ?
		ble.s	.mafins			-> non
		tst.w	(fire.flag-t,a5)	Tir puissance 2
		beq.s	.zafins			-> non
		move.w	#-1,(-8,a2)		"BRI.X" = Brique type 2 détruite
		bra.s	.fylene
.mafins		move.b	#1,(sound+S_TOUCHE.INDES1-t,a5)
		bra	.titi_fylene
.zafins		move.b	#1,(sound+S_TOUCHE.INDES2-t,a5)
		bra	.titi_fylene
.no_kasse	sub.w	d5,(JMM-2,a0)
		bgt.s	.encore
.fylene		clr.w	(a0)
		clr.w	(JMM-2,a0)
		move.w	(-2,a0),(a2)+		"BRI.NUM" =  Numéro brique
		move.w	#1,(a2)+		"BRI.FLAG"=  Affiche brique
		bra.s	.no_fylene
.encore		move.w	(a0),d0
		sub.w	d5,d0
		move.w	d0,(a0)
		move.w	d0,(a2)+		"BRI.NUM" =  Numéro brique
		clr.w	(a2)+			"BRI.FLAG"=  Affiche brique
.no_fylene	move.w	(set_guide-set_table,a0),(a2) "BRI.ADR"
		add.w	#BRI.SIZE,d4
		and.w	d3,d4
		move.b	#1,(a6)
.titi_fylene	tst.w	d6			Raquette de 64 ?
		bne.s	.fire_scie1		-> non
		cmp.w	#1,(a3)			Scie circulaire fixée ?
		beq.s	.next			-> oui
.fire_scie1	clr.w	(a3)
		move.l	(FIRE.RAK,a3),a0
		move.l	(BUMP.X,a0),(FIRE.X,a3)
.next		lea	(FIRE.SIZE,a3),a3
		dbf	d7,.loop
		move.w	d4,(brick.save-t,a5)
		move.l	(a7)+,a6
		rts
