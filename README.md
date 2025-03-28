Sandy Zhang

Operating System Assignment 1:

Producer-Consumer Problem

# Overview
This project stimulates the producer-consumer problem.

The producer generates items and puts them onto the table. The consumer will pick up items. 
The table can only hold two items at a same time. When the table is complete, the producer will wait.
When there are no items, the consumer will wait.

The project includes a header file (shared.hpp) that includes directive, the definition of table, structure, which stores 3 semaphores, a counter integer, and a static array of size 2. The producer.cpp for producer, consumer.cpp for consumer, and example.txt with the outputs.

It utilizes semaphores to synchronize producer and consumer, shared memories, and pthreads.

# Compilation
The code was written (C++) in VSCode and run in Linux environment. 

To compile, it uses the following commands:

g++ producer.cpp -pthread -lrt -o producer 

g++ consumer.cpp -pthread -lrt -o consumer

./producer & ./consumer

It might fail the first time, but works for the second time.
