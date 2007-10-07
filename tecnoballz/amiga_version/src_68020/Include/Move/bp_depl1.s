*=========================================================================*
;		Jeu : Test souris en mode "alone"		   *AMIGA*;
*=========================================================================*
* entrée	=> a5.l	adr. table 't'
bp_depl1:
		tst.w	(rak.team-t,a5)
		bne	bp_team		
		lea	(b1-t,a5),a4
		moveq	#0,d6
		move.w	d6,(rak.dep-t,a5)	raz flag : pas de dépl.
		move.w	d6,(rak.vdd-t,a5)	raz vites. dépl
		move.w	d6,(rak.vgg-t,a5)	raz vites. dépl
		move.l	d6,d4
		move.l	d6,d5
		move.b	(1+JOY0DAT,a6),d4	nvelle coord.
		move.b	(rak.axx-t,a5),d5	ancienne coord.
		move.b	d4,(rak.axx-t,a5)	sauve nvelle coord.
		sub.b	d5,d4			diff. en d4
		cmp.w	#BOUTON_BLITZ,(mouse0-t,a5)
		beq	.bp_depl1_end		-> oui
		tst.w	(rak.blitz-t,a5)	blizt sort de la raqu ?
		bne	.bp_depl1_end		-> oui
		tst.w	(rak.inver-t,a5)	inverseur ?
		beq.s	.philips		-> non
		not.b	d4
.philips	tst.b	d4
		bgt.s	.witri
		beq	.bp_depl1_end

		* déplacement à gauche *
		not.b	d4
		moveq	#BP_MIN,d0
		cmp.w	(BUMP.X,a4),d0
		bge	.bp_bloque
		move.w	#1,(rak.dep-t,a5)		flag dépl. à gauche
		sub.w	d4,(BUMP.X,a4)			x raq. 1
		sub.w	d4,(2*BUMP.SIZE+BUMP.X,a4)	x raq. 3
		sub.w	d4,(1*BUMP.SIZE+BUMP.Y,a4)	y raq. 2
		sub.w	d4,(3*BUMP.SIZE+BUMP.Y,a4)	y raq. 4
		cmp.w	(BUMP.X,a4),d0
		bge.s	.bp_bloque
		move.w	d4,(rak.vgg-t,a5)
		bra.s	.bp_depl1_end

		*/ déplacement à droite */
.witri:		move.w	#BP_MAX,d0
		sub.w	(rak.large-t,a5),d0
		cmp.w	(BUMP.X,a4),d0
		ble.s	.bp_bloque
		move.w	#2,(rak.dep-t,a5)		flag dépl. à droite
		add.w	d4,(BUMP.X,a4)			x raq. 1
		add.w	d4,(2*BUMP.SIZE+BUMP.X,a4)	x raq. 3
		add.w	d4,(1*BUMP.SIZE+BUMP.Y,a4)	y raq. 2
		add.w	d4,(3*BUMP.SIZE+BUMP.Y,a4)	y raq. 4
		cmp.w	(BUMP.X,a4),d0
		ble.s	.bp_bloque
		move.w	d4,(rak.vdd-t,a5)
		bra.s	.bp_depl1_end

		*/ Dépassement bloque raquette */
.bp_bloque	move.w	d0,(BUMP.X,a4)			x raq. 1
		move.w	d0,(2*BUMP.SIZE+BUMP.X,a4)	x raq. 3
		move.w	d0,(1*BUMP.SIZE+BUMP.Y,a4)	y raq. 2
		move.w	d0,(3*BUMP.SIZE+BUMP.Y,a4)	y raq. 4
.bp_depl1_end:	

		*/ Calcul table de collision suivant la vitesse */
		cmp.w	#1,(rak.dep-t,a5)	déplacement à gauche ?
		beq.s	.left			-> oui
		move.w	(rak.vdd-t,a5),d0
		move.l	(BUMP.RBRIGHT,a4),a0
		move.l	(BUMP.SIZE+BUMP.RBRIGHT,a4),a1
		move.l	(2*BUMP.SIZE+BUMP.RBRIGHT,a4),a2
		move.l	(3*BUMP.SIZE+BUMP.RBRIGHT,a4),a3
		bra.s	.zl
.left		move.w	(rak.vgg-t,a5),d0
		move.l	(BUMP.RBLEFT,a4),a0
		move.l	(BUMP.SIZE+BUMP.RBLEFT,a4),a1
		move.l	(2*BUMP.SIZE+BUMP.RBLEFT,a4),a2
		move.l	(3*BUMP.SIZE+BUMP.RBLEFT,a4),a3
.zl		cmp.w	#10,d0
		blt.s	.out
		moveq	#10,d0
.out		move.l	(a0,d0*4),a0
		move.l	(a1,d0*4),a1
		move.l	(a2,d0*4),a2
		move.l	(a3,d0*4),a3
		move.l	a0,(BUMP.REBOND,a4)
		move.l	a1,(BUMP.SIZE+BUMP.REBOND,a4)
		move.l	a2,(2*BUMP.SIZE+BUMP.REBOND,a4)
		move.l	a3,(3*BUMP.SIZE+BUMP.REBOND,a4)
		bra	atomino


