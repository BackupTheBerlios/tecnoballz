*/////////////////////////////////////////////////////////////////////////*
*----------------------->  Affiche les ejecteurs  <-----------------------*
*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l CUSTOM
init_eject:
		moveq	#BOB.EJECT1,d0
		move.w	#EJ1X,d1
		move.w	#EJ1Y,d2
		moveq	#0,d3
		bsr.l	bob32.gfx
		moveq	#BOB.EJECT2,d0
		move.w	#EJ2X,d1
		move.w	#EJ2Y,d2
		moveq	#0,d3
		bsr.l	bob32.gfx
		moveq	#BOB.EJECT3,d0
		move.w	#EJ3X,d1
		move.w	#EJ3Y,d2
		moveq	#0,d3
		bsr.l	bob32.gfx
		moveq	#BOB.EJECT4,d0
		move.w	#EJ4X,d1
		move.w	#EJ4Y,d2
		moveq	#0,d3
		bsr.l	bob32.gfx
		rts

* AFFICHE OMBRES EJECTEURS *
init_oeject:
		moveq	#BOB.EJECT1,d0
		move.w	#EJ1X-ODECA,d1
		move.w	#EJ1Y+ODECA,d2
		bsr.l	shado.gfx
		moveq	#BOB.EJECT2,d0
		move.w	#EJ2X-ODECA,d1
		move.w	#EJ2Y+ODECA,d2
		bsr.l	shado.gfx
		moveq	#BOB.EJECT3,d0
		move.w	#EJ3X-ODECA,d1
		move.w	#EJ3Y+ODECA,d2
		bsr.l	shado.gfx
		moveq	#BOB.EJECT4,d0
		move.w	#EJ4X-ODECA,d1
		move.w	#EJ4Y+ODECA,d2
		bsr.l	shado.gfx
		rts
