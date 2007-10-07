***************************************************************************
*------------------------>> Données des levels <<-------------------------*
***************************************************************************
pt.course:	dc.w	0	pt/table bonus
bonus.tombe	dc.w	0	nbre bonus tombés
bonus.achete	dc.w	0	nbre bonus achetés
bonus.step	dc.w	0	espace entre lachés de gad. 
malus.step	dc.w	0	compt. à 10 lache 1 malus
atom1.app	dc.w	0	temps d'apparition atom00
atom2.app	dc.w	0	temps d'apparition atom01
atom3.app	dc.w	0	temps d'apparition atom02
atom4.app	dc.w	0	temps d'apparition atom03
atom.reapp	dc.w	0	temps de réapparition
atom.resist	dc.w	0	résistance des atoms
malus.frek	dc.w	0	fréquence de laché de malus
monai.frek	dc.w	0	fréquence laché de monnaie
table.malus	dc.l	0	table malus
balle_speed:	dc.l	0	première vitesse de balle
balle_scount	dc.w	0	temps avant que la balle accellere
balle_start	dc.w	0	temps avant que balle parte
balle_glue	dc.w	0	temps que dure la colle
balle_tilt	dc.w	0	temps avant tilt possible

*/ TABLE DES BONUS ACHETES */
course:		dcb.w	NOPTIONS,0
		dc.w	-1	Fin de la table

*/ TABLE POINTEURS SUR ANIMATIONS DU BOB GADGET */
pascal:		dc.w	0		00 *inutilisé*
		dc.w	P.GLUE00	02 colle
		dc.w	0		04 *inutilisé*
		dc.w	P.FIRE01	06 tir puissance 1
		dc.w	P.FIRE02	08 tir puissance 2
		dc.w	P.SIZE_M	10 rétrécit la raquette
		dc.w	P.SIZE_P	12 allonge la raquette
		dc.w	P.LIFE_M	14 enlève une vie
		dc.w	P.LIFE_P	16 ajoute une vie
		dc.w	P.BALLE2	18 ajoute 2 balles
		dc.w	P.BALLE3	20 ajoute 3 balles
		dc.w	P.POWER1	22 balle puissance 1
		dc.w	P.POWER2	24 balle puissance 2
		dc.w	P.INVERS	26 inverseur de commandes
		dc.w	0		28 REBUILD MUR (ONLY SHOP)
		dc.w	0		30 Vitesse MAXI (pas de Gadgets)
		dc.w	0		32 BUMP1 ACTIF (pas de Gadgets)
		dc.w	0		34 BUMP2 ACTIF (pas de Gadgets)
		dc.w	0		36 BUMP3 ACTIF (pas de Gadgets)
		dc.w	0		38 BUMP4 ACTIF (pas de Gadgets)
		dc.w	P.SIZE01	40 Balle taille 2
		dc.w	P.SIZE02	42 Balle taille 3
		dc.w	P.RANDOM	44 Random
		dc.w	P.MEGA00	46 Toutes les options
		dc.w	0		48 Bonus Price=1 (pas de Gadgets)
		dc.w	P.WALL01	50 Mur du bas actif
		dc.w	P.ROBOT1	52 Robot actif
		dc.w	P.CONTRO	54 Contrôle balles
		dc.w	0		56 Oeil actif
		dc.w	0		58 LESS-BRICK (SHOP ONLY)
		dc.w	0		60 INFOS (SHOP ONLY)
		dc.w			62 EXIT (SHOP ONLY)

*/ TABLE POINTEURS SUR ANIMATIONS DU SBOB GADGET DES GARDIENS */
lauly:		dc.w	0		00 *inutilisé*
		dc.w	0		02 *inutilisé*
		dc.w	SP.PROTEC	04 Invinsibilité
		dc.w	0		06 *inutilisé*
		dc.w	0		08 *inutilisé*
		dc.w	0		10 *inutilisé*
		dc.w	0		12 *inutilisé*
		dc.w	0		14 *inutilisé*
		dc.w	SP.LIFE_P	16 ajoute une vie
		dc.w	0		18 *inutilisé*
		dc.w	SP.BALLE3	20 ajoute 3 balles
		dc.w	SP.POWER1	22 balle puissance 1
		dc.w	SP.POWER2	24 balle puissance 2
		dc.w	SP.INVERS	26 inverseur de commandes

