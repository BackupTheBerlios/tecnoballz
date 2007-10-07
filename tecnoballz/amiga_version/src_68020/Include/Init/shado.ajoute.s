*									  *
* "shadonly.ajoute" 	=> Réserve & Init. Struct. + Ajoute BOB
* "shado.ajoute"	=> Init. Struct. + Ajoute BOB
* "shadonly.ajoute_bob"	=> Réserve & Init. Struct. avec paramètres + Ajoute BOB
* "shado.plus"		=> Ajoute Structure BOB dans la liste d'affichage
*									  *
*-------------------------------------------------------------------------*
************** Ajoute une Sahdow dans la liste d'affichage ****************
*-------------------------------------------------------------------------*
* Entrée	=> d0.l	Numéro du Shadow-BOB
*		=> a1.l	Structure Shadow-BOB
*		=> a5.l	adresse 't'
* Sortie	<= a1.l	adresse structure Shadow-BOB
*		<= d7.l	#0 pas d'erreur
shado.ajoute:
		movem.l	d0-d4/a0/a2/a3/a4,-(a7)
		bsr.l	bob32_get.bob
		bne.s	.exit
		moveq	#0,d1			position x
		moveq	#0,d2			position y
		moveq	#0,d3			pointeur animation
		moveq	#1,d4			temporisation animation
		bsr.l	bob32_init.struct
		bsr	shado.plus
		moveq	#0,d7
.exit		movem.l	(a7)+,d0-d4/a0/a2/a3/a4
		tst.l	d7
		rts

*-------------------------------------------------------------------------*
*            AJOUTE UNE SHADOW + RESERVE STRUCTURE SHADOW-BOB	    	  *
*-------------------------------------------------------------------------*
* Entrée	=> d0.w	numéro du Shadow-BOB de 0 à n
*		=> a5.l	adresse 't'
* Sortie	<= a1.l	adresse structure Shaodw-BOB
*		<= d7.l	#0 pas d'erreur
* utilise	(d0,d1,d2,d3,d4,d7 / a0,a1,a2,a3,a4,a5)
shadonly.ajoute:
		movem.l	d0-d4/a0/a2/a3/a4,-(a7)
		bsr.l	bob32_get.bob		|-> Cherche Shadow-BOB liste
		bne.s	.exit
		moveq	#0,d1			position x
		moveq	#0,d2			position y
		moveq	#0,d3			pointeur animation
		moveq	#1,d4			temporisation animation
		bsr.l	cherche_bob_free	|-> Cherche structure libre
		bne.s	.exit
		bsr.l	bob32_init.struct
		bsr	shado.plus
		moveq	#0,d7
.exit		movem.l	(a7)+,d0-d4/a0/a2/a3/a4
		tst.l	d7
		rts

*-------------------------------------------------------------------------*
*           AJOUTE UNE SHADOW-BOB + RESERVE STRUCTURE SHADOW-BOB	  *
*-------------------------------------------------------------------------*
* Entrée	=> d0.w	numéro du Shadow-BOB de 0 à n
*		=> d1.w	position x
*		=> d2.w	position y
*		=> d3.w	première position de 0 à n*8
*		=> d4.w	tempo animation
*		=> a5.l	adresse 't'
* Sortie	<= a1.l	adresse structure Shadow-BOB
*		<= d7.l	#0 pas d'erreur
shadonly.ajoute_bob:
		movem.l	d0-d4/a0/a2/a3/a4,-(a7)
		bsr.l	bob32_get.bob		|-> Cherche BOB dans liste
		bne.s	.exit
		bsr.l	cherche_bob_free	|-> Cherche structure libre
		bne.s	.exit
		bsr.l	bob32_init.struct
		bsr	shado.plus
		moveq	#0,d7
.exit		movem.l	(a7)+,d0-d4/a0/a2/a3/a4
		tst.l	d7
		rts

