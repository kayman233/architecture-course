#include "Account.h"
#include "Bank.h"
#include "Client.h"
#include <iostream>
#include <set>
#include <string>
#include <stdio.h>
#include <stdlib.h>

struct UserBankInfo {
    Bank* bank;
    uint32_t clientId;
};

void print_menu() {
    printf("Что вы хотите сделать?\n");
    printf("1. Завести счет\n");
    printf("2. Пополнить счет\n");
    printf("3. Снять деньги\n");
    printf("4. Перевести деньги\n");
    printf("5. Дополнить свои данные\n");
    printf("6. Показать банки\n");
    printf("7. Показать мои счета\n");
    printf("8. Выйти\n");
    printf(">");
}

int get_variant(int count) {
    int variant;
    char s[100]; // строка для считывания введённых данных
    scanf("%s", s); // считываем строку

    while (sscanf(s, "%d", &variant) != 1 || variant < 1 || variant > count) {
        printf("Неверный ввод, повторите: "); // выводим сообщение об ошибке
        scanf("%s", s); // считываем строку повторно
    }

    return variant;
}

void print_banks(std::vector<Bank*> banks) {
    printf("+---------------------+---------------------+---------------+---------------+\n");
    printf("|         Банк        |       Swift код     |    %% Депозит  |   %% Комиссия  |\n");
    printf("+---------------------+---------------------+---------------+---------------+\n");

    for(Bank* bank: banks) {
        printf("| %19s | %19s | %12d  | %12d  |\n", bank->getName().c_str(), bank->getSwiftCode().c_str(),
               bank->getConfig().depositPercent, bank->getConfig().commision);
    }

    if (banks.empty())
        printf("|            Нет банков...          |\n");

    printf("+---------------------+---------------------+---------------+---------------+\n");
}

void add_account(int account_variant, Bank* bank,uint32_t client_id) {
    uint32_t account_id = 0;
    switch (account_variant) {
        case 1:
            account_id = bank->openDebit(client_id);
            break;

        case 2:
            account_id = bank->openDeposit(client_id);
            break;

        case 3:
            account_id = bank->openCredit(client_id);
            break;
        default:
            break;
    }

    printf("Успешно! ID счета: %d\n", account_id);
}

void add_client(std::vector<Bank*> banks, std::vector<UserBankInfo>& userBankInfo) {
    printf("В каком банке хотите завести счет?\n");
    int i = 1;
    for(Bank* bank: banks) {
        printf("%d. %s\n", i, bank->getName().c_str());
        ++i;
    }

    printf("%d. Отмена\n", i);
    printf(">");

    int bank_variant = get_variant(banks.size() + 1);

    Bank* bank = banks[bank_variant - 1];

    if (bank_variant == banks.size() + 1) {
        return;
    }

    printf("Какой счет вы хотите открыть?\n");
    printf("1. Дебетовый\n");
    printf("2. Депозит\n");
    printf("3. Кредитный\n");
    printf("4. Отмена\n");
    printf(">");

    int account_variant = get_variant(4);

    if (account_variant == 4) {
        return;
    }

    for(UserBankInfo info: userBankInfo) {
        if (info.bank->getSwiftCode() == bank->getSwiftCode()) {
            Client client = bank->client(info.clientId);
            printf("Уже авторизованы, %s %s\n", client.firstName().c_str(), client.lastName().c_str());
            add_account(account_variant, bank, info.clientId);
            if (!client.isSus()) {
                bank->checkClient(info.clientId);
            }
            return;
        }
    }

    char name[100];
    char surname[100];

    printf("Введите имя: ");
    scanf("%s", name);
    printf("Введите фамилию: ");
    scanf("%s", surname);

    Client client(name, surname);

    uint32_t client_id = bank->registerClient(client);

    add_account(account_variant, bank, client_id);
    UserBankInfo userInfo;
    userInfo.clientId = client_id;
    userInfo.bank = bank;
    userBankInfo.push_back(userInfo);

    printf("Хотите дополнить свою информацию для увеличения лимита?\n");
    printf("1. Нет\n");
    printf("2. Да\n");
    printf(">");

    int more_info_variant = get_variant(2);

    if (more_info_variant == 1 ) {
        return;
    }

    char address[100];
    char passport[100];

    printf("Введите адресс: ");
    scanf("%s", address);
    printf("Введите паспортные данные: ");
    scanf("%s", passport);

    bank->client(client_id).setAddress(address);
    bank->client(client_id).setPassportId(passport);
    bank->checkClient(client_id);
}

