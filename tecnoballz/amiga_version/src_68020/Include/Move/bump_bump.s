*//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/*
************************ GESTION LUTINS RAQUETTES *************************
*//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/*
* Entrée	=> a5.l	Adr. 't'
bump_bump:
		bsr	keaupie
		bsr	okapy
		bsr	main_actor
		bsr	astrapy
		rts

*»»»»»»»»»»» Caclul les mots de contrôle des raquettes lutins ««««««««««««*
* Entrée	=> a5.l	Adr. 't'
main_actor
		move.l	(coordx-t,a5),d6
		move.l	(coordy-t,a5),d7
		lea	(b3-t,a5),a4		Raquette du haut
		move.l	(rak.lutin0.1-t,a5),a0
		move.l	(rak.lutin1.1-t,a5),a1
		moveq	#8,d2
		bsr.s	comicua
		lea	(b2-t,a5),a4		Raquette Droite
		move.l	(rak.lutin0.2-t,a5),a0
		move.l	(rak.lutin1.2-t,a5),a1
		move.w	(rak.large-t,a5),d2
		bsr.s	comicua
		lea	(b1-t,a5),a4		Raquette bas
		move.l	(rak.lutin0.3-t,a5),a0
		move.l	(rak.lutin1.3-t,a5),a1
		moveq	#8,d2
		bsr.s	comicua
		lea	(b4-t,a5),a4		Raquette Gauche
		move.l	(rak.lutin2.1-t,a5),a0
		move.l	(rak.lutin3.1-t,a5),a1
		move.w	(rak.large-t,a5),d2
comicua		moveq	#0,d0
		move.w	(BUMP.X,a4),d0
		tst.w	(BUMP.ACTIF,a4)
		bne.s	.ya
		move.w	#-64,d0
.ya		asl.w	#2,d0
		moveq	#0,d1
		move.w	(BUMP.Y,a4),d1
		bsr.l	calcul_mot_ctrl_aga
		move.w	d3,(8,a0)
		or.w	#$80,d3
		move.w	d3,(8,a1)
		swap	d3
		move.w	d3,(a0)	
		move.w	d3,(a1)

		*/ Raquette Robot */
		moveq	#0,d0
		lea	(br-t,a5),a4		Raquette Robot
		move.l	(BUMP.BOB,a4),a3
		move.w	#1,(BOB.ACTIF,a3)
		move.w	(BUMP.X,a4),d0
		move.w	d0,(BOB.POSX,a3)
		tst.w	(robot-t,a5)
		bne.s	.tutu77
		move.w	#-64,d0
		clr.w	(BOB.ACTIF,a3)
.tutu77		move.l	(rak.lutin4-t,a5),a0
		move.l	(rak.lutin5-t,a5),a1
		moveq	#8,d2
		asl.w	#2,d0
		moveq	#0,d1
		move.w	(BUMP.Y,a4),d1
		bsr.l	calcul_mot_ctrl_aga
		move.w	d3,(8,a0)
		or.w	#$80,d3
		move.w	d3,(8,a1)
		swap	d3
		move.w	d3,(a0)	
		move.w	d3,(a1)	
	
		rts

