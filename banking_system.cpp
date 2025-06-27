#include<fstream>
#include<iostream>
#include<cstdlib>
#include<vector>
#include<map>

using namespace std;
#define min_balance 500

class InsufficientFunds{};


class Account{
    
private:
    long AccountNumber;
    string firstName;
    string lastname;
    float balance;
    static long NextAccountNumber;
    
public:
    Account() {}
    Account(string fname, string lname, float balance);
    long getAccNo(){return AccountNumber;}
    string getfirstName(){return firstName;}
    string getlastname(){return lastname;}
    float getBalance(){return balance;}
    
    void deposit(float amount);
    void withdraw(float amount);
    static void setlastAccountNumber(long accountNumber);
    static long getlastAccountNumber();
    
    friend ofstream & operator<<(ofstream &ofs,Account &acc);
    friend ifstream & operator>>(ifstream &ifs,Account &acc);
    friend ostream & operator<<(ostream &os,Account &acc);
    

};

long  Account::NextAccountNumber=0;

class bank{
private:
    map<long,Account>accounts;
    
public:
    bank();
    Account openAccount(string fname,string lname,float balance );
    Account BalanceEnquiry(long accountNumber);
    Account Deposit(long accountNumber, float amount);
    Account Withdraw(long accountNumber, float amount);
    
    void closeAccount(long accountNumber);
    void showAllAccounts();
    ~bank();
    
};

Account::Account(string fname,string lname,float balance)
{
    NextAccountNumber++;
    AccountNumber = NextAccountNumber;
    firstName = fname;
    lastname = lname;
    this->balance = balance;
}

void Account::deposit(float amount)
{
    balance+=amount;
}

void Account::withdraw(float amount)
{
    if(balance-amount<min_balance)
        throw InsufficientFunds();
    balance-=amount;
}

void Account::setlastAccountNumber(long accountNumber)
{
    NextAccountNumber = accountNumber;
}

long Account::getlastAccountNumber()
{
    return NextAccountNumber;
}

ofstream & operator<<(ofstream &ofs,Account &acc)
{
    ofs<<acc.AccountNumber<<endl;
    ofs<<acc.firstName<<endl;
    ofs<<acc.lastname<<endl;
    ofs<<acc.balance<<endl;
    return ofs;
}

ifstream & operator>>(ifstream &ifs,Account &acc)
{
    ifs>>acc.AccountNumber;
    ifs>>acc.firstName;
    ifs>>acc.lastname;
    ifs>>acc.balance;
    return ifs;
}

ostream & operator<<(ostream &os,Account &acc)
{
    os<<"First Name:"<<acc.getfirstName()<<endl;
    os<<"Last Name:"<<acc.getlastname()<<endl;
    os<<"Account Number:"<<acc.getAccNo()<<endl;
    os<<"Balance:"<<acc.getBalance()<<endl;
    return os;
}

bank::bank()
{
    Account account;
    ifstream infile;
    infile.open("Bank.data");
    if(!infile)
    {
    //Error in Opening! File Not Found!!
    return;
    }
    while(!infile.eof())
    {
    infile>>account;
    accounts.insert(pair<long,Account>(account.getAccNo(),account));
    }
    Account::setlastAccountNumber(account.getAccNo());
    infile.close();
}

Account bank::openAccount(string fname,string lname,float balance)
{
    ofstream outfile;
    Account account(fname,lname,balance);
    accounts.insert(pair<long,Account>(account.getAccNo(),account));
    
    
    outfile.open("Bank.data", ios::trunc);
    
    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++)
    {
        outfile<<itr->second;
    }
    outfile.close();
    return account;
}

Account bank::BalanceEnquiry(long accountNumber)
{
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    return itr->second;
}

Account bank::Deposit(long accountNumber,float amount)
{
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    itr->second.deposit(amount);
    return itr->second;
}

Account bank::Withdraw(long accountNumber,float amount)
{
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    itr->second.withdraw(amount);
    return itr->second;
}

void bank::closeAccount(long accountNumber)
{
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    cout<<"Account Deleted"<<itr->second;
    accounts.erase(accountNumber);
}

void bank::showAllAccounts()
{
    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++)
    {
        cout<<"Account "<<itr->first<<endl<<itr->second<<endl;
    }
}

bank::~bank()
{
    ofstream outfile;
    outfile.open("Bank.data", ios::trunc);
    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++)
    {
    outfile<<itr->second;
    }
    outfile.close();
}


int main(){
    bank b;
    Account acc;
    
    int choice;
    string fname, lname;
    long accountNumber;
    float balance;
    float amount;
    
    cout<<"***Banking System***"<<endl;
    
    do{
        cout<<"\n Select One Option Below";
        cout<<"\n\t1 Open an Account";
        cout<<"\n\t2 Balance Enquiry";
        cout<<"\n\t3 Deposit";
        cout<<"\n\t4 Withdrawl";
        cout<<"\n\t5 Close an Account";
        cout<<"\n\t6 Quit";
        
        cout<<"\n Enter Your Choice:";
        cin>>choice;
        
        switch(choice)
        {
            case 1:
                cout<<"Enter First Name:";
                cin>>fname;
                cout<<"Enter Last Name:";
                cin>>lname;
                cout<<"Enter Initial Balance:";
                cin>>balance;
                
                acc=b.openAccount(fname, lname, balance);
                cout<<endl<<"Account is Created"<<endl;
                cout<<acc;
                break;
                
            case 2:
                cout<<"Enter Account Number:";
                cin>>accountNumber;
                
                acc=b.BalanceEnquiry(accountNumber);
                cout<<endl<<"Your Account Details"<<endl;
                cout<<acc;
                break;
                
            case 3:
                cout<<"Enter Account Number:";
                cin>>accountNumber;
                cout<<"Enter Amount to be Deposit:";
                cin>>amount;
                
                acc=b.Deposit(accountNumber, amount);
                cout<<endl<<"Amount is Deposited"<<endl;
                cout<<acc;
                break;
                
            case 4:
                cout<<"Enter Account Number:";
                cin>>accountNumber;
                cout<<"Enter balance:";
                cin>>amount;
                
                acc=b.Withdraw(accountNumber,amount);
                cout<<endl<<"Amount Withdrawn"<<endl;
                cout<<acc;
                break;
                
            case 5:
                cout<<"Enter Account Number:";
                cin>>accountNumber;
                
                b.closeAccount(accountNumber);
                cout<<endl<<"Accout is Deleted"<<endl;
                cout<<acc;
                
            case 6:
                b.showAllAccounts();
                break;
                
            case 7:
                break;
                
            default:
                cout<<"\nEnter  Correct Choice";
                exit(0);
      
        }
    }
    while(choice!=7);
        return 0;
    
}