*/ jeu : pointeurs pour chaque tableaux sur table paramètres */
giga_amiga:	dc.l	amiga_10-t		area 1 ; level 1
		dc.l	amiga_11-t		area 1 ; level 2
		dc.l	amiga_11-t		area 1 ; level 3
		dc.l	amiga_11-t		area 1 ; level 4
		dc.l	amiga_11-t		area 1 ; level 5
		dc.l	atari_00-t		area 1 ; level 6
		dc.l	amiga_12-t		area 1 ; level 7
		dc.l	amiga_12-t		area 1 ; level 8
		dc.l	amiga_12-t		area 1 ; level 9
		dc.l	amiga_12-t		area 1 ; level 10
		dc.l	amiga_12-t		area 1 ; level 11
		dc.l	atari_04-t		area 1 ; level 12
		dc.l	amiga_20-t		area 2 ; level 1
		dc.l	amiga_20-t		area 2 ; level 2
		dc.l	amiga_20-t		area 2 ; level 3
		dc.l	amiga_20-t		area 2 ; level 4
		dc.l	amiga_20-t		area 2 ; level 5
		dc.l	atari_08-t		area 2 ; level 6
		dc.l	amiga_20-t		area 2 ; level 7
		dc.l	amiga_20-t		area 2 ; level 8
		dc.l	amiga_20-t		area 2 ; level 9
		dc.l	amiga_20-t		area 2 ; level 10
		dc.l	amiga_20-t		area 2 ; level 11
		dc.l	atari_12-t		area 2 ; level 12
		dc.l	amiga_30-t		area 3 ; level 1
		dc.l	amiga_30-t		area 3 ; level 2
		dc.l	amiga_30-t		area 3 ; level 3
		dc.l	amiga_30-t		area 3 ; level 4
		dc.l	amiga_30-t		area 3 ; level 5
		dc.l	atari_16-t		area 3 ; level 6
		dc.l	amiga_30-t		area 3 ; level 7
		dc.l	amiga_30-t		area 3 ; level 8
		dc.l	amiga_30-t		area 3 ; level 9
		dc.l	amiga_30-t		area 3 ; level 10
		dc.l	amiga_30-t		area 3 ; level 11
		dc.l	atari_20-t		area 3 ; level 12
		dc.l	amiga_40-t		area 4 ; level 1
		dc.l	amiga_40-t		area 4 ; level 2
		dc.l	amiga_40-t		area 4 ; level 3
		dc.l	amiga_40-t		area 4 ; level 4
		dc.l	amiga_40-t		area 4 ; level 5
		dc.l	atari_24-t		area 4 ; level 6
		dc.l	amiga_40-t		area 4 ; level 7
		dc.l	amiga_40-t		area 4 ; level 8
		dc.l	amiga_40-t		area 4 ; level 9
		dc.l	amiga_40-t		area 4 ; level 10
		dc.l	amiga_40-t		area 4 ; level 11
		dc.l	atari_28-t		area 4 ; level 12
		dc.l	amiga_50-t		area 5 ; level 1
		dc.l	amiga_50-t		area 5 ; level 2
		dc.l	amiga_50-t		area 5 ; level 3
		dc.l	amiga_50-t		area 5 ; level 4
		dc.l	amiga_50-t		area 5 ; level 5
		dc.l	atari_32-t		area 5 ; level 6
		dc.l	amiga_55-t		area 5 ; level 7
		dc.l	amiga_55-t		area 5 ; level 8
		dc.l	amiga_55-t		area 5 ; level 9
		dc.l	amiga_55-t		area 5 ; level 10
		dc.l	amiga_55-t		area 5 ; level 11
		dc.l	atari_36-t		area 5 ; level 12
		dc.l	atari_40-t		area 5 ; level 13

*/ DECLARE STRUCTURE PARAMETRE */
		RSRESET
