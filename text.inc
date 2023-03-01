; File: text.inc
; Purpose: Display text on the screen
;
; This code shows 5x7 pixel characters in 6x8 pixel cells.
; Because each character position is 6 pixels wide, which
; is not a standard tile width, we use both VERA layers,
; with L0 and L1 offset in the X direction by 2 pixels.
; Also, because a character postion may start on any even
; pixel boundary (relative to its VERA layer), there are
; 2 separate sets of tiles. One set of tiles has character
; data with a 0-pixel offset from the tile position. The
; other set of tiles has character data with a 4-pixel offset
; from the tile position. The 2 tile sets are combined
; into a single, long set of tiles for loading and usage in
; the VERA, so there is only 1 tile map involved.
;
; Here is an illustration of how adjacent 6-pixel wide
; character cells are populated: 'SC' references 'screen'
; pixels, 'L0' is for 'layer 0', and 'L1' is for 'layer 1'.
; The 'SC' number lines count pixels in the horizontal
; (X) direction. The 'L0' number line shows bit offsets
; within tile data bytes, for 'layer 0' (note that these
; offsets are in reverse order from the bit positions in
; the data bytes, because pixel #0 is at bit position #7).
; The 'L1' number line shows bit offsets within tile data
; bytes, for 'layer 1'.
;
; Desired visible effect:
;
; SC 0000000000111111111122222222223333333333
; SC 0123456789012345678901234567890123456789
;    aaaaaabbbbbbccccccddddddeeeeeeffffff
;
; L0 0123456701234567012345670123456701234567
; L0 aaaaaa      cccccc      eeeeee
; L1       bbbbbb      dddddd      ffffff
; L1   0123456701234567012345670123456701234567
;

TEXT_MAP_TILES_ACROSS       = 128
TEXT_MAP_TILES_DOWN         = 64
TEXT_SCREEN_TILES_ACROSS    = 106
TEXT_SCREEN_TILES_DOWN      = 60
TEXT_L0_MAP_BASE_ADDRESS    = $00000
TEXT_L1_MAP_BASE_ADDRESS    = $04000
TEXT_TILE_DATA_ADDRESS      = $08000

TEXT_TILE_DATA_ADDRESS_SHIFTED   = (TEXT_TILE_DATA_ADDRESS>>(11-2))
TEXT_L0_MAP_BASE_ADDRESS_SHIFTED = (TEXT_L0_MAP_BASE_ADDRESS>>9)
TEXT_L1_MAP_BASE_ADDRESS_SHIFTED = (TEXT_L1_MAP_BASE_ADDRESS>>9)

TILE_0_INDEX_BLANK          = 0
TILE_0_INDEX_EMPTY          = 127
TILE_4_INDEX_BLANK          = 96
TILE_4_INDEX_EMPTY          = 350

;-------------------------------------------------------------------
; Load text character pixel data
;-------------------------------------------------------------------

text:   .byte   "alphtile.bin"
end_text:
palette: .byte   "palette.bin"
end_palette:

load_text_data:
    lda	#1          ; Logical file number
	ldx	#SD_DEVICE  ; Device 8 = sd card
	ldy	#0          ; 0=ignore address in bin file (2 first bytes)
	jsr	SETLFS

	lda	#(end_text-text) ; Length of filename
	ldx	#<text	    ; Low byte of filename address
	ldy	#>text	    ; High byte of filename address
	jsr	SETNAM

	ldy	#>TEXT_TILE_DATA_ADDRESS ; VERA HIGH address
	ldx	#<TEXT_TILE_DATA_ADDRESS ; VERA LOW address
	lda	#0+2        ; VERA BANK + 2
	jsr	LOAD        ; Load binary file into VRAM, ignoring 2 first bytes
    rts

load_palette:
    lda	#1          ; Logical file number
	ldx	#SD_DEVICE  ; Device 8 = sd card
	ldy	#0          ; 0=ignore address in bin file (2 first bytes)
	jsr	SETLFS

	lda	#(end_palette-palette) ; Length of filename
	ldx	#<palette   ; Low byte of filename address
	ldy	#>palette   ; High byte of filename address
	jsr	SETNAM

	ldy	#>VRAM_palette ; VERA HIGH address
	ldx	#<VRAM_palette ; VERA LOW address
	lda	#1+2        ; VERA BANK + 2
	jsr	LOAD        ; Load binary file into VRAM, ignoring 2 first bytes
    rts

;-------------------------------------------------------------------
; Initialize alphanumeric tile information.
;-------------------------------------------------------------------

init_text_tile_information:
    ; set the various VERA registers needed

    ; setup layer 0 for text
    lda     #TILE_MAP_HEIGHT_064|TILE_MAP_WIDTH_128|TILE_MODE|LAYER_COLOR_DEPTH_2_BPP
    sta     VERA_L0_config
    lda     #TEXT_L0_MAP_BASE_ADDRESS_SHIFTED
    sta     VERA_L0_mapbase
    lda     #TEXT_TILE_DATA_ADDRESS_SHIFTED|TILE_HEIGHT_08_PIXELS|TILE_WIDTH_08_PIXELS
    sta     VERA_L0_tilebase
    stz     VERA_L0_hscroll_l
    stz     VERA_L0_hscroll_h
    stz     VERA_L0_vscroll_l
    stz     VERA_L0_vscroll_h

    ; setup layer 1 for text
    lda     #TILE_MAP_HEIGHT_064|TILE_MAP_WIDTH_128|TILE_MODE|LAYER_COLOR_DEPTH_2_BPP
    sta     VERA_L1_config
    lda     #TEXT_L1_MAP_BASE_ADDRESS_SHIFTED
    sta     VERA_L1_mapbase
    lda     #TEXT_TILE_DATA_ADDRESS_SHIFTED|TILE_HEIGHT_08_PIXELS|TILE_WIDTH_08_PIXELS
    sta     VERA_L1_tilebase
    lda     #4      ; 4-pixel horizontal (X) offset
    stz     VERA_L1_hscroll_l
    stz     VERA_L1_hscroll_h
    stz     VERA_L1_vscroll_l
    stz     VERA_L1_vscroll_h

    ; setup the tile map information for both layers
    VERA_SET_ADDR0 TEXT_L0_MAP_BASE_ADDRESS
    VERA_SET_ADDR1 TEXT_L1_MAP_BASE_ADDRESS
    ldy     #0                      ; start at tile row 0
init_text_row:
    ldx     #0                      ; start at tile column 0
init_text_cell:
    lda     #TILE_0_INDEX_EMPTY     ; use transparent (empty) tile
    sta     VERA_data0              ; store tile index (LO) in L0
    sta     VERA_data1              ; store tile index (LO) in L1
    lda     #(1<<4)                 ; get palette offset, flip, tile index (HI)
    sta     VERA_data0              ; set tile params in L0
    sta     VERA_data1              ; set tile params in L1
    inx                             ; advance column index
    cpx     #TEXT_MAP_TILES_ACROSS  ; are we done with columns?
    bne     init_text_cell          ; back if no
    iny                             ; advance row index
    cpy     #TEXT_MAP_TILES_DOWN    ; are we done with rows?
    bne     init_text_row           ; back if no
    rts

;-------------------------------------------------------------------
; Write text onto the screen.
;-------------------------------------------------------------------

write_text:
    lda     ZP_CHAR_CNT             ; any more characters to do?
    beq     text_done               ; go if no

    ; compute tile map addresses for row & column

    stz     ZP_PTR_LO               ; init upper pointer into tile map (LO)
    stz     ZP_PTR_HI               ; init upper pointer into tile map (HI)

    ; find the row in the tile map

    ldx     ZP_TILE_ROW             ; get row for text
    beq     dont_inc                ; go if in row #0
find_row:
    clc
    lda     ZP_PTR_LO               ; get upper pointer into tile map (LO)
    adc     #<(TEXT_MAP_TILES_ACROSS*2) ; add map offset for a single row (LO)
    sta     ZP_PTR_LO               ; set upper pointer into tile map (LO)
    lda     ZP_PTR_HI               ; get upper pointer into tile map (HI)
    adc     #>(TEXT_MAP_TILES_ACROSS*2) ; add map offset for a single row (HI)
    sta     ZP_PTR_HI               ; set upper pointer into tile map (HI)
    dex                             ; any more rows to advance?
    bne     find_row                ; go if yes
dont_inc:

    ; find the column in the tile map

    clc
    lda     ZP_TILE_COL             ; get column for text
    asl                             ; multiply by 2
    adc     ZP_PTR_LO               ; add upper pointer into tile map (LO)
    sta     ZP_PTR_LO               ; set upper pointer into tile map (LO)
    bcc     dont_inc2               ; go if no overflow
    inc     ZP_PTR_HI               ; adjust upper pointer into tile map (HI)
dont_inc2:

    ; setup the data pointers to write one character (upper and lower)

    ldx     ZP_PTR_LO               ; get upper pointer into tile map (LO)
    ldy     ZP_PTR_HI               ; get upper pointer into tile map (HI)
    lda     #0                      ; always in bank 0
    VERA_SET_ADDR0_XYA              ; use Data0 register for upper

    lda     #0
write_char:
    sta     VERA_data0
    ldy     VERA_data0              ; cause address increment
    inc

    ; advance to the next character to write

    dec     ZP_CHAR_CNT             ; we finished one character
    bne     write_char              ; back if more to write
text_done:
    rts
