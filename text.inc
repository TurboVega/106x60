; File: text.inc
; Purpose: Display text on the screen
; Copyright (c) 2023 by Curtis Whitley
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
; L0X = layer 0 pixel X coordinate (ignoring the layer being shifted)
; L1X = layer 1 pixel X coordinate (ignoring the layer being shifted)
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

TEXT_L0_MAP_BASE_ADDRESS    = $00000
TEXT_L1_MAP_BASE_ADDRESS    = $04000
TEXT_TILE_DATA_ADDRESS      = $08000

TEXT_TILE_DATA_ADDRESS_SHIFTED   = (TEXT_TILE_DATA_ADDRESS>>(11-2))
TEXT_L0_MAP_BASE_ADDRESS_SHIFTED = (TEXT_L0_MAP_BASE_ADDRESS>>9)
TEXT_L1_MAP_BASE_ADDRESS_SHIFTED = (TEXT_L1_MAP_BASE_ADDRESS>>9)

; Values that are absolute (not relative to start of tile group)

TILE_GROUP_A_NUM_CHARS              = 169
TILE_GROUP_A_OFFSET_0_INDEX_START   = 0
TILE_GROUP_A_OFFSET_4_INDEX_START   = (TILE_GROUP_A_OFFSET_0_INDEX_START+TILE_GROUP_A_NUM_CHARS)
TILE_GROUP_A_TOTAL_TILES            = (TILE_GROUP_A_NUM_CHARS*3)

TILE_GROUP_B_NUM_CHARS              = 169
TILE_GROUP_B_OFFSET_0_INDEX_START   = TILE_GROUP_A_TOTAL_TILES
TILE_GROUP_B_OFFSET_4_INDEX_START   = (TILE_GROUP_B_OFFSET_0_INDEX_START+TILE_GROUP_B_NUM_CHARS)
TILE_GROUP_B_TOTAL_TILES            = (TILE_GROUP_B_NUM_CHARS*3)

; Tile indexes relative to start of tile group

TILE_GROUP_A_OFFSET_0_INDEX_BLANK   = 0
TILE_GROUP_A_OFFSET_0_INDEX_EMPTY   = 95
TILE_GROUP_A_OFFSET_4_INDEX_BLANK   = TILE_GROUP_A_NUM_CHARS
TILE_GROUP_A_OFFSET_4_INDEX_EMPTY   = (TILE_GROUP_A_OFFSET_4_INDEX_BLANK+TILE_GROUP_A_OFFSET_0_INDEX_EMPTY*2)

TILE_GROUP_B_OFFSET_0_INDEX_BLANK   = 0
TILE_GROUP_B_OFFSET_0_INDEX_EMPTY   = 95
TILE_GROUP_B_OFFSET_4_INDEX_BLANK   = TILE_GROUP_B_NUM_CHARS
TILE_GROUP_B_OFFSET_4_INDEX_EMPTY   = (TILE_GROUP_B_OFFSET_4_INDEX_BLANK+TILE_GROUP_B_OFFSET_0_INDEX_EMPTY*2)

; Structure to hold above constants, per tile group

    .struct TileGroupLimits
        num_chars                   .word
        offset_0_index_start        .word
        offset_4_index_start        .word
        total_tiles                 .word
        offset_0_index_blank        .word
        offset_0_index_empty        .word
        offset_4_index_blank        .word
        offset_4_index_empty        .word
    .endstruct

tile_group_limits:

tile_group_a:   ; TileGroupLimits
    .word   TILE_GROUP_A_NUM_CHARS
    .word   TILE_GROUP_A_OFFSET_0_INDEX_START
    .word   TILE_GROUP_A_OFFSET_4_INDEX_START
    .word   TILE_GROUP_A_TOTAL_TILES
    .word   TILE_GROUP_A_OFFSET_0_INDEX_BLANK
    .word   TILE_GROUP_A_OFFSET_0_INDEX_EMPTY
    .word   TILE_GROUP_A_OFFSET_4_INDEX_BLANK
    .word   TILE_GROUP_A_OFFSET_4_INDEX_EMPTY

tile_group_b:   ; TileGroupLimits
    .word   TILE_GROUP_B_NUM_CHARS
    .word   TILE_GROUP_B_OFFSET_0_INDEX_START
    .word   TILE_GROUP_B_OFFSET_4_INDEX_START
    .word   TILE_GROUP_B_TOTAL_TILES
    .word   TILE_GROUP_B_OFFSET_0_INDEX_BLANK
    .word   TILE_GROUP_B_OFFSET_0_INDEX_EMPTY
    .word   TILE_GROUP_B_OFFSET_4_INDEX_BLANK
    .word   TILE_GROUP_B_OFFSET_4_INDEX_EMPTY

