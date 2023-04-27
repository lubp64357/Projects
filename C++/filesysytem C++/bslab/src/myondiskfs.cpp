//
// Created by Oliver Waldhorst on 20.03.20.
// Copyright © 2017-2020 Oliver Waldhorst. All rights reserved.
//

#include "myondiskfs.h"

// For documentation of FUSE methods see https://libfuse.github.io/doxygen/structfuse__operations.html

#undef DEBUG

// TODO: Comment lines to reduce debug messages
#define DEBUG
#define DEBUG_METHODS
#define DEBUG_RETURN_VALUES
#define DEFAULT_NUM_DATA_BLOCKS 100000
#define ROUNDUP(Z, N) ((Z + N - 1) / N) // Zähler und Nenner

#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "macros.h"
#include "myfs.h"
#include "myfs-info.h"
#include "blockdevice.h"

/// @brief Constructor of the on-disk file system class.
///
/// You may add your own constructor code here.
MyOnDiskFS::MyOnDiskFS() : MyFS()
{
    // create a block device object
    this->blockDevice = new BlockDevice(BLOCK_SIZE);

    // TODO: [PART 2] Add your constructor code here
}

/// @brief Destructor of the on-disk file system class.
///
/// You may add your own destructor code here.
MyOnDiskFS::~MyOnDiskFS()
{
    // free block device object
    delete this->blockDevice;

    // TODO: [PART 2] Add your cleanup code here
}

/// @brief Create a new file.
///
/// Create a new file with given name and permissions.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] mode Permissions for file access.
/// \param [in] dev Can be ignored.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseMknod(const char *path, mode_t mode, dev_t dev)
{
    LOGM();

    // TODO: [PART 2] Implement this!

    // check if file already exists
    int fileCreatePoint = findFileByName(path + 1);
    if (fileCreatePoint >= 0)
    {
        RETURN(-EEXIST);
    }
    // find empty spot
    fileCreatePoint = findFileByName("\0");
    if (fileCreatePoint < 0)
    {
        RETURN(-ENOSPC);
    }

    MyOnDiskFsFileInfo &fileInfo = files[fileCreatePoint];
    strcpy(fileInfo.name, path + 1);
    fileInfo.firstBlock = -1;
    fileInfo.size = 0;
    fileInfo.st_mode = mode;
    fileInfo.st_uid = getuid();
    fileInfo.st_gid = getgid();
    fileInfo.atimensec = time(NULL);
    fileInfo.mtimensec = time(NULL);
    fileInfo.ctimensec = time(NULL);

    updateFileMetaDataOnDisk(fileCreatePoint);
    RETURN(0);
}

/// @brief Delete a file.
///
/// Delete a file with given name from the file system.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseUnlink(const char *path)
{
    LOGM();

    // TODO: [PART 2] Implement this!
    int fileIndex = findFileByName(path + 1);
    if (fileIndex < 0)
    {
        RETURN(-ENOENT);
    }

    strcpy(files[fileIndex].name, "\0");
    //"delete" on disk, actually just changes dmap

    int blockNo = files[fileIndex].firstBlock;
    if (blockNo >= 0)
    {

        while (blockNo >= 0)
        {
            myDmap->blockSet[blockNo] = false;
            int newBlockNo = myFAT->nextBlock[blockNo];
            myFAT->nextBlock[blockNo] = -1;
            blockNo = newBlockNo;
        }

        files[fileIndex].firstBlock=-1;
        
        updateDmapOnDisk();
        updateFatOnDisk();
        updateFileMetaDataOnDisk(fileIndex);
    }

    RETURN(0);
}

/// @brief Rename a file.
///
/// Rename the file with with a given name to a new name.
/// Note that if a file with the new name already exists it is replaced (i.e., removed
/// before renaming the file.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] newpath  New name of the file, starting with "/".
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseRename(const char *path, const char *newpath)
{
    LOGM();

    // TODO: [PART 2] Implement this!
    int fileIndex = findFileByName(path + 1);
    if (fileIndex < 0)
    {
        RETURN(-ENOSPC);
    }

    int existentFile = findFileByName(newpath + 1);
    if (existentFile >= 0)
    {
        fuseUnlink(newpath);
    }
    strcpy(files[fileIndex].name, newpath + 1);

    updateFileMetaDataOnDisk(fileIndex);

    RETURN(0);
}

