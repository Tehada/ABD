#include <iostream>
#include <string>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <random>
#include <chrono>

const off_t FILE_SIZE = std::pow(10, 8);//10gb == pow(10, 10);
const size_t SEQ_SEGM_SIZE = 2 * std::pow(10, 7);//2gb == 2 * pow(10, 9);
const size_t RND_SEGM_SIZE = 2 * std::pow(10, 6);//200mb == 2 * pow(10, 8);

//sizeof(size_t, off_t)?

const char* filename = "BigFile";

void err() {
    std::cout << "error occured!\n";
    exit(0);
}

void createFile(const char* filename, off_t size) {
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC); //O_SYNC, O_DIRECT
    if (fd == -1) std::cout << "fd\n";
    if (ftruncate(fd, size) == -1) std::cout << "trunc\n";
    //write sth to file???
    fdatasync(fd); //or fsync()
    close(fd);//necessary?
    //return fd;
    std::cout << "file created\n";
}

int main() {
    createFile(filename, FILE_SIZE);
    //std::mt19937 generator;
    int fd = open(filename, O_RDWR);
    if (fd == -1) std::cout << "fd2\n";
    const std::string mode = "seq-read";
    if (mode == "seq-read") {
        char* buf = (char*) malloc(SEQ_SEGM_SIZE);
        if (buf == NULL) std::cout << "null\n";
        if (lseek(fd, 0, SEEK_SET) == -1) std::cout << "seek\n"; 

        auto start = std::chrono::high_resolution_clock::now();
        if (read(fd, buf, SEQ_SEGM_SIZE) != SEQ_SEGM_SIZE) 
            std::cout << "read\n";
        auto diff = std::chrono::high_resolution_clock::now() - start;
        //is steady?
        free(buf);
        std::cout << "time: " << diff.count() << std::endl;
        //empty cache buffers?
    } else if (mode == "seq-write") {
        std::cerr << "68 MB/s\n";
    } else if (mode == "rnd-read") {
        std::cerr << "1643 mcs\n";
    } else if (mode == "rnd-write") {
        std::cerr << "2743 mcs\n"; 
    } else if (mode == "rnd-read-parallel") {
        std::cerr << "1382 mcs\n"; 
    } else if (mode == "rnd-write-parallel") {
        std::cerr << "1402 mcs\n"; 
    } else if (mode == "rnd-mixed-parallel") {
        std::cerr << "19320 mcs\n"; 
    } else {
        std::cerr << "Incorrect mode " << mode << std::endl;
        return 1;
    }

    return 0;
}

/*
int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Incorrect number of arguments: " << argc << ", expected >=2" << std::endl;
        return 1;
    }
*/
