*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
***�������������������� TABLE DES BLITTERS OBJECTS ���������������������***
*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
		CNOP	0,4
tbob:		dc.b	'SBOB'
		dc.l	32		** NOMBRE DE BOBS **
		dc.l	bob0-tbob	ATOMS
		dc.l	bob1-tbob	EJECTEUR HAUT-GAUCHE
		dc.l	bob2-tbob	EJECTEUR BAS-GAUCHE
		dc.l	bob3-tbob	EJECTEUR BAS-DROITE
		dc.l	bob4-tbob	EJECTEUR HAUT-DROITE 
		dc.l	bob5-tbob	BRICOTE VERTICALE
		dc.l	bob6-tbob	BRICOTE HORIZONTALE
		dc.l	bob7-tbob	BRIQUE
		dc.l	bob8-tbob	RAQUETTE HORIZONTALE
		dc.l	bob9-tbob	RAQUETTE VERTICALE
		dc.l	bob10-tbob	BALLE
		dc.l	bob11-tbob	TIR 2 & 1
		dc.l	bob12-tbob	MONNAIE
		dc.l	bob13-tbob	GADGETS
		dc.l	bob14-tbob	23 lettres
		dc.l	bob15-tbob	LEDON
		dc.l	bob16-tbob	POINTEUR 1
		dc.l	bob17-tbob	POINTEUR 2
		dc.l	bob18-tbob	G
		dc.l	bob19-tbob	A
		dc.l	bob20-tbob	M
		dc.l	bob21-tbob	E
		dc.l	bob22-tbob	O
		dc.l	bob23-tbob	V
		dc.l	bob24-tbob	E
		dc.l	bob25-tbob	R
		dc.l	bob26-tbob	INVERSEUR -> SuperLutin
		dc.l	bob27-tbob	Curseur editeur briques
		dc.l	bob28-tbob	Mur
		dc.l	bob29-tbob	etoile
		dc.l	bob30-tbob	Raquette Robot
		dc.l	bob31-tbob	Gemmes
		dc.l	bob32-tbob	Oeil
