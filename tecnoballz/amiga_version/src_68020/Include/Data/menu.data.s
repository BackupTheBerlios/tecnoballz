*:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:*
*::*::*::*::*::*::*:: Données utilisés par les menus *::*::*::*::*::*::*::*
*:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:::-:*
		CNOP	0,4
viking.screen	dc.l	0
viking.input	dc.l	0
viking.menu	dc.l	0		pt/structure menu en cours
scroll.text	dc.l	0,0,"TEXT",0

		CNOP	0,4
ii.lutin	dc.l	0,0,"LUTI",MEMF_CHIP
ii.table	dc.l	0,64*VKN*4,"TABL",0
ii.luti		ds.l	8		Listes Lutins LOGO
ii.luti2	ds.l	8		Listes Lutins texte menu
ii.bob		dc.l	0		pt/SBOB du LOGO (ombre)
ii.bob2		dc.l	0		pt/SBOB pointeur Souris
ii1		dc.l	0		SBOB cache fonte gauche
ii2		dc.l	0		SBOB cache fonte droit
		dc.l	0		0=debut de la liste
iii		ds.l	AFNOR		pt/les SBOBS Fontes
iiii		dc.l	0		0=fin de la liste
pt.iii		dc.l	0		pt/iii
iis		dc.l	0		pt/texte
ii.haut		dc.w	0		Hauteur du LOGO
ii.x		dc.w	32		Poisiton X texte Menu
ii.y		dc.w	80		Position Y texte Menu
xs1		dc.w	0		varie largeur
xs.pt		dc.w	0	
acty		dc.w	0		action efface/affiche texte
act.line	dc.w	0		n° ligne à afficher
act.restore	dc.w	0	
act.flip	dc.w	0
act.select	dc.w	0
viking.cx	dc.w	0		position X en caractères
viking.cy	dc.w	0		position Y en ligne caractères
viking.lenght	dc.w	0		longeur chaine
viking.curs	dc.w	0
viking_wait	dc.w	1		tempo curseur
viking.left	dc.b	0		flag souris bouton gauche préssé
viking.right	dc.b	0		flag souris bouton droit préssé
viki0		dc.w	0		compteur répétion bouton gauche
viki1		dc.w	0		compteur répétion bouton droit

***************************** MENU PRINCIPAL ******************************
		CNOP	0,4
viki.main	dc.w	9,0				nbre de lignes
		dc.l	text.viking-t			pt/text ASCII
		dc.l	text.jmp-t			pt/table de saut
text.viking	dc.b	"        START THE GAMEq         "
		dc.b	"          ? OPTIONS ?           "
		dc.b	"          > CREDITS <           "
		dc.b	"         g GREETINGS g          "
		dc.b	"             INFOS              "
		dc.b	"          VIEW-SCORES           " 
		dc.b	"           PASSWORDq            "
tv2		dc.b	"        u ..........? t         "
		dc.b	"         LEVELS EDITOR          "
		dc.b	" EXIT AMIGADOS AND SAVE SCORESw "
		dc.b	0
		EVEN
		CNOP	0,4
text.jmp	dc.l	startg-t,startg-t
		dc.l	option-t,option-t
		dc.l	crediz-t,crediz-t
		dc.l	greeti-t,greeti-t
		dc.l	inform-t,inform-t
		dc.l	scorev-t,scorev-t
		dc.l	passwo-t,passwo-t
		dc.l	passwo-t,passwo-t
		dc.l	levedi-t,levedi-t
		dc.l	exitto-t,exitto-t

*««««««««««««««««««««««««««««« MENU OPTIONS »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
		CNOP	0,4
intro_name	dc.l	24+text.options3-t
		dc.l	24+text.options4-t
		dc.l	24+text.options5-t
		dc.l	24+text.options6-t
		dc.l	24+text.options7-t
		dc.l	24+text.options8-t
viki.options	dc.w	10,0
		dc.l	text.options1-t
		dc.l	options.jmp-t
