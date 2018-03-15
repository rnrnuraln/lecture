#include <iostream>
#include <map>
#include <set>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <random>
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
		for (; ISA[suf_ary[i] + doublescore] < pivot; i++){
		}
		// Move from the right until hitting a value no greater than the pivot.
		for (; pivot < ISA[suf_ary[j] + doublescore] && i < j; j--){
		}
		if (i >= j)  break;
		// Exchange target[i] and target[j].
		if (ISA[suf_ary[i] + doublescore] == pivot) {
			// printf("%d\n",target[(*i)]);
			swap(suf_ary + i, suf_ary + mi);
			mi++;
			i++;
		}// targetとcorrespondするときの場合分け
		else if (ISA[suf_ary[j] + doublescore] == pivot) {
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
		for (int j = i->first - 1; j > i->first - i->second; j--) {
			if (oldISA[suf_ary[j] + doublescore] != oldISA[suf_ary[j + 1] + doublescore])
				ISA[suf_ary[j]] = j;
			else if (j + 2 == targetLen) {
				ISA[suf_ary[j]] = ISA[suf_ary[j + 1]];
				newISAchild[ISA[suf_ary[j]]] = 2;
			}
			else if (ISA[suf_ary[j + 1]] != ISA[suf_ary[j + 2]]) {
				ISA[suf_ary[j]] = ISA[suf_ary[j + 1]];
				newISAchild[ISA[suf_ary[j]]] = 2;
			}
			else {
				ISA[suf_ary[j]] = ISA[suf_ary[j + 1]];
				newISAchild[ISA[suf_ary[j]]]++;
			}
		}
	}
	delete[]oldISA;
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
	count2[0] = 1; //SAの値を入れ始める
	count3[0] = 0; //initial ISA
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
	}*/

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
	int nowpoint = 0;  //current point to see; @ in slide
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
		int &nowc = nowcount[target[suf_ary[nowpoint] - 1]]; // nowc points where to put induced number into suffix array
		suf_ary[nowc] = suf_ary[nowpoint] - 1; //insertion done
		// the code below defines how the ISA of the induced number would change. There are two ways; merely points SA or same as '=' one
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
	return ifsame;
}

void ind_sort(int* target, int targetLen, int* suf_ary, int cnum) {
	// checking S or L type;
	char *LStype;
	LStype = new char[targetLen];
	LSallocate(target, targetLen, LStype); //defining if each element of target is L-type or S-type
	int Lnum = 0; //number of L

	int *LMSarray; //collecting all the LMS site in target
	LMSarray = new int[targetLen / 2 + 1]; 
	int LMSnum = 0; //the number of LMS
	for (int i = targetLen - 1; i > 0; i--) {
		if (LStype[i] == 'S') {
			if (LStype[i - 1] == 'L') {
				LMSarray[LMSnum] = i;
				LMSnum++;
			}
		}
		else
			Lnum++;
	} //constructed LMSarray
	if (LStype[0] == 'L')
		Lnum++;
	int* count; //how many times each character appears
	count = new int[cnum];
	int* ISA; //used to manage the '=' on sorting LMS. the number located on suffix array and related to the other number by '=' have  same number.
	ISA = new int[targetLen + 1]; 
	ISA[targetLen] = -2; // the value of out of bound

	for (int i = 0; i < cnum; i++) {
		count[i] = 0;
	}

	for (int i = 0; i < targetLen; i++) {
		suf_ary[i] = -1;
		count[target[i]]++;
		ISA[i] = -1;
	}

	int* count2; // the location to which each LMSs should be inserted
	count2 = new int[cnum];
	count2[0] = count[0] - 1;
	int* count3; //same as count2, however, this does not change even after next conduction
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
	} // first condition about LMS


	Lsort(target, suf_ary, count3, ISA, targetLen, cnum, Lnum, LStype); //sorting L-type


	for (int i = 1; i < LMSnum; i++) {
		ISA[LMSarray[i]] = -1;
	}

	int* ILMS; //used to find LMS from number
	ILMS = new int[targetLen];
	for (int i = 0; i < LMSnum; i++) {
		ILMS[LMSarray[LMSnum - i - 1]] = i; //LMSarray is sorted upside down.
	}

	int* LMSforsort; //the sorted LMS would be included to this
	LMSforsort = new int[LMSnum];
	

	int ifsame = Ssort(target, suf_ary, count3, ISA, targetLen, cnum, LStype, LMSnum, ILMS, LMSforsort);
	// sorting the S-type, and at the same time, seeing if the LMS are sorted, no equal LMS
	//if "ifsame" = 0, no same LMS prefix exists; that is, LMS substrings can be sorted

	int* LMSorder;
	LMSorder = new int[LMSnum + 1];
	LMSorder[LMSnum] = LMSnum;// this is like the suffix array only by LMS.
	if (ifsame != 0) {
		ind_sort(LMSforsort, LMSnum, LMSorder, LMSnum); //sorting LMS by recursion
	}
	else {
		for (int i = 0; i < LMSnum; i++)
			LMSorder[LMSforsort[i]] = i;
	}

	count2[0] = count[0] - 1;

	for (int i = 1; i < cnum; i++)
		count2[i] = count[i] + count2[i - 1];

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

