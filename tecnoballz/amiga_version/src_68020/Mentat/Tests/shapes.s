;««««««««««««««««««««««««««««««««««««»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»;
;  Le Samedi 25 décembre 1993 all coding by Casimir			  ;
;  GFX-RIPPED (Recupération des graphismes d'Ebonstar)			  ;
;  DH0:shapes = 187226 octets						  ;
;		182.8378906 kilo-octets					  ;
;		93613 mots						  ;
; VERSION DU 12-01-95							  ;
;««««««««««««««««««««««««««««««««««««»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»;
* pour sauvegarder aller au label "lunatic:" et sauter à la routine
* d'affichage correspondante puis sauveagarder entre 
* ladresse : {ecran},{ecran}+C7FF
* Boucle principale ===> "gfx_ripper:"

		INCDIR	DH0:Data/
		OPT	p=68020		assemble les instructions MC68020
		OPT	l-		auto link

*×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×*

SLINE		=40
NPLAN		=5
SPLAN		=SLINE*NPLAN
NLINE		=256
ECRAN_SIZE	=SPLAN*NLINE
T_MAXI		=24
T_MAXY		=16
CUSTOM		=$DFF000
BEAMCON0	=$1DC
COLOR00		=$180
BPLCON3		=$106
TROU_WIDTH	=14
TROU_HIGH	=105
TROU_HIGH2	=53
TROU_HIGH3	=53		page 8 ½ trous
TROU_HIGH4	=53		page 8 ½ trous
TROU_HIGH5	=25		69 bosses
TROU_HIGH7	=105		trou color
PTROU_WIDTH	=336		page 24 trous
P2TROU_WIDTH	=224		page 16 ½ trous hauts
P3TROU_WIDTH	=140		page 8 ½ trous hauts (custom)
P4TROU_WIDTH	=140		page 8 ½ trous hauts (custom)
P5TROU_WIDTH	=276		69 bosses
P7TROU_WIDTH	=14
SHAPES_SIZE	=187226

OFFSET1		=000000			24 images du trou entier (48 vu qu'on les retourne)
OFFSET2		=070560			16 ½ images du trou (64 positions en tout avec les 24 d'avant)
OFFSET3		=094304			8 ½ images apparition du trou
OFFSET4		=109144			8 ½ images disparition du trou
OFFSET5		=123984			69 images de la bosse
OFFSET7		=137784			Masque couleur du trou
OFFSET8		=142194			GROS TAILLE 1
OFFSET9		=147370			GROS TAILLE 2
OFFSET10	=148594			GROS TAILLE 3
OFFSET11	=149026			???
OFFSET12	=149628			TIR DU GROS TAILLE 1
OFFSET13	=151940			TIR DU GROS TAILLE 2
OFFSET14	=152876			TIR DU GROS TAILLE 3
OFFSET15	=153258			TIR DU VAISSEAU TAILLE 1
OFFSET16	=154786			TIR DU VAISSEAU TAILLE 2
OFFSET17	=155488			TIR DU VAISSEAU TAILLE 3???????
OFFSET18	=155626			VAISSEAU TAILLE 1 5 plans
OFFSET19	=158176			VAISSEAU TAILLE 2 5 plans
OFFSET20	=159372			VAISSEAU TAILLE 3 5 plans
OFFSET21	=159822+20		VAISSEAU TAILLE 1 6 plans
OFFSET22	=162902 		VAISSEAU TAILLE 2 6 plans
OFFSET23	=164332			VAISSEAU TAILLE 3 5? plans
OFFSET24	=164806			Boule Taille 1
OFFSET25	=164884			Boule Taille 2
OFFSET26	=164950			Boule Taille 3

OFFSET27	=165000
OFFSETAA	=175706
OFFSETGB	=173670
OFFSET_0	=172874
OFFSETE1	=181236
OFFSET_EBONSTAR	=183040
OFFSETK1	=184714
OFFSETK2	=184770
OFFSETK3	=184842
OFFSETK4	=184930
OFFSETK5	=185034
OFFSETK6	=185154


*««««««««««««««««««««««««««««««««««««»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
		SECTION ETB,code
debut:		bsr	save
		lea	(fzet,pc),a0
		move.l	#OFFSET27,(a0)
		lea	(tlk,pc),a5
		lea	(fonteraw,pc),a0
		move.l	a0,(adr_fonteraw-tlk,a5)
		lea	(ecran,pc),a0
		tst.l	(a0)
		beq.s	.exit
		lea	(fichier,pc),a0
		tst.l	(a0)
		beq.s	.exit
		lea	(handle,pc),a0
		tst.l	(a0)
		beq.s	.exit
		bsr	ecscolor
		lea	(xfichier,pc),a0
		move.l	(fichier,pc),(a0)
		move.w	#$8760,($0096,a6)
		move.w	#$C028,($009A,a6)
		move.w	#$000f,($01FC,a6)
		move.w	#$5201,($0100,a6)
		move.w	#$0000,($0102,a6)
		move.w	#$0024,($0104,a6)
		move.w	(sbplcon3,pc),(BPLCON3,a6)
		move.w	#SPLAN-SLINE,d0
		move.w	d0,($0108,a6)
		move.w	d0,($010A,a6)
		move.w	#$2881,($008E,a6)
		move.w	#$28c1,($0090,a6)
		move.w	#$0038,($0092,a6)
		move.w	#$00b8,($0094,a6)	* ECRAN 320 : (start=$38,stop=$9A à $B8, )
*/////////////////////////////////////////////////////////////////////////*
.lunatic:	bsr	gfx_ripper
		*bsr	 affiche24
		*bsr	affiche16
		*bsr	affiche_apparition
		*bsr	affiche_disparition
		*bsr	trou_color
		*bsr	gros
		*bsr	boby
		*bsr	text
*/////////////////////////////////////////////////////////////////////////*
		move.w	#$0020,(BEAMCON0,a6)
.exit		bsr	restore
		moveq	#0,d0
		rts
*-------------------------------------------------------------------------*
*-------------------------------------------------------------------------*
*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*
		* ATTEND QUE LE BLITTER EST TERMINE *
wait_blitter:	lea	CUSTOM,a6
		btst	#14,(2,a6)
		bne.s	wait_blitter
.wait_blitter:	btst	#14,(2,a6)
		bne.s	.wait_blitter
		rts
*-------------------------------------------------------------------------*
save:		move.l	4.w,a6
		move.l	#SHAPES_SIZE*2,d0
		move.l	#$10002,d1
		jsr	(-198,a6)		|-> Réserve mémoire fichier
		lea	(fichier,pc),a0
		move.l	d0,(a0)
		lea	(dosname,pc),a1
		moveq	#39,d0
		jsr	(-552,a6)		|-> Ouvre la librarie DOS
		lea	(dosbase,pc),a0
		move.l	d0,(a0)
		bsr	loadfile
		jsr	(-132,a6)		|-> Forbid
		lea	(gfxname,pc),a1
		moveq	#39,d0
		jsr	(-552,a6)		|-> Ouvre la graphique librairie
		lea	(gfxbase,pc),a0
		move.l	d0,(a0)
		move.l	#ECRAN_SIZE,d0
		move.l	#$10002,d1
		jsr	(-198,a6)		|-> Réserve l'écran
		lea	(ecran,pc),a0
		move.l	d0,(a0)
		lea	$dff000,a6
		lea	(save_intena,pc),a0
		move.w	($001C,a6),(a0)
		or.w	#$c000,(a0)+
		move.w	($0002,a6),(a0)
		or.w	#$8000,(a0)
		move.w	#$7fff,($009A,a6)
		move.w	#$7fff,($009C,a6)
		move.w	#$07ff,($0096,a6)

		lea	$68.w,a1
		lea	(vector68,pc),a0
		move.l	(a1),(a0)
		lea	(interruption_clavier,pc),a0
		move.l	a0,(a1)

		lea	$6C.w,a1
		lea	(vector6C,pc),a0
		move.l	(a1),(a0)
		lea	(vbl,pc),a0
		move.l	a0,(a1)
		rts

*-------------------------------------------------------------------------*
restore:	move.w	#$7fff,($009A,a6)
		move.w	#$7fff,($009C,a6)
		move.w	#$07ff,($0096,a6)
		lea	$68.w,a1
		move.l	(vector68,pc),(a1)
		lea	$6C.w,a1
		move.l	(vector6C,pc),(a1)
		move.l	(gfxbase,pc),a1
		move.l	(38,a1),($0080,a6)
		move.w	d0,($0088,a6)
		move.w	(save_dmacon,pc),($0096,a6)
		move.w	(save_intena,pc),($009A,a6)
		move.l	4.w,a6
		move.l	(gfxbase,pc),a1
		jsr	(-414,a6)
		jsr	(-138,a6)

		* libère mémoire *
li:		lea	(fichier,pc),a0
		tst.l	(a0)			fichier réservé ?
		beq.s	.ok			-> non
		move.l	(a0),a1			a1=adr. écran
		clr.l	(a0)
		move.l	#SHAPES_SIZE*2,d0	d0=taille fichier
		jsr	(-210,a6)		|-> libère mémoire
		lea	(ecran,pc),a0
		tst.l	(a0)			écran réservé ?
		beq.s	.ok			-> non
		move.l	(a0),a1			a1=adr. écran
		clr.l	(a0)
		move.l	#ECRAN_SIZE,d0		d0=taille écran
		jsr	(-210,a6)		|-> libère mémoire
.ok		moveq	#0,d0
		rts
*=========================================================================*
loadfile:
		move.l	a6,-(a7)
		lea	(fichier,pc),a0
		tst.l	(a0)
		beq.s	.exit
		lea	(dosbase,pc),a0
		tst.l	(a0)
		beq.s	.exit

		* OPEN FILE *
		lea	(fichiername,pc),a0
 		move.l	a0,d1
		move.l	#1005,d2		1005 = "Mode_old"
		move.l	(dosbase,pc),a6
		jsr	(-30,a6)
		lea	(handle,pc),a0
		move.l	d0,(a0)			sauve valeur du handle
		beq.s	.exit

		* READ FILE *
		moveq	#0,d1
		move.l	(fichier,pc),d2		d2 = adr. chargement
		move.l	(handle,pc),d1
		move.l	#SHAPES_SIZE,d3		d3 = nombre d'octet à charger
		jsr	(-42,a6)

		* CLOSE FILE *
		move.l	(handle,pc),d1
		jsr	(-36,a6)

.exit		move.l	(a7)+,a6
		rts

*=========================================================================*
		* MET 32 COULEURS ECS *
ecscolor:	move	(sbplcon3,pc),(BPLCON3,a6)
		lea	(COLOR00,a6),a0
		lea	(.tableecs,pc),a1
		moveq	#16-1,d0
.loop		move.l	(a1)+,(a0)+
		dbf	d0,.loop
		rts
.tableecs:	dc.w	$000,$00a,$8ff,$00f	00,01,02,03
		dc.w	$440,$008,$7ee,$00d	04,05,06,07
		dc.w	$660,$608,$6dd,$60b	08,09,10,11
		dc.w	$880,$a00,$5cc,$c00	12,13,14,15
		dc.w	$aa0,$d00,$fff,$f00	16,17,18,19
		dc.w	$bb0,$fef,$fcf,$f9f	20,21,22,23
		dc.w	$888,$066,$077,$088	24,25,26,27
		dc.w	$099,$0aa,$0dd,$0ff	28,29,30,31

*.tableecs:	dc.w	$000,$00a,$0ff,$00f	00,01,02,03
		dc.w	$000,$008,$0ff,$00d	04,05,06,07
		dc.w	$000,$608,$0ff,$60b	08,09,10,11
		dc.w	$000,$a00,$0ff,$c00	12,13,14,15
		dc.w	$000,$d00,$fff,$f00	16,17,18,19
		dc.w	$aaa,$0ff,$0ff,$0ff	20,21,22,23
		dc.w	$888,$066,$077,$088	24,25,26,27
		dc.w	$099,$0aa,$0dd,$0ff	28,29,30,31

* Couleur 00,04,08,12,16 sont noires
* Couleur 01,05,09,13,17 couleur du trou (traits longs)
* Couleur 03,07,11,15,19 couleur du trou (intersection traits)
* Couleur 18,20,24	 couleur gros & tir (jaune, orange, orange) 

*=========================================================================*

x_synchro:	lea	(vbl.flag,pc),a0
.wait		tst.w	(a0)
		beq.s	.wait	
		clr.w	(a0)
		rts

*=========================================================================*
x_key:		lea	(table_clavier,pc),a0
		lea	(fzet,pc),a2
		lea	(finc,pc),a3
		move.l	(a3),d2
		move.l	d2,d3			taille d'une ligne
		mulu	#NLINE,d3		taille d'une page écran

		tst.b	($66,a0)
		bne.s	amiga
		tst.b	($67,a0)
		bne.s	amiga
		tst.b	($64,a0)
		bne	alt
		tst.b	($65,a0)
		bne	alt
		tst.b	($60,a0)
		bne	shift
		tst.b	($61,a0)
		bne	shift
		tst.b	($60,a0)
		beq	no_shift

		*<*<* "AVEC AMIGA" *>*>*
amiga:		lea	($4C,a0),a1
		bsr	test_clavier
		beq.s	.no_haut
		move.l	d3,d0
		asl.l	#3,d0
		sub.l	d0,(a2)
.no_haut	lea	($4D,a0),a1
		bsr	test_clavier
		beq.s	.no_bas
		move.l	d3,d0
		asl.l	#3,d0
		add.l	d0,(a2)
.no_bas
		bra	x_key_limit

		*<*<* "AVEC ALT" *>*>*
alt:		lea	($4C,a0),a1
		bsr	test_clavier
		beq.s	.no_haut
		move.l	d2,d0
		asl.l	#3,d0
		sub.l	d0,(a2)
.no_haut	lea	($4D,a0),a1
		bsr	test_clavier
		beq.s	.no_bas
		move.l	d2,d0
		asl.l	#3,d0
		add.l	d0,(a2)
.no_bas
		bra	x_key_limit

		*<*<* "AVEC SHIFT" *>*>*
shift:		lea	($4C,a0),a1
		bsr	test_clavier
		beq.s	.no_haut
		sub.l	d3,(a2)
.no_haut	lea	($4D,a0),a1
		bsr	test_clavier
		beq.s	.no_bas
		add.l	d3,(a2)
.no_bas		lea	($4F,a0),a1
		bsr	test_clavier
		beq.s	.no_gauche
		move.l	(fhaut,pc),d0
		mulu	(finc+2,pc),d0
		mulu	(fplan+2,pc),d0
		sub.l	d0,(a2)
.no_gauche	lea	($4E,a0),a1
		bsr	test_clavier
		beq.s	.no_droite
		move.l	(fhaut,pc),d0
		mulu	(finc+2,pc),d0
		mulu	(fplan+2,pc),d0
		add.l	d0,(a2)
.no_droite	
		bra	x_key_limit

		*>*>* "SANS RIEN" *<*<*
no_shift	lea	($46,a0),a1
		bsr	test_clavier
		beq.s	.no_del
		clr.l	(a2)
.no_del		lea	($5f,a0),a1
		bsr	test_clavier
		beq.s	.no_help
		move.l	#SHAPES_SIZE,(a2)
.no_help	lea	($4C,a0),a1
		bsr	test_clavier
		beq.s	.no_haut
		sub.l	d2,(a2)
.no_haut	lea	($4D,a0),a1
		bsr	test_clavier
		beq.s	.no_bas
		add.l	d2,(a2)
.no_bas		
		lea	($2F,a0),a1
		bsr	test_clavier
		beq.s	.no_droite6
		addq.l	#2,(a3)
.no_droite6	lea	($2D,a0),a1
		bsr	test_clavier
		beq.s	.no_gauche4
		move.l	#1,(ffff-tlk,a5)
		subq.l	#2,(a3)
.no_gauche4	lea	($4F,a0),a1
		bsr	test_clavier
		beq.s	.no_gauche
		subq.l	#2,(a2)
.no_gauche	lea	($4E,a0),a1
		bsr	test_clavier
		beq.s	.no_droite
		addq.l	#2,(a2)
.no_droite	lea	($3E,a0),a1
		bsr	test_clavier
		beq.s	.no_haut8
		lea	(fhaut,pc),a4
		move.l	#1,(ffff-tlk,a5)
		subq.l	#1,(a4)
.no_haut8	lea	($1E,a0),a1
		bsr	test_clavier
		beq.s	.no_bas2
		lea	(fhaut,pc),a4
		move.l	#1,(ffff-tlk,a5)
		addq.l	#1,(a4)
.no_bas2	lea	($4A,a0),a1
		bsr	test_clavier
		beq.s	.no_moins
		move.l	#1,(ffff-tlk,a5)
		lea	(fplan,pc),a4
		subq.l	#1,(a4)
.no_moins	lea	($5E,a0),a1
		bsr	test_clavier
		beq.s	.no_plus
		lea	(fplan,pc),a4
		addq.l	#1,(a4)
.no_plus	lea	($5A,a0),a1
		bsr	test_clavier
		beq.s	.no_crochet_m
		lea	(fstart,pc),a4
		subq.l	#1,(a4)
		move.l	#1,(ffff-tlk,a5)
.no_crochet_m	lea	($5B,a0),a1
		bsr	test_clavier
		beq.s	.no_crochet_p
		lea	(fstart,pc),a4
		addq.l	#1,(a4)
		move.l	#1,(ffff-tlk,a5)
.no_crochet_p	lea	($0F,a0),a1		[-] table offset {0 num} 
		bsr	test_clavier
		beq.s	.nonum0
		lea	(fzx,pc),a4
		subq.l	#1,(a4)
.nonum0		lea	($3C,a0),a1		[+] table offset {. num}
		bsr	test_clavier
		beq.s	.nonumpoint
		lea	(fzx,pc),a4
		addq.l	#1,(a4)
.nonumpoint
		*>*>*> Test dépassements *<*<*<
x_key_limit	tst.l	(a3)
		beq.s	.zozo
		bpl.s	.positif
.zozo		move.l	#2,(a3)
.positif	cmp.l	#32766,(a3)
		blt.s	.wildcopper
		move.l	#32766,(a3)
.wildcopper	tst.l	(a2)
		bpl.s	.pos
		clr.l	(a2)
.pos		move.l	#SHAPES_SIZE,d3
		cmp.l	(a2),d3
		bhi.s	.ok
		move.l	d3,(a2)
.ok		lea	(fhaut,pc),a4		*/ Limites de la hauteur /*
		tst.l	(a4)
		beq.s	.capitaine
		bpl.s	.hadock
.capitaine	move.l	#1,(a4)
.hadock		cmp.l	#150,(a4)
		blt.s	.boit_de_l_eau
		move.l	#150,(a4)
.boit_de_l_eau	lea	(fplan,pc),a4		*/ Limites du nombre de plan/*
		tst.l	(a4)
		beq.s	.kapitaine
		bpl.s	.zadock
.kapitaine	move.l	#1,(a4)
.zadock		cmp.l	#5,(a4)
		blt.s	.boit_du_vin
		move.l	#5,(a4)
.boit_du_vin	lea	(fstart,pc),a4		*/ Limites du 1er plan */
		tst.l	(a4)
		beq.s	.delight
		bpl.s	.maxximum
.delight	move.l	#1,(a4)
.maxximum	cmp.l	#5,(a4)
		blt.s	.k_zi_mir
		move.l	#5,(a4)
.k_zi_mir	lea	(fzx,pc),a4		*/ limite de la table FZX /*
		move.l	#(fzx3-fzx1)/(fzx2-fzx1)-1,d0
		tst.l	(a4)
		bpl.s	.posivv
		move.l	d0,(a4)
.posivv		cmp.l	(a4),d0
		bge.s	.okvv
		clr.l	(a4)
.okvv		
		bsr	read_fzx
		rts


**** LIT VALEURS DE L'OFFSET SELECTIONNES ***
read_fzx:
		move.l	(fzx,pc),d0
		cmp.l	(fzx.old-tlk,a5),d0
		beq.s	.ok
		move.l	d0,(fzx.old-tlk,a5)
		mulu	#fzx2-fzx1,d0
		lea	(fzx1,pc),a1
		add.l	d0,a1
		lea	(fzet,pc),a4
		moveq	#5-1,d0
.loop		move.l	(a1)+,(a4)+
		dbf	d0,.loop
		move.l	#1,(ffff-tlk,a5)
.ok		rts


;**************** Test si une touche est prise en compte ******************
; Entrée	a1.l	pt/key flag
; sortie	d0.l	0 = non pris en compte/ 1 = pris en compte
TC1		=25				key repeat delay
TC2		=TC1+2				key repeat speed
test_clavier:	
		tst.b	(a1)			touche préssée ?
		beq.s	.no_flag		-> non
		moveq	#1,d0			d0=1=touche prise en compte
		cmp.b	(a1),d0			1ere pression ?
		beq.s	.ok_press		-> oui, pris en compte
		cmp.b	#TC1,(a1)		infer. delai de répétition ?
		blt.s	.no_press		-> oui, pas pris en compte
		beq.s	.ok_press		-> égal, pris en compte
		cmp.b	#TC2,(a1)		fin compteur de répétion ?
		bne.s	.no_press		-> non
		move.b	#TC1-1,(a1)
.no_press	add.b	d0,(a1)			inc. compteur
.no_flag	moveq	#0,d0			d0=0=touche non prise en compte
		rts				<-| sort routine
.ok_press	add.b	d0,(a1)			inc. compteur
		rts				<-| sort routine

*/////////////////////////////////////////////////////////////////////////*
*/////////////////////////////////////////////////////////////////////////*
*/////////////////////////////////////////////////////////////////////////*

*=========================================================================*
* RIPPER DE GRAPHISMES AVEC GESTION DU XORYGLU (all coding by Yoplait)*
gfx_ripper:
.loop
		bsr	x_synchro
		tst.l	(ffff-tlk,a5)		efface écran ?
		beq.s	.rhum_des_foins		-> non
		clr.l	(ffff-tlk,a5)
		bsr	clear_screen
.rhum_des_foins	bsr	affiche_offset
		bsr	affiche_large
		bsr	affiche_haut
		bsr	affiche_plan
		bsr	affiche_start
		bsr	affiche_offx

		* calcul modulo source *
		bsr	wait_blitter
		move.l	(finc,pc),d0
		sub.l	#T_MAXI,d0
		bpl.s	.moda
		clr.w	d0
.moda		move	d0,($64,a6)

		* calcul modulo destination *
		move.l	(finc,pc),d1
		move.w	#SPLAN-T_MAXI,d0
		cmp.l	#T_MAXI,d1
		bhi.s	.modd
		move.l	#SPLAN,d0
		sub.l	d1,d0
.modd		move.w	d0,($66,a6)

		* calcul fênêtre Blitter * 
		move.l	(finc,pc),d0
		cmp	#T_MAXI,d0
		blt.s	.window
		move	#T_MAXI,d0
.window		lsr.w	#1,d0
		add.w	#NLINE*64,d0
		move.l	#-1,($44,a6)
		move.l	#$09f00000,($40,a6)
		move.l	(fichier,pc),a0
		move.l	(ecran,pc),a1
		lea	(SLINE,a1),a1
		add.l	(fzet,pc),a0

		*> BLITTER AFFICHAGE <*
		bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move	d0,($58,a6)
		*bra	.endy

		*----------------------------*

		* calcul modulo source *
		bsr	wait_blitter
		move.l	(finc,pc),d0
		sub.l	#T_MAXY,d0
		bpl.s	.modaxx
		clr.w	d0
.modaxx		move	d0,($64,a6)

		* calcul modulo destination *
		move.l	(finc,pc),d1
		move.w	#SPLAN-T_MAXY,d0
		cmp.l	#T_MAXY,d1
		bhi.s	.moddxx
		move.l	#SPLAN,d0
		sub.l	d1,d0
.moddxx		move.w	d0,($66,a6)

		* calcul fênêtre Blitter * 
		move.l	(finc,pc),d0
		cmp	#T_MAXY,d0
		blt.s	.windowxx
		move	#T_MAXY,d0
.windowxx	lsr.w	#1,d0
		move.l	(fhaut,pc),d1
		lsl.w	#6,d1
		add.w	d1,d0			d0=FB

		move.l	(fichier,pc),a0
		add.l	(fzet,pc),a0
		move.l	(ecran,pc),a1
		lea	(T_MAXI,a1),a1
		move.l	(fstart,pc),d1
		subq.w	#1,d1
		mulu	#SLINE,d1
		add.l	d1,a1
		move.l	#SLINE,d3
		move.l	(finc,pc),d2
		move.l	(fhaut,pc),d1
		mulu	d1,d2

		* affichage *
		move.l	(fplan,pc),d7
		subq.w	#1,d7
.loopaff	bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move	d0,($58,a6)
		add.l	d2,a0
		add.l	d3,a1
		dbf	d7,.loopaff

.endy		bsr.s	change_affichage
		btst	#6,$bfe001
		bne	.loop
		rts

*/////////////////////////////////////////////////////////////////////////*
*/////////////////////////////////////////////////////////////////////////*
*/////////////////////////////////////////////////////////////////////////*
	
		*/*/ SAUTE A UNE ROUTINE D'AFFICHAGE */*/
change_affichage:
		lea	(tempo,pc),a0
		move.w	#1,(a0)+
		move.w	#1,(a0)+
		clr.w	(a0)
		lea	(table_clavier,pc),a0
		move.w	(-4,a0),d0		d0=code raw
		cmp.w	#$50,d0
		blt	.no_function
		cmp.w	#$59,d0
		bhi	.no_function
		sub.w	#$50,d0
		bsr	CLS
		move.l	(.table,pc,d0*4),a0
		add.l	a5,a0
		jsr	(a0)
.press		btst	#6,$bfe001
		beq.s	.press
		move.l	#1,(ffff-tlk,a5)
.no_function	rts
		CNOP	0,4
.table		dc.l	affiche24-tlk			F1
		dc.l	affiche16-tlk			F2
		dc.l	affiche_apparition-tlk		F3
		dc.l	affiche_disparition-tlk		F4
		dc.l	trou_color-tlk			F5
		dc.l	gros-tlk			F6
		dc.l	boby-tlk			F7
		dc.l	text-tlk			F8
		dc.l	text-tlk			F9
		dc.l	text-tlk			F10

*=========================================================================*
		* AFFICHE LES 24 TROUS *
affiche24:
.loop		bsr	x_synchro
		move	#23,d1
		move	#TROU_WIDTH,d2
		move.l	(fichier,pc),d3
		bsr	x_calcul
		*bsr	.clear_trou
		bsr	.affiche_trou
		btst	#6,$bfe001
		bne	.loop
		rts
.clear_trou:
		bsr	wait_blitter
		move.l	#$01000000,($40,a6)
		move.l	(ecran,pc),($54,a6)
		move	#SLINE-TROU_WIDTH,($66,a6)
		move	#TROU_HIGH*64*NPLAN+(TROU_WIDTH/2),($58,a6)
		rts
.affiche_trou:	
		move.l	(xfichier,pc),a0
		bsr	wait_blitter
		move.l	#-1,($44,a6)
		move.l	#$09f00000,($40,a6)
		move	#PTROU_WIDTH-TROU_WIDTH,($64,a6)
		move	#SPLAN-TROU_WIDTH,($66,a6)
		move.l	(ecran,pc),a1
		move.l	#PTROU_WIDTH*TROU_HIGH,d2
		move.l	#SLINE,d3	
		move	#TROU_HIGH*64+(TROU_WIDTH/2),d0
		bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move	d0,($58,a6)		PLAN 1
		add.l	d2,a0
		add.l	d3,a1
		bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move	d0,($58,a6)		PLAN 2


duran:		
		move.l	#TROU_HIGH7*TROU_WIDTH,d2
		move.l	(fichier,pc),a0
		add.l	#OFFSET7,a0
		move.l	(ecran,pc),a1
		add.l	#SLINE*2,a1
		bsr	wait_blitter
		move.w	#0,($64,a6)
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move	d0,($58,a6)		PLAN 3
		add.l	d2,a0
		add.l	d3,a1
		bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move	d0,($58,a6)		PLAN 4
		add.l	d2,a0
		add.l	d3,a1
		bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move	d0,($58,a6)		PLAN 5
		add.l	d2,a0
		add.l	d3,a1
		rts



*=========================================================================*
		* AFFICHE LES 16 TROUS *
affiche16:
.loop		bsr	x_synchro
		move	#15,d1
		move	#TROU_WIDTH,d2
		move.l	(fichier,pc),d3
		add.l	#OFFSET2,d3
		bsr	x_calcul
		bsr	.clear_trou
		bsr	.affiche_trou
		btst	#6,$bfe001
		bne	.loop
		rts
.clear_trou:	bsr	wait_blitter
		move.l	#$01000000,($40,a6)
		move.l	(ecran,pc),($54,a6)
		move	#SLINE-TROU_WIDTH,($66,a6)
		move	#TROU_HIGH2*64*NPLAN+(TROU_WIDTH/2),($58,a6)
		rts
.affiche_trou:	move.l	(xfichier,pc),a0
		bsr	wait_blitter
		move.l	#-1,($44,a6)
		move.l	#$09f00000,($40,a6)
		move	#P2TROU_WIDTH-TROU_WIDTH,($64,a6)
		move	#SPLAN-TROU_WIDTH,($66,a6)
		move.l	(ecran,pc),a1
		move.l	#P2TROU_WIDTH*TROU_HIGH2,d2
		move.l	#SLINE,d3	
		move	#TROU_HIGH2*64+(TROU_WIDTH/2),d0
		bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move	d0,($58,a6)		PLAN 1
		add.l	d2,a0
		add.l	d3,a1
		bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move	d0,($58,a6)		PLAN 2
		rts
*=========================================================================*
		* AFFICHE APPARITION DU TROU *
affiche_apparition:
.loop		bsr	x_synchro
		move.w	#9,d1
		move.w	#TROU_WIDTH,d2
		move.l	(fichier,pc),d3
		add.l	#OFFSET3,d3
		bsr	x_calcul
		bsr	.clear_trou
		bsr	.affiche_trou
		btst	#6,$bfe001
		bne	.loop
		rts
.clear_trou:	bsr	wait_blitter
		move.l	#$01000000,($40,a6)
		move.l	(ecran,pc),($54,a6)
		move	#SLINE-TROU_WIDTH,($66,a6)
		move	#TROU_HIGH3*64*NPLAN+(TROU_WIDTH/2),($58,a6)
		rts
.affiche_trou:	move.l	(xfichier,pc),a0
		bsr	wait_blitter
		move.l	#-1,($44,a6)
		move.l	#$09f00000,($40,a6)
		move	#P3TROU_WIDTH-TROU_WIDTH,($64,a6)
		move	#SPLAN-TROU_WIDTH,($66,a6)
		move.l	(ecran,pc),a1
		move.l	#P3TROU_WIDTH*TROU_HIGH3,d2
		move.l	#SLINE,d3	
		move	#TROU_HIGH3*64+(TROU_WIDTH/2),d0
		bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move	d0,($58,a6)		PLAN 1
		add.l	d2,a0
		add.l	d3,a1
		bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move	d0,($58,a6)		PLAN 2
		rts
*=========================================================================*
		* AFFICHE DISPARITION DU TROU *
affiche_disparition:
.loop		bsr	x_synchro
		move	#9,d1
		move	#TROU_WIDTH,d2
		move.l	(fichier,pc),d3
		add.l	#OFFSET4,d3
		bsr	x_calcul
		bsr	.clear_trou
		bsr	.affiche_trou
		btst	#6,$bfe001
		bne	.loop
		rts
.clear_trou:	bsr	wait_blitter
		move.l	#$01000000,($40,a6)
		move.l	(ecran,pc),($54,a6)
		move.w	#SLINE-TROU_WIDTH,($66,a6)
		move.w	#TROU_HIGH4*64*NPLAN+(TROU_WIDTH/2),($58,a6)
		rts
.affiche_trou:	move.l	(xfichier,pc),a0
		bsr	wait_blitter
		move.l	#-1,($44,a6)
		move.l	#$09f00000,($40,a6)
		move	#P4TROU_WIDTH-TROU_WIDTH,($64,a6)
		move	#SPLAN-TROU_WIDTH,($66,a6)
		move.l	(ecran,pc),a1
		move.l	#P4TROU_WIDTH*TROU_HIGH4,d2
		move.l	#SLINE,d3	
		move.w	#TROU_HIGH4*64+(TROU_WIDTH/2),d0
		bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move.w	d0,($58,a6)		PLAN 1
		add.l	d2,a0
		add.l	d3,a1
		bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move.w	d0,($58,a6)		PLAN 2
		rts

*=========================================================================*
trou_color:
.loop		bsr	x_synchro
		move.l	(fichier,pc),a0
		add.l	#OFFSET7,a0
		bsr	wait_blitter
		move.l	#-1,($44,a6)
		move.l	#$09f00000,($40,a6)
		move.w	#0,($64,a6)
		move.w	#SPLAN-TROU_WIDTH,($66,a6)
		move.l	(ecran,pc),a1
		move.l	#TROU_WIDTH*TROU_HIGH7,d2
		move.l	#SLINE,d3	
		move.w	#TROU_HIGH7*64+(TROU_WIDTH/2),d0
		bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move	d0,($58,a6)		PLAN 1
		add.l	d2,a0
		add.l	d3,a1
		bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move.w	d0,($58,a6)		PLAN 2
		add.l	d2,a0
		add.l	d3,a1
		bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move.w	d0,($58,a6)		PLAN 3
		btst	#6,$bfe001
		bne	.loop
		rts
*=========================================================================*
gros:
.loop		bsr	x_synchro

		*-----------------------------*
		move.l	(fichier,pc),a0		17 GROS TAILLE 1
		add.l	#OFFSET8,a0
		move.l	(ecran,pc),a1
		bsr	wait_blitter
		move.l	#$09f00000,($40,a6)
		move.l	#$ffff0000,($44,a6)
		move.w	#4-4,($64,a6)
		move.w	#SPLAN-4,($66,a6)
		move.w	#19*64+2,d0
		move.l	#19*4,d6
		move.l	#19*4*4,d7
		moveq	#10-1,d2
		bsr	blit32
		move.l	(ecran,pc),a1
		add.l	#SPLAN*19,a1
		moveq	#7-1,d2
		bsr	blit32

		move.l	(fichier,pc),a0		GROS TAILLE 2
		add.l	#OFFSET9,a0
		move.l	(ecran,pc),a1
		add.l	#SPLAN*19*2,a1
		bsr	wait_blitter
		move.l	#-1,($44,a6)
		move.w	#2-2,($64,a6)
		move.w	#SPLAN-2,($66,a6)
		move.l	#17*2,d6
		move.l	#17*2*4,d7
		move.w	#14*64+1,d0
		moveq	#9-1,d2
		bsr	blit
	
		*/*/ APPARITION DU GROS /*/*
		move.l	(ecran,pc),a1
		add.l	#SPLAN*19*2+20,a1
		lea	(SLINE*1,a1),a1		commence plan 2
		move.l	(fichier,pc),a0		Apparition gros taille 6
		add.l	#OFFSETK1,a0
		move.l	#7*2,d6
		move.l	#7*2*4,d7
		move.w	#4*64+1,d0
		bsr	blit_one
		move.l	(fichier,pc),a0		Apparition gros taille 5
		add.l	#OFFSETK2,a0
		move.l	#9*2,d6
		move.l	#9*2*4,d7
		move.w	#6*64+1,d0
		bsr	blit_one
		move.l	(fichier,pc),a0		Apparition gros taille 4
		add.l	#OFFSETK3,a0
		move.l	#11*2,d6
		move.l	#11*2*4,d7
		move.w	#08*64+1,d0
		bsr	blit_one
		move.l	(fichier,pc),a0		Apparition gros taille 3
		add.l	#OFFSETK4,a0
		move.l	#13*2,d6
		move.l	#13*2*4,d7
		move	#10*64+1,d0
		bsr	blit_one
		move.l	(fichier,pc),a0		Apparition gros taille 2
		add.l	#OFFSETK5,a0
		move.l	#15*2,d6
		move.l	#15*2*4,d7
		move.w	#12*64+1,d0
		bsr	blit_one
		move.l	(fichier,pc),a0		Apparition gros taille 1
		add.l	#OFFSETK6,a0
		move.l	#17*2,d6
		move.l	#17*2*4,d7
		move	#14*64+1,d0
		bsr	blit_one

		move.l	(fichier,pc),a0		GROS TAILLE 3
		add.l	#OFFSET10,a0
		move.l	(ecran,pc),a1
		add.l	#SPLAN*(19*2+17),a1
		bsr	wait_blitter
		move.l	#11*2,d6
		move.l	#11*2*4,d7
		move.w	#08*64+1,d0
		moveq	#5-1,d2
		bsr	blit


		*-----------------------------*
		move.l	(fichier,pc),a0		TIR DU GROS TAILLE 1
		add.l	#OFFSET12,a0
		move.l	(ecran,pc),a1
		add.l	#SPLAN*(19*2+17+11),a1
		bsr	wait_blitter
		move.l	#17*2,d6
		move.l	#17*2*4,d7
		move.w	#14*64+1,d0
		moveq	#17-1,d2
		bsr	blit

		move.l	(fichier,pc),a0		TIR DU GROS TAILLE 2
		add.l	#OFFSET13,a0
		move.l	(ecran,pc),a1
		add.l	#SPLAN*(19*2+17+11+17),a1
		bsr	wait_blitter
		move.l	#13*2,d6
		move.l	#13*2*4,d7
		move.w	#10*64+1,d0
		moveq	#9-1,d2
		bsr	blit

		move.l	(fichier,pc),a0		TIR DU GROS TAILLE 3
		add.l	#OFFSET14,a0
		move.l	(ecran,pc),a1
		add.l	#SPLAN*(19*2+17+11+17+13),a1
		bsr	wait_blitter
		move.l	#9*2,d6
		move.l	#9*2*4,d7
		move.w	#6*64+1,d0
		moveq	#5-1,d2
		bsr	blit

		*-----------------------------*
		move.l	(fichier,pc),a0		TIR VAISSEAU TAILLE 1
		add.l	#OFFSET15,a0
		move.l	(ecran,pc),a1
		add.l	#SPLAN*(19*2+17+11+17+13+9),a1
		bsr	wait_blitter
		move.l	#15*2,d6
		move.l	#15*2*3,d7
		move.w	#15*64+1,d0
		moveq	#17-1,d2
		bsr	blit3

		move.l	(fichier,pc),a0		TIR VAISSEAU TAILLE 2
		add.l	#OFFSET16,a0
		move.l	(ecran,pc),a1
		add.l	#SPLAN*(19*2+17+11+17+13+9+15),a1
		bsr	wait_blitter
		move.l	#13*2,d6
		move.l	#13*2*3,d7
		move.w	#13*64+1,d0
		moveq	#9-1+1,d2
		bsr	blit3

		move.l	(fichier,pc),a0		TIR VAISSEAU TAILLE 3
		add.l	#OFFSET17,a0
		move.l	(ecran,pc),a1
		add.l	#SPLAN*(19*2+17+11+17+13+9+15+13),a1
		bsr	wait_blitter
		move.l	#9*2,d6
		move.l	#9*2*3,d7
		move.w	#9*64+1,d0
		moveq	#3-1,d2
		*bsr	blit3

		*-----------------------------*
		move.l	(fichier,pc),a0		VAISSEAU TAILLE 1
		add.l	#OFFSET18,a0
		move.l	(ecran,pc),a1
		add.l	#SPLAN*(19*2+17+11+17+13+9+15+13+10),a1
		bsr	wait_blitter
		move.l	#15*2,d6
		move.l	#15*2*5,d7
		move.w	#15*64+1,d0
		moveq	#17-1,d2
		bsr	blit5

		move.l	(fichier,pc),a0		VAISSEAU TAILLE 2
		add.l	#OFFSET19,a0
		move.l	(ecran,pc),a1
		add.l	#SPLAN*(19*2+17+11+17+13+9+15+13+10+15),a1
		bsr	wait_blitter
		move.l	#13*2,d6
		move.l	#13*2*5,d7
		move.w	#13*64+1,d0
		moveq	#7-1,d2
		bsr	blit5
		moveq	#1-1,d2
		bsr	blitXX
		moveq	#1-1,d2
		bsr	blit5

		move.l	(fichier,pc),a0		VAISSEAU TAILLE 3
		add.l	#OFFSET20,a0
		move.l	(ecran,pc),a1
		add.l	#SPLAN*(19*2+17+11+17+13+9+15+13+10+15+13),a1
		bsr	wait_blitter
		move.l	#9*2,d6
		move.l	#9*2*5,d7
		move.w	#9*64+1,d0
		moveq	#5-1,d2
		bsr	blit5

		*-----------------------------*
		move.l	(fichier,pc),a0		VAISSEAU TAILLE 1 6 plans
		add.l	#OFFSET21,a0
		move.l	(ecran,pc),a1
		add.l	#SPLAN*(19*2+17+11+17+13+9+15+13+10+15+13+9),a1
		bsr	wait_blitter
		move.l	#15*2,d6
		move.l	#15*2*6,d7
		move.w	#15*64+1,d0
		moveq	#17-1,d2
		bsr	blitXX

		move.l	(fichier,pc),a0		VAISSEAU TAILLE 2 6 plans
		add.l	#OFFSET22,a0
		move.l	(ecran,pc),a1
		add.l	#SPLAN*(19*2+17+11+17+13+9+15+13+10+15+13+9+15),a1
		bsr	wait_blitter
		move.l	#13*2,d6
		move.l	#13*2*6,d7
		move.w	#13*64+1,d0
		moveq	#7-1,d2
		bsr	blitXX
		moveq	#1-1,d2
		bsr	blitXXX
		moveq	#1-1,d2
		bsr	blitXX

		move.l	(fichier,pc),a0		VAISSEAU TAILLE 3 5 plans
		add.l	#OFFSET23,a0
		move.l	(ecran,pc),a1
		add.l	#SPLAN*(19*2+17+11+17+13+9+15+13+10+15+13+9+15+13),a1
		bsr	wait_blitter
		move.l	#9*2,d6
		move.l	#9*2*5,d7
		move.w	#9*64+1,d0
		moveq	#5-1,d2
		bsr	blit5

		btst	#6,$bfe001
		bne	.loop
		rts

*=========================================================================*
boby:
.loop		bsr	x_synchro

		move.l	(fichier,pc),a0
		add.l	#OFFSET24,a0
		move.l	(ecran,pc),a1
		bsr	wait_blitter
		move.l	#$09f00000,($40,a6)
		move.l	#-1,($44,a6)
		move	#2-2,($64,a6)
		move	#SPLAN-2,($66,a6)
		move.l	#13*2,d6
		move.l	#13*2*3,d7
		move.w	#10*64+1,d0
		moveq	#1-1,d2
		bsr	blyt3

		move.l	(fichier,pc),a0
		add.l	#OFFSET25,a0
		move.l	(ecran,pc),a1
		addq.l	#2,a1
		move.l	#11*2,d6
		move.l	#11*2*3,d7
		move	#08*64+1,d0
		moveq	#1-1,d2
		bsr	blyt3

		move.l	(fichier,pc),a0
		add.l	#OFFSET26,a0
		move.l	(ecran,pc),a1
		addq.l	#4,a1
		move.l	#9*2,d6
		move.l	#9*2*3,d7
		move.w	#6*64+1,d0
		moveq	#1-1,d2
		bsr	blyt3

		moveq	#17-1,d4
		lea	(pince.fzx,pc),a2
		move.l	#10*SPLAN,d5
.pinceloop	bsr	gigablit
		addq.l	#2,d5
		lea	(fzx2-fzx1,a2),a2
		dbf	d4,.pinceloop

		lea	(generateur.fzx,pc),a2
		move.l	#32*SPLAN,d5
		bsr	gigablit

		lea	(emetteur.fzx,pc),a2
		moveq	#17-1,d4
		move.l	#52*SPLAN,d5
.emettloop	bsr	gigablit
		add.l	(4,a2),d5
		lea	(fzx2-fzx1,a2),a2
		dbf	d4,.emettloop


		btst	#6,$bfe001
		bne	.loop
		rts

*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/* 
text:
.loop		bsr	x_synchro
		move.l	(fichier,pc),a0
		move.l	(ecran,pc),a1
		add.l	#OFFSET_EBONSTAR,a0
		bsr	wait_blitter
		move.l	#$09f00000,($40,a6)
		move.l	#-1,($44,a6)
		move.w	#0,($64,a6)
		move.w	#SPLAN-40,($66,a6)
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move.w	#41*64+20,($58,a6)
		add.l	#41*SPLAN,a1
		bsr	alphaville
		btst	#6,$bfe001
		bne	.loop
		rts

alphaville:	move.l	(ecran,pc),d6		ça doit être l'adresse écran
		add.l	#SPLAN*45,d6		on évite le sigle
		bsr	wait_blitter
		move.l	#$09f00000,($40,a6)
		move.l	#-1,($44,a6)
		move.w	#0,($64,a6)
		move.w	#SPLAN-2,($66,a6)
		moveq	#SLINE,d4		compteur ligne
 		move.l	#2*12,d2		taille 1 plan source
 		lea	(.york,pc),a4		Oui Pierre on aime les tables
		move	#9*64+1,d0		Blitter window		

		* ZE LOOP OF THE LIFE *
.read_long:	move.l	(fichier,pc),a0
		tst.l	(a4)			fin de la table monsieur ?
		beq	.extravadance		-> Ben ouais !
		add.l	(a4)+,a0		+offset
		move.l	(a4)+,d7		nbre de plan(s)
		subq.w	#1,d7
		move.w	d7,d3
		move.l	d6,a1
		move.l	(a4)+,d1		plan start
		subq.w	#1,d1
		mulu	#SLINE,d1
		add.l	d1,a1
		move.l	(a4)+,d5

		* affichage *
.two		move.l	a1,a2
		move.w	d3,d7			
.loopaff	bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a2,($54,a6)
		move	d0,($58,a6)
		add.l	d2,a0
		lea	(SLINE,a2),a2
		dbf	d7,.loopaff
		addq.l	#2,a1
		addq.l	#2,d6
		subq.w	#2,d4			On à fini la ligne ?
		bne.s	.cont			-> non
		moveq	#SLINE,d4		Réinit. le compteur ligne
		add.l	#(SPLAN*12)-SLINE,d6	on va à la ligne
		add.l	#(SPLAN*12)-SLINE,a1	on va à la ligne j'ai dit
.cont		dbf	d5,.two
		bra.s	.read_long
.extravadance	
		* AFFICHE LE CIRCLE *
		lea	(circle,pc),a2
		moveq	#6-1,d7
		move.l	#SPLAN*80,d5
		bsr	megablit

		* AFFICHE  ELECTROSHIP *
		lea	(bobo,pc),a2
		moveq	#12-1,d7
		move.l	#SPLAN*110,d5
		bsr	megablit

		* AFFICHE LE BONUS *
		lea	(bonusprout,pc),a2
		moveq	#1-1,d7
		move.l	#SPLAN*130,d5
		bsr	megablit
		* AFFICHE LA BOULE 1 *
		lea	(boule1,pc),a2
		moveq	#1-1,d7
		bsr	megablit
		* AFFICHE LA BOULE 2 *
		lea	(boule2,pc),a2
		moveq	#1-1,d7
		bsr	megablit
		* AFFICHE LA BOULE 3 *
		lea	(boule3,pc),a2
		moveq	#1-1,d7
		bsr	megablit
		rts

		CNOP	0,4
.york:		dc.l	OFFSETAA,3,3,3		A,B,C,D
		dc.l	175994,1,1,0		E
		dc.l	176026,1,1,0		F
		dc.l	176058,3,3,0		G
		dc.l	176130,1,1,0		H
		dc.l	176162,1,1,0		I
		dc.l	176194,3,3,1		J,K
		dc.l	176338,1,1,0		L
		dc.l	176370,1,1,0		M
		dc.l	176402,1,1,0		N
		dc.l	176434,3,3,0		O
		dc.l	176506,1,1,0		P
		dc.l	176538,3,3,1		Q,R
		dc.l	176682,1,1,0		S
		dc.l	176714,1,1,0		T
		dc.l	176746,3,3,2		U,V,W
		dc.l	176962,1,1,0		X
		dc.l	176994,1,1,0		Y
		dc.l	177026,1,1,0		Z
		dc.l	177058,3,2,0		©
		dc.l	OFFSET_0,3,3,9		0,1,2,3,4,5,6,7,8,9
		dc.l	0			THE END

************** AFFICHE SUR 4 PLANS A PARTIR DU PLAN 2 *********************
* d0		fenêtre Blitter
* d2		nombre - 1
* d6		taille d'un plan source
* d7		taille d'un bob entier
* a0.l		adresse source
* a1.l		adresse destination
blit32:		lea	(SLINE*1,a1),a1		commence plan 2
.zooop		move.l	a0,a2
		move.l	a1,a3
		moveq	#4-1,d1
.nplan		bsr	wait_blitter
		move.l	a2,($50,a6)
		move.l	a3,($54,a6)
		move	d0,($58,a6)
		add.l	d6,a2
		lea	(SLINE,a3),a3
		dbf	d1,.nplan
		add.l	d7,a0
		addq.l	#4,a1
		dbf	d2,.zooop
		rts

************** AFFICHE SUR 4 PLANS A PARTIR DU PLAN 2 *********************
* d0		fenêtre Blitter
* d2		nombre - 1
* d6		taille d'un plan source
* d7		taille d'un bob entier
* a0.l		adresse source
* a1.l		adresse destination
* a6.l		CUSTOM
* utilise	d0,d1,d2,d6,d7 / a0,a1,a2,a3,a6
blit_one:	moveq	#1-1,d2
		bra.s	maxi_blit
blit:		lea	(SLINE*1,a1),a1		commence plan 2
maxi_blit
.zooop		move.l	a0,a2
		move.l	a1,a3
		moveq	#4-1,d1
.nplan		bsr	wait_blitter
		move.l	a2,($50,a6)
		move.l	a3,($54,a6)
		move	d0,($58,a6)
		add.l	d6,a2
		lea	(SLINE,a3),a3
		dbf	d1,.nplan
		add.l	d7,a0
		addq.l	#2,a1
		dbf	d2,.zooop
		rts

************** AFFICHE SUR 3 PLANS A PARTIR DU PLAN 2 *********************
* d0		fenêtre Blitter
* d2		nombre - 1
* d6		taille d'un plan source
* d7		taille d'un bob entier
* a0.l		adresse source
* a1.l		adresse destination
blyt3:		lea	(SLINE*1,a1),a1		commence plan 3
blit3:		lea	(SLINE*1,a1),a1		commence plan 2
blitO:
.zooop		move.l	a0,a2
		move.l	a1,a3
		moveq	#3-1,d1
.nplan		bsr	wait_blitter
		move.l	a2,($50,a6)
		move.l	a3,($54,a6)
		move	d0,($58,a6)
		add.l	d6,a2
		lea	(SLINE,a3),a3
		dbf	d1,.nplan
		add.l	d7,a0
		addq.l	#2,a1
		dbf	d2,.zooop
		rts

************** AFFICHE SUR 5 PLANS A PARTIR DU PLAN 1 *********************
* d0		fenêtre Blitter
* d2		nombre - 1
* d6		taille d'un plan source
* d7		taille d'un bob entier
* a0.l		adresse source
* a1.l		adresse destination
blit5:		
.zooop		move.l	a0,a2
		move.l	a1,a3
		moveq	#5-1,d1
.nplan		bsr	wait_blitter
		move.l	a2,($50,a6)
		move.l	a3,($54,a6)
		move	d0,($58,a6)
		add.l	d6,a2
		lea	(SLINE,a3),a3
		dbf	d1,.nplan
		add.l	d7,a0
		addq.l	#2,a1
		dbf	d2,.zooop
		rts

* SPECIAL POUR UN GFX DU VAISSEAU TAILLE 2 KI A 6 PLANS ???
blitXX:
.zooop		move.l	a1,a3
		moveq	#6-1,d1
.nplan		cmp	#1,d1
		beq.s	.skip
		bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a3,($54,a6)
		move	d0,($58,a6)
		lea	(SLINE,a3),a3
.skip		add.l	d6,a0
		dbf	d1,.nplan
		addq.l	#2,a1
		dbf	d2,.zooop
		rts

* SPECIAL POUR UN GFX DU VAISSEAU TAILLE 2 KI A 7 PLANS ???
blitXXX:
.zooop		move.l	a1,a3
		moveq	#7-1,d1
.nplan		cmp	#2,d1
		beq.s	.skip
		cmp	#1,d1
		beq.s	.skip
		bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a3,($54,a6)
		move	d0,($58,a6)
		lea	(SLINE,a3),a3
.skip		add.l	d6,a0
		dbf	d1,.nplan
		addq.l	#2,a1
		dbf	d2,.zooop
		rts

*=========================================================================*
* Gestion animation *
* entrée	d1.w	nombre maximum d'animation moins une
*		d2.w	espace qui sépare 2 anaimations
*		d3.l	adresse page graphique
x_calcul:	lea	(tempo,pc),a0
		subq.w	#1,(a0)
		bne.s	.exit
		move.w	#20,(a0)+		init. tempo.
		move.w	(a0)+,d0		d1=incrément 1 ou -1
		add.w	d0,(a0)
		beq.s	.nul
		cmp.w	(a0),d1			dernière image ?
		bne.s	.ok			-> non
.nul		neg.w	(-2,a0)			on inverse le sens
.ok		move.w	(a0),d0
		mulu	d2,d0
		add.l	d3,d0
		lea	(xfichier,pc),a0
		move.l	d0,(a0)
.exit		rts
		CNOP	0,4
tempo		dc.w	1		tempo.
		dc.w	1		inc.
		dc.w	0		offset

*=========================================================================*
*=========================================================================*
		CNOP	0,4
fzet		dc.l	0	^		offset sur le fichier
finc		dc.l	2	^		largeur du gfx
fhaut		dc.l	16	^		hauteur du gfx
fplan		dc.l	5	^		nombre de plan(s) du gfx
fstart		dc.l	1	!		commence à afficher à partir du plan 1
fzx:		dc.l	0
fzx.old:	dc.l	0
ffff:		dc.l	1			flag si=1 CLS

		* offset,largeur,hauteur,fplan,fstart
fzx1:		dc.l	OFFSET1,336,105,2,1	1 trou
fzx2		dc.l	OFFSET2,224,053,2,1	2 trou
		dc.l	OFFSET3,140,053,2,1	3 apparition trou
		dc.l	OFFSET4,140,053,2,1	4 disparition trou
		dc.l	OFFSET5,276,025,2,1	5 bosse
		dc.l	OFFSET_EBONSTAR,40,41,1,1 6 Sigle Ebonstar
		dc.l	OFFSET7,014,105,3,1	7 masque couleur
		dc.l	OFFSET8,004,019,4,2	8 Gros taille 1
		dc.l	OFFSET9,002,017,4,2	9 Gros taille 2
		dc.l	OFFSET10,002,011,4,2	10 Gros taille 3
		dc.l	OFFSET12,002,17,4,2	11 Tir du gros taille 1
		dc.l	OFFSET13,002,13,4,2	12 Tir du gros taille 2
		dc.l	OFFSET14,002,09,4,2	13 Tir du gros taille 3
		dc.l	OFFSET18,002,15,5,1	Vaisseau taille 1
		dc.l	OFFSET19,002,13,5,1	Vaisseau taille 2
		dc.l	OFFSET20,002,09,5,1	Vaisseau taille 3
		dc.l	OFFSET24,002,13,3,3	Boule taille 1
		dc.l	OFFSET25,002,11,3,3	Boule taille 2
		dc.l	OFFSET26,002,09,3,3	Boule taille 3
		dc.l	OFFSETAA,002,012,3,3	Lettre A
generateur.fzx:	dc.l	OFFSETGB,004,019,5,1	Boule générateur
		dc.l	OFFSET_0,002,012,3,3	Chiffre 0
		dc.l	OFFSETE1,002,015,5,1	Explosion ??
		dc.l	OFFSETK1,002,007,4,2	Gros apparition image 6
		dc.l	OFFSETK2,002,009,4,2	Gros apparition image 5
		dc.l	OFFSETK3,002,011,4,2	Gros apparition image 4
		dc.l	OFFSETK4,002,013,4,2	Gros apparition image 3
		dc.l	OFFSETK5,002,015,4,2	Gros apparition image 2
		dc.l	OFFSETK6,002,017,4,2	Gros apparition image 1

pince.fzx:	dc.l	174050,002,20,3,3	Pince
		dc.l	174170,002,20,3,3
		dc.l	174286,002,19,3,3
		dc.l	174400,002,19,3,3
		dc.l	174518,002,17,3,3
		dc.l	174620,002,16,3,3
		dc.l	174716,002,14,3,3
		dc.l	174800,002,12,3,3
		dc.l	174872,002,11,3,3
		dc.l	174938,002,11,3,3
		dc.l	175004,002,12,3,3
		dc.l	175076,002,14,3,3
		dc.l	175160,002,16,3,3
		dc.l	175256,002,17,3,3
		dc.l	175358,002,19,3,3
		dc.l	175472,002,19,3,3
		dc.l	175586,002,20,3,3

emetteur.fzx:	dc.l	167198,4,17,5,1
		dc.l	167742,4,17,5,1
		dc.l	168192,2,19,5,1
		dc.l	168490,2,18,5,1
		dc.l	168768,2,17,5,1
		dc.l	169046,2,18,5,1
		dc.l	169340,2,19,5,1
		dc.l	169656,2,21,5,1
		dc.l	169992,2,21,5,1
		dc.l	170328,2,21,5,1
		dc.l	170664,2,21,5,1
		dc.l	170988,2,19,5,1
		dc.l	171286,2,18,5,1
		dc.l	171568,2,17,5,1
		dc.l	171846,2,18,5,1
		dc.l	172140,2,19,5,1
		dc.l	172534,4,17,5,1

circle:		dc.l	185290,4,25,3,1
bobo:		dc.l	181240,2,15,5,1		electro-ship
bonusprout:	dc.l	177136,2,13,5,1
boule1		dc.l	OFFSET24,2,13,3,3
boule2		dc.l	OFFSET25,2,11,3,3
boule3		dc.l	OFFSET26,2,9,3,3
fzx3:
*»»»»»»»»»»»»»»»»»»»»» AFFICHAGE AVEC TABLE & BOUCLE «««««««««««««««««««««*
* Entrée	=> d5.l	offset écran
*		=> d7.w	nombre de gfx - 1
*		=> a2.l	table paramètre d'affichage
megablit:
		move.l	(a2),-(a7)	sauve offset
.loopa		move.l	d7,-(a7)
		bsr	gigablit
		move.l	(a7)+,d7
		move.l	(04,a2),d0	d0=largeur du gfx en octets
		add.l	d0,d5		INC. ADR. ECRAN
		move.l	(08,a2),d1	d1=hauteur du gfx en lignes
		move.l	(12,a2),d2	d2=nombre de plan
		mulu	d1,d0
		mulu	d2,d0		d0=taille du gfx
		add.l	d0,(a2)
		dbf	d7,.loopa
		move.l	(a7)+,(a2)	restitue offset
		rts

*»»»»»»»»»»»»» AFFICHAGE AVEC PARAMETRES DE LA TABLE 'fzx1' ««««««««««««««*
* Entrée	=> d5.l	offset écran
*		=> a2.l	table paramètre d'affichage
*		=> a6.l CUSTOM
* (utilise d0/d1/d2/d3/d5/a0/a1/a2/a6)
gigablit:
		* calcul modulo source *
		bsr	wait_blitter
		move.l	(4,a2),d0		"finc"
		sub.l	#T_MAXY,d0
		bpl.s	.modaxx
		clr.w	d0
.modaxx		move.w	d0,($64,a6)
		move.l	#-1,($44,a6)
		move.l	#$09f00000,($40,a6)

		* calcul modulo destination *
		move.l	(4,a2),d1		"finc"
		move.w	#SPLAN-T_MAXY,d0
		cmp.l	#T_MAXY,d1
		bhi.s	.moddxx
		move.l	#SPLAN,d0
		sub.l	d1,d0
.moddxx		move.w	d0,($66,a6)

		* calcul fenêtre Blitter * 
		move.l	(4,a2),d0		"finc"
		cmp	#T_MAXY,d0
		blt.s	.windowxx
		move	#T_MAXY,d0
.windowxx	lsr.w	#1,d0
		move.l	(8,a2),d1		"fhaut"
		lsl.w	#6,d1
		add.w	d1,d0			d0=FB

		move.l	(fichier,pc),a0
		add.l	(a2),a0
		move.l	(ecran,pc),a1
		add.l	d5,a1
		move.l	(16,a2),d1		"fstart"
		subq.w	#1,d1
		mulu	#SLINE,d1
		add.l	d1,a1
		move.l	#SLINE,d3
		move.l	(4,a2),d2		"finc"
		move.l	(8,a2),d1		"fhaut"
		mulu	d1,d2

		* affichage *
		move.l	(12,a2),d7		"fplan"
		subq.w	#1,d7
.loopaff	bsr	wait_blitter
		move.l	a0,($50,a6)
		move.l	a1,($54,a6)
		move	d0,($58,a6)
		add.l	d2,a0
		add.l	d3,a1
		dbf	d7,.loopaff
		rts

*«««««««««««««««««««««««««««««« TABLE 'TLK' »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
		CNOP	0,4
tlk:
adr_fonteraw	dc.l	0
vector68	dc.l	0
vector6C	dc.l	0
xfichier	dc.l	0
handle		dc.l	0
copper:		dc.l	0
vbl.flag	dc.w	0
sbplcon3	dc.w	$0000
save_intena	dc.w	0
save_dmacon	dc.w	0
gfxbase		dc.l	0
dosbase		dc.l	0
ecran		dc.l	0
fichier		dc.l	0
gfxname		dc.b	'graphics.library',0
dosname		dc.b	'dos.library',0
fichiername:	dc.b	"DH0:Shapes",0
version:	dc.b	"$VER : Spiderman VS Casimir / ver0.3749b+ / 25.12.93",0
		EVEN

***************************************************************************
*>>>>>>>>>>>* INTERRUPTION DE NIVEAU 2 : CIA-A, Test clavier *<<<<<<<<<<<<*
***************************************************************************
interruption_clavier:
		btst	#3,$bfed01
		beq.s	.exit2
		movem.l	d0/a0,-(a7)
		clr.w	d0
		move.b	$bfec01,d0
		ori.b	#$40,$bfee01
		swap	d0
		lea	$DFF006,a0
		move.b	(a0),d0
.wait1		cmp.b	(a0),d0
		beq.s	.wait1
		move.b	(a0),d0
.wait2		cmp.b	(a0),d0
		beq.s	.wait2
		move.b	(a0),d0
.wait3		cmp.b	(a0),d0
		beq.s	.wait3
		swap	d0
		andi.b	#$bf,$bfee01
		lea	(table_clavier,pc),a0
		not.b	d0
		ror.b	#1,d0
		move.w	d0,(-4,a0)
		cmp	#128,d0
		blt.s	.touche_enfonce
		sub	#128,d0		
		clr.b	(a0,d0)
		tst	(-2,a0)
		beq.s	.exit
		subq	#1,(-2,a0)
		bra.s	.exit
.touche_enfonce	move.b	#1,(a0,d0)
		addq	#1,(-2,a0)
.exit		movem.l	(a7)+,d0/a0
.exit2		move	#$0008,$DFF09C
		rte
		dc.w	0			-4 dernier code touche clavier
		dc.w	0			-2 nbre de touche enfoncées
table_clavier:	dcb.b	128,0			00 table des touches

*=========================================================================*

vbl:		movem.l	d0-d7/a0-a6,-(a7)
		lea	CUSTOM,a6
		lea	(vbl.flag,pc),a0
		move.w	#1,(a0)
		lea	sprite,a0
		lea	($120,a6),a1
		moveq	#8-1,d0
.loop_sprite	move.l	a0,(a1)+
		dbf	d0,.loop_sprite
		moveq	#NPLAN-1,d1
		move.l	(ecran,pc),d0
		lea	($00E0,a6),a0
.loop_bitplane	move.l	d0,(a0)+
		add.l	#SLINE,d0
		dbf	d1,.loop_bitplane
		bsr	x_key
		movem.l	(a7)+,d0-d7/a0-a6
		move.w	#$0020,$dff09c
		rte

*=========================================================================*

affiche_offset:	move.l	(fzet,pc),d2
		bsr	convertit_decimal
		moveq	#8-1,d1
		lea	(.number,pc),a0
		bsr	conv_number_ascii
		lea	(.text,pc),a0
		move	#24*8,d0
		move	#NLINE-8,d1
		bsr	printf
		rts
.text		dc.b	'OFFSET=>'
.number		dc.b	'00000000',0,0
		EVEN

affiche_large:	move.l	(finc,pc),d2
		bsr	convertit_decimal
		moveq	#5-1,d1
		lea	(.number,pc),a0
		bsr	conv_number_ascii
		lea	(.text,pc),a0
		move	#24*8,d0
		move	#NLINE-16,d1
		bsr	printf
		rts
.text		dc.b	'LARGE ====>'
.number		dc.b	'00000',0,0
		EVEN

affiche_haut:	move.l	(fhaut,pc),d2
		bsr	convertit_decimal
		moveq	#3-1,d1
		lea	(.number,pc),a0
		bsr	conv_number_ascii
		lea	(.text,pc),a0
		move	#24*8,d0
		move	#NLINE-24,d1
		bsr	printf
		rts
.text		dc.b	'HAUT  ======>'
.number		dc.b	'000',0,0
		EVEN

affiche_plan:	move.l	(fplan,pc),d2
		bsr	convertit_decimal
		moveq	#1-1,d1
		lea	(.number,pc),a0
		bsr	conv_number_ascii
		lea	(.text,pc),a0
		move	#24*8,d0
		move	#NLINE-32,d1
		bsr	printf
		rts
.text		dc.b	'PLAN =========>'
.number		dc.b	'0',0,0

affiche_start:	move.l	(fstart,pc),d2
		bsr	convertit_decimal
		moveq	#1-1,d1
		lea	(.number,pc),a0
		bsr	conv_number_ascii
		lea	(.text,pc),a0
		move	#24*8,d0
		move	#NLINE-40,d1
		bsr	printf
		rts
.text		dc.b	'START ========>'
.number		dc.b	'0',0,0
		EVEN

affiche_offx:	move.l	(fzx,pc),d2
		addq.l	#1,d2
		bsr	convertit_decimal
		moveq	#2-1,d1
		lea	(.number,pc),a0
		bsr	conv_number_ascii
		lea	(.text,pc),a0
		move	#24*8,d0
		move	#NLINE-48,d1
		bsr	printf
		rts
.text		dc.b	'======>OFFSET '
.number		dc.b	'00',0,0
		EVEN

*=========================================================================*
*----------------------- Affiche le text à l'écran -----------------------*
* Entrée	=> d0.w	Position x en pixel (multiple de 8)
*		=> d1.w	Position Y en pixel
*		=> a0.l	Adr. chaine de caractère /,0=fin ligne /,0,0=fin texte
* 		=> a5.l	adr. table 'tlk'
printf:		move.l	#SPLAN,d3
		move.l	d3,d4
		mulu	d3,d1
		lsl	#3,d3
		move.l	(ecran,pc),d2
		lsr	#3,d0
		add	d0,d1
		add.l	d1,d2
		lea	(fonteraw,pc),a2
		move.l	a2,d0
		moveq	#0,d1
		bra.s	.read_char
.new_line	move.l	d2,a1
.new_char	move.l	d0,a2
		add	d1,a2
		move.l	a1,a3
		moveq	#8-1,d5
.aff_loop	move.b	(a2),(a3)
		lea	(256,a2),a2
		add.l	d4,a3
		dbf	d5,.aff_loop
		addq.l	#1,a1
		move.b	(a0)+,d1
		bne.s	.new_char
		add.l	d3,d2
.read_char	move.b	(a0)+,d1
		bne.s	.new_line
.exit		rts

;****************** Convertit un nombre binaire en BCD ********************
; Entrée	d2.l	Chiffre binaire à convertir
; Sortie	d3.l	Chiffre BCD
; Utilise	d0,d1,d2,d3,d4 / a0,a1
convertit_decimal:
		moveq	#0,d3
		move.l	#99999999,d0		Maximum de points autorisés
		cmp.l	d2,d0			Dépassement ?
		bhi.s	.pas_depass		-> non
		move.l	d0,d2
.pas_depass	lea	(.chiffre,pc),a0
		lea	(.cont,pc),a1
		moveq	#8-1,d4
.loop		move.l	(a0)+,d0		d0=Chiffre binaire
		move.l	(a0)+,d1		d1=Chiffre DCB		
.cont		cmp.l	d2,d0
		bhi.s	.next
		add.l	d1,d3
		sub.l	d0,d2
		jmp	(a1)
.next		dbf	d4,.loop
		rts
		CNOP	0,4
.chiffre:	dc.l	10000000,$10000000,1000000,$1000000,100000,$100000
		dc.l	10000,$10000,1000,$1000,100,$100,10,$10,1,$1

***************** Convertit un nombre DCB en chaine ascii *****************
* Entrée	=> d1.w	Nombre de chiffre - 1
*		=> d3.l	Nombre DCB
*		=> a0.l	Chaine ascii
* Utilise	d0,d1,d2,d3,d4 / a0
conv_number_ascii:
		and.w	#7,d1			8 chiffres maximums
		moveq	#$f,d2
		move	#'0',d4
.loop		move.l	d3,d0
		lsr.l	d1,d0
		lsr.l	d1,d0
		lsr.l	d1,d0
		lsr.l	d1,d0
		and.l	d2,d0
		add.w	d4,d0
		cmp	#'9',d0
		bls.s	.ok
		add.w	#'A'-'9'-1,d0
.ok		move.b	d0,(a0)+
		dbf	d1,.loop
		rts
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"

*»»»»»»»»»»»»»»»»»»»»» Efface un écran de 320*256*5 ««««««««««««««««««««««*
CLS:		movem.l	d0-d7/a0-a4,-(a7)
		bsr.s	clear_screen
		movem.l	(a7)+,d0-d7/a0-a4
		rts
clear_screen:	movem.l	a5-a6,-(a7)
		lea	(.pile,pc),a0
		move.l	a7,(a0)	
		moveq	#0,d1
		moveq	#0,d2
		moveq	#0,d3
		moveq	#0,d4
		moveq	#0,d5
		moveq	#0,d6
		moveq	#0,d7
		sub.l	a1,a1
		sub.l	a2,a2
		sub.l	a3,a3
		sub.l	a4,a4
		sub.l	a5,a5
		sub.l	a6,a6
		sub.l	a7,a7
		move.l	(ecran,pc),a0
		add.l	#ECRAN_SIZE,a0

		* ON EFFACE 49392 OCTETS *
		moveq	#14-1,d0		14*63*56=49392 octets
.loop_clear
		REPT	63			63 instrutions = 252 octets 
		movem.l	d1-d7/a1-a7,-(a0)
		ENDR
		dbf	d0,.loop_clear		+ 4 octets = taille du cache

		* ON EFFACE 1792 OCTETS *
		REPT	32
		movem.l	d1-d7/a1-a7,-(a0)	32*56=1792 octets
		ENDR		
		* ON EFFACE 16 OCTETS *
		movem.l	d1-d4,-(a0)
		move.l	(.pile,pc),a7
		movem.l	(a7)+,a5-a6
		rts
		CNOP	0,4
.pile		dc.l	0

*=========================================================================*
**************************** Section graphique ****************************
*=========================================================================*
		SECTION		CASIMIR,DATA_C
		CNOP 0,8		* on aligne sur 64 bits
sprite		dc.l	0,0,0,0
		END

;«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«;

000000 |======>	Page graphique de 2688*105*2
		contient		: 24 images du trou noir
		taille d'un trou noir	: 112*105
		taille page 		: 336*105*2=70560 octets

070560 |======> Page graphique de 1792*053*2
		contient 		: 16 images ½ haute du trou noir 
		taille d'un ½ trou noir : 112*53
		taille page		: 224*53*2=23744 octets

094304 |======> Page graphique de 1120*053*2
		contient		: 10 ½ images appartion trou noir
		taille d'un ½ trou noir : 112*53
		taille page		: 140*53*2=14840 octets

109144 |======>	Page graphique de 1120*053*2
		contient		: 10 ½ images ½ disparition trou noir
		taille d'un ½ trou noir : 112*53
		taille page		: 140*53*2=14840 octets

123984 |======>	Page graphique de 2208*25*2
		contient		: 69 images de la bosse
		taille d'une bosse	: 32*25
		taille page		: 276*25*2=13800 octets

137784 |======>	Page graphique de 112*105*3
		contient		: masque couleur du trou
		taille			: 112*105
		taille page		: 14*105*3=4410

142194 |======>	Page graphique de 32*323*4
		contient 		: 17 positions du gros + msk
		taille			: 32*19
		taille page		: 4*323*4=5168

147362 |======>	Page graphique de 16*153*4
		contient 		: 9 positions du gros + msk en petit1
		taille			: 16*17
		taille page		: 2*153*4=1224

148586 |======>	Page graphique de 16*55*4
		contient 		: 5 positions du gros + msk en petit1
		taille			: 16*11
		taille page		: 2*55*4=440

149026 |======>	Page 	?????????????
		contient
		taille
		taille page		: 596 octets

149622 |======> Page graphique de 16*289*4
		contient		: 17 positions du tir du gros taille 1
		taille			: 16*17
		taille page		: 2*289*4=2312 octets

151934 |======> Page graphique de 16*117*4
		contient		: 9 positions du tir du gros taille 2
		taille			: 16*13
		taille page		: 2*117*4=936 octets

151934 |======> Page graphique de 16*45*4
		contient		: 5 positions du tir du gros taille 3
		taille			: 16*9
		taille page		: 2*45*4=360 octets

* TROU DE 20 OCTETS *

153250 |======> Page graphique de 16*255*3
		contient		: 17 positions du tir vaisseau taille1
		taille			: 16*15
		taille page		: 2*255*3=1530 octets

154780 |======> Page graphique de 16*117*3
		contient		: 9 positions du tir vaisseau taille2
		taille			: 16*13
		taille page		: 2*117*3=702 octets

155482 |======> Page graphique de ???
		contient		: ???
		taille			: ???
		taille page		: 144 octets

155626 |======> Page graphique de 16*255*5
		contient		: 17 positions du vaisseau taille 1
		taille			: 16*15
		taille page		: 2*255*5=2550 octets

158176 |======> Page graphique de 16*117*5
		contient		: 9 positions du vaisseau taille 2
		taille			: 16*13
		taille page		: 2*117*5+(13*2)=1196 octets


159372 |======> Page graphique de 16*45*5
		contient		: 5 positions du vaisseau taille 3
		taille			: 16*9
		taille page		: 2*45*5=450 octets

* TROU DE 20 OCTETS *

159842 |======> Page graphique de 16*255*6
		contient		: 17 positions du vaisseau taille 1
		taille			: 16*15
		taille page		: 2*255*6=2550 octets

162902 |======> Page graphique de 16*117*6
		contient		: 9 positions du vaisseau taille 2
		taille			: 16*13
		taille page		: 2*117*6+(13*2)=1430 octets

164332 |======> Page graphique de 16*45*5
		contient		: 5 positions du vaisseau taille 3
		taille			: 16*9
		taille page		: 2*45*5=450 octets

* TROU DE 20 OCTETS *

164802 |======> Page graphique de 16*13*3
		contient		: ??? taille 1
		taille			: 16*13
		taille page		: 2*13*3 78 octets

164880 |======> Page graphique de 16*11*3
		contient		: ??? taille 2
		taille			: 16*11
		taille page		: 2*11*3 66 octets

164946 |======> Page graphique de 16*09*3
		contient		: ??? taille 2
		taille			: 16*09
		taille page		: 2*09*3 66 octets

165000

177136 |======> Page graphique de 16*13*5
		contient		: Bonus tir prout
		taille			: 16*13
		taille page		: 2*13*5=130 octets

181240 |======> Page graphique de 16*180*5
		contient		: 12 animations de l'electroship
		taille			: 16*15
		taille page		: 2*180*5=1800 octets

183040 |======>	Page graphique de 320*41*1
		contient		: Le titre EBONSTAR
		taille			: 320*41
		taille page		: 40*41*1=1640 octets

184680 |======> Page grqphique


* Routine 'gfx_ripper:' (fonctions des touches)

* TOUCHE DEL			: OFFSET MINIMUM
* TOUCHE HELP			: OFFSET MAXIMUM
* TOUCHE CURSEUR-HAUT 		: - UNE LIGNE
* TOUCHE CUSRSEUR-BAS  		: - UNE LIGNE
* TOUCHE SHIFT-CURSEUR-HAUT 	: + UNE LIGNE * HAUTEUR ECRAN
* TOUCHE SHIFT-CUSRSEUR-BAS  	: - UNE LIGNE * HAUTEUR ECRAN
* TOUCHE ALT-CURSEUR-HAUT 	: + UNE LIGNE * 8
* TOUCHE ALT-CUSRSEUR-BAS  	: - UNE LIGNE * 8
* TOUCHE AMIGA-CURSEUR-HAUT 	: + UNE LIGNE * HAUTEUR ECRAN * 8
* TOUCHE AMIGA-CUSRSEUR-BAS  	: - UNE LIGNE * HAUTEUR ECRAN * 8
* TOUCHE CURSEUR-DROIT	 	: + 2 OCTETS
* TOUCHE CURSEUR-GAUCHE 	: - 2 OCTETS
* TOUCHE SHIFT-CURSEUR-DROIT	: + fhaut*finc*fplan
* TOUCHE SHIFT-CURSEUR-GAUCHE 	: - fhaut*finc*fplan
;-==-<>-==-<>-==-<>-==-<>-==-<>-==-<>-==-<>-==-<>-==-<>-==-<>-==-<>-==-<>-=;
* TOUCHE NUM-2			: HAUTEUR - UNE LIGNE
* TOUCHE NUM-8			: HAUTEUR + UNE LIGNE
* TOUCHE NUM-4			: LARGEUR - 2 OCTETS
* TOUCHE NUM-6			: LARGEUR + 2 OCTETS
* TOUCHE NUM-PLUS		: NOMBRE DE PLAN + 1
* TOUCHE NUM-MOINS		: NOMBRE DE PLAN -1
* TOUCHE NUM-0			: EN ARRIERE DANS LA TABLE FZX
* TOUCHE NUM-.			: EN AVANT DANS LA TABLE FZX

