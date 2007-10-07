*//*-*-*-*-*-*-*-*-*-*---*************************---*-*-*-*-*-*-*-*-*-*\\*
*\\********************||* INITIALISATION UNIQUE *||********************//*
*//*-*-*-*-*-*-*-*-*-*---*************************---*-*-*-*-*-*-*-*-*-*\\*
first_init:	
		lea	(first_init,pc),a5
		add.l	#t-first_init,a5
		lea	CUSTOM,a6
		move.w	#$07ff,(DMACON,a6)
		move.w	#$7fff,(INTENA,a6)
		move.w	#1,(flag_play-t,a5)	interdit saut à la PlayRoutine
		bsr.s	read_freq
		bsr.s	change_vecteur2
		bsr	change_vecteur3
		bsr	init_cia
		bsr	init_table_t
		bne.s	.exit
		bsr	raz_tableclavier
		bsr	init_intena
		bsr	init_copper
		bsr	init_dmacon
		moveq	#0,d7
.exit		rts
*
*--------------> First init : lit la fréquence de balayage <--------------*
* entrée	=> a5.l	adresse 't'
read_freq:
		tst.w	(frequence-t,a5)
		bne.s	.dejafreq
		move.l	4.w,a0			a0=ExecBase
		moveq	#0,d0
		move.b	(530,a0),d0		d0=VBlankFrequency
		move.w	d0,(frequence-t,a5)
		rts
.dejafreq	moveq	#$0020,d0
		cmp.w	#50,(frequence-t,a5)
		beq.s	.pal
		moveq	#0,d0
.pal		move.w	d0,(BEAMCON0,a6)
		rts
*
*--------> First Init : Change vecteur interruption de niveau 2 <---------*
* entrée	=> a5.l	adresse 't'
change_vecteur2:
		move.l	(base_vecteurs-t,a5),d1
		lea	interruption_clavier-t,a0
		add.l	a5,a0
		lea	$68.w,a1
		add.l	d1,a1
		move.l	a0,(a1)
		rts
*
*--------> First Init : Change vecteur interruption de niveau 3 <---------*
* entrée	=> a5.l	adresse 't'
change_vecteur3:
		move.l	(base_vecteurs-t,a5),d1
		lea	interruption_master-t,a0
		add.l	a5,a0
		lea	$6c.w,a1
		add.l	d1,a1
		move.l	a0,(a1)
		rts
*
*---------------------> First Init : Init. les CIAA <---------------------*
init_cia:
		bsr.s	.ciaa
		bsr.s	.ciab
		rts

		* CIA-A *
.ciaa		bset	#1,CIAA_PRA		éteind la "LED POWER" = filtre
		*move.b	#$7f,CIAA_ICR		vire demande d'IRQ
		move.b	#$88,CIAA_ICR		autorise IRQ serial port
		rts

		* CIA-B *
.ciab		move.b	#$7f,CIAB_ICR		vire demandes d'irq
		move.b	#$08,CIAB_CRA		one-shot-mode activé (arrête minuterie après chaque décompte et met le bit START à 0)
		move.b	#$80,CIAB_TALO		init. TIMER A, (PlayRoutine)
		move.b	#$01,CIAB_TAHI		$180 = 384 cycles
		moveq	#-1,d0
.loop		btst	#0,CIAB_ICR
		dbf	d0,.loop
		move.b	#$81,CIAB_ICR		autorise irq minuterie A
		rts

*-------> Fisrt Init : calcul quelques adresses de la table 't' <-------*
* entrée	=> a5.l	adresse 't'
init_table_t:		
		*/ Réserve la table clavier */
		lea	(tableclavier.pt-t,a5),a4
		bsr	reserve.mem
		bne.s	.exit
		lea	2+ki.adr-t,a0
		add.l	a5,a0
		move.l	(tableclavier.pt-t,a5),d0
		addq.l	#4,d0
		move.l	d0,(a0)
		move.l	d0,(atable_clavier-t,a5)

		move.l	a5,d0
		lea	(adr_fonteraw-t,a5),a0
		move.l	#fonteraw-t,(a0)
		add.l	d0,(a0)
		lea	(adr_resmem-t,a5),a0
		move.l	#reserve_memoire-t,(a0)
		add.l	d0,(a0)
		moveq	#0,d7
.exit		rts

*------------------> First Init : RAZ la table clavier <------------------*
* entrée	=> a5.l	adresse 't'
raz_tableclavier:
		move.l	(atable_clavier-t,a5),a0
		subq.l	#4,a0
		moveq	#0,d0
		moveq	#132/4-1,d1
.loop		move.l	d0,(a0)+
		dbf	d1,.loop
		rts

*---------------> First Init : Autorise les interruptions <---------------*
* entrée	=> a6.l CUSTOM
init_intena:
		move.w	#$C028,d0
		lea	(INTENA,a6),a0
		move.w	d0,(a0)
		rts

*----------------------> First Init : Init. Copper <----------------------*
* entrée	=> a5.l	adresse 't'
*		=> a6.l CUSTOM
init_copper:
		move.l	(acopper-t,a5),d0
		move.l	d0,(COP1LCH,a6)
		moveq	#0,d0
		move.w	d0,(COPJMP1,a6)
		rts
*
*----------------------> First Init : Init. DMACON <----------------------*
* entrée	=> a5.l	adresse 't'
*		=> a6.l CUSTOM
init_dmacon:
		lea	(DMACON,a6),a0
		move.w	(dmacon-t,a5),d0
		move.w	d0,(a0)
		rts