text.options1	dc.b	" e GAME MODE          u ALONE t "
text.options2	dc.b	" e NUMBER OF PLAYERS  u 1 t     "
text.options3	dc.b	" b PLAYER 1.......... u ALBERTt "
text.options4	dc.b	" b PLAYER 2.......... u LEON  t "
text.options5	dc.b	" b PLAYER 3.......... u ANDRE t "
text.options6	dc.b	" b PLAYER 4.......... u GERARDt "
text.options7	dc.b	" b PLAYER 5.......... u RAOUL t "
text.options8	dc.b	" b PLAYER 6.......... u MARCELt "
text.options9	dc.b	" e DIFFICULTY........ u EASY t  "
text.options10	dc.b	" e NUMBER OF LIFES... u 08 t    "
		dc.b	" c MAIN MENUqq                  "  
		EVEN
		CNOP	0,4
options.jmp	dc.l	gamemo-t,gamemo-t
		dc.l	nuofpa-t,nuofpa-t
		dc.l	change-t,change-t
		dc.l	change-t,change-t
		dc.l	change-t,change-t
		dc.l	change-t,change-t
		dc.l	change-t,change-t
		dc.l	change-t,change-t
		dc.l	levexx-t,levexx-t
		dc.l	livexx-t,livexx-t
		dc.l	main.rx-t,main.rx-t

*x««««««««««««««««««««««««««x* MENU  CREDIT *x»»»»»»»»»»»»»»»»»»»»»»»»»»»x*
		CNOP	0,4
viki.credit	dc.w	18,0
		dc.l	text.credit-t
		dc.l	0
text.credit	dc.b	"- MAIN CODING - AGA-CONVERTION -"
		dc.b	"      b BRUNO  ETHVIGNOT b      "
		dc.b	"-------- ANOTHER CODING --------"
		dc.b	"        b JEROME BOLOT b        "
		dc.b	"                                "
		dc.b	"----------- MAIN GFX -----------"
		dc.b	"bJEAN MICHEL  MARTIN DE SANTEROb"
		dc.b	" --------- OTHER  GFX --------- "
		dc.b	"       b RODOLPHE  BONO b       "
		dc.b	"                                "
		dc.b	"---------- ALL MUSICS ----------"  
		dc.b	"        b REGIS PARRET b        "
		dc.b	"---------SOUNDS EFFECTS---------"
		dc.b	"       b LAURENT  GUYON  b      "
		dc.b	"                                "
		dc.b	" COPYRIGHT (C)1992-96 TLK-GAMES "
		dc.b	"TLK-GAMES/BP 24/F81150 FLORENTIN"
		dc.b	"TEL 63.53.26.37  FAX 63.53.20.11"
text.creditx	dc.b	" ?????????????????????????????? "
		EVEN

*x««««««««««««««««««««««««««x* MENU  INFOS *x»»»»»»»»»»»»»»»»»»»»»»»»»»»x*
		CNOP	0,4
viki.infos 	dc.w	18,0
		dc.l	text.infos-t
		dc.l	0
text.infos 	dc.b	" MOUSE PORT 0       : MODE ALONE"
text.infos2  	dc.b	" MOUSES PORT 0 AND 1: MODE TEAM "
		dc.b	" LEFT MOUSE BUTTON  : FIRE      "
		dc.b	" RIGHT MOUSE BUTTON : BALL EJECT"
		dc.b	" LEFT AND RIGHT     : TILT      "
		dc.b	"                  AND GIGABLITZ " 
		dc.b	" 25000 POINTS       : BONUS LIFE"
		dc.b	" AMIGA-RIGHT AND P  : PAUSE     "
		dc.b	" AMIGA-RIGHT AND X  : GAME-OVER "
		dc.b	" AMIGA-RIGHT AND Q  : EXIT MENU "
		dc.b	" AMIGA-RIGHT AND S  : SFX - MFX "
	 	dc.b	" AMIGA-RIGHT AND D  : MFX ONLY  "
	 	dc.b	" AMIGA-RIGHT AND F  : SFX ONLY  "
		dc.b	"                                "
		dc.b	"     PLEASE CONTACT ME TO >     "
		dc.b	"        ETHVIGNOT  BRUNO        " 		
		dc.b	"      17A, RUE JULES FERRY      "
		dc.b	"     25000 BESANCON  FRANCE     "
		dc.b	"TEL : 81.52.41.85  / 81.52.77.36"
		EVEN

