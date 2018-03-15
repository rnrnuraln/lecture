#include <stdio.h>
#include <stdlib.h>
/*
void swap(int* a, int* b) {
	int c;
	c = *a;
	*a = *b;
	*b = c;
}

int digit(int num, int j, int m) {
  int quotient = num;
  int remainder = 0;
  int i;
  for (i = 0; i < j; i++) {
    remainder = quotient%m;
    quotient = quotient / m;
  }
  return remainder;
}
*/

void partition(char* target, int left, int right, int *i, int *j, int *suf_ary, int* ISA) {
	//Select a number between left and right at random.
	int random = selectRandomPosition(target, left, right);
	// To select the maximum element, use:
	// int random = selectMaxPosition(target, left, right);
	// Exchange target[right] and target[random].
	int tmp = target[right];
	target[right] = target[random];
	target[random] = tmp;
	int mj = right - 1;
	int mi = left;
	int pivot = target[right];
							  int i = left-1;  // i scans the array from the left.
							  int j = right;  // j scans the array from the right.
	for (;;) {
		// Move from the left until hitting a value no less than the pivot.
		for (; target[suf_ary[*i]] < pivot; (*i)++){
		}
		// Move from the right until hitting a value no greater than the pivot.
		for (; pivot < target[suf_ary[*j]] && (*i) < (*j); (*j)--){
		}
		if ((*i) >= (*j))  break;
		// Exchange target[i] and target[j].
		if (target[suf_ary[*i]] == pivot) {
			// printf("%d\n",target[(*i)]);
			swap(suf_ary + (*i), suf_ary + mi);
			mi++;
			(*i)++;
		}// targetとcorrespondするときの場合分け
		else if (target[suf_ary[*])] == pivot) {
			// printf("%d\n",target[(*j)]);
			swap(suf_ary + (*j), suf_ary + mj);
			mj--;
			(*j)--;
		}
		else {
			swap(suf_ary + (*j), suf_ary + (*i));
			(*i)++;
			(*j)--;
		}
	}
	int k;
	(*i)--; //iは大きい方の配列の一番左にある。これを小さい配列の一番右に移す。
	if (target[(*j)] == pivot)
		(*j) = (*i) + 2;
	else
		(*j) = (*i) + 1; //jをiの隣に送る。target[j]がpivotと一致していたらそのまた隣に送る。
	for (k = left; k < mi; k++) {
		swap(target + k, target + (*i));
		(*i)--;
	} //mi側にあるpivotと一致したものを真ん中に寄せる
	for (k = right; k > mj; k--) {
		swap(target + k, target + (*j));
		(*j)++;
	} //mj側にあるpivotと一致したものを真ん中に寄せる
}

/*
void radixSort_sub(int* target, int targetLen, int m, int j) {
	int* tmp = malloc(sizeof(int)*targetLen);
	int* count = malloc(sizeof(int)*m); // length of each radix
	int* count2 = malloc(sizeof(int)*m); // location of each radix
	int i;
	for (i = 0; i < m; i++)
		count[i] = 0;
	for (i = 0; i < targetLen; i++)
		(*(count + digit(target[i], j, m)))++;

	*(count2) = *(count);
	for (i = 1; i < m; i++){
		*(count2 + i) = (*(count + i)) + (*(count2 + i - 1));
	}
	for (i = targetLen - 1; 0 <= i; i--) {
		(*(count2 + digit(target[i], j, m)))--;
		*(tmp + *(count2 + digit(target[i], j, m))) = *(target + i);
	}
	for (i = 0; i < targetLen; i++)
		*(target + i) = *(tmp + i);
	/*
	for (i = 0; i<targetLen; i++) {
		printf("%d ", target[i]);
		printf("done\n");
	}*/
