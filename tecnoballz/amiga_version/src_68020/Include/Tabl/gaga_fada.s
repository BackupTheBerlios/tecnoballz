**==**==**==**==**==**==**==**==**==**==**==**==**==**==**==**==**==**==***
*---------------> Gestion fondu des dégradés des briques <----------------*
**==**==**==**==**==**==**==**==**==**==**==**==**==**==**==**==**==**==***
* Entrée	=> a5.l	Adr. 't'
gaga_fada:

		tst.w	(flag_fade_off-t,a5)
		bne	gaga_fade_off
		tst.w	(flag_fade_on-t,a5)
		bne	gaga_fade_on
		rts

************** fondu noir -> couleurs finales (AGA 24 bits) ***************
gaga_fade_on:
		move.l	(pt_screen-t,a5),a0
		moveq	#$00F,d2
		move	#$0F0,d3
		move	#$F00,d4
		move	#$100,d5
		move.w	(SFADEONSPEED,a0),d6		d6=nbre de passage-1
.loop		lea	(gaga-t,a5),a4		bits 4 à 7
		lea	(2,a4),a3		bits 0 à 3
		move.l	(degrade_copper-t,a5),a2
		lea	(16+8+2,a2),a2		bits 4 à 7
		lea	(8,a2),a1		bits 0 à 3
		moveq	#TBC-1,d7		nbre de couleurs
.loop_master	move.w	(a2),d0			*/ Bleu */
		move.w	(a4),d1
		and.w	d2,d0
		and.w	d2,d1
		cmp	d0,d1
		bne.s	.blue_modify
		move.w	(a1),d0
		move.w	(a3),d1
		and.w	d2,d0
		and.w	d2,d1
		cmp.w	d0,d1
		beq.s	.blue_end
.blue_modify	move.w	(a1),d0
		addq.w	#1,d0
		and.w	d2,d0
		bne.s	.blueok
		and.w	#$FF0,(a1)
		addq.w	#1,(a2)
		bra.s	.blue_end
.blueok		addq.w	#1,(a1)
.blue_end	move.w	(a2),d0			*/ Vert */
		move.w	(a4),d1
		and.w	d3,d0
		and.w	d3,d1
		cmp.w	d0,d1
		bne.s	.green_modify
		move.w	(a1),d0
		move.w	(a3),d1
		and.w	d3,d0
		and.w	d3,d1
		cmp.w	d0,d1
		beq.s	.green_end
.green_modify	move.w	(a1),d0
		add.w	#$010,d0
		and.w	d3,d0
		bne.s	.greenok
		and.w	#$F0F,(a1)
		add.w	#$010,(a2)
		bra.s	.green_end
.greenok	add.w	#$010,(a1)
.green_end	move.w	(a2),d0			*/ Rouge */
		move.w	(a4),d1
		and.w	d4,d0
		and.w	d4,d1
		cmp.w	d0,d1
		bne.s	.red_modify
		move.w	(a1),d0
		move.w	(a3),d1
		and.w	d4,d0
		and.w	d4,d1
		cmp.w	d0,d1
		beq.s	.red_end
.red_modify	move.w	(a1),d0
		add.w	d5,d0
		and.w	d4,d0
		bne.s	.redok
		and.w	#$0FF,(a1)
		add.w	d5,(a2)
		bra.s	.red_end
.redok		add.w	d5,(a1)
.red_end	lea	(20,a1),a1
		lea	(20,a2),a2
		addq.l	#4,a3
		addq.l	#4,a4
		dbf	d7,.loop_master
		dbf	d6,.loop
		bra	peek.citron

****************** fondu couleurs -> noir (AGA 24 bits) *******************
gaga_fade_off:
		move.l	(pt_screen-t,a5),a0
		moveq	#$00F,d2
		move.w	#$0F0,d3
		move.w	#$F00,d4
		moveq	#$010,d5
		move.w	(SFADEOFFSPEED,a0),d6
.loop		move.l	(degrade_copper-t,a5),a2
		lea	(16+8+2,a2),a2		Bits 4 à 7
		lea	(8,a2),a1		Bits 0 à 3
		moveq	#TBC-1,d7		nbre de couleurs
.loop_master	move.w	(a2),d0			*/ Bleu */
		and.w	d2,d0
		bne.s	.blue_modify
		move.w	(a1),d0
		and.w	d2,d0
		beq.s	.blue_end
.blue_modify	move.w	(a1),d0
		and.w	d2,d0
		bne.s	.blueok
		or.w	d2,(a1)	
		subq.w	#1,(a2)
		bra.s	.blue_end
.blueok		subq.w	#1,(a1)
.blue_end	move.w	(a2),d0			*/ Vert */
		and.w	d3,d0
		bne.s	.green_modify
		move.w	(a1),d0
		and.w	d3,d0
		beq.s	.green_end
.green_modify	move.w	(a1),d0
		and.w	d3,d0
		bne.s	.greenok
		or.w	d3,(a1)
		sub.w	d5,(a2)
		bra.s	.green_end
.greenok	sub.w	d5,(a1)
.green_end	move.w	(a2),d0			*/ Rouge */
		and.w	d4,d0
		bne.s	.red_modify
		move.w	(a1),d0
		and.w	d4,d0
		beq.s	.red_end
.red_modify	move.w	(a1),d0
		and.w	d4,d0
		bne.s	.redok
		or.w	d4,(a1)
		sub.w	#$100,(a2)
		bra.s	.red_end
.redok		sub.w	#$100,(a1)
.red_end	lea	(20,a1),a1
		lea	(20,a2),a2
		dbf	d7,.loop_master
		dbf	d6,.loop
peek.citron	move.l	(degrade_copper-t,a5),a0
		move.l	(4+degrade_copper-t,a5),a1
		lea	(16+8+2,a0),a0
		lea	(16+8+2,a1),a1
		moveq	#TBC-1,d7		nbre de couleurs
.rkp		move.w	(a0),(a1)		bits 4 à 7
		move.w	(8,a0),(8,a1)		bits 0 à 3
		lea	(20,a0),a0
		lea	(20,a1),a1
		dbf	d7,.rkp
		rts
