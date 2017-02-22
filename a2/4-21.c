#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct s {
    int len;
    int *numbers;
    int avg;
    int min;
    int max;
};

void *average(void *n)
{
    struct s *nums = (struct s*)n;
    nums->avg = 0;
    for (int i=0; i<nums->len; i++) {
        nums->avg += nums->numbers[i];
    }
    nums->avg /= nums->len;
    return NULL;
}
    
void *minimum(void *n)
{
    struct s *nums = (struct s*)n;
    nums->min = nums->numbers[0];
    for (int i=1; i<nums->len; i++) {
        if (nums->numbers[i] < nums->min) {
            nums->min = nums->numbers[i];
        }
    }
    return NULL;
}

void *maximum(void *n)
{
    struct s *nums = (struct s*)n;
    nums->max = nums->numbers[0];
    for (int i=1; i<nums->len; i++) {
        if (nums->numbers[i] > nums->max) {
            nums->max = nums->numbers[i];
        }
    }
    return NULL;
}

int main(int argc, char **argv)
{
    struct s *nums = malloc(sizeof(struct s));
    nums->len = argc-1;
    nums->numbers = malloc(sizeof(int)*nums->len);
    
    for (int i=0; i<nums->len; i++) {
        nums->numbers[i]=atoi(argv[i+1]);
    }

    pthread_t avg;
    pthread_t min;
    pthread_t max;

    pthread_create(&avg, NULL, average, nums);
    pthread_create(&min, NULL, minimum, nums);
    pthread_create(&max, NULL, maximum, nums);

    pthread_join(avg, NULL);
    pthread_join(min, NULL);
    pthread_join(max, NULL);

    printf("The average value is %d\n", nums->avg);
    printf("The minimum value is %d\n", nums->min);
    printf("The maximum value is %d\n", nums->max);



}
