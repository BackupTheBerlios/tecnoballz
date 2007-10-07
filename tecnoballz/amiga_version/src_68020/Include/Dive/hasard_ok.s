*/////////////////////////////////////////////////////////////////////////*
*»»»»»»»»»»»»»»»»»»»»»» Modifie la valeur "hasard" «««««««««««««««««««««««*
*/////////////////////////////////////////////////////////////////////////*
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l CUSTOM
hasard_ok:
		move.w	(VHPOSR,a6),d0
		move.w	d0,d1
		move.w	(VPOSR,a6),d2
		and.w	#$00FF,d0		d0=Pos. X du faisceau
		lsr.w	#8,d1
		and.w	#$0001,d2
		lsl.w	#8,d2
		add	d2,d1			d1=Pos. Y du faisceau
		add.w	d1,(hasard-t,a5)
		add.w	d0,(hasard-t,a5)
		rts
