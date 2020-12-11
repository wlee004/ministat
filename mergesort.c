// Adapted from https://stackoverflow.com/questions/52767944/merge-sort-with-pthreads-in-c

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

// thread control parameters
struct task {
    int task_no;
    int task_low;
    int task_high;
    double *b_result;
};

// merge function for merging two parts
void
merge(int low, int mid, int high, double * b_result)
{
    // n1 is size of left part and n2 is size of right part
    int n1 = mid - low + 1;
    int n2 = high - mid;

    double *left = malloc(n1 * sizeof(double));
    double *right = malloc(n2 * sizeof(double));
    
    if (left == NULL || right == NULL) {
        printf("couldnt malloc left and rigth");
        exit(1);
    }

    int i;
    int j;

    // storing values in left part
    for (i = 0; i < n1; i++) {
        left[i] = b_result[i + low];
    }

    // storing values in right part
    for (i = 0; i < n2; i++)
        right[i] = b_result[i + mid + 1];

    int k = low;
    i = j = 0;

    // merge left and right in ascending order
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            b_result[k++] = left[i++];
        else
            b_result[k++] = right[j++];
    }

    // insert remaining values from left
    while (i < n1)
        b_result[k++] = left[i++];

    // insert remaining values from right
    while (j < n2)
        b_result[k++] = right[j++];

    free(left);
    free(right);
}

// merge sort function
void
merge_sort(int low, int high, double *b_result)
{
    // calculating mid point of array
    int mid = low + (high - low) / 2;

    if (low < high) {
        // calling first half
        merge_sort(low, mid, b_result);

        // calling second half
        merge_sort(mid + 1, high, b_result);

        // merging the two halves
        merge(low, mid, high, b_result);
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
        merge_sort(low, mid, task->b_result);
        merge_sort(mid + 1, high, task->b_result);
        merge(low, mid, high, task->b_result);
    }

    return 0;
}

// Driver Code
int
multithreaded_mergsesort(double *a_arr, unsigned int n, double * b_result)
{
    struct task *task;

    int MAX = n;
    int THREAD_MAX = 1;
    
    for (int i = 0; i < MAX; i++)
        b_result[i] = a_arr[i];

    pthread_t threads[THREAD_MAX];
    struct task tasklist[THREAD_MAX];
    int len = floor(MAX / THREAD_MAX);

    int low = 0;
    
    // split the data into len-sized arrays for each thread to process
    for (int i = 0; i < THREAD_MAX; i++, low += len) {
        if ((i == THREAD_MAX - 1) && (MAX % 2 != 0)) {
            len += 1;
        }
        task = &tasklist[i];
        task->task_no = i;
        task->task_low = low;
        task->task_high = low + len - 1;
        task->b_result = b_result;
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

    // merging the final 4 parts
    struct task *taskm = &tasklist[0];
    for (int i = 1; i < THREAD_MAX; i++) {
        struct task *task = &tasklist[i];
        merge(taskm->task_low, task->task_low - 1, task->task_high, task->b_result);
    }

        return 0;
}
