*||*****************\\*****************************\\*******************||*
*||*****************// AJOUTE UN SSHADOW A L'ECRAN //*******************||*
*||*****************\\*****************************\\*******************||*
*									  *
* "ssha.only" 		=> Init. Struct. + Ajoute ssha
* "ssha.ajoute"		=> Réserve & Init. Struct. + Ajoute ssha
* "ssha.ajoute_bob"	=> Réserve & Init. Struct. avec paramètres + Ajoute ssha
* "ssha.plus"		=> Ajoute Structure ssha dans la liste d'affichage
*									  *
*-------------------------------------------------------------------------*
**************** Ajoute un ssha dans la liste d'affichage *****************
*-------------------------------------------------------------------------*
* Entrée	=> d0.l	Numéro du ssha
*		=> a1.l	Structure ssha
*		=> a5.l	adresse 't'
* Sortie	<= a1.l	adresse structure ssha
*		<= d7.l	#0 pas d'erreur
ssha.only:
		movem.l	d0-d4/a0/a2/a3/a4,-(a7)
		bsr	sbob.get
		bne.s	.exit
		moveq	#0,d1			position x
		moveq	#0,d2			position y
		moveq	#0,d3			pointeur animation
		moveq	#1,d4			temporisation animation
		bsr	sbob_init.struct
		bsr	ssha_plus
		moveq	#0,d7
.exit		movem.l	(a7)+,d0-d4/a0/a2/a3/a4
		tst.l	d7
		rts

*-------------------------------------------------------------------------*
*                  AJOUTE UN SSHA + RESERVE STRUCTURE SSHA	    	  *
*-------------------------------------------------------------------------*
* Entrée	=> d0.w	numéro du ssha de 0 à n
*		=> a5.l	adresse 't'
* Sortie	<= a1.l	adresse structure ssha
*		<= d7.l	#0 pas d'erreur
* utilise	(d0,d1,d2,d3,d4,d7 / a0,a1,a2,a3,a4,a5)
ssha.ajoute:
		movem.l	d0-d4/a0/a2/a3/a4,-(a7)
		bsr	sbob.get
		bne.s	.exit
		bsr	cherche_sbob_free
		bne.s	.exit
		moveq	#0,d1			position x
		moveq	#0,d2			position y
		moveq	#0,d3			pointeur animation
		moveq	#1,d4			temporisation animation
		bsr	sbob_init.struct
		bsr	ssha_plus
		moveq	#0,d7
.exit		movem.l	(a7)+,d0-d4/a0/a2/a3/a4
		tst.l	d7
		rts

*-------------------------------------------------------------------------*
*                  AJOUTE UN ssha + RESERVE STRUCTURE ssha	    	  *
*-------------------------------------------------------------------------*
* Entrée	=> d0.w	numéro du ssha de 0 à n
*		=> d1.w	position x
*		=> d2.w	position y
*		=> d3.w	première position de 0 à n*8
*		=> d4.w	tempo animation
*		=> a5.l	adresse 't'
* Sortie	<= a1.l	adresse structure ssha
*		<= d7.l	#0 pas d'erreur
ssha.ajoute_bob:
		movem.l	d0-d4/a0/a2/a3/a4,-(a7)
		bsr	sbob.get
		bne.s	.exit
		bsr	cherche_sbob_free
		bne.s	.exit
		bsr	sbob_init.struct
		bsr	ssha_plus
.exit		moveq	#0,d7
		movem.l	(a7)+,d0-d4/a0/a2/a3/a4
		tst.l	d7
		rts

