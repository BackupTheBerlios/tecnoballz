*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
*»»»»»»»»»»»»»»»»»»»»»»»»»»» STRUCTURE GADGETS «««««««««««««««««««««««««««*
*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
		RSRESET
GAD:		rs.b	BOB.SIZE
GAD.X		=BOB.POSX
GAD.Y		=BOB.POSY
GAD.ACTIF	=BOB.ACTIF	Gadget actif
GAD.DEP		rs.l	1	Routine de déplacement
GAD.RAK		rs.l	1	Raquette où va le gadget
GAD.PTBOB	rs.w	1	Offset animation du Gagdet
GAD.NUM		rs.w	1	n° bonus/malus
GAD.TEMPO	rs.w	1
GAD.PTANIM	rs.w	1
GAD.SIZE	rs	0
		RSRESET
SGAD:		rs.b	SBOB.SIZE
SGAD.X		=SBOB.POSX	
SGAD.Y		=SBOB.POSY
SGAD.ACTIF	=SBOB.ACTIF
SGAD.DEP	rs.l	1	*INUTILISE*
SGAD.RAK	rs.l	1	*INUTILISE*
SGAD.PTBOB	rs.w	1	Offset animation du Gagdet
SGAD.NUM	rs.w	1	N° bonus/malus
SGAD.TEMPO	rs.w	1
SGAD.PTANIM	rs.w	1
SGAD.SIZE	rs	0
		CNOP	0,4
pt_gadget:	dc.l	0,0,"SHAP",0
