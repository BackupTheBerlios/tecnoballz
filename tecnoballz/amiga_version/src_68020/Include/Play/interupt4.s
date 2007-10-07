*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
******************** ROUT=4 : Initialise les Gardiens *********************
*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
interupt4:



		bsr.l	np_end
		lea	(mod.gardi-t,a5),a3
		bsr.l	np_init
		bsr.l	ajust_music
		clr.w	(music.on-t,a5)
		bsr	gardien.init
		bne.l	sys_erreur
		bsr	init_bobtext2		|-> BOBs "LEVEL COMPLETED"
		bne.l	sys_erreur
		bsr	copy_s1_to_s2s3
		bsr	sprites_on
		move.w	#255,(flag_fade_on-t,a5)
		move.w	#5,(rout-t,a5)
		bra	irq