*x«x«x«x«x«x«x«x«x«x«x«x«x«x* MENU GREETINGS *x»x»x»x»x»x»x»x»x»x»x»x»x»x»*
		CNOP	0,4
viki.greet	dc.w	15,0
		dc.l	text.greet-t			04
		dc.l	0				08
text.greet	dc.b	"---> BEST GREETINGS  FLY TO <---"
		dc.b	" b ATOL         b AURELIEN      "
		dc.b	" b BIP-BIP      b DELPHINE      "
		dc.b	" b DAUBMAN      b DAVID         "
		dc.b	" b DIESEL       b JEANINE       "
		dc.b	" b DJI          b LA GRANDE ZAZA"
		dc.b	" b FABRICE      b LOUISA        "
		dc.b	" b FRED         b MARIE-JEANNE  "
		dc.b	" b GREG         b MULTISCAN     "
		dc.b	" b GRYZOR       b MME ZIBIBI    "
		dc.b	" b JMM FROM TLK b OLIVIER       "
		dc.b	" b HERETIQUES   b PASCAL E.     "   
		dc.b	" b PASCAL L.    b PIERRE        "
		dc.b	" b THIBAUD D.   b REGLIS        "
		dc.b	" b THIERRY V.   b ROMAIN        "
		dc.b	" b ZIBIBI       b SOPHIE        "
		EVEN
		CNOP	0,4
viki.scores	dc.w	15,0
		dc.l	text.score-t
		dc.l	0
text.score	dc.b	"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
		dc.b	"b        BEST  TECNOBALLZ      b"
		dc.b	"b                              b"
		dc.b	"b POS NAME   LEVEL AREA SCORE  b"
		dc.b	"b                              b"
lesscores	dc.b	"b 01  ?????? ??    ?    ?????? b"
		dc.b	"b 02  ?????? ??    ?    ?????? b"
		dc.b	"b 03  ?????? ??    ?    ?????? b"
		dc.b	"b 04  ?????? ??    ?    ?????? b"
		dc.b	"b 05  ?????? ??    ?    ?????? b"
		dc.b	"b 06  ?????? ??    ?    ?????? b"
		dc.b	"b 07  ?????? ??    ?    ?????? b"
		dc.b	"b 08  ?????? ??    ?    ?????? b"
		dc.b	"b 09  ?????? ??    ?    ?????? b"
		dc.b	"b 10  ?????? ??    ?    ?????? b"
		dc.b	"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
		EVEN

