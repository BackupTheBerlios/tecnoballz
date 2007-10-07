*-------------------------------------------------------------------------*
************** Données utilisées pour la gestion des balles ***************
*-------------------------------------------------------------------------*

*/ Déclare la structure balle */
		RSRESET
BALLE:		rs.b	BOB.SIZE
BALLE.X		=BOB.POSX ^	position X de la balle
BALLE.Y		=BOB.POSY !	position Y de la balle
BALLE.ACTIF	=BOB.ACTIF	flag balle active
BALLE.DIRECT	rs.l	1	pt direction de 0 à 56 step 4
BALLE.RAK	rs.l	1	pt/sructure raquette
BALLE.RAKGLUE	rs.l	1	colle option
BALLE.SPEED	rs.l	1	pt/table vitesse
BALLE.PB1	rs.l	1	adresse pointeur balle écran 1
BALLE.PB2	rs.l	1	adresse pointeur balle écran 2
BALLE.POI	rs.l	1	Table de collision de la balle avec briques
BALLE.POWER	rs.w	1	valeur diminuant la résistance des atoms
BALLE.PT	rs.w	1	pt/gfx
BALLE.EJECT1	rs.w	1 ^	flag : balle coin >haut-gauche<
BALLE.EJECT2	rs.w	1 ^	flag : balle coin >bas-gauche<
BALLE.EJECT3	rs.w	1 ^	flag : balle coin >bas-droite<
BALLE.EJECT4	rs.w	1 !	flag : balle coin >haut-droite<
BALLE.GLUE	rs.w	1 ^	flag balle collée
BALLE.PBTEMP	rs.w	1 ^	tempo. dépl. pt. balle
BALLE.PBROTA	rs.l	1 ^	rot. pt sprite
BALLE.PBX	rs.w	1 ^	pointeur balle x
BALLE.PBY	rs.w	1 ^	pointeur balle y
BALLE.TILT	rs.w	1	tilt
BALLE.ATOM	rs.w	1	si 0 no collision atom
BALLE.SCOUNT	rs.w	1	temps que dure la vitesse
BALLE.START	rs.w	1	temps avant que la balle parte
BALLE.VAL	rs.w	1	décrementation briques 2/4/6
BALLE.ROT	rs.w	1	1=rotation autour de l'oeil
BALLE.SIZE	rs	0

pt_balle	dc.l	0,0,"SHAP",0

*/ Adresses diverses */
		CNOP	0,4
balle_nbre	dc.w	0	Nombre de balle(s) -1 à l'écran
balle.rebond1	dc.w	0	Flag rebond

*/ jeu / gardien : gfx du pointeur de balle prédécalé */
pixtbl:		dc.b	%10000000
		dc.b	%01000000
		dc.b	%00100000
		dc.b	%00010000
		dc.b	%00001000
		dc.b	%00000100
		dc.b	%00000010
		dc.b	%00000001

*/ Paramètre de pos. d'apparition des balles (+2 ou +3 balles) */
		dc.w	0
furax:		dc.w	1,0,0,0,EJ1X+8,EJ1Y+8
		dc.w	0,1,0,0,EJ3X+8,EJ3Y+3
		dc.w	0,0,1,0,EJ4X+3,EJ4Y+3
		dc.w	0,0,0,1,EJ2X+3,EJ2Y+8
		dc.w	1,0,0,0,EJ1X+8,EJ1Y+8
		dc.w	0,1,0,0,EJ3X+8,EJ3Y+3
		dc.w	0,0,1,0,EJ4X+3,EJ4Y+3
		dc.w	0,0,0,1,EJ2X+3,EJ2Y+8

*/ Paramètres apparition des balles gadget MEGA */
furax2:
V		SET 1
		REPT	(NBALLE/4)+1
		dc.w	V,0,0,0,EJ1X+6,EJ1Y+6 (haut-gauche)
		dc.w	0,V,0,0,EJ3X+6,EJ3Y+1 (bas-gauche)
		dc.w	0,0,V,0,EJ4X+1,EJ4Y+1 (bas-droite)
		dc.w	0,0,0,V,EJ2X+1,EJ2Y+6 (haut-droite)
V		SET V+5
		ENDR

*/ Liste des points de collision de la balle (avec les briques) */
bri_poi:	dc.w	5,3		XMAXIM,MILIEU (balle1)
		dc.w	3,0		MILIEU,YMINIM
		dc.w	0,3		XMINIM,MILIEU
		dc.w	3,5		MILIEU,YMAXIM
bri_poi2	dc.w	7,4		XMAXIM,MILIEU (balle2)
		dc.w	4,0		MILIEU,YMINIM
		dc.w	0,4		XMINIM,MILIEU
		dc.w	4,7		MILIEU,YMAXIM