/// @brief Get file meta data.
///
/// Get the metadata of a file (user & group id, modification times, permissions, ...).
/// \param [in] path Name of the file, starting with "/".
/// \param [out] statbuf Structure containing the meta data, for details type "man 2 stat" in a terminal.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseGetattr(const char *path, struct stat *statbuf)
{
    LOGM();

    // TODO: [PART 2] Implement this!
    LOGF("\tAttributes of %s requested\n", path);

    // GNU's definitions of the attributes (http://www.gnu.org/software/libc/manual/html_node/Attribute-Meanings.html):
    // 		st_uid: 	The user ID of the file’s owner.
    //		st_gid: 	The group ID of the file.
    //		st_atime: 	This is the last access time for the file.
    //		st_mtime: 	This is the time of the last modification to the contents of the file.
    //		st_mode: 	Specifies the mode of the file. This includes file type information (see Testing File Type) and
    //		            the file permission bits (see Permission Bits).
    //		st_nlink: 	The number of hard links to the file. This count keeps track of how many directories have
    //	             	entries for this file. If the count is ever decremented to zero, then the file itself is
    //	             	discarded as soon as no process still holds it open. Symbolic links are not counted in the
    //	             	total.
    //		st_size:	This specifies the size of a regular file in bytes. For files that are really devices this field
    //		            isn’t usually meaningful. For symbolic links this specifies the length of the file name the link
    //		            refers to.

    statbuf->st_uid = getuid();     // The owner of the file/directory is the user who mounted the filesystem
    statbuf->st_gid = getgid();     // The group of the file/directory is the same as the group of the user who mounted the filesystem
    statbuf->st_atime = time(NULL); // The last "a"ccess of the file/directory is right now
    statbuf->st_mtime = time(NULL); // The last "m"odification of the file/directory is right now

    int ret = 0;

    if (strcmp(path, "/") == 0)
    {
        statbuf->st_mode = S_IFDIR | 0755;
        statbuf->st_nlink = 2; // Why "two" hardlinks instead of "one"? The answer is here: http://unix.stackexchange.com/a/101536
        statbuf->st_blocks=mySuperBlock->numDataBlocks;
        statbuf->st_blksize=BLOCK_SIZE;
        statbuf->st_size=mySuperBlock->numDataBlocks*BLOCK_SIZE;
    }
    else
    {
        int fileIndex = findFileByName(path + 1);
        if (fileIndex >= 0)
        {
            statbuf->st_mode = files[fileIndex].st_mode;
            statbuf->st_nlink = 1;
            statbuf->st_size = files[fileIndex].size;
            statbuf->st_uid = files[fileIndex].st_uid;
            statbuf->st_gid = files[fileIndex].st_gid;
            statbuf->st_atime = files[fileIndex].atimensec;
            statbuf->st_mtime = files[fileIndex].mtimensec;
            statbuf->st_ctime = files[fileIndex].ctimensec;
        }
        else
        {
            ret = -ENOENT;
        }
    }
    RETURN(ret);
}

/// @brief Change file permissions.
///
/// Set new permissions for a file.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] mode New mode of the file.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseChmod(const char *path, mode_t mode)
{
    LOGM();

    // TODO: [PART 2] Implement this!
    int fileIndex = findFileByName(path + 1);
    if (fileIndex < 0)
    {
        RETURN(-ENOENT);
    }
    files[fileIndex].st_mode = mode;

    files[fileIndex].ctimensec = time(NULL);
    updateFileMetaDataOnDisk(fileIndex);
    RETURN(0);
}

/// @brief Change the owner of a file.
///
/// Change the user and group identifier in the meta data of a file.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] uid New user id.
/// \param [in] gid New group id.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseChown(const char *path, uid_t uid, gid_t gid)
{
    LOGM();

    // TODO: [PART 2] Implement this!
    int fileIndex = findFileByName(path + 1);
    if (fileIndex < 0)
    {
        RETURN(-ENOENT);
    }
    files[fileIndex].st_uid = uid;
    files[fileIndex].st_gid = uid;

    files[fileIndex].ctimensec = time(NULL);
    updateFileMetaDataOnDisk(fileIndex);
    RETURN(0);
}

