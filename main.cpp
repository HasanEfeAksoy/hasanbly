/*
        HASANBLY



        AUTHOR:
    
    HASAN EFE AKSOY



        VERSION:

          1.7




        COMMANDS:

        <
        <<
        >
        DEF
        M
        GOTO
        EXIT
        IF
        INC
        DEC
        NULL
        FREE
        INDEX
        SLEEP
        RAND
        STRLEN
        CLEAR
        OSNAME
        REPLACE
        TERMINAL
        TIME
        CLAMP
        ADDESCAPE
        OPENFILE
        DELETEFILE
        READFILE
        OVERWRITEFILE
        APPENDWRITEFILE
        DEFEL
        FREEEL
        SETEL
        GETEL
*/



#include <iostream>
//#include <stdlib.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <thread>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <cstdio>


#define PI 3.141592


std::string get_os_name();
int factorial(int n);
int combination(int n, int r);
int permutation(int n, int r);
void interprete(std::string& text, std::vector<std::string>& lines, bool* unInterpreteLines, std::unordered_map<std::string, std::string>& stringVariables, std::unordered_map<std::string, int>& intVariables, std::unordered_map<std::string, double>& doubleVariables, std::string& os_name);

int main(int argc, char** argv) {
    srand(time(NULL));

    if (!argv[1]) {
        std::cout << "\nERROR:\nmessage: no input file.\n";
        return 0;
    }

    const std::string input = argv[1]; // hsnc.exe main.hsnc yazıldığı zaman main.hsnc yi alacak
    if (input.size() > 4 && input[input.size() - 4] == '.' && input[input.size() - 3] == 'h' && input[input.size() - 2] == 's' && input[input.size() - 1] == 'n') {
        std::string text = "";          // dosyanın tüm içeriği
        std::vector<std::string> lines; // dosyadaki tüm satırlar
        std::string newText = "";
        std::ifstream file(input);
        if (!file) {
            std::cout << "\nERROR:\nmessage: input file does not exist.\n";
            return 0;
        }

        while (std::getline(file, newText)) {
            text += newText + "\n";
            lines.push_back(newText);
        }
        file.close();

        //std::vector<bool> unInterpreteLines; // comment and empty lines (if interpretable false, if uninterpretable true)
        bool* unInterpreteLines = new bool[lines.size()];
        std::unordered_map<std::string, std::string> stringVariables; // string unordered_mapi
        std::unordered_map<std::string, int> intVariables; // int unordered_mapi
        std::unordered_map<std::string, double> doubleVariables; // double unordered_mapi


        // get os name
            std::string os_name = get_os_name();
        //
        
        interprete(text, lines, unInterpreteLines, stringVariables, intVariables, doubleVariables, os_name);

        delete[] unInterpreteLines;
        return 0;
    }
    else {
        std::cout << "\nERROR:\nmessage: wrong input file type.\n";
        return 0;
    }
}

std::string get_os_name() {
    std::string os_name;

    // Linux ve MacOS için /etc/os-release dosyasını kontrol ediyoruz
    std::ifstream file("/etc/os-release");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("NAME=") != std::string::npos) {
                os_name = line.substr(line.find('=') + 2);
                os_name.erase(os_name.find_last_not_of("\"") + 1); // Sonundaki çift tırnakları temizle
                break;
            }
        }
        file.close();
    }

    // Eğer Linux/MacOS tespit edemediysek Windows olarak kabul ediyoruz
    if (os_name.empty()) {
        os_name = "Windows";
    }

    return os_name;
}
int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}
int combination(int n, int r) {
    return factorial(n) / (factorial(r) * factorial(n - r));
}
int permutation(int n, int r) {
    return factorial(n) / factorial(n - r);
}