;-------------------------------------------------------------------
; Load text character pixel data and palette
;-------------------------------------------------------------------

text:   .byte   "tilegrps.bin"
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
; Initialize layer tile information.
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
    lda     #6
    sta     VERA_L0_hscroll_l
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
    lda     #4
    sta     VERA_L1_hscroll_l
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
    lda     #TILE_GROUP_A_OFFSET_0_INDEX_EMPTY ; use transparent (empty) tile
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
; Write text and/or border characters onto the screen.
;-------------------------------------------------------------------

l0_pixel_map_row_addresses:
    .repeat TEXT_SCREEN_TILES_DOWN, row
    .addr   TEXT_L0_MAP_BASE_ADDRESS+(row*TEXT_MAP_TILES_ACROSS*2)+2
    .endrepeat

l1_pixel_map_row_addresses:
    .repeat TEXT_SCREEN_TILES_DOWN, row
    .addr   TEXT_L1_MAP_BASE_ADDRESS+(row*TEXT_MAP_TILES_ACROSS*2)+2
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
    lda     ZP_PARAM_ROW            ; get row for text
    asl                             ; multiply by 2
    tay                             ; index into address table
    lda     l0_pixel_map_row_addresses,y ; get map address (LO)
    sta     ZP_PTR_LO               ; init pointer into tile map (LO)
    lda     l0_pixel_map_row_addresses+1,y ; get map address (HI)
    sta     ZP_PTR_HI               ; init pointer into tile map (HI)
    jsr     find_map_address_for_column
    stx     ZP_TILE_ADDR_0_LO
    sty     ZP_TILE_ADDR_0_HI
    VERA_SET_ADDR0_XYA              ; use Data0 register for upper
    rts


; Compute tile map addresses for row & column in layer #1
convert_map_col_to_address_l1:
    lda     ZP_PARAM_ROW            ; get row for text
    asl                             ; multiply by 2
    tay                             ; index into address table
    lda     l1_pixel_map_row_addresses,y ; get map address (LO)
    sta     ZP_PTR_LO               ; init pointer into tile map (LO)
    lda     l1_pixel_map_row_addresses+1,y ; get map address (HI)
    sta     ZP_PTR_HI               ; init pointer into tile map (HI)
    jsr     find_map_address_for_column
    stx     ZP_TILE_ADDR_1_LO
    sty     ZP_TILE_ADDR_1_HI
    VERA_SET_ADDR1_XYA              ; use Data1 register for upper
    rts

; Compute a tile map index using the character column index.
; To do this, we multiply by 75% (i.e., 3/4), in integer math.
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
;
compute_2nd_tile_map_address:
    lda     ZP_PARAM_COL            ; get character column index
    inc                             ; advance to next column
    bra    compute_address          ; go finish computing address
compute_1st_tile_map_address:
    lda     ZP_PARAM_COL            ; get character column index
compute_address:
    sta     ZP_TEMP_COL             ; save current column index
    jsr     convert_screen_col_to_map_col
    bbs1    ZP_TEMP_COL,case_2_or_3 ; go if column % 4 equals 2 or 3
    bbs0    ZP_TEMP_COL,case_1      ; go if column % 4 equals 1

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
    bbs0    ZP_TEMP_COL,case_3      ; go if column % 4 equals 3

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

; Compute character tile index (not map index), and tile attributes,
; based on given parameters.
;
compute_char_info:
    ldx     #0                          ; offset into table of TileGroupLimits structures
    lda     ZP_PARAM_GROUP              ; get tile group number
    beq     use_tile_grp                ; go if using tile group A
    ldx     #.sizeof(TileGroupLimits)   ; advance offset to be tile group B
use_tile_grp:
    lda     ZP_PARAM_PALETTE            ; get palette offset number
    asl                                 ; shift it into bits (7:4)
    asl
    asl
    asl
    sta     ZP_CHAR_ATTR                ; save shifted palette offset

    lda     ZP_PARAM_CODE               ; get character code parameter
    clc
    adc     tile_group_limits+TileGroupLimits::offset_0_index_start,x
    sta     ZP_TILE_INDEX_0             ; set offset 0 tile index (LO)
    lda     #0                          ; prepare to add HI part
    adc     tile_group_limits+TileGroupLimits::offset_0_index_start+1,x
    ora     ZP_CHAR_ATTR                ; include shifted palette offset
    sta     ZP_TILE_ATTR_0              ; set tile attributes & tile index (HI)

    ldy     #0                          ; will hold shifted HI bit
    lda     ZP_PARAM_CODE               ; get character code parameter
    asl                                 ; multiply by 2
    bcc     keep_x                      ; go if no overflow
    iny                                 ; increment HI part to add
