*=========================================================================*
**************** Affiche une chaine de caract�re � l'�cran ****************
*=========================================================================*
*----------------------> Affiche le text � l'�cran <----------------------*
* Entr�e	=> d0.w	Position x en pixel (multiple de 8)
*		=> d1.w	Position Y en pixel
*		=> a0.l	Adr. chaine de caract�re
* 		=> a5.l	adr. table 't'
printf:
		tst.l	(pt_screen-t,a5)	structure �cran ?
		beq	.exit			-> non
		move.l	(pt_screen-t,a5),a1	a1=Adr. structure �cran
		tst.w	(SFLAG,a1)		structure initialis�e ?
		beq	.exit			-> non
		moveq	#0,d3
		move.w	(SP_SLINE,a1),d3	d3=Taille ligne en octets
		move.w	d3,d4
		mulu	d3,d1			d1=Y * Nbre d'octets par ligne
		lsl.w	#3,d3
		move.l	(SPECRAN1,a1),d2	d2=Adr. �cran
		lsr.w	#3,d0
		add.w	d0,d1
		add.l	d1,d2			d2=Adr. aff. 1er caract�re
		move.l	(adr_fonteraw-t,a5),d0
		moveq	#0,d1
		bra.s	.read_char

*		-/- Boucle d'affichage -/-
* Entr�e	=> d0.l	Adr. police de 8*8
*		=> d1.l	#0
*		=> d2.l	Adr. d'affichage
*		=> d3.l	Offset prochaine ligne de caract�re
*		=> d4.w	Offset prochaine ligne
*		=> a0.l	Adr. texte ASCII
.new_line	move.l	d2,a1			a1=Adr. d'affichage
.new_char	move.l	d0,a2			a2=Adr. police de caract�re
		add.w	d1,a2			a2=Adr. caract. � afficher
		move.l	a1,a3			a3=Adr. d'affichage
		moveq	#8-1,d5			d5=7=hauteur caract. - 1
.aff_loop	move.b	(a2),(a3)		Affiche une ligne
		lea	(256,a2),a2		Ligne source suivante
		add.w	d4,a3			Ligne destination suivante
		dbf	d5,.aff_loop
		addq.l	#1,a1			X=X+1
		move.b	(a0)+,d1		Lit caract�re
		bne.s	.new_char		-> Change pas de ligne
		add.l	d3,d2			d2=Adr. aff. prochaine ligne
.read_char	move.b	(a0)+,d1		Lit caract�re
		bne.s	.new_line		-> Continue
.exit		rts
