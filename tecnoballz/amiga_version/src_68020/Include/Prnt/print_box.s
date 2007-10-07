
print_box:
		*/ NOM DU JOUEUR */
		moveq	#33,d0
		move.w	#39,d1
		moveq	#5,d2
		bsr	chameau
		lea	(name.x-t,a5),a0
		bsr	print_char

		*/ NOM DU MEILLEUR JOUEUR */
		moveq	#33,d0
		move.w	#188,d1
		moveq	#5,d2
		bsr	chameau
		lea	(joueur0-t,a5),a0
		bsr	print_char

		*/ SCORE DU MEILLEUR JOUEUR */
		moveq	#33,d0
		move.w	#207,d1
		moveq	#5,d2
		bsr	chameau
		lea	(score00-t,a5),a0
		bsr	print_char

		*/ LEVEL */
		moveq	#37,d0
		move.w	#153,d1
		moveq	#1,d2
		bsr	chameau
		lea	(lev1-t,a5),a0
		bsr	print_looping

		*/ AREA */
		moveq	#33,d0
		move.w	#153,d1
		moveq	#1,d2
		bsr	chameau
		lea	(area+2-t,a5),a0
		bsr	print_looping

		*/ SCORE */
		moveq	#33,d0
		moveq	#58,d1
		moveq	#5,d2
		bsr	chameau

		*/ LIFES */
		moveq	#37,d0
		moveq	#76,d1
		moveq	#1,d2
		bsr	chameau

		*/ BRICKS */
		moveq	#33,d0
		moveq	#76,d1
		moveq	#2,d2
		bsr	chameau
		rts

*                           MET PLAN OMBRE A UN				   *
* Entrée	=> d0.w	Pos. X en octets
*		=> d1.w Pos. Y en lignes
*		=> d2.w Nombre de caractère(s) - 1
chameau:	movem.l	d0-d2,-(a7)
		move.l	(pt_screen-t,a5),a3
		moveq	#0,d3
		move.w	(SP_SLINE,a3),d3
		mulu	d3,d1
		move.l	(SPECRAN1,a3),a2
		add.l	d1,a2
		add.w	d0,a2
		add.w	(SP_SPLAN,a3),a2
		add.w	(SP_SPLAN,a3),a2
		add.w	(SP_SPLAN,a3),a2
		moveq	#-1,d0
		moveq	#7,d4
.l2		move.l	a2,a0
		move.w	d2,d1
.l1		move.b	d0,(a0)+
		dbf	d1,.l1
		add.l	d3,a2
		dbf	d4,.l2
		movem.l	(a7)+,d0-d2
		rts	
