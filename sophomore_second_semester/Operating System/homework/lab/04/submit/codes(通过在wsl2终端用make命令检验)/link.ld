SECTIONS { 
  . = 0x8000; 
  .text : {*(.text)}   
  . = 0x81CC;   
  .rodata : {*(.data)}   
}
