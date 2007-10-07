*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
*--------------------------*MENU TECNOBALLZ AGA*--------------------------*
*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
menu.run
		bsr	hasard_ok
		bsr.l	wait_vbl
		bsr	frere_jacques		£=> Gère pointeur souris
		RASTER	#$008
		bsr	ii_astrapy		£=> Calcul position Logo
		bsr	ii_okapy		£=> Calcul position texte
		bsr	last_flip		|-> Echange listes Copper
		bsr	print.sprite		£=> Ecrit texte menu
		bsr	goscroll		£=> Affiche défilement
		bsr	ssha_restore
		bsr	sbob_restore
		bsr	zorglub2		|-> Scroll-Text
		bsr	ssha_affiche		|-> Affiche Shadows-SBOBs
		bsr	sbob_affiche		|-> Affiche Scroll-BOBs
		bsr	zelect
		RASTER	#$000
		bsr	pertedememoire		|-> Glou-Glou
		bsr	test_shcode
		bra	irq

*/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/*
*                         GESTION POINTEUR SOURIS			  *
*/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/*
* Entrée	=> a5.l	Adr. 't'
frere_jacques:
		bsr.l	move_mouse
		move.l	(ii.bob2-t,a5),a3
		move.w	(mouse_x-t,a5),(SBOB.POSX,a3)
		move.w	(mouse_y-t,a5),(SBOB.POSY,a3)
		lea	(SBOB.TEMPO,a3),a0
		subq.w	#1,(a0)+		tempo=tempo-1
		bne.s	.noamim			-> tempo > 0
		move.w	(a0)+,(SBOB.TEMPO,a3)
		move.w	(a0)+,d0		d0=pt/animation
		cmp.w	(a0)+,d0		pt < au pt maxi ?
		bls.s	.no_maxi		-> oui
		moveq	#0,d0
		move.w	d0,(SBOB.PTANIM,a3)
.no_maxi	addq.w	#8,(SBOB.PTANIM,a3)	
		move.l	(SBOB.TABLE,a3),a0	a0=table adr. gfx & msk
		add.w	d0,a0
		move.l	(a0)+,(SBOB.GFX,a3)
		move.l	(a0),(SBOB.MSK,a3)
.noamim		rts

*/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/*
************************/ CACLUL POSITION LOGO */*************************
*/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/*
* Entrée	=> a5.l	Adr. 't'
ii_astrapy:
		move.l	(coordx-t,a5),d6
		move.l	(coordy-t,a5),d7
		move.l	(ii.bob-t,a5),a4
		lea	(ii.luti-t,a5),a3
		moveq	#3,d5
.loop		move.l	(a3)+,a0
		move.l	(a3)+,a1
		moveq	#0,d0
		move.w	(SBOB.POSX,a4),d0
		lsl.w	#2,d0
		moveq	#0,d1
		move.w	(SBOB.POSY,a4),d1
		move.w	(ii.haut-t,a5),d2
		bsr.l	calcul_mot_ctrl_aga
		move.w	d3,(8,a0)
		or.w	#$80,d3
		move.w	d3,(8,a1)
		swap	d3
		move.w	d3,(a0)	
		move.w	d3,(a1)
		add.w	#64*4,d6
		dbf	d5,.loop
		rts

*/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/*
************************/ CACLUL POSITION TEXTE */*************************
*/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/*
* Entrée	=> a5.l	Adr. 't'
ii_okapy:
		move.l	(coordx-t,a5),d6
		move.l	(coordy-t,a5),d7
		lea	(ii.luti2-t,a5),a3
		moveq	#3,d5
.loop		move.l	(a3)+,a0
		move.l	(a3)+,a1
		moveq	#0,d0
		move.w	(ii.x-t,a5),d0
		lsl.w	#2,d0
		moveq	#0,d1
		move.w	(ii.y-t,a5),d1
		move.w	#VKN*VKH,d2
		bsr.l	calcul_mot_ctrl_aga
		move.w	d3,(8,a0)
		or.w	#$80,d3
		move.w	d3,(8,a1)
		swap	d3
		move.w	d3,(a0)	
		move.w	d3,(a1)
		add.w	#64*4,d6
		dbf	d5,.loop
		rts

*/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/*
*                             ECRIT LE TEXTE				  *
*/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/*
* Entrée	=> a5.l	Adr. 't'
print.sprite:
		move.w	#-1,(act.select-t,a5)
		move.l	(viking.menu-t,a5),a4	a4=structure menu
		move.w	(acty-t,a5),d0
		jsr	([.action,pc,d0*4])
		bra	hypertext
		CNOP	0,4
.action		dc.l	act.clear1		0
		dc.l	act.clear2		1
		dc.l	act.clear3		2
		dc.l	act.clear4		3
		dc.l	act.print7		4
		dc.l	act.zewait		5
act.clear1	clr.l	(viking.input-t,a5)
		moveq	#16,d0
		bra.s	act.clear
act.clear2	moveq	#16*2,d0
		bra.s	act.clear
act.clear3	moveq	#16*3,d0
		bra.s	act.clear
act.clear4	moveq	#16*4,d0
*=---=-------=-==* Efface les lutins toutes les 4 lignes *==-=-------=---=*
* Entrée	=> d0.l	offset prmière ligne à effacer
*		=> a5.l	Adr. 't'
act.clear:
		addq.w	#1,(acty-t,a5)
		movem.l	a5/a6,-(a7)
		move.l	a7,d7
		lea	(ii.luti2-t,a5),a0
		move.l	(a0)+,a7
		add.l	d0,a7
		move.l	(a0)+,a6
		add.l	d0,a6
		move.l	(a0)+,a5
		add.l	d0,a5
		move.l	(a0)+,a4
		add.l	d0,a4
		move.l	(a0)+,a3
		add.l	d0,a3
		move.l	(a0)+,a2
		add.l	d0,a2
		move.l	(a0)+,a1
		add.l	d0,a1
		move.l	(a0)+,a0
		add.l	d0,a0
		moveq	#0,d0
		moveq	#(VKN*VKH/4)-1,d2
		move.l	#16*3,d1	
.loop		move.l	d0,(a0)+
		move.l	d0,(a1)+
		move.l	d0,(a2)+
		move.l	d0,(a3)+
		move.l	d0,(a4)+
		move.l	d0,(a5)+
		move.l	d0,(a6)+
		move.l	d0,(a7)+
		move.l	d0,(a0)+
		move.l	d0,(a1)+
		move.l	d0,(a2)+
		move.l	d0,(a3)+
		move.l	d0,(a4)+
		move.l	d0,(a5)+
		move.l	d0,(a6)+
		move.l	d0,(a7)+
		move.l	d0,(a0)+
		move.l	d0,(a1)+
		move.l	d0,(a2)+
		move.l	d0,(a3)+
		move.l	d0,(a4)+
		move.l	d0,(a5)+
		move.l	d0,(a6)+
		move.l	d0,(a7)+
		move.l	d0,(a0)+
		add.l	d1,a0
		move.l	d0,(a1)+
		add.l	d1,a1
		move.l	d0,(a2)+
		add.l	d1,a2
		move.l	d0,(a3)+
		add.l	d1,a3
		move.l	d0,(a4)+
		add.l	d1,a4
		move.l	d0,(a5)+
		add.l	d1,a5
		move.l	d0,(a6)+
		add.l	d1,a6
		move.l	d0,(a7)+
		add.l	d1,a7
		dbf	d2,.loop
		move.l	d7,a7
		movem.l	(a7)+,a5/a6
		rts
*-------------> Affiche toutes les lignes une par une <-------------------*
* Entrée	=> a4.l	Structure Menu
*		=> a5.l	Adr. 't'
act.print7:
		moveq	#0,d0
		move.w	(act.line-t,a5),d0
		addq.w	#1,(act.line-t,a5)
		cmp.w	(VK.NUM,a4),d0
		bne.s	.go
		addq.w	#1,(acty-t,a5)
		clr.w	(act.line-t,a5)
.go		bsr	print.one
		clr.w	(act.restore-t,a5)
		rts
*-----------------------* Test position curseur  *------------------------*
* Entrée	=> a5.l	Adr. 't'
act.zewait:
		moveq	#0,d0
		move.w	(act.restore-t,a5),d0
		bsr	print.one
		move.l	(viking.menu-t,a5),a4	a4=structure menu
		moveq	#0,d0
		move.w	(mouse_y-t,a5),d0
		sub.w	(ii.y-t,a5),d0
		bmi.s	.exit
		divu	#VKH,d0
		moveq	#VKN-1,d1
		sub.w	(VK.NUM,a4),d1
		lsr.w	#1,d1
		sub.w	d1,d0
		bmi.s	.exit
		cmp.w	(VK.NUM,a4),d0
		bhi.s	.exit
		and.l	#$FFFF,d0
		move.w	d0,(act.select-t,a5)
		not.w	(act.flip-t,a5)
		beq.s	.exit
		move.w	d0,(act.restore-t,a5)
		bsr	clear.one
.exit		rts
*--------------------------> Affiche une ligne <--------------------------*
* Entrée	=> d0.l	n° de la ligne
*		=> a4.l	Structure Menu
*		=> a5.l	Adr. 't'
print.one:
		move.l	(VK.TEXT,a4),a2
		add.l	a5,a2			a2=pt/texte à afficher
		lsl.l	#5,d0			32 caractères par lignes
		add.l	d0,a2
fv77		move.l	(fonte80.pagegfx-t,a5),d4
		add.l	#PG_DATA,d4
		moveq	#VKN-1,d6
		sub.w	(VK.NUM,a4),d6
		lsr.w	#1,d6
		lsl.l	#8,d6
		lsl.l	#3,d0
		move.l	(ii.table-t,a5),a4
		add.l	d0,a4
		add.l	d6,a4
		moveq	#32,d5
		moveq	#0,d0
		moveq	#31,d6			32 caractères
.loop		move.b	(a2)+,d0		d0=ASCII
		move.l	(a4)+,a0		Lutin Paire
		move.l	(a4)+,a1		Lutin Impaire
		sub.w	d5,d0
		beq.s	.next
		bsr	overprint	
.next		dbf	d6,.loop
		rts
*------------> Affiche le texte entré au clavier + le curseur <-----------*
* Entrée	=> a5.l	Adr. 't'
hypertext:
		tst.l	(viking.input-t,a5)	On entre un texte ?
		beq	.exit			-> non
		bsr	hyper1			|-> Affiche la ligne
		bsr	hyper2			|-> Affiche le curseur
.exit		rts
*------------------> Affiche le texte entré au clavier <------------------*
* Entrée	=> a5.l	Adr. 't'
hyper1:		
		move.l	(viking.menu-t,a5),a4	a4=structure menu
		tst.l	(viking.input-t,a5)	On entre un texte ?
		beq	.exit			-> non
		move.l	(viking.input-t,a5),a2	
		move.l	(fonte80.pagegfx-t,a5),d4
		add.l	#PG_DATA,d4
		moveq	#VKN-1,d6
		sub.w	(VK.NUM,a4),d6
		lsr.w	#1,d6
		lsl.l	#8,d6
		moveq	#0,d0
		move.w	(viking.cy-t,a5),d0
		lsl.l	#8,d0
		move.l	(ii.table-t,a5),a4
		add.l	d0,a4
		add.l	d6,a4
		moveq	#0,d0
		move.w	(viking.cx-t,a5),d0
		lsl.l	#3,d0
		add.l	d0,a4
		move.l	a4,(viking.screen-t,a5)
		moveq	#0,d0
		move.w	(viking.lenght-t,a5),d6
		subq.w	#1,d6
.loop		move.b	(a2)+,d0		d0=ASCII
		move.l	(a4)+,a0		Lutin Paire
		move.l	(a4)+,a1		Lutin Impaire
		sub.w	#32,d0
		bsr	overprint	
		dbf	d6,.loop
.exit		rts
*-+-+-+-+-+-+-+-+-+-+-+- */ Affiche le curseur /* -+-+-+-+-+-+-+-+-+-+-+-+*
* Entrée	=> a5.l	Adr. 't'
hyper2:		
		move.l	(viking.screen-t,a5),a4
		subq.w	#1,(viking_wait-t,a5)	Réinitialise le compteur ?
		bne.s	.nonul			-> non
		move.w	#50,(viking_wait-t,a5)	Réinit. le compteur
.nonul		cmp.w	#30,(viking_wait-t,a5)	Affiche le curseur ?
		bhi	.exit			-> non
		move.l	(viking.screen-t,a5),a2
		move.w	(viking.curs-t,a5),d0
		lsl.w	#3,d0
		add.w	d0,a2
		move.l	(a2)+,a0		Lutin Paire
		move.l	(a2)+,a1		Lutin Impaire
		moveq	#-1,d0
		move.b	d0,(a0)
		move.b	d0,(1*16,a0)
		move.b	d0,(2*16,a0)
		move.b	d0,(3*16,a0)
		move.b	d0,(4*16,a0)
		move.b	d0,(5*16,a0)
		move.b	d0,(6*16,a0)
		move.b	d0,(7*16,a0)
.exit		rts
*OoOoOoOoOoOoOoOoOoOoOoO Affiche un caractère OoOoOoOoOoOoOoOoOoOoOoOoOoOo*
* Entrée	=> d0.w	n° du caractère à afficher
*		=> d4.l Adresse fontes de 56*8
*		=> a0.l	Lutin paire
*		=> a1.l	Lutin impaire
*		=> a5.l	Adr. 't'
overprint:
		move.b	(fonte80-t,a5,d0),d0	d0=offset caractère
		move.l	d4,a3			a1=adresse page fonte
		add.l	d0,a3			+offset caractère
		move.b	(a3),(a0)
		move.b	(1*56,a3),(1*08,a0)
		move.b	(2*56,a3),(0*08,a1)
		move.b	(3*56,a3),(1*08,a1)
		move.b	(4*56,a3),(2*08,a0)
		move.b	(5*56,a3),(3*08,a0)
		move.b	(6*56,a3),(2*08,a1)
		move.b	(7*56,a3),(3*08,a1)
		move.b	(8*56,a3),(4*08,a0)
		move.b	(9*56,a3),(5*08,a0)
		move.b	(10*56,a3),(4*08,a1)
		move.b	(11*56,a3),(5*08,a1)
		move.b	(12*56,a3),(6*08,a0)
		move.b	(13*56,a3),(7*08,a0)
		move.b	(14*56,a3),(6*08,a1)
		move.b	(15*56,a3),(7*08,a1)
		move.b	(16*56,a3),(8*08,a0)
		move.b	(17*56,a3),(9*08,a0)
		move.b	(18*56,a3),(8*08,a1)
		move.b	(19*56,a3),(9*08,a1)
		move.b	(20*56,a3),(10*08,a0)
		move.b	(21*56,a3),(11*08,a0)
		move.b	(22*56,a3),(10*08,a1)
		move.b	(23*56,a3),(11*08,a1)
		move.b	(24*56,a3),(12*08,a0)
		move.b	(25*56,a3),(13*08,a0)
		move.b	(26*56,a3),(12*08,a1)
		move.b	(27*56,a3),(13*08,a1)
		move.b	(28*56,a3),(14*08,a0)
		move.b	(29*56,a3),(15*08,a0)
		move.b	(30*56,a3),(14*08,a1)
		move.b	(31*56,a3),(15*08,a1)
		rts
*--------------------------> Efface une ligne <---------------------------*
* Entrée	=> d0.l	n° de la ligne
*		=> a4.l	Structure Menu
* 		=> a5.l	Adr. 't'
clear.one:
		moveq	#VKN-1,d6
		sub.w	(VK.NUM,a4),d6
		lsr.w	#1,d6
		lsl.l	#8,d6
		lsl.l	#8,d0
		move.l	(ii.table-t,a5),a0
		add.l	d0,a0
		add.l	d6,a0
		movem.l	a5/a6,-(a7)
		move.l	a7,d7
		move.l	(a0),a7
		lea	(32,a0),a0
		move.l	(a0),a6
		lea	(32,a0),a0
		move.l	(a0),a5
		lea	(32,a0),a0
		move.l	(a0),a4
		lea	(32,a0),a0
		move.l	(a0),a3
		lea	(32,a0),a0
		move.l	(a0),a2
		lea	(32,a0),a0
		move.l	(a0),a1
		lea	(32,a0),a0
		move.l	(a0),a0
		moveq	#0,d0
		moveq	#8*4-1,d1
.loop		move.l	d0,(a0)+
		move.l	d0,(a1)+
		move.l	d0,(a2)+
		move.l	d0,(a3)+
		move.l	d0,(a4)+
		move.l	d0,(a5)+
		move.l	d0,(a6)+
		move.l	d0,(a7)+
		dbf	d1,.loop
		move.l	d7,a7
		movem.l	(a7)+,a5/a6
		rts

*ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø*
*===============> Défilement Textes avec des Scrolls-BOBs <===============*
*ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø-ø*
* Entrée	=> a5.l	Adr. 't'
zorglub2:
		move.l	a6,-(a7)
		move.l	(pt_cos-t,a5),a2
		lea	(PT_SIN,a2),a1

		*/ Bouge le logo */
		move.w	(xs1-t,a5),d0
		moveq	#20,d1
		muls	(a1,d0),d1
		asr.l	#7,d1
		add.w	#32,d1
		move.l	(ii.bob-t,a5),a0
		move.w	d1,(SBOB.POSX,a0)
		tst.w	(sh.code-t,a5)
		beq.s	.yo
		moveq	#5,d1
		add.w	d0,d0
		add.w	d0,d0
		and.w	#1022,d0
		muls	(a2,d0),d1
		asr.l	#7,d1
		addq.w	#7,d1
		move.l	(ii.bob-t,a5),a0
		move.w	d1,(SBOB.POSY,a0)
