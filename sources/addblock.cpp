#include "addblock.h"


int addblock(std::string database, block_type block)
{

    const std::string dbpath = database;
    threadpool pool(1);
    leveldb_blockchain chain(pool);
    auto blockchain_start = [](const std::error_code& ec) {}; 
    
    chain.start(dbpath, blockchain_start);
    std::promise<std::error_code> ec_promise;
    auto import_finished =
        [&ec_promise](const std::error_code& ec)
        {
            ec_promise.set_value(ec);
        };
   chain.import(block, 1, import_finished); 
 //  chain.store(block, import_finished); 
   std::error_code ec = ec_promise.get_future().get();
   if (ec)
   {
        log_error() << "Importing block failed: " << ec.message();
        return -1;
   }
  log_info() << "Imported block " << hash_block_header(block);
  pool.stop();
  pool.join();
  chain.stop();
  return 0;
}


