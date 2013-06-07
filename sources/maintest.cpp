#include "addblock.h"
#include "creationblock.h"
#include "calculhash.h"

int main()
{
	// block : current block, block_prev: previous block header
	block_type block;
	block_header block_prev;
	//va chercher le prvious block header dans la db 
	export_bloc("../database",&block_prev); 
	//rempli le block avec les info du header precedent
	block.version=block_prev.version;

	// test pour le 2eme bloc  
//	block.timestamp=1231469665;
// 	hash_digest merkl{0x0e,0x3e,0x23,0x57,0xe8,0x06,0xb6,0xcd,0xb1,0xf7,0x0b,0x54,0xc3,0xa3,0xa1,0x7b,0x67,0x14,0xee,0x1f,0x0e,0x68,0xbe,0xbb,0x44,0xa7,0x4b,0x1e,0xfd,0x51,0x20,0x98};
//	block.nonce=2573394688;
//	block.merkle=merkl;

	block.previous_block_hash = calcul_hash(block_prev);
	// Ici calcul_hash va etre la fonction couteuse en temps de calcul
	block.merkle=generate_sha256_hash(uncast_type(block.previous_block_hash));
	block.timestamp=block_prev.timestamp;
	block.bits=block_prev.bits;
	block.nonce=0;
	// une fois toutes les info prisent, on peut inserer le block dans la db
	return addblock("../database", block);
}
