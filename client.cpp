/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** client
*/

#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

int main() {
    // Création du service principal et du résolveur.
	boost::asio::io_service ios;
			
	// On veut se connecter sur la machine locale, port 7171
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 7171);
				
	// On crée une socket // (1)
	boost::asio::ip::tcp::socket socket(ios);
				
	// Tentative de connexion, bloquante // (2)
	socket.connect(endpoint);
		
	// Création du buffer de réception // (3)
	boost::array<char, 128> buf;
	while (1)
	{
		boost::system::error_code error;
		// Réception des données, len = nombre d'octets reçus // (4)
		int len = socket.read_some(boost::asio::buffer(buf), error);
				
		if (error == boost::asio::error::eof) // (5)
		{
			std::cout << "\nTerminé !" << std::endl;
			break;
		}
				
		// On affiche (6)
		std::cout.write(buf.data(), len); 
	}
					
	return 0;
}