#ifndef __GATHER_H_
#define __GATHER_H_

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

namespace provallo 
{
    //if scatter_block_size is 0xFFFFFFFF then block size is variable, 
    //otherwise it is fixed.

    template <typename T,size_t scatter_block_size , size_t gather_alloc >
    class gather
    {
        bool fixed_block_size = false;
        //buffers :
        //atomic block counters :
        T* current_block_ = nullptr; //the block currently filling up
        T* gather_ = nullptr;   //the gather buffer memory region.

        std::atomic<size_t> gather_block_counter_;
        std::atomic<size_t> gather_blocks_expected_;
        std::atomic<size_t> gather_blocks_received_;

        
        public:
        
        gather():gather_block_counter_(0),gather_blocks_expected_(0),gather_blocks_received_(0)
        {
            if(scatter_block_size == 0xFFFFFFFF)
            {
                fixed_block_size = false;
            }
            else
            {
                fixed_block_size = true;
            }
            
            allocate();
        }
        ~gather()
        {
            
        }   
        private:
        std::shared_ptr<T> scatter_[scatter_size];
    };
    //gather from memor mapped files:
    template <typename T>
    class mmap_gather : public gather<T,0xFFFFFFFF,0xFFFFFFFF>
    {
        int fd_;
        size_t size_;
        T* mmap_;


        public:

        mmap_gather(int64_t size):size_(size),mmap_(nullptr)
        {
            //open the file:
            fd_ = open("/tmp/mmap", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
            if (fd_ == -1) 
            {
                throw std::runtime_error("open failed");
            }
            //allocate the memory region: 
            mmap_ = allocate();
        }   
        
        mmap_gather():gather<T,0xFFFFFFFF,0xFFFFFFFF>(),fd_(-1),size_(0),mmap_(nullptr) 
        {
            //open the file:
            fd_ = open("/tmp/mmap", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
            if (fd_ == -1) 
            {
                throw std::runtime_error("open failed");
            }
            //allocate the memory region: 
            mmap_ = allocate();

            fixed_block_size = false;

            
        }
        T* allocate()
        {
            //allocate the memory region: 
            mmap_ = (T*)mmap(NULL, size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0); 
            if (mmap_ == MAP_FAILED) 
            {
                close(fd_);
                throw std::runtime_error("mmap failed");
            }   
            return mmap_;

        }   
        ~mmap_gather()
        {
            
        }   
    };  


}

#endif //__GATHER_H_