public class HelloWorld {
	public static void main(String args[]) {
		System.out.println("Hello World!");
		Person p = new Person();
		p.name = "Thomias";
		p.firstname = "Julia";
		p.age = 18;
		p.talk();
		
		Person q = new Person();
		q.name = "Thomias";
		q.firstname = "Yannis";
		q.age = 23;
		q.talk();
		
		int a = 3;
		int b = 0;
		int c = Calc.divide(a, b);
		System.out.println(a + "/" + b + "=" + c);
	}
}