.yo
		*/ VARIE RAYON X (Largeur) */
		move.w	(xs1-t,a5),d0
		addq.w	#6,d0
		and.w	#1022,d0
		move.w	d0,(xs1-t,a5)
		moveq	#10,d6
		muls	(a1,d0),d6		sin(a)*rayon
		asr.l	#7,d6
		add.w	#132,d6
		move.l	(pt.iii-t,a5),a3
		moveq	#AFNOR-1,d7
		move.w	#2,d5			d5=DEP
		tst.w	(sh.code-t,a5)
		beq.s	.loop
		add.w	d5,d5

		*/ Lecture du pointeur SBOB */
.loop		tst.l	(a3)			Dernier SBOB ?
		bne.s	.pasencor		-> non
		lea	(iiii-4-t,a5),a3	Oui, repasse au premier
.pasencor	move.l	(a3),a4
		subq.l	#4,a3
		*/ Incrémente pointeur cosinus (déplacement horizontal) */
		move.w	(SBOB.POSX1,a4),d0	d0=A valeur de l'angle
		add.w	d5,d0			A=A+DEP
		cmp.w	#512,d0
		blt.s	.dangerous4		A=0 COS(A)=1

		*/ LECTURE NOUVEAU CARACTERE */
		move.l	(iis-t,a5),a0
		moveq	#0,d1
		move.b	(a0),d1
		bne.s	.noend
		move.l	(scroll.text-t,a5),a0
		move.l	a0,(iis-t,a5)
		move.b	(a0),d1