/// @brief Open a file.
///
/// Open a file for reading or writing. This includes checking the permissions of the current user and incrementing the
/// open file count.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [out] fileInfo Can be ignored in Part 1
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseOpen(const char *path, struct fuse_file_info *fileInfo)
{
    LOGM();

    // TODO: [PART 2] Implement this!
    int fileIndex = findFileByName(path + 1);
    if (fileIndex < 0)
    {
        RETURN(-ENOENT);
    }
    fileInfo->fh = fileIndex;
    //allocate buffer
    openFiles[fileIndex].buffer=new char[BLOCK_SIZE];
    RETURN(0);
}

/// @brief Read from a file.
///
/// Read a given number of bytes from a file starting form a given position.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// Note that the file content is an array of bytes, not a string. I.e., it is not (!) necessarily terminated by '\0'
/// and may contain an arbitrary number of '\0'at any position. Thus, you should not use strlen(), strcpy(), strcmp(),
/// ... on both the file content and buf, but explicitly store the length of the file and all buffers somewhere and use
/// memcpy(), memcmp(), ... to process the content.
/// \param [in] path Name of the file, starting with "/".
/// \param [out] buf The data read from the file is stored in this array. You can assume that the size of buffer is at
/// least 'size'
/// \param [in] size Number of bytes to read
/// \param [in] offset Starting position in the file, i.e., number of the first byte to read relative to the first byte of
/// the file
/// \param [in] fileInfo Can be ignored in Part 1
/// \return The Number of bytes read on success. This may be less than size if the file does not contain sufficient bytes.
/// -ERRNO on failure.
int MyOnDiskFS::fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo)
{
    LOGM();
    // TODO: [PART 2] Implement this!
    int fileIndex = fileInfo->fh;
    OpenFile &openFile = openFiles[fileIndex];
    int blockNo = files[fileIndex].firstBlock;
    if (files[fileIndex].size < size)
    {
        size = files[fileIndex].size;
    }

    int blocksToRead = ROUNDUP(size, BLOCK_SIZE);

    // apply offset to read
    for (int i = 0; i < ROUNDUP(offset, BLOCK_SIZE); i++)
    {
        if (blockNo < 0)
        {
            RETURN(-errno);
        }

        blockNo = myFAT->nextBlock[blockNo];
    }

    int blocksRead = 0;
    if (blocksToRead > 1)
    {
        // read all blocks except last one
        for (blocksRead = 0; blocksRead < blocksToRead; blocksRead++)
        {
            if (myFAT->nextBlock[blockNo] < 0) // this causes last block to NOT be read if file smaller than blocksToRead
            {
                break;
            }

            readOpenFileBlock(fileIndex, blockNo);
            memcpy(buf, openFile.buffer, BLOCK_SIZE);
            buf += BLOCK_SIZE;

            blockNo = myFAT->nextBlock[blockNo];
        }
    }

    // read last block (likely not full size)
    if (blockNo >= 0)
    {
        readOpenFileBlock(fileIndex, blockNo);
        int dataByteCount = size - blocksRead * BLOCK_SIZE;

        memcpy(buf, openFile.buffer, dataByteCount);
        blockNo = myFAT->nextBlock[blockNo];
    }
    else
    {
        // if EOT hit earlier
        size = blocksRead * BLOCK_SIZE;
    }

    return size;
}

