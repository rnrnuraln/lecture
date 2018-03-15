package logist_reg;

import java.io.IOException;

public class Imaging {
	public static void main(String[] args) {
		logist L = new logist(1000, 5);
		try {L.readData("test_d01234_n1000.txt");} catch (IOException e) {return;}
		try {L.MakeImage(5);} catch (IOException e) {System.out.println("fe"); return;}
	}
}