void interprete(std::string& text, std::vector<std::string>& lines, bool* unInterpreteLines, std::unordered_map<std::string, std::string>& stringVariables, std::unordered_map<std::string, int>& intVariables, std::unordered_map<std::string, double>& doubleVariables, std::string& os_name) {
    // unInterprete listine başlangıç değer ataması
    for (int i = 0; i < lines.size(); i++) {
        unInterpreteLines[i] = false;
    }


    // yorum satırlarını ve boş satırları unInterpreteLines a atama
    // sol boşluk silme
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            if (lines[i][j] == ' ' || lines[i][j] == '\t') { // her satırın her harfini 2 for ile dolaşıp başlarında boşluk olmayana kadar yani her satırın sol tarafındaki boşlukları siliyoruz.
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
        
        if (lines[i][lines[i].size() - 1] != ';') { // satırın ; ile bitip bitmediğini kontrol ediyoruz.
            std::cout << "\nERROR:\nmessage: missing semicolon. line:" << std::to_string(i + 1) << "\n";
            return;
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
            return;
        }
        else {
            break; // sonda EXIT kullanmış demek oluyor.
        }
    }


    

    /*
        MAIN FOR LOOP
    */
    for (int i = 0; i < lines.size(); i++)
    {
        // her yerde çağırmak yerine bir kere yazalım
        size_t line_i_size = lines[i].size();

     
        if (unInterpreteLines[i]) { // uninterprete olan (comment lines and empty lines) satırlar varsa geçmek
            continue;
        }


        if (lines[i][0] == '<') { // print etmek için ilk harf ne onu kontrol ediyoruz. yukarda zaten soldaki boşlukları siliğimiz için ilk harf diye kontrol edebiliriz.
            std::string myLine = lines[i];

            for (int j = 0; j < myLine.size(); j++) { // her satırın her harfini geziyoruz 2 for döngüsü ile.
                // sadece $ (dolar simgesi) yazdırmak istersek yani ^ bundan sonraki ilk karakter göz ardı edilecek ve yazılacak.
                if (myLine[j] == '^') {
                    myLine.erase(myLine.begin() + j);
                    //j++; //erase ettiğimiz için j yi arttırmaya gerek kalmadı.
                    continue;
                }
                else if (myLine[j] == '$') { //$ işareti buluyoruz
                    std::string varName = "";
                    int controller = 1;
                    

                    if (myLine[j + controller] == ':') {
                        if (myLine[j + controller + 1] == 's' && myLine[j + controller + 2] == 't' && myLine[j + controller + 3] == 'r' && myLine[j + controller + 4] == ':') { // if string
                            controller += 5;
                            while (myLine[j + controller] != '_') {
                                varName += myLine[j + controller];
                                controller++;
                            }
                            controller++;
                            std::string replaceStr = stringVariables.at(varName);
                            myLine.replace(j, controller, replaceStr); // j bizim kaçıncı elemandan replace etmeye başlayacağımızı gösterir. controller kaç eleman yerine bunu ekleyeceğimiz gösterir yani 1 eleman replace etsek dahi buraya yazacağımız int sayı kadar eleman silinir, sonuncusu ise replace edeceğimiz unordered_map ten alacağımız string değeri gösterir.
                            j += replaceStr.size() - 1;
                        }
                        else if (myLine[j + controller + 1] == 'i' && myLine[j + controller + 2] == 'n' && myLine[j + controller + 3] == 't' && myLine[j + controller + 4] == ':') { // if int
                            controller += 5;
                            while (myLine[j + controller] != '_') {
                                varName += myLine[j + controller];
                                controller++;
                            }
                            controller++;
                            std::string replaceStr = std::to_string(intVariables.at(varName));
                            myLine.replace(j, controller, replaceStr); // j bizim kaçıncı elemandan replace etmeye başlayacağımızı gösterir. controller kaç eleman yerine bunu ekleyeceğimiz gösterir yani 1 eleman replace etsek dahi buraya yazacağımız int sayı kadar eleman silinir, sonuncusu ise replace edeceğimiz unordered_map ten alacağımız int değeri gösterir.
                            j += replaceStr.size() - 1;
                        }
                        else if (myLine[j + controller + 1] == 'd' && myLine[j + controller + 2] == 'b' && myLine[j + controller + 3] == 'l' && myLine[j + controller + 4] == ':') { // if double
                            controller += 5;
                            while (myLine[j + controller] != '_') {
                                varName += myLine[j + controller];
                                controller++;
                            }
                            controller++;
                            std::string replaceStr = std::to_string(doubleVariables.at(varName));
                            myLine.replace(j, controller, replaceStr); // j bizim kaçıncı elemandan replace etmeye başlayacağımızı gösterir. controller kaç eleman yerine bunu ekleyeceğimiz gösterir yani 1 eleman replace etsek dahi buraya yazacağımız int sayı kadar eleman silinir, sonuncusu ise replace edeceğimiz unordered_map ten alacağımız double değeri gösterir.
                            j += replaceStr.size() - 1;
                        }
                        else {
                            std::cout << "\nERROR:\nmessage: need :str: or :int: or :dbl: type of variable when you call it. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                    }
                    else {
                        std::cout << "\nERROR:\nmessage: need '$:type:varName_' to call variable you may forgot ':'. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                }
            } //->değişkenlerin değerlerini metinde yerleştiriyoruz-----

            if (myLine[1] == '<') { // 2 tane yazdı mı kontrol ediyoruz sonuna \n eklemek için
                myLine.erase(myLine.begin()); // ilk harfi siliyoruz yani < bunu
                myLine.erase(myLine.begin()); // ikinci harfi siliyoruz yani < bunu. begin() ilk harfi belirtiyor o yüzden ilk silişimizde ikinci harf birinci harf oldu bu nedennle +1 yapmıyoruz.

                std::cout << myLine << '\n'; // satırı + \n yazıyoruz.
            }
            else {
                myLine.erase(myLine.begin()); // ikinci harfi siliyoruz yani < bunu. begin() ilk harfi belirtiyor o yüzden ilk silişimizde ikinci harf birinci harf oldu bu nedennle +1 yapmıyoruz.
                std::cout << myLine;          // satırı yazıyoruz
            }
        }
        else if (lines[i][0] == '>') { // input aldırmak için
            if (lines[i][1] != '$' || lines[i][line_i_size - 1] != '_') {
                std::cout << "\nERROR:\nmessage: > command need a variable. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            std::string in = "";
            std::getline(std::cin, in); // in değerine inputu atıyoruz.

            int type = 0;

            if (lines[i][2] == ':') {
                if (lines[i][3] == 's' && lines[i][4] == 't' && lines[i][5] == 'r' && lines[i][6] == ':') {
                    type = 0;
                }
                else if (lines[i][3] == 'i' && lines[i][4] == 'n' && lines[i][5] == 't' && lines[i][6] == ':') {
                    type = 1;
                }
                else if (lines[i][3] == 'd' && lines[i][4] == 'b' && lines[i][5] == 'l' && lines[i][6] == ':') {
                    type = 2;
                }
                else {
                    std::cout << "\nERROR:\nmessage: need :str: or :int: or :dbl: type of variable when you call it. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else {
                std::cout << "\nERROR:\nmessage: need '$:type:varName_' to call variable you may forgot ':'. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            std::string varName = "";

            // $ içinde verilen indexe ulaşma.
            for (int j = 7; j < line_i_size - 1; j++) {
                varName += lines[i][j];
            }
            
            if (type == 0) {
                stringVariables.at(varName) = in;
            }
            else if (type == 1) {
                intVariables.at(varName) = std::stoi(in);
            }
            else if (type == 2) {
                doubleVariables.at(varName) = std::stod(in);
            }
        }
        else if (lines[i][0] == 'D' && lines[i][1] == 'E' && lines[i][2] == 'F' && lines[i][3] == ':') { // değişken tanımlamak için DEF:int:count=12;
            std::string name = "";
            std::string value = "";

            bool isReadedName = false;

            if (lines[i][4] == 's' && lines[i][5] == 't' && lines[i][6] == 'r' && lines[i][7] == ':') {
                for (int j = 8; j < line_i_size; j++) {
                    if (isReadedName) {

                        //
                        // escape catching start
                        //
                        if (lines[i][j] == '\\') {
                            if (j + 1 < lines[i].size()) {
                                switch (lines[i][j + 1]) {
                                    case 'n': value += '\n'; break;
                                    case 't': value += '\t'; break;
                                    case 'r': value += '\r'; break;
                                    case 'v': value += '\v'; break;
                                    case '0': value += '\0'; break;
                                    case '\'': value += '\''; break;
                                    case '"': value += '"'; break;
                                    case '\\': value += '\\'; break;
                                    case '?': value += '\?'; break;
                                    case 'a': value += '\a'; break;
                                    case 'b': value += '\b'; break;
                                    case 'f': value += '\f'; break;
                                    default:
                                        std::cout << "\nERROR:\nmessage: you have to using just valid escape chars (documentation: https://github.com/HasanEfeAksoy/hasanbly/blob/main/README.md) when using DEF command. line:" << std::to_string(i + 1) << "\n";
                                        return;
                                }
                                j++;
                            } else {
                                std::cout << "\nERROR:\nmessage: you have to using just valid escape chars (documentation: https://github.com/HasanEfeAksoy/hasanbly/blob/main/README.md) when using DEF command. line:" << std::to_string(i + 1) << "\n";
                                return;
                            }
                        } else {
                            value += lines[i][j];
                        }
                        //
                        // escape catching end
                        //


                        // wihout escape catching
                        //value += lines[i][j];
                    }
                    else {
                        if (lines[i][j] != '=') {
                            if (lines[i][j] == ' ' || lines[i][j] == '_' || lines[i][j] == '_' || lines[i][j] == '\"' || lines[i][j] == '\'' || lines[i][j] == '\\' || lines[i][j] == '^' || lines[i][j] == '!' || lines[i][j] == '+' || lines[i][j] == '-' || lines[i][j] == '*' || lines[i][j] == '/' || lines[i][j] == '%' || lines[i][j] == '&' || lines[i][j] == '(' || lines[i][j] == ')' || lines[i][j] == '=' || lines[i][j] == '#' || lines[i][j] == '$' || lines[i][j] == '?' || lines[i][j] == '{' || lines[i][j] == '}' || lines[i][j] == '[' || lines[i][j] == ']' || lines[i][j] == '@' || lines[i][j] == ',' || lines[i][j] == '.' || lines[i][j] == ';' || lines[i][j] == '<' || lines[i][j] == '>') {
                                std::cout << "\nERROR:\nmessage: weird chars detected while you define variable. line:" << std::to_string(i + 1) << "\n";
                                return;
                            }
                            else {
                                name += lines[i][j];
                            }
                        }
                        else {
                            isReadedName = true;
                        }
                    }
                }
                
                try {
                    stringVariables.at(name);
                    // hata veriyorsa yani öyle bir key yoksa catch e düşecek ve hatasız olacak ama böyle bir key varsa already exist hatası verecek.
                    std::cout << "\nERROR:\nmessage: variable name is already exist. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
                catch(const std::exception& e) {
                    stringVariables.insert(std::pair<std::string, std::string>(name, value));
                }                
            }
            else if (lines[i][4] == 'i' && lines[i][5] == 'n' && lines[i][6] == 't' && lines[i][7] == ':') {            
                for (int j = 8; j < line_i_size; j++) {
                    if (isReadedName) {
                        value += lines[i][j];
                    }
                    else {
                        if (lines[i][j] != '=') {
                            if (lines[i][j] == ' ' || lines[i][j] == '_' || lines[i][j] == '_' || lines[i][j] == '\"' || lines[i][j] == '\'' || lines[i][j] == '\\' || lines[i][j] == '^' || lines[i][j] == '!' || lines[i][j] == '+' || lines[i][j] == '-' || lines[i][j] == '*' || lines[i][j] == '/' || lines[i][j] == '%' || lines[i][j] == '&' || lines[i][j] == '(' || lines[i][j] == ')' || lines[i][j] == '=' || lines[i][j] == '#' || lines[i][j] == '$' || lines[i][j] == '?' || lines[i][j] == '{' || lines[i][j] == '}' || lines[i][j] == '[' || lines[i][j] == ']' || lines[i][j] == '@' || lines[i][j] == ',' || lines[i][j] == '.' || lines[i][j] == ';' || lines[i][j] == '<' || lines[i][j] == '>') {
                                std::cout << "\nERROR:\nmessage: weird chars detected while you define variable. line:" << std::to_string(i + 1) << "\n";
                                return;
                            }
                            else {
                                name += lines[i][j];
                            }
                        }
                        else {
                            isReadedName = true;
                        }
                    }
                }

                try {
                    intVariables.at(name);
                    // hata veriyorsa yani öyle bir key yoksa catch e düşecek ve hatasız olacak ama böyle bir key varsa already exist hatası verecek.
                    std::cout << "\nERROR:\nmessage: variable name is already exist. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
                catch(const std::exception& e) {
                    intVariables.insert(std::pair<std::string, int>(name, std::stoi(value)));
                }
            }
            else if (lines[i][4] == 'd' && lines[i][5] == 'b' && lines[i][6] == 'l' && lines[i][7] == ':') {            
                for (int j = 8; j < line_i_size; j++) {
                    if (isReadedName) {
                        value += lines[i][j];
                    }
                    else {
                        if (lines[i][j] != '=') {
                            if (lines[i][j] == ' ' || lines[i][j] == '_' || lines[i][j] == '_' || lines[i][j] == '\"' || lines[i][j] == '\'' || lines[i][j] == '\\' || lines[i][j] == '^' || lines[i][j] == '!' || lines[i][j] == '+' || lines[i][j] == '-' || lines[i][j] == '*' || lines[i][j] == '/' || lines[i][j] == '%' || lines[i][j] == '&' || lines[i][j] == '(' || lines[i][j] == ')' || lines[i][j] == '=' || lines[i][j] == '#' || lines[i][j] == '$' || lines[i][j] == '?' || lines[i][j] == '{' || lines[i][j] == '}' || lines[i][j] == '[' || lines[i][j] == ']' || lines[i][j] == '@' || lines[i][j] == ',' || lines[i][j] == '.' || lines[i][j] == ';' || lines[i][j] == '<' || lines[i][j] == '>') {
                                std::cout << "\nERROR:\nmessage: weird chars detected while you define variable. line:" << std::to_string(i + 1) << "\n";
                                return;
                            }
                            else {
                                name += lines[i][j];
                            }
                        }
                        else {
                            isReadedName = true;
                        }
                    }
                }

                try {
                    doubleVariables.at(name);
                    // hata veriyorsa yani öyle bir key yoksa catch e düşecek ve hatasız olacak ama böyle bir key varsa already exist hatası verecek.
                    std::cout << "\nERROR:\nmessage: variable name is already exist. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
                catch(const std::exception& e) {
                    doubleVariables.insert(std::pair<std::string, double>(name, std::stod(value)));
                }
            }
            else {
                std::cout << "\nERROR:\nmessage: need :str: or :int: or :dbl: type of variable when you define it. line:" << std::to_string(i + 1) << "\n";
                return;
            }            
        }
        else if (lines[i][0] == 'M') {
            std::string varNameFirst = "";
            std::string varNameSecond = "";
            bool change = false;

            int typeFirst = 0;
            int typeSecond = 0;

            bool firstCalculated = false;
            bool secondCalculated = false;

            std::string process = "";
            
            // ne işlemi olduğunu kaydediyoruz
            for (int j = 1; j < line_i_size; j++) {
                if (lines[i][j] != ' ')
                    process += lines[i][j];
                else
                    break;
            }

            int processSize = process.size();


            if (lines[i][processSize + 1] != ' ' || lines[i][processSize + 2] != '$') {
                std::cout << "\nERROR:\nmessage: need a variable when using M command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            // $ içinde verilen indexe ulaşma.
            // processSize + 2 nedeni şu: örneğin MSIN $:int:num1_ $:int:num2_; dedi, SIN 3 karakterden
            // oluşuyor M harfi ile 4 karakter. 0, 1, 2, 3. indexler yazılı 4. index boşluk karakteri
            // 3 + 1 demek boşluk karakterini almak demek ama biz ondan sonrasını istiyoruz yani $ karakterini bu yüzden +2.
            // ama biz $ kontrolünü yukardaki if ile yaptığımız için +3 (direkt : a atlıyoruz)
            for (int j = processSize + 3; j < line_i_size; j++) {
                if (lines[i][j] == '_') {
                    if (change) {
                        break;
                    }
                    else {
                        change = true;
                        if (lines[i][j + 1] == ' ' && lines[i][j + 2] == '$') {
                            j += 2;
                        }
                        else {
                            std::cout << "\nERROR:\nmessage: need a variable when using M command. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                    }
                }
                else {
                    if (change) {
                        if (!secondCalculated) {
                            if (lines[i][j] == ':') {
                                if (lines[i][j + 1] == 's' && lines[i][j + 2] == 't' && lines[i][j + 3] == 'r' && lines[i][j + 4] == ':') {
                                    typeSecond = 0;
                                    secondCalculated = true;
                                    j += 4;
                                }
                                else if (lines[i][j + 1] == 'i' && lines[i][j + 2] == 'n' && lines[i][j + 3] == 't' && lines[i][j + 4] == ':') {
                                    typeSecond = 1;
                                    secondCalculated = true;
                                    j += 4;
                                }
                                else if (lines[i][j + 1] == 'd' && lines[i][j + 2] == 'b' && lines[i][j + 3] == 'l' && lines[i][j + 4] == ':') {
                                    typeSecond = 2;
                                    secondCalculated = true;
                                    j += 4;
                                }
                                else {
                                    std::cout << "\nERROR:\nmessage: need :str: or :int: or :dbl: type of variable when you using it. line:" << std::to_string(i + 1) << "\n";
                                    return;
                                }
                            }
                            else {
                                std::cout << "\nERROR:\nmessage: need '$:type:varName_' for type when you using a variable you may forgot ':'. line:" << std::to_string(i + 1) << "\n";
                                return;
                            }
                        }
                        else {
                            varNameSecond += lines[i][j];
                        }

                    }
                    else {
                        if (!firstCalculated) {
                            if (lines[i][j] == ':') {
                                if (lines[i][j + 1] == 's' && lines[i][j + 2] == 't' && lines[i][j + 3] == 'r' && lines[i][j + 4] == ':') {
                                    typeFirst = 0;
                                    firstCalculated = true;
                                    j += 4;
                                }
                                else if (lines[i][j + 1] == 'i' && lines[i][j + 2] == 'n' && lines[i][j + 3] == 't' && lines[i][j + 4] == ':') {
                                    typeFirst = 1;
                                    firstCalculated = true;
                                    j += 4;
                                }
                                else if (lines[i][j + 1] == 'd' && lines[i][j + 2] == 'b' && lines[i][j + 3] == 'l' && lines[i][j + 4] == ':') {
                                    typeFirst = 2;
                                    firstCalculated = true;
                                    j += 4;
                                }
                                else {
                                    std::cout << "\nERROR:\nmessage: need :str: or :int: or :dbl: type of variable when you using it. line:" << std::to_string(i + 1) << "\n";
                                    return;
                                }
                            }
                            else {
                                std::cout << "\nERROR:\nmessage: need '$:type:varName_' for type when you using a variable you may forgot ':'. line:" << std::to_string(i + 1) << "\n";
                                return;
                            }
                        }
                        else {
                            varNameFirst += lines[i][j];
                        }
                    }
                }
            }

            // ...variable.at(varNameFirst) gibi girmekten kurtulmak için adresi ile işlem yapmak            
            std::string* str1 = nullptr;
            int* int1 = nullptr;
            double* double1 = nullptr;

            std::string* str2 = nullptr;
            int* int2 = nullptr;
            double* double2 = nullptr;

            if (typeFirst == 0)
                str1 = &stringVariables.at(varNameFirst);
            else if (typeFirst == 1)
                int1 = &intVariables.at(varNameFirst);
            else if (typeFirst == 2)
                double1 = &doubleVariables.at(varNameFirst);

            if (typeSecond == 0)
                str2 = &stringVariables.at(varNameSecond);
            else if (typeSecond == 1)
                int2 = &intVariables.at(varNameSecond);
            else if (typeSecond == 2)
                double2 = &doubleVariables.at(varNameSecond);



            if (process == "=") {
                // 1. str str
                // 2. str int
                // 3. str dbl
                // 4. int str
                // 5. int int
                // 6. int dbl
                // 7. dbl str
                // 8. dbl int
                // 9. dbl dbl
                if (typeFirst == 0 && typeSecond == 0) *str1 = *str2;
                else if (typeFirst == 0 && typeSecond == 1) *str1 = std::to_string(*int2);
                else if (typeFirst == 0 && typeSecond == 2) *str1 = std::to_string(*double2);
                else if (typeFirst == 1 && typeSecond == 0) *int1 = std::stoi(*str2);
                else if (typeFirst == 1 && typeSecond == 1) *int1 = *int2;
                else if (typeFirst == 1 && typeSecond == 2) *int1 = static_cast<int>(*double2);
                else if (typeFirst == 2 && typeSecond == 0) *double1 = std::stod(*str2);
                else if (typeFirst == 2 && typeSecond == 1) *double1 = 0.0 + *int2;
                else if (typeFirst == 2 && typeSecond == 2) *double1 = *double2;
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M=. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == "+") {
                // 1. str str
                // 2. str int
                // 3. str dbl
                // 4. int str
                // 5. int int
                // 6. int dbl
                // 7. dbl str
                // 8. dbl int
                // 9. dbl dbl
                if (typeFirst == 0 && typeSecond == 0) *str1 += *str2;
                else if (typeFirst == 0 && typeSecond == 1) *str1 += std::to_string(*int2);
                else if (typeFirst == 0 && typeSecond == 2) *str1 += std::to_string(*double2);
                else if (typeFirst == 1 && typeSecond == 0) *int1 += std::stoi(*str2);
                else if (typeFirst == 1 && typeSecond == 1) *int1 += *int2;
                else if (typeFirst == 1 && typeSecond == 2) *int1 += static_cast<int>(*double2);
                else if (typeFirst == 2 && typeSecond == 0) *double1 += std::stod(*str2);
                else if (typeFirst == 2 && typeSecond == 1) *double1 += 0.0 + *int2;
                else if (typeFirst == 2 && typeSecond == 2) *double1 += *double2;
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M+. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == "-") {
                // 1. int int
                // 2. int dbl
                // 3. dbl int
                // 4. dbl dbl
                if (typeFirst == 1 && typeSecond == 1) *int1 -= *int2;
                else if (typeFirst == 1 && typeSecond == 2) *int1 -= static_cast<int>(*double2);
                else if (typeFirst == 2 && typeSecond == 1) *double1 -= 0.0 + *int2;
                else if (typeFirst == 2 && typeSecond == 2) *double1 -= *double2;
                else {
                    std::cout << "\nERROR:\nmessage:found incompatible variable types when using M-. line:" << std::to_string(i + 1) << "\n";
                    return;
                }

            }
            else if (process == "*") {
                // 1. int int
                // 2. int dbl
                // 3. dbl int
                // 4. dbl dbl
                if (typeFirst == 1 && typeSecond == 1) *int1 *= *int2;
                else if (typeFirst == 1 && typeSecond == 2) *int1 *= static_cast<int>(*double2);
                else if (typeFirst == 2 && typeSecond == 1) *double1 *= 0.0 + *int2;
                else if (typeFirst == 2 && typeSecond == 2) *double1 *= *double2;
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M*. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == "/") {
                // 1. int int
                // 2. int dbl
                // 3. dbl int
                // 4. dbl dbl
                if (typeFirst == 1 && typeSecond == 1) *int1 /= *int2;
                else if (typeFirst == 1 && typeSecond == 2) *int1 /= static_cast<int>(*double2);
                else if (typeFirst == 2 && typeSecond == 1) *double1 /= 0.0 + *int2;
                else if (typeFirst == 2 && typeSecond == 2) *double1 /= *double2;
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M/. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == "%") {
                // 1. int int
                // 2. int dbl
                if (typeFirst == 1 && typeSecond == 1) *int1 %= *int2;
                else if (typeFirst == 1 && typeSecond == 2) *int1 %= static_cast<int>(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M%. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }

            else if (process == ".SIN") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::sin(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.SIN. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".COS") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::cos(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.COS. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".TAN") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::tan(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.TAN. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".COT") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::cos(*double2) / std::sin(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.COT. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".SEC") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = 1.0 / std::cos(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.SEC. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".CSC") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = 1.0 / std::sin(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.CSC. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }

            else if (process == ".ASIN") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::asin(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.ASIN. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".ACOS") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::acos(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.ACOS. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".ATAN") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::atan(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.ATAN. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".ACOT") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = PI / 2 - std::atan(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.ACOT. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".ASEC") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::acos(1.0 / *double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.ASEC. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".ACSC") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = 1.0 / std::asin(1.0 / *double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.ACSC. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".SINH") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::sinh(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.SINH. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".COSH") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::cosh(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.COSH. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".TANH") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::tanh(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.TANH. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".COTH") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::cosh(*double2) / std::sinh(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.COTH. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".SECH") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = 1.0 / std::cosh(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.SECH. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".CSCH") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = 1.0 / std::sinh(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.CSCH. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".ASINH") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::asinh(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.ASINH. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".ACOSH") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::acosh(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.ACOSH. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".ATANH") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::atanh(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.ATANH. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".ACOTH") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = 0.5 * std::log((*double2 + 1) / (*double2 - 1));
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.ACOTH. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".ASECH") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::acosh(1.0 / *double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.ASECH. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".ACSCH") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::asinh(1.0 / *double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.ACSCH. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            




            else if (process == ".ABS") {
                // 1. int int
                // 2. int dbl
                // 3. dbl int
                // 4. dbl dbl
                if (typeFirst == 1 && typeSecond == 1) *int1 = std::abs(*int2);
                else if (typeFirst == 1 && typeSecond == 2) *int1 = static_cast<int>(std::abs(*double2));
                else if (typeFirst == 2 && typeSecond == 1) *double1 = std::abs(0.0 + *int2);
                else if (typeFirst == 2 && typeSecond == 2) *double1 = std::abs(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.ABS. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".SQRT") {
                // 1. int int
                // 2. int dbl
                // 3. dbl int
                // 4. dbl dbl
                if (typeFirst == 1 && typeSecond == 1) *int1 = static_cast<int>(std::sqrt(*int2));
                else if (typeFirst == 1 && typeSecond == 2) *int1 = static_cast<int>(std::sqrt(*double2));
                else if (typeFirst == 2 && typeSecond == 1) *double1 = std::sqrt(0.0 + *int2);
                else if (typeFirst == 2 && typeSecond == 2) *double1 = std::sqrt(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.SQRT. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".POW") {
                // 1. int int
                // 2. int dbl
                // 3. dbl int
                // 4. dbl dbl
                // first param is x and second param is y -> (x = x to the y)
                if (typeFirst == 1 && typeSecond == 1) *int1 = std::pow(*int1, *int2);
                else if (typeFirst == 1 && typeSecond == 2) *int1 = static_cast<int>(std::pow(*int1, *double2));
                else if (typeFirst == 2 && typeSecond == 1) *double1 = std::pow(*double1, 0.0 + *int2);
                else if (typeFirst == 2 && typeSecond == 2) *double1 = std::pow(*double1, *double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.POW. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".LN") {
                // 1. dbl dbl
                // 2. dbl int
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::log(*double2);
                if (typeFirst == 2 && typeSecond == 1) *double1 = std::log(*int2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.LN. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".LOG") {
                // 1. dbl dbl
                // 2. dbl int
                // 3. int int
                // 4. int dbl
                // first param is x and second param is y -> (x = logx(y))
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::log(*double2) / std::log(*double1);
                if (typeFirst == 2 && typeSecond == 1) *double1 = std::log(*int2) / std::log(*double1);
                if (typeFirst == 1 && typeSecond == 1) *double1 = std::log(*int2) / std::log(*int1);
                if (typeFirst == 1 && typeSecond == 2) *double1 = std::log(*double2) / std::log(*int1);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.LOG. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".EXP") {
                // 1. dbl dbl
                // 1. dbl int
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::exp(*double2);
                if (typeFirst == 2 && typeSecond == 1) *double1 = std::exp(*int2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.EXP. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".ROUND") {
                // 1. dbl dbl
                // 2. int dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::round(*double2);
                if (typeFirst == 1 && typeSecond == 2) *int1 = std::round(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.ROUND. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".CEIL") {
                // 1. dbl dbl
                // 2. int dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::ceil(*double2);
                if (typeFirst == 1 && typeSecond == 2) *int1 = std::ceil(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.CEIL. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".FLOOR") {
                // 1. dbl dbl
                // 2. int dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = std::floor(*double2);
                if (typeFirst == 1 && typeSecond == 2) *int1 = std::floor(*double2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.FLOOR. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".FACT") {
                // 1. int int
                if (typeFirst == 1 && typeSecond == 1) *int1 = factorial(*int2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.FACT. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".PERM") {
                // 1. int int
                if (typeFirst == 1 && typeSecond == 1) *int1 = permutation(*int1, *int2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.PERM. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".COMB") {
                // 1. int int
                if (typeFirst == 1 && typeSecond == 1) *int1 = combination(*int1, *int2);
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.COMB. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".DEG2RAD") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = (*double2 / 180) * PI;
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.DEG2RAD. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else if (process == ".RAD2DEG") {
                // 1. dbl dbl
                if (typeFirst == 2 && typeSecond == 2) *double1 = (*double2 / PI) * 180;
                else {
                    std::cout << "\nERROR:\nmessage: found incompatible variable types when using M.RAD2DEG. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            
            

            else {
                std::cout << "\nERROR:\nmessage: false math operator. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }
        

        else if (lines[i][0] == 'G' && lines[i][1] == 'O' && lines[i][2] == 'T' && lines[i][3] == 'O' && lines[i][4] == ' ') { // go to line
            std::string lineNumber = "";
            
            if (std::stoi(std::to_string(lines[i][5])) % 1 == 0) { //tam sayı mı kontrol
                for (int j = 5; j < line_i_size; j++) {
                    lineNumber += lines[i][j];
                }
            }
            else {
                std::cout << "\nERROR:\nmessage: You have to using pure integer number while using GOTO. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            int integerNumberOfLineNumber = std::stoi(lineNumber);
            if (integerNumberOfLineNumber < 1 || integerNumberOfLineNumber > lines.size()) {
                std::cout << "\nERROR:\nmessage: Line cannot found. It can be too big or too small. line:" << std::to_string(i + 1) << "\n";
                return;    
            }

            i = integerNumberOfLineNumber - 2; //go to i. satır
            continue;
        }
        else if (lines[i][0] == 'E' && lines[i][1] == 'X' && lines[i][2] == 'I' && lines[i][3] == 'T' && line_i_size == 4) { // stop program
            return;
        }

        else if (lines[i][0] == 'I' && lines[i][1] == 'F') { // if condition
            std::string varNameFirst = ""; // first state
            std::string varNameSecond = ""; // second state
            std::string elseLineNumber = ""; // else go to <number>

            int typeFirst = 0;
            int typeSecond = 0;
            int typeThird = 0;

            bool canPass = false;


            // string
            if (lines[i][5] == '$' && lines[i][6] == ':' && lines[i][7] == 's' && lines[i][8] == 't' && lines[i][9] == 'r' && lines[i][10] == ':') {
                int whereFirstEnded = 0;
                int whereSecondEnded = 0;
                
                for (int j = 11; j < line_i_size; j++) {
                    if (lines[i][j] != '_') {
                        varNameFirst += lines[i][j];
                    }
                    else {
                        whereFirstEnded = j + 1;
                        break;
                    }
                }

                // have to second variable is string
                if (lines[i][whereFirstEnded] == ' ' && lines[i][whereFirstEnded + 1] == '$' && lines[i][whereFirstEnded + 2] == ':' && lines[i][whereFirstEnded + 3] == 's' && lines[i][whereFirstEnded + 4] == 't' && lines[i][whereFirstEnded + 5] == 'r' && lines[i][whereFirstEnded + 6] == ':') {
                    
                    for (int j = whereFirstEnded + 7; j < line_i_size; j++) {
                        if (lines[i][j] != '_') {
                            varNameSecond += lines[i][j];
                        }
                        else {
                            whereSecondEnded = j + 1;
                            break;
                        }
                    }

                    if (lines[i][whereSecondEnded] == ' ' && lines[i][whereSecondEnded + 1] == 'E' && lines[i][whereSecondEnded + 2] == 'L' && lines[i][whereSecondEnded + 3] == 'S' && lines[i][whereSecondEnded + 4] == 'E' && lines[i][whereSecondEnded + 5] == '=') {
                        for (int j = whereSecondEnded + 6; j < line_i_size; j++) {
                            elseLineNumber += lines[i][j];
                        }

                        typeFirst = 0;
                        typeSecond = 0;
                    }
                    else {
                        std::cout << "\nERROR:\nmessage: you have to using ELSE=<pure integer> end of the IF condition. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using just string-string when using string in IF condition. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            // int
            else if (lines[i][5] == '$' && lines[i][6] == ':' && lines[i][7] == 'i' && lines[i][8] == 'n' && lines[i][9] == 't' && lines[i][10] == ':') {
                int whereFirstEnded = 0;
                int whereSecondEnded = 0;
                
                for (int j = 11; j < line_i_size; j++) {
                    if (lines[i][j] != '_') {
                        varNameFirst += lines[i][j];
                    }
                    else {
                        whereFirstEnded = j + 1;
                        break;
                    }
                }
                
                if (lines[i][whereFirstEnded] == ' ' && lines[i][whereFirstEnded + 1] == '$' && lines[i][whereFirstEnded + 2] == ':') {
                    if (lines[i][whereFirstEnded + 3] == 'i' && lines[i][whereFirstEnded + 4] == 'n' && lines[i][whereFirstEnded + 5] == 't' && lines[i][whereFirstEnded + 6] == ':') {
                        for (int j = whereFirstEnded + 7; j < line_i_size; j++) {
                            if (lines[i][j] != '_') {
                                varNameSecond += lines[i][j];
                            }
                            else {
                                whereSecondEnded = j + 1;
                                break;
                            }
                        }

                        typeFirst = 1;
                        typeSecond = 1;
                    }
                    else if (lines[i][whereFirstEnded + 3] == 'd' && lines[i][whereFirstEnded + 4] == 'b' && lines[i][whereFirstEnded + 5] == 'l' && lines[i][whereFirstEnded + 6] == ':') {
                        for (int j = whereFirstEnded + 7; j < line_i_size; j++) {
                            if (lines[i][j] != '_') {
                                varNameSecond += lines[i][j];
                            }
                            else {
                                whereSecondEnded = j + 1;
                                break;
                            }
                        }

                        typeFirst = 1;
                        typeSecond = 2;
                    }
                    else {
                        std::cout << "\nERROR:\nmessage: you have to using int-int or double-double when using numeric values in IF condition. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }

                    if (lines[i][whereSecondEnded] == ' ' && lines[i][whereSecondEnded + 1] == 'E' && lines[i][whereSecondEnded + 2] == 'L' && lines[i][whereSecondEnded + 3] == 'S' && lines[i][whereSecondEnded + 4] == 'E' && lines[i][whereSecondEnded + 5] == '=') {
                        for (int j = whereSecondEnded + 6; j < line_i_size; j++) {
                            elseLineNumber += lines[i][j];
                        }
                    }
                    else {
                        std::cout << "\nERROR:\nmessage: you have to using ELSE=<pure integer> end of the IF condition. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                }
            }
            // double
            else if (lines[i][5] == '$' && lines[i][6] == ':' && lines[i][7] == 'd' && lines[i][8] == 'b' && lines[i][9] == 'l' && lines[i][10] == ':') {
                int whereFirstEnded = 0;
                int whereSecondEnded = 0;
                
                for (int j = 11; j < line_i_size; j++) {
                    if (lines[i][j] != '_') {
                        varNameFirst += lines[i][j];
                    }
                    else {
                        whereFirstEnded = j + 1;
                        break;
                    }
                }



                if (lines[i][whereFirstEnded] == ' ' && lines[i][whereFirstEnded + 1] == '$' && lines[i][whereFirstEnded + 2] == ':') {
                    if (lines[i][whereFirstEnded + 3] == 'd' && lines[i][whereFirstEnded + 4] == 'b' && lines[i][whereFirstEnded + 5] == 'l' && lines[i][whereFirstEnded + 6] == ':') {
                        for (int j = whereFirstEnded + 7; j < line_i_size; j++) {
                            if (lines[i][j] != '_') {
                                varNameSecond += lines[i][j];
                            }
                            else {
                                whereSecondEnded = j + 1;
                                break;
                            }
                        }

                        typeFirst = 2;
                        typeSecond = 2;
                    }
                    else if (lines[i][whereFirstEnded + 3] == 'i' && lines[i][whereFirstEnded + 4] == 'n' && lines[i][whereFirstEnded + 5] == 't' && lines[i][whereFirstEnded + 6] == ':') {
                        for (int j = whereFirstEnded + 7; j < line_i_size; j++) {
                            if (lines[i][j] != '_') {
                                varNameSecond += lines[i][j];
                            }
                            else {
                                whereSecondEnded = j + 1;
                                break;
                            }
                        }

                        typeFirst = 2;
                        typeSecond = 1;
                    }
                    else {
                        std::cout << "\nERROR:\nmessage: you have to using int-int or double-double when using numeric values in IF condition. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }

                    if (lines[i][whereSecondEnded] == ' ' && lines[i][whereSecondEnded + 1] == 'E' && lines[i][whereSecondEnded + 2] == 'L' && lines[i][whereSecondEnded + 3] == 'S' && lines[i][whereSecondEnded + 4] == 'E' && lines[i][whereSecondEnded + 5] == '=') {
                        for (int j = whereSecondEnded + 6; j < line_i_size; j++) {
                            elseLineNumber += lines[i][j];
                        }
                    }
                    else {
                        std::cout << "\nERROR:\nmessage: you have to using ELSE=<pure integer> end of the IF condition. line:" << std::to_string(i + 1) << "\n";
                        return;
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
                    std::string first = stringVariables.at(varNameFirst);
                    std::string second = stringVariables.at(varNameSecond);
                    
                    if (first == second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";;
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else if (typeFirst == 1 && typeSecond == 1) {
                    // control
                    int first = intVariables.at(varNameFirst);
                    int second = intVariables.at(varNameSecond);

                    if (first == second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else if (typeFirst == 2 && typeSecond == 2) {
                    // control
                    double first = doubleVariables.at(varNameFirst);
                    double second = doubleVariables.at(varNameSecond);

                    if (first == second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using string-string or int-int or double-double in IF== condition. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            // !=
            // string-string
            // int-int
            // double-double
            else if (lines[i][2] == '!' && lines[i][3] == '=') {
                if (typeFirst == 0 && typeSecond == 0) {
                    // control
                    std::string first = stringVariables.at(varNameFirst);
                    std::string second = stringVariables.at(varNameSecond);
                    

                    if (first != second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else if (typeFirst == 1 && typeSecond == 1) {
                    // control
                    int first = intVariables.at(varNameFirst);
                    int second = intVariables.at(varNameSecond);


                    if (first != second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else if (typeFirst == 2 && typeSecond == 2) {
                    // control
                    double first = doubleVariables.at(varNameFirst);
                    double second = doubleVariables.at(varNameSecond);


                    if (first != second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using string-string or int-int or double-double in IF!= condition. line:" << std::to_string(i + 1) << "\n";
                    return;
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
                    int first = intVariables.at(varNameFirst);
                    int second = intVariables.at(varNameSecond);
                    

                    if (first > second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 1 && typeSecond == 2) {
                    // control
                    int first = intVariables.at(varNameFirst);
                    double second = doubleVariables.at(varNameSecond);
                    

                    if (first > second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 2 && typeSecond == 2) {
                    // control
                    double first = doubleVariables.at(varNameFirst);
                    double second = doubleVariables.at(varNameSecond);


                    if (first > second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 2 && typeSecond == 1) {
                    // control
                    double first = doubleVariables.at(varNameFirst);
                    int second = intVariables.at(varNameSecond);


                    if (first > second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using int-int/double or double-double/int in IF>> condition. line:" << std::to_string(i + 1) << "\n";
                    return;
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
                    int first = intVariables.at(varNameFirst);
                    int second = intVariables.at(varNameSecond);
                    

                    if (first >= second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 1 && typeSecond == 2) {
                    // control
                    int first = intVariables.at(varNameFirst);
                    double second = doubleVariables.at(varNameSecond);
                    

                    if (first >= second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 2 && typeSecond == 2) {
                    // control
                    double first = doubleVariables.at(varNameFirst);
                    double second = doubleVariables.at(varNameSecond);
                    

                    if (first >= second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 2 && typeSecond == 1) {
                    // control
                    double first = doubleVariables.at(varNameFirst);
                    int second = intVariables.at(varNameSecond);


                    if (first >= second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using int-int/double or double-double/int in IF>= condition. line:" << std::to_string(i + 1) << "\n";
                    return;
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
                    int first = intVariables.at(varNameFirst);
                    int second = intVariables.at(varNameSecond);
                    

                    if (first < second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 1 && typeSecond == 2) {
                    // control
                    int first = intVariables.at(varNameFirst);
                    double second = doubleVariables.at(varNameSecond);
                    

                    if (first < second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 2 && typeSecond == 2) {
                    // control
                    double first = doubleVariables.at(varNameFirst);
                    double second = doubleVariables.at(varNameSecond);


                    if (first < second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 2 && typeSecond == 1) {
                    // control
                    double first = doubleVariables.at(varNameFirst);
                    int second = intVariables.at(varNameSecond);


                    if (first < second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using int-int/double or double-double/int in IF<< condition. line:" << std::to_string(i + 1) << "\n";
                    return;
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
                    int first = intVariables.at(varNameFirst);
                    int second = intVariables.at(varNameSecond);
                    

                    if (first <= second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 1 && typeSecond == 2) {
                    // control
                    int first = intVariables.at(varNameFirst);
                    double second = doubleVariables.at(varNameSecond);


                    if (first <= second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 2 && typeSecond == 2) {
                    // control
                    double first = doubleVariables.at(varNameFirst);
                    double second = doubleVariables.at(varNameSecond);
                    

                    if (first <= second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                if (typeFirst == 2 && typeSecond == 1) {
                    // control
                    double first = doubleVariables.at(varNameFirst);
                    int second = intVariables.at(varNameSecond);
                    

                    if (first <= second) {
                        // correct and pass
                        continue;
                    }
                    else {
                        // go to command
                        int int_ElseLineNumber = std::stoi(elseLineNumber);
                        if (int_ElseLineNumber > lines.size() || int_ElseLineNumber < 1) {
                            std::cout << "\nERROR:\nmessage: line can not found. maybe its too big or too small. error in IF condition ELSE=. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        i = int_ElseLineNumber - 2;
                        continue;
                    }
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using int-int/double or double-double/int in IF<= condition. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }

            else {
                std::cout << "\nERROR:\nmessage: you have to using == != >> >= << <= in IF condition. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }
        

        else if (lines[i][0] == 'I' && lines[i][1] == 'N' && lines[i][2] == 'C' && lines[i][3] == ' ') {
            
            std::string varName = "";

            if (lines[i][4] == '$' && lines[i][5] == ':' && lines[i][line_i_size - 1] == '_') {
                if (lines[i][6] == 'i' && lines[i][7] == 'n' && lines[i][8] == 't' && lines[i][9] == ':') {
                    for (int j = 10; j < line_i_size - 1; j++) {
                        varName += lines[i][j];
                    }
                    intVariables.at(varName)++;
                }
                else if (lines[i][6] == 'd' && lines[i][7] == 'b' && lines[i][8] == 'l' && lines[i][9] == ':') {
                    for (int j = 10; j < line_i_size - 1; j++) {
                        varName += lines[i][j];
                    }
                    doubleVariables.at(varName)++;
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using int or double variable types when using INC command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to using variable when using INC command. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }
        else if (lines[i][0] == 'D' && lines[i][1] == 'E' && lines[i][2] == 'C' && lines[i][3] == ' ') {
            
            std::string varName = "";

            if (lines[i][4] == '$' && lines[i][5] == ':' && lines[i][line_i_size - 1] == '_') {
                if (lines[i][6] == 'i' && lines[i][7] == 'n' && lines[i][8] == 't' && lines[i][9] == ':') {
                    for (int j = 10; j < line_i_size - 1; j++) {
                        varName += lines[i][j];
                    }
                    intVariables.at(varName)--;
                }
                else if (lines[i][6] == 'd' && lines[i][7] == 'b' && lines[i][8] == 'l' && lines[i][9] == ':') {
                    for (int j = 10; j < line_i_size - 1; j++) {
                        varName += lines[i][j];
                    }
                    doubleVariables.at(varName)--;
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using int or double variable types when using DEC command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to using variable when using DEC command. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }


        else if (lines[i][0] == 'N' && lines[i][1] == 'U' && lines[i][2] == 'L' && lines[i][3] == 'L' && lines[i][4] == ' ') {
            
            if (lines[i][5] != '$' || lines[i][6] != ':' || lines[i][line_i_size - 1] != '_') {
                std::cout << "\nERROR:\nmessage: you have to using a variable when using NULL command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            std::string varName = "";
            for (int j = 11; j < line_i_size - 1; j++) {
                varName += lines[i][j];
            }

            if (lines[i][7] == 's' && lines[i][8] == 't' && lines[i][9] == 'r' && lines[i][10] == ':')
                stringVariables.at(varName) = "";
            else if (lines[i][7] == 'i' && lines[i][8] == 'n' && lines[i][9] == 't' && lines[i][10] == ':')
                intVariables.at(varName) = 0;
            else if (lines[i][7] == 'd' && lines[i][8] == 'b' && lines[i][9] == 'l' && lines[i][10] == ':')
                doubleVariables.at(varName) = 0.0;
            else {
                std::cout << "\nERROR:\nmessage: you have to using :int: or :string: or :double: variable when using NULL command for call variables. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }

        else if (lines[i][0] == 'F' && lines[i][1] == 'R' && lines[i][2] == 'E' && lines[i][3] == 'E' && lines[i][4] == ' ') {
            
            if (lines[i][5] != '$' || lines[i][6] != ':' || lines[i][line_i_size - 1] != '_') {
                std::cout << "\nERROR:\nmessage: you have to using a variable when using FREE command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            std::string varName = "";
            for (int j = 11; j < line_i_size - 1; j++) {
                varName += lines[i][j];
            }

            if (lines[i][7] == 's' && lines[i][8] == 't' && lines[i][9] == 'r' && lines[i][10] == ':') {
                stringVariables.at(varName) = "";
                stringVariables.erase(varName);
            }
            else if (lines[i][7] == 'i' && lines[i][8] == 'n' && lines[i][9] == 't' && lines[i][10] == ':') {
                intVariables.at(varName) = 0;
                intVariables.erase(varName);
            }
            else if (lines[i][7] == 'd' && lines[i][8] == 'b' && lines[i][9] == 'l' && lines[i][10] == ':') {
                doubleVariables.at(varName) = 0.0;
                doubleVariables.erase(varName);
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to using string or int or double variable when using NULL command for call variables. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }

        else if (lines[i][0] == 'I' && lines[i][1] == 'N' && lines[i][2] == 'D' && lines[i][3] == 'E' && lines[i][4] == 'X') {
            if (lines[i][5] == '[' && lines[i][line_i_size - 1] == '_') { // ikinci koşul olmasının sebebi komutumuz _ ile bitmek zorunda ikinci parametre değişkendir ve _ ile biter.
                int index = 0;
                int afterIndexSpace = 0;
                
                if (lines[i][6] == '$' && lines[i][7] == ':' && lines[i][8] == 'i' && lines[i][9] == 'n' && lines[i][10] == 't' && lines[i][11] == ':') {
                    std::string indexVarName = "";
                    for (int j = 12; j < line_i_size; j++) {
                        if (lines[i][j] != '_') {
                            indexVarName += lines[i][j];
                        }
                        else {
                            if (lines[i][j + 1] != ']') {
                                std::cout << "\nERROR:\nmessage: you have to put ']' in 'INDEX[<int>]' when using INDEX command. or you may forgot '_' at the end of call index variable. line:" << std::to_string(i + 1) << "\n";
                                return;
                            }
                            afterIndexSpace = j + 2;
                            break;
                        }
                    }
                    index = intVariables.at(indexVarName);
                }
                else if (std::stoi(std::to_string(lines[i][6])) % 1 == 0) {
                    std::string indexIntValue = "";
                    bool isEnded = false;
                    for (int j = 6; j < line_i_size; j++) {
                        if (lines[i][j] == ']') {
                            isEnded = true;
                            afterIndexSpace = j + 1;
                            break;
                        }
                        else {
                            indexIntValue += lines[i][j];
                        }
                    }
                    if (!isEnded) {
                        std::cout << "\nERROR:\nmessage: you have to put ']' in 'INDEX[<int>]' when using INDEX command. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                    index = std::stoi(indexIntValue);
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to use pure integer number or int variable when using INDEX command's index. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
        
                if (lines[i][afterIndexSpace] != ' ' || lines[i][afterIndexSpace + 1] != '$' || lines[i][afterIndexSpace + 2] != ':' || lines[i][afterIndexSpace + 3] != 's' || lines[i][afterIndexSpace + 4] != 't' || lines[i][afterIndexSpace + 5] != 'r' || lines[i][afterIndexSpace + 6] != ':') {
                    std::cout << "\nERROR:\nmessage: you have to using :str: and :str: variable types when using INDEX command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }

                std::string firstVarName = "";
                std::string secondVarName = "";
                bool isSwitch = false;
                for (int j = afterIndexSpace + 7; j < line_i_size; j++) {
                    if (isSwitch) {
                        if (lines[i][j] != '_') {
                            secondVarName += lines[i][j];
                        }
                        else {
                            if (j != line_i_size - 1) {
                                std::cout << "\nERROR:\nmessage: you have to using variable when using INDEX command. line:" << std::to_string(i + 1) << "\n";
                                return;
                            }
                        }
                    }
                    else {
                        if (lines[i][j] != '_') {
                            firstVarName += lines[i][j];
                        }
                        else {
                            if (lines[i][j + 1] != ' ' || lines[i][j + 2] != '$' || lines[i][j + 3] != ':' || lines[i][j + 4] != 's' || lines[i][j + 5] != 't' || lines[i][j + 6] != 'r' || lines[i][j + 7] != ':') {
                                std::cout << "\nERROR:\nmessage: you have to using :str: and :str: variable types when using INDEX command. line:" << std::to_string(i + 1) << "\n";
                                return;
                            }
                            j += 7; // 8 yapmamız gerekirdi ancak for döngüsü tamamlanınca zaten +1 yapacak o yüzden 7
                            isSwitch = true;
                        }
                    }
                }

                // ikinci parametrenin değerini ilk parametrenin index'ince indexine eşitliyoruz.
                stringVariables.at(secondVarName) = stringVariables.at(firstVarName).at(index);
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to put '[' in 'INDEX[<int>]' when using INDEX command. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }

        else if (lines[i][0] == 'S' && lines[i][1] == 'L' && lines[i][2] == 'E' && lines[i][3] == 'E' && lines[i][4] == 'P' && lines[i][5] == ' ') {
            int millisecond = 0;
            if (lines[i][6] == '$' && lines[i][7] == ':' && lines[i][8] == 'i' && lines[i][9] == 'n' && lines[i][10] == 't' && lines[i][11] == ':' && lines[i][line_i_size - 1] == '_') {
                std::string varName = "";
                for (int j = 12; j < line_i_size - 1; j++) {
                    varName += lines[i][j];
                }
                millisecond = intVariables.at(varName);
            }
            else if (std::stoi(std::to_string(lines[i][6])) % 1 == 0) {
                std::string number = "";
                for (int j = 6; j < line_i_size; j++) {
                    number += lines[i][j];
                }
                millisecond = std::stoi(number);
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to use pure integer number or int variable when using SLEEP command's millisecond parameter. line:" << std::to_string(i + 1) << "\n";
                return;
            }
            // main fonksiyonumuzun threadinde yani genel bir threadde bekletiyoruz.
            std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
        }

        else if (lines[i][0] == 'R' && lines[i][1] == 'A' && lines[i][2] == 'N' && lines[i][3] == 'D' && lines[i][4] == ' ') {
            if (lines[i][5] == '$' && lines[i][6] == ':' && lines[i][7] == 'i' && lines[i][8] == 'n' && lines[i][9] == 't' && lines[i][10] == ':' && lines[i][line_i_size - 1] == '_') {
                std::string varName = "";
                std::string varNameMinValue = "";
                std::string varNameMaxValue = "";
                int whereIsJ = 0;

                // read destination variable
                for (int j = 11; j < line_i_size; j++) {
                    if (lines[i][j] != '_') {
                        varName += lines[i][j];
                    }
                    else {
                        whereIsJ = j + 1; // ' '
                        break;
                    }
                }
                if (lines[i][whereIsJ] != ' ' || lines[i][whereIsJ + 1] != '$' || lines[i][whereIsJ + 2] != ':' || lines[i][whereIsJ + 3] != 'i' || lines[i][whereIsJ + 4] != 'n' || lines[i][whereIsJ + 5] != 't' || lines[i][whereIsJ + 6] != ':') {
                    std::cout << "\nERROR:\nmessage: you have to using integer variables when using RAND command or you may forgot put space between parameters of RAND command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
                whereIsJ += 7;

                // read min val variable
                for (int j = whereIsJ; j < line_i_size; j++) {
                    if (lines[i][j] != '_') {
                        varNameMinValue += lines[i][j];
                    }
                    else {
                        whereIsJ = j + 1; // ' '
                        break;
                    }
                }
                if (lines[i][whereIsJ] != ' ' || lines[i][whereIsJ + 1] != '$' || lines[i][whereIsJ + 2] != ':' || lines[i][whereIsJ + 3] != 'i' || lines[i][whereIsJ + 4] != 'n' || lines[i][whereIsJ + 5] != 't' || lines[i][whereIsJ + 6] != ':') {
                    std::cout << "\nERROR:\nmessage: you have to using integer variables when using RAND command or you may forgot put space between parameters of RAND command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
                whereIsJ += 7;

                // read max val variable
                for (int j = whereIsJ; j < line_i_size; j++) {
                    if (lines[i][j] != '_') {
                        varNameMaxValue += lines[i][j];
                    }
                    else {
                        if (j != line_i_size - 1) {
                            std::cout << "\nERROR:\nmessage: you have to using integer variables when using RAND command. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        //whereIsJ = j + 1; // ' '
                        break;
                    }
                }

                int max = intVariables.at(varNameMaxValue);
                int min = intVariables.at(varNameMinValue);

                if (max < min) {
                    std::cout << "\nERROR:\nmessage: second parameter can not greater than third parameter when using RAND command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }

                int randomNumber = rand() % (max + 1 - min) + min;
                // rand() % (max + 1 - min) + min
                // ((double)rand() * (max - min)) / (double)RAND_MAX + min;
                intVariables.at(varName) = randomNumber;
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to use integer variable when using RAND command (RAND $:int:dest_ $:int:min_ $:int:max_;). line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }


        else if (lines[i][0] == 'S' && lines[i][1] == 'T' && lines[i][2] == 'R' && lines[i][3] == 'L' && lines[i][4] == 'E' && lines[i][5] == 'N' && lines[i][6] == ' ') {
            if (lines[i][7] == '$' && lines[i][8] == ':' && lines[i][9] == 's' && lines[i][10] == 't' && lines[i][11] == 'r' && lines[i][12] == ':' && lines[i][line_i_size - 1] == '_') {
                std::string varNameSource = "";
                std::string varNameDestination = "";
                int whereIsJ = 0;

                // read source variable
                for (int j = 13; j < line_i_size; j++) {
                    if (lines[i][j] != '_') {
                        varNameSource += lines[i][j];
                    }
                    else {
                        whereIsJ = j + 1; // ' '
                        break;
                    }
                }
                if (lines[i][whereIsJ] != ' ' || lines[i][whereIsJ + 1] != '$' || lines[i][whereIsJ + 2] != ':' || lines[i][whereIsJ + 3] != 'i' || lines[i][whereIsJ + 4] != 'n' || lines[i][whereIsJ + 5] != 't' || lines[i][whereIsJ + 6] != ':') {
                    std::cout << "\nERROR:\nmessage: you have to using for parameters (first: str, second: int) (string, int) variables when using STRLEN command or you may forgot put space between parameters of STRLEN command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
                whereIsJ += 7;

                // read destination variable
                for (int j = whereIsJ; j < line_i_size; j++) {
                    if (lines[i][j] != '_') {
                        varNameDestination += lines[i][j];
                    }
                    else {
                        if (j != line_i_size - 1) {
                            std::cout << "\nERROR:\nmessage: you have to using for parameters (first: str, second: int) (string, int) variables when using STRLEN command or you may forgot put space between parameters of STRLEN command. line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        //whereIsJ = j + 1; // ' '
                        break;
                    }
                }

                // int or size_t
                size_t strSize = stringVariables.at(varNameSource).size();
                intVariables.at(varNameDestination) = strSize;
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to using for parameters (first: str, second: int) (string, int) variables when using STRLEN command. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }


        else if (lines[i][0] == 'C' && lines[i][1] == 'L' && lines[i][2] == 'E' && lines[i][3] == 'A' && lines[i][4] == 'R' && line_i_size == 5) { // clear console
            if (os_name == "Windows") {
                system("cls");
            }
            else {
                system("clear");
            }
        }

        else if (lines[i][0] == 'O' && lines[i][1] == 'S' && lines[i][2] == 'N' && lines[i][3] == 'A' && lines[i][4] == 'M' && lines[i][5] == 'E' && lines[i][6] == ' ') {
            
            if (lines[i][7] != '$' || lines[i][8] != ':' || lines[i][line_i_size - 1] != '_') {
                std::cout << "\nERROR:\nmessage: you have to using a :string: variable when using OSNAME command. line:" << std::to_string(i + 1) << "\n";
                return;
            }
            
            std::string varName = "";
            for (int j = 13; j < line_i_size - 1; j++) {
                varName += lines[i][j];
            }

            if (lines[i][9] == 's' && lines[i][10] == 't' && lines[i][11] == 'r' && lines[i][12] == ':') {
                stringVariables.at(varName) = os_name;
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to using :string: variable when using OSNAME command for call variables. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }


        else if (lines[i][0] == 'R' && lines[i][1] == 'E' && lines[i][2] == 'P' && lines[i][3] == 'L' && lines[i][4] == 'A'&& lines[i][5] == 'C' && lines[i][6] == 'E') {
            if (lines[i][7] == '[' && lines[i][line_i_size - 1] == '_') { // ikinci koşul olmasının sebebi komutumuz _ ile bitmek zorunda ikinci parametre değişkendir ve _ ile biter.
                int index = 0;
                int afterIndexSpace = 0;
                
                if (lines[i][8] == '$' && lines[i][9] == ':' && lines[i][10] == 'i' && lines[i][11] == 'n' && lines[i][12] == 't' && lines[i][13] == ':') {
                    std::string indexVarName = "";
                    for (int j = 14; j < line_i_size; j++) {
                        if (lines[i][j] != '_') {
                            indexVarName += lines[i][j];
                        }
                        else {
                            if (lines[i][j + 1] != ']') {
                                std::cout << "\nERROR:\nmessage: you have to put ']' in 'REPLACE[<int>]' when using REPLACE command. or you may forgot '_' at the end of call index variable. line:" << std::to_string(i + 1) << "\n";
                                return;
                            }
                            afterIndexSpace = j + 2;
                            break;
                        }
                    }
                    index = intVariables.at(indexVarName);
                }
                else if (std::stoi(std::to_string(lines[i][8])) % 1 == 0) {
                    std::string indexIntValue = "";
                    bool isEnded = false;
                    for (int j = 8; j < line_i_size; j++) {
                        if (lines[i][j] == ']') {
                            isEnded = true;
                            afterIndexSpace = j + 1;
                            break;
                        }
                        else {
                            indexIntValue += lines[i][j];
                        }
                    }
                    if (!isEnded) {
                        std::cout << "\nERROR:\nmessage: you have to put ']' in 'REPLACE[<int>]' when using REPLACE command. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                    index = std::stoi(indexIntValue);
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to use pure integer number or int variable when using REPLACE command's index. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
        
                if (lines[i][afterIndexSpace] != ' ' || lines[i][afterIndexSpace + 1] != '$' || lines[i][afterIndexSpace + 2] != ':' || lines[i][afterIndexSpace + 3] != 's' || lines[i][afterIndexSpace + 4] != 't' || lines[i][afterIndexSpace + 5] != 'r' || lines[i][afterIndexSpace + 6] != ':') {
                    std::cout << "\nERROR:\nmessage: you have to using :str: and :str: variable types when using REPLACE command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }

                std::string firstVarName = "";
                std::string secondVarName = "";
                bool isSwitch = false;
                for (int j = afterIndexSpace + 7; j < line_i_size; j++) {
                    if (isSwitch) {
                        if (lines[i][j] != '_') {
                            secondVarName += lines[i][j];
                        }
                        else {
                            if (j != line_i_size - 1) {
                                std::cout << "\nERROR:\nmessage: you have to using variable when using REPLACE command. line:" << std::to_string(i + 1) << "\n";
                                return;
                            }
                        }
                    }
                    else {
                        if (lines[i][j] != '_') {
                            firstVarName += lines[i][j];
                        }
                        else {
                            if (lines[i][j + 1] != ' ' || lines[i][j + 2] != '$' || lines[i][j + 3] != ':' || lines[i][j + 4] != 's' || lines[i][j + 5] != 't' || lines[i][j + 6] != 'r' || lines[i][j + 7] != ':') {
                                std::cout << "\nERROR:\nmessage: you have to using :str: and :str: variable types when using REPLACE command. line:" << std::to_string(i + 1) << "\n";
                                return;
                            }
                            j += 7; // 8 yapmamız gerekirdi ancak for döngüsü tamamlanınca zaten +1 yapacak o yüzden 7
                            isSwitch = true;
                        }
                    }
                }


                // ikinci parametrenin 0 veya daha fazla elemanlı string olma durumu.
                if (stringVariables.at(secondVarName).size() == 0) {
                    stringVariables.at(firstVarName).erase(index, 1);
                }
                else {
                    stringVariables.at(firstVarName).replace(index, 1, stringVariables.at(secondVarName));
                }

            }
            else {
                std::cout << "\nERROR:\nmessage: you have to put '[' in 'REPLACE[<int>]' when using REPLACE command. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }


        else if (lines[i][0] == 'T' && lines[i][1] == 'E' && lines[i][2] == 'R' && lines[i][3] == 'M' && lines[i][4] == 'I' && lines[i][5] == 'N' && lines[i][6] == 'A' && lines[i][7] == 'L' && lines[i][8] == ' ') {
            
            if (lines[i][9] != '$' || lines[i][10] != ':' || lines[i][line_i_size - 1] != '_') {
                std::cout << "\nERROR:\nmessage: you have to using a :string: variable when using TERMINAL command. line:" << std::to_string(i + 1) << "\n";
                return;
            }
            
            std::string varName = "";
            for (int j = 15; j < line_i_size - 1; j++) {
                varName += lines[i][j];
            }

            if (lines[i][11] == 's' && lines[i][12] == 't' && lines[i][13] == 'r' && lines[i][14] == ':') {
                system(stringVariables.at(varName).c_str());
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to using :string: variable when using TERMINAL command for call variables. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }


        else if (lines[i][0] == 'T' && lines[i][1] == 'I' && lines[i][2] == 'M' && lines[i][3] == 'E' && lines[i][4] == '.') {
            
            std::string timeCmd = "";
            int ind = 6;
            for (int j = 5; lines[i][j] != ' '; j++) {
                timeCmd += lines[i][j];
                ind++;
            }

            if (lines[i][ind - 1] != ' ' || lines[i][ind] != '$' || lines[i][ind + 1] != ':' || lines[i][ind + 2] != 'i' || lines[i][ind + 3] != 'n' || lines[i][ind + 4] != 't' || lines[i][ind + 5] != ':' || lines[i][line_i_size - 1] != '_') {
                std::cout << "\nERROR:\nmessage: you have to using a :int: variable when using TIME command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            ind += 6;

            std::string varName = "";
            for (int j = ind; j < line_i_size - 1; j++) {
                varName += lines[i][j];
            }


            std::time_t now = std::time(nullptr);
            std::tm* now_tm = std::localtime(&now);


            if (timeCmd == "SECONDS") {
                int seconds = now_tm->tm_sec;
                intVariables.at(varName) = seconds;
            }
            else if (timeCmd == "MINUTES") {
                int minutes = now_tm->tm_min;
                intVariables.at(varName) = minutes;
            }
            else if (timeCmd == "HOURS") {
                int hours = now_tm->tm_hour;
                intVariables.at(varName) = hours;
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to using time commands and have to using :int: variable when using TIME command for call variables. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }





        else if (lines[i][0] == 'C' && lines[i][1] == 'L' && lines[i][2] == 'A' && lines[i][3] == 'M' && lines[i][4] == 'P' && lines[i][5] == ' ') {
            if (lines[i][6] == '$' && lines[i][7] == ':' && lines[i][8] == 'd' && lines[i][9] == 'b' && lines[i][10] == 'l' && lines[i][11] == ':' && lines[i][line_i_size - 1] == '_') {
                std::string varName = "";
                std::string varNameMinValue1 = "";
                std::string varNameMaxValue1 = "";
                std::string varNameMinValue2 = "";
                std::string varNameMaxValue2 = "";
                int whereIsJ = 12;

                for (int j = 12; lines[i][j] != '_'; j++) {
                    varName += lines[i][j];
                    whereIsJ++;
                }
                whereIsJ++;
                if (lines[i][whereIsJ] != ' ' || lines[i][whereIsJ + 1] != '$' || lines[i][whereIsJ + 2] != ':' || lines[i][whereIsJ + 3] != 'd' || lines[i][whereIsJ + 4] != 'b' || lines[i][whereIsJ + 5] != 'l' || lines[i][whereIsJ + 6] != ':') {
                    std::cout << "\nERROR:\nmessage: you have to use double variable when using CLAMP command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
                whereIsJ += 7;


                for (int j = whereIsJ; lines[i][j] != '_'; j++) {
                    varNameMinValue1 += lines[i][j];
                    whereIsJ++;
                }
                whereIsJ++;
                if (lines[i][whereIsJ] != ' ' || lines[i][whereIsJ + 1] != '$' || lines[i][whereIsJ + 2] != ':' || lines[i][whereIsJ + 3] != 'd' || lines[i][whereIsJ + 4] != 'b' || lines[i][whereIsJ + 5] != 'l' || lines[i][whereIsJ + 6] != ':') {
                    std::cout << "\nERROR:\nmessage: you have to use double variable when using CLAMP command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
                whereIsJ += 7;


                for (int j = whereIsJ; lines[i][j] != '_'; j++) {
                    varNameMaxValue1 += lines[i][j];
                    whereIsJ++;
                }
                whereIsJ++;
                if (lines[i][whereIsJ] != ' ' || lines[i][whereIsJ + 1] != '$' || lines[i][whereIsJ + 2] != ':' || lines[i][whereIsJ + 3] != 'd' || lines[i][whereIsJ + 4] != 'b' || lines[i][whereIsJ + 5] != 'l' || lines[i][whereIsJ + 6] != ':') {
                    std::cout << "\nERROR:\nmessage: you have to use double variable when using CLAMP command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
                whereIsJ += 7;


                for (int j = whereIsJ; lines[i][j] != '_'; j++) {
                    varNameMinValue2 += lines[i][j];
                    whereIsJ++;
                }
                whereIsJ++;
                if (lines[i][whereIsJ] != ' ' || lines[i][whereIsJ + 1] != '$' || lines[i][whereIsJ + 2] != ':' || lines[i][whereIsJ + 3] != 'd' || lines[i][whereIsJ + 4] != 'b' || lines[i][whereIsJ + 5] != 'l' || lines[i][whereIsJ + 6] != ':') {
                    std::cout << "\nERROR:\nmessage: you have to use double variable when using CLAMP command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
                whereIsJ += 7;


                for (int j = whereIsJ; lines[i][j] != '_'; j++) {
                    varNameMaxValue2 += lines[i][j];
                    whereIsJ++;
                }


                double value = doubleVariables.at(varName);
                double a = doubleVariables.at(varNameMinValue1);
                double b = doubleVariables.at(varNameMaxValue1);
                double c = doubleVariables.at(varNameMinValue2);
                double d = doubleVariables.at(varNameMaxValue2);
                
                double result = 0.0;

                value = (value - a) / (b - a);
                result = c + value * (d - c);

                doubleVariables.at(varName) = result;

            }
            else {
                std::cout << "\nERROR:\nmessage: you have to use double variable when using CLAMP command. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }



        else if (lines[i][0] == 'A' && lines[i][1] == 'D' && lines[i][2] == 'D' && lines[i][3] == 'E' && lines[i][4] == 'S' && lines[i][5] == 'C' && lines[i][6] == 'A' && lines[i][7] == 'P' && lines[i][8] == 'E' && lines[i][9] == '\\' && lines[i][10] != ' ' && lines[i][11] == ' ') {
            if (lines[i][12] != '$' || lines[i][13] != ':' || lines[i][line_i_size - 1] != '_') {
                std::cout << "\nERROR:\nmessage: you have to using a :string: variable when using ADDESCAPE command. line:" << std::to_string(i + 1) << "\n";
                return;
            }
            
            std::string varName = "";
            for (int j = 18; j < line_i_size - 1; j++) {
                varName += lines[i][j];
            }

            if (lines[i][14] == 's' && lines[i][15] == 't' && lines[i][16] == 'r' && lines[i][17] == ':') {
                
                if (lines[i][10] == 'n') {
                    stringVariables.at(varName) += '\n';                    
                }
                else if (lines[i][10] == 't') {
                    stringVariables.at(varName) += '\t';
                }
                else if (lines[i][10] == 'r') {
                    stringVariables.at(varName) += '\r';
                }
                else if (lines[i][10] == 'v') {
                    stringVariables.at(varName) += '\v';
                }
                else if (lines[i][10] == '0') {
                    stringVariables.at(varName) += '\0';
                }
                else if (lines[i][10] == '\'') {
                    stringVariables.at(varName) += '\'';
                }
                else if (lines[i][10] == '"') {
                    stringVariables.at(varName) += '"';
                }
                else if (lines[i][10] == '\\') {
                    stringVariables.at(varName) += '\\';
                }
                else if (lines[i][10] == '\?') {
                    stringVariables.at(varName) += '\?';
                }
                else if (lines[i][10] == '\a') {
                    stringVariables.at(varName) += '\a';
                }
                else if (lines[i][10] == '\b') {
                    stringVariables.at(varName) += '\b';
                }
                else if (lines[i][10] == '\f') {
                    stringVariables.at(varName) += '\f';
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using just valid escape chars (\\n \\t \\r \\v \\0 \\\' \\\" \\\\ \\? \\a \\b \\f) parameters when using ADDESCAPE command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to using :string: variable when using ADDESCAPE command for call variables. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }




        else if (lines[i][0] == 'O' && lines[i][1] == 'P' && lines[i][2] == 'E' && lines[i][3] == 'N' && lines[i][4] == 'F' && lines[i][5] == 'I' && lines[i][6] == 'L' && lines[i][7] == 'E' && lines[i][8] == ' ') {
            if (lines[i][9] != '$' || lines[i][10] != ':'  || lines[i][11] != 's' || lines[i][12] != 't' || lines[i][13] != 'r' || lines[i][14] != ':' || lines[i][line_i_size - 1] != '_') {
                std::cout << "\nERROR:\nmessage: you have to using a :string: variable when using OPENFILE command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            std::string varName = "";

            for (int j = 15; lines[i][j] != '_'; j++)   {
                varName += lines[i][j];
            }

            try {
                stringVariables.at(varName);
            }
            catch(const std::exception& e) {
                std::cout << "\nERROR:\nmessage: undefined variable at OPENFILE command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            try {
                std::ofstream OPENFILEfile(stringVariables.at(varName).c_str());

                if (!OPENFILEfile) {
                    std::cout << "\nERROR:\nmessage: file can not be open. line:" << std::to_string(i + 1) << "\n";
                    return;
                }

                OPENFILEfile.close();
            }
            catch(const std::exception& e) {
                std::cout << "\nERROR:\nmessage: file can not be open. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }

        else if (lines[i][0] == 'D' && lines[i][1] == 'E' && lines[i][2] == 'L' && lines[i][3] == 'E' && lines[i][4] == 'T' && lines[i][5] == 'E' && lines[i][6] == 'F' && lines[i][7] == 'I' && lines[i][8] == 'L' && lines[i][9] == 'E' && lines[i][10] == ' ') {
            if (lines[i][11] != '$' || lines[i][12] != ':'  || lines[i][13] != 's' || lines[i][14] != 't' || lines[i][15] != 'r' || lines[i][16] != ':' || lines[i][line_i_size - 1] != '_') {
                std::cout << "\nERROR:\nmessage: you have to using a :string: variable when using DELETEFILE command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            std::string varName = "";

            for (int j = 17; lines[i][j] != '_'; j++)   {
                varName += lines[i][j];
            }

            try {
                stringVariables.at(varName);
            }
            catch(const std::exception& e) {
                std::cout << "\nERROR:\nmessage: undefined variable at DELETEFILE command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            try {
                if (std::remove(stringVariables.at(varName).c_str()) != 0) {
                    std::cout << "\nERROR:\nmessage: file can not be delete. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
            }
            catch(const std::exception& e) {
                std::cout << "\nERROR:\nmessage: file can not be delete. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }


        else if (lines[i][0] == 'R' && lines[i][1] == 'E' && lines[i][2] == 'A' && lines[i][3] == 'D' && lines[i][4] == 'F' && lines[i][5] == 'I' && lines[i][6] == 'L' && lines[i][7] == 'E' && lines[i][8] == ' ') {
            if (lines[i][9] != '$' || lines[i][10] != ':'  || lines[i][11] != 's' || lines[i][12] != 't' || lines[i][13] != 'r' || lines[i][14] != ':' || lines[i][line_i_size - 1] != '_') {
                std::cout << "\nERROR:\nmessage: you have to using a :string: variable when using READFILE command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            std::string firstVar = "";
            std::string secondVar = "";

            for (int j = 15; lines[i][j] != '_'; j++)   {
                firstVar += lines[i][j];
            }

            int secondVarStart = 15 + firstVar.size() + 1;
            
            if (lines[i][secondVarStart] != ' ' || lines[i][secondVarStart + 1] != '$' || lines[i][secondVarStart + 2] != ':' || lines[i][secondVarStart + 3] != 's' || lines[i][secondVarStart + 4] != 't' || lines[i][secondVarStart + 5] != 'r' || lines[i][secondVarStart + 6] != ':') {
                std::cout << "\nERROR:\nmessage: you have to using a :string: variable when using READFILE command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            secondVarStart += 7;

            for (int j = secondVarStart; lines[i][j] != '_'; j++)   {
                secondVar += lines[i][j];
            }

            try {
                stringVariables.at(firstVar);
                stringVariables.at(secondVar);
            }
            catch(const std::exception& e) {
                std::cout << "\nERROR:\nmessage: undefined variable at READFILE command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            try {
                std::string READFILEcommandtext = "";
                std::string READFILEcommandNewText = "";
                std::ifstream READFILEcommandfile(stringVariables.at(firstVar).c_str());
                if (!READFILEcommandfile) {
                    std::cout << "\nERROR:\nmessage: file does not exist. line:" << std::to_string(i + 1) << "\n";
                    return;
                }

                while (std::getline(READFILEcommandfile, READFILEcommandNewText)) {
                    READFILEcommandtext += READFILEcommandNewText + "\n";
                }                
                READFILEcommandfile.close();

                if (!READFILEcommandtext.empty() && READFILEcommandtext.back() == '\n') {
                    READFILEcommandtext.pop_back();
                }
                
                stringVariables.at(secondVar) = READFILEcommandtext;
            }
            catch(const std::exception& e) {
                std::cout << "\nERROR:\nmessage: file can not be read. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }



        else if (lines[i][0] == 'O' && lines[i][1] == 'V' && lines[i][2] == 'E' && lines[i][3] == 'R' && lines[i][4] == 'W' && lines[i][5] == 'R' && lines[i][6] == 'I' && lines[i][7] == 'T' && lines[i][8] == 'E' && lines[i][9] == 'F' && lines[i][10] == 'I' && lines[i][11] == 'L' && lines[i][12] == 'E' && lines[i][13] == ' ') {
            if (lines[i][14] != '$' || lines[i][15] != ':'  || lines[i][16] != 's' || lines[i][17] != 't' || lines[i][18] != 'r' || lines[i][19] != ':' || lines[i][line_i_size - 1] != '_') {
                std::cout << "\nERROR:\nmessage: you have to using a :string: variable when using OVERWRITE command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            std::string firstVar = "";
            std::string secondVar = "";

            for (int j = 20; lines[i][j] != '_'; j++)   {
                firstVar += lines[i][j];
            }

            int secondVarStart = 20 + firstVar.size() + 1;
            
            if (lines[i][secondVarStart] != ' ' || lines[i][secondVarStart + 1] != '$' || lines[i][secondVarStart + 2] != ':' || lines[i][secondVarStart + 3] != 's' || lines[i][secondVarStart + 4] != 't' || lines[i][secondVarStart + 5] != 'r' || lines[i][secondVarStart + 6] != ':') {
                std::cout << "\nERROR:\nmessage: you have to using a :string: variable when using OVERWRITEFILE command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            secondVarStart += 7;

            for (int j = secondVarStart; lines[i][j] != '_'; j++)   {
                secondVar += lines[i][j];
            }

            try {
                stringVariables.at(firstVar);
                stringVariables.at(secondVar);
            }
            catch(const std::exception& e) {
                std::cout << "\nERROR:\nmessage: undefined variable at OVERWRITEFILE command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            try {
                std::ofstream OVERWRITEFILEcommandfile(stringVariables.at(firstVar).c_str(), std::ios::trunc);

                if (!OVERWRITEFILEcommandfile.is_open()) {
                    std::cout << "\nERROR:\nmessage: file can not be create/open. line:" << std::to_string(i + 1) << "\n";
                    return;
                }

                OVERWRITEFILEcommandfile << stringVariables.at(secondVar);
                OVERWRITEFILEcommandfile.close();
            }
            catch(const std::exception& e) {
                std::cout << "\nERROR:\nmessage: file can not be create/open. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }


        else if (lines[i][0] == 'A' && lines[i][1] == 'P' && lines[i][2] == 'P' && lines[i][3] == 'E' && lines[i][4] == 'N' && lines[i][5] == 'D' && lines[i][6] == 'W' && lines[i][7] == 'R' && lines[i][8] == 'I' && lines[i][9] == 'T' && lines[i][10] == 'E' && lines[i][11] == 'F' && lines[i][12] == 'I' && lines[i][13] == 'L' && lines[i][14] == 'E' && lines[i][15] == ' ') {
            if (lines[i][16] != '$' || lines[i][17] != ':'  || lines[i][18] != 's' || lines[i][19] != 't' || lines[i][20] != 'r' || lines[i][21] != ':' || lines[i][line_i_size - 1] != '_') {
                std::cout << "\nERROR:\nmessage: you have to using a :string: variable when using APPENDWRITEFILE command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            std::string firstVar = "";
            std::string secondVar = "";

            for (int j = 22; lines[i][j] != '_'; j++)   {
                firstVar += lines[i][j];
            }

            int secondVarStart = 22 + firstVar.size() + 1;
            
            if (lines[i][secondVarStart] != ' ' || lines[i][secondVarStart + 1] != '$' || lines[i][secondVarStart + 2] != ':' || lines[i][secondVarStart + 3] != 's' || lines[i][secondVarStart + 4] != 't' || lines[i][secondVarStart + 5] != 'r' || lines[i][secondVarStart + 6] != ':') {
                std::cout << "\nERROR:\nmessage: you have to using a :string: variable when using APPENDWRITEFILE command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            secondVarStart += 7;

            for (int j = secondVarStart; lines[i][j] != '_'; j++)   {
                secondVar += lines[i][j];
            }

            try {
                stringVariables.at(firstVar);
                stringVariables.at(secondVar);
            }
            catch(const std::exception& e) {
                std::cout << "\nERROR:\nmessage: undefined variable at APPENDWRITEFILE command. line:" << std::to_string(i + 1) << "\n";
                return;
            }

            try {
                std::ofstream APPENDWRITEFILEcommandfile(stringVariables.at(firstVar).c_str(), std::ios::app);

                if (!APPENDWRITEFILEcommandfile.is_open()) {
                    std::cout << "\nERROR:\nmessage: file can not be create/open. line:" << std::to_string(i + 1) << "\n";
                    return;
                }

                APPENDWRITEFILEcommandfile << stringVariables.at(secondVar);
                APPENDWRITEFILEcommandfile.close();
            }
            catch(const std::exception& e) {
                std::cout << "\nERROR:\nmessage: file can not be create/open. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }




        else if (lines[i][0] == 'D' && lines[i][1] == 'E' && lines[i][2] == 'F' && lines[i][3] == 'E' && lines[i][4] == 'L') {
            if (lines[i][5] == '[' && lines[i][line_i_size - 1] == '_') {
                int count = 0;
                std::string countText = "";
                int controlIndex = 6;
                
                // 0: num | 1: variable
                int varOrNum = 0;

                if (lines[i][controlIndex] == '$' && lines[i][controlIndex + 1] == ':' && lines[i][controlIndex + 2] == 'i' && lines[i][controlIndex + 3] == 'n' && lines[i][controlIndex + 4] == 't' && lines[i][controlIndex + 5] == ':') {
                    varOrNum = 1;
                    controlIndex += 6;
                }
                else if (std::stoi(std::to_string(lines[i][controlIndex])) % 1 == 0) {
                    varOrNum = 0;
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to use integer number or integer variable at first parameter when using DEFEL command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
                
                for (int j = controlIndex; j < line_i_size; j++) {
                    if (lines[i][j] == ']') {
                        break;
                    }
                    else {
                        countText += lines[i][j];
                    }
                    controlIndex++;
                }

                if (varOrNum == 1) {
                    if (lines[i][controlIndex - 1] != '_' || countText.empty()) {
                        std::cout << "\nERROR:\nmessage: you have to use integer number or integer variable at first parameter when using DEFEL command. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                    else {
                        countText.pop_back();
                    }
                }

                if (varOrNum == 0) {
                    count = std::stoi(countText);
                }
                else if (varOrNum == 1) {
                    count = intVariables.at(countText);
                }

                // 0: str | 1: int | 2: dbl
                int varType = -1;

                if (lines[i][controlIndex + 1] == ' ' && lines[i][controlIndex + 2] == '$' && lines[i][controlIndex + 3] == ':' && lines[i][controlIndex + 7] == ':') {
                    if (lines[i][controlIndex + 4] == 's' && lines[i][controlIndex + 5] == 't' && lines[i][controlIndex + 6] == 'r') {
                        varType = 0;
                    }
                    else if (lines[i][controlIndex + 4] == 'i' && lines[i][controlIndex + 5] == 'n' && lines[i][controlIndex + 6] == 't') {
                        varType = 1;
                    }
                    else if (lines[i][controlIndex + 4] == 'd' && lines[i][controlIndex + 5] == 'b' && lines[i][controlIndex + 6] == 'l') {
                        varType = 2;
                    }
                    else {
                        std::cout << "\nERROR:\nmessage: you have to use variable when using DEFEL command's last parameter. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to use variable when using DEFEL command's last parameter. line:" << std::to_string(i + 1) << "\n";
                    return;
                }


                std::string secondParameter = "";
                
                controlIndex += 8;

                for (int j = controlIndex; lines[i][j] != '_'; j++) {
                    secondParameter += lines[i][j];
                }


                if (varType == 0) {
                    for (int j = 0; j < count; j++) {
                        try {
                            stringVariables.at(secondParameter + std::to_string(j));

                            std::cout << "\nERROR:\nmessage: variable name is already exist -> " + secondParameter + std::to_string(j) + " <- . line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        catch(const std::exception& e) {
                            stringVariables.insert(std::pair<std::string, std::string>(secondParameter + std::to_string(j), ""));
                        }
                    }
                }
                else if (varType == 1) {
                    for (int j = 0; j < count; j++) {
                        try {
                            intVariables.at(secondParameter + std::to_string(j));

                            std::cout << "\nERROR:\nmessage: variable name is already exist -> " + secondParameter + std::to_string(j) + " <- . line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        catch(const std::exception& e) {
                            intVariables.insert(std::pair<std::string, int>(secondParameter + std::to_string(j), 0));
                        }
                    }
                }
                else if (varType == 2) {
                    for (int j = 0; j < count; j++) {
                        try {
                            doubleVariables.at(secondParameter + std::to_string(j));

                            std::cout << "\nERROR:\nmessage: variable name is already exist -> " + secondParameter + std::to_string(j) + " <- . line:" << std::to_string(i + 1) << "\n";
                            return;
                        }
                        catch(const std::exception& e) {
                            doubleVariables.insert(std::pair<std::string, double>(secondParameter + std::to_string(j), 0.0));
                        }
                    }
                }
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to put '[' in 'DEFEL[<int>]' when using DEFEL command. or you may forgot '_' at the end of call index variable. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }


        else if (lines[i][0] == 'F' && lines[i][1] == 'R' && lines[i][2] == 'E' && lines[i][3] == 'E' && lines[i][4] == 'E' && lines[i][5] == 'L') {
            if (lines[i][6] == '[' && lines[i][line_i_size - 1] == '_') {
                int count = 0;
                std::string countText = "";
                int controlIndex = 7;
                
                // 0: num | 1: variable
                int varOrNum = 0;

                if (lines[i][controlIndex] == '$' && lines[i][controlIndex + 1] == ':' && lines[i][controlIndex + 2] == 'i' && lines[i][controlIndex + 3] == 'n' && lines[i][controlIndex + 4] == 't' && lines[i][controlIndex + 5] == ':') {
                    varOrNum = 1;
                    controlIndex += 6;
                }
                else if (std::stoi(std::to_string(lines[i][controlIndex])) % 1 == 0) {
                    varOrNum = 0;
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to use integer number or integer variable at first parameter when using FREEEL command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
                
                for (int j = controlIndex; j < line_i_size; j++) {
                    if (lines[i][j] == ']') {
                        break;
                    }
                    else {
                        countText += lines[i][j];
                    }
                    controlIndex++;
                }

                if (varOrNum == 1) {
                    if (lines[i][controlIndex - 1] != '_' || countText.empty()) {
                        std::cout << "\nERROR:\nmessage: you have to use integer number or integer variable at first parameter when using FREEEL command. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                    else {
                        countText.pop_back();
                    }
                }

                if (varOrNum == 0) {
                    count = std::stoi(countText);
                }
                else if (varOrNum == 1) {
                    count = intVariables.at(countText);
                }

                // 0: str | 1: int | 2: dbl
                int varType = -1;

                if (lines[i][controlIndex + 1] == ' ' && lines[i][controlIndex + 2] == '$' && lines[i][controlIndex + 3] == ':' && lines[i][controlIndex + 7] == ':') {
                    if (lines[i][controlIndex + 4] == 's' && lines[i][controlIndex + 5] == 't' && lines[i][controlIndex + 6] == 'r') {
                        varType = 0;
                    }
                    else if (lines[i][controlIndex + 4] == 'i' && lines[i][controlIndex + 5] == 'n' && lines[i][controlIndex + 6] == 't') {
                        varType = 1;
                    }
                    else if (lines[i][controlIndex + 4] == 'd' && lines[i][controlIndex + 5] == 'b' && lines[i][controlIndex + 6] == 'l') {
                        varType = 2;
                    }
                    else {
                        std::cout << "\nERROR:\nmessage: you have to use variable when using FREEEL command's last parameter. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to use variable when using FREEEL command's last parameter. line:" << std::to_string(i + 1) << "\n";
                    return;
                }


                std::string secondParameter = "";
                
                controlIndex += 8;

                for (int j = controlIndex; lines[i][j] != '_'; j++) {
                    secondParameter += lines[i][j];
                }


                if (varType == 0) {
                    stringVariables.erase(secondParameter + std::to_string(count));
                }
                else if (varType == 1) {
                    intVariables.erase(secondParameter + std::to_string(count));
                }
                else if (varType == 2) {
                    doubleVariables.erase(secondParameter + std::to_string(count));
                }
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to put '[' in 'FREEEL[<int>]' when using FREEEL command. or you may forgot '_' at the end of call index variable. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }


        else if (lines[i][0] == 'S' && lines[i][1] == 'E' && lines[i][2] == 'T' && lines[i][3] == 'E' && lines[i][4] == 'L') {
            if (lines[i][5] == '[' && lines[i][line_i_size - 1] == '_') { // ikinci koşul olmasının sebebi komutumuz _ ile bitmek zorunda ikinci parametre değişkendir ve _ ile biter.
                int index = 0;
                int afterIndexSpace = 0;
                
                if (lines[i][6] == '$' && lines[i][7] == ':' && lines[i][8] == 'i' && lines[i][9] == 'n' && lines[i][10] == 't' && lines[i][11] == ':') {
                    std::string indexVarName = "";
                    for (int j = 12; j < line_i_size; j++) {
                        if (lines[i][j] != '_') {
                            indexVarName += lines[i][j];
                        }
                        else {
                            if (lines[i][j + 1] != ']') {
                                std::cout << "\nERROR:\nmessage: you have to put ']' in 'SETEL[<int>]' when using SETEL command. or you may forgot '_' at the end of call index variable. line:" << std::to_string(i + 1) << "\n";
                                return;
                            }
                            afterIndexSpace = j + 2;
                            break;
                        }
                    }
                    index = intVariables.at(indexVarName);
                }
                else if (std::stoi(std::to_string(lines[i][6])) % 1 == 0) {
                    std::string indexIntValue = "";
                    bool isEnded = false;
                    for (int j = 6; j < line_i_size; j++) {
                        if (lines[i][j] == ']') {
                            isEnded = true;
                            afterIndexSpace = j + 1;
                            break;
                        }
                        else {
                            indexIntValue += lines[i][j];
                        }
                    }
                    if (!isEnded) {
                        std::cout << "\nERROR:\nmessage: you have to put ']' in 'SETEL[<int>]' when using SETEL command. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                    index = std::stoi(indexIntValue);
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to use pure integer number or int variable when using SETEL command's index. line:" << std::to_string(i + 1) << "\n";
                    return;
                }


        
                if (lines[i][afterIndexSpace] != ' ' || lines[i][afterIndexSpace + 1] != '$' || lines[i][afterIndexSpace + 2] != ':') {
                    std::cout << "\nERROR:\nmessage: you have to using variables except index when using SETEL command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
                

                // 0: str | 1: int | 2: dbl
                int typeOfVars = -1;

                if (lines[i][afterIndexSpace + 3] == 's' && lines[i][afterIndexSpace + 4] == 't' && lines[i][afterIndexSpace + 5] == 'r' && lines[i][afterIndexSpace + 6] == ':') {
                    typeOfVars = 0;
                }
                else if (lines[i][afterIndexSpace + 3] == 'i' && lines[i][afterIndexSpace + 4] == 'n' && lines[i][afterIndexSpace + 5] == 't' && lines[i][afterIndexSpace + 6] == ':') {
                    typeOfVars = 1;
                }
                else if (lines[i][afterIndexSpace + 3] == 'd' && lines[i][afterIndexSpace + 4] == 'b' && lines[i][afterIndexSpace + 5] == 'l' && lines[i][afterIndexSpace + 6] == ':') {
                    typeOfVars = 2;
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using variables except index when using SETEL command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }

                afterIndexSpace += 7;


                std::string firstVarName = "";
                std::string secondVarName = "";
                
                for (int j = afterIndexSpace; lines[i][j] != '_'; j++) {
                    firstVarName += lines[i][j];
                    afterIndexSpace++;
                }


                if (typeOfVars == 0) {
                    if (lines[i][afterIndexSpace] != '_' || lines[i][afterIndexSpace + 1] != ' ' || lines[i][afterIndexSpace + 2] != '$' || lines[i][afterIndexSpace + 3] != ':' || lines[i][afterIndexSpace + 4] != 's' || lines[i][afterIndexSpace + 5] != 't' || lines[i][afterIndexSpace + 6] != 'r' || lines[i][afterIndexSpace + 7] != ':') {
                        std::cout << "\nERROR:\nmessage: you have to using same variables types when using SETEL command. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                }
                else if (typeOfVars == 1) {
                    if (lines[i][afterIndexSpace] != '_' || lines[i][afterIndexSpace + 1] != ' ' || lines[i][afterIndexSpace + 2] != '$' || lines[i][afterIndexSpace + 3] != ':' || lines[i][afterIndexSpace + 4] != 'i' || lines[i][afterIndexSpace + 5] != 'n' || lines[i][afterIndexSpace + 6] != 't' || lines[i][afterIndexSpace + 7] != ':') {
                        std::cout << "\nERROR:\nmessage: you have to using same variables types when using SETEL command. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                }
                else if (typeOfVars == 2) {
                    if (lines[i][afterIndexSpace] != '_' || lines[i][afterIndexSpace + 1] != ' ' || lines[i][afterIndexSpace + 2] != '$' || lines[i][afterIndexSpace + 3] != ':' || lines[i][afterIndexSpace + 4] != 'd' || lines[i][afterIndexSpace + 5] != 'b' || lines[i][afterIndexSpace + 6] != 'l' || lines[i][afterIndexSpace + 7] != ':') {
                        std::cout << "\nERROR:\nmessage: you have to using same variables types when using SETEL command. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                }

                afterIndexSpace += 8;

                for (int j = afterIndexSpace; lines[i][j] != '_'; j++) {
                    secondVarName += lines[i][j];
                }


                // ilk parametrenin indexinci değeri = ikinci parametre
                if (typeOfVars == 0) {
                    stringVariables.at(firstVarName + std::to_string(index)) = stringVariables.at(secondVarName);
                }
                else if (typeOfVars == 1) {
                    intVariables.at(firstVarName + std::to_string(index)) = intVariables.at(secondVarName);
                }
                else if (typeOfVars == 2) {
                    doubleVariables.at(firstVarName + std::to_string(index)) = doubleVariables.at(secondVarName);
                }
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to put '[' in 'SETEL[<int>]' when using SETEL command. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }


        else if (lines[i][0] == 'G' && lines[i][1] == 'E' && lines[i][2] == 'T' && lines[i][3] == 'E' && lines[i][4] == 'L') {
            if (lines[i][5] == '[' && lines[i][line_i_size - 1] == '_') { // ikinci koşul olmasının sebebi komutumuz _ ile bitmek zorunda ikinci parametre değişkendir ve _ ile biter.
                int index = 0;
                int afterIndexSpace = 0;
                
                if (lines[i][6] == '$' && lines[i][7] == ':' && lines[i][8] == 'i' && lines[i][9] == 'n' && lines[i][10] == 't' && lines[i][11] == ':') {
                    std::string indexVarName = "";
                    for (int j = 12; j < line_i_size; j++) {
                        if (lines[i][j] != '_') {
                            indexVarName += lines[i][j];
                        }
                        else {
                            if (lines[i][j + 1] != ']') {
                                std::cout << "\nERROR:\nmessage: you have to put ']' in 'GETEL[<int>]' when using GETEL command. or you may forgot '_' at the end of call index variable. line:" << std::to_string(i + 1) << "\n";
                                return;
                            }
                            afterIndexSpace = j + 2;
                            break;
                        }
                    }
                    index = intVariables.at(indexVarName);
                }
                else if (std::stoi(std::to_string(lines[i][6])) % 1 == 0) {
                    std::string indexIntValue = "";
                    bool isEnded = false;
                    for (int j = 6; j < line_i_size; j++) {
                        if (lines[i][j] == ']') {
                            isEnded = true;
                            afterIndexSpace = j + 1;
                            break;
                        }
                        else {
                            indexIntValue += lines[i][j];
                        }
                    }
                    if (!isEnded) {
                        std::cout << "\nERROR:\nmessage: you have to put ']' in 'GETEL[<int>]' when using GETEL command. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                    index = std::stoi(indexIntValue);
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to use pure integer number or int variable when using GETEL command's index. line:" << std::to_string(i + 1) << "\n";
                    return;
                }


        
                if (lines[i][afterIndexSpace] != ' ' || lines[i][afterIndexSpace + 1] != '$' || lines[i][afterIndexSpace + 2] != ':') {
                    std::cout << "\nERROR:\nmessage: you have to using variables except index when using GETEL command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }
                

                // 0: str | 1: int | 2: dbl
                int typeOfVars = -1;

                if (lines[i][afterIndexSpace + 3] == 's' && lines[i][afterIndexSpace + 4] == 't' && lines[i][afterIndexSpace + 5] == 'r' && lines[i][afterIndexSpace + 6] == ':') {
                    typeOfVars = 0;
                }
                else if (lines[i][afterIndexSpace + 3] == 'i' && lines[i][afterIndexSpace + 4] == 'n' && lines[i][afterIndexSpace + 5] == 't' && lines[i][afterIndexSpace + 6] == ':') {
                    typeOfVars = 1;
                }
                else if (lines[i][afterIndexSpace + 3] == 'd' && lines[i][afterIndexSpace + 4] == 'b' && lines[i][afterIndexSpace + 5] == 'l' && lines[i][afterIndexSpace + 6] == ':') {
                    typeOfVars = 2;
                }
                else {
                    std::cout << "\nERROR:\nmessage: you have to using variables except index when using GETEL command. line:" << std::to_string(i + 1) << "\n";
                    return;
                }

                afterIndexSpace += 7;


                std::string firstVarName = "";
                std::string secondVarName = "";
                
                for (int j = afterIndexSpace; lines[i][j] != '_'; j++) {
                    firstVarName += lines[i][j];
                    afterIndexSpace++;
                }
                

                if (typeOfVars == 0) {
                    if (lines[i][afterIndexSpace] != '_' || lines[i][afterIndexSpace + 1] != ' ' || lines[i][afterIndexSpace + 2] != '$' || lines[i][afterIndexSpace + 3] != ':' || lines[i][afterIndexSpace + 4] != 's' || lines[i][afterIndexSpace + 5] != 't' || lines[i][afterIndexSpace + 6] != 'r' || lines[i][afterIndexSpace + 7] != ':') {
                        std::cout << "\nERROR:\nmessage: you have to using same variables types when using GETEL command. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                }
                else if (typeOfVars == 1) {
                    if (lines[i][afterIndexSpace] != '_' || lines[i][afterIndexSpace + 1] != ' ' || lines[i][afterIndexSpace + 2] != '$' || lines[i][afterIndexSpace + 3] != ':' || lines[i][afterIndexSpace + 4] != 'i' || lines[i][afterIndexSpace + 5] != 'n' || lines[i][afterIndexSpace + 6] != 't' || lines[i][afterIndexSpace + 7] != ':') {
                        std::cout << "\nERROR:\nmessage: you have to using same variables types when using GETEL command. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                }
                else if (typeOfVars == 2) {
                    if (lines[i][afterIndexSpace] != '_' || lines[i][afterIndexSpace + 1] != ' ' || lines[i][afterIndexSpace + 2] != '$' || lines[i][afterIndexSpace + 3] != ':' || lines[i][afterIndexSpace + 4] != 'd' || lines[i][afterIndexSpace + 5] != 'b' || lines[i][afterIndexSpace + 6] != 'l' || lines[i][afterIndexSpace + 7] != ':') {
                        std::cout << "\nERROR:\nmessage: you have to using same variables types when using GETEL command. line:" << std::to_string(i + 1) << "\n";
                        return;
                    }
                }

                afterIndexSpace += 8;

                for (int j = afterIndexSpace; lines[i][j] != '_'; j++) {
                    secondVarName += lines[i][j];
                }


                // ikinci parametre = ilk parametrenin indexinci değeri
                if (typeOfVars == 0) {
                    stringVariables.at(secondVarName) = stringVariables.at(firstVarName + std::to_string(index));
                }
                else if (typeOfVars == 1) {
                    intVariables.at(secondVarName) = intVariables.at(firstVarName + std::to_string(index));
                }
                else if (typeOfVars == 2) {
                    doubleVariables.at(secondVarName) = doubleVariables.at(firstVarName + std::to_string(index));
                }
            }
            else {
                std::cout << "\nERROR:\nmessage: you have to put '[' in 'GETEL[<int>]' when using GETEL command. line:" << std::to_string(i + 1) << "\n";
                return;
            }
        }








        
        // under devolopment
        else
        {
            std::cout << "ERROR!\nmessage: wrong command. line:" << std::to_string(i + 1) << "\n";
            return;
        }
    }
}