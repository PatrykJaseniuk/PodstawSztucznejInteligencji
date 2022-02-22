import java.util.List;

public class Siec
{
    private Warstwa[] warstwy;
    private double zadanySredniKoszt;

    public Siec(int iloscNeuronowWejsciowych, int iloscNeuronowWyjsciowych)
    {
        warstwy = new Warstwa[3];

        warstwy[0] = new Warstwa(iloscNeuronowWejsciowych);
        warstwy[1] = new Warstwa(warstwy[0], 30);
        warstwy[2] = new Warstwa(warstwy[1],iloscNeuronowWyjsciowych);
    }

    public double[] run(double[] wejscie)
    {
        warstwy[0].wyjscie = wejscie;
        for (int i = 1; i < warstwy.length; i++)
        {
            double wyjscieWarstwyPoprzedniej[] = warstwy[i - 1].wyjscie;
            warstwy[i].oblicz(wyjscieWarstwyPoprzedniej);
        }
        return warstwy[warstwy.length - 1].wyjscie;
    }

    public void learn(Przyklad[] vektorPrzykladow)
    {
        double sredniKoszt = 0;
        inicjalizujParametry();
        do
        {
            wyznaczZmianyParametrowDlaWszystkichPrzykladow(vektorPrzykladow);
            PrzesunParametry();
            sredniKoszt = wyliczSredniKoszt(vektorPrzykladow);
            System.out.println("sredniKoszt: " + sredniKoszt);
        } while (sredniKoszt > zadanySredniKoszt);    
    }

    private void inicjalizujParametry()
    {
        for (int i = 1; i< warstwy.length; i++)
        {
            warstwy[i].inicjalizujParametry();            
        }
    }

    private void wyznaczZmianyParametrowDlaWszystkichPrzykladow(Przyklad[] vektorPrzykladow)
    {
        for (Przyklad przyklad : vektorPrzykladow)
        {
            dodajZmianeParametrowDlaJednegoPrzykladu(przyklad);
        }
        podzielZmianeParametrowPrzezLiczbePrzykladow();
    }

    private void podzielZmianeParametrowPrzezLiczbePrzykladow()
    {

    }

    private void dodajZmianeParametrowDlaJednegoPrzykladu(Przyklad przyklad)
    {
        run(przyklad.wejscie);
        // wyznaczKoszt(przyklad.zadaneWyjscie);
        warstwy[warstwy.length - 1].dodajZmianeParametrow(przyklad.zadaneWyjscie); 
        for (int i = warstwy.length - 1; i > 1;i--)
        {
            warstwy[i-1].dodajZmianeParametrow(warstwy[i]);
        }
    }

    private void wyznaczKoszt(double[] zadaneWyjscie)
    {
    }

    private double wyliczSredniKoszt(Przyklad[] vektorPrzykladow)
    {   
        double sumaKosztow=0;
        for (Przyklad przyklad : vektorPrzykladow)
        {
            run(przyklad.wejscie);
            sumaKosztow += warstwy[warstwy.length-1].wyznaczKoszt(przyklad);
        }
        double sredniKoszt = sumaKosztow / vektorPrzykladow.length;
        return sredniKoszt;
    }

    private void PrzesunParametry()
    {
        for(Warstwa warstwa: warstwy)
        {
            warstwa.przesunParametry();
        }
    }
}