keep_x:
    clc
    adc     tile_group_limits+TileGroupLimits::offset_4_index_start,x
    sta     ZP_TILE_INDEX_4A            ; set offset 4 tile index (part A, LO)
    tya                                 ; prepare to add HI part
    adc     tile_group_limits+TileGroupLimits::offset_4_index_start+1,x
    ora     ZP_CHAR_ATTR                ; include shifted palette offset
    sta     ZP_TILE_ATTR_4A             ; set tile attributes & tile index (part A, HI)

    ldy     #0                          ; will hold shifted HI bit
    lda     ZP_PARAM_CODE               ; get character code parameter
    asl                                 ; multiply by 2
    bcc     keep_x2                     ; go if no overflow
    iny                                 ; increment HI part to add
keep_x2:
    inc                                 ; advance to part B of offset 4 character
    clc
    adc     tile_group_limits+TileGroupLimits::offset_4_index_start,x
    sta     ZP_TILE_INDEX_4B            ; set offset 4 tile index (part B, LO)
    tya                                 ; prepare to add HI part
    adc     tile_group_limits+TileGroupLimits::offset_4_index_start+1,x
    ora     ZP_CHAR_ATTR                ; include shifted palette offset
    sta     ZP_TILE_ATTR_4B             ; set tile attributes & tile index (part B, HI)
    rts

; Write a single character tile index multiple times
;
; The character code used here (ZP_PARAM_CODE) is actually a character index
; into the tile data, so its range is $00 to $A8. Printable ASCII characters
; that normally range from $20 to $7E must be reduced by $20 (yielding an index
; ranging $00 to $5E) before this routine is called. The same is true for the
; defined block characters that would range from $80 to $C8 (they must be
; reduced to the range $60 to $A8). Note that character index $7F (reduced to
; $5F) represents a completely empty (transparent) tile, which has no color,
; neither foreground nor background.
;
write_repeated_char:
    lda     ZP_PARAM_CNT            ; any more characters to do?
    beq     text_done               ; go if no
    jsr     compute_char_info
write_screen_raw_char:
    jsr     compute_1st_tile_map_address
    jsr     compute_2nd_tile_map_address
    bbs1    ZP_PARAM_COL,do_case_2_or_3 ; go if column % 4 equals 2 or 3
    bbs0    ZP_PARAM_COL,do_case_1      ; go if column % 4 equals 1
do_case_0:
    lda     ZP_TILE_INDEX_0         ; get character tile data index
    sta     VERA_data0              ; set character tile data index
    lda     ZP_TILE_ATTR_0          ; get character tile attributes
    sta     VERA_data0              ; set character tile attributes
    dec     ZP_PARAM_CNT            ; we finished one character
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
    dec     ZP_PARAM_CNT            ; we finished one character
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
    dec     ZP_PARAM_CNT            ; we finished one character
    beq     text_done               ; quit if no more to write
do_case_3:
    lda     ZP_TILE_INDEX_0         ; get character tile data index
    sta     VERA_data1              ; set character tile data index
    lda     ZP_TILE_ATTR_0          ; get character tile attributes
    sta     VERA_data1              ; set character tile attributes
    dec     ZP_PARAM_CNT            ; we finished one character
    bne     do_case_0               ; back if more to write
text_done:
    rts
do_case_2_or_3:
    bbs0    ZP_PARAM_COL,do_case_3  ; go if column % 4 equals 3
    bra     do_case_2               ; go if column % 4 equals 2

; Write a string of character indexes on a single row
;
; This routine expects ZP_PARAM_STR_PTR to point to a series of
; reduced character/border code indexes (not ASCII codes). The number
; of characters (size of the string) is given by ZP_PARAM_STR_SIZE.
; Other values which must be given (setup prior to calling this
; routine) are ZP_PARAM_ROW, ZP_PARAM_COL, ZP_PARAM_GROUP, and
; ZP_PARAM_PALETTE.
;
write_array_of_char_code_indexes:
    lda     ZP_PARAM_STR_SIZE       ; get the length of the string (array size)
    beq     nothing_to_do           ; go if array is empty
write_array_item:
    lda     (ZP_PARAM_STR_PTR)      ; get one character code index
    sta     ZP_PARAM_CODE           ; set which character code to write
    lda     #1                      ; load number of similar characters to write
    sta     ZP_PARAM_CNT            ; set number of similar characters to write
    jsr     write_repeated_char     ; go write a single character code
    inc     ZP_PARAM_COL            ; advance the column on the screen
    inc     ZP_PARAM_STR_PTR_LO     ; advance the array pointer (LO)
    bne     dont_inc2               ; go if no overflow
    inc     ZP_PARAM_STR_PTR_HI     ; update the pointer (HI)
