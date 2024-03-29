.ifndef X16_INC
X16_INC  = 1

.ifndef __CX16__
__CX16__ = 1
.endif

; X16 addresses, values, and macros
; Accurate for ROM release R38

SD_DEVICE   = 8 ; also host file system on emulator if no SD image mounted
DISK_DEVICE = SD_DEVICE ; change if to be loaded off IEC device


; RAM Addresses

; Kernal Registers
r0                = $02
r0L               = r0
r0H               = r0+1
r1                = $04
r1L               = r1
r1H               = r1+1
r2                = $06
r2L               = r2
r2H               = r2+1
r3                = $08
r3L               = r3
r3H               = r3+1
r4                = $0A
r4L               = r4
r4H               = r4+1
r5                = $0C
r5L               = r5
r5H               = r5+1
r6                = $0E
r6L               = r6
r6H               = r6+1
r7                = $10
r7L               = r7
r7H               = r7+1
r8                = $12
r8L               = r8
r8H               = r8+1
r9                = $14
r9L               = r9
r9H               = r9+1
r10               = $16
r10L              = r10
r10H              = r10+1
r11               = $18
r11L              = r11
r11H              = r11+1
r12               = $1A
r12L              = r12
r12H              = r12+1
r13               = $1C
r13L              = r13
r13H              = r13+1
r14               = $1E
r14L              = r14
r14H              = r14+1
r15               = $20
r15L              = r15
r15H              = r15+1

; I/O Registers
VERA_addr_low     = $9F20
VERA_addr_high    = $9F21
VERA_addr_bank    = $9F22
VERA_data0        = $9F23
VERA_data1        = $9F24
VERA_ctrl         = $9F25
VERA_ien          = $9F26
VERA_isr          = $9F27
VERA_irqline_l    = $9F28
VERA_dc_video     = $9F29 ; VERA_ctrl(1) (DCSEL) = 0
VERA_dc_hscale    = $9F2A ; VERA_ctrl(1) (DCSEL) = 0
VERA_dc_vscale    = $9F2B ; VERA_ctrl(1) (DCSEL) = 0
VERA_dc_border    = $9F2C ; VERA_ctrl(1) (DCSEL) = 0
VERA_dc_hstart    = $9F29 ; VERA_ctrl(1) (DCSEL) = 1
VERA_dc_hstop     = $9F2A ; VERA_ctrl(1) (DCSEL) = 1
VERA_dc_vsstart   = $9F2B ; VERA_ctrl(1) (DCSEL) = 1
VERA_dc_vstop     = $9F2C ; VERA_ctrl(1) (DCSEL) = 1
VERA_L0_config    = $9F2D
VERA_L0_mapbase   = $9F2E
VERA_L0_tilebase  = $9F2F
VERA_L0_hscroll_l = $9F30
VERA_L0_hscroll_h = $9F31
VERA_L0_vscroll_l = $9F32
VERA_L0_vscroll_h = $9F33
VERA_L1_config    = $9F34
VERA_L1_mapbase   = $9F35
VERA_L1_tilebase  = $9F36
VERA_L1_hscroll_l = $9F37
VERA_L1_hscroll_h = $9F38
VERA_L1_vscroll_l = $9F39
VERA_L1_vscroll_h = $9F3A
VERA_audio_ctrl   = $9F3B
VERA_audio_rate   = $9F3C
VERA_audio_data   = $9F3D
VERA_spi_data     = $9F3E
VERA_spi_ctrl     = $9F3F

ROM_BANK          = $9F60
RAM_BANK          = $9F61

YM_reg            = $9FE0
YM_data           = $9FE1

; VERA Registers values
VSYNC_BIT         = $01
SPRCOL_BIT        = $04
DISPLAY_SCALE     = $80 ; 1X zoom
ADDRSEL           = $01
DCSEL             = $02

ENABLE_CURRENT_FIELD    = $80
ENABLE_SPRITES          = $40
ENABLE_LAYER_1          = $20
ENABLE_LAYER_0          = $10

OUTPUT_MODE_DISABLED    = $00
OUTPUT_MODE_VGA         = $01
OUTPUT_MODE_NTSC        = $02
OUTPUT_MODE_RGBI        = $03

