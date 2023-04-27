//
//  myfs-structs.h
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#ifndef myfs_structs_h
#define myfs_structs_h

#define NAME_LENGTH 255
#define BLOCK_SIZE 512
#define NUM_DIR_ENTRIES 64
#define NUM_OPEN_FILES 64

// TODO: Add structures of your file system here

struct MyFsFileInfo
{
    char name[NAME_LENGTH] = "\0";
    size_t size = 0;
    char *data = nullptr;
    uid_t st_uid;
    gid_t st_gid;
    mode_t st_mode;
    uint32_t atimensec = 0; /* Time of last access */
    uint32_t mtimensec = 0; /* Time of last modification */
    uint32_t ctimensec = 0; /* Time of last status change */
};

struct MyOnDiskFsFileInfo
{
    char name[NAME_LENGTH] = "\0";
    size_t size = 0;
    int firstBlock;
    uid_t st_uid;
    gid_t st_gid;
    mode_t st_mode;
    uint32_t atimensec = 0; /* Time of last access */
    uint32_t mtimensec = 0; /* Time of last modification */
    uint32_t ctimensec = 0; /* Time of last status change */
};

struct OpenFile
{
    char *buffer;
    int blockNo; // buffered block
};

struct SuperBlock // Information Dateisystem und Postion Einträge
{
    char name[NAME_LENGTH];
    int numDataBlocks;
    // also just block amounts
    int dmapSize;
    int fatSize;
    int rootSize;
    int containerSize;

    // first blocks
    int dmapBlock;
    int fatBlock;
    int rootBlock;
    int dataBlock;
};

struct Dmap // Verzeichnis freier Blöcke
{
    bool *blockSet;
};

struct FAT // File allocation table
{
    int *nextBlock;
};

#endif /* myfs_structs_h */
