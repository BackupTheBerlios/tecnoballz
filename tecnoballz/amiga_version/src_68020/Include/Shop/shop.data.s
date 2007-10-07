***************************************************************************
*««««««««««««««« Tables et textes se rapportant au magasin »»»»»»»»»»»»»»»*
***************************************************************************
		CNOP	0,4
shop.lutin	ds.l	16
shop.sprite	dc.l	0,4*18*2*8,'SPRI',MEMF_CHIP
bob.led		dc.l	0
bob.get		dc.l	0		BOB Gadget
bob.cli		dc.l	0		pt/BOB selectionné
sh.course	dc.l	0		pt/table gadget sélectionné
sh.line1	dc.l	0	^	pt/1iere ligne text 
sh.line2	dc.l	0	^	pt/2ieme ligne text
sh.line3	dc.l	0	!	pt/3ieme ligne text
sh.tablept	ds.w	NOPTIONS	table de chaque "sh.pt"
sh.tempo	ds.w	NOPTIONS	table de chaque "sh.pt"
sh.get		dc.w	0		1=gadget se ballade
sh.ptget	dc.w	0		pt/tables du gadget 
sh.clito	dc.w	0		clignote pointeur souris
sh.pt		dc.w	0		pt/tables
sh.infovu	dc.w	0		Flag info dèjà vu
sh.optinfo	dc.w	0		Flag 1 = opt. info
sh.left		dc.w	0		Flag bouton gauche préssé
sh.mousex	dc.w	0	^	x pt souris magasin
sh.mousey	dc.w	0	!	y pt souris magasin
sh.ax		dc.b	0		depl. rel. x &
sh.ay		dc.b	0		depl. rel. y pt souris
shopout		dc.w	0		sort du magasin
sh.tempo2	dc.w	1		tempos anim pointeur souris
sh.ptanim2	dc.w	0		pt/anim pointeur souris
sh.prix		dc.w	0		0=prix habituels /1=prix réduits
*/ Positions de la led */
curs_pos:	dc.w	016,036,064,036,112,036,160,036,208,036,256,036
		dc.w	016,084,064,084,112,084,160,084,208,084,256,084
		dc.w	016,132,064,132,112,132,160,132,208,132,256,132
		dc.w	016,180,064,180,112,180,160,180,208,180,256,180
		dc.w	016,228,016,228,016,228,016,228,016,228,016,228
*/ positions des bonus dans la page */
curs_boby	dc.l	P.SIZE_P,P.FIRE01,P.FIRE02,P.VIDE00,P.BALLE2,P.BALLE3
		dc.l	P.POWER1,P.POWER2,P.VIDE00,P.LIFE_P,P.VIDE00,P.WALL01
		dc.l	P.VIDE00,P.VIDE00,P.VIDE00,P.SIZE01,P.SIZE02,P.ROBOT1
		dc.l	P.CONTRO,P.GLUE00,P.VIDE00,P.VIDE00,P.VIDE00,P.VIDE00
		dc.l	P.VIDE00,P.VIDE00,P.VIDE00,P.VIDE00,P.VIDE00,P.VIDE00

*/ code des options */
curs_dst	dc.l	12,06,08,28,18,20	S+/F1/F2/RW/B2/B3
		dc.l	22,24,58,16,60,50	P1/P2/LB/L+/??/WA
		dc.l	38,36,34,40,42,52	BL/BU/BR/S2/S3/RB
		dc.l	54,02,62,62,62,62	CT/GL/XX/XX/XX/XX
		dc.l	62,62,62,62,62,62	XX/XX/XX/XX/XX/XX
*/ Routine achat */
curs_routine	dc.l	s.size_p,s.fire01,s.fire02,s.rebuil,s.balle2,s.balle3
		dc.l	s.power1,s.power2,s.lessbk,s.life_p,s.inform,s.wall01
		dc.l	s.bump04,s.bump03,s.bump02,s.size01,s.size02,s.robot1
		dc.l	s.contro,s.glue00,s.__exit,s.__exit,s.__exit,s.__exit
		dc.l	s.__exit,s.__exit,s.__exit,s.__exit,s.__exit,s.__exit
*/ pt/prix des options */
curs_pri2:	dc.l	pri06-t,pri04-t,pri07-t,pri11-t,pri03-t,pri08-t
		dc.l	pri09-t,pri14-t,pri12-t,pri13-t,pri01-t,pri04-t
		dc.l	pri05-t,pri05-t,pri05-t,pri06-t,pri04-t,pri05-t
		dc.l	pri06-t,pri02-t,pri00-t,pri00-t,pri00-t,pri00-t
		dc.l	pri00-t,pri00-t,pri00-t,pri00-t,pri00-t,pri00-t
