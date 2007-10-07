*-------------------------------------------------------------------------*
*********** Recopie un Graphisme 4 couleurs dans un lutin AGA *************
*-------------------------------------------------------------------------*
* Entrée	=> d0.w	nombre de ligne du lutin - 1
*		=> d1.w	taille d'un plan raw-blitter
*		=> d2.w	taille d'une ligne
*		=> a0.l	Adr. du lutin 
*		=> a2.l	Adr. du GFX
* utilise (d0/d1/d2/a0/a2/a4)
* modifie (a0)

*»»»»»»»»»»»»»»»»»»» VERSION LUTIN 64 PIXELS DE LARGE ««««««««««««««««««««*
copytolutin4.3:
		lea	(16,a0),a0
copylutin4.3:	movem.l	d0/a2/a4,-(a7)
.loop		move.l	a2,a4
		move.l	(a4),(a0)+		Plan 1
		move.l	(4,a4),(a0)+		Plan 1
		add.w	d1,a4
		move.l	(a4),(a0)+		Plan 2
		move.l	(4,a4),(a0)+		Plan 2
		add.w	d2,a2
		dbf	d0,.loop
		movem.l	(a7)+,d0/a2/a4
		rts

*»»»»»»»»»»»»»»»»»»» VERSION LUTIN 32 PIXELS DE LARGE ««««««««««««««««««««*
copytolutin4.2:
		addq.l	#8,a0
copylutin4.2:	movem.l	d0/a2/a4,-(a7)
.loop		move.l	a2,a4
		move.l	(a4),(a0)+		Plan 1
		add.w	d1,a4
		move.l	(a4),(a0)+		Plan 2
		add.w	d2,a2
		dbf	d0,.loop
		movem.l	(a7)+,d0/a2/a4
		rts

*»»»»»»»»»»»»»»»»»»» VERSION LUTIN 16 PIXELS DE LARGE ««««««««««««««««««««*
copytolutin4.0:
		addq.l	#4,a0
copylutin4.0:	movem.l	d0/a2/a4,-(a7)
.loop		move.l	a2,a4
		move.w	(a4),(a0)+		Plan 1
		add.w	d1,a4
		move.w	(a4),(a0)+		Plan 2
		add.w	d2,a2
		dbf	d0,.loop
		movem.l	(a7)+,d0/a2/a4
		rts
