*=========================================================================*
*===============> QUITTE & REVIENT PROPREMENT AU SYSTEME <================*
*=========================================================================*

*----------------* SYSTEM : quitte proprement le système *----------------*
sys_start:
		*/ Le programme peut se lancer depuis une icône */
		movem.l	d0/a0,-(a7)
		sub.l	a1,a1
		move.l  4.w,a6
		jsr	(FindTask,a6)
		move.l	d0,a4
		tst.l	(pr_CLI,a4)
		bne.s   .from_cli
		lea	(pr_MsgPort,a4),a0
		move.l  4.w,a6
		jsr	(WaitPort,A6)
		lea	(pr_MsgPort,a4),a0
		jsr	(GetMsg,A6)
		lea	(returnMsg,pc),a0
		move.l	d0,(a0)
.from_cli	movem.l	(a7)+,d0/a0

		*/ Début de mon programme */
		movem.l	d0-d7/a0-a6,-(a7)
		lea	(sys_pile1,pc),a0
		move.l	a7,(a0)			sauve pt pile utilisateur
		bsr	sys_first		|-> cherche base vecteurs
		bsr	sys_memlist		|-> reserve liste mémoire
		bne	sys.no_memory		-> allocation échouée
		bsr	open_library		|-> ouvre les librairies
		bne	sys_erreur		-> ouverture échouée
		move.l	(gfxbase,pc),a6
		btst	#GFXB_AA_ALICE,(gb_ChipRevBits0,a6)
		bne.s	.is_aa
		moveq	#-29,d7			¤ pas de AGA chip-set
		bra	sys_erreur
.is_aa		bsr.l	sys_extra		|-> routine possible
		tst.l	d7
		bne	sys_erreur
		bsr	sys_save		|-> sauve valeur système
		bra	run			-> SAUTE DEBUT PROGRAMME

*---------------* SYSTEM : revient proprement au système *----------------*
sys_fin:	
		move.l	(sys_pile1,pc),a7	repointe pile utilisateur
		move.l	(at,pc),a5
		move.w	#$FFFF,(dmacon.msk-t,a5)
		bsr	sys_restore		|-> remet le système
		move.l	4.w,a6
sys.no_memory	bsr	sys_gere_erreur
		move.l	(intuitionbase,pc),a6
		jsr	(OpenWorkBench,a6)
		bsr	sys_extra2
		lea	CUSTOM,a6
li:		bsr	sys_freemem		<- BREAKPOINT MONAM
		bsr	sys_freememlist
		bsr	close_library
sys_retour	movem.l	(a7)+,d0-d7/a0-a6

		*/ Retour lorsque le programme a été appellée d'une icône */
		move.l	d0,-(a7)
		lea	(returnMsg,pc),a0
		tst.l	(a0)
		beq.s	.exit_dos
		move.l	4.w,a6
        	jsr	(Forbid,a6)
		move.l	(returnMsg,pc),a1
		jsr	(ReplyMsg,a6)
.exit_dos	move.l	(a7)+,d0
		rts

*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

*------------------* SYSTEM : Lit la base des vecteurs *------------------*
sys_first:
		lea	(erreur,pc),a5
		clr.l	(a5)			n° erreur = 0
		add.l	#t-erreur,a5
		lea	(at,pc),a0
		move.l	a5,(a0)			sauve adr. table 't'
		move.l	4.w,a6
		jsr	(SuperState,a6)		sortie : d0=SysStack
		move.l	(at,pc),a5
		movec	vbr,d1			d1=base des vecteurs
		move.l	d1,(base_vecteurs-t,a5)
		jsr	(UserState,a6)		entrée : d0=SysStack
		rts

*-----------------* SYSTEME : Reserve la liste mémoire *------------------*
sys_memlist:
		move.l	4.w,a6
		move.l	#NReservation*16+4,d0	taille table mémoire
		move.l	d0,(mem_liste.size-t,a5)
		moveq	#0,d1
		jsr	(AllocMem,a6)
		move.l	d0,(mem_liste.adr-t,a5)
		beq.s	.outmemory		-> plus de mémoire
		move.l	d0,a0
		move.w	#NReservation*2,d0
		moveq	#0,d1
