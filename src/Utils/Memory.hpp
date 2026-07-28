#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <new>
#include <list>
#include "utility"
#include <sys/types.h>

namespace MEMO
 {

 class MemoryArena
   {
    private:
      const std::size_t blockSize;
      std::size_t blockPos{0};
      std::size_t currAllocSize{0};
      uint8_t* currentBlock{nullptr};
      std::list<std::pair<std::size_t, uint8_t*>> usedBlocks;
      std::list<std::pair<std::size_t, uint8_t*>> availableBlocks;

     public:
       MemoryArena(std::size_t blockSize = 262144 /*256 kb*/)
       : blockSize(blockSize){}

       MemoryArena(const MemoryArena&) = delete;
       MemoryArena& operator=(const MemoryArena&) = delete;
       MemoryArena(MemoryArena&&) noexcept = default;
       MemoryArena& operator=(MemoryArena&&) noexcept = default;


       ~MemoryArena()
       {
        delete[] currentBlock;
        
        for(auto& block : usedBlocks)
          delete[] block.second;
        for(auto& block : availableBlocks)
          delete[] block.second;
       }

       void* alloc(std::size_t bytes)
       {
         bytes = ((bytes + 15) & ~static_cast<std::size_t>(15));
         if(blockPos + bytes > currAllocSize)
         {
           if(currentBlock)
           {
             usedBlocks.push_back(std::make_pair(currAllocSize, currentBlock));
             currentBlock = nullptr;
           }

           for(auto it{availableBlocks.begin()}; it != availableBlocks.end(); ++it)
           {
             if(it->first >= bytes)
             {
               currAllocSize = it->first;
               currentBlock = it->second;
               availableBlocks.erase(it);
               break;
             }
           }

           if(!currentBlock)
           {
             currAllocSize = std::max(bytes, blockSize);
             currentBlock = new uint8_t[currAllocSize];
           }
           blockPos = 0;
         }

         void* ret = currentBlock + blockPos;
         blockPos += bytes;
         return ret;
       }


       template<typename T>
       T* alloc(std::size_t n=1, bool runC = true) 
       {
         T* ret = static_cast<T*>(alloc(n * sizeof(T)));
        
         if(runC)
           for(std::size_t i{0}; i < n; ++i)
             new (&ret[i]) T();
        
         return ret;
       }

       void reset()
       {
         blockPos = 0;
         if(currentBlock)
         {
           usedBlocks.push_back(std::make_pair(currAllocSize, currentBlock));
           currentBlock = nullptr;
           currAllocSize = 0;
         }
         availableBlocks.splice(availableBlocks.begin(), usedBlocks);
       }

       std::size_t totalAllocated() const 
       {
          std::size_t total = currentBlock ? currAllocSize : 0;

          for(const auto& a : usedBlocks)
            total += a.first;
          for(const auto& a : availableBlocks)
            total += a.first;

          return total;
       }
   };

 } // namespace MEMO

#endif // MEMORY_HPP
