// ssh_manager.h
#ifndef SSH_MANAGER_H
#define SSH_MANAGER_H

#include <string>

// Palette di colori Modern Cyberpunk per Lubuntu Swag Style
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define MINT "\033[38;5;121m"
#define FLAMINGO "\033[38;5;212m"
#define SALMON "\033[38;5;209m"
#define RED_PASTEL "\033[38;5;203m"
#define CYAN_PASTEL "\033[38;5;117m"
#define BG_CYBER "\033[1;37;48;5;239m"

extern bool is_premium;

void connettiSSH();
void salvaHost();
void connettiHostSalvato();
void attivaLicenza();
void controllaLicenzaAllAvvio();
bool controlla_licenza_online(const std::string& chiave);
void apriSitoAcquisto();

#endif