bool isEmptyUserInfo(std::vector<UserBankInfo>& userBankInfo) {
    if (userBankInfo.empty()) {
        printf("\n");
        printf("У вас нет счетов...\n");
        printf("\n");
        return true;
    }

    return false;
}

void showUserInfo(std::vector<UserBankInfo>& userBankInfo) {
    if (isEmptyUserInfo(userBankInfo)) {
        return;
    }
    for(UserBankInfo info: userBankInfo) {
        printf("Счета в банке \"%s\":\n", info.bank->getName().c_str());
        printf("  id  Sum  Usage   Type\n");
        for (const auto &account : info.bank->getAccounts()) {
            if (account->getClientId() == info.clientId) {
                printf("  %d  %lld$  %lld$  %s\n", account->getId(), account->getBalance(), account->getCurrentUsage(), account->getType().c_str());
            }
        }
    }
}

void add_to_account(std::vector<UserBankInfo>& userBankInfo) {
    if (isEmptyUserInfo(userBankInfo)) {
        return;
    }
    std::vector<Bank*> banks;
    banks.reserve(userBankInfo.size());
    for(UserBankInfo info: userBankInfo) {
        banks.push_back(info.bank);
    }
    printf("В каком банке хотите пополнить счет?\n");
    int i = 1;
    for(Bank* bank: banks) {
        printf("%d. %s\n", i, bank->getName().c_str());
        ++i;
    }

    printf("%d. Отмена\n", i);
    printf(">");

    int bank_variant = get_variant(banks.size() + 1);

    if (bank_variant == banks.size() + 1) {
        return;
    }

    Bank* bank = banks[bank_variant - 1];
    uint32_t clientId = userBankInfo[bank_variant - 1].clientId;
    printf("Выберите счет\n");
    i = 1;
    std::vector<uint32_t> accounts;
    for (const auto &account : bank->getAccounts()) {
        if (account->getClientId() == clientId) {
            accounts.push_back(account->getId());
            printf("%d.  %d\t%lld$\n", i, account->getId(), account->getBalance());
            ++i;
        }
    }

    printf("%d. Отмена\n", i);
    printf(">");

    int account_variant = get_variant(i);

    if (account_variant == i) {
        return;
    }

    printf("Введите сумму(для отмены введите -1): ");

    int sum;
    char s[100]; // строка для считывания введённых данных
    scanf("%s", s); // считываем строку

    while (sscanf(s, "%d", &sum) != 1 || bank->getAccounts()[accounts[account_variant - 1] - 1]->topUpAccount(sum) == 0) {
        if (sum == -1) {
            return;
        }
        printf("Неверный ввод, повторите: "); // выводим сообщение об ошибке
        scanf("%s", s); // считываем строку повторно
    }

    printf("Успешно");
}

