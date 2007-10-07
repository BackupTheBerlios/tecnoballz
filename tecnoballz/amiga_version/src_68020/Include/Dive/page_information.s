*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
********************* Page d'information avant le jeu *********************
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l	CUSTOM
page_information:
		lea	(pt_screen-t,a5),a4
		bsr.l	screen_res
		bne.s	.exit
		move.w	#2,(SFREQ,a0)		force 60Hz
		lea	(adr_info-t,a5),a4
		move.l	(RESA.ADR,a4),d0
		move.l	(RESA.SIZ,a4),d1
		bsr.l	affiche_pp
		bne.s	.exit
.wait_mouse	bsr.l	wait_vbl
		bsr.s	x15color		|-> Cycle couleur 15
.nocolor	tst.b	(mouse0_left-t,a5)
		beq.s	.wait_mouse
		move.w	#256,(flag_fade_off-t,a5)
.waitcol	tst.w	(flag_fade_off-t,a5)
		bne.s	.waitcol
		lea	(adr_info-t,a5),a0
		bsr.l	libere_mem		|-> Vire fichier IFF info
		bsr.l	kill_screen		|-> Vire l'écran
		moveq	#0,d7
.exit		rts		
*-------------------------- Cycle la couleur 15 --------------------------*
x15color	lea	(COLOR15,a6),a2
xxxcolor:	move.l	(pt_screen-t,a5),a0
		lea	(.table,pc),a1
		addq.w	#2,(a1)			Inc. le pt sur la table couleur
		move.w	(a1),d0			Pt table couleur dans 'd0'
		move.w	(a1,d0),d0
		bne.s	.go			-> si non nul pas fin table
		clr.w	(a1)+
		move.w	(a1),d0
.go		move.w	(SBPLCON3,a0),(BPLCON3,a6)
		move.w	d0,(a2)
		rts
.table		dc.w	0
		dc.w	$ff0,$ee0,$dd0,$cc0,$bb0,$aa0,$990,$880,$770
		dc.w	$660,$550,$440,$330,$220,$110,$220,$330,$440
		dc.w	$550,$660,$770,$880,$990,$aa0,$bb0,$cc0,$dd0
		dc.w	$ee0,$ff0,$ff0,$ff0,$ff0,$ff0,$ff0,$ff0,$000
