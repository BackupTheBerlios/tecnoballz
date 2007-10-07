*||*******************\\*************************\\*********************||*
*||*******************// AJOUTE UN SBOB A L'ECRAN //********************||*
*||*******************\\*************************\\*********************||*
*									  *
* "sbob.only" 		=> Init. Struct. + Ajoute SBOB
* "sbob.ajoute"		=> Réserve & Init. Struct. + Ajoute SBOB
* "sbob.ajoute_bob"	=> Réserve & Init. Struct. avec paramètres + Ajoute SBOB
* "cherche_sbob_free"	=> Réserve une Structure SBOB
* "sbob_init.struct"	=> Init. Structure SBOB avec paramètres
* "sbob.plus"		=> Ajoute Structure SBOB dans la liste d'affichage
*									  *
*-------------------------------------------------------------------------*
**************** Ajoute un SBOB dans la liste d'affichage *****************
*-------------------------------------------------------------------------*
* Entrée	=> d0.l	Numéro du SBOB
*		=> a1.l	Structure SBOB
*		=> a5.l	adresse 't'
* Sortie	<= a1.l	adresse structure SBOB
*		<= d7.l	#0 pas d'erreur
sbob.only:
		movem.l	d0-d4/a0/a2/a3/a4,-(a7)
		bsr	sbob.get
		bne.s	.exit
		moveq	#0,d1			position x
		moveq	#0,d2			position y
		moveq	#0,d3			pointeur animation
		moveq	#1,d4			temporisation animation
		bsr	sbob_init.struct
		bsr	sbob_plus
		moveq	#0,d7
.exit		movem.l	(a7)+,d0-d4/a0/a2/a3/a4
		tst.l	d7
		rts

*-------------------------------------------------------------------------*
*                  AJOUTE UN SBOB + RESERVE STRUCTURE SBOB	    	  *
*-------------------------------------------------------------------------*
* Entrée	=> d0.w	numéro du SBOB de 0 à n
*		=> a5.l	adresse 't'
* Sortie	<= a1.l	adresse structure SBOB
*		<= d7.l	#0 pas d'erreur
* utilise	(d0,d1,d2,d3,d4,d7 / a0,a1,a2,a3,a4,a5)
sbob.ajoute:
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
		bsr	sbob_plus
		moveq	#0,d7
.exit		movem.l	(a7)+,d0-d4/a0/a2/a3/a4
		tst.l	d7
		rts

*-------------------------------------------------------------------------*
*                  AJOUTE UN SBOB + RESERVE STRUCTURE SBOB	    	  *
*-------------------------------------------------------------------------*
* Entrée	=> d0.w	numéro du SBOB de 0 à n
*		=> d1.w	position x
*		=> d2.w	position y
*		=> d3.w	première position de 0 à n*8
*		=> d4.w	tempo animation
*		=> a5.l	adresse 't'
* Sortie	<= a1.l	adresse structure SBOB
*		<= d7.l	#0 pas d'erreur
sbob.ajoute_bob:
		movem.l	d0-d4/a0/a2/a3/a4,-(a7)
		bsr	sbob.get
		bne.s	.exit
		bsr	cherche_sbob_free
		bne.s	.exit
		bsr	sbob_init.struct
		bsr	sbob_plus
.exit		moveq	#0,d7
		movem.l	(a7)+,d0-d4/a0/a2/a3/a4
		tst.l	d7
		rts

*»»»»»»»»»»»»»»»»»» On cherche une structure SBOB libre ««««««««««««««««««*
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d7.l #0 / Z=1 Pas d'erreur
*		<= a1.l Adresse de la structure SBOB 
cherche_sbob_free:
		move.w	#SBOB_MAXI-1,d7
		move.l	(sbob_struct-t,a5),a1
		lea	(SBOB.FREE,a1),a1
.cherche.bob	tst.w	(a1)			SBOB libre ?
		beq.s	.bobfree		-> oui
		lea	(SBOB.SIZE,a1),a1
		dbf	d7,.cherche.bob
		moveq	#-34,d7			¤ Plus de structure SBOB
		rts
.bobfree	not.w	(a1)			flag SBOB occupé (SBOB.FREE)
		lea	(-SBOB.FREE,a1),a1
		moveq	#0,d7
		rts

*------------------* Pointe un SBOB dans la liste SBOB *------------------*
* Entrée	=> d0.w Numéro du SBOB de 0 à n pas de 1
*		=> a5.l adr. 't'
* Sortie	<= a0.l	Pt/le SBOB
*		<= d7.l #0 si pas d'erreur
sbob.get:
		move.l	(sbob_liste-t,a5),a0
		addq.l	#4,a0			saute identificateur
		and.l	#$FFFF,d0
		cmp.l	(a0)+,d0		SBOB existe ?
		bhi.s	.no_sbob		-> non
		move.l	(a0,d0*4),a0		a0=adr. du SBOB dans liste SBOB
		moveq	#0,d7
		rts
.no_sbob:	move.l	(a0),a0
		moveq	#-33,d7			¤ N° de bob inexistant
		rts

