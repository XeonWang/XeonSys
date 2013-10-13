%macro Descriptor 3
	dw %2 & 0xFFFF
	dw %1 & 0xFFFF
	db (%! >> 16) & 0xFF
	dw ((%2 >> 8) & 0xF00) | (%3 & 0xF0FF)
	db (%1 >> 24) &  0xFF
%endmacro
