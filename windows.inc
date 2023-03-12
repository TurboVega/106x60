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
        cur_row             .byte   ; current cursor row within the window (0..total_height-2)
        cur_col             .byte   ; current cursor column within the window (0..total_width-2)
    .endstruct

; Copy window parameters to border parameters
;
copy_window_params_to_border_params:
    ldy     #WindowParams::upper_left_row
    lda     (ZP_WINDOW_PTR),y
    sta     ZP_BORDER_ROW
    inc
    sta     ZP_WINDOW_FIRST_ROW

    ldy     #WindowParams::upper_left_col
    lda     (ZP_WINDOW_PTR),y
    sta     ZP_BORDER_COL
    inc
    sta     ZP_WINDOW_FIRST_COL

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
    sta     ZP_WINDOW_LAST_COL
    inc
    sta     ZP_WINDOW_END_COL

    ldy     #WindowParams::total_height
    lda     (ZP_WINDOW_PTR),y
    sta     ZP_BORDER_HEIGHT
    dec
    dec
    clc
    adc     ZP_BORDER_ROW
    sta     ZP_WINDOW_LAST_ROW
    inc
    sta     ZP_WINDOW_END_ROW
    rts

; Create a window by drawing its border, possibly filling it
;
; This routine expects ZP_WINDOW_PTR to point to a properly
; initialized window parameter structure.
;
create_window:
    jsr     copy_window_params_to_border_params
    jsr     draw_border
    rts

; Set temporary current position in window
;
set_position_in_window:
    ldy     #WindowParams::cur_row
    lda     (ZP_WINDOW_PTR),y
    clc
    adc     ZP_WINDOW_FIRST_ROW
    sta     ZP_WINDOW_CUR_ROW

    ldy     #WindowParams::cur_col
    lda     (ZP_WINDOW_PTR),y
    clc
    adc     ZP_WINDOW_FIRST_COL
    sta     ZP_WINDOW_CUR_COL
    rts

; Update the current position in a window
;
update_window_position:
    lda     ZP_WINDOW_CUR_ROW           ; get current row position
    sec
    sbc     ZP_WINDOW_FIRST_ROW         ; subtract first row position
    ldy     #WindowParams::cur_row
    sta     (ZP_WINDOW_PTR),y           ; update row position in window

    lda     ZP_WINDOW_CUR_COL           ; get current column position
    sec
    sbc     ZP_WINDOW_FIRST_COL         ; subtract first column position
    ldy     #WindowParams::cur_col
    sta     (ZP_WINDOW_PTR),y           ; update column position in window
    rts

; Scroll a window vertically upward, opening the bottow row
;
scroll_window_up:
    rts

; Write an array of text to a window
;
; Before calling this routine, setup ZP_PARAM_STR_PTR
; and ZP_PARAM_STR_SIZE. After calling, both of those
; will have been modified.
;
; This routine expects ZP_WINDOW_PTR to point to a properly
; initialized window parameter structure, and that the
; create_window routine has already been called, just once.
; 
write_to_window:
    jsr     copy_window_params_to_border_params
    jsr     set_position_in_window
write_string:
    lda     ZP_PARAM_STR_SIZE           ; get the number of remaining characters
    beq     write_done                  ; go if nothing left to do
    lda     ZP_WINDOW_END_COL           ; get right border column
    sec
    sbc     ZP_WINDOW_CUR_COL           ; subtract current column to yield space left
    cmp     ZP_PARAM_STR_SIZE           ; enough room for remaining characters?
    bcc     write_substring             ; go if no
    lda     ZP_PARAM_STR_SIZE           ; get the number of remaining characters
    sta     ZP_WINDOW_CNT_NOW           ; save how many characters to write now
    stz     ZP_WINDOW_CNT_REM           ; clear how many characters to write later
    bra     write_win_chars             ; go write some characters
write_substring:
    sta     ZP_WINDOW_CNT_NOW           ; save how many characters to write now
    lda     ZP_PARAM_STR_SIZE           ; get the number of remaining characters
    sec
    sbc     ZP_WINDOW_CNT_NOW           ; compute chars left after this write
    sta     ZP_WINDOW_CNT_REM           ; save how many characters to write later
    lda     ZP_WINDOW_CNT_NOW           ; get how many characters to write now
    sta     ZP_PARAM_STR_SIZE           ; set number of characters for this write
write_win_chars:
    lda     ZP_WINDOW_CUR_ROW           ; get current row position
    sta     ZP_PARAM_ROW                ; set row position for writing
    lda     ZP_WINDOW_CUR_COL           ; get current column position
    sta     ZP_PARAM_COL                ; set column position for writing
    jsr     write_array_of_char_code_values ; write remaining characters
    lda     ZP_WINDOW_CNT_REM           ; get number of characters now remaining
    sta     ZP_PARAM_STR_SIZE           ; prepare for possible additional writing
    lda     ZP_WINDOW_CNT_NOW           ; get the number of characters just written
    clc
    adc     ZP_WINDOW_CUR_COL           ; add current column position
    sta     ZP_WINDOW_CUR_COL           ; save new current column position
    cmp     ZP_WINDOW_END_COL           ; did we complete the row (line)?
    bcc     write_done                  ; go if no
    lda     ZP_WINDOW_CUR_ROW           ; get current row position
    cmp     ZP_WINDOW_LAST_ROW          ; are we at the bottom already?
    bcs     must_scroll                 ; go if yes
    inc     ZP_WINDOW_CUR_ROW           ; move to the next row
    bra     set_first_col               ; go set column to first one
must_scroll:
    jsr     scroll_window_up            ; make room for a new row
set_first_col:
    lda     ZP_WINDOW_FIRST_COL         ; get column position on the new row
    sta     ZP_WINDOW_CUR_COL           ; set new current column
    bra     write_string                ; continue writing until done
write_done:
    bra     update_window_position      ; go update the position and return
