*<«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><*
*------------------ DONNEES UTILISEES PAR LES GARDIENS -------------------*
*<«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><«»><*
		CNOP	0,4
large.blitz	dc.w	0
x.blitz		dc.w	0
sin.blitz	dc.w	0
gd.color	ds.l	8			8 couleurs des dalles
mapeditor	dc.l	0,0,"EDMP",0
dalma.pagegfx	dc.l	0,0,'DALL',MEMF_CHIP
missile:	dc.l	0,MIS.SIZE*NBR_MISSILE,"SHAP",0
explomon:	dc.l	0,SBOB.SIZE*NBR_EXPLOMON,"SHAP",0
sc_mapadr	dc.l	0,0,"TABL",0
lissa		dc.l	0,0,"LISA",0
gd1:		dc.l	0,GARD.SIZE,"GAR1",0	adr. struct. gard 1
gd2:		dc.l	0,GARD.SIZE,"GAR2",0	adr. struct. gard 2
afline.restore	dc.l	0,0
alain1		dc.l	0		pt/sugar gardien 1
alain2		dc.l	0		pt/sugar gardien 2
gd1.lutin	dc.l	0,0,"LUTI",MEMF_CHIP
gd2.lutin	dc.l	0,0,"LUTI",MEMF_CHIP
sc_u1		dc.l	0
sc_w1		dc.l	0
adalles		dc.l	0
gd.pt		dc.w	0	 	pt/table gardien
gd.total	dc.w	0		nbre total de dalles
bump1.lutin0	dc.l	0
bump1.lutin1	dc.l	0
cred.lutin0	dc.l	0
cred.lutin1	dc.l	0
bump2.lutin2	dc.l	0
bump2.lutin3	dc.l	0
life.lutin2	dc.l	0
life.lutin3	dc.l	0
ss.life		ds.l	SS.SIZE
ss.credit	ds.l	SS.SIZE
gd.lutin45:	dc.l	0,0,"LUTI",MEMF_CHIP
name.lutin4	dc.l	0
name.lutin5	dc.l	0
power1.lutin4	dc.l	0
power1.lutin5	dc.l	0
power2.lutin4	dc.l	0
power2.lutin5	dc.l	0


		*/ PT/LES DIFFERENTS GIGABLITZ */
tgg1		dc.b	00,08,16,24,32,40,48,40		Gardien 64 
tgg2		dc.b	00,08,16,00,08,16,00,08		Gardien 32

		CNOP	0,4
sh.f		dc.w	0
nu_video	dc.w	0		n° ligne changement pointeurs vidéos
afline		dc.w	0		adr. rel. 1er ligne
sc_map1		dc.l	0		map-editor 1er ligne écran
asc_end		dc.l	0		dernière ligne map-editor
sc_y1		dc.w	0		pos. y sur la page écran 3
sc_copper	dc.l	0		adr. liste copper en cours
mmecran		dc.w	0		mot fort adr. écran en cours	
ffecran		dc.w	0		adr. dernière copie
flag_co		dc.w	0		si flag = 1 alors copie à faire dans l'autre écran
mapgo		dc.w	0		si = 0 dalles apparaissent

		*/ BANDE 320*16 ECRAN-> ECRAN */
		CNOP	0,4
mod_co		dc.l	0		SP_SLINE-SP_SPLAN
mod_af		dc.l	0		PG_SLINE-2,SP_SLINE-2
win_co		dc.w	0		16*64+SP_SPLAN/2
splan_co	dc.w	0		SP_SPLAN
s_nligne	dc.w	0	256+16	SPHAUTEUR
s_nligne2	dc.w	0		SPHAUTEUR-16
s_tligne	dc.w	0		SP_SLINE
m_haut		dc.w	0		M_LARGE*2*((SP_SLINE-16)/16)
s_tplan2	dc.w	0		SP_SPLAN*2-2
d_tplan		dc.w	0		PG_SPLAN
gd.explospace	dc.w	1		Explosion espacee
gd.clito	dc.w	0		Flag clignote gardien 1/2
gd.sspeed	dc.w	0		Vitesse défilement
gd.lasty	dc.w	1		Dernière position Y du gardien
gd.stemp	dc.w	0		Temps minimum avant défilement
gd.numb		dc.w	0		Nombre de gardiens

