*-------------------------------------------------------------------------*
*	Le 17 février 1994
*	Affichage 640x256 256 couleurs
*
* TOUCHE DEL =  1) CALCUL NOMBRE DE COULEURS UTILISEES
*		2) MET A ZERO LES COULEURS INUTILISEES
*		3) SAUVE FICHIER ILBM EN DH2:MENTAT.ILBM
*	
*-------------------------------------------------------------------------*
		SECTION	ETB,CODE
		OPT	P+		Pas de table de relogement
		OPT	C20+		Labels et symboles 20 caractères maxi.,  majuscules/minuscules respectées
		OPT	O-		Pas d'optimisation
		OPT	p=68020	
		INCDIR	dh0:Data/


;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONSTANTES <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
NReservation	=50	50 adresses mémoires réservées maximums
FLAG_ARP	=0	1=ouvre la librairie ARP
FLAG_TRACKDISK	=0	1=assemble programme pour être chargé en "TrackDisk"
FLAG_MEMORY	=0	1=information sur la mémoire possible
FLAG_ILBM	=1	1=assemble routines décompactage IFF-ILBM
FLAG_PP		=0	1=assemble routines décompactage PowerPacker
FLAG_MOUSE0	=0	1=assemble la routine déplacement souris port 0
FLAG_AMIGADOS	=1	1=assemble routines chargement AmigaDOS
FLAG_EXIT	=0	0=sortie sous système possible avec la touche "ESC"
FLAG_MUSIC	=0	1=assemble routines gestion module "ProTracker"
FLAG_KEY	=1	1=assemble routines de test du clavier
FLAG_SOUND	=0	1=assemble routines gestion des bruitages
FLAG_LUTIN	=0	1=assemble routines gestion des lutins
FLAG_TDISK	=0	1=assemble routines de lecture en "TrackDisk"
FLAG_TDISKW	=0	1=assemble routines d'écriture en "TrackDisk"
FLAG_SCROLL	=0	1=assemble routines gestion défilement écran
FLAG_BOB32	=0	Si=1 Alors assemble routines bobs (aff. écran 32 bits)
FLAG_FREQ	=0	Si=1 Alors assemble routine demande fréquence balayage
FLAG_CLOSEWB	=0	Si=1 Alors ferme l'écran WorkBench
FLAG_DMACON.MSK	=0

*>>>>>>>>>>>>>>>>>>>>>>>>>> Debut code MC68020 <<<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	
		INCLUDE	"Mentat/Include/mentat.s"
msg.pers	dc.b	"                                           "	¤-128
		EVEN


************* Routine appelée avant de quitter le multitâche **************
sys_extra:	
		lea	(name.ilbm,pc),a0
		moveq	#0,d0
		bsr	charge_fichier_dos
		bne.s	.exit
		move.l	d0,(adresse.ilbm-t,a5)
		moveq	#0,d7
.exit		rts
name.ilbm	dc.b	"DH2:pic/tronic.ilbm",0
		EVEN
sys_extra2	rts
*>>>>>>>>>>>>>>>>>>>>>>>>>> début du programme <<<<<<<<<<<<<<<<<<<<<<<<<<<*
run:	
		bsr	first_init
		bsr	sprites_off
		move.l	(adresse.ilbm-t,a5),d0
		moveq	#1,d1			3 buffers
		moveq	#0,d2			pas de défilement possible
		bsr	affiche_ilbm

*¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ BOUCLE PRINCIPALE HORS INTERRUPTION ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤*
main_loop:
		bsr	wait_vbl
		bsr	color_x
		btst	#6,$bfe001
		bne.s	main_loop
		bra	sys_fin