SPRITE_MODE_4_BPP = $00
SPRITE_MODE_8_BPP = $80

SPRITE_COLL_MASK_NONE   =   $00
SPRITE_COLL_MASK_0      =   $10
SPRITE_COLL_MASK_1      =   $20
SPRITE_COLL_MASK_2      =   $40
SPRITE_COLL_MASK_3      =   $80

SPRITE_ZDEPTH_DISABLED  =   $00
SPRITE_ZDEPTH_ABOVE_BG  =   $04
SPRITE_ZDEPTH_ABOVE_L0  =   $08
SPRITE_ZDEPTH_ABOVE_L1  =   $0C

SPRITE_V_FLIP           =   $02
SPRITE_H_FLIP           =   $01

SPRITE_HEIGHT_08_PIXELS =   $00
SPRITE_HEIGHT_16_PIXELS =   $40
SPRITE_HEIGHT_32_PIXELS =   $80
SPRITE_HEIGHT_64_PIXELS =   $C0

SPRITE_WIDTH_08_PIXELS  =   $00
SPRITE_WIDTH_16_PIXELS  =   $10
SPRITE_WIDTH_32_PIXELS  =   $20
SPRITE_WIDTH_64_PIXELS  =   $30

; Sprite data structure
SPRITE_ADDR_LO  =   0       ; Address(12:5)@(7:0)
SPRITE_ADDR_HI  =   1       ; Mode@(7), Address(16:3)@(3:0)
SPRITE_X_LO     =   2       ; X(7:0)
SPRITE_X_HI     =   3       ; X(9:8)@(1:0)
SPRITE_Y_LO     =   4       ; Y(7:0)
SPRITE_Y_HI     =   5       ; Y(9:8)@(1:0)
SPRITE_CTRL     =   6       ; Collision mask@(7:4), Z-depth@(3:2), V-flip@(1), H-flip@(0)
SPRITE_HEIGHT   =   7       ; Height(7:6), Width@(5:4), Palette offset@(3:0)
SPRITE_ATTR_SIZE =  8

; Tile related definitions
TILE_MAP_WIDTH_032  = $00   ; 32 tiles across
TILE_MAP_WIDTH_064  = $10   ; 64 tiles across
TILE_MAP_WIDTH_128  = $20   ; 128 tiles across
TILE_MAP_WIDTH_256  = $30   ; 256 tiles across
TILE_MAP_HEIGHT_032 = $00   ; 32 tiles down
TILE_MAP_HEIGHT_064 = $40   ; 64 tiles down
TILE_MAP_HEIGHT_128 = $80   ; 128 tiles down
TILE_MAP_HEIGHT_256 = $C0   ; 256 tiles down

TILE_WIDTH_08_PIXELS = $00  ; 8 pixels wide
TILE_WIDTH_16_PIXELS = $01  ; 16 pixels wide
TILE_HEIGHT_08_PIXELS = $00 ; 8 pixels high
TILE_HEIGHT_16_PIXELS = $02 ; 16 pixels high

BITMAP_MODE          = $04  ; layer bitmap mode
TILE_MODE            = $00  ; layer tile mode

LAYER_COLOR_DEPTH_1_BPP = $00 ; 1 bit per pixel (2 colors)
LAYER_COLOR_DEPTH_2_BPP = $01 ; 2 bits per pixel (4 colors)
LAYER_COLOR_DEPTH_4_BPP = $02 ; 4 bits per pixel (16 colors)
LAYER_COLOR_DEPTH_8_BPP = $03 ; 8 bits per pixel (256 colors)

; Emulator Registers
GIF_ctrl          = $9FB5

; ROM Banks
KERNAL_ROM_BANK   = 0
KEYBOARD_ROM_BANK = 1
CBDOS_ROM_BANK    = 2
GEOS_ROM_BANK     = 3
BASIC_ROM_BANK    = 4
MONITOR_ROM_BANK  = 5
CHARSET_ROM_BANK  = 6

; Banked Addresses
RAM_WIN           = $A000
RAM_WIN_SIZE      = $2000
ROM_WIN           = $C000