.noend		addq.l	#1,(iis-t,a5)
		sub.w	#32,d1
		move.w	(skid-t,a5,d1*2),d1
		move.l	(SBOB.TABLE,a4),a0	a0=table adr. gfx & msk
		add.w	d1,a0
		move.l	(a0)+,(SBOB.GFX,a4)
		move.l	(a0),(SBOB.MSK,a4)
		*/ Change pt/le caractère le plus à gauche */
		move.l	(pt.iii-t,a5),a0
		tst.l	(-4,a0)
		bne.s	.yvesderuytier
		lea	(iiii-t,a5),a0
		move.l	a0,(pt.iii-t,a5)
.yvesderuytier	subq.l	#4,(pt.iii-t,a5)
.dangerous4	and.w	#510,d0			On vairie sur 180°
		move.w	d0,(SBOB.POSX1,a4)
		move.w	d6,d1
		muls	(a2,d0),d1		cos(a)*rayon
		asr.l	#7,d1
		add.w	#152,d1
		move.w	d1,(SBOB.POSX,a4)
		dbf	d7,.loop

*-------------------------------* Varie Y *-------------------------------*
		move.w	(xs.pt-t,a5),d5
		addq.w	#4,d5
		and.w	#1022,d5
		move.w	d5,(xs.pt-t,a5)

		move.l	(pt.iii-t,a5),a3
		moveq	#AFNOR-1,d7
		moveq	#8,d4
		tst.w	(sh.code-t,a5)
		beq.s	.yloop
		add.w	d4,d4

		*/ Lecture du pointeur SBOB */