bri_poi3	dc.w	9,5		XMAXIM,MILIEU (balle3)
		dc.w	5,0		MILIEU,YMINIM
		dc.w	0,5		XMINIM,MILIEU
		dc.w	5,9		MILIEU,YMAXIM
* OLD bri_poi	dc.w	5,3,1,3,0,2,0,3,4,3,5,8

*/ Table des différentes vitesses de balle */

;	        16
;	      20/\ 12
;	    24 	||   08
;	  28	||     04
;	32<=====64=====> 00
;	  36  	||    60
;	    40 	||  56
;	      44\/52 
;	        48

speed1:		dc.w	+2,00	00
		dc.w	+2,-1	04
		dc.w	+2,-2	08
		dc.w	+1,-2	12
		dc.w	00,-2	16
		dc.w	-1,-2	20
		dc.w	-2,-2	24
		dc.w	-2,-1	28
		dc.w	-2,00	32
		dc.w	-2,+1	36
		dc.w	-2,+2	40
		dc.w	-1,+2	44
		dc.w	00,+2	48
		dc.w	+1,+2	52
		dc.w	+2,+2	56
		dc.w	+2,+1	60
		dc.w	00,00	64

speed2		dc.w	3,0,3,-1,2,-2,1,-3,0,-3,-1,-3,-2,-2,-3,-1,-3
		dc.w	0,-3,1,-2,2,-1,3,0,3,1,3,2,2,3,1,0,0
speed3		dc.w	4,0,4,-1,3,-3,1,-4,0,-4,-1,-4,-3,-3,-4,-1,-4
		dc.w	0,-4,1,-3,3,-1,4,0,4,1,4,3,3,4,1,0,0
speed4		dc.w	5,0,5,-2,4,-4,2,-5,0,-5,-2,-5,-4,-4,-5,-2
		dc.w	-5,0,-5,2,-4,4,-2,5,0,5,2,5,4,4,5,2,0,0

*/ Directions données par le pointeur de balle à la balle */
		CNOP	0,4
pets1:		dc.l	04,08,12,16,20,24,28
		dc.l	28,24,20,16,12,08,04
pets2		dc.l	20,24,28,32,36,40,44
		dc.l	44,40,36,32,28,24,20
pets3		dc.l	60,56,52,48,44,40,36
		dc.l	36,40,44,48,52,56,60
pets4		dc.l	12,08,04,00,60,56,52
		dc.l	52,56,60,00,04,08,12

*/  Position relative du pointeur de balle */
cool_list:	dc.l	8,2,8,-1,7,-3,5,-4,2,-4
		dc.l	-1,-4,-3,-3,-4,-1,-4,2
		dc.l	-4,5,-3,7,-1,8,2,8
		dc.l	5,8,7,7,8,5

*/ 4 ejections de la balle des 4 coins "hasard" */
eject1:		dc.l	52,56,60,60,52,56,60,60,52,52,56,52,52,60
		dc.l	56,52,56,56
eject2		dc.l	8,4,12,12,8,4,4,12,8,4,12,4,8,12,4,8,12,4,4
eject3		dc.l	20,28,24,20,20,28,28,24,20,28,24,24,28,28,20
		dc.l	20,24,24,28
eject4		dc.l	36,44,40,36,36,44,44,40,40,36,44,40,40,36,36
		dc.l	44,44,40,36

*/ table de rebonds de la balle sur les briques /*
		CNOP	0,4
bri_jump:	dc.l	rb1		1
		dc.l	rb3		2
		dc.l	rb2		3
		dc.l	rb5		4
		dc.l	rb1		5
		dc.l	rb4		6
		dc.l	rb3		7
		dc.l	rb7		8
		dc.l	rb8		9
		dc.l	rb2		10
		dc.l	rb1		11
		dc.l	rb6		12
		dc.l	rb7		13
		dc.l	rb5		14
		dc.l	rb1		15
rb0		dc.l	64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64
rb1		dc.l	32,28,24,20,20,24,24,28,32,36,40,40,44,44,40,36 *Droite
rb2		dc.l	48,36,40,44,32,44,24,28,32,36,40,44,48,48,44,40
rb3		dc.l	60,60,56,52,48,44,40,36,36,40,40,44,48,52,56,56 *Haut
rb4		dc.l	00,04,08,00,00,52,56,60,48,52,56,44,48,52,56,60
rb5		dc.l	00,04,08,08,12,12,08,04,00,60,56,52,52,36,56,60 *Gauche
rb6		dc.l	00,04,08,12,16,20,24,12,16,12,08,04,00,04,08,60
rb7		dc.l	04,08,12,12,16,20,20,24,28,28,24,20,16,12,08,04 *Bas
rb8		dc.l	16,20,24,12,16,20,24,28,32,36,40,28,32,20,24,28

*/	table de rebonds de la balle raquette du bas */
midi01_left:	dc.l	rb13		= 00
		dc.l	rb14		= 01
		dc.l	rb14		= 02
		dc.l	rb15		= 03
		dc.l	rb15		= 04
		dc.l	rb16		= 05
		dc.l	rb16		= 06
		dc.l	rb16		= 07
		dc.l	rb16		= 08
		dc.l	rb16		= 09
		dc.l	rb17		= 10
