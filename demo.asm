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

    .macro PAL_RGB r1, g1, b1, r2, g2, b2, r3, g3, b3
        .byte   (g1<<4)|b1,r1, (g2<<4)|b2,r2, (g3<<4)|b3,r3
    .endmacro

palette_colors:
    ;       color#1  color#2  color#3
    PAL_RGB 0,0,0,15,15,15,0,0,0    ; palette offset 1
    PAL_RGB 0,0,0, 15,0,0, 0,0,0      ; palette offset 2 
    PAL_RGB 0,0,0, 0,0,15, 0,0,0      ; palette offset 3
    PAL_RGB 0,0,0, 0,15,0, 0,0,0      ; palette offset 4
    PAL_RGB 0,0,0, 15,15,0, 0,0,0     ; palette offset 5
    PAL_RGB 8,0,0, 15,15,15, 8,0,0    ; palette offset 6
    PAL_RGB 0,0,8, 15,15,15, 0,0,8    ; palette offset 7
    PAL_RGB 12,12,0, 0,0,0, 12,12,0   ; palette offset 8
    PAL_RGB 0,8,0, 15,15,15, 0,8,0    ; palette offset 9
    PAL_RGB 12,12,12, 0,0,0, 12,12,12  ; palette offset 10
    PAL_RGB 5,5,5, 15,15,15, 5,5,5    ; palette offset 11
    PAL_RGB 10,0,10, 15,15,15, 10,0,10  ; palette offset 12
    PAL_RGB 14,0,14, 0,0,0, 14,0,14   ; palette offset 13
    PAL_RGB 0,12,12, 0,0,0, 0,12,12   ; palette offset 14
    PAL_RGB 4,8,12, 0,0,0, 4,8,12     ; palette offset 15
end_palette_colors:

text_array:
    .res    96+73
end_text_array:

window_0:
    .byte   0   ; upper_left_row
    .byte   0   ; upper_left_col
    .byte   0   ; tile_color_group
    .byte   1   ; palette_offset
    .byte   11  ; border_style
    .byte   0   ; fill_char
    .byte   18  ; total_width
    .byte   13  ; total_height
    .byte   0   ; cur_row
    .byte   0   ; cur_col

window_1:
    .byte   0   ; upper_left_row
    .byte   18  ; upper_left_col
    .byte   1   ; tile_color_group
    .byte   1   ; palette_offset
    .byte   12  ; border_style
    .byte   0   ; fill_char
    .byte   18  ; total_width
    .byte   13  ; total_height
    .byte   0   ; cur_row
    .byte   0   ; cur_col

window_2:
    .byte   0   ; upper_left_row
    .byte   36  ; upper_left_col
    .byte   0   ; tile_color_group
    .byte   2   ; palette_offset
    .byte   13  ; border_style
    .byte   0   ; fill_char
    .byte   18  ; total_width
    .byte   13  ; total_height
    .byte   0   ; cur_row
    .byte   0   ; cur_col

window_3:
    .byte   0   ; upper_left_row
    .byte   54  ; upper_left_col
    .byte   1   ; tile_color_group
    .byte   2   ; palette_offset
    .byte   13  ; border_style
    .byte   0   ; fill_char
    .byte   18  ; total_width
    .byte   13  ; total_height
    .byte   0   ; cur_row
    .byte   0   ; cur_col

window_4:
    .byte   0   ; upper_left_row
    .byte   72  ; upper_left_col
    .byte   0   ; tile_color_group
    .byte   3   ; palette_offset
    .byte   13  ; border_style
    .byte   0   ; fill_char
    .byte   18  ; total_width
    .byte   13  ; total_height
    .byte   0   ; cur_row
    .byte   0   ; cur_col

window_5:
    .byte   0   ; upper_left_row
    .byte   90  ; upper_left_col
    .byte   1   ; tile_color_group
    .byte   3   ; palette_offset
    .byte   13  ; border_style
    .byte   0   ; fill_char
    .byte   16  ; total_width
    .byte   13  ; total_height
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
 
    ; setup some palette colors
    lda     #<palette_colors
    sta     ZP_STYLE_PTR_LO
    lda     #>palette_colors
    sta     ZP_STYLE_PTR_HI
    lda     #<(VRAM_palette+$10*2+2)
    sta     ZP_PTR_LO
    lda     #>(VRAM_palette+$10*2+2)
    sta     ZP_PTR_HI
