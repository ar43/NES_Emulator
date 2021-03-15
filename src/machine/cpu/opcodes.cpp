#include "opcodes.h"
#include "../../logger/logger.h"
#include "../../utility/utility.h"
#include "../bus/bus.h"


namespace opcodes
{
	void LDA(Cpu* cpu, Bus *bus, int value, AddressingMode mode)
	{
		int new_val = value;
		if(mode != AddressingMode::IMMEDIATE)
			new_val = bus->ReadCPU(value);

		auto p = cpu->registers[(size_t)RegId::P];
		cpu->registers[static_cast<size_t>(RegId::A)]->set(new_val);

		p->set_flag(flags::Flags::Z, new_val == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(new_val,7));
	}

	void LDX(Cpu* cpu, Bus *bus, int value, AddressingMode mode)
	{
		int new_val = value;
		if(mode != AddressingMode::IMMEDIATE)
			new_val = bus->ReadCPU(value);

		auto p = cpu->registers[(size_t)RegId::P];
		cpu->registers[static_cast<size_t>(RegId::X)]->set(new_val);

		p->set_flag(flags::Flags::Z, new_val == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(new_val,7));
	}

	void LDY(Cpu* cpu, Bus *bus, int value, AddressingMode mode)
	{
		int new_val = value;
		if(mode != AddressingMode::IMMEDIATE)
			new_val = bus->ReadCPU(value);

		auto p = cpu->registers[(size_t)RegId::P];
		cpu->registers[static_cast<size_t>(RegId::Y)]->set(new_val);

		p->set_flag(flags::Flags::Z, new_val == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(new_val,7));
	}

	void STA(Cpu* cpu, Bus *bus, int value, AddressingMode mode)
	{
		size_t addr = static_cast<size_t>(value);
		bus->WriteCPU(addr, cpu->registers[static_cast<size_t>(RegId::A)]->get());
	}

	void STX(Cpu* cpu, Bus *bus, int value, AddressingMode mode)
	{
		size_t addr = static_cast<size_t>(value);
		bus->WriteCPU(addr, cpu->registers[static_cast<size_t>(RegId::X)]->get());
	}

	void STY(Cpu* cpu, Bus *bus, int value, AddressingMode mode)
	{
		size_t addr = static_cast<size_t>(value);
		bus->WriteCPU(addr, cpu->registers[static_cast<size_t>(RegId::Y)]->get());
	}

	void TAX(Cpu* cpu, Bus *bus, int value, AddressingMode mode)
	{
		auto a = cpu->registers[(size_t)RegId::A];
		auto x = cpu->registers[(size_t)RegId::X];
		auto p = cpu->registers[(size_t)RegId::P];

		x->set(a->get());

		p->set_flag(flags::Flags::Z, x->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(x->get(),7));
	}

	void TAY(Cpu* cpu, Bus *bus, int value, AddressingMode mode)
	{
		auto a = cpu->registers[(size_t)RegId::A];
		auto y = cpu->registers[(size_t)RegId::Y];
		auto p = cpu->registers[(size_t)RegId::P];
		
		y->set(a->get());

		p->set_flag(flags::Flags::Z, y->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(y->get(),7));
	}

	void TXA(Cpu* cpu, Bus *bus, int value, AddressingMode mode)
	{
		auto a = cpu->registers[(size_t)RegId::A];
		auto x = cpu->registers[(size_t)RegId::X];
		auto p = cpu->registers[(size_t)RegId::P];

		a->set(x->get());

		p->set_flag(flags::Flags::Z, a->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(a->get(),7));
	}

	void TYA(Cpu* cpu, Bus *bus, int value, AddressingMode mode)
	{
		auto a = cpu->registers[(size_t)RegId::A];
		auto y = cpu->registers[(size_t)RegId::Y];
		auto p = cpu->registers[(size_t)RegId::P];

		a->set(y->get());

		p->set_flag(flags::Flags::Z, a->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(a->get(),7));
	}

