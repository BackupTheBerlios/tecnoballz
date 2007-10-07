*/////////////////////////////////////////////////////////////////////////*
*»»»»»»»»»»»»»»»»»»»»»»»»»» ROUTINES DU MENTAT ««««««««««««««««««««««««««««
*/////////////////////////////////////////////////////////////////////////*
*FLAG_AMIGADOS	=0	Si=1 Alors assemble routines chargement AmigaDOS
*FLAG_BOB32	=0	Si=1 Alors assemble routines bobs (aff. écran 32 bits)
*FLAG_BOBSTRUCT	=0	Si=1 Assemble la structure BOB
*FLAG_DMACON.MSK=0	Si=1 Ferme dma bitplane lors d'un accès disque-système
*FLAG_EXIT	=0	Si=1 Sortie sous système possible avec la touche "ESC"
*FLAG_FREQ	=0	Si=1 Alors assemble routine demande fréquence balayage
*FLAG_ILBM	=0	Si=1 Alors assemble routine décompactage IFF-ILBM
*FLAG_KEY	=0	Si=1 Alors assemble routines de test du clavier
*FLAG_LUTIN	=0	Si=1 Alors assemble routines lutins
*FLAG_MEMORY	=0	Si=1 Alors information mémoire possible
*FLAG_MOUSE0	=0	Si=1 Alors test déplacement souris port 0
*FLAG_PP	=0	Si=1 Alors assemble routine décompactage PowerPacker
*
*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»		   «»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*=============================> CONSTANTES <==============================*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»		   «»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
		INCLUDE	"Mentat/Include/Constantes/customs_chips.s"
		INCLUDE	"Mentat/Include/Constantes/offset.s"
		INCLUDE	"Mentat/Include/Constantes/constantes_mentat.s"
		INCLUDE	"Mentat/Include/Clavier/code_raw_clavier.s"
*
*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*==========================> ROUTINES SYSTEMES <==========================*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*--------------------------------> 'debut' <------------------------------*
		INCLUDE	"Mentat/Include/Systeme/debut.s"
*
*----------------------------> 'first_init' <-----------------------------*
* Initialise le pointeur de pile superviseur
* Change vecteur interruption niveau 3 (VBL)
* Change vecteur interruption niveau 2 (Clavier)
* Init. CIA pour la Playroutine
* Reloge l'adresse des fontes systemes & de la table clavier
* Init INTENA, la liste Copper , et DMACON
		INCLUDE	"Mentat/Include/Systeme/first_init.s"
*
*--------------------------> 'reserve_memoire' <--------------------------*
* Reserve de la mémoire
* Entrée	=> d0.l	Taille en octet
*		=> d1.l	Caracteristique #00 Fast ou Chip Memory
*					#02 Chip Memory
*					#04 Fast Memory
*					#-1 Absolue
*		=> a1.l	Adresse absolue (si d1 = -1)
*		=> a5.l	Adr. 't'
* Sortie	<= d0.l	Adresse mémoire 32bits ou #0 si erreur
		INCLUDE	"Mentat/Include/Systeme/reserve_memoire.s"
*
*---------------------------> 'reserve_liste' <---------------------------*
* Reserve une liste mémoire en FAST ou CHIP
* Entrée	=> d0.l	Taille en octet
*		=> a0.l pt/adresse de la liste
* Sortie	<= d7.l=0 / Z=1 : pas d'erreur
		INCLUDE	"Mentat/Include/Systeme/reserve_liste.s"
*
*---------------------------> 'reserve_chip' <----------------------------*
* Reserve de la mémoire CHIP
* Entrée	=> d0.l	Taille en octet
*		=> a0.l pt/adresse de la chip
* Sortie	<= d7.l=0 / Z=1 : pas d'erreur
		INCLUDE	"Mentat/Include/Systeme/reserve_chip.s"
*
*----------------------------> 'reserve.mem' <----------------------------*
* Entrée	=> a4.l	pt/chaine RESA  (adresse,taille,identificateur,type)
*		=> a5.l	adr. table 't'
* Sortie	<= Z=1 / d7.l=0 pas d'erreur
		INCLUDE	"Mentat/Include/Systeme/reserve.mem.s"