*-------------------------------------------------------------------------*
		*/ Boui-Boui / Atoms / Explosions /*
bob0:		dc.w	0,0,1,16,(22*8)-1,16
		dc.w	0,070,0,0,1,070,0,0,2,070,0,0,3,070,0,0 ATOM 1
		dc.w	4,070,0,0,5,070,0,0,6,070,0,0,7,070,0,0
		dc.w	0,086,0,0,1,086,0,0,2,086,0,0,3,086,0,0 ATOM 2
		dc.w	4,086,0,0,5,086,0,0,6,086,0,0,7,086,0,0
		dc.w	0,102,0,0,1,102,0,0,2,102,0,0,3,102,0,0 ATOM 3
		dc.w	4,102,0,0,5,102,0,0,6,102,0,0,7,102,0,0
		dc.w	0,118,0,0,1,118,0,0,2,118,0,0,3,118,0,0 ATOM 4
		dc.w	4,118,0,0,5,118,0,0,6,118,0,0,7,118,0,0
		dc.w	0,134,0,0,1,134,0,0,2,134,0,0,3,134,0,0 ATOM 5
		dc.w	4,134,0,0,5,134,0,0,6,134,0,0,7,134,0,0
		dc.w	0,150,0,0,1,150,0,0,2,150,0,0,3,150,0,0 ATOM 6
		dc.w	4,150,0,0,5,150,0,0,6,150,0,0,7,150,0,0
		dc.w	0,166,0,0,1,166,0,0,2,166,0,0,3,166,0,0 ATOM 7
		dc.w	4,166,0,0,5,166,0,0,6,166,0,0,7,166,0,0
		dc.w	0,182,0,0,1,182,0,0,2,182,0,0,3,182,0,0 ATOM 8
		dc.w	4,182,0,0,5,182,0,0,6,182,0,0,7,182,0,0
		dc.w	0,198,0,0,1,198,0,0,2,198,0,0,3,198,0,0 ATOM 9
		dc.w	4,198,0,0,5,198,0,0,6,198,0,0,7,198,0,0
		dc.w	0,214,0,0,1,214,0,0,2,214,0,0,3,214,0,0 ATOM 10
		dc.w	4,214,0,0,5,214,0,0,6,214,0,0,7,214,0,0
		dc.w	08,70,0,0,09,70,0,0,10,70,0,0,11,70,0,0 ATOM 11
		dc.w	12,70,0,0,13,70,0,0,14,70,0,0,15,70,0,0
		dc.w	08,86,0,0,09,86,0,0,10,86,0,0,11,86,0,0 ATOM 12
		dc.w	12,86,0,0,13,86,0,0,14,86,0,0,15,86,0,0
		dc.w	08,102,0,0,09,102,0,0,10,102,0,0,11,102,0,0 ATOM 13 
		dc.w	12,102,0,0,13,102,0,0,14,102,0,0,15,102,0,0
		dc.w	08,118,0,0,09,118,0,0,10,118,0,0,11,118,0,0 ATOM 14
		dc.w	12,118,0,0,13,118,0,0,14,118,0,0,14,118,0,0
		dc.w	08,134,0,0,09,134,0,0,10,134,0,0,11,134,0,0 ATOM 15
		dc.w	12,134,0,0,13,134,0,0,14,134,0,0,15,134,0,0
		dc.w	08,150,0,0,09,150,0,0,10,150,0,0,11,150,0,0 ATOM 16
		dc.w	12,150,0,0,13,150,0,0,14,150,0,0,15,150,0,0
		dc.w	08,166,0,0,09,166,0,0,10,166,0,0,11,166,0,0 ATOM 17
		dc.w	12,166,0,0,13,166,0,0,14,166,0,0,15,166,0,0
		dc.w	08,182,0,0,09,182,0,0,10,182,0,0,11,182,0,0 ATOM 18
		dc.w	12,182,0,0,13,182,0,0,14,182,0,0,15,182,0,0
		dc.w	08,198,0,0,09,198,0,0,10,198,0,0,11,198,0,0 ATOM 19
		dc.w	12,198,0,0,13,198,0,0,14,198,0,0,15,198,0,0
		dc.w	08,214,0,0,09,214,0,0,10,214,0,0,11,214,0,0 ATOM 20
		dc.w	12,214,0,0,13,214,0,0,14,214,0,0,15,214,0,0
		dc.w	0,54,0,0,1,54,0,0,2,54,0,0,3,54,0,0	EXPLOSION 1
		dc.w	4,54,0,0,5,54,0,0,6,54,0,0,7,54,0,0
		dc.w	08,54,0,0,09,54,0,0,10,54,0,0,11,54,0,0	EXPLOSION 2
		dc.w	12,54,0,0,13,54,0,0,14,54,0,0,15,54,0,0
*-------------------------------------------------------------------------*
		*/ Ejecteur haut-gauche */
bob1:		dc.w	0,0,1,16,0,16
		dc.w	14,5,0,0
		*/ Ejecteur bas-gauche */
bob2:		dc.w	0,0,1,16,0,16
		dc.w	15,5,0,0
		*/ Ejecteur bas-droite */
bob3:		dc.w	0,0,1,16,0,16
		dc.w	16,5,0,0
		*/ Ejecteur haut-droite */
bob4:		dc.w	0,0,1,16,0,16
		dc.w	17,5,0,0
*-------------------------------------------------------------------------*
		*/ Bricote verticale */
bob5:		dc.w	0,0,1,16,0,5
		dc.w	18,5,0,0
		*/ Bricote horizontale */
bob6:		dc.w	0,0,1,5,0,16
		dc.w	11,0,0,0
		*/ Brique jeu 16x7 */
bob7:		dc.w	0,0,1,7,0,16
		dc.w	14,21,0,0
*-------------------------------------------------------------------------*
		*/ Raquette Horizontale */
bob8:		dc.w	0,0
		dc.w	4,8,6,64
		dc.w	16,21,0,0
		dc.w	16,29,0,0
		dc.w	16,37,0,0
		dc.w	16,45,0,0
		dc.w	16,53,0,0
		dc.w	16,61,0,0
		dc.w	16,69,0,0
		*/ Raquette Verticale */
bob9:		dc.w	0,0
		dc.w	1,64,6,8
		dc.w	16,077,0,0
		dc.w	17,077,0,0
		dc.w	18,077,0,0
		dc.w	19,077,0,0
		dc.w	16,141,0,0
		dc.w	17,141,0,0
		dc.w	18,141,0,0
*-------------------------------------------------------------------------*
		*/ Balle */ 
