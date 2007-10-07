;..........................................................................
;
;	(16/11/91)	éditeur de "scrolling" vertical
;
;			all coding by ETB and J / (c) TLK GAMES 1991
;
;			mardi 17 decembre 1991
;
;..........................................................................

		incdir	ram:tec_data/
		section	tlk,code_f
		opt	p+

* J = SELECTEUR DE DALLE / EDITEUR DE DALLES
* B = BROSSES
* S = SAUVE MAP 
* L = LOAD-MAP / LOAD DALLES

;..........................................................................

;==========================================================================
;		quitte & revient proprement au système
;==========================================================================

;		- ouvre la librairie graphique -

sys_start:	movem.l	d0-d7/a0-a6,-(a7)
		move.l	4.w,a6
		lea	gfxname(pc),a1
		moveq	#0,d0
		jsr	-552(a6)
		lea	gfxbase(pc),a0
		move.l	d0,(a0)
		tst.l	d0
		beq	sys_retour

;		- ouvre la librairie dos -

		lea	dosname(pc),a1
		moveq	#0,d0
		jsr	-552(a6)
		lea	dosbase(pc),a0
		move.l	d0,(a0)
		tst.l	d0
		beq	sys_retour1

;		- ouvre la librairie intuition -

		lea	intuitionname(pc),a1
		moveq	#0,d0
		jsr	-552(a6)
		lea	intuitionbase(pc),a0
		move.l	d0,(a0)
		tst.l	d0
		beq	sys_retour2

;		- ouvre la librairie arp -

		lea	arpname(pc),a1
		moveq	#0,d0
		jsr	-552(a6)
		lea	arpbase(pc),a0
		move.l	d0,(a0)
		tst.l	d0
		beq	sys_retour3

;		- réserve mémoire écran 1 -

		sub.l	a1,a1			commence demander adresse $0.l
sys_mem1	move.l	a1,-(a7)		sauve adresse demandée
		move.l	#ecran_size,d0		taille mémoire à reserver
		jsr	allocabs(a6)		|-> demande mémoire
		tst.l	d0			accepter ?
		bne.s	mem_go1			-> oui
		move.l	(a7)+,a1		adresse mémoire demandée
		cmp.l	#$f0000,a1		dernière adresse possible ?
		beq	sys_retour4		-> oui, pas assez de mémoire
		add.l	#ecran_size,a1		non, essaie 65336 octets + loin
		bra.s	sys_mem1
mem_go1		lea	mem_go1(pc),a0
		lea	zecran1-mem_go1,a1
		add.l	a0,a1
		move.l	d0,(a1)			sauve adresse mémoire réservée
		move.l	(a7)+,a1		adresse mémoire demandée

;		- réserve mémoire écran 2 -

sys_mem2	move.l	a1,-(a7)		sauve adresse demandée
		move.l	#ecran_size,d0		taille mémoire à reserver
		jsr	allocabs(a6)		|-> demande mémoire
		tst.l	d0			accepter ?
		bne.s	mem_go2			-> oui
		move.l	(a7)+,a1		adresse mémoire demandée
		cmp.l	#$f0000,a1		dernière adresse possible ?
		beq	sys_retour5		-> oui, pas assez de mémoire
		add.l	#ecran_size,a1		non, essaie 65336 octets + loin
		bra.s	sys_mem2
mem_go2		lea	mem_go2(pc),a0
		lea	zecran2-mem_go2,a1
		add.l	a0,a1
		move.l	d0,(a1)			sauve adresse mémoire réservée
		move.l	(a7)+,a1		adresse mémoire demandée

;		- réserve mémoire écran 3 -

sys_mem3	move.l	a1,-(a7)		sauve adresse demandée
		move.l	#ecran_size,d0		taille mémoire à reserver
		jsr	allocabs(a6)		|-> demande mémoire
		tst.l	d0			accepter ?
		bne.s	mem_go3			-> oui
		move.l	(a7)+,a1		adresse mémoire demandée
		cmp.l	#$f0000,a1		dernière adresse possible ?
		beq	sys_retour6		-> oui, pas assez de mémoire
		add.l	#ecran_size,a1		non, essaie 65336 octets + loin
		bra.s	sys_mem3
mem_go3		lea	mem_go3(pc),a0
		lea	zecran3-mem_go3,a1
		add.l	a0,a1
		move.l	d0,(a1)			sauve adresse mémoire réservée
		move.l	(a7)+,a1		adresse mémoire demandée

;		- réserve la mémoire chip pour la brosse -

sys_mem4	move.l	#bsize,d0
		move.l	#clear,d1
		jsr	allocmem(a6)
		tst.l	d0
		beq	sys_retour7
		lea	sys_mem4(pc),a0
		lea	page_brush-sys_mem4,a1
		add.l	a0,a1
		move.l	d0,(a1)			sauve adresse page brosse

;		- réserve la mémoire chip pour la page dalle -

sys_mem5	move.l	#d_tligne*d_nligne,d0
		move.l	#clear,d1
		jsr	allocmem(a6)
		tst.l	d0
		beq	sys_retour8
		lea	sys_mem5(pc),a0
		lea	adalles-sys_mem5,a1
		add.l	a0,a1
		move.l	d0,(a1)			sauve adresse page dalle

;		- réserve la mémoire map-editor -

sys_mem6	move.l	#m_size,d0
		move.l	#$10000,d1
		jsr	allocmem(a6)
		tst.l	d0
		beq	sys_retour9
		lea	sys_mem6(pc),a0
		lea	asc_map-sys_mem6,a1
		add.l	a0,a1
		move.l	d0,(a1)			sauve adresse map-editor

;		- sauve le pt de pile utilisateur -

		lea	sys_pile1(pc),a0
		move.l	a7,(a0)

;		- sauve le système -

		bsr	sys_save
		bra	sys_gogo




;--------------------------------------------------------------------------
;		système : guru
;--------------------------------------------------------------------------
sys_guru:	move	#$7fff,$dff000+intena
		move	#$7fff,$dff000+intreq
		lea	e_info(pc),a0
		move	(a7)+,(a0)
		move.l	(a7)+,e_adr-e_info(a0)
		move	(a7)+,e_inst-e_info(a0)
		move	(a7)+,e_sr-e_info(a0)
		move.l	(a7)+,e_pc-e_info(a0)
		move	#0,sr
		bra	sys_fin




;--------------------------------------------------------------------------
;		système : instruction illegale
;--------------------------------------------------------------------------
sys_illegal:	move	#$7fff,$dff000+intena
		move	#$7fff,$dff000+intreq
		move	#$07ff,$dff000+dmacon
		move	#$7f00,$dff09e

;		- restore les 64 premiers longs mots de la mémoire -

		sub.l	a0,a0
		lea	sys_liste(pc),a1
		moveq	#64-1,d0
sys_loopi2	move.l	(a1)+,(a0)+
		dbf	d0,sys_loopi2

;		- réactive la liste copper système -

		lea	$dff000,a6
		move.l	gfxbase(pc),a1
		move.l	38(a1),$80(a6)
		move	d0,$88(a6)

;		- remet les bits d'ADKCON du système -

		move	#$9100,$9e(a6)

;		- réactive les dmas -

		move	#$83f0,dmacon(a6)

;		- autorise les irq système -

		move	#$c02c,intena(a6)

;		- reprend au vecteur $10 de "MonAm" -

		lea	sys_liste(pc),a1
		move.l	$10(a1),a1
		jmp	(a1)





;--------------------------------------------------------------------------
;		système : quitte pendant l'interruption clavier
;--------------------------------------------------------------------------
superquit:	bclr	#6,$bfee01
		move	#0,sr
		bra	sys_fin



;--------------------------------------------------------------------------
;		système : sauve le système
;--------------------------------------------------------------------------

;		- gèle le système -

sys_save:	move.l	4.w,a6
		jsr	-132(a6)		|-> interdit le multitâche
		lea	$dff000,a6
		move	#$7fff,intena(a6)	intena : interdit toutes les irq 
		move	#$7fff,intreq(a6)	intreq : plus d'irq demandée
		move	#$07ff,dmacon(a6)	dmacon : coupe tous les dmas
		move	#$7f00,$9e(a6)		adkcon : raz bits disques
		move	#$4000,$24(a6)		dsklen : raz

;		- sauve les 64 1er longs mots de la mémoire  -

		sub.l	a0,a0
		lea	sys_liste(pc),a1
		moveq	#64-1,d0
sys_loop1	move.l	(a0)+,(a1)+
		dbf	d0,sys_loop1

;		- détourne les vecteurs guru -

		lea	sys_guru(pc),a0
		lea	8.w,a1
		moveq	#9,d0
sys_aubert	move.l	a0,(a1)+
		dbf	d0,sys_aubert
		lea	sys_illegal(pc),a0
		move.l	a0,$10.w		vecteur "instruction illegale"

;		- sauve le pt de pile superviseur -

		lea	sys_trap(pc),a0
		move.l	a0,$80.w
		trap	#0
sys_trap	lea	sys_pile2(pc),a0
		addq	#6,a7
		move.l	a7,(a0)
		move	#0,sr
		rts




;--------------------------------------------------------------------------
;		système : restore le système
;--------------------------------------------------------------------------

;		- gèle le système -

sys_restore:	move	#$07ff,$dff000+dmacon
		move	#$7fff,$dff000+intena
		move	#$7fff,$dff000+intreq
		move	#$7f00,$dff09e

;		- restore le pt de pile superviseur -

		lea	sys_trap2(pc),a0
		move.l	a0,$80.w
		trap	#0
sys_trap2	move.l	sys_pile2(pc),a7
		move	#0,sr

;		- restore les 64 premiers longs mots de la mémoire -

		sub.l	a0,a0
		lea	sys_liste(pc),a1
		moveq	#64-1,d0
sys_loop2	move.l	(a1)+,(a0)+
		dbf	d0,sys_loop2

;		- réactive la liste copper système -

		lea	$dff000,a6
		move.l	gfxbase(pc),a1
		move.l	38(a1),$80(a6)
		move	d0,$88(a6)

;		- remet les bits d'ADKCON du système -

		move	#$9100,$9e(a6)

;		- réactive les dmas -

		move	#$83f0,dmacon(a6)

;		- autorise les irq système -

		move	#$c02c,intena(a6)

;		- remet le multitâche -

		move.l	4.w,a6
		jmp	-138(a6)




;--------------------------------------------------------------------------
;		système : revient sous le système
;--------------------------------------------------------------------------

;		- restore le système -

sys_fin:	bsr	sys_restore

;		- restore le pt de pile utilisateur -

		move.l	sys_pile1(pc),a7

;		- libère la mémoire map-editor -

sys_retour10	lea	sys_retour10(pc),a0
		lea	asc_map-sys_retour10,a1
		add.l	a0,a1
		tst.l	(a1)			adresse nul ?
		beq.s	sys_retour9		-> oui
		move.l	(a1),a1			adresse mémoire à liberer
		move.l	#m_size,d0		taille mémoire à libérer
		jsr	freemem(a6)		|-> libère cette mémoire

;		- libère la mémoire chip de la dalle -

sys_retour9	lea	sys_retour9(pc),a0
		lea	adalles-sys_retour9,a1
		add.l	a0,a1
		tst.l	(a1)			adresse nul ?
		beq.s	sys_retour8		-> oui
		move.l	(a1),a1			adresse mémoire à liberer
		move.l	#d_tligne*d_nligne,d0	taille mémoire à libérer
		jsr	freemem(a6)		|-> libère cette mémoire

;		- libère la mémoire chip de la brosse -

sys_retour8	lea	sys_retour8(pc),a0
		lea	page_brush-sys_retour8,a1
		add.l	a0,a1
		tst.l	(a1)			adresse nul ?
		beq.s	sys_retour7		-> oui
		move.l	(a1),a1			adresse mémoire à liberer
		move.l	#bsize,d0		taille mémoire à libérer
		jsr	freemem(a6)		|-> libère cette mémoire

;		- libère la mémoire écran 3 -

sys_retour7	lea	sys_retour7(pc),a0
		lea	zecran3-sys_retour7,a1
		add.l	a0,a1
		tst.l	(a1)			adresse nul ?
		beq.s	sys_retour6		-> oui
		move.l	(a1),a1			adresse mémoire à liberer
		move.l	#ecran_size,d0		taille mémoire à libérer
		jsr	freemem(a6)		|-> libère cette mémoire

;		- libère la mémoire écran 2 -

sys_retour6	lea	sys_retour6(pc),a0
		lea	zecran2-sys_retour6,a1
		add.l	a0,a1
		tst.l	(a1)			adresse nul ?
		beq.s	sys_retour5		-> oui
		move.l	(a1),a1			adresse mémoire à liberer
		move.l	#ecran_size,d0		taille mémoire à libérer
		jsr	freemem(a6)		|-> libère cette mémoire

;		- libère la mémoire écran 1 -

sys_retour5	lea	sys_retour5(pc),a0
		lea	zecran1-sys_retour5,a1
		add.l	a0,a1
		tst.l	(a1)			adresse nul ?
		beq.s	sys_retour4		-> oui
		move.l	(a1),a1			adresse mémoire à liberer
		move.l	#ecran_size,d0		taille mémoire à libérer
		jsr	freemem(a6)		|-> libère cette mémoire

;		- ferme la librairie arp -

sys_retour4	move.l	4.w,a6
		move.l	arpbase(pc),a1
		jsr	-414(a6)

;		- ferme la librairie intuition -

sys_retour3	move.l	intuitionbase(pc),a1
		jsr	-414(a6)

;		- ferme la librairie dos -

sys_retour2	move.l	dosbase(pc),a1
		jsr	-414(a6)

;		- ferme la librairie graphique -

sys_retour1	move.l	gfxbase(pc),a1
		jsr	-414(a6)

;		- rend la main au système -

sys_retour	movem.l	(a7)+,d0-d7/a0-a6
		rts




;--------------------------------------------------------------------------
;		système : sauvegarde des 64 premiers longs mots
;--------------------------------------------------------------------------
sys_liste	ds.l	64



;--------------------------------------------------------------------------
;		système : adr. pile utilisateur/superviseur système
;--------------------------------------------------------------------------
sys_pile1	dc.l	0
sys_pile2	dc.l	0




;--------------------------------------------------------------------------
;		système : info sur le guru
;--------------------------------------------------------------------------
e_info		dc	0
e_adr		dc.l	0
e_inst		dc	0
e_sr		dc	0
e_pc		dc.l	0




;--------------------------------------------------------------------------
;		système : adr. libriaries
;--------------------------------------------------------------------------
gfxbase		dc.l	0
dosbase		dc.l	0
intuitionbase	dc.l	0
arpbase		dc.l	0
gfxname		dc.b	'graphics.library',0
dosname		dc.b	'dos.library',0
intuitionname	dc.b	'intuition.library',0
arpname		dc.b	'arp.library',0
tlkname		dc.b	'tlk.library',0
		even
sys_gogo





;..........................................................................








;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
;		déclaration des constantes
;
;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
;
;
;
;
;
;
;
;==========================================================================
;		écrans du "scrolling" au format "raw-blitter"
;==========================================================================
ecran_size	=$10000
s_tplan		=40				taille d'un plan (raw-blitter)
s_nplan		=5				nbre de pn
s_tligne	=s_tplan*s_nplan		taille d'une ligne (raw-blitter)
s_nligne	=256+16				nombre de ligne
s_ecran		=s_nligne*s_tligne		taille d'un écran
s_rasty		=$30
s_rastx		=$80








;==========================================================================
;		page dalle
;==========================================================================
d_tplan		=040
d_nplan		=003
d_tligne	=d_tplan*d_nplan
d_nligne	=624		544
d_ndalle	=(d_tplan/2)*(d_nligne/16)	nombre de dalle dans la page




;==========================================================================
;		map editor
;==========================================================================
m_large		=020				nombre de dalle en largeur
m_hauteur	=273				nombre de dalle en hauteur
m_size		=m_large*m_hauteur*2




;==========================================================================
;		page brosse
;==========================================================================
btplan		=40
bnplan		=3
btligne		=btplan*bnplan
bnligne		=256
bsize		=btligne*bnligne






;==========================================================================
;		repeat clavier
;==========================================================================
tc1		=25
tc2		=2








;==========================================================================
;		"customs-chips"
;==========================================================================
ciaapra		=$bfe001
dmaconr		=$002
vhposr		=$006
dmacon		=$096
intreq		=$09c
intena		=$09a
joy0dat		=$00a
potgor		=$016
cop1lch		=$080
cop1jmp		=$088
color00		=$180




;--------------------------------------------------------------------------
;		customs chip : les pointeurs lutins
;--------------------------------------------------------------------------
spr0pth		=$120
spr0ptl		=$122
spr1pth		=$124
spr1ptl		=$126
spr2pth		=$128
spr2ptl		=$12a
spr3pth		=$12c
spr3ptl		=$12e
spr4pth		=$130
spr4ptl		=$132
spr5pth		=$134
spr5ptl		=$136
spr6pth		=$138
spr6ptl		=$13a
spr7pth		=$13c
spr7ptl		=$13e




;--------------------------------------------------------------------------
;		customs chip : les pointeurs vidéos
;--------------------------------------------------------------------------
bpl1pth		=$0e0
bpl1ptl		=$0e2
bpl2pth		=$0e4
bpl2ptl		=$0e6
bpl3pth		=$0e8
bpl3ptl		=$0ea
bpl4pth		=$0ec
bpl4ptl		=$0ee
bpl5pth		=$0f0
bpl5ptl		=$0f2
bpl6pth		=$0f4
bpl6ptl		=$0f6




;--------------------------------------------------------------------------
;		registres blitter
;--------------------------------------------------------------------------
bltcon0		=$040
bltcon1		=$042
bltafwm		=$044
bltalwm		=$046
bltcpth		=$048
bltcptl		=$04a
bltbpth		=$04c
bltbptl		=$04e
bltapth		=$050
bltaptl		=$052
bltdpth		=$054
bltdptl		=$056
bltsize		=$058
bltcmod		=$060
bltbmod		=$062
bltamod		=$064
bltdmod		=$066
bltcdat		=$070
bltbdat		=$072
bltadat		=$074




;--------------------------------------------------------------------------
;		customs chip : les registres vidéo
;--------------------------------------------------------------------------
diwstrt		=$08e
diwstop		=$090
ddfstrt		=$092
ddfstop		=$094
bplcon0		=$100
bplcon1		=$102
bplcon2		=$104
bpl1mod		=$108
bpl2mod		=$10a




;==========================================================================
;		Bases & Offset Library
;==========================================================================
execbase	=4
allocmem	=-198
allocabs	=-204
freemem		=-210
chip		=2
fast		=4
clear		=chip+$10000
wbenchtofront	=-342
;
;
;
;
;-+-+-+-+-+-+-+-+-+-> fin déclaration des constantes <-+-+-+-+-+-+-+-+-+-+-








;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
; >*<		déclaration des macros
;
;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
;
;
;
;
;
;
;
;==========================================================================
;		MACRO : attend le blitter
;==========================================================================
; entrée	a6.l	$dff000
;
waitblitter:	macro
;
;
;
;
wait\@		btst	#14,dmaconr(a6)
		bne.s	wait\@
		endm


;-=-=-=-=-=-=-=-=-=-=-> fin macro attend le blitter <-=-=-=-=-=-=-=-=-=-=-=








;==========================================================================
;		MACRO : attend le blitter
;==========================================================================
; entrée	d7.w	#14 ("bbusy" de "dmaconr")
;		a6.l	$dff002 ("dmaconr")
;
wait_blitter:	macro
;
;
;
;
wait\@		btst	d7,(a6)
		bne.s	wait\@
		endm


;-=-=-=-=-=-=-=-=-=-=-> fin macro attend le blitter <-=-=-=-=-=-=-=-=-=-=-=
;
;
;
;
;-+-+-+-+-+-+-+-+-+-+-> fin déclaration des macros <-+-+-+-+-+-+-+-+-+-+-+-








