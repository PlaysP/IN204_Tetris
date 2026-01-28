# IN204_Tetris Paul Plays Rayan Ramadan

Projet tetris avec une fonction de multijoueur en ligne pour le cours IN204

<img src="assets/2players.png" alt="example" width="400"/>

## Pré-requis (Windows)
Vous aurez besoin de:
- **MinGW-w64** ou **w64devkit** - Compilateur g++ supportant C++17
- **Raylib** - Librairie graphique (https://github.com/raysan5/raylib)
- **ENet** - Librairie réseau pour le multijoueur (https://github.com/lsalzman/enet)

## Guide d'installation (Windows)

### 1. Installer w64devkit (Compilateur C++)
1. Téléchargez w64devkit : https://github.com/skeeto/w64devkit/releases
2. Extrayez l'archive dans `C:\raylib\w64devkit\`
3. Ajoutez `C:\raylib\w64devkit\bin` au PATH système (Paramètres → Système → Variables d'environnement)

### 2. Installer Raylib
**Option A - Version précompilée (recommandé) :**
1. Téléchargez raylib pour Windows : https://github.com/raysan5/raylib/releases
2. Extrayez dans `C:\raylib\raylib\`

**Option B - Compilation depuis les sources :**
```powershell
git clone https://github.com/raysan5/raylib.git C:\raylib\raylib
cd C:\raylib\raylib
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

### 3. Installer ENet

**Étapes d'installation :**
1. Téléchargez ENet depuis : https://github.com/lsalzman/enet
2. Extrayez dans un dossier temporaire ou clonez :
   ```powershell
   git clone https://github.com/lsalzman/enet.git
   cd enet
   ```

3. Compilez ENet avec CMake :
   ```powershell
   mkdir build
   cd build
   cmake .. -G "MinGW Makefiles"
   mingw32-make
   ```

4. Copiez les fichiers compilés :
   - Copiez `libenet.a` dans `C:\raylib\w64devkit\x86_64-w64-mingw32\lib\`
   - Copiez le dossier `include/enet` dans `C:\raylib\w64devkit\x86_64-w64-mingw32\include\`

**Vérification :** Les fichiers suivants doivent exister :
- `C:\raylib\w64devkit\x86_64-w64-mingw32\lib\libenet.a`
- `C:\raylib\w64devkit\x86_64-w64-mingw32\include\enet\enet.h`

**Note :** Si vous avez installé les librairies dans d'autres emplacements, modifiez les chemins dans le `Makefile`.

## Compilation

Pour compiler le projet :
```powershell
make
```

Pour nettoyer les fichiers objets :
```powershell
make clean
```

Une fois le fichier `main.exe` créé, double-cliquez dessus ou exécutez-le depuis PowerShell pour lancer le jeu.

## Configuration réseau (Multijoueur)

Pour le mode multijoueur :
- **Port utilisé :** 1233 (assurez-vous qu'il est ouvert dans le pare-feu Windows)
- **Obtenir votre IP :** Tapez `ipconfig` dans PowerShell et notez votre "Adresse IPv4"

## Guide d'utilisation 

Le jeu propose deux modes, un mode à 1 joueur et un mode à 2 joueur en ligne.

Pour jouer en solo, il suffit d'appuyer sur la touche 1 du clavier et la partie se lance.

<img src="assets/homescreen.png" alt="homescreen" width="300"/>
<img src="assets/1player.png" alt="1player" width="307"/>

### Mode Multijoueur

Pour jouer en mode multijoueur il faut suivre plusieurs étape :

- Le premier joueur lance le jeu et appuie sur la touche 2 afin d'accéder au mode multijoueur
- Un deuxième menu s'ouvre, il doit alors appuyer sur la touche C, c'est lui qui jouera le rôle du serveur

<img src="assets/multiscreen.png" alt="multiscreen" width="320"/>

- Le premier joueur est alors en attente du deuxième

<img src="assets/waitingscreen.png" alt="waitingscreen" width="320"/>

- Le deuxième joueur lance le jeu et appuie sur la touche 2
- Il appuie ensuite sur J pour rejoindre une partie, l'adresse IP du premier joueur lui sera alors demandé

<img src="assets/server_ip.png" alt="server_ip" width="320"/>

- Une fois l'adresse IP renseigné, le joueur 2 rejoins la partie et la partie se lance

<img src="assets/2players.png" alt="2players" width="320"/>