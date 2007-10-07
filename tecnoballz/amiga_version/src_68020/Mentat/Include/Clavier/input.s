* ROUTINE A EXECUTEE UNIQUEMENT PENDANT LA VBL *
* Cette routine modifie les "keys-flags" de la table clavier. Si cette
* routine fonctionne hors VBL, l'interruption clavier peut l'interrompre
* en plein travail. Une instruction teste le "key-flag" à 1, mais ce même
* "key-flag" (modifié par l'interruption clavier) peut se retouver à 0
* l'instruction suivante. Conséquence : La touche est considérée préssée alors
* qu'en réalité elle est relachée.
*-------------------------------------------------------------------------*
********************** Entre une chaine de caractère **********************
*-------------------------------------------------------------------------*
* Entrée	=> d7.w	Nombre de caractère
*		=> a4.l	Pt/chaine
* Sortie	<= d0.w	#1 = Return / Enter préssée
*		<= d6.w	Position du cuseur de 0 à d7-1
input:
		* test si position du curseur est correcte *
		move.w	(xcurs-t,a5),d6
		move.l	(atable_clavier-t,a5),a0
		lea	(test_clavier,pc),a3

		* Touche spéciales *
* entrée	=> a0.l	Adr. table clavier
*		=> a3.l	'test_clavier'
		lea	(.speciales,pc),a2
		moveq	#(.fin_spec-.speciales)/6-1,d2
.loop_spec	move.l	a0,a1			a1=adr. table clavier
		add.w	(a2)+,a1		+ Code raw
		jsr	(a3)			|-> 'test_clavier'
		tst.l	d0			Touche prise en compte ?
		beq.s	.next_spec		-> Non
		move.l	(a2),a1
		add.l	a5,a1
		jmp	(a1)			-> Saute routine correspondante
.next_spec	addq.l	#4,a2			Saute l'adr. de la routine
		dbf	d2,.loop_spec

		* Test touches shift *
* entrée	=> a0.l	Adr. table clavier
		move.w	#.no_shift-.code_raw,d3
		tst.b	(TOUCHE_CAPSLOCK,a0)
		bne.s	.shift_ok
		tst.b	(TOUCHE_SHIFT_G,a0)
		bne.s	.shift_ok
		tst.b	(TOUCHE_SHIFT_D,a0)
		beq.s	.shift_no
.shift_ok	move	#.shift-.code_raw,d3
.shift_no
		* AUTRES TOUCHES *
* entrée	=> a0.l	Adr. table clavier
*		=> a3.l	'test_clavier'
		move	#(.fin_cr-.code_raw)-1,d2
		lea	(.code_raw,pc),a2
		moveq	#0,d1
.loop		move.b	(a2),d1			d1=code raw
		lea	(a0,d1),a1
		jsr	(a3)			|-> 'test_clavier'
		tst.l	d0
		bne.s	.touche_presse
		addq.l	#1,a2
		dbf	d2,.loop
		bra.s	.exit

		* Une touche est préssée *
* entrée	=> d3.w	offset sur caratère à insérer
*		=> d6.w	pos. curseur
*		=> d7.w	nbre caractère chaine
*		=> a2.l	adr. code table codes raw
*		=> a4.l	adr. chaine
.touche_presse	move.w	d7,d0
		sub.w	d6,d0
		subq.w	#2,d0
		bmi.s	.ecrit_caract
		lea	(a4,d7),a1
		lea	(-1,a1),a0
.insert_loop	move.b	-(a0),-(a1)
		dbf	d0,.insert_loop
.ecrit_caract	move.b	(a2,d3),(a4,d6)
		* Curseur droite *
* entrée	=> d6.w	pos. curseur
*		=> d7.w	nbre caractère chaine
.right		move.w	d7,d0
		subq.w	#1,d0
		cmp.w	d6,d0
		beq.s	.exit
		addq.w	#1,d6
		bra.s	.pos_change
		* Curseur gauche *
* entrée	=> d6.w	pos. curseur
.left		tst.w	d6
		beq.s	.exit
		subq.w	#1,d6
.pos_change	lea	(xcurs-t,a5),a0
		move.w	d6,(a0)
		bra.s	.exit

		*/ Touche "RETURN" OU "ENTER" */
.return		moveq	#1,d0			d0=1=return préssée
		lea	(xcurs-t,a5),a0
		clr.w	(a0)			position curseur = 0
		bra.s	.fin

*		*/ Touche "BACK-SPACE" */
* entrée	=> d6.w	pos. curseur
.backspace	tst.w	d6
		beq.s	.exit
		subq.w	#1,d6
		lea	(xcurs-t,a5),a0
		move.w	d6,(a0)

		*/ Touche "DEL" */
* entrée	=> d6.w	pos. curseur
*		=> d7.w	nbre caractère chaine
*		=> a4.l	adr. chaine
.delete		lea	(a4,d6),a0
		lea	(1,a0),a1
		move.w	d7,d0
		sub.w	d6,d0
		subq.w	#2,d0
		bmi.s	.exit
.del_loop	move.b	(a1)+,(a0)+
		dbf	d0,.del_loop
		move.b	#' ',(a0)
.exit		moveq	#0,d0			d0=0 touche return no préssée
.fin		rts

		* Touches spéciales *
.speciales	dc.w	TOUCHE_GAUCHE
		dc.l	.left-t
		dc.w	TOUCHE_DROITE
		dc.l	.right-t
		dc.w	TOUCHE_RETURN
		dc.l	.return-t
		dc.w	TOUCHE_ENTER
		dc.l	.return-t
		dc.w	TOUCHE_BACK
		dc.l	.backspace-t
		dc.w	TOUCHE_DEL
		dc.l	.delete-t
.fin_spec
		* Autres touches (code RAW du clavier Amiga) *
.code_raw	dc.b	TOUCHE_A,TOUCHE_B,TOUCHE_C,TOUCHE_D,TOUCHE_E,TOUCHE_F
		dc.b	TOUCHE_G,TOUCHE_H,TOUCHE_I,TOUCHE_J,TOUCHE_K,TOUCHE_L
		dc.b	TOUCHE_M,TOUCHE_N,TOUCHE_O,TOUCHE_P,TOUCHE_Q,TOUCHE_R
		dc.b	TOUCHE_S,TOUCHE_T,TOUCHE_U,TOUCHE_V,TOUCHE_W,TOUCHE_X
		dc.b	TOUCHE_Y,TOUCHE_Z,TOUCHE_ESPACE
		dc.b	TOUCHE_1,TOUCHE_2,TOUCHE_3,TOUCHE_4,TOUCHE_5,TOUCHE_6
		dc.b	TOUCHE_7,TOUCHE_8,TOUCHE_9,TOUCHE_0,TOUCHE_DEGRE
		dc.b	TOUCHE_TIRET,TOUCHE_BARRE
		dc.b	TOUCHE_NUM_1,TOUCHE_NUM_2,TOUCHE_NUM_3,TOUCHE_NUM_4
		dc.b	TOUCHE_NUM_5,TOUCHE_NUM_6,TOUCHE_NUM_7,TOUCHE_NUM_8
		dc.b	TOUCHE_NUM_9,TOUCHE_NUM_0
.fin_cr
.no_shift	dc.b	'abcdefghijklmnopqrstuvwxyz '
		dc.b	'&é"',"'(§è!çà)-\"
		dc.b	'1234567890'		pavé numérique
.shift		dc.b	'ABCDEFGHIJKLMNOPQRSTUVWXYZ '
		dc.b	'1234567890°_|'
		dc.b	'1234567890'		pavé numérique
		EVEN
