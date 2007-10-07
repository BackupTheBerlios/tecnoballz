*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
*************** Calcul les deux mots de conrôle d'un lutin ****************
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
* Entrée	=> d0.l	Pos. X du lutin
*		=> d1.l	Pos. Y du lutin
*		=> d2.w	Hauteur du lutin
*		=> d6.w	Pos. X fenêtre écran
*		=> d7.w	Pos. Y fenêtre écran
* Sortie	<= d3.l	Les 2 mots de contrôles
* Utilise	(d0/d1/d2/d3/d6/d7)
* Bit n° 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00 (MOT CONTROLE 1)
*	 E7 E6 E5 E4 E3 E2 E1 E0 H8 H7 H6 H5 H4 H3 H2 H1
* Bit n° 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00 (MOT CONTROLE 2)
*	 L7 L6 L5 L4 L3 L2 L1 L0 AT 00 00 00 00 E8 L8 H0
* bits H0-H8 Pos. horz. du lutin (HSTART)
* bits E0-E8 Pos. vert. première ligne du lutin (VSTART)
* bits L0-L8 Pos. vert dernière ligne + 1 du lutin (VSTOP) 
calcul_mot_ctrl:
		add.w	d6,d0			Coordonnée X réelle
		add.w	d7,d1			Coordonnée Y réelle
		move.w	d1,d3			d3=Pos. Y 1ere ligne lutin
		add.w	d2,d3			d3=Pos. Y dernière ligne lutin
		lsl.w	#8,d3
		bcc.s	.not_e8			-> bit L8 à zéro
		or.w	#$0002,d3		Place le bit L8
.not_e8		lsr.w	#1,d0			Bit H0 mis ?
		bcc.s	.not_h0			-> non
		or.w	#$0001,d3		Place le bit H0
.not_h0		lsl	#8,d1
		bcc.s	.not_l8			-> bit E8 à zéro
		or.w	#$0004,d3		Place le bit E8
.not_l8		swap	d3
		move	d0,d3			d3=HSTART bit H1 à H8
		or.w	d1,d3			+ VSTART bit E0 à E7
		swap	d3
		rts

*************** Calcul les deux mots de contrôle d'un lutin ***************
* Entrée	=> d0.l	Pos. X du lutin en supershires
*		=> d1.l	Pos. Y du lutin
*		=> d2.w	Hauteur du lutin
*		=> d6.w	Pos. X fenêtre écran * 4 (superhires)
*		=> d7.w	Pos. Y fenêtre écran
* Sortie	<= d3.l	Les 2 mots de contrôles
* Utilise	(d0/d1/d2/d3/d6/d7)
* Bit n° 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00 (MOT CONTROLE 1)
*	 E7 E6 E5 E4 E3 E2 E1 E0 HA H9 H8 H7 H6 H5 H4 H3
* Bit n° 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00 (MOT CONTROLE 2)
*	 L7 L6 L5 L4 L3 L2 L1 L0 AT 00 00 H1 H0 E8 L8 H2
* bits HA-H8 Pos. horz. du lutin (HSTART) (11bits (AGA uniquement)
* bits E0-E8 Pos. vert. première ligne du lutin (VSTART)
* bits L0-L8 Pos. vert dernière ligne + 1 du lutin (VSTOP) 
* Attention la position x des lutins est gérés en superhires (1280 pixels)
calcul_mot_ctrl_aga:
		add.w	d6,d0			Coordonnée X réelle
		add.w	d7,d1			Coordonnée Y réelle
calcul_ctrlaga:	move.w	d1,d3			d3=Pos. Y 1ere ligne lutin
		add.w	d2,d3			d3=Pos. Y dernière ligne lutin
		lsl.w	#8,d3
		bcc.s	.not_e8			-> bit L8 à zéro
		or.w	#$0002,d3		Place le bit L8
.not_e8		lsr.w	#1,d0			Bit H0 mis ?
		bcc.s	.not_h0			-> non
		or.w	#$0008,d3		Place le bit H0
.not_h0		lsr.w	#1,d0			Bit H1 mis ?
		bcc.s	.not_h1			-> non
		or.w	#$0010,d3		Place le bit H1
.not_h1		lsr.w	#1,d0			Bit H2 mis ?
		bcc.s	.not_h2			-> non
		or.w	#$0001,d3		Place le bit H2
.not_h2		lsl.w	#8,d1
		bcc.s	.not_l8			-> bit E8 à zéro
		or.w	#$0004,d3		Place le bit E8
.not_l8		swap	d3
		move.w	d0,d3			d3=HSTART bit H1 à H8
		or.w	d1,d3			+ VSTART bit E0 à E7
		swap	d3
		rts
