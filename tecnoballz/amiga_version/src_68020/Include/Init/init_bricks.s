*--=X=--=X=--=X=--=X=--=X=--=X=--=X=--=X=--=X=--=X=--=X=--=X=--=X=--=X=--=*
*                          Affiche les briques 				  *
*--=X=--=X=--=X=--=X=--=X=--=X=--=X=--=X=--=X=--=X=--=X=--=X=--=X=--=X=--=*
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l CUSTOM
init_bricks:
		clr.w	(brick.save-t,a5)
		clr.w	(brick.clr-t,a5)
		move.l	#BRI.SIZE*BNP,d0
		lea	(bri_pnt-t,a5),a0
		bsr.l	clear.memory
		bsr	init.setxy
		bsr	init.set_table
		bsr	select.bricks
		bne.s	.exit
		bsr	select.tableau
		bne.s	.exit
		bsr	affiche.obricks
		bsr	affiche.bricks
		bsr	init.set_table2
		bsr	bricks.conv
		moveq	#0,d7
.exit		rts

*/*************/* Calcul table pointeurs sur 'set_table' */**************/*
* Entrée	=> a5.l	adr. 't'.
init.setxy:
		lea	(set.x-t,a5),a0
		moveq	#0,d2			X=0
		move.w	#BK.X*16-1,d1
.loop1		move.w	d2,d0
		lsr.w	#4,d0
		add.w	d0,d0
		add.w	d0,d0
		move.w	d0,(a0)+
		addq.w	#1,d2			X=X+1
		dbf	d1,.loop1
		lea	(set.y-t,a5),a0
		moveq	#0,d2			Y=0
		move.w	#BK.Y*8-1,d0
.loop2		move.w	d2,d1
		lsr.w	#3,d1
		mulu	#BK.X*4,d1
		move.w	d1,(a0)+
		addq.w	#1,d2			Y=Y+1
		dbf	d0,.loop2
		rts


*--------------------*/ Calcul la table 'set_table' /*--------------------*
* Entrée	=> a5.l	adr. 't'.
init.set_table:
		lea	(set_table-t,a5),a0
		moveq	#0,d2
		moveq	#BK.Y-1,d0
.loop_y0	moveq	#BK.X-1,d1
		move.w	d2,d3
.loop_x0	move.w	d3,(a0)+
		clr.w	(a0)+
		addq.w	#1,d3
		dbf	d1,.loop_x0
		add.w	#BK.X,d2
		dbf	d0,.loop_y0
		rts

*--------------------------* Seléctionne briques -------------------------*
* Entrée	=> a5.l	adr. 't'.
select.bricks:
		*/ Réserve page graphique pour les briques */
		lea	(bk.pagegfx-t,a5),a4
		move.l	#BK.SIZE+PG_DATA,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		bne	.exit
		move.l	(RESA.ADR,a4),a4
		move.l	#'PAGE',(PG_ID,a4)
		move.w	#BK.SPLAN,(PG_SPLAN,a4)
		move.w	#BK.NLINE,(PG_HEIGHT,a4)
		move.w	#BK.SLINE,(PG_SLINE,a4)
		move.w	#BK.NPLAN,(PG_NPLAN,a4)
		move.l	#BK.SIZE,(PG_SIZE,a4)

		*/ Décompacte la page graphique */
		sub.l	a2,a2
		lea	(brick.pp-t,a5),a3
		lea	(brick.pagegfx-t,a5),a4
		bsr.l	decode.pagepp
		bne	.exit

		*/ Init. */
		move.l	(brick.pagegfx-t,a5),a3
		move.l	(bk.pagegfx-t,a5),a4
		move.w	(PG_NPLAN,a4),d1
		mulu	(PG_HEIGHT,a4),d1
		lsl.w	#6,d1
		move.w	(PG_SPLAN,a4),d0
		lsr.w	#1,d0
		add.w	d0,d1			d1=fenêtre Blitter
		moveq	#0,d0
		move.w	(PG_SPLAN,a3),d0
		sub.w	(PG_SPLAN,a4),d0
		swap	d0			d0=Modulos
		move.w	(avi-t,a5),d2		n° des briques à utiliser
		tst.w	(rout-t,a5)		Editeur ?
		bne.	.nomax10		-> oui
		move.w	(hasard-t,a5),d2
		and.w	#$F,d2
		cmp.w	#10,d2
		blt.s	.nomax10
		moveq	#9,d2