void get_cash(std::vector<UserBankInfo>& userBankInfo) {
    if (isEmptyUserInfo(userBankInfo)) {
        return;
    }
    std::vector<Bank*> banks;
    banks.reserve(userBankInfo.size());
    for(UserBankInfo info: userBankInfo) {
        banks.push_back(info.bank);
    }
    printf("В каком банке хотите снять деньги?\n");
    int i = 1;
    for(Bank* bank: banks) {
        printf("%d. %s\n", i, bank->getName().c_str());
        ++i;
    }

    printf("%d. Отмена\n", i);
    printf(">");

    int bank_variant = get_variant(banks.size() + 1);

    if (bank_variant == banks.size() + 1) {
        return;
    }

    Bank* bank = banks[bank_variant - 1];
    uint32_t clientId = userBankInfo[bank_variant - 1].clientId;
    printf("Выберите счет\n");
    i = 1;
    std::vector<uint32_t> accounts;
    for (const auto &account : bank->getAccounts()) {
        if (account->getClientId() == clientId) {
            accounts.push_back(account->getId());
            printf("%d.  %d\t%lld$\n", i, account->getId(), account->getBalance());
            ++i;
        }
    }

    printf("%d. Отмена\n", i);
    printf(">");

    int account_variant = get_variant(i);

    if (account_variant == i) {
        return;
    }

    printf("Введите сумму(для отмены введите -1): ");

    int sum;
    char s[100]; // строка для считывания введённых данных
    scanf("%s", s); // считываем строку

    while (sscanf(s, "%d", &sum) != 1 || bank->getAccounts()[accounts[account_variant - 1] - 1]->getCash(sum) == 0) {
        if (sum == -1) {
            return;
        }
        printf("Неверный ввод, повторите: "); // выводим сообщение об ошибке
        scanf("%s", s); // считываем строку повторно
    }

    printf("Успешно\n");
}

void add_info(std::vector<UserBankInfo>& userBankInfo) {
    if (isEmptyUserInfo(userBankInfo)) {
        return;
    }
    std::vector<Bank*> banks;
    banks.reserve(userBankInfo.size());
    for(UserBankInfo info: userBankInfo) {
        banks.push_back(info.bank);
    }
    printf("В каком банке дополнить информацию о вас?\n");
    int i = 1;
    for(Bank* bank: banks) {
        printf("%d. %s\n", i, bank->getName().c_str());
        ++i;
    }

    printf("%d. Отмена\n", i);
    printf(">");

    int bank_variant = get_variant(banks.size() + 1);

    if (bank_variant == banks.size() + 1) {
        return;
    }

    Bank* bank = banks[bank_variant - 1];
    uint32_t clientId = userBankInfo[bank_variant - 1].clientId;

    char address[100];
    char passport[100];

    printf("Введите адресс: ");
    scanf("%s", address);
    printf("Введите паспортные данные: ");
    scanf("%s", passport);

    bank->client(clientId).setAddress(address);
    bank->client(clientId).setPassportId(passport);
    bank->checkClient(clientId);
}

int main() {
    std::vector<UserBankInfo> userBankInfo; // информация о текущем пользователе
    Client alice("Alice", "Hamburder");
    Client bob("Bob", "Ross");
    Client carl("Carl", "Wasovsky");

    BankConfig greenConfig;
    greenConfig.depositPercent = 5;

    BankConfig yellowConfig;
    greenConfig.depositPercent = 3;

    Bank greenBank("G-R-E-E-N", "Green Bank, Inc", greenConfig);
    Bank yellowBank("T-I-N-K-O-F-F", "Yellow Bank, Inc", yellowConfig);

    uint32_t aliceAtGreen = greenBank.registerClient(alice);
    uint32_t carlAtGreen = greenBank.registerClient(carl);
    uint32_t aliceAtYellow = yellowBank.registerClient(alice);
    uint32_t bobAtYellow = yellowBank.registerClient(bob);

    greenBank.openDebit(aliceAtGreen);
    greenBank.openDeposit(carlAtGreen);
    yellowBank.openDebit(aliceAtYellow);

    std::vector<Bank*> banks;
    banks.push_back(&greenBank);
    banks.push_back(&yellowBank);

    int variant;
    do {
        print_menu(); // выводим меню на экран

        variant = get_variant(7); // получаем номер выбранного пункта меню

        switch (variant) {
            case 1:
                add_client(banks, userBankInfo);
                break;

            case 2:
                add_to_account(userBankInfo);
                break;

            case 3:
                get_cash(userBankInfo);
                break;
            case 4:
                printf("Trans");
                break;
            case 5:
                add_info(userBankInfo);
                break;
            case 6:
                print_banks(banks);
                break;
            case 7:
                showUserInfo(userBankInfo);
                break;
        }

    } while (variant != 8);

    return 0;
}