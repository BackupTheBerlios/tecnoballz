*-------------------------------------------------------------------------*
*-------------------------->CYCLE LA COULEUR 30<--------------------------*
*-------------------------------------------------------------------------*
fire.color:
		moveq	#0,d0			pointeur sur la 1er table
		tst.w	(fire.flag-t,a5)		tir puissance 1 ?
		bne.s	.glouglou2		-> oui
		move.w	#cycl2-cycl1,d0		pointeur sur la 2ieme table
.glouglou2	lea	(cycl1-t,a5),a0
		addq.w	#2,(-2,a0)
		lea	(cycl2-cycl1,a0),a1
		add.w	(-2,a0),a0
		cmp.l	a0,a1
		bhi.s	.siemens
		lea	(cycl1-t,a5),a0
		clr.w	(-2,a0)
.siemens	move.w	(flag_flip-t,a5),d1
		move.l	(copper.tir-t,a5,d1),a1
		move.w	(a0,d0),(a1)
		rts

