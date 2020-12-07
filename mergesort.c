// Adapted from https://stackoverflow.com/questions/52767944/merge-sort-with-pthreads-in-c

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

double *global_a;

// thread control parameters
struct task {
    int task_no;
    int task_low;
    int task_high;
};

// merge function for merging two parts
void
merge(int low, int mid, int high)
{
    // n1 is size of left part and n2 is size of right part
    int n1 = mid - low + 1;
    int n2 = high - mid;

    double *left = malloc(n1 * sizeof(double));
    double *right = malloc(n2 * sizeof(double));

    int i;
    int j;

    // storing values in left part
    for (i = 0; i < n1; i++) {
        left[i] = global_a[i + low];
    }

    // storing values in right part
    for (i = 0; i < n2; i++)
        right[i] = global_a[i + mid + 1];

    int k = low;
    i = j = 0;

    // merge left and right in ascending order
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            global_a[k++] = left[i++];
        else
            global_a[k++] = right[j++];
    }

    // insert remaining values from left
    while (i < n1)
        global_a[k++] = left[i++];

    // insert remaining values from right
    while (j < n2)
        global_a[k++] = right[j++];

    free(left);
    free(right);
}

// merge sort function
void
merge_sort(int low, int high)
{
    // calculating mid point of array
    int mid = low + (high - low) / 2;

    if (low < high) {
        // calling first half
        merge_sort(low, mid);

        // calling second half
        merge_sort(mid + 1, high);

        // merging the two halves
        merge(low, mid, high);
    }
}

// thread function for multi-threading
void *
merge_sort_thread_fn(void *arg)
{
    struct task *task = arg;
    int low;
    int high;

    // calculating low and high
    low = task->task_low;
    high = task->task_high;

    // evaluating mid point
    int mid = low + (high - low) / 2;

    if (low < high) {
        merge_sort(low, mid);
        merge_sort(mid + 1, high);
        merge(low, mid, high);
    }

    return 0;
}

// Driver Code
int
multithreaded_mergsesort(double *a_arr, unsigned int n, double * b_result)
{
    struct task *task;

    int MAX = 100;
    int THREAD_MAX = 4;

    global_a = malloc(sizeof(double) * n);
    for (int i = 0; i < MAX; i++)
        global_a[i] = a_arr[i];

    pthread_t threads[THREAD_MAX];
    struct task tasklist[THREAD_MAX];
    int len = MAX / THREAD_MAX;

    int low = 0;
    
    // split the data into len-sized arrays for each thread to process
    for (int i = 0; i < THREAD_MAX; i++, low += len) {
        task = &tasklist[i];
        task->task_no = i;
        task->task_low = low;
        task->task_high = low + len - 1;
        if (i == (THREAD_MAX - 1))
                task->task_high = MAX - 1;
    }

    // creating 4 threads
    for (int i = 0; i < THREAD_MAX; i++) {
        task = &tasklist[i];
        pthread_create(&threads[i], NULL, merge_sort_thread_fn, task);
    }

    // joining all 4 threads
    for (int i = 0; i < THREAD_MAX; i++)
        pthread_join(threads[i], NULL);

        // show the array values for each thread
        //     for (int i = 0; i < THREAD_MAX; i++) {
        //         task = &tasklist[i];
        //         printf("SUB %d:", task->task_no);
        //         for (int j = task->task_low; j <= task->task_high; ++j)
        //         printf(" %d\n", a[j]);
        //     }

    // merging the final 4 parts
    struct task *taskm = &tasklist[0];
    for (int i = 1; i < THREAD_MAX; i++) {
        struct task *task = &tasklist[i];
        merge(taskm->task_low, task->task_low - 1, task->task_high);
    }

        // displaying sorted array
        //     printf("\n\nSorted array:");
        //     for (int i = 0; i < MAX; i++) {
        //         printf(" %f\n", global_a[i]); 
        //     }
    
    // Update b with return result
    for (int i = 0; i < n; i++) {
        b_result[i] = global_a[i];
    }

        return 0;
}

// Uncomment main to run only this file to test it.
// Compile: gcc -o mergesort mergesort.c -lpthread

// int
// main(int argc, char **argv)
// {
//         double a_arr[10] = {8075,44,3795,6193,6276,126,10,1346,92543,1772};
//         unsigned int n = 10;
//         double *b_result = malloc(sizeof(double) * n);
        
//         for (int i = 0; i < n; i++) {
//             printf("before: %f\n", a_arr[i]);
//         }
        
//         multithreaded_mergsesort(a_arr, n, b_result);
        
//         for (int i = 0; i < n; i++) {
//             printf("after: %f\n", b_result[i]);
//         }
//         return 0;
// }
