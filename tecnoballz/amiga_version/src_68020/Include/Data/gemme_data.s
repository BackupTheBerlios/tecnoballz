*-------------------------------------------------------------------------*
*************** Données utilisées pour la gestion des gemmes **************
*-------------------------------------------------------------------------*

*/ Déclare la structure "GEM" */
		RSRESET
GEM:		rs.b	BOB.SIZE
GEM.ACTIF	=BOB.ACTIF	flag gemme active
GEM.X		=BOB.POSX	Position X de la gemme
GEM.Y		=BOB.POSY	Position Y de la gemme
GEM.NUM		rs.w	1
		rs.w	1
GEM.SIZE	rs	0
		CNOP	0,4
pt_gemme	dc.l	0,0,"SHAP",0
gprint		dc.l	0,0,"PRIN",0
gemme.adr	dc.l	0,0,0,0,0,0,0,0,0,0,0,0

gemme_du_j1	dc.b	0,0,0,0,0,0
gemmex		dc.w	0
gemeau		ds.l	NATOM*2		Liste Coordonnées + Structure RAK
gemelle		dc.l	0
