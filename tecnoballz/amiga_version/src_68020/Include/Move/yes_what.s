*=========================================================================*
*********************** Test si envoie Bonus/Malus ************************
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
yes_what:
		addq.w	#1,(brick.kass-t,a5)	inc. compt. brik cassé
		move.l	(pt_gadget-t,a5),a0
		moveq	#NGADGET-1,d7
.loop		tst.w	(GAD.ACTIF,a0)		Gadget libre ?
		beq.s	.toto_what		-> oui
		lea	(GAD.SIZE,a0),a0
		dbf	d7,.loop
		rts
	
		*-=-* gestion des malus *-=-*
.toto_what	addq.w	#1,(malus.step-t,a5)	Incrèmente compteur malus
		move.w	(malus.frek-t,a5),d0	Fréquence des malus
		cmp.w	(malus.step-t,a5),d0	On envoie un malus ?
		bge.s	.no_malus1		-> Non
		clr.w	(malus.step-t,a5)	RAZ compt. malus
		move.w	(hasard-t,a5),d0
		and.w	#63,d0			Table de 64 Gadgets
		add.w	d0,d0
		move.l	(table.malus-t,a5),a1	Adresse table malus
		add.w	d0,a1		
		bra.s	.res_heros

		*-=-* Gestion des Bonus *-=-*
.no_malus1	move.w	(bonus.tombe-t,a5),d0	Nombre de bonus achété(s)
		cmp.w	(bonus.achete-t,a5),d0	= Nombre bonus tombé(s) ?
		beq	.no_what		-> Oui, pas de bonus
		move.w	(bonus.step-t,a5),d0	Brique(s) à casser pour bonus
		cmp.w	(brick.kass-t,a5),d0	Nombre brique(s) suffisant ?
		bge.s	.no_what		-> non
		clr.w	(brick.kass-t,a5)	RAZ compteur briques cassés
		addq.w	#1,(bonus.tombe-t,a5)	Incrèmente bonus tombés
		lea	(course-t,a5),a1	Table bonus
		add.w	(pt.course-t,a5),a1
		tst.w	(a1)
		bne.s	.res_pnt_course
		clr.w	(pt.course-t,a5)
		lea	(course-t,a5),a1
		add.w	(pt.course-t,a5),a1
.res_pnt_course	addq.w	#2,(pt.course-t,a5)

		*-=-* initialise BOB >MALUS/BONUS< *-=-*
.res_heros	move.w	(a1),d0
		*move.w	d0,(gadget.num-t,a5)	n° bonus/malus
		move.w	d0,(GAD.NUM,a0)
		lea	(pascal-t,a5),a1
		move.w	(a1,d0),d0
		move.w	d0,(GAD.PTBOB,a0)	Pt/Animation du BOB
		move.l	(BRI.RAK,a2),a1
		move.l	a1,(GAD.RAK,a0)		Raquette où va le bonus/malus
		move.l	(BUMP.GADGET,a1),(GAD.DEP,a0)
		move.w	(BRI.X,a2),(GAD.X,a0)	X Bonus/Malus
		move.w	(BRI.Y,a2),(GAD.Y,a0)	Y Bonus/Malus
		move.w	#1,(GAD.ACTIF,a0)	Active Bonus/Malus
.no_what	rts
