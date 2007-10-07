*=========================================================================*
*------------------> Gère la barre témoin du GigaBlitz <------------------*
*=========================================================================*
* entrée	=> a5.l	adr. table 't'
chunky:
		tst.w	(blitz.haut-t,a5)	GigaBlitz en action ?
		bne	blitz.move		-> oui, déplace Blitz
		move.l	(copper_blitz-t,a5),a0
		lea	(10,a0),a0
		lea	(blitz_color.aga-t,a5),a1
		move.w	(blitz.count-t,a5),d2	1er ligne Blitz de 0 à 26
		beq.s	no_riviere		-> Maxi lance GigaBlitz
		moveq	#0,d0
		move.w	d2,d1
.raz_blitz2	move.w	d0,(a0)
		move.w	d0,(8,a0)
		lea	(20,a0),a0
		dbf	d1,.raz_blitz2
		moveq	#NLINE.BLITZ-1,d1
		sub.w	d2,d1
		beq	uncle_ben
		subq.w	#1,d1
.raz_blitz4	move.w	(a1)+,(a0)
		move.w	(a1)+,(8,a0)
		lea	(20,a0),a0
		dbf	d1,.raz_blitz4
		rts
*_________________________________________________________________________*
*----------->Témoin gigablitz au maxi. =>  GigaBlitz est lancé<-----------*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
* Entrée	=> a0.l	{copper_blitz}+10
*		=> a1.l	{blitz_color.aga}
*		=> a5.l	table 't'
no_riviere:
		* Affiche tous les couleurs du témoin *
		move.w	#TETE_RIRE1,(head.req-t,a5)
		move.b	#1,(sound+S_TIR.GARDIEN-t,a5)
		moveq	#NLINE.BLITZ-1,d1
.raz_blitz3	move.w	(a1)+,(a0)
		move.w	(a1)+,(8,a0)
		lea	(20,a0),a0
		dbf	d1,.raz_blitz3

		* raz les 8 anciennes adr. lignes lutins *
		lea	(blitz.sprline-t,a5),a0
		moveq	#0,d6
		move.l	d6,(a0)+		lutin 0 plan 1
		move.l	d6,(a0)+		lutin 0 plan 1
		move.l	d6,(a0)+		lutin 0 plan 2
		move.l	d6,(a0)+		lutin 0 plan 2
		move.l	d6,(a0)+		lutin 1 plan 1
		move.l	d6,(a0)+		lutin 1 plan 1
		move.l	d6,(a0)+		lutin 1 plan 2
		move.l	d6,(a0)			lutin 1 plan 2
		move.w	d6,(blitz.restore-t,a5)

		* lit données gigablitz correspondant à la raquette *
		move.w	(rak.large-t,a5),d7	Longeur raquette
		sub.w	#16,d7
		lea	(blitz.type-t,a5),a0
		move.w	(6,a0,d7),(blitz.haut-t,a5)
		lea	(blitz.list-t,a5),a0
		move.l	(0,a0,d7),(blitz.lutin0-t,a5)
		move.l	(4,a0,d7),(blitz.lutin1-t,a5)

		** initialise les coordonnées du blitz *
		lea	(BUMP.X+b1-t,a5),a0
		move.w	(a0)+,d2
		move.w	d2,(blitz.x-t,a5)
		move.w	(a0),(blitz.y-t,a5)

		*- calcul la fenêtre x de collison briks/gigablitz -*
		move.w	d2,d0			d0=X bord gauche
		move.w	d0,d1
		add.w	(rak.large-t,a5),d1	d1=Y bord droit
		move.w	(set.x-t,a5,d0*2),d0
		move.w	(set.x-t,a5,d1*2),d1
		sub.w	d0,d1
		move.w	d0,(blitz.settab-t,a5)	Sauve pt horz./set_table
		lsr.w	#2,d1
		and.w	#$000F,d2
		beq.s	.z2
		addq.w	#1,d1
.z2		move.w	d1,(blitz.nbrick-t,a5)	Sauve nbre briks à tester par ligne

*_________________________________________________________________________*
*                        Déplacement du GigaBlitz			  *
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
* entrée	=> a5.l	adr. table 't'
blitz.move:
		* calcul la hauteur du lutin (coupe le haut) *
		lea	(blitz.sprline-t,a5),a3	pt/1er ligne gfx lutin à restorer
		move.w	(blitz.restore-t,a5),d5	pt/ligne à restorer
		moveq	#0,d6
		moveq	#0,d1
		move.w	d6,(blitz.out-t,a5)	RAZ Flag Blitz sort raquette
		subq.w	#8,(blitz.y-t,a5)	Y=Y-8
		move.w	(blitz.y-t,a5),d1	d1=Pos. Y
		move.w	(blitz.haut-t,a5),d2
		cmp.w	#YMAX.BLITZ,d1		Y > YMAX ?
		bge.s	.parfum			-> oui
		sub.w	#YMAX.BLITZ,d1
		neg.w	d1			d1=MAX-Y
		cmp.w	d1,d2			dépassement ?
		bhi.s	.sandrine		-> non
		move.w	d6,(blitz.haut-t,a5)
		move.w	d6,(blitz.restore-t,a5)
		moveq	#0,d1
		move.w	#-70,(blitz.x-t,a5)
		bra	.ligamed
