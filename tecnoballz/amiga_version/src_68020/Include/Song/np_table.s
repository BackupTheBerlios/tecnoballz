*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
**************** Table utilisées par les routines bruitages ***************
*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
* 'np1'		structures utilisés par replayroutine son
* 'xt'		table des sons (taille, fréquence...)
* 'sample_rate'	table des samples rates
* 'sound'	table des son demandés (byte-flag)
*
*/ INITIALISE STRUCTURE BRUITAGE */ 
		RSRESET
NP_NEWS		rs.w	1	Nouveau sample demandé (n° de ce sample)
NP_WAIT		rs.w	1	Temps d'émission du sample
NP_STAR		rs.l	1	Adresse du sample pour le repeat
NP_PRIO		rs.w	1	Priorité du sample
		rs.w	1	Aligement long-mot
NP_SIZE		rs	0
*-------------------------------------------------------------------------*
		CNOP	0,4
pt_poz:		dc.l	0			Adresse table positions des modules 
soundtable	dc.l	0,0,'BRUI',MEMF_CHIP
np_oldirq:	dc.l	0			Vecteur de niveau 6 système
*-------------------------------------------------------------------------*
np0		ds.l	SIZE	
np1		ds.l	SIZE	
np2		ds.l	SIZE	
np3		ds.l	SIZE	
*-------------------------------------------------------------------------*
sound		dcb.b	S_NOMBRE,0
		dc.b	"Vous aimez la vraie musique Techno ? "
		dc.b	"Rendez vous à l'AnFer 8 rue Marceau F-21000 DIJON"
		EVEN
*-------------------------------------------------------------------------*
np_dmacon	dc.w	0		 DMACON	**
music.flag	dc.w	0		0=Musique   /  1=Bruitage
music34		dc.w	0		0=3 Cannaux /  1=4 Cannaux
musiclost	dc.w	0		1=Musique "Lost" en cours
music4voix	dc.w	0		0=Musique 4 Cannaux
musicsound	dc.w	0		1=Musique + Bruitages
music.on	dc.w	0		0=Autorise saut à la "play-routine"
*/ table des positions des modules */-------------------------------------*
poz:		dc.w	00		00 pos. musique jeu 1
		dc.w	11		02 pos. musique jeu 2
		dc.w	23		04 pos. musique win
		dc.w	24		06 pos. musique lost
		dc.w	25		08 pos. musique shop
poz2		dc.w	00,15,22,23,24
		dc.w	00,15,28,29,30
		dc.w	00,11,18,19,20
		dc.w	00,15,30,31,32
*/ table des bruitages, adr. samples, taille, période */------------------*
xt:		dc.l	LIFE_MOINS		00 : adr. sample	(01)
		dc.w	2494/2			04 : taille sample en mot
		dc.w	2			06 : pt/période : $358 = C1
		dc.w	0			08 : nbre de vbl pendant lequel le son se joue 
		dc.w	S_ENLEVE.VIE		10 : priorité
		dc.l	0			12 : libre