/// @brief Write to a file.
///
/// Write a given number of bytes to a file starting at a given position.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// Note that the file content is an array of bytes, not a string. I.e., it is not (!) necessarily terminated by '\0'
/// and may contain an arbitrary number of '\0'at any position. Thus, you should not use strlen(), strcpy(), strcmp(),
/// ... on both the file content and buf, but explicitly store the length of the file and all buffers somewhere and use
/// memcpy(), memcmp(), ... to process the content.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] buf An array containing the bytes that should be written.
/// \param [in] size Number of bytes to write.
/// \param [in] offset Starting position in the file, i.e., number of the first byte to read relative to the first byte of
/// the file.
/// \param [in] fileInfo Can be ignored in Part 1 .
/// \return Number of bytes written on success, -ERRNO on failure.
int MyOnDiskFS::fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo)
{
    LOGM();
    // TODO: [PART 2] Implement this!
    int fileIndex = fileInfo->fh;
    const MyOnDiskFsFileInfo &file = files[fileIndex];

    // if not fitting resize
    if (file.size < size + offset)
    {
        reallocBlocks(path, size + offset, fileInfo);
    }

    int firstBlock = file.firstBlock;     // first block to write, so apply offset here
    int skipBlocks = offset / BLOCK_SIZE; // round down to get block e.g. 518 bytes in is still in the first block
    for (int i = 0; i < skipBlocks; i++)
    {
        firstBlock = myFAT->nextBlock[firstBlock];
    }

    // write first block
    readOpenFileBlock(fileIndex, firstBlock);
    char *w = new char[BLOCK_SIZE];
    memcpy(w, openFiles[fileIndex].buffer, BLOCK_SIZE);
    
    int inBlockOffset = offset - skipBlocks * BLOCK_SIZE;
    int bytesToWrite = BLOCK_SIZE - inBlockOffset;
    if (size < bytesToWrite)
    {
        bytesToWrite = size;
    }

    int writtenBytes = 0;
    memcpy(w + inBlockOffset, buf, bytesToWrite);
    
    writeDataBlock(firstBlock, w);
    memcpy(openFiles[fileIndex].buffer, w, BLOCK_SIZE); //Also update open file buffer

    writtenBytes = bytesToWrite;

    int blockNo = myFAT->nextBlock[firstBlock];
    while (writtenBytes < size)
    {
        if (blockNo < 0)
        {
            delete[] w;
            return (-errno);
        }

        if (size - writtenBytes > BLOCK_SIZE)
        {
            memcpy(w, buf + writtenBytes, BLOCK_SIZE);
            writeDataBlock(blockNo, w);
            blockNo = myFAT->nextBlock[blockNo];
            writtenBytes += BLOCK_SIZE;
        }
        else
        { // write last block
            readOpenFileBlock(fileIndex, blockNo);
            memcpy(w, openFiles[fileIndex].buffer, BLOCK_SIZE);
            bytesToWrite = size - writtenBytes;
            memcpy(w, buf + writtenBytes, bytesToWrite);

            writeDataBlock(blockNo, w);
            memcpy(openFiles[fileIndex].buffer, w, BLOCK_SIZE); //Also update open file buffer

            writtenBytes += bytesToWrite;
        }
    }
    delete[] w;

    // meta data
    files[fileIndex].mtimensec = time(NULL);
    files[fileIndex].ctimensec = time(NULL);
    updateFileMetaDataOnDisk(fileIndex);

    RETURN(size);
}

/// @brief Close a file.
///
/// \param [in] path Name of the file, starting with "/".
/// \param [in] File handel for the file set by fuseOpen.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseRelease(const char *path, struct fuse_file_info *fileInfo)
{
    LOGM();
    // TODO: [PART 2] Implement this!
    int fileIndex = fileInfo->fh;

    //free buffer again
    openFiles[fileIndex].blockNo = -1;
    delete[] openFiles[fileIndex].buffer;

    RETURN(0);
}

/// @brief Truncate a file.
///
/// Set the size of a file to the new size. If the new size is smaller than the old size, spare bytes are removed. If
/// the new size is larger than the old size, the new bytes may be random.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] newSize New size of the file.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseTruncate(const char *path, off_t newSize)
{
    LOGM();

    // TODO: [PART 2] Implement this!
    int fileIndex = findFileByName(path + 1);
    if (fileIndex < 0)
    {
        RETURN(-ENOENT);
    }

    fuse_file_info *fileInfo = new fuse_file_info();
    fileInfo->fh = fileIndex;
    fuseTruncate(path, newSize, fileInfo);
    delete fileInfo;

    RETURN(0);
}

/// @brief Truncate a file.
///
/// Set the size of a file to the new size. If the new size is smaller than the old size, spare bytes are removed. If
/// the new size is larger than the old size, the new bytes may be random. This function is called for files that are
/// open.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] newSize New size of the file.
/// \param [in] fileInfo Can be ignored in Part 1.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseTruncate(const char *path, off_t newSize, struct fuse_file_info *fileInfo)
{
    reallocBlocks(path, newSize, fileInfo);
}