*==================> Donne Nombre de couleur utilisées <===================*
color_x:	move.l	(atable_clavier-t,a5),a0
		tst.b	(TOUCHE_DEL,a0)
		beq.s	.exit
		move.w	#$0020,(INTENA,a6)
		move.l	(pt_screen-t,a5),a0
		move.w	(SBPLCON3,a0),(BPLCON3,a6)
		bsr	clear_color_count
		bsr	.yako			|-> Nbre de couleurs disponibles
		bsr	doudou
		bsr	convertit_decimal
		moveq	#2,d1
		lea	(.t2+35,pc),a0
		bsr	conv_number_ascii
		bsr	cocol			|-> Test le nombre de couleurs
		bsr	count_color		|-> Compte les couleurs
		bsr	convertit_decimal
		moveq	#2,d1
		lea	(.t1+73,pc),a0
		bsr	conv_number_ascii
		bsr	zzcolor			|-> vire couleurs inutilisées
		bsr	doudou
		bsr	convertit_decimal
		moveq	#2,d1
		lea	(.t2+39,pc),a0
		bsr	conv_number_ascii
		lea	(.text,pc),a0
		move.w	#5,(mentat_nline-t,a5)
		clr.l	(mentat_routine-t,a5)
		bsr	mentat_info
		move.w	#$C020,(INTENA,a6)
		bsr	colors_on		|-> met les couleurs ON
		*bsr	save_screen
		tst.l	d7
.exit		rts
.text:		dc.b "                             MENTAT INFORMATIONS                                "
		dc.b "                                                                                "
.t1		dc.b "    NOMBRE DE COULEURS POSSIBLES : ...    NOMBRE DE COULEURS UTILISEES : ...    "
.t2		dc.b "    NOMBRE DE COULEURS IDENTIQUES: .../...                                      "
		dc.b "                PRESSE LE BOUTON GAUCHE OU DROIT DE TA SOURIS                   "
		EVEN

*/ Calcul le nombre de couleurs disponibles */
.yako:
		move.l	(pt_screen-t,a5),a0
		moveq	#0,d2
		move.w	(NREGISTRE,a0),d2
		addq.w	#1,d2
		bsr	convertit_decimal
		moveq	#2,d1
		lea	(.t1+35,pc),a0
		bra	conv_number_ascii

*/ Calcul le nombre de registres couleurs avec une caleur identique */
doudou:		move.l	(pt_screen-t,a5),a0
		move.w	(NREGISTRE,a0),d7
		lea	(SCOLOR_TABLE,a0),a1
		move.l	a1,d6
		lea	(coco-t,a5),a3
		move.l	#-1,(a3)
		lea	(4,a3),a4
		move.w	d7,d5
.loop		move.l	(a1)+,d0		Lit le registre couleur
		move.l	a3,a2			Table des couleurs en double
		move.l	(a2)+,d2		d2=Nbre de couleurs en double
		bmi.s	.test
.deja		cmp.l	(a2)+,d0
		beq.s	.next
		dbf	d2,.deja
.test		move.l	d6,a2			pt/table couleur
		move.w	d7,d1
.shearch	cmp.l	(a2)+,d0
		bne.s	.nofound
		cmp.l	a2,a1			on l'a comparé à lui même ?
		beq.s	.shearch
		addq.l	#1,(a3)
		move.l	d0,(a4)+
		bra.s	.next
.nofound	dbf	d1,.shearch
.next		dbf	d5,.loop
		move.l	(a3),d2
		rts

*/ Test le nombre de couleurs utilisées par l'image */
cocol:
		move.l	(pt_screen-t,a5),a4
		moveq	#0,d6
		move.w	(SP_SPLAN,a4),d6
		move.l	d6,a0
		move.w	(SP_SLINE,a4),d6
		sub.w	(SP_SPLAN,a4),d6
		move.l	d6,a1
		move.w	(SPHAUTEUR,a4),d0
		subq.w	#1,d0
		move.w	(SP_SPLAN,a4),d1
		subq.w	#1,d1
		move.w	(SPPLAN,a4),d6
		subq.w	#1,d6
		lea	(color_count,pc),a3
		move.l	(SPECRAN1,a4),a4

* Entrée	=> d0.w	Nombre de lignes-1
*		=> d1.w	Nombre d'octets-1 sur une ligne
*		=> d6.w	Nombre de plan(s)-1
*		=> a0.l	Taille d'un plan
*		=> a1.l	Offset prochaine ligne
*		=> a3.l table couleur
*		=> a4.l	Adresse mémoire du graphisme
.loopd0		move.w	d1,d2
		swap	d0
		swap	d1
.loopd2		move.w	#$80,d7
		moveq	#1,d3