t2		dc.l	EXPLO_RAK					(02)
		dc.w	1866/2,2		$358 = C1
		dc.w	0
		dc.w	S_RAK.EXPLO
		dc.l	0
		dc.l	LIFE_PLUS					(03) 
		dc.w	5212/2,26		$1ac = C2
		dc.w	0
		dc.w	S_AJOUTE.VIE
		dc.l	0
		dc.l	BRICOTE					(04)
		dc.w	3096/2,38		$12e = F#2
		dc.w	0
		dc.w	S_MONNAIE
		dc.l	0
		dc.l	ALARM						(05)
		dc.w	2620/2,2		$358 = C1
		dc.w	0
		dc.w	S_TILT.ALARM
		dc.l	0
		dc.l	GADGET_SOUND					(06) 
		dc.w	5834/2,26		$1ac = C2
		dc.w	0
		dc.w	S_GADGET
		dc.l	0
		dc.l	TECNO						(07)
		dc.w	3688/2,2		$358 = C1
		dc.w	0
		dc.w	S_TECNO
		dc.l	0
		dc.l	APPAR						(08)
		dc.w	2484/2,2		$358 = C1
		dc.w	0
		dc.w	S_ATOM.APPARAIT
		dc.l	0
		dc.l	TRANSFO					(09)
		dc.w	5726/2,26		$1ac = C2
		dc.w	0
		dc.w	S_TRANSFORME
		dc.l	0
		dc.l	TIR_MONSTRE					(10)
		dc.w	3134/2,2		$358 = C1
		dc.w	0
		dc.w	S_TIR.GARDIEN
		dc.l	0
		dc.l	TIR_RAK					(11)
		dc.w	1446/2,26		$1ac = C2
		dc.w	0
		dc.w	S_RAK.TIR
		dc.l	0
		dc.l	EXPLO_BIG					(12)
		dc.w	3236/2,2		$358 = C1
		dc.w	0
		dc.w	S_BIG.EXPLO
		dc.l	0
		dc.l	DETRUIT_INDES					(13)
		dc.w	1720/2,2		$358 = C1
		dc.w	0
		dc.w	S_CASSE.INDEST
		dc.l	0
		dc.l	EXPLO_ATOM					(14)
		dc.w	1794/2,26		$1ac = C2
		dc.w	0
		dc.w	S_ATOM.EXPLO
		dc.l	0
		dc.l	MONSTRE_TOUCHE				(15)
		dc.w	1748/2,26		$1ac = C2
		dc.w	0
		dc.w	S_GARDIEN.TOUCH
		dc.l	0
		dc.l	ATOM						(16)
		dc.w	0392/2,28		$194 = C#2	
		dc.w	0
		dc.w	S_TOUCHE.ATOM
		dc.l	0
		dc.l	INDES_1					(17)
		dc.w	1730/2,2		$358 = C1
		dc.w	0
		dc.w	S_TOUCHE.INDES1
		dc.l	0
		dc.l	INDES_2					(18)
		dc.w	1732/2,26		$1ac = C2
		dc.w	0
		dc.w	S_TOUCHE.INDES2
		dc.l	0
		dc.l	RAQUETTE					(19)
		dc.w	2628/2,26		$1ac = C2	
		dc.w	0
		dc.w	S_TOUCHE.RAK
		dc.l	0
		dc.l	BRICOTE					(20)
		dc.w	4096/2,26		$1ac = C2
		dc.w	0
		dc.w	S_BRICOTE
		dc.l	0
		dc.l	BRIQUE					(21)
		dc.w	1052/2,26	2		$358 = C1
		dc.w	0
		dc.w	S_TOUCHE.BRIK
		dc.l	0
		dc.l	BRIQUE					(22)
		dc.w	1052/2,50 ;(c3)		32$168 = D#2
		dc.w	0
		dc.w	S_TOUCHE.BRIK2
		dc.l	0
		dc.l	BRIQUE					(23)
		dc.w	1052/2,36		$140 = F2
		dc.w	0
		dc.w	S_TOUCHE.BRIK3
		dc.l	0
		dc.l	BRIQUE					(24)
		dc.w	1052/2,40		$11d = G2
		dc.w	0
		dc.w	S_TOUCHE.BRIK4
		dc.l	0
		dc.l	BRIQUE					(25)
		dc.w	1052/2,44		$0fe = A2
		dc.w	0
		dc.w	S_TOUCHE.BRIK5
		dc.l	0
		dc.l	EJECT					(26)
		dc.w	1802/2,26		$1ac = C2
		dc.w	0
		dc.w	S_COIN.ASPIRE
		dc.l	0
		dc.l	ASPIRE					(27)
		dc.w	0300/2,26		$1ac = C2
		dc.w	0
		dc.w	S_COIN.EJECT
		dc.l	0
		dc.l	ASPIRE					(28)
		dc.w	0300/2,26		$1ac = C2
		dc.w	0
		dc.w	S_COIN.EJECT
		dc.l	0
