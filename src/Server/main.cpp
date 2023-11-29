/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
			
using boost::asio::ip::tcp;
				
int main()
{
	// Création du service principal et du résolveur.
	boost::asio::io_service ios;
				
	// Création de l'acceptor avec le port d'écoute 7171 et une adresse quelconque de type IPv4 // (1)
	tcp::acceptor acceptor(ios, tcp::endpoint(tcp::v4(), 7171));
				
	std::string msg ("Bienvenue sur le serveur !"); // (2)
	// On attend la venue d'un client
	while (1)
	{
		// Création d'une socket
		tcp::socket socket(ios); // (3)
				
		// On accepte la connexion
		acceptor.accept(socket); // (4)
		std::cout << "Client reçu ! " << std::endl;
			
		// On envoi un message de bienvenue
		socket.send(boost::asio::buffer(msg)); // (5)
	}
				
	return 0;
}
