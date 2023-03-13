# 106x60 Character Display for Commander X16
This demo program shows a 106x60 character text image, where each character is contained
within a 6x8 pixel cell. Most characters fit within a 5x7 pixel portion of a cell, but
some characters, especially block (drawing) characters, take up more of the cell.
<br>
<br>
<b>Features:</b>
<br>
* 106 text columns<br>
* 60 text rows<br>
* 96 ASCII characters, including blank and empty (transparent)<br>
* 73 block characters<br>
* 7 different line styles<br>
* most line styles can be joined (intersected)<br>
* supports using color palettes (color indexes 0, 1, 2, and 3, only)<br>
* character tiles support using color indexes 1 (background) and 2 (foreground), or 2 (background) and 3 (foreground)<br>
* in a particular palette offset, if color 1 equals color 3, then that palette offset can be used to display inverse characters using the same 2 colors as the normal (non-inverse) characters<br>
* supports defining "windows", which are boxes that hold text<br>
* windows automatically scroll if the lower right character cell is written using the write_to_window routine (it is possible to write without scrolling by using other functions)<br>
* characters can be written anywhere on the screen, even whether windows are defined or not<br>
* the window fill character can be any character, not just a blank character<br>
<br>
<br>
<b>Caviats:</b>
<br>
* does not support reverse scrolling or sideways scrolling (not impossible, just not coded)<br>
* does not support saving and restoring screen areas (e.g., for popup windows)<br>
* uses both VERA layers (L0 and L1), because characters are 6 pixels wide, and text tiles cannot be smaller than 8 pixels wide<br>
<br>
<br>
<b>Line Styles:</b>
<br>
* single-pixel vertical and single-pixel horizontal<br>
* single-pixel vertical and double-pixel horizontal<br>
* single-pixel vertical and triple-pixel horizontal<br>
* double-pixel vertical and single-pixel horizontal<br>
* double-pixel vertical and double-pixel horizontal<br>
* triple-pixel vertical and single-pixel horizontal<br>
* triple-pixel vertical and triple-pixel horizontal<br>

