public class Test
{
    Siec siec = new Siec(4, 4);
    double[] wejscie = { 1 };
    private double[] wyjscie;
    Test()
    {
        for (int i = 0; i < 100; i++)
        {
            wejscie[0] = i;
            wyjscie = siec.run(wejscie);
        }
    }
    
    void testUczenia()
    {
        Przyklad[] przyklady = new Przyklad[2];
        przyklady[0] = new Przyklad();
        przyklady[1]= new Przyklad();
        double[] wejscie = { 1,2,3,4 };
        double[] zadaneWyjscie = { 1, 0, 1, 0 };
        double[] wejscie1 = { 4,3,2,1 };
        double[] zadaneWyjscie1 = { 1, 1, 1, 1, };
        przyklady[0].wejscie = wejscie;
        przyklady[0].zadaneWyjscie = zadaneWyjscie;

        przyklady[1].zadaneWyjscie = zadaneWyjscie1;
        przyklady[1].wejscie = wejscie1;
        siec.learn(przyklady);
    }
    
    public static void main(String[] args)
    {        
       new Test().testUczenia();
    }
    
}
