
; File: zeropage.inc
; Purpose: Zero Page custom variables
; Copyright (c) 2023 by Curtis Whitley
;

ZP_PTR              = $30   ; temporary pointer #1
ZP_PTR_LO           = $30
ZP_PTR_HI           = $31

ZP_TEMP_INDEX       = $32   ; temporary index for calculations
ZP_TEMP_INDEX_LO    = $32   ; temporary index for calculations (LO)
ZP_TEMP_INDEX_HI    = $33   ; temporary index for calculations (HI)

ZP_CHAR_ATTR        = $34   ; palette offset, v-flip, h-flip
ZP_TEMP_COL      = $35   ; current column for computations

ZP_PARAM_ROW        = $40   ; character row (line) index
ZP_PARAM_COL        = $41   ; character column index
ZP_PARAM_CNT        = $42   ; number of characters to show
ZP_PARAM_GROUP      = $43   ; which character tile group (0 or 1)
ZP_PARAM_CODE       = $44   ; ASCII character code value or special code
ZP_PARAM_PALETTE    = $45   ; color palette offset (0..15)
ZP_PARAM_STYLE      = $46   ; border style number
ZP_PARAM_WIDTH      = $47   ; width of box, including border
ZP_PARAM_HEIGHT     = $48   ; height of box, including border

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