*
*--------------------------> 'libere_memoire' <---------------------------*
* Libère de la mémoire réservée par la routine 'reserve_memoire'
* Entrée	=> d0.l	Adresse de la mémoire à libérer
*		=> a5.l	Adr. 't'
* Sortie	<= d7.l	¤  0 libération effectuée
*		<= d7.l	¤-10 libération échouée
*----------------------------> 'libere_mem' <-----------------------------*
* Entrée	=> a0.l	pt/adresse mémoire à libérer
* 		=> a5.l	adr. table 't'
*---------------------------> 'cherche.zone' <----------------------------*
* Entrée	=> d0.l	adresse mémoire
*		=> a5.l	adr. table 't'
* Sortie	<= d0.l	adresse réelle mémoire (ID MENTAT & PPSIZE)
*		<= Z=1 / d7=0 zone mémoire trouvée
*		<= a0.l	pointeur sur la liste mémoire
		INCLUDE	"Mentat/Include/Systeme/libere_memoire.s"

*---------------------------> 'clear.memory' <----------------------------*
* Entrée	=> d0.l	taille de la zone mémoire en octets
*		=> a0.l	pointeur sur la zone mémoire
		INCLUDE	"Mentat/Include/Systeme/clear.memory.s"

*----------------------------> 'copy.memory' <----------------------------*
* Entrée	=> d0.l	taille de la zone mémoire en octets
*		=> a0.l	pointeur sur la zone mémoire source
*		=> a1.l	pointeur sur la zone mémoire destination
		INCLUDE	"Mentat/Include/Systeme/copy.memory.s"

*----------------------------> 'mentat_info' <----------------------------*
* Affiche un message dans un écran de 5 lignes de 80 caractères
* Entrée	=> a0.l	Adr. du texte en ASCII
*		=> a5.l	Adr. 't'
*		=> a6.l	CUSTOM
* Sortie	<= d7.w	#0 pas d"'erreur
		INCLUDE	"Mentat/Include/Systeme/mentat_info.s"

*----------------------------> 'memory_info' <----------------------------*
* Affiche des informations sur la mémoire (Appel routine 'mentat_info')
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l	CUSTOM
* Sortie	<= d7.w	#0 pas d"'erreur
		IFNE	FLAG_MEMORY
		INCLUDE	"Mentat/Include/Systeme/memory_info.s"
		ENDC

*---------------------------> 'memlist_info' <----------------------------*
* Affiche la liste des adresses mémoires (Appel routine 'mentat_info')
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l	CUSTOM
* Sortie	<= d7.w	#0 pas d"'erreur
		IFNE	FLAG_MEMORY
		INCLUDE	"Mentat/Include/Systeme/memlist_info.s"
		ENDC

*---------------------------> 'freq_request' <----------------------------*
* Demande à l'utilisateur de choisir sa fréquence de balayage écran
* Entrée 	=> a5.l Adr. 't'
*	 	=> a6.l CUSTOM
* Sortie 	<= d7.w #0 pas d"'erreur
		IFNE	FLAG_FREQ
		INCLUDE	"Mentat/Include/Systeme/freq_request.s"
		ENDC

*--------------------------> 'decompacte_ilbm' <--------------------------*
* Décompacte une image ILBM en Raw-Blitter
* Entrée => a2.l Adresse de la table de couleur ou 0 si pas de table
*	 => a3.l Adresse de décompactage ou 0 si pas de décompactage
*	 => a4.l Adresse du fichier IFF-ILBM
* Sortie <= d0.w Largeur de l'image en pixel
*	 <= d1.w Hauteur de l'image en ligne
*	 <= d2.l mot bas nombre de plan de l'image/mot haut flag HAM
*	 <= d3.l Taille en octet de l'image
*	 <= d4.w Format de compactage
*	 <= d5.l mot faible hauteur écran/mort fort largeur écran pixel
*	 <= d7.l ¤ 0 si aucune erreur
*		 ¤-2 Fichier non IFF
*		 ¤-3 Fichier nom ILBM
*		 ¤-4 Chunck non trouvé
*		 ¤-5 Format de compression inconnue
		IFNE	FLAG_ILBM
		INCLUDE	"Mentat/Include/Systeme/decompacte_ilbm.s"
		ENDC