.yloop		tst.l	(a3)			Derniere SBOB ?
		bne.s	.ypasencor		-> non
		lea	(iiii-4-t,a5),a3	Oui, repasse au premier
.ypasencor	move.l	(a3),a4
		subq.l	#4,a3

		*/ VARIE Y */
		add.w	d4,d5
		and.w	#1022,d5
		moveq	#25,d1
		muls	(a1,d5),d1
		asr.l	#7,d1
		add.w	#200,d1
		move.w	d1,(SBOB.POSY,a4)
		dbf	d7,.yloop

*-------------------------- Déplace les caches ---------------------------*
		*/ Cache droit */
		addq.l	#4,a3
		move.l	(a3),a3
		move.l	(ii1-t,a5),a4
		move.w	d6,d1
		muls	(a2),d1
		asr.l	#7,d1
		add.w	#152,d1
		move.w	d1,(SBOB.POSX,a4)
		move.w	(SBOB.POSY,a3),(SBOB.POSY,a4)

		*/ Cache gauche */
		move.l	(ii2-t,a5),a4
		move.w	d6,d1
		muls	(510,a2),d1
		asr.l	#7,d1
		add.w	#152,d1
		move.w	d1,(SBOB.POSX,a4)
		move.l	(pt.iii-t,a5),a3
		move.l	(a3),a3
		move.w	(SBOB.POSY,a3),(SBOB.POSY,a4)
