*=========================================================================*
*                    Recopie noms dans listes joueurs                     *
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
* Utilise	d0,d1,d2 / a0,a1,a2,a3,a5
recopie_noms:
		IFEQ	A_INTRO
		lea	(intro_name-t,a5),a2
		lea	(4+liste_joueur-t,a5),a3
		moveq	#NJOUEUR-1,d2		6 noms de joueurs
.rcln.loop	move.l	(a2)+,a0		a0=pt/nom joueur ASCII
		add.l	a5,a0
		move.l	(a3)+,a1		a1=Structure Joueur
		lea	(J.NAME,a1),a1		a1=pt/nom joueur
		moveq	#5,d0
		bsr	jconvert		|-> Convertit ASCII -> J
		dbf	d2,.rcln.loop		-> boucle

		*/ Nom fictif si editeur */
		tst.w	(editor-t,a5)
		beq.s	.exit
		lea	(.name,pc),a0
		lea	(4+liste_joueur-t,a5),a3
		move.l	(a3),a1			a1=Structure Joueur
		lea	(J.NAME,a1),a1		a1=pt/nom joueur
		moveq	#5,d0
		bsr	jconvert		|-> Convertit ASCII -> J
		ENDC
.exit		rts
.name		dc.b	" TEST "