midi01_right	dc.l	rb13		= 00
		dc.l	rb12		= 01
		dc.l	rb12		= 02
		dc.l	rb11		= 03
		dc.l	rb11		= 04
		dc.l	rb10		= 05
		dc.l	rb10		= 06
		dc.l	rb10		= 07
		dc.l	rb10		= 08
		dc.l	rb10		= 09
		dc.l	rb9		= 10
rb9		dc.l	04,04,08,12,16,20,24,28,28,32,32,32,32,20,24,28
rb10		dc.l	04,04,08,12,16,20,24,28,28,32,32,32,28,20,20,16
rb11		dc.l	04,04,08,12,16,20,24,28,28,28,28,28,24,16,16,12
rb12		dc.l	04,04,08,12,16,20,24,28,28,28,28,24,20,16,12,08
rb13		dc.l	04,04,08,12,16,20,24,28,28,28,24,20,16,12,08,04
rb14		dc.l	04,04,08,12,16,20,24,28,28,20,20,20,12,08,04,04
rb15		dc.l	04,04,08,12,16,20,24,28,28,16,16,16,08,08,04,00
rb16		dc.l	04,04,08,12,16,20,24,28,28,08,12,12,04,04,00,00
rb17		dc.l	04,04,08,12,16,20,24,28,28,04,08,12,00,00,00,00

		*/ jeu : table de rebonds de la balle raquette de droite */
midi02_left:	dc.l	rb22		= 00
		dc.l	rb21		= 01
		dc.l	rb21		= 02
		dc.l	rb20		= 03
		dc.l	rb20		= 04
		dc.l	rb19		= 05
		dc.l	rb19		= 06
		dc.l	rb19		= 07
		dc.l	rb19		= 08
		dc.l	rb19		= 09
		dc.l	rb18		= 10
midi02_right	dc.l	rb22		= 00
		dc.l	rb23		= 01
		dc.l	rb23		= 02
		dc.l	rb24		= 03
		dc.l	rb24		= 04
		dc.l	rb25		= 05
		dc.l	rb25		= 06
		dc.l	rb25		= 07
		dc.l	rb25		= 08
		dc.l	rb25		= 09
		dc.l	rb26		= 10
rb18		dc.l	32+16,20+16,24+16,28+16,04+16,04+16,08+16,12+16
		dc.l	16+16,20+16,24+16,28+16,28+16,32+16,32+16,32+16
rb19		dc.l	28+16,20+16,20+16,16+16,04+16,04+16,08+16,12+16
		dc.l	16+16,20+16,24+16,28+16,28+16,32+16,32+16,32+16
rb20		dc.l	24+16,16+16,16+16,12+16,04+16,04+16,08+16,12+16
		dc.l	16+16,20+16,24+16,28+16,28+16,28+16,28+16,28+16
rb21		dc.l	20+16,16+16,12+16,08+16,04+16,04+16,08+16,12+16
		dc.l	16+16,20+16,24+16,28+16,28+16,28+16,28+16,24+16
rb22		dc.l	16+16,12+16,08+16,04+16,04+16,04+16,08+16,12+16
		dc.l	16+16,20+16,24+16,28+16,28+16,28+16,24+16,20+16
rb23		dc.l	12+16,08+16,04+16,04+16,04+16,04+16,08+16,12+16
		dc.l	16+16,20+16,24+16,28+16,28+16,20+16,20+16,20+16
rb24		dc.l	08+16,08+16,04+16,00+16,04+16,04+16,08+16,12+16
		dc.l	16+16,20+16,24+16,28+16,28+16,16+16,16+16,16+16
rb25		dc.l	04+16,04+16,00+16,00+16,04+16,04+16,08+16,12+16
		dc.l	16+16,20+16,24+16,28+16,28+16,08+16,12+16,12+16
rb26		dc.l	00+16,00+16,00+16,00+16,04+16,04+16,08+16,12+16
		dc.l	16+16,20+16,24+16,28+16,28+16,04+16,08+16,12+16

