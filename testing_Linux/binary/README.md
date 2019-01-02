PROCÉDURE et INSTRUCTIONS EN COURS D'ÉCRITURE, MERCI DE PATIENTER 

* Actuellement : toutes les instructions sont données sur le wiki [ICI](https://framagit.org/ericb/miniDart/wikis/Cr%C3%A9ation-de-l'environnement-de-compilation-sous-Linux)

La version actuellement en test est la **0.9.2**



N.B. : merci de me contacter directement en cas de problème avec les instructions.


* **INSTALLATION SOUS LINUXMINT (18.3 mini)  INTEL**  (64 bits seulement)


**Cette page ne concerne que les développeurs** Il n'y a pas d'archive pour débutant sous Linux pour l'instant, désolé


* 1. UTILISATION du binaire

Les dépendances principales sont :

* 1.1 OpenGL  3.0 et + fonctionnel sur votre machine

  glew ou glad ou ... n'importe quel chargeur d'OpenGL n'est pas nécessaire, car inclus dans miniDart 
  (il s'agit de gl3w inclus dans Dear ImGui)

* 1.2 ffmpeg 4.0.3

  Il est préférable de compiler ffmpeg soi-même. C'est très facile, et si on a ne distribution de type Debian-like,
  le nommage des bibliothèques (aka libraries en anglais), évite par défaut le clash avec celle livrée avec la distribution.

*  1.3 OpenCV 3.4.0

  Les sources (modifiées) de la versin officielle seront bientôt en ligne, et chacun pourra vérifier les modifications apportées,
  par exemple en regardant les diffs. L'essentiel des modifications apportées se trouve dans le fichier cap_libv4l.cpp fourni
  dans le répertoire contenant les instructions (voir le lien + haut)

  Il est possible que cela fonctionne avec une autre 3.4.x, mais dans ce cas, il faudra ajouter les liens symboliques nécessaires, 
  mais aussi oublier certaines fonctionalités ajoutées. 

  N.B. la version 3.1.0 convient, mais dans ce cas, il faut tout compiler soi-même.

  Le binaire testable avec LinuxMint, ainsi que ses dépendances sont fourniée dans l'archive miniDart_0.9.2_LinuxMint18.3+.tar.gz
  (elle se trouve dans ce répertoire)


* 1.4 SDL2 (2.0.5 mini fortement suggéré). Pour SDL2_image, SDL2_ttf et SDL2_mixer,les versions fournies par votre distribution
  devraient suffire (2.0.5 mini fortement suggéré)


* 1.5  Autres dépendances : normalement satisfaites si vous installez les archive SDL2*


* 1.6 À venir : le binaire pour Ubuntu 18-04 (actuellement en retard, car très instable, sans raison éévidente => sera distribué
  quand le problème sera résolu.


* 2 Instructions d'installation

  une fois téléchargée, décompresser l'archive dans un répertoire dans lequel vous avez le droit d'écrire.
  - l'arborescence tient compte des dépendances, et si vous la modifiez, cela ne fonctionnera pas.
  - pour tester, ouvrir un terminal et se placer dans le répertoire qui contient le binaire miniDart :

  => ensuite, il suffit de taper la ligne de commande :

  **./miniDart_0.9.2_LinuxMint**

* 3 **Bugs et/ou limitations connues à ce jour**

  Vous utilisez ce logiciel à vos risques et périls

  FAITES DES COPIES DE SAUVEGARDE DE VOS VIDÉOS, et n'utilisez que ces copies avec miniDart.

  La plupart des vidéos (image et son) peuvent être lues avec miniDart, cela dépend des bibliothèques intallées sur votre machine.

  La valeur par défaut de 24 images par seconde (visualisation et enregistrement) n'est pas modifiable pour l'instant sous Linux. 

  Il est possible de modifier le container et le codec pour l'enregistrement, mais certains "mélanges" ne fonctionneront pas, et
  pourront même provoquer un crash de l'application.

  Avec le temps, une base de connaissance sur les cas qui fonctionnent bien sera maintenue, mais il n'a pas assez de retours 
  utilisateurs pour en conseiller d'autres pour l'instant.

  Le suivi d'équipe est pour l'instant incomplet (ce sera dans une version payante).

  Les dessins des impacts de buts, zone de faiblesse, zone d'efficaticé seront ajoutés dans une version ultérieure.

  Les notations sont en cours d'intégration (version ulitérieure).

  N.B.: il est préférable que la webcam (C920 fortement suggérée) soit BRANCHÉE AVANT DE LANCER miniDart (sous Linux)


  Le numéro des joueurs ne FONCTIONNE PAS CORRECTEMENT (le même numéro est attribué à tous les joueurs). Cela sera corrigé dans une version à venir.

   **Bugs connus**
   (en cours de mise à jour, en fonction des retours)
   On peut quitter l'application dans certaines conditions, lors de la fermeture d'une vidéo en fin de lecture (résolu, sera dans la prochaine version)


  Merci d'avance pour tout retour et n'oubliez pas de tester l'intégrité de l'archive téléchargée  

  Have Fun ;-)

  Eric Bachard


************************************************************************************************************

                   Somme de contrôle (sha 256)                                   Version

************************************************************************************************************

     e5796b33b8730da3af79a30418d7b2ffe3acb394ffd7aa0a1ea70354739ab77d  miniDart_0.9.2_LinuxMint18.3+.tar.gz

************************************************************************************************************

