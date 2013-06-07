//#include "creationblock.h"
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <sstream>
//#include <bitcoin/bitcoin.hpp>
////using namespace bc;
#include "calculhash.h"

void lecture_block (block_header blk)
{
   std::ifstream fichier;
   fichier.open("testblock", std::ios::in | std::ios::binary);
   if (fichier)
   {
	fichier.read((char*)&blk,sizeof(blk));
	fichier.close();
	std::cout<<" version : " << blk.version << std::endl;
	std::cout<<" prev_block : "<< blk.previous_block_hash << std::endl;
	std::cout<<" merkle : " << blk.merkle << std::endl;
	std::cout<<" timestamp : " << blk.timestamp << std::endl;
	std::cout<<" bits : " << blk.bits << std::endl;
	std::cout<<" nonce : " << blk.nonce << std::endl;
   }
   else
   {
        std::cout<<"erreur d'ouverture de fichier"<<std::endl;
   }	
}
void litlend_hex( hash_digest*in)
{
	int i;
	hash_digest tmp = *in;
	hash_digest lala;
	for(i=0;i<32;i++)
	{
		
	}
}
hash_digest calcul_hash(std::string path)
{
	block_header blk;
	hash_digest hash;
	export_bloc(path ,&blk); // on recupere un block de la db
	
	hash=hash_block_header((const libbitcoin::block_type&)blk);  // calcul du hash
//	std::cout << "hash : " << hash << std::endl;
	return hash;
}