*---------------------* TABLES PARAMETRES GARDIENS *----------------------*
		CNOP	0,4
sugar:		dc.l	sugar1-t	INT1
		dc.l	sugar3-t	FIN1
		dc.l	sugar4-t	INT2
		dc.l	sugar6-t	FIN2
		dc.l	sugar7-t	INT3
		dc.l	sugar9-t	FIN3
		dc.l	sugar10-t	INT4
		dc.l	sugar12-t	FIN4
		dc.l	sugar13-t	INT5
		dc.l	sugar15-t	FIN5
		dc.l	sugar16-t	FIN6
*/ AREA 1 : intermediaires */
sugar1		dc.l	sugar2-sugar1
		dc.w	25			S.POWER resistance du gardien
		dc.w	16			016 pos x depart du tir
		dc.w	555			028 delai entre 2 Blitz
		dc.w	2			030 vit. déplacement
		dc.w	7			042 xmin zone sensible
		dc.w	5			044 ymin zone sensible
		dc.w	25			046 xmax zone sensible
		dc.w	17			048 ymax zone sensible
		dc.w	11			050 pos y depart du tir
		dc.w	400			052 delai entre 2 tirs
		dc.w	SBOB.IN11
		dc.w	LISSA_01
		dc.l	00,-1
sugar2:		dc.w	30			S.POWER resistance du gardien
		dc.w	16			016
		dc.w	666			028 delai entre 2 tirs
		dc.w	2			030 vit. déplacement
		dc.w	8			042 xmin zone sensible
		dc.w	16			044 ymin zone sensible
		dc.w	24			046 xmax zone sensible
		dc.w	42			048 ymax zone sensible
		dc.w	24			050
		dc.w	240			052 delai entre 2 tirs
		dc.w	SBOB.IN12
		dc.w	LISSA_02
		dc.l	04,-1
*/ AERA 1 : Final */
sugar3:		dc.l	0
		dc.w	50			S.POWER
		dc.w	32			016
		dc.w	333			028
		dc.w	2			030
		dc.w	13			042
		dc.w	34			044
		dc.w	52			046
		dc.w	77			048
		dc.w	52			050
		dc.w	200			052
		dc.w	SBOB.FIN1
		dc.w	LISSA_03
		dc.l	00,04,08,-1
*/ AERA 2 : intermediaires */
sugar4		dc.l	sugar5-sugar4
		dc.w	30			S.POWER
		dc.w	16			016
		dc.w	444			028
		dc.w	2			030
		dc.w	9			042
		dc.w	5			044
		dc.w	23			046
		dc.w	30			048
		dc.w	21			050
		dc.w	300			052
		dc.w	SBOB.IN21
		dc.w	LISSA_02
		dc.l	12,-1
sugar5		dc.w	30			S.POWER
		dc.w	15			016
		dc.w	333			028
		dc.w	2			030
		dc.w	8			042
		dc.w	10			044
		dc.w	24			046
		dc.w	49			048
		dc.w	28			050
		dc.w	280			052
		dc.w	SBOB.IN22
		dc.w	LISSA_04
		dc.l	08,-1
*/ AERA 2 : final */
sugar6		dc.l	0
		dc.w	100			S.POWER
		dc.w	32			016
		dc.w	555			028
		dc.w	2			030
		dc.w	14			042
		dc.w	17			044
		dc.w	50			046
		dc.w	66			048
		dc.w	45			050
		dc.w	200			052
		dc.w	SBOB.FIN2
		dc.w	LISSA_15
		dc.l	16,00,08,04,12,-1
