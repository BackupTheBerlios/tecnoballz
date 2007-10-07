*=========================================================================*
*»»»»»»»»»»»»»»»»»»»» Test la souris port 0 et port 1 ««««««««««««««««««««*
*=========================================================================*
* Entrée	=> a5.l	Adr. tabel 't'
*		=> a6.l	CUSTOM
last_test_souris:

		*/ Déplacement raquette 1 */
		lea	(b1-t,a5),a4
		moveq	#0,d6
		move.w	d6,(rak.dep-t,a5)	RAZ flag : pas de dépl.
		move.w	d6,(rak.vdd-t,a5)	RAZ vites. dépl
		move.w	d6,(rak.vgg-t,a5)	RAZ vites. dépl
		move.l	d6,d4
		move.l	d6,d5
		move.b	(1+JOY0DAT,a6),d4	nvelle coord.
		move.b	(rak.axx-t,a5),d5	ancienne coord.
		move.b	d4,(rak.axx-t,a5)	sauve nvelle coord.
		sub.b	d5,d4			diff. en d4
		tst.b	(flag_pause-t,a5)	Pause ?
		bne	.lt_sourix		-> oui
		tst.w	(rak.inver-t,a5)	inverseur ?
		beq.s	.gphilips		-> non
		subq.w	#1,(rak.inver-t,a5)
		not.b	d4
.gphilips	tst.b	d4
		bgt.s	.gwitri
		beq.s	.gatomino

		*- dépl. à gauche -*
		not.b	d4
		moveq	#BP_MIN2,d0
		cmp.w	(BUMP.X,a4),d0
		bge	.gbp_bloque
		tst.w	(rak.team-t,a5)		mode "team" ?
		beq.s	.new_world

		*- en mode team test si les raquettes se touchent -*
		lea	(b4-t,a5),a0
		move.w	(BUMP.X,a0),d0
		add.w	(rak.large-t,a5),d0
		cmp.w	(BUMP.X,a4),d0
		bge	.gbp_bloque
.new_world	move.w	#1,(rak.dep-t,a5)	flag dépl. à gauche
		sub.w	d4,(BUMP.X,a4)		x raq. 1
		cmp.w	(BUMP.X,a4),d0
		bge.s	.gbp_bloque
		move.w	d4,(rak.vgg-t,a5)
		bra.s	.gatomino

		*- dépl. à droite -*
.gwitri:	move.w	#BP_MAX2,d0
		sub.w	(rak.large-t,a5),d0
		cmp.w	(BUMP.X,a4),d0
		ble.s	.gbp_bloque
		move.w	#2,(rak.dep-t,a5)	Flag dépl. à droite
		add.w	d4,(BUMP.X,a4)		x raq. 1
		cmp.w	(BUMP.X,a4),d0
		ble.s	.gbp_bloque
		move.w	d4,(rak.vdd-t,a5)
		bra.s	.gatomino

		*-=- dépass. bloque raqu. -=-*
.gbp_bloque	move.w	d0,(BUMP.X,a4)		x raq. 1


*------------*/ test souris port 1 : déplacement raquette 4 /*------------*
.gatomino	lea	(b4-t,a5),a4
		moveq	#0,d6
		move.w	d6,(rak.mov-t,a5)		raz flag : pas de dépl.
		move.w	d6,(rak.srr-t,a5)		raz vites. dépl
		move.w	d6,(rak.sll-t,a5)		raz vites. dépl
		move.l	d6,d4
		move.l	d6,d5
		move.b	(1+JOY1DAT,a6),d4	nvelle coord.
		move.b	(rak.axx2-t,a5),d5	ancienne coord.
		move.b	d4,(rak.axx2-t,a5)	sauve nvelle coord.
		sub.b	d5,d4			diff. en d4
		tst.w	(rak.inver-t,a5)	inverseur ?
		beq.s	.gfilips		-> non
		subq	#1,(rak.inver-t,a5)
		not.b	d4
.gfilips	tst.b	d4
		bgt.s	.gfitri
		beq.s	.lt_sourix

		* dépl. à gauche *
		not.b	d4
		moveq	#BP_MIN2,d0
		cmp.w	(BUMP.X,a4),d0
		bge	.gbp_bloqui
		move.w	#1,(rak.mov-t,a5)	Flag dépl. à gauche
		sub.w	d4,(BUMP.X,a4)		X raq. 2
		cmp.w	(BUMP.X,a4),d0
		bge.s	.gbp_bloqui
		move	d4,(rak.sll-t,a5)
		bra.s	.lt_sourix

		*- En mode team test si les raquettes se touchent -*
.gfitri:	lea	(b1-t,a5),a0
		move.w	(BUMP.X,a0),d0
		sub.w	#32,d0
		cmp.w	(BUMP.X,a4),d0
		ble.s	.gbp_bloqui

		*- dépl. à droite -*
		move.w	#2,(rak.mov-t,a5)	flag dépl. à droite
		add.w	d4,(BUMP.X,a4)		x raq. 2
		cmp.w	(BUMP.X,a4),d0
		ble.s	.gbp_bloqui
		move.w	d4,(rak.srr-t,a5)
		bra.s	.lt_sourix

		*- dépass. bloque raqu. -*
.gbp_bloqui	move.w	d0,(BUMP.X,a4)		x raq. 2
.lt_sourix:	
		*/ COORDONNEES BOBS */
		lea	(b1-t,a5),a4
		move.l	(BUMP.BOB,a4),a1
		move.w	(BUMP.X,a4),(BOB.POSX,a1)
		move.w	(BUMP.Y,a4),(BOB.POSY,a1)
		tst.w	(rak.team-t,a5)		mode "team" ?
		beq.s	.ex
		lea	(b4-t,a5),a4
		move.l	(BUMP.BOB,a4),a1
		move.w	(BUMP.X,a4),(BOB.POSX,a1)
		move.w	(BUMP.Y,a4),(BOB.POSY,a1)
.ex		rts
