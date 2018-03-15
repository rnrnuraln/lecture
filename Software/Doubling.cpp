#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <assert.h>
#include <iostream>
#include <time.h>
using namespace std;

void swap(int* a, int* b) {
int c;
c = *a;
*a = *b;
*b = c;
}

int selectRandomPosition(int* target, int left, int right){
	srand((unsigned)time(NULL));
	int length = right - left + 1;
	int r = left + rand() % length;
	return r;
}

void partition(int left, int right, int &i, int &j, int *suf_ary, int* ISA, int doublescore) {
	//Select a number between left and right at random.
	// int random = selectRandomPosition(suf_ary, left, right);
	// To select the maximum element, use:
	// Exchange target[right] and target[random].
	// swap(suf_ary[random], suf_ary[right]);
	// in this code, I skipped the randomizing step because I had no time, sorry.
	int mj = right - 1;
	int mi = left;
	int pivot = ISA[suf_ary[right] + doublescore];
	for (;;) {
		// Move from the left until hitting a value no less than the pivot.
		for (; ISA[suf_ary[i]+doublescore] < pivot; i++){
		}
		// Move from the right until hitting a value no greater than the pivot.
		for (; pivot < ISA[suf_ary[j]+doublescore] && i < j; j--){
		}
		if (i >= j)  break;
		// Exchange target[i] and target[j].
		if (ISA[suf_ary[i]+doublescore] == pivot) {
			// printf("%d\n",target[(*i)]);
			swap(suf_ary + i, suf_ary + mi);
			mi++;
			i++;
		}// targetとcorrespondするときの場合分け
		else if (ISA[suf_ary[j]+doublescore] == pivot) {
			// printf("%d\n",target[(*j)]);
			swap(suf_ary + j, suf_ary + mj);
			mj--;
			j--;
		}
		else {
			swap(suf_ary + j, suf_ary + i);
			i++;
			j--;
		}
	}
	int k;
	i--; //iは大きい方の配列の一番左にある。これを小さい配列の一番右に移す。
	if (ISA[suf_ary[j] + doublescore] == pivot)
		j = i + 2;
	else
		j = i + 1; //jをiの隣に送る。target[j]がpivotと一致していたらそのまた隣に送る。
	for (k = left; k < mi; k++) {
		swap(suf_ary + k, suf_ary + i);
		i--;
	} //mi側にあるpivotと一致したものを真ん中に寄せる
	for (k = right; k > mj; k--) {
		swap(suf_ary + k, suf_ary + j);
		j++;
	} //mj側にあるpivotと一致したものを真ん中に寄せる
}

void TernarySplitSort(int left, int right, int *suf_ary, int* ISA, int doublescore) {
	if (left < right){
		int i = left;
		int j = right - 1;
		partition(left, right, i, j, suf_ary, ISA, doublescore);
		// i indicates the position of the pivot.
		TernarySplitSort(left, i, suf_ary, ISA, doublescore);
		TernarySplitSort(j, right, suf_ary, ISA, doublescore);
	}
}

void makeISAchild(int* ISA, map<int, int>& ISAchild, int* suf_ary, int targetLen) {
	for (int i = 1; i < targetLen; i++) {
		if (ISA[suf_ary[i]] == ISA[suf_ary[i - 1]]) {
			if (ISA[suf_ary[i]] != ISA[suf_ary[i - 2]]) //key is found that it has more than one numbers
				ISAchild[ISA[suf_ary[i]]] = 2;
			else
				ISAchild[ISA[suf_ary[i]]]++; //adding its numbers
		}
	}
}

map<int, int> makeISA(int* ISA, map<int, int> ISAchild, int* suf_ary, int doublescore, int targetLen) {
	map<int, int> newISAchild;
	int *oldISA;
	oldISA = new int[targetLen];
	for (int i = 0; i<targetLen; i++) {
		oldISA[i] = ISA[i];
	}
	for (map<int, int>::const_iterator i = ISAchild.begin(); i != ISAchild.end(); i++) {
		for (int j = i->first-1; j > i->first - i->second; j--) {
			if (oldISA[suf_ary[j] + doublescore] != oldISA[suf_ary[j + 1] + doublescore])
				ISA[suf_ary[j]] = j;
			else if (j + 2 == targetLen) {
				ISA[suf_ary[j]] = ISA[suf_ary[j + 1]];
				newISAchild[ISA[suf_ary[j]]] = 2;
			}
			else if (ISA[suf_ary[j+1]] != ISA[suf_ary[j + 2]]) {
				ISA[suf_ary[j]] = ISA[suf_ary[j + 1]];
				newISAchild[ISA[suf_ary[j]]] = 2;
			}
			else {
				ISA[suf_ary[j]] = ISA[suf_ary[j + 1]];
				newISAchild[ISA[suf_ary[j]]]++;
			}
		}
	}
	delete []oldISA;
	return newISAchild;
}