*-----------* Ajoute une Shadow-BOB dans la liste d'affichage *-----------*
* Entrée	=> a0.l	Pt/Le Shadow-BOB dans las liste Shadow-BOB
*		=> a1.l	Structure Shadow-BOB
* 		=> a5.l	Adr. 't'
* Sortie	<= a0.l	Pt/Le Shadow-BOB dans las liste Shadow-BOB
* 		<= a1.l	adresse structure BOB
* Modifie (d0/d1/d7/a2/a3/a4)
shado.plus:
		* cherche s'il existe déjà un bob avec la même fenêtre blitter *
		move.w	(TBOB_WINDOW2,a0),d1	d1=fenêtre blitter du bob
		lea	(shado_pt16-t,a5),a2
		move.w	(TBOB_WIDTH,a0),d7	d7=largeur du bob en mots
		subq.w	#1,d7
		lsl.w	#3,d7			*8
		add.w	d7,a2
		move.l	(4,a2),a3		a3=table des fenêtres blitter
		move.w	(a3)+,d7		d7=nombre de fenêtre blitter
		beq.s	.cree_new_win
		subq.w	#1,d7
.loop_cherche	cmp.w	(a3),d1			bonne fenêtre blitter ?
		beq.s	.trouve
		lea	(10,a3),a3
		dbf	d7,.loop_cherche

		* la fenêtre blitter n'exite pas *
.cree_new_win	move.l	(4,a2),a4		a4=table des fenêtres blitter
		addq.w	#1,(a4)			une fenêtre blitter de plus
		move.w	d1,(a3)+		sauve nouvelle fenêtre blitter
		move.l	(a2),a4			pt/fin liste d'affichage
		move.w	d1,(a4)+		nvelle fenêtre blitter
		move.l	a4,(a3)+		sauve pt/début struct. bob+2
		clr.w	(a4)+			nbre de bob-1
		move.l	a1,(a4)+		adresse structure bob
		clr.w	(a4)			0=fin de la liste affichage
		move.l	a4,(a2)			sauve pt/fin liste d'affichage
		move.l	a4,(a3)			sauve pt/fin last struct. bob
		bra.s	.exit

		* rajoute un bob dans la liste d'affichage *
.trouve		move.l	(2,a3),a4		pt/début struct. bob+2
		addq.w	#1,(a4)			nbre de bob+1
		move.l	(a2),d0			d0=adr. fin liste d'affichage
		sub.l	(6,a3),d0		d0=nbre d'octets à déplacer
		beq.s	.nul
		lsr.w	#2,d0
		subq.w	#1,d0
		move.l	a3,d3
		move.l	(a2),a3			a3=adr. fin liste d'affichage
		lea	(4,a3),a4
.loop_copy	move.l	-(a3),-(a4)
		dbf	d0,.loop_copy
		move.l	d3,a3
.nul		move.l	(6,a3),a4		fin liste struct. bob
		move.l	a1,(a4)			sauve adr. nvelle struct. bob

		* corrige les autres pointeurs *
		move.l	(2,a3),d0		pt/nbre de bob liste d'aff.
		move.l	(4,a2),a4		a4=table des fenêtres blitter
		move.w	(a4)+,d7		d7=nombre de fenêtre blitter
		addq.l	#2,a4
		subq.w	#1,d7
		moveq	#4,d1
.loopcorrect	cmp.l	(a4),d0			pt est avant celui qui à changé?
		bge.s	.nextval		-> oui, il ne bouge pas
		add.l	d1,(a4)+
		add.l	d1,(a4)+
		addq.l	#2,a4
		dbf	d7,.loopcorrect
		bra.s	.okc
.nextval	lea	(10,a4),a4
		dbf	d7,.loopcorrect
.okc		addq.l	#4,(6,a3)		inc. pt/dernier bob
		addq.l	#4,(a2)			inc. pt/fin liste d'aff.
.exit		rts