.loop		move.l	d1,(a0)+
		dbf	d0,.loop
		moveq	#0,d7
		rts
.outmemory	moveq	#-12,d7			¤dépassement mémoire
		rts

*--------------* SYSTEM : Affichage de l'erreur si besoin *---------------*
sys_gere_erreur:
		lea	(erreur,pc),a0
		tst.l	(a0)			erreur ?
		beq.s	.exit			-> non
		moveq	#0,d0			AlertNumber
		lea	(alert_string,pc),a0
		moveq	#90,d1
		move.l	(intuitionbase,pc),a6
		jsr	DisplayAlert(a6)
.exit		rts

*--------------------* SYSTEM : gestion des erreurs *---------------------*
sys_erreur:
		bsr	sys_failure
		bra	sys_fin
sys_failure:	tst.l	d7			erreur ?
		beq.s	.exit			-> non
		cmp.l	#-35,d7			erreur bidon ?
		beq.s	.exit			-> oui
		lea	(erreur,pc),a1
		move.l	d7,(a1)			sauve n° de l'erreur
		neg.l	d7
		lea	(messages,pc),a1	pointe sur messages mentat
		cmp.l	#128,d7			erreur spécifique au programme ?
		blt.s	.nopersonnal		-> non
		lea	(msg.pers,pc),a1	pointe sur messages spécifiques
		sub.l	#127,d7
.nopersonnal	subq.l	#1,d7
		mulu	#MESSAGE_SIZE,d7
		add.l	d7,a1
		lea	(alert_message,pc),a2
		moveq	#MESSAGE_SIZE-1,d0
.loop		move.b	(a1)+,(a2)+
		dbf	d0,.loop
.exit		rts

*--------------------* SYSTEM : ouvre les librairies *--------------------*
open_library:
		move.l	4.w,a6
		lea	(gfxname,pc),a1		graphic.library
		moveq	#0,d0
		jsr	(OpenLibrary,a6)
		lea	(gfxbase,pc),a0
		move.l	d0,(a0)
		beq	.error
		IFNE	FLAG_AMIGADOS
		lea	(dosname,pc),a1		dos.library
		moveq	#0,d0
		jsr	(OpenLibrary,a6)
		lea	(dosbase,pc),a0
		move.l	d0,(a0)
		beq	.error
		ENDC
		lea	(intuitionname,pc),a1	intuition.library
		moveq	#0,d0
		jsr	(OpenLibrary,a6)
		lea	(intuitionbase,pc),a0
		move.l	d0,(a0)
		beq	.error
		moveq	#0,d7
.exit		rts
.error:		moveq	#-13,d7			¤ouverture librairie échoue
		bra.s	.exit

*--------------------* SYSTEM : ferme les librairies *--------------------*
close_library:
		move.l	4.w,a6
		lea	(intuitionbase,pc),a0
		bsr.s	.close
		lea	(dosbase,pc),a0
		bsr.s	.close
		lea	(gfxbase,pc),a0
.close		tst.l	(a0)
		beq.s	.exit
		move.l	(a0),a1
		clr.l	(a0)
		jsr	(CloseLibrary,a6)
.exit		rts

*----------------------* SYSTEM : Guru Meditation *-----------------------*
sys_guru:	
		move.w	#$7FFF,CUSTOM+INTENA
		move.w	#$7FFF,CUSTOM+INTREQ
		lea	(e_info,pc),a0
		move.w	(a7)+,(a0)
		move.l	(a7)+,(e_adr-e_info,a0)
		move.w	(a7)+,(e_inst-e_info,a0)
		move.w	(a7)+,(e_sr-e_info,a0)
		move.l	(a7)+,(e_pc-e_info,a0)
		move.w	#0,sr
		lea	(alert.info,pc),a0
		move.l	#'GURU',(a0)
		moveq	#-14,d7			¤Guru Meditation
		bra	sys_erreur

*-----------* SYSTEM : quitte pendant l'interruption clavier *------------*
superquit:	
		bclr	#6,CIAA_CRA
		move.w	#0,sr
		bra	sys_fin

