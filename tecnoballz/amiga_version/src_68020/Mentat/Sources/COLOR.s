*-------------------------------------------------------------------------*





*-------------------------------------------------------------------------*
		SECTION	ETB,CODE
		OPT	P+		pas de table de relogement
		OPT	C20+		labels et symboles 20 caractères maxi.,  majuscules/minuscules respectées
		OPT	O-		pas d'optimisation
		OPT	p=68020		assemble les instructions MC68020
		INCDIR	DH0:Data/

*>>>>>>>>>>>>>>>>>>>>>>>>>>* CONSTANTES MENTAT *<<<<<<<<<<<<<<<<<<<<<<<<<<*
NReservation	=50	50 adresses mémoires réservées maximums
FLAG_AMIGADOS	=0	1=assemble routines chargement AmigaDOS
FLAG_ARP	=0	1=ouvre la librairie ARP
FLAG_BOB32	=0	1=assemble routines BOBs (aff. écran 32 bits)
FLAG_BOBSTRUCT	=0	1=Assemble la structure BOB
FLAG_DMACON.MSK	=0	1=ferme dma bitplane lors d'un accès disque-système
FLAG_EXIT	=0	0=sortie sous système possible avec la touche "ESC"
FLAG_FREQ	=0	1=assemble routine demande fréquence balayage
FLAG_ILBM	=0	1=assemble routines décompactage IFF-ILBM
FLAG_KEY	=1	1=assemble routines de test du clavier
FLAG_LUTIN	=0	1=assemble routines gestion des lutins
FLAG_MEMORY	=0	1=information sur la mémoire possible
FLAG_MOUSE0	=0	1=assemble la routine déplacement souris port 0
FLAG_MUSIC	=0	1=assemble routines gestion module "ProTracker"
FLAG_PP		=0	1=assemble routines décompactage PowerPacker
FLAG_SOUND	=0	1=assemble routines gestion des bruitages
FLAG_SCROLL	=0	1=assemble routines gestion défilement écran
FLAG_TDISK	=0	1=assemble routines de lecture en "TrackDisk"
FLAG_TDISKW	=0	1=assemble routines d'écriture en "TrackDisk"
FLAG_TRACKDISK	=0	1=assemble programme pour être chargé en "TrackDisk"

*>>>>>>>>>>>>>>>>>>>>>>>>>* Debut code MC68020 *<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	INCLUDE	"Mentat/Include/mentat.s"
msg.pers	dc.b	"                                           "	¤-128
		EVEN

************* routine appelée avant de quitter le multitâche **************
sys_extra:	
		moveq	#0,d7
		rts
sys_extra2:	rts

*>>>>>>>>>>>>>>>>>>>>>>>>>* début du programme *<<<<<<<<<<<<<<<<<<<<<<<<<<*
* Entrée	a5.l	=> adr. 't'
*		a6.l	=> CUSTOM
run:		bsr	first_init
		bsr	sprites_off
		bsr	install_ecran
		tst.l	d7
		bne	sys_erreur
		moveq	#0,d0
		moveq	#0,d1
		lea	(version,pc),a0
		bsr	printf


*¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤* BOUCLE PRINCIPALE HORS INTERRUPTION *¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤*
main_loop:	
		bsr	wait_vbl
		move.l	(atable_clavier-t,a5),a0
		lea	(TOUCHE_F1,a0),a0
		moveq	#9,d1
.loop		tst.b	(a0)+
		bne.s	.go
		dbf	d1,.loop
		bra.s	.exit
.go		mulu	#degas2-degas,d1
		move.w	d1,(px-t,a5)
		bsr	degrade
.exit		btst	#6,$bfe001
		bne.s	main_loop
		bra	sys_fin

******** Routine appelée par l'interruption du temps mort vertical ********
* Entrée	a5.l	=> Adr. 't'
*		a6.l	=> CUSTOM
extra_vbl:
		rts

************************* Installe l'écran de jeu *************************
* Entrée	a5.l	=> adr. table 't'
install_ecran:
		bsr	screen_reserve
		tst.l	d7			dépassement mémoire ?
		bne	dexit			-> oui
		move.w	#3,(SFMODE_PLANS,a0)
		move.w	#3,(SFMODE_LUTINS,a0)
		move.w	#640,(SNPIXEL,a0)
		move.w	#256,(SNLIGNE,a0)
		move.b	#1,(SNPLAN,a0)
		move.w	#1,(SPBUFFER,a0)
		move.w	#640,(SPLARGE,a0)
		move.w	#256,(SPHAUTEUR,a0)
		move.b	#1,(SHIRES,a0)
		move.l	#$000000,(SCOLOR_TABLE,a0)
		move.l	#$CC0201,(4+SCOLOR_TABLE,a0)
		move.w	#20*17+4+27*20+16,(KKCOPPER,a0)		Intructions en +
		bsr	init_screen
		bne	dexit
		move.l	a1,(dg-t,a5)
		bsr	convert_aga_color
		bsr	colors_on

