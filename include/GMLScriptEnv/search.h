
#include <string>
#include <vector>
#include <windows.h>

void* scan(const std::vector<uint8_t>& bytes_to_find);
void* scan_local(const std::vector<uint8_t>& bytes_to_find, void* begin, int count);

void* read_ptr(void* start);
void* absolute_address(void* source, void* relative_addr, uint8_t instr_size);