*-------------------* SYSTEM : sauve valeurs systèmes *-------------------*
sys_gele2:
		tst.w	(flag_sys_gele2-t,a5)	système gelé lors de l'appel ?
		beq.s	.non_gele		-> non
		bsr.s	sys_save
		bsr	first_init
.non_gele	rts

		* test si système à dèjà été sauvé *
sys_save:	move.l	(at,pc),a5
		tst.w	(flag_sys_gele-t,a5)
		bne	.exit

		* gèle le système *
		move.l	4.w,a6
		jsr	(Forbid,a6)		|-> interdit le multitâche
		move.l	(gfxbase,pc),a6
		jsr	(WaitBlit,a6)
		jsr	(OwnBlitter,a6)
		lea	CUSTOM,a6
		lea	(save_intena,pc),a0
		tst.w	(a0)
		bne.s	.deja_sauve
		move.w	(INTENAR,a6),(a0)
		or.w	#$C000,(a0)+
		move.w	(DMACONR,a6),(a0)
		or.w	#$8000,(a0)+
		move.w	(ADKCONR,a6),(a0)
		or.w	#$8000,(a0)
.deja_sauve	move.w	#$7FFF,(INTENA,a6)
		move.w	#$7FFF,(INTREQ,a6)
		move.w	#$07FF,(DMACON,a6)
		move.w	#$7F00,(ADKCON,a6)
		move.w	#$4000,(DSKLEN,a6)

		* sauve les 64 premiers vecteurs 680x0 *
		move.l	(at,pc),a5
		move.l	(base_vecteurs-t,a5),a0
		lea	(sys_liste,pc),a1
		moveq	#64-1,d0
.loop1		move.l	(a0)+,(a1)+
		dbf	d0,.loop1

		* détourne les vecteurs guru *
		move.l	(at,pc),a5
		move.l	(base_vecteurs-t,a5),d1
		lea	(sys_guru,pc),a0
		lea	8.w,a1
		add.l	d1,a1
		moveq	#9,d0
.loop2		move.l	a0,(a1)+
		dbf	d0,.loop2

		* met flag système gelé *
		move.l	(at,pc),a5
		move.w	#1,(flag_sys_gele-t,a5)
.exit		rts

*---------------------* SYSTEM : restore le système *---------------------*
sys_restore2:
		move.w	(flag_sys_gele-t,a5),(flag_sys_gele2-t,a5)
		beq.s	.sys_non_gele		-> système non gelé
		bsr.s	sys_restore		|-> restore le système
.sys_non_gele	rts

sys_restore:
		* Test si système à dèjà été restoré *
		move.l	(at,pc),a5
		tst.w	(flag_sys_gele-t,a5)
		beq	.exit

		* Gèle le système *
		lea	CUSTOM,a6
		lea	(INTENA,a6),a0
		lea	(INTREQ,a6),a1
		move.w	#$7FFF,d0
		move.w	d0,(a0)
		move.w	d0,(a1)
		move.w	#$7F00,(ADKCON,a6)
		move.w	#$07FF,(DMACON,a6)

		* Restore les 64 premiers vecteurs 680x0 *
		move.l	(at,pc),a5
		move.l	(base_vecteurs-t,a5),a0
		lea	(sys_liste,pc),a1
		moveq	#64-1,d0
.loop		move.l	(a1)+,(a0)+
		dbf	d0,.loop

		* Réactive système *
		lea	CUSTOM,a6
		move.l	4.w,a0			a0=ExecBase
		moveq	#$0020,d0
		cmp.b	#50,530(a0)		50hz = pal ?
		beq.s	.pal			-> oui
		moveq	#0,d0
.pal		move.w	d0,(BEAMCON0,a6)

		move.l	(gfxbase,pc),a1
		move.l	(38,a1),(COP1LCH,a6)
		move.w	d0,(COPJMP1,a6)
		move.w	(save_adkcon,pc),(ADKCON,a6)
		move.w	(save_dmacon,pc),d0
		IFNE	FLAG_DMACON.MSK
		move.l	(at,pc),a5
		and.w	(dmacon.msk-t,a5),d0
		ENDC
		move.w	d0,(DMACON,a6)
		move.w	(save_intena,pc),(INTENA,a6)
		move.l	4.w,a6
		jsr	(Permit,a6)
		move.l	(gfxbase,pc),a6
		jsr	(DisownBlitter,a6)

		* raz flag système gélé *
		move.l	(at,pc),a5
		clr.w	(flag_sys_gele-t,a5)