*--------------------* Initialise une structure SBOB *--------------------*
* Entrée	=> d0.w	numéro du bob de 0 à n
*		=> d1.w	position x
*		=> d2.w	position y
*		=> d3.w	première position de 0 à n*8
*		=> d4.w	tempo animation
*		=> a0.l	Pt/Le SBOB dans las liste SBOB
*		=> a1.l	Structure SBOB
* 		=> a5.l	Adr. 't'
* Modifie (d7/a2/a3)
sbob_init.struct:
		move.l	a1,a2
		move.l	#'BOB1',(a2)+		flag affiche SBOB (SBOB.ACTIF)
		move.w	d1,(a2)+		pos. x du SBOB    (SBOB.POSX)
		move.w	d2,(a2)+		pos. y du SBOB    (SBOB.POSY)
		lea	(TSBOB_TABLE,a0),a3
		move.l	(a3,d3),(a2)+		adr. graphisme     (SBOB.GFX)
		move.l	(4,a3,d3),(a2)+		adr. masque        (SBOB.MSK)
		move.l	a0,(a2)+		adr. liste SBOB     (SBOB.PT)
		move.w	d0,(a2)			numéro du SBOB      (SBOB.NUM)
		lea	(SBOB.TEMPO,a1),a2
		move.w	d4,(a2)+		TEMPO (tempo en 50ieme de sec)
		move.w	d4,(a2)+		INITTEMPO (valeur init. tempo)
		move.w	d3,(a2)+		PTANIM (pt/animation de 8 en 8)
		move.w	(TSBOB_NUMBER,a0),d7	d7=nbre d'animation - 1
		lsl.w	#3,d7
		move.w	d7,(a2)+		PTMAXI (valeur maxi. de PTANIM)
		move.w	#8,(a2)+		OFFSET (offset PTANIM +8 ou -8)
		move.l	a3,(a2)			TABLE (table adr. gfx & msk)
		rts

*---------------* Ajoute un SBOB dans la liste d'affichage *--------------*
* Entrée	=> a0.l	Pt/Le SBOB dans las liste SBOB
*		=> a1.l	Structure SBOB
* 		=> a5.l	Adr. 't'
* Sortie	<= a0.l	Pt/Le SBOB dans las liste SBOB
* 		<= a1.l	adresse structure SBOB
* Modifie (d0/d1/d7/a2/a3/a4)
sbob_plus:
		* cherche s'il existe déjà un SBOB avec la même "last_table" *
		move.l	(TSBOB_LASTTABLE,a0),d1	d1=last_table du SBOB
		lea	(sbob_pt16-t,a5),a2
		move.w	(TSBOB_WIDTH,a0),d7	d7=largeur du SBOB en mots
		subq.w	#1,d7
		lsl.w	#3,d7			*8
		add.w	d7,a2
		move.l	(4,a2),a3		(sbob_pt??+4)=table LT
		move.l	(a3)+,d7		{sbob_pt+4} nbre de LT
		beq.s	.cree_new_win
		subq.w	#1,d7
.loop_cherche	cmp.l	(a3),d1			{sbob_pt+4} bonne LT ?
		beq.s	.trouve			-> oui
		lea	(SBOB_PTSIZE,a3),a3
		dbf	d7,.loop_cherche

		* la "last_table" n'existe pas *
.cree_new_win	move.l	(4,a2),a4	(sbob_pt??+4)=table LT
		addq.l	#1,(a4)		{sbob_pt+4} nbre LT + une
		move.l	d1,(a3)+	{sbob_pt+4} sauve nouvelle LT
		move.l	(a2),a4		(sbob_pt??)=>"sbob_width??" fin liste
		move.l	d1,(a4)+	{sbob_width??} nouvelle LT
		move.l	a4,(a3)+	{sbob_pt+4} début liste pour même LT
		clr.l	(a4)+		{sbob_width??} nbre de SBOB-1
		move.l	a1,(a4)+	{sbob_width??} adresse structure SBOB
		clr.l	(a4)		{sbob_width??} fin liste affichage
		move.l	a4,(a2)		(sbob_pt??) sauve fin liste affichage
		move.l	a4,(a3)		{sbob_pt+4} sauve fin last struct. bob
		bra.s	.exit

		* rajoute un SBOB dans la liste d'affichage *
.trouve		move.l	(4,a3),a4	{sbob_pt+4}={sbob_width} début liste aff.
		addq.l	#1,(a4)		{sbob_width} nbre de SBOBs+1
		move.l	(a2),d0		(sbob_pt??)=>"sbob_width??" fin liste d'affichage
		sub.l	(8,a3),d0	{sbob_pt+4} nbre d'octets à déplacer
		beq.s	.nul
		lsr.w	#2,d0
		subq.w	#1,d0
		move.l	a3,d3
		move.l	(a2),a3			a3=adr. fin liste d'affichage
		lea	(4,a3),a4
.loop_copy	move.l	-(a3),-(a4)
		dbf	d0,.loop_copy
		move.l	d3,a3
.nul		move.l	(8,a3),a4	{sbob_pt+4} fin liste struct. bob
		move.l	a1,(a4)		sauve adresse nouvelle structure SBOB

		* corrige les autres pointeurs *
		move.l	(4,a3),d0	{sbob_pt+4} pt/nbre de bob liste d'aff.
		move.l	(4,a2),a4	(sbob_pt??+4) table LT
		move.l	(a4)+,d7	{sbob_pt+4} nbre de LT
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
.okc		addq.l	#4,(8,a3)	{sbob_pt+4} inc. pt/dernier bob
		addq.l	#4,(a2)		(sbob_pt??) inc. pt/fin liste d'aff.
.exit		rts
