import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class PowlokaDoborPsa
{

    private static final int liczbaArgumentow = 0;
    private static final int iloscRasPsow = 0;
    public Siec siec;
    private Argument[] argumenty = new Argument[liczbaArgumentow];
    private List<Wynik> wyjscieRasy = new ArrayList<Wynik>();
    private Map<String, Integer> mapaWynikow = new HashMap<>();

    public List<Wynik> run(String[] input)
    {
        double wejscieDlaSieci[] = new double[liczbaArgumentow];
        // Iterator<String> inputIterator = input.iterator();
        for (int i = 0; i < argumenty.length; i++)
        {
            wejscieDlaSieci[i] = argumenty[i].wartosci.get(input[i]);
        }
        // for (Argument argument : argumenty)
        // {
        // double argumentLiczba = 0;
        // if (inputIterator.hasNext())
        // {
        // argumentLiczba = argument.wartosci.get(inputIterator.next());
        // }
        // wejscieDlaSieci[i] = argumentLiczba;
        // i++;
        // }
        double wyjscie[] = siec.run(wejscieDlaSieci);

        Iterator<Wynik> iteratorWyjscieRasy = wyjscieRasy.iterator();
        for (double dopasowanieRasy : wyjscie)
        {
            iteratorWyjscieRasy.next().dopasowanie = dopasowanieRasy;
        }
        return wyjscieRasy;
    }

    public void learn(String[][] lrnString)
    {
        generowanieMapyLrnStringDoLrndouble(lrnString);
        Przyklad[] vektorPrzykladow = generujWektorPrzykladowDlaSieci(lrnString);
        tworzenieSieci();

        siec.learn(vektorPrzykladow);
    }

    private void tworzenieSieci()
    {
        siec= new Siec(argumenty.length,wyjscieRasy.size());
    }

    private Przyklad[] generujWektorPrzykladowDlaSieci(String[][] lrnString)
    {
        List<Przyklad> przyklady = new ArrayList<>();
        for (String[] wiersz : lrnString)
        {
            przyklady.add(generujPrzyklad(wiersz));
        }

        return przyklady.toArray(new Przyklad[0]);
    }

    private Przyklad generujPrzyklad(String[] wiersz)
    {
        Przyklad przyklad = new Przyklad();
        przyklad.wejscie= new double[wiersz.length - 1];
         przyklad.zadaneWyjscie= new double[wyjscieRasy.size()];

        for (int i = 0; i < przyklad.wejscie.length; i++)
        {
            przyklad.wejscie[i] = argumenty[i].wartosci.get(wiersz[i]);
        }

        for (int i = 0; i < przyklad.zadaneWyjscie.length; i++)
        {
            int indexRasy = Wynik.getIndex(wyjscieRasy,wiersz[wiersz.length - 1]);

            for (int indexWyjscia=0; indexWyjscia<przyklad.zadaneWyjscie.length;indexWyjscia++)
            {
                przyklad.zadaneWyjscie[indexWyjscia] = 0;
            }
            przyklad.zadaneWyjscie[indexRasy] = 1;
        }
        return przyklad;
    }

    private void generowanieMapyLrnStringDoLrndouble(String[][] lrnString)
    {
        argumenty = new Argument[lrnString[0].length - 1];

        for (int indexArgument = 0; indexArgument < argumenty.length; indexArgument++)
        {
            double aktualnieWiazanaWartosc = 0;
            argumenty[indexArgument] = new Argument();
            for (int indexPrzyklad = 0; indexPrzyklad < lrnString.length; indexPrzyklad++)
            {
                // if(argumenty[indexArgument].wartosci.contains)

                if (!argumenty[indexArgument].wartosci.containsKey(lrnString[indexPrzyklad][indexArgument]))
                {
                    argumenty[indexArgument].wartosci.put(lrnString[indexPrzyklad][indexArgument],
                            aktualnieWiazanaWartosc);
                    aktualnieWiazanaWartosc += 1;
                }
            }
        }

        int indexKolumnyWynik = lrnString[0].length - 1;
        int aktualnyIndexWyniku = 0;
        for (int i = 1; i < lrnString.length; i++)
        {
            String nazwaRasy = lrnString[i][indexKolumnyWynik];
            if (!wyjscieRasy.contains(new Wynik(nazwaRasy)))
            {
                wyjscieRasy.add(new Wynik(nazwaRasy));
            }

            // Integer czyDodano = mapaWynikow.put(lrnString[i][indexKolumnyWynik],
            // aktualnyIndexWyniku);
            // if (czyDodano == null)
            // {
            // aktualnyIndexWyniku += 1;
            // }
        }
    }

    // private boolean czyJestJuzTakaRasa(String nazwaRasy)
    // {
    // return wyjscieRasy.contains(new Wynik(nazwaRasy));
    // }
}
