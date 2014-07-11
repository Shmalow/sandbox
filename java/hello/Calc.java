public class Calc {
	public static int divide(int a, int b) {
		int c = 0;
		try {
			c = a / b;
		} catch (Exception e) {
			//System.out.println("I have catched an exception.");
		}
		return c;
	}
}