*/ AERA 3 : intermediaires */
sugar7		dc.l	sugar8-sugar7
		dc.w	35			S.POWER
		dc.w	16			016
		dc.w	333			028
		dc.w	2			030
		dc.w	6			042
		dc.w	5			044
		dc.w	26			046
		dc.w	30			048
		dc.w	19			050
		dc.w	300			052
		dc.w	SBOB.IN31
		dc.w	LISSA_11
		dc.l	20,08,-1
sugar8		dc.w	30			S.POWER
		dc.w	15			016
		dc.w	555			028
		dc.w	2			030
		dc.w	10			042
		dc.w	7			044
		dc.w	22			046
		dc.w	78			048
		dc.w	46			050
		dc.w	200			052
		dc.w	SBOB.IN32
		dc.w	LISSA_13
		dc.l	24,04,-1
*/ AERA 3 : final /*
sugar9		dc.l	0
		dc.w	70			S.POWER
		dc.w	32			016
		dc.w	444			028
		dc.w	2			030
		dc.w	13			042
		dc.w	22			044
		dc.w	52			046
		dc.w	61			048
		dc.w	41			050
		dc.w	250			052
		dc.w	SBOB.FIN3
		dc.w	LISSA_07
		dc.l	24,00,08,12,04,20
		dc.l	16,-1
*/ AERA 4 : intermediaires */
sugar10		dc.l	sugar11-sugar10
		dc.w	30			S.POWER
		dc.w	16			016
		dc.w	222			028
		dc.w	2			030
		dc.w	10			042
		dc.w	6			044
		dc.w	22			046
		dc.w	42			048
		dc.w	23			050
		dc.w	300			052
		dc.w	SBOB.IN41
		dc.w	LISSA_08
		dc.l	16,28,00,-1
sugar11		dc.w	40			S.POWER
		dc.w	16			016
		dc.w	333			028
		dc.w	2			030
		dc.w	7			042
		dc.w	12			044
		dc.w	25			046
		dc.w	68			048
		dc.w	42			050
		dc.w	400			052
		dc.w	SBOB.IN42
		dc.w	LISSA_04
		dc.l	12,32,08,-1
*/ AERA 4 : final */
sugar12		dc.l	0
		dc.w	60			S.POWER
		dc.w	32			016
		dc.w	222			028
		dc.w	2			030
		dc.w	13			042
		dc.w	30			044
		dc.w	52			046
		dc.w	73			048
		dc.w	49			050
		dc.w	160			052
		dc.w	SBOB.FIN4
		dc.l	00,04,32,16,20,08,28
		dc.w	LISSA_05
		dc.l	12,24,-1
*/ AERA 5 : intermédiaires */
sugar13		dc.l	sugar14-sugar13
		dc.w	40			S.POWER
		dc.w	16			016
		dc.w	222			028
		dc.w	2			030
		dc.w	7			042
		dc.w	14			044
		dc.w	25			046
		dc.w	55			048
		dc.w	31			050
		dc.w	360			052
		dc.w	SBOB.IN51
		dc.w	LISSA_08
		dc.l	36,20,-1
sugar14		dc.w	40			S.POWER
		dc.w	16			016
		dc.w	333			028
		dc.w	2			030
		dc.w	6			042
		dc.w	14			044
		dc.w	26			046
		dc.w	77			048
		dc.w	41			050
		dc.w	180			052
		dc.w	SBOB.IN52
		dc.w	LISSA_12
		dc.l	40,00,-1
*/ AERA 5 : Final */
sugar15		dc.l	0
		dc.w	70			S.POWER
		dc.w	31			016
		dc.w	333			028
		dc.w	2			030
		dc.w	22			042
		dc.w	82			044
		dc.w	42			046
		dc.w	102			048
		dc.w	55			050
		dc.w	200			052
		dc.w	SBOB.FIN5
		dc.w	LISSA_09
		dc.l	24,08,40,12,04,00
		dc.l	16,20,28,36,32,-1
*/ Final TecnoballZ */
sugar16		dc.l	0
		dc.w	150
		dc.w	32	
		dc.w	222
		dc.w	2	
		dc.w	11	
		dc.w	68	
		dc.w	54	
		dc.w	128
		dc.w	83	
		dc.w	150
		dc.w	SBOB.FIN6
		dc.w	LISSA_12
		dc.l	20,04,36,08,32,12,24
		dc.l	16,00,28,40,-1

