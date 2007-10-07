***************************************************************************
******************** DEPLACEMENT DE LA RAQUETTE ROBOT *********************
***************************************************************************
gere_robot:
		*/ Cherche la Balle la plus près du Robot */
		tst.w	(robot-t,a5)
		beq	.exit
		moveq	#36,d5
		moveq	#0,d6
		moveq	#NBALLE-1,d7
		move.l	(pt_balle-t,a5),a4
.loop		tst.w	(a4)
		beq.s	.next
		cmp.w	(2+BALLE.DIRECT,a4),d5
		bhi.s	.next
		move.w	(BALLE.Y,a4),d1
		cmp.w	d1,d6
		bhi.s	.next
		move.w	d1,d6
		move.l	a4,a3
.next		lea	(BALLE.SIZE,a4),a4
		dbf	d7,.loop

		*/ Déplace le Robot */
		tst.w	d6
		beq	.exit
		lea	(br-t,a5),a4
		move.w	(BUMP.X,a4),d1
		move.w	(BALLE.X,a3),d6
		sub.w	#32,d6			d6=X-32
		moveq	#-5,d0			Deplacement=5
		cmp.w	d6,d1			PosXR>PxB-32
		bhi.s	.kok
		moveq	#5,d0
.kok		add.w	d0,d1			PosXR+=Deplacement
		tst.w	d0
		bmi.s	.xxx1
		cmp.w	d1,d6			PxB-32>PosXR
		bhi.s	.xxx2
		bra.s	.azerty
.xxx1		cmp.w	d1,d6			PxB-32>PosXR
		bhi.s	.azerty
.xxx2		move.w	d6,d1
.azerty		cmp.w	#BP_MIN,d1
		bgt.s	.ok
		moveq	#BP_MIN,d1
.ok		cmp.w	#BP_MAX-64,d1
		blt.s	.raoul
		move.w	#BP_MAX-64,d1
.raoul		move.w	(BUMP.X,a4),d0
		move.w	d1,(BUMP.X,a4)

		*/ Détermine vitesse & table rebond
		move.l	(BUMP.RBLEFT,a4),a0	table rebond gauche
		sub.w	d0,d1			d1=NewX-OldX
		bpl.s	.left
		move.l	(BUMP.RBRIGHT,a4),a0	table rebond droite
		neg.w	d1
.left		cmp.w	#10,d1
		blt.s	.nomax
		moveq	#10,d1
.nomax		move.l	(a0,d1*4),a0
		addq.l	#2,a0
		move.l	a0,(BUMP.REBOND,a4)
.exit		rts