;=========================================================================;
;		jeu : test déplacement souris en mode "team"	    *AMIGA*
;=========================================================================;
bp_team:
		**/ test souris team : test souris port 0 **/
		lea	(b1-t,a5),a4
		moveq	#0,d6
		move	d6,(rak.dep-t,a5)		raz flag : pas de dépl.
		move	d6,(rak.vdd-t,a5)		raz vites. dépl
		move	d6,(rak.vgg-t,a5)		raz vites. dépl
		move.l	d6,d4
		move.l	d6,d5
		move.b	(1+JOY0DAT,a6),d4	nvelle coord.
		move.b	(rak.axx-t,a5),d5	ancienne coord.
		move.b	d4,(rak.axx-t,a5)	sauve nvelle coord.
		sub.b	d5,d4			diff. en d4
		cmp	#BOUTON_BLITZ,(mouse0-t,a5)
		beq	bp_team1		-> oui
		tst.b	(flag_pause-t,a5)	pause ?
		bne	bp_team_end		-> oui
		tst	(blitz.out-t,a5)	blizt sort de la raqu ?
		bne	bp_team1		-> oui
		tst	(rak.inver-t,a5)		inverseur ?
		beq.s	filips			-> non
		not.b	d4
filips		tst.b	d4
		bgt.s	fitri
		beq.s	bp_team1

		*- test souris port 0 : dépl. à gauche -*
		not.b	d4
		moveq	#BP_MIN,d0
		cmp.w	(BUMP.X,a4),d0
		bge	bp_bloqua
		move.w	#1,(rak.dep-t,a5)	flag dépl. à gauche
		sub.w	d4,(BUMP.X,a4)			x raq. 1
		sub.w	d4,(BUMP.SIZE*2+BUMP.X,a4)	x raq. 3
		cmp.w	(BUMP.X,a4),d0
		bge.s	bp_bloqua
		move	d4,(rak.vgg-t,a5)
		bra.s	bp_team1

		*- test souris port 0 : dépl. à droite -*
fitri:		move.w	#BP_MAX,d0
		sub.w	(rak.large-t,a5),d0
		cmp.w	(BUMP.X,a4),d0
		ble.s	bp_bloqua
		move	#2,(rak.dep-t,a5)	flag dépl. à droite
		add.w	d4,(BUMP.X,a4)			x raq. 1
		add.w	d4,(BUMP.SIZE*2+BUMP.X,a4)	x raq. 3
		cmp.w	(BUMP.X,a4),d0
		ble.s	bp_bloqua
		move	d4,(rak.vdd-t,a5)
		bra.s	bp_team1

		*- test souris port 0 : dépass. bloque raqu. -*
bp_bloqua	move	d0,(BUMP.X,a4)			x raq. 1
		move	d0,(BUMP.SIZE*2+BUMP.X,a4)	x raq. 3

*----------------* test souris team : test souris port 1 *----------------*
bp_team1	lea	(b1-t,a5),a4
		moveq	#0,d6
		move	d6,(rak.mov-t,a5)		raz flag : pas de dépl.
		move	d6,(rak.srr-t,a5)		raz vites. dépl
		move	d6,(rak.sll-t,a5)		raz vites. dépl
		move.l	d6,d4
		move.l	d6,d5
		move.b	(1+JOY1DAT,a6),d4	nvelle coord.
		move.b	(rak.axx2-t,a5),d5	ancienne coord.
		move.b	d4,(rak.axx2-t,a5)	sauve nvelle coord.
		sub.b	d5,d4			diff. en d4
		tst	(rak.inver-t,a5)		inverseur ?
		beq.s	milips			-> non
		not.b	d4
milips		tst.b	d4
		bgt.s	mitri
		beq	atomino

		*- test souris port 1 : dépl. à gauche -*
		not.b	d4
		moveq	#BP_MIN,d0
		cmp.w	(BUMP.SIZE+BUMP.Y,a4),d0
		bge	bp_bloqui
		move	#1,(rak.mov-t,a5)		flag dépl. à gauche
		sub.w	d4,(BUMP.SIZE+BUMP.Y,a4)		y raq. 2 
		sub.w	d4,(BUMP.SIZE*3+BUMP.Y,a4)	y raq. 4
		cmp.w	(BUMP.SIZE+BUMP.Y,a4),d0
		bge.s	bp_bloqui
		move.w	d4,(rak.sll-t,a5)
		bra	atomino

		*- test souris port 1 : dépl. à droite -*
