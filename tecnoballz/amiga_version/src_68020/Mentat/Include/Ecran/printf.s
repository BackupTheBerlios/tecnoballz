*=========================================================================*
**************** Affiche une chaine de caractère à l'écran ****************
*=========================================================================*
*----------------------> Affiche le text à l'écran <----------------------*
* Entrée	=> d0.w	Position x en pixel (multiple de 8)
*		=> d1.w	Position Y en pixel
*		=> a0.l	Adr. chaine de caractère
* 		=> a5.l	adr. table 't'
printf:
		tst.l	(pt_screen-t,a5)	structure écran ?
		beq	.exit			-> non
		move.l	(pt_screen-t,a5),a1	a1=Adr. structure écran
		tst.w	(SFLAG,a1)		structure initialisée ?
		beq	.exit			-> non
		moveq	#0,d3
		move.w	(SP_SLINE,a1),d3	d3=Taille ligne en octets
		move.w	d3,d4
		mulu	d3,d1			d1=Y * Nbre d'octets par ligne
		lsl.w	#3,d3
		move.l	(SPECRAN1,a1),d2	d2=Adr. écran
		lsr.w	#3,d0
		add.w	d0,d1
		add.l	d1,d2			d2=Adr. aff. 1er caractère
		move.l	(adr_fonteraw-t,a5),d0
		moveq	#0,d1
		bra.s	.read_char

*		-/- Boucle d'affichage -/-
* Entrée	=> d0.l	Adr. police de 8*8
*		=> d1.l	#0
*		=> d2.l	Adr. d'affichage
*		=> d3.l	Offset prochaine ligne de caractère
*		=> d4.w	Offset prochaine ligne
*		=> a0.l	Adr. texte ASCII
.new_line	move.l	d2,a1			a1=Adr. d'affichage
.new_char	move.l	d0,a2			a2=Adr. police de caractère
		add.w	d1,a2			a2=Adr. caract. à afficher
		move.l	a1,a3			a3=Adr. d'affichage
		moveq	#8-1,d5			d5=7=hauteur caract. - 1
.aff_loop	move.b	(a2),(a3)		Affiche une ligne
		lea	(256,a2),a2		Ligne source suivante
		add.w	d4,a3			Ligne destination suivante
		dbf	d5,.aff_loop
		addq.l	#1,a1			X=X+1
		move.b	(a0)+,d1		Lit caractère
		bne.s	.new_char		-> Change pas de ligne
		add.l	d3,d2			d2=Adr. aff. prochaine ligne
.read_char	move.b	(a0)+,d1		Lit caractère
		bne.s	.new_line		-> Continue
.exit		rts
