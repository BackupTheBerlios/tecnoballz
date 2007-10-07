*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*
************************* Installe l'écran de jeu *************************
*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*©*
* Entrée	a5.l	=> adr. table 't'
install_ecran:
		lea	(pt_screen-t,a5),a4
		bsr	screen_res
		bne	sexit
		move.w	#3,(SFMODE_LUTINS,a0)
install_ecran2:	move.w	#1,(SFREQ,a0)		force 50Hz
		move.w	#3,(SFMODE_PLANS,a0)
		move.w	#320,(SNPIXEL,a0)
		move.w	#256,(SNLIGNE,a0)
		move.b	#5,(SNPLAN,a0)
		move.w	#3,(SPBUFFER,a0)
		move.w	#320,(SPLARGE,a0)
		move.w	#256+32,(SPHAUTEUR,a0)
		move.w	#1,(SLUTIN,a0)		pt lutins dans la liste
		move.w	#$24,(SBPLCON2,a0)
		move.w	#$11,(SBPLCON4,a0)	Couleur lutin
		move.w	#20*TBC+4+27*20+16,(KKCOPPER,a0)		Intructions en +
		move.w	#4,(SFADEOFFSPEED,a0)
		move.w	#2,(SFADEONSPEED,a0)
		move.w	#1,(NBCOPPER,a0)	deux liste Copper
		bsr	init_screen
		bne	sexit
		move.l	a1,(degrade_copper-t,a5)
		move.l	a2,(4+degrade_copper-t,a5)
		move.l	(pt_palette-t,a5),a1
		lea	(SCOLOR_TABLE,a0),a2
		move.w	#32-1,d0
.loop		move.l	(a1)+,(a2)+
		dbf	d0,.loop
		moveq	#0,d0
		moveq	#0,d1
		move.w	(SCOORDX,a0),d0
		move.w	(SCOORDY,a0),d1
		lsl.w	#2,d0
		move.l	d0,(coordx-t,a5)
		move.l	d1,(coordy-t,a5)

		*/ CHERCHE DIWSTRT */
		move.l	(SACOPPER,a0),a1
		move.l	(SACOPPER2,a0),a2
		move.w	#DIWSTRT,d0
.loopdiw	addq.l	#2,a2
		cmp.w	(a1)+,d0
		bne.s	.loopdiw
		move.l	a1,(copper.diwstrt-t,a5)
		move.l	a2,(4+copper.diwstrt-t,a5)

		*/ Remplit le plan 4 */
install_ecran3	move.l	(pt_screen-t,a5),a0
		move.w	(SP_SPLAN,a0),d4
		mulu	#NOPLAN,d4
		add.l	(SPECRAN1,a0),d4
		move.w	(SP_SLINE,a0),d5
		sub.w	#32,d5
		move.w	(SPHAUTEUR,a0),d2
		lsl.w	#6,d2
		add.w	#16,d2
		moveq	#-1,d1
		WAITBLITTER
		move.w	d1,(BLTADAT,a6)
		move.l	d1,(BLTAFWM,a6)
		move.l	#$01F00000,(BLTCON0,a6)
		move.l	d4,(BLTDPTH,a6)
		move.w	d5,(BLTDMOD,a6)
		move.w	d2,(BLTSIZE,a6)

		*/ Ombre haut */
		WAITBLITTER
		move.w	#0,(BLTADAT,a6)
		move.l	d4,(BLTDPTH,a6)
		move.w	#6*64+16,(BLTSIZE,a6)

		*/ Ombre côté droit */
		move.w	(SPHAUTEUR,a0),d2
		subq.w	#6,d2
		lsl.w	#6,d2
		addq.w	#1,d2
		move.w	(SP_SLINE,a0),d5
		subq.w	#2,d5
		add.l	#30,d4
		move.w	(SP_SLINE,a0),d1
		mulu	#6,d1
		add.l	d1,d4
		WAITBLITTER
		move.w	#$FFC0,(BLTADAT,a6)
		move.w	d5,(BLTDMOD,a6)
		move.l	d4,(BLTDPTH,a6)
		move.w	d2,(BLTSIZE,a6)

		*/ SAUVE QUELQUES VALEURS */
		move.l	(SPECRAN1,a0),(ecran1-t,a5)
		move.l	(SPECRAN2,a0),(ecran2-t,a5)
		move.l	(SPECRAN3,a0),(ecran3-t,a5)
		moveq	#0,d7
sexit		rts