;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
; >*<		première initialisation
;
;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
first_init:
;
;
;
;
;--------------------------------------------------------------------------
;		first init : détourne vecteur de niveau 3
;--------------------------------------------------------------------------
		lea	irq_level3(pc),a0
		move.l	a0,$6c.w




;--------------------------------------------------------------------------
;		first init : détourne vecteur de niveau 2
;--------------------------------------------------------------------------
		lea	irq_level2(pc),a0
		move.l	a0,$68.w




;--------------------------------------------------------------------------
;		first init : charge base des custom-chips dans a6
;--------------------------------------------------------------------------
		lea	$dff000,a6




;--------------------------------------------------------------------------
;		first init : charge adr. table 'tlk' dans a5
;--------------------------------------------------------------------------
		lea	tlk(pc),a5



;--------------------------------------------------------------------------
;		first init : sauve adresses écrans
;--------------------------------------------------------------------------
		move.l	zecran1-tlk(a5),aecran1-tlk(a5)
		move.l	zecran2-tlk(a5),aecran2-tlk(a5)
		move.l	zecran3-tlk(a5),aecran3-tlk(a5)




;--------------------------------------------------------------------------
;		first init : recopie lutin en chip-memory
;--------------------------------------------------------------------------
		lea	sprite-tlk(a5),a0
		move.l	zecran3-tlk(a5),a1
		add.l	#s_ecran,a1
		move.l	a1,asprite-tlk(a5)
		move	#spriteend-sprite-1,d0
rsp.loop	move.b	(a0)+,(a1)+
		dbf	d0,rsp.loop




;--------------------------------------------------------------------------
;		first init : recopie listes copper en chip-memory
;--------------------------------------------------------------------------
		move.l	a1,zcopper1-tlk(a5)
		lea	sc_c1-tlk(a5),a0
		move	#sc_c2-sc_c1-1,d0
rlc1.loop	move.b	(a0)+,(a1)+
		dbf	d0,rlc1.loop

		move.l	a1,zcopper2-tlk(a5)
		lea	sc_c2-tlk(a5),a0
		move	#sc_c3-sc_c2-1,d0
rlc2.loop	move.b	(a0)+,(a1)+
		dbf	d0,rlc2.loop
		subq	#4,a1
		move.l	a1,select_copper-tlk(a5)




;--------------------------------------------------------------------------
;		first init : autorise interruption vbl & clavier
;--------------------------------------------------------------------------
		move	#$c028,intena(a6)




;--------------------------------------------------------------------------
;		first init : autorise dmas, blitter, plans, copper, lutins
;--------------------------------------------------------------------------
		move	#$83e0,dmacon(a6)


;-+-+-+-+-+-+-+-+-+-+-> fin première initialisation <-+-+-+-+-+-+-+-+-+-+-+








;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
;	>*<	programme prinipal hors interruption	
;
;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
main_loop:
;
;
;
;
;--------------------------------------------------------------------------
; 		main loop : attend la vbl
;--------------------------------------------------------------------------
		lea	$dff000,a6
		lea	tlk(pc),a5
		clr	flag_vbl-tlk(a5)
wait_vbl:	tst	flag_vbl-tlk(a5)
		beq.s	wait_vbl
		clr.w	$01FC(a6)

;--------------------------------------------------------------------------
; 		main loop : saute au sous programme
;--------------------------------------------------------------------------
		move	flag_prg-tlk(a5),d0
		lea	main_list(pc),a0
		move.l	(a0,d0),a0
		add.l	a5,a0
		jsr	(a0)
		bra.s	main_loop
main_list	dc.l	lugubre-tlk		00
		dc.l	mysterious-tlk		04
;
;
;
;
;+-+-+-+-+-> fin programme prinipal hors interruption <-+-+-+-+-+-+-+-+-+-+








;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
;	>*<	routines disque AmigaDos
;
;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
; 'save_map'		Sauve map-editor
; 'load_map'		Charge map-editor
; 'load_dalles'		Charge fichier IFF ILBM de 320*544*8
; 'requester'		Ouvre Requester dans WorkbenchScreen
; 'open_read'		Ouvre fichier en lecture
; 'open_write'		Ouvre fichier en écriture
; 'zend_load'		Charge, ferme fichier, et attend 3 secondes
; 'zend_save'		Sauve, ferme fichier, et attend 8 secondes
; 'quitte'		Gèle le système, revient à l'éditeur
;
;
;
;
;
;
;
;
;==========================================================================
;		amigados : save fichier map-editor
;==========================================================================
;
save_map:
;
;
;
;
;		-  save map : revient sous le système multitache -

		bsr	sys_restore

;		- save map : met le screen workbench devant les autres -

		move.l	intuitionbase(pc),a6
		jsr	wbenchtofront(a6)

;		- save map: affiche requester, et attend sélection -

		lea	titre3(pc),a1
		bsr	requester		|-> ouvre choix + requester
		tst.l	d0
		beq	error2			-> "error"

;		- save map : ouvre le fichier dos en lecture -

		bsr	open_write
		move.l	d0,d1
		beq	error2			-> "error"

;		- save map : save fichier -

		move.l	hondle(pc),d1
		lea	save_map(pc),a0
		lea	asc_map-save_map,a1
		add.l	a0,a1
		move.l	(a1),d2			adr. sauvegarde
		move.l	#m_size,d3		taille du fichier
		bra	zend_save


;=-=-=-=-=-=-=-=-=-=-=-> fin sauvegarde map-editor <-=-=-=-=-=-=-=-=-=-=-=-








;==========================================================================
;		amigados : charge fichier map-editor
;==========================================================================
;
load_map:
;
;
;
;
;		- load map : revient sous le système multitache -

		bsr	sys_restore

;		- load map : met le screen workbench devant les autres -

		move.l	intuitionbase(pc),a6
		jsr	wbenchtofront(a6)

;		- load map: affiche requester, et attend sélection -

		lea	titre2(pc),a1
		bsr	requester		|-> ouvre choix + requester
		tst.l	d0
		beq	error2			-> "error"

;		- load map : ouvre le fichier dos en lecture -

		bsr	open_read
		move.l	d0,d1
		beq	error2			-> "error"

;		- load map : réserve mémoire nécessaire -

		move.l	#m_size,d0
		move.l	#$10000,d1
		move.l	4.w,a6
		jsr	allocmem(a6)
		tst.l	d0
		beq	close_quitte2		-> pas assez de mémoire
		lea	first8(pc),a0
		move.l	d0,(a0)			sauve adresse mémoire

;		- load map : charge fichier -

		move.l	hondle(pc),d1
		move.l	d0,d2			adresse chargement
		move.l	#m_size,d3		taille fichier
		move.l	dosbase(pc),a6
		jsr	-42(a6)			|-> charge fichier

;		- load map : verifie le fichier -

		lea	first8(pc),a0
		move.l	(a0),a0			adresse mémoire fichier
		move	#d_ndalle*4,d1		plus grand n° de dalle
		move	#$0003,d2
		move	#m_size/2-1,d3		taille fichier
map_verify:	move	(a0)+,d0
		and	d2,d0			multiple de 4 ?
		bne.s	free_rose		-> non, erreur
		cmp	d1,d0			n° dalle > maxi ?
		bhi.s	free_rose		-> oui, erreur
		dbf	d3,map_verify

;		- load map : recopie le map-editor -

		lea	first8(pc),a0
		lea	asc_map-first8,a1
		add.l	a0,a1
		move.l	(a1),a1			adresse vieux map-editor 
		move.l	(a0),a0			adresse map-editor chargé
		move	#m_size/2-1,d3		taille fichier
map_copy	move	(a0)+,(a1)+
		dbf	d3,map_copy
		move	#1,flag_load-tlk(a5)	flag chargement éffectué

;		- load map : libère mémoire nécessaire -

free_rose:	move.l	#m_size,d0		taille mémoire à liberer
		lea	first8(pc),a0
		move.l	(a0),a1			adresse mémoire à liberer
		move.l	4.w,a6
		jsr	freemem(a6)
close_quitte2	bra	zend_load2


;=-=-=-=-=-=-=-=-=-=-> fin routine charge map-editor <-=-=-=-=-=-=-=-=-=-=-








;==========================================================================
;		amigados : charge fichier IFF dalles 320*544*8
;==========================================================================
;
load_dalles:
;
;
;
;

;		- load dalles : revient sous le système multitache -

		bsr	sys_restore

;		- load dalles : met le screen workbench devant les autres -

		move.l	intuitionbase(pc),a6
		jsr	wbenchtofront(a6)

;		- load dalles : affiche requester, et attend sélection -

		lea	titre1(pc),a1
		bsr	requester		|-> ouvre choix + requester
		tst.l	d0
		beq	error2			-> "error"

;		- load dalles : ouvre le fichier dos en lecture -

		bsr	open_read
		move.l	d0,d1
		beq	error2			-> "error"

;		- load dalles : charge les 8 premiers octets -

		lea	first8(pc),a0
		clr.l	(a0)
		move.l	a0,d2			adresse chargement
		moveq	#8,d3			charge 8 premiers octets
		move.l	dosbase(pc),a6
		jsr	-42(a6)			|-> charge 8 premiers fichier

;		- load dalles : test si ficher IFF -

		lea	first8(pc),a0
		cmp.l	#'FORM',(a0)+
		bne	close_quitte		-> pas fichier IFF

;		- load dalles : réserve mémoire nécessaire -

		move.l	(a0),d0
		moveq	#0,d1
		move.l	4.w,a6
		jsr	allocmem(a6)
		tst.l	d0
		beq	close_quitte		-> pas assez de mémoire
		lea	first8(pc),a0
		move.l	d0,(a0)			sauve adresse mémoire

;		- load dalles : charge fichier -

		move.l	hondle(pc),d1
		move.l	d0,d2			adresse chargement
		move.l	4(a0),d3		taille fichier
		move.l	dosbase(pc),a6
		jsr	-42(a6)			|-> charge fichier

;		- load dalles : charge fichier -

		lea	first8(pc),a0
		move.l	(a0)+,a4		adresse mémoire fichier IFF
		move.l	(a0),d7			taille fichier

;		- load dalles : test si fichier IFF graphique - 

		cmp.l	#'ILBM',(a4)		fichier graphique ? 
		bne	fig_error		-> non




;--------------------------------------------------------------------------
;		load dalles :  analyse le chunk 'BMHD'
;--------------------------------------------------------------------------

;		- BMHD : cherche le chunk 'BMHD' - 

		move.l	a4,a0			début du fichier
		lea	(a4,d7.l),a1		fin du fichier
		move.l	#'BMHD',d1		
		moveq	#2,d2
bmhd1:		cmp.l	a1,a0			fin du fichier ?
		bge	fig_error		oui erreur
		cmp.l	(a0),d1			identificateur ?
		beq.s	bmhd2			oui, ok
		add	d2,a0			prochain long mot
		bra.s	bmhd1

;		- BMHD : init. regsitres - 

bmhd2		addq	#8,a0			saute nom & taille chunk
		moveq	#0,d0
		moveq	#0,d1
		moveq	#0,d2
		moveq	#0,d6

;		- BMHD : lit hauteur & largeur de l'image en pixel -

		move	(a0)+,d0		largeur image
		move	(a0)+,d1		hauteur image
		cmp	#d_tplan*8,d0
		bne	fig_error		-> largeur incorrect
		cmp	#d_nligne,d1
		bne	fig_error		-> hauteur incorrect

;		- BMHD : lit le nombre de plan -

		addq	#4,a0			saute la position écran
		move.b	(a0)+,d2		nombre de plans
		cmp	#d_nplan,d2
		bne	fig_error		-> nombre plan incorrect

;		- BMHD : lit le type de compression -

		addq	#1,a0			saute le type de masque
		move.b	(a0)+,d6		type de compression
		cmp	#1,d6
		bhi	fig_error		compression iconnue.
		addq	#1,a0			met sur une adresse paire.

;		- BMHD : calcul taille image en octet décompactée  -

		move	d0,d4
		lsr	#3,d0			convertit largeur en octet
		and	#$fffe,d0		octet paire
		and	#$fff0,d4		largeur multiple de 16 ?
		beq.s	juste			oui		
		addq	#2,d0			rajoute un mot
juste		mulu	d1,d0			taille d'un plan si raw normal
		mulu	d2,d0			taille image decompactée




;--------------------------------------------------------------------------
;		load dalles :  analyse le chunk 'CMAP'
;--------------------------------------------------------------------------

;		- CMAP : cherche le chunk -

		lea	(a4,d7.l),a1		fin du fichier
		move.l	#'CMAP',d1		
cmap1:		cmp.l	a1,a0			fin du fichier ?
		bge	fig_error		oui erreur
		cmp.l	(a0),d1			identificateur ?
		beq.s	cmap2			oui, ok
		addq	#2,a0			prochain long mot
		bra.s	cmap1
cmap2		addq	#8,a0			saute nom et taille chunck

;		- CMAP : recopie les couleurs -

		lea	coul(pc),a3
		lea	color-coul,a2		table couleur	
		add.l	a3,a2
		moveq	#8-1,d0			8 couleurs
coul		moveq	#0,d2
		moveq	#0,d3
		moveq	#0,d4
		move.b	(a0)+,d2		valeur du rouge
		move.b	(a0)+,d3	 	valeur du vert
		move.b	(a0)+,d4 		valeur du bleu
		lsl	#4,d2
		lsr 	#4,d4
		add	d2,d4
		add	d3,d4
		move	d4,(a2)+	 	couleur exacte
		dbf	d0,coul




;--------------------------------------------------------------------------
;		load dalles :  analyse le chunk 'BODY'
;--------------------------------------------------------------------------

;		- BODY : cherche le chunk body -=-

		move.l	#'BODY',d1
		lea	(a4,d7.l),a1		fin du fichier
body1:		cmp.l	a1,a0			fin du fichier ?
		bge	fig_error		oui erreur
		cmp.l	(a0),d1			identificateur ?
		beq.s	body2			oui, ok
		addq.l	#2,a0			prochain long mot
		bra.s	body1

;		- BODY : décompactage en raw blitter -

body2		addq	#4,a0			saute nom du chunk
		move.l	(a0)+,d0		conserve taille du chunck
		lea	body2(pc),a2
		lea	adalles-body2,a3
		add.l	a2,a3
		move.l	(a3),a3			! adresse décompactage !
		moveq	#1,d3
		moveq	#2,d2

;		- BODY : test si compactage ByteRun1 -

		tst	d6			fichier compacté ByteRun1 ?
		beq	nocrunch		-> non

;		- BODY : ByteRun1 ; loop ; lit octet de commande -

decomp:		moveq	#0,d1
		move.b	(a0)+,d1		octet de commande
		bmi.s	duplication		

;		- BODY : ByteRun1 ; loop ; copie simple des 'd1' octets suivants -	

copie:		move.b	(a0)+,(a3)+
		sub.l	d3,d0
		dbf	d1,copie	
		sub.l	d3,d0
		bne.s	decomp
		bra.s	nnn_error

;		- BODY : ByteRun1 ; loop ; duplique 'd1' fois l'octet suivant -

duplication	neg.b	d1
dupli2		move.b	(a0),(a3)+
		dbf	d1,dupli2
		add.l	d3,a0
		sub.l	d2,d0
		bne.s	decomp
		bra.s	nnn_error		-> fin décompactage

;		- BODY : NoCrunch ; loop ; fichier non compacté -

nocrunch	move.b	(a0)+,(a3)+
		sub.l	d3,d0
		bne.s	nocrunch
nnn_error	move	#1,flag_load-tlk(a5)	flag chargement éffectué




;--------------------------------------------------------------------------
;		load dalles :  libère mémoire et ferme fichier
;--------------------------------------------------------------------------

;		- load dalles : libère la mémoire -

fig_error	move.l	4.w,a6
		lea	first8(pc),a0
		move.l	(a0)+,a1		adresse mémoire à liberer
		move.l	(a0),d0			taille mémoire à libérer
		jsr	freemem(a6)		|-> libère cette mémoire

;		- load dalles : ferme le fichier + wait 3 secondes -

close_quitte	bra	zend_load2
first8		dc.l	0,0


;-=-=-=-=-=-=-=-=-=-> fin routine charge fichier IFF <-=-=-=-=-=-=-=-=--=-=








;==========================================================================
;		disque : requester fichiers selectionne le fichier
;==========================================================================
; entrée	a1	pt/titre fenêtre
;
requester:
;
;
;
;
		lea	filereq(pc),a0		structure requester
		move.l	a1,fr_titre(a0)		titre pret
		lea	file(pc),a1
		move.l	a1,fr_file(a0)		tampons
		lea	dir(pc),a1
		move.l	a1,fr_dir(a0)		divers
		moveq	#0,d0
		move.l	d0,fr_window(a0)	si ouvert dans fenetre active
		move.w	d0,fr_flags(a0)
		move.w	d0,fr_reserved1(a0)
		move.l	d0,fr_fonction(a0)
		move.l	d0,fr_reserved2(a0)
		move.l	arpbase(pc),a6
		jsr	-$126(a6)		envoie le requester
		tst.l	d0			d0=0 si erreur ou 'cancel'
		beq	.termine

;		- recopie le nom -

		lea	dir(pc),a0
		lea	nom_complet(pc),a1
		tst.b	(a0)
		beq.s	.fcopi			pas de dir
.bcle1		move.b	(a0)+,(a1)+
		tst.b	(a0)
		bne.s	.bcle1
		cmpi.b	#":",-1(a1)
		beq.s	.fcopi
		cmpi.b	#"/",-1(a1)
		beq.s	.fcopi
		move.b	#"/",(a1)+		ajoute un / avant nom de fichier					; si c'est necessaire
.fcopi		lea	file(pc),a0
		tst.b	(a0)
		beq.s	.termine		pas de fichier
.bcle2		move.b	(a0)+,(a1)+
		tst.b	(a0)
		bne.s	.bcle2
		move.b	#0,(a1)			+0 comme butee dans nom_complet
.termine	rts
		rsreset
filerequester	rs.b	0
fr_titre	rs.l	1		adr. tampon entete requester
fr_file		rs.l	1		adr. tampon nom de fichier
fr_dir		rs.l	1		adr. tampon nom du chemin
fr_window	rs.l	1		adr. struct. fenetre intuition ou 0
fr_flags	rs.w	1		flags
fr_reserved1	rs.w	1		a fixer a 0
fr_fonction	rs.l	1		adresse utilise pour certaine cartes
fr_reserved2	rs.l	1		a fixer a 0
fr_sizeof	rs.w	0		pour la taille de la structure
filereq		ds.b	fr_sizeof
titre1		dc.b	'LOAD IFF 320*544*8',0
titre2		dc.b	'LOAD map-editor',0
titre3		dc.b	'SAVE map-editor',0
		even
file		ds.b	200		pour le fichier
dir		ds.b	400		pour le directory
nom_complet	ds.b	600		pour le nom complet
handle		dc.l	0
taille_final	dc.l	0
hondle		dc.l	0


;=-=-=-=-> fin fichier requester fichiers selectionne le fichier <-=-=-=-=-







;==========================================================================
;		disque : ouvre un fichier en lecture
;==========================================================================
;
open_read:	
;
;
;
;
		lea	nom_complet(pc),a0
		move.l	a0,d1
		move.l	#1005,d2
		move.l	dosbase(pc),a6
		jsr	-30(a6)			|-> ouvre fichier
		lea	hondle(pc),a0
		move.l	d0,(a0)
		rts


;-=-=-=-=-=-=-=-=-=-=-> ouvre un fichier en lecture <-=-=-=-=-=-=-=-=-=-=-








