#include "Account.h"
#include "Bank.h"
#include "Client.h"
#include "Transaction.h"
#include <algorithm>
#include <iostream>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <string>

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
}

int get_variant(int count) {
    int variant;
    char s[100]; // строка для считывания введённых данных
    printf("> ");
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

    for (Bank* bank: banks) {
        printf("| %19s | %19s | %12d  | %12d  |\n", bank->getName().c_str(), bank->getSwiftCode().c_str(),
               bank->getConfig().depositPercent, bank->getConfig().commission);
    }

    if (banks.empty()) {
        printf("|            Нет банков...          |\n");
    }

    printf("+---------------------+---------------------+---------------+---------------+\n");
}

void add_account(int account_variant, Bank* bank, uint32_t client_id) {
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

Bank* choose_bank(const std::vector<Bank*> banks, const char* title = "Выберите банк") {
    printf("%s\n", title);
    int i = 1;
    for (Bank* bank: banks) {
        printf("%d. %s\n", i, bank->getName().c_str());
        ++i;
    }
    printf("%d. Отмена\n", i);
    printf(">");

    int bank_variant = get_variant(i);
    if (bank_variant == i) {
        return nullptr;
    }

    return banks[bank_variant - 1];
}

void add_client(std::vector<Bank*> banks, std::vector<UserBankInfo>& userBankInfo) {
    Bank* bank = choose_bank(banks, "В каком банке хотите завести счет?");
    if (!bank) {
        return;
    }

    printf("Какой счет вы хотите открыть?\n");
    printf("1. Дебетовый\n");
    printf("2. Депозит\n");
    printf("3. Кредитный\n");
    printf("4. Отмена\n");
    int account_variant = get_variant(4);
    if (account_variant == 4) {
        return;
    }

    for (UserBankInfo info: userBankInfo) {
        if (info.bank->getSwiftCode() == bank->getSwiftCode()) {
            Client client = bank->client(info.clientId);
            printf("Уже авторизованы, %s %s\n", client.firstName().c_str(), client.lastName().c_str());
            add_account(account_variant, bank, info.clientId);
            if (!client.isSus()) {
                bank->recheckClient(info.clientId);
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

    uint32_t client_id = bank->registerClient({name, surname});
    add_account(account_variant, bank, client_id);

    UserBankInfo userInfo;
    userInfo.clientId = client_id;
    userInfo.bank = bank;
    userBankInfo.push_back(userInfo);

    printf("Хотите дополнить свою информацию для увеличения лимита?\n");
    printf("1. Нет\n");
    printf("2. Да\n");
    int more_info_variant = get_variant(2);
    if (more_info_variant == 1) {
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
    bank->recheckClient(client_id);
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
    for (UserBankInfo info: userBankInfo) {
        printf("Счета в банке \"%s\":\n", info.bank->getName().c_str());
        printf("  id \t Sum \t Usage \t Type\n");
        for (const auto& account: info.bank->getAccounts()) {
            if (account->getClientId() == info.clientId) {
                printf("  %d \t %ld$ \t %ld$ \t %s\n", account->getId(), account->getBalance(),
                       account->getCurrentUsage(), account->getType().c_str());
            }
        }
    }
}

Account* choose_account(Bank& bank, uint32_t clientId = 0, const char* title = "Выберите счет") {
    auto& accounts = bank.getAccounts();
    printf("%s\n", title);

    int i = 1;
    std::vector<Account*> mapVariantToAccount;

    for (auto& acc: accounts) {
        if (clientId == 0 || clientId == acc->getClientId()) {
            auto name = bank.client(acc->getClientId());
            printf("%d. %s принадлежит %s %s с $%ld\n", i, acc->getType().c_str(), name.firstName().c_str(),
                   name.lastName().c_str(), acc->getBalance());
            mapVariantToAccount.push_back(acc.get());
            ++i;
        }
    }
    printf("%d. Отмена\n", i);

    int variant = get_variant(i);
    if (variant == i) {
        return nullptr;
    }

    return mapVariantToAccount[variant - 1];
}

uint32_t find_client_id_in_user_bank_infos(const std::vector<UserBankInfo>& infos, Bank* bank) {
    return std::find_if(infos.begin(), infos.end(), [bank](const UserBankInfo& info) { return info.bank == bank; })
            ->clientId;
}

void top_up_account(std::vector<UserBankInfo>& userBankInfo) {
    if (isEmptyUserInfo(userBankInfo)) {
        return;
    }

    std::vector<Bank*> banks;
    banks.reserve(userBankInfo.size());
    for (UserBankInfo info: userBankInfo) {
        banks.push_back(info.bank);
    }

    Bank* bank = choose_bank(banks, "В каком банке хотите пополнить счет?");
    if (!bank) {
        return;
    }

    uint32_t clientId = find_client_id_in_user_bank_infos(userBankInfo, bank);

    Account* account = choose_account(*bank, clientId);
    if (!account) {
        return;
    }

    printf("Введите сумму (для отмены введите -1): ");

    int sum;
    char s[100]; // строка для считывания введённых данных
    scanf("%s", s); // считываем строку

    while (sscanf(s, "%d", &sum) != 1 || !account->topUpAccount(sum)) {
        if (sum == -1) {
            return;
        }
        printf("Неверный ввод, повторите: "); // выводим сообщение об ошибке
        scanf("%s", s); // считываем строку повторно
    }

    printf("Успешно\n");
}

void get_cash(std::vector<UserBankInfo>& userBankInfo) {
    if (isEmptyUserInfo(userBankInfo)) {
        return;
    }

    std::vector<Bank*> banks;
    banks.reserve(userBankInfo.size());
    for (UserBankInfo info: userBankInfo) {
        banks.push_back(info.bank);
    }

    Bank* bank = choose_bank(banks, "В каком банке хотите снять деньги?");
    if (!bank) {
        return;
    }

    uint32_t clientId = find_client_id_in_user_bank_infos(userBankInfo, bank);
    Account* account = choose_account(*bank, clientId);
    if (!account) {
        return;
    }

    printf("Введите сумму (для отмены введите -1): ");

    int sum;
    char s[100]; // строка для считывания введённых данных
    scanf("%s", s); // считываем строку

    while (sscanf(s, "%d", &sum) != 1 || account->getCash(sum) == 0) {
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
    for (UserBankInfo info: userBankInfo) {
        banks.push_back(info.bank);
    }

    Bank* bank = choose_bank(banks, "В каком банке дополнить информацию о вас?");
    if (!bank) {
        return;
    }

    uint32_t clientId = find_client_id_in_user_bank_infos(userBankInfo, bank);

    char address[100];
    char passport[100];

    printf("Введите адрес: ");
    scanf("%s", address);
    printf("Введите паспортные данные: ");
    scanf("%s", passport);

    bank->client(clientId).setAddress(address);
    bank->client(clientId).setPassportId(passport);
    bank->recheckClient(clientId);
}

void do_transaction(const std::vector<Bank*>& banks) {
    Bank* sendBank = choose_bank(banks, "Из какого банка отправить деньги?");
    if (!sendBank) {
        return;
    }

    Account* sendAcc = choose_account(*sendBank);
    if (!sendAcc) {
        return;
    }

    Bank* receiveBank = choose_bank(banks, "В какой банк отправить деньги?");
    if (!receiveBank) {
        return;
    }

    Account* receiveAcc = choose_account(*receiveBank);
    if (!receiveAcc) {
        return;
    }

    uint32_t amount;
    printf("Сумма транзакции: ");
    scanf("%uld", &amount);

    Transaction tr{.amount = amount,
                   .senderBankSwift = sendBank->getSwiftCode(),
                   .senderAccountId = sendAcc->getId(),
                   .receiverBankSwift = receiveBank->getSwiftCode(),
                   .receiverAccountId = receiveAcc->getId()};
    auto error = sendBank->addTransaction(*receiveBank, tr);

    if (error.has_value()) {
        printf("%s\n", error.value().c_str());
        return;
    }

    printf("Транзакция %s успешно записана\n", tr.uuid.c_str());
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

    greenBank.openDeposit(aliceAtGreen);
    greenBank.openDebit(carlAtGreen);
    yellowBank.openDebit(aliceAtYellow);

    std::vector<Bank*> banks;
    banks.push_back(&greenBank);
    banks.push_back(&yellowBank);

    userBankInfo.push_back(UserBankInfo{.bank = &greenBank, .clientId = carlAtGreen});
    greenBank.findAccountById(carlAtGreen)->topUpAccount(100);
    printf("У вас есть дебетовый счет в Green Bank на имя Carl с $100\n");

    int variant;
    do {
        print_menu(); // выводим меню на экран

        variant = get_variant(8); // получаем номер выбранного пункта меню

        switch (variant) {
            case 1:
                add_client(banks, userBankInfo);
                break;

            case 2:
                top_up_account(userBankInfo);
                break;

            case 3:
                get_cash(userBankInfo);
                break;

            case 4:
                do_transaction(banks);
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