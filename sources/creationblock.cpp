#include <bitcoin/bitcoin.hpp>
#include <fstream>
#include <iostream>
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

block_header blocfetch;

void display_block_recupinfo(const std::error_code& ec, const block_type& blk);
void display(block_header blk);

blockchain* chain = nullptr;
// Completion handler for when the blockchain has finished initializing.
void blockchain_started(const std::error_code& ec);
// Fetch tbe last block now that we have the depth.
void depth_fetched(const std::error_code& ec, size_t last_depth);
// Result: print the block header.
void display_block_header(const std::error_code& ec, const block_type& blk);

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
    //log_info() << "merkle: " << blk.merkle;
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

void hex2bin(unsigned char* dest, unsigned char* src)
{
        unsigned char bin;
        int c, pos;
        char buf[3];
 
        pos=0;
        c=0;
        buf[2] = 0;
        while(c < strlen((const char*)src))
        {
                // read in 2 characaters at a time
                buf[0] = src[c++];
                buf[1] = src[c++];
                // convert them to a interger and recast to a char (uint8)
                dest[pos++] = (unsigned char)strtol(buf, NULL, 16);
        }
       
}
 
// this function is mostly useless in a real implementation, were only using it for demonstration purposes
void hexdump(unsigned char* data, int len)
{
        int c;
       
        c=0;
        while(c < len)
        {
                printf("%.2x", data[c++]);
        }
        printf("\n");
}
 
// this function swaps the byte ordering of binary data, this code is slow and bloated (write your own)
void byte_swap(hash_digest data, int len) {
        int c;
        unsigned char tmp[len];
       
        c=0;
        while(c<len)
        {
                tmp[c] = data[len-(c+1)];
                c++;
        }
       
        c=0;
        while(c<len)
        {
                data[c] = tmp[c];
                c++;
        }
}
int main(int argc, char **argv)
{
    if (argc != 2)
	return 1;
    // Define a threadpool with 1 thread.
    threadpool pool(1);
    const std::string dbpath = argv[1];
    // Create a LevelDB blockchain.
    leveldb_blockchain ldb_chain(pool);
    // Initialize our global 'chain' pointer from above.
    chain = &ldb_chain;
    // Start the database using its implementation specific method.
    ldb_chain.start(dbpath, blockchain_started);
    // Keep running until the user presses enter.
    // Since libbitcoin is asynchronous, you need to synchronise with
    // them to know when to exit safely.
    // For these examples we just pause until enter for simplicity sake.
    std::cin.get();
    display(blocfetch);
    std::cin.get();
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
    pool.stop();
    // Join them one by one.
    pool.join();
    // Finally stop the blockchain safely now everything has stopped.
    ldb_chain.stop();
    return 0;
}

