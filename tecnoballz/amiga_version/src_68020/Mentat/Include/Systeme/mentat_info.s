*«««»»»«««»»»«««»»»«««»»»«««»»»«««»»»«««»»»«««»»»«««»»»«««»»»«««»»»«««»»»«*
******************** AFFICHE UNE INFORMATION A L'ECRAN ********************
*«««»»»«««»»»«««»»»«««»»»«««»»»«««»»»«««»»»«««»»»«««»»»«««»»»«««»»»«««»»»«*
* Entrée	=> a0.l	Adr. texte à afficher 5 lignes de 80 caractères
*		=> a5.l	Adr. table 't'
*		=> a6.l	CUSTOM
* Sortie	<= d0.w	#0=bouton gauche / #1=bouton droit
mentat_info:
		lea	(.text,pc),a1
		move.l	a0,(a1)			Sauve adr. texte
		lea	(mentat_screen-t,a5),a4
		bsr	reserve.mem
		bne.s	.exit
		move.w	#$07F0,(DMACON,a6)
		bsr	sprites_off2
		bsr	.change_irq3
.wait_mouse	moveq	#0,d0			d0=0=bouton gauche
		tst.b	(mouse0_left-t,a5)
		bne.s	.left_button
		tst.b	(mouse0_right-t,a5)
		beq.s	.wait_mouse
		moveq	#1,d0			d0=1=bouton droit
.left_button	move.w	d0,-(a7)
		bsr	.restore_irq3
		lea	(mentat_screen-t,a5),a0
		bsr	libere_mem
		tst.l	(pt_screen-t,a5)
		beq.s	.noscreen
		move.l	(pt_screen-t,a5),a0
		move.w	(SBPLCON3,a0),(BPLCON3,a6)
		bsr	set_colors
.noscreen	move.w	(dmacon-t,a5),(DMACON,a6)
		move.w	(a7)+,d0
		clr.l	(mentat_routine-t,a5)
.exit		rts

*----------------------> Affiche le text à l'écran <----------------------*
* entrée	=> a5.l	adr. table 't'
.print_text:
		move.l	(.text,pc),a0
		move.l	(mentat_screen-t,a5),a1
		move.l	(adr_fonteraw-t,a5),d0
		moveq	#0,d1
		move.w	(mentat_nline-t,a5),d2
		subq.w	#1,d2
.aff_loop	moveq	#80-1,d3
.loop		move.b	(a0)+,d1
		move.l	d0,a2
		add.w	d1,a2
		move.b	(a2),(a1)
		move.b	(1*FT.SLINE,a2),(1*MT.SLINE,a1)
		move.b	(2*FT.SLINE,a2),(2*MT.SLINE,a1)
		move.b	(3*FT.SLINE,a2),(3*MT.SLINE,a1)
		move.b	(4*FT.SLINE,a2),(4*MT.SLINE,a1)
		move.b	(5*FT.SLINE,a2),(5*MT.SLINE,a1)
		move.b	(6*FT.SLINE,a2),(6*MT.SLINE,a1)
		move.b	(7*FT.SLINE,a2),(7*MT.SLINE,a1)
		addq.l	#1,a1
		dbf	d3,.loop
		lea	(MT.SLINE*8-80,a1),a1
		dbf	d2,.aff_loop
		rts
.text		dc.l	0

*------------------> Change l'interruption de niveau 3 <------------------*
* entrée	=> a5.l 't'
*		=> a6.l	CUSTOM
.change_irq3:
		move.w	#$0020,(INTENA,a6)
		lea	(.save_irq3,pc),a0
		lea	$6C.w,a1
		add.l	(base_vecteurs-t,a5),a1
		move.l	(a1),(a0)
		lea	(.irq3,pc),a0
		move.l	a0,(a1)
		move	#$C020,(INTENA,a6)
		rts
.save_irq3	dc.l	0

*--------------> Interruption VBL du "MENTAT INFORMATION" <---------------*
.irq3:
		movem.l	d0-d7/a0-a6,-(a7)

		* Pointe l'écran de 640*40 en 2 couleurs *
		move.l	(mentat_screen-t,a5),(BPL1PTH,a6)
		move.w	#$9201,(BPLCON0,a6)	haute résolution 1 plan
		moveq	#0,d0
		move.w	#$000F,(FMODE,a6)
		move.w	d0,(BPLCON1,a6)
		move.w	d0,(BPLCON2,a6)
		move.w	d0,(BPL1MOD,a6)
		move.w	d0,(BPL2MOD,a6)
		move.w	#$0020,(BPLCON3,a6)
		move.w	#$0006,(COLOR00,a6)

		move.w	(mentat_nline-t,a5),d2
		lsl.w	#3,d2
		move.w	#256,d0
		sub.w	d2,d0
		lsr.w	#1,d0
		add.w	#20,d0
		add.w	d0,d2
		lsl.w	#8,d0
		lsl.w	#8,d2
		or.w	#$81,d0
		or.w	#$C1,d2
		move.w	d0,(DIWSTRT,a6)
		move.w	d2,(DIWSTOP,a6)

		*move.w	#$8081,(DIWSTRT,a6)
		*move.w	#$A8C1,(DIWSTOP,a6)
		move.w	#$0038,(DDFSTRT,a6)
		move.w	#$00C0,(DDFSTOP,a6)
		move.w	#$8300,(DMACON,a6)

		*> VBL : cycle la couleur 1 <*
		moveq	#0,d2
		lea	(.color,pc),a0
		move.w	(a0)+,d2		d1=incrément -1 ou 1
		add.w	d2,(a0)
		move.w	(a0),d2			d2=valeur du rouge
		cmp.w	#$32,d2			plus petit valeur ?
		bne.s	.nomin
		move.w	#+5,(-2,a0)
.nomin		cmp.w	#$FF,d2			plus grande valeur ?
		bne.s	.nomax
		move.w	#-5,(-2,a0)
.nomax		lsl.l	#8,d2
		lsl.l	#8,d2
		or.l	#$00A700,d2
		bsr	aga_color
		move.w	#$0220,(BPLCON3,a6)	Bits 0 à 3 RVB
		move.w	d1,(COLOR01,a6)
		move.w	#$0020,(BPLCON3,a6)	Bits 4 à 7 RVB
		move.w	d0,(COLOR01,a6)

		*> VBL : test les boutons de la souris <*
		bsr	vbl_mouse_buttons

		*> VBL : affiche le text <*
		bsr	.print_text

		*> VBL : appel la routine <*
		move.l	(mentat_routine-t,a5),d0
		beq.s	.noroutine
		move.l	d0,a0
		jsr	(a0)
.noroutine
		*> quitte la VBL <*
		movem.l	(a7)+,d0-d7/a0-a6
		move.w	#$0020,CUSTOM+INTREQ
		rte
.color		dc.w	-5		incrément
		dc.w	$FF		Valeur du rouge

*------------------> Change l'interruption de niveau 3 <------------------*
* entrée	=> a5.l 't'
*		=> a6.l	CUSTOM
.restore_irq3:
		move.w	#$0020,(INTENA,a6)
		lea	$6C.w,a1
		add.l	(base_vecteurs-t,a5),a1
		move.l	(.save_irq3,pc),(a1)
		move.w	#$C020,(INTENA,a6)
		rts
