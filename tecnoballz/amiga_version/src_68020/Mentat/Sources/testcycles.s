*-------------------------------------------------------------------------*
*	Test de cycles machine programmation très hard des chips AGA
*
*	Vive Casimir et l'îles aux enfants
*
*	© 1993 TROUPIGNOFF Pére & Fils
*-------------------------------------------------------------------------*
		SECTION	SIDONY,CODE
		OPT	P+		Pas de table de relogement
		OPT	C20+		Labels et symboles 20 caractères maxi.,  majuscules/minuscules respectées
		OPT	O-		Pas d'optimisation
		OPT	p=68020	
		INCDIR	dh0:Data/

*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONSTANTES <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*
RASTERF		=1	
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


RASTER		MACRO	lcolor
		tst.b	(raster.f-t,a5)
		beq.s	.nocolors\@
		move.l	(pt_screen-t,a5),a0
		move.w	(SBPLCON3,a0),(BPLCON3,a6)
		move.w	\1,(COLOR00,a6)
.nocolors\@
		ENDM

*>>>>>>>>>>>>>>>>>>>>>>>>>> Debut code MC680x0 <<<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	
		INCLUDE	"Mentat/Include/mentat.s"

************* Routine appelée avant de quitter le multitâche **************
sys_extra:	
		lea	(name.ilbm,pc),a0
		moveq	#0,d0
		bsr	charge_fichier_dos
		bne.s	.exit
		move.l	d0,(adresse.ilbm-t,a5)
		moveq	#0,d7
.exit		rts
name.ilbm	dc.b	"DH0:DATA/ILBM/Planete1.ilbm",0
		EVEN

*>>>>>>>>>>>>>>>>>>>>>>>>>> début du programme <<<<<<<<<<<<<<<<<<<<<<<<<<<*
run:	
		bsr	first_init
		bsr	sprites_off
		move.l	(adresse.ilbm-t,a5),d0
		moveq	#2,d1			2 buffers
		moveq	#0,d2			pas de défilement possible
		bsr	affiche_ilbm

		move.l	#40*256*8,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(fastmemory-t,a5)
		beq	sys_fin
		bsr	colors_on

*¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ BOUCLE PRINCIPALE HORS INTERRUPTION ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤*
main_loop:
		bsr	wait_vbl

		* Attend la 50ieme ligne raster *
		RASTER	#$00A
		lea	(VPOSR,a6),a1
		lea	(VHPOSR,a6),a0
		moveq	#50,d0
.wait		moveq	#0,d1
		move.w	(a1),d1
		and.w	#$0001,d1
		lsr.w	#8,d1
		add.b	(a0),d1
		cmp.w	d1,d0
		bhi.s	.wait

		* copie au blitter *
		RASTER	#$FFF
		bsr	blitter_ok

		* Copie au MC680x0 *
		* RASTER	#$F00
		* bsr	ok680x0

		RASTER	#$000
		btst	#6,$bfe001
		bne.s	main_loop
		bra	sys_fin


********************* Recopie écran simple au Blitter *********************
blitter_ok:
		move.l	(pt_screen-t,a5),a0
		move.l	(SPECRAN2,a0),d0
		move.l	(SPECRAN1,a0),d1
.wait		btst	#14,(DMACONR,a6)
		bne.s	.wait
		*move.b	#$ff,(BLTCON0L,a6)

		move.l	#-1,(BLTAFWM,a6)
		move	#0,(BLTAMOD,a6)
		move	#0,(BLTDMOD,a6)

		move.l	#$090f0000,(BLTCON0,a6)
	
		move.b	#$f0,(BLTCON0L,a6)

		*move	#$0080,(BLTCON1,a6)


		move.l	d0,(BLTAPTH,a6)
		move.l	d1,(BLTDPTH,a6)
		move	#40,(BLTSIZV,a6)
		move	#64*8,(BLTSIZH,a6)
.wait2		btst	#14,(DMACONR,a6)
		bne.s	.wait2
		rts

********************* Recopie écran simple au MC680x0 *********************
ok680x0:
		movem.l	a5/a6,-(a7)
		lea	(.pile,pc),a0
		move.l	a7,(a0)
		move.l	(pt_screen-t,a5),a0
		move.l	(SPECRAN1,a0),a7
		move.l	(SPECRAN2,a0),a6
		*move.l	(fastmemory-t,a5),d6

		* 14 registres de 32 bits = 56 octets d'un coup (x50=2800 octets) *
		moveq	#16-1,d0			53200 octets
.loop		lea	(.loop_save,pc),a0
		move.w	d0,(a0)
		REPT	50
		movem.l	(a6)+,d0-d7/a0-a5
		move.l	d0,(a7)+
		move.l	d1,(a7)+
		move.l	d2,(a7)+
		move.l	d3,(a7)+
		move.l	d4,(a7)+
		move.l	d5,(a7)+
		move.l	d6,(a7)+
		move.l	d7,(a7)+
		move.l	a0,(a7)+
		move.l	a1,(a7)+
		move.l	a2,(a7)+
		move.l	a3,(a7)+
		move.l	a4,(a7)+
		move.l	a5,(a7)+
		ENDR
		move.w	(.loop_save,pc),d0
		dbf	d0,.loop
		move.l	(.pile,pc),a7
		movem.l	(a7)+,a5/a6
		rts
		CNOP	0,4
.pile		dc.l	0
.loop_save	dc.w	0

******** Routine appelée par l'interruption du temps mort vertical ********
* Entrée	=> a5.l	Adr. 't'
extra_vbl:	
		rts

*>>>>>>>>>>>>>>>>>* Adresses utilisées par le programme *<<<<<<<<<<<<<<<<<*
		CNOP	0,4
adresse.ilbm	dc.l	0
fastmemory	dc.l	0
raster.f	dc.b	RASTERF
		EVEN
		INCLUDE	"Mentat/Include/Systeme/t.s"
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
		dc.b	"$VER: Mentat-Test-Cycles 0.2 (09.08.93)",0
		EVEN

*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*


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
