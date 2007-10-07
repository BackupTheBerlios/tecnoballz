*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*
***************> VBL : Test les mouvement des 2 joysticks <****************
*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l	CUSTOM
vbl_test_joysticks:
		lea	(joy0_right-t,a5),a0
		move.w	(JOY0DAT,a6),d0
		btst	#1,d0			joystick vers la gauche ?
		sne	(a0)+
		btst	#9,d0			joystick vers la droite ?
		sne	(a0)+
		move.w	d0,d1
		lsr.w	#1,d1
		eor.w	d0,d1
		btst	#0,d1			joystick vers en bas ?
		sne	(a0)+
		btst	#8,d1			joystick vers le haut .?
		sne	(a0)+
		move.w	(JOY1DAT,a6),d0
		btst	#1,d0			joystick vers la gauche ?
		sne	(a0)+
		btst	#9,d0			joystick vers la droite ?
		sne	(a0)+
		move.w	d0,d1
		lsr.w	#1,d1
		eor.w	d0,d1
		btst	#0,d1			joystick vers en bas ?
		sne	(a0)+
		btst	#8,d1			joystick vers le haut .?
		sne	(a0)+
		rts
