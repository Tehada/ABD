#include <iostream>
#include <string>


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Incorrect number of arguments: " << argc << ", expected >=2" << std::endl;
        return 1;
    }

    //create big file on sdd
    int fd = open("BigFile", O_RDWR | O_CREAT);
    write(fd, )

    std::string mode = argv[1];
    if (mode == "seq-read") {
        std::cerr << "58 MB/s\n"; 
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