.loopd3		move.l	a4,a2
		move.w	d6,d5			d5=nombre de plans - 1
		clr.w	d1			RAZ valeur couleur
		move.w	d3,d4			valeur rotation
.loopd5		move.b	(a2),d0
		and.b	d7,d0
		rol.b	d4,d0
		or.b	d0,d1
		addq.w	#1,d4
		add.l	a0,a2			prochain plan
		dbf	d5,.loopd5
		move.b	#1,(a3,d1)
		lsr.w	#1,d7
		addq.w	#1,d3
		cmp.w	#9,d3
		bne.s	.loopd3
		addq.l	#1,a4
		move.w	d2,(COLOR00,a6)
		dbf	d2,.loopd2
		add.l	a1,a4			prochaine ligne
		swap	d1
		swap	d0
		dbf	d0,.loopd0
		rts
		CNOP	0,4
color_count	dcb.b	256,0
clear_color_count:
		lea	(color_count,pc),a3
		moveq	#0,d1
		move.w	#(256/4)-1,d0
.loop		move.l	d1,(a3)+
		dbf	d0,.loop
		rts
count_color:	lea	(color_count,pc),a3
		moveq	#0,d2
		move.w	#256-1,d0
.loop		tst.b	(a3)+
		beq.s	.next
		addq.w	#1,d2
.next		dbf	d0,.loop
		rts

		* MET A ZERO LES COULEURS INUTILISEES *
zzcolor:	move.l	(pt_screen-t,a5),a0
		lea	(SCOLOR_TABLE,a0),a1
		lea	(color_count,pc),a3
		moveq	#0,d2
		move.w	#256-1,d0
.loop		tst.b	(a3)+			couleur utilisée ?
		bne.s	.next			-> oui
		clr.l	(a1)			raz couleur inutilisée
.next		addq.l	#4,a1
		dbf	d0,.loop
		rts


***************************************************************************
*»»»»»»»»»»»»»»»»»»»»»» SAUVE L'ECRAN SUR LE DISQUE ««««««««««««««««««««««*
***************************************************************************
* Entrée	=> a5.l	Adr. 't'
save_screen:
		movem.l	a5/a6,-(a7)
		tst.l	(pt_screen-t,a5)
		beq	.exit
		move.l	(pt_screen-t,a5),a0
		tst.w	(SFLAG,a0)
		beq	.exit

		*/ Sauve format de l'image */
		move.w	(SNPIXEL,a0),(iff.snpixel-t,a5)
		move.w	(SNLIGNE,a0),(iff.snligne-t,a5)	
		move.w	(SPLARGE,a0),(iff.large-t,a5)	
		move.w	(SPHAUTEUR,a0),(iff.hauteur-t,a5)	
		move.w	(SPPLAN,a0),d0
		move.b	d0,(iff.plan-t,a5)	
		move.l	(SP_SIZE,a0),(iff.body_size-t,a5)
		move.l	(SPECRAN1,a0),(.buffer-t,a5)

		*/ Calcul taille du chunk CMAP */
		moveq	#0,d1
		move.w	(NREGISTRE,a0),d0	d0=nbre de registres -1
		addq.w	#1,d0
		move.w	d0,d1
		add.w	d1,d1
		add.w	d0,d1
		move.l	d1,(iff.cmapsize-t,a5)

		*/ Recopie les registres couleurs /*
		lea	(SCOLOR_TABLE,a0),a1
		lea	(iff.cmap,pc),a2
		move.w	(NREGISTRE,a0),d0	d0=nbre de registres -1
.loop		move.w	(a1)+,d1		..R1G1B1
		move.w	(a1)+,d2		..R2G2B2
		move.w	d2,d3
		lsr.w	#8,d3
		move.w	d1,d4
		lsr.w	#4,d4
		or.b	d3,d4
		move.b	d4,(a2)+		rouge
		move.b	d2,d3
		lsr.b	#4,d3
		move.b	d1,d4
		and.b	#$F0,d4	
		or.b	d4,d3
		move.b	d3,(a2)+		vert
		and.b	#$0F,d2
		lsl.b	#4,d1
		or.b	d2,d1
		move.b	d1,(a2)+		bleu
		dbf	d0,.loop

		*/ Calcul taille du fichier - 8 octets */
		move.l	#iff.cmap-iff2,d0
		add.l	(iff.cmapsize,pc),d0
		addq.l	#8,d0			en-tête du chunk body
		add.l	(iff.body_size,pc),d0
		move.l	d0,(iff.size-t,a5)

		*/ Retour au système */
		move.w	(flag_sys_gele-t,a5),d0
		lea	(.flag_sys_gele,pc),a0
		move.w	d0,(a0)			sauve flag
		beq.s	.sys_non_gele		-> système non gelé
		bsr	sys_restore		|-> restore le système
