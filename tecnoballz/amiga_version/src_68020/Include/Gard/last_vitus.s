*=========================================================================*
*                      Gestion de la balle + rebonds			  *
*=========================================================================*
* Entrée		=> a5.l	adr. table 't'
last_vitus:
		*/ test si la balle est collée */
		move.l	(pt_balle-t,a5),a3
		moveq	#NSBALL-1,d7
		moveq	#0,d6
.loop		tst.w	(SBALLE.ACTIF,a3)
		beq	.lvitus_end
		tst.w	(SBALLE.GLUE,a3)	balle collée ?
		bne	.edont_us		-> oui

		*/ test si la balle sort en bas */
		cmp.w	#YMAX,(SBALLE.Y,a3)	depassement en bas ?
		blt	.ean_balle		-> non
		move.w	d6,(SBALLE.ACTIF,a3)
		subq.w	#1,(balle_nbre-t,a5)	- 1 : une balle en moins
		bmi.s	.re_girl		-> plus de balle, perte 1 vie
		lea	(b1-t,a5),a1
		move.l	a1,(SBALLE.RAK,a3)	sauve adr. structure raquette bas
		move.w	d6,(SBALLE.GLUE,a3)	raz flag balle collée
		move.w	d6,(SBALLE.TILT,a3)
		move.w	#1,(SBALLE.POWER,a3)
		move.w	#5,(SBOB.WIDTH,a3)
		move.l	(SBOB.TABLE,a3),a1
		move.l	(a1)+,(SBOB.GFX,a3)
		move.l	(a1),(SBOB.MSK,a3)
		bra	.lvitus_end		-> sort de la routine

		*/ perte de la dernère balle : perte d'une vie */
.re_girl	move.w	d6,(balle_nbre-t,a5)
		lea	(b4-t,a5),a1
		move.w	#100,(BUMP.TOUCH,a1)	raquette clignote
		lea	(b1-t,a5),a1
		move.w	#100,(BUMP.TOUCH,a1)	raquette clignote
		move.l	a1,(SBALLE.RAK,a3)
		move.w	(BUMP.X,a1),(SBALLE.X,a3)
		move.w	(BUMP.Y,a1),(SBALLE.Y,a3)
		move.w	#1,(SBALLE.GLUE,a3)
		move.w	(balle_start-t,a5),(SBALLE.START,a3)
		bsr	lune
		move.w	#1,(SBALLE.ACTIF,a3)
		move.w	d6,(SBALLE.TILT,a3)
		move.w	#1,(SBALLE.POWER,a3)
		move.w	#5,(SBOB.WIDTH,a3)
		move.l	(SBOB.TABLE,a3),a1
		move.l	(a1)+,(SBOB.GFX,a3)
		move.l	(a1),(SBOB.MSK,a3)

		*/ test si balle peut rester collée */
.edont_us	subq.w	#1,(SBALLE.START,a3)	balle part tte seule ?
		bne.s	.edee_dee		-> non

		*/ balle part toute seule */
		move.w	d6,(SBALLE.GLUE,a3)
		move.w	(balle_glue-t,a5),(SBALLE.START,a3)

		*/ balle collée */
.edee_dee	subq.w	#1,(SBALLE.PBTEMP,a3)		tempo depl. pt balle - 1
		bne	.kol_ko			
		move.w	#5,(SBALLE.PBTEMP,a3)
		addq.w	#4,(2+SBALLE.PBROTA,a3)
		cmp.w	#56,(2+SBALLE.PBROTA,a3)
		blt.s	.raau1
		move.w	d6,(2+SBALLE.PBROTA,a3)
.raau1		lea	(pets1-t,a5),a0
		add.w	(2+SBALLE.PBROTA,a3),a0
		move.w	(2,a0),(2+SBALLE.DIRECT,a3)
		move.w	(SBALLE.X,a3),(SBALLE.PBX,a3)
		move.w	(SBALLE.Y,a3),(SBALLE.PBY,a3)
		move.w	(2,a0),d0		ba_pnt en d0
		add.w	d0,d0			*2 car va de 8 en 8
		lea	(cool_list-t,a5),a0
		move.w	(2,a0,d0),d1
		add.w	d1,(SBALLE.PBX,a3)
		move.w	(2+4,a0,d0),d1
		add.w	d1,(SBALLE.PBY,a3)

		*/ Centre la balle sur la raquette */
.kol_ko		lea	(b1-t,a5),a1
		move.w	(rak.large-t,a5),d0
		lsr.w	#1,d0
		subq.w	#3,d0
		moveq	#-1,d1
		sub.w	(SBOB.WIDTH,a3),d1
		add.w	(BUMP.X,a1),d0
		add.w	(BUMP.Y,a1),d1
		move.w	d0,(SBALLE.X,a3)
		move.w	d1,(SBALLE.Y,a3)
		bra	.ere_bump1

		*/ déplace la balle */
.ean_balle:	move.l	(SBALLE.SPEED,a3),a0	table vitesse balle
		add.l	(SBALLE.DIRECT,a3),a0	pt direction
		move.l	(a0),d0
		add.l	d0,(SBALLE.X,a3)

		*/  test collision balle / mur protector gauche */
.emur_h1:	cmp.w	#XMIN2,(SBALLE.X,a3)	xmin balle
		bge.s	.emur_h2
		move.w	#XMIN2,(SBALLE.X,a3)	corrige x
		move.b	#1,(sound+S_BRICOTE-t,a5)
		lea	(rb5-t,a5),a0
		bra	.ere_cas0		-> balle rebondit.

		*/ test collision balle / mur protector droit */
