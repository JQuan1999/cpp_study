#include"common.h"

int test(int a, int b){
    int c = a + b;
    LOG(DEBUG, "a = %d, b = %d, result = %d", a, b, c);
    return 0;
}

int main()
{
    unsigned long long id = pthread_self();
    printf("print id in 10 = %lld, in dec = %llx\n", id, id);
    return 0;
}