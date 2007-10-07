*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*
*||||||||||||||||||||||||| CONSTANTES DU MENTAT ||||||||||||||||||||||||||*
*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*
*
*-------------------* Constantes unité de disquettes *--------------------*
SECTEUR_SIZE	=512
BUFFER_SIZE	=13734+2+2
NBRE_SECTEUR	=11
NBRE_PISTE	=160
*
*-----------------------------* Powerpacker *-----------------------------*
PP_SIZE		=$10		Offset entre source & destination
*
*»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»» ECRAN «««««««««««««««««««««««««««««««««*
VSTART_STANDARD	=$2C		pos. Y standart du Hardware manuel d'un écran
HSTART_STANDARD	=$80		pos. X standart du Hardware manuel d'un écran
MAX_SUPERHIRES	=1472		nbre de pixels maximums en "super high résolution"
MAX_HIRES	=736		nbre de pixels maximums en "high résolution"
MAX_LORES	=368		nbre de pixels maximums en "low résolution"
MAX_NOLACE	=290		nbre de lignes maximums en non interlace
*
*---------------------------* PAGE GRAPHIQUE *----------------------------*
* Identification de la page graphique (juste avant les données)
* format de la page en RAW-BLITTER (plans entrelacés)
		RSRESET
PG_ID		rs.l	1	identification de la page 'PAGE'
PG_SPLAN	rs	1	larg. page en octets & taille plan en octets
PG_HEIGHT	rs	1	hauteur de la page en lignes
PG_SLINE	rs	1	taille d'une ligne (PG_SPLAN*PG_NPLAN)
PG_NPLAN	rs	1	nombre de plans de la page
PG_SIZE		rs.l	1	taille de la page en octets
PG_DATA		rs	0	données RAW-BLITTER
*
*----------------------* ECRAN INFORMATION MENTAT *------------------------*
MT.SPLAN	=80
MT.SLINE	=80
MT.NPLAN	=1
*
*-************************** FONTES RESIDENTES **************************-*
FT.SLINE	=256
FT.SPLAN	=256
*
*--------------------------* STUCTURE  "RESA" *---------------------------*
RESA.ADR	=0
RESA.SIZ	=4
RESA.IDE	=8
RESA.TYP	=12

