import java.util.List;

public class Wynik
{

    public Wynik(String nazwaRasy)
    {
        this.rasa = nazwaRasy;
    }

    public String rasa;
    public double dopasowanie;

    @Override
    public boolean equals(Object obj)
    {
        return (this.rasa.equals(((Wynik) obj).rasa));
    }

    public static int getIndex(List<Wynik> wyjscieRasy, String nazwaRasy)
    {
        for (int i=0;i< wyjscieRasy.size();i++)
        {
            if(wyjscieRasy.get(i).rasa.equals(nazwaRasy) )
            {
                return i;
            }
        }
        return 0;
    }
}
