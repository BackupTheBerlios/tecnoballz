*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
************************ ROUT=8 : Tableau Termin� *************************
*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
* Entr�e	=> a5.l	Adr. 't' 
propaganda:
		*/ Retour � l'�diteur */
		tst.w	(editor-t,a5)		Editeur en cours ?
		beq.s	.noed			-> non
		bsr.l	np_end
		bsr	rupt1.off
		move.w	#14,(rout-t,a5)
		bra	irq
.noed		bsr	nextplayer
		tst.l	d7			autre joueur ?
		bne.s	.propane		-> non
		bsr	rupt1.off
		clr.w	(rout-t,a5)
		bra	irq

		*/ Passe-t-on � un gardien */
.propane	move.w	(level-t,a5),d0
		cmp.w	#5,d0			gardien interm�diare ?
		beq.s	.guns_go			-> oui
		cmp.w	#11,d0			gardien final ?
		bne.s	.sandy			-> non
.guns_go	bsr	firstplayer
		tst.l	d7			plus de joueurs ?
		bne.s	.totalgameover		-> oui
		bsr.l	np_end
		bsr	level_plus
		bsr	super_famicom
		bsr	rupt1.off
		move.w	#4,(rout-t,a5)		>flag prepare gardien<
		bra	irq

		*/ Tableau suivant */
.sandy:		bsr	firstplayer
		tst.l	d7			plus de joueurs ?
		bne.s	.totalgameover		-> oui
		add.w	#TB.SIZE,(niv00-t,a5)
		bsr	level_plus
		bsr	super_famicom
		bsr	rupt1.off
		move.w	#2,(rout-t,a5)		>flag prepare magasin<
		bra	irq

		*/ Tous les joueurs sont d�finitivement morts */
.totalgameover:	move.w	#6,(rout-t,a5)		>flag pr�pare intro<
		bsr.l	np_end
		bsr	rupt1.off
		bra	irq

*�������������������������������������������������������������������������*
*-*************-* Lib�re m�moire utils�e par les tableaux *-*************-*
*�������������������������������������������������������������������������*
* Entr�e	=> a5.l	Adr. 't'
rupt1.off:
		bsr.s	.freemem
		bne.l	sys_fin
		rts
.freemem	clr.w	(tableau.end-t,a5)
		IFEQ	A_TAB
		bsr	shado.freemem
		bne	.exit
		bsr	bob32.freemem2
		bne	.exit
		bsr.l	kill_screen
		bne	.exit

		*/ Libere zones m�moire */
		lea	libere_mem,a4
		lea	(heads.pagegfx-t,a5),a0	Animation des t�tes
		jsr	(a4)
		bne	.exit
		lea	(blitz.lutin-t,a5),a0	Lutin du GigaBlitz
		jsr	(a4)
		bne	.exit
		lea	(bk.pagegfx-t,a5),a0	Page des briques
		jsr	(a4)
		bne	.exit
		lea	(tarte-t,a5),a0		Buffer sauvegarde fond Bricote
		jsr	(a4)
		bne	.exit
		lea	(gateau-t,a5),a0	Buffer sauvegarde fonds Briques
		jsr	(a4)
		bne	.exit
		lea	(rak.sprite-t,a5),a0	Sprites Raquettes
		jsr	(a4)
		bne.s	.exit
		lea	(rak.lutin-t,a5),a0	Liste Lutin 0 & 1 Raquettes
		jsr	(a4)
		bne.s	.exit
		lea	(rak.lutin2-t,a5),a0	Liste Lutin 2 & 3 Raquettes
		jsr	(a4)
		bne.s	.exit
		lea	(mess.fond-t,a5),a0	Buffer sauvegarde message
		jsr	(a4)
		bne.s	.exit
		lea	(pt_balle-t,a5),a0	Structures Balles
		jsr	(a4)
		bne.s	.exit
		lea	(pt_capsule-t,a5),a0	Structures Capsules
		jsr	(a4)
		bne.s	.exit
		lea	(sscap-t,a5),a0		SuperSprite Caspule
		jsr	(a4)
		bne.s	.exit
		lea	(ssinv-t,a5),a0		SuperSprite Inverseur
		jsr	(a4)
		bne.s	.exit
		lea	(hedgehog-t,a5),a0	Routine Print 2 plans
		jsr	(a4)
		bne.s	.exit
		lea	(pt_gadget-t,a5),a0	Structure Gadgets
		jsr	(a4)
		bne.s	.exit
		lea	(pt_objet-t,a5),a0	Structure Tirs raquettes
		jsr	(a4)
		bne.s	.exit
		lea	(mur_restore-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		lea	(mur_affiche-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		lea	(rak.lutinr-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		lea	(pt_oeil-t,a5),a0	BOB oeil
		jsr	(a4)
		bne.s	.exit
		lea	(pt_bobtext-t,a5),a0	Lettres LEVEL xx COMPLETED
		jsr	(a4)
		bne.s	.exit
		ENDC
		moveq	#0,d7
.exit		rts