degrade		move.l	(dg-t,a5),a1
		lea	(degas-t,a5),a2		a2 pt/liste couleur 32 bits
		add.w	(px-t,a5),a2
		move.l	(pt_screen-t,a5),a0
		move.w	(SBPLCON3,a0),d6	sélection bits 4 à 7 RVB
		move.w	(SBPLCON3,a0),d7	
		or.w	#$0200,d7		sélection bits 0 à 3 RVB
		move.w	(SCOORDY,a0),d5
		add.w	#100,d5

		move.w	#BPLCON3,(a1)+
		move.w	d6,(a1)+
		move.w	#COLOR00,(a1)+
		move.w	#0,(a1)+
		* bits 0 à 3 *
		move.w	#BPLCON3,(a1)+
		move.w	d7,(a1)+
		move.w	#COLOR00,(a1)+
		move.w	#0,(a1)+

		moveq	#17-1,d4		17 couleurs
		* Wait(x,y) *
.cj1.loop	move.b	d5,(a1)+		coordonnée Y
		move.b	#$01,(a1)+		coordonnée X
		move	#$FFFE,(a1)+
		* RGB 32 bits -> format AGA *
		move.l	(a2)+,d2		d2=$xxRRGGBB
		bsr	aga_color		(sortie d0 & d1)
		* bits 4 à 7 *
		move.w	#BPLCON3,(a1)+
		move.w	d6,(a1)+
		move.w	#COLOR00,(a1)+
		move.w	d0,(a1)+
		* bits 0 à 3 *
		move.w	#BPLCON3,(a1)+
		move.w	d7,(a1)+
		move.w	#COLOR00,(a1)+
		move.w	d1,(a1)+
		addq.w	#8,d5			Y=Y+8
		dbf	d4,.cj1.loop
		move.l	#$FFFFFFFE,(a1)		Fin de la liste Copper
		moveq	#0,d7
dexit		rts
*>>>>>>>>>>>>>>>>>* Adresses utilisées par le programme *<<<<<<<<<<<<<<<<<*
		INCLUDE	"Mentat/Include/Systeme/t.s"
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
version:	dc.b	"$VER: Mentat0.32 (13.02.94)",0
		dc.b	"L'Amiga et son père Jay Miner sont morts...",0
		dc.b	"Tout a une fin !",0
		dc.b	"Vivre et mourir.",0
		dc.b	"Plaisir, luxure, concupiscence, désir...",0,0
		CNOP	0,4

*/ 10 tables dégradé de 17 couleurs des briques */
dg		dc.l	0
px		dc.w	0
gaga		ds.l	17
degas:		dc.l	$400180,$420290,$440392,$500494,$600596,$700698 <1>
		dc.l	$800795,$900893,$A00990,$A20A80,$A40B70,$A60C60
		dc.l	$A80D50,$AA0E40,$AC0F30,$AE1020,$B01110

degas2:		dc.l	$014080,$024590,$034A92,$045094,$056096,$067097 <2>
		dc.l	$078096,$089093,$09A090,$0AA180,$0BA270,$0CA360
		dc.l	$0DA450,$0EA540,$0FA630,$10A720,$11A810

degas3		dc.l	$408001,$459002,$4A9203,$509404,$609605,$709806 <3>
		dc.l	$809507,$909308,$A19009,$A2800A,$A3700B,$A4600C
		dc.l	$A5500D,$A6400E,$A7300F,$A82010,$A91011

degas4		dc.l	$002080,$102190,$202292,$302394,$402496,$502598 <4>
		dc.l	$602695,$702793,$802890,$782980,$702A70,$602B60
		dc.l	$502C50,$402D40,$302E30,$202F20,$102A010

degas5		dc.l	$104080,$204590,$304A92,$405094,$506096,$607098 <5>
		dc.l	$708095,$809093,$90A090,$A0A280,$B0A470,$C0A660
		dc.l	$B0A850,$A0AA40,$90AC30,$80AE20,$70B010

degas6		dc.l	$4080B0,$4590A0,$4A9290,$509480,$609670,$709860 <6>
		dc.l	$809550,$909340,$A09030,$A18020,$A27010,$A36020
		dc.l	$A45030,$A54040,$A63050,$A72060,$A81070

degas7		dc.l	$101080,$181290,$201492,$281694,$301896,$381A98 <7>
		dc.l	$401C95,$451E93,$4A2080,$502270,$582460,$602650
		dc.l	$702840,$782B30,$802E20,$853010,$8A3200

degas8		dc.l	$404080,$484890,$504A91,$605092,$706093,$807094 <8>
		dc.l	$908093,$889092,$80A090,$81A280,$82A670,$83A860
		dc.l	$84AA50,$85AC40,$86AE30,$87B020,$88B210

degas9		dc.l	$458040,$4A9045,$40924A,$509450,$609655,$70985A <9>
		dc.l	$809562,$909364,$A09066,$A1806A,$A2706C,$A36070
		dc.l	$A45073,$A54076,$A6307A,$A72080,$A81085

degas10		dc.l	$407080,$457890,$4A8091,$508892,$609093,$709894 <10>
		dc.l	$80A093,$90A892,$A0B090,$A1B880,$A2C070,$A3B860
		dc.l	$A4B050,$A5A840,$A6A030,$A79820,$A89010


*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*

