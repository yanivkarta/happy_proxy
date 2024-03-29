#ifndef __PROVALLO_SCATTER_H_
#define __PROVALLO_SCATTER_H_

#include <string>
#include <vector>
#include <map>
#include <set>

#include <memory>
#include <mutex>
#include <thread>
#include <atomic>

#include <fstream>
#include <iostream>
#include <functional>

#include <iomanip>
#include <sstream>

#include <chrono>
#include <ctime>

#include <algorithm>
#include "gather.h"

//for variable scatter block size 
#define SCATTER_BLOCK_SIZE_VARIABLE 0xFFFFFFFF 
//for fixed scatter page block size
#define SCATTER_BLOCK_SIZE_FIXED_PAGE 0x1000



namespace provallo 
{
    //if scatter_block_size is 0xFFFFFFFF then block size is variable, 
    //otherwise it is fixed.

    template <typename T,size_t scatter_block_size , size_t gather_alloc >
    class scatter
    {
        bool fixed_block_size = false;
        //buffers :
        //atomic block counters :
        T* current_block_ = nullptr; //the block currently filling up
        T* scatter_ = nullptr;   //the scatter buffer memory region.

        std::atomic<size_t> scatter_block_counter_;
        std::atomic<size_t> scatter_blocks_expected_;
        std::atomic<size_t> scatter_blocks_received_;

        
        public:
        
        scatter():scatter_block_counter_(0),scatter_blocks_expected_(0),scatter_blocks_received_(0)
        {
            if(scatter_block_size == SCATTER_BLOCK_SIZE_VARIABLE)
            {
                fixed_block_size = false;
            }
            else
            {
                fixed_block_size = true;
            }
            
            allocate();
        }
        ~scatter()
        {
            
        }   
        private:
        void allocate()
        {
            if(fixed_block_size)
            {
                scatter_ = new T[scatter_block_size];
            }
            else
            {
                scatter_ = new T[gather_alloc];
            }
        }
    };
}   

#endif



