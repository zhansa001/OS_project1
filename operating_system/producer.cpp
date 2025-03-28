//producer.cpp
//Sandy Zhang
//3/28/25

#include "shared.hpp"

int main() {
    // Declaring the shared memory
    const char* share = "/sharedMemObject";

    shm_unlink(share);
    int file = shm_open(share, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    // Fails to open
    if (file == -1) {
        std::cerr << "Producer: Failed to open shared memory (file == -1)" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Allocating memory for shared structure
    if (ftruncate(file, sizeof(struct table)) == -1) {
        std::cerr << "Producer: Failed to set size of shared memory (ftruncate == -1)" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Map shared memory
    table* mem = static_cast<table*>(mmap(nullptr, sizeof(*mem), PROT_READ | PROT_WRITE, MAP_SHARED, file, 0));
    if (mem == MAP_FAILED) {
        perror("Producer: mmap failed");
        std::cerr << "Producer: Map failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Initialize semaphores in shared memory
    if (sem_init(&mem->full, 1, 0) == -1) { // Not full initially
        std::cerr << "Producer: Semaphore 'full' initialization failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (sem_init(&mem->empty, 1, 2) == -1) { // Empty initially
        std::cerr << "Producer: Semaphore 'empty' initialization failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (sem_init(&mem->S, 1, 1) == -1) { // Binary semaphore for mutual exclusion
        std::cerr << "Producer: Semaphore 'S' initialization failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Random number generator for items
    srand(time(nullptr));

    // Produce 6 items
    for (int i = 0; i < 6; ++i) {
        sem_wait(&mem->empty); // Wait for an empty slot
        sem_wait(&mem->S);   // Lock access to shared memory

        // Generate random number
        int X = rand() % 100;

        // Store in shared buffer
        mem->table[mem->item] = X;
        std::cout << "Produced: " << mem->table[mem->item] << std::endl;

        mem->item++; // Increment the buffer index

        sem_post(&mem->S);     // Signal mutex release
        sem_post(&mem->full);  // Signal that a new item is available
    }

    // Clean exit
    return EXIT_SUCCESS;
}