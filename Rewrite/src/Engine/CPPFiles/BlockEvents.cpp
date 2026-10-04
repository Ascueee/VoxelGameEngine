#include <BlockEvents.hpp>

void BlockEvents::TestEvent(Chunk& chunk, int blockIndex){
    ushort blockId = chunk.GetBlocks()[blockIndex];
    Block* currentBlock = BlockStorage::GetBlock(blockId);
    if(currentBlock->GetType() == BlockType::AIR)
        return;
    std::cout << "Test event worked for block {" << blockIndex << "} It is block {" << currentBlock->GetName() << "} \n";
}

//Runs for water blocks and needs to check if there is air beside and underneath the blocks
//if there is air change the airblock to a water block
//if there is a solid block dont change it to liquid
void BlockEvents::LiquidBlock(Chunk& chunk, int blockIndex){
    int blockBelow = blockIndex - 16 * 16;
    int blockRight = blockIndex + 1;
    int blockLeft = blockIndex - 1;
    int blockFront = blockIndex + 16;
    int blockBack  = blockIndex - 16;

    std::vector<ushort>& blocks = chunk.GetBlocks();

    if(blockBelow > 0 && BlockStorage::GetBlock(blocks[blockBelow])->GetType() == BlockType::AIR){
        blocks[blockBelow] = (ushort)BlockStorage::GetBlock(4)->GetId();
        chunk.SubscribeBlockEvent(blockBelow, BlockEvents::LiquidBlock);
        chunk.SetIsGenerated(false);
    }

    if((blockIndex % 16) < 15 && BlockStorage::GetBlock(blocks[blockRight])->GetType() == BlockType::AIR){
        blocks[blockRight] = (ushort)BlockStorage::GetBlock(4)->GetId();
        chunk.SubscribeBlockEvent(blockRight, BlockEvents::LiquidBlock);
        chunk.SetIsGenerated(false);
    }

    if((blockIndex % 16) > 0 && BlockStorage::GetBlock(blocks[blockLeft])->GetType() == BlockType::AIR){
        blocks[blockLeft] = (ushort)BlockStorage::GetBlock(4)->GetId();
        chunk.SubscribeBlockEvent(blockLeft, BlockEvents::LiquidBlock);
        chunk.SetIsGenerated(false);
    }

    if((blockIndex / 16) % 16 < 15 && BlockStorage::GetBlock(blocks[blockFront])->GetType() == BlockType::AIR){
        blocks[blockFront] = (ushort)BlockStorage::GetBlock(4)->GetId();
        chunk.SubscribeBlockEvent(blockFront, BlockEvents::LiquidBlock);
        chunk.SetIsGenerated(false);
    }

    if((blockIndex / 16) % 16 > 0 && BlockStorage::GetBlock(blocks[blockBack])->GetType() == BlockType::AIR){
        blocks[blockBack] = (ushort)BlockStorage::GetBlock(4)->GetId();
        chunk.SubscribeBlockEvent(blockBack, BlockEvents::LiquidBlock);
        chunk.SetIsGenerated(false);
    }
}