	void TSX(Cpu* cpu, Bus *bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		auto sp = cpu->registers[(size_t)RegId::SP]->get();
		cpu->registers[(size_t)RegId::X]->set(sp);

		p->set_flag(flags::Flags::Z, sp == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(sp,7));
	}

	void TXS(Cpu* cpu, Bus *bus, int value, AddressingMode mode)
	{
		auto x = cpu->registers[(size_t)RegId::X]->get();
		cpu->registers[(size_t)RegId::SP]->set(x);
	}

	void PHA(Cpu* cpu, Bus *bus, int value, AddressingMode mode)
	{
		auto addr = STACK_OFFSET;
		auto sp = cpu->registers[(size_t)RegId::SP];
		auto a = cpu->registers[(size_t)RegId::A];

		addr += sp->get();
		bus->WriteCPU(addr, a->get());

		sp->decrement();
	}

	void PHP(Cpu* cpu, Bus *bus, int value, AddressingMode mode)
	{
		auto addr = STACK_OFFSET;
		auto sp = cpu->registers[(size_t)RegId::SP];
		auto p = cpu->registers[(size_t)RegId::P];

		addr += sp->get();
		int to_write = p->get();
		utility::SetBit(&to_write, 5, 1);
		utility::SetBit(&to_write, 4, 1);
		bus->WriteCPU(addr, to_write);

		sp->decrement();
	}

	void PLA(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto addr = STACK_OFFSET;
		auto sp = cpu->registers[(size_t)RegId::SP];
		auto a = cpu->registers[(size_t)RegId::A];
		auto p = cpu->registers[(size_t)RegId::P];

		sp->increment();
		addr += sp->get();

		a->set(bus->ReadCPU(addr));

		p->set_flag(flags::Flags::Z,a->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(a->get(),7));
	}

	void PLP(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto addr = STACK_OFFSET;
		auto sp = cpu->registers[(size_t)RegId::SP];
		auto p = cpu->registers[(size_t)RegId::P];

		sp->increment();
		addr += sp->get();
		bool save_4 = p->get_flag(flags::Flags::UNUSED);
		bool save_5 = p->get_flag(flags::Flags::B);
		p->set(bus->ReadCPU(addr));
		p->set_flag(flags::Flags::UNUSED, save_4);
		p->set_flag(flags::Flags::B, save_5);
	}

	void AND(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = bus->ReadCPU(value);

		auto p = cpu->registers[(size_t)RegId::P];
		auto a = cpu->registers[(size_t)RegId::A];

		int new_a = (a->get() & value);
		a->set(new_a);

		p->set_flag(flags::Flags::Z, a->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(a->get(),7));

	}

	void EOR(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = bus->ReadCPU(value);

		auto p = cpu->registers[(size_t)RegId::P];
		auto a = cpu->registers[(size_t)RegId::A];

		int new_a = (a->get() ^ value);
		a->set(new_a);

		p->set_flag(flags::Flags::Z, a->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(a->get(),7));

	}

	void ORA(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = bus->ReadCPU(value);

		auto p = cpu->registers[(size_t)RegId::P];
		auto a = cpu->registers[(size_t)RegId::A];

		int new_a = (a->get() | value);
		a->set(new_a);

		p->set_flag(flags::Flags::Z, a->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(a->get(),7));

	}

	void BIT(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		value = bus->ReadCPU(value);

		auto p = cpu->registers[(size_t)RegId::P];
		auto a = cpu->registers[(size_t)RegId::A];

		int test = (a->get() & value);
		p->set_flag(flags::Flags::Z, test == 0);
		p->set_flag(flags::Flags::V, utility::IsBitSet(value,6));
		p->set_flag(flags::Flags::N, utility::IsBitSet(value,7));

	}