dont_inc2:
    dec     ZP_PARAM_STR_SIZE       ; one character is done; reduce the count
    bne     write_array_item        ; back if more characters to write
nothing_to_do:
    rts

; Write a string of ASCII and/or border characters on a single row
;
; This routine expects ZP_PARAM_STR_PTR to point to a series of
; normal character code values (i.e., ASCII codes and/or border codes).
; The number of characters (size of the string) is given by ZP_PARAM_STR_SIZE.
; Other values which must be given (setup prior to calling this
; routine) are ZP_PARAM_ROW, ZP_PARAM_COL, ZP_PARAM_GROUP, and
; ZP_PARAM_PALETTE.
;
write_array_of_char_code_values:
    lda     ZP_PARAM_STR_SIZE       ; get the length of the string (array size)
    beq     nothing_to_do2          ; go if array is empty
write_array_item2:
    lda     (ZP_PARAM_STR_PTR)      ; get one character code value
    sec                             ; prepare to subtract
    sbc     #$20                    ; compute character code index
    sta     ZP_PARAM_CODE           ; set which character code to write
    lda     #1                      ; load number of similar characters to write
    sta     ZP_PARAM_CNT            ; set number of similar characters to write
    jsr     write_repeated_char     ; go write a single character code
    inc     ZP_PARAM_COL            ; advance the column on the screen
    inc     ZP_PARAM_STR_PTR_LO     ; advance the array pointer (LO)
    bne     dont_inc4               ; go if no overflow
    inc     ZP_PARAM_STR_PTR_HI     ; update the pointer (HI)
dont_inc4:
    dec     ZP_PARAM_STR_SIZE       ; one character is done; reduce the count
    bne     write_array_item2       ; back if more characters to write
nothing_to_do2:
    rts

; Read a single character tile index and attributes from the screen
;
; The information is actually read from one of the two tile maps.
;
read_screen_char_raw:
    jsr     compute_1st_tile_map_address
    jsr     compute_2nd_tile_map_address
    bbs1    ZP_PARAM_COL,rr_case_2_or_3 ; go if column % 4 equals 2 or 3
    bbs0    ZP_PARAM_COL,rr_case_1      ; go if column % 4 equals 1
rr_case_0:
    lda     VERA_data0              ; get character tile data index
    sta     ZP_TILE_INDEX_0         ; save character tile data index
    lda     VERA_data0              ; get character tile attributes
    sta     ZP_TILE_ATTR_0          ; save character tile attributes
    lda     #0                      ; load case number
    bra     set_case                ; go set case number and return
rr_case_1:
    lda     VERA_data1              ; get character tile data index A
    sta     ZP_TILE_INDEX_4A        ; save character tile data index A
    lda     VERA_data1              ; get character tile attributes A
    sta     ZP_TILE_ATTR_4A         ; save character tile attributes A
    lda     VERA_data1              ; get character tile data index B
    sta     ZP_TILE_INDEX_4B        ; save character tile data index B
    lda     VERA_data1              ; get character tile attributes B
    sta     ZP_TILE_ATTR_4B         ; save character tile attributes B
    lda     #1                      ; load case number
    bra     set_case                ; go set case number and return
rr_case_2:
    lda     VERA_data0              ; get character tile data index A
    sta     ZP_TILE_INDEX_4A        ; save character tile data index A
    lda     VERA_data0              ; get character tile attributes A
    sta     ZP_TILE_ATTR_4A         ; save character tile attributes A
    lda     VERA_data0              ; get character tile data index B
    sta     ZP_TILE_INDEX_4B        ; save character tile data index B
    lda     VERA_data0              ; get character tile attributes B
    sta     ZP_TILE_ATTR_4B         ; save character tile attributes B
    lda     #2                      ; load case number
    bra     set_case                ; go set case number and return
rr_case_3:
    lda     VERA_data1              ; get character tile data index
    sta     ZP_TILE_INDEX_0         ; save character tile data index
    lda     VERA_data1              ; get character tile attributes
    sta     ZP_TILE_ATTR_0          ; save character tile attributes
    lda     #3                      ; load case number
set_case:
    sta     ZP_PARAM_CASE_NUM       ; set case number
    rts
rr_case_2_or_3:
    bbs0    ZP_PARAM_COL,rr_case_3  ; go if column % 4 equals 3
    bra     rr_case_2               ; go if column % 4 equals 2