PA:		rs	0
PA.ATOM1	rs.w	1	Temps apparition atom 1
PA.ATOM2	rs.w	1	Temps apparition atom 2
PA.ATOM3	rs.w	1	Temps apparition atom 3
PA.ATOM4	rs.w	1	Temps apparition atom 4
PA.REAPP	rs.w	1	Temps réapparition
PA.RESIS	rs.w	1	Resistance des atoms
PA.FREK1	rs.b	1	Fréquence de laché de malus
PA.FREK2	rs.b	1	Fréquence de laché de monnaies
PA.MALUS	rs.l	1	Table des malus
PA.SPEED	rs.l	1 	Première vitesse de balle
PA.COUNT	rs.w	1 	Temps avant accellération de la balle
PA.START	rs.w	1 	Temps avant que balle parte
PA.TGLUE	rs.w	1	Temps que dure la colle
PA.TTILT	rs.w	1 	Temps avant que le tilt soit possible
PA.SIZE		rs.w	0
;		- { area 1 : tableau 1 } -
amiga_10:	dc.w	01*50
		dc.w	45*50
		dc.w	35*50
		dc.w	10*50
		dc.w	80*50
		dc.w	1
		dc.b	8
		dc.b	2
		dc.l	course10-t
		dc.l	speed1-t
		dc.w	50*99
		dc.w	50*20
		dc.w	50*50
		dc.w	07*50
*/ area 1 : tableau 2 à 5 */
amiga_11:	dc.w	01*50
		dc.w	45*50
		dc.w	25*50
		dc.w	12*50
		dc.w	80*50
		dc.w	1
		dc.b	15
		dc.b	5
		dc.l	course11-t	14 table malus
		dc.l	speed1-t
		dc.w	50*77
		dc.w	50*15
		dc.w	50*40
		dc.w	07*50
*/ area 1 : tableau 7 à 11 */
amiga_12:	dc.w	01*50
		dc.w	35*50
		dc.w	25*50
		dc.w	12*50
		dc.w	70*50
		dc.w	1
		dc.b	17
		dc.b	6
		dc.l	course12-t
		dc.l	speed1-t
		dc.w	50*67
		dc.w	50*10
		dc.w	50*30
		dc.w	07*50
*/ area 2 */
amiga_20:	dc.w	01*50
		dc.w	40*50
		dc.w	30*50
		dc.w	10*50
		dc.w	50*50
		dc.w	5
		dc.b	19
		dc.b	7
		dc.l	course20-t
		dc.l	speed1-t
		dc.w	50*67
		dc.w	50*10
		dc.w	50*10
		dc.w	10*50
*/ area 3 */
amiga_30:	dc.w	01*50
		dc.w	30*50
		dc.w	20*50
		dc.w	10*50
		dc.w	40*50
		dc.w	10
		dc.b	20
		dc.b	8
		dc.l	course30-t
		dc.l	speed1-t
		dc.w	50*50
		dc.w	50*10
		dc.w	50*10
		dc.w	10*50
*/ area 4 */
amiga_40:	dc.w	01*50
		dc.w	21*50
		dc.w	14*50
		dc.w	17*50
		dc.w	30*50
		dc.w	20
		dc.b	15
		dc.b	8
		dc.l	course40-t
		dc.l	speed1-t
		dc.w	50*50
		dc.w	50*10
		dc.w	50*07
		dc.w	11*50
*/ area 5 : tableau 1 à 5 */
amiga_50:	dc.w	01*50
		dc.w	15*50
		dc.w	10*50
		dc.w	05*50
		dc.w	20*50
		dc.w	30
		dc.b	15
		dc.b	8
		dc.l	course50-t
		dc.l	speed1-t
		dc.w	50*40
		dc.w	50*10
		dc.w	50*07
		dc.w	11*50
*/ area 5 : tableau 7 à 11 */
amiga_55:	dc.w	01*50
		dc.w	04*50
		dc.w	03*50
		dc.w	02*50
		dc.w	05*50
		dc.w	50
		dc.b	15
		dc.b	15
		dc.l	course55-t
		dc.l	speed2-t
		dc.w	50*30
		dc.w	50*10
		dc.w	50*05
		dc.w	15*50
*-------------------------------------------------------------------------*
* 02=GLUE     /06=TIR1     /08=TIR2    /10=SIZ- /12=SIZ+ /14=VIE- /16=VIE+  /
* 18=BAL2    / 20=BAL3    / 22=POW1   / 24=POW2/ 26=INVE/ 40=SIZ2/ 42=SIZ2 / 
* 44=RAND   /  46=MEGA   /  48=PRICE /  04=LEVEL SUIVANT
* 34=BUMP1 /   36=BUMP2 /   38=BUMP3/   30=VITESSE BALLE MAXI
* 50=MUR  /    52=ROBOT/    54=CONTROLE DES BALLES / 56=OEIL

