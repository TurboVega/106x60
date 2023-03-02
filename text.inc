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
; character cells are populated, where:
;
; SCC = screen character column index
; SPX = screen pixel X coordinate
; SPD = screen pixels desired for viewing
; C00 = column index in layer 0 (0..105), for pixel offset 0 bits
; C04 = column index in layer 0 (0..105), for pixel offset 4 bits
; C10 = column index in layer 1 (0..211), for pixel offset 0 bits
; C14 = column index in layer 4 (0..211), for pixel offset 4 bits
; L0X = layer 0 pixel X coordinate
; L1X = layer 1 pixel X coordinate
; L0P = layer 0 pixels displayed
; L1P = layer 1 pixels displayed
; xxxxxx = one row within 6x8-pixel character cell (aaaaaa, bbbbbb, etc.)
; '+' = purposeful transparent leading pixels
; '-' = purposeful transparent trailing pixels
;
; SCC 0.....1.....2.....3.....4.....5.....
; SPX 0000000000111111111122222222223333333333
; SPX 0123456789012345678901234567890123456789
; SPD aaaaaabbbbbbccccccddddddeeeeeeffffff
;
; C00 0=======                3=======
; C04         1=======2=======
; L0X 0123456701234567012345670123456701234567
; L0P aaaaaa--++++cccccc------eeeeee--
; L1P   ++++bbbbbb------dddddd--++++ffffff------
; L1X   0123456701234567012345670123456701234567
; C10                   2=======
; C14   0=======1=======        3=======
;

TEXT_MAP_TILES_ACROSS       = 128
TEXT_MAP_TILES_DOWN         = 64
TEXT_SCREEN_TILES_ACROSS    = 106
TEXT_SCREEN_TILES_DOWN      = 60

TILE_0_INDEX_BLANK          = 0
TILE_0_INDEX_EMPTY          = 95
TILE_4_INDEX_BLANK          = 96
TILE_4_INDEX_EMPTY          = 350

TEXT_L0_MAP_BASE_ADDRESS    = $00000
TEXT_L1_MAP_BASE_ADDRESS    = $04000
TEXT_TILE_DATA_ADDRESS      = $08000

TEXT_TILE_DATA_ADDRESS_SHIFTED   = (TEXT_TILE_DATA_ADDRESS>>(11-2))
TEXT_L0_MAP_BASE_ADDRESS_SHIFTED = (TEXT_L0_MAP_BASE_ADDRESS>>9)
TEXT_L1_MAP_BASE_ADDRESS_SHIFTED = (TEXT_L1_MAP_BASE_ADDRESS>>9)

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
    lda     #2      ; 2-pixel horizontal (X) offset
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

l0_pixel_map_row_addresses:
    .repeat TEXT_SCREEN_TILES_DOWN, row
    .addr   TEXT_L0_MAP_BASE_ADDRESS+(row*TEXT_MAP_TILES_ACROSS*2)
    .endrepeat

l1_pixel_map_row_addresses:
    .repeat TEXT_SCREEN_TILES_DOWN, row
    .addr   TEXT_L1_MAP_BASE_ADDRESS+(row*TEXT_MAP_TILES_ACROSS*2)
    .endrepeat

; Find the column in the tile map
find_map_address_for_column:
    clc
    lda     ZP_TILE_COL             ; get column for text
    asl                             ; multiply by 2
    adc     ZP_PTR_LO               ; add pointer into tile map (LO)
    sta     ZP_PTR_LO               ; set pointer into tile map (LO)
    bcc     dont_inc2               ; go if no overflow
    inc     ZP_PTR_HI               ; adjust pointer into tile map (HI)
dont_inc2:
    ldx     ZP_PTR_LO               ; get pointer into tile map (LO)
    ldy     ZP_PTR_HI               ; get pointer into tile map (HI)
    lda     #0                      ; always in bank 0
    rts

; Compute tile map addresses for row & column in layer #0
set_map_address_for_l0_cell:
    lda     ZP_TILE_ROW             ; get row for text
    asl                             ; multiply by 2
    tay                             ; index into address table
    lda     l0_pixel_map_row_addresses,y ; get map address (LO)
    sta     ZP_PTR_LO               ; init pointer into tile map (LO)
    lda     l0_pixel_map_row_addresses+1,y ; get map address (HI)
    sta     ZP_PTR_HI               ; init pointer into tile map (HI)
    jsr     find_map_address_for_column
    VERA_SET_ADDR0_XYA              ; use Data0 register for upper
    rts

; Compute tile map addresses for row & column in layer #1
set_map_address_for_l1_cell:
    lda     ZP_TILE_ROW             ; get row for text
    asl                             ; multiply by 2
    tay                             ; index into address table
    lda     l1_pixel_map_row_addresses,y ; get map address (LO)
    sta     ZP_PTR_LO               ; init pointer into tile map (LO)
    lda     l1_pixel_map_row_addresses+1,y ; get map address (HI)
    stz     ZP_PTR_HI               ; init pointer into tile map (HI)
    jsr     find_map_address_for_column
    VERA_SET_ADDR1_XYA              ; use Data1 register for upper
    rts

; Write a single character tile index multiple times
write_repeated_char:
    lda     ZP_CHAR_CNT             ; any more characters to do?
    beq     text_done               ; go if no

    lda     ZP_CHAR_COL             ; get character column index
    bbs1    ZP_CHAR_COL,case_2_or_3 ; go if column % 4 equals 2 or 3
    bbs0    ZP_CHAR_COL,case_1      ; go if column % 4 equals 1

case_0:
    ; 1st character is on L0, using pixel offset 0
    ; map column index = ((character column index / 4) * 3)
    ; 2nd (next) character needs case 1
    bra     do_case_0

case_1:
    ; 1st character is on L1, using pixel offset 4
    ; 1st map column index = ((character column index / 4) * 3)
    ; 2nd map column index = ((character column index / 4) * 3) + 1
    ; 2nd (next) character needs case 2
    bra     do_case_1

case_2_or_3:
    bbs0    ZP_CHAR_COL,col_3       ; go if column % 4 equals 3

case_2:
    ; 1st character is on L0, using pixel offset 4
    ; 1st map column index = ((character column index / 4) * 3) + 1
    ; 2nd map column index = ((character column index / 4) * 3) + 2
    ; 2nd (next) character needs case 3
    bra     do_case_2

case_3:
    ; 1st character is on L1, using pixel offset 0
    ; map column index = ((character column index / 4) * 3) + 2
    ; 2nd (next) character needs case 0
    bra     do_case_3

    jsr     set_map_address_for_l0_cell
    jsr     set_map_address_for_l1_cell

    lda     ZP_TILE_INDEX           ; get character tile data index
do_case_0:
    sta     VERA_data0              ; set character tile data index
    ldy     VERA_data0              ; cause address increment
    dec     ZP_CHAR_CNT             ; we finished one character
    beq     text_done               ; quit if no more to write
do_case_1:
    sta     VERA_data1              ; set character tile data index
    ldy     VERA_data1              ; cause address increment
    dec     ZP_CHAR_CNT             ; we finished one character
    beq     text_done               ; quit if no more to write
do_case_2:
    sta     VERA_data0              ; set character tile data index
    ldy     VERA_data0              ; cause address increment
    dec     ZP_CHAR_CNT             ; we finished one character
    beq     text_done               ; quit if no more to write
do_case_3:
    sta     VERA_data1              ; set character tile data index
    ldy     VERA_data1              ; cause address increment
    dec     ZP_CHAR_CNT             ; we finished one character
    bne     do_case_0               ; back if more to write
text_done:
    rts
