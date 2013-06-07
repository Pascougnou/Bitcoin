#ifndef CALCUL_HASH_H
#define CALCUL_HASH_H

#include "creationblock.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <bitcoin/bitcoin.hpp>

hash_digest calcul_hash(std::string path);

#endif