mitri:		move.w	#BP_MAX,d0
		sub.w	(rak.large-t,a5),d0
		cmp.w	(BUMP.SIZE+BUMP.Y,a4),d0
		ble.s	bp_bloqui
		move.w	#2,(rak.mov-t,a5)	flag dépl. à droite
		add.w	d4,(BUMP.SIZE+BUMP.Y,a4)	y raq. 2
		add.w	d4,(BUMP.SIZE*3+BUMP.Y,a4)	y raq. 4 
		cmp.w	(BUMP.SIZE+BUMP.Y,a4),d0
		ble.s	bp_bloqui
		move	d4,(rak.srr-t,a5)
		bra	atomino

		*- test souris port 1 : dépass. bloque raqu. -*
bp_bloqui	move.w	d0,(BUMP.SIZE+BUMP.Y,a4)	y raq. 2
		move.w	d0,(BUMP.SIZE*3+BUMP.Y,a4)	y raq. 4
bp_team_end

		*/ Calcul table de collision suivant la vitesse */
		cmp.w	#1,(rak.dep-t,a5)	déplacement à gauche ?
		beq.s	.left			-> oui
		move.w	(rak.vdd-t,a5),d0
		move.w	(rak.srr-t,a5),d1
		move.l	(BUMP.RBRIGHT,a4),a0
		move.l	(BUMP.SIZE+BUMP.RBRIGHT,a4),a1
		move.l	(2*BUMP.SIZE+BUMP.RBRIGHT,a4),a2
		move.l	(3*BUMP.SIZE+BUMP.RBRIGHT,a4),a3
		bra.s	.zl
.left		move.w	(rak.vgg-t,a5),d0
		move.w	(rak.sll-t,a5),d1
		move.l	(BUMP.RBLEFT,a4),a0
		move.l	(BUMP.SIZE+BUMP.RBLEFT,a4),a1
		move.l	(2*BUMP.SIZE+BUMP.RBLEFT,a4),a2
		move.l	(3*BUMP.SIZE+BUMP.RBLEFT,a4),a3
.zl		cmp.w	#10,d0
		blt.s	.out
		moveq	#10,d0
.out		cmp.w	#10,d1
		blt.s	.out2
		moveq	#10,d1
.out2		move.l	(a0,d0*4),a0
		move.l	(a1,d1*4),a1
		move.l	(a2,d0*4),a2
		move.l	(a3,d1*4),a3
		move.l	a0,(BUMP.REBOND,a4)
		move.l	a1,(BUMP.SIZE+BUMP.REBOND,a4)
		move.l	a2,(2*BUMP.SIZE+BUMP.REBOND,a4)
		move.l	a3,(3*BUMP.SIZE+BUMP.REBOND,a4)


atomino:



		*/ Recopie les coordonnées dans les structures BOBS */
		*/ Pointe la bonne taille de raquette en GFX-BOBS */
		move.w	(rak.large-t,a5),d0	largeur 16,24,32,40,48,56,64
		sub.w	#16,d0

		move.l	(BUMP.BOB,a4),a1
		move.w	d0,(BOB.PTANIM,a1)
		move.w	(BUMP.X,a4),(BOB.POSX,a1)
		move.w	(BUMP.Y,a4),(BOB.POSY,a1)
		move.l	(BOB.TABLE,a1),a0	a0=table adr. gfx & msk
		add.w	d0,a0
		move.l	(a0)+,(BOB.GFX,a1)
		move.l	(a0),(BOB.MSK,a1)

		move.l	(BUMP.SIZE+BUMP.BOB,a4),a1
		move.w	d0,(BOB.PTANIM,a1)
		move.w	(BUMP.SIZE+BUMP.X,a4),(BOB.POSX,a1)
		move.w	(BUMP.SIZE+BUMP.Y,a4),(BOB.POSY,a1)
		move.l	(BOB.TABLE,a1),a0	a0=table adr. gfx & msk
		add.w	d0,a0
		move.l	(a0)+,(BOB.GFX,a1)
		move.l	(a0),(BOB.MSK,a1)

		move.l	(2*BUMP.SIZE+BUMP.BOB,a4),a1
		move.w	d0,(BOB.PTANIM,a1)
		move.w	(2*BUMP.SIZE+BUMP.X,a4),(BOB.POSX,a1)
		move.w	(2*BUMP.SIZE+BUMP.Y,a4),(BOB.POSY,a1)
		move.l	(BOB.TABLE,a1),a0	a0=table adr. gfx & msk
		add.w	d0,a0
		move.l	(a0)+,(BOB.GFX,a1)
		move.l	(a0),(BOB.MSK,a1)

		move.l	(3*BUMP.SIZE+BUMP.BOB,a4),a1
		move.w	d0,(BOB.PTANIM,a1)
		move.w	(3*BUMP.SIZE+BUMP.X,a4),(BOB.POSX,a1)
		move.w	(3*BUMP.SIZE+BUMP.Y,a4),(BOB.POSY,a1)
		move.l	(BOB.TABLE,a1),a0	a0=table adr. gfx & msk
		add.w	d0,a0
		move.l	(a0)+,(BOB.GFX,a1)
		move.l	(a0),(BOB.MSK,a1)
		rts

