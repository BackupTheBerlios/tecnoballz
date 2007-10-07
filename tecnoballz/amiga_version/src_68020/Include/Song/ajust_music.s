*=========================================================================*
*                 Définit préferences musique / bruitage		  *
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
*-------------------------------------------------------------------------*
* music.flag=0 music4voix=1 (F) musicsound=0 (V)	(Bruitages & Musique)
* music.flag=1 music4voix=1 (F) musicsound=1 (F)	(Bruitages seulement)	
* music.flag=2 music4voix=0 (V) musicsound=1 (F)	(Musique seulement)
*-------------------------------------------------------------------------*
ajust_music:
		move.w	#$2000,($09A,a6)
		move.w	#1,(music4voix-t,a5)	flag 3 cannaux de musique
		clr.w	(musicsound-t,a5)	flag bruitages + musique
		tst.w	(music.flag-t,a5)	musique + bruitages ?
		beq.s	.amx			-> oui

		move.w	#1,(musicsound-t,a5)
		cmp.w	#1,(music.flag-t,a5)	bruitages seulement ?
		beq.s	.amx			-> oui

		move.w	#2,(musicsound-t,a5)	musique seulement
		clr.w	(music4voix-t,a5)	musique 4 cannaux
		bra.s	.am

.amx		move.w	#$E000,($09A,a6)
.am		moveq	#0,d0
		move.w	d0,(AUD0VOL,a6)
		move.w	d0,(AUD1VOL,a6)
		move.w	d0,(AUD2VOL,a6)
		move.w	d0,(AUD3VOL,a6)
		move.w	#$F,(DMACON,a6)
		rts