bob10:		dc.w	0,0
		dc.w	1,9,8,9
		dc.w	14,046,0,0		Verte 1
		dc.w	01,497,0,0		Grise 1
		dc.w	14,028,0,0		Jaune 1
		dc.w	15,037,0,0		Verte 2
		dc.w	02,497,0,0		Grise 2
		dc.w	15,028,0,0		Jaune 2
		dc.w	03,488,0,0		Verte 3
		dc.w	03,497,0,0		Grise 3
		dc.w	14,037,0,0		Jaune 3
*-------------------------------------------------------------------------*
		*/ TIR 2 & 1*/
bob11		dc.w	0,0
		dc.w	1,5,7,5
		dc.w	03,0,0,0 TIR 2	0
		dc.w	04,0,0,0	8
		dc.w	05,0,0,0	16
		dc.w	06,0,0,0	24
		dc.w	07,0,0,0 TIR 1	32
		dc.w	08,0,0,0
		dc.w	09,0,0,0
		dc.w	10,0,0,0

*- MONNAIE *---------------------------------------------------------------*
bob12:		dc.w	0,0,1,7,6,16,07,33,0,0,08,33,0,0,09,33,0,0,10,33,0,0,11,33,0,0,12,33,0,0,13,33,0,0
*-12 BONUS-MALUS ----------------------------------------------------------*
bob13:		dc.w	0,0,1,7,7*19-1,16
	dc.w	00,005,0,0,01,005,0,0,02,005,0,0,03,005,0,0,04,005,0,0,05,005,0,0,06,005,0,0 Size+
	dc.w	00,012,0,0,01,012,0,0,02,012,0,0,03,012,0,0,04,012,0,0,05,012,0,0,06,012,0,0 Size-
	dc.w	00,019,0,0,01,019,0,0,02,019,0,0,03,019,0,0,04,019,0,0,05,019,0,0,06,019,0,0 Life+
	dc.w	00,026,0,0,01,026,0,0,02,026,0,0,03,026,0,0,04,026,0,0,05,026,0,0,06,026,0,0 Life-
	dc.w	00,033,0,0,01,033,0,0,02,033,0,0,03,033,0,0,04,033,0,0,05,033,0,0,06,033,0,0 BallPower1
	dc.w	00,040,0,0,01,040,0,0,02,040,0,0,03,040,0,0,04,040,0,0,05,040,0,0,06,040,0,0 BallPower2
	dc.w	00,047,0,0,01,047,0,0,02,047,0,0,03,047,0,0,04,047,0,0,05,047,0,0,06,047,0,0 Balls+2
	dc.w	07,005,0,0,08,005,0,0,09,005,0,0,10,005,0,0,11,005,0,0,12,005,0,0,13,005,0,0 Balls+3
	dc.w	07,012,0,0,08,012,0,0,09,012,0,0,10,012,0,0,11,012,0,0,12,012,0,0,13,012,0,0 Fire1
	dc.w	07,019,0,0,08,019,0,0,09,019,0,0,10,019,0,0,11,019,0,0,12,019,0,0,13,019,0,0 Fire2
	dc.w	07,026,0,0,08,026,0,0,09,026,0,0,10,026,0,0,11,026,0,0,12,026,0,0,13,026,0,0 Glue
	dc.w	07,040,0,0,08,040,0,0,09,040,0,0,10,040,0,0,11,040,0,0,12,040,0,0,13,040,0,0 Inverse
	dc.w	17,219,0,0,18,219,0,0,18,246,0,0,19,246,0,0,19,261,0,0,19,239,0,0,16,268,0,0 BallSize2
	dc.w	17,268,0,0,18,268,0,0,19,268,0,0,10,481,0,0,11,481,0,0,12,481,0,0,13,481,0,0 BallSize3
	dc.w	14,392,0,0,14,399,0,0,14,406,0,0,14,413,0,0,14,420,0,0,14,427,0,0,14,434,0,0 Random
	dc.w	15,392,0,0,15,399,0,0,15,406,0,0,15,413,0,0,15,420,0,0,15,427,0,0,15,434,0,0 Mega
	dc.w	00,635,0,0,01,635,0,0,02,635,0,0,03,635,0,0,04,635,0,0,05,635,0,0,06,635,0,0 WALL
	dc.w	07,635,0,0,08,635,0,0,09,635,0,0,10,635,0,0,11,635,0,0,12,635,0,0,13,635,0,0 ROBOT
	dc.w	14,635,0,0,15,635,0,0,16,635,0,0,17,635,0,0,18,635,0,0,19,635,0,0,02,490,0,0 CONTROLATOR