;==========================================================================
;		disque : ouvre un fichier en écriture
;==========================================================================
;
open_write:
;
;
;
;
		lea	nom_complet(pc),a0
		move.l	a0,d1
		move.l	#1006,d2
		move.l	dosbase(pc),a6
		jsr	-30(a6)			ouvre fichier
		lea	hondle(pc),a0
		move.l	d0,(a0)
		rts


;-=-=-=-=-=-=-=-=-=-=-> ouvre un fichier en écriture <-=-=-=-=-=-=-=-=-=-=-








;==========================================================================
;		disque : chargement + pause
;==========================================================================
; entrée	d1.l	handle
;		d2.l	adresse de chargement
;		d3.l	taille du fichier
;
zend_load:	
;
;
;
;	
;		- charge fichier amigados -

		move.l	dosbase(pc),a6
		jsr	-42(a6)			|-> charge fichier
zend_load2	move.l	dosbase(pc),a6
		move.l	hondle(pc),d1
		jsr	-36(a6)			|-> ferme fichier

;		- détourne VBL -

zl		lea	$6c.w,a2
		lea	zl(pc),a1
		lea	zair+2-zl,a0
		add.l	a1,a0
		move.l	(a2),(a0)		sauve vecteur
		lea	zend_irq-zl,a0
		add.l	a1,a0
		move.l	a0,d0
		move.l	d0,(a2)

;		- pause de 3 secondes -

		lea	waitdisk-zl,a0
		add.l	a1,a0
		move	#50*3,(a0)		pause de 3 sec.
pas_elegant2	tst	(a0)
		bne.s	pas_elegant2
		bra	quitte


;-=-=-=-=-=-=-=-=-=-=-=-> fin chargement + pause <-=-=-=-=-=-=-=-=-=-=-=-=-








;==========================================================================
;		disque : sauvegarde + pause
;==========================================================================
; entrée	d2.l	adresse de chargement
;		d3.l	taille du fichier
;
zend_save:	
;
;
;
;
;		- sauve fichier amigados -

		move.l	dosbase(pc),a6
		jsr	-48(a6)			écrit fichier
		move.l	hondle(pc),d1
		jsr	-36(a6)			ferme fichier

;		- détourne VBL -

		lea	$6c.w,a2
		lea	zl(pc),a1
		lea	zair+2-zl,a0
		add.l	a1,a0
		move.l	(a2),(a0)
		lea	zend_irq-zl,a0
		add.l	a1,a0
		move.l	a0,d0
		move.l	d0,(a2)

;		- pause de 8 secondes -

		lea	waitdisk-zl,a0
		add.l	a1,a0
		move	#50*8,(a0)		pause de 8 sec.
pas_elegant	tst	(a0)
		bne.s	pas_elegant


;-=-=-=-=-=-=-=-=-=-=-=-> fin sauvegarde + pause <-=-=-=-=-=-=-=-=-=-=-=-=-








;==========================================================================
;		disque : restitue vecteur de niveau 3
;==========================================================================
;
quitte:		
;
;
;
;
;		- quitte : restitue vecteur d'interruption -

		lea	quitte(pc),a1
		lea	zair+2-quitte,a0
		add.l	a1,a0			pt/adresse ancien vecteur
		lea	$6c.w,a2		pt/vecteur niveau 3
		move.l	(a0),d0			lit adresse ancien vecteur
		move.l	d0,(a2)			restitue ancien vecteur

;		- quitte : gèle le système - 

error2:		bsr	sys_save

;		- quitte : charge adresse de la table 'tlk' dans 'A5' -

		lea	quitte(pc),a0
		lea	tlk-quitte,a5
		add.l	a0,a5

;		- quitte : quitte raz la table clavier -

		lea	clara-tlk(a5),a0
		moveq	#0,d1
		move.l	d1,-4(a0)
		moveq	#127,d0
geulante	move.b	d1,(a0)+
		dbf	d0,geulante

;		- quitte : charge base des customs chips dans A6 -

		lea	$dff000,a6

;		- quitte : détourne vecteur de niveau 3 -

		lea	irq_level3(pc),a0
		move.l	a0,$6c.w

;		- quitte : détourne vecteur de niveau 2 -

		lea	irq_level2(pc),a0
		move.l	a0,$68.w

;		- quitte : autorise interruption vbl & clavier -

		move	#$c028,intena(a6)

;		- quitte : autorise dmas, blitter, plans, copper, lutins -

		move	#$83e0,dmacon(a6)
		rts


;=-=-=-=-=-=-=-=-=-=-=-=-> fin retour au système <-=-=-=-=-=-=-=-=-=-=-=-=-
;
;
;
;
;+-+-+-+-+-+-+-+-+-> fin des routines disques AmigaDos <-+-+-+-+-+-+-+-+-+-








;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
; >*<		editeur de dalles
;
;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
; entrée	a5.l	adr. table 'tlk'
;		a6.l	$dff000
;
mysterious:
;
;
;
;
;--------------------------------------------------------------------------
;		ed dalles : flip les listes copper
;--------------------------------------------------------------------------
		tst	(a5)
		beq	gx
		clr	(a5)
		move.l	zcopper1-tlk(a5),a0
		move	aecran1-tlk(a5),mmecran-tlk(a5)
		bra.s	go_xmen
gx		move	#4,(a5)
		move.l	zcopper2-tlk(a5),a0
		move	aecran2-tlk(a5),mmecran-tlk(a5)
go_xmen		move.l	a0,sc_copper-tlk(a5)
		move.l	a0,cop1lch(a6)




;--------------------------------------------------------------------------
;		ed dalles : inc. position y 
;--------------------------------------------------------------------------
		moveq	#0,d2
		lea	flag_up-tlk(a5),a0
		lea	flag_down-tlk(a5),a1
		move	(a0),d0
		move	(a1),d1
		move	d2,(a0)
		move	d2,(a1)
		cmp	#$f,d0
		bls.s	mala
		moveq	#$f,d0
mala		move	d0,d7
		bne.s	go_scroll
		cmp	#$f,d1
		bls.s	mala2
		moveq	#$f,d1
mala2		move	d1,d7
		beq.s	go_scroll
		and	#$f,d7
		neg	d7




;--------------------------------------------------------------------------
;		ed dalles : test touche curseur haut/bas
;--------------------------------------------------------------------------
go_scroll	tst.l	d7			déplacement souris ?
		bne.s	go_scroll2		-> oui
		lea	clara+$4c-tlk(a5),a1
		bsr	testclavier
		tst.l	d0			touche curseur haut ?
		beq.s	.down			-> non
		moveq	#1,d7
		bra.s	go_scroll2
.down		lea	clara+$4d-tlk(a5),a1
		bsr	testclavier
		tst.l	d0			touche curseur bas ?
		beq.s	go_scroll2		-> non
		moveq	#-1,d7



;--------------------------------------------------------------------------
;		ed dalles : gestion du défilement vertical
;--------------------------------------------------------------------------
go_scroll2	*cmp.b	#$50,vhposr(a6)
		*blt.s	go_scroll
		*move	#$f00,color00(a6)
		bsr	scroll
		*move	#$000,color00(a6)


;--------------------------------------------------------------------------
;		ed dalles : restaure decor ligne horizontale 1
;--------------------------------------------------------------------------
		move	(a5),d6
		lea	ed_hori1_adr-tlk(a5),a0
		move.l	(a0,d6),d0
		bmi.s	madonna1
		move.l	#-1,(a0,d6)
		lea	zecran1-tlk(a5),a0
		move.l	(a0,d6),d7			adr. écran aff.
		move.l	zecran3-tlk(a5),d1		adr. 3ieme écran
		move.l	d1,a1
		move.l	d7,a2
		add.l	d0,a1
		add.l	d0,a2
		moveq	#19,d0
indra		move	80(a1),80(a2)
		move	40(a1),40(a2)
		move	(a1)+,(a2)+
		dbf	d0,indra


;--------------------------------------------------------------------------
;		ed dalles : restaure decor ligne verticale 1
;--------------------------------------------------------------------------
madonna1	lea	ed_vert1_adr-tlk(a5),a0
		move.l	(a0,d6),d0
		bmi.s	madonna
		move.l	#-1,(a0,d6)
		lea	zecran1-tlk(a5),a0
		move.l	(a0,d6),d7			adr. écran aff.
		move.l	zecran3-tlk(a5),d1		adr. 3ieme écran
		move.l	d1,a1
		move.l	d7,a2
		add.l	d0,a1
		add.l	d0,a2
		moveq	#0,d2
		move	#s_tligne,d1
		move	#255+16,d0
indrab		move	80(a1),80(a2)
		move	40(a1),40(a2)
		move	(a1),(a2)
		add	d1,a1
		add	d1,a2
		dbf	d0,indrab


;--------------------------------------------------------------------------
;		ed dalles : restaure decor ligne horizontale 2
;--------------------------------------------------------------------------
madonna		lea	ed_hori2_adr-tlk(a5),a0
		move.l	(a0,d6),d0
		bmi.s	madonna2
		move.l	#-1,(a0,d6)
		lea	zecran1-tlk(a5),a0
		move.l	(a0,d6),d7			adr. écran aff.
		move.l	zecran3-tlk(a5),d1		adr. 3ieme écran
		move.l	d1,a1
		move.l	d7,a2
		add.l	d0,a1
		add.l	d0,a2
		lea	ed_long2_adr-tlk(a5),a0
		move	(a0,d6),d0
indra2		move	80(a1),80(a2)
		move	40(a1),40(a2)
		move	(a1)+,(a2)+
		dbf	d0,indra2

;--------------------------------------------------------------------------
;		ed dalles : restaure decor ligne horizontale 2
;--------------------------------------------------------------------------
madonna2	lea	ed_hori3_adr-tlk(a5),a0
		move.l	(a0,d6),d0
		bmi.s	madonna3
		move.l	#-1,(a0,d6)
		lea	zecran1-tlk(a5),a0
		move.l	(a0,d6),d7			adr. écran aff.
		move.l	zecran3-tlk(a5),d1		adr. 3ieme écran
		move.l	d1,a1
		move.l	d7,a2
		add.l	d0,a1
		add.l	d0,a2
		lea	ed_long3_adr-tlk(a5),a0
		move	(a0,d6),d0
indra3		move	80(a1),80(a2)
		move	40(a1),40(a2)
		move	(a1)+,(a2)+
		dbf	d0,indra3




;--------------------------------------------------------------------------
;		ed dalles : calcul fenêtre blitter
;--------------------------------------------------------------------------
madonna3	move	block_x2-tlk(a5),d0
		and	#$fff0,d0
		bne.s	cal1
		moveq	#16,d0
cal1		move	d0,dalle_large-tlk(a5)	sauve largeur en pixel
		move	block_y2-tlk(a5),d1
		and	#$fff0,d1
		bne.s	cal2
		moveq	#16,d1
cal2		move	d1,dalle_haut-tlk(a5)	sauve hauteur en ligne
		lsl	#6,d1			hauteur * 64
		lsr	#4,d0			convertit largeur en mot
		add	d0,d1			fenêtre blitter
		move	d1,d_window-tlk(a5)	sauve fenêtre blitter




;--------------------------------------------------------------------------
;		ed dalles : réaffiche décor sous dalle
;--------------------------------------------------------------------------
		tst	stop_mouse-tlk(a5)	stop test souris ?
		beq.s	go_reaff
		addq	#1,stop_mouse-tlk(a5)
		move	stop_mouse-tlk(a5),d1
		move.l	aecran3-tlk(a5),d0
		cmp	#4,d1
		bne.s	non_44
		move.l	d0,aecran1-tlk(a5)
non_44		cmp	#6,d1
		bne.s	non_45
		move.l	d0,aecran2-tlk(a5)
non_45		cmp	#7,stop_mouse-tlk(a5)
		bls	no_exit
		clr	stop_mouse-tlk(a5)
		move.l	zecran1-tlk(a5),aecran1-tlk(a5)
		move.l	zecran2-tlk(a5),aecran2-tlk(a5)
		bra	no_exit




;--------------------------------------------------------------------------
;		ed dalles : réaffiche décor sous dalle
;--------------------------------------------------------------------------
go_reaff	bsr	reaff_decor_dalle



;--------------------------------------------------------------------------
;		ed dalles : calcul adr. et ligne horizontale 1
;--------------------------------------------------------------------------

		tst	ed_flag_bloc-tlk(a5)	prise de brush ?
		beq	nikita3			-> non

;		-=- raz flag de reaffichage des decors -=-

		moveq	#0,d4
		move.l	d4,d_wdw-tlk(a5,d6)	eff 2 fenêtres blitter ecran


		cmp	#1,ed_flag_bloc-tlk(a5)	1ere prise ?
		bne	nikita1			-> non


;--------------------------------------------------------------------------
;		ed dalles : calcul adr. et affiche H1 et V1
;--------------------------------------------------------------------------
		bra	first_brush

;--------------------------------------------------------------------------
;		ed dalles : le joueur commence à faire sa brush
;--------------------------------------------------------------------------
nikita1:	cmp	#1,flag_souris-tlk(a5)	bouton gauche ?
		bne	nikita2			-> non


;--------------------------------------------------------------------------
;		ed dalles : le joueur est en train de faire sa brush
;--------------------------------------------------------------------------
		bra	make_brush



;--------------------------------------------------------------------------
;		ed dalles : calcul la longeur et hauteur de la brush
;--------------------------------------------------------------------------
nikita2:	move	#0,ed_flag_bloc-tlk(a5)
		move	ed_blocx2-tlk(a5),d0
		sub	ed_blocx1-tlk(a5),d0
		move	d0,block_x2-tlk(a5)		sauve largeur en pixel
		move	ed_blocy2-tlk(a5),d0
		and	#$fff0,d0
		move	ed_blocy1-tlk(a5),d1
		and	#$fff0,d1
		sub	d1,d0
		move	d0,block_y2-tlk(a5)		sauve hauteur en ligne


;--------------------------------------------------------------------------
;		ed dalles : sauve le map-brush dans le buffer
;--------------------------------------------------------------------------
		move	block_x2-tlk(a5),d0
		and	#$fff0,d0
		bne.s	yal1
		moveq	#16,d0
yal1		move	d0,dalle_large-tlk(a5)		sauve largeur en pixel
		move	block_y2-tlk(a5),d1
		and	#$fff0,d1
		bne.s	yal2
		moveq	#16,d1
yal2		move	d1,dalle_haut-tlk(a5)		sauve hauteur en ligne
		bsr	prends_map
		bra	set_brush



;--------------------------------------------------------------------------
;		ed dalles : si sortie demandée, n'aff. plus la dalle
;--------------------------------------------------------------------------
nikita3		tst	flag_sddd-tlk(a5)	demande sélecteur de dalles ?
		beq.s	no_exit			-> non
		addq	#1,flag_sddd-tlk(a5)
		cmp	#4,flag_sddd-tlk(a5)	3 vbl passé ?
		blt	ms_exit			-> non, sort
		clr	flag_sddd-tlk(a5)
		clr	flag_prg-tlk(a5)	>flag sélecteur dalles<
		bra	ms_exit			-> sort




;--------------------------------------------------------------------------
;		ed dalles : affiche dalle sur l'écran de travail
;--------------------------------------------------------------------------
no_exit		bsr	aff_dalle




;--------------------------------------------------------------------------
;		ed dalles : test bouton souris, rentre nouvelles dalles
;--------------------------------------------------------------------------
		cmp	#1,flag_souris-tlk(a5)	bouton gauche ?
		bne	no_mouse		-> non
		clr	flag_souris-tlk(a5)
		move	#1,stop_mouse-tlk(a5)	stop test souris
		move	#0,ed_flag_bloc-tlk(a5)	raz flag : select brush
		bsr	insert_map




;--------------------------------------------------------------------------
;		ed dalles : test si passe dans sélecteur de dalles
;--------------------------------------------------------------------------
no_mouse	lea	clara-4-tlk(a5),a0
		move	(a0),d0			lit code clavier raw
		cmp	#$26,d0			touche 'J' ?
		bne.s	martika
		clr	(a0)			raz code clavier
		clr	flag_eddd-tlk(a5)	raz flag demande ed de dalles
		move	#1,flag_sddd-tlk(a5)	demande sélecteur de dalles

;		remets la bonne taille de la brush

		move	sblock_x2-tlk(a5),block_x2-tlk(a5)
		move	sblock_y2-tlk(a5),block_y2-tlk(a5)

		bra.s	ms_exit




;--------------------------------------------------------------------------
;		sélecteur : test si prend une brosse [B]
;--------------------------------------------------------------------------
martika		cmp	#$35,d0			touche B ?
		bne.s	no_b
		clr	(a0)
		move	#1,ed_flag_bloc-tlk(a5)




;--------------------------------------------------------------------------
;		sélecteur : test si charge map-edior touche 'S'
;--------------------------------------------------------------------------
no_b		cmp	#$21,d0
		bne.s	no_s
		clr	(a0)			raz code clavier
		bsr	save_map
		bsr	sc_first1		|-> init. reg. vidéos




;--------------------------------------------------------------------------
;		sélecteur : test si charge map-edior touche 'L'
;--------------------------------------------------------------------------
no_s		cmp	#$28,d0			touche 'L' ?
		bne.s	ms_exit
		clr	(a0)			raz code clavier

;		- charge map-editor -

		clr	flag_load-tlk(a5)	raz flag charg. échoué
		bsr	load_map

;		- remet regitres vidéos  -

		tst	flag_load-tlk(a5)	chargement à échoué ?
		bne.s	.no			-> non
		bsr	sc_first1		|-> init. reg. vidéos
		bra.s	ms_exit

;		- initialise le scroll -

.no		clr	flag_first-tlk(a5)
		bsr	scroll_init




;--------------------------------------------------------------------------
;		ed dalles : sort de la routine
;--------------------------------------------------------------------------
ms_exit:	rts
;
;
;
;
;+-+-+-+-+-+-+-+-+-+-+-+-> fin editeur de dalle <-+-+-+-+-+-+-+-+-+-+-+-+-+








;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
;	>*<	Routines appellées par 'mysterious'
;
;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
;
;
;
;
;
;
;
;==========================================================================
;		routine de scrolling
;==========================================================================
; entrée	d7.w	déplacement positif / négatif
;		a5.l	adr. table 'tlk'
;		a6.l	$dff000
;
scroll:
;
;
;
;
;--------------------------------------------------------------------------
;		scroll : recopie dans l'écran d'aff.
;--------------------------------------------------------------------------

;		- init. registres blitters -

		tst	flag_co-tlk(a5)		modification précédent écran ?
		beq.s	loupgarou		-> non
		lea	bltaptl(a6),a2
		lea	bltdptl(a6),a3
		lea	bltsize(a6),a4
		move	aecran3-tlk(a5),d0	mot fort adr. source  écran3
		move	mmecran-tlk(a5),d1	mot fort adr. desti.
		move	ffecran-tlk(a5),d2	mot faible adr. desti./source
		move	#s_tligne-s_tplan,d3	modulo A & D
		moveq	#-1,d4
		move	#$09f0,d5		combinaison blitter
		moveq	#0,d6
		move	d6,flag_co-tlk(a5)	raz flag modification
		waitblitter
		move.l	d4,bltafwm(a6)
		move	d5,bltcon0(a6)
		move	d6,bltcon1(a6)
		move	d3,bltamod(a6)
		move	d3,bltdmod(a6)
		move	d0,bltapth(a6)
		move	d1,bltdpth(a6)

;		- plan 1 -

		move	#16*64+(s_tplan/2),d3	fenêtre blitter
		move	d2,(a2)			bltaptl : source
		move	d2,(a3)			bltdptl : destination
		move	d3,(a4)			bltsize : fenêtre blitter

;		- plan 2 -

		moveq	#s_tplan,d4
		add	d4,d2
		waitblitter
		move	d2,(a2)			bltaptl : source
		move	d2,(a3)			bltdptl : destination
		move	d3,(a4)			bltsize : fenêtre blitter

