package baum_welch;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Baum_Welch {
	public static void main(String[] args) {
		int DNANum = 10;
		String[] Seqs = new String[DNANum]; 
		try {
			for (int i = 0; i < DNANum; i++) {
				String A = String.format("Baum_Welches/DNA%d.txt", i);
				Seqs[i] = readSeqs(A);
			}
		} catch (IOException error) {
			System.out.println("Error!");
			return;
		}
		double[][] StateTransMat = new double[3][3];
		double[][] ExpProb = new double[3][3];
		double[][][] S = new double[2][][];
		HashMap<Character, Integer> Alphabets = new HashMap<Character, Integer>();
		try {
			S = Baum_Welch.readParams(StateTransMat, ExpProb, Alphabets);
			StateTransMat = S[0];
			ExpProb = S[1];
		} catch (IOException error) {
			System.out.println("readparams Error!");
			return;
		}
		for (int i = 0; i < 50; i++) {
			double[] A = Baum_Welch_Sub(Seqs, StateTransMat, ExpProb, Alphabets);
			//System.out.println(i + ": STM: " + A[0]);
			//System.out.println(i + ": EP: " + A[1]);
			if (A[0] + A[1] < 0.001)
				{System.out.println(i); break;}
		}
		int StateNum = StateTransMat.length;
		int AlphabetNum = ExpProb[0].length;
		System.out.println("State Transit Matrix");
		for (int i = 0; i < StateNum; i++) {
			for (int j = 0; j < StateNum; j++) {
				System.out.print(StateTransMat[i][j] + " ");
			}
			System.out.print('\n');
		}
		System.out.println("Expression Probability");
		for (int i = 0; i < StateNum; i++) {
			for (int j = 0; j < AlphabetNum; j++) {
				System.out.print(ExpProb[i][j] + " ");
			}
			System.out.print('\n');
		}
	}
	public static double[] Baum_Welch_Sub(String[] Seqs, double[][] StateTransMat, double[][] ExpProb, HashMap<Character, Integer> Alphabets) {
		int DNANum = Seqs.length;
		int StateNum = StateTransMat.length;
		int AlphabetNum = ExpProb[0].length;
		int N = Seqs[0].length();
		int[] Ns = new int[DNANum];
		for (int i = 0; i < DNANum; i++) {
			if (Seqs[i].length() > N)
				N = Seqs[i].length();
			Ns[i] = Seqs[i].length();
		}
		double[][][] ForDynamicTables = new double[DNANum][N][StateNum]; //Dynamic table of each DNA sequence of forward algorithm
		double[][][] BackDynamicTables = new double[DNANum][N][StateNum];//Dynamic table of each DNA sequence of backward algorithm
		double[][] AllExpSTM = new double[StateNum][StateNum]; //expectation of StateTransMat
		double[][] AllExpEP = new double[StateNum][AlphabetNum]; //expectation of ExpProb
		double[][] logSTM = new double[StateNum][StateNum]; //for ease of calculation, we take the logarithm of STM
		double[][] logEP = new double[StateNum][AlphabetNum]; // same as logSTM
		for (int i = 0; i < StateNum; i++) {
			for (int j = 1; j < StateNum; j++) {
				logSTM[i][j] = Math.log(StateTransMat[i][j]);
			}
			for (int j = 0; j < AlphabetNum; j++) {
				logEP[i][j] = Math.log(ExpProb[i][j]);
			}
		}
		double AllLiklihood = 0;
		for (int i = 0; i < DNANum; i++) {
			double[][] ExpSTM = new double[StateNum][StateNum]; //expectation of StateTransMat
			double[][] ExpEP = new double[StateNum][AlphabetNum]; //expectation of ExpProb
			double AllProb = 0; //対数全確率
			AllProb += Forward.ForwardSub(Seqs[i], StateTransMat, ExpProb, Alphabets, ForDynamicTables[i]);
			AllProb += Backward.BackwardSub(Seqs[i], StateTransMat, ExpProb, Alphabets, BackDynamicTables[i]);
			AllLiklihood += AllProb/2;
			AllProb = Math.exp(AllProb/2);
			int Alpnext = Alphabets.get(Seqs[i].charAt(0));
			for (int j = 1; j < StateNum; j++) {
				ExpSTM[0][j] = Math.exp(logSTM[0][j] + logEP[j][Alpnext] + BackDynamicTables[i][0][j]);
			}
			for (int j = 0; j < Ns[i]; j++) {
				int Alpnow = Alpnext;
				if (j < Ns[i]-1)
					Alpnext = Alphabets.get(Seqs[i].charAt(j+1));
				for (int k = 1; k < StateNum; k++) {
					//calculate the expectation of STM
					if (j < Ns[i]-1) {
						for (int l = 1; l < StateNum; l++) {
							double tmp = ForDynamicTables[i][j][k] + logSTM[k][l] + logEP[l][Alpnext] + BackDynamicTables[i][j+1][l];
							ExpSTM[k][l] += Math.exp(tmp);
						}
					}
					//calculate the expectation of ExpProb
					double tmp = ForDynamicTables[i][j][k] + BackDynamicTables[i][j][k];
					ExpEP[k][Alpnow] += Math.exp(tmp);
				}
			}
			//divide by entire probablity
			for (int j = 0; j < StateNum; j++) {
				for (int k = 0; k < StateNum; k++) {
					AllExpSTM[j][k] += ExpSTM[j][k] / AllProb;
				}
				for (int k = 0; k < AlphabetNum; k++) {
					AllExpEP[j][k] += ExpEP[j][k] / AllProb;
				}
			}
		}
		System.out.println(AllLiklihood);
		double[] A = new double[2];//A[0] ... sum of square difference on index of STM. A[1] ... that of EP
		//replaced by new parameters
		for (int i = 0; i < StateNum; i++) {
			double AllSTM = 0;
			double AllEP = 0;
			for (int j = 0; j < StateNum; j++) 
				AllSTM += AllExpSTM[i][j];
			for (int j = 0; j < AlphabetNum; j++)
				AllEP += AllExpEP[i][j];
			for (int j = 0; j < StateNum; j++) {
				double tmp = StateTransMat[i][j] - AllExpSTM[i][j]/AllSTM;
				A[0] += tmp*tmp;
				StateTransMat[i][j] -= tmp;
				//System.out.println("STM[" + i +  "][" + j + "] = " + StateTransMat[i][j]);
			}
			if (i > 0) {
				for (int j = 0; j < AlphabetNum; j++) {
					double tmp = ExpProb[i][j] - AllExpEP[i][j]/AllEP;
					A[1] += tmp*tmp;
					ExpProb[i][j] -= tmp;
					//System.out.println(ExpProb[i][j]);
				}
			}
		}
		return A;
	}
	public static double[][][] readParams(double[][] StateTransMat, double[][] ExpProb, HashMap<Character, Integer> Alphabets) throws IOException{
		BufferedReader din = new BufferedReader(new FileReader("HMMParams.txt"));
		int i = 0; //段階の処理に使う
		String target = "([0-9\\.])+"; //searching the string that express number (includes real number)
		Pattern p = Pattern.compile(target);
		String target2 = "(?<= )((.+)$)";
		Pattern p2 = Pattern.compile(target2);
		int AlphabetNum = 1;
		int StateNum = 1;
		while (i < 5) {
			String s = din.readLine();
			if (s.charAt(0) != '%') {
				switch (i) {
				case 0: //alphabet size
				{
					Matcher m;
					if ((m = p.matcher(s)).find()) {
			    		String matchedText = m.group();
						AlphabetNum = Integer.parseInt(matchedText); // to integer
			    	}
				}
					break;
				case 1:  //alphabet
					//alphabetを読むとき
					for (int j = 0; j < AlphabetNum; j++) {
						Alphabets.put(s.charAt(j*2), j);
						//読んでいく
					}
					break;
				case 2: //the number of state
				{
					Matcher m;
					if ((m = p.matcher(s)).find()) {
			    		String matchedText = m.group();
						StateNum = Integer.parseInt(matchedText); // to integer
			    	}
				}
					break;
				case 3: //state transition matrix
				{
					StateTransMat = null;
					StateTransMat = new double[StateNum][StateNum];
					for (int j = 0; j < StateNum; j++) {
						Matcher m;
						if ((m = p.matcher(s)).find()) {
							String matchedText = m.group();
							StateTransMat[0][j] = Double.parseDouble(matchedText); // to integer
							if (j != StateNum-1) {
								Matcher m2;
								if ((m2 = p2.matcher(s)).find()) {
									s = m2.group();
								}
								else {
									System.out.println("no");
								}
							}
						}
					}
					for (int j = 1; j < StateNum; j++) {
						s = din.readLine();
						for (int k = 0; k < StateNum; k++) {
							Matcher m;
							if ((m = p.matcher(s)).find()) {
								String matchedText = m.group();
								StateTransMat[j][k] = Double.parseDouble(matchedText); // to integer
								if (k != StateNum-1) {
									Matcher m2;
									if ((m2 = p2.matcher(s)).find()) {
										s = m2.group();
									}
								}
							}
						}
					}
				}
					break;
				case 4: //expression probability
				{
					ExpProb = null;
					ExpProb = new double[StateNum][AlphabetNum];
					for (int j = 0; j < AlphabetNum; j++) {
						ExpProb[0][j] = 0;
					}
					for (int j = 0; j < AlphabetNum; j++) {
						Matcher m;
						if ((m = p.matcher(s)).find()) {
							String matchedText = m.group();
							ExpProb[1][j] = Double.parseDouble(matchedText); // to integer
							if (j != AlphabetNum-1) {
								Matcher m2;
								if ((m2 = p2.matcher(s)).find()) {
									s = m2.group();
								}
							}
						}
					}
					for (int j = 2; j < StateNum; j++) {
						s = din.readLine();
						for (int k = 0; k < AlphabetNum; k++) {
							Matcher m;
							if ((m= p.matcher(s)).find()) {
								String matchedText = m.group();
								ExpProb[j][k] = Double.parseDouble(matchedText); // to integer
								if (k != AlphabetNum-1) {
									Matcher m2;
									if ((m2= p2.matcher(s)).find()) {
										s = m2.group();
									}
								}
							}
						}
					}
				}
					break;
				default: System.out.println("何かの間違い"); break;
				}
				i++;
			}
		}
		din.close();
		double[][][] S = new double[2][][];
		S[0] = StateTransMat;
		S[1] = ExpProb;
		return S;
	}
	
	public static String readSeqs(String FileName) throws IOException {
		BufferedReader din = new BufferedReader(new FileReader(FileName));
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
}