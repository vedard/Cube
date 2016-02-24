#	Documentation développeur
420-6GD-HY - Entretien de logiciels d’applications

#	Description
HellRain est un clone de minecraft fait par une équipe de 8 programmeurs.
Le jeu a été codé en c++, nous étions supposé faire un jeu d'horreur,
mais nous avons décidé de faire un jeu qui était plus oriente sur le combat contre monstre,
il n'y a pas vraiment d'histoire pour ce jeu.

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
-	Windows10 (testé) 
-	Windows7 (testé)
-	Windows8 (non testé)
-	Linux Mint 17.3 Rosa(Testé)
-	Arch Linux 4.4.1(Testé)

#	Installation sur Linux

#	Installation sur Windows
1.	Faire un gitclone avec cet URL: https://github.com/CegepSTH/HellRain.git
2.	Ouvrir le fichier Cube.sln 
	situé à cet emplacement:\HellRain\Cube
3.	Compiler en mode release.

#	Renseignements sur le fonctionement
La plupart des touches sont enregistrées dans un fichier de config, donc pour les modifier il faut ouvrir le menu dans le jeu.
Sinon, les touches par défaut sont:

	Supprimer un bloc: Clic gauche
	Placer un bloc: Clic droit
	Déplacement: WASD
	Changer de bloc en mode créatif: La roulette de la souris
	S'accroupir: Ctrl gauche
	Courrir: Shift gauche
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
	Valider une option du menu: Entrée
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
	
##	Multijoueur
Pour faire fonctionner le mode multijoueur, il faut tout d'abord changer son nom dans le fichier de configuration.
Le fichier de configuration se situe à cet emplacement: \HellRain\Cube\Cube\Cube.conf
il faut changer la ligne où il est écrit "playername" pour "playername TonNom".
Ensuite, il faut ouvrir le jeu, ouvrir le menu, 
naviguer vers l'option Multiplayer, entrer l'ip du serveur où l'on veut se connecter, dans notre cas, 
l'ip est 45.55.42.126.

#	Qu'est-ce qui serait a améliorer ?
-	Changer le pathing des monstres. En ce moment, ils vont vers le joueur et ils sautent s'il y a collision.
-	Faire que l'on puisse cliquer sur le menu pour selectionner les options.
-	Faire que l'on puisse cliquer sur l'inventaire pour bouger/placer/enlever les blocs.
-	Faire qu'il y a plusieurs maps et que l'on puisse choirsir laquelle nous voulons.
-	Synchroniser les monstres entre le serveur et client.
-	Faire que les blocs ont une durabilité.
-	Ajouter des armures.
-	Optimisation de la sauvegarde de la map.
-	Fall damage pour les monstres.
-O	ptimiser la façon que les monstres spawnent.
