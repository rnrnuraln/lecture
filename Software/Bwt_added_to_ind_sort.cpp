#include <iostream>
#include <map>
#include <set>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "MT.h"
using namespace std;

void swap(int* a, int* b) {
	int c;
	c = *a;
	*a = *b;
	*b = c;
}

class Count{
private:
	int count2[4];
public:
	void makecount(char* BWT, int targetLen){
		int count[5];
		for (int i = 0; i < 5; i++)
			count[i] = 0;
		for (int i = 0; i < targetLen; i++) {
			switch (BWT[i]) {
			case '$': count[0]++; break;
			case 'A': count[1]++; break;
			case 'C': count[2]++; break;
			case 'G': count[3]++; break;
			case 'T': count[4]++; break;
			}
		}
		count2[0] = 1;
		for (int i = 1; i < 4; i++)
			count2[i] = count[i] + count2[i - 1];

	}
	int countnumber(char x){
		switch (x) {
		case '$': printf("nothing\n"); return 0;
		case 'A': return count2[0];
		case 'C': return count2[1];
		case 'G': return count2[2];
		case 'T': return count2[3];
		}
	}
};

class Occ{
private:
	int *element[4];
public:
	void Occmalloc(int targetLen) {
		for (int i = 0; i < 4; i++)
			element[i] = new int[targetLen];
	}

	void Occfree() {
		for (int i = 0; i < 4; i++)
			delete[] element[i];
	}

	void makeOcc(char* BWT, int targetLen){
		for (int i = 0; i < 4; i++)
			element[i][0] = 0;

		switch (BWT[0]) {
		case '$': break;
		case 'A': element[0][0]++; break;
		case 'C': element[1][0]++; break;
		case 'G': element[2][0]++; break;
		case 'T': element[3][0]++; break;
		}

		for (int i = 1; i < targetLen; i++) {
			for (int j = 0; j < 4; j++)
				element[j][i] = element[j][i - 1];
			switch (BWT[i]) {
			case '$': break;
			case 'A': element[0][i]++; break;
			case 'C': element[1][i]++; break;
			case 'G': element[2][i]++; break;
			case 'T': element[3][i]++; break;
			}
		}
	}

	int occnum(char x, int i) {
		if (i < 0)
			return 0;
		else {
			switch (x) {
			case '$': printf("none\n"); return 0;
			case 'A': return element[0][i];
			case 'C': return element[1][i];
			case 'G': return element[2][i];
			case 'T': return element[3][i];
			}
		}
	}
};


void LSallocate(int* target, int targetLen, char* LStype){
	LStype[targetLen - 1] = 'S';
	for (int i = targetLen - 2; 0 <= i; i--) {
		if (target[i] == target[i + 1])
			LStype[i] = LStype[i + 1];
		else {
			if (target[i] < target[i + 1])
				LStype[i] = 'S';
			else
				LStype[i] = 'L';
		}
	}
}

int nextpoint(int* suf_ary, int nowpoint, char* LStype, int targetLen) {
	int i = nowpoint + 1;
	if (i == targetLen) return i;
	while (suf_ary[i] == -1 || suf_ary[i] == 0) {
		i++;
		if (i == targetLen) return i;
	}
	if (LStype[suf_ary[i] - 1] == 'S')
		i = nextpoint(suf_ary, i, LStype, targetLen);
	return i;
}

void Lsort(int* target, int* suf_ary, int* count3, int* ISA, int targetLen, int cnum, int Lnum, char* LStype) {
	int nowpoint = 0;  //current point to view; @ in slide
	int* nowcount;
	nowcount = new int[cnum];  //^ in slide
	nowcount[0] = 0;
	for (int i = 1; i < cnum; i++) {
		nowcount[i] = count3[i - 1] + 1;
	}

	//loop start

	int ii = 0;
	int previouspoint = 0;
	while (ii < Lnum) {
		int &nowc = nowcount[target[suf_ary[nowpoint] - 1]];
		suf_ary[nowc] = suf_ary[nowpoint] - 1; //insertion done
		if (ISA[suf_ary[nowpoint]] != ISA[suf_ary[previouspoint]])
			ISA[suf_ary[nowc]] = nowc;
		else if (nowc == count3[target[suf_ary[nowpoint] - 1] - 1] + 1)
			ISA[suf_ary[nowc]] = nowc;
		else if (ISA[suf_ary[nowpoint]] != ISA[suf_ary[nowc - 1] + 1])
			ISA[suf_ary[nowc]] = nowc;
		else
			ISA[suf_ary[nowc]] = ISA[suf_ary[nowc - 1]];
		nowc++;
		previouspoint = nowpoint;
		nowpoint = nextpoint(suf_ary, nowpoint, LStype, targetLen);
		ii++;
	}
	delete[] nowcount;
}

