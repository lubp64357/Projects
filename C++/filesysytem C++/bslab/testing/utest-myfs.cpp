//
//  test-myfs.cpp
//  testing
//
//  Created by Oliver Waldhorst on 15.12.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//
#include "../catch/catch.hpp"

#include "tools.hpp"
#include "myfs.h"

#include "../catch/catch.hpp"

#include <stdio.h>
#include <string.h>

#include "tools.hpp"

#include "myinmemoryfs.h"

#define MYFS_PATH "/myfs.bin"
#define NUM_TESTBLOCKS 1024
#define BLOCK_SIZE 512

// Declarations of helper functions
void myfsWriteRead(MyFS *myfs, int noBlocks=1);

TEST_CASE( "MYFS_CREATE_WRITE_READ_NEW_FILE", "[myFs]" ) {

   // remove(MYFS_PATH);

    mode_t mode = S_IRWXU;
    MyInMemoryFS myFs;
    REQUIRE(myFs.fuseMknod(MYFS_PATH, mode, 0) == 0);

    SECTION("write single block") {
        myfsWriteRead(&myFs);
    }

    SECTION("write multiple blocks") {
        myfsWriteRead(&myFs, NUM_TESTBLOCKS);
    }

    REQUIRE(myFs.fuseRelease(MYFS_PATH, nullptr) == 0);
    //remove(MYFS_PATH);

}

TEST_CASE( "MYFS_CREATE_WRITE_READ_EXISTING_FILE", "[myFs]" ) {

    // Write to new file

    //remove(MYFS_PATH);
    mode_t mode = S_IRWXU;
    MyInMemoryFS myFs1;
    REQUIRE(myFs1.fuseMknod(MYFS_PATH, mode, 0) == 0);

    myfsWriteRead(&myFs1, NUM_TESTBLOCKS);

    REQUIRE(myFs1.fuseRelease(MYFS_PATH, nullptr) == 0);

    // Open existing file

    MyInMemoryFS myFs2;
    REQUIRE(myFs2.fuseMknod(MYFS_PATH, mode, 0) == 0);

    myfsWriteRead(&myFs2, NUM_TESTBLOCKS);

    REQUIRE(myFs2.fuseRelease(MYFS_PATH, nullptr) == 0);

    // Create on existing file
    MyInMemoryFS myFs3;
    REQUIRE(myFs3.fuseMknod(MYFS_PATH, mode, 0) == 0);

    myfsWriteRead(&myFs3, NUM_TESTBLOCKS);

    REQUIRE(myFs3.fuseRelease(MYFS_PATH, nullptr) == 0);

}

TEST_CASE( "MYFS_OPEN_NON-EXISTING_FILE", "[myfs]" ) {

    MyInMemoryFS myFs;
    REQUIRE(myFs.fuseOpen(MYFS_PATH, nullptr) < 0);
}

// ***
// *** Helper functions
// ***

void myfsWriteRead(MyFS *myfs, int noBlocks) {
    char* r= new char[BD_BLOCK_SIZE * noBlocks];
    memset(r, 0, BD_BLOCK_SIZE * noBlocks);

    char* w= new char[BD_BLOCK_SIZE * noBlocks];
    gen_random(w, BD_BLOCK_SIZE * noBlocks);

    // write all blocks
    int offset = 0;
    for(int b= 0; b < noBlocks; b++) {
        REQUIRE(myfs->fuseWrite(MYFS_PATH, w, BD_BLOCK_SIZE * noBlocks, offset, nullptr) == BD_BLOCK_SIZE * noBlocks);
        offset += strlen(w);
    }

    // read all blocks
    offset = 0;
    for(int b= 0; b < noBlocks; b++) {
        REQUIRE(myfs->fuseRead(MYFS_PATH, r, BD_BLOCK_SIZE * noBlocks, offset, nullptr) == BD_BLOCK_SIZE * noBlocks);
        offset += strlen(r);
    }

    REQUIRE(memcmp(w, r, BD_BLOCK_SIZE * noBlocks) == 0);

    delete [] r;
    delete [] w;
}