*/ table de rebonds de la balle raquette du haut */
midi03_left:	dc.l	rb31		= 00
		dc.l	rb30		= 01
		dc.l	rb30		= 02
		dc.l	rb29		= 03
		dc.l	rb29		= 04
		dc.l	rb28		= 05
		dc.l	rb28		= 06
		dc.l	rb28		= 07
		dc.l	rb28		= 08
		dc.l	rb28		= 09
		dc.l	rb27		= 10
midi03_right	dc.l	rb31		= 00
		dc.l	rb32		= 01
		dc.l	rb32		= 02
		dc.l	rb33		= 03
		dc.l	rb33		= 04
		dc.l	rb34		= 05
		dc.l	rb34		= 06
		dc.l	rb34		= 07
		dc.l	rb34		= 08
		dc.l	rb34		= 09
		dc.l	rb35		= 10
rb27		dc.l	60,00,00,00,00,52,56,60
		dc.l	04+32,04+32,08+32,12+32,16+32,20+32,24+32,28+32
rb28		dc.l	60,00,00,00,60,52,52,48
		dc.l	04+32,04+32,08+32,12+32,16+32,20+32,24+32,28+32
rb29		dc.l	60,60,60,60,56,48,48,44
		dc.l	04+32,04+32,08+32,12+32,16+32,20+32,24+32,28+32
rb30		dc.l	60,60,60,56,52,48,48,40
		dc.l	04+32,04+32,08+32,12+32,16+32,20+32,24+32,28+32
rb31		dc.l	60,60,56,52,48,44,40,36
		dc.l	04+32,04+32,08+32,12+32,16+32,20+32,24+32,28+32
rb32		dc.l	60,52,52,48,44,40,36,36
		dc.l	04+32,04+32,08+32,12+32,16+32,20+32,24+32,28+32
rb33		dc.l	60,48,48,16+32,40,40,32,32
		dc.l	04+32,04+32,08+32,12+32,16+32,20+32,24+32,28+32
rb34		dc.l	60,40,44,12+32,36,36,32,32
		dc.l	04+32,04+32,08+32,12+32,16+32,20+32,24+32,28+32
rb35		dc.l	60,36,40,12+32,32,32,32,32
		dc.l	04+32,04+32,08+32,12+32,16+32,20+32,24+32,28+32

*/ table de rebonds de la balle raquette de gauche */
midi04_left:	dc.l	rb40		= 00
		dc.l	rb41		= 01
		dc.l	rb41		= 02
		dc.l	rb42		= 03
		dc.l	rb42		= 04
		dc.l	rb43		= 05
		dc.l	rb43		= 06
		dc.l	rb43		= 07
		dc.l	rb43		= 08
		dc.l	rb43		= 09
		dc.l	rb44		= 10
midi04_right	dc.l	rb40		= 00
		dc.l	rb39		= 01
		dc.l	rb39		= 02
		dc.l	rb38		= 03
		dc.l	rb38		= 04
		dc.l	rb37		= 05
		dc.l	rb37		= 06
		dc.l	rb37		= 07
		dc.l	rb37		= 08
		dc.l	rb37		= 09
		dc.l	rb36		= 10
rb36		dc.l	16-16,20-16,24-16,28-16,28-16,32-16,32-16,32-16
		dc.l	32-16,20-16,24-16,28-16,04+48,04+48,08+48,12+48
rb37		dc.l	16-16,20-16,24-16,28-16,28-16,32-16,32-16,32-16
		dc.l	28-16,20-16,20-16,16-16,04+48,04+48,08+48,12+48
rb38		dc.l	16-16,20-16,24-16,28-16,28-16,28-16,28-16,28-16
		dc.l	24-16,16-16,16-16,12+48,04+48,04+48,08+48,12+48
rb39		dc.l	16-16,20-16,24-16,28-16,28-16,28-16,28-16,24-16
		dc.l	20-16,16-16,12+48,08+48,04+48,04+48,08+48,12+48
rb40		dc.l	16-16,20-16,24-16,28-16,28-16,28-16,24-16,20-16
		dc.l	16-16,12+48,08+48,04+48,04+48,04+48,08+48,12+48
rb41		dc.l	16-16,20-16,24-16,28-16,28-16,20-16,20-16,20-16
		dc.l	12+48,08+48,04+48,04+48,04+48,04+48,08+48,12+48
rb42		dc.l	16-16,20-16,24-16,28-16,28-16,16-16,16-16,16-16
		dc.l	08+48,08+48,04+48,00+48,04+48,04+48,08+48,12+48
rb43		dc.l	16-16,20-16,24-16,28-16,28-16,08+48,12+48,12+48
		dc.l	04+48,04+48,00+48,00+48,04+48,04+48,08+48,12+48
rb44		dc.l	16-16,20-16,24-16,28-16,28-16,04+48,08+48,12+48
		dc.l	00+48,00+48,00+48,00+48,04+48,04+48,08+48,12+48