;		- plan 3 -

		add	d4,d2
		waitblitter
		move	d2,(a2)			bltaptl : source
		move	d2,(a3)			bltdptl : destination
		move	d3,(a4)			bltsize : fenêtre blitter




;--------------------------------------------------------------------------
;		scroll : test s'il ya défilement de l'écran
;--------------------------------------------------------------------------
loupgarou	tst	d7
		beq	carbalas		-> pas de défilement




;--------------------------------------------------------------------------
;		scroll : calcul nouvelle ligne changement des pts vidéos
;--------------------------------------------------------------------------
		move	#s_nligne,d3		nombre de ligne de la page
		move	nu_video-tlk(a5),d1	n° ligne chg. pts vidéos
		add	d7,d1			+ ou - le déplacement
etra_006	tst	d1			dépassement en haut ?
		bpl.s	etra_007		-> pas de dépassement négatif
		add	d3,d1			oui, + s_nligne
		bra.s	etra_008
etra_007	cmp	d3,d1			dépassement en bas ?
		blt.s	etra_008		-> non
		sub	d3,d1			oui, - s_nligne
etra_008	move	d1,nu_video-tlk(a5)	sauve nouveau n° ligne chg.




;--------------------------------------------------------------------------
;		scroll : calcul adr. 1er ligne balayée
;--------------------------------------------------------------------------
		beq.s	etra_nul		-> pas de changement pt vidéo, 1er ligne balayee = 1er ligne de la page
		sub	#s_nligne,d1
		neg	d1			nbre de ligne aprés changement
		mulu	#s_tligne,d1
etra_nul	move	d1,afline-tlk(a5)




;--------------------------------------------------------------------------
;		scroll : test s'il ya réaff. de nouvelles dalles
;--------------------------------------------------------------------------
		move.l	asc_map-tlk(a5),d1	adr. 1er ligne map-editor 
		move.l	asc_end-tlk(a5),d2	adr. dernière ligne map-editor
		moveq	#m_large*2,d3		taille ligne map-editor
		lea	sc_map1-tlk(a5),a1	pt/1er ligne écran map-editor




;--------------------------------------------------------------------------
;		scroll : met à jour la pos. y sur la page écran (0 à 271)
;--------------------------------------------------------------------------
		move	sc_y1-tlk(a5),d0	lit pos. y sur la page écran
		move	d0,d4
		sub	d7,d4			met à jour
		bpl.s	non_neg			-> pas de dépassement négatif
		add	#s_nligne,d4
non_neg		cmp	#s_nligne,d4
		blt.s	non_dep			pas de dépassement positif
		sub	#s_nligne,d4
non_dep		move	d4,sc_y1-tlk(a5)	sauve nouvelle position y




;--------------------------------------------------------------------------
;		scroll : test si apparition de nouvelle dalles
;--------------------------------------------------------------------------
		move	d4,d5
		lsr	#4,d0			divise par 16
		lsr	#4,d5			divise par 16
		cmp	d5,d0			reaff. de dalles ?
		beq	carbalas		-> non
		tst	d7			dalles apparaissent en bas ?
		bmi	carba			-> oui




;--------------------------------------------------------------------------
;		scroll : test si apparition de nouvelle dalles en haut
;--------------------------------------------------------------------------

;		- déc. pointeur map-editor d'une seule ligne -

		cmp.l	(a1),d1			1er ligne ?
		bne.s	no_fzi			-> non
		move.l	d2,(a1)			oui, pointe dernière ligne
no_fzi		sub.l	d3,(a1)
		move.l	(a1),a0

;		- calcul adr. d'affichage -

		lsl	#4,d5
		mulu	#s_tligne,d5
		move	d5,d0			adr. relative d'aff.
		bra.s	sc_affiche




;--------------------------------------------------------------------------
;		scroll : test si apparition de nouvelle dalles en haut
;--------------------------------------------------------------------------

;		- inc. pointeur map-editor d'une seule ligne -

carba		add.l	d3,(a1)
		cmp.l	(a1),d2			dernère ligne ?
		bne.s	no_fzy			-> non
		move.l	d1,(a1)			oui, pointe 1er ligne
no_fzy		move.l	(a1),a0

;		- les dalles apparaissent en bas additionne 16 lignes -

		lea	m_large*2*((s_nligne-16)/16)(a0),a0
		cmp.l	d2,a0			dépassement ?
		blt.s	no_dep3			-> non
		sub.l	#m_large*2*m_hauteur,a0

;		- calcul adr. d'affichage -

no_dep3		add	#s_nligne-16,d4
		cmp	#s_nligne,d4		dépassement ?  
		blt.s	non_dep2		-> non
		sub	#s_nligne,d4
non_dep2	and	#$fff0,d4		multiple de 16
		mulu	#s_tligne,d4
		move	d4,d0			adr. relative écran




;--------------------------------------------------------------------------
;		scroll : init. pour aff. des 20 dalles dans l'écran 3 *GFX*
;--------------------------------------------------------------------------

;		- init. regsitres blitters -

sc_affiche	moveq	#-1,d1
		move	#$09f0,d2		combinaison blitter
		move	#s_tligne-2,d3		modulo D	
		move	#d_tligne-2,d4		modulo A
		move	aecran3-tlk(a5),d5
		moveq	#0,d6
		waitblitter
		move.l	d1,bltafwm(a6)
		move	d2,bltcon0(a6)
		move	d6,bltcon1(a6)
		move	d3,bltdmod(a6)
		move	d4,bltamod(a6)
		move	d5,bltdpth(a6)

;		- init. registre MC68000 -

		lea	sc_mapadr-tlk(a5),a1	adr. table adr. des dalles
		lea	bltapth(a6),a2
		lea	bltdptl(a6),a3
		lea	bltsize(a6),a4
		lea	dmaconr(a6),a6
		move.l	adalles-tlk(a5),d1	adr. de la page dalle
		move	#16*64+1,d3		fenêtre blitter
		moveq	#s_tplan*2-2,d4
		moveq	#d_tplan,d5		taille d'un plan (page dalle)
		moveq	#s_tplan,d6		taille d'un plan (écran)
		moveq	#14,d7			bit "bbusy" de "dmaconr"
		move	d0,ffecran-tlk(a5)



;--------------------------------------------------------------------------
;		scroll : aff. les 20 dalles de 16*16		      *GFX*
;--------------------------------------------------------------------------
		rept	m_large			nbre de dalle en largeur
		move	(a0)+,d2		pt/adr. page dalle
		move.l	(a1,d2),d2		adr. de la dalle à afficher
		add.l	d1,d2			adr. de la page dalle

;		- plan 1 -

		wait_blitter
		move.l	d2,(a2)			bltapth : source
		move	d0,(a3)			bltdptl : destination
		move	d3,(a4)			bltsize : fenêtre blitter

;		- plan 2 -

		add.l	d5,d2
		add	d6,d0
		wait_blitter
		move.l	d2,(a2)			bltapth : source
		move	d0,(a3)			bltdptl : destination
		move	d3,(a4)			bltsize : fenêtre blitter

;		- plan 3 -

		add.l	d5,d2
		add	d6,d0
		wait_blitter
		move.l	d2,(a2)			bltapth : source
		move	d0,(a3)			bltdptl : destination
		move	d3,(a4)			bltsize : fenêtre blitter
		sub	d4,d0
		endr
		lea	$dff000,a6




;--------------------------------------------------------------------------
;		scroll : recopie dans l'écran d'aff.
;--------------------------------------------------------------------------

;		- init. regsitres blitters -

ff1		addq	#2,a2
		move	aecran3-tlk(a5),d0	mot fort adr. source  écran3
		move	mmecran-tlk(a5),d1	mot fort adr. desti.
		move	ffecran-tlk(a5),d2	mot faible adr. desti./source
		move	#1,flag_co-tlk(a5)	flag 1 = modification d'un écran
		move	#16*64+(s_tplan/2),d3	fenêtre blitter
		moveq	#s_tplan,d4
		move	#s_tligne-s_tplan,d6
		waitblitter
		move	d6,bltamod(a6)
		move	d6,bltdmod(a6)
		move	d0,bltapth(a6)
		move	d1,bltdpth(a6)

;		- plan 1 -

		move	d2,(a2)			bltaptl : source
		move	d2,(a3)			bltdptl : destination
		move	d3,(a4)			bltsize : fenêtre blitter

;		- plan 2 -

		add	d4,d2
		waitblitter
		move	d2,(a2)			bltaptl : source
		move	d2,(a3)			bltdptl : destination
		move	d3,(a4)			bltsize : fenêtre blitter

;		- plan 3 -

		add	d4,d2
		waitblitter
		move	d2,(a2)			bltaptl : source
		move	d2,(a3)			bltdptl : destination
		move	d3,(a4)			bltsize : fenêtre blitter




;--------------------------------------------------------------------------
;		scroll : calcul adr. 1er pointeurs vidéos
;--------------------------------------------------------------------------
carbalas	move.l	sc_copper-tlk(a5),a4
		lea	sc_u1+6-sc_c1(a4),a0	adr. liste copper
		moveq	#s_tplan,d1		taille d'un plan
		move	afline-tlk(a5),d0	adr. 1er ligne balayée
		rept	s_nplan
		move	d0,(a0)
		addq	#8,a0
		add	d1,d0
		endr




;--------------------------------------------------------------------------
;		scroll : test si changement des pointeurs vidéos
;--------------------------------------------------------------------------
		lea	sc_w1-sc_c1(a4),a1
		move.l	#$fffffffe,(a1)
		move	nu_video-tlk(a5),d1	n° ligne chg. pts vidéos
		beq	scroll_exit		-> pas de chg. pts vidéos
		cmp	#s_nligne-16,d1
		bge.s	scroll_exit		-> pas de chg. pts vidéos




;--------------------------------------------------------------------------
;		scroll : test si le n° "wait" est > 255
;--------------------------------------------------------------------------
		move.l	#$01800000,(a1)
		add.b	#s_rasty,d1
		bcc.s	cancras
		move.l	#$ffe1fffe,(a1)




;--------------------------------------------------------------------------
;		scroll : écris ordonnée du "wait"
;--------------------------------------------------------------------------
cancras		move.b	d1,4(a1)




;--------------------------------------------------------------------------
;		scroll : sort de la routine
;--------------------------------------------------------------------------
scroll_exit	rts


;-=-=-=-=-=-=-=-=-=-> fin de la routine de défilement <-=-=-=-=-=-=-=-=-=-=



;--------------------------------------------------------------------------
;		ed dalles : calcul adr. et affiche H1 et V1
;--------------------------------------------------------------------------
first_brush	moveq	#0,d0
		moveq	#0,d1
		moveq	#7,d3
		lea	zecran1-tlk(a5),a0
		move.l	(a0,d6),d7		adr. 'ecran1' ou 'ecran2'

		moveq	#0,d0			pos. x
		move	souris_y-tlk(a5),d1	pos. y
		cmp	#320-32,d1
		ble.s	abiale
		move	#320-32,d1
abiale		and	#$fff0,d1
		move	nu_video-tlk(a5),d5	pos. y changement pts vidéos
		move	d5,d2
		and	#$f,d2
		add	d2,d1
abial		cmp	d5,d1			1er ligne > y chang. pt vidéo
		bge.s	ap_bas			-> affiche 2ieme partie
		mulu	#s_tligne,d1
		add	afline-tlk(a5),d1	y = y + adr. 1er ligne balayée
		bra.s	ap_go
ap_bas		sub	d5,d1
		mulu	#s_tligne,d1
ap_go		move.l	d1,a0

		moveq	#-1,d2
		moveq	#$0,d1
		lsr	#3,d0	
		add	d0,a0	
		lea	ed_hori1_adr-tlk(a5),a1
		move.l	a0,(a1,d6)
		add.l	d7,a0			adresse. affichage
		moveq	#19,d0
indra1		move	d2,80(a0)
		move	d1,40(a0)
		move	d1,(a0)+
		dbf	d0,indra1


;--------------------------------------------------------------------------
;		first brush : calcul adr. et affiche ligne verticale 1
;--------------------------------------------------------------------------

		move	(a5),d6
		lea	zecran1-tlk(a5),a0
		move.l	(a0,d6),d7		adr. 'ecran1' ou 'ecran2'
		move	#s_tligne,d3
		moveq	#0,d0
		move	souris_x-tlk(a5),d0	pos. x
		and	#$fff0,d0
		lsr	#3,d0
		lea	ed_vert1_adr-tlk(a5),a0
		move.l	d0,(a0,d6)

		move.l	d7,a0			adresse. affichage
		add	d0,a0
		move	#255+16,d4
		move	#$8000,d2
		move	#$7fff,d1
indra1c		or	d2,80(a0)
		and	d1,40(a0)
		and	d1,(a0)
		add	d3,a0
		dbf	d4,indra1c

		cmp	#1,flag_souris-tlk(a5)	bouton gauche ?
		bne.s	indra_end		-> non

		move	#2,ed_flag_bloc-tlk(a5)	flag de 1ere prise
		move	souris_x-tlk(a5),d0
		and	#$fff0,d0
		move	d0,ed_blocx1-tlk(a5)
		add	#16,d0
		move	d0,ed_blocx2-tlk(a5)

		move	souris_y-tlk(a5),d0
		cmp	#320-32,d1
		ble.s	abiali
		move	#320-32,d1
abiali		move	d0,ed_blocy1-tlk(a5)
		and	#$fff0,d0
		move	nu_video-tlk(a5),d2	pos. y changement pts vidéos
		and	#$f,d2
		add	d2,d0
		add	#16,d0
		move	d0,ed_blocy2-tlk(a5)
indra_end	rts


;--------------------------------------------------------------------------
;		make brush : le joueur est en train de faire sa brush
;--------------------------------------------------------------------------
make_brush	move	souris_x-tlk(a5),d0

;		test si depassement en x

		cmp	ed_blocx1-tlk(a5),d0
		bge.s	nikita12
		move	ed_blocx1-tlk(a5),d0

nikita12	and	#$fff0,d0
		add	#16,d0
		move	d0,ed_blocx2-tlk(a5)


;		Calcul sur les y

		move	souris_y-tlk(a5),d0
		cmp	#320-32,d0
		ble.s	abialo
		move	#320-32,d0
abialo		move	d0,d2
		move	nu_video-tlk(a5),d1
		and	#$000f,d1
		add	d1,d0
		and	#$fff0,d0

;		test si depassement en y

		cmp	ed_blocy1-tlk(a5),d0
		bge.s	nikita13
		move	ed_blocy1-tlk(a5),d2
nikita13	add	#16,d2
		move	d2,ed_blocy2-tlk(a5)



;--------------------------------------------------------------------------
;		make brush : calcul adr. et affiche ligne horizontale 2
;--------------------------------------------------------------------------
		moveq	#0,d0
		moveq	#0,d1
		moveq	#7,d3
		move	(a5),d6
		lea	zecran1-tlk(a5),a0
		move.l	(a0,d6),d7		adr. 'ecran1' ou 'ecran2'
		move	nu_video-tlk(a5),d5	pos. y changement pts vidéos
		move	ed_blocy1-tlk(a5),d1	pos. y
		and	#$fff0,d1
		move	d5,d0
		and	#$000f,d0
		add	d0,d1
		move	ed_blocx1-tlk(a5),d0	pos. x
		cmp	d5,d1			1er ligne > y chang. pt vidéo
		bge.s	yp_bas			-> affiche 2ieme partie
		mulu	#s_tligne,d1
		add	afline-tlk(a5),d1	y = y + adr. 1er ligne balayée
		bra.s	yp_go
yp_bas		sub	d5,d1
		mulu	#s_tligne,d1
yp_go		move.l	d1,a0
		moveq	#-1,d2
		moveq	#$0,d1
		lsr	#3,d0	
		add	d0,a0	
		lea	ed_hori2_adr-tlk(a5),a1		sauve l'adresse
		move.l	a0,(a1,d6)
		add.l	d7,a0				adresse. affichage
		move	ed_blocx2-tlk(a5),d0
		sub	ed_blocx1-tlk(a5),d0
		lsr	#4,d0
		subq	#1,d0
		lea	ed_long2_adr-tlk(a5),a1		sauve la longeur
		move	d0,(a1,d6)
yndra1		or	d2,80(a0)
		and	d1,40(a0)
		and	d1,(a0)+
		dbf	d0,yndra1



;--------------------------------------------------------------------------
;		make brush : calcul adr. et affiche ligne horizontale 3
;--------------------------------------------------------------------------
		moveq	#0,d0
		moveq	#0,d1
		moveq	#7,d3
		move	(a5),d6
		lea	zecran1-tlk(a5),a0
		move.l	(a0,d6),d7		adr. 'ecran1' ou 'ecran2'
		move	nu_video-tlk(a5),d5	pos. y changement pts vidéos
		move	ed_blocy2-tlk(a5),d1	pos. y
		and	#$fff0,d1
		move	d5,d0
		and	#$000f,d0
		add	d0,d1
		move	ed_blocx1-tlk(a5),d0	pos. x
		cmp	d5,d1			1er ligne > y chang. pt vidéo
		bge.s	yp_bas2			-> affiche 2ieme partie
		mulu	#s_tligne,d1
		add	afline-tlk(a5),d1	y = y + adr. 1er ligne balayée
		bra.s	yp_go2
yp_bas2		sub	d5,d1
		mulu	#s_tligne,d1
yp_go2		move.l	d1,a0
		moveq	#-1,d2
		moveq	#$0,d1
		lsr	#3,d0	
		add	d0,a0	
		lea	ed_hori3_adr-tlk(a5),a1		sauve l'adresse
		move.l	a0,(a1,d6)
		add.l	d7,a0				adresse. affichage
		move	ed_blocx2-tlk(a5),d0
		sub	ed_blocx1-tlk(a5),d0
		lsr	#4,d0
		subq	#1,d0
		lea	ed_long3_adr-tlk(a5),a1		sauve la longeur
		move	d0,(a1,d6)
yndra2		or	d2,80(a0)
		and	d1,40(a0)
		and	d1,(a0)+
		dbf	d0,yndra2
		rts






;==========================================================================
;		ed dalles : test clavier + durée de préssion
;==========================================================================
; entreé	a1.l	pt/table clavier
; sortie	d0.l	0 = non pris en compte/ 1 = pris en compte
;
testclavier:	
;
;
;
;
		tst.b	(a1)			touche préssée ?
		beq.s	tc_no2			-> non
		cmp.b	#1,(a1)			1er préssion ?
		beq.s	tc_ok			-> oui
		cmp.b	#tc1,(a1)
		blt.s	tc_no			on lui laisse une chance
		beq.s	tc_ok
		cmp.b	#tc1+tc2,(a1)		end delay ?
		bne.s	tc_no
		move.b	#tc1-1,(a1)
tc_no		addq.b	#1,(a1)
tc_no2		moveq	#0,d0
		rts
tc_ok		addq.b	#1,(a1)
		moveq	#1,d0
		rts


;-=-=-=-=-=-=-=-> fin routine testant une touche clavier <-=-=-=-=-=-=-=-=-








;==========================================================================
;		ed dalles : aff. dalle
;==========================================================================
; entrée	a5.l	adr. table 'tlk'
;		a6.l	$dff000
;
aff_dalle:
;
;
;
;
;--------------------------------------------------------------------------
;		aff dalle : init. quelques registres
;--------------------------------------------------------------------------

		moveq	#0,d4
		move	(a5),d6			n° écran 0 ou 4
		move	dalle_haut-tlk(a5),d7	haut. dalle en ligne
		and	#$fff0,d7
		move.l	d4,d_wdw-tlk(a5,d6)	eff 2 fenêtres blitter