	void ADC(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = bus->ReadCPU(value);

		auto p = cpu->registers[(size_t)RegId::P];
		auto a = cpu->registers[(size_t)RegId::A];
		bool a_negative = false;
		bool value_negative = false;
		bool result_negative = false;

		if (a->get() >= 0x80)
			a_negative = true;

		if (value >= 0x80)
			value_negative = true;

		int c = 0;
		if (p->get_flag(flags::Flags::C))
			c = 1;

		int result = a->get() + value + c;

		if (result > 0xFF)
		{
			p->set_flag(flags::Flags::C);
			result = result % 0x100;
		}
		else
		{
			p->reset_flag(flags::Flags::C);
		}

		if (result >= 0x80)
			result_negative = true;

		p->set_flag(flags::Flags::V, a_negative && value_negative && !result_negative || !a_negative && !value_negative && result_negative);

		a->set(result);

		p->set_flag(flags::Flags::N, utility::IsBitSet(a->get(),7));
		p->set_flag(flags::Flags::Z, a->get() == 0);

	}

	void SBC(Cpu* cpu, Bus* bus, int value, AddressingMode mode) //who knows if this works
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = bus->ReadCPU(value);

		auto p = cpu->registers[(size_t)RegId::P];
		auto a = cpu->registers[(size_t)RegId::A];
		bool a_negative = false;
		bool value_negative = false;
		bool result_negative = false;

		if (a->get() >= 0x80)
			a_negative = true;

		if (value < 0x80)
			value_negative = true;

		int c = 0;
		if (p->get_flag(flags::Flags::C))
			c = 1;

		int result = a->get() - value - (1 - c);

		if (result > 0xFF || result < 0)
		{
			p->reset_flag(flags::Flags::C);

			if (result > 0)
				result = result % 0x100;
			else if(result < 0)
				result += 0x100;
		}
		else
		{
			p->set_flag(flags::Flags::C);
		}

		if (result >= 0x80)
			result_negative = true;

		p->set_flag(flags::Flags::V, a_negative && value_negative && !result_negative || !a_negative && !value_negative && result_negative);

		a->set(result);