.sandrine	move.w	d1,d6			d6=MAX-Y
		lsl.w	#4,d6			*16 (16 octets par ligne)
		move.w	d6,(blitz.restore-t,a5)
		sub.w	d1,d2
		moveq	#YMAX.BLITZ,d1
		bra	.ligamed

		* calul la hauteur du lutin (coupe le bas) *
.parfum		sub.w	#YRAKB,d1
		neg.w	d1			distance haut g-b & raqu. bas
		cmp.w	d2,d1			distance suffisante ?
		bge.s	.marcel
		move.w	d1,d2
		move.w	#1,(blitz.out-t,a5)	Flag Blitz sort de la raquette

		* calcul mot de contrôle *
.marcel		move.w	(blitz.y-t,a5),d1
.ligamed	lea	(24+table_lutins-t,a5),a4	a4=Adr. table des 8 adresses
		move.l	(blitz.lutin0-t,a5),a0
		move.l	(00,a3),(00,a0,d5)	*/ Restore Gfx Lutin */
		move.l	(04,a3),(04,a0,d5)
		move.l	(08,a3),(08,a0,d5)
		move.l	(12,a3),(12,a0,d5)
		add.w	d6,a0
		move.l	a0,(a4)+
		move.l	(00,a0),(a3)+		*/ Sauve Gfx Lutin */
		move.l	(04,a0),(a3)+
		move.l	(08,a0),(a3)+
		move.l	(12,a0),(a3)+
		move.l	(blitz.lutin1-t,a5),a1
		move.l	(00,a3),(00,a1,d5)
		move.l	(04,a3),(04,a1,d5)
		move.l	(08,a3),(08,a1,d5)
		move.l	(12,a3),(12,a1,d5)
		add.w	d6,a1
		move.l	a1,(a4)
		move.l	(00,a1),(a3)+
		move.l	(04,a1),(a3)+
		move.l	(08,a1),(a3)+
		move.l	(12,a1),(a3)+
		move.w	(sin.blitz-t,a5),d0
		add.w	#100,d0
		and.w	#1022,d0
		move.w	d0,(sin.blitz-t,a5)
		moveq	#20,d3
		move.l	(pt_cos-t,a5),a0
		muls	(a0,d0),d3
		asr.l	#7,d3
		moveq	#0,d0
		move.w	(blitz.x-t,a5),d0
		add.w	d0,d0
		add.w	d0,d0
		add.w	d3,d0
		move.l	(coordx-t,a5),d6
		move.l	(coordy-t,a5),d7
		bsr.l	calcul_mot_ctrl_aga
		move.l	d3,(blitz.motctrl-t,a5)
		bra	uncle_tom
		rts

blitzalair:	move.l	(blitz.motctrl-t,a5),d3
		beq.s	.goto
		lea	(24+table_lutins-t,a5),a1	a1=Adr. table des 8 adresses
		move.l	(a1)+,a0
		move.l	(a1),a1
		move.w	d3,(8,a0)
		or.w	#$80,d3
		move.w	d3,(8,a1)
		swap	d3
		move.w	d3,(a0)+	2
		move.w	d3,(a1)+	
		moveq	#0,d0
		move.w	d0,(a0)+	4
		move.l	d0,(a0)+	8
		addq.w	#2,a0		10
		move.w	d0,(a0)+	12
		move.l	d0,(a0)+	16
		move.w	d0,(a1)+
		move.l	d0,(a1)+
		addq.w	#2,a1
		move.w	d0,(a1)+
		move.l	d0,(a1)+
.goto		rts
*_________________________________________________________________________*
;		jeu : collisions gigablitz/briks
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
* entrée	=> a5.l	adr. table 't'
uncle_tom:
		*- test collision megablitz/briques -*
		move.w	(blitz.nbrick-t,a5),d0	Nbre de briques à tester horz.
		beq	uncle_ben		-> Aucune
		subq.w	#1,d0
		lea	(2+set_table-t,a5),a0
		move.w	(blitz.y-t,a5),d1	d1=Coordonnée y du GigaBlitz
		add.w	(set.y-t,a5,d1*2),a0
		add.w	(blitz.settab-t,a5),a0
		moveq	#BRI.SIZE,d1
		move.w	#BRI.MSK,d2
		move.w	(brick.save-t,a5),d3
		lea	(bri_pnt-t,a5),a1
		lea	(a1,d3),a2
		move.w	#56*TLIGNEB,d4
		moveq	#1,d5
		moveq	#0,d6
		move.w	(set_guide-set_table,a0),d7
.mylene_loop	tst.w	(a0)			Il existe une brique ?
		beq.s	.m			-> Non
		move.w	#512,(BRI.X,a2)		Flag brique détruit au Blizt
		cmp.w	(a0),d4			Brique indestructible ?
		bhi.s	.jk			-> Oui
		move.w	#-1,(BRI.X,a2)		Youpi
.jk		move.w	d6,(a0)			RAZ code brique
		move.w	d6,(JMM,a0)		RAZ code brique
		move.w	d5,(BRI.FLAG,a2)	Flag efface brique
		move.w	(-2,a0),(BRI.NUM,a2)	Numéro de la brique
		move.w	d7,(BRI.ADR,a2)		adresse relative d'affichage
		add.w	d1,d3
		and.w	d2,d3
		lea	(a1,d3),a2
.m		addq.w	#2,d7			Adresse + 2
		addq.l	#4,a0			Pt + 4
		dbf	d0,.mylene_loop
		move.w	d3,(brick.save-t,a5)
uncle_ben	rts
