*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*
*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*
* Entrée 	=> d0.b	Code RAW clavier
* Sortie	<= a0.l	Pointeur sur la chaine
* Modifie	(a0/d1)
codeclavier:
	lea	(.table,pc),a0
	moveq	#(.end-.table)/8-1,d1
.loop	cmp.b	(a0)+,d0
	beq.s	.find
	addq.l	#7,a0
	dbf	d1,.loop
.find	rts
.table	dc.b	TOUCHE_APOSTROP
	dc.b	".. ` .",0
	dc.b	TOUCHE_1
	dc.b	".. 1 .",0
	dc.b	TOUCHE_2
	dc.b	".. 2 .",0
	dc.b	TOUCHE_3
	dc.b	".. 3 .",0
	dc.b	TOUCHE_4
	dc.b	".. 4 .",0
	dc.b	TOUCHE_5
	dc.b	".. 5 .",0
	dc.b	TOUCHE_6
	dc.b	".. 6 .",0
	dc.b	TOUCHE_7
	dc.b	".. 7 .",0
	dc.b	TOUCHE_8
	dc.b	".. 8 .",0
	dc.b	TOUCHE_9
	dc.b	".. 9 .",0
	dc.b	TOUCHE_0
	dc.b	".. 0 .",0
	dc.b	TOUCHE_DEGRE
	dc.b	".. ° .",0
	dc.b	TOUCHE_TIRET
	dc.b	".. - .",0
	dc.b	TOUCHE_BARRE
	dc.b	".. | .",0
	dc.b	TOUCHE_CHAPEAU
	dc.b	".. ^ .",0
	dc.b	TOUCHE_DOLLAR
	dc.b	".. $ .",0
	dc.b	TOUCHE_UACCENT
	dc.b	".. ù .",0
	dc.b	TOUCHE_MICRO
	dc.b	".. µ .",0
	dc.b	TOUCHE_SUPINF
	dc.b	".. < .",0
	dc.b	TOUCHE_ESPACE
	dc.b	"ESPACE",0
	dc.b	TOUCHE_BACK
	dc.b	".BACK.",0
	dc.b	TOUCHE_TAB
	dc.b	"..TAB.",0
	dc.b	TOUCHE_ENTER
	dc.b	".ENTER",0
	dc.b	TOUCHE_RETURN
	dc.b	"RETURN",0
	dc.b	TOUCHE_ESC
	dc.b	"..ESC.",0
	dc.b	TOUCHE_DEL
	dc.b	"..DEL.",0
	dc.b	TOUCHE_HAUT
	dc.b	".HAUT.",0
	dc.b	TOUCHE_BAS
	dc.b	"..BAS.",0
	dc.b	TOUCHE_GAUCHE
	dc.b	"GAUCHE",0
	dc.b	TOUCHE_DROITE
	dc.b	"DROITE",0
	dc.b	TOUCHE_F1
	dc.b	"..F1..",0
	dc.b	TOUCHE_F2
	dc.b	"..F2..",0
	dc.b	TOUCHE_F3
	dc.b	"..F3..",0
	dc.b	TOUCHE_F4
	dc.b	"..F4..",0
	dc.b	TOUCHE_F5
	dc.b	"..F5..",0
	dc.b	TOUCHE_F6
	dc.b	"..F6..",0
	dc.b	TOUCHE_F7
	dc.b	"..F7..",0
	dc.b	TOUCHE_F8
	dc.b	"..F8..",0
	dc.b	TOUCHE_F9
	dc.b	"..F9..",0
	dc.b	TOUCHE_F10
	dc.b	"..F10.",0
	dc.b	TOUCHE_HELP
	dc.b	".HELP.",0
	dc.b	TOUCHE_SHIFT_G
	dc.b	"SHIFTG",0
	dc.b	TOUCHE_SHIFT_D
	dc.b	"SHIFTD",0
	dc.b	TOUCHE_CAPSLOCK
	dc.b	"CAPSLO",0
	dc.b	TOUCHE_ALT_G
	dc.b	"ALT-GA",0
	dc.b	TOUCHE_ALT_D
	dc.b	"ALT-DR",0
	dc.b	TOUCHE_AMIGA_G
	dc.b	"AMI-GA",0
	dc.b	TOUCHE_AMIGA_D
	dc.b	"AMI-DR",0
	dc.b	TOUCHE_CTRL
	dc.b	".CTRL.",0
	dc.b	TOUCHE_A
	dc.b	".. A .",0
	dc.b	TOUCHE_Z
	dc.b	".. Z .",0
	dc.b	TOUCHE_E
	dc.b	".. E .",0
	dc.b	TOUCHE_R
	dc.b	".. R .",0
	dc.b	TOUCHE_T
	dc.b	".. T .",0
	dc.b	TOUCHE_Y
	dc.b	".. Y .",0
	dc.b	TOUCHE_U
	dc.b	".. U .",0
	dc.b	TOUCHE_I
	dc.b	".. I .",0
	dc.b	TOUCHE_O
	dc.b	".. O .",0
	dc.b	TOUCHE_P
	dc.b	".. P .",0
	dc.b	TOUCHE_Q
	dc.b	".. Q .",0
	dc.b	TOUCHE_S
	dc.b	".. S .",0
	dc.b	TOUCHE_D
	dc.b	".. D .",0
	dc.b	TOUCHE_F
	dc.b	".. F .",0
	dc.b	TOUCHE_G
	dc.b	".. G .",0
	dc.b	TOUCHE_H
	dc.b	".. H .",0
	dc.b	TOUCHE_J
	dc.b	".. J .",0
	dc.b	TOUCHE_K
	dc.b	".. K .",0
	dc.b	TOUCHE_L
	dc.b	".. L .",0
	dc.b	TOUCHE_M
	dc.b	".. M .",0
	dc.b	TOUCHE_W
	dc.b	".. W .",0
	dc.b	TOUCHE_X
	dc.b	".. X .",0
	dc.b	TOUCHE_C
	dc.b	".. C .",0
	dc.b	TOUCHE_V
	dc.b	".. V .",0
	dc.b	TOUCHE_B
	dc.b	".. B .",0
	dc.b	TOUCHE_N
	dc.b	".. N .",0
	dc.b	TOUCHE_VIRGULE
	dc.b	".. , .",0
	dc.b	TOUCHE_POINTVIR
	dc.b	".. ; .",0
	dc.b	TOUCHE_DEUXPOIN
	dc.b	".. : .",0
	dc.b	TOUCHE_EGALE
	dc.b	".. = .",0
	dc.b	TOUCHE_NUM_1
	dc.b	".NUM1.",0
	dc.b	TOUCHE_NUM_2
	dc.b	".NUM2.",0
	dc.b	TOUCHE_NUM_3
	dc.b	".NUM3.",0
	dc.b	TOUCHE_NUM_4
	dc.b	".NUM4.",0
	dc.b	TOUCHE_NUM_5
	dc.b	".NUM5.",0
	dc.b	TOUCHE_NUM_6
	dc.b	".NUM6.",0
	dc.b	TOUCHE_NUM_7
	dc.b	".NUM7.",0
	dc.b	TOUCHE_NUM_8
	dc.b	".NUM8.",0
	dc.b	TOUCHE_NUM_9
	dc.b	".NUM9.",0
	dc.b	TOUCHE_NUM_0
	dc.b	".NUM0.",0
	dc.b	TOUCHE_NUM_MOIN
	dc.b	".NUM-.",0
	dc.b	TOUCHE_NUM_PLUS
	dc.b	".NUM+.",0
	dc.b	TOUCHE_NUM_POIN
	dc.b	" NUM. ",0
	dc.b	TOUCHE_NUM_CROG
	dc.b	".NUM[.",0
	dc.b	TOUCHE_NUM_CROD
	dc.b	".NUM].",0
	dc.b	TOUCHE_NUM_SLAS
	dc.b	".NUM/.",0
	dc.b	TOUCHE_NUM_ETOI
	dc.b	".NUM*.",0
.end	dc.b	0
	dc.b	"??????",0