******************/ CACLUL POSITION CREDIT & INVERSEUR */******************
* Entrée	=> a5.l	Adr. 't'
astrapy:
		move.l	(rak.lutin0.4-t,a5),a0
		move.l	(rak.lutin1.4-t,a5),a1
		moveq	#24,d0
		moveq	#0,d1
		move.w	(invy-t,a5),d1
		cmp.w	#YINVERS,d1
		beq.s	.yax
		addq.w	#1,d1		
.yax		move.w	d1,(invy-t,a5)
		bsr.s	.barbes3
		move.l	(rak.lutin2.2-t,a5),a0
		move.l	(rak.lutin3.2-t,a5),a1
		moveq	#0,d0
		move.w	#-65,d0
		moveq	#0,d1
		move.w	#YINVERS,d1
		tst.w	(rak.inver-t,a5)	Inverseur en cours ?
		beq.s	.barbes3		-> non
		move.w	#215,d0
.barbes3	asl.w	#2,d0
		moveq	#8,d2
		bsr.l	calcul_mot_ctrl_aga
		move.w	d3,(8,a0)
		or.w	#$80,d3
		move.w	d3,(8,a1)
		swap	d3
		move.w	d3,(a0)	
		move.w	d3,(a1)	
		rts

***********************Recopie toutes les raquettes************************
* Entrée	=> a5.l	Adr. 't'
keaupie:	
		tst.w	(rak.allcopy-t,a5)
		beq	.exit
		move.w	(rak.large-t,a5),d6	d6=Largeur raquette
		sub.w	#16,d6

		*/ RAQUETTE DU HAUT */
		lea	(b3-t,a5),a4
		clr.w	(BUMP.COPY,a4)
		move.l	(rak.lutin0.1-t,a5),a2
		move.l	(rak.lutin1.1-t,a5),a3
		lea	(16,a2),a2
		lea	(16,a3),a3
		bsr	rocky.hpointe
		bsr	hh.scopy

		*/ RAQUETTE DE GAUCHE */
		lea	(b2-t,a5),a4
		clr.w	(BUMP.COPY,a4)
		move.l	a2,(rak.lutin0.2-t,a5)
		move.l	a3,(rak.lutin1.2-t,a5)
		lea	(16,a2),a2
		lea	(16,a3),a3
		bsr	rocky.vpointe
		bsr	vv.scopy

		*/ RAQUETTE DU BAS */
		lea	(b1-t,a5),a4
		clr.w	(BUMP.COPY,a4)
		move.l	a2,(rak.lutin0.3-t,a5)
		move.l	a3,(rak.lutin1.3-t,a5)
		lea	(16,a2),a2
		lea	(16,a3),a3
		bsr	rocky.hpointe
		bsr	hh.scopy
		move.l	a2,(rak.lutin0.4-t,a5)
		move.l	a3,(rak.lutin1.4-t,a5)
		lea	(9*16,a2),a2
		lea	(9*16,a3),a3
		moveq	#0,d0
		move.l	d0,(a2)+
		move.l	d0,(a2)+
		move.l	d0,(a2)+
		move.l	d0,(a2)
		move.l	d0,(a3)+
		move.l	d0,(a3)+
		move.l	d0,(a3)+
		move.l	d0,(a3)

		*/ RAQUETTE DE GAUCHE */
		lea	(b4-t,a5),a4
		clr.w	(BUMP.COPY,a4)
		move.l	(rak.lutin2.1-t,a5),a2
		move.l	(rak.lutin3.1-t,a5),a3
		lea	(16,a2),a2
		lea	(16,a3),a3
		bsr	rocky.vpointe
		bsr	vv.scopy
		move.l	a2,(rak.lutin2.2-t,a5)
		move.l	a3,(rak.lutin3.2-t,a5)
		lea	(9*16,a2),a2
		lea	(9*16,a3),a3
		moveq	#0,d0
		move.l	d0,(a2)+
		move.l	d0,(a2)+
		move.l	d0,(a2)+
		move.l	d0,(a2)
		move.l	d0,(a3)+
		move.l	d0,(a3)+
		move.l	d0,(a3)+
		move.l	d0,(a3)
		bsr.l	pub2
.exit		rts


**************************Recopie une raquette*****************************
* Entrée	=> a5.l	Adr. 't'
okapy:
		move.w	(rak.large-t,a5),d6	d6=Largeur raquette
		sub.w	#16,d6

		*/ RAQUETTE DU HAUT */
		lea	(b3-t,a5),a4
		tst.w	(BUMP.COPY,a4)
		beq.s	.nob3
		clr.w	(BUMP.COPY,a4)
		move.l	(rak.lutin0.1-t,a5),a2
		move.l	(rak.lutin1.1-t,a5),a3
		lea	(16,a2),a2
		lea	(16,a3),a3
		bsr	rocky.hpointe
		bsr	hh.scopy
.nob3
		*/ RAQUETTE DE GAUCHE */
		lea	(b2-t,a5),a4
		tst.w	(BUMP.COPY,a4)
		beq.s	.nob2
		clr.w	(BUMP.COPY,a4)
		move.l	(rak.lutin0.2-t,a5),a2
		move.l	(rak.lutin1.2-t,a5),a3
		lea	(16,a2),a2
		lea	(16,a3),a3
		bsr	rocky.vpointe
		bsr	vv.scopy
