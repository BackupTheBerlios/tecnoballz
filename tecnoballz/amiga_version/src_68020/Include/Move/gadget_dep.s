*=========================================================================*
*-------------------->> Déplacement des Bonus/Malus <<--------------------*
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
gadget_dep:
		move.l	a6,-(a7)
		moveq	#NGADGET-1,d7
		move.l	(pt_gadget-t,a5),a6
.loop		tst.w	(GAD.ACTIF,a6)
		beq.s	.cdi01
		move.l	(GAD.DEP,a6),a0
		jsr	(a0)
.cdi01		lea	(GAD.SIZE,a6),a6
		dbf	d7,.loop
		move.l	(a7)+,a6
		rts

		*-=-* Déplacement en bas *-=-*
gad_bas		addq.w	#1,(GAD.Y,a6)
		cmp.w	#NLIGNE-10,(GAD.Y,a6)
		bge	gad_out
		lea	(b1-t,a5),a1
		move	(rak.large-t,a5),d4
		move	#8,d5
		bra.s	nescafe

		*-=-* Déplacement en haut *-=-*
gad_haut:	subq.w	#1,(GAD.Y,a6)
		cmp.w	#10,(GAD.Y,a6)
		bmi	gad_out
		lea	(b3-t,a5),a1
		move.w	(rak.large-t,a5),d4
		moveq	#8,d5
		bra.s	nescafe

		*-=-* Déplacement à droite *-=-*
gad_est:	addq.w	#1,(GAD.X,a6)
		cmp.w	#256-16,(GAD.X,a6)
		bge	gad_out
		lea	(b2-t,a5),a1
		moveq	#9,d4
		move.w	(rak.large-t,a5),d5
		bra.s	nescafe

		*-=-* Déplacement à gauche *-=-*
gad_ouest:	subq.w	#1,(GAD.X,a6)
		cmp.w	#3,(GAD.X,a6)
		bls	gad_out
		lea	(b4-t,a5),a1
		moveq	#9,d4
		move.w	(rak.large-t,a5),d5

		*-=-* Test collisions Gadget/Raquette *-=-*
nescafe		move.w	(BUMP.X,a1),d0		x raquette
		move.w	(BUMP.Y,a1),d1		y raquette
		move.w	(GAD.X,a6),d2		x capsule
		move.w	(GAD.Y,a6),d3		y capsule
		addq.w	#5,d2
		cmp.w	d0,d2
		blt	gad_fin
		addq.w	#7,d3
		cmp.w	d1,d3
		blt	gad_fin
		add.w	d4,d0
		move	(GAD.X,a6),d2
		cmp.w	d0,d2
		bgt	gad_fin
		add.w	d5,d1
		move.w	(GAD.Y,a6),d3
		cmp	d1,d3
		bgt	gad_fin
		clr.w	(GAD.ACTIF,a6)
		bsr	venus1
		bsr	venus1
		add.w	#20,(score.life-t,a5)	score +20
		move.w	(GAD.NUM,a6),d0
		jmp	([gad_jump1,pc,d0*2])
		CNOP	0,4
gad_jump1:	dc.l	gad_go		`	00 *inutilisé*
		dc.l	gad_colle	1	04 colle
		dc.l	voleur		2	08 level suivant
		dc.l	gad_fire1	3	12 tir puissance 1
		dc.l	gad_fire2	4	16 tir puissance 2
		dc.l	gad_taim	5	20 rétrécit la raquette
		dc.l	gad_taip	6	24 allonge la raquette
		dc.l	gad_viem	7	28 enlève une vie
		dc.l	gad_viep	8	32 ajoute une vie
		dc.l	gad_2ball	9	36 ajoute 2 balles
		dc.l	gad_3ball	0	40 ajoute 3 balles
		dc.l	gad_destr1	°	44 balle puissance 1
		dc.l	gad_destr2	_	48 balle puissance 2
		dc.l	gad_inverse	|	52 inverseur
		dc.l	0			56 *inutilisé*
		dc.l	gad_speed	0-NUM	60 vitesse balle maxi
		dc.l	gad_bump1	A	64
		dc.l	gad_bump2	Z	68
		dc.l	gad_bump3	E	72
		dc.l	gad_bump4	R	76
		dc.l	gad_size1	T	80 balle taille 2
		dc.l	gad_size2	Y	84 balle taille 3
		dc.l	gad_random	U	88 au hasard
		dc.l	gad_mega	I	92 toutes options
		dc.l	gad_prixone	O	96 Prix magasin à 1
		dc.l	gad_mur		P	100 Mur actif
		dc.l	gad_robot	ê	110 Robot
		dc.l	gad_controlator	$	114 Contrôle balles
		dc.l	gad_oeil		118 Active l'oeil

		*-=- bonus/malus sort de l'écran -=-*
gad_out:	clr.w	(GAD.ACTIF,a6)
gad_fin		rts
