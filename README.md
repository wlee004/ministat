## Senior Design II
 	Team G
 	William Lee, Sarah Edkins, Sagun Pandey, Tejnoor Singh
 	Git Repository Link: https://github.com/wlee004/ministat

## Optimization of ministat
### Goal:
	The main objective of this project is to optimize ministat to take advantage of modern CPU features and compiler optimizations.

### - Original ministat Perf Report

### - Original ministat Flame Graph

### - Optimized ministat Perf Report

### - Optimized ministat Flame Graph
![Flamegraph](https://raw.githubusercontent.com/wlee004/ministat/blob/master/perf-ministat.svg)

### Strategies for optimization:
	We did the following to optimize our version of ministat.
	
### 1) Implemented Micro-optimizations

- We implemented a new data structure for inserting new data points by changing the algorithm to just use realloc without using calloc or memcpy.
- Replaced the usage of qsort with an_qsort for a faster sorting algorithm for the dataset.
- Implemented a new option “-v” that emits verbose timing data. This function is used to time the ReadSet function as that is the main function that’s being used in the program where most of the computation was performed.
- Replaced C library FILE (fopen, fclose, etc.) with faster system calls of open, read, close for ReadSet(). This allowed the reading of a much larger buffer however truncation was to be accounted for where as before the program read byte by byte until a whole double value was read.
Truncation was addressed by saving the first half of the double and setting a truncation flag to be true. When the next read occurs the truncation flag would be checked and it would append the rest of the value to the previous hence, truncation. 
- Replaced strtod() with strtod_fast() which was from an open source repository called dtoa-fast. This provided a faster conversion from string to double.

### 2) Implemented a Multi-thread architecture

- Assigned one thread to each file that the ministat function is computing
- In the ReadSet function, where all the reading of a file took place. More threads were created to split the file into segments for each thread to read concurrently. When splitting the file, truncation is accounted for so some threads might be reading more than others by some number of bytes but all threads are ensured to be reading full double values.
- Implemented a multithreaded merge sort. The calling function allocates memory for the sorted result data and passes a pointer to it along with the unsorted data into the sort function. The sort function divides the data into 6 threads and calls merge sort on them, updating the data in the memory pointed to by the result pointer. When all threads return, each smaller sorted array is then merged into the final sorted result, and the function returns to the calling function, which has the sorted result.  

### Performance before Optimization

### Performance after Optimization

