*//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/*
*                    RETOUR AU MENU (AMIGA-DROIT + Q)			  *
*//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/*
* Entrée	=> a5.l	Adr. 't'
quit:
		tst.b	(flag.quit-t,a5)
		beq.s	.exit
		clr.b	(flag.quit-t,a5)
		bsr.l	go_fade_off
.test		bsr.l	wait_vbl
		IFEQ	A_TAB
		cmp.w	#15,(rout-t,a5)
		beq.s	.r
		cmp.w	#1,(rout-t,a5)
		bne.s	.s
.r		bsr	gaga_fada		|-> Fade couleurs briques
		ENDC
.s		tst.w	(flag_fade_off-t,a5)
		bne.s	.test
		IFEQ	A_TAB
		bsr	rupt1.off
		ENDC
		IFEQ	A_SHOP
		bsr.l	rupt3.off
		ENDC
		IFEQ	A_GARD
		bsr	rupt4.off
		ENDC
		bsr	rupt15.off
		bsr.l	np_end

		*/ Retour à l'éditeur */
		tst.w	(editor-t,a5)		Editeur en cours ?
		beq.s	.noed			-> non
		cmp.w	#1,(rout-t,a5)		Tableaux ?
		bne.s	.noed			-> non
		move.w	#14,(rout-t,a5)
		bra.s	.go

		*/ Retour au menu */
.noed		move.w	#6,(rout-t,a5)
.go		move.l	#irq,(a7)
.exit		rts