.exit		move.l	(a7)+,a6
		rts

*=========================================================================*
*-------------------------------*SELECTION*-------------------------------*
*=========================================================================*
zelect:
		cmp.w	#5,(acty-t,a5)
		bne.s	.exit

		*/ Test boutons souris */
		moveq	#0,d0
		tst.b	(viking.left-t,a5)	Bouton gauche ?
		bne.s	.go			-> oui
		moveq	#4,d0
		tst.b	(viking.right-t,a5)	Bouton droit ?
		beq.s	.exit			-> non
.go
		*/ Saut routines */
		move.l	(viking.menu-t,a5),a4	a4=structure menu
		tst.l	(VK.JMP,a4)		pas de routines spéciales ?
		beq	main.rx			-> non
		move.w	(act.select-t,a5),d1
		bmi	.exit
		lsl.w	#3,d1
		add.w	d0,d1
		move.l	(VK.JMP,a4),a0
		add.l	a5,a0
		move.l	(a0,d1),a0
		add.l	a5,a0
		jmp	(a0)
.exit		rts

		*/ START GAME */	
startg:		bsr.l	np_end
		bsr.l	go_fade_off
.test786	tst.w	(flag_fade_off-t,a5)
		bne.s	.test786
		bsr	ii.kill
		bsr	star
		move.l	#irq,(a7)
		rts

		*/ OPTIONS */
