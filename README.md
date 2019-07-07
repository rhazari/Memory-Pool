## How to use ##
1. git clone https://github.com/rhazari/Memory-Pool.git
2. cd Memory-Pool
3. mkdir build
4. cd build
5. cmake ..
6. make

### Run the unit test ###
./runUnitTests

### Run the code in main.cpp ###
./MemoryPool

### Summary ###
The code in **main.cpp** does a simulation run of 10,000 iterations

**Case 1** We attempt to allocate memory to 20 objects utilizing 20 threads. Each of the threads
makes a call to a single function which returns a pointer to the newly created object.

**Case 2** We test the simulation run for the MemoryPool, which we initialize with 20 threads.
We attempt to allocate memory to 20 objects utilizing the MemoryPool.

**Case 3** In this scenario we initialize the MemoryPool with 10 threads and attempt to allocate memory to 20 objects utilizing the MemoryPool.

For all the scenarios we only attempt to calculate the elapsed time for the object creation process.