;--------------------------------------------------------------------------
;		aff dalle : affiche la dalle
;--------------------------------------------------------------------------
		move	aecran1-tlk(a5,d6),d0	mot fort adr. écran d'aff.
		move	#$09f0,d3		combinaison blitter
		move	dalle_large-tlk(a5),d5
		lsr	#3,d5
		and	#$fffe,d5
		move	d5,d1
		sub	#s_tligne,d5
		sub	#d_tligne,d1
		neg	d5			modulo D
		move	d5,d_mod-tlk(a5,d6)	sauve modulo D
		neg	d1			modulo A
		waitblitter
		move	d5,bltdmod(a6)
		move	d1,bltamod(a6)
		move	d3,bltcon0(a6)
		move	d4,bltcon1(a6)
		move	d0,bltdpth(a6)




;--------------------------------------------------------------------------
;		aff dalle : calcul positions x et y de la dalle
;--------------------------------------------------------------------------

;		- lit coordonnées -

		move	souris_x-tlk(a5),d0	abscisse souris
		move	souris_y-tlk(a5),d1	ordonnée souris

;		- test dépassement -

		move	dalle_large-tlk(a5),d2
		and	#$fff0,d2
		sub	#s_tplan*8,d2
		neg	d2
		cmp	d0,d2
		bhi.s	ada2
		move	d2,d0
ada2		move	d7,d2
		sub	#s_nligne-16,d2
		neg	d2
		cmp	d1,d2
		bhi.s	ada3
		move	d2,d1

;		- corrige pour que les pos. soient des multiples de 16 -

ada3		and	#$fff0,d0		pos. x pixel, multiple de 16
		move	sc_y1-tlk(a5),d2	pos. y sur la page écran 3
		and	#$000f,d2
		sub	#16,d2
		neg	d2
		and	#$fff0,d1		multiple de 16
		add	d2,d1

;		- sauve pos. (x,y) -

		move	d0,d_posx-tlk(a5)
		move	d1,d_posy-tlk(a5)

;		- convertit pos. x pixel en octet -

		lsr	#3,d0			convertit pos. x pixel en octet


;--------------------------------------------------------------------------
;		aff dalle : test si aff. en une fois 2ieme partie écran
;--------------------------------------------------------------------------
		move	nu_video-tlk(a5),d4	pos. y changement pts vidéos
		cmp	d4,d1			1er ligne > y chang. pt vidéo
		bge.s	aff_1_fois		-> affiche en une fois




;--------------------------------------------------------------------------
;		aff dalle : test si aff. en une fois 1ere partie écran
;--------------------------------------------------------------------------
		add	d7,d1			+ hauteur dalle = dernière ligne
		cmp	d4,d1			dern. ligne > y chang. pt vidéo
		bhi.s	aff_2_fois		-> affiche en 2 parties
		sub	d7,d1			pos. y 1er ligne
		bra.s	aff_1_foisb		-> affiche en une fois




;--------------------------------------------------------------------------
;		aff dalle : calcul affichage en 2 fois
;--------------------------------------------------------------------------

;		- paramètre 1er aff. -

aff_2_fois	move	d_window-tlk(a5),d5
		and	#$003f,d5		largeur fenetre blitter
		sub	d7,d1			pos. y 1er ligne
		sub	d1,d4			pos. chg. pts. videos. - pos. y = hauteur 1er aff.
		mulu	#s_tligne,d1
		add	afline-tlk(a5),d1	+1ere ligne balayée
		add	d0,d1			+pos. x en octet
		move	d1,d_adr-tlk(a5,d6)	sauve adr. relative d'aff.
		move	d4,d1
		lsl	#6,d1			*64
		add	d5,d1
		move	d1,d_wdw-tlk(a5,d6)	sauve fenêtre blitter

;		- paramètre 2ieme aff. -

		move	d0,2+d_adr-tlk(a5,d6)	sauve adr. relative d'aff.
		sub	d7,d4
		neg	d4
		lsl	#6,d4			*64
		add	d5,d4
		move	d4,2+d_wdw-tlk(a5,d6)	sauve fenêtre blitter
		bra.s	aff_go




;--------------------------------------------------------------------------
;		aff dalle : calcul affichage en 1 fois, 2ieme partie écran 
;--------------------------------------------------------------------------
aff_1_fois:	sub	nu_video-tlk(a5),d1	pos. y changement pts vidéos
		mulu	#s_tligne,d1
		add	d1,d0
		move	d0,d_adr-tlk(a5,d6)	sauve adr. relative d'aff.
		move	d_window-tlk(a5),d1
		move	d1,d_wdw-tlk(a5,d6)
		bra.s	aff_go




;--------------------------------------------------------------------------
;		aff dalle : calcul affichage en 1 fois, 1er partie écran 
;--------------------------------------------------------------------------
aff_1_foisb	mulu	#s_tligne,d1
		add	d1,d0
		add	afline-tlk(a5),d0	+1ere ligne balayée
		move	d0,d_adr-tlk(a5,d6)	sauve adr. relative d'aff.
		move	d_window-tlk(a5),d1
		move	d1,d_wdw-tlk(a5,d6)




;--------------------------------------------------------------------------
;		aff dalle : affiche la dalle
;--------------------------------------------------------------------------

;		- init registres 68000 -

aff_go:		moveq	#s_tplan,d5		taille d'un plan
		move.l	page_brush-tlk(a5),a0	adr. dalle à afficher (3 plans)
		move	d_adr-tlk(a5,d6),d1	adres. écran 1er aff.
		move	d_wdw-tlk(a5,d6),d2	fen. blitter 1er aff.
		move	2+d_adr-tlk(a5,d6),d3	adres. écran 2ieme aff.
		move	2+d_wdw-tlk(a5,d6),d4	fen. blitter 2ieme aff.

;		- plan 1 -

		move.l	a0,bltapth(a6)
		move	d1,bltdptl(a6)
		move	d2,bltsize(a6)
		tst	d4			aff. en 2 fois ?
		beq.s	go_plan2		-> non
		waitblitter
		move	d3,bltdptl(a6)
		move	d4,bltsize(a6)

;		- plan 2 -

go_plan2	lea	d_tplan(a0),a0
		add	d5,d1
		add	d5,d3
		waitblitter
		move.l	a0,bltapth(a6)
		move	d1,bltdptl(a6)
		move	d2,bltsize(a6)
		tst	d4			aff. en 2 fois ?
		beq.s	go_plan3		-> non
		waitblitter
		move	d3,bltdptl(a6)
		move	d4,bltsize(a6)

;		- plan 3 -

go_plan3	lea	d_tplan(a0),a0
		add	d5,d1
		add	d5,d3
		waitblitter
		move.l	a0,bltapth(a6)
		move	d1,bltdptl(a6)
		move	d2,bltsize(a6)
		tst	d4			aff. en 2 fois ?
		beq.s	aff_dalle_end		-> non
		waitblitter
		move	d3,bltdptl(a6)
		move	d4,bltsize(a6)




;--------------------------------------------------------------------------
;		aff dalle : sort de la routine
;--------------------------------------------------------------------------
aff_dalle_end	rts


;=-=-=-=-=-=-=-=-> fin routine ed dalles, affiche dalle <-=-=-=-=-=-=-=-=-=








;==========================================================================
;		ed dalles : reaff. decor sous la dalle
;==========================================================================
; entrée	a5.l	adr. 'tlk'
;		a6.l	$dff000
;
reaff_decor_dalle:
;
;
;
;
;--------------------------------------------------------------------------
;		reaff decor : init. blitter
;--------------------------------------------------------------------------
		move	(a5),d6			n° écran 0 ou 4
		move	aecran1-tlk(a5,d6),d0
		move	#$09f0,d3
		moveq	#0,d4
		move	d_mod-tlk(a5,d6),d5
		move	aecran3-tlk(a5),d6
		moveq	#s_tplan,d7
		waitblitter
		move	d5,bltdmod(a6)
		move	d5,bltamod(a6)
		move	d3,bltcon0(a6)
		move	d4,bltcon1(a6)
		move	d6,bltapth(a6)
		move	d0,bltdpth(a6)




;--------------------------------------------------------------------------
;		reaff decor : 1er partie de la dalle
;--------------------------------------------------------------------------

;		- plan 1 -

		move	(a5),d6
		move	d_wdw-tlk(a5,d6),d2	1er fenêtre blitter
		beq	rdd_exit		-> sort
		move	d_adr-tlk(a5,d6),d1	1er adr. écran
		move	d1,bltaptl(a6)
		move	d1,bltdptl(a6)
		move	d2,bltsize(a6)

;		- plan 2 -

		add	d7,d1
		waitblitter
		move	d1,bltaptl(a6)
		move	d1,bltdptl(a6)
		move	d2,bltsize(a6)

;		- plan 3 -

		add	d7,d1
		waitblitter
		move	d1,bltaptl(a6)
		move	d1,bltdptl(a6)
		move	d2,bltsize(a6)




;--------------------------------------------------------------------------
;		reaff decor : 2ieme partie de la dalle
;--------------------------------------------------------------------------

;		- plan 1 -

		move	2+d_wdw-tlk(a5,d6),d2	2ieme fenêtre blitter
		beq.s	rdd_exit		-> sort
		move	2+d_adr-tlk(a5,d6),d1	2ieme adr. écran
		waitblitter
		move	d1,bltaptl(a6)
		move	d1,bltdptl(a6)
		move	d2,bltsize(a6)

;		- plan 2 -

		add	d7,d1
		waitblitter
		move	d1,bltaptl(a6)
		move	d1,bltdptl(a6)
		move	d2,bltsize(a6)

;		- plan 3 -

		add	d7,d1
		waitblitter
		move	d1,bltaptl(a6)
		move	d1,bltdptl(a6)
		move	d2,bltsize(a6)




;--------------------------------------------------------------------------
;		reaff decor : sort de la routine
;--------------------------------------------------------------------------
rdd_exit:	rts


;-=-=-=-=-=-=-=-> fin routine reaff. decor sous la dalle <-=-=-=-=-=-=-=-=-







;==========================================================================
;	ed dalles : entre dalles map-editor, à partir de la page dalle
;==========================================================================
; entrée	a5.l	adr. table 'tlk'
;
insert_map:
;
;
;
;
;--------------------------------------------------------------------------
;		insert map : calcul adr. sur le map-editor
;--------------------------------------------------------------------------
		lea	sc_map_brush-tlk(a5),a2	map-editor de la brush
		move.l	asc_end-tlk(a5),d2	adr. dernière ligne map-editor
		move.l	sc_map1-tlk(a5),a0	map-editor 1er ligne écran
		move	d_posx-tlk(a5),d0	pos. x écran 1er dalle
		move	d_posy-tlk(a5),d1	pos. y écran 1er dalle
		subq	#1,d1
		move	d1,d3
		and	#$000f,d3
		beq.s	no_retrait
		add	#16,d1
no_retrait	lsr	#4,d0
		lsr	#4,d1
		mulu	#m_large*2,d1		* taille ligne map-editor
		add	d1,a0
		add	d0,d0
		add	d0,a0			+pos. x
		cmp.l	d2,a0			dépassement ?
		blt.s	no_dep7			-> non
		sub.l	#m_large*2*m_hauteur,a0




;--------------------------------------------------------------------------
;		insert map : calcul nbre de dalle à inserer
;--------------------------------------------------------------------------
no_dep7		tst	human_league-tlk(a5)	brush neuve ?
		beq.s	no_dep71		-> non




;--------------------------------------------------------------------------
;		insert map : calcul largeur et hauteur du perimetre
;--------------------------------------------------------------------------
		move	block_x2-tlk(a5),d0
		and	#$fff0,d0
		bne.s	hal1
		moveq	#16,d0
hal1		move	d0,dalle_large-tlk(a5)		sauve largeur en pixel
		move	block_y2-tlk(a5),d1
		and	#$fff0,d1
		bne.s	hal2
		moveq	#16,d1
hal2		move	d1,dalle_haut-tlk(a5)		sauve hauteur en ligne

;--------------------------------------------------------------------------
;		insert map : calcul nbre de dalle à inserer
;--------------------------------------------------------------------------
no_dep71	move	dalle_large-tlk(a5),d0	largeur en pixel
		move	dalle_haut-tlk(a5),d1	hauteur en pixel
		lsr	#4,d0
		lsr	#4,d1
		subq	#1,d0
		bmi	ins_map_end
		subq	#1,d1
		bmi	ins_map_end

;-------------------------------------------------------------------------
;		insert map : test si la brush est neuve
;--------------------------------------------------------------------------
		tst	human_league-tlk(a5)	brush neuve ?
		beq.s	map_loop1		-> non

;--------------------------------------------------------------------------
;		insert map : calcul pointeur sur table adr. des dalles
;--------------------------------------------------------------------------
		move	d1,d2
		move.l	adecor_y1-tlk(a5),d4	adr. dalle à afficher
		sub.l	adalles-tlk(a5),d4	adr. relative dalle
		divu	#d_tligne,d4		/nbre d'octet par ligne
		move.l	d4,d3			sauve reste de la division
		lsr	#4,d4			/16 : hauteur dalle de 16
		mulu	#(d_tplan/2)*4,d4	taille d'1 ligne de 'sc_mapadr'
		swap	d3			reste de la division
		lsr	#1,d3			convertit octet en mot
		lsl	#2,d3			*4 (1 long mot par adr. dalle)
		add	d3,d4			pt / table 'sc_mapadr'

;--------------------------------------------------------------------------
;		insert map : copie codes dans le map-editor brush
;--------------------------------------------------------------------------
map_loop	move	d4,d3			1er pointeur sur 'sc_mapadr'
		move	d0,d5			nbre de dalle en largeur
		move.l	a2,a1			adr. map-editor
map_loop21	move	d3,(a1)+		sauve code dalle
		addq	#4,d3
		dbf	d5,map_loop21
		add	#d_tplan*2,d4		+une ligne
		lea	m_large*2(a2),a2	+une ligne
no_dep75	dbf	d2,map_loop
		move	#0,human_league-tlk(a5)	-> broche usée
		bra	set_brush


;--------------------------------------------------------------------------
;		insert map : copie codes dans le map-editor
;--------------------------------------------------------------------------
map_loop1	move	d0,d5			nbre de dalle en largeur
		move.l	a0,a1			adr. map-editor
		move.l	a2,a3			adr. map-editor brush
map_loop2	move	(a3)+,(a1)+		sauve code dalle
		dbf	d5,map_loop2
		lea	m_large*2(a0),a0	+une ligne
		lea	m_large*2(a2),a2	+une ligne
		cmp.l	d2,a0			dépassement ?
		blt.s	no_dep74		-> non
		sub.l	#m_large*2*m_hauteur,a0
no_dep74	dbf	d1,map_loop1



;--------------------------------------------------------------------------
;		insert map : sort de la routine
;--------------------------------------------------------------------------
ins_map_end	rts






;==========================================================================
;	ed dalles : prends dalles map-editor, pour la nouvelles brush
;==========================================================================
; entrée	a5.l	adr. table 'tlk'
;
prends_map:
;
;
;
;
;--------------------------------------------------------------------------
;		prends map : calcul adr. sur le map-editor
;--------------------------------------------------------------------------



		lea	sc_map_brush-tlk(a5),a2	map-editor de la brush
		move.l	asc_end-tlk(a5),d2	adr. dernière ligne map-editor
		move.l	sc_map1-tlk(a5),a0	map-editor 1er ligne écran

		move	nu_video-tlk(a5),d5	pos. y changement pts vidéos
		move	ed_blocy1-tlk(a5),d1	pos. y
		and	#$fff0,d1
		and	#$000f,d5
		add	d5,d1

		move	ed_blocx1-tlk(a5),d0
		move	d1,d3
		and	#$000f,d3
		beq.s	pr_retrait
		add	#16,d1
pr_retrait	lsr	#4,d0
		lsr	#4,d1
		mulu	#m_large*2,d1		* taille ligne map-editor
		add	d1,a0
		add	d0,d0
		add	d0,a0			+pos. x
		cmp.l	d2,a0			dépassement ?
		blt.s	pr_dep7			-> non
		sub.l	#m_large*2*m_hauteur,a0



;--------------------------------------------------------------------------
;		prends map : calcul nbre de dalle à inserer
;--------------------------------------------------------------------------
pr_dep7		move	dalle_large-tlk(a5),d0	largeur en pixel
		move	dalle_haut-tlk(a5),d1	hauteur en pixel
		lsr	#4,d0
		lsr	#4,d1
		subq	#1,d0
		bmi.s	pr_map_end
		subq	#1,d1
		bmi.s	pr_map_end




;--------------------------------------------------------------------------
;		prends map : copie codes dans le map-editor
;--------------------------------------------------------------------------
pr_loop		move	d0,d5			nbre de dalle en largeur
		move.l	a0,a1			adr. map-editor
		move.l	a2,a3			adr. map-editor brush
pr_loop2	move	(a1)+,(a3)+		sauve code dalle
		dbf	d5,pr_loop2
		lea	m_large*2(a0),a0	+une ligne
		lea	m_large*2(a2),a2	+une ligne
		cmp.l	d2,a0			dépassement ?
		blt.s	pr_dep74		-> non
		sub.l	#m_large*2*m_hauteur,a0
pr_dep74	dbf	d1,pr_loop



;--------------------------------------------------------------------------
;		prends map : sort de la routine
;--------------------------------------------------------------------------
pr_map_end	rts






;--------------------------------------------------------------------------
;		ed dalles : aff. les dalles dans la nouvelle brush
;--------------------------------------------------------------------------

;		- loop : init. registres blitters -

set_brush	moveq	#0,d0
		moveq	#-1,d1
		move	#$09f0,d2		combinaison blitter
		move	#d_tligne-2,d3		modulo D	scroll
		move	#d_tligne-2,d4		modulo A	dalle
		waitblitter
		move.l	d1,bltafwm(a6)
		move	d2,bltcon0(a6)
		move	d0,bltcon1(a6)
		move	d3,bltdmod(a6)
		move	d4,bltamod(a6)
		move.l	page_brush-tlk(a5),d5	adr. dalle à afficher (3 plans)
		move.l	d5,d6
		move.l	d5,d7

;		- loop : init. registre MC68000 -

		move.l	adalles-tlk(a5),a0	adr. de la page dalle
		lea	sc_map_brush-tlk(a5),a1	adr. du map-editor
		lea	sc_mapadr-tlk(a5),a2	table des positions des dalles
		move	#s_nligne/16-1,d0	nbre de ligne de dalle
		moveq	#0,d2
		moveq	#0,d3			adr. relative écran
		move	#16*64+1,d4		fenêtre blitter
		moveq	#15,d3

;		- loop : init. compteur boucle nbre de dalles hozr. -

st_init.loop5	move	#s_tplan/2-1,d1		nbre de dalles par ligne

;		- loop : calcul adr. de la dalle à afficher -
							
st_init.loop4	move.l	a0,a3			adr. de la page dalle
		moveq	#0,d2
		move	(a1)+,d2		pt/adr. page dalle
		add.l	(a2,d2.l),a3		adr. de la dalle à afficher

;		- loop : aff. dalle dans l'écran 1, plan 1 -
	
		waitblitter
		move.l	a3,bltapth(a6)
		move.l	d5,bltdpth(a6)
		move	d4,bltsize(a6)

;		- loop : aff. dalle dans l'écran 1, plan 2 -
		lea	d_tplan(a3),a3
		add.l	#d_tplan,d5
		waitblitter
		move.l	a3,bltapth(a6)
		move.l	d5,bltdpth(a6)
		move	d4,bltsize(a6)

;		- loop : aff. dalle dans l'écran 1, plan 3 -
		lea	d_tplan(a3),a3
		add.l	#d_tplan,d5
		waitblitter
		move.l	a3,bltapth(a6)
		move.l	d5,bltdpth(a6)
		move	d4,bltsize(a6)

		addq.l	#2,d7
		move.l	d7,d5
		dbf	d1,st_init.loop4

		move	#d_tligne,d2
		mulu	#16,d2
		add.l	d2,d6
		move.l	d6,d7
		move.l	d6,d5
		dbf	d3,st_init.loop5
		rts



