package logist_reg;


import test_gradient.LBFGS;
import test_gradient.LBFGS.ExceptionWithIflag;

import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import javax.imageio.ImageIO;

public class logist {
	int yLen; //the number of class
	int DataNum; //the number of data
	int xLen; // the number of input variable + 1(includes the dummy variable)
	
	byte[][] xData; // data of x, input variable
	byte[][] tData; //data of t, real output variable. 1-of-K schema
	double[] w; //parameters of logistic regression, including b
	double lambda; //the weight of regularization term
	double[][] yData; //data of y, the output variable calculated from xData and w
	double ErrorRate; //the error rate when the data are tested
	//below are used for lbfgs, the gradient descent library
	double f; //error function; this should be minimized
	double[] g; //the gradient of the error function on w
	static double eps = 10.0;
	static double xtol = Double.MIN_VALUE;
	boolean diagco;
	double[] diag;
	int[] iflag;
	int[] iprint;
	static int m = 5;
	public logist(int dataNum, int yLen) {
		iflag = new int[1];
		iprint = new int[2];
		diagco = false;
		lambda = 1.0;
		DataNum = dataNum;
		this.yLen = yLen;
	}
	public static void main(String[] args) {
		logist L = new logist(1000, 5);
		//reading data
		try {
			L.readData("train_d01234_n1000.txt");
			//L.ReadImage("train-images.txt");
			//L.ReadLabels("train-labels.txt");
		} 
		catch (IOException e) {
			System.out.println("readparams Error!");
			return;
		}
		L.w = new double[L.yLen*L.xData[0].length];
		try {
			for (int i = 0; i < 200; i++) {
				L.trainingData();
				if (L.iflag[0] != 1) break;
			}
		} catch(ExceptionWithIflag e) {
			System.out.println("error!");
			return;
		}
		//below is for testing
		logist Test = new logist(1000, L.yLen);
		try {
			Test.readData("test_d01234_n1000.txt");
			//Test.ReadImage("test-images.txt");
			//Test.ReadLabels("test-labels.txt");
		} catch (IOException e) {
			System.out.println("readparams Error!");
			return;
		}
		Test.TestData(L.w);
	}
	public void readData(String FileName) throws IOException {
		BufferedReader din = new BufferedReader(new FileReader(FileName));
		String s = din.readLine();
		xLen = s.split("\t", -1).length - yLen + 1;
		xData = new byte[DataNum][xLen];
		tData = new byte[DataNum][yLen];
		String[] tmp;
		for (int i = 0; i < DataNum; i++) {
			s = din.readLine();
			tmp = s.split("\t", -1);
			for (int j = 0; j < yLen; j++) {
				tData[i][j] = Byte.parseByte(tmp[j]);
			}
			for (int j = yLen; j < xLen + yLen - 1; j++) {
				xData[i][j-yLen] = (byte) (Double.parseDouble(tmp[j])*255-128);
			}
			xData[i][xLen - 1] = 1;//the dummy variable; so that the product of x and w can be calculated easily.
		}
		din.close();
	}
	public void trainingData() throws ExceptionWithIflag {
		//here we calculate the error function and the gradient
		long start = System.currentTimeMillis();//for measuring consumed time
		yData = new double[DataNum][yLen];
		for (int i = 0; i < DataNum; i++) yData[i] = this.SoftMax(xData[i]);
		f = this.CrossEntropyError(); //calculate the cross entropy error function
		for (int i = 0; i < w.length; i++) f += 0.5*lambda*w[i]*w[i]; //the restriction on W
		g = this.Gradient();
		for (int i = 0; i < w.length; i++) g[i] += lambda*w[i]; //add the restriction effect on W
		diag = new double[w.length];
		for (int i = 0; i < w.length; i++) diag[i] = 1;
		iflag[0] = 0;
		LBFGS.lbfgs(w.length, m, w, f, g, diagco, diag, iprint, eps, xtol, iflag);
		long end = System.currentTimeMillis();
		double WNorm = 0;
		for (int i = 0; i < w.length; i++) WNorm += w[i]*w[i];
		System.out.println(Math.sqrt(WNorm));
		System.out.println((end - start) + "ms");
	}
	public void TestData(double[] w) {
		//here, we calculate and display cross entropy and error rate
		this.w = w;
		yData = new double[DataNum][yLen];
		for (int i = 0; i < DataNum; i++) yData[i] = this.SoftMax(xData[i]);
		ErrorRate = 0;
		for (int i = 0; i < DataNum; i++) {
			int j = MaxOfArray(yData[i]);
			if (tData[i][j] == 0) ErrorRate++;
		}
		ErrorRate = ErrorRate/DataNum;
		f = this.CrossEntropyError();
		System.out.println("Cross Entropy is " + CrossEntropyError()/DataNum);
		System.out.println("Error Rate is " + ErrorRate);
	}
	private double[] SoftMax(byte[] xdata) {
		double[] Output = new double[yLen]; //return this array
		double SumOfOutput = 0;
		int xLen = xdata.length;
		for (int i = 0; i < yLen; i++) {
			//calculate the product of w and x, and then exponent them
			for (int j = 0; j < xLen; j++) {
				Output[i] += (w[i*xLen+j] * xdata[j])/256;
			}
			Output[i] = Math.exp(Output[i]);
			SumOfOutput += Output[i];
		}
		for (int i = 0; i < yLen; i++) {
			Output[i] = Output[i] / SumOfOutput;
		}
		return Output;
	}
	public void ReadImage(String FileName) throws IOException {
		BufferedReader din = new BufferedReader(new FileReader(FileName));
		xLen = 785;
		xData = new byte[DataNum][xLen];
		String[] tmp;
		for (int i = 0; i < DataNum; i++) {
			String s = din.readLine();
			tmp = s.split(" ", -1);
			for (int j = 0; j < xLen-1; j++) {
				xData[i][j] = (byte) (Short.parseShort(tmp[j])-128);
			}
			xData[i][xLen - 1] = 1;//the dummy variable; so that the product of x and w can be calculated easily.
		}
		din.close();
	}
	public void ReadLabels(String FileName) throws IOException {
		BufferedReader din = new BufferedReader(new FileReader(FileName));
		tData = new byte[DataNum][yLen];
		for (int i = 0; i < DataNum; i++) {
			int j = Byte.parseByte(din.readLine());
			tData[i][j] = 1;
		}
		din.close();
	}
	public double CrossEntropyError() {
		double f = 0;
		for (int i = 0; i < DataNum; i++) {
			int j = 0;
			while (tData[i][j] != 1) j++;
			f -= Math.log(yData[i][j]);
		}
		return f;
	}
	private double[] Gradient() {
		int xLen = xData[0].length;
		double[] Grad = new double[xLen*yLen];
		for (int i = 0; i < xLen; i++) {
			for (int j = 0; j < yLen; j++) {
				for (int k = 0; k < DataNum; k++) {
					Grad[i+j*xLen] += (yData[k][j] - tData[k][j])*xData[k][i]/256;
				}
			}
		}
		return Grad;
	}
	private int MaxOfArray(double[] A) {
		int j = -1;
		double Max = -100000000;
		for (int i = 0; i < A.length; i++) {
			if (A[i] > Max) {
				Max = A[i];
				j = i;
			}
		}
		return j;
	}
	public void MakeImage(int i) throws IOException{
		int width = 28;
		int height = 28;
		BufferedImage bufImage = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
			for(int j=0;j<width;j++){
				for(int k=0;k<height;k++){
					int a = -(xData[i][j+k*width]-127);
					bufImage.setRGB(j, k, (0<<24|a<<16|a<<8|a));
				}
			}
			ImageIO.write(bufImage, "bmp", new File("ouput.bmp"));
	  }
}