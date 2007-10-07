*-------------------------------------------------------------------------*
************ Données utilisées pour la gestion des textes BOBS ************
*-------------------------------------------------------------------------*

*/ Déclare la structure BobText pour les tableaux */
		RSRESET
BT:		rs.b	BOB.SIZE
BT.SIZE		rs	0
*/ Déclare la structure BobText pour les gardiens */
		RSRESET
SBT:		rs.b	SBOB.SIZE
SBT.SIZE	rs	0
		CNOP	0,4
pt_bobtext	dc.l	0,0,"SHAP",0
pt_tb:		dc.l	0		Pt/Le texte
ze_bobtext	dc.b	"LEVEL[AA",0
		dc.b	"COMPLETETED",0
ze_bobtext2	dc.b	"LEVEL[AA",0
		dc.b	"COMPLETETED",0
		EVEN
tb.l1		dc.w	0		Nombre de lettres -1 1ere ligne
tb.l2		dc.w	0		Nombre de lettres -1 2ieme ligne
tb.lx		dc.w	0		Nombre de lettres totales - 1
tb.la		dc.w	0		256 tableau & 320 gardien 
