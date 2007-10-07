***************************************************************************
*-------------------DONNEES UTILISEES GESTION BRICOTES--------------------*
***************************************************************************
	CNOP	0,4
tarte:	dc.l	0
	dc.l	(2*04*2*BRICOT.NUM)+(2*16*2*BRICOT.NUM)+(2*16*2*BRICOT.NUM)
	dc.l	"SABC",MEMF_CHIP
pt_cote		dc.l	0	pt/cote_pnt
mur.kas_rg	dc.w	0	flag 1 = mur gauche cassable
mur.kas_up	dc.w	0	flag 1 = mur haut cassable
mur.kas_lf	dc.w	0	flag 1 = mur droit cassable  
brico.rebuild	dc.w	0	flag 1 = reconstruit mur
brico.save	dc.w	0	pt/table bricote à sauver
brico.clr	dc.w	0	pt/table bricote à effacer

*/ STRUCTURE EFFACEMENT BRICOTES */
		RSRESET
COTE		rs	0
COTE.FLAG	rs.w	1	00 1 bricote haut / 2 droit / 3 gauche
COTE.PT		rs.w	1	02 pointeur dans map-editor
COTE.SIZE	rs	0
** table des bricotes à effacer **
		CNOP	0,4
cote_pnt	ds.b	COTE.SIZE*CNP

*/ Map-editor des murs de coté */
bri_h1:		dc.b	-1,0
		dcb.b	BRICOT.NUM,1			mur gauche
		dc.b	0
bri_h2		dc.b	-1,0
		dcb.b	BRICOT.NUM,1			mur droit
		dc.b	0,-1
bri_h3		dc.b	-1,0
		dcb.b	BRICOT.NUM,1			mur haut
		dc.b	0
*/ Map-editor des murs de coté (effacement)  */
bry_x1:		dc.b	-1,0
		dcb.b	BRICOT.NUM,1			mur gauche
		dc.b	0
bry_x2		dc.b	-1,0
		dcb.b	BRICOT.NUM,1			mur droit
		dc.b	0,-1
bry_x3		dc.b	-1,0
		dcb.b	BRICOT.NUM,1			mur haut
		dc.b	0
		EVEN
		*/ position écran des Bricotes du mur gauche */
mpos_gauche	dc.w	0,0
mpos_gauch2	ds.w	BRICOT.NUM
		*/ position écran des Bricotes du mur droite */
mpos_droit:	dc.w	0,0
mpos_droi2	ds.w	BRICOT.NUM
		*/  position écran des Bricotes du mur haut */
mpos_haut:	dc.w	0,0
mpos_hau2	ds.w	BRICOT.NUM
		CNOP	0,4
*/ adr. buffer des fonds 4 couleurs des bricotes mur gauche */
sadr_gauche:	dc.l	0,0
sadr_gauch2	ds.l	BRICOT.NUM
*/ adr. buffer des fonds 4 couleurs des bricotes mur droit */
sadr_droit:	dc.l	0,0
sadr_droi2	ds.l	BRICOT.NUM
*/ jeu : adr. buffer des fonds 4 couleurs des bricotes mur haut */
sadr_haut:	dc.l	0,0
sadr_hau2	ds.l	BRICOT.NUM