*/ table sinus => offset x déplacement des gardiens */
		dc	0
offzet:		incbin	Raw1/Offset.table
offvet
*/ table cosinus / sinus => utilisée pour les tirs */
tabcos:		incbin	Raw1/DataSin.table
tabsin		=tabcos+720

*/  tir gardien : table sinus */
tir01_pos:	dc.w	32,32,32,31,31,30,29,28,27,25
		dc.w	24,22,21,19,17,16,14,12,11,09
		dc.w	08,06,05,04,03,02,01,01,00,00
		dc.w	00,00,01,01,02,03,04,05,06,08
		dc.w	09,11,12,14,16,17,19,21,22,24
		dc.w	25,27,28,29,30,31,31,32,32,32
		dc.w	99,99

*/ tir gardien :  table de points d'un cercle de 8 pixels de rayon */
tir02_pos	dc.w	008,000,008,-01,006,-03,004,-04,003,-06,001,-08,000,-08
		dc.w	000,-08,-01,-08,-03,-06,-04,-04,-06,-03,-08,-01,-08,000
		dc.w	-08,000,-08,001,-06,003,-04,004,-03,006,-01,008,000,008
		dc.w	000,008,001,008,003,006,004,004,006,003,008,001,008,000
		dc.w	99,99,99,99

*/ tir gardien : table de points d'un cercle de rayon variable */
tir03_pos	dc.w	008,000,008,-01,006,-03,004,-04,003,-06,001,-08,000,-08
		dc.w	000,-08,-01,-08,-03,-06,-04,-04,-06,-03,-08,-01,-08,000
		dc.w	-08,000,-08,001,-06,003,-04,004,-03,006,-01,008,000,008
		dc.w	000,008,001,008,003,006,004,004,006,003,008,001,008,000
		dc.w	99,99,99,99

*/ tir gardien : table des directions de la roue accelerante */
tir04_pos	dc.w	-1,05,03,05,01,06,-2,06,00,06,-1,04,02,06

*/ tir gardien : position des points, epee de DAMOCLES */
tir10_pos	dc.w	07,00,00,07,07,07,14,07,07,14,07,21

*/ Table des positions de son explosion */
elegy_table:	dc.w	0,0,10,20,20,04,30,25,04,15,17,18,14,35,99,99

*-------------------* DECLARE STRUCTURES DES GARDIENS *-------------------*
		RSRESET
GARD.BOB	rs.l	1	Adresse structure Shadow-SBOB
GARD.SPR0	rs.l	1	Lutin paire
GARD.SPR1	rs.l	1	Lutin impaire
GARD.FIRE	rs.l	1	Adresse liste PT des tirs
GARD.GFX	rs.l	1	Adresse GFX dans la page BOB
GARD.POW	rs.w	1	Hauteur du témoin resistance
GARD.WIDTH	rs.w	1	Largeur Gardien en pixels
GARD.HEIGHT	rs.w	1	Hauteur Gardien en lignes
GARD.X		rs.w	1	Position x écran
GARD.Y		rs.w	1	Position y écran
GARD.MINX	rs.w	1	Minimum x
GARD.MAXX	rs.w	1	Maximum x
GARD.MINY	rs.w	1	Minimum y
GARD.MAXY	rs.w	1	Maximum y
GARD.POWER	rs.w	1	Résistance du Gardien	(S.POWER)
GARD.XFIRE	rs.w	1	Pos. x départ du tir	(S.XCENT)
GARD.YFIRE	rs.w	1	Pos. y départ du tir	(S.YCENT)
GARD.WAIT	rs.w	1	Fréquence tirs		(S.WAITF)
GARD.SPEED	rs.w	1	Vitesse déplacement	(S.SPEED)
GARD.LISSA	rs.l	1	Adresse de la courbe
GARD.LISSA2	rs.l	1	Adresse initiale de la courbe
GARD.TOUCH	rs.w	1	Flag : touché et clignote
GARD.X1		rs.w	1	Xmin zone sensible	(S.COLX1)
GARD.Y1		rs.w	1	Ymin zone sensible	(S.COLY1)
GARD.X2		rs.w	1	Xmax zone sensible	(S.COLX2)
GARD.Y2		rs.w	1	Ymax zone sensible	(S.COLY2)
GARD.WAIT2	rs.w	1	Délai entre 2 tirs	(S.WAIT2)
GARD.PTFIRE	rs.w	1	Pointeur tirs
GARD.LOVE	rs.w	1
GARD.EXPTIME	rs.w	1	Durée explosion
GARD.PTEXP	rs.w	1	Pointeur table explosion
GARD.WAITF	rs.w	1	Délai initiale tir GigaBlitz
GARD.WAITF2	rs.w	1	Délai tir GigaBlitz
GARD.SIZE	rs	0
*-----------------* Déclare Structure Tirs des Gardiens *-----------------*
		RSRESET
