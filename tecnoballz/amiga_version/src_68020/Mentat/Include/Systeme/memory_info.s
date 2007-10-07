*+-----------------------------------------------------------------------+*
*=«+==================> Donne infos sur la mémoire <===================+»=*
*+-----------------------------------------------------------------------+*
memory_help:	move.l	(atable_clavier-t,a5),a0
		tst.b	(TOUCHE_HELP,a0)
		beq.s	.exit
		bsr	memory_info
.exit		rts

***************************************************************************
******************** Donne informations sur la mémoire ********************
***************************************************************************
memory_info:
		bsr	.free_memory
		bsr	.conv_free
		bsr	.mem_alloc
		bsr	.conv_alloc
		bsr	.conv_adr
		bsr	.zone
		bsr	.envoie
		rts

*----------------* Demande la taille de la mémoire libre *-----------------*
.free_memory:
		movem.l	d0-d7/a0-a6,-(a7)
		moveq	#2,d1			Mémoire Chip
		move.l	4.w,a6
		jsr	(AvailMem,a6)
		lea	(.mem_chip,pc),a0
		move.l	d0,(a0)			Sauve taille Chip
		moveq	#4,d1			Mémoire Fast
		jsr	(AvailMem,a6)
		lea	(.mem_fast,pc),a0
		move.l	d0,(a0)			Sauve taille Fast
		movem.l	(a7)+,d0-d7/a0-a6
		rts

*----------* Convertit en caractères aff. de la mémoire libre *-----------*
.conv_free:
		move.l	(.mem_chip,pc),d2
		lea	(.text+80+27,pc),a0
		bsr	.conv
		move.l	(.mem_fast,pc),d2
		lea	(.text+80+43,pc),a0
		bsr	.conv
		move.l	(.mem_chip,pc),d2
		add.l	(.mem_fast,pc),d2
		lea	(.text+80+67,pc),a0
		bsr	.conv
		rts

*---------------------------* Convertit ASCII *---------------------------*
* entrée	=> d2.l	taille mémoire en octet
*		=> a0.l	pointeur sur la chaine ASCII
.conv:
		lsr.l	#8,d2			Divivise par 1024
		lsr.l	#2,d2			Convertit octet en Kilo octet	
		move.l	a0,-(a7)
		bsr	convertit_decimal
		move.l	(a7)+,a0
		moveq	#4-1,d1
		bsr	conv_number_ascii
		rts

*------------------------* Calcul mémoire prise *-------------------------*
* entrée	=> a5.l	adresse de la table 't'
.mem_alloc:
		moveq	#0,d1
		moveq	#0,d2
		move.l	(mem_liste.adr-t,a5),a0
		move.l	(a0)+,d0		Nombre d'adresse(s) réservée(s)
		subq.w	#1,d0
		move.l	4.w,a1			a1=ExecBase
		move.l	(62,a1),d3		d3=Adr. maximum CHIP-MEMORY
.loop_mem	cmp.l	(a0)+,d3		mémoire CHIP ?
		bhi.s	.chip			-> oui
		add.l	(a0)+,d2		Inc. compteur mém. Fast
		bra.s	.next
.chip		add.l	(a0)+,d1		Inc. compteur mém. Chip
.next		addq.l	#8,a0
		dbf	d0,.loop_mem
		lea	(.alloc_chip,pc),a0
		move.l	d1,(a0)+
		move.l	d2,(a0)+
		move.l	#fin_prg-debut_prg,(a0)
		rts

*----------» Convertit en caractères aff. de la mémoire libre «-----------*
.conv_alloc:
		move.l	(.alloc_chip,pc),d2
		lea	(.text+160+27,pc),a0
		bsr	.conv
		move.l	(.alloc_fast,pc),d2
		lea	(.text+160+43,pc),a0
		bsr	.conv
		move.l	(.alloc_prg,pc),d2
		lea	(.text+160+58,pc),a0
		bsr	.conv
		move.l	(.alloc_chip,pc),d2
		add.l	(.alloc_fast,pc),d2
		add.l	(.alloc_prg,pc),d2
		lea	(.text+160+67,pc),a0
		bsr	.conv
		rts

*---------------------» Convertit adresse de départ «---------------------*
* entrée	=> a5.l	Adr. table 't'
.conv_adr:
		move.l	#debut_prg-t,d3
		add.l	a5,d3
		moveq	#8-1,d1
		lea	(.text+240+65,pc),a0
		bsr	conv_number_ascii
		rts
*-------------------» Donne le nombre de zone mémoire «-------------------*
.zone
		move.l	(mem_liste.adr-t,a5),a0
		move.l	(a0),d2			Nombre d'adresse(s) réservée(s)
		bsr	convertit_decimal
		lea	(.zz+15,pc),a0
		moveq	#2-1,d1
		bra	conv_number_ascii

*--------------------------» Envoie le message «--------------------------*
.envoie:
		lea	(.text,pc),a0
		move.w	#5,(mentat_nline-t,a5)
		clr.l	(mentat_routine-t,a5)
		bsr	mentat_info
		tst.l	d7
		rts
.alloc_chip	dc.l	0
.alloc_fast	dc.l	0
.alloc_prg	dc.l	0
.mem_chip	dc.l	0
.mem_fast	dc.l	0
.text:		dc.b "                             MENTAT INFORMATIONS                                "
		dc.b "       MEM FREE  => CHIP : 0000 K + FAST : 0000 K                = 0000 K       "
		dc.b "       MEM ALLOC => CHIP : 0000 K + FAST : 0000 K + PRG : 0000 K = 0000 K       "
.zz		dc.b "       ZONES : 00                             PRG START ADDRESS $00000000       "
		dc.b "                            PRESS LEFT MOUSE BUTTON                             "
		EVEN
