*=========================================================================*
*           Recopie liste joueur dans adr. utilisés par le jeu		  *
*=========================================================================*
* entrée	=> a0.l	adr. liste joueur
*		=> a5.l	adr. table 't'
* utilise	d0 / a0,a3,a4,a5
marauder:

		* Recopie nom du joueur *
		move.l	(J.NAME,a0),(name.x-t,a5)
		move.w	(4+J.NAME,a0),(4+name.x-t,a5)

		* Recopie le score *
		move.b	(5+J.SCORE,a0),(1+score0-t,a5)
		move.b	(4+J.SCORE,a0),(1+score1-t,a5)
		move.b	(3+J.SCORE,a0),(1+score2-t,a5)
		move.b	(2+J.SCORE,a0),(1+score3-t,a5)
		move.b	(1+J.SCORE,a0),(1+score4-t,a5)
		move.b	(0+J.SCORE,a0),(1+score5-t,a5)

		* Recopie le nombre de vie(s) *
		move.b	(0+J.LIFE,a0),(1+life0-t,a5)
		move.b	(1+J.LIFE,a0),(1+life1-t,a5)

		* Recopie la liste des 20 bonus *
		IFEQ	A_TAB
		lea	(J.COURSE,a0),a3
		lea	(course-t,a5),a4	adresse table des bonus
		moveq	#NOPTIONS-1,d0
.force		move.w	(a3)+,(a4)+
		dbf	d0,.force

		* Recopie le nombre de bonus acheté(s) *
		move.w	(J.OPT,a0),(bonus.achete-t,a5)

		* Recopie le crédit *
		move.w	(J.CREDIT0,a0),(credit0-t,a5)
		move.w	(J.CREDIT1,a0),(credit1-t,a5)
		move.w	(J.CREDIT2,a0),(credit2-t,a5)
		move.w	(J.CREDIT3,a0),(credit3-t,a5)
		move.w	(J.CREDIT4,a0),(credit4-t,a5)
		move.w	(J.CREDIT5,a0),(credit5-t,a5)
		move.w	(J.TOTAL,a0),(credit-t,a5)

		* Recopie état des 3 raquettes *
		lea	(b2-t,a5),a4
		move.w	(J.BUMP2,a0),(0*BUMP.SIZE+BUMP.ACTIF,a4)
		move.w	(J.BUMP3,a0),(1*BUMP.SIZE+BUMP.ACTIF,a4)
		move.w	(J.BUMP4,a0),(2*BUMP.SIZE+BUMP.ACTIF,a4)

		* Recopie état du flag "rebuild-mur" *
		move.w	(J.REBUILD,a0),(lessbrick-t,a5)

		* Recopie état du flag "less-bricks" *
		move.w	(J.LESS,a0),(lessbrick-t,a5)

		* Recopie le nombre de points pour une vie  *
		move.w	(J.BONUS,a0),(score.life-t,a5)

		* Recopie la largeur des raquettes de 16 à 64 *
		move.w	(J.LARGE,a0),(rak.large-t,a5)

		* Recopie l'état du mur gauche *
		lea	(J.MLEFT,a0),a3
		lea	(bri_h1+2-t,a5),a4
		moveq	#BRICOT.NUM-1,d0
.l_cote_br1	move.b	(a3)+,(a4)+
		dbf	d0,.l_cote_br1

		* Recopie l'état du mur droit *
		lea	(bri_h2+2-t,a5),a4
		moveq	#BRICOT.NUM-1,d0
.l_cote_br2	move.b	(a3)+,(a4)+
		dbf	d0,.l_cote_br2

		* Recopie l'état du mur haut *
		lea	(bri_h3+2-t,a5),a4
		moveq	#BRICOT.NUM-1,d0
.l_cote_br3	move.b	(a3)+,(a4)+
		dbf	d0,.l_cote_br3

		* Recopie flag PRICE BONUS *
		move.w	(J.PRIX,a0),(sh.prix-t,a5)
		
		* Etat des gemmes *
		lea	(J.GEMME,a0),a3
		lea	(gemme_du_j1-t,a5),a4
		move.l	(a3)+,(a4)+
		move.w	(a3),(a4)
		move.w	(J.GEMMEX,a0),(gemmex-t,a5)
		ENDC
		rts
