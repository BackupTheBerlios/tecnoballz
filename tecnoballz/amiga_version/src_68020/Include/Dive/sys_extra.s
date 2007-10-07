*//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/*
************* Routine appelée avant de quitter le multitâche **************
*//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/*
* Entrée	=> a5.l adr. 't' 
sys_extra:
		*«««« Charge page début information »»»»*
		lea	(.info_name,pc),a0
		lea	(adr_info-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit

		*«««« Charge page des Bobs »»»»* 
		lea	(pt_palette-t,a5),a4
		bsr.l	reserve.mem
		bne	.exit
		lea	(.bobs_name,pc),a0
		move.l	(pt_palette-t,a5),a2
		lea	(bob32_pagegfx-t,a5),a4
		bsr.l	charge.pagepp
		bne	.exit
		bsr.l	bob32_calcul_msk
		bne	.exit
		move.l	(bob32_pagemsk-t,a5),(sbob_pagemsk-t,a5)
		*«««« Charge page fonte 16 couleurs crédit & inverseur ««««*
		lea	(.fontex_name,pc),a0
		sub.l	a2,a2
		lea	(fontex.pagegfx-t,a5),a4
		bsr.l	charge.page
		bne	.exit
		*«««« Charge la page fonte 2 couleurs ««««*
		lea	(.fonte2_name,pc),a0
		sub.l	a2,a2
		lea	(fonte2.pagegfx-t,a5),a4
		bsr.l	charge.page
		bne	.exit
		*«««« Charge GigaBlitz ««««*
		lea	(.blitz_name,pc),a0
		lea	(blitz.pp-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		*«««« Charge la table sinus ««««*
		lea	(.sinname,pc),a0
		lea	(pt_cos-t,a5),a4
		bsr.l	charge.pp
		bne	.exit
*/----------------------------*/ TABLEAUX /*-----------------------------/*
		IFEQ	A_TAB
		*«««« Charge page de 64*256*5 bandes des scores »»»»* 
		lea	(.score_name,pc),a0
		lea	(score.pp-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		*«««« Charge page des 60 maps »»»»* 
		lea	(.map60_name,pc),a0
		lea	(map60.pp-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		*«««« Charge 60 valeurs couleurs possibles »»»*
		lea	(.map60col_name,pc),a0
		lea	(map60.color-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		*«««« Charge les 100 tableaux ««««*
		lea	(.tab_name,pc),a0
		lea	(tablo.pp-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		*«««« Charge les 10 jeux de briques ««««*
		lea	(.bk_name,pc),a0
		lea	(brick.pp-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		*«««« Charge la première raquette ««««*
		lea	(.rak1_name,pc),a0
		lea	(rak01.pp-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		*«««« Charge la deuxième raquette ««««*
		lea	(.rak2_name,pc),a0
		lea	(rak02.pp-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		moveq	#0,d7
		*«««« Charge la page fonte 4 couleurs 352*8 ««««*
		lea	(.fonte4_name,pc),a0
		sub.l	a2,a2
		lea	(fonte4.pagegfx-t,a5),a4
		bsr.l	charge.page
		bne	.exit
		moveq	#0,d7
		*«««« Charge têtes ««««*
		lea	(.tete_name,pc),a0
		lea	(heads.pp-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		*«««« Charge la page minifonte messages ««««*
		lea	(.fontem_name,pc),a0
		sub.l	a2,a2
		lea	(mini.pagegfx-t,a5),a4
		bsr.l	charge.page
		bne	.exit
		ENDC
		*«««« Charge les 2 magasins compacté »»»»* 
		IFEQ	A_SHOP
		lea	(.shop1_name,pc),a0
		lea	(shop1.pp-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		ENDC
*-----------------------------*/ GARDIENS /*------------------------------*
		IFEQ	A_GARD
		*«««« Charge page Dalles fond Gardien 8 couleurs »»»»* 
		lea	(.dalma_name,pc),a0
		lea	(dalma.pp-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		*«««« Charge Carte map-editor Gardien  »»»»* 
		lea	(.maped_name,pc),a0
		lea	(maped.pp-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		lea	(.maped2_name,pc),a0
		lea	(maped2.pp-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		*«««« Charge Courbes Lissa déplacement des Gardiens  »»»»* 
		lea	(.lissa_name,pc),a0
		lea	(lissa-t,a5),a4
		bsr.l	charge.pp
		bne	.exit
		*««« Charge Palette pour le GameOver des Gardiens »»»*
		lea	(pt_overpal-t,a5),a4
		bsr.l	reserve.mem
		bne	.exit
		lea	(.over_name,pc),a0
		move.l	(pt_overpal-t,a5),a2
		sub.l	a4,a4
		bsr.l	charge.page
		bne	.exit
		*««« Charge Palette pour le TecnoWinner »»»*
		lea	(pt_winnpal-t,a5),a4
		bsr.l	reserve.mem
		bne	.exit
		lea	(.winn_name,pc),a0
		move.l	(pt_winnpal-t,a5),a2
		sub.l	a4,a4
		bsr.l	charge.page
		bne	.exit
		ENDC
*--------------------*«««« Charge les 12 modules »»»»*--------------------*
		IFEQ	A_SOUND
		lea	(.sound_name,pc),a0
		lea	(soundtable-t,a5),a4
		bsr.l	charge.pp
		bne	.exit
		lea	(.area1_name,pc),a0
		lea	(mod.area1-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		lea	(.area2_name,pc),a0
		lea	(mod.area2-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		lea	(.area3_name,pc),a0
		lea	(mod.area3-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		lea	(.area4_name,pc),a0
		lea	(mod.area4-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		lea	(.area5_name,pc),a0
		lea	(mod.area5-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		lea	(.endth_name,pc),a0
		lea	(mod.endth-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		lea	(.gardi_name,pc),a0
		lea	(mod.gardi-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		lea	(.hisco_name,pc),a0
		lea	(mod.hisco-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		lea	(.tecno_name,pc),a0
		lea	(mod.tecno-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		lea	(.winne_name,pc),a0
		lea	(mod.winne-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		ENDC
*---------------------------*/INTRODUCTION/MENU/*-------------------------*
		IFEQ	A_INTRO
		*«««« Charge ScrollText ««««*
		lea	(.texname,pc),a0
		lea	(scroll.text-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		*»»»» Charge Map-Editor ««««*
		lea	(.maped3_name,pc),a0
		lea	(maped3.pp-t,a5),a4
		bsr.l	charge.fichier
		bne	.exit
		*«««« Charge page de 640x8x3 => 80 fontes de 8x8 »»»»* 
		lea	(.fontes80_name,pc),a0
		sub.l	a2,a2
		lea	(fonte80.pagegfx-t,a5),a4
		bsr.l	charge.page
		bne	.exit
		*««« Charge Palette pour de l'intro »»»*
		lea	(pt_menupal-t,a5),a4
		bsr.l	reserve.mem
		bne	.exit
		lea	(.menuname,pc),a0
		move.l	(pt_menupal-t,a5),a2
		sub.l	a4,a4
		bsr.l	charge.page
		bne	.exit
		*««« Charge Palette pour de le logo »»»*
		lea	(pt_logopal-t,a5),a4
		bsr.l	reserve.mem
		bne	.exit
		lea	(.logoname,pc),a0
		move.l	(pt_logopal-t,a5),a2
		sub.l	a4,a4
		bsr.l	charge.page
		bne	.exit
		ENDC
*------------------------***/CHARGE LES SCORES/***------------------------*
		bsr.l	charge_score
		moveq	#0,d7
.exit		rts
*»*»*»*»*»*»*»*»*»*»*»*»*»* NOMS DES FICHIERS *«*«*«*«*«*«*«*«*«*«*«*«*«*«*
* TIROIR "GFX/" *---------------------------------------------------------*
.fonte2_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/font_score.ilbm",0
.fontem_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/font_messa.ilbm",0
.fontex_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/font_money.ilbm",0
.fonte4_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/font_zgame.ilbm",0
.fontes80_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/font_zmenu.ilbm",0
.bobs_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/page_zbobs.ilbm",0
.map60col_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/pal_60maps.list",0
.over_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/pal_gmover.ilbm",0
.winn_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/pal_winner.ilbm",0
.logoname	dc.b	"TecnoballZ:TecnoballZ/Gfx/pal_zelogo.ilbm",0
.menuname	dc.b	"TecnoballZ:TecnoballZ/Gfx/pal_zemenu.ilbm",0
.map60_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/pp_60-maps.ilbm",0
.tete_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/pp_anihead.ilbm",0
.score_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/pp_bscores.ilbm",0
.rak1_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/pp_bumper1.ilbm",0
.rak2_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/pp_bumper2.ilbm",0
.blitz_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/pp_ggblitz.ilbm",0
.dalma_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/pp_mapedit.ilbm",0
.shop1_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/pp_newshop.ilbm",0
.info_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/pp_sharwar.ilbm",0
.bk_name	dc.b	"TecnoballZ:TecnoballZ/Gfx/pp_zebrick.ilbm",0
* TIROIR "Raw/" *---------------------------------------------------------*
.sinname	dc.b	"TecnoballZ:TecnoballZ/Raw/cosinus128.list",0
.lissa_name	dc.b	"TecnoballZ:TecnoballZ/Raw/gard_lissa.list",0
.maped_name	dc.b	"TecnoballZ:TecnoballZ/Raw/pp_edmap01.data",0
.maped2_name	dc.b	"TecnoballZ:TecnoballZ/Raw/pp_edmap02.data",0
.maped3_name	dc.b	"TecnoballZ:TecnoballZ/Raw/pp_edmap03.data",0
.tab_name	dc.b	"TecnoballZ:TecnoballZ/Raw/pp_tableau.data",0
.texname	dc.b	"TecnoballZ:TecnoballZ/Raw/scrolltext.asci",0
.sound_name	dc.b	"TecnoballZ:TecnoballZ/Raw/sound_game.data",0
* TIROIR "Mod/" *---------------------------------------------------------*
.area1_name	dc.b	"TecnoballZ:TecnoballZ/Mod/mod.area1-game",0
.area2_name	dc.b	"TecnoballZ:TecnoballZ/Mod/mod.area2-game",0
.area3_name	dc.b	"TecnoballZ:TecnoballZ/Mod/mod.area3-game",0
.area4_name	dc.b	"TecnoballZ:TecnoballZ/Mod/mod.area4-game",0
.area5_name	dc.b	"TecnoballZ:TecnoballZ/Mod/mod.area5-game",0
.gardi_name	dc.b	"TecnoballZ:TecnoballZ/Mod/mod.gardien-go",0
.hisco_name	dc.b	"TecnoballZ:TecnoballZ/Mod/mod.high-score",0
.endth_name	dc.b	"TecnoballZ:TecnoballZ/Mod/mod.over-theme",0
.winne_name	dc.b	"TecnoballZ:TecnoballZ/Mod/mod.tecno-winn",0
.tecno_name	dc.b	"TecnoballZ:TecnoballZ/Mod/mod.tecnoballz",0
		EVEN
