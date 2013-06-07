#ifndef ADDBLOCK_H
#define ADDBLOCK_H

#include <future>
#include <bitcoin/bitcoin.hpp>
using namespace bc;

int addblock(std::string database, block_type block);

#endif
