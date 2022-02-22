public class Program
{

    public static void main(String[] args)
    {
        UI ui = new UI();
        // Siec siec = new Siec();
        PowlokaDoborPsa powlokaDoborPsa = new PowlokaDoborPsa();
        // powlokaDoborPsa.siec = siec;
        ui.shellDogAdviser = powlokaDoborPsa;

        ui.run(args);
    }
}