* 50.MUR	14
* 06.TIR1	 8
* 08.TIR2	 5
* 10.SIZ-	 7 
* 52.ROBO	10
* 18.BAL2	08

random.list	dc.w	50,56,08,10,52,18,20,22,54,40,42,56,38,30,30,30
		dc.w	14,54,02,06,08,10,12,18,20,22,30,40,42,42,38,56
		dc.w	04,54,14,30,52,06,08,10,50,18,52,52,52,40,42,30
		dc.w	34,34,36,26,16,16,50,56,48,10,52,18,20,22,30,40
		dc.w	50,06,52,56,52,18,20,22,50,40,42,36,38,30,30,56
		dc.w	14,26,02,06,08,10,12,18,20,22,30,40,42,42,38,50
		dc.w	04,26,14,50,52,06,08,10,12,18,20,22,30,40,50,50
		dc.w	34,34,36,26,50,50,02,06,48,10,12,18,20,22,50,50

*/ liste des malus pouvant tomber pendant le jeu */
* table { area 1 : level 1 } *
course10:	dc.w	40,06,18,12,06,06,18,18,06,18,20,52,42,18,20,40
		dc.w	18,40,50,40,42,44,54,06,22,18,06,06,52,40,18,06
		dc.w	18,06,18,12,06,42,18,18,06,44,20,18,18,18,20,18
		dc.w	44,44,50,18,42,18,20,06,22,40,06,54,22,40,18,06
* table { area 1 : level 2 à 5 } *
course11:	dc.w	40,10,40,12,18,10,40,12,06,10,02,18,18,42,02,12
		dc.w	18,02,44,18,18,02,42,10,10,18,12,02,18,18,52,12
		dc.w	40,10,18,12,40,10,40,12,06,46,02,18,18,42,02,12
		dc.w	18,02,18,44,54,02,42,10,10,44,12,02,18,18,10,12
* table { area 1 : level 7 à 11 } *
course12:	dc.w	40,26,42,18,02,26,18,18,06,52,02,42,18,16,02,18
		dc.w	40,02,18,14,10,02,20,10,10,18,10,02,26,18,10,10
		dc.w	18,26,10,18,02,26,54,44,06,46,02,10,44,16,02,44
		dc.w	40,02,40,14,10,02,20,10,10,18,10,02,26,40,10,10
* table { area 2 } *
course20:	dc.w	42,26,10,10,10,42,18,44,10,14,10,10,10,10,10,10
		dc.w	42,10,26,10,10,16,10,44,10,18,10,42,10,10,18,10
		dc.w	42,26,10,40,10,10,18,10,46,14,10,10,10,10,10,10
		dc.w	42,10,26,10,10,16,44,54,10,18,10,10,42,10,18,10
* table { area 3 } *
course30:	dc.w	10,26,10,50,50,10,18,18,14,14,10,10,40,40,10,18
		dc.w	10,10,26,10,42,16,42,14,44,18,10,10,40,40,18,14
		dc.w	10,26,10,44,46,10,54,18,14,14,40,40,10,10,10,18
		dc.w	10,10,26,10,10,16,44,14,42,18,40,40,10,52,18,14
* table { area 4 } *
course40:	dc.w	10,26,26,20,42,44,20,10,44,14,26,10,40,40,10,10
		dc.w	10,10,26,10,52,16,10,10,10,20,26,10,40,40,20,10
		dc.w	10,26,26,20,52,50,20,54,44,14,26,10,40,40,46,10
		dc.w	10,10,26,10,42,16,10,50,10,20,26,10,40,40,20,10
* table { area 5, level 1 à 5 } *
course50:	dc.w	42,26,26,10,10,10,20,44,10,14,26,10,10,10,20,10
		dc.w	42,10,26,10,50,14,20,44,46,18,26,10,10,10,20,14
		dc.w	10,26,26,10,50,10,20,54,10,14,26,10,52,10,20,10
		dc.w	10,10,26,10,10,14,20,44,10,18,26,10,52,10,20,14
