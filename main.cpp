// main.cpp
#include <cstdlib>
#include <iostream>
#include <limits>

#include "ssh_manager.h"

int main()
{
    controllaLicenzaAllAvvio();
    int scelta = 0;

    do
    {
        std::system("clear");
        std::cout << BG_CYBER << " ╔═════════════════════════════════════════╗ " << RESET << "\n";
        std::cout << BG_CYBER << " ║     SSH MANAGER PRO - SSH PRO           ║ " << RESET << "\n";
        std::cout << BG_CYBER << " ╚═════════════════════════════════════════╝ " << RESET << "\n";

        if (is_premium)
        {
            std::cout << " Licenza: " << MINT << BOLD << "[ LICENZA PREMIUM ATTIVA ]" << RESET << "\n\n";
        }
        else
        {
            std::cout << " Licenza: " << RED_PASTEL << BOLD << "[ MODALITÀ FREE LITE ]" << RESET << " -> Opzione [5] per sbloccare\n\n";
        }

        std::cout << "1. Connessione Manuale\n";
        std::cout << "2. Salva indirizzo " << (is_premium ? "" : "[PREMIUM]") << "\n";
        std::cout << "3. Connessione Rapida & Copy-ID " << (is_premium ? "" : "[PREMIUM]") << "\n";

        if (!is_premium)
        {
            std::cout << "5. ATTIVA LICENZA PREMIUM VIA KEYGEN.SH\n";
            std::cout << "6. ACQUISTA LICENZA PREMIUM (Apri Sito)\n";
        }

        std::cout << "4. Esci\n";
        std::cout << "\nssh-manager@user:~$ ";

        if (!(std::cin >> scelta))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (scelta)
        {
            case 1:
                connettiSSH();
                break;
            case 2:
                if (is_premium)
                    salvaHost();
                else
                    std::cout << "\n"
                              << RED_PASTEL << "[X] Funzione Premium! Sbloccala con l'opzione 5 o 6." << RESET << "\n";
                break;
            case 3:
                if (is_premium)
                    connettiHostSalvato();
                else
                    std::cout << "\n"
                              << RED_PASTEL << "[X] Funzione Premium! Sbloccala con l'opzione 5 o 6." << RESET << "\n";
                break;
            case 5:
                if (!is_premium) attivaLicenza();
                break;
            case 6:
                if (!is_premium) apriSitoAcquisto();
                break;
        }

        if (scelta != 4)
        {
            std::cout << "\nPremere INVIO per ricaricare...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
    } while (scelta != 4);

    return 0;
}