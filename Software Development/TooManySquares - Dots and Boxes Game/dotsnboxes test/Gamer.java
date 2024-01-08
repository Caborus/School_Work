public class Gamer {
    String name = "";
    int score = 0;
    int gamerNumber = 0;
    int totalGamers = 0;


    public void SetTotalGamers(int n){
        this.totalGamers = n;
    }
    public void SetGamerName(String n){
        n = ConvertTo1337(n);
        this.name = n;
    }

    public void SetGamerScore(int s){
        this.score = s;
    }

    public void SetGamerNumber(int num){
        this.gamerNumber = num;
    }

    public int GetTotalGamers(){
        return this.totalGamers;
    }

    public String GetGamerName(){
        return this.name;
    }

    public int GetGamerScore(){
        return this.score;
    }

    public int GetGamerNumber(){
        return this.gamerNumber;
    }

    /*
    public String CompareScore(int s1, int s2){
        if(s1 > s2)
            return s1;
        else
            return s2;
    }
    */


    public String ConvertTo1337(String n){

        String leetName = "";
        String[][] leetChars = {{"/-\\", "/\\", "4", "@"}, // a
            {"|3", "8", "|o"}, // b
            {"(", "<"}, // c
            {"|)", "o|", "|>", "<|"}, // d
            {"3"}, // e
            {"|=", "ph"}, // f
            {"9", "6"}, // g
            {"|-|", "]-[", "}-{", "#"}, // h
            {"|", "]["}, // i
            {"_|"}, // j
            {"|<"},  // k
            {"|_", "1"}, // l
            {"|\\/|", "/\\/\\", "/v\\"}, // m
            {"|\\|", "/\\/"}, // n
            {"0", "()"}, // o
            {"|*"},// p
            {"(,)", "kw"}, // q
            {"|2", "|Z"}, // r
            {"5", "$"},  // s
            {"+", "'}['", "7"}, // t
            {"|_|"}, // u
            {"\\/", "|/", "\\|"}, // v
            {"\\/\\/", "\\|\\|", "\\|/", "\\^/"}, // w
            {"><", "}{"}, // x
            {"'/", "`/", "j"}, // y 
            {"2", "(\\)"}}; // z

        for(int i = 0; i < n.length(); i++){
            char c = n.charAt(i);
            switch (c){
                case 'a':
                case 'A':
                    leetName += leetChars[0][(int)(Math.random() * 4)];
                    break;
                case 'b':
                case 'B':
                    leetName += leetChars[1][(int)(Math.random() * 3)];
                    break;
                case 'c':
                case 'C':
                    leetName += leetChars[2][(int)(Math.random() * 2)];
                    break;
                case 'd':
                case 'D':
                    leetName += leetChars[3][(int)(Math.random() * 4)];
                    break;
                case 'e':
                case 'E':
                    leetName += leetChars[4][0];
                    break;
                case 'f':
                case 'F':
                    leetName += leetChars[5][(int)(Math.random() * 2)];
                    break;
                case 'g':
                case 'G':
                    leetName += leetChars[6][(int)(Math.random() * 2)];
                    break;
                case 'h':
                case 'H':
                    leetName += leetChars[7][(int)(Math.random() * 4)];
                    break;
                case 'i':
                case 'I':
                    leetName += leetChars[8][(int)(Math.random() * 2)];
                    break;
                case 'j':
                case 'J':
                    leetName += leetChars[9][0];
                    break;
                case 'k':
                case 'K':
                    leetName += leetChars[10][0];
                    break;
                case 'l':
                case 'L':
                    leetName += leetChars[11][(int)(Math.random() * 4)];
                    break;
                case 'm':
                case 'M':
                    leetName += leetChars[12][(int)(Math.random() * 3)];
                    break;
                case 'n':
                case 'N':
                    leetName += leetChars[13][(int)(Math.random() * 2)];
                    break;
                case 'o':
                case 'O':
                    leetName += leetChars[14][(int)(Math.random() * 2)];
                    break;
                case 'p':
                case 'P':
                    leetName += leetChars[15][0];
                    break;
                case 'q':
                case 'Q':
                    leetName += leetChars[16][(int)(Math.random() * 2)];
                    break;
                case 'r':
                case 'R':
                    leetName += leetChars[17][(int)(Math.random() * 2)];
                    break;
                case 's':
                case 'S':
                    leetName += leetChars[18][(int)(Math.random() * 2)];
                    break;
                case 't':
                case 'T':
                    leetName += leetChars[19][(int)(Math.random() * 3)];
                    break;
                case 'u':
                case 'U':
                    leetName += leetChars[20][0];
                    break;
                case 'v':
                case 'V':
                    leetName += leetChars[21][(int)(Math.random() * 3)];
                    break;
                case 'w':
                case 'W':
                    leetName += leetChars[22][(int)(Math.random() * 4)];
                    break;
                case 'x':
                case 'X':
                    leetName += leetChars[23][(int)(Math.random() * 2)];
                    break;
                case 'y':
                case 'Y':
                    leetName += leetChars[24][(int)(Math.random() * 3)];
                    break;
                case 'z':
                case 'Z':
                    leetName += leetChars[25][(int)(Math.random() * 2)];
                    break;
                default:
                    leetName += c;
            }
        }
        return(leetName);
    }
    
}
