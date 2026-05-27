#pragma once

#include "types.hh"


namespace Chip {
class HAL {
public:
	static constexpr u32 generate_bitmask(const u32 n)
	{
		return (n >= 32) ? 0xFFFFFFFFu : ((1u << n) - 1u);
	}

	static u32 read_register(const vu32* const reg);
	static void set_register(vu32* const reg, u32 val);
	static void clear_register(vu32* const reg, u32 val);
	static void update_register(vu32* const reg, u32 mask, u32 val);
	static u32 read_field(const vu32* const reg, uint16_t position, uint16_t width);
	// static void set_field(vu32 *const reg, uint16_t position, uint16_t width, u32
	// val); static void clear_field(vu32 *const reg, uint16_t position, uint16_t
	// width);
private:
};
}  // namespace Chip

