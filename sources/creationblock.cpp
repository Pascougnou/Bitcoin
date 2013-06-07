#include "creationblock.h"
//#include <bitcoin/bitcoin.hpp>
//using namespace bc ;
blockchain* chain = nullptr;

//typedef struct block_header
//{
//    uint32_t version;
//    hash_digest previous_block_hash;
//    hash_digest merkle;
//    uint32_t timestamp;
//    uint32_t bits;
//    uint32_t nonce;
//}block_header;
block_header blocfetch;

void display_block_recupinfo(const std::error_code& ec, const block_type& blk)
{
    if (ec)
    {
        log_error() << "Failure fetching block header: " << ec.message();
        return;
    }
    // 32 byte std::array of uint8_t
    const hash_digest& blk_hash = hash_block_header(blk);
    // Encode block hash into a pretty hex string.
    //log_info() << "hash: " << pretty_hex(blk_hash);
    // Display a few fields from the block header.
    // See <bitcoin/primitives.hpp> for the definition of block_type.
    //log_info() << "version: " << blk.version;
    blocfetch.version=blk.version;
    // hash_digest can be used directly in log_info(),
    // implicity calling pretty_hex() on the hash_digest.
    //log_info() << "previous_block_hash: " << blk.previous_block_hash;
    blocfetch.previous_block_hash=blk.previous_block_hash;
    blocfetch.merkle=blk.merkle;
    //log_info() << "timestamp: " << blk.timestamp;
    blocfetch.timestamp=blk.timestamp;
    //log_info() << "bits: " << blk.bits;
    blocfetch.bits=blk.bits;
    //log_info() << "nonce: " << blk.nonce;
    blocfetch.nonce=blk.nonce;
    // This is not the full block, only the header.
    // For the full block use fetch_block() instead.
    //assert(blk.transactions.size() == 0);
    // A goodbye message.
    //log_info() << "Finished.";
}


void display(block_header blk)
{
    // 32 byte std::array of uint8_t
    // Encode block hash into a pretty hex string.
    // Display a few fields from the block header.
    // See <bitcoin/primitives.hpp> for the definition of block_type.
    log_info() << "version: " << blk.version;
    // hash_digest can be used directly in log_info(),
    // implicity calling pretty_hex() on the hash_digest.
    log_info() << "previous_block_hash: " << blk.previous_block_hash;
    log_info() << "merkle: " << blk.merkle;
    log_info() << "timestamp: " << blk.timestamp;
    log_info() << "bits: " << blk.bits;
    log_info() << "nonce: " << blk.nonce;
    // This is not the full block, only the header.
    // For the full block use fetch_block() instead.
    // A goodbye message.
    log_info() << "Finished.";
}
void depth_fetched(const std::error_code& ec, size_t last_depth)
{
    if (ec)
    {
        log_error() << "Failed to fetch last depth: " << ec.message();
        return;
    }
    // Display the block number.
    log_info() << "depth: " << last_depth;
    assert(chain);
   // Begin fetching the block header.
    chain->fetch_block_header(last_depth, display_block_recupinfo);
}

void blockchain_started(const std::error_code& ec)
{
    // std::error_code's can be tested like bools, and
    // compared against specific error enums.
    // See <bitcoin/error.hpp> for a full list of them.
    if (ec)
    {
        log_error() << "Blockchain failed to start: " << ec.message();
        return;
    }
    // Blockchain has safely started.
    log_info() << "Blockchain started.";
    assert(chain);
    chain->fetch_last_depth(depth_fetched);
}


 
int export_bloc(std::string argv ,block_header *bloc)
{
    // Define a threadpool with 1 thread.
    threadpool pool(1);
    const std::string dbpath = argv;
    // Create a LevelDB blockchain.
    leveldb_blockchain ldb_chain(pool);
    // Initialize our global 'chain' pointer from above.
    chain = &ldb_chain;
    // Start the database using its implementation specific method.
    ldb_chain.start(dbpath, blockchain_started);
    std::fstream fichier;
    fichier.open("testblock", std::ios::out | std::ios::binary);
    if (fichier)
    {
        fichier.write(((char*)&blocfetch), sizeof(blocfetch));
	fichier.close();
    }
    else
    {
        std::cout<<"erreur d'ouverture de fichier"<<std::endl;
    }

    
    std::cout << "taille du header " << sizeof(block_header) << std::endl;
    // Begin stopping the threadpools in parallel (only 1 here).
    std::cin.get(); // il faut attendre que la db soit lue
    pool.stop();
    // Join them one by one.
    pool.join();
    // Finally stop the blockchain safely now everything has stopped.
    ldb_chain.stop();
    *bloc=blocfetch;
    return 0;
}