;-=-> fin routine entre dalles map-editor, à partir de la page dalle <-=-=-
;
;
;
;
;-+-+-+-+-+-+-+-> fin routines appellées par 'mysterious' <-+-+-+-+-+-+-+-+








;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
; >*<		sélecteur de dalle
;
;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
; entrée	a5.l	adr. table 'tlk'
;		a6.l	$dff000
;
lugubre:
;
;
;
;
;--------------------------------------------------------------------------
;		sélecteur : inc. position y de la page
;--------------------------------------------------------------------------
		lea	page_x-tlk(a5),a2
		lea	page_y-tlk(a5),a3
		moveq	#0,d2
		lea	flag_up-tlk(a5),a0
		lea	flag_down-tlk(a5),a1
		move	(a0),d0
		move	(a1),d1
		move	d2,(a0)
		move	d2,(a1)
		sub	d0,(a3)			dépl. en haut
		add	d1,(a3)			dépl. en bas



;--------------------------------------------------------------------------
;		sélecteur : inc. position x de la page
;--------------------------------------------------------------------------
		lea	flag_left-tlk(a5),a0
		lea	flag_right-tlk(a5),a1
		move	(a0),d0
		move	(a1),d1
		move	d2,(a0)
		move	d2,(a1)
		sub	d0,(a2)			dépl. à gauche
		add	d1,(a2)			dépl. à droite




;--------------------------------------------------------------------------
;		sélecteur : test dépassement horz. de la page
;--------------------------------------------------------------------------
		move	d2,(a2)




;--------------------------------------------------------------------------
;		sélecteur : test dépassement vert. de la page
;--------------------------------------------------------------------------
		tst	(a3)
		bmi.s	ruba3
		move	#d_nligne-s_nligne+16,d1
		cmp	(a3),d1
		bhi.s	ruba2
ruba		move	d1,(a3)
		bra.s	ruba2
ruba3		move	d2,(a3)




;--------------------------------------------------------------------------
;		sélecteur : calcul adr. de l'écran à pointer
;--------------------------------------------------------------------------
ruba2		moveq	#0,d0
		moveq	#0,d1
		move	(a3),d0			pos. y de la page
		and	#$fff0,d0
		mulu	#d_tligne,d0		*taille d'une ligne
		move.l	d0,d1




;--------------------------------------------------------------------------
;		sélecteur : calcul pointeurs vidéos de la liste copper
;--------------------------------------------------------------------------
		add.l	adalles-tlk(a5),d1
		move.l	d1,a4
		lea	bpl1pth(a6),a0
		moveq	#0,d0
		moveq	#d_tplan,d0
		move.l	d1,(a0)+		bpl1pth
		add.l	d0,d1	
		move.l	d1,(a0)+		bpl2pth
		add.l	d0,d1	
		move.l	d1,(a0)+		bpl3pth




;--------------------------------------------------------------------------
;		sélecteur : init. palette
;--------------------------------------------------------------------------
		lea	color00(a6),a0
		lea	color-tlk(a5),a1
		moveq	#16-1,d0
color_loop	move.l	(a1)+,(a0)+
		dbf	d0,color_loop




;--------------------------------------------------------------------------
;		sélecteur : init. registres vidéos
;--------------------------------------------------------------------------
		move.l	select_copper-tlk(a5),a0
		move.l	a0,cop1lch(a6)
		move	#d_tplan*(d_nplan-1),d0
		move	d0,bpl1mod(a6)
		move	d0,bpl2mod(a6)
		move	#d_nplan,d0
		ror	#4,d0
		or	#$0200,d0
		move	d0,bplcon0(a6)
		move	#$0000,bplcon1(a6)
		move	#$0024,bplcon2(a6)
		move	#$3081,diwstrt(a6)
		move	#$30c1,diwstop(a6)
		move	#$0038,ddfstrt(a6)
		move	#$00d0,ddfstop(a6)




;--------------------------------------------------------------------------
;		sélecteur : test souris bouton gauche
;--------------------------------------------------------------------------
		cmp	#1,flag_souris-tlk(a5)	bouton gauche ?
		bne.s	kerm_a			-> non
		cmp	#1,flag_prise-tlk(a5)	dèjà en pleine prise de block ?
		beq.s	kerm_c			-> oui
		move	#1,flag_prise-tlk(a5)
		move	souris_x-tlk(a5),d0
		move	souris_y-tlk(a5),d1
		and	#$fff0,d0
		and	#$fff0,d1
		move	d0,block_x1-tlk(a5)
		move	d1,block_y1-tlk(a5)
		moveq	#16,d0
		move	d0,block_x2-tlk(a5)
		move	d0,block_y2-tlk(a5)
		bra.s	kerm_c




;--------------------------------------------------------------------------
;		sélecteur : test souris bouton droit
;--------------------------------------------------------------------------
kerm_a		cmp	#2,flag_souris-tlk(a5)	bouton droit ?
		bne.s	kerm_f			-> non
		*moveq	#0,d0
		*move	d0,block_x1-tlk(a5)
		*move	d0,block_y1-tlk(a5)
		*move	d0,block_x2-tlk(a5)
		*move	d0,block_y2-tlk(a5)
		*move	d0,flag_prise-tlk(a5)
		bra.s	kerm_c




;--------------------------------------------------------------------------
;		sélecteur : aucun bouton préssé
;--------------------------------------------------------------------------
kerm_f		lea	flag_prise-tlk(a5),a0
		tst	(a0)
		beq.s	kerm_c
		move	#2,(a0)




;--------------------------------------------------------------------------
;		sélecteur : test si charge page touche 'L'
;--------------------------------------------------------------------------
kerm_c		lea	clara-4-tlk(a5),a0
		move	(a0),d0			lit code clavier raw
		cmp	#$28,d0			touche 'L' ?
		bne.s	no_l
		clr	(a0)			raz code clavier

;		- charge dalles -

		clr	flag_load-tlk(a5)
		bsr	load_dalles

;		- raz adresses du scroll -

		tst	flag_load-tlk(a5)	chargement à échoué ?
		beq.s	no_l			-> oui
		moveq	#0,d0
		move	d0,flag_first-tlk(a5)
		move.l	d0,adecor_y-tlk(a5)
		move.l	d0,adecor_x-tlk(a5)
		move.l	d0,adecor_x1-tlk(a5)
		move.l	d0,adecor_x2-tlk(a5)
		move.l	d0,adecor_y1-tlk(a5)
		move.l	d0,adecor_y2-tlk(a5)




;--------------------------------------------------------------------------
;		sélecteur : affiche lignes
;--------------------------------------------------------------------------
no_l		bsr	lignes





;--------------------------------------------------------------------------
;		sélecteur : test si passe dans éditeur de dalles
;--------------------------------------------------------------------------
		lea	clara-4-tlk(a5),a0
		move	(a0),d0			lit code clavier raw
		cmp	#$26,d0			touche 'J' ?
		bne.s	no_spaced
		clr	(a0)			raz code clavier
		move	#1,flag_eddd-tlk(a5)	flag demande ed de dalles

;		sauve la bonne taille de la brush

		move	block_x2-tlk(a5),sblock_x2-tlk(a5)
		move	block_y2-tlk(a5),sblock_y2-tlk(a5)

;		init les adresses des reaffichages des decors lignes

		lea	ed_hori1_adr-tlk(a5),a0
		moveq	#10-1,d0
		moveq	#-1,d1
db_loop		move.l	d1,(a0)+
		dbf	d0,db_loop




;--------------------------------------------------------------------------
;		sélecteur : test si passe dans éditeur de dalles
;--------------------------------------------------------------------------
no_spaced	cmp	#2,flag_eddd-tlk(a5)	lignes éffacées ?
		bne.s	lugubre_exit		-> non
		bsr	scroll_init
		move	#4,flag_prg-tlk(a5)	>flag éditeur dalles<
		move	#1,human_league-tlk(a5)	flag brush neuve
		bsr	insert_map




;--------------------------------------------------------------------------
;		sélecteur : sort de la routine
;--------------------------------------------------------------------------
lugubre_exit:	rts
;
;
;
;
;-+-+-+-+-+-+-+-+-+-+-> fin du sélecteur de dalles <-+-+-+-+-+-+-+-+-+-+-+







;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
;		routines appellées par 'lugubre'
;
;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
;
;
;
;
;
;
;
;==========================================================================
;		sélecteur : initialise le défilement
;==========================================================================
; entrée	a5.l	adr. table 'tlk'
;		a6.l	$dff000
;
scroll_init:
;
;
;
;
;--------------------------------------------------------------------------
;		scroll init : supprime écran
;--------------------------------------------------------------------------
		move	#$0200,bplcon0(a6)




;--------------------------------------------------------------------------
;		scroll init : si 2ieme init, alors saute
;--------------------------------------------------------------------------
		tst	flag_first-tlk(a5)
		bne	sc_first1
		move	#1,flag_first-tlk(a5)




;--------------------------------------------------------------------------
;		scroll init : init. liste copper 1
;--------------------------------------------------------------------------
		lea	sc_u1+2-tlk(a5),a0
		move.l	aecran1-tlk(a5),d0
		moveq	#s_nplan-1,d1
		moveq	#s_tplan,d2
sc_init.loop7	swap	d0
		move	d0,(a0)
		addq	#4,a0
		swap	d0
		move	d0,(a0)
		addq	#4,a0
		add	d2,d0
		dbf	d1,sc_init.loop7




;--------------------------------------------------------------------------
;		scroll init : init. liste copper 1, chg. de pointeurs
;--------------------------------------------------------------------------
		lea	sc_v1+2-tlk(a5),a0
		move.l	aecran1-tlk(a5),d0
		moveq	#s_nplan-1,d1
		moveq	#s_tplan,d2
sc_init.loop71	swap	d0
		move	d0,(a0)
		addq	#4,a0
		swap	d0
		move	d0,(a0)
		addq	#4,a0
		add	d2,d0
		dbf	d1,sc_init.loop71




;--------------------------------------------------------------------------
;		scroll init : init. liste copper 2
;--------------------------------------------------------------------------
		lea	sc_u2+2-tlk(a5),a0
		move.l	aecran2-tlk(a5),d0
		moveq	#s_nplan-1,d1
		moveq	#s_tplan,d2
sc_init.loop8	swap	d0
		move	d0,(a0)
		addq	#4,a0
		swap	d0
		move	d0,(a0)
		addq	#4,a0
		add	d2,d0
		dbf	d1,sc_init.loop8




;--------------------------------------------------------------------------
;		scroll init : init. liste copper 2, chg. pointeurs vidéos
;--------------------------------------------------------------------------
		lea	sc_v2+2-tlk(a5),a0
		move.l	aecran2-tlk(a5),d0
		moveq	#s_nplan-1,d1
		moveq	#s_tplan,d2
sc_init.loop81	swap	d0
		move	d0,(a0)
		addq	#4,a0
		swap	d0
		move	d0,(a0)
		addq	#4,a0
		add	d2,d0
		dbf	d1,sc_init.loop81




;--------------------------------------------------------------------------
;		scroll init : recopie listes copper en chip
;--------------------------------------------------------------------------
		lea	sc_c1-tlk(a5),a0
		move.l	zcopper1-tlk(a5),a1
		move	#sc_c2-sc_c1-1,d0
zlc1.loop	move.b	(a0)+,(a1)+
		dbf	d0,zlc1.loop

		lea	sc_c2-tlk(a5),a0
		move.l	zcopper2-tlk(a5),a1
		move	#sc_c3-sc_c2-1,d0
zlc2.loop	move.b	(a0)+,(a1)+
		dbf	d0,zlc2.loop
		subq	#4,a1




;--------------------------------------------------------------------------
;		scroll init : init. table pt de dalles
;--------------------------------------------------------------------------
		lea	sc_mapadr-tlk(a5),a0
		moveq	#0,d3
		move	#d_nligne/16-1,d0	nbre ligne de la page dalle - 1
sc_init.loop1	move	#d_tplan/2-1,d1		nbre de dalle par ligne - 1
sc_init.loop2	move.l	d3,(a0)+
		addq.l	#2,d3
		dbf	d1,sc_init.loop2
		add.l	#d_tligne*16-d_tplan,d3
		dbf	d0,sc_init.loop1




;--------------------------------------------------------------------------
;		scroll init : efface les 3 écrans
;--------------------------------------------------------------------------
		move.l	zecran1-tlk(a5),a1
		move.l	zecran2-tlk(a5),a2
		move.l	zecran3-tlk(a5),a3
		moveq	#0,d0
		move	#s_ecran/8-1,d1
sc_cls.loop	move.l	d0,(a1)+
		move.l	d0,(a1)+
		move.l	d0,(a2)+
		move.l	d0,(a2)+
		move.l	d0,(a3)+
		move.l	d0,(a3)+
		dbf	d1,sc_cls.loop




;--------------------------------------------------------------------------
;		scroll init : aff. les dalles dans les 3 écrans
;--------------------------------------------------------------------------

;		- loop : init. regsitres blitters -

		moveq	#0,d0
		moveq	#-1,d1
		move	#$09f0,d2		combinaison blitter
		move	#s_tligne-2,d3		modulo D	
		move	#d_tligne-2,d4		modulo A
		waitblitter
		move.l	d1,bltafwm(a6)
		move	d2,bltcon0(a6)
		move	d0,bltcon1(a6)
		move	d3,bltdmod(a6)
		move	d4,bltamod(a6)

;		- loop : init. registre MC68000 -

		move.l	adalles-tlk(a5),a0	adr. de la page dalle
		move.l	asc_map-tlk(a5),a1	adr. du map-editor
		lea	sc_mapadr-tlk(a5),a2
		move	#s_nligne/16-1,d0	nbre de ligne de dalle
		moveq	#0,d2
		moveq	#0,d3			adr. relative écran
		move	#16*64+1,d4		fenêtre blitter
		move	aecran1-tlk(a5),d5
		move	aecran2-tlk(a5),d6
		move	aecran3-tlk(a5),d7

;		- loop : init. compteur boucle nbre de dalles hozr. -

sc_init.loop5	move	#s_tplan/2-1,d1		nbre de dalles par ligne

;		- loop : calcul adr. de la dalle à afficher -
							
sc_init.loop4	move.l	a0,a3			adr. de la page dalle
		move	(a1)+,d2		pt/adr. page dalle
		add.l	(a2,d2.l),a3		adr. de la dalle à afficher

;		- loop : aff. dalle dans l'écran 1, plan 1 -
	
		waitblitter
		move.l	a3,bltapth(a6)
		move	d5,bltdpth(a6)
		move	d3,bltdptl(a6)
		move	d4,bltsize(a6)

;		- loop : aff. dalle dans l'écran 2, plan 1 -

		waitblitter
		move.l	a3,bltapth(a6)
		move	d6,bltdpth(a6)
		move	d3,bltdptl(a6)
		move	d4,bltsize(a6)

;		- loop : aff. dalle dans l'écran 3, plan 1 -

		waitblitter
		move.l	a3,bltapth(a6)
		move	d7,bltdpth(a6)
		move	d3,bltdptl(a6)
		move	d4,bltsize(a6)

;		- loop : aff. dalle dans l'écran 1, plan 2 -

		lea	d_tplan(a3),a3
		add	#s_tplan,d3
		waitblitter
		move.l	a3,bltapth(a6)
		move	d5,bltdpth(a6)
		move	d3,bltdptl(a6)
		move	d4,bltsize(a6)

;		- loop : aff. dalle dans l'écran 2, plan 2 -

		waitblitter
		move.l	a3,bltapth(a6)
		move	d6,bltdpth(a6)
		move	d3,bltdptl(a6)
		move	d4,bltsize(a6)

;		- loop : aff. dalle dans l'écran 3, plan 2 -

		waitblitter
		move.l	a3,bltapth(a6)
		move	d7,bltdpth(a6)
		move	d3,bltdptl(a6)
		move	d4,bltsize(a6)

;		- loop : aff. dalle dans l'écran 1, plan 3 -

		lea	d_tplan(a3),a3
		add	#s_tplan,d3
		waitblitter
		move.l	a3,bltapth(a6)
		move	d5,bltdpth(a6)
		move	d3,bltdptl(a6)
		move	d4,bltsize(a6)

;		- loop : aff. dalle dans l'écran 2, plan 3 -

		waitblitter
		move.l	a3,bltapth(a6)
		move	d6,bltdpth(a6)
		move	d3,bltdptl(a6)
		move	d4,bltsize(a6)

;		- loop : aff. dalle dans l'écran 3, plan 3 -

		waitblitter
		move.l	a3,bltapth(a6)
		move	d7,bltdpth(a6)
		move	d3,bltdptl(a6)
		move	d4,bltsize(a6)

;		- loop : calcul adr. prochaine dalle horz. -

		sub	#s_tplan*2-2,d3
		dbf	d1,sc_init.loop4

;		- loop : calcul adr. prochaine ligne -

		add	#s_tligne*16-s_tplan,d3
		dbf	d0,sc_init.loop5




;--------------------------------------------------------------------------
;		init scroll : calcul pt sur map-editor
;--------------------------------------------------------------------------
		move.l	asc_map-tlk(a5),a0	adr. 1er ligne
		move.l	a0,sc_map1-tlk(a5)	adr. 1er ligne
		move.l	a0,a1
		add.l	#m_large*2*m_hauteur,a0
		move.l	a0,asc_end-tlk(a5)	adr. dernière ligne
		moveq	#0,d0
		move	d0,sc_y1-tlk(a5)
		move	d0,nu_video-tlk(a5)
		move	d0,afline-tlk(a5)



;--------------------------------------------------------------------------
;		scroll init : init. registres vidéo
;--------------------------------------------------------------------------
sc_first1:	move	#s_tplan*(s_nplan-1),d0
		move	d0,bpl1mod(a6)
		move	d0,bpl2mod(a6)
		move	#s_nplan,d0
		ror	#4,d0
		or	#$0200,d0
		move	d0,bplcon0(a6)
		move	#$0000,bplcon1(a6)
		move	#$0024,bplcon2(a6)
		move	#$3081,diwstrt(a6)
		move	#$30c1,diwstop(a6)
		move	#$0038,ddfstrt(a6)
		move	#$00d0,ddfstop(a6)




;--------------------------------------------------------------------------
;		scroll init : pointe la liste copper
;--------------------------------------------------------------------------
		move.l	zcopper1-tlk(a5),a0
		move.l	a0,cop1lch(a6)
		move	d0,cop1jmp(a6)




;--------------------------------------------------------------------------
;		scroll init : init. palette
;--------------------------------------------------------------------------
		lea	color00(a6),a0
		lea	color-tlk(a5),a1
		moveq	#16-1,d0
color_loop2	move.l	(a1)+,(a0)+
		dbf	d0,color_loop2




;--------------------------------------------------------------------------
;		scroll init : sort de la routine
;--------------------------------------------------------------------------
sc_exit		rts


;=-=-=-=-=-=-=-=-=-=-> fin initialise le défilement <-=-=-=-=-=-=-=-=-=-=-=








;==========================================================================
;		sélecteur : affiche les 4 lignes du block
;==========================================================================
; entrée	a4.l	adr. écran page dalle, 1er mot balayé
;		a5.l	adr. table 'tlk'
;
lignes:
;
;
;
;
;--------------------------------------------------------------------------
;		lignes : compteur d'attente
;--------------------------------------------------------------------------
		tst	cycles_wait-tlk(a5)
		beq.s	nowe
		subq	#1,cycles_wait-tlk(a5)
		bra	aff_lignes_end		-> sort routine
nowe		move	#6,cycles_wait-tlk(a5)




;--------------------------------------------------------------------------
;		lignes : inc. pointeur plan (cycle aff. ligne pointillés)
;--------------------------------------------------------------------------
		addq	#2,a_cycles2-tlk(a5)
		and	#$000e,a_cycles2-tlk(a5)




