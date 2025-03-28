//consummer.cpp
//Sandy Zhang
//3/28/25

#include "shared.hpp"

int main() {
    // Declaring the shared memory
    const char* share = "/sharedMemObject";

    int file = shm_open(share, O_RDWR, S_IRUSR | S_IWUSR);

    // Fails to open
    if (file == -1) {
        perror("Consumer: Failed to open shared memory");
        std::cerr << "Consumer: File == -1" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Map shared memory
    table* mem = static_cast<table*>(mmap(nullptr, sizeof(*mem), PROT_READ | PROT_WRITE, MAP_SHARED, file, 0));
    if (mem == MAP_FAILED) {
        perror("Consumer: mmap failed");
        std::cerr << "Consumer: Map Failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Consume items
    for (int i = 0; i < 6; ++i) {
        sem_wait(&mem->full); // Wait for an item to be available
        sem_wait(&mem->S);   // Lock access to shared memory

        // Take item from the buffer
        mem->item--;
        int Y = mem->table[mem->item];

        // Print buffer state
        std::cout << "Consumer: Buffer state: ";
        for (int j = 0; j < 2; ++j) {
            std::cout << mem->table[j] << " ";
        }
        std::cout << std::endl;

        // Reset the consumed slot in the buffer
        mem->table[mem->item] = 0;

        // Print consumed item
        std::cout << "Consumer: " << Y << std::endl;

        sem_post(&mem->empty); // Signal that an empty slot is available
        sem_post(&mem->S);     // Unlock access to shared memory
    }

    // Clean up and unlink shared memory
    shm_unlink(share);

    return EXIT_SUCCESS;
}