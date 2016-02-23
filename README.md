#	Documentation développeur
420-6GD-HY - Entretien de logiciels d’applications

#	Description
	HellRain est un clone de minecraft fait par une équipe de 8 programmeurs.
	Le jeu a été codé en c++, on était supposé faire un jeu d'horreur,
	mais on a décidé de faire un jeu qui était plus sur le combat contre monstre,
	il n'y a pas vraiment d'histoire pour ce jeu.
#	Collaborateur
	Mikael Andries-Gounant
	Vincent Bédard
	Maxime Noël
	Michaël Veilleux
	Christophe Martineau
	Gabriel Cholette
	Dominic Bouchard
	Stéphane Letarte
#	Les systèmes d'exploitation supportés/testés
	-Windows10(testé) 
	-Windows7(testé)
	-Windows8 (non testé)
	-Linux Mint 17.3 Rosa(Testé)
	-Arch Linux 4.4.1(Testé)

#	Installation sur Linux

#	Installation sur Windows
Faire un gitclone avec cette URL: https://github.com/CegepSTH/HellRain.git
Ouvrir le fichie Cube.sln situé à cette emplacement : \HellRain\Cube
Compiler en mode release.
#	Renseignement sur le fonctionement
	La plupart des touches sont enregistrés dans un fichier de config, donc pour les modifier il faut ouvrir le menu dans le jeu.
	Sinon, les touches par défaut sont:
		Supprimer un bloc: Clic gauche
		Placer un bloc: Clic droit
		Déplacement: WASD
		Changer de bloc en mode créatif: La roulette de la souris
		S'accroupir: Ctrl gauche
		Courir: Shift gauche
		Sauter: Espace
		Noclip: v
		Wireframe: y
		Informations: F3
		Plein écran: F10
		Ouvrir une map: Shift Droit + O
		Sauvegarder: Shift Droit + W
		Refaire une map: Shift Droit + R
		Faire une map plate: Shift Droit + F
		Supprimer le cache: Shift Droit + F5
		Ouvrir le menu: Échap
		Naviguer dans le menu: les flèches de directions
		Inventaire de bloc: 1
		Pistol: 2
		Mp5: 3
		Shotgun: 4
		Ak: 5
		Awp : 6
		Ouvrir l'inventaire: I
		Inventaire bloc 1: Z
		Inventaire bloc 2: X
		Inventaire bloc 3: C
		Choisir un bloc dans l'inventaire: B
		Enlever un bloc de l'inventaire: E
		
#	Qu'est-ce qui serait a ameliorer ?
	-Changer le pathing des monstres. En ce moment, ils se promènent aléatoirement et ils sautent si il y a collision.
	-Faire que l'on puisse clicker sur le menu pour selectionné les options.
	-Faire que l'on puisse clicker sur l'inventaire pour bouger/placer/enlever les blocques.
	-Faire qu'il y a plusieurs map et que l'on puisse choirsir laquel nous voulons.
	-Faire que les clients download la map du serveur.
	-Synchroniser les monstres entre le serveur et client.
	-Faire que les blocs on une durabilité.
	-Ajouter des armures.
	-Optimisation de la sauvegarde de la map.
	-Fall damage pour les monstres.
	-Optimiser la façon que les monstres spawn.
