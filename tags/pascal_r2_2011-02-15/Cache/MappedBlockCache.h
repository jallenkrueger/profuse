#ifndef __MAPPED_BLOCK_CACHE_H__
#define __MAPPED_BLOCK_CACHE_H__

#include <Cache/BlockCache.h>

namespace Device {

class MappedBlockCache : public BlockCache {
    public:

    MappedBlockCache(BlockDevice *, void *data);
    virtual ~MappedBlockCache();

    virtual void sync();
    virtual void write(unsigned block, const void *vp);

    virtual void zeroBlock(unsigned block);    
    

    virtual void *acquire(unsigned block);
    virtual void release(unsigned block, int flags);
    virtual void markDirty(unsigned block);

    private:
    
    void sync(unsigned block);
        
    uint8_t *_data;
    bool _dirty;
};

} // namespace

#endif
