********************************
* LIT DEBUT DE LA LISTE COPPER *
********************************
CloseLibrary	=-414
OpenLibrary	=-552
debut:		move.l	4.w,a6
		lea	gfxname(pc),a1		graphic.library
		moveq	#0,d0
		jsr	OpenLibrary(a6)
		lea	gfxbase(pc),a0
		move.l	d0,(a0)
		beq.s	.error

		*move.l	d0,a0
		*jsr	-30(a0)

****************************************************************
		move.l	d0,a0
		move.l	50(a0),d0
		move.l	d0,0
****************************************************************
		lea	gfxbase(pc),a0
.close		tst.l	(a0)
		beq.s	.error
		move.l	(a0),a1
		clr.l	(a0)
		jsr	CloseLibrary(a6)
.error:		moveq	#0,d0
		rts

gfxbase		dc.l	0
gfxname:	dc.b	'graphics.library',0
		EVEN