;--------------------------------------------------------------------------
;		lignes : inc. pointeur plan (cycle aff. ligne pointillés)
;--------------------------------------------------------------------------
		addq	#1,a_cycles-tlk(a5)
		and	#$3,a_cycles-tlk(a5)



;--------------------------------------------------------------------------
;		blocks lines : test si prise de block en cours
;--------------------------------------------------------------------------
		cmp	#1,flag_prise-tlk(a5)
		bne.s	bl_rere
		move	souris_x-tlk(a5),d0
		move	souris_y-tlk(a5),d1
		and	#$fff0,d0
		and	#$fff0,d1
		move	block_x1-tlk(a5),d2
		sub	d2,d0
		bge.s	farmer
		moveq	#16,d0
farmer		add	#16,d0
		move	d0,block_x2-tlk(a5)

		move	block_y1-tlk(a5),d2
		sub	d2,d1
		bge.s	mylene
		moveq	#16,d1
mylene		add	#16,d1
		move	d1,block_y2-tlk(a5)



;--------------------------------------------------------------------------
;		aff 2 lignes : restitue décor ligne verticale
;--------------------------------------------------------------------------
bl_rere		tst.l	adecor_y-tlk(a5)	dèjà adr. sauvegardée ?
		beq.s	foz			-> non, 1er passage
		move.l	adecor_y-tlk(a5),a0	adr. écran précédent aff.
		clr.l	adecor_y-tlk(a5)
		lea	decor_y-tlk(a5),a1	adr. buffer sauvegarde écran
		move	#256*3-1,d0		256 lignes d'écran
		moveq	#d_tplan,d1		taille d'une ligne (écran raw-blitter)
rd_lv		move	(a1)+,(a0)
		add	d1,a0
		dbf	d0,rd_lv




;--------------------------------------------------------------------------
;		aff 2 lignes : restitue décor ligne horizontale
;--------------------------------------------------------------------------
foz		tst.l	adecor_x-tlk(a5)	dèjà adr. sauvegardée ?
		beq.s	foz2			-> non, 1er passage
		move.l	adecor_x-tlk(a5),a0	adr. écran précédent aff.
		clr.l	adecor_x-tlk(a5)
		lea	decor_x-tlk(a5),a1	adr. buffer sauvegarde écran
		move	#20*3-1,d0		20*3 mots / lignes d'écran
rd_lh		move	(a1)+,(a0)+
		dbf	d0,rd_lh




;--------------------------------------------------------------------------
;		block lines : restitue décor ligne horizontale 1
;--------------------------------------------------------------------------
foz2		tst.l	adecor_x1-tlk(a5)	dèjà adr. sauvegardée ?
		beq.s	foz7			-> non, 1er passage
		move.l	adecor_x1-tlk(a5),a0	adr. écran précédent aff.
		clr.l	adecor_x1-tlk(a5)
		lea	decor_x1-tlk(a5),a1	adr. buffer sauvegarde écran
		move	-2(a1),d0
		beq.s	foz7
		subq	#1,d0
rd_lh1		move	(a1)+,80(a0)
		move	(a1)+,40(a0)
		move	(a1)+,(a0)+
		dbf	d0,rd_lh1




;--------------------------------------------------------------------------
;		block lines : restitue décor ligne horizontale 2
;--------------------------------------------------------------------------
foz7		tst.l	adecor_x2-tlk(a5)	dèjà adr. sauvegardée ?
		beq.s	foz8			-> non, 1er passage
		move.l	adecor_x2-tlk(a5),a0	adr. écran précédent aff.
		clr.l	adecor_x2-tlk(a5)
		lea	decor_x2-tlk(a5),a1	adr. buffer sauvegarde écran
		move	-2(a1),d0
		beq.s	foz8
		subq	#1,d0
rd_lh2		move	(a1)+,80(a0)
		move	(a1)+,40(a0)
		move	(a1)+,(a0)+
		dbf	d0,rd_lh2




;--------------------------------------------------------------------------
;		block lines : restitue décor ligne verticale 1
;--------------------------------------------------------------------------
foz8		tst.l	adecor_y1-tlk(a5)	dèjà adr. sauvegardée ?
		beq.s	foz9			-> non, 1er passage
		move.l	adecor_y1-tlk(a5),a0	adr. écran précédent aff.
		clr.l	adecor_y1-tlk(a5)
		lea	decor_y1-tlk(a5),a1	adr. buffer sauvegarde écran
		move	-2(a1),d0
		beq.s	foz9
		mulu	#d_nplan,d0		*nombre de plan
		subq	#1,d0
		moveq	#d_tplan,d1
rd_lv1		move	(a1)+,(a0)
		add	d1,a0
		dbf	d0,rd_lv1




;--------------------------------------------------------------------------
;		block lines : restitue décor ligne verticale 2
;--------------------------------------------------------------------------
foz9		tst.l	adecor_y2-tlk(a5)	dèjà adr. sauvegardée ?
		beq.s	foz10			-> non, 1er passage
		move.l	adecor_y2-tlk(a5),a0	adr. écran précédent aff.
		subq	#2,a0
		clr.l	adecor_y2-tlk(a5)
		lea	decor_y2-tlk(a5),a1	adr. buffer sauvegarde écran
		move	-2(a1),d0
		beq.s	foz10
		mulu	#d_nplan,d0		*nombre de plan
		subq	#1,d0
		moveq	#d_tplan,d1
rd_lv2		move	(a1)+,(a0)
		add	d1,a0
		dbf	d0,rd_lv2





;--------------------------------------------------------------------------
;		aff 2 lignes : calcul adr. écran ligne verticale
;--------------------------------------------------------------------------
foz10		tst	flag_prise-tlk(a5)	block ?
		bne.s	foz223			-> oui
		move	souris_x-tlk(a5),d1	pos. x souris en pixel
		lsr	#3,d1			convertit en octet
		and	#$fffe,d1		octet paire
		lea	(a4,d1),a3
		move.l	a3,adecor_y-tlk(a5)	sauve adr. écran




;--------------------------------------------------------------------------
;		aff 2 lignes : calcul adr. écran ligne horizontale
;--------------------------------------------------------------------------
		move	souris_y-tlk(a5),d1	pos. y souris en ligne
		and	#$fff0,d1		multiple de 16
		mulu	#d_tligne,d1
		move.l	a4,a3
		add.l	d1,a3
		move.l	a3,adecor_x-tlk(a5)	sauve adr. écran




;--------------------------------------------------------------------------
;		block lines : calcul adr. des 2 lignes horz.
;--------------------------------------------------------------------------
foz223		moveq	#0,d6
		lea	decor_x1-2-tlk(a5),a0
		lea	decor_x2-2-tlk(a5),a1
		move	d6,(a0)			raz nbre de mot 1er ligne horz.
		move	d6,(a1)			raz nbre de mot 2ieme ligne horz.
		move	block_x1-tlk(a5),d0
		move	d0,d1
		move	block_x2-tlk(a5),d2
		cmp	#16,d2
		bge.s	lesson
		moveq	#16,d2
lesson		add	d2,d1
		cmp	d0,d1
		beq	o_y1y2
		bhi.s	o_x1x2
		exg	d0,d1

;		- ligne horizontale 1 -

o_x1x2:		move	page_y-tlk(a5),d3
		and	#$fff0,d3
		move	block_y1-tlk(a5),d2
		and	#$fff0,d2
		mulu	#d_tligne,d2
		move.l	a4,a3
		add.l	d2,a3
		move	d0,d2			pos x 1er point en pixel
		lsr	#3,d2			convertit en octet
		and	#$fffe,d2		octet paire
		add.l	d2,a3			adr. 1er point
		move.l	a3,adecor_x1-tlk(a5)

;		- ligne horizontale 2 -

		move	block_y2-tlk(a5),d2
		and	#$fff0,d2
		bne.s	to_love
		moveq	#16,d2
to_love		add	block_y1-tlk(a5),d2
		and	#$fff0,d2
		subq	#1,d2
		mulu	#d_tligne,d2
		move.l	a4,a3			adresse écran
		add.l	d2,a3
		move	d0,d2			pos x 1er point en pixel
		lsr	#3,d2			convertit en octet
		and	#$fffe,d2		octet paire
		add.l	d2,a3			adr. 1er point
		move.l	a3,adecor_x2-tlk(a5)
		
;		- calcul nombre de mot de la ligne -

		sub	d0,d1			taille en pixel de la droite
		lsr	#4,d1			convertit en mot
		move	d1,(a0)
		move	d1,(a1)




;--------------------------------------------------------------------------
;		block lines : calcul adr. des 2 lignes vert
;--------------------------------------------------------------------------
o_y1y2		lea	decor_y1-2-tlk(a5),a0
		lea	decor_y2-2-tlk(a5),a1
		move	d6,(a0)			raz nbre de mot 1er ligne horz.
		move	d6,(a1)			raz nbre de mot 2ieme ligne horz.

		move	block_y2-tlk(a5),d1
		and	#$fff0,d1
		bne.s	lesson1
		moveq	#16,d1
lesson1		move	block_y1-tlk(a5),d0
		and	#$fff0,d0
		add	d0,d1
		cmp	d0,d1
		beq	o_afflb
		bhi.s	k_y1y2
		exg	d0,d1

;		- ligne verticale 1 -

k_y1y2:		move	d0,d2
		and	#$fff0,d2
		mulu	#d_tligne,d2
		move.l	a4,a3
		add.l	d2,a3
		move	block_x1-tlk(a5),d2	pos x 1er point en pixel
		lsr	#3,d2			convertit en octet
		and	#$fffe,d2		octet paire
		add.l	d2,a3			adr. 1er point
		move.l	a3,adecor_y1-tlk(a5)

;		- ligne verticale 2 -

		move	d0,d2
		and	#$fff0,d2
		mulu	#d_tligne,d2
		move.l	a4,a3
		add.l	d2,a3

		move	block_x2-tlk(a5),d2
		and	#$fff0,d2
		bne.s	to_love1
		moveq	#16,d2
to_love1	add	block_x1-tlk(a5),d2
		lsr	#3,d2			convertit en octet
		and	#$fffe,d2		octet paire
		add.l	d2,a3			adr. 1er point
		move.l	a3,adecor_y2-tlk(a5)

;		- calcul nombre de ligne de la ligne -

		sub	d0,d1			taille en pixel de la droite
		and	#$fff0,d1
		move	d1,(a0)
		move	d1,(a1)




;--------------------------------------------------------------------------
;		aff 2 lignes : sauve decor ligne verticale
;--------------------------------------------------------------------------
o_afflb		tst.l	adecor_y-tlk(a5)	adr. nul ?
		beq.s	sd_lhh			-> oui, pas de sauve
		move.l	adecor_y-tlk(a5),a0
		lea	decor_y-tlk(a5),a1
		move	#256*3-1,d0
		moveq	#d_tplan,d1
sd_lv		move	(a0),(a1)+
		add	d1,a0
		dbf	d0,sd_lv




;--------------------------------------------------------------------------
;		aff 2 lignes : sauve decor ligne horizontale
;--------------------------------------------------------------------------
sd_lhh		tst.l	adecor_x-tlk(a5)	adr. nul ?
		beq.s	h2o			-> oui, pas de sauve
		move.l	adecor_x-tlk(a5),a0
		lea	decor_x-tlk(a5),a1
		move	#20*3-1,d0
sd_lh		move	(a0)+,(a1)+
		dbf	d0,sd_lh





;--------------------------------------------------------------------------
;		block lines : sauve décor ligne horizontale 1
;--------------------------------------------------------------------------
h2o		tst.l	adecor_x1-tlk(a5)
		beq.s	fyz7
		move.l	adecor_x1-tlk(a5),a0	adr. écran précédent aff.
		lea	decor_x1-tlk(a5),a1	adr. buffer sauvegarde écran
		move	-2(a1),d0
		beq.s	fyz7
		subq	#1,d0
sd_lh1		move	80(a0),(a1)+
		move	40(a0),(a1)+
		move	(a0)+,(a1)+
		dbf	d0,sd_lh1




;--------------------------------------------------------------------------
;		block lines : sauve décor ligne horizontale 2
;--------------------------------------------------------------------------
fyz7		tst.l	adecor_x2-tlk(a5)
		beq.s	fyz8
		move.l	adecor_x2-tlk(a5),a0	adr. écran précédent aff.
		lea	decor_x2-tlk(a5),a1	adr. buffer sauvegarde écran
		move	-2(a1),d0
		beq.s	fyz8
		subq	#1,d0
sd_lh2		move	80(a0),(a1)+
		move	40(a0),(a1)+
		move	(a0)+,(a1)+
		dbf	d0,sd_lh2




;--------------------------------------------------------------------------
;		block lines : sauve décor ligne verticale 1
;--------------------------------------------------------------------------
fyz8		tst.l	adecor_y1-tlk(a5)
		beq.s	fyz9
		move.l	adecor_y1-tlk(a5),a0	adr. écran précédent aff.
		lea	decor_y1-tlk(a5),a1	adr. buffer sauvegarde écran
		move	-2(a1),d0
		beq.s	fyz9
		mulu	#d_nplan,d0
		subq	#1,d0
		moveq	#d_tplan,d1
sd_lv1		move	(a0),(a1)+
		add	d1,a0
		dbf	d0,sd_lv1




;--------------------------------------------------------------------------
;		block lines : sauve décor ligne verticale 2
;--------------------------------------------------------------------------
fyz9		tst.l	adecor_y2-tlk(a5)
		beq.s	fyz10
		move.l	adecor_y2-tlk(a5),a0	adr. écran précédent aff.
		subq	#2,a0
		lea	decor_y2-tlk(a5),a1	adr. buffer sauvegarde écran
		move	-2(a1),d0
		beq.s	fyz10
		mulu	#d_nplan,d0
		subq	#1,d0
		moveq	#d_tplan,d1
sd_lv2		move	(a0),(a1)+
		add	d1,a0
		dbf	d0,sd_lv2




;--------------------------------------------------------------------------
;		lignes : si demane à aller éditeur de dalles n'affiche pas
;--------------------------------------------------------------------------
fyz10:		tst	flag_eddd-tlk(a5)
		beq.s	fyz1010	
		addq	#1,flag_eddd-tlk(a5)	lignes éffacées
		bra	aff_lignes_end		-> sort



;--------------------------------------------------------------------------
;		blocks lines : affiche ligne horizontale 1
;--------------------------------------------------------------------------
fyz1010		move	a_cycles2-tlk(a5),d4
		lea	aflh(pc),a0		table motif d'affichage
		move	(a0,d4),d4		
		move	d4,d3
		not	d3
		tst.l	adecor_x1-tlk(a5)
		beq.s	fyz11
		move.l	adecor_x1-tlk(a5),a0
		move	decor_x1-2-tlk(a5),d0
		beq.s	fyz11
		subq	#1,d0
a_lh1		or	d4,80(a0)		
		and	d3,40(a0)		
		and	d3,(a0)+
		dbf	d0,a_lh1




;--------------------------------------------------------------------------
;		blocks lines : affiche ligne horizontale 2
;--------------------------------------------------------------------------
fyz11		tst.l	adecor_x2-tlk(a5)
		beq.s	fyz12
		move.l	adecor_x2-tlk(a5),a0
		move	decor_x2-2-tlk(a5),d0
		beq.s	fyz12
		subq	#1,d0
a_lh2		or	d4,80(a0)		
		and	d3,40(a0)		
		and	d3,(a0)+
		dbf	d0,a_lh2




;--------------------------------------------------------------------------
;		block lignes : affiche ligne verticale 1
;--------------------------------------------------------------------------
fyz12		tst.l	adecor_y1-tlk(a5)
		beq.s	fyz13
		move	a_cycles-tlk(a5),d4
		move	decor_y1-2-tlk(a5),d0	init. compt. ligne
		beq.s	fyz13
		moveq	#40*3,d1		taille d'un plan
		move	#$8000,d2		motfif à afficher
		move.l	adecor_y1-tlk(a5),a0
a_lv1		and	#$7fff,(a0)		affiche motif
		and	#$7fff,40(a0)		
		or	d2,80(a0)		
		subq	#1,d0			déc. compteur ligne
		beq.s	fyz13			-> stop, plus de ligne 
		add	d1,a0			adr. prochaine ligne écran
		dbf	d4,a_lv1		-> boucle
		add	d1,a0			adr. prochaine ligne écran
		moveq	#4-1,d4		
		subq	#1,d0			déc. compteur ligne
		bne.s	a_lv1			-> cont., encore ligne(s)




;--------------------------------------------------------------------------
;		block lignes : affiche ligne verticale 2
;--------------------------------------------------------------------------
fyz13		tst.l	adecor_y2-tlk(a5)
		beq.s	bl_end
		move	a_cycles-tlk(a5),d4
		move	decor_y2-2-tlk(a5),d0	init. compt. ligne
		beq.s	bl_end
		moveq	#40*3,d1		taille d'un plan
		move	#$0001,d2		motfif à afficher
		move.l	adecor_y2-tlk(a5),a0
		subq	#2,a0
a_lv2		and	#$fffe,(a0)		affiche motif
		and	#$fffe,40(a0)		
		or	d2,80(a0)		
		subq	#1,d0			déc. compteur ligne
		beq.s	bl_end			-> stop, plus de ligne 
		add	d1,a0			adr. prochaine ligne écran
		dbf	d4,a_lv2		-> boucle
		add	d1,a0			adr. prochaine ligne écran
		moveq	#4-1,d4		
		subq	#1,d0			déc. compteur ligne
		bne.s	a_lv2			-> cont., encore ligne(s)




;--------------------------------------------------------------------------
;		aff 2 lignes : affiche ligne verticale
;--------------------------------------------------------------------------
bl_end		tst.l	adecor_y-tlk(a5)
		beq.s	aff_ligne_horz
		move	a_cycles-tlk(a5),d4
		move	#256,d0			init. compt. ligne : 256 lignes
		moveq	#d_tligne,d1		taille d'un plan
		move	#$8000,d2		motfif à afficher
		move.l	adecor_y-tlk(a5),a0
a_lv		and	#$7fff,(a0)			affiche motif
		and	#$7fff,d_tplan(a0)		
		or	d2,d_tplan*2(a0)		
		subq	#1,d0			déc. compteur ligne
		beq.s	aff_ligne_horz		-> stop, plus de ligne 
		add	d1,a0			adr. prochaine ligne écran
		dbf	d4,a_lv			-> boucle
		add	d1,a0			adr. prochaine ligne écran
		moveq	#4-1,d4		
		subq	#1,d0			déc. compteur ligne
		bne.s	a_lv			-> cont., encore ligne(s)




;--------------------------------------------------------------------------
;		aff 2 lignes : affiche ligne horinzontale
;--------------------------------------------------------------------------
aff_ligne_horz	tst.l	adecor_x-tlk(a5)
		beq.s	aff_lignes_end
		move	a_cycles2-tlk(a5),d4
		lea	aflh(pc),a0		table motif d'affichage
		move	(a0,d4),d4		
		move	d4,d3
		not	d3
		move.l	adecor_x-tlk(a5),a0
		moveq	#20-1,d0		20 mots par ligne
a_lh		or	d4,d_tplan*2(a0)		
		and	d3,d_tplan(a0)		
		and	d3,(a0)+
		dbf	d0,a_lh



;--------------------------------------------------------------------------
;		lignes : compteur sort de la routine
;--------------------------------------------------------------------------
aff_lignes_end	rts




;--------------------------------------------------------------------------
;		lignes : table des motifs aff. ligne horz.
;--------------------------------------------------------------------------
aflh:		dc	$f0f0,$7878,$3c3c,$1e1e
		dc	$0f0f,$8787,$c3c3,$e1e1


