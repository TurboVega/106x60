; File: windows.asm
; Purpose: Draw windows and write text into them
; Copyright (c) 2023 by Curtis Whitley
;

    .struct WindowParams
        upper_left_row      .byte   ; upper-left character row (line) index
        upper_left_col      .byte   ; upper-left character column index
        tile_color_group    .byte   ; which character tile group (0 or 1)
        palette_offset      .byte   ; color palette offset (0..15)
        border_style        .byte   ; border style number (0..6)
        fill_char           .byte   ; character used to fill the window, or zero for none
        total_width         .byte   ; width of window, including border
        total_height        .byte   ; height of window, including border
        cur_row             .byte   ; current cursor row within the window
        cur_col             .byte   ; current cursor column within the window
    .endstruct

; Copy window parameters to border parameters
;
copy_window_params_to_border_params:
    ldy     #WindowParams::upper_left_row
    lda     (ZP_WINDOW_PTR),y
    sta     ZP_BORDER_ROW
    inc
    sta     ZP_WINDOW_START_ROW

    ldy     #WindowParams::upper_left_col
    lda     (ZP_WINDOW_PTR),y
    sta     ZP_BORDER_COL
    inc
    sta     ZP_WINDOW_START_COL

    ldy     #WindowParams::tile_color_group
    lda     (ZP_WINDOW_PTR),y
    sta     ZP_BORDER_GROUP

    ldy     #WindowParams::palette_offset
    lda     (ZP_WINDOW_PTR),y
    sta     ZP_BORDER_PALETTE

    ldy     #WindowParams::border_style
    lda     (ZP_WINDOW_PTR),y
    sta     ZP_BORDER_STYLE

    ldy     #WindowParams::fill_char
    lda     (ZP_WINDOW_PTR),y
    beq     no_fill
    sec
    sbc     #$20
    sta     ZP_BORDER_FILL
    bra     next_param
no_fill:
    lda     #$FF
    sta     ZP_BORDER_FILL
next_param:

    ldy     #WindowParams::total_width
    lda     (ZP_WINDOW_PTR),y
    sta     ZP_BORDER_WIDTH
    dec
    dec
    clc
    adc     ZP_BORDER_COL
    sta     ZP_WINDOW_END_COL

    ldy     #WindowParams::total_height
    lda     (ZP_WINDOW_PTR),y
    sta     ZP_BORDER_HEIGHT
    dec
    dec
    clc
    adc     ZP_BORDER_ROW
    sta     ZP_WINDOW_END_ROW
    rts

; Create a window by drawing its border, possibly filling it
;
create_window:
    jsr     copy_window_params_to_border_params
    jsr     draw_border
    rts
