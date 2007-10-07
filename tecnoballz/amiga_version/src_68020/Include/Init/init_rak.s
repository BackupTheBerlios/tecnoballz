*-------------------------------------------------------------------------*
****************** INITIALISATION DES RAQUETTES-BUMPERS *******************
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
init_rak:
		move.w	#YINVERS,(invy-t,a5)
		bsr	init_bump
		bne.s	.exit
		bsr	reko_bump
		bne.s	.exit
		moveq	#BOB.INVERS,d0
		lea	(ssinv-t,a5),a3
		bsr	supersprite
		bne.s	.exit
		bsr	init.robot
		bne.s	.exit
		clr.w	(rak.inver-t,a5)	supprime inverseur
		move.w	#8,(rak.haut-t,a5)	raquette 8 lignes en hauteur
		moveq	#0,d7
.exit		rts

*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
*-------------------* Initialise les structures BUMP *--------------------*
*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
* Entrée	=> a5.l	adr. 't'
init_bump:
		*/ Efface structure BUMP */
		lea	(b1-t,a5),a0
		move.w	(1*BUMP.SIZE+BUMP.ACTIF,a0),d2
		move.w	(2*BUMP.SIZE+BUMP.ACTIF,a0),d3
		move.w	(3*BUMP.SIZE+BUMP.ACTIF,a0),d4
		move.l	#4*BUMP.SIZE,d0
		bsr	clear.memory
		lea	(b1-t,a5),a0
		move.w	d2,(1*BUMP.SIZE+BUMP.ACTIF,a0)
		move.w	d3,(2*BUMP.SIZE+BUMP.ACTIF,a0)
		move.w	d4,(3*BUMP.SIZE+BUMP.ACTIF,a0)

		*/ Centre les raquettes */
		move.w	#BP_MAX-BP_MIN,d6	ligne de déplacement
		sub.w	(rak.large-t,a5),d6	- largeur raquette
		lsr.w	#1,d6			/2
		add.w	#32,d6

		*/ Raquette basse <1> */
		lea	(b1-t,a5),a4
		move.w	#1,(BUMP.NUMBER,a4)
		moveq	#BOB.HRAK,d0
		bsr	shadonly.ajoute
		bne	.exit
		move.l	#'BOB1',(BOB.ACTIF,a1)
		move.l	a1,(BUMP.BOB,a4)
		move.w	d6,(BOB.POSX,a1)
		move.w	#YRAKB,(BOB.POSY,a1)
		move.w	d6,(BUMP.X,a4)
		move.w	#YRAKB,(BUMP.Y,a4)
		lea	(midi01_left-t,a5),a0
		move.l	a0,(BUMP.RBLEFT,a4)
		lea	(midi01_right-t,a5),a0
		move.l	a0,(BUMP.RBRIGHT,a4)
		move.w	#2,(BUMP.NORMAL,a4)
		clr.w	(BUMP.FIRE,a4)
		clr.w	(BUMP.GLUE,a4)
		move.w	#1,(BUMP.ACTIF,a4)	Raquette bas toujours active
		lea	cap_bas,a0
		move.l	a0,(BUMP.MONNAIE,a4)
		lea	gad_bas,a0
		move.l	a0,(BUMP.GADGET,a4)
		move.w	#00,(BUMP.FX0,a4)
		move.w	#-5,(BUMP.FY0,a4)
		move.w	#-1,(BUMP.FX1,a4)
		move.w	#-4,(BUMP.FY1,a4)
		move.w	#+2,(BUMP.TFIRE,a4)
		move.w	#+1,(BUMP.FX2,a4)
		move.w	#-4,(BUMP.FY2,a4)
		move.w	#32,(BUMP.XSCIE,a4)
		move.w	#-20,(BUMP.YSCIE,a4)
		move.w	#000,(BUMP.XDECA,a4)
		move.w	#-10,(BUMP.YDECA,a4)
		move.l	(pt_objet-t,a5),a0
		move.l	a0,(BUMP.OBJET,a4)
		
		*/ Raquette Droite <2> */
		lea	(b2-t,a5),a4
		move.w	#2,(BUMP.NUMBER,a4)
		moveq	#BOB.VRAK,d0
		bsr	shadonly.ajoute
		bne	.exit
		clr.l	(BOB.ACTIF,a1)
		move.l	a1,(BUMP.BOB,a4)
		move.w	#XRAKD,(BOB.POSX,a1)
		move.w	d6,(BOB.POSY,a1)
		move.w	#XRAKD,(BUMP.X,a4)
		move.w	d6,(BUMP.Y,a4)
		lea	(midi02_left-t,a5),a0
		move.l	a0,(BUMP.RBLEFT,a4)
		lea	(midi02_right-t,a5),a0
		move.l	a0,(BUMP.RBRIGHT,a4)
		move.w	#2,(BUMP.NORMAL,a4)
		clr.w	(BUMP.FIRE,a4)
		clr.w	(BUMP.GLUE,a4)
		tst.w	(BUMP.ACTIF,a4)
		beq.s	.you_two2
		subq.w	#1,(BUMP.ACTIF,a4)
		beq.s	.you_two2
		move.l	#'BOB1',(BOB.ACTIF,a1)
