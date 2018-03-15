package baum_welch;

import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Backward {
	public static void main(String[] args) {
		//first, we must construct IHMM from the file
		//however, we do not have to actually construct IHMM, when we conduct only viterbi algorithm
		//状態遷移行列と出力確率さえあれば問題ない
		int StateNum = 1;
		HashMap<Character, Integer> Alphabets = new HashMap<Character, Integer>();
		double[][] StateTransMat = new double[3][3];
		double[][] ExpProb = new double[3][3];
		double[][][] S = new double[2][][];
		//the reading of param file
		try {
			S = Baum_Welch.readParams(StateTransMat, ExpProb, Alphabets);
		}
		catch (IOException error) {
			System.out.println("there is an io error");
		}
		StateTransMat = S[0];
		ExpProb = S[1];
		
		//the reading of sequence
		String Seq = "d";
		try {
			Seq = Baum_Welch.readSeqs("Baum_Welches/DNA0.txt");

			StateNum = StateTransMat[0].length;
			int N = Seq.length();
			double[][] DynamicTable = new double[N][StateNum]; //動的計画法に用いる
			double AllScore = BackwardSub(Seq, StateTransMat, ExpProb, Alphabets, DynamicTable);
			System.out.println("AllScore: " + AllScore);
		}
		catch (IOException error) {
			System.out.println("there is an io error");
		}
	}
	
	public static double BackwardSub(String Seq, double[][] StateTransMat, double[][] ExpProb, HashMap<Character, Integer> Alphabets, double[][] DynamicTable) {
		//here we write the main part of the algorithm
		//this is also used in the BW-algorithm
		//void関数だが、ここでDynamicTableとScaleを書き換える
		//DynamicTableではbackwardのdynamic programmingの結果を入れる。
		int StateNum = StateTransMat[0].length;
		int N = Seq.length();
		double[] Scale = new double[N];
		//initialization
		for (int i = 1; i < StateNum; i++) {
			DynamicTable[N-1][i] = 1;
		}
		Scale[N-1] = Math.log(1);
		//loop
		for (int i = N-2; i > -1; i--) {
			int seqint = Alphabets.get(Seq.charAt(i+1));
			for (int j = 1; j < StateNum; j++) {
				//以下、各状態までの全ての確率の和を取っていく
				for (int k = 1; k < StateNum; k++) {
					DynamicTable[i][j] += DynamicTable[i+1][k]*StateTransMat[j][k]*ExpProb[k][seqint];
				}
				Scale[i] += DynamicTable[i][j];
			}
			//scaling; so that the max probability of each state should not be so low
			//and so that the sum of probability of all state should not be so low
			//with that, max go to prevmax
			for (int j = 1; j < StateNum; j++) {
				DynamicTable[i][j] = DynamicTable[i][j] / Scale[i];
				DynamicTable[i+1][j] = Math.log(DynamicTable[i+1][j]) + Scale[i+1];
			}
			Scale[i] = Math.log(Scale[i]) + Scale[i+1];
		}
		double AllScore = 0;//返り値。全確率
		//termination
		for (int i = 1; i < StateNum; i++) {
			int seqint = Alphabets.get(Seq.charAt(0));
			AllScore += DynamicTable[0][i]*StateTransMat[0][i]*ExpProb[i][seqint];
			DynamicTable[0][i] = Math.log(DynamicTable[0][i]) + Scale[0];
		}
		AllScore = Math.log(AllScore) + Scale[0];
		return AllScore;
	}
}