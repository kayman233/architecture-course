#include "Account.h"
#include "Bank.h"
#include "Client.h"
#include <iostream>
#include <set>
#include <string>
#include <stdio.h>
#include <stdlib.h>

void print_menu() {
    printf("Что вы хотите сделать?\n");
    printf("1. Завести счет\n");
    printf("2. Перевести деньги\n");
    printf("3. Снять деньги\n");
    printf("4. Дополнить свои данные\n");
    printf("5. Показать банки\n");
    printf("6. Выйти\n");
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
    printf("+---------------------+---------------------+\n");
    printf("|         Банк        |       Swift код     |\n");
    printf("+---------------------+---------------------+\n");

    for(Bank* bank: banks) {
        printf("| %19s | %19s |\n", bank->getName().c_str(), bank->getSwiftCode().c_str());
    }

    if (banks.empty())
        printf("|            Нет банков...          |\n");

    printf("+---------------------+-------------+-------+\n");
}

void add_client(std::vector<Bank*> banks) {
    printf("В каком банке хотите завести счет?\n");
    int i = 1;
    for(Bank* bank: banks) {
        printf("%d. %s\n", i, bank->getName().c_str());
        ++i;
    }

    printf("%d. Отмена\n", i);

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

    int account_variant = get_variant(banks.size() + 1);

    if (account_variant == 4) {
        return;
    }

    char name[100];
    char surname[100];

    printf("Введите имя: ");
    scanf("%s", name);
    printf("Введите фамилию: ");
    scanf("%s", surname);

    Client client(name, surname);

    uint32_t client_id = bank->registerClient(client);

    uint32_t account_id = 0;
    switch (account_variant) {
        case 1:
            account_id = bank->openDebit(client_id);
            break;

        case 2:
            account_id = bank->openDeposit(client_id);
            break;

        case 3:
            account_id = bank->openDebit(client_id);
            break;
        default:
            break;
    }

    printf("Успешно! %d\n", account_id);
}

int main() {
    Client alice("Alice", "Hamburder");
    Client bob("Bob", "Ross");
    Client carl("Carl", "Wasovsky");

    Bank greenBank("G-R-E-E-N", "Green Bank, Inc", {});
    Bank yellowBank("T-I-N-K-O-F-F", "Yellow Bank, Inc", {});

    uint32_t aliceAtGreen = greenBank.registerClient(alice);
    uint32_t carlAtGreen = greenBank.registerClient(carl);
    uint32_t aliceAtYellow = yellowBank.registerClient(alice);
    uint32_t bobAtYellow = yellowBank.registerClient(bob);

    greenBank.openDebit(aliceAtGreen);

    std::vector<Bank*> banks;
    banks.push_back(&greenBank);
    banks.push_back(&yellowBank);

    int variant;
    do {
        print_menu(); // выводим меню на экран

        variant = get_variant(6); // получаем номер выбранного пункта меню

        switch (variant) {
            case 1:
                add_client(banks);
                break;

            case 2:
                printf("variant2\n");
                break;

            case 3:
                printf("variant3\n");
                break;

            case 4:
                printf("variant4\n");
                break;
            case 5:
                print_banks(banks);
                break;
        }

    } while (variant != 6);

    return 0;
}