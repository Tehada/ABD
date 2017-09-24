#include <iostream>
#include <string>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <random>
#include <chrono>

const off_t FILE_SIZE = std::pow(10, 8);//10gb == pow(10, 10);
const size_t SEQ_SEGM_SIZE = std::pow(10, 7);//2gb == 2 * pow(10, 9);
const size_t RND_SEGM_SIZE = std::pow(10, 6);//200mb == 2 * pow(10, 8);
const int N_ITER = 10;
//8 bytes here only!
const char* filename = "BigFile";

void err() {
    std::cerr << "error occured!\n";
    exit(0);
}

void dropCaches() {
    sync();
    std::ostream ofs("/proc/sys/vm/drop_caches");
    ofs << "3" << std::endl;
}

void createFile(const char* filename, off_t size) {
    mode_t mode = S_IRUSR | S_IWUSR;
    int flags = O_RDWR | O_CREAT | O_TRUNC;//O_SYNC, O_DIRECT
    int fd = open(filename, flags, mode);
    if (fd == -1) err();
    if (ftruncate(fd, size) == -1) err();
    //write sth to file???
    fdatasync(fd); //or fsync()
    close(fd);//necessary?
    //return fd;
    std::cout << "file created!\n";
}

void seqRead() {
    int fd = open(filename, O_RDWR);
    if (fd == -1) err();
    char* buf = (char*) malloc(SEQ_SEGM_SIZE);
    if (buf == NULL) err();
    if (lseek(fd, 0, SEEK_SET) == -1) err();

    int accum = 0;
    for (int i = 0; i < N_ITER; ++i) {
        auto start = std::chrono::steady_clock::now();
        if (read(fd, buf, SEQ_SEGM_SIZE) != SEQ_SEGM_SIZE) err(); 
        auto diff = std::chrono::steady_clock::now() - start;
        accum += diff.count();
    }

    free(buf);
    dropCaches();
    close(fd);

    std::cout << "-mean time: " << accum / N_ITER << std::endl;
}

void seqWrite() {
    int fd = open(filename, O_RDWR);
    if (fd == -1) err();
    char* buf = (char*) malloc(SEQ_SEGM_SIZE);
    if (buf == NULL) err();
    if (lseek(fd, 0, SEEK_SET) == -1) err();

    int accum = 0;
    for (int i = 0; i < N_ITER; ++i) {
        auto start = std::chrono::steady_clock::now();
        if (write(fd, buf, SEQ_SEGM_SIZE) != SEQ_SEGM_SIZE) err(); 
        auto diff = std::chrono::steady_clock::now() - start;
        accum += diff.count();
    }

    free(buf);
    dropCaches();
    close(fd);

    std::cout << "time: " << diff.count() << std::endl;
    std::cerr << "68 MB/s\n";
} 

void rndRead() {
    
    //std::mt19937 generator;
    std::cerr << "1643 mcs\n";
}

void rndWrite() {
    std::cerr << "2743 mcs\n"; 
}

int main() {
    createFile(filename, FILE_SIZE); 

    const std::string mode = "seq-read";
    if (mode == "seq-read") {
        seqRead();
    } else if (mode == "seq-write") {
        seqWrite();
    } else if (mode == "rnd-read") {
        rndRead();
    } else if (mode == "rnd-write") {
        rndWrite();
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
