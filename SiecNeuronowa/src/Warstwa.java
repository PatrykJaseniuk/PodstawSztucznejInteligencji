import java.util.Random;

public class Warstwa
{

    // model neuronu:
    //
    // bias--------------+
    // |
    // wektorWag---------+---->wzbudzenieNeuronu---->aktywacjaNeuronu(wyjscie)
    // |
    // wejscie(wektor)---+

    public double[] wyjscie;
    private double[] wejscie = null;
    private double[][] macierzWag = new double[0][0];
    private double[] wektorBias = new double[0];

    private double krokUczenia;
    private double[] pochodneKosztuPoWzbudzeniuNeuronow;
    private double[] wektorWzbudzen;
    private double[][] macierzZmianWagNeuronow = new double[0][0];
    private double[] wektorZmianBias = new double[0];

    public Warstwa(double[] wyjscie)
    {
        this.wyjscie = wyjscie;
    }

    public Warstwa(int iloscNeuronowWyjsciowych)
    {
        // macierzWag= new double[iloscNeuronowWejsciowych][il]
        wyjscie = new double[iloscNeuronowWyjsciowych];
    }

    public Warstwa(Warstwa warstwa, int iloscNeuronow)
    {
        wejscie = new double[warstwa.wyjscie.length];
        wyjscie = new double[iloscNeuronow];
        macierzWag = new double[iloscNeuronow][warstwa.wyjscie.length];
        wektorBias = new double[iloscNeuronow];

        krokUczenia = 0.005;
        pochodneKosztuPoWzbudzeniuNeuronow = new double[iloscNeuronow];
        wektorWzbudzen = new double[iloscNeuronow];
        macierzZmianWagNeuronow = new double[iloscNeuronow][warstwa.wyjscie.length];
        wektorZmianBias = new double[iloscNeuronow];
        inicializujJedynkami();
    }

    private void inicializujJedynkami()
    {
        for (int i = 0; i < wektorBias.length; i++)
        {
            wektorBias[i] = 1;
        }

        for (int i = 0; i < macierzWag.length; i++)
        {
            for (int j = 0; j < macierzWag[i].length; j++)
            {
                macierzWag[i][j] = 1;
            }
        }
    }

    public void oblicz(double[] wyjscieWarstwyPoprzedniej)
    {
        this.wejscie = wyjscieWarstwyPoprzedniej;
        double[] wektor1 = mnozenieMacierzyWagIWektoraWejsciowego(macierzWag, wyjscieWarstwyPoprzedniej);
        wektorWzbudzen = dodanieBias(wektor1, wektorBias);
        wyjscie = obliczenieSigmoidy(wektorWzbudzen);
    }

    private double[] obliczenieSigmoidy(double[] wektorWzbudzen)
    {
        double[] wyjscie = new double[wektorWzbudzen.length];
        for (int i = 0; i < wektorWzbudzen.length; i++)
        {
            if (wektorWzbudzen[i] > 300) //warunki potrzebne aby uniknac problemow z przpelnieniem
            {
                wyjscie[i] = 1;
            }
            else if (wektorWzbudzen[i] < -300)
            {
                wyjscie[i] = 0;
            }
            else
            {
                wyjscie[i] = (double) (1.0 / (1.0 + Math.exp(-wektorWzbudzen[i])));
            }
            if (Double.isNaN(wyjscie[i]))
            {
                int j = 0;
            }   
        }
        return wyjscie;
    }

    private double[] dodanieBias(double[] wektor1, double[] wektorBias2)
    {
        double[] wektor2 = new double[wektor1.length];

        for (int i = 0; i < wektor1.length; i++)
        {
            wektor2[i] = wektor1[i] + wektorBias[i];
        }
        return wektor2;
    }

    private double[] mnozenieMacierzyWagIWektoraWejsciowego(double[][] macierzWag2, double[] wyjscieWarstwyPoprzedniej)
    {
        double[] wektor1 = new double[macierzWag.length];
        for (int i = 0; i < macierzWag.length; i++)
        {
            wektor1[i] = iloczynSkalarny(wyjscieWarstwyPoprzedniej, macierzWag[i]);
        }
        return wektor1;
    }

    private double iloczynSkalarny(double[] wektor1, double[] wektor2)
    {
        double wynik = 0;
        for (int i = 0; i < wektor1.length; i++)
        {
            wynik += wektor1[i] * wektor2[i];
        }
        return wynik;
    }

    public void dodajZmianeParametrow(double[] zadaneWyjscie)
    {
        for (int i = 0; i < macierzWag.length; i++)
        {
            double pochodnaKosztuPoWyjsciuNeuronu = getPochodnaKosztuPoWyjsciuNeuronu(wyjscie[i], zadaneWyjscie[i]);
            double pochodnaWyjsciaNeuronuPoWzbudzeniuNeuronu = getPochodnaWyjsciaPoWzbudzeniuNeuronuProstrza(
                    wyjscie[i]); //getPochodnaWyjsciaPoWzbudzeniuNeuronu(wektorWzbudzen[i]);
            pochodneKosztuPoWzbudzeniuNeuronow[i] = pochodnaKosztuPoWyjsciuNeuronu
                    * pochodnaWyjsciaNeuronuPoWzbudzeniuNeuronu;

            dodajZmianeWagNeuronu(i);
            dodajZmianeBiasNeuronu(i);
        }
    }