; Kernal Subroutines
ENTROPY_GET                   := $FECF
CONSOLE_set_paging_message    := $FED5
CONSOLE_put_image             := $FED8
CONSOLE_init                  := $FEDB
CONSOLE_put_char              := $FEDE
CONSOLE_get_char              := $FEE1
MEMORY_FILL                   := $FEE4
MEMORY_COPY                   := $FEE7
MEMORY_CRC                    := $FEEA
MEMORY_DECOMPRESS             := $FEED
SPRITE_set_image              := $FEF0
SPRITE_set_position           := $FEF3
FB_init                       := $FEF6
FB_get_info                   := $FEF9
FB_set_palette                := $FEFC
FB_cursor_position            := $FEFF
FB_cursor_next_line           := $FF02
FB_get_pixel                  := $FF05
FB_get_pixels                 := $FF08
FB_set_pixel                  := $FF0B
FB_set_pixels                 := $FF0E
FB_set_8_pixels               := $FF11
FB_set_8_pixels_opaque        := $FF14
FB_fill_pixels                := $FF17
FB_filter_pixels              := $FF1A
FB_move_pixels                := $FF1D
GRAPH_init                    := $FF20
GRAPH_clear                   := $FF23
GRAPH_set_window              := $FF26
GRAPH_set_colors              := $FF29
GRAPH_draw_line               := $FF2C
GRAPH_draw_rect               := $FF2F
GRAPH_move_rect               := $FF32
GRAPH_draw_oval               := $FF35
GRAPH_draw_image              := $FF38
GRAPH_set_font                := $FF3B
GRAPH_get_char_size           := $FF3E
GRAPH_put_char                := $FF41
MONITOR                       := $FF44
ENTER_BASIC                   := $FF47
CLOSE_ALL                     := $FF4A
CLOCK_SET_DATE_TIME           := $FF4D
CLOCK_GET_DATE_TIME           := $FF50
JOYSTICK_SCAN                 := $FF53
JOYSTICK_GET                  := $FF56
SCREEN_SET_MODE               := $FF5F
SCREEN_SET_CHARSET            := $FF62
PFKEY                         := $FF65
MOUSE_CONFIG                  := $FF68
MOUSE_GET                     := $FF6B
JSRFAR_kernal_addr            := $FF6E
MOUSE_SCAN                    := $FF71
FETCH                         := $FF74
STASH                         := $FF77
CMPARE                        := $FF7A
PRIMM                         := $FF7D

SCINIT                        := $FF81
IOINIT                        := $FF84
RAMTAS                        := $FF87
RESTOR                        := $FF8A
LKUPLA                        := $FF8D
SETMSG                        := $FF90
SECOND                        := $FF93
TKSA                          := $FF96
MEMTOP                        := $FF99
MEMBOT                        := $FF9C
SETTMO                        := $FFA2
IECIN                         := $FFA5
IECOUT                        := $FFA8
UNTALK                        := $FFAB
UNLISTEN                      := $FFAE
LISTEN                        := $FFB1
TALK                          := $FFB4
READST                        := $FFB7
SETLFS                        := $FFBA
SETNAM                        := $FFBD
OPEN                          := $FFC0
CLOSE                         := $FFC3
CHKIN                         := $FFC6
CHKOUT                        := $FFC9
CLRCHN                        := $FFCC
CHRIN                         := $FFCF
BASIN                         := CHRIN ; alias
CHROUT                        := $FFD2
BSOUT                         := CHROUT ; alias
LOAD                          := $FFD5
SAVE                          := $FFD8
SETTIM                        := $FFDB
RDTIM                         := $FFDE
STOP                          := $FFE1
GETIN                         := $FFE4
CLALL                         := $FFE7
UDTIM                         := $FFEA
SCREEN                        := $FFED
PLOT                          := $FFF0
IOBASE                        := $FFF3

; BASIC Vectors
BASIC_PANIC       := $C000
BASIC_INIT        := $C003


; VRAM Addresses
VRAM_petscii   = $0F800
VRAM_psg       = $1F9C0
VRAM_palette   = $1FA00
VRAM_sprattr   = $1FC00

; RAM Vectors
IRQVec         = $0314
BRKVec         = $0316
NMIVec         = $0318
FETVec         = $03AF
;STAVec         = TBD
;CMPVec         = TBD