;=-=-=-=-=-=-=-> fin routine affiche les lignes sélection <-=-=-=-=-=-=-=-
;
;
;
;
;+-+-+-+-+-+-+-+-> fin routines appellées par 'lugubre' <-+-+-+-+-+-+-+-+-+








;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
;	<*>	interruption de niveau 3, interruption vbl
;
;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
irq_level3:
;
;
;
;
;--------------------------------------------------------------------------
;		vbl : sauve registres + init a5 & a6
;--------------------------------------------------------------------------
		movem.l	d0-d7/a0-a6,-(a7)
		lea	tlk(pc),a5
		lea	$dff000,a6




;--------------------------------------------------------------------------
;		vbl : raffraichît pointeur lutins
;--------------------------------------------------------------------------
		move.l	asprite-tlk(a5),d0
		lea	spr0pth(a6),a0
		moveq	#8-1,d1
raff_loop	move.l	d0,(a0)+
		dbf	d1,raff_loop




;--------------------------------------------------------------------------
;		vbl : inc. compteur / flag vbl
;--------------------------------------------------------------------------
		addq	#1,flag_vbl-tlk(a5)




;--------------------------------------------------------------------------
;		vbl : test déplacement pointeur souris + calcul mot ctrl
;--------------------------------------------------------------------------
		tst	stop_mouse-tlk(a5)
		bne.s	no_test_mouse
		bsr	souris




;--------------------------------------------------------------------------
;		vbl : test les 2 boutons de la souris
;--------------------------------------------------------------------------
		bsr	mouse_buttons




;--------------------------------------------------------------------------
;		vbl : quitte vbl, & restitue regsitres
;--------------------------------------------------------------------------
no_test_mouse	move	#$0020,intreq(a6)
		movem.l	(a7)+,d0-d7/a0-a6
		rte
;
;
;
;
;-+-+-+-+-+-> fin interruption de niveau 3, interruption vbl <-+-+-+-+-+-+-









;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
;	<!>	interruption de niveau 3, routine appéllées par 'irq_level3'
;
;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
;
;
;
;
;
;
;
;=========================================================================
;		test souris & caclul coord. (x,y) du pointeur souris
;=========================================================================
; entrée	a6.l	$dff000
;		a5.l	adr. table 'TLK'
; utilise	d0,d1,d2,d4,d5,d6,a0,a1,a2,a5,a6
;
souris:
;
;
;
;
;--------------------------------------------------------------------------
;		souris : charge adr. souris_x & souris_y dans a1 & a2
;--------------------------------------------------------------------------
		lea	souris_x-tlk(a5),a1
		lea	souris_y-tlk(a5),a2




;--------------------------------------------------------------------------
;		souris : test déplacement horizontal
;--------------------------------------------------------------------------
		moveq	#0,d4
		moveq	#0,d5
		move.b	joy0dat+1(a6),d4
		move.b	joy0+1-tlk(a5),d5
		move.b	d4,joy0+1-tlk(a5)
		sub.b	d5,d4
		tst.b	d4
		bge.s	witri
		moveq	#0,d6
		move.b	d4,d6
		not.b	d6
		sub	d6,(a1)
		bra.s	neil
witri		add	d4,(a1)




;--------------------------------------------------------------------------
;		souris : test déplacement horizontal
;--------------------------------------------------------------------------
neil		moveq	#0,d4
		moveq	#0,d5
		move.b	joy0dat(a6),d4
		move.b	joy0-tlk(a5),d5
		move.b	d4,joy0-tlk(a5)
		sub.b	d5,d4
		tst.b	d4
		bge.s	witri2
		moveq	#0,d6
		move.b	d4,d6
		not.b	d6
		sub	d6,(a2)
		bra.s	neil2
witri2		add	d4,(a2)




;--------------------------------------------------------------------------
;		souris : test dépassement de coordonnées
;--------------------------------------------------------------------------

;		- dépas. gauche -

neil2		tst	(a1)			dépassement à gauche ?
		bpl.s	dxg			-> non
		move	(a1),d0
		neg	d0
		clr	(a1)
		move	d0,flag_left-tlk(a5)	défilement gauche demandé

;		- dépas. haut -

dxg		tst	(a2)			dépassement en haut ?
		bpl.s	dyb			-> non
		move	(a2),d0
		neg	d0
		clr	(a2)
		move	d0,flag_up-tlk(a5)	défilement haut demandé

;		- dépas. droite -

dyb		cmp	#320,(a1)		dépassement à droite ?
		blt.s	dyh			-> non
		move	(a1),d0
		sub	#319,d0
		move	#319,(a1)
		move	d0,flag_right-tlk(a5)	défilement droite demandé

;		- dépas. bas -

dyh		cmp	#256,(a2)		dépassement en bas ?
		blt.s	coor			-> non
		move	(a2),d0
		sub	#255,d0
		move	#255,(a2)
		move	d0,flag_down-tlk(a5)	défilement bas demandé




;--------------------------------------------------------------------------
;		souris : calcul mot de contrôle du lutin
;--------------------------------------------------------------------------
coor		moveq	#0,d0
		moveq	#0,d1
		move.l	asprite-tlk(a5),a0
		move	(a1),d0			pos. x du lutin
		move	(a2),d1			pos. y du lutin
		add	#s_rastx-1,d0
		add	#s_rasty,d1
		moveq	#11,d2			hauteur du lutin
		move.l	d1,d5
		add	d2,d5
		move.l	d0,d2
		and	#1,d2
		asr	#1,d0
		asl.l	#8,d1
		add	d1,d0
		swap	d1
		asl.l	#2,d1
		add	d1,d2
		asl.l	#8,d5
		add	d5,d2
		swap	d5
		asl	#1,d5
		add	d5,d2
		swap	d0
		move	#0,d0
		add	d2,d0
		move.l	d0,(a0)			écrit mot de contrôle
		rts


;=-=-=-=-=-=-=-=-=-=-=-> fin gestion de la souris <-=-=-=-=-=-=-=-=-=-=-=-=








;=========================================================================
;		vbl : test boutons de la souris
;=========================================================================
; entrée	a6.l		$dff000
;		a5.l		adr. table 'TLK'
; sortie	flag_souris.w	#0 pas de bouton
;				#1 bouton gauche
;				#2 bouton droit
;				#3 boutons gauche et droit en même temps
mouse_buttons:
;
;
;
;
		lea	flag_souris-tlk(a5),a0
		clr	(a0)
		btst	#6,ciaapra		bouton gauche ?
		bne.s	mb2			-> non
		move	#1,(a0)
mb2		btst	#10,potgor(a6)		bouton droit ?
		bne.s	mb3			-> non
		addq	#2,(a0)
mb3		rts


;=-=-=-=-=-=-=-=-> fin routine test boutons de la souris <-=-=-=-=-=-=-=-=-
;
;
;
;
;-+-> fin interruption de niveau 3, routine appéllées par 'irq_level3' <-+-








;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
;	<*>	interruption de niveau 3, disque AmigaDos : Wait X secondes
;
;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
zend_irq:	
;
;
;
;
		move.l	a0,-(a7)
		lea	waitdisk(pc),a0
		subq	#1,(a0)
		move.l	(a7)+,a0
zair		jmp	0		;*
waitdisk	dc	0
;
;
;
;
;-+-+-+-+-> fin irq après sauvegarde, decrémentant compt. pause <-+-+-+-+-+








;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
;	<*>	interruption de niveau 2, interruption sclavier 
;
;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
; utilise	d0 / a0
;
irq_level2:
;
;
;
;
		movem.l	d0/a0,-(a7)	
		btst	#3,$bfed01		irq du port série ?
		beq.s	irq2fin
		moveq	#0,d0
		move.b	$bfec01,d0
		bset	#6,$bfee01

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
		bclr	#6,$bfee01

		not.b	d0
		ror.b	#1,d0			code clavier
		lea	clara(pc),a0
		cmp	#128,d0			touche enfoncée ?
		blt.s	clara_no		oui
		move	d0,-4(a0)
		sub	#128,d0		
		move.b	#0,(a0,d0)
		tst	-2(a0)			nbre de touche enfoncée nul ?
		beq.s	irq2ok
		subq	#1,-2(a0)
		bra.s	irq2ok
clara_no:	move	d0,-4(a0)
		move.b	#1,(a0,d0)
		cmp	#$45,d0			touche "esc" ?
		beq	superquit		-> oui, retour sous le système
		addq	#1,-2(a0)
irq2ok		
irq2fin:	move	#$0008,$dff000+intreq
		movem.l	(a7)+,d0/a0
		rte
;
;
;
;
;-+-+-+-+-> fin interruption de niveau 2, interruption clavier  <-+-+-+-+-+








;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;
;		table
;
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;
;
;
;
;
;
;
;
;==========================================================================
;		table clavier
;==========================================================================
		dc	0			code dernère touche pressée
		dc	0			nbre de touche enfoncées
clara		dcb.b	128,0			table des touches








;==========================================================================
;		table 'tlk'
;==========================================================================
tintin		dc.l	0
flag_vbl	dc	0		compteur vbl
flag_prg	dc	0
flag_souris	dc	0		flag bouton souris préssé 1 gauche / 2 droit / 3 gauche & droit
souris_x	dc	0		pos. x pointeur souris
souris_y	dc	0		pos. y pointeur souris
joy0		dc	0		sauvegarde du registre 'joy0dat'
asprite		dc.l	0		adr. pointeur souris
select_copper	dc.l	0		adr. fin liste copper
flag_left	dc	0		dépl. demandé à gauche
flag_right	dc	0		dépl. demandé à droite
flag_up		dc	0		dépl. demandé en haut
flag_down	dc	0		dépl. demandé en bas
page_x		dc	0		pos. x de la page dalle
page_y		dc	0		pos. y de la page dalle
adecor_y	dc.l	0		adr. ecran 1er mot ligne vertical
adecor_x	dc.l	0		adr. ecran 1er mot ligne horz.
a_cycles	dc	0		compt. aff. ligne vert. pointillé
a_cycles2	dc	0		point. table aff. ligne horz. pointil.
cycles_wait	dc	0		compt d'attente animat des pointil.
flag_prise	dc	0		si 1 alors prend un block / 2 block prix
block_x1	dc	0
block_y1	dc	0
block_x2	dc	0
block_y2	dc	0
sblock_x2	dc	0
sblock_y2	dc	0
adecor_x1	dc.l	0		adr. ecran 1er mot ligne horz. 1 block
adecor_x2	dc.l	0		adr. ecran 1er mot ligne horz. 2 block
adecor_y1	dc.l	0		adr. ecran 1er mot ligne vert. 1 block
adecor_y2	dc.l	0		adr. ecran 1er mot ligne vert. 2 block
flag_eddd	dc	0		flag go editeur de dalles
flag_sddd	dc	0		flag go sélecteur de dalles
flag_first	dc	0		flag 1er init. scroll
dalle_large	dc	0		largeur dalle à afficher
dalle_haut	dc	0		hauteur dalle à afficher
d_window	dc	0		fenêtre blitter de la dalle à afficher
d_adr		dc	0,0,0,0		adresses d'affichage de la dalle
d_wdw		dc	0,0,0,0		fenêtres blitter aff. dalle
d_mod		dc	0,0,0,0		modulo blitter aff. dalle destination
d_posx		dc	0		position x pixel de dalle
d_posy		dc	0		position y pixel de dalle
stop_mouse	dc	0		flag si = 1 pas test souris pendant la VBL
flag_load	dc	0		si 1 alors chargement à eu lieu

tlk:		dc	0

;		- adr. utilisés pour le défilement -

zecran1		dc.l	0		adr. 3ieme buffer écran
zecran2		dc.l	0		adr. 2ieme buffer écran
zecran3		dc.l	0		adr. 3 buffer écran
aecran1		dc.l	0		adr. 3ieme buffer écran
aecran2		dc.l	0		adr. 2ieme buffer écran
aecran3		dc.l	0		adr. 3 buffer écran
adalles		dc.l	0		adr. page dalles
nu_video	dc	0		n° ligne changement pointeurs vidéos
afline		dc	0		adr. rel. 1er ligne
asc_map		dc.l	0		adr. map-editor
sc_map1		dc.l	0		map-editor 1er ligne écran
asc_end		dc.l	0		dernière ligne map-editor
sc_y1		dc	0		pos. y sur la page écran 3
sc_copper	dc.l	0		adr. liste copper en cours
mmecran		dc	0		mot fort adr. écran en cours	
ffecran		dc	0		adr. dernière copie
flag_co		dc	0		si flag = 1 alors copie à faire dans l'autre écran
zcopper1	dc.l	0		adr. 1er liste copper
zcopper2	dc.l	0		adr. 2ieme liste copper

;		- adr. utilisées pour les lignes dans le scroll -

ed_flag_bloc	dc	0		flag>0 le joueur prends un bloc
ed_blocx1	dc	0		position x mini de la brush
ed_blocy1	dc	0		position y mini de la brush
ed_blocx2	dc	0		position x maxi de la brush
ed_blocy2	dc	0		position y maxi de la brush
ed_hori1_adr	dc.l	0		adr. ecran ligne horizontale 1
		dc.l	0
ed_hori2_adr	dc.l	0		adr. ecran ligne horizontale 2
		dc.l	0
ed_hori3_adr	dc.l	0		adr. ecran ligne horizontale 3
		dc.l	0
ed_vert1_adr	dc.l	0		adr. ecran ligne verticale 1
		dc.l	0
ed_vert2_adr	dc.l	0		adr. ecran ligne verticale 2
		dc.l	0
ed_long2_adr	dc	0		longeur de l' horizontale 2
ed_long3_adr	dc	0		longeur de l' horizontale 3
		dc	0
		dc	0
human_league	dc	0		flag : 1 = brush neuve
page_brush	dc.l	0		adr. de la page brosse




;==========================================================================
;		table du "map-editor" de la brush
;==========================================================================
sc_map_brush	dcb	20*16,0




;==========================================================================
;		tampon sauvegarde décor page ecran dalle de 320*544
;==========================================================================
decor_x		dcb	040*3,0			ligne centrage horz.
decor_y		dcb	956*3,0			ligne centrage vert.
		dc	0			ligne block horz. 1
decor_x1	dcb	940*3,0
		dc	0			ligne block horz. 2
decor_x2	dcb	940*3,0
		dc	0			ligne block vert. 1
decor_y1	dcb	544*3,0
		dc	0			ligne block vert. 2
decor_y2	dcb	544*3,0







;==========================================================================
;		table couleur
;==========================================================================
color:		dc	$000,$468,$357,$135,$99b,$779,$557,$246
		dc	$000,$234,$123,$012,$446,$334,$223,$123
		*dc	$000,$224,$446,$668,$88a,$aac,$cce,$eee
		dc	$000,$f00,$a00,$800,$88a,$aac,$cce,$eee
		dc	$ee0,$fc6,$fa4,$d70,$a30,$720,$ff0,$fff








;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
;		tables du scrolling
;
;+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
;
;
;
;
;
;==========================================================================
;		adr. relative des dalles du "map-editor"
;==========================================================================
sc_mapadr:	dcb.l	d_ndalle,0







;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;
;		table recopiée en >CHIP RAM<
;
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;
;
;
;
;
;
;
;
;==========================================================================
;		graphisme du pointeur souris : lutin 4 couleurs 16*11
;==========================================================================
sprite:		dc.l	0
		dc.l	$fc00fc00,$fe008200,$8600fa00,$8c00f400
		dc.l	$8600fa00,$9300fd00,$69806e80,$04c00740
		dc.l	$026003a0,$014001c0,$00800080
		dc.l	0
spriteend




;==========================================================================
;		liste copper 1 sélecteur de dalle
;==========================================================================
sc_c1:		;dc	color00,$000

;		- init. pointeurs vidéos -

v		set	bpl1pth
sc_u1		
		rept	s_nplan
		dc	v,0,v+2,0
v		set	v+4
		endr

;		- init. pointeurs lutins -

v		set	spr0pth
sc_s1		
		rept	8
		dc	v,0,v+2,0
v		set	v+4
		endr

;		- fin de la liste / wait (255) / move #0,color00 -

sc_w1		dc	$ffff,$fffe
		dc	$0001,$fffe
		;dc	color00,$00f

;		- change pointeurs vidéos -

v		set	bpl1pth
sc_v1		
		rept	s_nplan
		dc	v,0,v+2,0
v		set	v+4
		endr

;		- fin de la liste -

		dc	$ffff,$fffe




;==========================================================================
;		liste copper 2 sélecteur de dalle
;==========================================================================
sc_c2:		;dc	color00,$000

;		- init. pointeurs vidéos -

v		set	bpl1pth
sc_u2		
		rept	s_nplan
		dc	v,0,v+2,0
v		set	v+4
		endr

;		- init. pointeurs lutins -

v		set	spr0pth
sc_s2		
		rept	8
		dc	v,0,v+2,0
v		set	v+4
		endr

;		- fin de la liste / wait (255) / move #0,color00 -

sc_w2		dc	$ffff,$fffe
		dc	$0001,$fffe
		;dc	color00,$00f
	
;		- change pointeurs vidéos -

v		set	bpl1pth
sc_v2		
		rept	s_nplan
		dc	v,0,v+2,0
v		set	v+4
		endr

;		- fin de la liste -

		dc	$ffff,$fffe
sc_c3







;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
f:		end
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>








	===================================================================
		* struture du programme *
	-------------------------------------------------------------------
	
	>*<			> déclaration des constantes

	>*< 			> déclaration des macros

	>*< 'first_init'	> première initialisation

	>*< 'main_loop'		> boucle principal

	>!< 			> routines chargement AmigaDos
	... 'load_map'		> charge map-editor
	... 'load_dalles'	> charge fichier IFF dalles de 320*544*8
	... 'requester'		> affiche fenêtre requester dans WorkbenchScreen
	... 'open_read'		> ouvre fichier AmigaDos en lecture
	... 'open_write'	> ouvre fichier AmigaDos en écriture
	... 'zend_load'		> ferme fichier + attend 3 secondes
	... 'zend_save'		> ferme fichier + attend 8 secondes
	... 'quitte'		> gèle le système, revient à l'éditeur

	>*< 'mysterious'	> éditeur de dalles (appellée par 'main_loop')

	>!<			> routines appellées par 'mysterious'
	... 'scroll'		: routine défilement vertical "map-editor"
	... 'first_brush'	: routine calculant et dessinant les 2 grandes lignes
	... 'make_brush'	: routine calculant et dessinant la brush
	... 'testclavier'	: test durée pression d'une touche
	... 'aff_dalle'		: affiche dalle (brosse)
	... 'reaff_decor_dalle'	: restitue décor sous la dalle (brosse)
	... 'insert_map'	: entre dalle(s) dans map-editor, à partir de la page de dalles (adr. dans sc_mapadr)

	>*< 'lugubre'		> sélecteur de dalles (appellée par 'main_loop')

	>!< 			> routines appellées par 'lugubre'
	... 'scroll_init'	: init. scrolling
	... 'aff_lignes'	: aff. les lignes centrage & blocks

	-------------------------------------------------------------------
	
	<*> 'irq_level3'	> interruption niveau 3 : vbl

	<!> 			> routines appellées par 'irq_level3'

	<*> 'zend_irq'		> interruption niveau 3 : pour le "waitdisk"

	<*> 'irq_level2'	> interruption niveau 2 : clavier

	-------------------------------------------------------------------

	.=. 'clara'		> table clavier

	.=. 'tlk'		> table principal
	
	.=. 'color'		> table couleur
	
	.=. 'sc_mapadr'		> scroll : adr. relat. des dalles dans la page dalle

	-------------------------------------------------------------------

	=*= 'sprite'		> GFX pointeur souris

	=*= 'sc_c1'		> liste copper 1 utilisé par l'éditeur de dalles

	=*= 'sc_c2'		> liste copper 2 utilisé par l'éditeur de dalles

	===================================================================

