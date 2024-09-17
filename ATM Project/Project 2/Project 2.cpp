#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iomanip>

using namespace std;


struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

enum enATMMainMenueOptions {
    eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3,
    eCheckBalance = 4, eLogout = 5
};

enum enQuickWithdrow {
    e20 = 1, e50 = 2, e100 = 3, e200 = 4,
    e400 = 5, e600 = 6, e800 = 7, e1000 = 8,
    eQuickLogout = 9
};

//enum enQuickWithdrow {
//    e20 = 1, e50 = 2, e100 = 3, e200 = 4,
//    e400 = 5, e600 = 6, e800 = 7, e1000 = 8,
//    eQuickLogout = 9  // Renamed from eLogout to eQuickLogout
//};

const string ClientsFileName = "Clients.txt";
sClient CurrentClient;

void Login();
void ShowATMMainMenue();


vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLineToRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}

vector <sClient> LoadClientsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLineToRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;

}

bool FindUserByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient& Client)
{

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }

    }
    return false;

}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient>& vClients, sClient ClientCurrent)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient& C : vClients)
        {

            if (C.AccountNumber == ClientCurrent.AccountNumber)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(ClientCurrent);
                MyFile << DataLine << endl;
            }
            else
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }

        MyFile.close();

    }

    return vClients;

}

void GoBackToATMMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowATMMainMenue();

}

short ReadQuickWithdrawChoose()
{
    short Choice = 0;

    while (Choice < 1 || Choice > 9)
    {
        cout << "\nChoose what to do withdraw from [1] to [9] ? ";
        cin >> Choice;
    }

    return Choice;
}

void ChoocePossibleAndWithdraw(short NumChooseToWithdraw)
{
    char WithdrawOrNo = 'Y';

    if (CurrentClient.AccountBalance >= NumChooseToWithdraw)
    {
        sClient Client;
        vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
        string AccountNumber = CurrentClient.AccountNumber;

        cout << "\nAre you sure want perform this transaction? y/n ? ";
        cin >> WithdrawOrNo;

        if (tolower(WithdrawOrNo) == 'y')
        {

            CurrentClient.AccountBalance -= NumChooseToWithdraw;
            SaveCleintsDataToFile(ClientsFileName, vClients, CurrentClient);
            cout << "Done Successfully. New Balance is: " << CurrentClient.AccountBalance;
        }
        else
            cout << "Ok, I Am NOT Withdraw\n";
    }
    else
        cout << "\nThe amount exceeds your balance, make another choice\n";

}

void PerformQuickWithdrawOption(enQuickWithdrow WithdrawChoose)
{
    switch (WithdrawChoose)
    {
    case enQuickWithdrow::e20:
        ChoocePossibleAndWithdraw(20);
        GoBackToATMMainMenue();
        break;
    case enQuickWithdrow::e50:
        ChoocePossibleAndWithdraw(50);
        GoBackToATMMainMenue();
        break;
    case enQuickWithdrow::e100:
        ChoocePossibleAndWithdraw(100);
        GoBackToATMMainMenue();
        break;
    case enQuickWithdrow::e200:
        ChoocePossibleAndWithdraw(200);
        GoBackToATMMainMenue();
        break;
    case enQuickWithdrow::e400:
        ChoocePossibleAndWithdraw(400);
        GoBackToATMMainMenue();
        break;
    case enQuickWithdrow::e600:
        ChoocePossibleAndWithdraw(600);
        GoBackToATMMainMenue();
        break;
    case enQuickWithdrow::e800:
        ChoocePossibleAndWithdraw(800);
        GoBackToATMMainMenue();
        break;
    case enQuickWithdrow::e1000:
        ChoocePossibleAndWithdraw(1000);
        GoBackToATMMainMenue();
        break;
    case enQuickWithdrow::eQuickLogout:

        GoBackToATMMainMenue();
    }
}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";


    sClient Client;

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = CurrentClient.AccountNumber;


    double Amount = 0;
    cout << "\nEnter a positif Deposit Amount? ";
    cin >> Amount;

    char WithdrawOrNo = 'Y';
    cout << "\nAre you sure want perform this transaction? y/n ? ";
    cin >> WithdrawOrNo;

    if (tolower(WithdrawOrNo) == 'y')
    {

        CurrentClient.AccountBalance += Amount;
        SaveCleintsDataToFile(ClientsFileName, vClients, CurrentClient);
        cout << "Done Successfully. New Balance is: " << CurrentClient.AccountBalance;
    }
    else
        cout << "Ok, I Am NOT Deposif This Amount\n";

}

