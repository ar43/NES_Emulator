#include "opcodes.h"
#include "../../logger/logger.h"
#include "../../utility/utility.h"


namespace opcodes
{
	void LDA(Cpu* cpu, Memory *mem, int value, AddressingMode mode)
	{
		int new_val = value;
		if(mode != AddressingMode::IMMEDIATE)
			new_val = mem->Read(value);

		auto p = cpu->registers[(size_t)RegId::P];
		cpu->registers[static_cast<size_t>(RegId::A)]->set(new_val);

		p->set_flag(flags::Flags::Z, new_val == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(new_val,7));
	}

	void LDX(Cpu* cpu, Memory *mem, int value, AddressingMode mode)
	{
		int new_val = value;
		if(mode != AddressingMode::IMMEDIATE)
			new_val = mem->Read(value);

		auto p = cpu->registers[(size_t)RegId::P];
		cpu->registers[static_cast<size_t>(RegId::X)]->set(new_val);

		p->set_flag(flags::Flags::Z, new_val == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(new_val,7));
	}

	void LDY(Cpu* cpu, Memory *mem, int value, AddressingMode mode)
	{
		int new_val = value;
		if(mode != AddressingMode::IMMEDIATE)
			new_val = mem->Read(value);

		auto p = cpu->registers[(size_t)RegId::P];
		cpu->registers[static_cast<size_t>(RegId::Y)]->set(new_val);

		p->set_flag(flags::Flags::Z, new_val == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(new_val,7));
	}

	void STA(Cpu* cpu, Memory *mem, int value, AddressingMode mode)
	{
		size_t addr = static_cast<size_t>(value);
		mem->Write(addr, cpu->registers[static_cast<size_t>(RegId::A)]->get());
	}

	void STX(Cpu* cpu, Memory *mem, int value, AddressingMode mode)
	{
		size_t addr = static_cast<size_t>(value);
		mem->Write(addr, cpu->registers[static_cast<size_t>(RegId::X)]->get());
	}

	void STY(Cpu* cpu, Memory *mem, int value, AddressingMode mode)
	{
		size_t addr = static_cast<size_t>(value);
		mem->Write(addr, cpu->registers[static_cast<size_t>(RegId::Y)]->get());
	}

	void TAX(Cpu* cpu, Memory *mem, int value, AddressingMode mode)
	{
		auto a = cpu->registers[(size_t)RegId::A];
		auto x = cpu->registers[(size_t)RegId::X];
		auto p = cpu->registers[(size_t)RegId::P];

		x->set(a->get());

		p->set_flag(flags::Flags::Z, x->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(x->get(),7));
	}

	void TAY(Cpu* cpu, Memory *mem, int value, AddressingMode mode)
	{
		auto a = cpu->registers[(size_t)RegId::A];
		auto y = cpu->registers[(size_t)RegId::Y];
		auto p = cpu->registers[(size_t)RegId::P];
		
		y->set(a->get());

		p->set_flag(flags::Flags::Z, y->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(y->get(),7));
	}

	void TXA(Cpu* cpu, Memory *mem, int value, AddressingMode mode)
	{
		auto a = cpu->registers[(size_t)RegId::A];
		auto x = cpu->registers[(size_t)RegId::X];
		auto p = cpu->registers[(size_t)RegId::P];

		a->set(x->get());

		p->set_flag(flags::Flags::Z, a->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(a->get(),7));
	}

	void TYA(Cpu* cpu, Memory *mem, int value, AddressingMode mode)
	{
		auto a = cpu->registers[(size_t)RegId::A];
		auto y = cpu->registers[(size_t)RegId::Y];
		auto p = cpu->registers[(size_t)RegId::P];

		a->set(y->get());

		p->set_flag(flags::Flags::Z, a->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(a->get(),7));
	}

	void TSX(Cpu* cpu, Memory *mem, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		auto sp = cpu->registers[(size_t)RegId::SP]->get();
		cpu->registers[(size_t)RegId::X]->set(sp);

		p->set_flag(flags::Flags::Z, sp == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(sp,7));
	}

	void TXS(Cpu* cpu, Memory *mem, int value, AddressingMode mode)
	{
		auto x = cpu->registers[(size_t)RegId::X]->get();
		cpu->registers[(size_t)RegId::SP]->set(x);
	}

	void PHA(Cpu* cpu, Memory *mem, int value, AddressingMode mode)
	{
		auto addr = STACK_OFFSET;
		auto sp = cpu->registers[(size_t)RegId::SP];
		auto a = cpu->registers[(size_t)RegId::A];

		addr += sp->get();
		mem->Write(addr, a->get());

		sp->decrement();

		assert(sp->get() >= STACK_OFFSET && sp->get() <= STACK_OFFSET+0xFF);
	}

	void PHP(Cpu* cpu, Memory *mem, int value, AddressingMode mode)
	{
		auto addr = STACK_OFFSET;
		auto sp = cpu->registers[(size_t)RegId::SP];
		auto p = cpu->registers[(size_t)RegId::P];

		addr += sp->get();
		mem->Write(addr, p->get());

		sp->decrement();

		assert(sp->get() >= STACK_OFFSET && sp->get() <= STACK_OFFSET+0xFF);
	}

