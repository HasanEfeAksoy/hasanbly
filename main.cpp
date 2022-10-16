#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <algorithm>



void run(std::string& text, std::vector<std::string>& lines, std::vector<bool>& unInterpreteLines, std::vector<std::string>& stringVec, std::vector<int>& intVec, std::vector<double>& doubleVec);
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

        std::vector<bool> unInterpreteLines; // comment and empty lines (if interpretable false, if uninterpretable true)
        std::vector<std::string> stringVec; // değişkenleri tanımladığımız zaman depolanacak vector
        std::vector<int> intVec; // int vectoru
        std::vector<double> doubleVec; // double vectoru
        
        run(text, lines, unInterpreteLines, stringVec, intVec, doubleVec);
    }
    else
    {
        std::cout << "\nERROR:\nmessage: wrong input file type.\n";
        return 0;
    }
}
void run(std::string& text, std::vector<std::string>& lines, std::vector<bool>& unInterpreteLines, std::vector<std::string>& stringVec, std::vector<int>& intVec, std::vector<double>& doubleVec)
{
    // unInterprete vectorune başlangıç değer ataması
    for (int i = 0; i < lines.size(); i++) {
        unInterpreteLines.push_back(false);
    }


    // yorum satırlarını ve boş satırları vectore atama
    // sol boşluk silme
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < lines[i].size(); j++) {
            if (lines[i][j] == ' ' || lines[i][j] == '\t') // her satırın her harfini 2 for ile dolaşıp başlarında boşluk olmayana kadar yani her satırın sol tarafındaki boşlukları siliyoruz.
            {
                lines[i].erase(lines[i].begin()); // begin() e +j yapmaya gerek yok çünkü spesifik eleman silmiyoruz. hep baştaki boşluk harfini siliyoruz.
                j--;
            }
            else {
                break;
            }
        }


        if (lines[i].size() == 0 || (lines[i][0] == '/' && lines[i][1] == '/')) {
            unInterpreteLines[i] = true;
            lines[i] = "";
        }
    }


    // sağ boşluk silme
    for (int i = 0; i < lines.size(); i++) {
        if (unInterpreteLines[i]) continue;
        

        for (int j = 1; j < lines.size(); j++) {
            if (lines[i][lines[i].size() - j] == ' ' || lines[i][lines[i].size() - j] == '\t') {
                lines[i].pop_back();
                j--;
            }
            else {
                break;
            }
        }
    }
    
    
    // ; ile bitmeyen satır varsa error verme
    for (int i = 0; i < lines.size(); i++) {
        if (unInterpreteLines[i]) { // uninterprete line a gelindiğinde görmezden gel ve geç
            continue;
        }
        
        if (lines[i][lines[i].size() - 1] != ';') // satırın ; ile bitip bitmediğini kontrol ediyoruz.
        {
            std::cout << "\nERROR:\nmessage: missing semicolon. line:" << std::to_string(i + 1) << "\n";
            exit(0);
        }
        else {
            lines[i].pop_back(); // sondaki noktalı virgülü siliyoruz.
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
                        else if (myLine[j + controller + 1] == 'd' && myLine[j + controller + 2] == 'b' && myLine[j + controller + 3] == 'l') { // if double
                            controller += 4;
                            while (myLine[j + controller] != '_') {
                                num += myLine[j + controller];
                                controller++;
                            }
                            controller++;
                            myLine.replace(j, controller, std::to_string(doubleVec[std::stod(num)])); // r bizim kaçıncı elemandan replace etmeye başlayacağımızı gösterir. controller kaç eleman yerine bunu ekleyeceğimiz gösterir yani 1 eleman replace etsek dahi buraya yazacağımız int sayı kadar eleman silinir, sonuncusu ise replace edeceğimiz int vectorundan alacağımız string değeri gösterir..      
                        }
                        else {
                            std::cout << "\nERROR:\nmessage: need :str or :int or :dbl type of variable when you call it. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                    }
                    else {
                        std::cout << "\nERROR:\nmessage: need ':' to call variable. line:" << std::to_string(i + 1) << "\n";
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
                std::cout << "\nERROR:\nmessage: > command need a variable. line:" << std::to_string(i + 1) << "\n";
                exit(0);
            }

            std::string in = "";
            std::getline(std::cin, in); // in değerine inputu atıyoruz.
            std::string defnumCopy = "";

            int type = 0;

            if (lines[i][2] == ':') {
                if (lines[i][3] == 's' && lines[i][4] == 't' && lines[i][5] == 'r') {
                    type = 0;
                }
                else if (lines[i][3] == 'i' && lines[i][4] == 'n' && lines[i][5] == 't') {
                    type = 1;
                }
                else if (lines[i][3] == 'd' && lines[i][4] == 'b' && lines[i][5] == 'l') {
                    type = 2;
                }
                else {
                    std::cout << "\nERROR:\nmessage: need :str or :int or :dbl type of variable when you call it. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
                }
            }
            else {
                std::cout << "\nERROR:\nmessage: need ':' to call variable. line:" << std::to_string(i + 1) << "\n";
                exit(0);
            }

            // $ içinde verilen indexe ulaşma.
            for (int j = 6; j < lines[i].size() - 1; j++)
            {
                defnumCopy += lines[i][j];
            }
            
            if (type == 0) {
                stringVec[std::stoi(defnumCopy)] = in;
            }
            else if (type == 1) {
                intVec[std::stoi(defnumCopy)] = std::stoi(in);
            }
            else if (type == 2) {
                doubleVec[std::stoi(defnumCopy)] = std::stod(in);
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
            else if (lines[i][4] == 'd' && lines[i][5] == 'b' && lines[i][6] == 'l' && lines[i][7] == ' ') {            
                for (int j = 8; j < lines[i].size(); j++) // 8 den başlıyoruz çünkü 'DEF:dbl ' [8]
                {
                    defined += lines[i][j]; // karşılaştığımız harfi defined içine ekliyoruz.
                }
                doubleVec.push_back(std::stod(defined)); // vectore kaydediyoruz.
            }
            else {
                std::cout << "\nERROR:\nmessage: need :str or :int or :dbl type of variable when you define it. line:" << std::to_string(i + 1) << "\n";
                exit(0);
            }            
        }
        else if (lines[i][0] == 'M')
        {
            std::string defCopy = "";
            std::string secondCopy = "";
            bool changeSaveDeftoSecondCopy = false;

            int typeFirst = 0;
            int typeSecond = 0;

            bool firstCalculated = false;
            bool secondCalculated = false;


            if (lines[i][3] != '$') {
                std::cout << "\nERROR:\nmessage: need a variable when using M command. line:" << std::to_string(i + 1) << "\n";
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
                            std::cout << "\nERROR:\nmessage: need a variable when using M command. line:" << std::to_string(i + 1) << "\n";
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
                                    typeSecond = 0;
                                    secondCalculated = true;
                                    j += 3; // for başında j artacağı için 3 yaptık yoksa 4 yapmamız gerirdi. (:str) 4 char.
                                }
                                else if (lines[i][j + 1] == 'i' && lines[i][j + 2] == 'n' && lines[i][j + 3] == 't') {
                                    typeSecond = 1;
                                    secondCalculated = true;
                                    j += 3; // for başında j artacağı için 3 yaptık yoksa 4 yapmamız gerirdi. (:int) 4 char.
                                }
                                else if (lines[i][j + 1] == 'd' && lines[i][j + 2] == 'b' && lines[i][j + 3] == 'l') {
                                    typeSecond = 2;
                                    secondCalculated = true;
                                    j += 3; // for başında j artacağı için 3 yaptık yoksa 4 yapmamız gerirdi. (:dbl) 4 char.
                                }
                                else {
                                    std::cout << "\nERROR:\nmessage: need :str or :int or :dbl type of variable when you using it. line:" << std::to_string(i + 1) << "\n";
                                    exit(0);
                                }
                            }
                            else {
                                std::cout << "\nERROR:\nmessage: need : for type when you using a variable. line:" << std::to_string(i + 1) << "\n";
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
                                    typeFirst = 0;
                                    firstCalculated = true;
                                    j += 3; // for başında j artacağı için 3 yaptık yoksa 4 yapmamız gerirdi. (:str) 4 char.
                                }
                                else if (lines[i][j + 1] == 'i' && lines[i][j + 2] == 'n' && lines[i][j + 3] == 't') {
                                    typeFirst = 1;
                                    firstCalculated = true;
                                    j += 3; // for başında j artacağı için 3 yaptık yoksa 4 yapmamız gerirdi. (:int) 4 char.
                                }
                                else if (lines[i][j + 1] == 'd' && lines[i][j + 2] == 'b' && lines[i][j + 3] == 'l') {
                                    typeFirst = 2;
                                    firstCalculated = true;
                                    j += 3; // for başında j artacağı için 3 yaptık yoksa 4 yapmamız gerirdi. (:dbl) 4 char.
                                }
                                else {
                                    std::cout << "\nERROR:\nmessage: need :str or :int or :dbl type of variable when you using it. line:" << std::to_string(i + 1) << "\n";
                                    exit(0);
                                }
                            }
                            else {
                                std::cout << "\nERROR:\nmessage: need : for type when you using a variable. line:" << std::to_string(i + 1) << "\n";
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
                // 1. str str
                // 2. str int
                // 3. str dbl
                // 4. int str
                // 5. int int
                // 6. int dbl
                // 7. dbl str
                // 8. dbl int
                // 9. dbl dbl
                if (typeFirst == 0 && typeSecond == 0) stringVec[std::stoi(defCopy)] = stringVec[std::stoi(secondCopy)];
                else if (typeFirst == 0 && typeSecond == 1) stringVec[std::stoi(defCopy)] = std::to_string(intVec[std::stoi(secondCopy)]);
                else if (typeFirst == 0 && typeSecond == 2) stringVec[std::stoi(defCopy)] = std::to_string(doubleVec[std::stoi(secondCopy)]);
                else if (typeFirst == 1 && typeSecond == 0) intVec[std::stoi(defCopy)] = std::stoi(stringVec[std::stoi(secondCopy)]);
                else if (typeFirst == 1 && typeSecond == 1) intVec[std::stoi(defCopy)] = intVec[std::stoi(secondCopy)];
                else if (typeFirst == 1 && typeSecond == 2) intVec[std::stoi(defCopy)] = static_cast<int>(doubleVec[std::stoi(secondCopy)]);
                else if (typeFirst == 2 && typeSecond == 0) doubleVec[std::stoi(defCopy)] = std::stod(stringVec[std::stoi(secondCopy)]);
                else if (typeFirst == 2 && typeSecond == 1) doubleVec[std::stoi(defCopy)] = 0.0 + intVec[std::stoi(secondCopy)];
                else if (typeFirst == 2 && typeSecond == 2) doubleVec[std::stoi(defCopy)] = doubleVec[std::stoi(secondCopy)];
                else {
                    std::cout << "\nERROR:\nmessage: when using M= command you have to using perfect variable types. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
                }
            }
            else if (lines[i][1] == '+')
            {
                // 1. str str
                // 2. str int
                // 3. str dbl
                // 4. int str
                // 5. int int
                // 6. int dbl
                // 7. dbl str
                // 8. dbl int
                // 9. dbl dbl
                if (typeFirst == 0 && typeSecond == 0) stringVec[std::stoi(defCopy)] += stringVec[std::stoi(secondCopy)];
                else if (typeFirst == 0 && typeSecond == 1) stringVec[std::stoi(defCopy)] += std::to_string(intVec[std::stoi(secondCopy)]);
                else if (typeFirst == 0 && typeSecond == 2) stringVec[std::stoi(defCopy)] += std::to_string(doubleVec[std::stoi(secondCopy)]);
                else if (typeFirst == 1 && typeSecond == 0) intVec[std::stoi(defCopy)] += std::stoi(stringVec[std::stoi(secondCopy)]);
                else if (typeFirst == 1 && typeSecond == 1) intVec[std::stoi(defCopy)] += intVec[std::stoi(secondCopy)];
                else if (typeFirst == 1 && typeSecond == 2) intVec[std::stoi(defCopy)] += static_cast<int>(doubleVec[std::stoi(secondCopy)]);
                else if (typeFirst == 2 && typeSecond == 0) doubleVec[std::stoi(defCopy)] += std::stod(stringVec[std::stoi(secondCopy)]);
                else if (typeFirst == 2 && typeSecond == 1) doubleVec[std::stoi(defCopy)] += 0.0 + intVec[std::stoi(secondCopy)];
                else if (typeFirst == 2 && typeSecond == 2) doubleVec[std::stoi(defCopy)] += doubleVec[std::stoi(secondCopy)];
                else {
                    std::cout << "\nERROR:\nmessage: when using M+ command you have to using perfect variable types. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
                }
            }
            else if (lines[i][1] == '-')
            {
                // 1. int int
                // 2. int dbl
                // 3. dbl int
                // 4. dbl dbl
                if (typeFirst == 1 && typeSecond == 1) intVec[std::stoi(defCopy)] -= intVec[std::stoi(secondCopy)];
                else if (typeFirst == 1 && typeSecond == 2) intVec[std::stoi(defCopy)] -= static_cast<int>(doubleVec[std::stoi(secondCopy)]);
                else if (typeFirst == 2 && typeSecond == 1) doubleVec[std::stoi(defCopy)] -= 0.0 + intVec[std::stoi(secondCopy)];
                else if (typeFirst == 2 && typeSecond == 2) doubleVec[std::stoi(defCopy)] -= doubleVec[std::stoi(secondCopy)];
                else {
                    std::cout << "\nERROR:\nmessage: when using M- command you have to using perfect variable types. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
                }

            }
            else if (lines[i][1] == '*')
            {
                // 1. int int
                // 2. int dbl
                // 3. dbl int
                // 4. dbl dbl
                if (typeFirst == 1 && typeSecond == 1) intVec[std::stoi(defCopy)] *= intVec[std::stoi(secondCopy)];
                else if (typeFirst == 1 && typeSecond == 2) intVec[std::stoi(defCopy)] *= static_cast<int>(doubleVec[std::stoi(secondCopy)]);
                else if (typeFirst == 2 && typeSecond == 1) doubleVec[std::stoi(defCopy)] *= 0.0 + intVec[std::stoi(secondCopy)];
                else if (typeFirst == 2 && typeSecond == 2) doubleVec[std::stoi(defCopy)] *= doubleVec[std::stoi(secondCopy)];
                else {
                    std::cout << "\nERROR:\nmessage: when using M* command you have to using perfect variable types. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
                }
            }
            else if (lines[i][1] == '/')
            {
                // 1. int int
                // 2. int dbl
                // 3. dbl int
                // 4. dbl dbl
                if (typeFirst == 1 && typeSecond == 1) intVec[std::stoi(defCopy)] /= intVec[std::stoi(secondCopy)];
                else if (typeFirst == 1 && typeSecond == 2) intVec[std::stoi(defCopy)] /= static_cast<int>(doubleVec[std::stoi(secondCopy)]);
                else if (typeFirst == 2 && typeSecond == 1) doubleVec[std::stoi(defCopy)] /= 0.0 + intVec[std::stoi(secondCopy)];
                else if (typeFirst == 2 && typeSecond == 2) doubleVec[std::stoi(defCopy)] /= doubleVec[std::stoi(secondCopy)];
                else {
                    std::cout << "\nERROR:\nmessage: when using M/ command you have to using perfect variable types. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
                }
            }
            else if (lines[i][1] == '%')
            {
                // 1. int int
                // 2. int dbl
                if (typeFirst == 1 && typeSecond == 1) intVec[std::stoi(defCopy)] %= intVec[std::stoi(secondCopy)];
                else if (typeFirst == 1 && typeSecond == 2) intVec[std::stoi(defCopy)] %= static_cast<int>(doubleVec[std::stoi(secondCopy)]);
                else {
                    std::cout << "\nERROR:\nmessage: when using M% command you have to using perfect variable types. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
                }
            }
            else
            {
                std::cout << "\nERROR:\nmessage: false operator. need + - * / %. line:" << std::to_string(i + 1) << "\n";
                exit(0);
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
                    std::cout << "\nERROR:\nmessage: GOTO command need line number as integer. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
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
                std::cout << "\nERROR:\nmessage: You have to using integer number or as variable while using GOTO. line:" << std::to_string(i + 1) << "\n";
                exit(0);
            }

            int integerNumberOfLineNumber = std::stoi(defCopy);
            if (integerNumberOfLineNumber < 1 || integerNumberOfLineNumber > lines.size()) {
                std::cout << "\nERROR:\nmessage: Line cannot found. It can be too big or too small. line:" << std::to_string(i + 1) << "\n";
                exit(0);    
            }

            i = integerNumberOfLineNumber - 2; //go to i. satır
            continue;
        }
        else if (lines[i][0] == 'E' && lines[i][1] == 'X' && lines[i][2] == 'I' && lines[i][3] == 'T' && lines[i].size() == 4) // stop program
        {
            exit(0);
        }


        else if (lines[i][0] == 'I' && lines[i][1] == 'F') // if condition
        {
            std::string defFirst = ""; // first state
            std::string defSecond = ""; // second state
            std::string defThird = ""; // third state
            std::string elseLineNumber = ""; // else go to <number>

            //bool isFirstString = false;
            //bool isSecondString = false;

            int typeFirst = 0;
            int typeSecond = 0;
            int typeThird = 0;

            bool canPass = false;


            // string
            if (lines[i][5] == '$' && lines[i][6] == ':' && lines[i][7] == 's' && lines[i][8] == 't' && lines[i][9] == 'r') {
                int whereFirstEnded = 0;
                int whereSecondEnded = 0;
                
                for (int j = 10; j < lines[i].size(); j++) {
                    if (lines[i][j] != '_') {
                        defFirst += lines[i][j];
                    }
                    else {
                        whereFirstEnded = j + 1;
                        break;
                    }
                }

                // have to second variable is string
                if (lines[i][whereFirstEnded] == ' ' && lines[i][whereFirstEnded + 1] == '$' && lines[i][whereFirstEnded + 2] == ':' && lines[i][whereFirstEnded + 3] == 's' && lines[i][whereFirstEnded + 4] == 't' && lines[i][whereFirstEnded + 5] == 'r') {
                    
                    
                    for (int j = whereFirstEnded + 6; j < lines[i].size(); j++) {
                        if (lines[i][j] != '_') {
                            defSecond += lines[i][j];
                        }
                        else {
                            whereSecondEnded = j + 1;
                            break;
                        }
                    }

                    if (lines[i][whereSecondEnded] == ' ' && lines[i][whereSecondEnded + 1] == 'E' && lines[i][whereSecondEnded + 2] == 'L' && lines[i][whereSecondEnded + 3] == 'S' && lines[i][whereSecondEnded + 4] == 'E' && lines[i][whereSecondEnded + 5] == ':') {
                        for (int j = whereSecondEnded + 6; j < lines[i].size(); j++) {
                            elseLineNumber += lines[i][j];
                        }

                        typeFirst = 0;
                        typeSecond = 0;
                    }
                    else {
                        std::cout << "\nERROR:\nmessage: you have to using ELSE:<pure integer> end of the IF condition. line:" << std::to_string(i + 1) << "\n";
                        exit(0);
                    }
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using string-string or int-int or double-double in IF condition. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
                }
            }
            // int
            else if (lines[i][5] == '$' && lines[i][6] == ':' && lines[i][7] == 'i' && lines[i][8] == 'n' && lines[i][9] == 't') {
                int whereFirstEnded = 0;
                int whereSecondEnded = 0;
                
                for (int j = 10; j < lines[i].size(); j++) {
                    if (lines[i][j] != '_') {
                        defFirst += lines[i][j];
                    }
                    else {
                        whereFirstEnded = j + 1;
                        break;
                    }
                }
                
                if (lines[i][whereFirstEnded] == ' ' && lines[i][whereFirstEnded + 1] == '$' && lines[i][whereFirstEnded + 2] == ':') {
                    if (lines[i][whereFirstEnded + 3] == 'i' && lines[i][whereFirstEnded + 4] == 'n' && lines[i][whereFirstEnded + 5] == 't') {
                        for (int j = whereFirstEnded + 6; j < lines[i].size(); j++) {
                            if (lines[i][j] != '_') {
                                defSecond += lines[i][j];
                            }
                            else {
                                whereSecondEnded = j + 1;
                                break;
                            }
                        }

                        typeFirst = 1;
                        typeSecond = 1;
                    }
                    else if (lines[i][whereFirstEnded + 3] == 'd' && lines[i][whereFirstEnded + 4] == 'b' && lines[i][whereFirstEnded + 5] == 'l') {
                        for (int j = whereFirstEnded + 6; j < lines[i].size(); j++) {
                            if (lines[i][j] != '_') {
                                defSecond += lines[i][j];
                            }
                            else {
                                whereSecondEnded = j + 1;
                                break;
                            }
                        }

                        typeFirst = 1;
                        typeSecond = 2;
                    }

                    if (lines[i][whereSecondEnded] == ' ' && lines[i][whereSecondEnded + 1] == 'E' && lines[i][whereSecondEnded + 2] == 'L' && lines[i][whereSecondEnded + 3] == 'S' && lines[i][whereSecondEnded + 4] == 'E' && lines[i][whereSecondEnded + 5] == ':') {
                        for (int j = whereSecondEnded + 6; j < lines[i].size(); j++) {
                            elseLineNumber += lines[i][j];
                        }
                    }
                    else {
                        std::cout << "\nERROR:\nmessage: you have to using ELSE:<pure integer> end of the IF condition. line:" << std::to_string(i + 1) << "\n";
                        exit(0);
                    }
                }
            }
            // double
            else if (lines[i][5] == '$' && lines[i][6] == ':' && lines[i][7] == 'd' && lines[i][8] == 'b' && lines[i][9] == 'l') {
                int whereFirstEnded = 0;
                int whereSecondEnded = 0;
                
                for (int j = 10; j < lines[i].size(); j++) {
                    if (lines[i][j] != '_') {
                        defFirst += lines[i][j];
                    }
                    else {
                        whereFirstEnded = j + 1;
                        break;
                    }
                }



                if (lines[i][whereFirstEnded] == ' ' && lines[i][whereFirstEnded + 1] == '$' && lines[i][whereFirstEnded + 2] == ':') {
                    if (lines[i][whereFirstEnded + 3] == 'd' && lines[i][whereFirstEnded + 4] == 'b' && lines[i][whereFirstEnded + 5] == 'l') {
                        for (int j = whereFirstEnded + 6; j < lines[i].size(); j++) {
                            if (lines[i][j] != '_') {
                                defSecond += lines[i][j];
                            }
                            else {
                                whereSecondEnded = j + 1;
                                break;
                            }
                        }

                        typeFirst = 2;
                        typeSecond = 2;
                    }
                    else if (lines[i][whereFirstEnded + 3] == 'i' && lines[i][whereFirstEnded + 4] == 'n' && lines[i][whereFirstEnded + 5] == 't') {
                        for (int j = whereFirstEnded + 6; j < lines[i].size(); j++) {
                            if (lines[i][j] != '_') {
                                defSecond += lines[i][j];
                            }
                            else {
                                whereSecondEnded = j + 1;
                                break;
                            }
                        }

                        typeFirst = 2;
                        typeSecond = 1;
                    }

                    if (lines[i][whereSecondEnded] == ' ' && lines[i][whereSecondEnded + 1] == 'E' && lines[i][whereSecondEnded + 2] == 'L' && lines[i][whereSecondEnded + 3] == 'S' && lines[i][whereSecondEnded + 4] == 'E' && lines[i][whereSecondEnded + 5] == ':') {
                        for (int j = whereSecondEnded + 6; j < lines[i].size(); j++) {
                            elseLineNumber += lines[i][j];
                        }
                    }
                    else {
                        std::cout << "\nERROR:\nmessage: you have to using ELSE:<pure integer> end of the IF condition. line:" << std::to_string(i + 1) << "\n";
                        exit(0);
                    }
                }
            }



            // ==
            // string-string
            // int-int
            // double-double
            if (lines[i][2] == '=' && lines[i][3] == '=' && lines[i][4] == ' ') {
                if (typeFirst == 0 && typeSecond == 0) {
                    // control
                    std::string first = stringVec[std::stoi(defFirst)];
                    std::string second = stringVec[std::stoi(defSecond)];
                    
                    if (first == second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";;
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else if (typeFirst == 1 && typeSecond == 1) {
                    // control
                    int first = intVec[std::stoi(defFirst)];
                    int second = intVec[std::stoi(defSecond)];
                    

                    if (first == second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else if (typeFirst == 2 && typeSecond == 2) {
                    // control
                    double first = doubleVec[std::stoi(defFirst)];
                    double second = doubleVec[std::stoi(defSecond)];
                    

                    if (first == second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using string-string or int-int or double-double in IF== condition. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
                }
            }
            // !=
            // string-string
            // int-int
            // double-double
            else if (lines[i][2] == '!' && lines[i][3] == '=') {
                if (typeFirst == 0 && typeSecond == 0) {
                    // control
                    std::string first = stringVec[std::stoi(defFirst)];
                    std::string second = stringVec[std::stoi(defSecond)];
                    
                    if (first != second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else if (typeFirst == 1 && typeSecond == 1) {
                    // control
                    int first = intVec[std::stoi(defFirst)];
                    int second = intVec[std::stoi(defSecond)];
                    

                    if (first != second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else if (typeFirst == 2 && typeSecond == 2) {
                    // control
                    double first = doubleVec[std::stoi(defFirst)];
                    double second = doubleVec[std::stoi(defSecond)];
                    

                    if (first != second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using string-string or int-int or double-double in IF!= condition. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
                }
            }
            // >
            // int-int
            // int-double
            // double-double
            // double-int
            else if (lines[i][2] == '>' && lines[i][3] == '>') {
                if (typeFirst == 1 && typeSecond == 1) {
                    // control
                    int first = intVec[std::stoi(defFirst)];
                    int second = intVec[std::stoi(defSecond)];
                    

                    if (first > second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 1 && typeSecond == 2) {
                    // control
                    int first = intVec[std::stoi(defFirst)];
                    double second = doubleVec[std::stoi(defSecond)];
                    

                    if (first > second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 2 && typeSecond == 2) {
                    // control
                    double first = doubleVec[std::stoi(defFirst)];
                    double second = doubleVec[std::stoi(defSecond)];
                    

                    if (first > second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 2 && typeSecond == 1) {
                    // control
                    double first = doubleVec[std::stoi(defFirst)];
                    int second = intVec[std::stoi(defSecond)];
                    

                    if (first > second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using int-int/double or double-double/int in IF>> condition. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
                }
            }
            // >=
            //int-int
            //int-double
            //double-double
            //double-int
            else if (lines[i][2] == '>' && lines[i][3] == '=') {
                if (typeFirst == 1 && typeSecond == 1) {
                    // control
                    int first = intVec[std::stoi(defFirst)];
                    int second = intVec[std::stoi(defSecond)];
                    

                    if (first >= second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 1 && typeSecond == 2) {
                    // control
                    int first = intVec[std::stoi(defFirst)];
                    double second = doubleVec[std::stoi(defSecond)];
                    

                    if (first >= second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 2 && typeSecond == 2) {
                    // control
                    double first = doubleVec[std::stoi(defFirst)];
                    double second = doubleVec[std::stoi(defSecond)];
                    

                    if (first >= second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 2 && typeSecond == 1) {
                    // control
                    double first = doubleVec[std::stoi(defFirst)];
                    int second = intVec[std::stoi(defSecond)];
                    

                    if (first >= second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using int-int/double or double-double/int in IF>= condition. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
                }
            }
            // <
            // int-int
            // int-double
            // double-double
            // double-int
            else if (lines[i][2] == '<' && lines[i][3] == '<') {
                if (typeFirst == 1 && typeSecond == 1) {
                    // control
                    int first = intVec[std::stoi(defFirst)];
                    int second = intVec[std::stoi(defSecond)];
                    

                    if (first < second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 1 && typeSecond == 2) {
                    // control
                    int first = intVec[std::stoi(defFirst)];
                    double second = doubleVec[std::stoi(defSecond)];
                    

                    if (first < second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 2 && typeSecond == 2) {
                    // control
                    double first = doubleVec[std::stoi(defFirst)];
                    double second = doubleVec[std::stoi(defSecond)];
                    

                    if (first < second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 2 && typeSecond == 1) {
                    // control
                    double first = doubleVec[std::stoi(defFirst)];
                    int second = intVec[std::stoi(defSecond)];
                    

                    if (first < second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using int-int/double or double-double/int in IF<< condition. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
                }
            }
            // <=
            // int-int
            // int-double
            // double-double
            // double-int
            else if (lines[i][2] == '<' && lines[i][3] == '=') {
                if (typeFirst == 1 && typeSecond == 1) {
                    // control
                    int first = intVec[std::stoi(defFirst)];
                    int second = intVec[std::stoi(defSecond)];
                    

                    if (first <= second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 1 && typeSecond == 2) {
                    // control
                    int first = intVec[std::stoi(defFirst)];
                    double second = doubleVec[std::stoi(defSecond)];
                    

                    if (first <= second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 2 && typeSecond == 2) {
                    // control
                    double first = doubleVec[std::stoi(defFirst)];
                    double second = doubleVec[std::stoi(defSecond)];
                    

                    if (first <= second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 2 && typeSecond == 1) {
                    // control
                    double first = doubleVec[std::stoi(defFirst)];
                    int second = intVec[std::stoi(defSecond)];
                    

                    if (first <= second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE:. line:" << std::to_string(i + 1) << "\n";
                            exit(0);
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using int-int/double or double-double/int in IF<= condition. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
                }
            }

            else {
                std::cout << "\nERROR:\nmessage: you have to using == != >> >= << <= in IF condition. line:" << std::to_string(i + 1) << "\n";
                exit(0);
            }
        }
        

        else if (lines[i][0] == 'I' && lines[i][1] == 'N' && lines[i][2] == 'C' && lines[i][3] == ' ') {
            
            std::string defCopy = "";

            if (lines[i][4] == '$' && lines[i][5] == ':' && lines[i][lines[i].size() - 1] == '_') {
                if (lines[i][6] == 'i' && lines[i][7] == 'n' && lines[i][8] == 't') {
                    for (int j = 9; j < lines[i].size() - 1; j++) {
                        defCopy += lines[i][j];
                    }
                    intVec[std::stoi(defCopy)]++;
                }
                else if (lines[i][6] == 'd' && lines[i][7] == 'b' && lines[i][8] == 'l') {
                    for (int j = 9; j < lines[i].size() - 1; j++) {
                        defCopy += lines[i][j];
                    }
                    doubleVec[std::stoi(defCopy)]++;
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using int or double variable when using INC command. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
                }
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to using variable when using INC command. line:" << std::to_string(i + 1) << "\n";
                exit(0);
            }
        }
        else if (lines[i][0] == 'D' && lines[i][1] == 'E' && lines[i][2] == 'C' && lines[i][3] == ' ') {
            
            std::string defCopy = "";

            if (lines[i][4] == '$' && lines[i][5] == ':' && lines[i][lines[i].size() - 1] == '_') {
                if (lines[i][6] == 'i' && lines[i][7] == 'n' && lines[i][8] == 't') {
                    for (int j = 9; j < lines[i].size() - 1; j++) {
                        defCopy += lines[i][j];
                    }
                    intVec[std::stoi(defCopy)]--;
                }
                else if (lines[i][6] == 'd' && lines[i][7] == 'b' && lines[i][8] == 'l') {
                    for (int j = 9; j < lines[i].size() - 1; j++) {
                        defCopy += lines[i][j];
                    }
                    doubleVec[std::stoi(defCopy)]--;
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using int or double variable when using DEC command. line:" << std::to_string(i + 1) << "\n";
                    exit(0);
                }
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to using variable when using DEC command. line:" << std::to_string(i + 1) << "\n";
                exit(0);
            }
        }


        else if (lines[i][0] == 'N' && lines[i][1] == 'U' && lines[i][2] == 'L' && lines[i][3] == 'L' && lines[i][4] == ' ') {
            
            if (lines[i][5] != '$' || lines[i][6] != ':' || lines[i][lines[i].size() - 1] != '_') {
                std::cout << "\nERROR:\nmessage: you have to using a variable when using NULL command. line:" << std::to_string(i + 1) << "\n";
                exit(0);
            }

            std::string defCopy = "";
            for (int j = 10; j < lines[i].size() - 1; j++) {
                defCopy += lines[i][j];
            }

            if (lines[i][7] == 's' && lines[i][8] == 't' && lines[i][9] == 'r')
                stringVec[std::stoi(defCopy)] = "";
            else if (lines[i][7] == 'i' && lines[i][8] == 'n' && lines[i][9] == 't')
                intVec[std::stoi(defCopy)] = 0;
            else if (lines[i][7] == 'd' && lines[i][8] == 'b' && lines[i][9] == 'l')
                doubleVec[std::stoi(defCopy)] = 0.0;
            else {
                std::cout << "\nERROR:\nmessage: you have to using int or string or double variable when using NULL command. line:" << std::to_string(i + 1) << "\n";
                exit(0);
            }
        }

        else if (lines[i][0] == 'F' && lines[i][1] == 'R' && lines[i][2] == 'E' && lines[i][3] == 'E' && lines[i][4] == ' ') {
            
            if (lines[i][5] != '$' || lines[i][6] != ':' || lines[i][lines[i].size() - 1] != '_') {
                std::cout << "\nERROR:\nmessage: you have to using a variable when using FREE command. line:" << std::to_string(i + 1) << "\n";
                exit(0);
            }

            std::string defCopy = "";
            for (int j = 10; j < lines[i].size() - 1; j++) {
                defCopy += lines[i][j];
            }

            int index = std::stoi(defCopy);

            if (lines[i][7] == 's' && lines[i][8] == 't' && lines[i][9] == 'r') {
                stringVec[index] = "";
                stringVec.erase(stringVec.begin() + index);
            }
            else if (lines[i][7] == 'i' && lines[i][8] == 'n' && lines[i][9] == 't') {
                intVec[index] = 0;
                intVec.erase(intVec.begin() + index);
            }
            else if (lines[i][7] == 'd' && lines[i][8] == 'b' && lines[i][9] == 'l') {
                doubleVec[index] = 0.0;
                doubleVec.erase(doubleVec.begin() + index);
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to using string or int or double variable when using NULL command. line:" << std::to_string(i + 1) << "\n";
                exit(0);
            }
        }



        
        // under devolopment
        else
        {
            std::cout << "ERROR!\nmessage: wrong command. line:" << std::to_string(i + 1) << "\n";
            exit(0);
        }
    }
}