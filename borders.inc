
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
BORDER_B2_R1   = ($80-$20) ; 002
BORDER_FULL    = ($81-$20) ; 000
BORDER_V3_H3   = ($82-$20) ; 001
BORDER_B2_H1   = ($83-$20) ; 003
BORDER_B2_L1   = ($84-$20) ; 004
BORDER_T3_H1   = ($85-$20) ; 005
BORDER_GRID_12 = ($86-$20) ; 006
BORDER_B3_R1   = ($87-$20) ; 007
BORDER_B1_R3   = ($88-$20) ; 009
BORDER_B1_H3   = ($89-$20) ; 010
BORDER_B1_L3   = ($8A-$20) ; 011
BORDER_B3_L1   = ($8B-$20) ; 013
BORDER_B1_R2   = ($8C-$20) ; 016
BORDER_B1_H2   = ($8D-$20) ; 017
BORDER_B1_L2   = ($8E-$20) ; 018
BORDER_B1_R1   = ($8F-$20) ; 021
BORDER_H1      = ($90-$20) ; 022
BORDER_B1_L1   = ($91-$20) ; 023
BORDER_V2      = ($92-$20) ; 024
BORDER_B3_R3   = ($93-$20) ; 028
BORDER_H3      = ($94-$20) ; 029
BORDER_B3_L3   = ($95-$20) ; 030
BORDER_V1      = ($96-$20) ; 031
BORDER_B2_R2   = ($97-$20) ; 035
BORDER_H2      = ($98-$20) ; 036
BORDER_B2_L2   = ($99-$20) ; 037
BORDER_R1_V1   = ($9A-$20) ; 044
BORDER_V2_H1   = ($9B-$20) ; 045
BORDER_L1_V1   = ($9C-$20) ; 046
BORDER_V3      = ($9D-$20) ; 049
BORDER_R3_V3   = ($9E-$20) ; 051
BORDER_V1_H3   = ($9F-$20) ; 052
BORDER_L3_V3   = ($A0-$20) ; 053
BORDER_R2_V2   = ($A1-$20) ; 058
BORDER_V1_H2   = ($A2-$20) ; 059
BORDER_L2_V2   = ($A3-$20) ; 060
BORDER_T1_R1   = ($A4-$20) ; 063
BORDER_B1_H1   = ($A5-$20) ; 064
BORDER_T1_L1   = ($A6-$20) ; 065
BORDER_T3_R3   = ($A7-$20) ; 070
BORDER_B3_H3   = ($A8-$20) ; 071
BORDER_T3_L3   = ($A9-$20) ; 072
BORDER_T2_R2   = ($AA-$20) ; 077
BORDER_B2_H2   = ($AB-$20) ; 078
BORDER_T2_L2   = ($AC-$20) ; 079
BORDER_24_DOTS = ($AD-$20) ; 084
BORDER_R2_V1   = ($AE-$20) ; 085
BORDER_V2_H2   = ($AF-$20) ; 087
BORDER_L2_V1   = ($B0-$20) ; 089
BORDER_ALT_12  = ($B1-$20) ; 090
BORDER_R1_V3   = ($B2-$20) ; 092
BORDER_V1_H1   = ($B3-$20) ; 094
BORDER_L1_V3   = ($B4-$20) ; 096
BORDER_R1_V2   = ($B5-$20) ; 099
BORDER_L1_V2   = ($B6-$20) ; 103
BORDER_T1_H1   = ($B7-$20) ; 106
BORDER_T3_H3   = ($B8-$20) ; 113
BORDER_T2_H2   = ($B9-$20) ; 120
BORDER_B3_H1   = ($BA-$20) ; 168
BORDER_R3_V1   = ($BB-$20) ; 169
BORDER_T2_R1   = ($BC-$20) ; 170
BORDER_T2_H1   = ($BD-$20) ; 171
BORDER_T2_L1   = ($BE-$20) ; 172
BORDER_L3_V1   = ($BF-$20) ; 173
BORDER_V3_H1   = ($C0-$20) ; 174
BORDER_T3_R1   = ($C1-$20) ; 175
BORDER_T1_R3   = ($C2-$20) ; 177
BORDER_T1_H3   = ($C3-$20) ; 178
BORDER_T1_L3   = ($C4-$20) ; 179
BORDER_T3_L1   = ($C5-$20) ; 181
BORDER_T1_R2   = ($C6-$20) ; 184
BORDER_T1_H2   = ($C7-$20) ; 185
BORDER_T1_L2   = ($C8-$20) ; 186

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

; Compute a pointer to the current style definition
;
find_border_style:
    lda     #<border_style_definitions  ; get pointer to style #0 (LO)
    sta     ZP_STYLE_PTR_LO             ; init pointer to style (LO)
    lda     #>border_style_definitions  ; get pointer to style #0 (HI)
    sta     ZP_STYLE_PTR_HI             ; init pointer to style (HI)
    ldx     #NUM_BORDER_STYLES          ; init style counter
style_loop:
    ldy     #BorderStyleDefinition::style_code_hex
    lda     (ZP_STYLE_PTR),y            ; get first valid style number
    cmp     ZP_BORDER_STYLE             ; compare to given border style number
    beq     have_style                  ; go if match
    ldy     #BorderStyleDefinition::style_code_dec
    lda     (ZP_STYLE_PTR),y            ; get first valid style number
    cmp     ZP_BORDER_STYLE             ; compare to given border style number
    beq     have_style                  ; go if match
    clc
    lda     ZP_STYLE_PTR_LO             ; get pointer to style (LO)
    adc     #<.sizeof(BorderStyleDefinition) ; add size of border style definition (LO)
    sta     ZP_STYLE_PTR_LO             ; set pointer to style (LO)
    lda     ZP_STYLE_PTR_HI             ; get pointer to style (HI)
    adc     #>.sizeof(BorderStyleDefinition) ; add size of border style definition (HI)
    sta     ZP_STYLE_PTR_HI             ; set pointer to style (HI)
    dex                                 ; found correct style yet?
    bne     style_loop                  ; back if no
