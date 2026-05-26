#pragma once

#include "types.hh"


namespace Chip {
class HAL {
public:
	static constexpr u32 generate_bitmask(const u32 n)
	{
		return (n >= 32) ? 0xFFFFFFFFu : ((1u << n) - 1u);
	}

	static u32 read_register(volatile const u32* const reg);
	static void set_register(volatile u32* const reg, u32 val);
	static void clear_register(volatile u32* const reg, u32 val);
	static void update_register(volatile u32* const reg, u32 mask, u32 val);
	static u32 read_field(volatile const u32* const reg, uint16_t position, uint16_t width);
	// static void set_field(volatile u32 *const reg, uint16_t position, uint16_t width, u32
	// val); static void clear_field(volatile u32 *const reg, uint16_t position, uint16_t
	// width);
private:
};
}  // namespace Chip

