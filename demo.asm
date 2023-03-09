; File: demo.asm
; Purpose: Show a 106 character by 60 line display
; Copyright (c) 2023 by Curtis Whitley
;

.org $080D
.segment "STARTUP"
.segment "INIT"
.segment "ONCE"
.segment "CODE"

   jmp start      ; skip the data definitions, and go to code

.include "x16.inc"
.include "zeropage.inc"
.include "text.inc"
.include "borders.inc"

default_irq_vector: .addr 0

start:
    stz     VERA_ctrl     ; no reset/DCSEL/ADDRSEL
    stz     VERA_dc_video ; disable display

    FILLVRAM $00, $00000, $8000
    FILLVRAM $00, $08000, $8000
    FILLVRAM $00, $10000, $8000
    FILLVRAM $00, $18000, $79C0
    FILLVRAM $00, $1FC00, $0400

    jsr     load_text_data
    jsr     load_palette
    jsr     init_global_video_regs
    jsr     init_text_tile_information

    lda     #2
    sta     ZP_PARAM_GROUP
    lda     #1
    sta     ZP_PARAM_PALETTE

    lda     #$41-$20
    sta     ZP_PARAM_CODE
    lda     #4
    sta     ZP_PARAM_CNT
    lda     #30
    sta     ZP_PARAM_ROW
    lda     #0
    sta     ZP_PARAM_COL
    jsr     write_repeated_char

    lda     #110-$20
    sta     ZP_PARAM_CODE
    lda     #106
    sta     ZP_PARAM_CNT
    lda     #0
    sta     ZP_PARAM_ROW
    lda     #0
    sta     ZP_PARAM_COL
    jsr     write_repeated_char

    lda     #69-$20
    sta     ZP_PARAM_CODE
    lda     #50
    sta     ZP_PARAM_CNT
    lda     #5
    sta     ZP_PARAM_ROW
    lda     #0
    sta     ZP_PARAM_COL
    jsr     write_repeated_char

    lda     #71-$20
    sta     ZP_PARAM_CODE
    lda     #50
    sta     ZP_PARAM_CNT
    lda     #6
    sta     ZP_PARAM_ROW
    lda     #50
    sta     ZP_PARAM_COL
    jsr     write_repeated_char

    lda     #73-$20
    sta     ZP_PARAM_CODE
    lda     #6
    sta     ZP_PARAM_CNT
    lda     #7
    sta     ZP_PARAM_ROW
    lda     #100
    sta     ZP_PARAM_COL
    jsr     write_repeated_char

    lda     #111-$20
    sta     ZP_PARAM_CODE
    lda     #106
    sta     ZP_PARAM_CNT
    lda     #59
    sta     ZP_PARAM_ROW
    lda     #0
    sta     ZP_PARAM_COL
    jsr     write_repeated_char

    stz     VERA_ctrl     ; no reset/DCSEL/ADDRSEL
    lda     #ENABLE_LAYER_0|ENABLE_LAYER_1|OUTPUT_MODE_VGA
    sta     VERA_dc_video

@main_loop:
    wai
    bra @main_loop  ; never return, just wait for reset

init_global_video_regs:
    ; set the various VERA registers needed

    ; for DCSEL = 0
    stz     VERA_ctrl           ; no reset/DCSEL/ADDRSEL
    lda     #DISPLAY_SCALE
    sta     VERA_dc_hscale
    sta     VERA_dc_vscale
    stz     VERA_dc_border

    ; for DCSEL = 1
    lda     #DCSEL
    sta     VERA_ctrl
    stz     VERA_dc_hstart
    lda     #(640>>2)
    sta     VERA_dc_hstop
    stz     VERA_dc_vsstart
    lda     #(480>>1)
    sta     VERA_dc_vstop
    rts
