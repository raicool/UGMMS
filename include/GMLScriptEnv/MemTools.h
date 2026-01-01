
#include <string>
#include <vector>
#include <windows.h>

namespace MemTools 
{
	void* scan(const std::vector<uint8_t>& bytes_to_find);
	void* readPointer(void* start);
	void* scanLocal(const std::vector<uint8_t>& bytes_to_find, void* begin, int count);
	void* absolute_address(void* source, void* relative_addr, uint8_t instr_size);
}