have_style:
    rts

; Draw a border
;
draw_border:
    jsr     find_border_style           ; compute pointer to border style definition
    lda     ZP_BORDER_GROUP             ; copy the border group to the character param group
    sta     ZP_PARAM_GROUP
    lda     ZP_BORDER_PALETTE           ; copy the border palette to the character param palette
    sta     ZP_PARAM_PALETTE

    ; draw the upper-left corner
    lda     ZP_BORDER_ROW
    sta     ZP_PARAM_ROW
    lda     ZP_BORDER_COL
    sta     ZP_PARAM_COL
    lda     #1
    sta     ZP_PARAM_CNT
    ldy     #BorderStyleDefinition::upper_left_code
    lda     (ZP_STYLE_PTR),y
    sta     ZP_PARAM_CODE               ; set character code
    jsr     write_repeated_char

    ; draw the upper-right corner
    lda     ZP_BORDER_ROW
    sta     ZP_PARAM_ROW
    lda     ZP_BORDER_COL
    clc
    adc     ZP_BORDER_WIDTH
    dec
    sta     ZP_PARAM_COL
    lda     #1
    sta     ZP_PARAM_CNT
    ldy     #BorderStyleDefinition::upper_right_code
    lda     (ZP_STYLE_PTR),y
    sta     ZP_PARAM_CODE               ; set character code
    jsr     write_repeated_char

    ; draw the lower-left corner
    lda     ZP_BORDER_ROW
    clc
    adc     ZP_BORDER_HEIGHT
    dec
    sta     ZP_PARAM_ROW
    lda     ZP_BORDER_COL
    sta     ZP_PARAM_COL
    lda     #1
    sta     ZP_PARAM_CNT
    ldy     #BorderStyleDefinition::lower_left_code
    lda     (ZP_STYLE_PTR),y
    sta     ZP_PARAM_CODE               ; set character code
    jsr     write_repeated_char

    ; draw the lower-right corner
    lda     ZP_BORDER_ROW
    clc
    adc     ZP_BORDER_HEIGHT
    dec
    sta     ZP_PARAM_ROW
    lda     ZP_BORDER_COL
    clc
    adc     ZP_BORDER_WIDTH
    dec
    sta     ZP_PARAM_COL
    lda     #1
    sta     ZP_PARAM_CNT
    ldy     #BorderStyleDefinition::lower_right_code
    lda     (ZP_STYLE_PTR),y
    sta     ZP_PARAM_CODE               ; set character code
    jsr     write_repeated_char

    ; draw the upper horizontal bar
    lda     ZP_BORDER_ROW
    sta     ZP_PARAM_ROW
    lda     ZP_BORDER_COL
    inc
    sta     ZP_PARAM_COL
    lda     ZP_BORDER_WIDTH
    dec
    dec
    sta     ZP_PARAM_CNT
    ldy     #BorderStyleDefinition::horizontal_code
    lda     (ZP_STYLE_PTR),y
    sta     ZP_PARAM_CODE               ; set character code
    jsr     write_repeated_char

    ; draw the lower horizontal bar
    lda     ZP_BORDER_ROW
    clc
    adc     ZP_BORDER_HEIGHT
    dec
    sta     ZP_PARAM_ROW
    lda     ZP_BORDER_COL
    inc
    sta     ZP_PARAM_COL
    lda     ZP_BORDER_WIDTH
    dec
    dec
    sta     ZP_PARAM_CNT
    ldy     #BorderStyleDefinition::horizontal_code
    lda     (ZP_STYLE_PTR),y
    sta     ZP_PARAM_CODE               ; set character code
    jsr     write_repeated_char

    ; draw the left vertical bar
    lda     ZP_BORDER_ROW
    sta     ZP_PARAM_ROW
    lda     ZP_BORDER_COL
    sta     ZP_PARAM_COL
    lda     ZP_BORDER_HEIGHT
    dec
    dec
    sta     ZP_BORDER_CNT
    ldy     #BorderStyleDefinition::vertical_code
    lda     (ZP_STYLE_PTR),y
    sta     ZP_PARAM_CODE               ; set character code
vert_loop:
    inc     ZP_PARAM_ROW
    lda     #1
    sta     ZP_PARAM_CNT
    jsr     write_repeated_char
    dec     ZP_BORDER_CNT
    bne     vert_loop

    ; draw the right vertical bar
    lda     ZP_BORDER_ROW
    sta     ZP_PARAM_ROW
    lda     ZP_BORDER_COL
    clc
    adc     ZP_BORDER_WIDTH
    dec
    sta     ZP_PARAM_COL
    lda     ZP_BORDER_HEIGHT
    dec
    dec
    sta     ZP_BORDER_CNT
    ldy     #BorderStyleDefinition::vertical_code
    lda     (ZP_STYLE_PTR),y
    sta     ZP_PARAM_CODE               ; set character code
vert_loop2:
    inc     ZP_PARAM_ROW
    lda     #1
    sta     ZP_PARAM_CNT
    jsr     write_repeated_char
    dec     ZP_BORDER_CNT
    bne     vert_loop2

    rts
