;=========================================================================;
;		jeu : affiche les pixels pointeur de balles	      *GFX*
;=========================================================================;
; entrée	=> a5.l	adr. table 't'
msx:
		moveq	#0,d0
		moveq	#0,d1
		move.l	(bob32_ymulu-t,a5),a2
		lea	(pixtbl-t,a5),a4
		move.l	(pt_balle-t,a5),a3
		move.l	(ecran_travail-t,a5),d7
		move.w	#%111,d3
		moveq	#NBALLE-1,d4
		move.w	(flag_flip-t,a5),d6
.rabin_loop	tst.w	(BALLE.GLUE,a3)
		beq.s	.nopixel
		move.w	(BALLE.PBX,a3),d0
		move.w	(BALLE.PBY,a3),d1
		move.l	(a2,d1*4),a0
		move.w	d0,d2
		and.w	d3,d2
		move.b	(a4,d2),d2
		lsr.w	#3,d0	
		add.w	d0,a0	
		move.l	a0,(BALLE.PB1,a3,d6)
		add.l	d7,a0			adresse affichage
		or.b	d2,(a0)
		or.b	d2,(40,a0)
		or.b	d2,(80,a0)
		or.b	d2,(120,a0)
		or.b	d2,(160,a0)
.nopixel	lea	(BALLE.SIZE,a3),a3
		dbf	d4,.rabin_loop
		rts
;=========================================================================;
;		jeu : efface les pointeur de balles
;=========================================================================;
msx.off:
		moveq	#0,d0
		move.l	(pt_balle-t,a5),a3
		lea	(BALLE.PB1,a3),a3
		move.w	(flag_flip-t,a5),d6		pt écran 0 ou 4
		move.l	(ecran_travail-t,a5),d7
		move.l	(ecran3-t,a5),d1
		add.w	d6,a3
		moveq	#NBALLE-1,d6
.loop		move.l	d1,a1			adr. 'ecran3'
		move.l	d7,a2			adr. 'ecran2' ou 'ecran3'
		move.l	(a3),d0			pos. pointeur balle à l'écran
		lea	(BALLE.SIZE,a3),a3
		add.l	d0,a1
		add.l	d0,a2
		move.b	(a1),(a2)
		move.b	(TPLAN,a1),(TPLAN,a2)
		move.b	(TPLAN*2,a1),(TPLAN*2,a2)
		move.b	(TPLAN*3,a1),(TPLAN*3,a2)
		move.b	(TPLAN*4,a1),(TPLAN*4,a2)
		dbf	d6,.loop
		rts