		p->set_flag(flags::Flags::N, utility::IsBitSet(a->get(),7));
		p->set_flag(flags::Flags::Z, a->get() == 0);

	}

	void CMP(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = bus->ReadCPU(value);

		auto p = cpu->registers[(size_t)RegId::P];
		auto a = cpu->registers[(size_t)RegId::A];

		p->set_flag(flags::Flags::C, a->get() >= value);
		p->set_flag(flags::Flags::Z, a->get() == value);

		int zero_check = a->get() - value;

		if (zero_check < 0)
			zero_check += 0x100;
		else if (zero_check > 0xFF)
			zero_check = zero_check % 0x100;

		p->set_flag(flags::Flags::N, utility::IsBitSet(zero_check, 7));
	}

	void CPX(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = bus->ReadCPU(value);

		auto p = cpu->registers[(size_t)RegId::P];
		auto x = cpu->registers[(size_t)RegId::X];

		p->set_flag(flags::Flags::C, x->get() >= value);
		p->set_flag(flags::Flags::Z, x->get() == value);

		int zero_check = x->get() - value;

		if (zero_check < 0)
			zero_check += 0x100;
		else if (zero_check > 0xFF)
			zero_check = zero_check % 0x100;

		p->set_flag(flags::Flags::N, utility::IsBitSet(zero_check, 7));
	}

	void CPY(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = bus->ReadCPU(value);

		auto p = cpu->registers[(size_t)RegId::P];
		auto y = cpu->registers[(size_t)RegId::Y];

		p->set_flag(flags::Flags::C, y->get() >= value);
		p->set_flag(flags::Flags::Z, y->get() == value);

		int zero_check = y->get() - value;

		if (zero_check < 0)
			zero_check += 0x100;
		else if (zero_check > 0xFF)
			zero_check = zero_check % 0x100;

		p->set_flag(flags::Flags::N, utility::IsBitSet(zero_check, 7));
	}

	void INC(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];

		uint8_t m = bus->ReadCPU(value);
		m++;
		m = m % 0x100;
		bus->WriteCPU(value, m);

		p->set_flag(flags::Flags::Z, m == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(m,7));
	}

	void INX(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		auto x = cpu->registers[(size_t)RegId::X];

		x->increment();

		p->set_flag(flags::Flags::Z, x->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(x->get(),7));
	}

	void INY(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		auto y = cpu->registers[(size_t)RegId::Y];

		y->increment();

		p->set_flag(flags::Flags::Z, y->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(y->get(),7));
	}

	void DEC(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];

		int m = bus->ReadCPU(value);
		m--;
		if (m < 0)
			m += 0x100;
		bus->WriteCPU(value, m);

		p->set_flag(flags::Flags::Z, m == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(m,7));
	}

	void DEX(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		auto x = cpu->registers[(size_t)RegId::X];

		x->decrement();

		p->set_flag(flags::Flags::Z, x->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(x->get(),7));
	}

	void DEY(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		auto y = cpu->registers[(size_t)RegId::Y];

		y->decrement();

		p->set_flag(flags::Flags::Z, y->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(y->get(),7));
	}

	void ASL(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		auto a = cpu->registers[(size_t)RegId::A];
		int m = 0;

		if (mode == AddressingMode::ACCUMULATOR)
		{
			m = a->get();
			p->set_flag(flags::Flags::C, utility::IsBitSet(m, 7));
			m = (m << 1) & 0xFF;
			a->set(m);
		}
		else
		{
			int addr = value;
			m = bus->ReadCPU(addr);
			p->set_flag(flags::Flags::C, utility::IsBitSet(m, 7));
			m = (m << 1) & 0xFF;
			bus->WriteCPU(addr,m);
		}

		p->set_flag(flags::Flags::Z, m == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(m, 7));
	}

	void LSR(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		auto a = cpu->registers[(size_t)RegId::A];
		int m = 0;

		if (mode == AddressingMode::ACCUMULATOR)
		{
			m = a->get();
			p->set_flag(flags::Flags::C, utility::IsBitSet(m, 0));
			m = (m >> 1) & 0xFF;
			a->set(m);
		}
		else
		{
			int addr = value;
			m = bus->ReadCPU(addr);
			p->set_flag(flags::Flags::C, utility::IsBitSet(m, 0));
			m = (m >> 1) & 0xFF;
			bus->WriteCPU(addr,m);
		}

		p->set_flag(flags::Flags::Z, m == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(m, 7));
	}

	void ROL(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		auto a = cpu->registers[(size_t)RegId::A];
		int m = 0;

		if (mode == AddressingMode::ACCUMULATOR)
		{
			m = a->get();
			bool old_carry = p->get_flag(flags::Flags::C);
			p->set_flag(flags::Flags::C, utility::IsBitSet(m, 7));
			m = (m << 1) & 0xFF;
			utility::SetBit(&m, 0, old_carry);
			a->set(m);
		}
		else
		{
			int addr = value;
			m = bus->ReadCPU(addr);
			bool old_carry = p->get_flag(flags::Flags::C);
			p->set_flag(flags::Flags::C, utility::IsBitSet(m, 7));
			m = (m << 1) & 0xFF;
			utility::SetBit(&m, 0, old_carry);
			bus->WriteCPU(addr,m);
		}

		p->set_flag(flags::Flags::Z, m == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(m, 7));
	}

	void ROR(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		auto a = cpu->registers[(size_t)RegId::A];
		int m = 0;

		if (mode == AddressingMode::ACCUMULATOR)
		{
			m = a->get();
			bool old_carry = p->get_flag(flags::Flags::C);
			p->set_flag(flags::Flags::C, utility::IsBitSet(m, 0));
			m = (m >> 1) & 0xFF;
			utility::SetBit(&m, 7, old_carry);
			a->set(m);
		}
		else
		{
			int addr = value;
			m = bus->ReadCPU(addr);
			bool old_carry = p->get_flag(flags::Flags::C);
			p->set_flag(flags::Flags::C, utility::IsBitSet(m, 0));
			m = (m >> 1) & 0xFF;
			utility::SetBit(&m, 7, old_carry);
			bus->WriteCPU(addr,m);
		}

		p->set_flag(flags::Flags::Z, m == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(m, 7));
	}

	void JMP(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto pc = cpu->registers[(size_t)RegId::PC];
		pc->set(value);
	}

	void JSR(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto pc = cpu->registers[(size_t)RegId::PC];
		auto sp = cpu->registers[(size_t)RegId::SP];
		int to_push = pc->get() - 1;
		assert(to_push >= 0 && to_push <= 0xFFFF);
		uint8_t ls = to_push & 0xFF;
		uint8_t ms = (to_push >> 8) & 0xFF;

		bus->WriteCPU(sp->get()+STACK_OFFSET, ms);
		sp->decrement();
		bus->WriteCPU(sp->get()+STACK_OFFSET, ls);
		sp->decrement();

		pc->set(value);
	}

	void RTS(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto pc = cpu->registers[(size_t)RegId::PC];
		auto sp = cpu->registers[(size_t)RegId::SP];
		
		sp->increment();
		uint8_t ls = bus->ReadCPU(sp->get()+STACK_OFFSET);
		sp->increment();
		uint8_t ms = bus->ReadCPU(sp->get()+STACK_OFFSET);

		int new_pc = (ms << 8) | ls;

		pc->set(new_pc + 1);

	}

	void BCC(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto pc = cpu->registers[(size_t)RegId::PC];
		auto p = cpu->registers[(size_t)RegId::P];

		if (!p->get_flag(flags::Flags::C))
		{
			int old_pc_page = pc->get() / 256;
			pc->set(pc->get() + value);
			int new_pc_page = pc->get() / 256;
			cpu->AddCycles(1);
			if (old_pc_page != new_pc_page)
				cpu->AddCycles(1);
		}
	}

	void BCS(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto pc = cpu->registers[(size_t)RegId::PC];
		auto p = cpu->registers[(size_t)RegId::P];

		if (p->get_flag(flags::Flags::C))
		{
			int old_pc_page = pc->get() / 256;
			pc->set(pc->get() + value);
			int new_pc_page = pc->get() / 256;
			cpu->AddCycles(1);
			if (old_pc_page != new_pc_page)
				cpu->AddCycles(1);
		}
	}

	void BEQ(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto pc = cpu->registers[(size_t)RegId::PC];
		auto p = cpu->registers[(size_t)RegId::P];

		if (p->get_flag(flags::Flags::Z))
		{
			int old_pc_page = pc->get() / 256;
			pc->set(pc->get() + value);
			int new_pc_page = pc->get() / 256;
			cpu->AddCycles(1);
			if (old_pc_page != new_pc_page)
				cpu->AddCycles(1);
		}
	}

	void BMI(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto pc = cpu->registers[(size_t)RegId::PC];
		auto p = cpu->registers[(size_t)RegId::P];

		if (p->get_flag(flags::Flags::N))
		{
			int old_pc_page = pc->get() / 256;
			pc->set(pc->get() + value);
			int new_pc_page = pc->get() / 256;
			cpu->AddCycles(1);
			if (old_pc_page != new_pc_page)
				cpu->AddCycles(1);
		}
	}

	void BNE(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto pc = cpu->registers[(size_t)RegId::PC];
		auto p = cpu->registers[(size_t)RegId::P];

		if (!p->get_flag(flags::Flags::Z))
		{
			int old_pc_page = pc->get() / 256;
			pc->set(pc->get() + value);
			int new_pc_page = pc->get() / 256;
			cpu->AddCycles(1);
			if (old_pc_page != new_pc_page)
				cpu->AddCycles(1);
		}
	}

	void BPL(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto pc = cpu->registers[(size_t)RegId::PC];
		auto p = cpu->registers[(size_t)RegId::P];

		if (!p->get_flag(flags::Flags::N))
		{
			int old_pc_page = pc->get() / 256;
			pc->set(pc->get() + value);
			int new_pc_page = pc->get() / 256;
			cpu->AddCycles(1);
			if (old_pc_page != new_pc_page)
				cpu->AddCycles(1);
		}
	}

	void BVC(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto pc = cpu->registers[(size_t)RegId::PC];
		auto p = cpu->registers[(size_t)RegId::P];

		if (!p->get_flag(flags::Flags::V))
		{
			int old_pc_page = pc->get() / 256;
			pc->set(pc->get() + value);
			int new_pc_page = pc->get() / 256;
			cpu->AddCycles(1);
			if (old_pc_page != new_pc_page)
				cpu->AddCycles(1);
		}
	}

	void BVS(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto pc = cpu->registers[(size_t)RegId::PC];
		auto p = cpu->registers[(size_t)RegId::P];

		if (p->get_flag(flags::Flags::V))
		{
			int old_pc_page = pc->get() / 256;
			pc->set(pc->get() + value);
			int new_pc_page = pc->get() / 256;
			cpu->AddCycles(1);
			if (old_pc_page != new_pc_page)
				cpu->AddCycles(1);
		}
	}

	void CLC(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		p->reset_flag(flags::Flags::C);
	}

	void CLD(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		p->reset_flag(flags::Flags::D);
	}

	void CLI(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		p->reset_flag(flags::Flags::I);
	}

	void CLV(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		p->reset_flag(flags::Flags::V);
	}

	void SEC(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		p->set_flag(flags::Flags::C);
	}

	void SED(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		p->set_flag(flags::Flags::D);
	}

	void SEI(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		p->set_flag(flags::Flags::I);
	}

	void BRK(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		auto sp = cpu->registers[(size_t)RegId::SP];
		auto pc = cpu->registers[(size_t)RegId::PC];
		p->set_flag(flags::Flags::B);
		pc->increment();
		uint8_t ms = (pc->get() >> 8) & 0xFF;
		uint8_t ls = pc->get() & 0xFF;
		
		//push pc+1 on stack
		bus->WriteCPU(sp->get()+STACK_OFFSET, ms);
		sp->decrement();
		bus->WriteCPU(sp->get()+STACK_OFFSET, ls);
		sp->decrement();

		//push flags on stack
		int to_write = p->get();
		utility::SetBit(&to_write, 5, 1);
		utility::SetBit(&to_write, 4, 1);
		bus->WriteCPU(sp->get()+STACK_OFFSET, to_write);
		sp->decrement();

		//jump to IRQ addr
		uint8_t new_ls = bus->ReadCPU(0xFFFE);
		uint8_t new_ms = bus->ReadCPU(0xFFFF);
		int new_pc = (new_ms << 8) | new_ls;
		pc->set(new_pc);
	}

	void RTI(Cpu* cpu, Bus* bus, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		auto sp = cpu->registers[(size_t)RegId::SP];
		auto pc = cpu->registers[(size_t)RegId::PC];
		
		//pull flags
		sp->increment();
		bool save_4 = p->get_flag(flags::Flags::UNUSED);
		bool save_5 = p->get_flag(flags::Flags::B);
		p->set(bus->ReadCPU(sp->get()+STACK_OFFSET));
		p->set_flag(flags::Flags::UNUSED, save_4);
		p->set_flag(flags::Flags::B, save_5);

		//pull pc
		sp->increment();
		uint8_t new_ls = bus->ReadCPU(sp->get()+STACK_OFFSET);
		sp->increment();
		uint8_t new_ms = bus->ReadCPU(sp->get()+STACK_OFFSET);

		int new_pc = (new_ms << 8) | new_ls;
		pc->set(new_pc);

	}

	void NOP(Cpu* cpu, Bus* bus, int value, AddressingMode mode) {}

}