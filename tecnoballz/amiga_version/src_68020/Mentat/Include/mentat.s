*/////////////////////////////////////////////////////////////////////////*
*�������������������������� ROUTINES DU MENTAT ����������������������������
*/////////////////////////////////////////////////////////////////////////*
*FLAG_AMIGADOS	=0	Si=1 Alors assemble routines chargement AmigaDOS
*FLAG_BOB32	=0	Si=1 Alors assemble routines bobs (aff. �cran 32 bits)
*FLAG_BOBSTRUCT	=0	Si=1 Assemble la structure BOB
*FLAG_DMACON.MSK=0	Si=1 Ferme dma bitplane lors d'un acc�s disque-syst�me
*FLAG_EXIT	=0	Si=1 Sortie sous syst�me possible avec la touche "ESC"
*FLAG_FREQ	=0	Si=1 Alors assemble routine demande fr�quence balayage
*FLAG_ILBM	=0	Si=1 Alors assemble routine d�compactage IFF-ILBM
*FLAG_KEY	=0	Si=1 Alors assemble routines de test du clavier
*FLAG_LUTIN	=0	Si=1 Alors assemble routines lutins
*FLAG_MEMORY	=0	Si=1 Alors information m�moire possible
*FLAG_MOUSE0	=0	Si=1 Alors test d�placement souris port 0
*FLAG_PP	=0	Si=1 Alors assemble routine d�compactage PowerPacker
*
*
*������������������������������		   �������������������������������*
*=============================> CONSTANTES <==============================*
*������������������������������		   �������������������������������*
		INCLUDE	"Mentat/Include/Constantes/customs_chips.s"
		INCLUDE	"Mentat/Include/Constantes/offset.s"
		INCLUDE	"Mentat/Include/Constantes/constantes_mentat.s"
		INCLUDE	"Mentat/Include/Clavier/code_raw_clavier.s"
*
*
*�������������������������������������������������������������������������*
*==========================> ROUTINES SYSTEMES <==========================*
*�������������������������������������������������������������������������*
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
* Reserve de la m�moire
* Entr�e	=> d0.l	Taille en octet
*		=> d1.l	Caracteristique #00 Fast ou Chip Memory
*					#02 Chip Memory
*					#04 Fast Memory
*					#-1 Absolue
*		=> a1.l	Adresse absolue (si d1 = -1)
*		=> a5.l	Adr. 't'
* Sortie	<= d0.l	Adresse m�moire 32bits ou #0 si erreur
		INCLUDE	"Mentat/Include/Systeme/reserve_memoire.s"
*
*---------------------------> 'reserve_liste' <---------------------------*
* Reserve une liste m�moire en FAST ou CHIP
* Entr�e	=> d0.l	Taille en octet
*		=> a0.l pt/adresse de la liste
* Sortie	<= d7.l=0 / Z=1 : pas d'erreur
		INCLUDE	"Mentat/Include/Systeme/reserve_liste.s"
*
*---------------------------> 'reserve_chip' <----------------------------*
* Reserve de la m�moire CHIP
* Entr�e	=> d0.l	Taille en octet
*		=> a0.l pt/adresse de la chip
* Sortie	<= d7.l=0 / Z=1 : pas d'erreur
		INCLUDE	"Mentat/Include/Systeme/reserve_chip.s"
*
*----------------------------> 'reserve.mem' <----------------------------*
* Entr�e	=> a4.l	pt/chaine RESA  (adresse,taille,identificateur,type)
*		=> a5.l	adr. table 't'
* Sortie	<= Z=1 / d7.l=0 pas d'erreur
		INCLUDE	"Mentat/Include/Systeme/reserve.mem.s"
