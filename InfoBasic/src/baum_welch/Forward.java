package baum_welch;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Forward {
	public static void main(String[] args) {
		//first, we must construct IHMM from the file
		//however, we do not have to actually construct IHMM, when we conduct only viterbi algorithm
		//状態遷移行列と出力確率さえあれば問題ない
		int StateNum = 1;
		HashMap<Character, Integer> Alphabets = new HashMap<Character, Integer>();
		//file reading
		
		double[][] StateTransMat = new double[3][3];
		double[][] ExpProb = new double[3][3];
		double[][][] S = new double[2][][];
		//the reading of param file
		try {
			S = Baum_Welch.readParams(StateTransMat, ExpProb, Alphabets);
			StateTransMat = S[0];
			ExpProb = S[1];
			//the reading of sequence
			String Seq = "d";
			try {
				Seq = Baum_Welch.readSeqs("Baum_Welches/DNA0.txt");
				StateNum = StateTransMat[0].length;
				int N = Seq.length();
				double[][] DynamicTable = new double[N][StateNum]; //動的計画法の途中の値を記録
				double AllScore2 = ForwardSub(Seq, StateTransMat, ExpProb, Alphabets, DynamicTable);
				System.out.println("AllScore: "+ AllScore2);
			}
			catch (IOException error) {
				System.out.println("there is an io error");
			}
		}
		catch (IOException error) {
			System.out.println("there is an io error");
		}
	}
	public static double ForwardSub(String Seq, double[][] StateTransMat, double[][] ExpProb, HashMap<Character, Integer> Alphabets, double[][] DynamicTable) {
		// 
		int N = Seq.length();
		int StateNum = StateTransMat.length;
		double[] Scale = new double[N]; 
		//initialization
		for (int i = 1; i < StateNum; i++) {
			int seqint = Alphabets.get(Seq.charAt(0));
			DynamicTable[0][i] = StateTransMat[0][i] * ExpProb[i][seqint];
		}
		Scale[0] = Math.log(1);
		for (int i = 1; i < N; i++) {
			int seqint = Alphabets.get(Seq.charAt(i));
			for (int j = 1; j < StateNum; j++) {
				//以下、各状態までの全ての確率の和を取っていく
				for (int k = 1; k < StateNum; k++) {
					DynamicTable[i][j] += DynamicTable[i-1][k]*StateTransMat[k][j];
				}
				DynamicTable[i][j] = DynamicTable[i][j] * ExpProb[j][seqint];
				Scale[i] += DynamicTable[i][j];
			}
			//scaling; so that the max probability of each state should not be so low
			//and so that the sum of probability of all state should not be so low
			//with that, max go to prevmax
			for (int j = 1; j < StateNum; j++) {
				DynamicTable[i][j] = DynamicTable[i][j] / Scale[i];
				DynamicTable[i-1][j] = Math.log(DynamicTable[i-1][j]) + Scale[i-1];
			}
			Scale[i] = Math.log(Scale[i]) + Scale[i-1];
		}
		double AllScore = 0;//返り値。全確率
		//termination
		for (int j = 0; j < StateNum; j++) {
			AllScore += DynamicTable[N-1][j];
			DynamicTable[N-1][j] = Math.log(DynamicTable[N-1][j]) + Scale[N-1];
		}
		AllScore = Math.log(AllScore) + Scale[N-1];
		return AllScore;
	}
}