/*	*(count2) = 0;
	for (i = 1; i<m; i++)
		*(count2 + i) = *(count + i-1) + *(count2 + i - 1);
	if (j > 1) {
		for (i = 0; i < m; i++) {
			if (count[i] != 0)
				radixSort_sub(target + (*(count2 + i)), (*(count + i)), m, j - 1);
		}
	}
	free(tmp);
	free(count);
	free(count2);
}

void radixSort(int* target, int targetLen, int m) {
	int max = target[0];
	int i;
	for (i = 1; i < targetLen; i++) {
		if (max < target[i])
			max = target[i];
	}
	int maxDigit = 0; //一番大きな数の桁
	for (; max > 0; maxDigit++) max = max / m;
	radixSort_sub(target, targetLen, m, maxDigit);
}
*/
void TernarySplitSort(int left, int right, int suf_ary, int* ISA) {
	if (left <= right){
		int i = left;
		int j = right - 1;
		partition(target, left, right, &i, &j, suf_ary, ISA);
		// i indicates the position of the pivot.
		TernarySplitSort1(target, left, i, suf_ary, ISA);
		TernarySplitSort1(target, j, right, suf_ary, ISA);
	}
}

void lars_sada(char* target, int targetLen, int* suf_ary) {
	int* ISA = malloc(sizeof(int)*targetLen);
	int i;
	int count[5] = { 0, 0, 0, 0, 0 };
	for (i = 0; i < targetLen; i++) {
		suf_ary[i] = i;	
		switch (target[i]) {
		case '$': count[0]++; break;
		case 'A': count[1]++; break;
		case 'C': count[2]++; break;
		case 'G': count[3]++; break;
		case 'T': count[4]++; break;
		}
	}  //counting the number of each character
	int count2[5];
	int count3[5];
	count2[0] = 1; //SAの値を入れ始める
	count3[0] = 0; //initial ISA
	for (i = 1; i < 5; i++) {
		count2[i] = count[i] + count2[i - 1];
		count3[i] = count2[i];
		printf("%d\n", count2[i]);
	}
	for (i = 0; i < targetLen; i++) {
		switch (target[i]) {
		case '$': suf_ary[0] = i; ISA[suf_ary[0]] = count3[0]; break;
		case 'A': suf_ary[count2[0]] = i; ISA[suf_ary[count2[0]]] = count3[1]; count2[0]++; break;
		case 'C': suf_ary[count2[1]] = i; ISA[suf_ary[count2[1]]] = count3[2]; count2[1]++; break;
		case 'G': suf_ary[count2[2]] = i; ISA[suf_ary[count2[2]]] = count3[3]; count2[2]++; break;
		case 'T': suf_ary[count2[3]] = i; ISA[suf_ary[count2[3]]] = count3[4]; count2[3]++; break;
		}
	} //construncing SA_1 and ISA_1 by radix sort

	int doublescore = 1; //doubling score
	while () {
		TernallySplitSort(suf_ary, ISA);
		// sorting ISA by doubling
		
		// constructing next ISA
		doublescore++;
	}
}

int main(int argc, char** argv){

	int i = 0;
	int n = 16;
	// Generate the running example array of size 12
	/*int n = 1000; //array length
	int i;
	int* target;
	target = (char *)malloc(n);
	for (i = 0; i < n; i++) target[i] = rand() % 256;*/
	char target[16] = { 'A', 'T', 'A', 'A', 'T', 'A', 'C', 'G', 'A', 'T', 'A', 'A', 'T', 'A', 'A', '$' };
	int* suf_ary = malloc(sizeof(int)*n);
	for (i = 0; i < n; i++) suf_ary[i] = 0;
  	lars_sada(target, n, suf_ary); //constructing suffix array by Lassson-Sadakane method

	/*
	int sorted = 1;
	for (i = 0; i < (n - 1); i++){
		printf("%s ,", target[i]);
		if (target[i] > target[i + 1]) sorted = 0;
	}
	if (sorted == 1) printf("sorted\n");
	*/
	for (i = 0; i < n; i++) {
		printf("%d\n", suf_ary[i]);
	}
	return 0;  // 0 means EXIT_SUCCESS
}