t_end
KOFFER		=(t_end-xt)/(t2-xt)
*/ Table des "SampleRates" */---------------------------------------------*
sample_rate:	dc.w	4181			02 C1	 261,7 hz °
		dc.w	4430			04 C#1	 277,2 hz
		dc.w	4709			06 D1	 293,7 hz
		dc.w	4971			08 D#1	 311,2 hz
		dc.w	5264			10 E1	 329,7 hz
		dc.w	5593			12 F1	 349,3 hz
		dc.w	5965			14 F#1	 370,0 hz
		dc.w	6302			16 G1	 392,0 hz
		dc.w	6678			18 G#1	 415,3 hz
		dc.w	7046			20 A1	 440,0 hz
		dc.w	7457			22 A#1	 466,2 hz
		dc.w	7919			24 B1	 493,9 hz
		dc.w	8363			26 C2	 523,4 hz °
		dc.w	8860			28 C#2	 554,4 hz
		dc.w	9419			30 D2	 587,4 hz
		dc.w	9943			32 D#2	 622,4 hz
		dc.w	10528			34 E2	 659,4 hz
		dc.w	11186			36 F2	 698,6 hz
		dc.w	11931			38 F#2	 740,0 hz
		dc.w	12604			40 G2	 784,0 hz
		dc.w	13356			42 G#2	 830,6 hz
		dc.w	14092			44 A2	 880,0 hz
		dc.w	14914			46 A#2	 932,4 hz
		dc.w	15838			48 B2	 987,8 hz
		dc.w	16726			50 C3	1046,8 hz
		dc.w	17720			52 C#3	1108,8 hz
		dc.w	18839			54 D3	1174,8 hz
		dc.w	19886			56 D#3	1244,8 hz
		dc.w	21056			58 E3	1318,8 hz
		dc.w	22372			60 F3	1397,2 hz
		dc.w	23863			62 F#3	1480,0 hz
		dc.w	25208			64 G3	1568.0 hz
		dc.w	26713			66 G#3	1661,2 hz
		dc.w	28185			68 A3	1760,0 hz
		dc.w	29829			70 A#3	1864,8 hz
		dc.w	31677			72 B3	1975,6 hz
*-------------------------------------------------------------------------*
np_per1:	dc.w	$0358			02 C1	 261,7 hz °
		dc.w	$0328			04 C#1	 277,2 hz
		dc.w	$02fa			06 D1	 293,7 hz
		dc.w	$02d0			08 D#1	 311,2 hz
		dc.w	$02a6			10 E1	 329,7 hz
		dc.w	$0280			12 F1	 349,3 hz
		dc.w	$025c			14 F#1	 370,0 hz
		dc.w	$023a			16 G1	 392,0 hz
		dc.w	$021a			18 G#1	 415,3 hz
		dc.w	$01fc			20 A1	 440,0 hz
		dc.w	$01e0			22 A#1	 466,2 hz
		dc.w	$01c5			24 B1	 493,9 hz
		dc.w	$01ac			26 C2	 523,4 hz °
		dc.w	$0194			28 C#2	 554,4 hz
		dc.w	$017d			30 D2	 587,4 hz
		dc.w	$0168			32 D#2	 622,4 hz
		dc.w	$0153			34 E2	 659,4 hz
		dc.w	$0140			36 F2	 698,6 hz
		dc.w	$012e			38 F#2	 740,0 hz
		dc.w	$011d			40 G2	 784,0 hz
		dc.w	$010d			42 G#2	 830,6 hz
		dc.w	$00fe			44 A2	 880,0 hz
		dc.w	$00f0			46 A#2	 932,4 hz
		dc.w	$00e2			48 B2	 987,8 hz
		dc.w	$00d6			50 C3	1046,8 hz ° 
		dc.w	$00ca			52 C#3	1108,8 hz
		dc.w	$00be			54 D3	1174,8 hz
		dc.w	$00b4			56 D#3	1244,8 hz
		dc.w	$00aa			58 E3	1318,8 hz
		dc.w	$00a0			60 F3	1397,2 hz
		dc.w	$0097			62 F#3	1480,0 hz
		dc.w	$008f			64 G3	1568.0 hz
		dc.w	$0087			66 G#3	1661,2 hz
		dc.w	$007f			68 A3	1760,0 hz
		dc.w	$0078			70 A#3	1864,8 hz
		dc.w	$0071			72 B3	1975,6 HZ
*-------------------------------------------------------------------------*
