*»»»»»»»»»»»						      ««««««««««««*
*>>>>>>>>>>>> INTERRUPTION DE NIVEAU 3 : Temps mort vertical <<<<<<<<<<<<<*
*»»»»»»»»»»»						      ««««««««««««*
interruption_master:
		movem.l	d0-d7/a0-a6,-(a7)
		lea	CUSTOM,a6
		lea	(interruption_master,pc),a5
		add.l	#t-interruption_master,a5
		addq.w	#1,(vbl_counter-t,a5)
		bsr	vbl_pointe_video
		tst.b	(vblfade.flag-t,a5)
		bne.s	.nofade
		bsr	vbl_fade_on
		bsr	vbl_fade_off
.nofade
		IFNE	FLAG_MOUSE0
		bsr	move_mouse
		ENDC
		bsr	vbl_test_joysticks
		bsr	vbl_mouse_buttons
		bsr	extra_vbl
		movem.l	(a7)+,d0-d7/a0-a6
		move	#$0020,CUSTOM+INTREQ
		rte