*--==--==--==--> Offset des fontes dans  "fonte80.pagegfx" <--==--==--==--*
fonte80 dc.b	26	 32 ' '
	dc.b	37	 33 '!'
	dc.b	40	 34 '"'
	dc.b	26	 35 '#' ð=espace
	dc.b	26	 36 '$' ð=espace
	dc.b	26	 37 '%' ð=espace
	dc.b	26	 38 '&' ð=espace
	dc.b	40	 39 "'"
	dc.b	48	 40 '('
	dc.b	49	 41 ')'
	dc.b	47	 42 '*'
	dc.b	26	 43 '+' ð=espace
	dc.b	26	 44 ',' 
	dc.b	41	 45 '-'
	dc.b	42	 46 '.'
	dc.b	26	 47 '/' ð=espace
	dc.b	27	 48 '0'
	dc.b	28	 49 '1'
	dc.b	29	 50 '2'
	dc.b	30	 51 '3'
	dc.b	31	 52 '4'
	dc.b	32	 53 '5'
	dc.b	33	 54 '6'
	dc.b	34	 55 '7'
	dc.b	35	 56 '8'
	dc.b	36	 57 '9'
	dc.b	39	 58 ':'
	dc.b	38	 59 ';'
	dc.b	44	 60 '<'
	dc.b	26	 61 '=' ð=espace
	dc.b	45	 62 '>'
	dc.b	43	 63 '?'
	dc.b	26	 64 '@' ð=espace
	dc.b	0	 65 'A'
	dc.b	1	 66 'B'
	dc.b	2	 67 'C'
	dc.b	3	 68 'D'
	dc.b	4	 69 'E'
	dc.b	5	 70 'F'
	dc.b	6	 71 'G'
	dc.b	7	 72 'H'
	dc.b	8	 73 'I'
	dc.b	9	 74 'J'
	dc.b	10	 75 'K'
	dc.b	11	 76 'L'
	dc.b	12	 77 'M'
	dc.b	13	 78 'N'
	dc.b	14	 79 'O'
	dc.b	15	 80 'P'
	dc.b	16	 81 'Q'
	dc.b	17	 82 'R'
	dc.b	18	 83 'S'
	dc.b	19	 84 'T'
	dc.b	20	 85 'U'
	dc.b	21	 86 'V'
	dc.b	22	 87 'W'
	dc.b	23	 88 'X'
	dc.b	24	 89 'Y'
	dc.b	25	 90 'Z'
	dc.b	48	 91 '['
	dc.b	26	 92 '\' ð=espace
	dc.b	49	 93 ']'
	dc.b	26	 94 '^' ð=espace
	dc.b	26	 95 '_' ð=espace
	dc.b	26	 96 '`' ð=espace
	dc.b	26	 97 'a' ð=espace
	dc.b	47	 98 'b' étoile grise
	dc.b	52	 99 'c' traits horizontaux
	dc.b	26	100 'd' ð=espace
	dc.b	50	101 'e' carré plein
	dc.b	26	102 'f' ð=espace
	dc.b	46	103 'g' coeur gris
	dc.b	26	104 'h' ð=espace
	dc.b	26	105 'i' ð=espace
	dc.b	26	106 'j' ð=espace
	dc.b	26	107 'k' ð=espace
	dc.b	26	108 'l' ð=espace
	dc.b	26	109 'm' ð=espace
	dc.b	26	110 'n' ð=espace
	dc.b	26	111 'o' ð=espace
	dc.b	26	112 'p' ð=espace
	dc.b	55	113 'q' ! blanc
	dc.b	26	114 'r' ð=espace
	dc.b	26	115 's' ð=espace
	dc.b	53	116 't' < blanc
	dc.b	54	117 'u' > blanc
	dc.b	26	118 'v' ð=espace
	dc.b	51	119 'w' 3 points de suspensions roses
	dc.b	26	120 'x' ð=espace
	dc.b	26	121 'y' ð=espace
	dc.b	26	122 'z' ð=espace
	dc.b	26	123 '{' ð=espace
	dc.b	26	124 '|' ð=espace
	dc.b	26	125 '}' ð=espace
	dc.b	26	126 '~' ð=espace
	dc.b	26	127 ' ' ð=espace
	EVEN