tcurs_pri2	dc.l	pri99-t,pri99-t,pri99-t,pri99-t,pri99-t,pri99-t
		dc.l	pri99-t,pri99-t,pri99-t,pri99-t,pri99-t,pri99-t
		dc.l	pri99-t,pri99-t,pri99-t,pri99-t,pri99-t,pri99-t
		dc.l	pri99-t,pri99-t,pri99-t,pri99-t,pri99-t,pri99-t
		dc.l	pri99-t,pri99-t,pri99-t,pri99-t,pri99-t,pri99-t
*/ prix des options */
pri99:		dc.w	1,0,0,0,0,0,001
pri00:		dc.w	0,0,0,0,0,0,000
pri01:		dc.w	0,1,0,0,0,0,010
pri02		dc.w	0,5,1,0,0,0,150
pri03		dc.w	5,2,0,0,0,0,025
pri04		dc.w	5,7,0,0,0,0,075
pri05		dc.w	0,0,1,0,0,0,100
pri06		dc.w	0,6,0,0,0,0,060
pri07		dc.w	0,5,1,0,0,0,150
pri08		dc.w	0,5,0,0,0,0,050
pri09		dc.w	0,5,2,0,0,0,250
pri10		dc.w	0,0,3,0,0,0,300
pri11		dc.w	0,5,3,0,0,0,350
pri12		dc.w	0,0,4,0,0,0,400
pri13		dc.w	0,5,4,0,0,0,450
pri14		dc.w	0,0,5,0,0,0,500

*=========================================================================*
***************************> TEXTES DU MAGASIN <***************************
*=========================================================================*
*/ TEXTE DE BIENVENU */
text00:		dc.b	'WELCOME[[[[[[[[TO[THE['
		dc.b	'[[[[TECNOBALL[SHOP[[[['
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
		dc.b	'RANDOM[PRICE[BONUS[OK['
*/ T'AS PU DE FRIC */
text41:		dc.b	'SORRYj[BUT[YOU[HAVENhT'
		dc.b	'[[[GOT[ENOUGH[MONEY[[['
		dc.b	'[[TO[BUY[THIS[OPTION[['
*/ T'AS PLUS DE 20 GADGETS */
text63		dc.b	'[[[YOU[CAN[ONLY[BUY[[['
		dc.b	'[[[[[[^\[GADGETS[[[[[['
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
*/ REBUILD MUR A L'AREA 5 SEULEMENT */
text56		dc.b	'[[SORRY[[THIS[OPTION[['
		dc.b	'[[[CAN[ONLY[BE[USED[[['
		dc.b	'[[[[[[FOR[AREA[a[[[[[['

*/ TEXTES POUR CHAQUE OPTIONS ACHETEES */
police:		dc.l	00,01,02,03,04,05	S+/F1/F2/RW/B2/B3
		dc.l	06,07,08,09,10,11	P1/P2/LB/L+/??/WA
		dc.l	12,13,14,15,16,17	BL/BU/BR/S2/S3/RB
		dc.l	18,19,20,20,20,20	CT/GL/XX/XX/XX/XX
		dc.l	20,20,20,20,20,20	XX/XX/XX/XX/XX/XX
*- S+ --------------------------------------------------------------------*
text12:		dc.b	'[[[[AH[YEAH[IT[IS[[[[['
		dc.b	'[[MUCH[BETTER[WITH[A[['
		dc.b	'[SUCH[LONGER[BUMPER[[['
*- F1 --------------------------------------------------------------------*
		dc.b	'THIS[FIRE[IS[NOT[AS[SO'
		dc.b	'[POWERFULL[AS[FIRE[^[['
		dc.b	'[[BUT[IT[IS[CHEAPER[[['
*- F2 --------------------------------------------------------------------*
		dc.b	'[[[BIGGER[POWER[[TO[[['
		dc.b	'[[DESTROY[MORE[BRICS[['
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
*- RW --------------------------------------------------------------------*
		dc.b	'[[[OKj[THE[RED[WALLS[['
		dc.b	'[[[[WILL[BE[REBUILT[[['
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
*- B1 --------------------------------------------------------------------*
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
		dc.b	'[[TWO[BALLS[TO[CATCH[['
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
*- B2 --------------------------------------------------------------------*
		dc.b	'[[[[THREE[BALLS[NOW[[['
		dc.b	'[YOU[WILL[BRAKE[MORE[['
		dc.b	'[AND[FASTER[THE[WALL[['