*
*--------------------------> 'libere_memoire' <---------------------------*
* Lib�re de la m�moire r�serv�e par la routine 'reserve_memoire'
* Entr�e	=> d0.l	Adresse de la m�moire � lib�rer
*		=> a5.l	Adr. 't'
* Sortie	<= d7.l	�  0 lib�ration effectu�e
*		<= d7.l	�-10 lib�ration �chou�e
*----------------------------> 'libere_mem' <-----------------------------*
* Entr�e	=> a0.l	pt/adresse m�moire � lib�rer
* 		=> a5.l	adr. table 't'
*---------------------------> 'cherche.zone' <----------------------------*
* Entr�e	=> d0.l	adresse m�moire
*		=> a5.l	adr. table 't'
* Sortie	<= d0.l	adresse r�elle m�moire (ID MENTAT & PPSIZE)
*		<= Z=1 / d7=0 zone m�moire trouv�e
*		<= a0.l	pointeur sur la liste m�moire
		INCLUDE	"Mentat/Include/Systeme/libere_memoire.s"

*---------------------------> 'clear.memory' <----------------------------*
* Entr�e	=> d0.l	taille de la zone m�moire en octets
*		=> a0.l	pointeur sur la zone m�moire
		INCLUDE	"Mentat/Include/Systeme/clear.memory.s"

*----------------------------> 'copy.memory' <----------------------------*
* Entr�e	=> d0.l	taille de la zone m�moire en octets
*		=> a0.l	pointeur sur la zone m�moire source
*		=> a1.l	pointeur sur la zone m�moire destination
		INCLUDE	"Mentat/Include/Systeme/copy.memory.s"

*----------------------------> 'mentat_info' <----------------------------*
* Affiche un message dans un �cran de 5 lignes de 80 caract�res
* Entr�e	=> a0.l	Adr. du texte en ASCII
*		=> a5.l	Adr. 't'
*		=> a6.l	CUSTOM
* Sortie	<= d7.w	#0 pas d"'erreur
		INCLUDE	"Mentat/Include/Systeme/mentat_info.s"

*----------------------------> 'memory_info' <----------------------------*
* Affiche des informations sur la m�moire (Appel routine 'mentat_info')
* Entr�e	=> a5.l	Adr. 't'
*		=> a6.l	CUSTOM
* Sortie	<= d7.w	#0 pas d"'erreur
		IFNE	FLAG_MEMORY
		INCLUDE	"Mentat/Include/Systeme/memory_info.s"
		ENDC

*---------------------------> 'memlist_info' <----------------------------*
* Affiche la liste des adresses m�moires (Appel routine 'mentat_info')
* Entr�e	=> a5.l	Adr. 't'
*		=> a6.l	CUSTOM
* Sortie	<= d7.w	#0 pas d"'erreur
		IFNE	FLAG_MEMORY
		INCLUDE	"Mentat/Include/Systeme/memlist_info.s"
		ENDC

*---------------------------> 'freq_request' <----------------------------*
* Demande � l'utilisateur de choisir sa fr�quence de balayage �cran
* Entr�e 	=> a5.l Adr. 't'
*	 	=> a6.l CUSTOM
* Sortie 	<= d7.w #0 pas d"'erreur
		IFNE	FLAG_FREQ
		INCLUDE	"Mentat/Include/Systeme/freq_request.s"
		ENDC

*--------------------------> 'decompacte_ilbm' <--------------------------*
* D�compacte une image ILBM en Raw-Blitter
* Entr�e => a2.l Adresse de la table de couleur ou 0 si pas de table
*	 => a3.l Adresse de d�compactage ou 0 si pas de d�compactage
*	 => a4.l Adresse du fichier IFF-ILBM
* Sortie <= d0.w Largeur de l'image en pixel
*	 <= d1.w Hauteur de l'image en ligne
*	 <= d2.l mot bas nombre de plan de l'image/mot haut flag HAM
*	 <= d3.l Taille en octet de l'image
*	 <= d4.w Format de compactage
*	 <= d5.l mot faible hauteur �cran/mort fort largeur �cran pixel
*	 <= d7.l � 0 si aucune erreur
*		 �-2 Fichier non IFF
*		 �-3 Fichier nom ILBM
*		 �-4 Chunck non trouv�
*		 �-5 Format de compression inconnue
		IFNE	FLAG_ILBM
		INCLUDE	"Mentat/Include/Systeme/decompacte_ilbm.s"
		ENDC