;   Macros

.macro JSRFAR far_sr_addr, bank
	jsr JSRFAR_kernal_addr
	.addr far_sr_addr
	.byte bank
.endmacro

.endif

; set data port 0 to start writing to VRAM address
; the address is a constant
.macro VERA_SET_ADDR0 vram_addr, stride
    .scope
        .ifnblank stride
            lda #((^vram_addr) | (stride << 4))
        .else
            lda #((^vram_addr) | $10)
        .endif

        stz VERA_ctrl
        sta VERA_addr_bank
        lda #>vram_addr
        sta VERA_addr_high
        lda #<vram_addr
        sta VERA_addr_low
    .endscope
.endmacro

; set data port 1 to start writing to VRAM address
; the address is a constant
.macro VERA_SET_ADDR1 vram_addr, stride
    .scope
        lda #ADDRSEL
        sta VERA_ctrl

        .ifnblank stride
            lda #((^vram_addr) | (stride << 4))
        .else
            lda #((^vram_addr) | $10)
        .endif

        sta VERA_addr_bank
        lda #>vram_addr
        sta VERA_addr_high
        lda #<vram_addr
        sta VERA_addr_low
    .endscope
.endmacro

; set data port 0 to start writing to VRAM address
; the address is in X (LO), Y (HI), and A (BANK)
.macro VERA_SET_ADDR0_XYA stride
    .scope
        .ifnblank stride
            ora #(stride << 4)
        .else
            ora #($10)
        .endif

        stz VERA_ctrl
        sta VERA_addr_bank
        sty VERA_addr_high
        stx VERA_addr_low
    .endscope
.endmacro

; set data port 1 to start writing to VRAM address
; the address is in X (LO), Y (HI), and A (BANK)
.macro VERA_SET_ADDR1_XYA stride
    .scope
        pha
        lda #ADDRSEL
        sta VERA_ctrl
        pla

        .ifnblank stride
            ora #(stride << 4)
        .else
            ora #($10)
        .endif

        sta VERA_addr_bank
        sty VERA_addr_high
        stx VERA_addr_low
    .endscope
.endmacro

.macro RAM2VRAM ram_addr, vram_addr, num_bytes
   .scope
      ; set data port 0 to start writing to VRAM address
      stz VERA_ctrl
      lda #($10 | ^vram_addr) ; stride = 1
      sta VERA_addr_bank
      lda #>vram_addr
      sta VERA_addr_high
      lda #<vram_addr
      sta VERA_addr_low
       ; ZP pointer = start of video data in CPU RAM
      lda #<ram_addr
      sta ZP_PTR_LO
      lda #>ram_addr
      sta ZP_PTR_HI
      ; use index pointers to compare with number of bytes to copy
      ldx #0
      ldy #0
   vram_loop:
      lda (ZP_PTR),y
      sta VERA_data0
      iny
      cpx #>num_bytes ; last page yet?
      beq check_end
      cpy #0
      bne vram_loop ; not on last page, Y non-zero
      inx ; next page
      inc ZP_PTR_HI
      bra vram_loop
   check_end:
      cpy #<num_bytes ; last byte of last page?
      bne vram_loop ; last page, before last byte
   .endscope
.endmacro

.macro FILLVRAM value, vram_addr, num_bytes
   .scope
      ; set data port 0 to start writing to VRAM address
      stz VERA_ctrl
      lda #($10 | ^vram_addr) ; stride = 1
      sta VERA_addr_bank
      lda #>vram_addr
      sta VERA_addr_high
      lda #<vram_addr
      sta VERA_addr_low
      ; use index pointers to compare with number of bytes to copy
      ldx #0
      ldy #0
   vram_loop:
      lda #value
      sta VERA_data0
      iny
      cpx #>num_bytes ; last page yet?
      beq check_end
      cpy #0
      bne vram_loop ; not on last page, Y non-zero
      inx ; next page
      inc ZP_PTR_HI
      bra vram_loop
   check_end:
      cpy #<num_bytes ; last byte of last page?
      bne vram_loop ; last page, before last byte
   .endscope
.endmacro