.exit		rts

*---------------------* SYSTEM : libère la mémoire *----------------------*
sys_freemem:
		move.l	4.w,a6
		move.l	(at,pc),a5
		move.l	(mem_liste.adr-t,a5),a3
		move.l	(a3),d1			d1=nbre section mémoire
		beq	.exit			-> pas de section mémoire
		clr.l	(a3)
		lea	(4,a3),a0
		subq.w	#1,d1
.loop		move.l	(a0)+,a1		a1=adresse mémoire
		move.l	(a0)+,d0		d0=taille mémoire
		addq.l	#8,a0
		cmp.l	#'MENT',(a1)
		bne.s	.error38
		cmp.l	#'AT*M',(4,a1)
		bne.s	.error38
		cmp.l	#'EM*S',(8,a1)
		bne.s	.error38
		cmp.l	#'TART',(12,a1)
		bne.s	.error38
		cmp.l	#'STOP',(-16,a1,d0.l)
		bne.s	.error37
		cmp.l	#'*MEN',(-12,a1,d0.l)
		bne.s	.error37
		cmp.l	#'TAT*',(-08,a1,d0.l)
		bne.s	.error37
		cmp.l	#'MEM*',(-04,a1,d0.l)
		bne.s	.error37
.raptor		movem.l	d1/a0,-(a7)
		jsr	(FreeMem,a6)
		movem.l	(a7)+,d1/a0
		dbf	d1,.loop
.exit		rts

.error37	movem.l	d0-d7/a0-a6,-(a7)
		moveq	#-37,d7			débordement mémoire aprés
		bra.s	.viva
.error38	movem.l	d0-d7/a0-a6,-(a7)
		moveq	#-38,d7			débordement mémoire avant
.viva		lea	(sys.adr,pc),a0
		move.l	a1,(a0)
		lea	(sys.siz,pc),a0
		move.l	d0,(a0)
		lea	(sys.num,pc),a0
		addq.l	#1,(a0)

		moveq	#8-1,d1
		lea	(alert.info+7,pc),a0
		move.l	(sys.adr,pc),d3
		add.l	#4*4,d3
		bsr	conv_number_ascii
		move.l	(sys.siz,pc),d2
		sub.l	#4*8,d2
		bsr	convertit_decimal
		moveq	#8-1,d1
		lea	(alert.info+25,pc),a0
		bsr	conv_number_ascii

		bsr	sys_failure
		bsr	sys_gere_erreur
		movem.l	(a7)+,d0-d7/a0-a6
		bra.s	.raptor

*------------------* SYSTEME : libère la liste mémoire *------------------*
sys_freememlist:
		move.l	(at,pc),a5
		tst.l	(mem_liste.adr-t,a5)	liste mémoire ?
		beq.s	.exit			-> non
		move.l	(mem_liste.adr-t,a5),a1	adresse mémoire à libérer
		move.l	(mem_liste.size-t,a5),d0 taille mémoire à libérer
		clr.l	(mem_liste.adr-t,a5)
		clr.l	(mem_liste.size-t,a5)
		move.l	4.w,a6
		jsr	(FreeMem,a6)		|-> libère mémoire
.exit		rts

*----* SYSTEME : Réexecute le programme après un breakpoint en 'li' *-----*
st:		bsr	sys_save
		bsr	first_init
		bsr	colors_on
		bra.l	main_loop

*---------------------* SYSTEM : adresses utilisées *----------------------*
		CNOP	0,4
sys_liste	ds.l	64	64 premiers vecteurs
sys_pile1	dc.l	0	pt pile utilisateur
returnMsg	dc.l	0
save_intena	dc.w	0
save_dmacon	dc.w	0
save_adkcon	dc.w	0

		* information sur un guru *
e_info		dc.w	0
e_adr		dc.l	0
e_inst		dc.w	0
e_sr		dc.w	0
e_pc		dc.l	0	adr. où s'est produit l'erreur

		CNOP	0,4
