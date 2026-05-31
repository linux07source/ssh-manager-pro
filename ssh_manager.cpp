// ssh_manager.cpp
/*#include "ssh_manager.h"

#include <curl/curl.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

bool is_premium = false;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp)
{
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string getPercorsoFile()
{
    const char* homeDir = std::getenv("HOME");
    if (!homeDir) return "host_salvati.txt";

    std::string cartellaConfig = std::string(homeDir) + "/.config/ssh-manager";
    std::system(("mkdir -p " + cartellaConfig).c_str());

    return cartellaConfig + "/host_salvati.txt";
}

std::string getPercorsoLicenza()
{
    const char* homeDir = std::getenv("HOME");
    if (!homeDir) return ".license";
    return std::string(homeDir) + "/.config/ssh-manager/.license";
}

// Bussa alle API di Keygen.sh per validare la chiave
bool controlla_licenza_online(const std::string& chiave)
{
    CURL* curl = curl_easy_init();
    std::string readBuffer;
    bool valida = false;

    if (curl)
    {
        // URL delle API di verifica di Gumroad
        std::string url = "https://api.gumroad.com/v2/licenses/verify";

        // Inserisci qui il permalink del tuo prodotto Gumroad (es. "sshpro")
        std::string product_permalink = "IL_TUO_PERMALINK_QUI";

        // I dati da inviare a Gumroad
        std::string dati_post = "product_permalink=" + product_permalink + "&license_key=" + chiave;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dati_post.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "LubuntuSwagStyleManager/1.0");

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK)
        {
            // Gumroad risponde con "success":true se la licenza è valida
            if (readBuffer.find("\"success\":true") != std::string::npos ||
                readBuffer.find("\"success\": true") != std::string::npos)
            {
                valida = true;
            }
            else
            {
                // Debug per vedere cosa dice Gumroad in caso di errore
                std::cout << "\n [Debug Gumroad]: " << readBuffer << "\n";
            }
        }
    }
    return valida;
}

void controllaLicenzaAllAvvio()
{
    std::ifstream file(getPercorsoLicenza());
    if (file.is_open())
    {
        std::string chiave_salvata;
        file >> chiave_salvata;
        file.close();
        if (controlla_licenza_online(chiave_salvata))
        {
            is_premium = true;
        }
    }
}

void attivaLicenza()
{
    std::string chiave;
    std::cout << "\n"
              << FLAMINGO << "┌─── [ ATTIVAZIONE CHIAVE LICENZA KEYGEN ] ──────────┐" << RESET << "\n";
    std::cout << " Incolla la tua chiave (es. 2B211A-...): ";
    std::cin >> chiave;

    std::cout << "\n Connessione ai server di Keygen.sh in corso...";
    if (controlla_licenza_online(chiave))
    {
        is_premium = true;
        std::ofstream file(getPercorsoLicenza());
        if (file.is_open())
        {
            file << chiave;
            file.close();
        }
        std::cout << "\n " << MINT << "[✓] Licenza verificata con successo! Funzioni Premium sbloccate." << RESET << "\n";
    }
    else
    {
        std::cout << "\n " << RED_PASTEL << "[X] Chiave rifiutata o non valida per questo prodotto." << RESET << "\n";
    }
}

void connettiSSH()
{
    std::string user, ip;
    std::cout << "\n--- Connessione SSH Manovrata ---\n";
    std::cout << "Inserisci il nome utente: ";
    std::cin >> user;
    std::cout << "Inserisci l'indirizzo IP o l'Host: ";
    std::cin >> ip;
    std::string comando = "ssh " + user + "@" + ip;
    std::system(comando.c_str());
}

void salvaHost()
{
    std::string user, ip, alias;
    std::cout << "\n--- Salva Nuovo Host ---\n";
    std::cout << "Inserisci un nome mnemonico: ";
    std::cin >> alias;
    std::cout << "Inserisci il nome utente: ";
    std::cin >> user;
    std::cout << "Inserisci l'IP: ";
    std::cin >> ip;

    std::string percorsoFile = getPercorsoFile();
    std::ofstream file(percorsoFile, std::ios::app);
    if (file.is_open())
    {
        file << alias << " " << user << " " << ip << "\n";
        file.close();
        std::cout << "Dispositivo memorizzato in: " << percorsoFile << "\n";
    }
}

void connettiHostSalvato()
{
    std::string percorsoFile = getPercorsoFile();
    std::ifstream file(percorsoFile);
    if (!file.is_open()) return;

    std::string alias, user, ip, aliasScelto;
    std::cout << "\n--- Host Salvati nel System ---\n";
    while (file >> alias >> user >> ip)
    {
        std::cout << "- " << alias << " (" << user << "@" << ip << ")\n";
    }
    file.close();

    std::cout << "\nScegli l'alias della macchina: ";
    std::cin >> aliasScelto;
    file.open(percorsoFile);
    while (file >> alias >> user >> ip)
    {
        if (alias == aliasScelto)
        {
            int opz = 1;
            std::cout << "\n1) Connessione SSH Standard\n2) Copia chiave pubblica (ssh-copy-id)\nScelta: ";
            std::cin >> opz;
            if (opz == 2)
            {
                std::system(("ssh-copy-id " + user + "@" + ip).c_str());
            }
            else
            {
                std::system(("ssh " + user + "@" + ip).c_str());
            }
            break;
        }
    }
    file.close();
}

void apriSitoAcquisto()
{
    std::cout << "\n Reindirizzamento alla paggina di acquisto in cosrso...\n";
    std::string url = "https://1357856495547.gumroad.com/l/panegu";
    std::string comando = "xdg-open " + url + " > /dev/null 2>&1 &";
    std::system(comando.c_str());
}*/

