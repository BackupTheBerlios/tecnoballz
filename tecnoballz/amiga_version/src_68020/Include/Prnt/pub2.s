***************************************************************************
****************************AFFICHE LES CREDITS****************************
***************************************************************************
pub2:
		tst.w	(rak.allcopy-t,a5)
		beq	.exit
		clr.w	(rak.allcopy-t,a5)
		movem.l	a5/a6,-(a7)
		move.l	a7,d7	
		move.l	(rak.lutin0.4-t,a5),a0
		move.l	(rak.lutin1.4-t,a5),a1
		lea	(2+16,a0),a0
		lea	(2+16,a1),a1
		move.l	(fontex.pagegfx-t,a5),a2
		moveq	#0,d3
		move.w	(PG_SPLAN,a2),d3
		lea	(PG_DATA,a2),a2
		move.l	a2,a3
		move.l	a2,a4
		move.l	a2,d5
		move.l	a2,a6
		move.l	a2,a7
		add.w	(credit5-t,a5),a2
		add.w	(credit4-t,a5),a3
		add.w	(credit3-t,a5),a4
		add.w	(credit2-t,a5),d5
		add.w	(credit1-t,a5),a6
		add.w	(credit0-t,a5),a7
		move.l	d5,a5
		moveq	#0,d0
		moveq	#7,d1

		*/ PLAN 1 */
.avril		move.b	(a2),(a0)+
		add.l	d3,a2
		move.b	(a3),(a0)+
		add.l	d3,a3
		move.b	(a4),(a0)+
		add.l	d3,a4
		move.b	(a5),(a0)+
		add.l	d3,a5
		move.b	(a6),(a0)+
		add.l	d3,a6
		move.b	(a7),(a0)+
		add.l	d3,a7
		move.w	d0,(a0)+

		*/ PLAN 2 */
		move.b	(a2),(a0)+
		add.l	d3,a2
		move.b	(a3),(a0)+
		add.l	d3,a3
		move.b	(a4),(a0)+
		add.l	d3,a4
		move.b	(a5),(a0)+
		add.l	d3,a5
		move.b	(a6),(a0)+
		add.l	d3,a6
		move.b	(a7),(a0)+
		add.l	d3,a7
		move.w	d0,(a0)+

		*/ PLAN 3 */
		move.b	(a2),(a1)+
		add.l	d3,a2
		move.b	(a3),(a1)+
		add.l	d3,a3
		move.b	(a4),(a1)+
		add.l	d3,a4
		move.b	(a5),(a1)+
		add.l	d3,a5
		move.b	(a6),(a1)+
		add.l	d3,a6
		move.b	(a7),(a1)+
		add.l	d3,a7
		move.w	d0,(a1)+

		*/ PLAN 4 */
		move.b	(a2),(a1)+
		add.l	d3,a2
		move.b	(a3),(a1)+
		add.l	d3,a3
		move.b	(a4),(a1)+
		add.l	d3,a4
		move.b	(a5),(a1)+
		add.l	d3,a5
		move.b	(a6),(a1)+
		add.l	d3,a6
		move.b	(a7),(a1)+
		add.l	d3,a7
		move.w	d0,(a1)+

		dbf	d1,.avril
		move.l	d7,a7
		movem.l	(a7)+,a5/a6
.exit		rts
