***************************************************************************
*------------------*DONNEES POUR LA GESTION DES JOUEURS*------------------*
***************************************************************************
		RSRESET
J		rs	0
J.NAME		rs.b	6		Nom ASCII
J.SCORE		rs.b	6		Score ASCII
J.AREA		rs.b	1		N° Area
J.LEV		rs.b	2		N° Level
J.LIFE		rs.b	2		Nombre de vie(s)
J.ACTIF		rs.b	1		Flag Joueur actif
J.COURSE	rs.w	NOPTIONS	Liste des bonus
J.OPT		rs.w	1		Nombre d'options
J.CREDIT0	rs.w	1		Credit unité
J.CREDIT1	rs.w	1		Credit dizaine
J.CREDIT2	rs.w	1		Credit centaine
J.CREDIT3	rs.w	1		Credit millier
J.CREDIT4	rs.w	1		Credit dizaine de milliers
J.CREDIT5	rs.w	1		Credit centaine de milliers
J.TOTAL		rs.w	1		crédit total
J.MLEFT		rs.b	BRICOT.NUM	flag des 12 bricotes gauches
J.MRIGHT	rs.b	BRICOT.NUM	flag des 12 bricotes droites
J.MUP		rs.b	BRICOT.NUM	flag des 12 bricotes hautes
		rs.b	8		*inutilisé*
J.BUMP2		rs.w	1		état raquette droite 
J.BUMP3		rs.w	1		état raquette haute
J.BUMP4		rs.w	1		état raquette gauche
J.REBUILD	rs.w	1		flag 'rebuild mur'
J.LESS		rs.w	1		flag 'less bricks'
J.BONUS		rs.w	1		compt. points vie gratuite
J.LARGE		rs.w	1		largeur de la raquette
J.PRIX		rs.w	1		prix à un dans le magasin
J.GEMME		rs.b	6		Etat des gemmes
J.GEMMEX	rs.w	1		Nombre de gemmes ramassées
J.SIZE		rs	0

* Structure RESA *
pt_player	dc.l	0,NJOUEUR*J.SIZE,"LIST",0

* Table des adresse des structures joueurs *
liste_joueur:	dc.l	0
		ds.l	NJOUEUR
player		dc.w	1			N° du joueur
player.num	dc.w	1			Nombre de joueur

