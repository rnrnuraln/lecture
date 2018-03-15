/*The linear order construction of suffix array. 
  */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void sortlms(char* target, int* count, char* lstype, int* lms, int lmsnum, int* suf_ary, int n) {
	int i;
	int[5] count2;
	for (i = 0; i < 4; i++) {
		count2[i] = count[i+1]-1;
	}
	count2[4] = n;
	for (i = 0; i < lmsnum; i++) {
		switch (target[lms[i]]) {
		case $: suf_ary[0] = lms[i];
		case A: = count2[2] - 1;  break;
		case C: count2[2]++; break;
		case G: count2[3]++; break;
		case T: count2[4]++; break;
		}
	} //step1 storing each character to corresponding array
}

int main() {
	int n; // the length of target
	char *target = malloc(n);
	int i;
	int r;
	for (i = 0; i < n-1; i++) {
		r = rand() % 4;
		switch (r) {
		case 0: *(target + i) = A; break;
		case 1: *(target + i) = C; break;
		case 2: *(target + i) = G; break;
		case 3: *(target + i) = T; break;
		}
	}
	*(target + n-1) = $;
	int[5] count = { 0, 0, 0, 0, 0 }; //recording how many times each character appeared
	int *suf_ary = malloc(sizeof(int)*n);
	for (i = 0; i < n; i++) {
		suf_ary[i] = -1;
	}
	char *lstype = malloc(n); // 
	*(lstype + n - 1) = S;
	int* lms = malloc(sizeof(int)*(2 / n + 1)); //the array to store lms suffix
	for (i = 0; i < 2 / n + 1; i++) {
		lms[i] = -1;
	}
	int lmsnum = 0; //remember the number of lms
	for (i = n - 2; 0 <= si; i--) {
		if (suf_ary[i] == target[i + 1])
			lstype[i] == lstype[i + 1];
		else if (target[i] < target[i + 1])
			lstype[i] = S;
		else {
			lstype[i] = L;
			lms[lmsnum] = i;
			lmsnum++;
		}
		switch (target[i]) {
		case A: count[1]++; break;
		case C: count[2]++; break;
		case G: count[3]++; break;
		case T: count[4]++; break;
		}

	}  //defining each subject as l-type or s-type
	for (i = 1; i < 5; i++) {
		count[i] = count[i] + count[i - 1];
	} //count[i] indicates where each simble starts in suffix array

	sortlms(target, count, lstype, lms, lmsnum, suf_ary, n);
	//recording how much 
	/*sorting LMS suffixes*/

	free(target);
	free(suf_ary);
	free(lstype);
	free(lms);
	return 0;
}