*---------------------------> 'decompacte_pp' <---------------------------*
* D�compacte un fichier powerpack�
* Entr�e	=> a0.l	Adr. fin fichier compact� 
*		=> a1.l	Adr. d�compactage
*		=> a2.l	Adr. d�but fichier compact� 
* Sortie	<= d7.l	#-16 Fichier non powerpack�
		IFNE	FLAG_PP
		INCLUDE	"Mentat/Include/Systeme/decompacte_pp.s"
		ENDC

*----------------------------> 'decrunch_pp' <----------------------------*
* D�compacte un fichier powerpack� avec reservation m�moire
* entr�e	=> d0.l	adresse du fichier powerpack�
*		=> d1.l	taille du fichier powerpack�
*		=> d2.l	exigence m�moire (chip ou fast)
* sortie	<= d0.l	adr. fichier d�compact�
*		<= d1.l	taille fichier d�compact�
* 		<= d7.l=0 / Z=1 pas d'erreur
		IFNE	FLAG_PP
		INCLUDE	"Mentat/Include/Systeme/decrunch_pp.s"
		ENDC

*----------------------------> 'move_mouse' <-----------------------------*
* Test d�placement de la souris port 0. 
* (routine appell�e par 'interruption_master')
* entr�e	=> a5.l	adr. 't'
		INCLUDE	"Mentat/Include/Systeme/move_mouse.s"

*------------------------> 'interruption_master' <------------------------*
* Interruption du temps mort vertical de niveau 3
		INCLUDE	"Mentat/Include/Systeme/interruption_master.s"