.sys_non_gele	
		*/ Ouvre le fichier */
		lea	(.namefile,pc),a1
		move.l	a1,d1
		move.l	#1006,d2		1006 = "Mode_new"
		move.l	(dosbase,pc),a6
		jsr	(Open,a6)
		lea	(.handle,pc),a1
		move.l	d0,(a1)
		beq	.open_erreur

		*/ Ecrit le début du fichier */
		moveq	#0,d0
		lea	(iff,pc),a1
		move.l	a1,d2
		move.l	(.handle,pc),d1
		move.l	#iff.cmap-iff,d3
		move.l	(dosbase,pc),a6
		jsr	(Write,a6)
		tst.l	d0			erreur de d'écriture ?
		bmi.s	.write_filerror		-> oui

		*/ Ecrit la table couleur */
		moveq	#0,d0
		lea	(iff.cmap,pc),a1
		move.l	a1,d2
		move.l	(.handle,pc),d1
		move.l	(iff.cmapsize,pc),d3
		move.l	(dosbase,pc),a6
		jsr	(Write,a6)
		tst.l	d0			erreur de d'écriture ?
		bmi.s	.write_filerror		-> oui

		*/ Ecrit en-tête du chunk BODY */
		moveq	#0,d0
		lea	(iff.body,pc),a1
		move.l	a1,d2
		move.l	(.handle,pc),d1
		moveq	#8,d3
		move.l	(dosbase,pc),a6
		jsr	(Write,a6)
		tst.l	d0			erreur de d'écriture ?
		bmi.s	.write_filerror		-> oui

		*/ Ecrit l'image RAW-BLITTER */
		moveq	#0,d0
		move.l	(.buffer,pc),d2
		move.l	(.handle,pc),d1
		move.l	(iff.body_size,pc),d3
		move.l	(dosbase,pc),a6
		jsr	(Write,a6)
		tst.l	d0			erreur de d'écriture ?
		bmi.s	.write_filerror		-> oui
		bsr.s	.close
		bsr.s	.wait
		bsr.s	.gele

		*/ SORTIE */
.exit		movem.l	(a7)+,a5/a6
		moveq	#0,d7
		rts
.open_erreur	bsr.s	.gele
		movem.l	(a7)+,a5/a6
		moveq	#-27,d7			¤ ouverture impossible
		rts
.write_filerror	bsr.s	.close
		bsr.s	.wait
		bsr.s	.gele
		movem.l	(a7)+,a5/a6
		moveq	#-26,d7			¤ erreur d'écriture
		rts

		*/ ferme le fichier */
.close		move.l	(.handle,pc),d1
		move.l	(dosbase,pc),a6
		jsr	(Close,a6)
		rts

		*/ Attend 5 secondes */
.wait		move.l	(dosbase,pc),a6
		move.l	#50*5,d1
		jsr	(Delay,a6)		|-> Attend 5 secondes
		rts

		*/ GELE LE SYSTEME */
.gele		lea	(.flag_sys_gele,pc),a0
		tst.w	(a0)			système gelé lors de l'appel ?
		beq.s	.no_gele		-> non
		bsr	sys_save
		bsr	first_init
		lea	(t,pc),a5
		bsr	colors_on
.no_gele	rts

.namefile	dc.b	"DH2:Mentat.ILBM",0
		EVEN
.flag_sys_gele	dc.w	0


		CNOP	0,4
.handle		dc.l	0
.buffer		dc.l	0
iff:		dc.l	"FORM"
iff.size	dc.l	0	ø	taille du fichier - 8 octets
iff2:		dc.l	"ILBM"
		dc.l	"BMHD"
		dc.l	$14		taile du chunk BMHD