int Ssort(int* target, int* suf_ary, int* count3, int* ISA, int targetLen, int cnum, char* LStype, int LMSnum, int* ILMS, int* LMStosort) {
	int nowpoint = targetLen - 1;  //current point to view; @ in slide
	int* nowcount;
	nowcount = new int[cnum];  //^ in slide
	nowcount[0] = 0;
	for (int i = 1; i < cnum; i++) {
		nowcount[i] = count3[i];
	}

	int* equalcount;
	equalcount = new int[cnum];
	int ifsame = 0;
	int previousLMS = targetLen - 2;
	int nowLMS = LMSnum - 1;
	//loop start
	for (; nowpoint > -1; nowpoint--) {
		if (suf_ary[nowpoint] != 0) {
			if (LStype[suf_ary[nowpoint] - 1] == 'S') {
				int &nowc = nowcount[target[suf_ary[nowpoint] - 1]];
				suf_ary[nowc] = suf_ary[nowpoint] - 1;
				if (nowpoint == targetLen - 1)
					ISA[suf_ary[nowc]] = nowc;
				else if (ISA[suf_ary[nowpoint]] != ISA[suf_ary[nowpoint + 1]])
					ISA[suf_ary[nowc]] = nowc;
				else if (nowc == count3[target[suf_ary[nowpoint] - 1]])
					ISA[suf_ary[nowc]] = nowc;
				else if (ISA[suf_ary[nowpoint]] != ISA[suf_ary[nowc + 1] + 1])
					ISA[suf_ary[nowc]] = nowc;
				else
					ISA[suf_ary[nowc]] = ISA[suf_ary[nowc + 1]];
				nowc--;
			}
			else if (LStype[suf_ary[nowpoint]] == 'S') {
				if (ISA[previousLMS] == ISA[suf_ary[nowpoint]]) {
					LMStosort[ILMS[suf_ary[nowpoint]]] = LMStosort[ILMS[previousLMS]];
					ifsame++;
				}
				else {
					LMStosort[ILMS[suf_ary[nowpoint]]] = nowLMS;
				}
				previousLMS = suf_ary[nowpoint];
				nowLMS--;
			}
		}
	}
	delete[] nowcount;
	delete[] equalcount;
	return ifsame;
}

void ind_sort(int* target, int targetLen, int* suf_ary, int cnum) {
	// checking S or L type;
	char *LStype;
	LStype = new char[targetLen];
	LSallocate(target, targetLen, LStype);
	int Lnum = 0;
	int Snum = 0;

	int *LMSarray;
	LMSarray = new int[targetLen / 2 + 1];
	int LMSnum = 0;
	for (int i = targetLen - 1; i > 0; i--) {
		if (LStype[i] == 'S') {
			Snum++;
			if (LStype[i - 1] == 'L') {
				LMSarray[LMSnum] = i;
				LMSnum++;
			}
		}
		else
			Lnum++;
	}
	if (LStype[0] == 'L')
		Lnum++;
	int* count;
	count = new int[cnum];
	int* ISA;
	ISA = new int[targetLen + 1];
	ISA[targetLen] = -2;

	for (int i = 0; i < cnum; i++) {
		count[i] = 0;
	}

	for (int i = 0; i < targetLen; i++) {
		suf_ary[i] = -1;
		count[target[i]]++;
		ISA[i] = -1;
	}

	int* count2;
	count2 = new int[cnum];
	count2[0] = count[0] - 1;
	int* count3;
	count3 = new int[cnum];
	count3[0] = count2[0];

	for (int i = 1; i < cnum; i++) {
		count2[i] = count[i] + count2[i - 1];
		count3[i] = count2[i];
	}

	for (int i = 0; i < LMSnum; i++) {
		suf_ary[count2[target[LMSarray[i]]]] = LMSarray[i];
		count2[target[LMSarray[i]]]--;
		ISA[LMSarray[i]] = count3[target[LMSarray[i]]];
	}


	Lsort(target, suf_ary, count3, ISA, targetLen, cnum, Lnum, LStype);


	for (int i = 1; i < LMSnum; i++) {
		ISA[LMSarray[i]] = -1;
	}

	int* ILMS; //find LMS
	ILMS = new int[targetLen];
	for (int i = 0; i < LMSnum; i++) {
		ILMS[LMSarray[LMSnum - i - 1]] = i;
	}
	int* LMSforsort;
	LMSforsort = new int[LMSnum];
	int ifsame = Ssort(target, suf_ary, count3, ISA, targetLen, cnum, LStype, LMSnum, ILMS, LMSforsort);


	int* LMSorder;
	LMSorder = new int[LMSnum + 1];
	LMSorder[LMSnum] = LMSnum;
	if (ifsame != 0) {
		ind_sort(LMSforsort, LMSnum, LMSorder, LMSnum);
	}
	else {
		for (int i = 0; i < LMSnum; i++)
			LMSorder[LMSforsort[i]] = i;
	}

	count2[0] = count[0] - 1;

	for (int i = 1; i < cnum; i++) {
		count2[i] = count[i] + count2[i - 1];
	}
	for (int i = 0; i < targetLen; i++) {
		ISA[i] = -1;
		suf_ary[i] = -1;
	}
	for (int i = LMSnum - 1; i > -1; i--) {
		int j = LMSnum - LMSorder[i] - 1;
		suf_ary[count2[target[LMSarray[j]]]] = LMSarray[j];
		ISA[LMSarray[j]] = count2[target[LMSarray[j]]];
		count2[target[LMSarray[j]]]--;
	}

	Lsort(target, suf_ary, count3, ISA, targetLen, cnum, Lnum, LStype);


	Ssort(target, suf_ary, count3, ISA, targetLen, cnum, LStype, LMSnum, ILMS, LMSforsort);



	delete[] LStype;
	delete[] LMSarray;
	delete[] ISA;
	delete[] count;
	delete[] count2;
	delete[] count3;
	delete[] ILMS;
	delete[] LMSforsort;
	delete[] LMSorder;
}

