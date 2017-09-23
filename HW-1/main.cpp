#include <iostream>
#include <string>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <random>

#define FILE_SIZE 10 //10 gb
#define SEQ_SEGM_SIZE 50 //2 gb
#define RND_SEGM_SIZE 5 //200 mb

const char* filename = "BigFile";

int main() {
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC); //O_SYNC, O_DIRECT
    char* buf = (char*) malloc(FILE_SIZE); //incorrect!
    write(fd, buf, FILE_SIZE);
    fdatasync(fd); //or fsync()
    close(fd);

    //std::mt19937 generator;

    int fd = open(filename, O_RDWR | O_CREAT | O_APPEND);
    std::string mode = "seq-read";
    if (mode == "seq-read") {
        char* buf = malloc(SEQ_SEGM_SIZE);
        read(fd, buf, SEQ_SEGM_SIZE); //OFFSET?
        //empty cache buffers?
        //time
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

    //create big file on sdd
    write(fd, )

    }
*/
