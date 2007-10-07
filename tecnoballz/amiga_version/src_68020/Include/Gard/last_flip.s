*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
********************* ECHANGE LES DEUX LISTES COPPERS *********************
*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l CUSTOM
last_flip:
		move.l	(pt_screen-t,a5),a0
		tst.w	(flag_flip-t,a5)
		beq.w	.gx
		clr.w	(flag_flip-t,a5)
		move.l	(SPECRAN1,a0),(ecran_travail-t,a5)
		move.l	(SPWAIT1,a0),a2
		move.l	(SPPOINTEUR,a0),a1
		move.l	(SACOPPER,a0),a0
		bra.s	.go_xmen
.gx		move.w	#4,(flag_flip-t,a5)
		move.l	(SPECRAN2,a0),(ecran_travail-t,a5)
		move.l	(SPWAIT2,a0),a2
		move.l	(SPPOINTEUR2,a0),a1
		move.l	(SACOPPER2,a0),a0
.go_xmen	addq.l	#2,a1
		move.l	a1,(sc_u1-t,a5)
		move.l	a2,(sc_w1-t,a5)
		move.l	a0,(acopper-t,a5)
		move.l	a0,(COP1LCH,a6)


		rts
