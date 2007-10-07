*-------------------------------------------------------------------------*
********* DONNES UTILISEES POUR LA GESTION DES CAPSULES MONNAIES **********
*-------------------------------------------------------------------------*
		CNOP	0,4
pt_capsule:	dc.l	0,0,"SHAP",0
cap.frek	dc.w	0
sscap		dcb.l	SS.SIZE,0	*/ Animation Monaie dans le lutin
ssinv		dcb.l	SS.SIZE,0	*/ Animation Inverseur dans le lutin
*/ Valeur + Vitesse des capsules monnaies */
cap.val		dc.w	10,1,20,2,30,3,20,2
* STRUCTURES CAPSULE TABLEAU */
		RSRESET
CAP:		rs.b	BOB.SIZE
CAP.X		=BOB.POSX
CAP.Y		=BOB.POSY
CAP.ACTIF	=BOB.ACTIF
CAP.DEP		rs.l	1	Routine de déplacement
CAP.RAK		rs.l	1	Raquette où va la monnaie
CAP.VAL		rs.w	1	Valeur de la monnaie 10 20 ou 30
CAP.SPEED	rs.w	1	Vitesse de déplacement
CAP.SIZE	rs	0
* STRUCTURES CAPSULE GARDIEN */
		RSRESET
SCAP:		rs.b	BOB.SIZE
SCAP.X		=BOB.POSX
SCAP.Y		=BOB.POSY
SCAP.ACTIF	=BOB.ACTIF
SCAP.DEP	rs.l	1	*INUTILISE*
SCAP.RAK	rs.l	1	*INUTILISE*
SCAP.VAL	rs.w	1	Valeur de la monnaie 10 20 ou 30
SCAP.SPEED	rs.w	1	Vitesse de déplacement
SCAP.SIZE	rs	0
