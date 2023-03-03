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
    lda     #1      ; 1-pixel horizontal (X) offset
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
    lda     #3      ; 3-pixel horizontal (X) offset
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
    lda     ZP_TEMP_INDEX_LO        ; get index for column (LO)
    asl                             ; multiply by 2 (size of map item)
    bcc     dont_inc3               ; go if no overflow
    inc     ZP_TEMP_INDEX_HI        ; adjust index for column (HI)
dont_inc3:
    clc
    adc     ZP_PTR_LO               ; add pointer into tile map (LO)
    sta     ZP_PTR_LO               ; set pointer into tile map (LO)
    lda     ZP_TEMP_INDEX_HI        ; get index for column (HI)
    adc     ZP_PTR_HI               ; adjust pointer into tile map (HI)
    tay                             ; move pointer (HI) to Y
    ldx     ZP_PTR_LO               ; get pointer into tile map (LO)
    lda     #0                      ; always in bank 0
    rts

; Compute tile map addresses for row & column in layer #0
convert_map_col_to_address_l0:
    lda     ZP_CHAR_ROW             ; get row for text
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
convert_map_col_to_address_l1:
    lda     ZP_CHAR_ROW             ; get row for text
    asl                             ; multiply by 2
    tay                             ; index into address table
    lda     l1_pixel_map_row_addresses,y ; get map address (LO)
    sta     ZP_PTR_LO               ; init pointer into tile map (LO)
    lda     l1_pixel_map_row_addresses+1,y ; get map address (HI)
    sta     ZP_PTR_HI               ; init pointer into tile map (HI)
    jsr     find_map_address_for_column
    VERA_SET_ADDR1_XYA              ; use Data1 register for upper
    rts

; Compute a tile map index using the character column index.
; Expects: A contains screen character column (0..105)
; Outputs: ZP_TEMP_INDEX_LO, ZP_TEMP_INDEX_HI
;
convert_screen_col_to_map_col:
    lsr                             ; divide by 2
    lsr                             ; divide by 2 again
    sta     ZP_TEMP_INDEX_LO        ; save (column index / 4)
    asl                             ; compute ((column index / 4) * 2)
    clc
    adc     ZP_TEMP_INDEX_LO        ; now ((column index / 4) * 3)
    sta     ZP_TEMP_INDEX_LO
    stz     ZP_TEMP_INDEX_HI        ; always zero at this point
    rts

; Compute the tile map address, which could be in either layer,
; and could use either 0 or 4 pixel offset. The corresponding
; tile map index is about 3/4ths (75%) as large as the original
; character column index, plus some possible small adjustment.
; Expects: ZP_CHAR_COL, ZP_CHAR_ROW
; Outputs: VERA_data0 or VERA_data1 address prepared
;
compute_2nd_tile_map_address:
    lda     ZP_CHAR_COL             ; get character column index
    inc                             ; advance to next column
    bra    compute_address            ; go finish computing address
compute_1st_tile_map_address:
    lda     ZP_CHAR_COL             ; get character column address
compute_address:
    jsr     convert_screen_col_to_map_col
    bbs1    ZP_CHAR_COL,case_2_or_3 ; go if column % 4 equals 2 or 3
    bbs0    ZP_CHAR_COL,case_1      ; go if column % 4 equals 1

case_0:
    ; 1st character is on L0, using pixel offset 0
    ; Map column index adjustment is 0
    jmp     convert_map_col_to_address_l0

case_1:
    ; 1st character is on L1, using pixel offset 4
    ; Part A map column index adjustment is 0
    ; Part B map column index adjustment is 1
    jmp     convert_map_col_to_address_l1

case_2_or_3:
    bbs0    ZP_CHAR_COL,case_3       ; go if column % 4 equals 3

case_2:
    ; 1st character is on L0, using pixel offset 4
    ; Part A map column index adjustment is 1
    ; Part B map column index adjustment is 2
    inc     ZP_TEMP_INDEX_LO
    jmp     convert_map_col_to_address_l0