void SAtoBWT(char* target, int* suf_ary, char* BWT, int targetLen) {
	for (int i = 0; i < targetLen; i++) {
		if (suf_ary[i] == 0)
			BWT[i] = '$';
		else
			BWT[i] = target[suf_ary[i] - 1];
	}
}

void rangesearch(char* query, int &lb, int &ub, int targetLen, int k_len, Count C, Occ occ) {
	lb = 0;
	ub = targetLen - 1;
	for (int i = k_len - 1; i > -1; i--) {
		lb = C.countnumber(query[i]) + occ.occnum(query[i], lb - 1);
		ub = C.countnumber(query[i]) + occ.occnum(query[i], ub) - 1;
	}
}

int main(){
	// main function is almost the same as the one of doubling code

	// Generate the running example array of size 12
	int targetLen = 100; //array length
	scanf("%d", &targetLen);
	char* target;
	target = new char[targetLen];
	init_genrand((unsigned) time(NULL));
	for (int i = 0; i < targetLen - 1; i++) {
		switch (genrand_int32() % 4) {
		case 0: target[i] = 'A'; break;
		case 1: target[i] = 'C'; break;
		case 2: target[i] = 'G'; break;
		case 3: target[i] = 'T'; break;
		}
	}
	target[targetLen - 1] = '$';
	//  for (int i = 0; i < targetLen; i++)
	// 	printf("%c", target[i]);
	// int targetLen = 16;
	// char target[16] = { 'A', 'T', 'A', 'A', 'T', 'A', 'C', 'G', 'A', 'T', 'A', 'A', 'T', 'A', 'A', '$' };
	// char target[12] = { 'T', 'A', 'A', 'T', 'A', 'A', 'T', 'A', 'A', 'T', 'C', '$'};
	int *itarget;
	itarget = new int[targetLen];
	for (int i = 0; i < targetLen; i++) {
		switch (target[i]) {
		case '$': itarget[i] = 0; break;
		case 'A': itarget[i] = 1; break;
		case 'C': itarget[i] = 2; break;
		case 'G': itarget[i] = 3; break;
		case 'T': itarget[i] = 4; break;
		} //constructing itarget; changing type, from char* type to integer*, for ease of equipment
	}
	int* suf_ary;
	suf_ary = new int[targetLen + 1];
	suf_ary[targetLen] = targetLen;

	ind_sort(itarget, targetLen, suf_ary, 5); //constructing suffix array by Lassson-Sadakane method

	char* BWT;
	BWT = new char[targetLen];
	SAtoBWT(target, suf_ary, BWT, targetLen);
	// for (int i = 0; i < targetLen; i++)
		// printf("%c", BWT[i]);

	Count C;
	C.makecount(BWT, targetLen);
	
	Occ occ;
	occ.Occmalloc(targetLen);
	occ.makeOcc(BWT, targetLen);
	
	int n = 6;
	scanf("%d", &n);
	const int k = n;
	char query[k];
	// char query[6] = {'A', 'C', 'T', 'A', 'G', 'T'};
	time_t start, end;
	int timesum = 0;
	for (int i = 0; i < 100000; i++) {
		for (int j = 0; j < k; j++) {
			switch (genrand_int32() % 4) {
			case 0: query[j] = 'A'; break;
			case 1: query[j] = 'C'; break;
			case 2: query[j] = 'G'; break;
			case 3: query[j] = 'T'; break;
			}
		}
		int lb = 0;
		int ub = 0;
		start = clock();
		rangesearch(query, lb, ub, targetLen, k, C, occ);
		end = clock();
		timesum = (double)(end - start) + timesum;
		/*for (int j = lb; j <= ub; j++){
			for (int k = 0; k < 6; k++)
				printf("%c", target[suf_ary[j] + k]);
			printf("%c", '\n');
		}*/
	}

	printf("%d\n", timesum);
	

	occ.Occfree();
	delete[] BWT;
	delete[] suf_ary;
	delete[] itarget;
	return 0;  // 0 means EXIT_SUCCESS
}