sys.num		dc.l	0
sys.siz		dc.l	0
sys.adr		dc.l	0
at:		dc.l	0	adr. de la table 't'
gfxbase		dc.l	0
dosbase		dc.l	0
intuitionbase	dc.l	0
gfxname		dc.b	'graphics.library',0
dosname		dc.b	'dos.library',0
intuitionname	dc.b	'intuition.library',0
myname		dc.b	'===> '
		dc.b	'Bruno Ethvignot / 17A rue Jules Ferry / '
		dc.b	'25000 Besançon / France / TEL : 81.52.42.85 <===',0 
		EVEN
erreur		dc.l	0			n °erreur
messages	dc.b	"Initialise écran : dépassement mémoire     "	¤ -1
messages2	dc.b	"Décompactage ILBM : Fichier non IFF        "	¤ -2
		dc.b	"Décompactage ILBM : Fichier non ILBM       "	¤ -3
		dc.b	"Décompactage ILBM : Chunk non trouvé       "	¤ -4
		dc.b	"Décompactage ILBM : Compression inconnue   "	¤ -5
		dc.b	"Initialise écran : structure initialisée   "   ¤ -6
		dc.b	"Chargement AmigaDos : Ouverture impossible "	¤ -7
		dc.b	"Chargement AmigaDos : Erreur de lecture    "	¤ -8
		dc.b	"Chargement AmigaDos : Dépassement mémoire  "	¤ -9
		dc.b	"Gestion mémoire : la libération a échoué   "   ¤-10
		dc.b	"Lit piste AmigaDos : disque non inséré     "	¤-11
		dc.b	"Gestion mémoire : dépassement mémoire      "   ¤-12
		dc.b	"Système : Ouverture d'une librairie échoue "	¤-13
		dc.b	"Système : Guru Meditation évité de justesse"	¤-14
		dc.b	"Initialise écran : écran déjà détruit      "   ¤-15
		dc.b	"                                           "	¤-16
		dc.b	"                                           "	¤-17
		dc.b	"                                           "	¤-18
		dc.b	"                                           "	¤-19
		dc.b	"                                           "	¤-20
		dc.b	"                                           "	¤-21
		dc.b	"                                           "	¤-22
		dc.b	"                                           "	¤-23
		dc.b	"   Attention ! Erreur non répertoriée !    "	¤-24
		dc.b	"ECRAN : Pas de structure écran initialisé  "	¤-25
		dc.b	"Sauvegarde AmigaDos : Erreur d'écriture    "	¤-26
		dc.b	"Sauvegarde AmigaDos : Ouverture impossible "	¤-27
		dc.b	"PowerPacker : Fichier non PP20             "   ¤-28 
		dc.b	"  Ce programme nécéssites le chip-set AGA  "	¤-29
		dc.b	"BOB initialisation : pas de 'bob32_pagegfx'"	¤-30 
		dc.b	"BOB initialisation : pas de 'bob32_liste'  "	¤-31
		dc.b	"BOB initialisation : pas de 'pt_screen'    "	¤-32
		dc.b	"BOB initialisation : numéro BOB inexistant "	¤-33
		dc.b	"BOB initialisation : Plus de structure BOBs"	¤-34
		dc.b	"message d'erreur non affiché=erreur bidon  "	¤-35
		dc.b	">>>> Erreur plus de liste disponible <<<<< "	¤-36
		dc.b	"Gestion mémoire : débordement mémoire après"   ¤-37
		dc.b	"Gestion mémoire : débordement mémoire avant"   ¤-38
		dc.b	"Gestion mémoire : maximum de zones atteints"   ¤-39

		EVEN
MESSAGE_SIZE	=messages2-messages
alert_string	dc.w	248
		dc.b	20	
		dc.b	"MENTAT MEDITATION"
		dc.b	0,1
		dc.w	152
		dc.b	40
alert_message	dcb.b	MESSAGE_SIZE," "
		dc.b	0,1
		dc.w	104
		dc.b	60
		dc.b	"Pressez sur un des boutons de la souris pour continuer "
		dc.b	0,1
		dc.w	104
		dc.b	80
alert.info:	dc.b	"ADR : $........ / SIZE : ........ "
		dc.b	0,0
		EVEN
