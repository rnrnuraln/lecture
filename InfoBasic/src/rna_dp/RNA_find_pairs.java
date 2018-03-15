package rna_dp;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

/*âΩÇ©javaÇÃóLå¯ê´Çê∂Ç©ÇπÇ»Ç¢ä¥Ç∂Ç…Ç»Ç¡ÇƒÇµÇ‹Ç¢Ç‹ÇµÇΩÅcÅc
 *
 */

public class RNA_find_pairs {
	public static void main(String[] args) {
		String Seq = "afe";
		try {
			Seq = readSeqs();
		}
		catch (IOException error) {
			System.out.println("there is an io error");
		}
		int SeqLen = Seq.length();
		int d = 3;
		int[][] Scores = new int[SeqLen][SeqLen]; //memorizing scores of DP
		//since we do not only have to show the maximum number of pairs but also which two becomes pair, we have to memory them
		//to express them, we want to utilize 'traceback' system.
		int[][] trb = new int[SeqLen][SeqLen]; //memorizing the traceback
		
		//loop of DP
		for (int j = 1; j < SeqLen; j++) {
			for (int i = 0; i < j; i++) {
				//calculate S_{ij}
				int S_ij_1 = Scores[i][j-1];//S_{i,(j-1)}
				int maxscore = 0;
				int maxk = -1;
				for (int k = i; k < j-d; k++) {
					if (matches(Seq, k, j) == true) {
						if (k > 0) {
							int score = Scores[i][k-1] + Scores[k+1][j-1] + 1;
							if (score > maxscore) {
								maxscore = score;
								maxk = k;
							}
						}
						else {
							int score = Scores[k+1][j-1] + 1;
							if (score > maxscore) {
								maxscore = score;
								maxk = k;
							}
						}
					}
				}
				if (maxscore > S_ij_1) {
					Scores[i][j] = maxscore;
					trb[i][j] = maxk;
				}
				else {
					Scores[i][j] = S_ij_1;
					trb[i][j] = -1;
				}
			}
		}
		//tracebacking using recursing
		System.out.println(Seq);
		char[] result = new char[SeqLen];
		traceback(trb, result, 0, SeqLen-1);
		int m = 0;
		int n = 0;
		for (int i = 0; i < SeqLen; i++) {
			System.out.print(result[i]);
			if (result[i] == '(')
				m++;
			else if (result[i] == ')')
				n++;
		}
		System.out.print('\n');
		// System.out.println(n + ", " + m);
		/*for (int i = 0; i < SeqLen; i++) {
			for (int j = 0; j < SeqLen; j++) {
				System.out.print(trb[i][j] + ", ");
			}
			System.out.print('\n');
		}*/
	}
	
	public static String readSeqs() throws IOException {
		BufferedReader din = new BufferedReader(new FileReader("SampleRNA.txt"));
		din.readLine();
		String s;
		StringBuilder s1 = new StringBuilder();
		while ((s = din.readLine()) != null) {
			s1.append(s);
		}
		din.close();
		String s2 = s1.toString();
		return s2;
	}
	
	public static boolean matches(String s, int k, int j) {
		char a = s.charAt(k);
		char b = s.charAt(j);
		switch(a) {
		case 'a': 
			if (b == 'u') 
				return true;
			else
				return false;
		case 'c': 
			if (b == 'g') 
				return true;
			else
				return false;
		case 'g': 
			if (b == 'c' || b == 'u') 
				return true;
			else
				return false;
		case 'u': 
			if (b == 'a' || b == 'g') 
				return true;
			else
				return false;
		default: 
			System.out.println("something's wrong!");
			return false;
		}
	}

	public static void traceback(int[][] trb, char[] result, int left, int right) {
		if (left < right) {
			int k = trb[left][right];
			if (k > 0) {
				result[k] = '(';
				result[right] = ')';
				traceback(trb, result, left, k-1);
				traceback(trb, result, k+1, right-1);
			}
			else if (k == 0) {
				result[k] = '(';
				result[right] = ')';
				traceback(trb, result, k+1, right-1);
			}
			else {
				result[right] = '.';
				traceback(trb, result, left, right-1);
			}
		}
		else if (left == right) {
			result[left] = '.';
		}
	}
}