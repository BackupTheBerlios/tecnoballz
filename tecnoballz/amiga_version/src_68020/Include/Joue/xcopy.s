*=========================================================================*
*                Sauve données du jeu dans la liste joueur		  *
*=========================================================================*
* Entrée	=> a0.l	adr. liste  joueur
*		=> a5.l	adr. table 't'
* Utilise	(d0 / a0,a3,a4,a5)
xcopy:
		* Sauve le score *
		move.b	(1+score0-t,a5),(5+J.SCORE,a0)
		move.b	(1+score1-t,a5),(4+J.SCORE,a0)
		move.b	(1+score2-t,a5),(3+J.SCORE,a0)
		move.b	(1+score3-t,a5),(2+J.SCORE,a0)
		move.b	(1+score4-t,a5),(1+J.SCORE,a0)
		move.b	(1+score5-t,a5),(0+J.SCORE,a0)

		* Sauve le nombre de vie(s) *
		move.b	(1+life0-t,a5),(0+J.LIFE,a0)
		move.b	(1+life1-t,a5),(1+J.LIFE,a0)

		* Sauve la liste des 20 bonus *
		IFEQ	A_TAB
		lea	(course-t,a5),a3	adresse table des bonus
		lea	(J.COURSE,a0),a4
		moveq	#NOPTIONS-1,d0
.force		move.w	(a3)+,(a4)+
		dbf	d0,.force

		* Sauve le nombre de bonus acheté(s) *
		move.w	(bonus.achete-t,a5),(J.OPT,a0)

		* Sauve le crédit *
		move.w	(credit0-t,a5),(J.CREDIT0,a0)
		move.w	(credit1-t,a5),(J.CREDIT1,a0)
		move.w	(credit2-t,a5),(J.CREDIT2,a0)
		move.w	(credit3-t,a5),(J.CREDIT3,a0)
		move.w	(credit4-t,a5),(J.CREDIT4,a0)
		move.w	(credit5-t,a5),(J.CREDIT5,a0)
		move.w	(credit-t,a5),(J.TOTAL,a0)

		* Sauve état des 3 raquettes *
		lea	(b2-t,a5),a4
		move.w	(0*BUMP.SIZE+BUMP.ACTIF,a4),(J.BUMP2,a0)
		move.w	(1*BUMP.SIZE+BUMP.ACTIF,a4),(J.BUMP3,a0)
		move.w	(2*BUMP.SIZE+BUMP.ACTIF,a4),(J.BUMP4,a0)

		* Sauve état du flag "rebuild-mur" *
		move.w	(lessbrick-t,a5),(J.REBUILD,a0)

		* Sauve état du flag "less-bricks" *
		move.w	(lessbrick-t,a5),(J.LESS,a0)

		* Sauve le nombre de points pour une vie  *
		move.w	(score.life-t,a5),(J.BONUS,a0)

		* Sauve la largeur des raquettes de 16 à 64 *
		move.w	(rak.large-t,a5),(J.LARGE,a0)

		* Sauve l'état du mur gauche *
		lea	(bri_h1+2-t,a5),a3
		lea	(J.MLEFT,a0),a4
		moveq	#BRICOT.NUM-1,d0
.l_cote_br1	move.b	(a3)+,(a4)+
		dbf	d0,.l_cote_br1

		* Sauve l'état du mur droit *
		lea	(bri_h2+2-t,a5),a3
		moveq	#BRICOT.NUM-1,d0
.l_cote_br2	move.b	(a3)+,(a4)+
		dbf	d0,.l_cote_br2

		* Sauve l'état du mur haut *
		lea	(bri_h3+2-t,a5),a3
		moveq	#BRICOT.NUM-1,d0
.l_cote_br3	move.b	(a3)+,(a4)+
		dbf	d0,.l_cote_br3

		* sauve flag PRICE BONUS *
		move.w	(sh.prix-t,a5),(J.PRIX,a0)

		* Sauve état des gemmes *
		lea	(gemme_du_j1-t,a5),a3
		lea	(J.GEMME,a0),a4
		move.l	(a3)+,(a4)+
		move.w	(a3),(a4)
		move.w	(gemmex-t,a5),(J.GEMMEX,a0)
		ENDC
		rts
