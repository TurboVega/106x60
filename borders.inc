
; File: borders.asm
; Purpose: Draw boxes, areas, and borders from characters
; Copyright (c) 2023 by Curtis Whitley
;

; In these border style values, the first digit (whether represented
; in hexadecimal or decimal) tells the number of foreground-color
; pixels in vertical lines. The second digit tells the number of
; foreground-color pixels in horizontal lines.
;
BORDER_STYLE_V1_H1_HEX  = $11
BORDER_STYLE_V1_H1_DEC  = 11
BORDER_STYLE_V1_H2_HEX  = $12
BORDER_STYLE_V1_H2_DEC  = 12
BORDER_STYLE_V1_H3_HEX  = $13
BORDER_STYLE_V1_H3_DEC  = 13

BORDER_STYLE_V2_H1_HEX  = $21
BORDER_STYLE_V2_H1_DEC  = 21
BORDER_STYLE_V2_H2_HEX  = $22
BORDER_STYLE_V2_H2_DEC  = 22

BORDER_STYLE_V3_H1_HEX  = $31
BORDER_STYLE_V3_H1_DEC  = 31
BORDER_STYLE_V3_H3_HEX  = $33
BORDER_STYLE_V3_H3_DEC  = 33

NUM_BORDER_STYLES       = 7

; Character code numbers for border (box/line) characters.
; The commented numbers are indexes of sections of a bitmap
; containing the original drawing of the various border styles,
; and may be ignored; this program does not use those numbers.
; Only the equated (hexadecimal) numbers matter.
;
BORDER_B2_R1   = $80 ; 002
BORDER_FULL    = $81 ; 000
BORDER_V3_H3   = $82 ; 001
BORDER_B2_H1   = $83 ; 003
BORDER_B2_L1   = $84 ; 004
BORDER_T3_H1   = $85 ; 005
BORDER_GRID_12 = $86 ; 006
BORDER_B3_R1   = $87 ; 007
BORDER_B1_R3   = $88 ; 009
BORDER_B1_H3   = $89 ; 010
BORDER_B1_L3   = $8A ; 011
BORDER_B3_L1   = $8B ; 013
BORDER_B1_R2   = $8C ; 016
BORDER_B1_H2   = $8D ; 017
BORDER_B1_L2   = $8E ; 018
BORDER_B1_R1   = $8F ; 021
BORDER_H1      = $90 ; 022
BORDER_B1_L1   = $91 ; 023
BORDER_V2      = $92 ; 024
BORDER_B3_R3   = $93 ; 028
BORDER_H3      = $94 ; 029
BORDER_B3_L3   = $95 ; 030
BORDER_V1      = $96 ; 031
BORDER_B2_R2   = $97 ; 035
BORDER_H2      = $98 ; 036
BORDER_B2_L2   = $99 ; 037
BORDER_R1_V1   = $9A ; 044
BORDER_V2_H1   = $9B ; 045
BORDER_L1_V1   = $9C ; 046
BORDER_V3      = $9D ; 049
BORDER_R3_V3   = $9E ; 051
BORDER_V1_H3   = $9F ; 052
BORDER_L3_V3   = $A0 ; 053
BORDER_R2_V2   = $A1 ; 058
BORDER_V1_H2   = $A2 ; 059
BORDER_L2_V2   = $A3 ; 060
BORDER_T1_R1   = $A4 ; 063
BORDER_B1_H1   = $A5 ; 064
BORDER_T1_L1   = $A6 ; 065
BORDER_T3_R3   = $A7 ; 070
BORDER_B3_H3   = $A8 ; 071
BORDER_T3_L3   = $A9 ; 072
BORDER_T2_R2   = $AA ; 077
BORDER_B2_H2   = $AB ; 078
BORDER_T2_L2   = $AC ; 079
BORDER_24_DOTS = $AD ; 084
BORDER_R2_V1   = $AE ; 085
BORDER_V2_H2   = $AF ; 087
BORDER_L2_V1   = $B0 ; 089
BORDER_ALT_12  = $B1 ; 090
BORDER_R1_V3   = $B2 ; 092
BORDER_V1_H1   = $B3 ; 094
BORDER_L1_V3   = $B4 ; 096
BORDER_R1_V2   = $B5 ; 099
BORDER_L1_V2   = $B6 ; 103
BORDER_T1_H1   = $B7 ; 106
BORDER_T3_H3   = $B8 ; 113
BORDER_T2_H2   = $B9 ; 120
BORDER_B3_H1   = $BA ; 168
BORDER_R3_V1   = $BB ; 169
BORDER_T2_R1   = $BC ; 170
BORDER_T2_H1   = $BD ; 171
BORDER_T2_L1   = $BE ; 172
BORDER_L3_V1   = $BF ; 173
BORDER_V3_H1   = $C0 ; 174
BORDER_T3_R1   = $C1 ; 175
BORDER_T1_R3   = $C2 ; 177
BORDER_T1_H3   = $C3 ; 178
BORDER_T1_L3   = $C4 ; 179
BORDER_T3_L1   = $C5 ; 181
BORDER_T1_R2   = $C6 ; 184
BORDER_T1_H2   = $C7 ; 185
BORDER_T1_L2   = $C8 ; 186