option:		move.w	(hard-t,a5),d0
		bsr	levexx2
		lea	(viki.options-t,a5),a0
		bra	rex

		*/ INFORMATIONS */
inform:		lea	(viki.infos-t,a5),a0
		bra	rex

		*/ SCORE */
scorev:		bsr	rekop58
		lea	(viki.scores-t,a5),a0
		bra	rex

		*/ PASSWORD */
passwo:		bsr	hyper1
		lea	(10+tv2-t,a5),a0
		clr.w	(xcurs-t,a5)
		clr.w	(viking.curs-t,a5)
		move.l	a0,(viking.input-t,a5)
		move.w	#10,(viking.cx-t,a5)
		move.w	#7,(viking.cy-t,a5)
		move.w	#10,(viking.lenght-t,a5)
		rts

		*/ CREDITS */
crediz:		lea	version+6-t,a0
		add.l	a5,a0
		lea	(text.creditx+1-t,a5),a1
		moveq	#30-1,d0
.loop		move.b	(a0)+,(a1)+
		dbf	d0,.loop
		lea	(viki.credit-t,a5),a0
		bra	rex

		*/ GREETINGS */
greeti:		clr.w	(music.on-t,a5)
		lea	(viki.greet-t,a5),a0
		bra	rex

		*/ LEVEL-EDITOR */