.you_two2	lea	cap_est,a0
		move.l	a0,(BUMP.MONNAIE,a4)
		lea	gad_est,a0
		move.l	a0,(BUMP.GADGET,a4)
		move.w	#-5,(BUMP.FX0,a4)
		move.w	#00,(BUMP.FY0,a4)
		move.w	#-4,(BUMP.FX1,a4)
		move.w	#01,(BUMP.FY1,a4)
		move.w	#02,(BUMP.TFIRE,a4)
		move.w	#-4,(BUMP.FX2,a4)
		move.w	#-1,(BUMP.FY2,a4)
		move.w	#-20,(BUMP.XSCIE,a4)
		move.w	#032,(BUMP.YSCIE,a4)
		move.w	#-10,(BUMP.XDECA,a4)
		move.w	#000,(BUMP.YDECA,a4)
		move.l	(pt_objet-t,a5),a0
		lea	(NTIR*FIRE.SIZE,a0),a0
		move.l	a0,(BUMP.OBJET,a4)

		*/ Raquette Haute <3> */
		lea	(b3-t,a5),a4
		move.w	#3,(BUMP.NUMBER,a4)
		moveq	#BOB.HRAK,d0
		bsr	shadonly.ajoute
		bne	.exit
		clr.l	(BOB.ACTIF,a1)
		move.l	a1,(BUMP.BOB,a4)
		move.w	d6,(BOB.POSX,a1)
		move.w	#YRAKH,(BOB.POSY,a1)
		move.w	d6,(BUMP.X,a4)
		move.w	#YRAKH,(BUMP.Y,a4)
		lea	(midi03_left-t,a5),a0
		move.l	a0,(BUMP.RBLEFT,a4)
		lea	(midi03_right-t,a5),a0
		move.l	a0,(BUMP.RBRIGHT,a4)
		move.w	#2,(BUMP.NORMAL,a4)
		clr.w	(BUMP.FIRE,a4)
		clr.w	(BUMP.GLUE,a4)
		tst.w	(BUMP.ACTIF,a4)
		beq.s	.you_two3
		subq.w	#1,(BUMP.ACTIF,a4)
		beq.s	.you_two3
		move.l	#'BOB1',(BOB.ACTIF,a1)
.you_two3	lea	cap_haut,a0
		move.l	a0,(BUMP.MONNAIE,a4)
		lea	gad_haut,a0
		move.l	a0,(BUMP.GADGET,a4)
		move.w	#00,(BUMP.FX0,a4)
		move.w	#05,(BUMP.FY0,a4)
		move.w	#01,(BUMP.FX1,a4)
		move.w	#04,(BUMP.FY1,a4)
		move.w	#02,(BUMP.TFIRE,a4)
		move.w	#-1,(BUMP.FX2,a4)
		move.w	#04,(BUMP.FY2,a4)
		move.w	#32,(BUMP.XSCIE,a4)
		move.w	#24,(BUMP.YSCIE,a4)
		move.w	#00,(BUMP.XDECA,a4)
		move.w	#10,(BUMP.YDECA,a4)
		move.l	(pt_objet-t,a5),a0
		lea	(2*NTIR*FIRE.SIZE,a0),a0
		move.l	a0,(BUMP.OBJET,a4)

		*/ Raquette Gauche <4> */
		lea	(b4-t,a5),a4
		move.w	#4,(BUMP.NUMBER,a4)
		moveq	#BOB.VRAK,d0
		bsr	shadonly.ajoute
		bne	.exit
		clr.l	(BOB.ACTIF,a1)
		move.l	a1,(BUMP.BOB,a4)
		move.w	#XRAKG,(BOB.POSX,a1)
		move.w	d6,(BOB.POSY,a1)
		move.w	#XRAKG,(BUMP.X,a4)
		move.w	d6,(BUMP.Y,a4)
		lea	(midi04_left-t,a5),a0
		move.l	a0,(BUMP.RBLEFT,a4)
		lea	(midi04_right-t,a5),a0
		move.l	a0,(BUMP.RBRIGHT,a4)
		move.w	#2,(BUMP.NORMAL,a4)
		clr.w	(BUMP.FIRE,a4)
		clr.w	(BUMP.GLUE,a4)
		tst.w	(rak.team-t,a5)
		beq.s	.rodo
		move.w	#3,(BUMP.ACTIF,a4)