*---------------------------> 'decompacte_pp' <---------------------------*
* Décompacte un fichier powerpacké
* Entrée	=> a0.l	Adr. fin fichier compacté 
*		=> a1.l	Adr. décompactage
*		=> a2.l	Adr. début fichier compacté 
* Sortie	<= d7.l	#-16 Fichier non powerpacké
		IFNE	FLAG_PP
		INCLUDE	"Mentat/Include/Systeme/decompacte_pp.s"
		ENDC

*----------------------------> 'decrunch_pp' <----------------------------*
* Décompacte un fichier powerpacké avec reservation mémoire
* entrée	=> d0.l	adresse du fichier powerpacké
*		=> d1.l	taille du fichier powerpacké
*		=> d2.l	exigence mémoire (chip ou fast)
* sortie	<= d0.l	adr. fichier décompacté
*		<= d1.l	taille fichier décompacté
* 		<= d7.l=0 / Z=1 pas d'erreur
		IFNE	FLAG_PP
		INCLUDE	"Mentat/Include/Systeme/decrunch_pp.s"
		ENDC

*----------------------------> 'move_mouse' <-----------------------------*
* Test déplacement de la souris port 0. 
* (routine appellée par 'interruption_master')
* entrée	=> a5.l	adr. 't'
		INCLUDE	"Mentat/Include/Systeme/move_mouse.s"

*------------------------> 'interruption_master' <------------------------*
* Interruption du temps mort vertical de niveau 3
		INCLUDE	"Mentat/Include/Systeme/interruption_master.s"