// ssh_manager.cpp
#include "ssh_manager.h"

#include <curl/curl.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

bool is_premium = false;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp)
{
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string getPercorsoFile()
{
    const char* homeDir = std::getenv("HOME");
    if (!homeDir) return "host_salvati.txt";

    std::string cartellaConfig = std::string(homeDir) + "/.config/ssh-manager";
    std::system(("mkdir -p " + cartellaConfig).c_str());

    return cartellaConfig + "/host_salvati.txt";
}

std::string getPercorsoLicenza()
{
    const char* homeDir = std::getenv("HOME");
    if (!homeDir) return ".license";
    return std::string(homeDir) + "/.config/ssh-manager/.license";
}

// Bussa alle API di Gumroad per validare la chiave
// Bussa alle API di Gumroad per validare la chiave
bool controlla_licenza_online(const std::string& chiave)
{
    CURL* curl = curl_easy_init();
    std::string readBuffer;
    bool valida = false;

    if (curl)
    {
        // URL delle API di verifica di Gumroad
        std::string url = "https://api.gumroad.com/v2/licenses/verify";

        // Il tuo permalink estratto da Gumroad
        std::string product_permalink = "panegu";

        // L'ID univoco che il server di Gumroad ci ha richiesto esplicitamente
        std::string product_id = "wcKDX_5c6imT55zAcdPyGQ==";

        // I dati da inviare a Gumroad (incluso il product_id codificato correttamente per la POST)
        // Usiamo curl_easy_escape per gestire i caratteri speciali come '=' nell'ID del prodotto
        char* output_id = curl_easy_escape(curl, product_id.c_str(), product_id.length());
        std::string product_id_encoded = output_id;
        curl_free(output_id);

        std::string dati_post = "product_permalink=" + product_permalink +
                                "&product_id=" + product_id_encoded +
                                "&license_key=" + chiave;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dati_post.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "LubuntuSwagStyleManager/1.0");

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK)
        {
            // Gumroad risponde con "success":true se la licenza è valida
            if (readBuffer.find("\"success\":true") != std::string::npos ||
                readBuffer.find("\"success\": true") != std::string::npos)
            {
                valida = true;
            }
            else
            {
                // Debug per vedere cosa dice Gumroad in caso di errore
                std::cout << "\n [Debug Gumroad]: " << readBuffer << "\n";
            }
        }
    }
    return valida;
}