/// @brief Read a directory.
///
/// Read the content of the (only) directory.
/// You do not have to check file permissions, but can assume that it is always ok to access the directory.
/// \param [in] path Path of the directory. Should be "/" in our case.
/// \param [out] buf A buffer for storing the directory entries.
/// \param [in] filler A function for putting entries into the buffer.
/// \param [in] offset Can be ignored.
/// \param [in] fileInfo Can be ignored.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo)
{
    LOGM();

    // TODO: [PART 2] Implement this!
    LOGF("--> Getting The List of Files of %s\n", path);

    filler(buf, ".", NULL, 0);  // Current Directory
    filler(buf, "..", NULL, 0); // Parent Directory

    for (int fileIndex = 0; fileIndex < NUM_DIR_ENTRIES; fileIndex++)
    {
        if (strcmp(files[fileIndex].name, "\0") != 0) // If the user is trying to show the files/directories of the root directory show the following
        {
            filler(buf, files[fileIndex].name, NULL, 0);
        }
    }

    RETURN(0);
}

/// Initialize a file system.
///
/// This function is called when the file system is mounted. You may add some initializing code here.
/// \param [in] conn Can be ignored.
/// \return 0.
void *MyOnDiskFS::fuseInit(struct fuse_conn_info *conn)
{
    // Open logfile
    this->logFile = fopen(((MyFsInfo *)fuse_get_context()->private_data)->logFile, "w+");
    if (this->logFile == NULL)
    {
        fprintf(stderr, "ERROR: Cannot open logfile %s\n", ((MyFsInfo *)fuse_get_context()->private_data)->logFile);
    }
    else
    {
        // turn of logfile buffering
        setvbuf(this->logFile, NULL, _IOLBF, 0);

        LOG("Starting logging...\n");

        LOG("Using on-disk mode");

        LOGF("Container file name: %s", ((MyFsInfo *)fuse_get_context()->private_data)->contFile);

        int ret = this->blockDevice->open(((MyFsInfo *)fuse_get_context()->private_data)->contFile);

        myDmap = new Dmap();
        myFAT = new FAT();
        mySuperBlock = new SuperBlock();

        if (ret >= 0)
        {
            LOG("Container file does exist, reading");

            // TODO: [PART 2] Read existing structures form file

            // read Superblock, D-Map, FAT
            readStructures();
        }
        else if (ret == -ENOENT)
        {
            LOG("Container file does not exist, creating a new one");

            ret = this->blockDevice->create(((MyFsInfo *)fuse_get_context()->private_data)->contFile);

            if (ret >= 0)
            {

                // TODO: [PART 2] Create empty structures in file
                calculateAndSetBlockPositions(DEFAULT_NUM_DATA_BLOCKS);

                //  set SuperBlock
                char *w = new char[BLOCK_SIZE];
                memcpy(w, mySuperBlock, sizeof(SuperBlock));
                ret = this->blockDevice->write(0, w);

                delete[] w;

                // write DMAP
                myDmap->blockSet = new bool[mySuperBlock->numDataBlocks];
                updateDmapOnDisk();

                // write FAT
                myFAT->nextBlock = new int[mySuperBlock->numDataBlocks];
                //-1 is EOT
                for (int i = 0; i < mySuperBlock->numDataBlocks; i++)
                {
                    myFAT->nextBlock[i] = -1;
                }
                updateFatOnDisk();

                // fill root directory with empty files, just to ensure less problems later down the line
                for (int i = 0; i < NUM_DIR_ENTRIES; i++)
                {
                    w = new char[BLOCK_SIZE];
                    memcpy(w, &files[i], sizeof(MyOnDiskFsFileInfo));
                    writeBlocks(mySuperBlock->rootBlock + i, 1, w);
                    delete[] w;
                }

                // fill in empty data for rest
                w = new char[BLOCK_SIZE];
                memset(w, 0, BLOCK_SIZE);
                for (int i = 0; i < mySuperBlock->numDataBlocks; i++)
                {
                    ret = this->blockDevice->write(mySuperBlock->dataBlock + i, w);
                }
            }
        }

        if (ret < 0)
        {
            LOGF("ERROR: Access to container file failed with error %d", ret);
        }
    }

    return 0;
}

/// @brief Clean up a file system.
///
/// This function is called when the file system is unmounted. You may add some cleanup code here.
void MyOnDiskFS::fuseDestroy()
{
    LOGM();

    // TODO: [PART 2] Implement this!
    delete mySuperBlock;
    delete myDmap;
    delete myFAT;
}

// TODO: [PART 2] You may add your own additional methods here!

