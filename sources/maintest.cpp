#include "addblock.h"
#include "creationblock.h"
#include "calculhash.h"

int main()
{
	std::cout << "hash : " << calcul_hash("../database") <<std::endl;
	return addblock("../database", genesis_block());
}