void controllaLicenzaAllAvvio()
{
    std::ifstream file(getPercorsoLicenza());
    if (file.is_open())
    {
        std::string chiave_salvata;
        file >> chiave_salvata;
        file.close();
        if (controlla_licenza_online(chiave_salvata))
        {
            is_premium = true;
        }
    }
}

void attivaLicenza()
{
    std::string chiave;
    std::cout << "\n"
              << FLAMINGO << "┌─── [ ATTIVAZIONE CHIAVE LICENZA GUMROAD ] ──────────┐" << RESET << "\n";
    std::cout << " Incolla la tua chiave (es. 42CF5DC3-...): ";
    std::cin >> chiave;

    std::cout << "\n Connessione ai server di Gumroad.com in corso...";
    if (controlla_licenza_online(chiave))
    {
        is_premium = true;
        std::ofstream file(getPercorsoLicenza());
        if (file.is_open())
        {
            file << chiave;
            file.close();
        }
        std::cout << "\n " << MINT << "[✓] Licenza verificata con successo! Funzioni Premium sbloccate." << RESET << "\n";
    }
    else
    {
        std::cout << "\n " << RED_PASTEL << "[X] Chiave rifiutata o non valida per questo prodotto." << RESET << "\n";
    }
}

void connettiSSH()
{
    std::string user, ip;
    std::cout << "\n--- Connessione SSH Manovrata ---\n";
    std::cout << "Inserisci il nome utente: ";
    std::cin >> user;
    std::cout << "Inserisci l'indirizzo IP o l'Host: ";
    std::cin >> ip;
    std::string comando = "ssh " + user + "@" + ip;
    std::system(comando.c_str());
}

void salvaHost()
{
    std::string user, ip, alias;
    std::cout << "\n--- Salva Nuovo Host ---\n";
    std::cout << "Inserisci un nome mnemonico: ";
    std::cin >> alias;
    std::cout << "Inserisci il nome utente: ";
    std::cin >> user;
    std::cout << "Inserisci l'IP: ";
    std::cin >> ip;

    std::string percorsoFile = getPercorsoFile();
    std::ofstream file(percorsoFile, std::ios::app);
    if (file.is_open())
    {
        file << alias << " " << user << " " << ip << "\n";
        file.close();
        std::cout << "Dispositivo memorizzato in: " << percorsoFile << "\n";
    }
}

void connettiHostSalvato()
{
    std::string percorsoFile = getPercorsoFile();
    std::ifstream file(percorsoFile);
    if (!file.is_open()) return;

    std::string alias, user, ip, aliasScelto;
    std::cout << "\n--- Host Salvati nel System ---\n";
    while (file >> alias >> user >> ip)
    {
        std::cout << "- " << alias << " (" << user << "@" << ip << ")\n";
    }
    file.close();

    std::cout << "\nScegli l'alias della macchina: ";
    std::cin >> aliasScelto;
    file.open(percorsoFile);
    while (file >> alias >> user >> ip)
    {
        if (alias == aliasScelto)
        {
            int opz = 1;
            std::cout << "\n1) Connessione SSH Standard\n2) Copia chiave pubblica (ssh-copy-id)\nScelta: ";
            std::cin >> opz;
            if (opz == 2)
            {
                std::system(("ssh-copy-id " + user + "@" + ip).c_str());
            }
            else
            {
                std::system(("ssh " + user + "@" + ip).c_str());
            }
            break;
        }
    }
    file.close();
}

void apriSitoAcquisto()
{
    std::cout << "\n Reindirizzamento alla pagina di acquisto in corso...\n";
    std::string url = "https://1357856495547.gumroad.com/l/panegu";
    std::string comando = "xdg-open " + url + " > /dev/null 2>&1 &";
    std::system(comando.c_str());
}