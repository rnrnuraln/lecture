package viterbi;

import java.io.*;
import java.util.HashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Viterbi {
	public static void main(String[] args) {
		//first, we must construct IHMM from the file
		//however, we do not have to actually construct IHMM, when we conduct only viterbi algorithm
		//状態遷移行列と出力確率さえあれば問題ない
		int AlphabetNum = 1;
		int StateNum = 1;
		HashMap<Character, Integer> Alphabets = new HashMap<Character, Integer>();
		//file reading
		
		double[][] StateTransMat = new double[3][3];
		double[][] ExpProb = new double[3][3];
		double[][][] S = new double[2][][];
		//the reading of param file
		try {
			S = readParams(StateTransMat, ExpProb, Alphabets);
		}
		catch (IOException error) {
			System.out.println("there is an io error");
		}
		StateTransMat = S[0];
		ExpProb = S[1];
		//the reading of sequence
		String Seq = "d";
		try {
			Seq = readSeqs();
		}
		catch (IOException error) {
			System.out.println("there is an io error");
		}
		StateNum = StateTransMat[0].length;
		AlphabetNum = Alphabets.size();
		
		System.out.println(Seq);
		
		int N = Seq.length();
		
		int[][] trb = new int[StateNum][N]; //for traceback
		//dynamic programming
		//for each state, we must memory the max score of viterbi algorithm
		//we also must memory the way they have gone.
		double[] MaxScore = new double[StateNum];
		double[] PrevMax = new double[StateNum];
		double scale = 0;
		//initialization
		System.out.println(StateNum);
		System.out.println(AlphabetNum);
		for (int i = 1; i < StateNum; i++) {
			int seqint = Alphabets.get(Seq.charAt(0));
			trb[i][0] = 0;
			MaxScore[i] = StateTransMat[0][i] * ExpProb[i][seqint];
			PrevMax[i] = MaxScore[i];
		}
		//loop
		for (int i = 1; i < Seq.length(); i++) {
			scale = 0;
			int seqint = Alphabets.get(Seq.charAt(i));
			for (int j = 1; j < StateNum; j++) {
				//以下、各状態までのmaxを取っていく
				int Max = 1;
				MaxScore[j] = PrevMax[1]*StateTransMat[1][j];
				for (int k = 2; k < StateNum; k++) {
					double A = PrevMax[k]*StateTransMat[k][j];
					if (A > MaxScore[j]) {
						MaxScore[j] = A;
						Max = k;
					}
				}
				MaxScore[j] = MaxScore[j] * ExpProb[j][seqint];
				trb[j][i] = Max;
				scale += MaxScore[j];
			}
			//scaling; so that the max probability of each state should not be so low
			//and so that the sum of probability of all state should not be so low
			//with that, max go to prevmax
			for (int j = 1; j < StateNum; j++) {
				PrevMax[j] = MaxScore[j] / scale;
			}
		}
		//trbの0行目は何にも入っていないから、ここをtracebackとして使う
		//以下、まずどこからtracebackを行うべきか見る
		int Maxtrb = 1;
		double MaxSc = MaxScore[1];
		for (int i = 2; i < StateNum; i++) {
			if (MaxScore[i] > MaxSc) {
				Maxtrb = i;
				MaxSc = MaxScore[i];
			}
		}
		//Maxtrbのところからtracebackを行う
		//以下、Maxtrbを用いてtrackbackを行う
		trb[0][N-1] = Maxtrb;
		for (int i = N-1; i > 0; i--) {
			Maxtrb = trb[Maxtrb][i];
			trb[0][i-1] = Maxtrb;
		}
		System.out.print(0);
		for (int i = 0; i < N; i++) {
			System.out.print(trb[0][i]);
		}
		try {
			output(trb[0]);
		}
		catch (IOException e) {
			System.out.println("IOError");
		}
		
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
					System.out.println(s);
					Matcher m;
					if ((m = p.matcher(s)).find()) {
			    		String matchedText = m.group();
			    		System.out.println(matchedText);
						AlphabetNum = Integer.parseInt(matchedText); // to integer
			    	}
				}
					break;
				case 1:  //alphabet
					//alphabetを読むとき
					for (int j = 0; j < AlphabetNum; j++) {
						Alphabets.put(s.charAt(j*2), j);
						System.out.println(s.charAt(j*2));
						//読んでいく
					}
					break;
				case 2: //the number of state
				{
					System.out.println(s);
					Matcher m;
					if ((m = p.matcher(s)).find()) {
			    		String matchedText = m.group();
			    		System.out.println(matchedText);
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
							System.out.println(matchedText);
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
	
	public static void output(int[] trb) throws IOException {
		int n = trb.length;
		FileWriter filewriter = new FileWriter("output.txt");
		for (int i = 0; i < n; i++) {
			filewriter.write(trb[i]);
		}
	}
}