.emur_h2:	cmp.w	#XMAX2-1,(SBALLE.X,a3)	xmax balle
		ble.s	.emur_h3
		move.w	#XMAX2-1,(SBALLE.X,a3)	corrige x
		move.b	#1,(sound+S_BRICOTE-t,a5)
		lea	(rb1-t,a5),a0
		bra	.ere_cas0		-> balle rebondit.

		*/ Test collision balle / mur protector haut */
.emur_h3:	cmp.w	#YMIN,(SBALLE.Y,a3)	ymin balle
		bge.s	.ere_bump1
		move.w	#YMIN,(SBALLE.Y,a3)	corrige y
		move.b	#1,(sound+S_BRICOTE-t,a5)
		lea	(rb3-t,a5),a0
		bra	.ere_cas0

		*/ Collisions raquette port 0 */
.ere_bump1:	lea	(b1-t,a5),a1
		move.w	(BUMP.X,a1),d0		x raquette
		move.w	(BUMP.Y,a1),d1		y raquette
		move.w	(SBALLE.X,a3),d2		x balle
		move.w	(SBALLE.Y,a3),d3		y balle
		add.w	(SBOB.WIDTH,a3),d2
		add.w	(SBOB.WIDTH,a3),d3
		cmp.w	d0,d2
		blt	.ere_not
		cmp.w	d1,d3
		blt	.ere_not
		add.w	(rak.large-t,a5),d0
		move.w	(SBALLE.X,a3),d2
		cmp.w	d0,d2
		bgt	.ere_not
		add.w	(rak.haut-t,a5),d1
		move.w	(SBALLE.Y,a3),d3
		cmp.w	d1,d3
		bgt	.ere_not
		move.l	a1,(SBALLE.RAK,a3)
.ere_nocolle	move.b	#1,(sound+S_TOUCHE.RAK-t,a5)
		move.w	d6,(SBALLE.TILT,a3)
		* corrige la ordonnée balle en fct de la raquette *
		move.w	(BUMP.Y,a1),d0
		sub.w	(SBOB.WIDTH,a3),d0
		move.w	d0,(SBALLE.Y,a3)

		*/ gestion rebond sur raquette port 0 */
		move	(rak.vgg-t,a5),d0		vitesse dépl. à gauche
		move.l	(BUMP.RBLEFT,a1),a0	table rebond gauche
		cmp.w	#1,(rak.dep-t,a5)	déplacement raqu. à gauche ?
		beq.s	.boire_eau2		-> oui
		move.w	(rak.vdd-t,a5),d0	vitesse dépl. à droite
		move.l	(BUMP.RBRIGHT,a1),a0	table rebond droite
.boire_eau2	moveq	#10,d1
		cmp.w	d1,d0			vitesse de superieur à 10 ?
		blt.s	.boire_vin2		-> non
		move.w	d1,d0
.boire_vin2	add.w	d0,d0
		add.w	d0,d0
		move.l	(a0,d0),a0
		add.w	(2+SBALLE.DIRECT,a3),a0
		move.w	(2,a0),d0
		move.w	d0,(2+SBALLE.DIRECT,a3)

		*/  collisions raquette port 1 */
.ere_not	tst.w	(rak.team-t,a5)		mode "team" ?
		beq	.lvitus_end		-> non, sort de la routine
		lea	(b1-t,a5),a2
		lea	(b4-t,a5),a1
		move.w	(BUMP.X,a1),d0		x raquette
		move.w	(BUMP.Y,a1),d1		y raquette
		move.w	(SBALLE.X,a3),d2		x balle
		move.w	(SBALLE.Y,a3),d3		y balle
		add.w	(SBOB.WIDTH,a3),d2
		add.w	(SBOB.WIDTH,a3),d3
		cmp.w	d0,d2
		blt	.lvitus_end
		cmp.w	d1,d3
		blt	.lvitus_end
		add.w	(rak.large-t,a5),d0
		move.w	(SBALLE.X,a3),d2
		cmp.w	d0,d2
		bgt	.lvitus_end
		add.w	(rak.haut-t,a5),d1
		move.w	(SBALLE.Y,a3),d3
		cmp.w	d1,d3
		bgt	.lvitus_end
		move.l	a0,(SBALLE.RAK,a3)
		move.b	#1,(sound+S_TOUCHE.RAK-t,a5)
		move.w	d6,(SBALLE.TILT,a3)

		*- corrige la ordonnée balle en fct de la raquette -*
		move.w	(BUMP.Y,a2),d0
		sub.w	(SBOB.WIDTH,a3),d0
		move.w	d0,(SBALLE.Y,a3)

		*/ gestion rebond sur raquette port 1 */
		move.w	(rak.sll-t,a5),d0	vitesse dépl. à gauche
		move.l	(BUMP.RBLEFT,a2),a0	table rebond gauche
		cmp.w	#1,(rak.mov-t,a5)	déplacement raqu. à gauche ?
		beq.s	.boire_iau2		-> oui
		move.w	(rak.srr-t,a5),d0	vitesse dépl. à droite
		move.l	(BUMP.RBRIGHT,a2),a0	table rebond droite
.boire_iau2	moveq	#10,d1
		cmp.w	d1,d0			vitesse de superieur à 10 ?
		blt.s	.boire_iin2		-> non
		move.w	d1,d0
.boire_iin2	add.w	d0,d0
		add.w	d0,d0
		move.l	(a0,d0),a0
.ere_cas0:	add.w	(2+SBALLE.DIRECT,a3),a0
		move.w	(2,a0),d0
		move	d0,(2+SBALLE.DIRECT,a3)
.lvitus_end:	lea	(SBALLE.SIZE,a3),a3
		dbf	d7,.loop
		rts