int MyOnDiskFS::findFileByName(const char *name)
{
    for (int i = 0; i < NUM_DIR_ENTRIES; i++)
    {
        if (strcmp(files[i].name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void MyOnDiskFS::readStructures()
{
    // read super block
    char *r = new char[BLOCK_SIZE];
    int ret = readBlocks(0, 1, r);
    memcpy(mySuperBlock, r, sizeof(SuperBlock));
    delete[] r;

    // now we can read dmap and fat with given offsets
    myDmap->blockSet = new bool[mySuperBlock->numDataBlocks];
    r = new char[BLOCK_SIZE * mySuperBlock->dmapSize];
    ret = readBlocks(mySuperBlock->dmapBlock, mySuperBlock->dmapSize, r);
    memcpy(myDmap->blockSet, r, ROUNDUP(mySuperBlock->numDataBlocks, 8));
    delete[] r;

    myFAT->nextBlock = new int[mySuperBlock->numDataBlocks];
    r = new char[BLOCK_SIZE * mySuperBlock->fatSize];
    ret = readBlocks(mySuperBlock->fatBlock, mySuperBlock->fatSize, r);
    memcpy(myFAT->nextBlock, r, mySuperBlock->numDataBlocks * sizeof(int));
    delete[] r;

    for (int i = 0; i < NUM_DIR_ENTRIES; i++)
    {
        readFileMetaDataOnDisk(i);
    }
}

void MyOnDiskFS::readFileMetaDataOnDisk(int fileIndex)
{
    char *r = new char[BLOCK_SIZE];
    int ret = readBlocks(mySuperBlock->rootBlock + fileIndex, 1, r);
    memcpy(&files[fileIndex], r, sizeof(MyOnDiskFsFileInfo));
    delete[] r;
}

void MyOnDiskFS::readOpenFileBlock(int fileIndex, int blockNo)
{
    // check if block already buffered, if not read it
    if (openFiles[fileIndex].blockNo != blockNo)
    {
        blockDevice->read(blockNo + mySuperBlock->dataBlock, openFiles[fileIndex].buffer);
        openFiles[fileIndex].blockNo = blockNo;
    }
}

void MyOnDiskFS::calculateAndSetBlockPositions(int numBlocks)
{
    memcpy(mySuperBlock->name, "FAT420", sizeof("FAT420"));
    mySuperBlock->numDataBlocks = numBlocks;
    mySuperBlock->dmapSize = ROUNDUP((mySuperBlock->numDataBlocks + 7) / 8, BLOCK_SIZE); // cause booleans just bits
    mySuperBlock->fatSize = ROUNDUP(mySuperBlock->numDataBlocks * sizeof(int), BLOCK_SIZE);
    mySuperBlock->rootSize = NUM_DIR_ENTRIES;
    mySuperBlock->containerSize = mySuperBlock->numDataBlocks + /*Just Data*/
                                  mySuperBlock->dmapSize +
                                  mySuperBlock->fatSize +
                                  mySuperBlock->rootSize +
                                  1 /*Super Block*/;

    mySuperBlock->dmapBlock = 1;
    mySuperBlock->fatBlock = mySuperBlock->dmapBlock + mySuperBlock->dmapSize;
    mySuperBlock->rootBlock = mySuperBlock->fatBlock + mySuperBlock->fatSize;
    mySuperBlock->dataBlock = mySuperBlock->rootBlock + mySuperBlock->rootSize;
}

int MyOnDiskFS::writeBlocks(int startBlock, int numBlocks, char *buffer)
{
    for (int b = startBlock; b < startBlock + numBlocks; b++)
    {
        int ret = this->blockDevice->write(b, buffer + (b - startBlock) * BLOCK_SIZE);
        if (ret < 0)
        {
            RETURN(ret);
        }
    }
    RETURN(0);
}

// Given buffer needs to be large enough
int MyOnDiskFS::readBlocks(int startBlock, int numBlocks, char *buffer)
{
    for (int b = 0; b < numBlocks; b++)
    {
        int ret = this->blockDevice->read(b + startBlock, buffer + b * BLOCK_SIZE);
        if (ret < 0)
        {
            RETURN(ret);
        }
    }
    RETURN(0);
}

int MyOnDiskFS::updateFileMetaDataOnDisk(int fileIndex)
{
    char *w = new char[BLOCK_SIZE];

    memcpy(w, &files[fileIndex], sizeof(MyOnDiskFsFileInfo));
    int ret = writeBlocks(mySuperBlock->rootBlock + fileIndex, 1, w);
    delete[] w;
    RETURN(ret);
}

int MyOnDiskFS::reallocBlocks(const char *path, size_t newSize, struct fuse_file_info *fileInfo){
    LOGM();
    // TODO: [PART 2] Implement this!
    int fileIndex = fileInfo->fh;
    if (fileIndex < 0 || fileIndex > NUM_DIR_ENTRIES)
    {
        RETURN(-ENOENT);
    }

    MyOnDiskFsFileInfo &file = files[fileIndex];
    int numCurrentBlocks = ROUNDUP(file.size, BLOCK_SIZE);
    int numCompleteBlocks = ROUNDUP(newSize, BLOCK_SIZE);
    int numCreateBlocks = numCompleteBlocks - numCurrentBlocks;

    // make file larger
    if (numCreateBlocks > 0)
    {

        int blockNo = file.firstBlock;
        int searchBlock = 0; // just for first time 0, afterwards always last block +1

        // If file currently has no blocks allocate first one and reduce the ones to create below by one
        if (blockNo < 0)
        {
            blockNo = getNextFreeBlock(searchBlock);
            searchBlock = blockNo + 1;
            numCreateBlocks -= 1;
            myDmap->blockSet[blockNo] = true;
            file.firstBlock = blockNo;
        }

        for (int i = 0; i < numCurrentBlocks-1; i++)
        {
            blockNo = myFAT->nextBlock[blockNo];
        }

        for (int i = 0; i < numCreateBlocks; i++)
        {
            int newBlockNo = getNextFreeBlock(searchBlock);
            if (newBlockNo < 0)
            {
                return -errno;
            }

            myDmap->blockSet[newBlockNo] = true;
            myFAT->nextBlock[blockNo] = newBlockNo;
            blockNo = newBlockNo;
            searchBlock = blockNo + 1;
        }

        myFAT->nextBlock[blockNo] = -1;
    }
    //make file smaller
    else if(numCreateBlocks < 0){
        int blockNo = file.firstBlock;
        for (int i = 0; i < numCompleteBlocks-1; i++)
        {
            blockNo = myFAT->nextBlock[blockNo];
        }
        for (int i = 0; i < -numCreateBlocks; i++)
        {   
            int nextBlockNo= myFAT->nextBlock[blockNo];
            
            //just free in maps
            myFAT->nextBlock[blockNo]=-1;
            myDmap->blockSet[blockNo]=false;

            blockNo = nextBlockNo;
        }
        

    }

    file.size = newSize;
    updateDmapOnDisk();
    updateFatOnDisk();
    updateFileMetaDataOnDisk(fileIndex);
    RETURN(0);
}

int MyOnDiskFS::updateDmapOnDisk()
{
    char *w = new char[mySuperBlock->dmapSize * BLOCK_SIZE];
    memcpy(w, myDmap->blockSet, ROUNDUP(mySuperBlock->numDataBlocks, 8));
    int ret = writeBlocks(mySuperBlock->dmapBlock, mySuperBlock->dmapSize, w);
    delete[] w;
    RETURN(ret);
}
int MyOnDiskFS::updateFatOnDisk()
{
    char *w = new char[mySuperBlock->fatSize * BLOCK_SIZE];
    memcpy(w, myFAT->nextBlock, mySuperBlock->numDataBlocks * sizeof(int));
    int ret = writeBlocks(mySuperBlock->fatBlock, mySuperBlock->fatSize, w);
    delete[] w;
    RETURN(ret);
}

int MyOnDiskFS::getNextFreeBlock(int block)
{
    if (block < 0)
    {
        return -1;
    }

    while (block < mySuperBlock->numDataBlocks)
    {
        if (myDmap->blockSet[block] == false)
        {
            return block;
        }
        block++;
    }

    return -1;
}

int MyOnDiskFS::writeDataBlock(int blockNo, char *buffer)
{
    if (blockNo < 0)
    {
        return -errno;
    }

    blockDevice->write(blockNo + mySuperBlock->dataBlock, buffer);
}

// DO NOT EDIT ANYTHING BELOW THIS LINE!!!

/// @brief Set the static instance of the file system.
///
/// Do not edit this method!
void MyOnDiskFS::SetInstance()
{
    MyFS::_instance = new MyOnDiskFS();
}