iff.large	dc.w	0	ø	largeur de la page
iff.hauteur	dc.w	0	ø	hauteur	de la page
		dc.l	0
iff.plan	dc.b	0	ø	nombre de plan(s) (1 à 8)
		dc.b	0		type de masque
		dc.b	0		pas de compression (0=aucune)
		dc.b	0
		dc.l	$00002C2C
iff.snpixel	dc.w	0	ø	Largeur fenêtre écran
iff.snligne	dc.w	0	ø	Hauteur fenêtre écran

		* CHUNK CMAP * 
		dc.l	"CMAP"
iff.cmapsize	dc.l	0	ø
iff.cmap	dcb.b	256*3		Maximum 256 couleurs

		* CHUNK BODY *
iff.body:	dc.l	"BODY"
iff.body_size	dc.l	0	ø

		CNOP	0,4
coco		dc.l	0
		ds.l	256


******** Routine appelée par l'interruption du temps mort vertical ********
* Entrée	a5.l	=> Adr. 't'
extra_vbl:	rts
adresse.ilbm	dc.l	0

*>>>>>>>>>>>>>>>>>> Adresses utilisées par le programme <<<<<<<<<<<<<<<<<<*
		INCLUDE	"Mentat/Include/Systeme/t.s"
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
		dc.b	"$VER: Mentat0.3 (17.11.95)",0
		EVEN