    private double getPochodnaWyjsciaPoWzbudzeniuNeuronuProstrza(double d)
    {
        return d*(1-d);
    }

    private void dodajZmianeBiasNeuronu(int i)
    {
        wektorZmianBias[i] += pochodneKosztuPoWzbudzeniuNeuronow[i] * -krokUczenia;
    }

    private void dodajZmianeWagNeuronu(int indexNeuronuAktualnejWarstwy)
    {
        double[] wektorZmianWagNeuronu = macierzZmianWagNeuronow[indexNeuronuAktualnejWarstwy];
        for (int indexNeuronuWarstwyPoprzedniej = 0; indexNeuronuWarstwyPoprzedniej < wektorZmianWagNeuronu.length; indexNeuronuWarstwyPoprzedniej++)
        {
            wektorZmianWagNeuronu[indexNeuronuWarstwyPoprzedniej] += pochodneKosztuPoWzbudzeniuNeuronow[indexNeuronuAktualnejWarstwy]
                    * wejscie[indexNeuronuWarstwyPoprzedniej] * -krokUczenia;
        }
    }

    private double getPochodnaWyjsciaPoWzbudzeniuNeuronu(double wzbudzenie)
    {
        if(wzbudzenie > 300 || wzbudzenie< -300) //aby uniknac przepelnienia
        {
            return 0;
        }
        double a = (double) (1 + Math.exp(-wzbudzenie));
        return (double) ((1.0 / (a * a)) * Math.exp(-wzbudzenie));
    }

    private double getPochodnaKosztuPoWyjsciuNeuronu(double wyjscie, double zadaneWyjscie)
    {
        return 2 * (wyjscie - zadaneWyjscie);
    }

    public void dodajZmianeParametrow(Warstwa nastepnaWarstwa)
    {
        for (int i = 0; i < macierzWag.length; i++)
        {
            double pochodnaKosztuPoWyjsciuNeuronu = getPochodnaKosztuPoWyjsciuNeuron(i, nastepnaWarstwa);
            double pochodnaWyjsciaNeuronuPoWzbudzeniuNeuronu = getPochodnaWyjsciaPoWzbudzeniuNeuronu(wektorWzbudzen[i]);
            pochodneKosztuPoWzbudzeniuNeuronow[i] = pochodnaKosztuPoWyjsciuNeuronu
                    * pochodnaWyjsciaNeuronuPoWzbudzeniuNeuronu;
            dodajZmianeWagNeuronu(i);
            dodajZmianeBiasNeuronu(i);
        }
    }

    private double getPochodnaKosztuPoWyjsciuNeuron(int indexNeuronuAktualnejWarstwy, Warstwa nastepnaWarstwa)
    {
        double pochodnaKosztuPoWyjsciuNeuronu = 0;
        for (int indexNeuronuNastepnejWarstwy = 0; indexNeuronuNastepnejWarstwy < nastepnaWarstwa.macierzWag.length; indexNeuronuNastepnejWarstwy++)
        {
            pochodnaKosztuPoWyjsciuNeuronu += nastepnaWarstwa.macierzWag[indexNeuronuNastepnejWarstwy][indexNeuronuAktualnejWarstwy]
                    * nastepnaWarstwa.pochodneKosztuPoWzbudzeniuNeuronow[indexNeuronuNastepnejWarstwy];
        }
        return pochodnaKosztuPoWyjsciuNeuronu;
    }

    public void inicjalizujParametry()
    {
        Random rand = new Random();
        for (int i = 0; i < wektorBias.length; i++)
        {
            wektorBias[i] = rand.nextDouble(2) - 1;
        }

        for (int i = 0; i < macierzWag.length; i++)
        {
            for (int j = 0; j < macierzWag[i].length; j++)
            {
                macierzWag[i][j] = rand.nextDouble(2) - 1;
            }
        }
    }

    public void przesunParametry()
    {
        for (int indexNeuronu = 0; indexNeuronu < macierzWag.length; indexNeuronu++)
        {
            for (int indexNeuronuWarstwyPoprzedniej = 0; indexNeuronuWarstwyPoprzedniej < macierzWag[0].length; indexNeuronuWarstwyPoprzedniej++)
            {
                macierzWag[indexNeuronu][indexNeuronuWarstwyPoprzedniej] += macierzZmianWagNeuronow[indexNeuronu][indexNeuronuWarstwyPoprzedniej];
            }
            wektorBias[indexNeuronu] += wektorZmianBias[indexNeuronu];
        }
    }

    public double wyznaczKoszt(Przyklad przyklad)
    {
        double suma = 0;
        for (int i = 0; i < wyjscie.length; i++)
        {            
            suma += (double) Math.pow((wyjscie[i] - przyklad.zadaneWyjscie[i]), 2);
        }
        return suma;
    }
}