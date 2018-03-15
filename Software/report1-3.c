
//
//  main.c
//  Sorting
//

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void swap(int* a, int* b) {
	int c;
	c = *a;
	*a = *b;
	*b = c;
}

void insertionSort(int* target, int left, int right) {
	int i;
	int j;
	for (i = left; i <= right; i++) {
		j = i;
		while (j > left && *(target + j) < *(target + j - 1)) {
			swap(target + j, target + j - 1);
			j--;
		}
	}
}

int selectRandomPosition(int* target, int left, int right){
    srand((unsigned) time(NULL));
    int length = right - left + 1;
    int r = left + rand()%length;
    return r;
}

int selectMaxPosition(int* target, int left, int right){
    int m = target[left];
    int r = left;
	int i;
    for(i=left+1; i<= right; i++){
        if( m < target[i]){
            m = target[i];
            r = i;
        }
    }
    return r;
}

int partition(int* target, int left, int right, int k, int l, int mi, int mj) {
    // Select a number between left and right at random.
    int random = selectRandomPosition(target, left, right);
    // To select the maximum element, use:
    // int random = selectMaxPosition(target, left, right);
    // Exchange target[right] and target[random].
    int tmp = target[right];
    target[right] = target[random];
    target[random] = tmp;
    
    int pivot = target[right];
    int i = left-1;  // i scans the array from the left.
    int j = right;  // j scans the array from the right.
    for (;;) {
        // Move from the left until hitting a value no less than the pivot.
        for(i++; target[i] < pivot; i++){}
        // Move from the right until hitting a value no greater than the pivot.
        for(j--; pivot < target[j] && i < j; j--){}
        if (i >= j)  break;
        // Exchange target[i] and target[j].
	if (target[i] == pivot) {
	  swap(target[i], mi);
	  mi++;
	}// targetとcorrespondするときの場合分け
	else if (target[j] == pivot) {
	  swap(target[j], mj);
	  mj--;
	}
	else {
	  tmp = target[i];  target[i] = target[j];  target[j] = tmp;
	}
    }
    // Exchange target[i] and target[right].
    tmp = target[i];  target[i] = target[right];  target[right] = tmp;
    return i;
}


void randomQuickSort1(int* target, int left, int right ) {
    if (left < right){
        int i = partition(target,left,right);
	int mi = right;
	int mj = left;
        // i indicates the position of the pivot.
        randomQuickSort1(target, left, i - 1);
        randomQuickSort1(target, i + 1, right);
    }
}

void randomQuickSort2(int* target, int aLeft, int right) {
	int left = aLeft;
	while (left < right) {
		int i = partition(target, left, right);
		randomQuickSort2(target, left, i - 1);
		left = i + 1;
	}
}

void randomQuickSort3(int* target, int aLeft, int aRight) {
	int left = aLeft; int right = aRight;
	while (left < right) {
		int i = partition(target, left, right);
		if (i - left <= right - i){ // The left interval is shorter.
			randomQuickSort3(target, left, i - 1);
			left = i + 1;
		}
		else{ // The right interval is shorter.
			randomQuickSort3(target, i + 1, right);
			right = i - 1;
		}
	}
}

void randomQuickSort4(int* target, int aLeft, int aRight, int minSize) {
	int left = aLeft; int right = aRight;
	while(left + minSize < right) {
		int i = partition(target, left, right);
		if(i - left <= right - i){ // The left interval is shorter.
			randomQuickSort4(target, left, i - 1, minSize);
			left = i + 1;
		}
		else{ // The right interval is shorter.
			randomQuickSort4(target, i + 1, right, minSize);
			right = i - 1;
		}
	}
	// Insertion sort the target array of the range [left, right]
		insertionSort(target, left, right);
}

void randomQuickSort5sub(int* target, int aLeft, int aRight, int minSize) {
	int left = aLeft; int right = aRight;
	int mi; int mj;
	while (left + minSize < right) {
		int i = partition(target, left, right);
		if (i - left <= right - i){ // The left interval is shorter.
			randomQuickSort5sub(target, left, i - 1, minSize);
			left = i + 1;
		}
		else{ // The right interval is shorter.
			randomQuickSort5sub(target, i + 1, right, minSize);
			right = i - 1;
		}
	}
}


void randomQuickSort5(int* target, int left, int right, int minSize){
	randomQuickSort5sub(target, left, right, minSize);
	insertionSort(target, left, right);
}


int main(int argc, char** argv){

/*
    // Generate the running example array of size 12
    int n = 12;
    int target[12] = {48, 195, 12, 49, 198, 24, 99, 140, 48, 195, 12, 48};
*/
    
    // Generate an array of random numbers
    int n; // array length
	int m; //試行回数
	scanf("%d%d", &n, &m);
	m = 10;
    int* target;
    target = (int *)malloc( sizeof(int) * n);  // Use the heap to generate a long array
	int i;
	clock_t sTime, eTime;
	double *k;
	if ((k = (double *)malloc(sizeof(double) * 5 * n)) == NULL) {
    printf("malloc error\n");
    exit(EXIT_FAILURE);
  }//各sortに対してm回の試行を記録
	int j;
	// for (i = 0; i < n; i++) target[i] = rand() % 256;
	for (j = 0; j < m; j++){
		for (i = 0; i < n; i++) target[i] = rand() % 256;
		sTime = clock(); randomQuickSort1(target, 0, n - 1); eTime = clock();
		*(k + j) = eTime - sTime;

		for (i = 0; i < n; i++) target[i] = rand() % 256;
		sTime = clock(); randomQuickSort2(target, 0, n - 1); eTime = clock();
		*(k+100+j) = eTime - sTime;

		for (i = 0; i < n; i++) target[i] = rand() % 256;
		sTime = clock(); randomQuickSort3(target, 0, n - 1); eTime = clock();
		*(k+200+j) = eTime - sTime;


		for (i = 0; i < n; i++) target[i] = rand() % 256;
		sTime = clock(); randomQuickSort4(target, 0, n - 1, 10); eTime = clock();
		*(k+300+j) = eTime - sTime;


		for (i = 0; i < n; i++) target[i] = rand() % 256;
		sTime = clock(); randomQuickSort5(target, 0, n - 1, 10); eTime = clock();
		*(k+400+j) = eTime - sTime;
	}

	double mean[5];
	double var[5];
	// int l;
	// int h;
	for (i = 0; i < 5; i++){
		for (j = 0; j < m; j++){
			mean[i] = *(k+100*i+j) + mean[i];
		}
		mean[i] = mean[i] / m;
		for (j=0; j<m; j++){
			var[i] += (mean[i] - *(k + 100 * i + j))*(mean[i] - *(k+100*i+j));
		}
		var[i] = var[i] / m;
		printf("%f %f\n", mean[i], var[i]);
	}
    
    // Check if the target is sorted
    int sorted = 1;
    for(i = 0; i < (n-1); i++){
        if( target[i] > target[i+1]) sorted = 0;
    }
    if (sorted == 1) printf("sorted\n");
    
    free(target);
	free(k);
    return 0;  // 0 means EXIT_SUCCESS
}
