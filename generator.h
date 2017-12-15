#ifndef GENERATOR_H
#define GENERATOR_H

#include "ensitlm.h"

#define ADDR_MEM 0x10000000
#define SIZE_MEM 0x00015400

#define ADDR_LCD 0x10015400
#define SIZE_LCD 0x0000000C

#define ADDR_ROM 0x1001540C
#define SIZE_ROM 0x00009600

#define IMG_SIZE 76800

SC_MODULE(Generator){
  ensitlm::initiator_socket<Generator, true> initiator;
	sc_core::sc_in<bool> display_int;
	sc_core::sc_event display_ended;
	void thread(void);
	void notif_LCD();
  SC_CTOR(Generator);
};

#endif