.nomax10	btst	#0,d2
		beq.s	.olala
		move.w	#BK.NLINE,d3
		mulu	(PG_SLINE,a3),d3
		add.l	d3,a3
.olala		lsr.w	#1,d2
		mulu	#BK.SPLAN,d2
		add.l	d2,a3
		lea	(PG_DATA,a3),a3
		lea	(PG_DATA,a4),a4

		*/ Recopie la page brique */
.wait3		btst	#14,(DMACONR,a6)
		bne.s	.wait3
		move.l	#$09F00000,(BLTCON0,a6)
		move.l	#-1,(BLTAFWM,a6)
		move.l	d0,(BLTAMOD,a6)
		move.l	a3,(BLTAPTH,a6)
		move.l	a4,(BLTDPTH,a6)
		move.w	d1,(BLTSIZE,a6)

		*/ Libère la page graphique */
		lea	(brick.pagegfx-t,a5),a0
		bsr.l	libere_mem
.exit		rts

*--------------------------* Décode le tableau *--------------------------*
* Entrée	=> a5.l	adr. 't'.
select.tableau:

		*/ Décompacte les 100 tableaux */
		lea	(tablo.pp-t,a5),a4
		move.l	(RESA.ADR,a4),d0
		move.l	(RESA.SIZ,a4),d1
		lea	(tableaux-t,a5),a4
		bsr.l	decrunch.pp
		bne.s	.exit

		*/ Pointe sur un des 100 tableaux */
		move.l	(tableaux-t,a5),a0
		tst.w	(newtab-t,a5)		tableaux créés
		beq.s	.ok7			-> non
		tst.l	(pt_tableaux-t,a5)
		beq.s	.ok7
		move.l	(pt_tableaux-t,a5),a0
.ok7		moveq	#0,d0
		move.w	(niv00-t,a5),d0
		move.w	(area-t,a5),d1		n° area de 1 à 5
		subq.w	#1,d1			n° area = n° area - 1
		mulu	#TB.SIZE*10,d1
		add.w	d1,d0
		tst.w	(newtab-t,a5)		tableaux créés
		bne.s	.jmmds			-> oui
		move.w	(hasard-t,a5),d1
		and.w	#$0001,d1
		beq.s	.jmmds
		add.w	#TB.SIZE*50,d0
.jmmds		add.l	d0,a0
		bsr	tab2settable

		*/ Libère mémoire des 100 tableaux */
		lea	(tableaux-t,a5),a0
		bsr.l	libere_mem
.exit		rts

