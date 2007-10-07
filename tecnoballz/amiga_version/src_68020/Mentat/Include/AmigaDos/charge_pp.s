*=========================================================================*
************** Charge un fichier powerpacké et le décompacte **************
*=========================================================================*
* Entrée	=> d0.l	#2 Chip / #4 Fast / #0 Public
*		=> a0.l	Adr. nom du fichier
*		=> a5.l	Adr. table 't'
* Sortie	<= d0.l	Adr. fichier décompacté
*		<= d1.l	Taille fichier décompacté
*		<= a0.l	Adr. nom du fichier
*		<= d7.l =0 / Z=1 pas d'erreur
charge_pp:
		lea	(.mem_requester,pc),a1
		move.l	d0,(a1)
		bsr	.load_file		|-> Charge fichier powerpacké
		bne.s	.exit			-> erreur
		move.l	(.adr_fichierpp,pc),d0
		move.l	(.size_pp,pc),d1
		move.l	(.mem_requester,pc),d2
		bsr	decrunch_pp
		bne.s	.exit			-> erreur
		movem.l	d0/d1,-(a7)
		bsr	.libere_memory
		movem.l	(a7)+,d0/d1
		tst.l	d7
		bne.s	.exit			-> erreur
		move.l	(.nom_fichierpp,pc),a0	Adr. Nom du fichier
		moveq	#0,d7
.exit		rts

*-------------------> Charge le fichier powerpacké  <---------------------*
* entrée	=> a0.l	Adr. nom fichier
.load_file	
		lea	(.nom_fichierpp,pc),a1	nom du fichier
		move.l	a0,(a1)
		moveq	#0,d0			mémoire CHIP ou FAST
		bsr	charge_fichier_dos
		lea	(.adr_fichierpp,pc),a0
		move.l	d0,(a0)			sauve adresse du fichier pp
		lea	(.size_pp,pc),a0
		move.l	d1,(a0)			sauve taille fichier pp
		tst.l	d7			erreur ?
		rts

*----------------------> Libère mémoire chargement <----------------------*
.libere_memory
		move.l	(.adr_fichierpp,pc),d0
		bsr	libere_memoire
		tst.l	d7
		rts

*-------------------------> adresses utilisées <--------------------------*
		CNOP	0,4
.adr_fichierpp	dc.l	0		Adr. fichier powerpacké
.size_pp	dc.l	0		Taille fichier powerpacké
.nom_fichierpp	dc.l	0		Adr. nom ASCII powerpacké
.mem_requester	dc.l	0		Type mémoire Chip/Fast/Public

*=========================================================================*
************** Charge un fichier powerpacké et le décompacte **************
*=========================================================================*
* Entrée	=> a0.l	Adr. nom du fichier
*		=> a4.l Structure RESA
*		=> a5.l	Adr. table 't'
* Sortie	<= d7.l =0 / Z=1 pas d'erreur
charge.pp:
		move.l	a0,(load.name-t,a5)
		move.l	a4,(pp.resa-t,a5)
		lea	(pp.temp-t,a5),a4
		bsr	charge.fichier
		bne.s	.exit			-> erreur
		lea	(pp.temp-t,a5),a4
		move.l	(RESA.ADR,a4),d0	d0=adr. fichier PP
		move.l	(RESA.SIZ,a4),d1	d1=taillee fichier PP
		move.l	(pp.resa-t,a5),a4
		bsr	decrunch.pp
		bne.s	.exit			-> erreur
		lea	(pp.temp-t,a5),a0
		bsr	libere_mem
		bne.s	.exit
		moveq	#0,d7
.exit		rts