garnet_offset:
	dc	08	' ' 00
	dc	08	' ' 01
	dc	08	' ' 02
	dc	08	' ' 03
	dc	08	' ' 04
	dc	08	' ' 05
	dc	08	' ' 06
	dc	08	' ' 07
	dc	08	' ' 08
	dc	08	' ' 09
	dc	08	' ' 10
	dc	08	' ' 11
	dc	08	' ' 12
	dc	08	' ' 13
	dc	08	' ' 14
	dc	08	' ' 15
	dc	08	' ' 16
	dc	08	' ' 17
	dc	08	' ' 18
	dc	08	' ' 19
	dc	08	' ' 20
	dc	08	' ' 21
	dc	08	' ' 22
	dc	08	' ' 23
	dc	08	' ' 24
	dc	08	' ' 25
	dc	08	' ' 26
	dc	08	' ' 27
	dc	08	' ' 28
	dc	08	' ' 29
	dc	08	' ' 30
	dc	08	' ' 31
	dc	08	' ' 32
	dc	05	'!' 33
	dc	09	'"' 34
	dc	12	'#' 35
	dc	11	'$' 36
	dc	12	'%' 37
	dc	13	'&' 38
	dc	05	"'" 39
	dc	08	'(' 40
	dc	08	')' 41
	dc	10	'*' 42
	dc	10	'+' 43
	dc	05	',' 44
	dc	08	'-' 45
	dc	07	'.' 46
	dc	12	'/' 47
	dc	09	'0' 48
	dc	05	'1' 49
	dc	09	'2' 50
	dc	08	'3' 51
	dc	10	'4' 52
	dc	09	'5' 53
	dc	09	'6' 54
	dc	09	'7' 55
	dc	09	'8' 56
	dc	09	'9' 57
	dc	06	':' 58
	dc	06	';' 59
	dc	07	'<' 60
	dc	08	'=' 61
	dc	07	'>' 62
	dc	09	'?' 63
	dc	17	'@" 64
	dc	16	'A' 65
	dc	14	'B' 66
	dc	10	'C' 67
	dc	14	'D' 68
	dc	10	'E' 69
	dc	14	'F' 70
	dc	11	'G' 71
	dc	15	'H' 72
	dc	08	'I' 73
	dc	09	'J' 74
	dc	17	'K' 75
	dc	14	'L' 76
	dc	19	'M' 77
	dc	15	'N' 78
	dc	12	'O' 79
	dc	14	'P' 80
	dc	12	'Q' 81
	dc	15	'R' 82
	dc	11	'S' 83
	dc	12	'T' 84
	dc	11	'U' 85
	dc	12	'V' 86
	dc	20	'W' 87
	dc	13	'X' 88
	dc	12	'Y' 89
	dc	10	'Z' 90
	dc	07	'[' 91
	dc	11	'\' 92
	dc	07	']' 93
	dc	08	'^' 94
	dc	10	'_' 95
	dc	06	'`' 96
	dc	10	'a' 97
	dc	09	'b' 98
	dc	09	'c' 99
	dc	11	'd'100
	dc	09	'e'101
	dc	07	'f'102
	dc	10	'g'103
	dc	10	'h'104
	dc	06	'i'105
	dc	06	'j'106
	dc	10	'k'107
	dc	06	'l'108
	dc	15	'm'109
	dc	10	'n'110
	dc	09	'o'111
	dc	09	'p'112
	dc	09	'q'113
	dc	09	'r'114
	dc	09	's'115
	dc	07	't'116
	dc	10	'u'117
	dc	10	'v'118
	dc	15	'w'119
	dc	10	'x'120
	dc	09	'y'121
	dc	10	'z'122
	dc	08	'{'123
	dc	08	'|'124
	dc	08	'}'125
	dc	11	'~'126
	dc	08	' '127
	dc	08	' '128
	dc	08	' '129
	dc	08	' '130
	dc	08	' '131
	dc	08	' '132
	dc	08	' '133
	dc	08	' '134
	dc	08	' '135
	dc	08	' '136
	dc	08	' '137
	dc	08	' '138
	dc	08	' '139
	dc	08	' '140
	dc	08	' '141
	dc	08	' '142
	dc	08	' '143
	dc	08	' '144
	dc	08	' '145
	dc	08	' '146
	dc	08	' '147
	dc	08	' '148
	dc	08	' '149
	dc	08	' '150
	dc	08	' '151
	dc	08	' '152
	dc	08	' '153
	dc	08	' '154
	dc	08	' '155
	dc	08	' '156
	dc	08	' '157
	dc	08	' '158
	dc	08	' '159
	dc	08	' '160
	dc	04	'¡'161
	dc	09	'¢'162
	dc	14	'£'163
	dc	10	'¤'164
	dc	09	'¥'165
	dc	05	'¦'166
	dc	08	'§'167
	dc	08	' '168
	dc	16	'©'169
	dc	09	'ª'170
	dc	08	' '171
	dc	08	' '172
	dc	08	' '173
	dc	19	'®'174
	dc	08	' '175
	dc	09	'°'176
	dc	08	' '177
	dc	08	' '178
	dc	08	' '179
	dc	08	' '180
	dc	13	'µ'181
	dc	08	' '182
	dc	08	' '183
	dc	08	' '184
	dc	08	' '185
	dc	08	' '186
	dc	08	' '187
	dc	08	' '188
	dc	08	' '189
	dc	08	' '190
	dc	08	' '191
	dc	08	' '192
	dc	08	' '193
	dc	08	' '194
	dc	08	' '195
	dc	08	' '196
	dc	08	' '197
	dc	08	' '198
	dc	08	' '199
	dc	08	' '200
	dc	08	' '201
	dc	08	' '202
	dc	08	' '203
	dc	08	' '204
	dc	08	' '205
	dc	08	' '206
	dc	08	' '207
	dc	08	' '208
	dc	08	' '209
	dc	08	' '210
	dc	08	' '211
	dc	08	' '212
	dc	08	' '213
	dc	08	' '214
	dc	08	' '215
	dc	08	' '216
	dc	08	' '217
	dc	08	' '218
	dc	08	' '219
	dc	08	' '220
	dc	08	' '221
	dc	08	' '222
	dc	08	' '223
	dc	09	'à'224
	dc	08	' '225
	dc	08	' '226
	dc	08	' '227
	dc	08	' '228
	dc	08	' '229
	dc	08	' '230
	dc	08	'ç'231
	dc	08	'è'232
	dc	08	'é'233
	dc	08	' '234
	dc	08	' '235
	dc	08	' '236
	dc	08	' '237
	dc	08	' '238
	dc	08	' '239
	dc	10	'ð'240
	dc	08	' '241
	dc	08	' '242
	dc	08	' '243
	dc	08	' '244
	dc	08	' '245
	dc	08	' '246
	dc	08	' '247
	dc	08	' '248
	dc	10	'ù'249
	dc	08	' '250
	dc	08	' '251
	dc	08	' '252
	dc	08	' '253
	dc	08	' '254
	dc	08	' '255

*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><*


