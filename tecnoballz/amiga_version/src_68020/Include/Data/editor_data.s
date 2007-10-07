*//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/*
*-------------* Données utilisées par l'éditeur de tableaux *-------------*
*//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/*
		CNOP	0,4
niveau		dc.l	0		pt/le niveau
ed.sprite1	dc.l	0,4*9*2,'SPRI',MEMF_CHIP
ed.sprite2	dc.l	0,4*9*2,'SPRI',MEMF_CHIP
kx1		dc.l	0		flag 4 touches curseurs
kx2		dc.l	0		flag 4 touches curseurs pavé
kx3		dc.w	0		flag touche -/+ numéro tableau
kx4		dc.w	0		flag change résistance briques -/+
kx5		dc.w	0		flag change le jeu de briques
kx6		dc.w	0		flag change le fond	
kx7		dc.w	0		flag change couleur du fond
kx8		dc.w	0		flag change couleur des briques
editor		dc.w	0		flag 1=editeur

*/ dest/source copie & echange */
ecs		dc.l	0	"Swap" pt/tableau source
ecd		dc.l	0	"Swap" pt/tableau destination
cos		dc.l	0	"Copy" pt/tableau source
cod		dc.l	0	"Copy" pt/tableau destination
spx		dc.l	48	position X curseur tableau
spy		dc.l	BRIY	position Y curseur tableau
zpx		dc.l	16	position X curseur menu brique
zpy		dc.l	0	position Y curseur menu brique
neo		dc.w	0	n° brique selectionné
cbs		dc.w	0	code brique sélectionné
nix		dc.w	0	pt/colonne tableau en cours
niy		dc.w	0	pt/ligne tableau en cours
ava		dc.w	0	N° du fond de 0 à 59
avi		dc.w	0	N° des briques de 0 à 9
avo		dc.w	0	N° couleur de fond de 0 à 59
avu		dc.w	0	N° couleur des briques de 0 à 9
snif		dc.w	0	n° du fichier

edmenu:		dc.b	'* EXIT *',0	*000
		dc.b	'--------',0	*008
		dc.b	'  Save  ',0	*016
		dc.b	'  Load  ',0	*024
		dc.b	'--------',0	*032
		dc.b	'  File  ',0	*040
fix		dc.b	' < 00 > ',0	*048
		dc.b	'--------',0	*056
		dc.b	'TABLEAU ',0	*064
tz1		dc.b	'< 0000 >',0	*072
		dc.b	'  AREA  ',0	*080
tz2		dc.b	' < 00 > ',0	*088
		dc.b	' LEVEL  ',0	*096
tz3		dc.b	' < 00 > ',0	*104
		dc.b	'--------',0	*112
		dc.b	'  COPY	 ',0	*120
pe1		dc.b	'< 0000 >',0	*128
pe2		dc.b	'< 0000 >',0	*136
		dc.b	'  SWAP  ',0	*144
pe3		dc.b	'< 0000 >',0	*152
pe4		dc.b	'< 0000 >',0	*160
		dc.b	'--------',0	*168
		dc.b	'  PLAY  ',0	*176
		dc.b	'--------',0	*184
		dc.b	'  FILL  ',0	*192
czx		dc.b	'00-00-00',0,0
filex		dc.b	"TecnoballZ:TecnoballZ/Raw/tableaux"
filexx		dc.b	"00.data",0

axx1		dc.b	"  ------------------------------------  ",0
		dc.b	"> Are you sure to SAVE ?  Y=Yes / N=No <",0
		dc.b	"  ------------------------------------  ",0,0

axx2		dc.b	"  - - - - - - - - -  - - - - - - - - -  ",0
		dc.b	"| Are you sure to LOAD ?  Y=Yes / N=No |",0
		dc.b	"  - - - - - - - - -  - - - - - - - - -  ",0,0

		EVEN



*/ POSITION DES BRIQUES MENUS */
sg:		dc.b	32,02,32,04,32,06,32,08
		dc.b	32,10,32,12,32,14,32,16
		dc.b	32,18,32,20,32,22,32,24
		dc.b	32,26,32,28
		dc.b	39,02,39,04,39,06,39,08
		dc.b	39,10,39,12,39,14,39,16
		dc.b	39,18,39,20,39,22,39,24
		dc.b	39,26,39,28
		dc.b	46,02,46,04,46,06,46,08
		dc.b	46,10,46,12,46,14,46,16
		dc.b	46,18,46,20,46,22,46,24
		dc.b	46,26,46,28
		dc.b	53,02,53,04,53,06,53,08
		dc.b	53,10,53,12,53,14,53,16
		dc.b	53,18,53,20,53,22,53,24
		dc.b	53,26,53,28
		dc.b	60,02,60,04,60,06,60,08
		dc.b	60,10,60,12,60,14,60,16
		dc.b	60,18,60,20,60,22,60,24
		dc.b	60,26,60,28
		EVEN

		dc.w	-1		Flag table non calculée
bg:		dc.b	000,00,000,02,000,04,000,06,000,08,000,10,000,12
		dc.b	001,00,001,02,001,04,001,06,001,08,001,10,001,12
		dc.b	002,00,002,02,002,04,002,06,002,08,002,10,002,12
		dc.b	003,00,003,02,003,04,003,06,003,08,003,10,003,12
		dc.b	004,00,004,02,004,04,004,06,004,08,004,10,004,12
		dc.b	005,00,005,02,005,04,005,06,005,08,005,10,005,12
		dc.b	006,00,006,02,006,04,006,06,006,08,006,10,006,12
		dc.b	007,00,007,02,007,04,007,06,007,08,007,10,007,12
		dc.b	008,00,008,02,008,04,008,06,008,08,008,10,008,12