*-------------------------------------------------------------------------*
******************/ Recopie le tableau dans "set_table" /******************
*-------------------------------------------------------------------------*
* Entrée	=> a0.l	pt/tableau
*		=> a5.l	adr. 't'.
tab2settable:
		lea	(set_table-t,a5),a1
		lea	(7*RIZ,a1),a1
		move.l	(bk.pagegfx-t,a5),a4
		move.w	(PG_SLINE,a4),d4
		mulu	#BK.HEIGHT,d4
		moveq	#TB.Y-1,d1
.copy_loop	lea	(12,a1),a1
		moveq	#TB.X-1,d0
.copy_loop2	move.w	(a0)+,d2		d2=code brique y,x
		beq.s	.nul			-> pas de brique
		move.w	d2,d3
		lsr.w	#8,d3		
		move.w	d3,(2+JMM,a1)
		mulu	d4,d3			*hauteur d'une ligne
		and.w	#$00FF,d2
		move.w	d2,(JMM,a1)
		add.w	d3,d2
.nul		move.w	d2,(2,a1)
		addq.l	#4,a1
		dbf	d0,.copy_loop2
		lea	(12,a1),a1
		dbf	d1,.copy_loop
		rts

*-------------------------* Affiche les ombres *------------------------*
affiche.obricks:
		lea	(2+set_table-t,a5),a2
		moveq	#BK.POSX-ODECA,d4
		moveq	#BK.POSY+ODECA,d5
		moveq	#BK.Y-1,d6
.loop_y		move.w	d4,d3
		swap	d6
		move.w	#BK.X-1,d6
.loop_x		tst.w	(a2)
		beq.s	.next
		move.w	#BOB.BRICK,d0
		move.w	d3,d1
		move.w	d5,d2
		bsr.l	shado.gfx
.next		add.w	#16,d3			X=X+16
		addq.l	#4,a2
		dbf	d6,.loop_x
		add.w	#BK.HEIGHT+1,d5
		swap	d6
		dbf	d6,.loop_y
		rts

*-------------------------* Affiche les briques *------------------------*
* Entrée	=> a5.l	adr. 't'.
*		=> a6.l CUSTOM
affiche.bricks:
		*/ Calcul Modulos */
		move.l	(bk.pagegfx-t,a5),a3
		move.l	(pt_screen-t,a5),a0
		move.w	(PG_SPLAN,a3),d0
		subq.w	#2,d0
		swap	d0
		move.w	(SP_SPLAN,a0),d0
		subq.w	#2,d0

		*/ Calcul fenêtre Blitter */
		move.w	(PG_NPLAN,a3),d5
		mulu	#BK.HEIGHT,d5
		lsl.w	#6,d5
		addq.w	#1,d5

		move.l	(SPECRAN1,a0),d3
		clr.w	(brick.count-t,a5)
		clr.w	(brick.destroy-t,a5)
		clr.w	(brick.kass-t,a5)
		lea	(set_guide-t,a5),a1
		lea	(2+set_table-t,a5),a0
		move.w	#NID-1,d2
		lea	(PG_DATA,a3),a3
		WAITBLITTER
		move.l	#$09F00000,(BLTCON0,a6)
		move.l	#-1,(BLTAFWM,a6)
		move.l	d0,(BLTAMOD,a6)

		*/ Boucle d'affichage */
.zet_zoop	moveq	#0,d0
		move.l	(a1)+,d1
		move.w	(a0),d0			lit le code brique
		beq.s	.set_fin		-> 0 pas de brique
		cmp	#56*TLIGNEB,d0		brique indestrutible ?
		bge.s	.set_noad		-> non
		addq.w	#1,(brick.count-t,a5)	inc. compt. de briques
.set_noad	add.l	a3,d0
		add.l	d3,d1
		WAITBLITTER
		move.l	d0,(BLTAPTH,a6)
		move.l	d1,(BLTDPTH,a6)
		move.w	d5,(BLTSIZE,a6)
.set_fin	addq.l	#4,a0
		dbf	d2,.zet_zoop
		addq.w	#1,(brick.count-t,a5)
		rts

*--------------------*/ Calcul la table 'set_table' /*--------------------*
* Entrée	=> a5.l	adr. 't'.
init.set_table2:
		lea	(set_table_end-t,a5),a1
		moveq	#0,d6
		moveq	#BK.X-1,d0		16-1
.loop		move.l	d6,(a1)+
		dbf	d0,.loop
		lea	(set_table-t,a5),a0
		lea	(set_table2-t,a5),a1
		move.w	#(set_table2-set_table)/4-1,d0
.kokopy_right	move.l	(a0)+,(a1)+
		dbf	d0,.kokopy_right
		rts

*-------------------------------------------------------------------------*
*-----------* Convertit nbre brique > caractères affichables *------------*
*-------------------------------------------------------------------------*
bricks.conv:
		clr.w	(brick0-t,a5)
		clr.w	(brick1-t,a5)
		clr.w	(brick2-t,a5)
		lea	(brick0-t,a5),a0	pt/caract. ascii (unité)
		moveq	#0,d0
		move.w	(brick.count-t,a5),d0
.sc_init:	subq.w	#1,d0
		beq.s	.sc_organise
		addq.w	#1,(a0)
		cmp.w	#10,(a0)
		blt.s	.sc_init
		sub.w	#10,(a0)
		addq.w	#1,(2,a0)
		cmp.w	#10,(2,a0)
		blt.s	.sc_init
		sub.w	#10,(2,a0)
		addq.w	#1,(4,a0)
		bra.s	.sc_init
.sc_organise	rts
