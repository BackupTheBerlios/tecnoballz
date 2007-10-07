*=========================================================================*
*------------------------> INITIALISE LES ATOMS <-------------------------*
*=========================================================================*
* Entrée	=> a5.l	Adr "t"
init_atom:
		*/ Réserve structure ATOM */
		lea	(pt_atom-t,a5),a4
		move.l	#NATOM*ATOM.SIZE,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		bne	.exit

		*/ Réserve les BOBs */
		move.l	(pt_atom-t,a5),a1
		moveq	#NATOM-1,d6
.loop		moveq	#BOB.ATOMS,d0
		bsr	bob32.only
		bne	.exit
		moveq	#BOB.ATOMS,d0
		bsr	shado.ajoute
		bne	.exit
		clr.l	(BOB.ACTIF,a1)
		move.w	#ATOM_XMIN,(ATOM.X,a1)
		add.w	d6,(ATOM.X,a1)
		move.w	#ATOM_YMIN,(ATOM.Y,a1)
		add.w	d6,(ATOM.Y,a1)
		move.w	#3,(BOB.INITTEMPO,a1)
		move.w	#1,(ATOM.COUNT1,a1)
		move.w	(atom.resist-t,a5),(ATOM.POWER,a1)
		lea	(ATOM.SIZE,a1),a1
		dbf	d6,.loop

		*/ Boule d''initialisation 2 */
		lea	(ghost_c0-t,a5),a0
		lea	(ghost_c1-t,a5),a1
		lea	(ghost_c2-t,a5),a2
		lea	(ghost_c3-t,a5),a3
		move.l	(pt_atom-t,a5),a4
		moveq	#NATOM-1,d6
.loopx726	move.w	(atom1.app-t,a5),(ATOM.ACTIF,a4)
		add.w	d6,(ATOM.ACTIF,a4)
		move.l	a0,(ATOM.GHOST,a4)
		move.w	#O.EXPLO1,(ATOM.OEXP,a4)
		subq.w	#1,d6
		bmi.s	.cross
		lea	(ATOM.SIZE,a4),a4
		move.w	(atom2.app-t,a5),(ATOM.ACTIF,a4)
		add.w	d6,(ATOM.ACTIF,a4)
		move.w	#O.EXPLO2,(ATOM.OEXP,a4)
		move.l	a1,(ATOM.GHOST,a4)
		subq.w	#1,d6
		bmi.s	.cross
		lea	(ATOM.SIZE,a4),a4
		move.w	(atom3.app-t,a5),(ATOM.ACTIF,a4)
		add.w	d6,(ATOM.ACTIF,a4)
		move.w	#O.EXPLO1,(ATOM.OEXP,a4)
		move.l	a2,(ATOM.GHOST,a4)
		subq.w	#1,d6
		bmi.s	.cross
		lea	(ATOM.SIZE,a4),a4
		move.w	(atom4.app-t,a5),(ATOM.ACTIF,a4)
		add.w	d6,(ATOM.ACTIF,a4)
		move.w	#O.EXPLO2,(ATOM.OEXP,a4)
		move.l	a3,(ATOM.GHOST,a4)
		lea	(ATOM.SIZE,a4),a4
		dbf	d6,.loopx726
.cross		moveq	#0,d7
.exit		rts