*- P1 --------------------------------------------------------------------*
		dc.b	'[[A[VERY[STRONG[BALL[['
		dc.b	'[[[[TO[BREAK[BRIKS[[[['
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
*- P2 --------------------------------------------------------------------*
		dc.b	'[[WITH[THAT[YOU[WILL[['
		dc.b	'[[BREAK[ALL[YOU[WANT[['
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
*- LB --------------------------------------------------------------------*
		dc.b	'[THERE[WILL[]\[BRICKS['
		dc.b	'[[[[LESS[TO[BREAK[[[[['
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
*- L+ --------------------------------------------------------------------*
		dc.b	'[kkkkkkkkkkkkkkkkkkkk['
		dc.b	'[[[EXTRA[LIFE[ADDED[[['
		dc.b	'[kkkkkkkkkkkkkkkkkkkk['
*- ?? --------------------------------------------------------------------*
text.info	dc.b	'[[[[[[[[[[[[[[[[[[[[[['
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
*- WA --------------------------------------------------------------------*
		dc.b	'[[[[[[[[[[OK[[[[[[[[[['
		dc.b	'[[[[[[[FOR[[THE[[[[[[['
		dc.b	'[[[[[[[[[WALL[[[[[[[[['
*- BL --------------------------------------------------------------------*
		dc.b	'[OK[FOR[A[LEFT[BUMPER['
		dc.b	'BUT[DON[T[FORGET[IT[IS'
		dc.b	'[OVER[AFTER[_[LEVELS[['
*- BU --------------------------------------------------------------------*
		dc.b	'[[OK[FOR[[UP[[BUMPER[['
		dc.b	'BUT[DON[T[FORGET[IT[IS'
		dc.b	'[OVER[AFTER[_[LEVELS[['
*- BR --------------------------------------------------------------------*
		dc.b	'OK[[FOR[A[RIGHT[BUMPER'
		dc.b	'BUT[DON[T[FORGET[IT[IS'
		dc.b	'[OVER[AFTER[_[LEVELS[['
*- S2 --------------------------------------------------------------------*
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
		dc.b	'[[[[BIGSIZE[[BALLSf[[['
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
*- S3 --------------------------------------------------------------------*
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
		dc.b	'[[[MAXISIZE[[BALLSf[[['
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
*- RB --------------------------------------------------------------------*
		dc.b	'[[[[[[[[[[OK[[[[[[[[[['
		dc.b	'[[[[[[[FOR[[THE[[[[[[['
		dc.b	'[[[[[[[[ROBOTf[[[[[[[['
*- CT --------------------------------------------------------------------*
		dc.b	'[[[[[[[[[[OK[[[[[[[[[['
		dc.b	'[[[[[[[FOR[[THE[[[[[[['
		dc.b	'[[[[[CONTROLATORf[[[[['
*- GL --------------------------------------------------------------------*
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
		dc.b	'[[[OK[FOR[THE[GLUE[[[['
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['
*- XX --------------------------------------------------------------------*
		dc.b	'[[[[[[GO[AND[SEE[[[[[['
		dc.b	'[[[[[[[[MY[LOVE[[[[[[['
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['


text22:		dc.b	'LEFT[BUMPER[i[[ACTIVE['
		dc.b	'LEFT[BUMPER[iNO[ACTIVE'
		dc.b	'[[BUY[A[LEFT[BUMPER[[['
		dc.b	'UP[BUMPER[[[i[[ACTIVE['
		dc.b	'UP[BUMPER[[[iNO[ACTIVE'
		dc.b	'[[[[BUY[A[UP[BUMPER[[['
		dc.b	'RIGHT[BUMPERi[[ACTIVE['
		dc.b	'RIGHT[BUMPERiNO[ACTIVE'
		dc.b	'[[BUY[A[RIGHT[BUMPER[['

text23:		dc.b	'[[[[[YOU[ONLY[HAVE[[[['	sh.optinfo=1
text70:		dc.b	'[[[[[\\[LIVES[LEFT[[[['
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['

		dc.b	'[THE[PASSWORD[OF[THIS['	sh.optinfo=2
text74:		dc.b	'[[AREA[IS[[[[[[[[[[[[['
text75:		dc.b	'[[DIFFICULTY[IS[[[[[[['

		dc.b	'I[HOPE[THIS[ASSISTANCE'	sh.optinfo=3
		dc.b	'[[[[[WILL[HELP[YOU[[[['
		dc.b	'[[[[[[[[[[[[[[[[[[[[[['

		dc.b	'[[[[[[TLK[[GAMES[[[[[['	si sh.code=1
		dc.b	'[[[[[TECNOBALL[[Z[[[[['
		dc.b	'[[[[[[TLK[[GAMES[[[[[['

		dc.b	'[[LA[CORRUPTION[REVET['	si trainer=ETB
		dc.b	'[[[DES[[DEGUISEMENTS[['
		dc.b	'[[[[[[[[INFINIS[[[[[[['
		EVEN