*----------* POINTEUR SUR L'IMAGE DU SBOB SUIVANT LE CARACTERE *----------*
skid:	dc.w	26*8	 32 ' ' ESPACE
	dc.w	37*8	 33 '!'
	dc.w	39*8	 34 '''
	dc.w	26*8	 35 '#' ð=espace
	dc.w	26*8	 36 '$' ð=espace
	dc.w	26*8	 37 '%' ð=espace
	dc.w	26*8	 38 '&' ð=espace
	dc.w	39*8	 39 "'"
	dc.w	26*8	 40 '(' ð=espace
	dc.w	26*8	 41 ')' ð=espace
	dc.w	26*8	 42 '*' ð=espace
	dc.w	26*8	 43 '+' ð=espace
	dc.w	40*8	 44 ',' 
	dc.w	41*8	 45 '-'
	dc.w	38*8	 46 '.'
	dc.w	26*8	 47 '/' ð=espace
	dc.w	27*8	 48 '0'
	dc.w	28*8	 49 '1'
	dc.w	29*8	 50 '2'
	dc.w	30*8	 51 '3'
	dc.w	31*8	 52 '4'
	dc.w	32*8	 53 '5'
	dc.w	33*8	 54 '6'
	dc.w	34*8	 55 '7'
	dc.w	35*8	 56 '8'
	dc.w	36*8	 57 '9'
	dc.w	26*8	 58 ':' ð=espace
	dc.w	26*8	 59 ';' ð=espace
	dc.w	26*8	 60 '<' ð=espace
	dc.w	26*8	 61 '=' ð=espace
	dc.w	26*8	 62 '>' ð=espace
	dc.w	26*8	 63 '?' ð=espace
	dc.w	26*8	 64 '@' ð=espace
	dc.w	00*8	 65 'A'
	dc.w	01*8	 66 'B'
	dc.w	02*8	 67 'C'
	dc.w	03*8	 68 'D'
	dc.w	04*8	 69 'E'
	dc.w	05*8	 70 'F'
	dc.w	06*8	 71 'G'
	dc.w	07*8	 72 'H'
	dc.w	08*8	 73 'I'
	dc.w	09*8	 74 'J'
	dc.w	10*8	 75 'K'
	dc.w	11*8	 76 'L'
	dc.w	12*8	 77 'M'
	dc.w	13*8	 78 'N'
	dc.w	14*8	 79 'O'
	dc.w	15*8	 80 'P'
	dc.w	16*8	 81 'Q'
	dc.w	17*8	 82 'R'
	dc.w	18*8	 83 'S'
	dc.w	19*8	 84 'T'
	dc.w	20*8	 85 'U'
	dc.w	21*8	 86 'V'
	dc.w	22*8	 87 'W'
	dc.w	23*8	 88 'X'
	dc.w	24*8	 89 'Y'
	dc.w	25*8	 90 'Z'
	dc.w	26*8	 91 '[' ð=espace
	dc.w	26*8	 92 '\' ð=espace
	dc.w	26*8	 93 ']' ð=espace
	dc.w	26*8	 94 '^' ð=espace
	dc.w	26*8	 95 '_' ð=espace
	dc.w	26*8	 96 '`' ð=espace
	dc.w	26*8	 97 'a' ð=espace
	dc.w	26*8	 98 'b' ð=espace
	dc.w	26*8	 99 'c' ð=espace
	dc.w	26*8	100 'd' ð=espace
	dc.w	26*8	101 'e' ð=espace
	dc.w	26*8	102 'f' ð=espace
	dc.w	26*8	103 'g' ð=espace
	dc.w	26*8	104 'h' ð=espace
	dc.w	26*8	105 'i' ð=espace
	dc.w	26*8	106 'j' ð=espace
	dc.w	26*8	107 'k' ð=espace
	dc.w	26*8	108 'l' ð=espace
	dc.w	26*8	109 'm' ð=espace
	dc.w	26*8	110 'n' ð=espace
	dc.w	26*8	111 'o' ð=espace
	dc.w	26*8	112 'p' ð=espace
	dc.w	26*8	113 'q' ð=espace
	dc.w	26*8	114 'r' ð=espace
	dc.w	26*8	115 's' ð=espace
	dc.w	26*8	116 't' ð=espace
	dc.w	26*8	117 'u' ð=espace
	dc.w	26*8	118 'v' ð=espace
	dc.w	26*8	119 'w' ð=espace
	dc.w	26*8	120 'x' ð=espace
	dc.w	26*8	121 'y' ð=espace
	dc.w	26*8	122 'z' ð=espace
	dc.w	26*8	123 '{' ð=espace
	dc.w	26*8	124 '|' ð=espace
	dc.w	26*8	125 '}' ð=espace
	dc.w	26*8	126 '~' ð=espace
	dc.w	26*8	127 ' ' ð=espace