.rodo		tst.w	(BUMP.ACTIF,a4)
		beq.s	.you_two4
		subq.w	#1,(BUMP.ACTIF,a4)
		beq.s	.you_two4
		move.l	#'BOB1',(BOB.ACTIF,a1)
.you_two4	lea	cap_ouest,a0
		move.l	a0,(BUMP.MONNAIE,a4)
		lea	gad_ouest,a0
		move.l	a0,(BUMP.GADGET,a4)
		move.w	#05,(BUMP.FX0,a4)
		move.w	#00,(BUMP.FY0,a4)
		move.w	#04,(BUMP.FX1,a4)
		move.w	#01,(BUMP.FY1,a4)
		move.w	#02,(BUMP.TFIRE,a4)
		move.w	#04,(BUMP.FX2,a4)
		move.w	#-1,(BUMP.FY2,a4)
		move.w	#24,(BUMP.XSCIE,a4)
		move.w	#32,(BUMP.YSCIE,a4)
		move.w	#10,(BUMP.XDECA,a4)
		move.w	#00,(BUMP.YDECA,a4)
		move.l	(pt_objet-t,a5),a0
		lea	(3*NTIR*FIRE.SIZE,a0),a0
		move.l	a0,(BUMP.OBJET,a4)

		moveq	#0,d7
.exit		rts

*--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:-*
***********************Convetit Raquettes -> Sprites***********************
*--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:--:-*
* Entrée	=> a5.l	adr. 't'
reko_bump:
		*/ Décompacte la page graphique 448*96*4 (21504 octets) */
		sub.l	a2,a2
		lea	(rak01.pp-t,a5),a3
		move.w	(hasard-t,a5),d1
		and.w	#$0001,d1
		beq.s	.sqs
		lea	(rak02.pp-t,a5),a3
.sqs		lea	(rak.pagegfx-t,a5),a4
		bsr	decode.pagepp
		bne	.exit

		*/ Réserve mémoire pour les 112 sprites raquettes */
		lea	(rak.pagegfx-t,a5),a3
		lea	(rak.sprite-t,a5),a4
		move.l	(RESA.SIZ,a3),(RESA.SIZ,a4)
		bsr	reserve.mem
		bne	.exit

		*/ Calcul adresses des 112 sprites 4 couleurs */
		lea	(rocky-t,a5),a1		Adr. lutin raquette horizontales
		lea	(rak.sprite-t,a5),a0
		move.l	(RESA.ADR,a0),a0
		moveq	#55,d0
.hrocky		move.l	a0,(a1)+
		lea	(16*8,a0),a0		16*8*56 = 7168
		dbf	d0,.hrocky
		lea	(rocky2-t,a5),a1	Adr. lutin raquette verticales
		moveq	#55,d0
.vrocky		move.l	a0,(a1)+
		lea	(4*64,a0),a0		04*64*56= 14336
		dbf	d0,.vrocky
						* TOTAL = 21506
		*/ Recopie les raquettes horizontales */
		lea	(rocky-t,a5),a4		table adr.  lutin raquette
		lea	(rak.pagegfx-t,a5),a3
		move.l	(RESA.ADR,a3),a3
		move.w	(PG_SPLAN,a3),d1
		move.w	(PG_SLINE,a3),d2
		move.w	d2,d5
		mulu	#8,d5
		sub.w	d1,d5
		lea	(PG_DATA,a3),a2		
		moveq	#8-1,d0			Hauteur des raquettes
		moveq	#4-1,d4			4 rangées horz.  de raqu.
.rockabilly	moveq	#7-1,d3			7 raqu. par rangées horz.
.rockar		move.l	(a4)+,a0
		move.l	(a4)+,a1
		bsr	copylutin16.3
		addq.l	#8,a2
		dbf	d3,.rockar
		add.l	d5,a2
		dbf	d4,.rockabilly

		*/ Recopie les raquettes verticales */
		lea	(rocky2-t,a5),a4
		lea	(rak.pagegfx-t,a5),a3
		move.l	(RESA.ADR,a3),a3
		move.w	d2,d5			Taille d'une ligne
		mulu	#32,d5			Saute raquettes horizontales
		lea	(PG_DATA,a3,d5),a2		
		moveq	#63,d0			Hauteur des raquettes-1
		moveq	#27,d3			28 raquettes verticales
