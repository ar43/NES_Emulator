#include "opcodes.h"
#include "../../logger/logger.h"
#include "../../utility/utility.h"


namespace opcodes
{
	void LDA(Cpu* cpu, Memory *mem, int value)
	{
		int new_val = mem->Read(value);
		cpu->registers[static_cast<size_t>(RegId::A)]->set(new_val);

		if (new_val == 0)
			cpu->registers[(size_t)RegId::P]->set_flag(flags::Flags::Z);

		if (utility::IsBitSet(new_val,7))
			cpu->registers[(size_t)RegId::P]->set_flag(flags::Flags::N);
	}

	void LDX(Cpu* cpu, Memory *mem, int value)
	{
		int new_val = mem->Read(value);
		cpu->registers[static_cast<size_t>(RegId::X)]->set(new_val);

		if (new_val == 0)
			cpu->registers[(size_t)RegId::P]->set_flag(flags::Flags::Z);

		if (utility::IsBitSet(new_val,7))
			cpu->registers[(size_t)RegId::P]->set_flag(flags::Flags::N);
	}

	void LDY(Cpu* cpu, Memory *mem, int value)
	{
		int new_val = mem->Read(value);
		cpu->registers[static_cast<size_t>(RegId::Y)]->set(new_val);

		if (new_val == 0)
			cpu->registers[(size_t)RegId::P]->set_flag(flags::Flags::Z);

		if (utility::IsBitSet(new_val,7))
			cpu->registers[(size_t)RegId::P]->set_flag(flags::Flags::N);
	}

	void STA(Cpu* cpu, Memory *mem, int value)
	{
		size_t addr = static_cast<size_t>(value);
		mem->Write(addr, cpu->registers[static_cast<size_t>(RegId::A)]->get());
	}

	void STX(Cpu* cpu, Memory *mem, int value)
	{
		size_t addr = static_cast<size_t>(value);
		mem->Write(addr, cpu->registers[static_cast<size_t>(RegId::X)]->get());
	}

	void STY(Cpu* cpu, Memory *mem, int value)
	{
		size_t addr = static_cast<size_t>(value);
		mem->Write(addr, cpu->registers[static_cast<size_t>(RegId::Y)]->get());
	}

	void TAX(Cpu* cpu, Memory *mem, int value)
	{
		auto a = cpu->registers[(size_t)RegId::A]->get();
		cpu->registers[(size_t)RegId::X]->set(a);

		if (a == 0)
			cpu->registers[(size_t)RegId::P]->set_flag(flags::Flags::Z);

		if (utility::IsBitSet(a,7))
			cpu->registers[(size_t)RegId::P]->set_flag(flags::Flags::N);
	}

	void TAY(Cpu* cpu, Memory *mem, int value)
	{
		auto a = cpu->registers[(size_t)RegId::A]->get();
		cpu->registers[(size_t)RegId::Y]->set(a);

		if (a == 0)
			cpu->registers[(size_t)RegId::P]->set_flag(flags::Flags::Z);

		if (utility::IsBitSet(a,7))
			cpu->registers[(size_t)RegId::P]->set_flag(flags::Flags::N);
	}

	void TXA(Cpu* cpu, Memory *mem, int value)
	{
		auto x = cpu->registers[(size_t)RegId::X]->get();
		cpu->registers[(size_t)RegId::A]->set(x);

		if (x == 0)
			cpu->registers[(size_t)RegId::P]->set_flag(flags::Flags::Z);

		if (utility::IsBitSet(x,7))
			cpu->registers[(size_t)RegId::P]->set_flag(flags::Flags::N);
	}

	void TYA(Cpu* cpu, Memory *mem, int value)
	{
		auto y = cpu->registers[(size_t)RegId::Y]->get();
		cpu->registers[(size_t)RegId::A]->set(y);

		if (y == 0)
			cpu->registers[(size_t)RegId::P]->set_flag(flags::Flags::Z);

		if (utility::IsBitSet(y,7))
			cpu->registers[(size_t)RegId::P]->set_flag(flags::Flags::N);
	}

	void TSX(Cpu* cpu, Memory *mem, int value)
	{
		auto sp = cpu->registers[(size_t)RegId::SP]->get();
		cpu->registers[(size_t)RegId::X]->set(sp);

		if (sp == 0)
			cpu->registers[(size_t)RegId::P]->set_flag(flags::Flags::Z);

		if (utility::IsBitSet(sp,7))
			cpu->registers[(size_t)RegId::P]->set_flag(flags::Flags::N);
	}

	void TXS(Cpu* cpu, Memory *mem, int value)
	{
		auto x = cpu->registers[(size_t)RegId::X]->get();
		cpu->registers[(size_t)RegId::SP]->set(x);
	}

	void PHA(Cpu* cpu, Memory *mem, int value)
	{
		auto addr = STACK_OFFSET;
		auto sp = cpu->registers[(size_t)RegId::SP];
		auto a = cpu->registers[(size_t)RegId::A];

		addr += sp->get();
		mem->Write(addr, a->get());

		sp->decrement();

		assert(sp->get() >= STACK_OFFSET && sp->get() <= STACK_OFFSET+0xFF);
	}

	void PHP(Cpu* cpu, Memory *mem, int value)
	{
		auto addr = STACK_OFFSET;
		auto sp = cpu->registers[(size_t)RegId::SP];
		auto p = cpu->registers[(size_t)RegId::P];

		addr += sp->get();
		mem->Write(addr, p->get());

		sp->decrement();

		assert(sp->get() >= STACK_OFFSET && sp->get() <= STACK_OFFSET+0xFF);
	}

	void PLA(Cpu* cpu, Memory* mem, int value)
	{
		auto addr = STACK_OFFSET;
		auto sp = cpu->registers[(size_t)RegId::SP];
		auto a = cpu->registers[(size_t)RegId::A];
		auto p = cpu->registers[(size_t)RegId::P];

		sp->increment();
		addr += sp->get();

		a->set(mem->Read(addr));

		if (a->get() == 0)
			p->set_flag(flags::Flags::Z);

		if (utility::IsBitSet(a->get(),7))
			p->set_flag(flags::Flags::Z);

		assert(sp->get() >= STACK_OFFSET && sp->get() <= STACK_OFFSET+0xFF);
	}

	void PLP(Cpu* cpu, Memory* mem, int value)
	{
		auto addr = STACK_OFFSET;
		auto sp = cpu->registers[(size_t)RegId::SP];
		auto p = cpu->registers[(size_t)RegId::P];

		sp->increment();
		addr += sp->get();

		p->set(mem->Read(addr));

		assert(sp->get() >= STACK_OFFSET && sp->get() <= STACK_OFFSET+0xFF);
	}

}