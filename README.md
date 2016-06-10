#	Description
HellRain est un clone de minecraft fait par une équipe de 8 programmeurs. Le
jeu a été codé en C++ et fonctionne sur Windows et Linux. Le jeu a été
développé dans le cadre de notre cours d'Entretien de logiciels d’application
au Cegep de Saint-Hyacinthe.

#	Capture d'écran

![Screenshot](http://i.imgur.com/RLC02Hx.png)

#	Collaborateurs
-	Mikael Andries-Gounant
-	Vincent Bédard
-	Maxime Noël
-	Michaël Veilleux
-	Christophe Martineau
-	Gabriel Cholette
-	Dominic Bouchard
-	Stéphane Letarte

#	Les systèmes d'exploitation supportés/testés
-	Windows 10
-	Windows 7
-	Linux Mint 17.3 Rosa
-	Arch Linux 4.4.1

La version serveur du jeu peut facilement fonctionner sur une machine virtuelle
sans environnement graphique (exemple Digital Oceans). Il suffit alors de compiler avec le flag
"-DNODISPLAY" (Ajouter à la ligne 3 du makefile)

La version originale a besoin au minimun d'une carte graphique capable de
supporter GLSL 3.30.

Le jeu a été testé avec la carte NVIDIA NVS 5400m et un processeur Intel i5-3320M @2.60

#	Les librairies utilisées
-	Enet
-	SFML 2
-	Glew
-	Glut
-	DevIL

#	Installation sur Linux
Il faut d'abord installer le nécessaire.

Sur Debian, Ubuntu, Mint, etc:

	# apt install gcc make libsfml-dev libdevil-dev libglew-dev libenet-dev freeglut3-dev

Sur Arch Linux:

	# pacman -S gcc make sfml devil glew freeglut enet

Ensuite pour compiler:

	$ git clone https://github.com/cegepsth/hellrain.git
	$ cd Hellrain/Cube/Cube
	$ make
	$ ./Cube

#	Installation sur Windows
Rien n'est à installer sauf Visual Studio

1.	Faire un gitclone avec cet URL: https://github.com/CegepSTH/HellRain.git
2.	Ouvrir le fichier Cube.sln avec Visual Studio situé à cet emplacement:\HellRain\Cube
3.	Compiler en mode release pour de meilleurs performances

#	Renseignements sur le fonctionnement
La plupart des options sont enregistrées dans un fichier de configuration
"Hellrain/Cube/Cube/Cube.conf". Il est possible de changer d'autres éléments du
jeu avant la compilation en changeant les directives dans le "define.h"
(Attention des résultats étranges peuvent se produire, Utilisation à vos propres
risques)

##  Voici les touches par défaut


|Fonction							   |Touche
|--------------------------------------|:--------------------------:
|Supprimer un bloc			 		   |Clic gauche
|Placer un bloc			 			   |Clic droit
|Déplacement			 			   |WASD
|Changer de bloc en mode créatif	   |La roulette de la souris
|S'accroupir			 			   |Ctrl gauche
|Courrir			 				   |Shift gauche
|Sauter			 					   |Espace
|Noclip			 					   |v
|Wireframe			 				   |y
|Informations		 				   |F3
|Plein écran			 			   |F10
|Ouvrir une map		 				   |Shift Droit + O
|Sauvegarder			 			   |Shift Droit + W
|Refaire une map		 			   |Shift Droit + R
|Faire une map plate	 			   |Shift Droit + F
|Supprimer le cache	 				   |Shift Droit + F5
|Ouvrir le menu		 				   |Échap
|Naviguer dans le menu				   |les flèches de directions
|Valider une option du menu			   |Entrée
|Inventaire de bloc			 		   |1
|Pistol			 					   |2
|Mp5			 					   |3
|Shotgun			 				   |4
|Ak			 						   |5
|Awp 			 					   |6
|Ouvrir l'inventaire			 	   |I
|Inventaire bloc 1			 		   |Z
|Inventaire bloc 2			 		   |X
|Inventaire bloc 3			 		   |C
|Choisir un bloc dans l'inventaire	   |B
|Enlever un bloc de l'inventaire	   |E



##	Multijoueur
Pour démarrer le serveur il faut aller dans le fichier de configuration et mettre
la propriété "server" à "true". Ensuite, il suffit de démarrer Cube.exe

Pour qu'un client se connecte au serveur il faut qu'il s'assure d'avoir un nom
unique (modifiable dans le fichier de configuration), car le serveur affiche
seulement les joueurs ayant des noms différents. Ensuite, il suffit de démarrer
le jeu normalement, aller dans le menu, aller dans multijoueur et entrer
l'address du serveur (127.0.0.1 pour un serveur sur le même ordinateur).

Une fois connecté le jeu prend un certain moment à télécharger la map il faut donc
être un peu patient ;)

#	Qu'est-ce qui serait à améliorer ?
-	Changer le IA des monstres. En ce moment, ils vont vers le joueur et ils sautent s'il y a collision.
-	Faire que l'on puisse cliquer sur le menu pour sélectionner les options.
-	Faire que l'on puisse cliquer sur l'inventaire pour bouger/placer/enlever les blocs.
-	Faire qu'il y a plusieurs maps et que l'on puisse choisir laquelle nous voulons.
-	Synchroniser les monstres entre le serveur et client.
-	Faire que les blocs ont une durabilité.
-	Ajouter des armures.
-	Optimisation de la sauvegarde de la map.
-	Fall damage pour les monstres.
-	Optimiser la façon que les monstres apparaissent.
-	Compression des paquets
