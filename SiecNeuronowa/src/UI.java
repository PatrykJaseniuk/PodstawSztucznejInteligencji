import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;

public class UI
{

    public PowlokaDoborPsa shellDogAdviser;

    public void run(String[] args)
    {
        String[] input = readInputFromFile(null);
        String[][] lrn = readLrnFromFile();

        if (lrn != null)
        {
            shellDogAdviser.learn(lrn);
        }
        try
        {
            List<Wynik> output = shellDogAdviser.run(input);

            for (Wynik wynik : output)
            {
                System.out.println("Rasa : "+ wynik.rasa+ "dopasowanie: "+ wynik.dopasowanie);
            }

        } catch (Exception e)
        {
            System.out.println(e.getMessage());
        }
    }

    private String[][] readLrnFromFile()
    {

        List<String[]> lrn = new ArrayList<>();
        try
        {
            File myObj = new File("C:\\S5\\PodstawSztucznejInteligencji\\SiecNeuronowa\\pliki\\rasa_psow_mniejsze_nazwy.lrn");
            Scanner myReader = new Scanner(myObj);
            pominiecieNaglowka(myReader);
            while (myReader.hasNextLine())
            {
                lrn.add((myReader.nextLine()).split("\t"));
            }
            myReader.close();
        } catch (FileNotFoundException e)
        {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
        return lrn.toArray(new String[0][0]);
    }

    private void pominiecieNaglowka(Scanner myReader)
    {
        myReader.nextLine();
        myReader.nextLine();
    }

    private String[] readInputFromFile(String string)
    {
        String[] input = null;
        try
        {
            File myObj = new File("C:\\S5\\PodstawSztucznejInteligencji\\SiecNeuronowa\\pliki\\input.in");
            Scanner myReader = new Scanner(myObj);
            odczytanieNaglowka(myReader);
            while (myReader.hasNextLine())
            {
                // String data = myReader.nextLine();
                // System.out.println(data);
                input = (myReader.nextLine()).split("[ \t]");
            }
            myReader.close();
        } catch (FileNotFoundException e)
        {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
        return input;
    }

    // private List<String> odczytajLinie(String string)
    // {
    //     List<String> linia = new LinkedList<String>();

    //     linia = ;

    //     return linia;
    // }

    private void odczytanieNaglowka(Scanner myReader)
    {
    }
    
}
