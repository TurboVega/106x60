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
.include "windows.inc"

default_irq_vector: .addr 0

text_array:
    .res    96
end_text_array:

window_0:
    .byte   0   ; upper_left_row
    .byte   0   ; upper_left_col
    .byte   0   ; tile_color_group
    .byte   1   ; palette_offset
    .byte   33  ; border_style
    .byte   0   ; fill_char
    .byte   106 ; total_width
    .byte   60  ; total_height
    .byte   0   ; cur_row
    .byte   0   ; cur_col

window_1:
    .byte   10  ; upper_left_row
    .byte   10  ; upper_left_col
    .byte   1   ; tile_color_group
    .byte   1   ; palette_offset
    .byte   22  ; border_style
    .byte   ' ' ; fill_char
    .byte   15  ; total_width
    .byte   5   ; total_height
    .byte   0   ; cur_row
    .byte   0   ; cur_col

start:
    stz     VERA_ctrl     ; no reset/DCSEL/ADDRSEL
    stz     VERA_dc_video ; disable display

    FILLVRAM $00, $00000, $8000
    FILLVRAM $00, $08000, $8000
    FILLVRAM $00, $10000, $8000
    FILLVRAM $00, $18000, $79C0
    FILLVRAM $00, $1FC00, $0400

    stz     VERA_ctrl     ; no reset/DCSEL/ADDRSEL
    lda     #ENABLE_LAYER_0|ENABLE_LAYER_1|OUTPUT_MODE_VGA
    sta     VERA_dc_video

    jsr     load_text_data
    jsr     load_palette
    jsr     init_global_video_regs
    jsr     init_text_tile_information

    lda     #<text_array
    sta     ZP_PARAM_STR_PTR_LO
    lda     #>text_array
    sta     ZP_PARAM_STR_PTR_HI
    lda     #end_text_array-text_array
    sta     ZP_PARAM_STR_SIZE
    ldy     #0
make_data2:
    tya
    clc
    adc     #$20
    sta     (ZP_PARAM_STR_PTR),y
    iny
    cpy     #end_text_array-text_array
    bne     make_data2

    lda     #<window_0
    sta     ZP_WINDOW_PTR_LO
    lda     #>window_0
    sta     ZP_WINDOW_PTR_HI
    jsr     create_window

    lda     #<window_1
    sta     ZP_WINDOW_PTR_LO
    lda     #>window_1
    sta     ZP_WINDOW_PTR_HI
    jsr     create_window

    lda     #39
    sta     ZP_PARAM_STR_SIZE
    jsr     write_to_window

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
