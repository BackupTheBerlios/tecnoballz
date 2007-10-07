*=========================================================================*
***---------------------------Initialise OEIL --------------------------***
*=========================================================================*
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d7.l 0 pas d'erreur
init_oeil:
		*/ Réserve les structures  */
		lea	(pt_oeil-t,a5),a4
		move.l	#OEIL.SIZE,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne	.exit

		move.l	(pt_oeil-t,a5),a1
		moveq	#BOB.OEIL,d0
		bsr	bob32.only
		bne	.exit
		bsr	shado.ajoute
		bne	.exit

		clr.l	(a1)

		move.w	(hasard-t,a5),d0
		add.w	(VHPOSR,a6),d0
		add.w	(VPOSR,a6),d0
		add.w	d6,d0
		and.w	#31,d0
		move.w	(bu.x-t,a5,d0*2),(BOB.POSX1,a1)

		add.w	(VHPOSR,a6),d0
		and.w	#31,d0
		move.w	(bu.y-t,a5,d0*2),(BOB.POSY1,a1)

		add.w	(hasard-t,a5),d0
		add.w	(VHPOSR,a6),d0
		add.w	(VPOSR,a6),d0
		add.w	d6,d0
		and.w	#31,d0
		move.w	(bu.x-t,a5,d0*2),(OEIL.XA,a1)
	
		add.w	(VHPOSR,a6),d0
		add.w	(VPOSR,a6),d0
		add.w	d6,d0
		and.w	#31,d0
		move.w	(bu.y-t,a5,d0*2),(OEIL.YA,a1)


		moveq	#0,d7
.exit		rts
