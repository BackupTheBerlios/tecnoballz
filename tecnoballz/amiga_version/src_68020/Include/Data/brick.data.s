*=========================================================================*
******************* DONNEES UTILISEES POUR LES BRIQUES ********************
*=========================================================================*
		CNOP	0,4
gateau		dc.l	0,CONORS*NID,"REST",MEMF_CHIP (fond es briques 4 couleurs)
brick.kass	dc.w	0	Compteur de briques cassées
brick.save	dc.w	0	Pt/table briques à sauver
brick.clr	dc.w	0	Pt/table briques à effacer
brick.count:	dc.w	0	Nombre de briques
brick.destroy:	dc.w	0	Nombre de briques détruites
lessbrick	dc.w	0	Flag 1="lessbrick"
lessbrick.count	dc.w	0	compteur "lessbrick"
*=========================================================================*
set.x		ds.w	BK.X*16		pointeurs horizontals sur 'set_table'
set.y		ds.w	BK.Y*8		pointeurs verticals sur 'set_table'
		CNOP	0,4
set_guide	ds.l	BK.X*BK.Y	adresse rel. ecr. des briques
set_table	ds.l	BK.X*BK.Y	12*17*4=816 octets tableau actuel
set_table_end	ds.l	BK.X
set_table2	ds.l	BK.X*BK.Y	tableau actuel adresse (effacement)
		ds.l	BK.X
set_table3	ds.l	BK.X*BK.Y	tableau actuel valeur (collision)
		ds.l	BK.X
JMM		=set_table3-set_table
*=========================================================================*
		RSRESET		*/ Structure Efface & Réaffiche briques */
BRI:		rs	0
BRI.X		rs.w	1		Position X de la balle
BRI.Y		rs.w	1		Position Y de la balle
BRI.RAK		rs.l	1		Adresse de la structure raquette
BRI.NUM		rs.w	1		Numéro brique
BRI.FLAG	rs.w	1		0=Affiche brique / 1=Affiche décor
BRI.ADR		rs.w	1		Adresse relative d'affichage
		rs.w	1
BRI.SIZE	rs	0
*=========================================================================*
* table des briques à effacer *
		CNOP	0,4
bri_pnt:	ds.b	BRI.SIZE*BNP
*=========================================================================*