case_3:
    ; 1st character is on L1, using pixel offset 1
    ; Map column index adjustment is 2
    lda     ZP_TEMP_INDEX_LO        ; get computed tile map index (LO)
    clc
    adc     #2
    sta     ZP_TEMP_INDEX_LO        ; save computed tile map index (LO)
    jmp     convert_map_col_to_address_l1

; Write a single character tile index multiple times
; Expects: ZP_CHAR_CNT, ZP_CHAR_ROW, ZP_CHAR_COL, ZP_CHAR_ATTR
;
write_repeated_char:
    lda     ZP_CHAR_CNT             ; any more characters to do?
    beq     text_done               ; go if no
    jsr     compute_1st_tile_map_address
    jsr     compute_2nd_tile_map_address
    ;jsr     compute_offset_0_tile_data_info
    ;jsr     compute_offset_4_tile_data_info
    lda ZP_CHAR_CODE
    sta ZP_TILE_INDEX_0
    lda ZP_CHAR_ATTR
    sta ZP_TILE_ATTR_0
    lda ZP_CHAR_CODE
    asl
    clc
    adc #TILE_4_INDEX_BLANK
    sta ZP_TILE_INDEX_4A
    inc
    sta ZP_TILE_INDEX_4B
    lda #0 ;ZP_CHAR_ATTR
    sta ZP_TILE_ATTR_4A
    sta ZP_TILE_ATTR_4B

    bbs1    ZP_CHAR_COL,do_case_2_or_3 ; go if column % 4 equals 2 or 3
    bbs0    ZP_CHAR_COL,do_case_1      ; go if column % 4 equals 1
do_case_0:
    lda     ZP_TILE_INDEX_0         ; get character tile data index
    sta     VERA_data0              ; set character tile data index
    lda     ZP_TILE_ATTR_0          ; get character tile attributes
    sta     VERA_data0              ; set character tile attributes
    dec     ZP_CHAR_CNT             ; we finished one character
    beq     text_done               ; quit if no more to write
do_case_1:
    lda     ZP_TILE_INDEX_4A        ; get character tile data index A
    sta     VERA_data1              ; set character tile data index A
    lda     ZP_TILE_ATTR_4A         ; get character tile attributes A
    sta     VERA_data1              ; set character tile attributes A
    lda     ZP_TILE_INDEX_4B        ; get character tile data index B
    sta     VERA_data1              ; set character tile data index B
    lda     ZP_TILE_ATTR_4B         ; get character tile attributes B
    sta     VERA_data1              ; set character tile attributes B
    dec     ZP_CHAR_CNT             ; we finished one character
    beq     text_done               ; quit if no more to write
do_case_2:
    lda     ZP_TILE_INDEX_4A        ; get character tile data index A
    sta     VERA_data0              ; set character tile data index A
    lda     ZP_TILE_ATTR_4A         ; get character tile attributes A
    sta     VERA_data0              ; set character tile attributes A
    lda     ZP_TILE_INDEX_4B        ; get character tile data index B
    sta     VERA_data0              ; set character tile data index B
    lda     ZP_TILE_ATTR_4B         ; get character tile attributes B
    sta     VERA_data0              ; set character tile attributes B
    dec     ZP_CHAR_CNT             ; we finished one character
    beq     text_done               ; quit if no more to write
do_case_3:
    lda     ZP_TILE_INDEX_0         ; get character tile data index
    sta     VERA_data1              ; set character tile data index
    lda     ZP_TILE_ATTR_0          ; get character tile attributes
    sta     VERA_data1              ; set character tile attributes
    dec     ZP_CHAR_CNT             ; we finished one character
    bne     do_case_0               ; back if more to write
text_done:
    rts
do_case_2_or_3:
    bbs0    ZP_CHAR_COL,do_case_3   ; go if column % 4 equals 3
    bra     do_case_2               ; go if column % 4 equals 2