* table { area 5, level 7 à 11 } *
course55:	dc.w	14,26,26,10,16,10,20,14,10,14,26,42,52,44,20,10
		dc.w	14,10,26,44,18,14,20,10,10,18,26,42,52,10,20,14
		dc.w	14,26,26,10,16,10,20,54,46,14,26,42,42,44,20,10
		dc.w	14,10,26,44,18,14,20,50,50,18,26,42,42,10,20,14
*-------------------------------------------------------------------------*
* Différentes tables des paramètres des gardiens *
* area 1 : intermediaire *
		CNOP	0,4
atari_00:	dc.l	speed3-t	00 vitesse de balle		-104(a5)
		dc.w	50*20		04 temps avant balle parte	-110(a5)
		dc.w	10*50		06 temps avant autoriser tilt	-280(a5)
		dc.w	10*50		08 temps minimum avant défilement
		dc.w	7		10 Fréquence Gadgets
		dc.l	xcourse-t	12 Table gadgets
* area 1 : final *
atari_04:	dc.l	speed3-t
		dc.w	50*15
		dc.w	10*50
		dc.w	10*50
		dc.w	8
		dc.l	xcourse-t
* area 2 : intermediaire *
atari_08:	dc.l	speed4-t
		dc.w	50*10
		dc.w	10*50
		dc.w	05*50
		dc.w	9
		dc.l	xcourse-t
* area 2 : final *
atari_12:	dc.l	speed4-t
		dc.w	50*10
		dc.w	10*50
		dc.w	05*50
		dc.w	9
		dc.l	xcourse-t
* area 3 : intermediaire *
atari_16:	dc.l	speed4-t
		dc.w	50*10
		dc.w	10*50
		dc.w	05*50
		dc.w	10
		dc.l	xcourse-t
* area 3 : final *
atari_20:	dc.l	speed4-t
		dc.w	50*10
		dc.w	10*50
		dc.w	03*50
		dc.w	10
		dc.l	xcourse-t
* area 4 : intermediaire *
atari_24:	dc.l	speed4-t
		dc.w	50*10
		dc.w	10*50
		dc.w	02*50
		dc.w	12
		dc.l	xcourse-t
* area 4 : final *
atari_28:	dc.l	speed4-t
		dc.w	50*10
		dc.w	10*50
		dc.w	01*50
		dc.w	12
		dc.l	xcourse-t
* area 5 : intermediaire *
atari_32:	dc.l	speed4-t
		dc.w	50*10
		dc.w	10*50
		dc.w	01*50
		dc.w	12
		dc.l	xcourse-t
* area 5 : final *
atari_36:	dc.l	speed4-t
		dc.w	50*10
		dc.w	10*50
		dc.w	01*50
		dc.w	15
		dc.l	xcourse-t
* area 5 : final-final *
atari_40:	dc.l	speed4-t
		dc.w	50*10
		dc.w	10*50
		dc.w	01*50
		dc.w	15
		dc.l	xcourse-t

xcourse:	dc.w	04,22,20,24,20,20,04,20
		dc.w	20,04,20,24,20,04,22,04
		dc.w	20,22,04,16,04,24,22,20
		dc.w	20,22,20,04,24,20,22,20

*/ CODE AREA */ (UN CODE PAR AREA & PAR NIVEAU DE DIFFICULTE)
z		=65
code.area1	dc.b	'['-z,'['-z,'['-z,'['-z,'['-z,'['-z,'['-z,'['-z,'['-z,'['-z
		dc.b	'['-z,'['-z,'['-z,'['-z,'['-z,'['-z,'['-z,'['-z,'['-z,'['-z
		dc.b	'['-z,'['-z,'['-z,'['-z,'['-z,'['-z,'['-z,'['-z,'['-z,'['-z
		dc.b	'['-z,'['-z,'['-z,'['-z,'['-z,'['-z,'['-z,'['-z,'['-z,'['-z
code.area2	dc.b	"L"-z,"A"-z,"R"-z,"R"-z,"Y"-z,"H"-z,"E"-z,"A"-z,"R"-z,"D"-z
		dc.b	"S"-z,"A"-z,"U"-z,"N"-z,"D"-z,"E"-z,"R"-z,"S"-z,"O"-z,"N"-z
		dc.b	"J"-z,"U"-z,"A"-z,"N"-z,"A"-z,"T"-z,"K"-z,"I"-z,"N"-z,"S"-z
		dc.b	"S"-z,"T"-z,"E"-z,"P"-z,"H"-z,"E"-z,"N"-z,"S"-z,"O"-z,"N"-z
