SECTIONS { 
  . = 0x8000; 
  .text : {*(.text)}   
  . = 0x8100;   
  .rodata : {*(.data)}   
}
