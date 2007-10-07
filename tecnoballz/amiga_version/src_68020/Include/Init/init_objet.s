*-------------------------------------------------------------------------*
*---------------------* Initialise les objets tirs  *---------------------*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d7.l	0=pas d'erreur
init_objet:
		*/ Réserve les Structures */
		lea	(pt_objet-t,a5),a4
		move.l	#FIRE.SIZE*NTIR*4,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne	.exit

		*/ Init. les structures */
		move.l	(pt_objet-t,a5),a1
		moveq	#0,d5			pt sinus
		moveq	#NTIR*4-1,d6
.loopinit	moveq	#BOB.FIRE,d0
		bsr	bob32.only
		bne	.exit
		move.w	#1,(FIRE.TEMPO,a1)
		clr.l	(BOB.ACTIF,a1)
		move.w	d5,(FIRE.SINUS,a1)
		add.w	#16,d5
		lea	(FIRE.SIZE,a1),a1
		dbf	d6,.loopinit

		*/ Chaque tir à sa raquette */
		move.l	(pt_objet-t,a5),a1
		lea	(b1-t,a5),a0
		moveq	#NTIR-1,d6
.loop1		move.l	a0,(FIRE.RAK,a1)
		lea	(FIRE.SIZE,a1),a1
		dbf	d6,.loop1
		lea	(b2-t,a5),a0
		moveq	#NTIR-1,d6
.loop2		move.l	a0,(FIRE.RAK,a1)
		lea	(FIRE.SIZE,a1),a1
		dbf	d6,.loop2
		lea	(b3-t,a5),a0
		moveq	#NTIR-1,d6
.loop3		move.l	a0,(FIRE.RAK,a1)
		lea	(FIRE.SIZE,a1),a1
		dbf	d6,.loop3
		lea	(b4-t,a5),a0
		moveq	#NTIR-1,d6
.loop4		move.l	a0,(FIRE.RAK,a1)
		lea	(FIRE.SIZE,a1),a1
		dbf	d6,.loop4
		moveq	#0,d7
.exit		rts