levedi		bsr.l	np_end
		bsr.l	go_fade_off
.test786	tst.w	(flag_fade_off-t,a5)
		bne.s	.test786
		bsr	ii.kill
		move.w	#14,(rout-t,a5)
		move.l	#irq,(a7)
		rts
		rts

		*/ EXIT TO AMIGADOS */
exitto:		bra.l	sys_fin

*|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|*
********************************  OPTIONS  ********************************
*|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|*
* Entrée	=> a5.l	Adr. 't'

		*/ Mode Team/Alone */
gamemo:		lea	(24+text.options1-t,a5),a0
		not.w	(rak.team-t,a5)
		beq.s	.alone
		move.l	#'TEAM',(a0)+
		move.b	#'!',(a0)
		rts
.alone		move.l	#'ALON',(a0)+
		move.b	#'E',(a0)
		rts

		*/ Nombre de joueur /*
nuofpa:		lea	(player.num-t,a5),a0
		moveq	#6,d1
		lea	(24+text.options2-t,a5),a2
bazar:		subq.w	#1,(a0)
		tst.b	(viking.right-t,a5)	Bouton gauche ?
		bne.s	.go
		addq.w	#2,(a0)
.go		tst.w	(a0)
		bmi.s	.neg			-> negatif
		bne.s	.k2000			-> different de 0
.neg		move.w	d1,(a0)
.k2000		cmp.w	(a0),d1
		bge.s	.kiki
		move.w	#1,(a0)
.kiki		moveq	#0,d2
		move.w	(a0),d2
		bsr.l	convertit_decimal
		moveq	#1-1,d1
		move.l	a2,a0
		bsr.l	conv_number_ascii
		rts

		*/ ENTRE NOM D'UN JOUEUR */
change:		bsr	hyper1
		lea	(24+text.options3-t,a5),a0
		move.w	(act.select-t,a5),d0
		move.w	#24,(viking.cx-t,a5)
		move.w	d0,(viking.cy-t,a5)
		subq.w	#2,d0
		mulu	#32,d0
		add.l	d0,a0
		clr.w	(xcurs-t,a5)
		clr.w	(viking.curs-t,a5)
		move.l	a0,(viking.input-t,a5)
		move.w	#6,(viking.lenght-t,a5)
		rts

		*/ Change difficulté */
levexx:		lea	(hard-t,a5),a0
		move.w	(a0),d0
		addq.w	#1,d0
		and.w	#3,d0
		move.w	d0,(a0)
levexx2		move.l	(.lev,pc,d0*4),d0
		move.l	d0,(24+text.options9-t,a5)
		rts
		CNOP	0,4
.lev		dc.l	"EASY","HARD","MAD!","DEAD"

		*/ Change nombre de vie(s) */
livexx:		lea	(love-t,a5),a0
		moveq	#9,d1
		lea	(25+text.options10-t,a5),a2
		bra	bazar

main.rx		lea	(viki.main-t,a5),a0
rex		move.l	a0,(viking.menu-t,a5)
		clr.w	(acty-t,a5)
		rts

*-------------------------------------------------------------------------*
*----------*/ TEST SI TOUS LES NOMS ONT MA DATE DE NAISSANCE */-----------*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	adr. 't'
test_shcode:

		moveq	#NJOUEUR-1,d2		6 noms de joueurs
		lea	(intro_name-t,a5),a2
.loop		move.l	(a2)+,a0		a0=pt/nom joueur ASCII
		add.l	a5,a0
		cmp.l	#'0406',(a0)+
		bne.s	.nocode
		cmp.w	#'70',(a0)+
		bne.s	.nocode
		dbf	d2,.loop
		move.w	#'EB',(sh.code-t,a5)
		move.w	#1,(love1-t,a5)
		move.w	#3,(gd.sspeed-t,a5)
		rts
.nocode		clr.w	(sh.code-t,a5)
		clr.w	(love1-t,a5)
		move.w	#1,(gd.sspeed-t,a5)
		rts

