*+-----------------------------------------------------------------------+*
*=«+=================> Donne liste adresses mémoire <==================+»=*
*+-----------------------------------------------------------------------+*
memory_list:	move.l	(atable_clavier-t,a5),a0
		tst.b	(TOUCHE_TAB,a0)
		beq.s	.exit
		bsr	memlist_info
.exit		rts

***************************************************************************
******************** Donne informations sur la mémoire ********************
***************************************************************************
memlist_info:
		lea	(.num,pc),a0
		clr.w	(a0)
		lea	(.vbl,pc),a0
		move.l	a0,(mentat_routine-t,a5)
		move.w	#8,(mentat_nline-t,a5)
		bsr	.goprint
		lea	(.text,pc),a0
		bsr	mentat_info
		tst.l	d7
		rts

*---------------------* Routine appellée par la VBL *---------------------*
.vbl:
		bsr	.curx			|-> Affiche bande curseur
		move.l	(atable_clavier-t,a5),a0
		move.l	(mem_liste.adr-t,a5),a1
		move.l	(a1)+,d7		Nombre d'adresse(s) réservée(s)
		beq.s	.exit
		lea	(.posy,pc),a2
		move.w	(mentat_nline-t,a5),d6
		subq.w	#2,d6

		*/ Touche curseur bas \/ */
		lea	(TOUCHE_BAS,a0),a1
		bsr	test_clavier
		beq.s	.no_bas
		cmp.w	(a2)+,d6
		beq.s	.yoyo
		addq.w	#1,(-2.w,a2)
		bra.s	.exit
.yoyo		addq.w	#1,(a2)
		cmp.w	(a2),d7
		bne.s	.zob
		clr.w	(a2)
.zob		bsr	.goprint
		bra.s	.exit
.no_bas
		*/ Touche curseur haut /\ */
		lea	(TOUCHE_HAUT,a0),a1
		bsr	test_clavier
		beq.s	.no_haut
		tst.w	(a2)+
		beq.s	.fir
		subq.w	#1,(-2.w,a2)
		bra.s	.exit
.fir		subq.w	#1,(a2)
		bpl.s	.ket
		move.w	d7,(a2)
		subq.w	#1,(a2)
.ket		bsr	.goprint
.no_haut
.exit		rts


*--------------------*  Modifie le text à afficher  *---------------------*
.goprint:
		move.l	a6,-(a7)
		move.l	4.w,a4			a4=ExecBase
		move.l	(62,a4),a4		a4=Adr. maximum CHIP-MEMORY
		lea	(.text2,pc),a3
		move.l	(mem_liste.adr-t,a5),a2
		move.l	(a2)+,d6	Nombre d'adresse(s) réservée(s)
		beq	.stop
		move.w	(mentat_nline-t,a5),d7
		subq.w	#2,d7
		moveq	#0,d5
		move.w	(.num,pc),d5
		move.l	d5,d0
		lsl.w	#4,d0
		add.l	d0,a2
.loop_mem	
		*/ Test la fin de la liste */
		cmp.w	d5,d6			adresse nulle ?
		bne.s	.noend			-> non
		move.l	(mem_liste.adr-t,a5),a2
		addq.l	#4,a2
		moveq	#0,d5
.noend
		*/ Numéro de la ligne */
		move.l	d5,d2
		bsr	convertit_decimal
		move.l	a3,a0
		moveq	#2,d1
		bsr	conv_number_ascii

		*/ Adresse mémoire */
		lea	(6.w,a3),a0
		move.l	(a2)+,d3
		moveq	#7,d1
		bsr	conv_number_ascii

		*/ Taille mémoire */
		move.l	(a2)+,d2
		bsr	convertit_decimal
		lea	(17.w,a3),a0
		moveq	#7,d1
		bsr	conv_number_ascii

		*/ Pointeur */
		lea	(30.w,a3),a0
		move.l	(a2),d3
		beq.s	.pt0
		move.l	d3,a1
		move.l	(12,a1),d0		d0=Type 0/2/4
		beq.s	.pub
		subq.w	#2,d0
		beq.s	.chip
		subq.w	#2,d0
		bne.s	.pt0
.fast		move.l	#'FAST',d1
		bra.s	.tt
.chip		move.l	#'CHIP',d1
		bra.s	.tt
.pub		move.l	#'PUBL',d1
		bra.s	.tt
.pt0		move.l	(-8.w,a2),d0		d0=adresse
		move.l	#'CHIP',d1
		cmp.l	d0,a4	
		bhi.s	.tt			-> oui
		move.l	#'FAST',d1
.tt		move.l	d1,(76.w,a3)
		moveq	#7,d1
		bsr	conv_number_ascii

		*/ Name Pointeur  */
		lea	(40.w,a3),a6
		move.l	a6,a0
		move.l	#'....',d0
		move.l	d0,(a0)+
		move.l	d0,(a0)+
		move.l	d0,(a0)+
		move.l	d0,(a0)+
		move.l	d0,(a0)+
		move.l	d0,(a0)+
		move.l	(a2)+,d3		d3=adresse pointeur
		beq.s	.ptok			-> nulle
		lea	fin_prg-t,a1
		add.l	a5,a1
		move.l	(a1)+,d1		d1=taille chunk
		tst.l	(a1)+
		bne.s	.ptok
		cmp.l	#$3F0,(a1)+
		bne.s	.ptok
		sub.l	#12,d1			-identificateur
.voto		move.l	a6,a0
		move.l	(a1)+,d0		d0=taille label en longs mots
		subq.w	#1,d0
.ccxx		move.l	(a1)+,(a0)+
		subq.l	#4,d1
		dbf	d0,.ccxx
		cmp.l	(a1)+,d3
		beq.s	.ptok
		subq.l	#8,d1
		bpl.s	.voto
.ptok
		*/ Identificateur */
		move.l	(a2)+,(68.w,a3)
		lea	(.text2-.text,a3),a3
		addq.l	#1,d5
		dbf	d7,.loop_mem
.stop		move.l	(a7)+,a6
		rts

*-----------------------* Afiche la bande curseur *-----------------------*
.curx:
		lea	(.xxx,pc),a0
		subq.w	#1,(a0)
		bne.s	.nonul
		move.w	#50,(a0)
.nonul		cmp.w	#30,(a0)
		bhi	.x8x
		move.l	(mentat_screen-t,a5),a0
		move.w	(.posy,pc),d0
		addq.w	#1,d0
		mulu	#MT.SLINE*8,d0
		add.l	d0,a0
		move.w	#(MT.SLINE*8)/4-1,d0
.loopxx		not.l	(a0)+
		dbf	d0,.loopxx
.x8x		rts



		CNOP	0,4
.posy		dc.w	0	^
.num		dc.w	0	!
.xxx		dc.w	10

.text		dc.b "Num.  Address    Size        Pointeur   Pointeur Name               Id.     Type"
.text2		dc.b "000 -$00000000- »00000000« >$00000000< (????????????????????????)  {----}   ----"
		dc.b "000 -$00000000- »00000000« >$00000000< (????????????????????????)  {----}   ----"
		dc.b "000 -$00000000- »00000000« >$00000000< (????????????????????????)  {----}   ----"
		dc.b "000 -$00000000- »00000000« >$00000000< (????????????????????????)  {----}   ----"
		dc.b "000 -$00000000- »00000000« >$00000000< (????????????????????????)  {----}   ----"
		dc.b "000 -$00000000- »00000000« >$00000000< (????????????????????????)  {----}   ----"
		dc.b "000 -$00000000- »00000000« >$00000000< (????????????????????????)  {----}   ----"
		EVEN

