## Senior Design II
 	Team G
 	William Lee, Sarah Edkins, Sagun Pandey, Tejnoor Singh
 	Git Repository Link: https://github.com/wlee004/ministat

## Optimization of ministat
### Goal:
	The main objective of this project is to optimize ministat to take advantage of modern CPU features
	and compiler optimizations.

### Original ministat Perf Report
![Original Perf Report](https://github.com/wlee004/ministat/blob/master/screenshots/Original_perf1.png)
<!---![Original Perf Report](https://github.com/wlee004/ministat/blob/master/screenshots/Original_perf2.png)--->

### Original ministat Flame Graph
![Original Flamegraph](https://raw.githubusercontent.com/wlee004/ministat/master/screenshots/perf-ministat.svg)

### Optimized ministat Perf Report
![Optimized Perf Report](https://github.com/wlee004/ministat/blob/master/screenshots/Optimized/Optimized_perf1.png)
<!--- ![Optimized Perf Report](https://github.com/wlee004/ministat/blob/master/screenshots/Optimized/Optimizedperf2.png)--->

### Optimized ministat Flame Graph
![Optimized Flamegraph](https://raw.githubusercontent.com/wlee004/ministat/master/screenshots/Optimized/perf-ministat.svg)

### Strategies for optimization:
	We did the following to optimize our version of ministat.
	
### 1) Implemented Micro-optimizations

- We implemented a new data structure for inserting new data points by changing the algorithm to just use realloc without using calloc or memcpy.
- Replaced the usage of qsort with an_qsort for a faster sorting algorithm for the dataset.
- Implemented a new option “-v” that emits verbose timing data. This function is used to time the ReadSet function as that is the main function that’s being used in the program where most of the computation was performed.
- Replaced C library FILE (fopen, fclose, etc.) with faster system calls of open, read, close for ReadSet(). This allowed the reading of a much larger buffer however truncation was to be accounted for where as before the program read byte by byte until a whole double value was read.
Truncation was addressed by saving the first half of the double and setting a truncation flag to be true. When the next read occurs the truncation flag would be checked and it would append the rest of the value to the previous hence, truncation. 
- Replaced strtod() with strtod_fast() which was from an open source repository called dtoa-fast. This provided a faster conversion from string to double.
-Replaced strtok() with strsep() which made the is better since strtok() is not thread-safe which didn't allow us to perform multithreading on ReadSet() and strsep() did. 

### 2) Implemented a Multi-thread architecture

- Assigned one thread to each file that the ministat function is computing
- In the ReadSet function, where all the reading of a file took place. Two threads were created to split the file into segments for each thread to read concurrently. When splitting the file, truncation is accounted for so some threads might be reading more than others by some number of bytes but all threads are ensured to be reading full double values.
- Implemented a multithreaded merge sort. The calling function allocates memory for the sorted result data and passes a pointer to it along with the unsorted data into the sort function. The sort function divides the data into 6 threads and calls merge sort on them, updating the data in the memory pointed to by the result pointer. When all threads return, each smaller sorted array is then merged into the final sorted result, and the function returns to the calling function, which has the sorted result.  

### Performance before Optimization
![Before Optimization](https://github.com/wlee004/ministat/blob/master/screenshots/OriginalPerformance.png)


### Performance after Optimization
![After Optimization](https://github.com/wlee004/ministat/blob/master/screenshots/Optimized/OptimizedPerformance.png)

### Conclusion

- An optimization that was most beneficial was on ReadSet. The original perf report on ReadSet took about 6.5% of the cpu and our optimized version takes less than 1% of the cpu. By implementing a raw I/O we were able to reduce the throughput on the cpu, this is important as most of the computation of the program is being performed on ReadSet. With the optimizations mentioned above this helped reduce the cpu usgae of main function considerably as well. An optimization that that was least effective was to AddPoint. The overall cpu usage of AddPoint increased after changing calloc to realloc, although this may have improved memory usage perhaps realloc has more overhead therefore resulted higher cpu usage as AddPoint is called very often. 

- Challenges that we encountered was getting the multithreading to work on the reading of each file. The current main function is coded to run on only two threads per file. A more dynamic version of multithreading was attempted to run any number of threads per file read however it ran into issues of a thread reading outside the file even when parameters given to the thread seemed to be correct. Another challenege we faced was multithreaded sorting. We implemented multithreaded merge-sort which ran faster than the original qsort function however it fell short of the an_qsort function so we resorted in keeping the an_qsort. 
