*									  *
*-------------------------*  DONNEES RAQUETTES  *-------------------------*
*									  *
		CNOP	0,4
rak.pagegfx	dc.l	0,0,"RAQU",0
rak.sprite	dc.l	0,0,"RKSP",0
rak.lutin	dc.l	0,0,"LUTI",MEMF_CHIP
rak.lutin2	dc.l	0,0,"LUTI",MEMF_CHIP
rak.lutinr	dc.l	0,0,"LUTI",MEMF_CHIP
rocky:		ds.l	28*2		pt/lutins raquettes horizontales
rocky2:		ds.l	28*2		pt/lutins raquettes verticales

* TYPE RAQUETTE : Normal / Fire / Glue / FireGlue
* 7 TAILLES	: 16,24,32,40,48,56,64
*		=> 28 raquettes différentes

rak.lutin0.1	dc.l	0	Raquette du haut
rak.lutin1.1	dc.l	0
rak.lutin0.2	dc.l	0	Raquette de gauche
rak.lutin1.2	dc.l	0
rak.lutin0.3	dc.l	0	Raquette du bas
rak.lutin1.3	dc.l	0
rak.lutin0.4	dc.l	0	Crédit
rak.lutin1.4	dc.l	0
rak.lutin2.1	dc.l	0	Raquette de droite
rak.lutin3.1	dc.l	0
rak.lutin2.2	dc.l	0	Inverseur
rak.lutin3.2	dc.l	0
rak.lutin4	dc.l	0	Robot
rak.lutin5	dc.l	0

robot		dc.w	0	Flag 1=Robot actif
rak.haut	dc.w	0	hauteur raquette (8 lignes)
rak.axx2	dc.w	0	deplacement rel. x & y pt souris 2
rak.slarge	dc.w	0	sauvegarde long. raqu. pendant les gardiens
rak.large	dc.w	0	largeur raquette (8,16,24,32,40,48,56,64)
rak.dep		dc.w	0	flag depl. raqu. (souris 1)
rak.vdd		dc.w	0	vites. dépl. raqu. droite
rak.vgg		dc.w	0	vites. dépl. raqu. gauche
rak.axx		dc.w	0	deplacement rel. x & y pt souris 1
flag_fire0	dc.w	0	flag bouton souris port 1 préssé
rak.blitz	dc.w	0	flag blizt sort de la raquette
rak.inver	dc.w	0	flag/compt : inverseur
rak.mov		dc.w	0	flag depl. raqu. (souris 2)
rak.srr		dc.w	0	vites. dépl. raqu. droite
rak.sll		dc.w	0	vites. dépl. raqu. gauche
rak.team	dc.w	0	flag, 1= raqu. horz. et vert. independantes
inv50		dc.w	0	invers. compt. 50ieme de sec.
inv0		dc.w	0	invers. unité
inv1		dc.w	0	invers. dizaine
rak.allcopy	dc.w	0	recopie toutes les raquettes lutins
invy		dc.w	0

*STRUCTURE BUMP *
		RSRESET
BUMP:		rs.w	0
BUMP.BOB	rs.l	1
BUMP.MONNAIE	rs.l	1	Pt/routine déplacement des capsules monnaies
BUMP.GADGET	rs.l	1	Pt/routine déplacement Bonus & Malus
BUMP.RBLEFT	rs.l	1	Pt/table rebond déplacement à gauche
BUMP.RBRIGHT	rs.l	1	Pt/table rebond déplacement à droite
BUMP.REBOND	rs.l	1	Pt/table rebond gauche ou droite
BUMP.BALLE	rs.l	1	Pt/structure balle collée
BUMP.OBJET	rs.l	1	Pt/les 7 structures tirs
BUMP.COPY	rs.w	1	Flag = 1 alors copie raquette lutin
BUMP.NUMBER	rs.w	1	Numérp de la raquette
BUMP.TOUCH	rs.w	1	Flag = 1 touchée par un tir des gardiens
BUMP.X		rs.w	1	Coordonée X en pixels
BUMP.Y		rs.w	1	Coordonée Y en pixels
BUMP.NORMAL	rs.w	1	normale
BUMP.FIRE	rs.w	1	fire
BUMP.GLUE	rs.w	1	colle
BUMP.ACTIF	rs.w	1	marche t'il
BUMP.FX0	rs.w	1	inc0. x tir
BUMP.FY0	rs.w	1	inc0. y tir
BUMP.FX1	rs.w	1	inc1. x tir
BUMP.FY1	rs.w	1	inc1. y tir
BUMP.TFIRE	rs.w	1	quel tir 0,1 ou 2
BUMP.FX2	rs.w	1	inc2. x tir
BUMP.FY2	rs.w	1	inc2. y tir
BUMP.XSCIE	rs.w	1	x centre scie
BUMP.YSCIE	rs.w	1	y centre scie
BUMP.XDECA	rs.w	1	decalage x tir 6
BUMP.YDECA	rs.w	1	decalage y tir 6
BUMP.SPEED	rs.w	1	vitesse de déplacement (maxium 10)
		rs.w	1	

BUMP.SIZE	rs	0
		CNOP	0,4
b1:		ds.b	BUMP.SIZE	Raquette bas
b2:		ds.b	BUMP.SIZE	Raquette droite
b3:		ds.b	BUMP.SIZE	Raquette haute
b4:		ds.b	BUMP.SIZE	Raquette gauche
br:		ds.b	BUMP.SIZE	Raquette Robot (en bas)