.zockazilly	move.l	(a4)+,a0
		move.l	(a4)+,a1
		bsr	copylutin16.0
		addq.l	#2,a2
		dbf	d3,.zockazilly

		*/ Libère la page graphique */
		lea	(rak.pagegfx-t,a5),a0
		bsr	libere_mem

		*/ Réserve liste lutins */
		lea	(rak.lutin-t,a5),a4
		move.l	#(1+8+1+64+1+8+1+8+1)*16*2,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne	.exit
		*/ Sauve adresse des listes lutins 0 & 1 */
		lea	(rak.lutin-t,a5),a4
		move.l	(RESA.ADR,a4),d0
		lea	(table_lutins-t,a5),a0
		move.l	d0,d1
		add.l	#(1+8+1+64+1+8+1+8+1)*16,d1
		move.l	d0,(a0)+
		move.l	d1,(a0)+
		move.l	d0,(rak.lutin0.1-t,a5)
		move.l	d1,(rak.lutin1.1-t,a5)

		*/ Réserve liste lutins 1 & 2 */
		lea	(rak.lutin2-t,a5),a4
		move.l	#(1+64+1+8+1)*16*2,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne	.exit
		*/ Sauve adresse des listes */
		lea	(rak.lutin2-t,a5),a4
		lea	(8+table_lutins-t,a5),a0
		move.l	(RESA.ADR,a4),d0
		move.l	d0,d1
		add.l	#(1+64+1+8+1)*16,d1
		move.l	d0,(a0)+
		move.l	d1,(a0)+
		move.l	d0,(rak.lutin2.1-t,a5)
		move.l	d1,(rak.lutin3.1-t,a5)
		add.l	#(1+64)*16,d0
		add.l	#(1+64)*16,d1
		move.l	d0,(rak.lutin2.2-t,a5)
		move.l	d1,(rak.lutin3.2-t,a5)
		moveq	#1,d0
		move.w	d0,(rak.allcopy-t,a5)
		moveq	#0,d7
.exit		rts


***********************  Initialise le lutin Robot  ***********************
init.robot:	
		*/ Réserve liste lutins */
		clr.w	(robot-t,a5)
		lea	(rak.lutinr-t,a5),a4
		move.l	#(1+8+1)*16*2,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne	.exit
		lea	(rak.lutinr-t,a5),a4
		move.l	(RESA.ADR,a4),d0
		lea	(table_lutins+16-t,a5),a0
		move.l	d0,d1
		add.l	#(1+8+1)*16,d1
		move.l	d0,(a0)+
		move.l	d1,(a0)+
		move.l	d0,(rak.lutin4-t,a5)
		move.l	d1,(rak.lutin5-t,a5)

		*/ Recopie GFX -> Lutin */
		moveq	#BOB.ROBOT,d0
		bsr.l	bob32_get.bob
		bne	.exit
		move.l	(TBOB_TABLE,a0),a2
		move.l	(bob32_pagegfx-t,a5),a3
		move.w	(PG_SPLAN,a3),d1
		move.w	(PG_SLINE,a3),d2
		moveq	#7,d0
		move.l	(rak.lutin4-t,a5),a0
		move.l	(rak.lutin5-t,a5),a1
		bsr.l	copytolutin16.3

		*/ Efface structure BUMP */
		lea	(br-t,a5),a0
		move.l	#BUMP.SIZE,d0
		bsr	clear.memory
		lea	(br-t,a5),a4
		moveq	#BOB.ROBOT,d0
		bsr	shadonly.ajoute
		bne	.exit
		clr.l	(BOB.ACTIF,a1)
		move.l	a1,(BUMP.BOB,a4)
		move.w	#YROBOT,d0
		move.w	d0,(BUMP.Y,a4)
		move.w	d0,(BOB.POSY,a1)
		lea	(midi01_left-t,a5),a0
		move.l	a0,(BUMP.RBLEFT,a4)
		lea	(midi01_right-t,a5),a0
		move.l	a0,(BUMP.RBRIGHT,a4)
		move.l	a0,(BUMP.REBOND,a4)
		moveq	#0,d7
.exit		rts
