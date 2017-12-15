#include "ensitlm.h"
#include "generator.h"
#include "memory.h"
#include "bus.h"
#include "LCDC.h"
#include "ROM.h"
#include "systemc.h"


using namespace std;

int sc_main(int argc, char **argv){
  Generator gen1("Generator1");
  Memory mem("Memory",SIZE_LCD+SIZE_MEM);
  Bus bus("Bus");
  LCDC lcd("Ecran", sc_time(1.0/25, SC_SEC));
	ROM rom("ROM");

	sc_signal < bool , SC_MANY_WRITERS > irq_signal ("_IRQ_" );

// Mappage bus
  bus.map(mem.target, ADDR_MEM, SIZE_MEM);
  bus.map(lcd.target_socket, ADDR_LCD, SIZE_LCD);
	bus.map(rom.socket, ADDR_ROM, SIZE_ROM);

// Binding generator and memory
  gen1.initiator.bind(bus.target);
  bus.initiator.bind(mem.target);

// Binding LCD
	bus.initiator.bind(lcd.target_socket);
	lcd.initiator_socket.bind(bus.target);

// Binding ROM
	bus.initiator.bind(rom.socket);

// Gestion interruption
	lcd.display_int.bind(irq_signal);
  gen1.display_int.bind(irq_signal);

  sc_core::sc_start();

  return 0;
}
