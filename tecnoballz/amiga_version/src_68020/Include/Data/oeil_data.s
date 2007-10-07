*-------------------------------------------------------------------------*
*************** Données utilisées pour la gestion de l'oeil **************
*-------------------------------------------------------------------------*

*/ Déclare la structure OEIL */
		RSRESET
OEIL:		rs.b	BOB.SIZE
OEIL.ACTIF	=BOB.ACTIF	flag Oeil active
OEIL.X		=BOB.POSX	Position X de l'Oeil
OEIL.Y		=BOB.POSY	Position Y de l'Oeil
OEIL.XA		rs.w	1
OEIL.YA		rs.w	1
OEIL.SIZE	rs	0
		CNOP	0,4
pt_oeil		dc.l	0,0,"SHAP",0


	*/ Coordonéées d'apparitions */
bu.x	dc.w 040,045,050,055,060,065,070,075,080,085,092,096,100,105,111,115
	dc.w 118,120,122,128,130,132,135,140,144,146,150,152,160,164,166,170
bu.y	dc.w 040,044,056,058,060,062,070,078,080,082,085,090,092,094,096,100
	dc.w 101,120,122,124,130,138,144,146,148,150,152,153,154,155,160,170

