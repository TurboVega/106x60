; Zero Page custom variables

ZP_PTR              = $30    ; temporary pointer #1
ZP_PTR_LO           = $30
ZP_PTR_HI           = $31

ZP_TEMP_INDEX       = $32    ; temporary index for calculations
ZP_TEMP_INDEX_LO    = $32    ; temporary index for calculations (LO)
ZP_TEMP_INDEX_HI    = $33    ; temporary index for calculations (HI)

ZP_CHAR_ROW         = $40    ; character row (line) index
ZP_CHAR_COL         = $41    ; character column index
ZP_CHAR_CNT         = $42    ; number of characters to show
ZP_CHAR_CODE        = $43    ; ASCII character code value or special code
ZP_CHAR_ATTR        = $44    ; palette offset, v-flip, h-flip

ZP_CURRENT_COL      = $45    ; current column for computations

ZP_TILE_INDEX_0     = $50    ; tile index (LO) for 0 pixel offset image
ZP_TILE_ATTR_0      = $51    ; tile attributes for 0 pixel offset image
ZP_TILE_INDEX_4A    = $52    ; tile index (LO) for 4 pixel offset image (part A)
ZP_TILE_ATTR_4A     = $53    ; tile attributes for 4 pixel offset image (part A)
ZP_TILE_INDEX_4B    = $54    ; tile index (LO) for 4 pixel offset image (part B)
ZP_TILE_ATTR_4B     = $55    ; tile attributes for 4 pixel offset image (part B)

ZP_TILE_ADDR_0_LO   = $60
ZP_TILE_ADDR_0_HI   = $61
ZP_TILE_ADDR_1_LO   = $62
ZP_TILE_ADDR_1_HI   = $63
