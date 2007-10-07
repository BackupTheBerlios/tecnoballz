*-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-*
************************* TEST DEPLACEMENT SOURIS *************************
*-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-*
* Entrée	=> a5.l	Adr. table 't'
*		=> a6.l	CUSTOM
move_mouse:
		bsr	.test_depla
		bsr	.test_depass
		rts

*--------------------> Test déplacement de la souris <--------------------*
* entrée	=> a5.l	Adr. table 't'
*		=> a6.l	CUSTOM
* sortie	<= a0.l	'mouse_x'+2
.test_depla
		moveq	#0,d0
		moveq	#0,d1
		lea	(mouse_y-t,a5),a0
		lea	(joy0dat-t,a5),a1
		move.b	(JOY0DAT,a6),d0
		move.b	(a1),d1
		move.b	d0,(a1)+
		bsr.s	.test
		move.b	(JOY0DAT+1,a6),d0
		move.b	(a1),d1
		move.b	d0,(a1)
.test		sub.b	d1,d0
		tst.b	d0
		bge.s	.positif
		not.b	d0
		sub.w	d0,(a0)+
		rts
.positif	add.w	d0,(a0)+
		rts

*-----------------> Test les dépassement de coordonnées <-----------------*
* Entrée	=> a0.l	'mouse_x'+2 = 'mouse_minix'
*		=> a5.l	adr. table 't'
.test_depass:
		* test dépassement à gauche *
		move.l	a0,a1
		move.w	(a1)+,d0
		cmp.w	-(a0),d0		Pos. X trop petite ?
		bmi.s	.droite			-> non
		move.w	d0,(a0)

		* test dépassement à droite *
.droite		move.w	(a1)+,d0
		cmp.w	(a0),d0			Pos. X Trop grande ?
		bhi.s	.haut			-> non
		move.w	d0,(a0)

		* test dépassement en haut *
.haut		move.w	(a1)+,d0
		cmp.w	-(a0),d0		Pos. Y trop petite ?
		bmi.s	.bas			-> non
		move.w	d0,(a0)

		* test dépassement en bas *
.bas		move.w	(a1),d0
		cmp.w	(a0),d0			Pos. Y trop grande ?
		bhi.s	.fin			-> non
		move.w	d0,(a0)
.fin		rts
