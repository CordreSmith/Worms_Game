; Listing generated by Microsoft (R) Optimizing Compiler Version 19.00.24215.1 

	TITLE	D:\TouchGFX\4.8.0\app\template\PowerSupplySystem\generated\texts\src\LanguageGb.cpp
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB MSVCRTD
INCLUDELIB OLDNAMES

PUBLIC	?textsGb@@3QBQBGB				; textsGb
CONST	SEGMENT
?T_VoltageText_Gb@@3QBGB DW 02H				; T_VoltageText_Gb
	DW	020H
	DW	056H
	DW	00H
?T_CurrentText_Gb@@3QBGB DW 02H				; T_CurrentText_Gb
	DW	020H
	DW	043H
	DW	00H
?T_MultiplierX_Gb@@3QBGB DW 078H			; T_MultiplierX_Gb
	DW	00H
?T_MultiplierText_Gb@@3QBGB DW 04dH			; T_MultiplierText_Gb
	DW	075H
	DW	06cH
	DW	074H
	DW	069H
	DW	070H
	DW	06cH
	DW	069H
	DW	065H
	DW	072H
	DW	020H
	DW	02H
	DW	00H
	ORG $+2
?textsGb@@3QBQBGB DD FLAT:?T_VoltageText_Gb@@3QBGB	; textsGb
	DD	FLAT:?T_CurrentText_Gb@@3QBGB
	DD	FLAT:?T_MultiplierX_Gb@@3QBGB
	DD	FLAT:?T_MultiplierText_Gb@@3QBGB
CONST	ENDS
END