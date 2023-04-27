//
// Created by Oliver Waldhorst on 20.03.20.
// Copyright © 2017-2020 Oliver Waldhorst. All rights reserved.
//

#ifndef MYFS_MYONDISKFS_H
#define MYFS_MYONDISKFS_H

#include "myfs.h"

/// @brief On-disk implementation of a simple file system.
class MyOnDiskFS : public MyFS
{
protected:
public:
    static MyOnDiskFS *
    Instance();

    // TODO: [PART 1] Add attributes of your file system here
    MyOnDiskFsFileInfo files[NUM_DIR_ENTRIES];
    OpenFile openFiles[NUM_DIR_ENTRIES];

    SuperBlock *mySuperBlock;
    Dmap *myDmap;
    FAT *myFAT;

    MyOnDiskFS();
    ~MyOnDiskFS();

    static void SetInstance();

    // --- Methods called by FUSE ---
    // For Documentation see https://libfuse.github.io/doxygen/structfuse__operations.html
    virtual int fuseGetattr(const char *path, struct stat *statbuf);
    virtual int fuseMknod(const char *path, mode_t mode, dev_t dev);
    virtual int fuseUnlink(const char *path);
    virtual int fuseRename(const char *path, const char *newpath);
    virtual int fuseChmod(const char *path, mode_t mode);
    virtual int fuseChown(const char *path, uid_t uid, gid_t gid);
    virtual int fuseTruncate(const char *path, off_t newSize);
    virtual int fuseOpen(const char *path, struct fuse_file_info *fileInfo);
    virtual int fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo);
    virtual int fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo);
    virtual int fuseRelease(const char *path, struct fuse_file_info *fileInfo);
    virtual void *fuseInit(struct fuse_conn_info *conn);
    virtual int fuseReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo);
    virtual int fuseTruncate(const char *path, off_t offset, struct fuse_file_info *fileInfo);
    virtual void fuseDestroy();

    // TODO: Add methods of your file system here
private:
    int findFileByName(const char *name);

    void readStructures();
    void readFileMetaDataOnDisk(int fileIndex);
    void readOpenFileBlock(int fileIndex, int blockNo);

    void calculateAndSetBlockPositions(int numBlocks);
    int writeBlocks(int startBlock, int numBlocks, char *buffer);
    int writeDataBlock(int blockNo, char *buffer);
    int readBlocks(int startBlock, int numBlocks, char *buffer);
    int updateFileMetaDataOnDisk(int fileIndex);
    int reallocBlocks(const char *path, size_t newSize, struct fuse_file_info *fileInfo);

    int getNextFreeBlock(int block); // starts looking from block

    // klären ob ok, dass die beiden im Hauptspeicher bleiben, sonst muss man mit buffern arbeiten und schreiben falls neuer Block
    int updateDmapOnDisk();
    int updateFatOnDisk();
};

#endif // MYFS_MYONDISKFS_H