code.area3	dc.b	"D"-z,"A"-z,"N"-z,"C"-z,"E"-z,"F"-z,"L"-z,"O"-z,"O"-z,"R"-z
		dc.b	"R"-z,"E"-z,"V"-z,"O"-z,"L"-z,"U"-z,"T"-z,"I"-z,"O"-z,"N"-z
		dc.b	"L"-z,"O"-z,"O"-z,"K"-z,"T"-z,"O"-z,"S"-z,"E"-z,"X"-z,"Y"-z
		dc.b	"R"-z,"E"-z,"A"-z,"C"-z,"H"-z,"I"-z,"N"-z,"G"-z,"U"-z,"P"-z
code.area4	dc.b	"Z"-z,"U"-z,"L"-z,"U"-z,"N"-z,"A"-z,"T"-z,"I"-z,"O"-z,"N"-z
		dc.b	"H"-z,"O"-z,"U"-z,"S"-z,"E"-z,"P"-z,"I"-z,"M"-z,"P"-z,"S"-z
		dc.b	"A"-z,"N"-z,"D"-z,"R"-z,"O"-z,"N"-z,"I"-z,"C"-z,"U"-z,"S"-z
		dc.b	"D"-z,"E"-z,"F"-z,"I"-z,"N"-z,"I"-z,"T"-z,"I"-z,"V"-z,"E"-z
code.area5	dc.b	"D"-z,"A"-z,"N"-z,"C"-z,"E"-z,"M"-z,"A"-z,"N"-z,"I"-z,"A"-z
		dc.b	"P"-z,"E"-z,"P"-z,"P"-z,"E"-z,"R"-z,"M"-z,"I"-z,"N"-z,"T"-z
		dc.b	"S"-z,"O"-z,"L"-z,"A"-z,"R"-z,"T"-z,"R"-z,"I"-z,"B"-z,"E"-z
		dc.b	"P"-z,"R"-z,"O"-z,"J"-z,"E"-z,"C"-z,"T"-z,"X"-z,"Y"-z,"Z"-z
code.area6	dc.b	"R"-z,"I"-z,"N"-z,"G"-z,"O"-z,"F"-z,"F"-z,"I"-z,"R"-z,"E"-z
		dc.b	"P"-z,"O"-z,"I"-z,"N"-z,"T"-z,"B"-z,"L"-z,"A"-z,"N"-z,"K"-z
		dc.b	"T"-z,"E"-z,"M"-z,"P"-z,"T"-z,"A"-z,"T"-z,"I"-z,"O"-z,"N"-z
		dc.b	"B"-z,"L"-z,"U"-z,"E"-z,"M"-z,"O"-z,"N"-z,"D"-z,"A"-z,"Y"-z
code.area7	dc.b	"S"-z,"H"-z,"E"-z,"L"-z,"L"-z,"S"-z,"H"-z,"O"-z,"C"-z,"K"-z
		dc.b	"H"-z,"O"-z,"U"-z,"S"-z,"E"-z,"M"-z,"U"-z,"S"-z,"I"-z,"C"-z
		dc.b	"D"-z,"A"-z,"V"-z,"E"-z,"C"-z,"L"-z,"A"-z,"R"-z,"K"-z,"E"-z
		dc.b	"C"-z,"Y"-z,"B"-z,"E"-z,"R"-z,"A"-z,"C"-z,"T"-z,"I"-z,"F"-z
codearea	dc.b	"CURIEUX!!!"

*/"LARRYHEARD"/"SAUNDERSON"/"JUANATKINS"/"STEPHENSON" Area 2
*/"DANCEFLOOR"/"REVOLUTION"/"LOOKTOSEXY"/"REACHINGUP" Area 3
*/"ZULUNATION"/"HOUSEPIMPS"/"ANDRONICUS"/"DEFINITIVE" Area 4
*/"DANCEMANIA"/"PEPPERMINT"/"SOLARTRIBE"/"PROJECTXYZ" Area 5
*/"RINGOFFIRE"/"POINTBLANK"/"TEMPTATION"/"BLUEMONDAY" Area5 Fin	Si sh.code = 1
*/"SHELLSHOCK"/"HOUSEMUSIC"/"DAVECLARKE"/"CYBERACTIF" Final	Si sh.code = 1

