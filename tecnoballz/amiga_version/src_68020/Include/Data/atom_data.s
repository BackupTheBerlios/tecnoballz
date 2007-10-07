*=========================================================================*
*---------------------DONNEES UTILISEES PAR LES ATOMS---------------------*
*=========================================================================*
		RSRESET
ATOMS		rs.b	BOB.SIZE
ATOM.X		=BOB.POSX
ATOM.Y		=BOB.POSY
ATOM.GHOST	rs.l	1
ATOM.INC	rs.l	1
ATOM.ACTIF	rs.w	1	0=actif >0 inactif
ATOM.COUNT1	rs.w	1	tempo déplacement
ATOM.PT		rs.w	1
ATOM.POWER	rs.w	1	Résistance
ATOM.OFF	rs.w	1
ATOM.EXPLO	rs.w	1
ATOM.OEXP	rs.w	1
		rs.w	1
ATOM.SIZE	rs	0
		CNOP	0,4
pt_atom		dc.l	0,0,"SHAP",0

*/ temps pendant lequel les atoms restent sur la même trajectoire */
ghost_c0:	dc.w	50,50,30,50,20,12,08,50,45,32,12,56,33,22,22,20
ghost_c1	dc.w	40,20,30,47,22,33,50,50,55,52,42,32,42,32,42,32
ghost_c2	dc.w	10,10,20,27,12,13,20,10,10,20,10,25,26,12,32,33
ghost_c3	dc.w	60,20,50,37,42,33,33,20,20,22,33,70,20,22,32,32

*/ deplacement des atoms : incrément x,y */
ghost_trajec:	dc.w	00,01,00,00
		dc.w	-1,00,00,00
		dc.w	00,-1,-1,01
		dc.w	-1,-1,-1,00
		dc.w	-1,01,01,-1
		dc.w	01,-1,01,01
		dc.w	01,01,01,00
		dc.w	00,01,00,01
		REPT	NATOM
		dc.w	01,-1,00,-1
		dc.w	01,00,01,-1
		dc.w	-1,-1,-1,00
		ENDR

ghost.bob:	dc.w	O.ATOM1,O.ATOM2,O.ATOM3,O.ATOM4
		dc.w	O.ATOM5,O.ATOM6,O.ATOM7,O.ATOM8
		dc.w	O.ATOM9,O.ATOM10,O.ATOM11,O.ATOM12
		dc.w	O.ATOM13,O.ATOM14,O.ATOM15,O.ATOM16
		dc.w	O.ATOM17,O.ATOM18,O.ATOM19,O.ATOM20
		dc.w	O.ATOM1,O.ATOM2,O.ATOM3,O.ATOM4
		dc.w	O.ATOM5,O.ATOM6,O.ATOM7,O.ATOM8
		dc.w	O.ATOM9,O.ATOM10,O.ATOM11,O.ATOM12