; Definitions of border styles, in terms of character codes
; used to produce them:
;

    .struct BorderStyleDefinition
        style_code_hex      .byte
        style_code_dec      .byte
        upper_left_code     .byte
        upper_middle_code   .byte
        upper_right_code    .byte
        middle_right_code   .byte
        lower_right_code    .byte
        lower_middle_code   .byte
        lower_left_code     .byte
        middle_left_code    .byte
        horizontal_code     .byte
        vertical_code       .byte
        cross_code          .byte
    .endstruct

border_style_definitions:

    .byte   BORDER_STYLE_V1_H1_HEX, BORDER_STYLE_V1_H1_DEC
    .byte   BORDER_B1_R1, BORDER_B1_H1, BORDER_B1_L1, BORDER_L1_V1
    .byte   BORDER_T1_L1, BORDER_T1_H1, BORDER_T1_R1, BORDER_R1_V1
    .byte   BORDER_H1, BORDER_V1, BORDER_V1_H1

    .byte   BORDER_STYLE_V1_H2_HEX, BORDER_STYLE_V1_H2_DEC
    .byte   BORDER_B1_R2, BORDER_B1_H2, BORDER_B1_L2, BORDER_L2_V1
    .byte   BORDER_T1_L2, BORDER_T1_H2, BORDER_T1_R2, BORDER_R2_V1
    .byte   BORDER_H2, BORDER_V1, BORDER_V1_H2

    .byte   BORDER_STYLE_V1_H3_HEX, BORDER_STYLE_V1_H3_DEC
    .byte   BORDER_B1_R3, BORDER_B1_H3, BORDER_B1_L3, BORDER_L3_V1
    .byte   BORDER_T1_L3, BORDER_T1_H3, BORDER_T1_R3, BORDER_R3_V1
    .byte   BORDER_H3, BORDER_V1, BORDER_V1_H3

    .byte   BORDER_STYLE_V2_H1_HEX, BORDER_STYLE_V2_H1_DEC
    .byte   BORDER_B2_R1, BORDER_B2_H1, BORDER_B2_L1, BORDER_L1_V2
    .byte   BORDER_T2_L1, BORDER_T2_H1, BORDER_T2_R1, BORDER_R1_V2
    .byte   BORDER_H1, BORDER_V2, BORDER_V2_H1

    .byte   BORDER_STYLE_V2_H2_HEX, BORDER_STYLE_V2_H2_DEC
    .byte   BORDER_B2_R2, BORDER_B2_H2, BORDER_B2_L2, BORDER_L2_V2
    .byte   BORDER_T2_L2, BORDER_T2_H2, BORDER_T2_R2, BORDER_R2_V2
    .byte   BORDER_H2, BORDER_V2, BORDER_V2_H2

    .byte   BORDER_STYLE_V3_H1_HEX, BORDER_STYLE_V3_H1_DEC
    .byte   BORDER_B3_R1, BORDER_B3_H1, BORDER_B3_L1, BORDER_L1_V3
    .byte   BORDER_T3_L1, BORDER_T3_H1, BORDER_T3_R1, BORDER_R1_V3
    .byte   BORDER_H1, BORDER_V3, BORDER_V3_H1

    .byte   BORDER_STYLE_V3_H3_HEX, BORDER_STYLE_V3_H3_DEC
    .byte   BORDER_B3_R3, BORDER_B3_H3, BORDER_B3_L3, BORDER_L3_V3
    .byte   BORDER_T3_L3, BORDER_T3_H3, BORDER_T3_R3, BORDER_R3_V3
    .byte   BORDER_H3, BORDER_V3, BORDER_V3_H3

end_border_style_definitions:

