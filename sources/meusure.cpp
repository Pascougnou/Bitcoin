#include "addblock.h"
#include "creationblock.h"
#include "calculhash.h"
#include <papi.h> // lib pour le calcul des perfs

int main()
{
	// uitlisés pour le calcul de performance
	long_long start_cycles, end_cycles, start_usec, end_usec;
	if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT)
  		exit(1);

	// block : current block, block_prev: previous block header
	block_type block;
	block_header block_prev;

	std::cout<<"coucou"<<std::endl;

	//va chercher le prvious block header dans la db 
	export_bloc("../database",&block_prev); 

	// test pour le 2eme bloc  
	block.timestamp=1231469665;
 	hash_digest merkl{0x0e,0x3e,0x23,0x57,0xe8,0x06,0xb6,0xcd,0xb1,0xf7,0x0b,0x54,0xc3,0xa3,0xa1,0x7b,0x67,0x14,0xee,0x1f,0x0e,0x68,0xbe,0xbb,0x44,0xa7,0x4b,0x1e,0xfd,0x51,0x20,0x98};
	block.merkle=merkl;
//	block.nonce=2573394689; c'est le bon nonce
	block.nonce=2573394689-10000000; // - nb est le nb de nonce++ a realiser et donc le nb de passage dans la bloucle de double hashage
	block.bits=block_prev.bits;
	block.version=block_prev.version;

	
	start_usec = PAPI_get_virt_usec();
	start_cycles = PAPI_get_virt_cyc();
	// Ici calcul_hash va etre la fonction couteuse en temps de calcul
	block.previous_block_hash = calcul_hash(block_prev);
//	std::cout<<"coucou"<<std::endl;
	end_cycles = PAPI_get_virt_cyc();
	end_usec = PAPI_get_virt_usec();
	std::cout<<"coucou"<<std::endl;


//	block.merkle=generate_sha256_hash(uncast_type(block.previous_block_hash));
//	block.timestamp=block_prev.timestamp;
//	block.nonce=0;
	std::cout << " cycle prise par le hash : " << end_cycles - start_cycles << std::endl;
	std::cout << " temps 		       : " << end_usec - start_usec << std::endl;
	return addblock("../database", block);
//	return 0;
}
