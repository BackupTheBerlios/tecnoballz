*-------------------------------------------------------------------------*
**********************  Ajoute une Gemme à l'écran  ***********************
*-------------------------------------------------------------------------*
* Entrée	=> d0.w Position X  & Position Y
*		=> d5.w Hasard
add_gemme:
		move.l	(pt_gemme-t,a5),a2
		moveq	#NGEMME-1,d2
.loop		tst.w	(a2)
		bne.s	.next
		add.w	d2,d5
		move.w	#1,(a2)
		move.l	d0,(GEM.X,a2)
		add.w	d2,d5
		move.w	(hasard2-t,a5),d0
		add.w	d2,d0
		and.w	#14,d0
		move.w	(.xx,pc,d0),d0
		move.w	d0,(GEM.NUM,a2)
		move.l	(BOB.TABLE,a2),a1	a1=table adr. gfx & msk
		lea	(a1,d0*8),a1
		move.l	(a1)+,(BOB.GFX,a2)
		move.l	(a1),(BOB.MSK,a2)
		rts
.next		lea	(GEM.SIZE,a2),a2
		dbf	d2,.loop
		rts
.xx		dc.w	0,1,2,3
		dc.w	4,5,0,1


