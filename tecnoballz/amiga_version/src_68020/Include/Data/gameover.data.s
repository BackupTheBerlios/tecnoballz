		CNOP	0,4
go.beast	dc.l	0,0,"LUTI",MEMF_CHIP	Lutin HiScores
pt_boule	dc.l	0,0,"SHAP",0		pt/SBOB
go.list		ds.l	8
gobot.adr	dc.l	0	Début affichage texte
go.tempo	dc.w	0	temporisation des lettres suivant la courbe
go.aff		dc.w	0	pt/lettre à afficher
go.dep		dc.w	0	pt/routine déplacement lettres "GAMEOVER"
gobot.flag	dc.w	0	flag 1=affichage des scores 
gobot.pt	dc.w	0	pt/text scores
gobot.offset	dc.w	0	pt/ position affichage texte
go.parasite	dc.w	0
gameover.out	dc.w	0
gow		dc.w	0	WINNER BOULE
gow2		dc.w	0
gow3		dc.w	0
gow4		dc.w	0

*/ courbe déplacement des lettres */
sinus_over:	dc.b	159,159,159,159,158,158,158,157,156,156
		dc.b	155,154,153,152,151,150,149,148,147,145
		dc.b	144,142,141,139,137,135,134,132,130,128
		dc.b	126,124,122,120,117,115,113,111,108,106
		dc.b	104,101,099,096,094,091,089,086,084,081
		dc.b	079,076,074,071,069,066,064,061,059,057
		dc.b	054,052,050,047,045,043,040,038,036,034
		dc.b	032,030,028,026,024,023,021,019,018,016
		dc.b	015,013,012,011,009,008,007,006,005,004
		dc.b	004,003,002,002,001,001,000,000,000,000
		dc.b	000,000,000,000,001,001,002,002,003,004
		dc.b	004,005,006,007,008,009,011,012,013,015
		dc.b	016,018,019,021,023,024,026,028,030,032
		dc.b	034,036,038,040,043,045,047,050,052,054
		dc.b	057,059,061,064,066,069,071,074,076,079
		dc.b	081,084,086,089,091,094,096,099,101,104
		dc.b	106,108,111,113,115,117,120,122,124,126
		dc.b	128,130,132,134,135,137,139,141,142,144
		dc.b	145,147,148,149,150,151,152,153,154,155
		dc.b	156,156,157,158,158,158,159,159,159,159

*/ coodonnées initiales des lettres bobs 'GAME OVER' */
zeus_over:	dc.w	000,175			G
		dc.w	025,150			A
		dc.w	050,125			M
		dc.w	075,100			E
		dc.w	100,075			O
		dc.w	125,050			V
		dc.w	150,025			E
		dc.w	175,000			R

		*/ TEXTES DES HIGH-SCORES SI PROBLEME DE LECTURE DISQUE */
xxgo_te		dc.b	'T'-65,'E'-65,'C'-65,'N'-65,'O'-65,'B'-65,'A'-65
		dc.b	'L'-65,'L'-65,'['-65,'T'-65,'O'-65,'P'-65,'['-65
		dc.b	'T'-65,'E'-65,'N'-65
		dc.b	$FF
		dc.b	'N'-65,'A'-65,'M'-65,'E'-65,'['-65,'['-65
		dc.b	'S'-65,'C'-65,'O'-65,'R'-65,'E'-65,'['-65,'['-65
		dc.b	'A'-65,'R'-65,'E'-65,'A'-65,'['-65
		dc.b	'L'-65,'E'-65,'V'-65,'E'-65,'L'-65
		dc.b	$FF
		dc.b	'L'-65,'A'-65,'M'-65,'E'-65,'R'-65,'['-65	nom
		dc.b	'['-65
		dc.b	'\'-65,'\'-65,'\'-65,'\'-65,'\'-65,'\'-65	score
		dc.b	'['-65,'['-65,'i'-65
		dc.b	'\'-65						area
		dc.b	'['-65,'['-65,'['-65,'['-65
		dc.b	'\'-65,'\'-65					level
		dc.b	$ff
