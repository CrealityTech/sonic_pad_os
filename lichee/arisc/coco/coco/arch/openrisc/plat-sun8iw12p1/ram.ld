MEMORY
{
	vectors : ORIGIN = 0x00000000, LENGTH = 0x00004000
	/* 112k ram area for arisc code */
	paras	: ORIGIN = 0x00004000, LENGTH = 0x000003f0
	ram     : ORIGIN = 0x000043f0, LENGTH = 0x00017c10
	/* 16k dram area, 12K for code, 4K for back data */
	dram    : ORIGIN = 0x48100000, LENGTH = 0x00003000
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
		*coco*(.text)
	}  > ram

	rodata_vma = ALIGN(4);
	.rodata ALIGN(4): AT(rodata_vma)
	{
		*coco*(.rodata);
		*coco*(.rodata.*)
	} > ram

	shbss_vma = ALIGN(4);
	.shbss ALIGN(4): AT(shbss_vma)
	{
		*coco*(.shbss)
	} > ram

	data_vma = ALIGN(4);
	.data ALIGN(4): AT(data_vma)
	{
		*coco*(.data)
	} > ram

	.bss SIZEOF(.data) + ADDR(.data) :
	{
		bss_start_addr = ALIGN(0x8);
		*coco*(.bss)
		*coco*(COMMON)
		bss_end_addr = ALIGN(0x8);
	} > ram

	.stack :
	{
		stack_limit_addr = ALIGN(0x8);
		*(.stack)
		stack_base_addr = ALIGN(0x8);
	} > ram

	.text1 : AT(0x1c000)
	{
		mem_start_addr = ALIGN(0x8);
		*bytb*(.text)
		*bytb*(.shdata)
		mem_end_addr = ALIGN(0x8);
	} > dram

	rodata_vma1 = ALIGN(4);
	.rodata1 ALIGN(4):
	{
		*bytb*(.rodata.*)
	} > dram


}
