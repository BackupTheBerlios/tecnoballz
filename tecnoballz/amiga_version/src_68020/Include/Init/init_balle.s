*=========================================================================*
*--------------------------Initialise les balles--------------------------*
*=========================================================================*
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d7.l 0 pas d'erreur
init_balle:
		*/ Réserve les structures Balles */
		lea	(pt_balle-t,a5),a4
		move.l	#BALLE.SIZE*NBALLE,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne	.exit

		*/ Init. les structures */
		move.l	(pt_balle-t,a5),a1
		moveq	#NBALLE-1,d6
.loopinit	moveq	#BOB.BALLE,d0
		bsr	bob32.only
		bne	.exit
		bsr	shado.ajoute
		bne	.exit
		clr.l	(BOB.ACTIF,a1)
		lea	(b1-t,a5),a2
		move.w	#200,(BALLE.X,a1)
		move.w	(BUMP.Y,a2),(BALLE.Y,a1)
		move.w	#5,(BALLE.PBTEMP,a1)
		move.w	#2,(BALLE.PT,a1)
		move.w	(balle_start-t,a5),(BALLE.START,a1)
		move.w	#5,(BOB.WIDTH,a1)
		lea	(bri_poi-t,a5),a2
		move.l	a2,(BALLE.POI,a1)
		move.w	#2,(BALLE.VAL,a1)
		lea	(BALLE.SIZE,a1),a1
		dbf	d6,.loopinit

		*/ Spécial Balle 1 */
		move.l	(pt_balle-t,a5),a4
		lea	(b1-t,a5),a1
		move.l	a1,(BALLE.RAK,a4)
		move.w	#1,(BALLE.GLUE,a4)
		move.w	#1,(BALLE.ACTIF,a4)
		move.l	(balle_speed-t,a5),(BALLE.SPEED,a4)
		move.w	(balle_scount-t,a5),(BALLE.SCOUNT,a4)
		clr.w	(balle_nbre-t,a5)	débute avec une balle
		moveq	#0,d7
.exit		rts