void ShowNormalWithDrawScreen()
{

    cout << "\n-----------------------------------\n";
    cout << "\tNormal Withdraw Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = CurrentClient.AccountNumber;

    int Amount = 0;
    char WithdrawOrNo = 'Y';

    do
    {
        cout << "\nEnter an amount multiple of 5's ? ";
        cin >> Amount;

    } while (Amount <= 0 || Amount % 5 != 0);

    //Validate that the amount does not exceeds the balance
    while (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Enter an amount multiple of 5's ? ";
        cin >> Amount;
    }


    cout << "\nAre you sure want perform this transaction? y/n ? ";
    cin >> WithdrawOrNo;

    if (tolower(WithdrawOrNo) == 'y')
    {

        CurrentClient.AccountBalance -= Amount;
        SaveCleintsDataToFile(ClientsFileName, vClients, CurrentClient);
        cout << "Done Successfully. New Balance is: " << CurrentClient.AccountBalance;
    }
    else
        cout << "Ok, I Am NOT Withdraw\n";
}

void ShowQuickWithdraw()
{
    cout << "===========================================\n";
    cout << "\t\tQuick withdaw\n";
    cout << "===========================================\n";
    cout << "\t[1] 20\t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "===========================================\n";
    cout << "Your Balance is : " << CurrentClient.AccountBalance << endl;

    PerformQuickWithdrawOption((enQuickWithdrow)ReadQuickWithdrawChoose());
}

void ShowCheckBalance()
{
    cout << "===========================================\n";
    cout << "\t\tDeposit Screen\n";
    cout << "===========================================\n";
    cout << "Your Balance is : " << CurrentClient.AccountBalance << endl;

}

short ReadATMMainMenueOptions()
{
    cout << "Choose what do you want to do? [1 to 5]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromATMMainMenueOptions(enATMMainMenueOptions ATMMainMenueOptions)
{
    switch (ATMMainMenueOptions)
    {
    case enATMMainMenueOptions::eQuickWithdraw:
    {
        system("cls");
        ShowQuickWithdraw();
        GoBackToATMMainMenue();
        break;
    }

    case enATMMainMenueOptions::eNormalWithdraw:
    {
        system("cls");
        ShowNormalWithDrawScreen();
        GoBackToATMMainMenue();
        break;
    }
    case enATMMainMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToATMMainMenue();
        break;
    }


    case enATMMainMenueOptions::eCheckBalance:
    {
        system("cls");
        ShowCheckBalance();
        GoBackToATMMainMenue();

    }
    case enATMMainMenueOptions::eLogout:
    {
        system("cls");
        Login();
    }
    }

}

//PerformQuickWithdrawOption

void ShowATMMainMenue()
{

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";
    PerfromATMMainMenueOptions((enATMMainMenueOptions)ReadATMMainMenueOptions());
}
// 
bool  LoadClientInfo(string AccountNumber, string PinCode)
{

    if (FindUserByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;

}

void Login()
{
    bool LoginFailed = false;

    string AccountNumber, PinCode;
    do
    {
        system("cls");

        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (LoginFailed)
        {
            cout << "Invlaid Account Number/PinCode!\n";
        }

        cout << "Enter Account Number? ";
        cin >> AccountNumber;

        cout << "Enter PinCode? ";
        cin >> PinCode;

        LoginFailed = !LoadClientInfo(AccountNumber, PinCode);

    } while (LoginFailed);

    ShowATMMainMenue();

}


int main()
{
    Login();

    system("pause>0");
    return 0;
}