int main(){
	// main function is almost the same as the one of doubling code

	// Generate the running example array of size 12
	int targetLen = 100000000; //array length
	char* target;
	target = new char[targetLen];
	random_device rnd;
	mt19937 mt(rnd());
	for (int i = 0; i < targetLen - 1; i++) {
		switch (mt() % 4) {
		case 0: target[i] = 'A'; break;
		case 1: target[i] = 'C'; break;
		case 2: target[i] = 'G'; break;
		case 3: target[i] = 'T'; break;
		}
	}
	target[targetLen - 1] = '$';
	//  for (int i = 0; i < targetLen; i++)
	// 	printf("%c", target[i]);
	// int targetLen = 12;
	// char target[16] = { 'A', 'T', 'A', 'A', 'T', 'A', 'C', 'G', 'A', 'T', 'A', 'A', 'T', 'A', 'A', '$' };
	// char target[12] = { 'T', 'A', 'A', 'T', 'A', 'A', 'T', 'A', 'A', 'T', 'C', '$'};
	// int const targetLen = 20;
	// char target[targetLen] = { 'A', 'A', 'T', 'A', 'T', 'A', 'A', 'T', 'A', 'A', 'T', 'A', 'T', 'A', 'A', 'T', 'A', 'A', 'A', '$' };
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
	
	int* suf_ary2;
	suf_ary2 = new int[targetLen];

	time_t sTime, eTime;
	sTime = clock();
	ind_sort(itarget, targetLen, suf_ary, 5);
	eTime = clock();
	// lars_sada(target, targetLen, suf_ary2);

	/*int s = 0;
	for (int i = 1; i < targetLen; i++) {
		int u = suf_ary[i];
		int t = suf_ary[i - 1];
		while (u != targetLen - 1 || t != targetLen - 1) {
			if (target[u] < target[t]) {
				s++;
			}
			else if (target[u] > target[t])
				break;
			u++;
			t++;
		}
	}
	printf("%d\n", s);*/
	/*
	for (int i = 0; i < targetLen; i++) {
		if (suf_ary[i] != suf_ary2[i])
			s++;
	}
	printf("%d\n", s);*/

	printf("%d[ms]\n", (int)(eTime - sTime));
	/*
	for (int i = 0; i < targetLen; i++) {
	int s = suf_ary[i];
	for (int j = s; j < targetLen; j++) {
	printf("%c", target[j]);
	}
	printf("end\n");
	}*/


	delete[] suf_ary2;
	delete[] suf_ary;
	delete[] itarget;
	return 0;  // 0 means EXIT_SUCCESS
}