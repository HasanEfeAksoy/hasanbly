#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<bool> unInterpreteLines; // comment and empty lines (if interpretable false, if uninterpretable true)
std::vector<std::string> stringVec; // değişkenleri tanımladığımız zaman depolanacak vector
std::vector<int> intVec; // int vectoru

void run(std::string& text, std::vector<std::string>& lines);
int main(int argc, char *argv[])
{
    if (!argv[1])
    {
        std::cout << "\nERROR:\nmessage: no input file.\n";
        return 0;
    }

    const std::string input = argv[1]; // hsnc.exe main.hsnc yazıldığı zaman main.hsnc yi alacak
    if (input.size() > 4 && input[input.size() - 4] == '.' && input[input.size() - 3] == 'h' && input[input.size() - 2] == 's' && input[input.size() - 1] == 'n')
    {
        std::string text = "";          // dosyanın tüm içeriği
        std::vector<std::string> lines; // dosyadaki tüm satırlar
        std::string newText = "";
        std::ifstream file(input);
        if (!file)
        {
            std::cout << "\nERROR:\nmessage: input file don't exist.\n";
            return 0;
        }

        while (std::getline(file, newText))
        {
            text += newText + "\n";
            lines.push_back(newText);
        }
        file.close();
        run(text, lines);
    }
    else
    {
        std::cout << "\nERROR:\nmessage: wrong input file type.\n";
        return 0;
    }
}
void run(std::string& text, std::vector<std::string>& lines)
{
    // unInterprete vectorune başlangıç değer ataması
    for (int i = 0; i < lines.size(); i++) {
        unInterpreteLines.push_back(false);
    }


    // yorum satırlarını ve boş satırları vectore atama
    for (int i = 0; i < lines.size(); i++)
    {
        if (lines[i].size() == 0 || (lines[i][0] == '/' && lines[i][1] == '/')) {
            unInterpreteLines[i] = true;
        }
    }


    // sol boşluk silme
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < lines[i].size(); j++)
        {
            if (lines[i][j] == ' ' || lines[i][j] == '\t') // her satırın her harfini 2 for ile dolaşıp başlarında boşluk olmayana kadar yani her satırın sol tarafındaki boşlukları siliyoruz.
            {
                lines[i].erase(lines[i].begin()); // begin() e +j yapmaya gerek yok çünkü spesifik eleman silmiyoruz. hep baştaki boşluk harfini siliyoruz.
                j--;                           // yukardaki nedenden dolayı j-=1
            }
            else
            {
                break;
            }
        }
    }
    

    // ; ile bitmeyen satır varsa error verme
    for (int i = 0; i < lines.size(); i++)
    {    
        if (unInterpreteLines[i]) { // uninterprete line a gelindiğinde görmezden gel ve geç
            continue;
        }


        if (lines[i][lines[i].size() - 1] != ';') // satırın ; ile bitip bitmediğini kontrol ediyoruz.
        {
            std::cout << "\nERROR:\nmessage: missing semicolon.\n";
            exit(0);
        }
        else {
            lines[i].erase(lines[i].begin() + lines[i].size() - 1); // sondaki noktalı virgülü siliyoruz.   
        }
    }


    // EXIT ile bitmediyse error verme
    for (int i = lines.size() - 1; i >= 0; i--) { // for döngüsünü sondan başlattık çünkü exit sonda mı onu kontrol ettireceğiz.
        if (unInterpreteLines[i]) { // eğer exit komutundan sonra uninterprete satırlar varsa onları görmezden gel.
            continue;
        }

        if (lines[i].size() != 4 || lines[i][0] != 'E' || lines[i][1] != 'X' || lines[i][2] != 'I' || lines[i][3] != 'T') { // sonda exit var mı diye kontrol ediyoruz yoksa error
            std::cout << "\nERROR:\nmessage: program needs EXIT command at the end of the program.\n";
            exit(0);
        }
        else {
            break; // sonda EXIT kullanmış demek oluyor.
        }
    }


    


    for (int i = 0; i < lines.size(); i++)
    {
        if (unInterpreteLines[i]) { // uninterprete olan (comment lines and empty lines) satırlar varsa geçmek
            continue;
        }


        if (lines[i][0] == '<') // print etmek için ilk harf ne onu kontrol ediyoruz. yukarda zaten soldaki boşlukları siliğimiz için ilk harf diye kontrol edebiliriz.
        {
            std::string myLine = lines[i];

            for (int j = 0; j < myLine.size(); j++) // her satırın her harfini geziyoruz 2 for döngüsü ile.
            {
                // sadece $ (dolar simgesi) yazdırmak istersek yani ^ bundan sonraki ilk karakter göz ardı edilecek ve yazılacak.
                if (myLine[j] == '^') {
                    myLine.erase(myLine.begin() + j);
                    //j++; //erase ettiğimiz için j yi arttırmaya gerek kalmadı.
                    continue;
                }
                else if (myLine[j] == '$') //$ işareti buluyoruz
                {
                    std::string num = "";
                    int controller = 1;                     // değişkenikarşılığı ile değiştireceğimiz için (replace) kaç karakter olduğunu hesaplıyoruz. $ bu bir _ bu da bir ve bunların arasındaki sayının kaç harften oluştuğunun toplamı
                    

                    if (myLine[j + controller] == ':') {
                        if (myLine[j + controller + 1] == 's' && myLine[j + controller + 2] == 't' && myLine[j + controller + 3] == 'r') { // if string
                            controller += 4;
                            while (myLine[j + controller] != '_') {
                                num += myLine[j + controller];
                                controller++;
                            }
                            controller++;
                            myLine.replace(j, controller, stringVec[std::stoi(num)]); // r bizim kaçıncı elemandan replace etmeye başlayacağımızı gösterir. controller kaç eleman yerine bunu ekleyeceğimiz gösterir yani 1 eleman replace etsek dahi buraya yazacağımız int sayı kadar eleman silinir, sonuncusu ise replace edeceğimiz string vectorunden alacağımız string değeri gösterir. burada "2" şu şekilde bir stringi inte çevirip yani 2 ye çavirip sonra vectorde karşılığını bulduk.      
                        }
                        else if (myLine[j + controller + 1] == 'i' && myLine[j + controller + 2] == 'n' && myLine[j + controller + 3] == 't') { // if int
                            controller += 4;
                            while (myLine[j + controller] != '_') {
                                num += myLine[j + controller];
                                controller++;
                            }
                            controller++;
                            myLine.replace(j, controller, std::to_string(intVec[std::stoi(num)])); // r bizim kaçıncı elemandan replace etmeye başlayacağımızı gösterir. controller kaç eleman yerine bunu ekleyeceğimiz gösterir yani 1 eleman replace etsek dahi buraya yazacağımız int sayı kadar eleman silinir, sonuncusu ise replace edeceğimiz int vectorundan alacağımız string değeri gösterir..      
                        }
                        else {
                            std::cout << "\nERROR:\nmessage: need :str or :int type of variable when you call it.\n";
                            exit(0);
                        }
                    }
                    else {
                        std::cout << "\nERROR:\nmessage: need ':' to call variable.\n";
                        exit(0);
                    }
                }
            } //->değişkenlerin değerlerini metinde yerleştiriyoruz-----

            if (myLine[1] == '<') // 2 tane yazdı mı kontrol ediyoruz sonuna \n eklemek için
            {
                myLine.erase(myLine.begin()); // ilk harfi siliyoruz yani < bunu
                myLine.erase(myLine.begin()); // ikinci harfi siliyoruz yani < bunu. begin() ilk harfi belirtiyor o yüzden ilk silişimizde ikinci harf birinci harf oldu bu nedennle +1 yapmıyoruz.

                std::cout << myLine + "\n"; // satırı + \n yazıyoruz.
            }
            else
            {
                myLine.erase(myLine.begin()); // ikinci harfi siliyoruz yani < bunu. begin() ilk harfi belirtiyor o yüzden ilk silişimizde ikinci harf birinci harf oldu bu nedennle +1 yapmıyoruz.
                std::cout << myLine;          // satırı yazıyoruz
            }
        }
        else if (lines[i][0] == '>') // input aldırmak için
        {
            if (lines[i][1] != '$' || lines[i][lines[i].size() - 1] != '_') {
                std::cout << "\nERROR:\nmessage: > command need a variable.\n";
                break;
            }

            std::string in = "";
            std::getline(std::cin, in); // in değerine inputu atıyoruz.
            std::string defnumCopy = "";

            bool isStr = false;

            if (lines[i][2] == ':') {
                if (lines[i][3] == 's' && lines[i][4] == 't' && lines[i][5] == 'r') {
                    isStr = true;
                }
                else if (lines[i][3] == 'i' && lines[i][4] == 'n' && lines[i][5] == 't') {
                    isStr = false;
                }
                else {
                    std::cout << "\nERROR:\nmessage: need :str or :int type of variable when you call it.\n";
                    break;
                }
            }
            else {
                std::cout << "\nERROR:\nmessage: need ':' to call variable.\n";
                break;
            }

            // $ içinde verilen indexe ulaşma.
            for (int j = 6; j < lines[i].size() - 1; j++)
            {
                defnumCopy += lines[i][j];
            }
            
            if (isStr) {
                stringVec[std::stoi(defnumCopy)] = in;
            }
            else {
                intVec[std::stoi(defnumCopy)] = std::stoi(in);
            }
        }
        else if (lines[i][0] == 'D' && lines[i][1] == 'E' && lines[i][2] == 'F' && lines[i][3] == ':') // değişken tanımlamak için DEF isim gibi bir şey yazabilmek için
        {
            std::string defined = "";

            if (lines[i][4] == 's' && lines[i][5] == 't' && lines[i][6] == 'r' && lines[i][7] == ' ') {
                for (int j = 8; j < lines[i].size(); j++) // 8 den başlıyoruz çünkü 'DEF:str ' [8]
                {
                    defined += lines[i][j]; // karşılaştığımız harfi defined içine ekliyoruz.
                }
                stringVec.push_back(defined); // vectore kaydediyoruz.
            }
            else if (lines[i][4] == 'i' && lines[i][5] == 'n' && lines[i][6] == 't' && lines[i][7] == ' ') {            
                for (int j = 8; j < lines[i].size(); j++) // 8 den başlıyoruz çünkü 'DEF:int ' [8]
                {
                    defined += lines[i][j]; // karşılaştığımız harfi defined içine ekliyoruz.
                }
                intVec.push_back(std::stoi(defined)); // vectore kaydediyoruz.
            }
            else {
                std::cout << "\nERROR:\nmessage: need :str or :int type of variable when you define it.\n";
                break;
            }            
        }
        else if (lines[i][0] == 'M')
        {
            std::string defCopy = "";
            std::string secondCopy = "";
            bool changeSaveDeftoSecondCopy = false;

            bool firstIsStr = false;
            bool secondIsStr = false;

            bool firstCalculated = false;
            bool secondCalculated = false;


            if (lines[i][3] != '$') {
                std::cout << "\nERROR:\nmessage: need a variable when using M command.\n";
                exit(0);
            }

            // $ içinde verilen indexe ulaşma.
            for (int j = 4; j < lines[i].size(); j++)
            {
                if (lines[i][j] == '_')
                {
                    if (changeSaveDeftoSecondCopy)
                    {
                        break;
                    }
                    else
                    {
                        changeSaveDeftoSecondCopy = true;
                        if (lines[i][j + 1] == ' ' && lines[i][j + 2] == '$') {
                            j += 2;
                        }
                        else {
                            std::cout << "\nERROR:\nmessage: need a variable when using M command.\n";
                            exit(0);
                        }
                    }
                }
                else
                {
                    if (changeSaveDeftoSecondCopy)
                    {
                        if (!secondCalculated) {
                            if (lines[i][j] == ':') {
                                if (lines[i][j + 1] == 's' && lines[i][j + 2] == 't' && lines[i][j + 3] == 'r') {
                                    secondIsStr = true;
                                    secondCalculated = true;
                                    j += 3; // for başında j artacağı için 3 yaptık yoksa 4 yapmamız gerirdi. (:str) 4 char.
                                }
                                else if (lines[i][j + 1] == 'i' && lines[i][j + 2] == 'n' && lines[i][j + 3] == 't') {
                                    secondIsStr = false;
                                    secondCalculated = true;
                                    j += 3; // for başında j artacağı için 3 yaptık yoksa 4 yapmamız gerirdi. (:int) 4 char.
                                }
                                else {
                                    std::cout << "\nERROR:\nmessage: need :str or :int type of variable when you using it.\n";
                                    exit(0);
                                }
                            }
                            else {
                                std::cout << "\nERROR:\nmessage: need : for type when you using a variable.\n";
                                exit(0);
                            }
                        }
                        else {
                            secondCopy += lines[i][j];
                        }

                    }
                    else
                    {
                        if (!firstCalculated) {
                            if (lines[i][j] == ':') {
                                if (lines[i][j + 1] == 's' && lines[i][j + 2] == 't' && lines[i][j + 3] == 'r') {
                                    firstIsStr = true;
                                    firstCalculated = true;
                                    j += 3; // for başında j artacağı için 3 yaptık yoksa 4 yapmamız gerirdi. (:str) 4 char.
                                }
                                else if (lines[i][j + 1] == 'i' && lines[i][j + 2] == 'n' && lines[i][j + 3] == 't') {
                                    firstIsStr = false;
                                    firstCalculated = true;
                                    j += 3; // for başında j artacağı için 3 yaptık yoksa 4 yapmamız gerirdi. (:int) 4 char.
                                }
                                else {
                                    std::cout << "\nERROR:\nmessage: need :str or :int type of variable when you using it.\n";
                                    exit(0);
                                }
                            }
                            else {
                                std::cout << "\nERROR:\nmessage: need : for type when you using a variable.\n";
                                exit(0);
                            }
                        }
                        else {
                            defCopy += lines[i][j];
                        }
                    }
                }
            }


            if (lines[i][1] == '=')
            {
                // 1. ikisi de str ise
                // 2. ikisi de int ise
                // 3. ilki str ikincisi int ise
                // 4. ilki int ikincisi str ise
                if (firstIsStr && secondIsStr) stringVec[std::stoi(defCopy)] = stringVec[std::stoi(secondCopy)];
                else if (!firstIsStr && !secondIsStr) intVec[std::stoi(defCopy)] = intVec[std::stoi(secondCopy)];
                else if (firstIsStr && !secondIsStr) stringVec[std::stoi(defCopy)] = std::to_string(intVec[std::stoi(secondCopy)]);
                else if (!firstIsStr && secondIsStr) intVec[std::stoi(defCopy)] = std::stoi(stringVec[std::stoi(secondCopy)]);
                else {
                    std::cout << "\nERROR:\nmessage: when using M= command you have to using str=str or int=int or str=int or int=str.\n";
                    break;
                }
            }
            else if (lines[i][1] == '+')
            {
                if (firstIsStr && secondIsStr) stringVec[std::stoi(defCopy)] += stringVec[std::stoi(secondCopy)];
                else if (!firstIsStr && !secondIsStr) intVec[std::stoi(defCopy)] += intVec[std::stoi(secondCopy)];
                else if (firstIsStr && !secondIsStr) stringVec[std::stoi(defCopy)] += std::to_string(intVec[std::stoi(secondCopy)]);
                else if (!firstIsStr && secondIsStr) intVec[std::stoi(defCopy)] += std::stoi(stringVec[std::stoi(secondCopy)]);
                else {
                    std::cout << "\nERROR:\nmessage: when using M+ command you have to using str+str or int+int or str+int or int+str.\n";
                    break;
                }
            }
            else if (lines[i][1] == '-')
            {
                if (!firstIsStr && !secondIsStr) intVec[std::stoi(defCopy)] -= intVec[std::stoi(secondCopy)];
                else {
                    std::cout << "\nERROR:\nmessage: when using M- command you have to using int+int.\n";
                    break;
                }
            }
            else if (lines[i][1] == '*')
            {
                if (!firstIsStr && !secondIsStr) intVec[std::stoi(defCopy)] *= intVec[std::stoi(secondCopy)];
                else {
                    std::cout << "\nERROR:\nmessage: when using M* command you have to using int+int.\n";
                    break;
                }
            }
            else if (lines[i][1] == '/')
            {
                if (!firstIsStr && !secondIsStr) intVec[std::stoi(defCopy)] /= intVec[std::stoi(secondCopy)];
                else {
                    std::cout << "\nERROR:\nmessage: when using M/ command you have to using int+int.\n";
                    break;
                }
            }
            else if (lines[i][1] == '%')
            {
                if (!firstIsStr && !secondIsStr) intVec[std::stoi(defCopy)] %= intVec[std::stoi(secondCopy)];
                else {
                    std::cout << "\nERROR:\nmessage: when using M% command you have to using int+int.\n";
                    break;
                }
            }
            else
            {
                std::cout << "\nERROR:\nmessage: false operator.\n";
                break;
            }
        }
        
        else if (lines[i][0] == 'G' && lines[i][1] == 'O' && lines[i][2] == 'T' && lines[i][3] == 'O' && lines[i][4] == ' ') // go to line
        {
            std::string defCopy = "";
            if (lines[i][5] == '$' && lines[i][6] == ':' && lines[i][7] == 'i' && lines[i][8] == 'n' && lines[i][9] == 't' && lines[i][lines[i].size() - 1] == '_') // değişken verilirse onun değeri sayı verilirse direkt sayı
            {
                for (int j = 10; j < lines[i].size() - 1; j++)
                {
                    defCopy += lines[i][j];
                }
                defCopy = std::to_string(intVec[std::stoi(defCopy)]);
                if (std::stoi(defCopy) % 1 != 0) { //tam sayı mı kontrol
                    std::cout << "\nERROR:\nmessage: GOTO command need line number as integer.\n";
                    break;
                }
            }
            else if (std::stoi(std::to_string(lines[i][5])) % 1 == 0) { //tam sayı mı kontrol
                for (int j = 5; j < lines[i].size(); j++)
                {
                    defCopy += lines[i][j];
                }
            }
            else
            {
                std::cout << "\nERROR:\nmessage: You have to using integer number or as variable while using GOTO.\n";
                break;
            }

            int integerNumberOfLineNumber = std::stoi(defCopy);
            if (integerNumberOfLineNumber < 1 || integerNumberOfLineNumber > lines.size()) {
                std::cout << "\nERROR:\nmessage: Line cannot found. It can be too big or too small.\n";
                break;    
            }

            i = integerNumberOfLineNumber - 2; //go to i. satır
            continue;
        }
        else if (lines[i][0] == 'E' && lines[i][1] == 'X' && lines[i][2] == 'I' && lines[i][3] == 'T' && lines[i].size() == 4) // stop program
        {
            exit(0);
        }
        
        // under devolopment
        else
        {
            std::cout << "ERROR!\nmessage: wrong command.\n";
            break;
        }
    }
}