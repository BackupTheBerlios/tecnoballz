*//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/*
************************ CONSTANTES DE TECNOBALLZ *************************
*//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/*
*									  *
*****************************  HAUTS-SCORES  ******************************
*									  *
IS		=8		taille identificateur (Somme ctrl + Id.)
KS		=22		ligne titre (taille ligne "EASY MODE TOP TEN"
NS		=28		taille ligne score & ligne intitulé
TS		=10
LS		=TS+1		11 lignes (10 scores + intitulé)
OFF.SCORE	=KS+(NS*LS)
SIZE_FSCORE	=IS+(OFF.SCORE*4)
*									  *
*»<»<»<»<»<»<»<»<»<»<»<» COMMUN TABLEAUX-GARDIENS >«>«>«>«>«>«>«>«>«>«>«>«*
*		
PT_SIN		=766
LIFE_POINTS	=25000		Gagne une vie tous les 25000 points
*/ NOMBRE D'OBJETS */
NLETTRE		=8		Lettres 'GAME OVER'
NBALLE		=20		Balles des tableaux
NSBALL		=10		Balles des gardiens
NTIR		=7		Tirs des raquettes
NATOM		=8		Nombre d'atoms
NGEMME		=8
NMONNAIE	=6		Capsules monnaies
NGADGET		=6		Gadgets Bonus & Malus
NJOUEUR		=6		Joueurs
NOPTIONS	=20		maximum d'options achetées au magasin
NGAD		=NOPTIONS+2
NBR_MISSILE	=40		Tirs des Gardiens
NBR_EXPLOMON	=14		Explosions des Gardien
NBR_BOULE	=8
*/ FLAG BOUTONS SOURIS */
BOUTON_LACHE	=2		Bouton droit
BOUTON_TIR	=1		Bouton gauche
BOUTON_BLITZ	=3		Bouton gauche & droit
BOUTON_TILT	=3		Bouton gauche & droit
*									  *
*<»<»<»<»<»<»<»<»<»<»<»<»<»<»<»<»< MENU >«>«>«>«>«>«>«>«>«>«>«>«>«>«>«>«>«*
*									  *
*
*/ STRUCTURES */
VK.NUM		=0	Nombre de ligne -1 du menu
VK.TEXT		=4	pt/le texte ASCII
VK.JMP		=8	table saut routine
VKN		=20	20 LIGNES MAXIMUM pour les menus
VKH		=9 
AFNOR		=14	Nombre de lettres SBOB du Scroll-Text
*									  *
*<»<»<»<»<»<»<»<»<»<»<»<»<»<»<»< GARDIENS >«>«>«>«>«>«>«>«>«>«>«>«>«>«>«>«*
*									  *
DRAX1		=8		Hauteur du Nom
DRAX2		=150		Hauteur de la barre resistance des Gardiens
DRAX		=1+DRAX1+1+DRAX2+1
HSGO		=102		Hauteur du lutin GameOver HiScore
ODECAFEINE	=6		Décalage ombre = 6 pixels
*/ MAP-EDITOR (pointeur 'mapeditor') */
M_LARGE		=020		Nombre de dalles en largeur
M_HAUTEUR	=273		Nombre de dalles en hauteur
*/ GARDIEN */
GARD_I1		=00		Intermédiare area 1
GARD_F1		=04		Final area 1
GARD_I2		=08		Intermédiaire area 2
GARD_F2		=12		Final area 2
GARD_I3		=16		Intermédiaire area 3
GARD_F3		=20		Final area 3
GARD_I4		=24		Intermédiaire area 4
GARD_F4		=28		Final area 4
GARD_I5		=32		Intermédiaire area 5
GARD_F5		=36		Final area 5
GARD_FF		=40		Final TecnoballZ
*/ NUMERO DES SCROLL-BOBS (SBOB) */
SBOB.EXP1	=0
SBOB.EXP2	=1
SBOB.BUMP	=2		Raquette
SBOB.BALL	=3
SBOB.FIRE	=4
SBOB.CAPS	=5		Monnaie
SBOB.GGGG	=6
SBOB.AAAA	=7
SBOB.MMMM	=8
SBOB.EEEE	=9
SBOB.OOOO	=10
SBOB.VVVV	=11
SBOB.EEE2	=12
SBOB.RRRR	=13
SBOB.FIN1	=14		Gardiens finaux
SBOB.FIN2	=15
SBOB.FIN3	=16
SBOB.FIN4	=17
SBOB.FIN5	=18
SBOB.FIN6	=19
SBOB.IN11	=20		Gardiens intermédiaires
SBOB.IN12	=21
SBOB.IN21	=22
SBOB.IN22	=23
SBOB.IN31	=24
SBOB.IN32	=25
SBOB.IN41	=26
SBOB.IN42	=27
SBOB.IN51	=28
SBOB.IN52	=29
SBOB.LIFE	=30
SBOB.TIR2	=31
SBOB.BOUL	=32
SBOB.TECN	=33
SBOB.MOUS	=34
SBOB.FONT	=35
SBOB.GADG	=36
SBOB.TEXT	=37
*/ Pointeur GadGet */
SP.VIDE00	=-1
SP.PROTEC	=00*56
SP.BALLE3	=01*56
SP.LIFE_P	=02*56
SP.POWER1	=03*56
SP.POWER2	=04*56
SP.INVERS	=05*56
*/ NOMBRE MAXIMUM DE SCROLL-BOB PAR LARGEUR /*
SBOB_MAXI	=100
SBOB_MAXI16	=70
SBOB_MAXI32	=50
SBOB_MAXI48	=0
SBOB_MAXI64	=10
SBOB_MAXI80	=0
SBOB_MAXI256	=4
VSBOB_MAXI16	=0
VSBOB_MAXI32	=0
VSBOB_MAXI48	=0
VSBOB_MAXI64	=0
VSBOB_MAXI80	=0
VSBOB_MAXI96	=0
*/ PONTEURS SUR TABLE COURBES /*
LISSA_01	=0
LISSA_02	=1
LISSA_03	=2
LISSA_04	=3
LISSA_05	=4
LISSA_07	=5
LISSA_08	=6
LISSA_09	=7
LISSA_11	=8
LISSA_12	=9
LISSA_13	=10
LISSA_15	=11
TIR_MINI_X	=002
TIR_MAXI_X	=303
TIR_MINI_Y	=002
TIR_MAXI_Y	=240
*									  *
*»<»<»<»<»<»<»<»<»<»<»<»<»<»<»<» TABLEAUX >«>«>«>«>«>«>«>«>«>«>«>«>«>«>«>«*
*
*/ TETE */
TETE_PARASITE1	=04
TETE_PARASITE2	=12
TETE_BAILLE1	=16
TETE_BAILLE2	=32
TETE_RIRE1	=36
TETE_RIRE2	=52
TETE_SPEED	=5		Vitesse d'animation
TETE_SPEED2	=20		Temps reste sur la dernière animation
*/ TAILLE ECRAN */
NLIGNE		=256
NPLAN		=5
TPLAN		=40
TLIGNE		=40*NPLAN
OPLAN		=(TPLAN*3)	Plan sur lequel s'affiche les ombres
NOPLAN		=3		Les ombres se font sur le plan 4
ODECA		=3		Décalage ombre = 3 pixels
*/ GIGABLITZ */
TEMPO.BLITZ	=10		Vitesse du témoin GigaBlitz
YMAX.BLITZ	=16		Hauteur maxi du GigaBlitz
POS.GIGABLITZ	=226		Position numéro ligne témoin du GigaBlitz
NLINE.BLITZ	=27		Nombre de lignes du témoin du GigaBlitz
HGB1		=032	16	Hauteur des 7 GigaBlitz
HGB2		=048	24
HGB3		=064	32
HGB4		=080	40
HGB5		=096	48
HGB6		=112	56
HGB7		=128	64
TGB1		=(HGB1+2)*16	Taille des 7 Lutins GigaBlitz
TGB2		=(HGB2+2)*16
TGB3		=(HGB3+2)*16
TGB4		=(HGB4+2)*16
TGB5		=(HGB5+2)*16
TGB6		=(HGB6+2)*16
TGB7		=(HGB7+2)*16
TGB		=(TGB1+TGB2+TGB3+TGB4+TGB5+TGB6+TGB7)*2
*/ BALLES */
XMIN		=005		Limite gauche balle
XMAX		=250		Limite droite balles 
YMIN		=008		Limite haute  balle
YMAX		=245		Limite basse balle
XMIN2		=016				gardien : limite gauche balle
XMAX2		=300				gardien : limite droite balle
*/ RAQUETTES */
YRAKH		=020		Position Y Bumper Haut
YRAKB		=232		Position U Bumper Bas
XRAKG		=020		Position X Bumper Gauche
XRAKD		=228		Position X Bumper Droite
BP_MAX		=224
BP_MIN		=032
BP_MAX2		=300
BP_MIN2		=016
YROBOT		=YRAKB+8
YINVERS		=247
*/ BRICOTES */
MUR_POSY	=YRAKB+8
BRICOT.NUM	=12		Nombres de Bricotes dans un mur
BCG.X		=12
BCG.Y		=32
BCD.X		=240
BCD.Y		=32
BCH.X		=32
BCH.Y		=12
CNP		=12*3		Nombres Bricotes dans la liste d'effacement
*/ COORDONNEES DES EJECTEURS */
EJ1X		=016		Haut-Gauche
EJ1Y		=016
EJ2X		=224		Haut-Droite
EJ2Y		=016
EJ3X		=016		Bas-Gauche
EJ3Y		=224
EJ4X		=224		Bas-Droite
EJ4Y		=224
*/ BOUIBOUI-ATOM */
ATOM_XMIN	=032
ATOM_XMAX	=191
ATOM_YMIN	=040
ATOM_YMAX	=191
*/ NUMERO DES BOBS */
BOB.ATOMS	=0
BOB.EJECT1	=1	haut-gauche
BOB.EJECT3	=2	bas-gauche
BOB.EJECT4	=3	bas-droite
BOB.EJECT2	=4	haut-droite
BOB.VBRICOTE	=5
BOB.HBRICOTE	=6
BOB.BRICK	=7	brique jeu
BOB.HRAK	=8
BOB.VRAK	=9
BOB.BALLE	=10
BOB.FIRE	=11
BOB.MONNAI	=12
BOB.GADGET	=13
BOB.TEXT	=14
BOB.LED1	=15
BOB.PT1		=16
BOB.PT2		=17
BOB.G		=18
BOB.A		=19
BOB.M		=20
BOB.E		=21
BOB.O		=22
BOB.V		=23
BOB.E2		=24
BOB.R		=25
BOB.INVERS	=26
BOB.CURS	=27
BOB.MUR		=28
BOB.STAR	=29
BOB.ROBOT	=30
BOB.GEMME	=31
BOB.OEIL	=32
*/ POINTEUR SUR CHACUN DES 12 BOBS UNE ANIMATION=7*8=56 */
P.VIDE00	=-1
P.SIZE_P	=00*56
P.SIZE_M	=01*56
P.LIFE_P	=02*56
P.LIFE_M	=03*56
P.POWER1	=04*56
P.POWER2	=05*56
P.BALLE2	=06*56
P.BALLE3	=07*56
P.FIRE01	=08*56
P.GLUE00	=09*56
P.INVERS	=10*56
P.FIRE02	=11*56
P.SIZE01	=12*56
P.SIZE02	=13*56
P.RANDOM	=14*56
P.MEGA00	=15*56
P.WALL01	=16*56
P.ROBOT1	=17*56
P.CONTRO	=18*56

PFIRE1		=32		pt/animation BOB TIR 1
PFIRE2		=00		pt/animation BOB TIR 2
O.ATOM1		=00*64
O.ATOM2		=01*64
O.ATOM3		=02*64
O.ATOM4		=03*64
O.ATOM5		=04*64
O.ATOM6		=05*64
O.ATOM7		=06*64
O.ATOM8		=07*64
O.ATOM9		=08*64
O.ATOM10	=09*64
O.ATOM11	=10*64
O.ATOM12	=11*64
O.ATOM13	=12*64
O.ATOM14	=13*64
O.ATOM15	=14*64
O.ATOM16	=15*64
O.ATOM17	=16*64
O.ATOM18	=17*64
O.ATOM19	=18*64
O.ATOM20	=19*64
O.EXPLO1	=20*64
O.EXPLO2	=21*64
*/ BRIQUES */
BK.SPLAN	=14		Largeur page brique
BK.NLINE	=63		Hauteur page brique
BK.NPLAN	=5		Nombre de plans page brique
BK.SLINE	=BK.NPLAN*BK.SPLAN
BK.SIZE		=BK.SLINE*BK.NLINE
TLIGNEB		=BK.SLINE
TPLANB		=BK.SPLAN
BK.X		=16		Nombre de colonnes de briques à l'écran
BK.Y		=32		Nombre de lignes de briques à l'écran
TB.X		=10		Largeur d'un tableau
TB.Y		=17		Hauteur d'un tableau
TBC		=BK.Y+1		Hauteur du dégradé
TB.SIZE		=TB.X*TB.Y*2
NIVEA		=TB.X*TB.Y	taille en mot d'1 tableau 
REA		=5		nbre d'areas
BEAU		=10		nbre de tableau/area
TNT		=REA*BEAU	nbre de tableaux
BK.POSX		=0
BK.POSY		=0
BK.HEIGHT	=7		Hauteur d'une brique
NID		=BK.X*BK.Y	Nombre de briques maxi. à l'écran
BRIY		=56
BRIY2		=191
BNP		=256		nbre briks ds table d'effacem.
CONORS		=2*2*7		espace entre chaque brique sauvée
RIZ		=BK.X*4		Largeur en octets ligne map-editor
BRI.MSK		=$0FF0		=4080 (table de 4096 octets de 16 en 16)
SH.TEXT		=22

*/ DALLES DE FONDS 4 COULEURS */
MAP60.X		=8	Nombre de colonnes de dalles affichées 
MAP60.Y		=8	Nombre de lignes de dalles affichées
MAP60.HAUT	=32	Hauteur d'une dalle en lignes
MAP60.LARG	=4	Largeur d'une dalle en octets
*/ NOMBRE MAXIMUM DE BOBs PAR LARGEUR /*
BOB32_MAXI	=80
BOB32_MAXI16	=40
BOB32_MAXI32	=10
BOB32_MAXI48	=0
BOB32_MAXI64	=10
BOB32_MAXI80	=0
BOB32_MAXI96	=0
BOB32_MAXI192	=4
BOB32_MAXI256	=0
VBOB32_MAXI16	=0
VBOB32_MAXI32	=0
VBOB32_MAXI48	=0
VBOB32_MAXI64	=0
VBOB32_MAXI80	=0
VBOB32_MAXI96	=0
*>>>>>>>>>>>>>>>>>>>>>>>>>>* CONSTANTES MENTAT *<<<<<<<<<<<<<<<<<<<<<<<<<<*
NReservation	=150	nombre adresses mémoires réservées maximums
FLAG_AMIGADOS	=1	1=assemble routines chargement AmigaDOS
FLAG_ARP	=0	1=ouvre la librairie ARP
FLAG_BOB32	=1	1=assemble routines BOBs (aff. écran 32 bits)
FLAG_BOBSTRUCT	=1	1=Assemble la structure BOB
FLAG_DMACON.MSK	=0	1=ferme dma bitplane lors d'un accès disque-système
FLAG_EXIT	=0	0=sortie sous système possible avec la touche "ESC"
FLAG_FREQ	=0	1=assemble routine demande fréquence balayage
FLAG_ILBM	=1	1=assemble routines décompactage IFF-ILBM
FLAG_KEY	=1	1=assemble routines de test du clavier
FLAG_LUTIN	=1	1=assemble routines gestion des lutins
FLAG_MEMORY	=1	1=information sur la mémoire possible
FLAG_MOUSE0	=0	1=assemble la routine déplacement souris port 0
FLAG_MUSIC	=0	1=assemble routines gestion module "ProTracker"
FLAG_PP		=1	1=assemble routines décompactage PowerPacker
FLAG_SOUND	=0	1=assemble routines gestion des bruitages
FLAG_SCROLL	=0	1=assemble routines gestion défilement écran
FLAG_TDISK	=0	1=assemble routines de lecture en "TrackDisk"
FLAG_TDISKW	=0	1=assemble routines d'écriture en "TrackDisk"
FLAG_TRACKDISK	=0	1=assemble programme pour être chargé en "TrackDisk"
*/ NUMERO DES BRUITAGES */
S_ENLEVE.VIE	=01
S_RAK.EXPLO	=02
S_AJOUTE.VIE	=03
S_MONNAIE	=04
S_TILT.ALARM	=05
S_GADGET	=06
S_TECNO		=07
S_ATOM.APPARAIT	=08
S_TRANSFORME	=09
S_TIR.GARDIEN	=10
S_RAK.TIR	=11
S_BIG.EXPLO	=12
S_CASSE.INDEST	=13
S_ATOM.EXPLO	=14
S_GARDIEN.TOUCH	=15
S_TOUCHE.ATOM	=16
S_TOUCHE.INDES1	=17
S_TOUCHE.INDES2	=18
S_TOUCHE.RAK	=19
S_BRICOTE	=20
S_TOUCHE.BRIK	=21
S_TOUCHE.BRIK2	=22
S_TOUCHE.BRIK3	=23
S_TOUCHE.BRIK4	=24
S_TOUCHE.BRIK5	=25
S_COIN.EJECT	=26
S_COIN.ASPIRE	=27
S_NOMBRE	=28
*/ TAILLE DES BRUITAGES */
		RSRESET
GADGET_SOUND	rs.b	5834
DETRUIT_INDES	rs.b	1720
TECNO		rs.b	3688
LIFE_PLUS	rs.b	5212
LIFE_MOINS	rs.b	2494
APPAR		rs.b	2484
EXPLO_ATOM	rs.b	1794
INDES_1		rs.b	1730
INDES_2		rs.b	1732
TIR_MONSTRE	rs.b	3134
ALARM		rs.b	2620
EXPLO_RAK	rs.b	1866
TRANSFO		rs.b	5726
TIR_RAK		rs.b	1446
RAQUETTE	rs.b	2628
BRICOTE		rs.b	4096
BRIQUE		rs.b	1052
ATOM		rs.b	0392
EJECT		rs.b	1802
ASPIRE		rs.b	0300
EXPLO_BIG	rs.b	3236
MONSTRE_TOUCHE	rs.b	1748
