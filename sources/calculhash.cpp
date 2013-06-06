//#Copyright (c) 2011, Joseph Matheney
//#All rights reserved.
//
//#Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
//
//#    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
//#    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
//
//#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
//#ifdef fail
//	#!/bin/bash
//	# NOTE you can chmod 0755 this file and then execute it to compile (or just copy and paste)
//	gcc -o hashblock hashblock.c -lssl
//	exit 0
//#endif
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/block.hpp>
using namespace bc;

typedef struct block_header
{
    uint32_t version;
    hash_digest previous_block_hash;
    hash_digest merkle;
    uint32_t timestamp;
    uint32_t bits;
    uint32_t nonce;
}block_header;

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
int main() {
	// start with a block header struct
	block_header header;
	hash_digest hash;
        lecture_block(header);	
	hash=hash_block_header((const libbitcoin::block_type&)header);
	std::cout << "hash : " << hash << std::endl;
	std::string(header.merkle);
	std::cout << bytes_from_pretty(header.merkle ) << std::endl;
	return 0;
}