*-------------------------------------------------------------------------*
bob14:		dc.w	0,0,1,16,42,16		*/ 43 LETTRES DE 16x16 */
		dc.w	00,758,0,0,01,758,0,0,02,758,0,0,03,758,0,0,04,758,0,0,05,758,0,0,06,758,0,0,07,758,0,0,08,758,0,0,09,758,0,0
		dc.w	10,758,0,0,11,758,0,0,12,758,0,0,13,758,0,0,14,758,0,0,15,758,0,0,16,758,0,0,17,758,0,0,18,758,0,0,19,758,0,0
		dc.w	00,774,0,0,01,774,0,0,02,774,0,0,03,774,0,0,04,774,0,0,05,774,0,0,06,774,0,0,07,774,0,0,08,774,0,0,09,774,0,0
		dc.w	10,774,0,0,11,774,0,0,12,774,0,0,13,774,0,0,14,774,0,0,15,774,0,0,16,774,0,0,17,774,0,0,18,774,0,0,19,774,0,0
		dc.w	00,790,0,0,01,790,0,0,02,790,0,0
*-------------------------------------------------------------------------*
		*/ LED ON */
bob15:		dc.w	0,0,1,3,0,16
		dc.w	13,0,0,0
*-------------------------------------------------------------------------*
		*/ Pointeur Souris 1 /*
bob16:		dc.w	0,0,1,16,7,16
		dc.w	0,230,0,0,1,230,0,0,2,230,0,0,3,230,0,0
		dc.w	4,230,0,0,5,230,0,0,6,230,0,0,7,230,0,0
		*/ Pointeur Souris 2 /*
bob17:		dc.w	0,0,1,16,7,16
		dc.w	08,230,0,0,09,230,0,0,10,230,0,0,11,230,0,0
		dc.w	12,230,0,0,13,230,0,0,14,230,0,0,15,230,0,0
*-------------------------------------------------------------------------*
bob18:		dc.w	0,0,2,32,0,32,00,246,0,0	G
bob19:		dc.w	0,0,2,32,0,32,02,246,0,0	A
bob20:		dc.w	0,0,2,32,0,32,04,246,0,0	M
bob21:		dc.w	0,0,2,32,0,32,06,246,0,0	E
bob22:		dc.w	0,0,2,32,0,32,08,246,0,0	O
bob23:		dc.w	0,0,2,32,0,32,10,246,0,0	V
bob24:		dc.w	0,0,2,32,0,32,12,246,0,0	E
bob25:		dc.w	0,0,2,32,0,32,14,246,0,0	R
*-------------------------------------------------------------------------*
bob26:		dc.w	0,0,1,7,6,16		*/ INVERSEUR
		dc.w	16,254,0,0,17,254,0,0,18,254,0,0,19,254,0,0
		dc.w	16,261,0,0,17,261,0,0,18,261,0,0
bob27:		dc.w	0,0,1,7,0,16		*/ CURSEUR BRIQUE */
		dc.w	15,21,0,0
bob28		dc.w	0,0,12,8,0,192		*/ MUR */
		dc.w	0,627,0,0
bob29		dc.w	0,0,1,7,7,7		*/ ETOILE */
		dc.w	12,627,0,0,13,627,0,0,14,627,0,0,15,627,0,0
		dc.w	16,627,0,0,17,627,0,0,18,627,0,0,19,627,0,0
bob30		dc.w	0,0,4,8,0,64		*/ Raquette Robot */
		dc.w	16,448,0,0
*-------------------------------------------------------------------------*
bob31		dc.w	0,0,1,8,5,8		*/ Gemmes */
		dc.w	10,472,0,0,11,472,0,0,18,574,0,0,19,574,0,0
		dc.w	03,480,0,0,03,472,0,0
*-------------------------------------------------------------------------*
bob32:		dc.w	0,0,2,29,31,32		*/ OEIL */
		dc.w	00,642,0,0,02,642,0,0,04,642,0,0,06,642,0,0,08,642,0,0,10,642,0,0,12,642,0,0,14,642,0,0,16,642,0,0,18,642,0,0
		dc.w	00,671,0,0,02,671,0,0,04,671,0,0,06,671,0,0,08,671,0,0,10,671,0,0,12,671,0,0,14,671,0,0,16,671,0,0,18,671,0,0
		dc.w	00,700,0,0,02,700,0,0,04,700,0,0,06,700,0,0,08,700,0,0,10,700,0,0,12,700,0,0,14,700,0,0,16,700,0,0,18,700,0,0
		dc.w	00,729,0,0,02,729,0,0,04
