package logist_reg;

import java.io.IOException;

import test_gradient.LBFGS.ExceptionWithIflag;

public class CrossValid {
	int DividNum; //how many the data is divided to and cross-validated
	public static void main(String[] args) {
		CrossValid C = new CrossValid();
		C.Validate(5);
	}
	public void Validate(int dividNum) {
		int DataNum = 10000;
		this.DividNum = dividNum;
		logist AllData = new logist(DataNum, 10);
		try {
			//AllData.readData("train_d01234_n1000.txt");
			AllData.ReadImage("train-images.txt");
			AllData.ReadLabels("train-labels.txt");
		} catch (IOException e) {return;}
		logist[] L = new logist[DividNum];
		logist[] Test = new logist[DividNum];
		int EachDataNum = DataNum/DividNum;
		for (int i = 0; i < DividNum; i++) {
			L[i] = new logist(EachDataNum*(DividNum-1), AllData.yLen);
			L[i].xData = new byte[L[i].DataNum][AllData.xLen];
			L[i].tData = new byte[L[i].DataNum][AllData.yLen];
			L[i].w = new double[AllData.yLen*AllData.xLen];
			for (int j = 0; j < i*EachDataNum; j++) {
				for (int k = 0; k < AllData.xLen; k++) {
					L[i].xData[j][k] = AllData.xData[j][k];
				}
				for (int k = 0; k < AllData.yLen; k++) {
					L[i].tData[j][k] = AllData.tData[j][k];
				}
			}
			for (int j = (i+1)*EachDataNum; j < DataNum; j++) {
				for (int k = 0; k < AllData.xLen; k++) {
					L[i].xData[j-EachDataNum][k] = AllData.xData[j][k];
				}
				for (int k = 0; k < AllData.yLen; k++) {
					L[i].tData[j-EachDataNum][k] = AllData.tData[j][k];
				}
			}
			Test[i] = new logist(EachDataNum, AllData.yLen);
			Test[i].xData = new byte[Test[i].DataNum][AllData.xLen];
			Test[i].tData = new byte[Test[i].DataNum][AllData.yLen];
			for (int j = i*EachDataNum; j < (i+1)*EachDataNum; j++) {
				for (int k = 0; k < AllData.xLen; k++) {
					Test[i].xData[j-i*EachDataNum][k] = AllData.xData[j][k];
				}
				for (int k = 0; k < AllData.yLen; k++) {
					Test[i].tData[j-i*EachDataNum][k] = AllData.tData[j][k];
				}
			}
		}
		AllData.f = 0;
		AllData.w = new double[AllData.yLen*AllData.xLen];
		for (int i = 0; i < DividNum; i++) {
			try {for (int j = 0; j < 200; j++) L[i].trainingData();} catch (ExceptionWithIflag e) {System.out.println("error!"); return;}
			Test[i].TestData(L[i].w);
			for (int j = 0; j < AllData.w.length; j++) AllData.w[j] += L[i].w[j];
			AllData.f += Test[i].CrossEntropyError();
		}
		for (int i = 0; i < AllData.w.length; i++) AllData.w[i] = AllData.w[i]/DividNum;
		AllData.ErrorRate = 0;
		for (int i = 0; i < DividNum; i++) {
			AllData.ErrorRate += Test[i].ErrorRate;
		}
		System.out.println("error rate of all data is " + AllData.ErrorRate/DividNum);
		System.out.println("f = " + AllData.f);
		logist Test2 = new logist(5000, AllData.yLen);
		try {
			//Test2.readData("test_d01234_n1000.txt");
			Test2.ReadImage("test-images.txt");
			Test2.ReadLabels("test-labels.txt");
		} catch (IOException e) {
			System.out.println("readparams Error!");
			return;
		}
		Test2.TestData(AllData.w);
		
	}
}