setup_colors:
    ldx     ZP_PTR_LO
    ldy     ZP_PTR_HI
    lda     #^VRAM_palette
    VERA_SET_ADDR0_XYA
    ldy     #0
copy_color:
    lda     (ZP_STYLE_PTR),y
    sta     VERA_data0
    iny
    cpy     #6
    bne     copy_color
    lda     ZP_STYLE_PTR_LO
    clc
    adc     #6
    sta     ZP_STYLE_PTR_LO
    bcc     no_ptr_inc
    inc     ZP_STYLE_PTR_HI
no_ptr_inc:
    lda     ZP_PTR_LO
    clc
    adc     #$10*2
    sta     ZP_PTR_LO
    bcc     no_ptr_inc2
    inc     ZP_PTR_HI
no_ptr_inc2:
    cmp     #$02
    bne     setup_colors

    ; turn on video layers
    stz     VERA_ctrl     ; no reset/DCSEL/ADDRSEL
    lda     #ENABLE_LAYER_0|ENABLE_LAYER_1|OUTPUT_MODE_VGA
    sta     VERA_dc_video

    ; prepare the tile system
    jsr     load_text_data
    ;jsr     load_palette   -- don't do for this demo
    jsr     init_global_video_regs
    jsr     init_text_tile_information

    ; create some sample text data
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

    ; write data to sample text windows

    lda     #<window_0
    sta     ZP_WINDOW_PTR_LO
    lda     #>window_0
    sta     ZP_WINDOW_PTR_HI
    jsr     create_window
    lda     #<text_array
    sta     ZP_PARAM_STR_PTR_LO
    lda     #>text_array
    sta     ZP_PARAM_STR_PTR_HI
    lda     #end_text_array-text_array
    sta     ZP_PARAM_STR_SIZE
    jsr     write_to_window

    lda     #<window_1
    sta     ZP_WINDOW_PTR_LO
    lda     #>window_1
    sta     ZP_WINDOW_PTR_HI
    jsr     create_window
    lda     #<text_array
    sta     ZP_PARAM_STR_PTR_LO
    lda     #>text_array
    sta     ZP_PARAM_STR_PTR_HI
    lda     #end_text_array-text_array
    sta     ZP_PARAM_STR_SIZE
    jsr     write_to_window

    lda     #<window_2
    sta     ZP_WINDOW_PTR_LO
    lda     #>window_2
    sta     ZP_WINDOW_PTR_HI
    jsr     create_window
    lda     #<text_array
    sta     ZP_PARAM_STR_PTR_LO
    lda     #>text_array
    sta     ZP_PARAM_STR_PTR_HI
    lda     #end_text_array-text_array
    sta     ZP_PARAM_STR_SIZE
    jsr     write_to_window

    lda     #<window_3
    sta     ZP_WINDOW_PTR_LO
    lda     #>window_3
    sta     ZP_WINDOW_PTR_HI
    jsr     create_window
    lda     #<text_array
    sta     ZP_PARAM_STR_PTR_LO
    lda     #>text_array
    sta     ZP_PARAM_STR_PTR_HI
    lda     #end_text_array-text_array
    sta     ZP_PARAM_STR_SIZE
    jsr     write_to_window

    lda     #<window_4
    sta     ZP_WINDOW_PTR_LO
    lda     #>window_4
    sta     ZP_WINDOW_PTR_HI
    jsr     create_window
    lda     #<text_array
    sta     ZP_PARAM_STR_PTR_LO
    lda     #>text_array
    sta     ZP_PARAM_STR_PTR_HI
    lda     #end_text_array-text_array
    sta     ZP_PARAM_STR_SIZE
    jsr     write_to_window

    lda     #<window_5
    sta     ZP_WINDOW_PTR_LO
    lda     #>window_5
    sta     ZP_WINDOW_PTR_HI
    jsr     create_window
    lda     #<text_array
    sta     ZP_PARAM_STR_PTR_LO
    lda     #>text_array
    sta     ZP_PARAM_STR_PTR_HI
    lda     #end_text_array-text_array
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
