***************************************************************************
*----------------- Test les 2 boutons souris des 2 ports -----------------*
***************************************************************************
* Entrée	=> a5.l	Adr. 't'
mouse_buttons:
		*/ SOURIS PORT 0 */
		clr.w	(mouse0-t,a5)
		tst.b	(mouse0_left-t,a5)
		beq.s	.nocreve0
		move.w	#1,(mouse0-t,a5)
.nocreve0	tst.b	(mouse0_right-t,a5)
		beq.s	.creve0
		addq.w	#2,(mouse0-t,a5)
.creve0
		*/ SOURIS PORT 1 */
		clr.w	(mouse1-t,a5)
		tst.b	(mouse1_left-t,a5)
		beq.s	.nocreve1
		move.w	#1,(mouse1-t,a5)
.nocreve1	tst.b	(mouse1_right-t,a5)
		beq.s	.creve1
		addq.w	#2,(mouse1-t,a5)
.creve1		rts

