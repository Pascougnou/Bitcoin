//#include "creationblock.h"
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <sstream>
//#include <bitcoin/bitcoin.hpp>
////using namespace bc;
#include "calculhash.h"

// sert a lire un block -- maintement inutile
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
//Calcule le target a partir de block.bits
hash_digest calc_target(block_header blk)
{
	hash_digest target;
	int byte1 = blk.bits >> 24; 
	int byte2 = blk.bits -( byte1 << 24);
	for (int i=0;i<32;i++)
	{
		target[i]=(unsigned char)0;
	}
	target[32-byte1+1]=(unsigned char)byte2;
	target[32-byte1+2]=(unsigned char)(byte2 >> 8);
	target[32-byte1+3]=(unsigned char)(byte2 >> 16);
//	std::cout<< "b1 " << byte1 <<" b2 "<< byte2 << " target "<< target << std::endl;
//	printf("%x",(unsigned char)byte2);
	return target;
}
hash_digest calcul_hash(block_header blk)
{

	hash_digest hash;
	// le target est ce que l'on va comparer avec le hash pour savoir si il a le bon nombre de 0.
	hash_digest target = calc_target(blk);
	
	hash=hash_block_header((const libbitcoin::block_type&)blk);  // calcul du hash
	while(hash>target)
	{	
//		std::cout << " nonce = :" << blk.nonce <<std::endl;
//		std::cout << "hash : " << hash << std::endl << "target : " << target << std::endl;
//	
		blk.nonce=blk.nonce + 1;
		hash=hash_block_header((const libbitcoin::block_type&)blk); 
	}
	std::cout << "hash : " << hash << std::endl << "target : " << target << std::endl;
	return hash;
}