void lars_sada(char* target, int targetLen, int* suf_ary) {
	int *ISA;
	ISA = new int[targetLen];
	int count[5] = { 0, 0, 0, 0, 0 };
	for (int i = 0; i < targetLen; i++) {
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
	count2[0] = 1; //used to construct initial SA
	count3[0] = 0; //used to construct initial ISA
	for (int i = 1; i < 5; i++) {
		count2[i] = count[i] + count2[i - 1];
		count3[i] = count2[i] - 1;
	}
	for (int i = 0; i < targetLen; i++) {
		switch (target[i]) {
		case '$': suf_ary[0] = i; ISA[suf_ary[0]] = count3[0]; break;
		case 'A': suf_ary[count2[0]] = i; ISA[suf_ary[count2[0]]] = count3[1]; count2[0]++; break;
		case 'C': suf_ary[count2[1]] = i; ISA[suf_ary[count2[1]]] = count3[2]; count2[1]++; break;
		case 'G': suf_ary[count2[2]] = i; ISA[suf_ary[count2[2]]] = count3[3]; count2[2]++; break;
		case 'T': suf_ary[count2[3]] = i; ISA[suf_ary[count2[3]]] = count3[4]; count2[3]++; break;
		}
	} //construncing SA_1 and ISA_1 by radix sort
	
	map<int, int> ISAchild; //indicating which part of ISA must be sorted. key indicates the corresponding ISA, and value indicates how many times the key appears
	makeISAchild(ISA, ISAchild, suf_ary, targetLen); //making ISAchild from ISA
	// printf("%d", ISAchild.size());
	/*for (map<int, int>::const_iterator i = ISAchild.begin(); i != ISAchild.end(); i++) {
		int a = i->first;
		printf("%d %d", i->first, i->second);
		for (int j = a; j > a - i->second + 1; j--) {
			printf("%d\n", i->first);
			// assert(ISA[suf_ary[a]] != ISA[suf_ary[a-1]]);
		}
	}*///  
	
	int doublescore = 1; //doubling score
	while (ISAchild.size() != 0) {
		for (map<int, int>::const_iterator i = ISAchild.begin(); i != ISAchild.end(); i++) {
			int x = i->first;
			int y = i->second;
			TernarySplitSort(x - y + 1, x, suf_ary, ISA, doublescore);
		}
		// sorting ISA by doubling
		for (int i = 0; i < targetLen; i++) {
			// printf("%d\n", suf_ary[i]);
		}
		ISAchild = makeISA(ISA, ISAchild, suf_ary, doublescore, targetLen);
		for (int i = 0; i < targetLen; i++) {
			// printf("%d\n", ISA[suf_ary[i]]);
		}
		for (map<int, int>::const_iterator i = ISAchild.begin(); i != ISAchild.end(); i++) {
			// printf("%d %d\n", i->first, i->second);
		}
		// constructing next ISA
		doublescore = doublescore * 2;
	}
	delete[] ISA;
}

int main(int argv, char** argc){

	// Generate the running example array of size 12
	int targetLen = 100000; //array length
	char* target;
	target = new char[targetLen];
	for (int i = 0; i < targetLen-1; i++) {
		int r = rand() % 4;
		switch (r) {
		case 0: target[i] = 'A'; break;
		case 1: target[i] = 'C'; break;
		case 2: target[i] = 'G'; break;
		case 3: target[i] = 'T'; break;
		}
	}
	target[targetLen - 1] = '$';
	// printf("%99s\n", target);
	// char target[16] = { 'A', 'T', 'A', 'A', 'T', 'A', 'C', 'G', 'A', 'T', 'A', 'A', 'T', 'A', 'A', '$' };
	int* suf_ary;
	suf_ary = new int[targetLen];
	for (int i = 0; i < targetLen; i++) suf_ary[i] = 0;
	time_t start, end;

	start = clock();
	lars_sada(target, targetLen, suf_ary); //constructing suffix array by Lassson-Sadakane method
	end = clock();
	
	cout << (int)(end - start) << "[ms]" << endl;
	
	delete []suf_ary;
	delete[] target;
	return 0;  // 0 means EXIT_SUCCESS
}