*----------------------------> 'wait_timer' <-----------------------------*
* Attente temporisée avec le Timer B du CIA-B
* entrée	d0.l	Valeur de l'attente
* (attente d'une seconde d0.l = 709379)
		INCLUDE	"Mentat/Include/Systeme/wait_timer.s"

*-----------------------------> 'wait_vbl' <------------------------------*
* Attend le temps mort vertical.
* entrée	=> a5.l	zdr. 't'
		INCLUDE	"Mentat/Include/Systeme/wait_vbl.s"

*-------------------------> 'vbl_mouse_buttons' <-------------------------*
* Test les 3 boutons des 2 souris ou boutons joysticks.
* (routine appellée par 'interruption_master')
* entrée	=> a5.l	adr. 't'
*		=> a6.l	CUSTOM
		INCLUDE	"Mentat/Include/Systeme/vbl_mouse_buttons.s"

*------------------------> 'vbl_test_joysticks' <-------------------------*
* entrée	=> a5.l	adr. 't'
*		=> a6.l	CUSTOM
* Test les mouvements des 2 joysticks
* (routine appellée par 'interruption_master')
		INCLUDE	"Mentat/Include/Systeme/vbl_test_joysticks.s"

*----------------------------> 'decode_page' <---------------------------*
* décompacte fichier ILBM pour en faire une page graphique avec un
* hunk d'information
* entrée	=> d0.l	adr. page gfx ILBM
*		=> a2.l	adr. table couleur
* sortie	<= d0.l	adr. page graphique
*		<= a2.l	adr. table couleur ou 0 si pas de table couleur
*		<= d7.l	=0 ou Z=1 si pas d'erreur
		IFNE	FLAG_ILBM
		INCLUDE	"Mentat/Include/Systeme/decode_page.s"
		ENDC

*---------------------------> 'decode_page_pp' <--------------------------*
* décompacte fichier ILBM-PP pour en faire une page graphique avec un
* hunk d'information
* entrée	=> d0.l	adresse du fichier ILBM powerpacké
*		=> d1.l	taille du fichier ILBM powerpacké
* 		=> a2.l	Adr. table couleur ou 0 si pas de table couleur
* sortie	<= d0.l	adr. page graphique
*		<= a2.l	adr. table couleur ou 0 si pas de table couleur
*		<= d7.l	=0 ou Z=1 si pas d'erreur
		IFNE	FLAG_ILBM
		IFNE	FLAG_PP
		INCLUDE	"Mentat/Include/Systeme/decode_page_pp.s"
		ENDC
		ENDC
*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*====================> ROUTINES CHARGEMENT AMIGADOS <=====================*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*
*-----------------------------> 'load_file' <-----------------------------*
* Charge un fichier AmigaDos
* Entrée	=> d0.l	adresse de chargement ou 0 si pas de chargement
*		=> d1.l	nombre d'octet(s) à charger
*		=> a0.l	pt/le nom du fichier AmigaDos
*		=> a5.l	adr. 't'
* Sortie	<= d0.l	taille du fichier
*		<= d1.l	nombre d'octet(s) effectivement chargé(s)
*		<= d7.l	¤ 0 / Z=1 pas d'erreur
*			¤-7 impossible d'ouvrir fichier
*			¤-8 erreur de lecture
*		<= a0.l	pt/le nom du fichier AmigaDos
		IFNE	FLAG_AMIGADOS
		INCLUDE	"Mentat/Include/AmigaDos/load_file.s"
		ENDC

*------------------------> 'charge_fichier_dos' <-------------------------*
* Réserve mémoire et charge fichier AmigaDos
* Entrée	=> a0.l	pt/nom du fichier
*		=> d0.l	exigence mémoire #0 FAST-CHIP, #2 CHIP, #4 FAST
* Sortie	<= d0.l	adresse chargement
*		<= d1.l	taille fichier
*		<= d7.l	¤0 si aucune erreur
*		<= a0.l	pt/nom du fichier
		IFNE	FLAG_AMIGADOS
		INCLUDE	"Mentat/Include/AmigaDos/charge_fichier_dos.s"
		ENDC

*-----------------------------> 'charge_pp' <-----------------------------*
* Charge un fichier compacté et le décompacte
* Entrée	=> d0.l	#2 Chip / #4 Fast / #0 Public
*		=> a0.l	Adr. nom du fichier
*		=> a5.l	Adr. 't'
* Sortie	<= d0.l	Adr. fichier décompacté
*		<= d1.l	Taille fichier décompacté
*		<= a0.l	Adr. nom du fichier
*		<= d7.l =0 / Z=1 pas d'erreur
		IFNE	FLAG_AMIGADOS
		IFNE	FLAG_PP
		INCLUDE	"Mentat/Include/AmigaDos/charge_pp.s"
		ENDC
		ENDC

*----------------------------> 'charge_page' <----------------------------*
* Charge page graphique IFF-ILBM et la décompacte en CHIP-MEMORY
* Entrée	=> a0.l	Adr. nom fichier AmigaDos ILBM
*		=> a2.l	Adr. table couleur ou 0 si pas de table couleur
*		=> a5.l	Adr. 't'
* Sortie	<= d0.l	adr. page graphique
*		<= a0.l	Adr. nom fichier AmigaDos ILBM
*		<= a2.l	Adr. table couleur ou 0 si pas de table couleur
*		<= d7.l =0 / Z=1 pas d'erreur
		IFNE	FLAG_AMIGADOS
		IFNE	FLAG_ILBM
		INCLUDE	"Mentat/Include/AmigaDos/charge_page.s"
		ENDC
		ENDC

*--------------------------> 'charge_page_pp' <---------------------------*
* Charge page graphique IFF-ILBM powerpackée et la décompacte en CHIP-MEMORY
* Entrée	=> a0.l	Adr. nom fichier AmigaDos ILBM
*		=> a2.l	Adr. table couleur ou 0 si pas de table couleur
*		=> a5.l	Adr. 't'
* Sortie	<= d0.l	adr. page graphique raw-blitter
*		<= a0.l	Adr. nom fichier AmigaDos ILBM
*		<= a2.l	Adr. table couleur ou 0 si pas de table couleur
*		<= d7.l =0 / Z=1 pas d'erreur
		IFNE	FLAG_PP
		IFNE	FLAG_AMIGADOS
		IFNE	FLAG_ILBM
		INCLUDE	"Mentat/Include/AmigaDos/charge_page_pp.s"
		ENDC
		ENDC
		ENDC

*-----------------------------> 'save_file' <-----------------------------*
* Entrée	=> d0.l	adresse du buffer à sauver
*		=> d1.l	nombre d'octet à sauver
*		=> a0.l	pt/le nom du fichier AmigaDos
*		=> a5.l	adr. table 't'
* Sortie	<= d0.l	adr. du buffer à sauver
*		<= d1.l	nombre d'octets effectivement sauvés
*		<= d7.l	¤ 0 / Z=1 pas d'erreur
*			¤-27 impossible d'ouvrir fichier
*			¤-26 erreur d'écriture
*		<= a0.l	pt/le nom du fichier AmigaDos
		IFNE	FLAG_AMIGADOS
		INCLUDE	"Mentat/Include/AmigaDos/save_file.s"
		ENDC
*
*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*======================> ROUTINES GESTION D'ECRAN <=======================*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*
*---------------------------> 'ScreenRoutines.s' <-------------------------*
* "init_screen:" 	Initialise un nouvel écran.
* "SCREEN:" 		Structure écran.
* "screen_reserve:" 	Réserve une structure écran (non initialisée).
* "convert_aga_color:"  Convertit la palette "SCOLOR_TABLE" 32 bits au format AGA.
* "aga_color"		Convertit une couleur 32 bits au format AGA.
* "colors_on"		Met toutes les couleurs dans les registres hard (sans fondu).
* "set_colors:"		Initialise les couleurs de la table 'SCOLOR_TABLE2" Copper ou registres.
* "aga_fade_on:"	Initialise les couleurs avec fondu (appellée toutes les VBL).
* "aga_fade_off:"	Met à zéro les couleurs avec fondu (appellée toutes les VBL).
* "raz_256colors:"	Met à zéro les 256 regitres couleurs.
* "set_256colors:"	Met la même valeur dans les 256 registres couleurs.
* "kill_screen:"	Détruit un écran.
* "vbl_fade_on:"	Appelle la routine d'initialisation des couleurs avec fondu.
* "vbl_fade_off:"	Appelle la routine de mise à zéro des couleurs avec fondu.
* "vbl_interlace:"	Gestion d'un écran entrelacé.
* "copy_s1_to_s2:"	Copie de l'écran 1 dans l'écran 2.
* "copy_s1_to_s2:"	Copie de l'écran 1 dans l'écran 2.
* "copy_s1_to_s2s3:"	Copie de l'écran 1 dans l'écran 2 et l"écran 3.
* "copy_s2_to_s1:"	Copie de l'écran 2 dans l'écran 1.
* "copy_s2_to_s3:"	Copie de l'écran 2 dans l'écran 3.
* "copy_s2_to_s1s3:"	Copie de l'écran 2 dans l'écran 1 et l'écran 3.
* "copy_s3_to_s1:"	Copie de l'écran 3 dans l'écran 1.
* "copy_s3_to_s2:"	Copie de l'écran 3 dans l'écran 2.
* "copy_s3_to_s1s2:"	Copie de l'écran 3 dans l'écran 1 et l'écran3.
* "clear_ecran1:"	Efface l'écran 1.
* "clear_ecran2:"	Efface l'écran 2.
* "clear_ecran3:"	Efface l'écran 3.
* "pointe:"		Ecrit les adresses écran pour les pointeurs videos de la liste Copper.
* "pointe_ecran1:"	Affiche écran 1.
* "pointe_ecran2:"	Affiche écran 2.
* "pointe_ecran3:"	Affiche écran 3.
* "flip_ecrans:"	Echange les 2 écrans pour faire du "double buffering".
		INCLUDE	"Mentat/Include/Ecran/ScreenRoutines.s"

*---------------------------> 'affiche_ilbm' <----------------------------*
* Affiche une image IFF-ILBM à l'écran
* Entrée	=>	d0.l	Adresse image IFF-ILBM
*		=>	d1.w	Nombre de buffer(s) 1 2 ou 3
*		=>	d2.w	#1 = Défilement possible horizontal
*		=>	a5.l	Adr. table 't'
*		=>	a6.l	CUSTOM
* Sortie	<=	d0.l	Adresse image IFF-ILBM
*		<=	a0.l	Adresse structure écran
		IFNE	FLAG_ILBM
		INCLUDE	"Mentat/Include/Ecran/affiche_ilbm.s"
		ENDC

*----------------------------> 'affiche_pp' <-----------------------------*
* Affiche une image IFF-ILBM powerpackée à l'écran
* Entrée	=>	d0.l	Adresse fichier powerpacké
*		=>	d1.l	Taille du fichier powerpacké
*		=>	d2.w	Nombre de buffer(s) 1 2 ou 3
*		=>	d3.w	#1 = Défilement possible horizontal
*		=>	a5.l	Adr. table 't'
*		=>	a6.l	CUSTOM
* Sortie	<=	a0.l	Adresse structure écran
		IFNE	FLAG_PP
		IFNE	FLAG_ILBM
		INCLUDE	"Mentat/Include/Ecran/affiche_pp.s"
		ENDC
		ENDC

*-----------------------------> 'printf' <--------------------------------*
* Affiche une chaine de caractère de 8x8, 0.b pour aller à la ligne, et
* deux fois 0.b pour fin du texte
* Entrée	d0.w	Position x en pixel(s) (multiple de 8)
*		d1.w	Position Y en ligne(s)
*		a0.l	Adr. chaine de caractère
* 		a5.l	adr. table 't'
		INCLUDE	"Mentat/Include/Ecran/printf.s"

*---------------------------> 'genere.print' <----------------------------*
* Entrée	=> d3.w Taille ligne page fonte
*		=> d4.w Largeur page fonte
*		=> d5.w Taille ligne écran
*		=> d6.w Largeur page écran
*		=> d7.w	Nombre de plan(s) - 1
*		=> a4.l Structure 'RESA'
*		=> a5.l	Adr. 't'
* Sortie	<= d7.l=0 / Z=1 Pas d'erreur
		INCLUDE	"Mentat/Include/Ecran/genere.print.s"

*
*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*=====================> ROUTINE DE GESTION CLAVIER <======================*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*
*-------------------------------> 'input' <-------------------------------*
* Entrée d'une  chaine de caractère au clavier
* A appeler OBLIGATOIREMENT pendant l'interruption de niveau 3
* Entrée	=> d7.w	Nombre de caractère
*		=> a4.l	Pt/chaine
* Sortie	<= d0.w	#1 = Return / Enter préssée
*		<= d6.w	Position du cuseur de 0 à d7-1
		INCLUDE	"Mentat/Include/Clavier/input.s"
*
*---------------------------> 'test_clavier' <----------------------------*
* Test si une touche doit être prise en compte
* A appeler OBLIGATOIREMENT pendant l'interruption de niveau 3
* Entrée	=> a1.l	pt/key flag
* sortie	<= d0.l	0=non pris en compte/ 1 = pris en compte
		INCLUDE	"Mentat/Include/Clavier/test_clavier.s"
*
*-----------------------> 'interruption_clavier' <------------------------*
* Interruption appelée si une touche est préssée ou relachée
		INCLUDE	"Mentat/Include/Clavier/interruption_clavier.s"
*
*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*====================> ROUTINE DE GESTION DES LUTINS <====================*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*

*--------------------------> 'calcul_mot_ctrl' <--------------------------*
* Caclul mot de contrôle d'un lutin
* Entrée	=> d0.l	Pos. X du lutin en basse résolution
*		=> d1.l	Pos. Y du lutin
*		=> d2.w	Hauteur du lutin
*		=> d6.w	Pos. X fenêtre écran
*		=> d7.w	Pos. X fenêtre écran en basse résolution
* Sortie	d3.l	Les 2 mots de contrôles
		IFNE	FLAG_LUTIN
		INCLUDE	"Mentat/Include/Lutins/calcul_mot_ctrl.s"
		ENDC
* "calcul_mot_ctrl_aga:"	calcul un mot de contrôle AGA
* Entrée	=> d0.l	Pos. X du lutin en super-haute résolution
*		=> d1.l	Pos. Y du lutin
*		=> d2.w	Hauteur du lutin
*		=> d6.w	Pos. X fenêtre écran en super-haute résolution
*		=> d7.w	Pos. Y fenêtre écran

*----------------------------> 'sprites_off' <----------------------------*
* Vire tous les lutins
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l	CUSTOM
		INCLUDE	"Mentat/Include/Lutins/sprites_off.s"
*"sprite_off:"	interdit les cannaux DMA à afficher les lutins
*"sprite_on:"	autorise les cannaux DMA à afficher les lutins

*---------------------------> 'copytolutin16' <---------------------------*
* Recopie un GFX Raw-blitter dans un lutin 16 couleurs
* Entrée	=> d0.w	nombre de ligne du lutin - 1
*		=> a0.l	Adr. du lutin paire
*		=> a1.l	Adr. du lutin impaire
*		=> a2.l	Adr. du GFX
* 'copytolutin16.3:'	lutins 64 pixels (saute les mots de contrôle )
* 'copylutin16.3:'	lutins 64 pixels
* 'copytolutin16.2:'	lutins 32 pixels (saute les mots de contrôle )
* 'copylutin16.2:'	lutins 32 pixels
* 'copytolutin16.1:'	lutins 16 pixels (saute les mots de contrôle )
* 'copylutin16.1:'	lutins 16 pixels
		IFNE	FLAG_LUTIN
		INCLUDE	"Mentat/Include/Lutins/copytolutin16.s"
		ENDC

*---------------------------> 'copytolutin4' <---------------------------*
* Recopie un GFX Raw-blitter dans un lutin 4 couleurs
* Entrée	=> d0.w	nombre de ligne du lutin - 1
*		=> a0.l	Adr. du lutin 
*		=> a2.l	Adr. du GFX
* 'copytolutin4.3:'	lutins 64 pixels (saute les mots de contrôle )
* 'copylutin4.3:'	lutins 64 pixels
* 'copytolutin4.2:'	lutins 32 pixels (saute les mots de contrôle )
* 'copylutin4.2:'	lutins 32 pixels
* 'copytolutin4.1:'	lutins 16 pixels (saute les mots de contrôle )
* 'copylutin4.1:'	lutins 16 pixels
		IFNE	FLAG_LUTIN
		INCLUDE	"Mentat/Include/Lutins/copytolutin4.s"
		ENDC

*--------------------------->'init_pts_lutins'<---------------------------*
* Entrée	=> a5.l	Adr. table 't'
		IFNE	FLAG_LUTIN
		INCLUDE	"Mentat/Include/Lutins/init_pts_lutins.s"
		ENDC

*--------------------------->'raz_table_lutins'<--------------------------*
* Entrée	=> a5.l	Adr. table 't'
		IFNE	FLAG_LUTIN
		INCLUDE	"Mentat/Include/Lutins/raz_table_lutins.s"
		ENDC
*
*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*=======================> ROUTINES DE CONVERTION <========================*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*
*------------------------>  'convertit_decimal' <------------------------*
		INCLUDE	"Mentat/Include/Convertion/convertit_decimal.s"

*------------------------> 'conv_number_ascii' <-------------------------*
		INCLUDE	"Mentat/Include/Convertion/conv_number_ascii.s"

*-----------------------------> 'binarise' <------------------------------*
* Donne le nombre puissance de 2 supérieur
* Entrée	=> d0.l Nombre
* Sortie	<= d0.l Nombre puissance de 2 supérieur ou égal si déjà puissance de 2
		INCLUDE	"Mentat/Include/Convertion/binarise.s"
*
*
*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*=====================> ROUTINE DE GESTION DES BOBS >=====================*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
		IFNE	FLAG_BOB32

*-------------------------> 'bob32_main.init.s' <-------------------------*
* Entrée	=>	d0.l	page graphique BOB
*		=>	a0.l	adresse table BOB
*		=>	a5.l	adresse 't'
* Sortie	<=	d7.l=0	pas d'erreur
*		<=	Z=1	pas d'erreur
* Routine principale d'initialisation des bobs
* Cette routine appelle les routines : 	'bob32_calcul_msk'
*					'bob32_init_liste'
*					'bob32_initbob'
 		INCLUDE	"Mentat/Include/BOB32/bob32_main.init.s"
		INCLUDE	"Mentat/Include/BOB32/vbob32_restore.s"
		INCLUDE	"Mentat/Include/BOB32/vbob32_affiche.s"
		INCLUDE	"Mentat/Include/BOB32/vbob32.ajoute_bob.s"
		INCLUDE	"Mentat/Include/BOB32/bob32_restore.s"
		INCLUDE	"Mentat/Include/BOB32/bob32_affiche.s"
		INCLUDE	"Mentat/Include/BOB32/bob32.ajoute_bob.s"
		INCLUDE	"Mentat/Include/BOB32/bob32_init_liste.s"
		INCLUDE	"Mentat/Include/BOB32/bob32_calcul_msk.s"
		INCLUDE	"Mentat/Include/BOB32/bob32_initbob.s"
		INCLUDE	"Mentat/Include/BOB32/bob32_get.bob.s"
		INCLUDE	"Mentat/Include/BOB32/bob32_anim.s"
		ENDC
