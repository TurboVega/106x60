
; File: zeropage.inc
; Purpose: Zero Page custom variables
; Copyright (c) 2023 by Curtis Whitley
;

ZP_STYLE_PTR        = $22   ; border style pointer
ZP_STYLE_PTR_LO     = $22
ZP_STYLE_PTR_HI     = $23

ZP_PTR              = $30   ; temporary pointer
ZP_PTR_LO           = $30
ZP_PTR_HI           = $31

ZP_TEMP_INDEX       = $32   ; temporary index for calculations
ZP_TEMP_INDEX_LO    = $32   ; temporary index for calculations (LO)
ZP_TEMP_INDEX_HI    = $33   ; temporary index for calculations (HI)

ZP_CHAR_ATTR        = $34   ; palette offset, v-flip, h-flip
ZP_TEMP_COL         = $35   ; current column for computations

ZP_BORDER_FILL      = $37   ; if nonzero, fills the box with blank characters
ZP_BORDER_ROW       = $38   ; border param: character row (line) index
ZP_BORDER_COL       = $39   ; border param: character column index
ZP_BORDER_CNT       = $3A   ; border param: number of characters to show
ZP_BORDER_GROUP     = $3B   ; border param: which character tile group (0 or 1)
ZP_BORDER_PALETTE   = $3C   ; border param: color palette offset (0..15)
ZP_BORDER_STYLE     = $3D   ; border param: border style number
ZP_BORDER_WIDTH     = $3E   ; border param: width of box, including border
ZP_BORDER_HEIGHT    = $3F   ; border param: height of box, including border

ZP_PARAM_ROW        = $40   ; character row (line) index
ZP_PARAM_COL        = $41   ; character column index
ZP_PARAM_CNT        = $42   ; number of characters to show
ZP_PARAM_GROUP      = $43   ; which character tile group (0 or 1)
ZP_PARAM_CODE       = $44   ; Character index code value (not an ASCII code)
ZP_PARAM_PALETTE    = $45   ; color palette offset (0..15)

ZP_PARAM_STR_PTR    = $46   ; points to a string of character index codes (not ASCII codes)
ZP_PARAM_STR_PTR_LO = $46
ZP_PARAM_STR_PTR_HI = $47

ZP_PARAM_STR_SIZE   = $48   ; size (length) of the string pointed to by ZP_PARAM_STR_PTR

ZP_TILE_INDEX_0     = $50   ; tile index (LO) for 0 pixel offset image
ZP_TILE_ATTR_0      = $51   ; tile attributes for 0 pixel offset image
ZP_TILE_INDEX_4A    = $52   ; tile index (LO) for 4 pixel offset image (part A)
ZP_TILE_ATTR_4A     = $53   ; tile attributes for 4 pixel offset image (part A)
ZP_TILE_INDEX_4B    = $54   ; tile index (LO) for 4 pixel offset image (part B)
ZP_TILE_ATTR_4B     = $55   ; tile attributes for 4 pixel offset image (part B)

ZP_TILE_ADDR_0_LO   = $60   ; used for debugging
ZP_TILE_ADDR_0_HI   = $61   ; used for debugging
ZP_TILE_ADDR_1_LO   = $62   ; used for debugging
ZP_TILE_ADDR_1_HI   = $63   ; used for debugging
