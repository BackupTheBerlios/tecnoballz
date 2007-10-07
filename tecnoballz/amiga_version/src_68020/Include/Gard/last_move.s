*=========================================================================*
*                        Déplacement des gardiens			  *
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
last_move:
		*/ Lecture de l'offset (valeur de 0 à 63) /*
		lea	(offzet-2-t,a5),a0	table sinus de 0 à 63
		addq.w	#1,(a0)			inc. pointeur sur cette table
		cmp.w	#offvet-offzet,(a0)	Dépassement table ?
		bne.s	.offhet			-> non
		clr.w	(a0)			RAZ pointeur cette
.offhet		add.w	(a0)+,a0		add. pointeur à cette table
		moveq	#0,d7
		move.b	(a0),d7			valeur de l'offset de 0 à 63

		*/ Déplace Gardien 1 */
		move.l	(gd1-t,a5),a4		Adr. structure gardien 1
		bsr.s	.like_to_moved

		*/ Déplace Gardien 2 */
		cmp.w	#1,(gd.numb-t,a5)	1 seul gardien
		beq	.last_move_exit		-> oui
		sub.w	#63,d7
		neg.w	d7			Inverse offset
		move.l	(gd2-t,a5),a4		Adr. structure gardien 1

		*/ Routine de déplacement */
		*=> a4.l Adr. structure GARD
		*=> d7.w Offset de déplacement de 0 à 63
.like_to_moved:	move.l	(GARD.LISSA,a4),a0	Adr. courbe
		moveq	#0,d0
		moveq	#0,d1
		move.b	(a0),d0			coord. x.
		move.b	(1,a0),d1		coord. y.
		add.w	d7,d0			+offset x de 0 à 63
		cmp.w	(GARD.MINX,a4),d0	dépass. à gauche ?
		bge.s	.pierre_01		-> non
		move.w	(GARD.MINX,a4),d0
.pierre_01	cmp.w	(GARD.MAXX,a4),d0	dépass. à droite ?
		bls.s	.pierre_02		-> non
		move.w	(GARD.MAXX,a4),d0
.pierre_02	cmp.w	(GARD.MINY,a4),d1	dépass. en haut ?
		bge.s	.pierre_03		-> non
		move.w	(GARD.MINY,a4),d1
.pierre_03	cmp.w	(GARD.MAXY,a4),d1	dépass. en bas ?
		bls.s	.pierre_04		-> non
		move.w	(GARD.MAXY,a4),d1
.pierre_04	move.w	d0,(GARD.X,a4)
		move.w	d1,(GARD.Y,a4)
		move.l	(GARD.BOB,a4),a1
		move.w	d0,(SBOB.POSX,a1)
		move.w	d1,(SBOB.POSY,a1)
		move.w	(GARD.SPEED,a4),d0	vitesse avancé dans tableau
		add.w	d0,d0			*2
		add.w	d0,a0			nouvelle adresse
		cmp.w	#-1,(a0)		fin du tableau
		bne.s	.kok_cont		-> Non
		move.l	(GARD.LISSA2,a4),a0
.kok_cont	move.l	a0,(GARD.LISSA,a4)		
.last_move_exit	rts
