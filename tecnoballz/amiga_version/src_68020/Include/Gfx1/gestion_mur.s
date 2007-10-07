*//?\\//?\\//?\\//?\\//?\\//?\\//?\\//?\\//?\\//?\\//?\\//?\\//?\\//?\\//?*
*************************> GESTION DU MUR DU BAS <*************************
*//?\\//?\\//?\\//?\\//?\\//?\\//?\\//?\\//?\\//?\\//?\\//?\\//?\\//?\\//?*
gestion_mur:
		lea	(mur_du_bas-t,a5),a0
		tst.w	(a0)			Mur actif ?
		beq.s	.clr			-> Non, peut-être effacement
		subq.w	#1,(a0)			Temps mur écoulé ?
		bne.s	.yo			->  non
		move.w	#3,(mur_off-t,a5)	oui, flag efface (3 passes)
		clr.w	(mur_on-t,a5)		RAZ flag affiche
		bra.s	.clr			-> efface "ecran3"
.yo		lea	(mur_on-t,a5),a0
		move.w	(a0),d0			Mur en affichage ?
		beq.s	.clr			-> Non, peut-être effacement
		move.l	(mur_affiche-t,a5),d1	d1=adresse gfx mur actif
		bra.s	.go			-> affiche
		*/ Efface le mur */
.clr		lea	(mur_off-t,a5),a0
		move.w	(a0),d0			Mur en effacement ?
		beq.s	.exit			-> non
		move.l	(mur_restore-t,a5),d1	d1=adresse gfx mur inactif
.go		subq.w	#1,(a0)
		move.l	(ecran-t,a5,d0*4),d0
		add.l	(mur_adresse-t,a5),d0	d0=adresse écran (1, 2 ou 3)
.wait		btst	#14,(DMACONR,a6)
		bne.s	.wait
		move.l	(mur_modulos-t,a5),(BLTAMOD,a6)
		move.l	#$09F00000,(BLTCON0,a6)
		move.l	#-1,(BLTAFWM,a6)
		move.l	d1,(BLTAPTH,a6)
		move.l	d0,(BLTDPTH,a6)
		move.w	(mur_window-t,a5),(BLTSIZE,a6)
.exit		rts
