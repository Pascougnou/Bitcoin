#ifndef ADDBLOCK_HPP
#define ADDBLOCK_HPP_HPP

#include <future>
#include <bitcoin/bitcoin.hpp>
using namespace bc;

int addblock(std::string database, block_type block);

#endif