.nob2
		*/ RAQUETTE DU BAS */
		lea	(b1-t,a5),a4
		tst.w	(BUMP.COPY,a4)
		beq.s	.nob1
		clr.w	(BUMP.COPY,a4)
		move.l	(rak.lutin0.3-t,a5),a2
		move.l	(rak.lutin1.3-t,a5),a3
		lea	(16,a2),a2
		lea	(16,a3),a3
		bsr	rocky.hpointe
		bsr	hh.scopy
.nob1
		*/ RAQUETTE DE GAUCHE */
		lea	(b4-t,a5),a4
		tst.w	(BUMP.COPY,a4)
		beq.s	.nob4
		clr.w	(BUMP.COPY,a4)
		move.l	(rak.lutin2.1-t,a5),a2
		move.l	(rak.lutin3.1-t,a5),a3
		lea	(16,a2),a2
		lea	(16,a3),a3
		bsr	rocky.vpointe
		bsr	vv.scopy
.nob4
.exit		rts

****************** Donne Sprites raquettes horizontales *******************
* Entrée	=> d6.w pt/taille raquette
* 		=> a4.l	Structure BUMP
*		=> a5.l	Adr. 't'
* Sortie	<= a0.l	pt/Sprite paire
* 		<= a1.l pt/Sprite paire
* Modifie (d0)
rocky.hpointe:
		move.w	d6,d0			pt/taille raquette
		tst.w	(BUMP.FIRE,a4)
		beq.s	.nofire
		add.w	#7*8,d0
.nofire		tst.w	(BUMP.GLUE,a4)
		beq.s	.noglue
		add.w	#7*8*2,d0
.noglue		lea	(rocky-t,a5,d0),a1	table adresse lutin raquette
		move.l	(a1)+,a0
		move.l	(a1)+,a1
		rts

******************* Donne Sprites raquettes verticales ********************
* Entrée	=> d6.w pt/taille raquette
* 		=> a4.l	Structure BUMP
*		=> a5.l	Adr. 't'
* Sortie	<= a0.l	pt/Sprite paire
* 		<= a1.l pt/Sprite paire
* Modifie (d0)
rocky.vpointe:
		move.w	d6,d0			pt/taille raquette
		tst.w	(BUMP.FIRE,a4)
		beq.s	.nofire
		add.w	#7*8,d0
.nofire		tst.w	(BUMP.GLUE,a4)
		beq.s	.noglue
		add.w	#7*8*2,d0
.noglue		lea	(rocky2-t,a5,d0),a1	table adresse lutin raquette
		move.l	(a1)+,a0
		move.l	(a1)+,a1
		rts
****************Copie le Sprite de la raquette horizontale*****************
* Entrée	=> a0.l	Sprite paire
*		=> a1.l	Sprite impaire
*		=> a2.l	Liste lutin paire
*		=> a3.l	Liste lutin impaire
* Modifie (d0/a0-a3)
hh.scopy:
		moveq	#7,d0
.loop		move.l	(a0)+,(a2)+
		move.l	(a0)+,(a2)+
		move.l	(a0)+,(a2)+
		move.l	(a0)+,(a2)+
		move.l	(a1)+,(a3)+
		move.l	(a1)+,(a3)+
		move.l	(a1)+,(a3)+
		move.l	(a1)+,(a3)+
		dbf	d0,.loop
		rts
*****************Copie le Sprite de la raquette Verticale******************
* Entrée	=> a0.l	Sprite paire
*		=> a1.l	Sprite impaire
*		=> a2.l	Liste lutin paire
*		=> a3.l	Liste lutin impaire
* Modifie (d0/a0-a3)
vv.scopy:
		move.w	(rak.large-t,a5),d0
		subq.w	#1,d0
		moveq	#0,d1
.loop		move.w	(a0)+,(a2)+
		move.w	d1,(a2)+
		move.l	d1,(a2)+
		move.w	(a0)+,(a2)+
		move.w	d1,(a2)+
		move.l	d1,(a2)+
		move.w	(a1)+,(a3)+
		move.w	d1,(a3)+
		move.l	d1,(a3)+
		move.w	(a1)+,(a3)+
		move.w	d1,(a3)+
		move.l	d1,(a3)+
		dbf	d0,.loop
		rts
