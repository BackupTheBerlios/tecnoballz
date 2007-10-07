*».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»»*
*---------------------- GESTION DES ATOMS-BOUIBOUI -----------------------*
*».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»».»»*
* Entrée	=> a5.l Adr. 't'
atom_dep:
		move.w	(hasard3-t,a5),d0
		and.w	#$F,d0
		lea	(ghost_trajec-t,a5,d0*4),a2

		move.l	(pt_atom-t,a5),a4
		moveq	#NATOM-1,d6
.loop		move.w	#1,(BOB.ACTIF,a4)

		*/ Explosion d'un Atom (Animation du BOB) */
		tst.w	(ATOM.EXPLO,a4)
		beq.s	.noanim2
		lea	(BOB.TEMPO,a4),a0
		subq.w	#1,(a0)+
		bne	.next
		move.w	(a0)+,(BOB.TEMPO,a4)
		move.w	(a0)+,d0
		cmp.w	#60,d0
		bls.s	.no_maxi
		clr.w	(BOB.ACTIF,a4)
		clr.w	(ATOM.EXPLO,a4)
		bra	.next
.no_maxi	addq.w	#8,(BOB.PTANIM,a4)	
		move.l	(BOB.TABLE,a4),a0
		add.w	d0,a0
		add.w	(ATOM.OFF,a4),a0
		move.l	(a0)+,(BOB.GFX,a4)
		move.l	(a0),(BOB.MSK,a4)
		bra	.next
.noanim2
		*/ Apparition de l'Atom */
		tst.w	(ATOM.ACTIF,a4)		Atom Actif ?
		beq.s	.lauly01		-> oui
		subq.w	#1,(ATOM.ACTIF,a4)
		bne.s	.noapp
		move.b	#1,(sound+S_ATOM.APPARAIT-t,a5)
		lea	(ghost.bob-t,a5),a0
		move.w	(hasard-t,a5),d0
		add.w	(VHPOSR,a6),d0
		add.w	(VPOSR,a6),d0
		add.w	d6,d0
		and.w	#31,d0
		move.w	(a0,d0*2),(ATOM.OFF,a4)
.noapp		clr.w	(BOB.ACTIF,a4)

		*/ Déplacement de l'Atom */
.lauly01	subq.w	#1,(ATOM.COUNT1,a4)	Change de direction ?
		bne.s	.milou			-> non
		move.l	(ATOM.GHOST,a4),a1	table tempo change trajectoire
		addq.w	#2,(ATOM.PT,a4)
		and.w	#$1F,(ATOM.PT,a4)
		add.w	(ATOM.PT,a4),a1
		move.w	(a1),(ATOM.COUNT1,a4)	Tempo même direction
		move.l	(a2)+,(ATOM.INC,a4)	Offsets x,y déplacement
.milou		move.l	(ATOM.INC,a4),d0
		add.l	d0,(ATOM.X,a4)

		*/ Animation de l'Atom */
		lea	(BOB.TEMPO,a4),a0
		subq.w	#1,(a0)+		tempo=tempo-1
		bne.s	.noanim			-> tempo > 0
		move.w	(a0)+,(BOB.TEMPO,a4)
		move.w	(a0)+,d0		d0=pt/animation
		cmp.w	#60,d0			pt < au pt maxi ?
		bls.s	.no_maxi2		-> oui
		moveq	#0,d0
		move.w	d0,(BOB.PTANIM,a4)
.no_maxi2	addq.w	#8,(BOB.PTANIM,a4)	
		move.l	(BOB.TABLE,a4),a0	a0=table adr. gfx & msk
		add.w	d0,a0
		add.w	(ATOM.OFF,a4),a0
		move.l	(a0)+,(BOB.GFX,a4)
		move.l	(a0),(BOB.MSK,a4)
.noanim		
		*/ Test dépassement de coordonnées */
		cmp.w	#ATOM_XMIN,(ATOM.X,a4)
		bhi.s	.noxmini
		move.w	#ATOM_XMIN,(ATOM.X,a4)
.noxmini	cmp.w	#ATOM_XMAX,(ATOM.X,a4)
		blt.s	.noxmaxi
		move.w	#ATOM_XMAX,(ATOM.X,a4)
.noxmaxi	cmp.w	#ATOM_YMIN,(ATOM.Y,a4)
		bhi.s	.noymini
		move.w	#ATOM_YMIN,(ATOM.Y,a4)		
.noymini	cmp.w	#ATOM_YMAX,(ATOM.Y,a4)		
		blt.s	.noymaxi
		move.w	#ATOM_YMAX,(ATOM.Y,a4)		
.noymaxi	
.next		lea	(ATOM.SIZE,a4),a4
		dbf	d6,.loop
		rts