*----------------------------> 'wait_timer' <-----------------------------*
* Attente temporis�e avec le Timer B du CIA-B
* entr�e	d0.l	Valeur de l'attente
* (attente d'une seconde d0.l = 709379)
		INCLUDE	"Mentat/Include/Systeme/wait_timer.s"

*-----------------------------> 'wait_vbl' <------------------------------*
* Attend le temps mort vertical.
* entr�e	=> a5.l	zdr. 't'
		INCLUDE	"Mentat/Include/Systeme/wait_vbl.s"

*-------------------------> 'vbl_mouse_buttons' <-------------------------*
* Test les 3 boutons des 2 souris ou boutons joysticks.
* (routine appell�e par 'interruption_master')
* entr�e	=> a5.l	adr. 't'
*		=> a6.l	CUSTOM
		INCLUDE	"Mentat/Include/Systeme/vbl_mouse_buttons.s"

*------------------------> 'vbl_test_joysticks' <-------------------------*
* entr�e	=> a5.l	adr. 't'
*		=> a6.l	CUSTOM
* Test les mouvements des 2 joysticks
* (routine appell�e par 'interruption_master')
		INCLUDE	"Mentat/Include/Systeme/vbl_test_joysticks.s"

*----------------------------> 'decode_page' <---------------------------*
* d�compacte fichier ILBM pour en faire une page graphique avec un
* hunk d'information
* entr�e	=> d0.l	adr. page gfx ILBM
*		=> a2.l	adr. table couleur
* sortie	<= d0.l	adr. page graphique
*		<= a2.l	adr. table couleur ou 0 si pas de table couleur
*		<= d7.l	=0 ou Z=1 si pas d'erreur
		IFNE	FLAG_ILBM
		INCLUDE	"Mentat/Include/Systeme/decode_page.s"
		ENDC

*---------------------------> 'decode_page_pp' <--------------------------*
* d�compacte fichier ILBM-PP pour en faire une page graphique avec un
* hunk d'information
* entr�e	=> d0.l	adresse du fichier ILBM powerpack�
*		=> d1.l	taille du fichier ILBM powerpack�
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
*�������������������������������������������������������������������������*
*====================> ROUTINES CHARGEMENT AMIGADOS <=====================*
*�������������������������������������������������������������������������*
*
*-----------------------------> 'load_file' <-----------------------------*
* Charge un fichier AmigaDos
* Entr�e	=> d0.l	adresse de chargement ou 0 si pas de chargement
*		=> d1.l	nombre d'octet(s) � charger
*		=> a0.l	pt/le nom du fichier AmigaDos
*		=> a5.l	adr. 't'
* Sortie	<= d0.l	taille du fichier
*		<= d1.l	nombre d'octet(s) effectivement charg�(s)
*		<= d7.l	� 0 / Z=1 pas d'erreur
*			�-7 impossible d'ouvrir fichier
*			�-8 erreur de lecture
*		<= a0.l	pt/le nom du fichier AmigaDos
		IFNE	FLAG_AMIGADOS
		INCLUDE	"Mentat/Include/AmigaDos/load_file.s"
		ENDC

*------------------------> 'charge_fichier_dos' <-------------------------*
* R�serve m�moire et charge fichier AmigaDos
* Entr�e	=> a0.l	pt/nom du fichier
*		=> d0.l	exigence m�moire #0 FAST-CHIP, #2 CHIP, #4 FAST
* Sortie	<= d0.l	adresse chargement
*		<= d1.l	taille fichier
*		<= d7.l	�0 si aucune erreur
*		<= a0.l	pt/nom du fichier
		IFNE	FLAG_AMIGADOS
		INCLUDE	"Mentat/Include/AmigaDos/charge_fichier_dos.s"
		ENDC

*-----------------------------> 'charge_pp' <-----------------------------*
* Charge un fichier compact� et le d�compacte
* Entr�e	=> d0.l	#2 Chip / #4 Fast / #0 Public
*		=> a0.l	Adr. nom du fichier
*		=> a5.l	Adr. 't'
* Sortie	<= d0.l	Adr. fichier d�compact�
*		<= d1.l	Taille fichier d�compact�
*		<= a0.l	Adr. nom du fichier
*		<= d7.l =0 / Z=1 pas d'erreur
		IFNE	FLAG_AMIGADOS
		IFNE	FLAG_PP
		INCLUDE	"Mentat/Include/AmigaDos/charge_pp.s"
		ENDC
		ENDC

*----------------------------> 'charge_page' <----------------------------*
* Charge page graphique IFF-ILBM et la d�compacte en CHIP-MEMORY
* Entr�e	=> a0.l	Adr. nom fichier AmigaDos ILBM
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
* Charge page graphique IFF-ILBM powerpack�e et la d�compacte en CHIP-MEMORY
* Entr�e	=> a0.l	Adr. nom fichier AmigaDos ILBM
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
* Entr�e	=> d0.l	adresse du buffer � sauver
*		=> d1.l	nombre d'octet � sauver
*		=> a0.l	pt/le nom du fichier AmigaDos
*		=> a5.l	adr. table 't'
* Sortie	<= d0.l	adr. du buffer � sauver
*		<= d1.l	nombre d'octets effectivement sauv�s
*		<= d7.l	� 0 / Z=1 pas d'erreur
*			�-27 impossible d'ouvrir fichier
*			�-26 erreur d'�criture
*		<= a0.l	pt/le nom du fichier AmigaDos
		IFNE	FLAG_AMIGADOS
		INCLUDE	"Mentat/Include/AmigaDos/save_file.s"
		ENDC
*
*
*�������������������������������������������������������������������������*
*======================> ROUTINES GESTION D'ECRAN <=======================*
*�������������������������������������������������������������������������*
*
*---------------------------> 'ScreenRoutines.s' <-------------------------*
* "init_screen:" 	Initialise un nouvel �cran.
* "SCREEN:" 		Structure �cran.
* "screen_reserve:" 	R�serve une structure �cran (non initialis�e).
* "convert_aga_color:"  Convertit la palette "SCOLOR_TABLE" 32 bits au format AGA.
* "aga_color"		Convertit une couleur 32 bits au format AGA.
* "colors_on"		Met toutes les couleurs dans les registres hard (sans fondu).
* "set_colors:"		Initialise les couleurs de la table 'SCOLOR_TABLE2" Copper ou registres.
* "aga_fade_on:"	Initialise les couleurs avec fondu (appell�e toutes les VBL).
* "aga_fade_off:"	Met � z�ro les couleurs avec fondu (appell�e toutes les VBL).
* "raz_256colors:"	Met � z�ro les 256 regitres couleurs.
* "set_256colors:"	Met la m�me valeur dans les 256 registres couleurs.
* "kill_screen:"	D�truit un �cran.
* "vbl_fade_on:"	Appelle la routine d'initialisation des couleurs avec fondu.
* "vbl_fade_off:"	Appelle la routine de mise � z�ro des couleurs avec fondu.
* "vbl_interlace:"	Gestion d'un �cran entrelac�.
* "copy_s1_to_s2:"	Copie de l'�cran 1 dans l'�cran 2.
* "copy_s1_to_s2:"	Copie de l'�cran 1 dans l'�cran 2.
* "copy_s1_to_s2s3:"	Copie de l'�cran 1 dans l'�cran 2 et l"�cran 3.
* "copy_s2_to_s1:"	Copie de l'�cran 2 dans l'�cran 1.
* "copy_s2_to_s3:"	Copie de l'�cran 2 dans l'�cran 3.
* "copy_s2_to_s1s3:"	Copie de l'�cran 2 dans l'�cran 1 et l'�cran 3.
* "copy_s3_to_s1:"	Copie de l'�cran 3 dans l'�cran 1.
* "copy_s3_to_s2:"	Copie de l'�cran 3 dans l'�cran 2.
* "copy_s3_to_s1s2:"	Copie de l'�cran 3 dans l'�cran 1 et l'�cran3.
* "clear_ecran1:"	Efface l'�cran 1.
* "clear_ecran2:"	Efface l'�cran 2.
* "clear_ecran3:"	Efface l'�cran 3.
* "pointe:"		Ecrit les adresses �cran pour les pointeurs videos de la liste Copper.
* "pointe_ecran1:"	Affiche �cran 1.
* "pointe_ecran2:"	Affiche �cran 2.
* "pointe_ecran3:"	Affiche �cran 3.
* "flip_ecrans:"	Echange les 2 �crans pour faire du "double buffering".
		INCLUDE	"Mentat/Include/Ecran/ScreenRoutines.s"

*---------------------------> 'affiche_ilbm' <----------------------------*
* Affiche une image IFF-ILBM � l'�cran
* Entr�e	=>	d0.l	Adresse image IFF-ILBM
*		=>	d1.w	Nombre de buffer(s) 1 2 ou 3
*		=>	d2.w	#1 = D�filement possible horizontal
*		=>	a5.l	Adr. table 't'
*		=>	a6.l	CUSTOM
* Sortie	<=	d0.l	Adresse image IFF-ILBM
*		<=	a0.l	Adresse structure �cran
		IFNE	FLAG_ILBM
		INCLUDE	"Mentat/Include/Ecran/affiche_ilbm.s"
		ENDC

*----------------------------> 'affiche_pp' <-----------------------------*
* Affiche une image IFF-ILBM powerpack�e � l'�cran
* Entr�e	=>	d0.l	Adresse fichier powerpack�
*		=>	d1.l	Taille du fichier powerpack�
*		=>	d2.w	Nombre de buffer(s) 1 2 ou 3
*		=>	d3.w	#1 = D�filement possible horizontal
*		=>	a5.l	Adr. table 't'
*		=>	a6.l	CUSTOM
* Sortie	<=	a0.l	Adresse structure �cran
		IFNE	FLAG_PP
		IFNE	FLAG_ILBM
		INCLUDE	"Mentat/Include/Ecran/affiche_pp.s"
		ENDC
		ENDC

*-----------------------------> 'printf' <--------------------------------*
* Affiche une chaine de caract�re de 8x8, 0.b pour aller � la ligne, et
* deux fois 0.b pour fin du texte
* Entr�e	d0.w	Position x en pixel(s) (multiple de 8)
*		d1.w	Position Y en ligne(s)
*		a0.l	Adr. chaine de caract�re
* 		a5.l	adr. table 't'
		INCLUDE	"Mentat/Include/Ecran/printf.s"

*---------------------------> 'genere.print' <----------------------------*
* Entr�e	=> d3.w Taille ligne page fonte
*		=> d4.w Largeur page fonte
*		=> d5.w Taille ligne �cran
*		=> d6.w Largeur page �cran
*		=> d7.w	Nombre de plan(s) - 1
*		=> a4.l Structure 'RESA'
*		=> a5.l	Adr. 't'
* Sortie	<= d7.l=0 / Z=1 Pas d'erreur
		INCLUDE	"Mentat/Include/Ecran/genere.print.s"

*
*
*�������������������������������������������������������������������������*
*=====================> ROUTINE DE GESTION CLAVIER <======================*
*�������������������������������������������������������������������������*
*
*-------------------------------> 'input' <-------------------------------*
* Entr�e d'une  chaine de caract�re au clavier
* A appeler OBLIGATOIREMENT pendant l'interruption de niveau 3
* Entr�e	=> d7.w	Nombre de caract�re
*		=> a4.l	Pt/chaine
* Sortie	<= d0.w	#1 = Return / Enter pr�ss�e
*		<= d6.w	Position du cuseur de 0 � d7-1
		INCLUDE	"Mentat/Include/Clavier/input.s"
*
*---------------------------> 'test_clavier' <----------------------------*
* Test si une touche doit �tre prise en compte
* A appeler OBLIGATOIREMENT pendant l'interruption de niveau 3
* Entr�e	=> a1.l	pt/key flag
* sortie	<= d0.l	0=non pris en compte/ 1 = pris en compte
		INCLUDE	"Mentat/Include/Clavier/test_clavier.s"
*
*-----------------------> 'interruption_clavier' <------------------------*
* Interruption appel�e si une touche est pr�ss�e ou relach�e
		INCLUDE	"Mentat/Include/Clavier/interruption_clavier.s"
*
*
*�������������������������������������������������������������������������*
*====================> ROUTINE DE GESTION DES LUTINS <====================*
*�������������������������������������������������������������������������*

*--------------------------> 'calcul_mot_ctrl' <--------------------------*
* Caclul mot de contr�le d'un lutin
* Entr�e	=> d0.l	Pos. X du lutin en basse r�solution
*		=> d1.l	Pos. Y du lutin
*		=> d2.w	Hauteur du lutin
*		=> d6.w	Pos. X fen�tre �cran
*		=> d7.w	Pos. X fen�tre �cran en basse r�solution
* Sortie	d3.l	Les 2 mots de contr�les
		IFNE	FLAG_LUTIN
		INCLUDE	"Mentat/Include/Lutins/calcul_mot_ctrl.s"
		ENDC
* "calcul_mot_ctrl_aga:"	calcul un mot de contr�le AGA
* Entr�e	=> d0.l	Pos. X du lutin en super-haute r�solution
*		=> d1.l	Pos. Y du lutin
*		=> d2.w	Hauteur du lutin
*		=> d6.w	Pos. X fen�tre �cran en super-haute r�solution
*		=> d7.w	Pos. Y fen�tre �cran

*----------------------------> 'sprites_off' <----------------------------*
* Vire tous les lutins
* Entr�e	=> a5.l	Adr. 't'
*		=> a6.l	CUSTOM
		INCLUDE	"Mentat/Include/Lutins/sprites_off.s"
*"sprite_off:"	interdit les cannaux DMA � afficher les lutins
*"sprite_on:"	autorise les cannaux DMA � afficher les lutins

*---------------------------> 'copytolutin16' <---------------------------*
* Recopie un GFX Raw-blitter dans un lutin 16 couleurs
* Entr�e	=> d0.w	nombre de ligne du lutin - 1
*		=> a0.l	Adr. du lutin paire
*		=> a1.l	Adr. du lutin impaire
*		=> a2.l	Adr. du GFX
* 'copytolutin16.3:'	lutins 64 pixels (saute les mots de contr�le )
* 'copylutin16.3:'	lutins 64 pixels
* 'copytolutin16.2:'	lutins 32 pixels (saute les mots de contr�le )
* 'copylutin16.2:'	lutins 32 pixels
* 'copytolutin16.1:'	lutins 16 pixels (saute les mots de contr�le )
* 'copylutin16.1:'	lutins 16 pixels
		IFNE	FLAG_LUTIN
		INCLUDE	"Mentat/Include/Lutins/copytolutin16.s"
		ENDC

*---------------------------> 'copytolutin4' <---------------------------*
* Recopie un GFX Raw-blitter dans un lutin 4 couleurs
* Entr�e	=> d0.w	nombre de ligne du lutin - 1
*		=> a0.l	Adr. du lutin 
*		=> a2.l	Adr. du GFX
* 'copytolutin4.3:'	lutins 64 pixels (saute les mots de contr�le )
* 'copylutin4.3:'	lutins 64 pixels
* 'copytolutin4.2:'	lutins 32 pixels (saute les mots de contr�le )
* 'copylutin4.2:'	lutins 32 pixels
* 'copytolutin4.1:'	lutins 16 pixels (saute les mots de contr�le )
* 'copylutin4.1:'	lutins 16 pixels
		IFNE	FLAG_LUTIN
		INCLUDE	"Mentat/Include/Lutins/copytolutin4.s"
		ENDC

*--------------------------->'init_pts_lutins'<---------------------------*
* Entr�e	=> a5.l	Adr. table 't'
		IFNE	FLAG_LUTIN
		INCLUDE	"Mentat/Include/Lutins/init_pts_lutins.s"
		ENDC

*--------------------------->'raz_table_lutins'<--------------------------*
* Entr�e	=> a5.l	Adr. table 't'
		IFNE	FLAG_LUTIN
		INCLUDE	"Mentat/Include/Lutins/raz_table_lutins.s"
		ENDC
*
*
*�������������������������������������������������������������������������*
*=======================> ROUTINES DE CONVERTION <========================*
*�������������������������������������������������������������������������*
*
*------------------------>  'convertit_decimal' <------------------------*
		INCLUDE	"Mentat/Include/Convertion/convertit_decimal.s"

*------------------------> 'conv_number_ascii' <-------------------------*
		INCLUDE	"Mentat/Include/Convertion/conv_number_ascii.s"

*-----------------------------> 'binarise' <------------------------------*
* Donne le nombre puissance de 2 sup�rieur
* Entr�e	=> d0.l Nombre
* Sortie	<= d0.l Nombre puissance de 2 sup�rieur ou �gal si d�j� puissance de 2
		INCLUDE	"Mentat/Include/Convertion/binarise.s"
*
*
*
*�������������������������������������������������������������������������*
*=====================> ROUTINE DE GESTION DES BOBS >=====================*
*�������������������������������������������������������������������������*
		IFNE	FLAG_BOB32

*-------------------------> 'bob32_main.init.s' <-------------------------*
* Entr�e	=>	d0.l	page graphique BOB
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