	void PLA(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
	{
		auto addr = STACK_OFFSET;
		auto sp = cpu->registers[(size_t)RegId::SP];
		auto a = cpu->registers[(size_t)RegId::A];
		auto p = cpu->registers[(size_t)RegId::P];

		sp->increment();
		addr += sp->get();

		a->set(mem->Read(addr));

		p->set_flag(flags::Flags::Z,a->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(a->get(),7));

		assert(sp->get() >= STACK_OFFSET && sp->get() <= STACK_OFFSET+0xFF);
	}

	void PLP(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
	{
		auto addr = STACK_OFFSET;
		auto sp = cpu->registers[(size_t)RegId::SP];
		auto p = cpu->registers[(size_t)RegId::P];

		sp->increment();
		addr += sp->get();

		p->set(mem->Read(addr));

		assert(sp->get() >= STACK_OFFSET && sp->get() <= STACK_OFFSET+0xFF);
	}

	void AND(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = mem->Read(value);

		auto p = cpu->registers[(size_t)RegId::P];
		auto a = cpu->registers[(size_t)RegId::A];

		int new_a = (a->get() & value);
		a->set(new_a);

		p->set_flag(flags::Flags::Z, a->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(a->get(),7));

	}

	void EOR(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = mem->Read(value);

		auto p = cpu->registers[(size_t)RegId::P];
		auto a = cpu->registers[(size_t)RegId::A];

		int new_a = (a->get() ^ value);
		a->set(new_a);

		p->set_flag(flags::Flags::Z, a->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(a->get(),7));

	}

	void ORA(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = mem->Read(value);

		auto p = cpu->registers[(size_t)RegId::P];
		auto a = cpu->registers[(size_t)RegId::A];

		int new_a = (a->get() | value);
		a->set(new_a);

		p->set_flag(flags::Flags::Z, a->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(a->get(),7));

	}

	void BIT(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = mem->Read(value);

		auto p = cpu->registers[(size_t)RegId::P];
		auto a = cpu->registers[(size_t)RegId::A];

		int test = (a->get() & value);
		p->set_flag(flags::Flags::Z, test == 0);
		p->set_flag(flags::Flags::V, utility::IsBitSet(value,6));
		p->set_flag(flags::Flags::N, utility::IsBitSet(value,7));

	}

	void ADC(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = mem->Read(value);

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

	void SBC(Cpu* cpu, Memory* mem, int value, AddressingMode mode) //who knows if this works
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = mem->Read(value);

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

	void CMP(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = mem->Read(value);

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

	void CPX(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = mem->Read(value);

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

	void CPY(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
	{
		if (mode != AddressingMode::IMMEDIATE)
			value = mem->Read(value);

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

	void INC(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];

		uint8_t m = mem->Read(value);
		m++;
		m = m % 0x100;
		mem->Write(value, m);

		p->set_flag(flags::Flags::Z, m == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(m,7));
	}

	void INX(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		auto x = cpu->registers[(size_t)RegId::X];

		x->increment();

		p->set_flag(flags::Flags::Z, x->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(x->get(),7));
	}

	void INY(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		auto y = cpu->registers[(size_t)RegId::Y];

		y->increment();

		p->set_flag(flags::Flags::Z, y->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(y->get(),7));
	}

	void DEC(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];

		int m = mem->Read(value);
		m--;
		if (m < 0)
			m += 0x100;
		mem->Write(value, m);

		p->set_flag(flags::Flags::Z, m == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(m,7));
	}

	void DEX(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		auto x = cpu->registers[(size_t)RegId::X];

		x->decrement();

		p->set_flag(flags::Flags::Z, x->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(x->get(),7));
	}

	void DEY(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
	{
		auto p = cpu->registers[(size_t)RegId::P];
		auto y = cpu->registers[(size_t)RegId::Y];

		y->decrement();

		p->set_flag(flags::Flags::Z, y->get() == 0);
		p->set_flag(flags::Flags::N, utility::IsBitSet(y->get(),7));
	}

	void ASL(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
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
			m = mem->Read(addr);
			p->set_flag(flags::Flags::C, utility::IsBitSet(m, 7));
			m = (m << 1) & 0xFF;
			mem->Write(addr,m);
		}

		p->set_flag(flags::Flags::Z, a->get() == 0);
		p->set_flag(flags::Flags::C, utility::IsBitSet(m, 7));
	}

	void LSR(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
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
			m = mem->Read(addr);
			p->set_flag(flags::Flags::C, utility::IsBitSet(m, 0));
			m = (m >> 1) & 0xFF;
			mem->Write(addr,m);
		}

		p->set_flag(flags::Flags::Z, a->get() == 0);
		p->set_flag(flags::Flags::C, utility::IsBitSet(m, 7));
	}

	void ROL(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
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
			m = mem->Read(addr);
			bool old_carry = p->get_flag(flags::Flags::C);
			p->set_flag(flags::Flags::C, utility::IsBitSet(m, 7));
			m = (m << 1) & 0xFF;
			utility::SetBit(&m, 0, old_carry);
			mem->Write(addr,m);
		}

		p->set_flag(flags::Flags::Z, a->get() == 0);
		p->set_flag(flags::Flags::C, utility::IsBitSet(m, 7));
	}

	void ROR(Cpu* cpu, Memory* mem, int value, AddressingMode mode)
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
			m = mem->Read(addr);
			bool old_carry = p->get_flag(flags::Flags::C);
			p->set_flag(flags::Flags::C, utility::IsBitSet(m, 0));
			m = (m >> 1) & 0xFF;
			utility::SetBit(&m, 7, old_carry);
			mem->Write(addr,m);
		}

		p->set_flag(flags::Flags::Z, a->get() == 0);
		p->set_flag(flags::Flags::C, utility::IsBitSet(m, 7));
	}

}