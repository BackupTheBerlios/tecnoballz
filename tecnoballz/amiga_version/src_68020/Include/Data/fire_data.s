*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-*
*-*-*-*-*- DONNEES UTILISEES POUR LA GESTION DES TIRS RAQUETTES *-*-*-*-*-*
*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-.-*-*
		CNOP	0,4
pt_objet	dc.l	0,0,'SHAP',0
fire.flag	dc.w	0		Flag 0 = tir 1 / 1 = tir 2
fire.val	dc.w	0		Décrementation briques (2 ou 4)
*/ Structure Shape tirs */
		RSRESET
FIRE:		rs.b	BOB.SIZE
FIRE.ACTIF	=BOB.ACTIF
FIRE.X		=BOB.POSX
FIRE.Y		=BOB.POSY
FIRE.RAK	rs.l	1	raquette tireuse
FIRE.PTANIM	rs.w	1	pt/gfx
FIRE.SINUS	rs.w	1	pt/sinus
FIRE.PTBOB	rs.w	1	pt gfx
FIRE.TEMPO	rs.w	1	compteur animation
FIRE.XSCIE	rs.w	1	x scie
FIRE.YSCIE	rs.w	1	y scie
FIRE.SIZE	rs	0
*/ sinus pour le tir 7 ; "la scie circulaire" */
		CNOP	0,4
sinus:		dc.w	015,-03,013,-07,011,-10,009,-12,007,-13,003,-15,000,-15
		dc.w	-03,-15,-06,-14,-09,-12,-11,-10,-13,-07,-14,-04,-15,-01
		dc.w	-15,003,-13,007,-11,010,-09,012,-07,013,-03,015,000,015
		dc.w	003,015,006,014,009,012,011,010,013,007,014,004,015,001
		dc.w	99,99,99,99
