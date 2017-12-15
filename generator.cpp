#include "generator.h"
#include "LCDC_registermap.h"
#include "ensitlm.h"
#include "systemc.h"
#include <unistd.h>

#define TEST 0

using namespace std;

void Generator::thread(void){

	int size_emb = (87040 - IMG_SIZE);
	ensitlm::data_t d;

	int phase = 0;

	while(true) {
		for (int i = 0; i < IMG_SIZE/8 ; i ++) {

			// Lecture des données dans la ROM
			initiator.read(ADDR_ROM + 4*((phase + i)%(IMG_SIZE/8)), d);

			// Ecriture des données dans la mémoire vidéo, avec séparation des pixels
			initiator.write( ADDR_MEM + size_emb + 8*i,
				(d & 0xF0000000) + ((d & 0x0F000000) >> 4)
				 + ((d & 0x00F00000) >> 8) + ((d & 0x000F0000) >> 12) );
			initiator.write( ADDR_MEM + size_emb + 8*i + 4,
				((d & 0x0000F000) << 16) + ((d & 0x00000F00) << 12)
				 + ((d & 0x000000F0) << 8) + ((d & 0x0000000F) << 4) );

		}

		// Paramètrage de l'adresse de départ
		initiator.write(ADDR_LCD+LCDC_ADDR_REG, ADDR_MEM+size_emb);

		// Activation du LCD
		initiator.write(ADDR_LCD+LCDC_START_REG, 0x00000001);

		// -------- Gestion Interruption -----------------

		wait(display_ended);
		initiator.write(ADDR_LCD+LCDC_INT_REG, 0x00000000);
		usleep(20000);
		phase++;
	}
}

void Generator::notif_LCD() {
	if (display_int.read() == true) {
		display_ended.notify();
//		cout << "OOO Interruption Détectée !!! OOO" << endl;
	}
}

Generator::Generator(sc_core::sc_module_name name) :
   sc_core::sc_module(name)
{
   SC_THREAD(thread);
	 SC_METHOD(notif_LCD);
	 sensitive << display_int;
}