MIS:		rs.b	SBOB.SIZE
MIS.X		=SBOB.POSX	position X
MIS.Y		=SBOB.POSY	position Y
MIS.ACTIF	=SBOB.ACTIF	flag : 1=tir actif
MIS.FD1		rs.w	1	flag : deplacement
MIS.FD2		rs.w	1	flag : deplacement
MIS.FD3		rs.w	1	flag : deplacement
MIS.FD4		rs.w	1	flag : deplacement
MIS.FD5		rs.w	1	flag : deplacement
MIS.FD6		rs.w	1	flag : deplacement
MIS.FD7		rs.w	1	flag : deplacement
MIS.SIZE	rs	0
*-----------------------* Déclare Structure Balle *-----------------------*
		RSRESET
SBALLE:		rs.b	SBOB.SIZE
SBALLE.X	=BOB.POSX ^	position X de la balle
SBALLE.Y	=BOB.POSY !	position Y de la balle
SBALLE.ACTIF	=BOB.ACTIF	flag balle active
SBALLE.DIRECT	rs.l	1	pt direction de 0 à 56 step 4
SBALLE.RAK	rs.l	1	pt/sructure raquette
SBALLE.RAKGLUE	rs.l	1	colle option
SBALLE.SPEED	rs.l	1	pt/table vitesse
SBALLE.PBROTA	rs.l	1	rot. pt sprite
SBALLE.PB1	rs.l	1	adresse pointeur balle écran 1
SBALLE.PB2	rs.l	1	adresse pointeur balle écran 2
SBALLE.GLUE	rs.w	1	flag balle collée
SBALLE.PBX	rs.w	1	pointeur balle x
SBALLE.PBTEMP	rs.w	1	tempo. dépl. pt. balle
SBALLE.TILT	rs.w	1	tilt
SBALLE.PBY	rs.w	1	pointeur balle y
SBALLE.SCOUNT	rs.w	1	temps que dure la vitesse
SBALLE.START	rs.w	1	temps avant que la balle parte
SBALLE.POWER	rs.w	1
SBALLE.SIZE	rs	0

*----------------* Déclare structure paramètres Gardiens *----------------*
		RSRESET
S.POWER		rs.w	1	Résistance
S.XCENT		rs.w	1	Centre X départ tir
S.WAITF		rs.w	1	Fréquence Tir GigaBlitz
S.SPEED		rs.w	1	Vitesse de déplacement
S.COLX1		rs.w	1	Fenêtre de collision x-mini
S.COLY1		rs.w	1	Fenêtre de collision y-mini
S.COLX2		rs.w	1	Fenêtre de collision x-maxi
S.COLY2		rs.w	1	Fenêtre de collision y-maxi
S.YCENT		rs.w	1	Centre Y départ tir
S.WAIT2		rs.w	1	Fréquence tir
S.NSBOB		rs.w	1	Numéro du BOB   
S.LISSA		rs.w	1	Numéro de la courbe
S.TFIRE		rs	0	Table des pointeurs sur la liste tirs
