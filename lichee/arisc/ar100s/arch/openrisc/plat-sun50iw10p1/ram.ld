MEMORY
{
	vectors : ORIGIN = 0x00000000, LENGTH = 0x00004000
	/* 64k ram area for arisc code */
	paras	: ORIGIN = 0x00004000, LENGTH = 0x000000f0
	ram     : ORIGIN = 0x000040f0, LENGTH = 0x0000ff10
	/* 256k dram area, for dts data */
	dram    : ORIGIN = 0x48100000, LENGTH = 0x00040000
}

SECTIONS
{
	.vectors ALIGN(4):
	{
		*(.vectors)
	} > vectors

	dts_paras :
	{
		paras_start_addr = .;
		KEEP(*(dts_paras))
		paras_end_addr = .;
	} > paras

	.text :
	{
		*arch*(.text)
		*driver*(.text)
		*library*(.text)
		*system*(.text)
		*service*(.text)
	}  > ram

	rodata_vma = ALIGN(4);
	.rodata ALIGN(4): AT(rodata_vma)
	{
		*arch*(.rodata.*)
		*driver*(.rodata.*)
		*library*(.rodata.*)
		*system*(.rodata.*)
		*service*(.rodata.*)
		*(.rodata.*)
	} > ram

	shbss_vma = ALIGN(4);
	.shbss ALIGN(4): AT(shbss_vma)
	{
		*(.shbss)
	} > ram

	data_vma = ALIGN(4);
	.data ALIGN(4): AT(data_vma)
	{
		*(.data)
	} > ram

	.bss SIZEOF(.data) + ADDR(.data) :
	{
		bss_start_addr = ALIGN(0x8);
		*(.bss)
		*(COMMON)
		bss_end_addr = ALIGN(0x8);
	} > ram

	.stack :
	{
		stack_limit_addr = ALIGN(0x8);
		*(.stack)
		stack_base_addr = ALIGN(0x8);
	} > ram

	.text5 : AT(0x14000)
	{
		LONG(0)
	} > dram
}