*---------------* Ajoute un ssha dans la liste d'affichage *--------------*
* Entrée	=> a0.l	Pt/Le ssha dans las liste ssha
*		=> a1.l	Structure ssha
* 		=> a5.l	Adr. 't'
* Sortie	<= a0.l	Pt/Le ssha dans las liste ssha
* 		<= a1.l	adresse structure ssha
* Modifie (d0/d1/d7/a2/a3/a4)
ssha_plus:
		* cherche s'il existe déjà un ssha avec la même "last_table" *
		move.l	(TSBOB_LASTSHADO,a0),d1	d1=last_table du SBOB
		lea	(ssha_pt16-t,a5),a2
		move.w	(TSBOB_WIDTH,a0),d7	d7=largeur du SBOB en mots
		subq.w	#1,d7
		lsl.w	#3,d7			*8
		add.w	d7,a2
		move.l	(4,a2),a3		(ssha_pt??+4)=table LT
		move.l	(a3)+,d7		{ssha_pt+4} nbre de LT
		beq.s	.cree_new_win
		subq.w	#1,d7
.loop_cherche	cmp.l	(a3),d1			{ssha_pt+4} bonne LT ?
		beq.s	.trouve			-> oui
		lea	(SBOB_PTSIZE,a3),a3
		dbf	d7,.loop_cherche

		* la "last_table" n'existe pas *
.cree_new_win	move.l	(4,a2),a4	(ssha_pt??+4)=table LT
		addq.l	#1,(a4)		{ssha_pt+4} nbre LT + une
		move.l	d1,(a3)+	{ssha_pt+4} sauve nouvelle LT
		move.l	(a2),a4		(ssha_pt??)=>"ssha_width??" fin liste
		move.l	d1,(a4)+	{ssha_width??} nouvelle LT
		move.l	a4,(a3)+	{ssha_pt+4} début liste pour même LT
		clr.l	(a4)+		{ssha_width??} nbre de ssha-1
		move.l	a1,(a4)+	{ssha_width??} adresse structure ssha
		clr.l	(a4)		{ssha_width??} fin liste affichage
		move.l	a4,(a2)		(ssha_pt??) sauve fin liste affichage
		move.l	a4,(a3)		{ssha_pt+4} sauve fin last struct. bob
		bra.s	.exit

		* rajoute un ssha dans la liste d'affichage *
.trouve		move.l	(4,a3),a4	{ssha_pt+4}={ssha_width} début liste aff.
		addq.l	#1,(a4)		{ssha_width} nbre de sshas+1
		move.l	(a2),d0		(ssha_pt??)=>"ssha_width??" fin liste d'affichage
		sub.l	(8,a3),d0	{ssha_pt+4} nbre d'octets à déplacer
		beq.s	.nul
		lsr.w	#2,d0
		subq.w	#1,d0
		move.l	a3,d3
		move.l	(a2),a3			a3=adr. fin liste d'affichage
		lea	(4,a3),a4
.loop_copy	move.l	-(a3),-(a4)
		dbf	d0,.loop_copy
		move.l	d3,a3
.nul		move.l	(8,a3),a4	{ssha_pt+4} fin liste struct. bob
		move.l	a1,(a4)		sauve adresse nouvelle structure ssha

		* corrige les autres pointeurs *
		move.l	(4,a3),d0	{ssha_pt+4} pt/nbre de bob liste d'aff.
		move.l	(4,a2),a4	(ssha_pt??+4) table LT
		move.l	(a4)+,d7	{ssha_pt+4} nbre de LT
		addq.l	#4,a4		saute LT
		subq.l	#1,d7
		moveq	#4,d1
.loopcorrect	cmp.l	(a4),d0			pt est avant celui qui à changé?
		bge.s	.nextval		-> oui, il ne bouge pas
		add.l	d1,(a4)+
		add.l	d1,(a4)+
		addq.l	#4,a4			saute "last_table"
		dbf	d7,.loopcorrect
		bra.s	.okc
.nextval	lea	(SBOB_PTSIZE,a4),a4
		dbf	d7,.loopcorrect
.okc		addq.l	#4,(8,a3)	{ssha_pt+4} inc. pt/dernier bob
		addq.l	#4,(a2)		(ssha_pt??) inc. pt/fin liste d'aff.
.exit		rts
