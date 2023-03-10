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

text_array:
    .res    96
end_text_array:

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

    lda     #1
    sta     ZP_PARAM_GROUP
    lda     #1
    sta     ZP_PARAM_PALETTE
    lda     #end_text_array-text_array
    sta     ZP_PARAM_STR_SIZE
    lda     #<text_array
    sta     ZP_PARAM_STR_PTR_LO
    lda     #>text_array
    sta     ZP_PARAM_STR_PTR_HI
    lda     #30
    sta     ZP_PARAM_ROW
    lda     #0
    sta     ZP_PARAM_COL
    ldy     #0
make_data:
    tya
    sta     (ZP_PARAM_STR_PTR),y
    iny
    cpy     #end_text_array-text_array
    bne     make_data
    jsr     write_array_of_char_code_indexes

    lda     #1
    sta     ZP_PARAM_GROUP
    lda     #1
    sta     ZP_PARAM_PALETTE
    lda     #end_text_array-text_array
    sta     ZP_PARAM_STR_SIZE
    lda     #<text_array
    sta     ZP_PARAM_STR_PTR_LO
    lda     #>text_array
    sta     ZP_PARAM_STR_PTR_HI
    lda     #31
    sta     ZP_PARAM_ROW
    lda     #0
    sta     ZP_PARAM_COL
    ldy     #0
make_data2:
    tya
    clc
    adc     #$20
    sta     (ZP_PARAM_STR_PTR),y
    iny
    cpy     #end_text_array-text_array
    bne     make_data2
    jsr     write_array_of_char_code_values

    lda     #1
    sta     ZP_BORDER_FILL
    lda     #21
    sta     ZP_BORDER_ROW
    lda     #50
    sta     ZP_BORDER_COL
    lda     #0
    sta     ZP_BORDER_GROUP
    lda     #1
    sta     ZP_BORDER_PALETTE
    lda     #$11
    sta     ZP_BORDER_STYLE
    lda     #10
    sta     ZP_BORDER_WIDTH
    lda     #15
    sta     ZP_BORDER_HEIGHT
    jsr     draw_border

    lda     #1
    sta     ZP_BORDER_FILL
    lda     #12
    sta     ZP_BORDER_ROW
    lda     #12
    sta     ZP_BORDER_COL
    lda     #1
    sta     ZP_BORDER_GROUP
    lda     #1
    sta     ZP_BORDER_PALETTE
    lda     #21
    sta     ZP_BORDER_STYLE
    lda     #6
    sta     ZP_BORDER_WIDTH
    lda     #7
    sta     ZP_BORDER_HEIGHT
    jsr     draw_border

    lda     #0
    sta     ZP_BORDER_FILL
    lda     #25
    sta     ZP_BORDER_ROW
    lda     #40
    sta     ZP_BORDER_COL
    lda     #0
    sta     ZP_BORDER_GROUP
    lda     #1
    sta     ZP_BORDER_PALETTE
    lda     #$12
    sta     ZP_BORDER_STYLE
    lda     #6
    sta     ZP_BORDER_WIDTH
    lda     #7
    sta     ZP_BORDER_HEIGHT
    jsr     draw_border

    lda     #0
    sta     ZP_BORDER_FILL
    lda     #28
    sta     ZP_BORDER_ROW
    lda     #57
    sta     ZP_BORDER_COL
    lda     #1
    sta     ZP_BORDER_GROUP
    lda     #1
    sta     ZP_BORDER_PALETTE
    lda     #$13
    sta     ZP_BORDER_STYLE
    lda     #6
    sta     ZP_BORDER_WIDTH
    lda     #7
    sta     ZP_BORDER_HEIGHT
    jsr     draw_border

    lda     #25
    sta     ZP_BORDER_ROW
    lda     #50
    sta     ZP_BORDER_COL
    lda     #0
    sta     ZP_BORDER_GROUP
    lda     #1
    sta     ZP_BORDER_PALETTE
    lda     #$22
    sta     ZP_BORDER_STYLE
    lda     #6
    sta     ZP_BORDER_WIDTH
    lda     #7
    sta     ZP_BORDER_HEIGHT
    jsr     draw_border

    lda     #25
    sta     ZP_BORDER_ROW
    lda     #70
    sta     ZP_BORDER_COL
    lda     #0
    sta     ZP_BORDER_GROUP
    lda     #1
    sta     ZP_BORDER_PALETTE
    lda     #$33
    sta     ZP_BORDER_STYLE
    lda     #6
    sta     ZP_BORDER_WIDTH
    lda     #7
    sta     ZP_BORDER_HEIGHT
    jsr     draw_border

    lda     #23
    sta     ZP_BORDER_ROW
    lda     #100
    sta     ZP_BORDER_COL
    lda     #0
    sta     ZP_BORDER_GROUP
    lda     #1
    sta     ZP_BORDER_PALETTE
    lda     #$31
    sta     ZP_BORDER_STYLE
    lda     #6
    sta     ZP_BORDER_WIDTH
    lda     #7
    sta     ZP_BORDER_HEIGHT
    jsr     draw_border

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
