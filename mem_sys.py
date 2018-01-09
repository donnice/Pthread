# Memory system pseudocode from GT-Refresher-Advanced OS

def load(virtual_addr):
	try:
		physical_addr = translate_addr(virtual_addr)
	except page_default:
		handle_page_fault(virtual_addr)
		return load(virtual_addr)
	
	if is_in_cache(physical_addr):
		return read_from_cache(physical_addr)
	else:
		return read_from_memory(physical_addr)

def translate_addr(virtual_addr):
	#Table Lookaside Buffer, cache for page table entry
	if is_in_tlb(virtual_addr):
		return read_from_tlb(virtual_addr)
	elif is_access_violation(virtual_addr):
		raise access_violation
	elif is_present(virtual_addr):
		return read_from_page_table(virtual_addr)
	else:
		raise page_fault
