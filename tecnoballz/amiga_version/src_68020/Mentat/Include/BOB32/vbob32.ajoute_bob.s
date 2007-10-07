*�x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��*
*������������������* Ajoute un Vertical-BOB � l'�cran *�������������������*
*�x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��x��*
* Entr�e	=> d0.w	num�ro du bob de 0 � n
*		=> a5.l	adresse 't'
* Sortie	<= a1.l	adresse structure bob
* "vbob32.only" 	=> Init. Struct. + Ajoute VBOB
* "vbob32.ajoute"	=> R�serve & Init. Struct. + Ajoute VBOB
* "vbob32.ajoute_bob"	=> R�serve & Init. Struct. avec param�tres + Ajoute VBOB
* "vbob32.plus"		=> Ajoute Structure VBOB dans la liste d'affichage
*									  *
*-------------------------------------------------------------------------*
**************** Ajoute un BOB dans la liste d'affichage ******************
*-------------------------------------------------------------------------*
* Entr�e	=> d0.l	Num�ro du BOB
*		=> a1.l	Structure BOB
*		=> a5.l	adresse 't'
* Sortie	<= a1.l	adresse structure bob
*		<= d7.l	#0 pas d'erreur
vbob32.only:
		movem.l	d0-d4/a0/a2/a3/a4,-(a7)
		bsr	bob32_get.bob
		bne.s	.exit
		moveq	#0,d1			position x
		moveq	#0,d2			position y
		moveq	#0,d3			pointeur animation
		moveq	#1,d4			temporisation animation
		bsr	bob32_init.struct
		bsr	vbob32.plus
		moveq	#0,d7
.exit		movem.l	(a7)+,d0-d4/a0/a2/a3/a4
		tst.l	d7
		rts

*-------------------------------------------------------------------------*
*                  AJOUTE UN BOB + RESERVE STRUCTURE BOB	    	  *
*-------------------------------------------------------------------------*
* Entr�e	=> d0.w	num�ro du bob de 0 � n
*		=> a5.l	adresse 't'
* Sortie	<= a1.l	adresse structure bob
*		<= d7.l	#0 pas d'erreur
* utilise	(d0,d1,d2,d3,d4,d7 / a0,a1,a2,a3,a4,a5)
vbob32.ajoute:
		movem.l	d0-d4/a0/a2/a3/a4,-(a7)
		bsr	bob32_get.bob		|-> Cherche BOB dans liste
		bne.s	.exit
		moveq	#0,d1			position x
		moveq	#0,d2			position y
		moveq	#0,d3			pointeur animation
		moveq	#1,d4			temporisation animation
		bsr	cherche_bob_free	|-> Cherche structure libre
		bne	.exit
		bsr	bob32_init.struct
		bsr	vbob32.plus
		moveq	#0,d7
.exit		movem.l	(a7)+,d0-d4/a0/a2/a3/a4
		tst.l	d7
		rts
*-------------------------------------------------------------------------*
*                  AJOUTE UN BOB + RESERVE STRUCTURE BOB	    	  *
*-------------------------------------------------------------------------*
* Entr�e	=> d0.w	num�ro du bob de 0 � n
*		=> d1.w	position x
*		=> d2.w	position y
*		=> d3.w	premi�re position de 0 � n*8
*		=> d4.w	tempo animation
*		=> a5.l	adresse 't'
* Sortie	<= a1.l	adresse structure bob
*		<= d7.l	#0 pas d'erreur
vbob32.ajoute_bob:
		movem.l	d0-d4/a0/a2/a3/a4,-(a7)
		bsr	bob32_get.bob		|-> Cherche BOB dans liste
		bne.s	.exit
		bsr	cherche_bob_free	|-> Cherche structure libre
		bne	.exit
		bsr	bob32_init.struct
		bsr	vbob32.plus
		moveq	#0,d7
.exit		movem.l	(a7)+,d0-d4/a0/a2/a3/a4
		tst.l	d7
		rts

*---------------* Ajoute un BOB dans la liste d'affichage *---------------*
* Entr�e	=> a0.l	Pt/Le BOB dans las liste BOB
*		=> a1.l	Structure BOB
* 		=> a5.l	Adr. 't'
* Sortie	<= a0.l	Pt/Le BOB dans las liste BOB
* 		<= a1.l	adresse structure BOB
* Modifie (d0/d1/d7/a2/a3/a4)
vbob32.plus:
		* cherche s'il existe d�j� un bob avec la m�me fen�tre blitter *
		move.w	(TBOB_WINDOW,a0),d1	d1=fen�tre blitter du bob
		subq.w	#1,d1
		lea	(vbob32_pt16-t,a5),a2
		move.w	(TBOB_WIDTH,a0),d7	d7=largeur du bob en mots
		subq.w	#1,d7
		lsl.w	#3,d7			*8
		add.w	d7,a2
		move.l	(4,a2),a3		a3=table des fen�tres blitter
		move.w	(a3)+,d7		d7=nombre de fen�tre blitter
		beq.s	.cree_new_win
		subq.w	#1,d7
.loop_cherche	cmp.w	(a3),d1			bonne fen�tre blitter ?
		beq.s	.trouve
		lea	(10,a3),a3
		dbf	d7,.loop_cherche

		* la fen�tre blitter n'exite pas *
.cree_new_win	move.l	(4,a2),a4		a4=table des fen�tres blitter
		addq.w	#1,(a4)			une fen�tre blitter de plus
		move.w	d1,(a3)+		sauve nouvelle fen�tre blitter
		move.l	(a2),a4			pt/fin liste d'affichage
		move.w	d1,(a4)+		nvelle fen�tre blitter
		move.l	a4,(a3)+		sauve pt/d�but struct. bob+2
		clr.w	(a4)+			nbre de bob-1
		move.l	a1,(a4)+		adresse structure bob
		clr.w	(a4)			0=fin de la liste affichage
		move.l	a4,(a2)			sauve pt/fin liste d'affichage
		move.l	a4,(a3)			sauve pt/fin last struct. bob
		bra.s	.exit

		* rajoute un bob dans la liste d'affichage *
.trouve		move.l	(2,a3),a4		pt/d�but struct. bob+2
		addq.w	#1,(a4)			nbre de bob+1
		move.l	(a2),d0			d0=adr. fin liste d'affichage
		sub.l	(6,a3),d0		d0=nbre d'octets � d�placer
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
		move.l	(4,a2),a4		a4=table des fen�tres blitter
		move.w	(a4)+,d7		d7=nombre de fen�tre blitter
		addq.l	#2,a4
		subq.w	#1,d7
		moveq	#4,d1
.loopcorrect	cmp.l	(a4),d0			pt est avant celui qui � chang�?
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
